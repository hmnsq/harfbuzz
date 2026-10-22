/*
 * Copyright © 2011  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#undef HB_DEBUG_WASM
#define HB_DEBUG_WASM 1

#અન્તર્ભાવ "hb-shaper-impl.hh"

#ifdef HAVE_WASM

/* Compile wasm-micro-runtime with:
 *
 * $ cmake -DWAMR_BUILD_MULTI_MODULE=1 -DWAMR_BUILD_REF_TYPES=1 -DWAMR_BUILD_FAST_JIT=1
 * $ make
 *
 * If you manage to build a wasm shared module successfully and want to use it,
 * do the following:
 *
 *   - Add -DWAMR_BUILD_MULTI_MODULE=1 to your cmake build for wasm-micro-runtime,
 *
 *   - Remove the #define HB_WASM_NO_MODULES line below,
 *
 *   - Install your shared module with name ending in .wasm in
 *     $(prefix)/$(libdir)/harfbuzz/wasm/
 *
 *   - Build your font's wasm code importing the shared modules with the desired
 *     name. This can be done eg.: __attribute__((import_module("graphite2")))
 *     before each symbol in the shared-module's headers.
 *
 *   - Try shaping your font and hope for the best...
 *
 * I haven't been able to get this to work since emcc's support for shared libraries
 * requires support from the host that seems to be missing from wasm-micro-runtime?
 */

#અન્તર્ભાવ "hb-wasm-api.hh"
#અન્તર્ભાવ "hb-wasm-api-list.hh"

#ifndef HB_WASM_NO_MODULES
#define HB_WASM_NO_MODULES
#endif


#ifndef HB_WASM_NO_MODULES
સ્થિર તાર્કિક HB_UNUSED
_hb_wasm_module_reader (અચલ અક્ષર *module_name,
		       uint8_t **p_buffer, uint32_t *p_size)
{
  અક્ષર path[કદ (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (path, module_name, કદ (path) - કદ (HB_WASM_MODULE_DIR) - 16);
  strncat (path, ".wasm", 6);

  auto *blob = hb_blob_create_from_file (path);

  unsigned length;
  auto *data = hb_blob_get_data (blob, &length);

  *p_buffer = (uint8_t *) hb_malloc (length);

  જો (length && !p_buffer)
    પાછા ખોટું;

  memcpy (*p_buffer, data, length);
  *p_size = length;

  hb_blob_destroy (blob);

  પાછા સાચું;
}

સ્થિર શૂન્ય HB_UNUSED
_hb_wasm_module_destroyer (uint8_t *buffer, uint32_t size)
{
  hb_free (buffer);
}
#endif

/*
 * shaper face data
 */

#define HB_WASM_TAG_WASM HB_TAG('W','a','s','m')

સંરચના hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(શૂન્ય, wasm_shape_plan);
};

સંરચના hb_wasm_face_data_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  mutable hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

સ્થિર તાર્કિક
_hb_wasm_init ()
{
  /* XXX
   *
   * Umm. Make this threadsafe. How?!
   * It's clunky that we can't allocate a static mutex.
   * So we have to first allocate one on the heap atomically...
   *
   * Do we also need to lock around module creation?
   *
   * Also, wasm-micro-runtime uses a singleton instance. So if
   * another library or client uses it, all bets are off. :-(
   * If nothing else, around HB_REF2OBJ().
   */

  સ્થિર તાર્કિક initialized;
  જો (initialized)
    પાછા સાચું;

  RuntimeInitArgs init_args;
  hb_memset (&init_args, 0, કદ (RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_Allocator;
  init_args.mem_alloc_option.allocator.malloc_func = (શૂન્ય *) hb_malloc;
  init_args.mem_alloc_option.allocator.realloc_func = (શૂન્ય *) hb_realloc;
  init_args.mem_alloc_option.allocator.free_func = (શૂન્ય *) hb_free;

  // Native symbols need below registration phase
  init_args.n_native_symbols = ARRAY_LENGTH (_hb_wasm_native_symbols);
  init_args.native_module_name = "env";
  init_args.native_symbols = _hb_wasm_native_symbols;

  જો (unlikely (!wasm_runtime_full_init (&init_args)))
  {
    DEBUG_સંદેશ (WASM, nullptr, "Init runtime environment failed.");
    પાછા ખોટું;
  }

#ifndef HB_WASM_NO_MODULES
  wasm_runtime_set_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#endif

  initialized = સાચું;
  પાછા સાચું;
}

hb_wasm_face_data_t *
_hb_wasm_shaper_face_data_create (hb_face_t *face)
{
  અક્ષર error[128];
  hb_wasm_face_data_t *data = nullptr;
  wasm_module_t wasm_module = nullptr;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_table (face, HB_WASM_TAG_WASM));

  unsigned length = hb_blob_get_length (wasm_blob);
  જો (!length)
    પાછા nullptr;

  જો (!_hb_wasm_init ())
    પાછા nullptr;

  wasm_module = wasm_runtime_load ((uint8_t *) hb_blob_get_data_writable (wasm_blob, nullptr),
				   length, error, કદ (error));
  જો (unlikely (!wasm_module))
  {
    DEBUG_સંદેશ (WASM, nullptr, "Load wasm module failed: %s", error);
    પાછા nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  data = (hb_wasm_face_data_t *) hb_calloc (1, કદ (hb_wasm_face_data_t));
  જો (unlikely (!data))
    પાછા nullptr;

  data->wasm_blob = wasm_blob.release ();
  data->wasm_module = wasm_module;

  module_guard.release ();
  પાછા data;
}

સ્થિર hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    અચલ hb_wasm_face_data_t *face_data)
{
  અક્ષર error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_data->plan.get_acquire ();
  જો (likely (plan && face_data->plan.cmpexch (plan, nullptr)))
    પાછા plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, કદ (hb_wasm_shape_plan_t));
  auto plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t ઢગલો_size = 32 * 1024, heap_size = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_data->wasm_module, ઢગલો_size, heap_size, error, કદ (error));
  જો (unlikely (!module_inst))
  {
    DEBUG_સંદેશ (WASM, face_data, "Create wasm module instance failed: %s", error);
    પાછા nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, ઢગલો_size);
  જો (unlikely (!exec_env)) {
    DEBUG_સંદેશ (WASM, face_data, "Create wasm execution environment failed.");
    પાછા nullptr;
  }
  auto exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  જો (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    જો (unlikely (!faceref))
    {
      DEBUG_સંદેશ (WASM, face_data, "Failed to register face object.");
      પાછા nullptr;
    }

    results[0].kind = WASM_I32;
    arguments[0].kind = WASM_I32;
    arguments[0].of.i32 = faceref;
    તાર્કિક ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_LENGTH (results), results,
					 ARRAY_LENGTH (arguments), arguments);

    જો (unlikely (!ret))
    {
      DEBUG_સંદેશ (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      પાછા nullptr;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  પાછા plan;
}

સ્થિર શૂન્ય
release_shape_plan (અચલ hb_wasm_face_data_t *face_data,
		    hb_wasm_shape_plan_t *plan,
		    તાર્કિક cache = ખોટું)
{
  જો (cache && face_data->plan.cmpexch (nullptr, plan))
    પાછા;

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  જો (plan->wasm_shape_planptr)
  {

    auto *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    જો (func)
    {
      wasm_val_t arguments[1];

      arguments[0].kind = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      તાર્કિક ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, nullptr,
					   ARRAY_LENGTH (arguments), arguments);

      જો (unlikely (!ret))
      {
	DEBUG_સંદેશ (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

શૂન્ય
_hb_wasm_shaper_face_data_destroy (hb_wasm_face_data_t *data)
{
  જો (data->plan.get_relaxed ())
    release_shape_plan (data, data->plan);
  wasm_runtime_unload (data->wasm_module);
  hb_blob_destroy (data->wasm_blob);
  hb_free (data);
}


/*
 * shaper font data
 */

સંરચના hb_wasm_font_data_t {};

hb_wasm_font_data_t *
_hb_wasm_shaper_font_data_create (hb_font_t *font HB_UNUSED)
{
  પાછા (hb_wasm_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

શૂન્ય
_hb_wasm_shaper_font_data_destroy (hb_wasm_font_data_t *data HB_UNUSED)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_wasm_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_buffer_t        *buffer,
		અચલ hb_feature_t *features,
		unsigned પૂર્ણાંક        અંક_features)
{
  જો (unlikely (buffer->in_error ()))
    પાછા ખોટું;

  તાર્કિક ret = સાચું;
  hb_face_t *face = font->face;
  અચલ hb_wasm_face_data_t *face_data = face->data.wasm;

  તાર્કિક retried = ખોટું;
  જો (0)
  {
retry:
    DEBUG_સંદેશ (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = nullptr;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_data);
  જો (unlikely (!plan))
  {
    DEBUG_સંદેશ (WASM, face_data, "Acquiring shape-plan failed.");
    પાછા ખોટું;
  }

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (buffer);
  જો (unlikely (!fontref || !bufferref))
  {
    DEBUG_સંદેશ (WASM, module_inst, "Failed to register objects.");
    જાઓ fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  જો (unlikely (!func))
  {
    DEBUG_સંદેશ (WASM, module_inst, "Shape function not found.");
    જાઓ fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = અંક_features ? wasm_runtime_module_dup_data (module_inst,
							     (અચલ અક્ષર *) features,
							     અંક_features * કદ (features[0])) : 0;

  results[0].kind = WASM_I32;
  arguments[0].kind = WASM_I32;
  arguments[0].of.i32 = plan->wasm_shape_planptr;
  arguments[1].kind = WASM_I32;
  arguments[1].of.i32 = fontref;
  arguments[2].kind = WASM_I32;
  arguments[2].of.i32 = bufferref;
  arguments[3].kind = WASM_I32;
  arguments[3].of.i32 = featuresptr;
  arguments[4].kind = WASM_I32;
  arguments[4].of.i32 = અંક_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_LENGTH (results), results,
				  ARRAY_LENGTH (arguments), arguments);

  જો (અંક_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  જો (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_સંદેશ (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    જો (!buffer->ensure_unicode ())
    {
      DEBUG_સંદેશ (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      જાઓ fail;
    }
    જો (retried)
    {
      DEBUG_સંદેશ (WASM, font, "Giving up...");
      જાઓ fail;
    }
    buffer->successful = સાચું;
    retried = સાચું;
    release_shape_plan (face_data, plan);
    plan = nullptr;
    જાઓ retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  જો (!results[0].of.i32)
  {
fail:
    ret = ખોટું;
  }

  release_shape_plan (face_data, plan, ret);

  જો (ret)
  {
    buffer->clear_glyph_flags ();
    buffer->unsafe_to_break ();
  }

  પાછા ret;
}

#endif

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

#अन्तर्भाव "hb-shaper-impl.hh"

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

#अन्तर्भाव "hb-wasm-api.hh"
#अन्तर्भाव "hb-wasm-api-list.hh"

#ifndef HB_WASM_NO_MODULES
#define HB_WASM_NO_MODULES
#endif


#ifndef HB_WASM_NO_MODULES
स्थिर तार्किक HB_UNUSED
_hb_wasm_module_reader (ध्रुव वर्ण *module_name,
		       uint8_t **p_buffer, uint32_t *p_size)
{
  वर्ण path[परिमाण (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (path, module_name, परिमाण (path) - परिमाण (HB_WASM_MODULE_DIR) - 16);
  strncat (path, ".wasm", 6);

  auto *blob = hb_blob_create_from_file (path);

  unsigned length;
  auto *data = hb_blob_get_data (blob, &length);

  *p_buffer = (uint8_t *) hb_malloc (length);

  यदि (length && !p_buffer)
    प्रत्यागम असत्य;

  memcpy (*p_buffer, data, length);
  *p_size = length;

  hb_blob_destroy (blob);

  प्रत्यागम सत्य;
}

स्थिर शून्य HB_UNUSED
_hb_wasm_module_destroyer (uint8_t *buffer, uint32_t size)
{
  hb_free (buffer);
}
#endif

/*
 * shaper face data
 */

#define HB_WASM_TAG_WASM HB_TAG('W','a','s','m')

संरचना hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(शून्य, wasm_shape_plan);
};

संरचना hb_wasm_face_data_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  mutable hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

स्थिर तार्किक
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

  स्थिर तार्किक initialized;
  यदि (initialized)
    प्रत्यागम सत्य;

  RuntimeInitArgs init_args;
  hb_memset (&init_args, 0, परिमाण (RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_Allocator;
  init_args.mem_alloc_option.allocator.malloc_func = (शून्य *) hb_malloc;
  init_args.mem_alloc_option.allocator.realloc_func = (शून्य *) hb_realloc;
  init_args.mem_alloc_option.allocator.free_func = (शून्य *) hb_free;

  // Native symbols need below registration phase
  init_args.n_native_symbols = ARRAY_LENGTH (_hb_wasm_native_symbols);
  init_args.native_module_name = "env";
  init_args.native_symbols = _hb_wasm_native_symbols;

  यदि (unlikely (!wasm_runtime_full_init (&init_args)))
  {
    DEBUG_संदेश (WASM, nullptr, "Init runtime environment failed.");
    प्रत्यागम असत्य;
  }

#ifndef HB_WASM_NO_MODULES
  wasm_runtime_set_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#endif

  initialized = सत्य;
  प्रत्यागम सत्य;
}

hb_wasm_face_data_t *
_hb_wasm_shaper_face_data_create (hb_face_t *face)
{
  वर्ण error[128];
  hb_wasm_face_data_t *data = nullptr;
  wasm_module_t wasm_module = nullptr;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_table (face, HB_WASM_TAG_WASM));

  unsigned length = hb_blob_get_length (wasm_blob);
  यदि (!length)
    प्रत्यागम nullptr;

  यदि (!_hb_wasm_init ())
    प्रत्यागम nullptr;

  wasm_module = wasm_runtime_load ((uint8_t *) hb_blob_get_data_writable (wasm_blob, nullptr),
				   length, error, परिमाण (error));
  यदि (unlikely (!wasm_module))
  {
    DEBUG_संदेश (WASM, nullptr, "Load wasm module failed: %s", error);
    प्रत्यागम nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  data = (hb_wasm_face_data_t *) hb_calloc (1, परिमाण (hb_wasm_face_data_t));
  यदि (unlikely (!data))
    प्रत्यागम nullptr;

  data->wasm_blob = wasm_blob.release ();
  data->wasm_module = wasm_module;

  module_guard.release ();
  प्रत्यागम data;
}

स्थिर hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    ध्रुव hb_wasm_face_data_t *face_data)
{
  वर्ण error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_data->plan.get_acquire ();
  यदि (likely (plan && face_data->plan.cmpexch (plan, nullptr)))
    प्रत्यागम plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, परिमाण (hb_wasm_shape_plan_t));
  auto plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t चळत_size = 32 * 1024, heap_size = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_data->wasm_module, चळत_size, heap_size, error, परिमाण (error));
  यदि (unlikely (!module_inst))
  {
    DEBUG_संदेश (WASM, face_data, "Create wasm module instance failed: %s", error);
    प्रत्यागम nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, चळत_size);
  यदि (unlikely (!exec_env)) {
    DEBUG_संदेश (WASM, face_data, "Create wasm execution environment failed.");
    प्रत्यागम nullptr;
  }
  auto exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  यदि (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    यदि (unlikely (!faceref))
    {
      DEBUG_संदेश (WASM, face_data, "Failed to register face object.");
      प्रत्यागम nullptr;
    }

    results[0].kind = WASM_I32;
    arguments[0].kind = WASM_I32;
    arguments[0].of.i32 = faceref;
    तार्किक ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_LENGTH (results), results,
					 ARRAY_LENGTH (arguments), arguments);

    यदि (unlikely (!ret))
    {
      DEBUG_संदेश (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      प्रत्यागम nullptr;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  प्रत्यागम plan;
}

स्थिर शून्य
release_shape_plan (ध्रुव hb_wasm_face_data_t *face_data,
		    hb_wasm_shape_plan_t *plan,
		    तार्किक cache = असत्य)
{
  यदि (cache && face_data->plan.cmpexch (nullptr, plan))
    प्रत्यागम;

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  यदि (plan->wasm_shape_planptr)
  {

    auto *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    यदि (func)
    {
      wasm_val_t arguments[1];

      arguments[0].kind = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      तार्किक ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, nullptr,
					   ARRAY_LENGTH (arguments), arguments);

      यदि (unlikely (!ret))
      {
	DEBUG_संदेश (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

शून्य
_hb_wasm_shaper_face_data_destroy (hb_wasm_face_data_t *data)
{
  यदि (data->plan.get_relaxed ())
    release_shape_plan (data, data->plan);
  wasm_runtime_unload (data->wasm_module);
  hb_blob_destroy (data->wasm_blob);
  hb_free (data);
}


/*
 * shaper font data
 */

संरचना hb_wasm_font_data_t {};

hb_wasm_font_data_t *
_hb_wasm_shaper_font_data_create (hb_font_t *font HB_UNUSED)
{
  प्रत्यागम (hb_wasm_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

शून्य
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
		ध्रुव hb_feature_t *features,
		unsigned पूर्णाङ्क        अंक_features)
{
  यदि (unlikely (buffer->in_error ()))
    प्रत्यागम असत्य;

  तार्किक ret = सत्य;
  hb_face_t *face = font->face;
  ध्रुव hb_wasm_face_data_t *face_data = face->data.wasm;

  तार्किक retried = असत्य;
  यदि (0)
  {
retry:
    DEBUG_संदेश (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = nullptr;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_data);
  यदि (unlikely (!plan))
  {
    DEBUG_संदेश (WASM, face_data, "Acquiring shape-plan failed.");
    प्रत्यागम असत्य;
  }

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (buffer);
  यदि (unlikely (!fontref || !bufferref))
  {
    DEBUG_संदेश (WASM, module_inst, "Failed to register objects.");
    गच्छ fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  यदि (unlikely (!func))
  {
    DEBUG_संदेश (WASM, module_inst, "Shape function not found.");
    गच्छ fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = अंक_features ? wasm_runtime_module_dup_data (module_inst,
							     (ध्रुव वर्ण *) features,
							     अंक_features * परिमाण (features[0])) : 0;

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
  arguments[4].of.i32 = अंक_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_LENGTH (results), results,
				  ARRAY_LENGTH (arguments), arguments);

  यदि (अंक_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  यदि (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_संदेश (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    यदि (!buffer->ensure_unicode ())
    {
      DEBUG_संदेश (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      गच्छ fail;
    }
    यदि (retried)
    {
      DEBUG_संदेश (WASM, font, "Giving up...");
      गच्छ fail;
    }
    buffer->successful = सत्य;
    retried = सत्य;
    release_shape_plan (face_data, plan);
    plan = nullptr;
    गच्छ retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  यदि (!results[0].of.i32)
  {
fail:
    ret = असत्य;
  }

  release_shape_plan (face_data, plan, ret);

  यदि (ret)
  {
    buffer->clear_glyph_flags ();
    buffer->unsafe_to_break ();
  }

  प्रत्यागम ret;
}

#endif

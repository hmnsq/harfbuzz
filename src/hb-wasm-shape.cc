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

#ಅನ್ತರ್ಭಾವ "hb-shaper-impl.hh"

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

#ಅನ್ತರ್ಭಾವ "hb-wasm-api.hh"
#ಅನ್ತರ್ಭಾವ "hb-wasm-api-list.hh"

#ifndef HB_WASM_NO_MODULES
#define HB_WASM_NO_MODULES
#endif


#ifndef HB_WASM_NO_MODULES
ಸ್ಥಿರ ತಾರ್ಕಿಕ HB_UNUSED
_hb_wasm_module_reader (ಧ್ರುವ ವರ್ಣ *module_name,
		       uint8_t **p_buffer, uint32_t *p_size)
{
  ವರ್ಣ path[ಪರಿಮಾಣ (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (path, module_name, ಪರಿಮಾಣ (path) - ಪರಿಮಾಣ (HB_WASM_MODULE_DIR) - 16);
  strncat (path, ".wasm", 6);

  auto *blob = hb_blob_create_from_file (path);

  unsigned length;
  auto *data = hb_blob_get_data (blob, &length);

  *p_buffer = (uint8_t *) hb_malloc (length);

  ಯದಿ (length && !p_buffer)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  memcpy (*p_buffer, data, length);
  *p_size = length;

  hb_blob_destroy (blob);

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ HB_UNUSED
_hb_wasm_module_destroyer (uint8_t *buffer, uint32_t size)
{
  hb_free (buffer);
}
#endif

/*
 * shaper face data
 */

#define HB_WASM_TAG_WASM HB_TAG('W','a','s','m')

ಸಂರಚನಾ hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(ಶೂನ್ಯ, wasm_shape_plan);
};

ಸಂರಚನಾ hb_wasm_face_data_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  mutable hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ
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

  ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized;
  ಯದಿ (initialized)
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  RuntimeInitArgs init_args;
  hb_memset (&init_args, 0, ಪರಿಮಾಣ (RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_Allocator;
  init_args.mem_alloc_option.allocator.malloc_func = (ಶೂನ್ಯ *) hb_malloc;
  init_args.mem_alloc_option.allocator.realloc_func = (ಶೂನ್ಯ *) hb_realloc;
  init_args.mem_alloc_option.allocator.free_func = (ಶೂನ್ಯ *) hb_free;

  // Native symbols need below registration phase
  init_args.n_native_symbols = ARRAY_LENGTH (_hb_wasm_native_symbols);
  init_args.native_module_name = "env";
  init_args.native_symbols = _hb_wasm_native_symbols;

  ಯದಿ (unlikely (!wasm_runtime_full_init (&init_args)))
  {
    DEBUG_ಸಂದೇಶ (WASM, nullptr, "Init runtime environment failed.");
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

#ifndef HB_WASM_NO_MODULES
  wasm_runtime_set_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#endif

  initialized = ಸತ್ಯ;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

hb_wasm_face_data_t *
_hb_wasm_shaper_face_data_create (hb_face_t *face)
{
  ವರ್ಣ error[128];
  hb_wasm_face_data_t *data = nullptr;
  wasm_module_t wasm_module = nullptr;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_table (face, HB_WASM_TAG_WASM));

  unsigned length = hb_blob_get_length (wasm_blob);
  ಯದಿ (!length)
    ಪ್ರತ್ಯಾಗಮ nullptr;

  ಯದಿ (!_hb_wasm_init ())
    ಪ್ರತ್ಯಾಗಮ nullptr;

  wasm_module = wasm_runtime_load ((uint8_t *) hb_blob_get_data_writable (wasm_blob, nullptr),
				   length, error, ಪರಿಮಾಣ (error));
  ಯದಿ (unlikely (!wasm_module))
  {
    DEBUG_ಸಂದೇಶ (WASM, nullptr, "Load wasm module failed: %s", error);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  data = (hb_wasm_face_data_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_wasm_face_data_t));
  ಯದಿ (unlikely (!data))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  data->wasm_blob = wasm_blob.release ();
  data->wasm_module = wasm_module;

  module_guard.release ();
  ಪ್ರತ್ಯಾಗಮ data;
}

ಸ್ಥಿರ hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    ಧ್ರುವ hb_wasm_face_data_t *face_data)
{
  ವರ್ಣ error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_data->plan.get_acquire ();
  ಯದಿ (likely (plan && face_data->plan.cmpexch (plan, nullptr)))
    ಪ್ರತ್ಯಾಗಮ plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_wasm_shape_plan_t));
  auto plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t ರಾಶಿ_size = 32 * 1024, heap_size = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_data->wasm_module, ರಾಶಿ_size, heap_size, error, ಪರಿಮಾಣ (error));
  ಯದಿ (unlikely (!module_inst))
  {
    DEBUG_ಸಂದೇಶ (WASM, face_data, "Create wasm module instance failed: %s", error);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, ರಾಶಿ_size);
  ಯದಿ (unlikely (!exec_env)) {
    DEBUG_ಸಂದೇಶ (WASM, face_data, "Create wasm execution environment failed.");
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  auto exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  ಯದಿ (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    ಯದಿ (unlikely (!faceref))
    {
      DEBUG_ಸಂದೇಶ (WASM, face_data, "Failed to register face object.");
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }

    results[0].kind = WASM_I32;
    arguments[0].kind = WASM_I32;
    arguments[0].of.i32 = faceref;
    ತಾರ್ಕಿಕ ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_LENGTH (results), results,
					 ARRAY_LENGTH (arguments), arguments);

    ಯದಿ (unlikely (!ret))
    {
      DEBUG_ಸಂದೇಶ (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  ಪ್ರತ್ಯಾಗಮ plan;
}

ಸ್ಥಿರ ಶೂನ್ಯ
release_shape_plan (ಧ್ರುವ hb_wasm_face_data_t *face_data,
		    hb_wasm_shape_plan_t *plan,
		    ತಾರ್ಕಿಕ cache = ಅಸತ್ಯ)
{
  ಯದಿ (cache && face_data->plan.cmpexch (nullptr, plan))
    ಪ್ರತ್ಯಾಗಮ;

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  ಯದಿ (plan->wasm_shape_planptr)
  {

    auto *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    ಯದಿ (func)
    {
      wasm_val_t arguments[1];

      arguments[0].kind = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      ತಾರ್ಕಿಕ ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, nullptr,
					   ARRAY_LENGTH (arguments), arguments);

      ಯದಿ (unlikely (!ret))
      {
	DEBUG_ಸಂದೇಶ (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

ಶೂನ್ಯ
_hb_wasm_shaper_face_data_destroy (hb_wasm_face_data_t *data)
{
  ಯದಿ (data->plan.get_relaxed ())
    release_shape_plan (data, data->plan);
  wasm_runtime_unload (data->wasm_module);
  hb_blob_destroy (data->wasm_blob);
  hb_free (data);
}


/*
 * shaper font data
 */

ಸಂರಚನಾ hb_wasm_font_data_t {};

hb_wasm_font_data_t *
_hb_wasm_shaper_font_data_create (hb_font_t *font HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ (hb_wasm_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

ಶೂನ್ಯ
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
		ಧ್ರುವ hb_feature_t *features,
		unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features)
{
  ಯದಿ (unlikely (buffer->in_error ()))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ತಾರ್ಕಿಕ ret = ಸತ್ಯ;
  hb_face_t *face = font->face;
  ಧ್ರುವ hb_wasm_face_data_t *face_data = face->data.wasm;

  ತಾರ್ಕಿಕ retried = ಅಸತ್ಯ;
  ಯದಿ (0)
  {
retry:
    DEBUG_ಸಂದೇಶ (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = nullptr;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_data);
  ಯದಿ (unlikely (!plan))
  {
    DEBUG_ಸಂದೇಶ (WASM, face_data, "Acquiring shape-plan failed.");
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (buffer);
  ಯದಿ (unlikely (!fontref || !bufferref))
  {
    DEBUG_ಸಂದೇಶ (WASM, module_inst, "Failed to register objects.");
    ಗಚ್ಛ fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  ಯದಿ (unlikely (!func))
  {
    DEBUG_ಸಂದೇಶ (WASM, module_inst, "Shape function not found.");
    ಗಚ್ಛ fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = ಅಂಕೆ_features ? wasm_runtime_module_dup_data (module_inst,
							     (ಧ್ರುವ ವರ್ಣ *) features,
							     ಅಂಕೆ_features * ಪರಿಮಾಣ (features[0])) : 0;

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
  arguments[4].of.i32 = ಅಂಕೆ_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_LENGTH (results), results,
				  ARRAY_LENGTH (arguments), arguments);

  ಯದಿ (ಅಂಕೆ_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  ಯದಿ (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_ಸಂದೇಶ (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    ಯದಿ (!buffer->ensure_unicode ())
    {
      DEBUG_ಸಂದೇಶ (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      ಗಚ್ಛ fail;
    }
    ಯದಿ (retried)
    {
      DEBUG_ಸಂದೇಶ (WASM, font, "Giving up...");
      ಗಚ್ಛ fail;
    }
    buffer->successful = ಸತ್ಯ;
    retried = ಸತ್ಯ;
    release_shape_plan (face_data, plan);
    plan = nullptr;
    ಗಚ್ಛ retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  ಯದಿ (!results[0].of.i32)
  {
fail:
    ret = ಅಸತ್ಯ;
  }

  release_shape_plan (face_data, plan, ret);

  ಯದಿ (ret)
  {
    buffer->clear_glyph_flags ();
    buffer->unsafe_to_break ();
  }

  ಪ್ರತ್ಯಾಗಮ ret;
}

#endif

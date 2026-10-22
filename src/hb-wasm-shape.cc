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

#ଅନ୍ତର୍ଭାଵ "hb-shaper-impl.hh"

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

#ଅନ୍ତର୍ଭାଵ "hb-wasm-api.hh"
#ଅନ୍ତର୍ଭାଵ "hb-wasm-api-list.hh"

#ifndef HB_WASM_NO_MODULES
#define HB_WASM_NO_MODULES
#endif


#ifndef HB_WASM_NO_MODULES
ସ୍ଥିର ତାର୍କିକ HB_UNUSED
_hb_wasm_module_reader (ଅଚଳ ଅକ୍ଷର *module_name,
		       uint8_t **p_buffer, uint32_t *p_size)
{
  ଅକ୍ଷର path[ଆକାର (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (path, module_name, ଆକାର (path) - ଆକାର (HB_WASM_MODULE_DIR) - 16);
  strncat (path, ".wasm", 6);

  auto *blob = hb_blob_create_from_file (path);

  unsigned length;
  auto *data = hb_blob_get_data (blob, &length);

  *p_buffer = (uint8_t *) hb_malloc (length);

  ଯଦି (length && !p_buffer)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  memcpy (*p_buffer, data, length);
  *p_size = length;

  hb_blob_destroy (blob);

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ HB_UNUSED
_hb_wasm_module_destroyer (uint8_t *buffer, uint32_t size)
{
  hb_free (buffer);
}
#endif

/*
 * shaper face data
 */

#define HB_WASM_TAG_WASM HB_TAG('W','a','s','m')

ସଂରଚନା hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(ଶୂନ୍ୟ, wasm_shape_plan);
};

ସଂରଚନା hb_wasm_face_data_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  mutable hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

ସ୍ଥିର ତାର୍କିକ
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

  ସ୍ଥିର ତାର୍କିକ initialized;
  ଯଦି (initialized)
    ଫେରନ୍ତୁ ସତ୍ୟ;

  RuntimeInitArgs init_args;
  hb_memset (&init_args, 0, ଆକାର (RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_Allocator;
  init_args.mem_alloc_option.allocator.malloc_func = (ଶୂନ୍ୟ *) hb_malloc;
  init_args.mem_alloc_option.allocator.realloc_func = (ଶୂନ୍ୟ *) hb_realloc;
  init_args.mem_alloc_option.allocator.free_func = (ଶୂନ୍ୟ *) hb_free;

  // Native symbols need below registration phase
  init_args.n_native_symbols = ARRAY_LENGTH (_hb_wasm_native_symbols);
  init_args.native_module_name = "env";
  init_args.native_symbols = _hb_wasm_native_symbols;

  ଯଦି (unlikely (!wasm_runtime_full_init (&init_args)))
  {
    DEBUG_ସନ୍ଦେଶ (WASM, nullptr, "Init runtime environment failed.");
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

#ifndef HB_WASM_NO_MODULES
  wasm_runtime_set_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#endif

  initialized = ସତ୍ୟ;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

hb_wasm_face_data_t *
_hb_wasm_shaper_face_data_create (hb_face_t *face)
{
  ଅକ୍ଷର error[128];
  hb_wasm_face_data_t *data = nullptr;
  wasm_module_t wasm_module = nullptr;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_table (face, HB_WASM_TAG_WASM));

  unsigned length = hb_blob_get_length (wasm_blob);
  ଯଦି (!length)
    ଫେରନ୍ତୁ nullptr;

  ଯଦି (!_hb_wasm_init ())
    ଫେରନ୍ତୁ nullptr;

  wasm_module = wasm_runtime_load ((uint8_t *) hb_blob_get_data_writable (wasm_blob, nullptr),
				   length, error, ଆକାର (error));
  ଯଦି (unlikely (!wasm_module))
  {
    DEBUG_ସନ୍ଦେଶ (WASM, nullptr, "Load wasm module failed: %s", error);
    ଫେରନ୍ତୁ nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  data = (hb_wasm_face_data_t *) hb_calloc (1, ଆକାର (hb_wasm_face_data_t));
  ଯଦି (unlikely (!data))
    ଫେରନ୍ତୁ nullptr;

  data->wasm_blob = wasm_blob.release ();
  data->wasm_module = wasm_module;

  module_guard.release ();
  ଫେରନ୍ତୁ data;
}

ସ୍ଥିର hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    ଅଚଳ hb_wasm_face_data_t *face_data)
{
  ଅକ୍ଷର error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_data->plan.get_acquire ();
  ଯଦି (likely (plan && face_data->plan.cmpexch (plan, nullptr)))
    ଫେରନ୍ତୁ plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, ଆକାର (hb_wasm_shape_plan_t));
  auto plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t ଗଦା_size = 32 * 1024, heap_size = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_data->wasm_module, ଗଦା_size, heap_size, error, ଆକାର (error));
  ଯଦି (unlikely (!module_inst))
  {
    DEBUG_ସନ୍ଦେଶ (WASM, face_data, "Create wasm module instance failed: %s", error);
    ଫେରନ୍ତୁ nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, ଗଦା_size);
  ଯଦି (unlikely (!exec_env)) {
    DEBUG_ସନ୍ଦେଶ (WASM, face_data, "Create wasm execution environment failed.");
    ଫେରନ୍ତୁ nullptr;
  }
  auto exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  ଯଦି (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    ଯଦି (unlikely (!faceref))
    {
      DEBUG_ସନ୍ଦେଶ (WASM, face_data, "Failed to register face object.");
      ଫେରନ୍ତୁ nullptr;
    }

    results[0].kind = WASM_I32;
    arguments[0].kind = WASM_I32;
    arguments[0].of.i32 = faceref;
    ତାର୍କିକ ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_LENGTH (results), results,
					 ARRAY_LENGTH (arguments), arguments);

    ଯଦି (unlikely (!ret))
    {
      DEBUG_ସନ୍ଦେଶ (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      ଫେରନ୍ତୁ nullptr;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  ଫେରନ୍ତୁ plan;
}

ସ୍ଥିର ଶୂନ୍ୟ
release_shape_plan (ଅଚଳ hb_wasm_face_data_t *face_data,
		    hb_wasm_shape_plan_t *plan,
		    ତାର୍କିକ cache = ମିଥ୍ୟା)
{
  ଯଦି (cache && face_data->plan.cmpexch (nullptr, plan))
    ଫେରନ୍ତୁ;

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  ଯଦି (plan->wasm_shape_planptr)
  {

    auto *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    ଯଦି (func)
    {
      wasm_val_t arguments[1];

      arguments[0].kind = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      ତାର୍କିକ ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, nullptr,
					   ARRAY_LENGTH (arguments), arguments);

      ଯଦି (unlikely (!ret))
      {
	DEBUG_ସନ୍ଦେଶ (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

ଶୂନ୍ୟ
_hb_wasm_shaper_face_data_destroy (hb_wasm_face_data_t *data)
{
  ଯଦି (data->plan.get_relaxed ())
    release_shape_plan (data, data->plan);
  wasm_runtime_unload (data->wasm_module);
  hb_blob_destroy (data->wasm_blob);
  hb_free (data);
}


/*
 * shaper font data
 */

ସଂରଚନା hb_wasm_font_data_t {};

hb_wasm_font_data_t *
_hb_wasm_shaper_font_data_create (hb_font_t *font HB_UNUSED)
{
  ଫେରନ୍ତୁ (hb_wasm_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

ଶୂନ୍ୟ
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
		ଅଚଳ hb_feature_t *features,
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features)
{
  ଯଦି (unlikely (buffer->in_error ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ତାର୍କିକ ret = ସତ୍ୟ;
  hb_face_t *face = font->face;
  ଅଚଳ hb_wasm_face_data_t *face_data = face->data.wasm;

  ତାର୍କିକ retried = ମିଥ୍ୟା;
  ଯଦି (0)
  {
retry:
    DEBUG_ସନ୍ଦେଶ (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = nullptr;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_data);
  ଯଦି (unlikely (!plan))
  {
    DEBUG_ସନ୍ଦେଶ (WASM, face_data, "Acquiring shape-plan failed.");
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (buffer);
  ଯଦି (unlikely (!fontref || !bufferref))
  {
    DEBUG_ସନ୍ଦେଶ (WASM, module_inst, "Failed to register objects.");
    ଯାଅ fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  ଯଦି (unlikely (!func))
  {
    DEBUG_ସନ୍ଦେଶ (WASM, module_inst, "Shape function not found.");
    ଯାଅ fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = ଅଙ୍କ_features ? wasm_runtime_module_dup_data (module_inst,
							     (ଅଚଳ ଅକ୍ଷର *) features,
							     ଅଙ୍କ_features * ଆକାର (features[0])) : 0;

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
  arguments[4].of.i32 = ଅଙ୍କ_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_LENGTH (results), results,
				  ARRAY_LENGTH (arguments), arguments);

  ଯଦି (ଅଙ୍କ_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  ଯଦି (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_ସନ୍ଦେଶ (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    ଯଦି (!buffer->ensure_unicode ())
    {
      DEBUG_ସନ୍ଦେଶ (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      ଯାଅ fail;
    }
    ଯଦି (retried)
    {
      DEBUG_ସନ୍ଦେଶ (WASM, font, "Giving up...");
      ଯାଅ fail;
    }
    buffer->successful = ସତ୍ୟ;
    retried = ସତ୍ୟ;
    release_shape_plan (face_data, plan);
    plan = nullptr;
    ଯାଅ retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  ଯଦି (!results[0].of.i32)
  {
fail:
    ret = ମିଥ୍ୟା;
  }

  release_shape_plan (face_data, plan, ret);

  ଯଦି (ret)
  {
    buffer->clear_glyph_flags ();
    buffer->unsafe_to_break ();
  }

  ଫେରନ୍ତୁ ret;
}

#endif

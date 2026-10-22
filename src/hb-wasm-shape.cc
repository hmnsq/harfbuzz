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

#অন্তর্ভাব "hb-shaper-impl.hh"

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

#অন্তর্ভাব "hb-wasm-api.hh"
#অন্তর্ভাব "hb-wasm-api-list.hh"

#ifndef HB_WASM_NO_MODULES
#define HB_WASM_NO_MODULES
#endif


#ifndef HB_WASM_NO_MODULES
স্থির যৌক্তিক HB_UNUSED
_hb_wasm_module_reader (ধ্রুবক অক্ষর *module_name,
		       uint8_t **p_বাফার, uint32_t *p_আকার)
{
  অক্ষর path[আকার (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (path, module_name, আকার (path) - আকার (HB_WASM_MODULE_DIR) - 16);
  strncat (path, ".wasm", 6);

  auto *blob = hb_blob_create_from_file (path);

  unsigned দৈর্ঘ্য;
  auto *তথ্য = hb_blob_get_তথ্য (blob, &দৈর্ঘ্য);

  *p_বাফার = (uint8_t *) hb_malloc (দৈর্ঘ্য);

  যদি (দৈর্ঘ্য && !p_বাফার)
    ফেরত মিথ্যা;

  memcpy (*p_বাফার, তথ্য, দৈর্ঘ্য);
  *p_আকার = দৈর্ঘ্য;

  hb_blob_destroy (blob);

  ফেরত সত্য;
}

স্থির শূন্য HB_UNUSED
_hb_wasm_module_destroyer (uint8_t *বাফার, uint32_t আকার_2)
{
  hb_free (বাফার);
}
#endif

/*
 * shaper face data
 */

#define HB_WASM_TAG_WASM HB_TAG('W','a','s','m')

কাঠামো hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(শূন্য, wasm_shape_plan);
};

কাঠামো hb_wasm_face_তথ্য_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  mutable hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

স্থির যৌক্তিক
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

  স্থির যৌক্তিক initialized;
  যদি (initialized)
    ফেরত সত্য;

  RuntimeInitArgs init_args;
  hb_memset (&init_args, 0, আকার (RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_Allocator;
  init_args.mem_alloc_option.allocator.malloc_func = (শূন্য *) hb_malloc;
  init_args.mem_alloc_option.allocator.realloc_func = (শূন্য *) hb_realloc;
  init_args.mem_alloc_option.allocator.free_func = (শূন্য *) hb_free;

  // Native symbols need below registration phase
  init_args.n_native_symbols = ARRAY_দৈর্ঘ্য (_hb_wasm_native_symbols);
  init_args.native_module_name = "env";
  init_args.native_symbols = _hb_wasm_native_symbols;

  যদি (unlikely (!wasm_runtime_full_init (&init_args)))
  {
    DEBUG_বার্তা (WASM, nullptr, "Init runtime environment failed.");
    ফেরত মিথ্যা;
  }

#ifndef HB_WASM_NO_MODULES
  wasm_runtime_set_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#endif

  initialized = সত্য;
  ফেরত সত্য;
}

hb_wasm_face_তথ্য_t *
hb_wasm_shaper_face_তথ্য_create (hb_face_t *face)
{
  অক্ষর error[128];
  hb_wasm_face_তথ্য_t *তথ্য = nullptr;
  wasm_module_t wasm_module = nullptr;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_table (face, HB_WASM_TAG_WASM));

  unsigned দৈর্ঘ্য = hb_blob_get_দৈর্ঘ্য (wasm_blob);
  যদি (!দৈর্ঘ্য)
    ফেরত nullptr;

  যদি (!_hb_wasm_init ())
    ফেরত nullptr;

  wasm_module = wasm_runtime_load ((uint8_t *) hb_blob_get_তথ্য_writable (wasm_blob, nullptr),
				   দৈর্ঘ্য, error, আকার (error));
  যদি (unlikely (!wasm_module))
  {
    DEBUG_বার্তা (WASM, nullptr, "Load wasm module failed: %s", error);
    ফেরত nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  তথ্য = (hb_wasm_face_তথ্য_t *) hb_calloc (1, আকার (hb_wasm_face_তথ্য_t));
  যদি (unlikely (!তথ্য))
    ফেরত nullptr;

  তথ্য->wasm_blob = wasm_blob.release ();
  তথ্য->wasm_module = wasm_module;

  module_guard.release ();
  ফেরত তথ্য;
}

স্থির hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    ধ্রুবক hb_wasm_face_তথ্য_t *face_তথ্য)
{
  অক্ষর error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_তথ্য->plan.get_acquire ();
  যদি (likely (plan && face_তথ্য->plan.cmpexch (plan, nullptr)))
    ফেরত plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, আকার (hb_wasm_shape_plan_t));
  auto plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t স্তূপ_আকার = 32 * 1024, heap_আকার = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_তথ্য->wasm_module, স্তূপ_আকার, heap_আকার, error, আকার (error));
  যদি (unlikely (!module_inst))
  {
    DEBUG_বার্তা (WASM, face_তথ্য, "Create wasm module instance failed: %s", error);
    ফেরত nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, স্তূপ_আকার);
  যদি (unlikely (!exec_env)) {
    DEBUG_বার্তা (WASM, face_তথ্য, "Create wasm execution environment failed.");
    ফেরত nullptr;
  }
  auto exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  যদি (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    যদি (unlikely (!faceref))
    {
      DEBUG_বার্তা (WASM, face_তথ্য, "Failed to register face object.");
      ফেরত nullptr;
    }

    results[0].kind = WASM_I32;
    arguments[0].kind = WASM_I32;
    arguments[0].of.i32 = faceref;
    যৌক্তিক ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_দৈর্ঘ্য (results), results,
					 ARRAY_দৈর্ঘ্য (arguments), arguments);

    যদি (unlikely (!ret))
    {
      DEBUG_বার্তা (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      ফেরত nullptr;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  ফেরত plan;
}

স্থির শূন্য
release_shape_plan (ধ্রুবক hb_wasm_face_তথ্য_t *face_তথ্য,
		    hb_wasm_shape_plan_t *plan,
		    যৌক্তিক cache = মিথ্যা)
{
  যদি (cache && face_তথ্য->plan.cmpexch (nullptr, plan))
    ফেরত;

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  যদি (plan->wasm_shape_planptr)
  {

    auto *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    যদি (func)
    {
      wasm_val_t arguments[1];

      arguments[0].kind = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      যৌক্তিক ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, nullptr,
					   ARRAY_দৈর্ঘ্য (arguments), arguments);

      যদি (unlikely (!ret))
      {
	DEBUG_বার্তা (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

শূন্য
hb_wasm_shaper_face_তথ্য_destroy (hb_wasm_face_তথ্য_t *তথ্য)
{
  যদি (তথ্য->plan.get_relaxed ())
    release_shape_plan (তথ্য, তথ্য->plan);
  wasm_runtime_unload (তথ্য->wasm_module);
  hb_blob_destroy (তথ্য->wasm_blob);
  hb_free (তথ্য);
}


/*
 * shaper font data
 */

কাঠামো hb_wasm_font_তথ্য_t {};

hb_wasm_font_তথ্য_t *
hb_wasm_shaper_font_তথ্য_create (hb_font_t *font HB_UNUSED)
{
  ফেরত (hb_wasm_font_তথ্য_t *) HB_SHAPER_তথ্য_SUCCEEDED;
}

শূন্য
hb_wasm_shaper_font_তথ্য_destroy (hb_wasm_font_তথ্য_t *তথ্য HB_UNUSED)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_wasm_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_বাফার_t        *বাফার,
		ধ্রুবক hb_feature_t *features,
		unsigned পূর্ণসংখ্যা        অঙ্ক_features)
{
  যদি (unlikely (বাফার->in_error ()))
    ফেরত মিথ্যা;

  যৌক্তিক ret = সত্য;
  hb_face_t *face = font->face;
  ধ্রুবক hb_wasm_face_তথ্য_t *face_তথ্য = face->তথ্য.wasm;

  যৌক্তিক retried = মিথ্যা;
  যদি (0)
  {
retry:
    DEBUG_বার্তা (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = nullptr;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_তথ্য);
  যদি (unlikely (!plan))
  {
    DEBUG_বার্তা (WASM, face_তথ্য, "Acquiring shape-plan failed.");
    ফেরত মিথ্যা;
  }

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (বাফার);
  যদি (unlikely (!fontref || !bufferref))
  {
    DEBUG_বার্তা (WASM, module_inst, "Failed to register objects.");
    যাও fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  যদি (unlikely (!func))
  {
    DEBUG_বার্তা (WASM, module_inst, "Shape function not found.");
    যাও fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = অঙ্ক_features ? wasm_runtime_module_dup_তথ্য (module_inst,
							     (ধ্রুবক অক্ষর *) features,
							     অঙ্ক_features * আকার (features[0])) : 0;

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
  arguments[4].of.i32 = অঙ্ক_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_দৈর্ঘ্য (results), results,
				  ARRAY_দৈর্ঘ্য (arguments), arguments);

  যদি (অঙ্ক_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  যদি (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_বার্তা (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    যদি (!বাফার->ensure_unicode ())
    {
      DEBUG_বার্তা (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      যাও fail;
    }
    যদি (retried)
    {
      DEBUG_বার্তা (WASM, font, "Giving up...");
      যাও fail;
    }
    বাফার->successful = সত্য;
    retried = সত্য;
    release_shape_plan (face_তথ্য, plan);
    plan = nullptr;
    যাও retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  যদি (!results[0].of.i32)
  {
fail:
    ret = মিথ্যা;
  }

  release_shape_plan (face_তথ্য, plan, ret);

  যদি (ret)
  {
    বাফার->clear_glyph_flags ();
    বাফার->unsafe_to_break ();
  }

  ফেরত ret;
}

#endif

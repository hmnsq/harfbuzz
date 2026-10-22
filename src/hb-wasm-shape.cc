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

#अघोषित HB_DEBUG_WASM
#घोषणा HB_DEBUG_WASM 1

#समावेश "hb-shaper-impl.hh"

#अगर_घोषित HAVE_WASM

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

#समावेश "hb-wasm-api.hh"
#समावेश "hb-wasm-api-list.hh"

#अगर_अघोषित HB_WASM_NO_MODULES
#घोषणा HB_WASM_NO_MODULES
#पूर्ण_अगर


#अगर_अघोषित HB_WASM_NO_MODULES
अटल bool HB_UNUSED
_hb_wasm_module_reader (स्थिर अक्षर *module_name,
		       uint8_t **p_बफर, uint32_t *p_आकार)
{
  अक्षर path[माप (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  म_जोड़न (path, module_name, माप (path) - माप (HB_WASM_MODULE_DIR) - 16);
  म_जोड़न (path, ".wasm", 6);

  स्वतः *blob = hb_blob_create_from_file (path);

  अचिन्हित लंबाई;
  स्वतः *तथ्य = hb_blob_get_तथ्य (blob, &लंबाई);

  *p_बफर = (uint8_t *) hb_malloc (लंबाई);

  अगर (लंबाई && !p_बफर)
    वापस false;

  स_नकल (*p_बफर, तथ्य, लंबाई);
  *p_आकार = लंबाई;

  hb_blob_destroy (blob);

  वापस true;
}

अटल व्योम HB_UNUSED
_hb_wasm_module_destroyer (uint8_t *बफर, uint32_t आकार)
{
  hb_free (बफर);
}
#पूर्ण_अगर

/*
 * shaper face data
 */

#घोषणा HB_WASM_TAG_WASM HB_TAG('W','a','s','m')

काष्ठा hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(व्योम, wasm_shape_plan);
};

काष्ठा hb_wasm_face_तथ्य_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  mutable hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

अटल bool
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

  अटल bool initialized;
  अगर (initialized)
    वापस true;

  RuntimeInitArgs init_args;
  hb_memset (&init_args, 0, माप (RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_Allocator;
  init_args.mem_alloc_option.allocator.malloc_func = (व्योम *) hb_malloc;
  init_args.mem_alloc_option.allocator.realloc_func = (व्योम *) hb_realloc;
  init_args.mem_alloc_option.allocator.free_func = (व्योम *) hb_free;

  // Native symbols need below registration phase
  init_args.n_native_symbols = ARRAY_लंबाई (_hb_wasm_native_symbols);
  init_args.native_module_name = "env";
  init_args.native_symbols = _hb_wasm_native_symbols;

  अगर (unlikely (!wasm_runtime_full_init (&init_args)))
  {
    DEBUG_संदेश (WASM, nullptr, "Init runtime environment failed.");
    वापस false;
  }

#अगर_अघोषित HB_WASM_NO_MODULES
  wasm_runtime_set_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#पूर्ण_अगर

  initialized = true;
  वापस true;
}

hb_wasm_face_तथ्य_t *
hb_wasm_shaper_face_तथ्य_create (hb_face_t *face)
{
  अक्षर error[128];
  hb_wasm_face_तथ्य_t *तथ्य = nullptr;
  wasm_module_t wasm_module = nullptr;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_table (face, HB_WASM_TAG_WASM));

  अचिन्हित लंबाई = hb_blob_get_लंबाई (wasm_blob);
  अगर (!लंबाई)
    वापस nullptr;

  अगर (!_hb_wasm_init ())
    वापस nullptr;

  wasm_module = wasm_runtime_load ((uint8_t *) hb_blob_get_तथ्य_writable (wasm_blob, nullptr),
				   लंबाई, error, माप (error));
  अगर (unlikely (!wasm_module))
  {
    DEBUG_संदेश (WASM, nullptr, "Load wasm module failed: %s", error);
    वापस nullptr;
  }
  स्वतः module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  तथ्य = (hb_wasm_face_तथ्य_t *) hb_calloc (1, माप (hb_wasm_face_तथ्य_t));
  अगर (unlikely (!तथ्य))
    वापस nullptr;

  तथ्य->wasm_blob = wasm_blob.release ();
  तथ्य->wasm_module = wasm_module;

  module_guard.release ();
  वापस तथ्य;
}

अटल hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    स्थिर hb_wasm_face_तथ्य_t *face_तथ्य)
{
  अक्षर error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_तथ्य->plan.get_acquire ();
  अगर (likely (plan && face_तथ्य->plan.cmpexch (plan, nullptr)))
    वापस plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, माप (hb_wasm_shape_plan_t));
  स्वतः plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t ढेर_आकार = 32 * 1024, heap_आकार = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_तथ्य->wasm_module, ढेर_आकार, heap_आकार, error, माप (error));
  अगर (unlikely (!module_inst))
  {
    DEBUG_संदेश (WASM, face_तथ्य, "Create wasm module instance failed: %s", error);
    वापस nullptr;
  }
  स्वतः module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, ढेर_आकार);
  अगर (unlikely (!exec_env)) {
    DEBUG_संदेश (WASM, face_तथ्य, "Create wasm execution environment failed.");
    वापस nullptr;
  }
  स्वतः exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  अगर (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    अगर (unlikely (!faceref))
    {
      DEBUG_संदेश (WASM, face_तथ्य, "Failed to register face object.");
      वापस nullptr;
    }

    results[0].kind = WASM_I32;
    arguments[0].kind = WASM_I32;
    arguments[0].of.i32 = faceref;
    bool ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_लंबाई (results), results,
					 ARRAY_लंबाई (arguments), arguments);

    अगर (unlikely (!ret))
    {
      DEBUG_संदेश (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      वापस nullptr;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  वापस plan;
}

अटल व्योम
release_shape_plan (स्थिर hb_wasm_face_तथ्य_t *face_तथ्य,
		    hb_wasm_shape_plan_t *plan,
		    bool cache = false)
{
  अगर (cache && face_तथ्य->plan.cmpexch (nullptr, plan))
    वापस;

  स्वतः *module_inst = plan->module_inst;
  स्वतः *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  अगर (plan->wasm_shape_planptr)
  {

    स्वतः *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    अगर (func)
    {
      wasm_val_t arguments[1];

      arguments[0].kind = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      bool ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, nullptr,
					   ARRAY_लंबाई (arguments), arguments);

      अगर (unlikely (!ret))
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

व्योम
hb_wasm_shaper_face_तथ्य_destroy (hb_wasm_face_तथ्य_t *तथ्य)
{
  अगर (तथ्य->plan.get_relaxed ())
    release_shape_plan (तथ्य, तथ्य->plan);
  wasm_runtime_unload (तथ्य->wasm_module);
  hb_blob_destroy (तथ्य->wasm_blob);
  hb_free (तथ्य);
}


/*
 * shaper font data
 */

काष्ठा hb_wasm_font_तथ्य_t {};

hb_wasm_font_तथ्य_t *
hb_wasm_shaper_font_तथ्य_create (hb_font_t *font HB_UNUSED)
{
  वापस (hb_wasm_font_तथ्य_t *) HB_SHAPER_तथ्य_SUCCEEDED;
}

व्योम
hb_wasm_shaper_font_तथ्य_destroy (hb_wasm_font_तथ्य_t *तथ्य HB_UNUSED)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_wasm_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_बफर_t        *बफर,
		स्थिर hb_feature_t *features,
		अचिन्हित पूर्णांक        अंक_features)
{
  अगर (unlikely (बफर->in_error ()))
    वापस false;

  bool ret = true;
  hb_face_t *face = font->face;
  स्थिर hb_wasm_face_तथ्य_t *face_तथ्य = face->तथ्य.wasm;

  bool retried = false;
  अगर (0)
  {
retry:
    DEBUG_संदेश (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = nullptr;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_तथ्य);
  अगर (unlikely (!plan))
  {
    DEBUG_संदेश (WASM, face_तथ्य, "Acquiring shape-plan failed.");
    वापस false;
  }

  स्वतः *module_inst = plan->module_inst;
  स्वतः *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (बफर);
  अगर (unlikely (!fontref || !bufferref))
  {
    DEBUG_संदेश (WASM, module_inst, "Failed to register objects.");
    जाओ हार;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  अगर (unlikely (!func))
  {
    DEBUG_संदेश (WASM, module_inst, "Shape function not found.");
    जाओ हार;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = अंक_features ? wasm_runtime_module_dup_तथ्य (module_inst,
							     (स्थिर अक्षर *) features,
							     अंक_features * माप (features[0])) : 0;

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
				  ARRAY_लंबाई (results), results,
				  ARRAY_लंबाई (arguments), arguments);

  अगर (अंक_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  अगर (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_संदेश (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    अगर (!बफर->ensure_unicode ())
    {
      DEBUG_संदेश (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      जाओ हार;
    }
    अगर (retried)
    {
      DEBUG_संदेश (WASM, font, "Giving up...");
      जाओ हार;
    }
    बफर->successful = true;
    retried = true;
    release_shape_plan (face_तथ्य, plan);
    plan = nullptr;
    जाओ retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  अगर (!results[0].of.i32)
  {
हार:
    ret = false;
  }

  release_shape_plan (face_तथ्य, plan, ret);

  अगर (ret)
  {
    बफर->clear_glyph_flags ();
    बफर->unsafe_to_break ();
  }

  वापस ret;
}

#पूर्ण_अगर

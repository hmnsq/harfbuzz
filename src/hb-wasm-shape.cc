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

#నిర్వచనం_రద్దు HB_DEBUG_WASM
#నిర్వచించు HB_DEBUG_WASM 1

#అన్తర్భావ "hb-shaper-impl.hh"

#నిర్వచితమైతే HAVE_WASM

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

#అన్తర్భావ "hb-wasm-api.hh"
#అన్తర్భావ "hb-wasm-api-list.hh"

#నిర్వచితం_కాకపోతే HB_WASM_NO_MODULES
#నిర్వచించు HB_WASM_NO_MODULES
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_WASM_NO_MODULES
స్థిర తార్కిక HB_వాడని
_hb_wasm_module_reader (స్థిరరాశి అక్షరం *module_name,
		       uint8_t **p_బఫరు, uint32_t *p_కొలత)
{
  అక్షరం మార్గం[పరిమాణం (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (మార్గం, module_name, పరిమాణం (మార్గం) - పరిమాణం (HB_WASM_MODULE_DIR) - 16);
  strncat (మార్గం, ".wasm", 6);

  స్వయంచాలక *blob = hb_blob_create_from_file (మార్గం);

  గుర్తులేని నిడివి;
  స్వయంచాలక *దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &నిడివి);

  *p_బఫరు = (uint8_t *) hb_malloc (నిడివి);

  ఒకవేళ (నిడివి && !p_బఫరు)
    తిరిగి అబద్ధం;

  జ్ఞాపకం_నకలు (*p_బఫరు, దత్తాంశం, నిడివి);
  *p_కొలత = నిడివి;

  hb_blob_destroy (blob);

  తిరిగి నిజం;
}

స్థిర శూన్య HB_వాడని
_hb_wasm_module_destroyer (uint8_t *బఫరు, uint32_t కొలత)
{
  hb_free (బఫరు);
}
#అయితే_ముగింపు

/*
 * shaper face data
 */

#నిర్వచించు HB_WASM_ట్యాగ్_WASM HB_ట్యాగ్('W','a','s','m')

నిర్మాణం hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(శూన్య, wasm_shape_plan);
};

నిర్మాణం hb_wasm_face_దత్తాంశం_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  మార్చదగిన hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

స్థిర తార్కిక
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

  స్థిర తార్కిక initialized;
  ఒకవేళ (initialized)
    తిరిగి నిజం;

  Runtime_Init_వాదనలు init_వాదనలు;
  hb_memset (&init_వాదనలు, 0, పరిమాణం (Runtime_Init_వాదనలు));

  init_వాదనలు.mem_alloc_type = Alloc_With_Allocator;
  init_వాదనలు.mem_alloc_option.allocator.malloc_func = (శూన్య *) hb_malloc;
  init_వాదనలు.mem_alloc_option.allocator.realloc_func = (శూన్య *) hb_realloc;
  init_వాదనలు.mem_alloc_option.allocator.free_func = (శూన్య *) hb_free;

  // Native symbols need below registration phase
  init_వాదనలు.n_native_symbols = ARRAY_నిడివి (_hb_wasm_native_symbols);
  init_వాదనలు.native_module_name = "env";
  init_వాదనలు.native_symbols = _hb_wasm_native_symbols;

  ఒకవేళ (unlikely (!wasm_runtime_నిండిన_init (&init_వాదనలు)))
  {
    DEBUG_సందేశం (WASM, శూన్యసూచిక, "Init runtime environment failed.");
    తిరిగి అబద్ధం;
  }

#నిర్వచితం_కాకపోతే HB_WASM_NO_MODULES
  wasm_runtime_సమితి_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#అయితే_ముగింపు

  initialized = నిజం;
  తిరిగి నిజం;
}

hb_wasm_face_దత్తాంశం_t *
hb_wasm_shaper_face_దత్తాంశం_create (hb_face_t *face)
{
  అక్షరం error[128];
  hb_wasm_face_దత్తాంశం_t *దత్తాంశం = శూన్యసూచిక;
  wasm_module_t wasm_module = శూన్యసూచిక;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_పట్టిక (face, HB_WASM_ట్యాగ్_WASM));

  గుర్తులేని నిడివి = hb_blob_get_నిడివి (wasm_blob);
  ఒకవేళ (!నిడివి)
    తిరిగి శూన్యసూచిక;

  ఒకవేళ (!_hb_wasm_init ())
    తిరిగి శూన్యసూచిక;

  wasm_module = wasm_runtime_ఎక్కించు ((uint8_t *) hb_blob_get_దత్తాంశం_writable (wasm_blob, శూన్యసూచిక),
				   నిడివి, error, పరిమాణం (error));
  ఒకవేళ (unlikely (!wasm_module))
  {
    DEBUG_సందేశం (WASM, శూన్యసూచిక, "Load wasm module failed: %s", error);
    తిరిగి శూన్యసూచిక;
  }
  స్వయంచాలక module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  దత్తాంశం = (hb_wasm_face_దత్తాంశం_t *) hb_calloc (1, పరిమాణం (hb_wasm_face_దత్తాంశం_t));
  ఒకవేళ (unlikely (!దత్తాంశం))
    తిరిగి శూన్యసూచిక;

  దత్తాంశం->wasm_blob = wasm_blob.release ();
  దత్తాంశం->wasm_module = wasm_module;

  module_guard.release ();
  తిరిగి దత్తాంశం;
}

స్థిర hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    స్థిరరాశి hb_wasm_face_దత్తాంశం_t *face_దత్తాంశం)
{
  అక్షరం error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_దత్తాంశం->plan.get_acquire ();
  ఒకవేళ (likely (plan && face_దత్తాంశం->plan.cmpexch (plan, శూన్యసూచిక)))
    తిరిగి plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, పరిమాణం (hb_wasm_shape_plan_t));
  స్వయంచాలక plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t దొంతర_కొలత = 32 * 1024, కుప్ప_కొలత = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_దత్తాంశం->wasm_module, దొంతర_కొలత, కుప్ప_కొలత, error, పరిమాణం (error));
  ఒకవేళ (unlikely (!module_inst))
  {
    DEBUG_సందేశం (WASM, face_దత్తాంశం, "Create wasm module instance failed: %s", error);
    తిరిగి శూన్యసూచిక;
  }
  స్వయంచాలక module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, దొంతర_కొలత);
  ఒకవేళ (unlikely (!exec_env)) {
    DEBUG_సందేశం (WASM, face_దత్తాంశం, "Create wasm execution environment failed.");
    తిరిగి శూన్యసూచిక;
  }
  స్వయంచాలక exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  ఒకవేళ (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    ఒకవేళ (unlikely (!faceref))
    {
      DEBUG_సందేశం (WASM, face_దత్తాంశం, "Failed to register face object.");
      తిరిగి శూన్యసూచిక;
    }

    results[0].రకం = WASM_I32;
    arguments[0].రకం = WASM_I32;
    arguments[0].of.i32 = faceref;
    తార్కిక ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_నిడివి (results), results,
					 ARRAY_నిడివి (arguments), arguments);

    ఒకవేళ (unlikely (!ret))
    {
      DEBUG_సందేశం (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      తిరిగి శూన్యసూచిక;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  తిరిగి plan;
}

స్థిర శూన్య
release_shape_plan (స్థిరరాశి hb_wasm_face_దత్తాంశం_t *face_దత్తాంశం,
		    hb_wasm_shape_plan_t *plan,
		    తార్కిక cache = అబద్ధం)
{
  ఒకవేళ (cache && face_దత్తాంశం->plan.cmpexch (శూన్యసూచిక, plan))
    తిరిగి;

  స్వయంచాలక *module_inst = plan->module_inst;
  స్వయంచాలక *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  ఒకవేళ (plan->wasm_shape_planptr)
  {

    స్వయంచాలక *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    ఒకవేళ (func)
    {
      wasm_val_t arguments[1];

      arguments[0].రకం = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      తార్కిక ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, శూన్యసూచిక,
					   ARRAY_నిడివి (arguments), arguments);

      ఒకవేళ (unlikely (!ret))
      {
	DEBUG_సందేశం (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

శూన్య
hb_wasm_shaper_face_దత్తాంశం_destroy (hb_wasm_face_దత్తాంశం_t *దత్తాంశం)
{
  ఒకవేళ (దత్తాంశం->plan.get_relaxed ())
    release_shape_plan (దత్తాంశం, దత్తాంశం->plan);
  wasm_runtime_unload (దత్తాంశం->wasm_module);
  hb_blob_destroy (దత్తాంశం->wasm_blob);
  hb_free (దత్తాంశం);
}


/*
 * shaper font data
 */

నిర్మాణం hb_wasm_font_దత్తాంశం_t {};

hb_wasm_font_దత్తాంశం_t *
hb_wasm_shaper_font_దత్తాంశం_create (hb_font_t *font HB_వాడని)
{
  తిరిగి (hb_wasm_font_దత్తాంశం_t *) HB_SHAPER_దత్తాంశం_SUCCEEDED;
}

శూన్య
hb_wasm_shaper_font_దత్తాంశం_destroy (hb_wasm_font_దత్తాంశం_t *దత్తాంశం HB_వాడని)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_wasm_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_బఫరు_t        *బఫరు,
		స్థిరరాశి hb_feature_t *features,
		గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  ఒకవేళ (unlikely (బఫరు->లోపల_error ()))
    తిరిగి అబద్ధం;

  తార్కిక ret = నిజం;
  hb_face_t *face = font->face;
  స్థిరరాశి hb_wasm_face_దత్తాంశం_t *face_దత్తాంశం = face->దత్తాంశం.wasm;

  తార్కిక retried = అబద్ధం;
  ఒకవేళ (0)
  {
retry:
    DEBUG_సందేశం (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = శూన్యసూచిక;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_దత్తాంశం);
  ఒకవేళ (unlikely (!plan))
  {
    DEBUG_సందేశం (WASM, face_దత్తాంశం, "Acquiring shape-plan failed.");
    తిరిగి అబద్ధం;
  }

  స్వయంచాలక *module_inst = plan->module_inst;
  స్వయంచాలక *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (బఫరు);
  ఒకవేళ (unlikely (!fontref || !bufferref))
  {
    DEBUG_సందేశం (WASM, module_inst, "Failed to register objects.");
    వెళ్ళు fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  ఒకవేళ (unlikely (!func))
  {
    DEBUG_సందేశం (WASM, module_inst, "Shape function not found.");
    వెళ్ళు fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = అంకె_features ? wasm_runtime_module_dup_దత్తాంశం (module_inst,
							     (స్థిరరాశి అక్షరం *) features,
							     అంకె_features * పరిమాణం (features[0])) : 0;

  results[0].రకం = WASM_I32;
  arguments[0].రకం = WASM_I32;
  arguments[0].of.i32 = plan->wasm_shape_planptr;
  arguments[1].రకం = WASM_I32;
  arguments[1].of.i32 = fontref;
  arguments[2].రకం = WASM_I32;
  arguments[2].of.i32 = bufferref;
  arguments[3].రకం = WASM_I32;
  arguments[3].of.i32 = featuresptr;
  arguments[4].రకం = WASM_I32;
  arguments[4].of.i32 = అంకె_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_నిడివి (results), results,
				  ARRAY_నిడివి (arguments), arguments);

  ఒకవేళ (అంకె_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  ఒకవేళ (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_సందేశం (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    ఒకవేళ (!బఫరు->ensure_unicode ())
    {
      DEBUG_సందేశం (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      వెళ్ళు fail;
    }
    ఒకవేళ (retried)
    {
      DEBUG_సందేశం (WASM, font, "Giving up...");
      వెళ్ళు fail;
    }
    బఫరు->successful = నిజం;
    retried = నిజం;
    release_shape_plan (face_దత్తాంశం, plan);
    plan = శూన్యసూచిక;
    వెళ్ళు retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  ఒకవేళ (!results[0].of.i32)
  {
fail:
    ret = అబద్ధం;
  }

  release_shape_plan (face_దత్తాంశం, plan, ret);

  ఒకవేళ (ret)
  {
    బఫరు->clear_glyph_flags ();
    బఫరు->unsafe_గమ్యం_break ();
  }

  తిరిగి ret;
}

#అయితే_ముగింపు

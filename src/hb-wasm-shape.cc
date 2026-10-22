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

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DEBUG_WASM
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_WASM 1

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_WASM

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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api-list.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_NO_MODULES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_NO_MODULES
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_NO_MODULES
ਸਥਿਰ ਤਾਰਕਿਕ HB_ਅਣਵਰਤਿਆ
_hb_wasm_module_reader (ਅਚਲ ਅੱਖਰ *module_name,
		       uint8_t **p_ਬਫਰ, uint32_t *p_ਮਾਪ)
{
  ਅੱਖਰ ਰਸਤਾ[ਆਕਾਰ (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (ਰਸਤਾ, module_name, ਆਕਾਰ (ਰਸਤਾ) - ਆਕਾਰ (HB_WASM_MODULE_DIR) - 16);
  strncat (ਰਸਤਾ, ".wasm", 6);

  ਸਵੈਚਾਲਿਤ *blob = hb_blob_create_from_file (ਰਸਤਾ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ;
  ਸਵੈਚਾਲਿਤ *ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &ਲੰਮਾਈ);

  *p_ਬਫਰ = (uint8_t *) hb_malloc (ਲੰਮਾਈ);

  ਜੇ (ਲੰਮਾਈ && !p_ਬਫਰ)
    ਵਾਪਸ ਝੂਠ;

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (*p_ਬਫਰ, ਅੰਕੜੇ, ਲੰਮਾਈ);
  *p_ਮਾਪ = ਲੰਮਾਈ;

  hb_blob_destroy (blob);

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ HB_ਅਣਵਰਤਿਆ
_hb_wasm_module_destroyer (uint8_t *ਬਫਰ, uint32_t ਮਾਪ)
{
  hb_free (ਬਫਰ);
}
#ਜੇ_ਅੰਤ

/*
 * shaper face data
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_ਟੈਗ_WASM HB_ਟੈਗ('W','a','s','m')

ਬਣਤਰ hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(ਖਾਲੀ, wasm_shape_plan);
};

ਬਣਤਰ hb_wasm_face_ਅੰਕੜੇ_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  ਬਦਲਣਯੋਗ hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

ਸਥਿਰ ਤਾਰਕਿਕ
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

  ਸਥਿਰ ਤਾਰਕਿਕ initialized;
  ਜੇ (initialized)
    ਵਾਪਸ ਸੱਚ;

  Runtime_Init_ਦਲੀਲਾਂ init_ਦਲੀਲਾਂ;
  hb_memset (&init_ਦਲੀਲਾਂ, 0, ਆਕਾਰ (Runtime_Init_ਦਲੀਲਾਂ));

  init_ਦਲੀਲਾਂ.mem_alloc_type = Alloc_With_Allocator;
  init_ਦਲੀਲਾਂ.mem_alloc_option.allocator.malloc_func = (ਖਾਲੀ *) hb_malloc;
  init_ਦਲੀਲਾਂ.mem_alloc_option.allocator.realloc_func = (ਖਾਲੀ *) hb_realloc;
  init_ਦਲੀਲਾਂ.mem_alloc_option.allocator.free_func = (ਖਾਲੀ *) hb_free;

  // Native symbols need below registration phase
  init_ਦਲੀਲਾਂ.n_native_symbols = ARRAY_ਲੰਮਾਈ (_hb_wasm_native_symbols);
  init_ਦਲੀਲਾਂ.native_module_name = "env";
  init_ਦਲੀਲਾਂ.native_symbols = _hb_wasm_native_symbols;

  ਜੇ (unlikely (!wasm_runtime_ਭਰਿਆ_init (&init_ਦਲੀਲਾਂ)))
  {
    DEBUG_ਸੁਨੇਹਾ (WASM, ਖਾਲੀ_ਸੂਚਕ, "Init runtime environment failed.");
    ਵਾਪਸ ਝੂਠ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_NO_MODULES
  wasm_runtime_ਸੈੱਟ_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#ਜੇ_ਅੰਤ

  initialized = ਸੱਚ;
  ਵਾਪਸ ਸੱਚ;
}

hb_wasm_face_ਅੰਕੜੇ_t *
hb_wasm_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face)
{
  ਅੱਖਰ error[128];
  hb_wasm_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  wasm_module_t wasm_module = ਖਾਲੀ_ਸੂਚਕ;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_ਸਾਰਣੀ (face, HB_WASM_ਟੈਗ_WASM));

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = hb_blob_get_ਲੰਮਾਈ (wasm_blob);
  ਜੇ (!ਲੰਮਾਈ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (!_hb_wasm_init ())
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  wasm_module = wasm_runtime_ਲੱਦੋ ((uint8_t *) hb_blob_get_ਅੰਕੜੇ_writable (wasm_blob, ਖਾਲੀ_ਸੂਚਕ),
				   ਲੰਮਾਈ, error, ਆਕਾਰ (error));
  ਜੇ (unlikely (!wasm_module))
  {
    DEBUG_ਸੁਨੇਹਾ (WASM, ਖਾਲੀ_ਸੂਚਕ, "Load wasm module failed: %s", error);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਸਵੈਚਾਲਿਤ module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  ਅੰਕੜੇ = (hb_wasm_face_ਅੰਕੜੇ_t *) hb_calloc (1, ਆਕਾਰ (hb_wasm_face_ਅੰਕੜੇ_t));
  ਜੇ (unlikely (!ਅੰਕੜੇ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅੰਕੜੇ->wasm_blob = wasm_blob.release ();
  ਅੰਕੜੇ->wasm_module = wasm_module;

  module_guard.release ();
  ਵਾਪਸ ਅੰਕੜੇ;
}

ਸਥਿਰ hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    ਅਚਲ hb_wasm_face_ਅੰਕੜੇ_t *face_ਅੰਕੜੇ)
{
  ਅੱਖਰ error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_ਅੰਕੜੇ->plan.get_acquire ();
  ਜੇ (likely (plan && face_ਅੰਕੜੇ->plan.cmpexch (plan, ਖਾਲੀ_ਸੂਚਕ)))
    ਵਾਪਸ plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, ਆਕਾਰ (hb_wasm_shape_plan_t));
  ਸਵੈਚਾਲਿਤ plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t ਢੇਰ_ਮਾਪ = 32 * 1024, ਢੇਰ_ਮਾਪ_2 = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_ਅੰਕੜੇ->wasm_module, ਢੇਰ_ਮਾਪ, ਢੇਰ_ਮਾਪ_2, error, ਆਕਾਰ (error));
  ਜੇ (unlikely (!module_inst))
  {
    DEBUG_ਸੁਨੇਹਾ (WASM, face_ਅੰਕੜੇ, "Create wasm module instance failed: %s", error);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਸਵੈਚਾਲਿਤ module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, ਢੇਰ_ਮਾਪ);
  ਜੇ (unlikely (!exec_env)) {
    DEBUG_ਸੁਨੇਹਾ (WASM, face_ਅੰਕੜੇ, "Create wasm execution environment failed.");
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਸਵੈਚਾਲਿਤ exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  ਜੇ (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    ਜੇ (unlikely (!faceref))
    {
      DEBUG_ਸੁਨੇਹਾ (WASM, face_ਅੰਕੜੇ, "Failed to register face object.");
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }

    results[0].ਕਿਸਮ = WASM_I32;
    arguments[0].ਕਿਸਮ = WASM_I32;
    arguments[0].of.i32 = faceref;
    ਤਾਰਕਿਕ ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_ਲੰਮਾਈ (results), results,
					 ARRAY_ਲੰਮਾਈ (arguments), arguments);

    ਜੇ (unlikely (!ret))
    {
      DEBUG_ਸੁਨੇਹਾ (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  ਵਾਪਸ plan;
}

ਸਥਿਰ ਖਾਲੀ
release_shape_plan (ਅਚਲ hb_wasm_face_ਅੰਕੜੇ_t *face_ਅੰਕੜੇ,
		    hb_wasm_shape_plan_t *plan,
		    ਤਾਰਕਿਕ cache = ਝੂਠ)
{
  ਜੇ (cache && face_ਅੰਕੜੇ->plan.cmpexch (ਖਾਲੀ_ਸੂਚਕ, plan))
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ *module_inst = plan->module_inst;
  ਸਵੈਚਾਲਿਤ *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  ਜੇ (plan->wasm_shape_planptr)
  {

    ਸਵੈਚਾਲਿਤ *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    ਜੇ (func)
    {
      wasm_val_t arguments[1];

      arguments[0].ਕਿਸਮ = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      ਤਾਰਕਿਕ ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, ਖਾਲੀ_ਸੂਚਕ,
					   ARRAY_ਲੰਮਾਈ (arguments), arguments);

      ਜੇ (unlikely (!ret))
      {
	DEBUG_ਸੁਨੇਹਾ (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

ਖਾਲੀ
hb_wasm_shaper_face_ਅੰਕੜੇ_destroy (hb_wasm_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  ਜੇ (ਅੰਕੜੇ->plan.get_relaxed ())
    release_shape_plan (ਅੰਕੜੇ, ਅੰਕੜੇ->plan);
  wasm_runtime_unload (ਅੰਕੜੇ->wasm_module);
  hb_blob_destroy (ਅੰਕੜੇ->wasm_blob);
  hb_free (ਅੰਕੜੇ);
}


/*
 * shaper font data
 */

ਬਣਤਰ hb_wasm_font_ਅੰਕੜੇ_t {};

hb_wasm_font_ਅੰਕੜੇ_t *
hb_wasm_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ (hb_wasm_font_ਅੰਕੜੇ_t *) HB_SHAPER_ਅੰਕੜੇ_SUCCEEDED;
}

ਖਾਲੀ
hb_wasm_shaper_font_ਅੰਕੜੇ_destroy (hb_wasm_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_wasm_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_ਬਫਰ_t        *ਬਫਰ,
		ਅਚਲ hb_feature_t *features,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  ਜੇ (unlikely (ਬਫਰ->ਵਿੱਚ_error ()))
    ਵਾਪਸ ਝੂਠ;

  ਤਾਰਕਿਕ ret = ਸੱਚ;
  hb_face_t *face = font->face;
  ਅਚਲ hb_wasm_face_ਅੰਕੜੇ_t *face_ਅੰਕੜੇ = face->ਅੰਕੜੇ.wasm;

  ਤਾਰਕਿਕ retried = ਝੂਠ;
  ਜੇ (0)
  {
retry:
    DEBUG_ਸੁਨੇਹਾ (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = ਖਾਲੀ_ਸੂਚਕ;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_ਅੰਕੜੇ);
  ਜੇ (unlikely (!plan))
  {
    DEBUG_ਸੁਨੇਹਾ (WASM, face_ਅੰਕੜੇ, "Acquiring shape-plan failed.");
    ਵਾਪਸ ਝੂਠ;
  }

  ਸਵੈਚਾਲਿਤ *module_inst = plan->module_inst;
  ਸਵੈਚਾਲਿਤ *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (ਬਫਰ);
  ਜੇ (unlikely (!fontref || !bufferref))
  {
    DEBUG_ਸੁਨੇਹਾ (WASM, module_inst, "Failed to register objects.");
    ਜਾਓ fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  ਜੇ (unlikely (!func))
  {
    DEBUG_ਸੁਨੇਹਾ (WASM, module_inst, "Shape function not found.");
    ਜਾਓ fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = ਅੰਕ_features ? wasm_runtime_module_dup_ਅੰਕੜੇ (module_inst,
							     (ਅਚਲ ਅੱਖਰ *) features,
							     ਅੰਕ_features * ਆਕਾਰ (features[0])) : 0;

  results[0].ਕਿਸਮ = WASM_I32;
  arguments[0].ਕਿਸਮ = WASM_I32;
  arguments[0].of.i32 = plan->wasm_shape_planptr;
  arguments[1].ਕਿਸਮ = WASM_I32;
  arguments[1].of.i32 = fontref;
  arguments[2].ਕਿਸਮ = WASM_I32;
  arguments[2].of.i32 = bufferref;
  arguments[3].ਕਿਸਮ = WASM_I32;
  arguments[3].of.i32 = featuresptr;
  arguments[4].ਕਿਸਮ = WASM_I32;
  arguments[4].of.i32 = ਅੰਕ_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_ਲੰਮਾਈ (results), results,
				  ARRAY_ਲੰਮਾਈ (arguments), arguments);

  ਜੇ (ਅੰਕ_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  ਜੇ (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_ਸੁਨੇਹਾ (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    ਜੇ (!ਬਫਰ->ensure_unicode ())
    {
      DEBUG_ਸੁਨੇਹਾ (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      ਜਾਓ fail;
    }
    ਜੇ (retried)
    {
      DEBUG_ਸੁਨੇਹਾ (WASM, font, "Giving up...");
      ਜਾਓ fail;
    }
    ਬਫਰ->successful = ਸੱਚ;
    retried = ਸੱਚ;
    release_shape_plan (face_ਅੰਕੜੇ, plan);
    plan = ਖਾਲੀ_ਸੂਚਕ;
    ਜਾਓ retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  ਜੇ (!results[0].of.i32)
  {
fail:
    ret = ਝੂਠ;
  }

  release_shape_plan (face_ਅੰਕੜੇ, plan, ret);

  ਜੇ (ret)
  {
    ਬਫਰ->clear_glyph_flags ();
    ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break ();
  }

  ਵਾਪਸ ret;
}

#ਜੇ_ਅੰਤ

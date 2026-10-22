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

#تعریف_لغوه HB_DEBUG_WASM
#تعریف_کړه HB_DEBUG_WASM 1

#ضمن "hb-shaper-impl.hh"

#که_تعریف_شوی HAVE_WASM

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

#ضمن "hb-wasm-api.hh"
#ضمن "hb-wasm-api-list.hh"

#که_نه_تعریف_شوی HB_WASM_NO_MODULES
#تعریف_کړه HB_WASM_NO_MODULES
#د_که_پای


#که_نه_تعریف_شوی HB_WASM_NO_MODULES
ساكن منطقي HB_ناکارول_شوی
_hb_wasm_module_reader (ثابت حرف *module_name,
		       uint8_t **p_بفر, uint32_t *p_اندازه)
{
  حرف لار[حجم (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (لار, module_name, حجم (لار) - حجم (HB_WASM_MODULE_DIR) - 16);
  strncat (لار, ".wasm", 6);

  خپلکاری *blob = hb_blob_create_from_file (لار);

  بینښه اوږدتیا;
  خپلکاری *معلومات = hb_blob_get_معلومات (blob, &اوږدتیا);

  *p_بفر = (uint8_t *) hb_malloc (اوږدتیا);

  إذا (اوږدتیا && !p_بفر)
    أرجع خطأ;

  حافظه_نقل (*p_بفر, معلومات, اوږدتیا);
  *p_اندازه = اوږدتیا;

  hb_blob_destroy (blob);

  أرجع ریښتیا;
}

ساكن فراغ HB_ناکارول_شوی
_hb_wasm_module_destroyer (uint8_t *بفر, uint32_t اندازه)
{
  hb_free (بفر);
}
#د_که_پای

/*
 * shaper face data
 */

#تعریف_کړه HB_WASM_نښان_WASM HB_نښان('W','a','s','m')

بنية hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(فراغ, wasm_shape_plan);
};

بنية hb_wasm_face_معلومات_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  بدلیدونکی hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

ساكن منطقي
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

  ساكن منطقي initialized;
  إذا (initialized)
    أرجع ریښتیا;

  Runtime_Init_دلیلونه init_دلیلونه;
  hb_memset (&init_دلیلونه, 0, حجم (Runtime_Init_دلیلونه));

  init_دلیلونه.mem_alloc_type = Alloc_With_Allocator;
  init_دلیلونه.mem_alloc_option.allocator.malloc_func = (فراغ *) hb_malloc;
  init_دلیلونه.mem_alloc_option.allocator.realloc_func = (فراغ *) hb_realloc;
  init_دلیلونه.mem_alloc_option.allocator.free_func = (فراغ *) hb_free;

  // Native symbols need below registration phase
  init_دلیلونه.n_native_symbols = ARRAY_اوږدتیا (_hb_wasm_native_symbols);
  init_دلیلونه.native_module_name = "env";
  init_دلیلونه.native_symbols = _hb_wasm_native_symbols;

  إذا (unlikely (!wasm_runtime_ډک_init (&init_دلیلونه)))
  {
    DEBUG_پیغام (WASM, تش_اشاره, "Init runtime environment failed.");
    أرجع خطأ;
  }

#که_نه_تعریف_شوی HB_WASM_NO_MODULES
  wasm_runtime_سېټ_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#د_که_پای

  initialized = ریښتیا;
  أرجع ریښتیا;
}

hb_wasm_face_معلومات_t *
hb_wasm_shaper_face_معلومات_create (hb_face_t *face)
{
  حرف error[128];
  hb_wasm_face_معلومات_t *معلومات = تش_اشاره;
  wasm_module_t wasm_module = تش_اشاره;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_جدول (face, HB_WASM_نښان_WASM));

  بینښه اوږدتیا = hb_blob_get_اوږدتیا (wasm_blob);
  إذا (!اوږدتیا)
    أرجع تش_اشاره;

  إذا (!_hb_wasm_init ())
    أرجع تش_اشاره;

  wasm_module = wasm_runtime_بارول ((uint8_t *) hb_blob_get_معلومات_writable (wasm_blob, تش_اشاره),
				   اوږدتیا, error, حجم (error));
  إذا (unlikely (!wasm_module))
  {
    DEBUG_پیغام (WASM, تش_اشاره, "Load wasm module failed: %s", error);
    أرجع تش_اشاره;
  }
  خپلکاری module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  معلومات = (hb_wasm_face_معلومات_t *) hb_calloc (1, حجم (hb_wasm_face_معلومات_t));
  إذا (unlikely (!معلومات))
    أرجع تش_اشاره;

  معلومات->wasm_blob = wasm_blob.release ();
  معلومات->wasm_module = wasm_module;

  module_guard.release ();
  أرجع معلومات;
}

ساكن hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    ثابت hb_wasm_face_معلومات_t *face_معلومات)
{
  حرف error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_معلومات->plan.get_acquire ();
  إذا (likely (plan && face_معلومات->plan.cmpexch (plan, تش_اشاره)))
    أرجع plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, حجم (hb_wasm_shape_plan_t));
  خپلکاری plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t ډېری_اندازه = 32 * 1024, ډېرۍ_اندازه = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_معلومات->wasm_module, ډېری_اندازه, ډېرۍ_اندازه, error, حجم (error));
  إذا (unlikely (!module_inst))
  {
    DEBUG_پیغام (WASM, face_معلومات, "Create wasm module instance failed: %s", error);
    أرجع تش_اشاره;
  }
  خپلکاری module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, ډېری_اندازه);
  إذا (unlikely (!exec_env)) {
    DEBUG_پیغام (WASM, face_معلومات, "Create wasm execution environment failed.");
    أرجع تش_اشاره;
  }
  خپلکاری exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  إذا (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    إذا (unlikely (!faceref))
    {
      DEBUG_پیغام (WASM, face_معلومات, "Failed to register face object.");
      أرجع تش_اشاره;
    }

    results[0].ډول = WASM_I32;
    arguments[0].ډول = WASM_I32;
    arguments[0].of.i32 = faceref;
    منطقي ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_اوږدتیا (results), results,
					 ARRAY_اوږدتیا (arguments), arguments);

    إذا (unlikely (!ret))
    {
      DEBUG_پیغام (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      أرجع تش_اشاره;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  أرجع plan;
}

ساكن فراغ
release_shape_plan (ثابت hb_wasm_face_معلومات_t *face_معلومات,
		    hb_wasm_shape_plan_t *plan,
		    منطقي cache = خطأ)
{
  إذا (cache && face_معلومات->plan.cmpexch (تش_اشاره, plan))
    أرجع;

  خپلکاری *module_inst = plan->module_inst;
  خپلکاری *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  إذا (plan->wasm_shape_planptr)
  {

    خپلکاری *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    إذا (func)
    {
      wasm_val_t arguments[1];

      arguments[0].ډول = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      منطقي ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, تش_اشاره,
					   ARRAY_اوږدتیا (arguments), arguments);

      إذا (unlikely (!ret))
      {
	DEBUG_پیغام (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

فراغ
hb_wasm_shaper_face_معلومات_destroy (hb_wasm_face_معلومات_t *معلومات)
{
  إذا (معلومات->plan.get_relaxed ())
    release_shape_plan (معلومات, معلومات->plan);
  wasm_runtime_unload (معلومات->wasm_module);
  hb_blob_destroy (معلومات->wasm_blob);
  hb_free (معلومات);
}


/*
 * shaper font data
 */

بنية hb_wasm_font_معلومات_t {};

hb_wasm_font_معلومات_t *
hb_wasm_shaper_font_معلومات_create (hb_font_t *font HB_ناکارول_شوی)
{
  أرجع (hb_wasm_font_معلومات_t *) HB_SHAPER_معلومات_SUCCEEDED;
}

فراغ
hb_wasm_shaper_font_معلومات_destroy (hb_wasm_font_معلومات_t *معلومات HB_ناکارول_شوی)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_wasm_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_بفر_t        *بفر,
		ثابت hb_feature_t *features,
		بینښه صحيح        عدد_features)
{
  إذا (unlikely (بفر->دننه_error ()))
    أرجع خطأ;

  منطقي ret = ریښتیا;
  hb_face_t *face = font->face;
  ثابت hb_wasm_face_معلومات_t *face_معلومات = face->معلومات.wasm;

  منطقي retried = خطأ;
  إذا (0)
  {
retry:
    DEBUG_پیغام (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = تش_اشاره;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_معلومات);
  إذا (unlikely (!plan))
  {
    DEBUG_پیغام (WASM, face_معلومات, "Acquiring shape-plan failed.");
    أرجع خطأ;
  }

  خپلکاری *module_inst = plan->module_inst;
  خپلکاری *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (بفر);
  إذا (unlikely (!fontref || !bufferref))
  {
    DEBUG_پیغام (WASM, module_inst, "Failed to register objects.");
    اذهب fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  إذا (unlikely (!func))
  {
    DEBUG_پیغام (WASM, module_inst, "Shape function not found.");
    اذهب fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = عدد_features ? wasm_runtime_module_dup_معلومات (module_inst,
							     (ثابت حرف *) features,
							     عدد_features * حجم (features[0])) : 0;

  results[0].ډول = WASM_I32;
  arguments[0].ډول = WASM_I32;
  arguments[0].of.i32 = plan->wasm_shape_planptr;
  arguments[1].ډول = WASM_I32;
  arguments[1].of.i32 = fontref;
  arguments[2].ډول = WASM_I32;
  arguments[2].of.i32 = bufferref;
  arguments[3].ډول = WASM_I32;
  arguments[3].of.i32 = featuresptr;
  arguments[4].ډول = WASM_I32;
  arguments[4].of.i32 = عدد_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_اوږدتیا (results), results,
				  ARRAY_اوږدتیا (arguments), arguments);

  إذا (عدد_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  إذا (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_پیغام (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    إذا (!بفر->ensure_unicode ())
    {
      DEBUG_پیغام (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      اذهب fail;
    }
    إذا (retried)
    {
      DEBUG_پیغام (WASM, font, "Giving up...");
      اذهب fail;
    }
    بفر->successful = ریښتیا;
    retried = ریښتیا;
    release_shape_plan (face_معلومات, plan);
    plan = تش_اشاره;
    اذهب retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  إذا (!results[0].of.i32)
  {
fail:
    ret = خطأ;
  }

  release_shape_plan (face_معلومات, plan, ret);

  إذا (ret)
  {
    بفر->clear_glyph_flags ();
    بفر->unsafe_منزل_break ();
  }

  أرجع ret;
}

#د_که_پای

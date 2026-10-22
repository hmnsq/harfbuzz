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

#لغو_تعریف HB_DEBUG_WASM
#تعریف_کن HB_DEBUG_WASM 1

#ضمن "hb-shaper-impl.hh"

#اگر_تعریف_شده HAVE_WASM

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

#اگر_تعریف_نشده HB_WASM_NO_MODULES
#تعریف_کن HB_WASM_NO_MODULES
#پایان_اگر


#اگر_تعریف_نشده HB_WASM_NO_MODULES
ساكن منطقي HB_استفادهنشده
_hb_wasm_module_reader (ثابت حرف *module_name,
		       uint8_t **p_میانگیر, uint32_t *p_اندازه)
{
  حرف مسیر[حجم (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (مسیر, module_name, حجم (مسیر) - حجم (HB_WASM_MODULE_DIR) - 16);
  strncat (مسیر, ".wasm", 6);

  خودکار *blob = hb_blob_create_from_file (مسیر);

  بیعلامت درازی;
  خودکار *داده = hb_blob_get_داده (blob, &درازی);

  *p_میانگیر = (uint8_t *) hb_malloc (درازی);

  إذا (درازی && !p_میانگیر)
    أرجع خطأ;

  کپی_حافظه (*p_میانگیر, داده, درازی);
  *p_اندازه = درازی;

  hb_blob_destroy (blob);

  أرجع درست;
}

ساكن فراغ HB_استفادهنشده
_hb_wasm_module_destroyer (uint8_t *میانگیر, uint32_t اندازه)
{
  hb_free (میانگیر);
}
#پایان_اگر

/*
 * shaper face data
 */

#تعریف_کن HB_WASM_برچسب_WASM HB_برچسب('W','a','s','m')

بنية hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(فراغ, wasm_shape_plan);
};

بنية hb_wasm_face_داده_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  تغییرپذیر hb_atomic_t<hb_wasm_shape_plan_t *> plan;
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
    أرجع درست;

  Runtime_Init_آرگومانها init_آرگومانها;
  hb_memset (&init_آرگومانها, 0, حجم (Runtime_Init_آرگومانها));

  init_آرگومانها.mem_alloc_type = Alloc_With_Allocator;
  init_آرگومانها.mem_alloc_option.allocator.malloc_func = (فراغ *) hb_malloc;
  init_آرگومانها.mem_alloc_option.allocator.realloc_func = (فراغ *) hb_realloc;
  init_آرگومانها.mem_alloc_option.allocator.free_func = (فراغ *) hb_free;

  // Native symbols need below registration phase
  init_آرگومانها.n_native_symbols = ARRAY_درازی (_hb_wasm_native_symbols);
  init_آرگومانها.native_module_name = "env";
  init_آرگومانها.native_symbols = _hb_wasm_native_symbols;

  إذا (unlikely (!wasm_runtime_پر_init (&init_آرگومانها)))
  {
    DEBUG_پیام (WASM, اشاره_تهی, "Init runtime environment failed.");
    أرجع خطأ;
  }

#اگر_تعریف_نشده HB_WASM_NO_MODULES
  wasm_runtime_مجموعه_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#پایان_اگر

  initialized = درست;
  أرجع درست;
}

hb_wasm_face_داده_t *
hb_wasm_shaper_face_داده_create (hb_face_t *face)
{
  حرف error[128];
  hb_wasm_face_داده_t *داده = اشاره_تهی;
  wasm_module_t wasm_module = اشاره_تهی;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_جدول (face, HB_WASM_برچسب_WASM));

  بیعلامت درازی = hb_blob_get_درازی (wasm_blob);
  إذا (!درازی)
    أرجع اشاره_تهی;

  إذا (!_hb_wasm_init ())
    أرجع اشاره_تهی;

  wasm_module = wasm_runtime_بارگیری ((uint8_t *) hb_blob_get_داده_writable (wasm_blob, اشاره_تهی),
				   درازی, error, حجم (error));
  إذا (unlikely (!wasm_module))
  {
    DEBUG_پیام (WASM, اشاره_تهی, "Load wasm module failed: %s", error);
    أرجع اشاره_تهی;
  }
  خودکار module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  داده = (hb_wasm_face_داده_t *) hb_calloc (1, حجم (hb_wasm_face_داده_t));
  إذا (unlikely (!داده))
    أرجع اشاره_تهی;

  داده->wasm_blob = wasm_blob.release ();
  داده->wasm_module = wasm_module;

  module_guard.release ();
  أرجع داده;
}

ساكن hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    ثابت hb_wasm_face_داده_t *face_داده)
{
  حرف error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_داده->plan.get_acquire ();
  إذا (likely (plan && face_داده->plan.cmpexch (plan, اشاره_تهی)))
    أرجع plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, حجم (hb_wasm_shape_plan_t));
  خودکار plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t پشته_اندازه = 32 * 1024, انباشته_اندازه = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_داده->wasm_module, پشته_اندازه, انباشته_اندازه, error, حجم (error));
  إذا (unlikely (!module_inst))
  {
    DEBUG_پیام (WASM, face_داده, "Create wasm module instance failed: %s", error);
    أرجع اشاره_تهی;
  }
  خودکار module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, پشته_اندازه);
  إذا (unlikely (!exec_env)) {
    DEBUG_پیام (WASM, face_داده, "Create wasm execution environment failed.");
    أرجع اشاره_تهی;
  }
  خودکار exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  إذا (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    إذا (unlikely (!faceref))
    {
      DEBUG_پیام (WASM, face_داده, "Failed to register face object.");
      أرجع اشاره_تهی;
    }

    results[0].نوع = WASM_I32;
    arguments[0].نوع = WASM_I32;
    arguments[0].of.i32 = faceref;
    منطقي ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_درازی (results), results,
					 ARRAY_درازی (arguments), arguments);

    إذا (unlikely (!ret))
    {
      DEBUG_پیام (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      أرجع اشاره_تهی;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  أرجع plan;
}

ساكن فراغ
release_shape_plan (ثابت hb_wasm_face_داده_t *face_داده,
		    hb_wasm_shape_plan_t *plan,
		    منطقي cache = خطأ)
{
  إذا (cache && face_داده->plan.cmpexch (اشاره_تهی, plan))
    أرجع;

  خودکار *module_inst = plan->module_inst;
  خودکار *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  إذا (plan->wasm_shape_planptr)
  {

    خودکار *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    إذا (func)
    {
      wasm_val_t arguments[1];

      arguments[0].نوع = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      منطقي ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, اشاره_تهی,
					   ARRAY_درازی (arguments), arguments);

      إذا (unlikely (!ret))
      {
	DEBUG_پیام (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

فراغ
hb_wasm_shaper_face_داده_destroy (hb_wasm_face_داده_t *داده)
{
  إذا (داده->plan.get_relaxed ())
    release_shape_plan (داده, داده->plan);
  wasm_runtime_unload (داده->wasm_module);
  hb_blob_destroy (داده->wasm_blob);
  hb_free (داده);
}


/*
 * shaper font data
 */

بنية hb_wasm_font_داده_t {};

hb_wasm_font_داده_t *
hb_wasm_shaper_font_داده_create (hb_font_t *font HB_استفادهنشده)
{
  أرجع (hb_wasm_font_داده_t *) HB_SHAPER_داده_SUCCEEDED;
}

فراغ
hb_wasm_shaper_font_داده_destroy (hb_wasm_font_داده_t *داده HB_استفادهنشده)
{
}


/*
 * shaper
 */

hb_bool_t
_hb_wasm_shape (hb_shape_plan_t    *shape_plan,
		hb_font_t          *font,
		hb_میانگیر_t        *میانگیر,
		ثابت hb_feature_t *features,
		بیعلامت صحيح        رقم_features)
{
  إذا (unlikely (میانگیر->درون_error ()))
    أرجع خطأ;

  منطقي ret = درست;
  hb_face_t *face = font->face;
  ثابت hb_wasm_face_داده_t *face_داده = face->داده.wasm;

  منطقي retried = خطأ;
  إذا (0)
  {
retry:
    DEBUG_پیام (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = اشاره_تهی;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_داده);
  إذا (unlikely (!plan))
  {
    DEBUG_پیام (WASM, face_داده, "Acquiring shape-plan failed.");
    أرجع خطأ;
  }

  خودکار *module_inst = plan->module_inst;
  خودکار *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (میانگیر);
  إذا (unlikely (!fontref || !bufferref))
  {
    DEBUG_پیام (WASM, module_inst, "Failed to register objects.");
    اذهب fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  إذا (unlikely (!func))
  {
    DEBUG_پیام (WASM, module_inst, "Shape function not found.");
    اذهب fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = رقم_features ? wasm_runtime_module_dup_داده (module_inst,
							     (ثابت حرف *) features,
							     رقم_features * حجم (features[0])) : 0;

  results[0].نوع = WASM_I32;
  arguments[0].نوع = WASM_I32;
  arguments[0].of.i32 = plan->wasm_shape_planptr;
  arguments[1].نوع = WASM_I32;
  arguments[1].of.i32 = fontref;
  arguments[2].نوع = WASM_I32;
  arguments[2].of.i32 = bufferref;
  arguments[3].نوع = WASM_I32;
  arguments[3].of.i32 = featuresptr;
  arguments[4].نوع = WASM_I32;
  arguments[4].of.i32 = رقم_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_درازی (results), results,
				  ARRAY_درازی (arguments), arguments);

  إذا (رقم_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  إذا (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_پیام (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    إذا (!میانگیر->ensure_unicode ())
    {
      DEBUG_پیام (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      اذهب fail;
    }
    إذا (retried)
    {
      DEBUG_پیام (WASM, font, "Giving up...");
      اذهب fail;
    }
    میانگیر->successful = درست;
    retried = درست;
    release_shape_plan (face_داده, plan);
    plan = اشاره_تهی;
    اذهب retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  إذا (!results[0].of.i32)
  {
fail:
    ret = خطأ;
  }

  release_shape_plan (face_داده, plan, ret);

  إذا (ret)
  {
    میانگیر->clear_glyph_flags ();
    میانگیر->unsafe_مقصد_break ();
  }

  أرجع ret;
}

#پایان_اگر

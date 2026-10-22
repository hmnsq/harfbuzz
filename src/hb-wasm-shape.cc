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

#تعریف_منسوخ HB_DEBUG_WASM
#تعریف_کرو HB_DEBUG_WASM 1

#شامل "hb-shaper-impl.hh"

#اگر_تعریف_شدہ HAVE_WASM

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

#شامل "hb-wasm-api.hh"
#شامل "hb-wasm-api-list.hh"

#اگر_غیر_تعریف_شدہ HB_WASM_NO_MODULES
#تعریف_کرو HB_WASM_NO_MODULES
#اگر_ختم


#اگر_غیر_تعریف_شدہ HB_WASM_NO_MODULES
ساکن منطقی HB_غیر_مستعمل
_hb_wasm_module_reader (ثابت حرف *module_name,
		       uint8_t **p_بفر, uint32_t *p_سائز)
{
  حرف راستہ[حجم (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (راستہ, module_name, حجم (راستہ) - حجم (HB_WASM_MODULE_DIR) - 16);
  strncat (راستہ, ".wasm", 6);

  خودکار *blob = hb_blob_create_from_file (راستہ);

  غیرعلامتی لمبائی;
  خودکار *ڈیٹا = hb_blob_get_ڈیٹا (blob, &لمبائی);

  *p_بفر = (uint8_t *) hb_malloc (لمبائی);

  اگر (لمبائی && !p_بفر)
    واپس جھوٹ;

  حافظہ_نقل (*p_بفر, ڈیٹا, لمبائی);
  *p_سائز = لمبائی;

  hb_blob_destroy (blob);

  واپس سچ;
}

ساکن خالی HB_غیر_مستعمل
_hb_wasm_module_destroyer (uint8_t *بفر, uint32_t سائز)
{
  hb_free (بفر);
}
#اگر_ختم

/*
 * shaper face data
 */

#تعریف_کرو HB_WASM_لیبل_WASM HB_لیبل('W','a','s','m')

ساخت hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(خالی, wasm_shape_plan);
};

ساخت hb_wasm_face_ڈیٹا_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  تغیرپذیر hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

ساکن منطقی
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

  ساکن منطقی initialized;
  اگر (initialized)
    واپس سچ;

  Runtime_Init_دلائل init_دلائل;
  hb_memset (&init_دلائل, 0, حجم (Runtime_Init_دلائل));

  init_دلائل.mem_alloc_type = Alloc_With_Allocator;
  init_دلائل.mem_alloc_option.allocator.malloc_func = (خالی *) hb_malloc;
  init_دلائل.mem_alloc_option.allocator.realloc_func = (خالی *) hb_realloc;
  init_دلائل.mem_alloc_option.allocator.free_func = (خالی *) hb_free;

  // Native symbols need below registration phase
  init_دلائل.n_native_symbols = ARRAY_لمبائی (_hb_wasm_native_symbols);
  init_دلائل.native_module_name = "env";
  init_دلائل.native_symbols = _hb_wasm_native_symbols;

  اگر (unlikely (!wasm_runtime_بھرا_init (&init_دلائل)))
  {
    DEBUG_پیغام (WASM, خالی_اشاریہ, "Init runtime environment failed.");
    واپس جھوٹ;
  }

#اگر_غیر_تعریف_شدہ HB_WASM_NO_MODULES
  wasm_runtime_سیٹ_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#اگر_ختم

  initialized = سچ;
  واپس سچ;
}

hb_wasm_face_ڈیٹا_t *
hb_wasm_shaper_face_ڈیٹا_create (hb_face_t *face)
{
  حرف error[128];
  hb_wasm_face_ڈیٹا_t *ڈیٹا = خالی_اشاریہ;
  wasm_module_t wasm_module = خالی_اشاریہ;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_جدول (face, HB_WASM_لیبل_WASM));

  غیرعلامتی لمبائی = hb_blob_get_لمبائی (wasm_blob);
  اگر (!لمبائی)
    واپس خالی_اشاریہ;

  اگر (!_hb_wasm_init ())
    واپس خالی_اشاریہ;

  wasm_module = wasm_runtime_بھرو ((uint8_t *) hb_blob_get_ڈیٹا_writable (wasm_blob, خالی_اشاریہ),
				   لمبائی, error, حجم (error));
  اگر (unlikely (!wasm_module))
  {
    DEBUG_پیغام (WASM, خالی_اشاریہ, "Load wasm module failed: %s", error);
    واپس خالی_اشاریہ;
  }
  خودکار module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  ڈیٹا = (hb_wasm_face_ڈیٹا_t *) hb_calloc (1, حجم (hb_wasm_face_ڈیٹا_t));
  اگر (unlikely (!ڈیٹا))
    واپس خالی_اشاریہ;

  ڈیٹا->wasm_blob = wasm_blob.release ();
  ڈیٹا->wasm_module = wasm_module;

  module_guard.release ();
  واپس ڈیٹا;
}

ساکن hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    ثابت hb_wasm_face_ڈیٹا_t *face_ڈیٹا)
{
  حرف error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_ڈیٹا->plan.get_acquire ();
  اگر (likely (plan && face_ڈیٹا->plan.cmpexch (plan, خالی_اشاریہ)))
    واپس plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, حجم (hb_wasm_shape_plan_t));
  خودکار plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t ڈھیر_سائز = 32 * 1024, ڈھیر_سائز_2 = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_ڈیٹا->wasm_module, ڈھیر_سائز, ڈھیر_سائز_2, error, حجم (error));
  اگر (unlikely (!module_inst))
  {
    DEBUG_پیغام (WASM, face_ڈیٹا, "Create wasm module instance failed: %s", error);
    واپس خالی_اشاریہ;
  }
  خودکار module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, ڈھیر_سائز);
  اگر (unlikely (!exec_env)) {
    DEBUG_پیغام (WASM, face_ڈیٹا, "Create wasm execution environment failed.");
    واپس خالی_اشاریہ;
  }
  خودکار exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  اگر (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    اگر (unlikely (!faceref))
    {
      DEBUG_پیغام (WASM, face_ڈیٹا, "Failed to register face object.");
      واپس خالی_اشاریہ;
    }

    results[0].قسم = WASM_I32;
    arguments[0].قسم = WASM_I32;
    arguments[0].of.i32 = faceref;
    منطقی ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_لمبائی (results), results,
					 ARRAY_لمبائی (arguments), arguments);

    اگر (unlikely (!ret))
    {
      DEBUG_پیغام (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      واپس خالی_اشاریہ;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  واپس plan;
}

ساکن خالی
release_shape_plan (ثابت hb_wasm_face_ڈیٹا_t *face_ڈیٹا,
		    hb_wasm_shape_plan_t *plan,
		    منطقی cache = جھوٹ)
{
  اگر (cache && face_ڈیٹا->plan.cmpexch (خالی_اشاریہ, plan))
    واپس;

  خودکار *module_inst = plan->module_inst;
  خودکار *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  اگر (plan->wasm_shape_planptr)
  {

    خودکار *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    اگر (func)
    {
      wasm_val_t arguments[1];

      arguments[0].قسم = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      منطقی ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, خالی_اشاریہ,
					   ARRAY_لمبائی (arguments), arguments);

      اگر (unlikely (!ret))
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

خالی
hb_wasm_shaper_face_ڈیٹا_destroy (hb_wasm_face_ڈیٹا_t *ڈیٹا)
{
  اگر (ڈیٹا->plan.get_relaxed ())
    release_shape_plan (ڈیٹا, ڈیٹا->plan);
  wasm_runtime_unload (ڈیٹا->wasm_module);
  hb_blob_destroy (ڈیٹا->wasm_blob);
  hb_free (ڈیٹا);
}


/*
 * shaper font data
 */

ساخت hb_wasm_font_ڈیٹا_t {};

hb_wasm_font_ڈیٹا_t *
hb_wasm_shaper_font_ڈیٹا_create (hb_font_t *font HB_غیر_مستعمل)
{
  واپس (hb_wasm_font_ڈیٹا_t *) HB_SHAPER_ڈیٹا_SUCCEEDED;
}

خالی
hb_wasm_shaper_font_ڈیٹا_destroy (hb_wasm_font_ڈیٹا_t *ڈیٹا HB_غیر_مستعمل)
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
		غیرعلامتی صحیح        عدد_features)
{
  اگر (unlikely (بفر->اندر_error ()))
    واپس جھوٹ;

  منطقی ret = سچ;
  hb_face_t *face = font->face;
  ثابت hb_wasm_face_ڈیٹا_t *face_ڈیٹا = face->ڈیٹا.wasm;

  منطقی retried = جھوٹ;
  اگر (0)
  {
retry:
    DEBUG_پیغام (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = خالی_اشاریہ;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_ڈیٹا);
  اگر (unlikely (!plan))
  {
    DEBUG_پیغام (WASM, face_ڈیٹا, "Acquiring shape-plan failed.");
    واپس جھوٹ;
  }

  خودکار *module_inst = plan->module_inst;
  خودکار *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (بفر);
  اگر (unlikely (!fontref || !bufferref))
  {
    DEBUG_پیغام (WASM, module_inst, "Failed to register objects.");
    جاؤ fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  اگر (unlikely (!func))
  {
    DEBUG_پیغام (WASM, module_inst, "Shape function not found.");
    جاؤ fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = عدد_features ? wasm_runtime_module_dup_ڈیٹا (module_inst,
							     (ثابت حرف *) features,
							     عدد_features * حجم (features[0])) : 0;

  results[0].قسم = WASM_I32;
  arguments[0].قسم = WASM_I32;
  arguments[0].of.i32 = plan->wasm_shape_planptr;
  arguments[1].قسم = WASM_I32;
  arguments[1].of.i32 = fontref;
  arguments[2].قسم = WASM_I32;
  arguments[2].of.i32 = bufferref;
  arguments[3].قسم = WASM_I32;
  arguments[3].of.i32 = featuresptr;
  arguments[4].قسم = WASM_I32;
  arguments[4].of.i32 = عدد_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_لمبائی (results), results,
				  ARRAY_لمبائی (arguments), arguments);

  اگر (عدد_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  اگر (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_پیغام (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    اگر (!بفر->ensure_unicode ())
    {
      DEBUG_پیغام (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      جاؤ fail;
    }
    اگر (retried)
    {
      DEBUG_پیغام (WASM, font, "Giving up...");
      جاؤ fail;
    }
    بفر->successful = سچ;
    retried = سچ;
    release_shape_plan (face_ڈیٹا, plan);
    plan = خالی_اشاریہ;
    جاؤ retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  اگر (!results[0].of.i32)
  {
fail:
    ret = جھوٹ;
  }

  release_shape_plan (face_ڈیٹا, plan, ret);

  اگر (ret)
  {
    بفر->clear_glyph_flags ();
    بفر->unsafe_مقصد_break ();
  }

  واپس ret;
}

#اگر_ختم

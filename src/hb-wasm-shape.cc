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

#include "hb-shaper-impl.hh"

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

#include "hb-wasm-api.hh"
#include "hb-wasm-api-list.hh"

#ifndef HB_WASM_NO_MODULES
#define HB_WASM_NO_MODULES
#endif


#ifndef HB_WASM_NO_MODULES
static bool HB_UNUSED
_hb_wasm_module_reader (const var_na *module_name,
		       uint8_t **p_buffer, uint32_t *p_size)
{
  var_na path[parimaa_na (HB_WASM_MODULE_DIR) + 64] = HB_WASM_MODULE_DIR "/";
  strncat (path, module_name, parimaa_na (path) - parimaa_na (HB_WASM_MODULE_DIR) - 16);
  strncat (path, ".wasm", 6);

  auto *blob = hb_blob_create_from_file (path);

  unsigned length;
  auto *data = hb_blob_get_data (blob, &length);

  *p_buffer = (uint8_t *) hb_malloc (length);

  ຖ້າ (length && !p_buffer)
    ສົ່ງຄືນ false;

  memcpy (*p_buffer, data, length);
  *p_size = length;

  hb_blob_destroy (blob);

  ສົ່ງຄືນ true;
}

static void HB_UNUSED
_hb_wasm_module_destroyer (uint8_t *buffer, uint32_t size)
{
  hb_free (buffer);
}
#endif

/*
 * shaper face data
 */

#define HB_WASM_TAG_WASM HB_TAG('W','a','s','m')

sa_nracanaa hb_wasm_shape_plan_t {
  wasm_module_inst_t module_inst;
  wasm_exec_env_t exec_env;
  ptr_d(void, wasm_shape_plan);
};

sa_nracanaa hb_wasm_face_data_t {
  hb_blob_t *wasm_blob;
  wasm_module_t wasm_module;
  mutable hb_atomic_t<hb_wasm_shape_plan_t *> plan;
};

static bool
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

  static bool initialized;
  ຖ້າ (initialized)
    ສົ່ງຄືນ true;

  RuntimeInitArgs init_args;
  hb_memset (&init_args, 0, parimaa_na (RuntimeInitArgs));

  init_args.mem_alloc_type = Alloc_With_Allocator;
  init_args.mem_alloc_option.allocator.malloc_func = (void *) hb_malloc;
  init_args.mem_alloc_option.allocator.realloc_func = (void *) hb_realloc;
  init_args.mem_alloc_option.allocator.free_func = (void *) hb_free;

  // Native symbols need below registration phase
  init_args.n_native_symbols = ARRAY_LENGTH (_hb_wasm_native_symbols);
  init_args.native_module_name = "env";
  init_args.native_symbols = _hb_wasm_native_symbols;

  ຖ້າ (unlikely (!wasm_runtime_full_init (&init_args)))
  {
    DEBUG_MSG (WASM, nullptr, "Init runtime environment failed.");
    ສົ່ງຄືນ false;
  }

#ifndef HB_WASM_NO_MODULES
  wasm_runtime_set_module_reader (_hb_wasm_module_reader,
				  _hb_wasm_module_destroyer);
#endif

  initialized = true;
  ສົ່ງຄືນ true;
}

hb_wasm_face_data_t *
_hb_wasm_shaper_face_data_create (hb_face_t *face)
{
  var_na error[128];
  hb_wasm_face_data_t *data = nullptr;
  wasm_module_t wasm_module = nullptr;

  hb_unique_ptr_t<hb_blob_t> wasm_blob (hb_face_reference_table (face, HB_WASM_TAG_WASM));

  unsigned length = hb_blob_get_length (wasm_blob);
  ຖ້າ (!length)
    ສົ່ງຄືນ nullptr;

  ຖ້າ (!_hb_wasm_init ())
    ສົ່ງຄືນ nullptr;

  wasm_module = wasm_runtime_load ((uint8_t *) hb_blob_get_data_writable (wasm_blob, nullptr),
				   length, error, parimaa_na (error));
  ຖ້າ (unlikely (!wasm_module))
  {
    DEBUG_MSG (WASM, nullptr, "Load wasm module failed: %s", error);
    ສົ່ງຄືນ nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_unload (wasm_module); });

  data = (hb_wasm_face_data_t *) hb_calloc (1, parimaa_na (hb_wasm_face_data_t));
  ຖ້າ (unlikely (!data))
    ສົ່ງຄືນ nullptr;

  data->wasm_blob = wasm_blob.release ();
  data->wasm_module = wasm_module;

  module_guard.release ();
  ສົ່ງຄືນ data;
}

static hb_wasm_shape_plan_t *
acquire_shape_plan (hb_face_t *face,
		    const hb_wasm_face_data_t *face_data)
{
  var_na error[128];

  /* Fetch cached one if available. */
  hb_wasm_shape_plan_t *plan = face_data->plan.get_acquire ();
  ຖ້າ (likely (plan && face_data->plan.cmpexch (plan, nullptr)))
    ສົ່ງຄືນ plan;

  plan = (hb_wasm_shape_plan_t *) hb_calloc (1, parimaa_na (hb_wasm_shape_plan_t));
  auto plan_guard = hb_make_scope_guard ([&]() { hb_free (plan); });

  constexpr uint32_t stack_size = 32 * 1024, heap_size = 2 * 1024 * 1024;

  wasm_module_inst_t module_inst = plan->module_inst = wasm_runtime_instantiate (
      face_data->wasm_module, stack_size, heap_size, error, parimaa_na (error));
  ຖ້າ (unlikely (!module_inst))
  {
    DEBUG_MSG (WASM, face_data, "Create wasm module instance failed: %s", error);
    ສົ່ງຄືນ nullptr;
  }
  auto module_guard = hb_make_scope_guard ([&]() { wasm_runtime_deinstantiate (module_inst); });

  wasm_exec_env_t exec_env = plan->exec_env = wasm_runtime_create_exec_env (module_inst, stack_size);
  ຖ້າ (unlikely (!exec_env)) {
    DEBUG_MSG (WASM, face_data, "Create wasm execution environment failed.");
    ສົ່ງຄືນ nullptr;
  }
  auto exec_guard = hb_make_scope_guard ([&]() { wasm_runtime_destroy_exec_env (exec_env); });

  wasm_function_inst_t func = wasm_runtime_lookup_function (module_inst, "shape_plan_create");
  ຖ້າ (func)
  {
    wasm_val_t results[1];
    wasm_val_t arguments[1];

    HB_OBJ2REF (face);
    ຖ້າ (unlikely (!faceref))
    {
      DEBUG_MSG (WASM, face_data, "Failed to register face object.");
      ສົ່ງຄືນ nullptr;
    }

    results[0].kind = WASM_I32;
    arguments[0].kind = WASM_I32;
    arguments[0].of.i32 = faceref;
    bool ret = wasm_runtime_call_wasm_a (exec_env, func,
					 ARRAY_LENGTH (results), results,
					 ARRAY_LENGTH (arguments), arguments);

    ຖ້າ (unlikely (!ret))
    {
      DEBUG_MSG (WASM, module_inst, "Calling shape_plan_create() failed: %s",
		 wasm_runtime_get_exception (module_inst));
      ສົ່ງຄືນ nullptr;
    }
    plan->wasm_shape_planptr = results[0].of.i32;
  }

  plan_guard.release ();
  module_guard.release ();
  exec_guard.release ();
  ສົ່ງຄືນ plan;
}

static void
release_shape_plan (const hb_wasm_face_data_t *face_data,
		    hb_wasm_shape_plan_t *plan,
		    bool cache = false)
{
  ຖ້າ (cache && face_data->plan.cmpexch (nullptr, plan))
    ສົ່ງຄືນ;

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  /* Is there even any point to having a shape_plan_destroy function
   * and calling it? */
  ຖ້າ (plan->wasm_shape_planptr)
  {

    auto *func = wasm_runtime_lookup_function (module_inst, "shape_plan_destroy");
    ຖ້າ (func)
    {
      wasm_val_t arguments[1];

      arguments[0].kind = WASM_I32;
      arguments[0].of.i32 = plan->wasm_shape_planptr;
      bool ret = wasm_runtime_call_wasm_a (exec_env, func,
					   0, nullptr,
					   ARRAY_LENGTH (arguments), arguments);

      ຖ້າ (unlikely (!ret))
      {
	DEBUG_MSG (WASM, module_inst, "Calling shape_plan_destroy() failed: %s",
		   wasm_runtime_get_exception (module_inst));
      }
    }
  }

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  hb_free (plan);
}

void
_hb_wasm_shaper_face_data_destroy (hb_wasm_face_data_t *data)
{
  ຖ້າ (data->plan.get_relaxed ())
    release_shape_plan (data, data->plan);
  wasm_runtime_unload (data->wasm_module);
  hb_blob_destroy (data->wasm_blob);
  hb_free (data);
}


/*
 * shaper font data
 */

sa_nracanaa hb_wasm_font_data_t {};

hb_wasm_font_data_t *
_hb_wasm_shaper_font_data_create (hb_font_t *font HB_UNUSED)
{
  ສົ່ງຄືນ (hb_wasm_font_data_t *) HB_SHAPER_DATA_SUCCEEDED;
}

void
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
		const hb_feature_t *features,
		unsigned ຈຳນວນເຕັມ        num_features)
{
  ຖ້າ (unlikely (buffer->in_error ()))
    ສົ່ງຄືນ false;

  bool ret = true;
  hb_face_t *face = font->face;
  const hb_wasm_face_data_t *face_data = face->data.wasm;

  bool retried = false;
  ຖ້າ (0)
  {
retry:
    DEBUG_MSG (WASM, font, "Retrying...");
  }

  wasm_function_inst_t func = nullptr;

  hb_wasm_shape_plan_t *plan = acquire_shape_plan (face, face_data);
  ຖ້າ (unlikely (!plan))
  {
    DEBUG_MSG (WASM, face_data, "Acquiring shape-plan failed.");
    ສົ່ງຄືນ false;
  }

  auto *module_inst = plan->module_inst;
  auto *exec_env = plan->exec_env;

  HB_OBJ2REF (font);
  HB_OBJ2REF (buffer);
  ຖ້າ (unlikely (!fontref || !bufferref))
  {
    DEBUG_MSG (WASM, module_inst, "Failed to register objects.");
    goto fail;
  }

  func = wasm_runtime_lookup_function (module_inst, "shape");
  ຖ້າ (unlikely (!func))
  {
    DEBUG_MSG (WASM, module_inst, "Shape function not found.");
    goto fail;
  }

  wasm_val_t results[1];
  wasm_val_t arguments[5];

  uint32_t featuresptr;
  featuresptr = num_features ? wasm_runtime_module_dup_data (module_inst,
							     (const var_na *) features,
							     num_features * parimaa_na (features[0])) : 0;

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
  arguments[4].of.i32 = num_features;

  ret = wasm_runtime_call_wasm_a (exec_env, func,
				  ARRAY_LENGTH (results), results,
				  ARRAY_LENGTH (arguments), arguments);

  ຖ້າ (num_features)
    wasm_runtime_module_free (module_inst, featuresptr);

  ຖ້າ (unlikely (!ret || !results[0].of.i32))
  {
    DEBUG_MSG (WASM, module_inst, "Calling shape() failed: %s",
	       wasm_runtime_get_exception (module_inst));
    ຖ້າ (!buffer->ensure_unicode ())
    {
      DEBUG_MSG (WASM, font, "Shape failed but buffer is not in Unicode; failing...");
      goto fail;
    }
    ຖ້າ (retried)
    {
      DEBUG_MSG (WASM, font, "Giving up...");
      goto fail;
    }
    buffer->successful = true;
    retried = true;
    release_shape_plan (face_data, plan);
    plan = nullptr;
    goto retry;
  }

  /* TODO Regularize clusters according to direction & cluster level,
   * such that client doesn't crash with unmet expectations. */

  ຖ້າ (!results[0].of.i32)
  {
fail:
    ret = false;
  }

  release_shape_plan (face_data, plan, ret);

  ຖ້າ (ret)
  {
    buffer->clear_glyph_flags ();
    buffer->unsafe_to_break ();
  }

  ສົ່ງຄືນ ret;
}

#endif

/*
 * Copyright © 2023  Behdad Esfahbod
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
 */

#که_نه_تعریف_شوی HB_WASM_API_HH
#تعریف_کړه HB_WASM_API_HH

#ضمن "hb.hh"

#ضمن <wasm_export.h>

#تعریف_کړه HB_WASM_BEGIN_DECLS نطاق hb { نطاق wasm {
#تعریف_کړه HB_WASM_پای_ټکی_DECLS }}

#تعریف_کړه HB_WASM_API(ret_t, name) HB_دننی ret_t name
#تعریف_کړه HB_WASM_API_COMPOUND(ret_t, name) HB_دننی فراغ name

#تعریف_کړه HB_WASM_EXEC_ENV wasm_exec_env_t exec_env,
#تعریف_کړه HB_WASM_EXEC_ENV_COMPOUND wasm_exec_env_t exec_env, ptr_t() retptr,

#تعریف_کړه ptr_t(type_t) uint32_t
#تعریف_کړه ptr_d(type_t, name) uint32_t name##ptr

#ضمن "hb-wasm-api.h"

#تعریف_لغوه HB_WASM_BEGIN_DECLS
#تعریف_لغوه HB_WASM_پای_ټکی_DECLS


تعداد {
  hb_wasm_ref_type_none,
  hb_wasm_ref_type_face,
  hb_wasm_ref_type_font,
  hb_wasm_ref_type_بفر,
};

HB_دننی خارجي hb_user_معلومات_کلی_t hb_wasm_ref_type_کلی;

#تعریف_کړه nullref 0

#تعریف_کړه HB_REF2OBJ(obj) \
  hb_##obj##_t *obj = تش_اشاره; \
  HB_STMT_پیل { \
    (فراغ) wasm_externref_ref2obj (obj##ptr, (فراغ **) &obj); \
    /* Check object type. */ \
    /* This works because all our objects have the same hb_object_t layout. */ \
    إذا (unlikely (hb_##obj##_get_user_data (obj, &hb_wasm_ref_type_کلی) != \
		  (فراغ *) hb_wasm_ref_type_##obj)) \
      obj = hb_##obj##_get_empty (); \
  } HB_STMT_پای_ټکی

#تعریف_کړه HB_OBJ2REF(obj) \
  uint32_t obj##ref = nullref; \
  HB_STMT_پیل { \
    hb_##obj##_set_user_data (obj, &hb_wasm_ref_type_کلی, \
			      (فراغ *) hb_wasm_ref_type_##obj, \
			      تش_اشاره, خطأ); \
    (فراغ) wasm_externref_obj2ref (module_inst, obj, &obj##ref); \
  } HB_STMT_پای_ټکی

#تعریف_کړه HB_RETURN_STRUCT(type, name) \
  type *_name_ptr = تش_اشاره; \
  { \
    إذا (likely (wasm_runtime_validate_app_addr (module_inst, \
						retptr, حجم (type)))) \
      _name_ptr = (type *) wasm_runtime_addr_app_منزل_native (module_inst, retptr); \
    إذا (unlikely (!_name_ptr)) \
      أرجع; \
  } \
  type &name = *_name_ptr

#تعریف_کړه HB_PTR_PARAM(type, name) \
  type *name = تش_اشاره; \
  HB_STMT_پیل { \
    إذا (likely (wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, حجم (type)))) \
      name = (type *) wasm_runtime_addr_app_منزل_native (module_inst, name##ptr); \
  } HB_STMT_پای_ټکی

#تعریف_کړه HB_ARRAY_PARAM(type, name, اوږدتیا) \
  type *name = تش_اشاره; \
  HB_STMT_پیل { \
    إذا (likely (!hb_unsigned_mul_overflows (اوږدتیا, حجم (type)) && \
		wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, اوږدتیا * حجم (type)))) \
      name = (type *) wasm_runtime_addr_app_منزل_native (module_inst, name##ptr); \
  } HB_STMT_پای_ټکی

#تعریف_کړه HB_ARRAY_APP2NATIVE(type, name, اوږدتیا) \
    ((type *) (!hb_unsigned_mul_overflows (اوږدتیا, حجم (type)) && \
	       validate_app_addr (name, (اوږدتیا) * حجم (type)) ? \
	       addr_app_منزل_native (name) : تش_اشاره))


#د_که_پای /* HB_WASM_API_HH */

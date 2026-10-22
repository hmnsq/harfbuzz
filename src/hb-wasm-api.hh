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

#جے_بنا_تعریف HB_WASM_API_HH
#تعریف_کرو HB_WASM_API_HH

#ضمن "hb.hh"

#ضمن <wasm_export.h>

#تعریف_کرو HB_WASM_BEGIN_DECLS نطاق hb { نطاق wasm {
#تعریف_کرو HB_WASM_سرا_DECLS }}

#تعریف_کرو HB_WASM_API(ret_t, name) HB_اندرونی ret_t name
#تعریف_کرو HB_WASM_API_COMPOUND(ret_t, name) HB_اندرونی فراغ name

#تعریف_کرو HB_WASM_EXEC_ENV wasm_exec_env_t exec_env,
#تعریف_کرو HB_WASM_EXEC_ENV_COMPOUND wasm_exec_env_t exec_env, ptr_t() retptr,

#تعریف_کرو ptr_t(type_t) uint32_t
#تعریف_کرو ptr_d(type_t, name) uint32_t name##ptr

#ضمن "hb-wasm-api.h"

#تعریف_رد HB_WASM_BEGIN_DECLS
#تعریف_رد HB_WASM_سرا_DECLS


تعداد {
  hb_wasm_ref_type_none,
  hb_wasm_ref_type_face,
  hb_wasm_ref_type_font,
  hb_wasm_ref_type_بفر,
};

HB_اندرونی خارجي hb_user_انکڑے_کلید_t hb_wasm_ref_type_کلید;

#تعریف_کرو nullref 0

#تعریف_کرو HB_REF2OBJ(obj) \
  hb_##obj##_t *obj = خالی_سوچک; \
  HB_STMT_شروع { \
    (فراغ) wasm_externref_ref2obj (obj##ptr, (فراغ **) &obj); \
    /* Check object type. */ \
    /* This works because all our objects have the same hb_object_t layout. */ \
    إذا (unlikely (hb_##obj##_get_user_data (obj, &hb_wasm_ref_type_کلید) != \
		  (فراغ *) hb_wasm_ref_type_##obj)) \
      obj = hb_##obj##_get_empty (); \
  } HB_STMT_سرا

#تعریف_کرو HB_OBJ2REF(obj) \
  uint32_t obj##ref = nullref; \
  HB_STMT_شروع { \
    hb_##obj##_set_user_data (obj, &hb_wasm_ref_type_کلید, \
			      (فراغ *) hb_wasm_ref_type_##obj, \
			      خالی_سوچک, خطأ); \
    (فراغ) wasm_externref_obj2ref (module_inst, obj, &obj##ref); \
  } HB_STMT_سرا

#تعریف_کرو HB_RETURN_STRUCT(type, name) \
  type *_name_ptr = خالی_سوچک; \
  { \
    إذا (likely (wasm_runtime_validate_app_addr (module_inst, \
						retptr, حجم (type)))) \
      _name_ptr = (type *) wasm_runtime_addr_app_منزل_native (module_inst, retptr); \
    إذا (unlikely (!_name_ptr)) \
      أرجع; \
  } \
  type &name = *_name_ptr

#تعریف_کرو HB_PTR_PARAM(type, name) \
  type *name = خالی_سوچک; \
  HB_STMT_شروع { \
    إذا (likely (wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, حجم (type)))) \
      name = (type *) wasm_runtime_addr_app_منزل_native (module_inst, name##ptr); \
  } HB_STMT_سرا

#تعریف_کرو HB_ARRAY_PARAM(type, name, لمائی) \
  type *name = خالی_سوچک; \
  HB_STMT_شروع { \
    إذا (likely (!hb_unsigned_mul_overflows (لمائی, حجم (type)) && \
		wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, لمائی * حجم (type)))) \
      name = (type *) wasm_runtime_addr_app_منزل_native (module_inst, name##ptr); \
  } HB_STMT_سرا

#تعریف_کرو HB_ARRAY_APP2NATIVE(type, name, لمائی) \
    ((type *) (!hb_unsigned_mul_overflows (لمائی, حجم (type)) && \
	       validate_app_addr (name, (لمائی) * حجم (type)) ? \
	       addr_app_منزل_native (name) : خالی_سوچک))


#جے_انت /* HB_WASM_API_HH */

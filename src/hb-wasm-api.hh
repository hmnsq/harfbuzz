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

#جيڪڏهن_وصف_نه_ٿيل HB_WASM_API_HH
#وصف_ڪريو HB_WASM_API_HH

#ضمن "hb.hh"

#ضمن <wasm_export.h>

#وصف_ڪريو HB_WASM_BEGIN_DECLS نطاق hb { نطاق wasm {
#وصف_ڪريو HB_WASM_پڇاڙي_DECLS }}

#وصف_ڪريو HB_WASM_API(ret_t, name) HB_اندروني ret_t name
#وصف_ڪريو HB_WASM_API_COMPOUND(ret_t, name) HB_اندروني فراغ name

#وصف_ڪريو HB_WASM_EXEC_ENV wasm_exec_env_t exec_env,
#وصف_ڪريو HB_WASM_EXEC_ENV_COMPOUND wasm_exec_env_t exec_env, ptr_t() retptr,

#وصف_ڪريو ptr_t(type_t) uint32_t
#وصف_ڪريو ptr_d(type_t, name) uint32_t name##ptr

#ضمن "hb-wasm-api.h"

#وصف_رد HB_WASM_BEGIN_DECLS
#وصف_رد HB_WASM_پڇاڙي_DECLS


تعداد {
  hb_wasm_ref_type_none,
  hb_wasm_ref_type_face,
  hb_wasm_ref_type_font,
  hb_wasm_ref_type_بفر,
};

HB_اندروني خارجي hb_user_معلومات_ڪنجي_t hb_wasm_ref_type_ڪنجي;

#وصف_ڪريو nullref 0

#وصف_ڪريو HB_REF2OBJ(obj) \
  hb_##obj##_t *obj = خالي_اشارو; \
  HB_STMT_شروعات { \
    (فراغ) wasm_externref_ref2obj (obj##ptr, (فراغ **) &obj); \
    /* Check object type. */ \
    /* This works because all our objects have the same hb_object_t layout. */ \
    إذا (unlikely (hb_##obj##_get_user_data (obj, &hb_wasm_ref_type_ڪنجي) != \
		  (فراغ *) hb_wasm_ref_type_##obj)) \
      obj = hb_##obj##_get_empty (); \
  } HB_STMT_پڇاڙي

#وصف_ڪريو HB_OBJ2REF(obj) \
  uint32_t obj##ref = nullref; \
  HB_STMT_شروعات { \
    hb_##obj##_set_user_data (obj, &hb_wasm_ref_type_ڪنجي, \
			      (فراغ *) hb_wasm_ref_type_##obj, \
			      خالي_اشارو, خطأ); \
    (فراغ) wasm_externref_obj2ref (module_inst, obj, &obj##ref); \
  } HB_STMT_پڇاڙي

#وصف_ڪريو HB_RETURN_STRUCT(type, name) \
  type *_name_ptr = خالي_اشارو; \
  { \
    إذا (likely (wasm_runtime_validate_app_addr (module_inst, \
						retptr, حجم (type)))) \
      _name_ptr = (type *) wasm_runtime_addr_app_منزل_native (module_inst, retptr); \
    إذا (unlikely (!_name_ptr)) \
      أرجع; \
  } \
  type &name = *_name_ptr

#وصف_ڪريو HB_PTR_PARAM(type, name) \
  type *name = خالي_اشارو; \
  HB_STMT_شروعات { \
    إذا (likely (wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, حجم (type)))) \
      name = (type *) wasm_runtime_addr_app_منزل_native (module_inst, name##ptr); \
  } HB_STMT_پڇاڙي

#وصف_ڪريو HB_ARRAY_PARAM(type, name, ڊيگهائي) \
  type *name = خالي_اشارو; \
  HB_STMT_شروعات { \
    إذا (likely (!hb_unsigned_mul_overflows (ڊيگهائي, حجم (type)) && \
		wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, ڊيگهائي * حجم (type)))) \
      name = (type *) wasm_runtime_addr_app_منزل_native (module_inst, name##ptr); \
  } HB_STMT_پڇاڙي

#وصف_ڪريو HB_ARRAY_APP2NATIVE(type, name, ڊيگهائي) \
    ((type *) (!hb_unsigned_mul_overflows (ڊيگهائي, حجم (type)) && \
	       validate_app_addr (name, (ڊيگهائي) * حجم (type)) ? \
	       addr_app_منزل_native (name) : خالي_اشارو))


#جيڪڏهن_پورو /* HB_WASM_API_HH */

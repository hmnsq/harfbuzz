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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <wasm_export.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_BEGIN_DECLS ਨਾਮਾਕਾਸ਼ hb { ਨਾਮਾਕਾਸ਼ wasm {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_ਸਿਰਾ_DECLS }}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API(ret_t, name) HB_ਅੰਦਰੂਨੀ ret_t name
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_COMPOUND(ret_t, name) HB_ਅੰਦਰੂਨੀ ਖਾਲੀ name

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_EXEC_ENV wasm_exec_env_t exec_env,
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_EXEC_ENV_COMPOUND wasm_exec_env_t exec_env, ptr_t() retptr,

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ptr_t(type_t) uint32_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ptr_d(type_t, name) uint32_t name##ptr

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api.h"

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_WASM_BEGIN_DECLS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_WASM_ਸਿਰਾ_DECLS


ਗਿਣਤੀ {
  hb_wasm_ref_type_none,
  hb_wasm_ref_type_face,
  hb_wasm_ref_type_font,
  hb_wasm_ref_type_ਬਫਰ,
};

HB_ਅੰਦਰੂਨੀ ਬਾਹਰੀ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_wasm_ref_type_ਕੁੰਜੀ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ nullref 0

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_REF2OBJ(obj) \
  hb_##obj##_t *obj = ਖਾਲੀ_ਸੂਚਕ; \
  HB_STMT_ਸ਼ੁਰੂ { \
    (ਖਾਲੀ) wasm_externref_ref2obj (obj##ptr, (ਖਾਲੀ **) &obj); \
    /* Check object type. */ \
    /* This works because all our objects have the same hb_object_t layout. */ \
    ਜੇ (unlikely (hb_##obj##_get_user_data (obj, &hb_wasm_ref_type_ਕੁੰਜੀ) != \
		  (ਖਾਲੀ *) hb_wasm_ref_type_##obj)) \
      obj = hb_##obj##_get_empty (); \
  } HB_STMT_ਸਿਰਾ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OBJ2REF(obj) \
  uint32_t obj##ref = nullref; \
  HB_STMT_ਸ਼ੁਰੂ { \
    hb_##obj##_set_user_data (obj, &hb_wasm_ref_type_ਕੁੰਜੀ, \
			      (ਖਾਲੀ *) hb_wasm_ref_type_##obj, \
			      ਖਾਲੀ_ਸੂਚਕ, ਝੂਠ); \
    (ਖਾਲੀ) wasm_externref_obj2ref (module_inst, obj, &obj##ref); \
  } HB_STMT_ਸਿਰਾ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RETURN_STRUCT(type, name) \
  type *_name_ptr = ਖਾਲੀ_ਸੂਚਕ; \
  { \
    ਜੇ (likely (wasm_runtime_validate_app_addr (module_inst, \
						retptr, ਆਕਾਰ (type)))) \
      _name_ptr = (type *) wasm_runtime_addr_app_ਮੰਜ਼ਿਲ_native (module_inst, retptr); \
    ਜੇ (unlikely (!_name_ptr)) \
      ਵਾਪਸ; \
  } \
  type &name = *_name_ptr

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PTR_PARAM(type, name) \
  type *name = ਖਾਲੀ_ਸੂਚਕ; \
  HB_STMT_ਸ਼ੁਰੂ { \
    ਜੇ (likely (wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, ਆਕਾਰ (type)))) \
      name = (type *) wasm_runtime_addr_app_ਮੰਜ਼ਿਲ_native (module_inst, name##ptr); \
  } HB_STMT_ਸਿਰਾ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ARRAY_PARAM(type, name, ਲੰਮਾਈ) \
  type *name = ਖਾਲੀ_ਸੂਚਕ; \
  HB_STMT_ਸ਼ੁਰੂ { \
    ਜੇ (likely (!hb_unsigned_mul_overflows (ਲੰਮਾਈ, ਆਕਾਰ (type)) && \
		wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, ਲੰਮਾਈ * ਆਕਾਰ (type)))) \
      name = (type *) wasm_runtime_addr_app_ਮੰਜ਼ਿਲ_native (module_inst, name##ptr); \
  } HB_STMT_ਸਿਰਾ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ARRAY_APP2NATIVE(type, name, ਲੰਮਾਈ) \
    ((type *) (!hb_unsigned_mul_overflows (ਲੰਮਾਈ, ਆਕਾਰ (type)) && \
	       validate_app_addr (name, (ਲੰਮਾਈ) * ਆਕਾਰ (type)) ? \
	       addr_app_ਮੰਜ਼ਿਲ_native (name) : ਖਾਲੀ_ਸੂਚਕ))


#ਜੇ_ਅੰਤ /* HB_WASM_API_HH */

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

#अगर_अघोषित HB_WASM_API_HH
#घोषणा HB_WASM_API_HH

#समावेश "hb.hh"

#समावेश <wasm_export.h>

#घोषणा HB_WASM_BEGIN_DECLS namespace hb { namespace wasm {
#घोषणा HB_WASM_END_DECLS }}

#घोषणा HB_WASM_API(ret_t, name) HB_INTERNAL ret_t name
#घोषणा HB_WASM_API_COMPOUND(ret_t, name) HB_INTERNAL व्योम name

#घोषणा HB_WASM_EXEC_ENV wasm_exec_env_t exec_env,
#घोषणा HB_WASM_EXEC_ENV_COMPOUND wasm_exec_env_t exec_env, ptr_t() retptr,

#घोषणा ptr_t(type_t) uint32_t
#घोषणा ptr_d(type_t, name) uint32_t name##ptr

#समावेश "hb-wasm-api.h"

#अघोषित HB_WASM_BEGIN_DECLS
#अघोषित HB_WASM_END_DECLS


क्रमागत {
  hb_wasm_ref_type_none,
  hb_wasm_ref_type_face,
  hb_wasm_ref_type_font,
  hb_wasm_ref_type_बफर,
};

HB_INTERNAL बाह्य hb_user_तथ्य_कुंजी_t hb_wasm_ref_type_कुंजी;

#घोषणा nullref 0

#घोषणा HB_REF2OBJ(obj) \
  hb_##obj##_t *obj = nullptr; \
  HB_STMT_START { \
    (व्योम) wasm_externref_ref2obj (obj##ptr, (व्योम **) &obj); \
    /* Check object type. */ \
    /* This works because all our objects have the same hb_object_t layout. */ \
    अगर (unlikely (hb_##obj##_get_user_data (obj, &hb_wasm_ref_type_कुंजी) != \
		  (व्योम *) hb_wasm_ref_type_##obj)) \
      obj = hb_##obj##_get_empty (); \
  } HB_STMT_END

#घोषणा HB_OBJ2REF(obj) \
  uint32_t obj##ref = nullref; \
  HB_STMT_START { \
    hb_##obj##_set_user_data (obj, &hb_wasm_ref_type_कुंजी, \
			      (व्योम *) hb_wasm_ref_type_##obj, \
			      nullptr, false); \
    (व्योम) wasm_externref_obj2ref (module_inst, obj, &obj##ref); \
  } HB_STMT_END

#घोषणा HB_RETURN_STRUCT(type, name) \
  type *_name_ptr = nullptr; \
  { \
    अगर (likely (wasm_runtime_validate_app_addr (module_inst, \
						retptr, माप (type)))) \
      _name_ptr = (type *) wasm_runtime_addr_app_to_native (module_inst, retptr); \
    अगर (unlikely (!_name_ptr)) \
      वापस; \
  } \
  type &name = *_name_ptr

#घोषणा HB_PTR_PARAM(type, name) \
  type *name = nullptr; \
  HB_STMT_START { \
    अगर (likely (wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, माप (type)))) \
      name = (type *) wasm_runtime_addr_app_to_native (module_inst, name##ptr); \
  } HB_STMT_END

#घोषणा HB_ARRAY_PARAM(type, name, लंबाई) \
  type *name = nullptr; \
  HB_STMT_START { \
    अगर (likely (!hb_unsigned_mul_overflows (लंबाई, माप (type)) && \
		wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, लंबाई * माप (type)))) \
      name = (type *) wasm_runtime_addr_app_to_native (module_inst, name##ptr); \
  } HB_STMT_END

#घोषणा HB_ARRAY_APP2NATIVE(type, name, लंबाई) \
    ((type *) (!hb_unsigned_mul_overflows (लंबाई, माप (type)) && \
	       validate_app_addr (name, (लंबाई) * माप (type)) ? \
	       addr_app_to_native (name) : nullptr))


#पूर्ण_अगर /* HB_WASM_API_HH */

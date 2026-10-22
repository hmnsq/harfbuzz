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

#నిర్వచితం_కాకపోతే HB_WASM_API_HH
#నిర్వచించు HB_WASM_API_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ <wasm_export.h>

#నిర్వచించు HB_WASM_BEGIN_DECLS నామాకాశ hb { నామాకాశ wasm {
#నిర్వచించు HB_WASM_చివర_DECLS }}

#నిర్వచించు HB_WASM_API(ret_t, name) HB_అంతర్గత ret_t name
#నిర్వచించు HB_WASM_API_COMPOUND(ret_t, name) HB_అంతర్గత శూన్య name

#నిర్వచించు HB_WASM_EXEC_ENV wasm_exec_env_t exec_env,
#నిర్వచించు HB_WASM_EXEC_ENV_COMPOUND wasm_exec_env_t exec_env, ptr_t() retptr,

#నిర్వచించు ptr_t(type_t) uint32_t
#నిర్వచించు ptr_d(type_t, name) uint32_t name##ptr

#అన్తర్భావ "hb-wasm-api.h"

#నిర్వచనం_రద్దు HB_WASM_BEGIN_DECLS
#నిర్వచనం_రద్దు HB_WASM_చివర_DECLS


గణన {
  hb_wasm_ref_type_none,
  hb_wasm_ref_type_face,
  hb_wasm_ref_type_font,
  hb_wasm_ref_type_బఫరు,
};

HB_అంతర్గత బాహ్య hb_user_దత్తాంశం_కీలకం_t hb_wasm_ref_type_కీలకం;

#నిర్వచించు nullref 0

#నిర్వచించు HB_REF2OBJ(obj) \
  hb_##obj##_t *obj = శూన్యసూచిక; \
  HB_STMT_ప్రారంభం { \
    (శూన్య) wasm_externref_ref2obj (obj##ptr, (శూన్య **) &obj); \
    /* Check object type. */ \
    /* This works because all our objects have the same hb_object_t layout. */ \
    ఒకవేళ (unlikely (hb_##obj##_get_user_data (obj, &hb_wasm_ref_type_కీలకం) != \
		  (శూన్య *) hb_wasm_ref_type_##obj)) \
      obj = hb_##obj##_get_empty (); \
  } HB_STMT_చివర

#నిర్వచించు HB_OBJ2REF(obj) \
  uint32_t obj##ref = nullref; \
  HB_STMT_ప్రారంభం { \
    hb_##obj##_set_user_data (obj, &hb_wasm_ref_type_కీలకం, \
			      (శూన్య *) hb_wasm_ref_type_##obj, \
			      శూన్యసూచిక, అబద్ధం); \
    (శూన్య) wasm_externref_obj2ref (module_inst, obj, &obj##ref); \
  } HB_STMT_చివర

#నిర్వచించు HB_RETURN_STRUCT(type, name) \
  type *_name_ptr = శూన్యసూచిక; \
  { \
    ఒకవేళ (likely (wasm_runtime_validate_app_addr (module_inst, \
						retptr, పరిమాణం (type)))) \
      _name_ptr = (type *) wasm_runtime_addr_app_గమ్యం_native (module_inst, retptr); \
    ఒకవేళ (unlikely (!_name_ptr)) \
      తిరిగి; \
  } \
  type &name = *_name_ptr

#నిర్వచించు HB_PTR_PARAM(type, name) \
  type *name = శూన్యసూచిక; \
  HB_STMT_ప్రారంభం { \
    ఒకవేళ (likely (wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, పరిమాణం (type)))) \
      name = (type *) wasm_runtime_addr_app_గమ్యం_native (module_inst, name##ptr); \
  } HB_STMT_చివర

#నిర్వచించు HB_ARRAY_PARAM(type, name, నిడివి) \
  type *name = శూన్యసూచిక; \
  HB_STMT_ప్రారంభం { \
    ఒకవేళ (likely (!hb_unsigned_mul_overflows (నిడివి, పరిమాణం (type)) && \
		wasm_runtime_validate_app_addr (module_inst, \
						name##ptr, నిడివి * పరిమాణం (type)))) \
      name = (type *) wasm_runtime_addr_app_గమ్యం_native (module_inst, name##ptr); \
  } HB_STMT_చివర

#నిర్వచించు HB_ARRAY_APP2NATIVE(type, name, నిడివి) \
    ((type *) (!hb_unsigned_mul_overflows (నిడివి, పరిమాణం (type)) && \
	       validate_app_addr (name, (నిడివి) * పరిమాణం (type)) ? \
	       addr_app_గమ్యం_native (name) : శూన్యసూచిక))


#అయితే_ముగింపు /* HB_WASM_API_HH */

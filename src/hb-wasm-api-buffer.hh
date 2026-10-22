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

#اگر_تعریف_نشده HB_WASM_API_میانگیر_HH
#تعریف_کن HB_WASM_API_میانگیر_HH

#ضمن "hb-wasm-api.hh"

#ضمن "hb-buffer.hh"

نطاق hb {
نطاق wasm {

static_assert (حجم (glyph_info_t) == حجم (hb_glyph_info_t), "");
static_assert (حجم (glyph_position_t) == حجم (hb_glyph_position_t), "");

HB_WASM_API (bool_t, میانگیر_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(میانگیر_contents_t, contents),
					       uint32_t اندازه)
{
  HB_PTR_PARAM (میانگیر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع خطأ;

  إذا (اندازه <= contents->درازی)
    أرجع درست;

  بیعلامت بایتها;
  إذا (hb_unsigned_mul_overflows (اندازه, حجم (glyph_info_t), &بایتها))
    أرجع خطأ;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->درازی);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->درازی);

  إذا (unlikely (!info || !pos))
    أرجع خطأ;

  glyph_info_t *new_info = اشاره_تهی;
  uint32_t new_inforef = module_malloc (بایتها, (فراغ **) &new_info);
  glyph_position_t *new_pos = اشاره_تهی;
  uint32_t new_posref = module_malloc (بایتها, (فراغ **) &new_pos);

  بیعلامت old_بایتها = contents->درازی * حجم (glyph_info_t);
  إذا (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_بایتها);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  إذا (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_بایتها);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  إذا (likely (new_info && new_pos))
  {
    contents->درازی = اندازه;
    أرجع درست;
  }

  أرجع خطأ;
}

HB_WASM_API (فراغ, میانگیر_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(میانگیر_contents_t, contents))
{
  HB_PTR_PARAM (میانگیر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع;

  module_free (contents->info);
  module_free (contents->pos);

  contents->info = nullref;
  contents->pos = nullref;
  contents->درازی = 0;
}

HB_WASM_API (bool_t, میانگیر_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(میانگیر_t, میانگیر),
					    ptr_d(میانگیر_contents_t, contents))
{
  HB_REF2OBJ (میانگیر);
  HB_PTR_PARAM (میانگیر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع خطأ;

  إذا (میانگیر->have_output)
    میانگیر->متزامن ();
  إذا (!میانگیر->have_positions)
    میانگیر->clear_positions ();

  بیعلامت درازی = میانگیر->len;

  إذا (درازی <= contents->درازی)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, درازی);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, درازی);

    إذا (unlikely (!info || !pos))
    {
      contents->درازی = 0;
      أرجع خطأ;
    }

    بیعلامت بایتها = درازی * حجم (hb_glyph_info_t);
    hb_memcpy (info, میانگیر->info, بایتها);
    hb_memcpy (pos, میانگیر->pos, بایتها);

    أرجع درست;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->درازی = درازی;
  بیعلامت بایتها = درازی * حجم (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_داده (module_inst, (ثابت حرف *) میانگیر->info, بایتها);
  contents->pos = wasm_runtime_module_dup_داده (module_inst, (ثابت حرف *) میانگیر->pos, بایتها);

  إذا (درازی && (!contents->info || !contents->pos))
  {
    contents->درازی = 0;
    أرجع خطأ;
  }

  أرجع درست;
}

HB_WASM_API (bool_t, میانگیر_مجموعه_contents) (HB_WASM_EXEC_ENV
					   ptr_d(میانگیر_t, میانگیر),
					   ptr_d(ثابت میانگیر_contents_t, contents))
{
  HB_REF2OBJ (میانگیر);
  HB_PTR_PARAM (میانگیر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع خطأ;

  بیعلامت درازی = contents->درازی;
  بیعلامت بایتها;
  إذا (unlikely (hb_unsigned_mul_overflows (درازی, حجم (میانگیر->info[0]), &بایتها)))
    أرجع خطأ;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, بایتها) ? addr_app_مقصد_native (contents->info) : اشاره_تهی);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, بایتها) ? addr_app_مقصد_native (contents->pos) : اشاره_تهی);
  إذا (unlikely (!info || !pos))
    أرجع خطأ;

  إذا (unlikely (!میانگیر->resize (درازی)))
    أرجع خطأ;

  إذا (!میانگیر->have_positions)
    میانگیر->clear_positions (); /* This is wasteful. */

  hb_memcpy (میانگیر->info, info, بایتها);
  hb_memcpy (میانگیر->pos, pos, بایتها);
  میانگیر->len = درازی;

  أرجع درست;
}

HB_WASM_API (direction_t, میانگیر_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(میانگیر_t, میانگیر))
{
  HB_REF2OBJ (میانگیر);

  أرجع (direction_t) hb_میانگیر_get_direction (میانگیر);
}

HB_WASM_API (script_t, میانگیر_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(میانگیر_t, میانگیر))
{
  HB_REF2OBJ (میانگیر);

  أرجع hb_script_مقصد_iso15924_برچسب (hb_میانگیر_get_script (میانگیر));
}

HB_WASM_API (فراغ, میانگیر_معکوس) (HB_WASM_EXEC_ENV
				    ptr_d(میانگیر_t, میانگیر))
{
  HB_REF2OBJ (میانگیر);

  hb_میانگیر_معکوس (میانگیر);
}

HB_WASM_API (فراغ, میانگیر_معکوس_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(میانگیر_t, میانگیر))
{
  HB_REF2OBJ (میانگیر);

  hb_میانگیر_معکوس_clusters (میانگیر);
}

}}

#پایان_اگر /* HB_WASM_API_BUFFER_HH */

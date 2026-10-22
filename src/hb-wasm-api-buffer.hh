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

#اگر_غیر_تعریف_شدہ HB_WASM_API_بفر_HH
#تعریف_کرو HB_WASM_API_بفر_HH

#شامل "hb-wasm-api.hh"

#شامل "hb-buffer.hh"

نامخلا hb {
نامخلا wasm {

static_assert (حجم (glyph_info_t) == حجم (hb_glyph_info_t), "");
static_assert (حجم (glyph_position_t) == حجم (hb_glyph_position_t), "");

HB_WASM_API (bool_t, بفر_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(بفر_contents_t, contents),
					       uint32_t سائز)
{
  HB_PTR_PARAM (بفر_contents_t, contents);
  اگر (unlikely (!contents))
    واپس جھوٹ;

  اگر (سائز <= contents->لمبائی)
    واپس سچ;

  غیرعلامتی بائٹیں;
  اگر (hb_unsigned_mul_overflows (سائز, حجم (glyph_info_t), &بائٹیں))
    واپس جھوٹ;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->لمبائی);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->لمبائی);

  اگر (unlikely (!info || !pos))
    واپس جھوٹ;

  glyph_info_t *new_info = خالی_اشاریہ;
  uint32_t new_inforef = module_malloc (بائٹیں, (خالی **) &new_info);
  glyph_position_t *new_pos = خالی_اشاریہ;
  uint32_t new_posref = module_malloc (بائٹیں, (خالی **) &new_pos);

  غیرعلامتی old_بائٹیں = contents->لمبائی * حجم (glyph_info_t);
  اگر (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_بائٹیں);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  اگر (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_بائٹیں);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  اگر (likely (new_info && new_pos))
  {
    contents->لمبائی = سائز;
    واپس سچ;
  }

  واپس جھوٹ;
}

HB_WASM_API (خالی, بفر_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(بفر_contents_t, contents))
{
  HB_PTR_PARAM (بفر_contents_t, contents);
  اگر (unlikely (!contents))
    واپس;

  module_free (contents->info);
  module_free (contents->pos);

  contents->info = nullref;
  contents->pos = nullref;
  contents->لمبائی = 0;
}

HB_WASM_API (bool_t, بفر_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(بفر_t, بفر),
					    ptr_d(بفر_contents_t, contents))
{
  HB_REF2OBJ (بفر);
  HB_PTR_PARAM (بفر_contents_t, contents);
  اگر (unlikely (!contents))
    واپس جھوٹ;

  اگر (بفر->have_output)
    بفر->ہمآہنگ ();
  اگر (!بفر->have_positions)
    بفر->clear_positions ();

  غیرعلامتی لمبائی = بفر->len;

  اگر (لمبائی <= contents->لمبائی)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, لمبائی);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, لمبائی);

    اگر (unlikely (!info || !pos))
    {
      contents->لمبائی = 0;
      واپس جھوٹ;
    }

    غیرعلامتی بائٹیں = لمبائی * حجم (hb_glyph_info_t);
    hb_memcpy (info, بفر->info, بائٹیں);
    hb_memcpy (pos, بفر->pos, بائٹیں);

    واپس سچ;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->لمبائی = لمبائی;
  غیرعلامتی بائٹیں = لمبائی * حجم (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_ڈیٹا (module_inst, (ثابت حرف *) بفر->info, بائٹیں);
  contents->pos = wasm_runtime_module_dup_ڈیٹا (module_inst, (ثابت حرف *) بفر->pos, بائٹیں);

  اگر (لمبائی && (!contents->info || !contents->pos))
  {
    contents->لمبائی = 0;
    واپس جھوٹ;
  }

  واپس سچ;
}

HB_WASM_API (bool_t, بفر_سیٹ_contents) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر),
					   ptr_d(ثابت بفر_contents_t, contents))
{
  HB_REF2OBJ (بفر);
  HB_PTR_PARAM (بفر_contents_t, contents);
  اگر (unlikely (!contents))
    واپس جھوٹ;

  غیرعلامتی لمبائی = contents->لمبائی;
  غیرعلامتی بائٹیں;
  اگر (unlikely (hb_unsigned_mul_overflows (لمبائی, حجم (بفر->info[0]), &بائٹیں)))
    واپس جھوٹ;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, بائٹیں) ? addr_app_مقصد_native (contents->info) : خالی_اشاریہ);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, بائٹیں) ? addr_app_مقصد_native (contents->pos) : خالی_اشاریہ);
  اگر (unlikely (!info || !pos))
    واپس جھوٹ;

  اگر (unlikely (!بفر->resize (لمبائی)))
    واپس جھوٹ;

  اگر (!بفر->have_positions)
    بفر->clear_positions (); /* This is wasteful. */

  hb_memcpy (بفر->info, info, بائٹیں);
  hb_memcpy (بفر->pos, pos, بائٹیں);
  بفر->len = لمبائی;

  واپس سچ;
}

HB_WASM_API (direction_t, بفر_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  واپس (direction_t) hb_بفر_get_direction (بفر);
}

HB_WASM_API (script_t, بفر_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  واپس hb_script_مقصد_iso15924_لیبل (hb_بفر_get_script (بفر));
}

HB_WASM_API (خالی, بفر_الٹ) (HB_WASM_EXEC_ENV
				    ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  hb_بفر_الٹ (بفر);
}

HB_WASM_API (خالی, بفر_الٹ_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  hb_بفر_الٹ_clusters (بفر);
}

}}

#اگر_ختم /* HB_WASM_API_BUFFER_HH */

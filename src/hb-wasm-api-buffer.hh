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

#جيڪڏهن_وصف_نه_ٿيل HB_WASM_API_بفر_HH
#وصف_ڪريو HB_WASM_API_بفر_HH

#ضمن "hb-wasm-api.hh"

#ضمن "hb-buffer.hh"

نطاق hb {
نطاق wasm {

static_assert (حجم (glyph_info_t) == حجم (hb_glyph_info_t), "");
static_assert (حجم (glyph_position_t) == حجم (hb_glyph_position_t), "");

HB_WASM_API (bool_t, بفر_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(بفر_contents_t, contents),
					       uint32_t سائيز)
{
  HB_PTR_PARAM (بفر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع خطأ;

  إذا (سائيز <= contents->ڊيگهائي)
    أرجع سچ;

  بنانشان بائيٽون;
  إذا (hb_unsigned_mul_overflows (سائيز, حجم (glyph_info_t), &بائيٽون))
    أرجع خطأ;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->ڊيگهائي);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->ڊيگهائي);

  إذا (unlikely (!info || !pos))
    أرجع خطأ;

  glyph_info_t *new_info = خالي_اشارو;
  uint32_t new_inforef = module_malloc (بائيٽون, (فراغ **) &new_info);
  glyph_position_t *new_pos = خالي_اشارو;
  uint32_t new_posref = module_malloc (بائيٽون, (فراغ **) &new_pos);

  بنانشان old_بائيٽون = contents->ڊيگهائي * حجم (glyph_info_t);
  إذا (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_بائيٽون);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  إذا (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_بائيٽون);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  إذا (likely (new_info && new_pos))
  {
    contents->ڊيگهائي = سائيز;
    أرجع سچ;
  }

  أرجع خطأ;
}

HB_WASM_API (فراغ, بفر_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(بفر_contents_t, contents))
{
  HB_PTR_PARAM (بفر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع;

  module_free (contents->info);
  module_free (contents->pos);

  contents->info = nullref;
  contents->pos = nullref;
  contents->ڊيگهائي = 0;
}

HB_WASM_API (bool_t, بفر_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(بفر_t, بفر),
					    ptr_d(بفر_contents_t, contents))
{
  HB_REF2OBJ (بفر);
  HB_PTR_PARAM (بفر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع خطأ;

  إذا (بفر->have_output)
    بفر->متزامن ();
  إذا (!بفر->have_positions)
    بفر->clear_positions ();

  بنانشان ڊيگهائي = بفر->len;

  إذا (ڊيگهائي <= contents->ڊيگهائي)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, ڊيگهائي);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, ڊيگهائي);

    إذا (unlikely (!info || !pos))
    {
      contents->ڊيگهائي = 0;
      أرجع خطأ;
    }

    بنانشان بائيٽون = ڊيگهائي * حجم (hb_glyph_info_t);
    hb_memcpy (info, بفر->info, بائيٽون);
    hb_memcpy (pos, بفر->pos, بائيٽون);

    أرجع سچ;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->ڊيگهائي = ڊيگهائي;
  بنانشان بائيٽون = ڊيگهائي * حجم (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_معلومات (module_inst, (ثابت حرف *) بفر->info, بائيٽون);
  contents->pos = wasm_runtime_module_dup_معلومات (module_inst, (ثابت حرف *) بفر->pos, بائيٽون);

  إذا (ڊيگهائي && (!contents->info || !contents->pos))
  {
    contents->ڊيگهائي = 0;
    أرجع خطأ;
  }

  أرجع سچ;
}

HB_WASM_API (bool_t, بفر_سيٽ_contents) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر),
					   ptr_d(ثابت بفر_contents_t, contents))
{
  HB_REF2OBJ (بفر);
  HB_PTR_PARAM (بفر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع خطأ;

  بنانشان ڊيگهائي = contents->ڊيگهائي;
  بنانشان بائيٽون;
  إذا (unlikely (hb_unsigned_mul_overflows (ڊيگهائي, حجم (بفر->info[0]), &بائيٽون)))
    أرجع خطأ;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, بائيٽون) ? addr_app_منزل_native (contents->info) : خالي_اشارو);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, بائيٽون) ? addr_app_منزل_native (contents->pos) : خالي_اشارو);
  إذا (unlikely (!info || !pos))
    أرجع خطأ;

  إذا (unlikely (!بفر->resize (ڊيگهائي)))
    أرجع خطأ;

  إذا (!بفر->have_positions)
    بفر->clear_positions (); /* This is wasteful. */

  hb_memcpy (بفر->info, info, بائيٽون);
  hb_memcpy (بفر->pos, pos, بائيٽون);
  بفر->len = ڊيگهائي;

  أرجع سچ;
}

HB_WASM_API (direction_t, بفر_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  أرجع (direction_t) hb_بفر_get_direction (بفر);
}

HB_WASM_API (script_t, بفر_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  أرجع hb_script_منزل_iso15924_ٽيگ (hb_بفر_get_script (بفر));
}

HB_WASM_API (فراغ, بفر_ابتو) (HB_WASM_EXEC_ENV
				    ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  hb_بفر_ابتو (بفر);
}

HB_WASM_API (فراغ, بفر_ابتو_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  hb_بفر_ابتو_clusters (بفر);
}

}}

#جيڪڏهن_پورو /* HB_WASM_API_BUFFER_HH */

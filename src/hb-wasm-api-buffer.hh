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

#که_نه_تعریف_شوی HB_WASM_API_بفر_HH
#تعریف_کړه HB_WASM_API_بفر_HH

#ضمن "hb-wasm-api.hh"

#ضمن "hb-buffer.hh"

نطاق hb {
نطاق wasm {

static_assert (حجم (glyph_info_t) == حجم (hb_glyph_info_t), "");
static_assert (حجم (glyph_position_t) == حجم (hb_glyph_position_t), "");

HB_WASM_API (bool_t, بفر_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(بفر_contents_t, contents),
					       uint32_t اندازه)
{
  HB_PTR_PARAM (بفر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع خطأ;

  إذا (اندازه <= contents->اوږدتیا)
    أرجع ریښتیا;

  بینښه بایټونه;
  إذا (hb_unsigned_mul_overflows (اندازه, حجم (glyph_info_t), &بایټونه))
    أرجع خطأ;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->اوږدتیا);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->اوږدتیا);

  إذا (unlikely (!info || !pos))
    أرجع خطأ;

  glyph_info_t *new_info = تش_اشاره;
  uint32_t new_inforef = module_malloc (بایټونه, (فراغ **) &new_info);
  glyph_position_t *new_pos = تش_اشاره;
  uint32_t new_posref = module_malloc (بایټونه, (فراغ **) &new_pos);

  بینښه old_بایټونه = contents->اوږدتیا * حجم (glyph_info_t);
  إذا (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_بایټونه);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  إذا (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_بایټونه);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  إذا (likely (new_info && new_pos))
  {
    contents->اوږدتیا = اندازه;
    أرجع ریښتیا;
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
  contents->اوږدتیا = 0;
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

  بینښه اوږدتیا = بفر->len;

  إذا (اوږدتیا <= contents->اوږدتیا)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, اوږدتیا);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, اوږدتیا);

    إذا (unlikely (!info || !pos))
    {
      contents->اوږدتیا = 0;
      أرجع خطأ;
    }

    بینښه بایټونه = اوږدتیا * حجم (hb_glyph_info_t);
    hb_memcpy (info, بفر->info, بایټونه);
    hb_memcpy (pos, بفر->pos, بایټونه);

    أرجع ریښتیا;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->اوږدتیا = اوږدتیا;
  بینښه بایټونه = اوږدتیا * حجم (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_معلومات (module_inst, (ثابت حرف *) بفر->info, بایټونه);
  contents->pos = wasm_runtime_module_dup_معلومات (module_inst, (ثابت حرف *) بفر->pos, بایټونه);

  إذا (اوږدتیا && (!contents->info || !contents->pos))
  {
    contents->اوږدتیا = 0;
    أرجع خطأ;
  }

  أرجع ریښتیا;
}

HB_WASM_API (bool_t, بفر_سېټ_contents) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر),
					   ptr_d(ثابت بفر_contents_t, contents))
{
  HB_REF2OBJ (بفر);
  HB_PTR_PARAM (بفر_contents_t, contents);
  إذا (unlikely (!contents))
    أرجع خطأ;

  بینښه اوږدتیا = contents->اوږدتیا;
  بینښه بایټونه;
  إذا (unlikely (hb_unsigned_mul_overflows (اوږدتیا, حجم (بفر->info[0]), &بایټونه)))
    أرجع خطأ;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, بایټونه) ? addr_app_منزل_native (contents->info) : تش_اشاره);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, بایټونه) ? addr_app_منزل_native (contents->pos) : تش_اشاره);
  إذا (unlikely (!info || !pos))
    أرجع خطأ;

  إذا (unlikely (!بفر->resize (اوږدتیا)))
    أرجع خطأ;

  إذا (!بفر->have_positions)
    بفر->clear_positions (); /* This is wasteful. */

  hb_memcpy (بفر->info, info, بایټونه);
  hb_memcpy (بفر->pos, pos, بایټونه);
  بفر->len = اوږدتیا;

  أرجع ریښتیا;
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

  أرجع hb_script_منزل_iso15924_نښان (hb_بفر_get_script (بفر));
}

HB_WASM_API (فراغ, بفر_سرچپه) (HB_WASM_EXEC_ENV
				    ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  hb_بفر_سرچپه (بفر);
}

HB_WASM_API (فراغ, بفر_سرچپه_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(بفر_t, بفر))
{
  HB_REF2OBJ (بفر);

  hb_بفر_سرچپه_clusters (بفر);
}

}}

#د_که_پای /* HB_WASM_API_BUFFER_HH */

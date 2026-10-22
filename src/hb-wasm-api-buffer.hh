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

#ifndef HB_WASM_API_বাফার_HH
#define HB_WASM_API_বাফার_HH

#অন্তর্ভাব "hb-wasm-api.hh"

#অন্তর্ভাব "hb-buffer.hh"

নামাকাশ hb {
নামাকাশ wasm {

static_assert (আকার (glyph_info_t) == আকার (hb_glyph_info_t), "");
static_assert (আকার (glyph_position_t) == আকার (hb_glyph_position_t), "");

HB_WASM_API (bool_t, বাফার_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(বাফার_contents_t, contents),
					       uint32_t আকার_2)
{
  HB_PTR_PARAM (বাফার_contents_t, contents);
  যদি (unlikely (!contents))
    ফেরত মিথ্যা;

  যদি (আকার_2 <= contents->দৈর্ঘ্য)
    ফেরত সত্য;

  unsigned bytes;
  যদি (hb_unsigned_mul_overflows (আকার_2, আকার (glyph_info_t), &bytes))
    ফেরত মিথ্যা;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->দৈর্ঘ্য);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->দৈর্ঘ্য);

  যদি (unlikely (!info || !pos))
    ফেরত মিথ্যা;

  glyph_info_t *new_info = nullptr;
  uint32_t new_inforef = module_malloc (bytes, (শূন্য **) &new_info);
  glyph_position_t *new_pos = nullptr;
  uint32_t new_posref = module_malloc (bytes, (শূন্য **) &new_pos);

  unsigned old_bytes = contents->দৈর্ঘ্য * আকার (glyph_info_t);
  যদি (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_bytes);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  যদি (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_bytes);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  যদি (likely (new_info && new_pos))
  {
    contents->দৈর্ঘ্য = আকার_2;
    ফেরত সত্য;
  }

  ফেরত মিথ্যা;
}

HB_WASM_API (শূন্য, বাফার_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(বাফার_contents_t, contents))
{
  HB_PTR_PARAM (বাফার_contents_t, contents);
  যদি (unlikely (!contents))
    ফেরত;

  module_free (contents->info);
  module_free (contents->pos);

  contents->info = nullref;
  contents->pos = nullref;
  contents->দৈর্ঘ্য = 0;
}

HB_WASM_API (bool_t, বাফার_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(বাফার_t, বাফার),
					    ptr_d(বাফার_contents_t, contents))
{
  HB_REF2OBJ (বাফার);
  HB_PTR_PARAM (বাফার_contents_t, contents);
  যদি (unlikely (!contents))
    ফেরত মিথ্যা;

  যদি (বাফার->have_output)
    বাফার->সমলয় ();
  যদি (!বাফার->have_positions)
    বাফার->clear_positions ();

  unsigned দৈর্ঘ্য = বাফার->len;

  যদি (দৈর্ঘ্য <= contents->দৈর্ঘ্য)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, দৈর্ঘ্য);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, দৈর্ঘ্য);

    যদি (unlikely (!info || !pos))
    {
      contents->দৈর্ঘ্য = 0;
      ফেরত মিথ্যা;
    }

    unsigned bytes = দৈর্ঘ্য * আকার (hb_glyph_info_t);
    hb_memcpy (info, বাফার->info, bytes);
    hb_memcpy (pos, বাফার->pos, bytes);

    ফেরত সত্য;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->দৈর্ঘ্য = দৈর্ঘ্য;
  unsigned bytes = দৈর্ঘ্য * আকার (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_তথ্য (module_inst, (ধ্রুবক অক্ষর *) বাফার->info, bytes);
  contents->pos = wasm_runtime_module_dup_তথ্য (module_inst, (ধ্রুবক অক্ষর *) বাফার->pos, bytes);

  যদি (দৈর্ঘ্য && (!contents->info || !contents->pos))
  {
    contents->দৈর্ঘ্য = 0;
    ফেরত মিথ্যা;
  }

  ফেরত সত্য;
}

HB_WASM_API (bool_t, বাফার_set_contents) (HB_WASM_EXEC_ENV
					   ptr_d(বাফার_t, বাফার),
					   ptr_d(ধ্রুবক বাফার_contents_t, contents))
{
  HB_REF2OBJ (বাফার);
  HB_PTR_PARAM (বাফার_contents_t, contents);
  যদি (unlikely (!contents))
    ফেরত মিথ্যা;

  unsigned দৈর্ঘ্য = contents->দৈর্ঘ্য;
  unsigned bytes;
  যদি (unlikely (hb_unsigned_mul_overflows (দৈর্ঘ্য, আকার (বাফার->info[0]), &bytes)))
    ফেরত মিথ্যা;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, bytes) ? addr_app_to_native (contents->info) : nullptr);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, bytes) ? addr_app_to_native (contents->pos) : nullptr);
  যদি (unlikely (!info || !pos))
    ফেরত মিথ্যা;

  যদি (unlikely (!বাফার->resize (দৈর্ঘ্য)))
    ফেরত মিথ্যা;

  যদি (!বাফার->have_positions)
    বাফার->clear_positions (); /* This is wasteful. */

  hb_memcpy (বাফার->info, info, bytes);
  hb_memcpy (বাফার->pos, pos, bytes);
  বাফার->len = দৈর্ঘ্য;

  ফেরত সত্য;
}

HB_WASM_API (direction_t, বাফার_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(বাফার_t, বাফার))
{
  HB_REF2OBJ (বাফার);

  ফেরত (direction_t) hb_বাফার_get_direction (বাফার);
}

HB_WASM_API (script_t, বাফার_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(বাফার_t, বাফার))
{
  HB_REF2OBJ (বাফার);

  ফেরত hb_script_to_iso15924_tag (hb_বাফার_get_script (বাফার));
}

HB_WASM_API (শূন্য, বাফার_reverse) (HB_WASM_EXEC_ENV
				    ptr_d(বাফার_t, বাফার))
{
  HB_REF2OBJ (বাফার);

  hb_বাফার_reverse (বাফার);
}

HB_WASM_API (শূন্য, বাফার_reverse_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(বাফার_t, বাফার))
{
  HB_REF2OBJ (বাফার);

  hb_বাফার_reverse_clusters (বাফার);
}

}}

#endif /* HB_WASM_API_BUFFER_HH */

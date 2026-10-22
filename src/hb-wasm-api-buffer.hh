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

#ifndef HB_WASM_API_BUFFER_HH
#define HB_WASM_API_BUFFER_HH

#অন্তর্ভাৱ "hb-wasm-api.hh"

#অন্তর্ভাৱ "hb-buffer.hh"

নামাকাশ hb {
নামাকাশ wasm {

static_assert (পরিমাণ (glyph_info_t) == পরিমাণ (hb_glyph_info_t), "");
static_assert (পরিমাণ (glyph_position_t) == পরিমাণ (hb_glyph_position_t), "");

HB_WASM_API (bool_t, buffer_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(buffer_contents_t, contents),
					       uint32_t size)
{
  HB_PTR_PARAM (buffer_contents_t, contents);
  যদি (unlikely (!contents))
    ঘূৰাই মিছা;

  যদি (size <= contents->length)
    ঘূৰাই সঁচা;

  unsigned bytes;
  যদি (hb_unsigned_mul_overflows (size, পরিমাণ (glyph_info_t), &bytes))
    ঘূৰাই মিছা;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->length);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->length);

  যদি (unlikely (!info || !pos))
    ঘূৰাই মিছা;

  glyph_info_t *new_info = nullptr;
  uint32_t new_inforef = module_malloc (bytes, (শূন্য **) &new_info);
  glyph_position_t *new_pos = nullptr;
  uint32_t new_posref = module_malloc (bytes, (শূন্য **) &new_pos);

  unsigned old_bytes = contents->length * পরিমাণ (glyph_info_t);
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
    contents->length = size;
    ঘূৰাই সঁচা;
  }

  ঘূৰাই মিছা;
}

HB_WASM_API (শূন্য, buffer_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(buffer_contents_t, contents))
{
  HB_PTR_PARAM (buffer_contents_t, contents);
  যদি (unlikely (!contents))
    ঘূৰাই;

  module_free (contents->info);
  module_free (contents->pos);

  contents->info = nullref;
  contents->pos = nullref;
  contents->length = 0;
}

HB_WASM_API (bool_t, buffer_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(buffer_t, buffer),
					    ptr_d(buffer_contents_t, contents))
{
  HB_REF2OBJ (buffer);
  HB_PTR_PARAM (buffer_contents_t, contents);
  যদি (unlikely (!contents))
    ঘূৰাই মিছা;

  যদি (buffer->have_output)
    buffer->সমকাল ();
  যদি (!buffer->have_positions)
    buffer->clear_positions ();

  unsigned length = buffer->len;

  যদি (length <= contents->length)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, length);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, length);

    যদি (unlikely (!info || !pos))
    {
      contents->length = 0;
      ঘূৰাই মিছা;
    }

    unsigned bytes = length * পরিমাণ (hb_glyph_info_t);
    hb_memcpy (info, buffer->info, bytes);
    hb_memcpy (pos, buffer->pos, bytes);

    ঘূৰাই সঁচা;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->length = length;
  unsigned bytes = length * পরিমাণ (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_data (module_inst, (ধ্রুৱ ৱর্ণ *) buffer->info, bytes);
  contents->pos = wasm_runtime_module_dup_data (module_inst, (ধ্রুৱ ৱর্ণ *) buffer->pos, bytes);

  যদি (length && (!contents->info || !contents->pos))
  {
    contents->length = 0;
    ঘূৰাই মিছা;
  }

  ঘূৰাই সঁচা;
}

HB_WASM_API (bool_t, buffer_set_contents) (HB_WASM_EXEC_ENV
					   ptr_d(buffer_t, buffer),
					   ptr_d(ধ্রুৱ buffer_contents_t, contents))
{
  HB_REF2OBJ (buffer);
  HB_PTR_PARAM (buffer_contents_t, contents);
  যদি (unlikely (!contents))
    ঘূৰাই মিছা;

  unsigned length = contents->length;
  unsigned bytes;
  যদি (unlikely (hb_unsigned_mul_overflows (length, পরিমাণ (buffer->info[0]), &bytes)))
    ঘূৰাই মিছা;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, bytes) ? addr_app_to_native (contents->info) : nullptr);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, bytes) ? addr_app_to_native (contents->pos) : nullptr);
  যদি (unlikely (!info || !pos))
    ঘূৰাই মিছা;

  যদি (unlikely (!buffer->resize (length)))
    ঘূৰাই মিছা;

  যদি (!buffer->have_positions)
    buffer->clear_positions (); /* This is wasteful. */

  hb_memcpy (buffer->info, info, bytes);
  hb_memcpy (buffer->pos, pos, bytes);
  buffer->len = length;

  ঘূৰাই সঁচা;
}

HB_WASM_API (direction_t, buffer_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(buffer_t, buffer))
{
  HB_REF2OBJ (buffer);

  ঘূৰাই (direction_t) hb_buffer_get_direction (buffer);
}

HB_WASM_API (script_t, buffer_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(buffer_t, buffer))
{
  HB_REF2OBJ (buffer);

  ঘূৰাই hb_script_to_iso15924_tag (hb_buffer_get_script (buffer));
}

HB_WASM_API (শূন্য, buffer_reverse) (HB_WASM_EXEC_ENV
				    ptr_d(buffer_t, buffer))
{
  HB_REF2OBJ (buffer);

  hb_buffer_reverse (buffer);
}

HB_WASM_API (শূন্য, buffer_reverse_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(buffer_t, buffer))
{
  HB_REF2OBJ (buffer);

  hb_buffer_reverse_clusters (buffer);
}

}}

#endif /* HB_WASM_API_BUFFER_HH */

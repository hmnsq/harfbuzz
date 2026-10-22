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

#ضمن "hb-wasm-api.hh"

#ضمن "hb-buffer.hh"

نطاق hb {
نطاق wasm {

static_assert (حجم (glyph_info_t) == حجم (hb_glyph_info_t), "");
static_assert (حجم (glyph_position_t) == حجم (hb_glyph_position_t), "");

HB_WASM_API (bool_t, buffer_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(buffer_contents_t, contents),
					       uint32_t size)
{
  HB_PTR_PARAM (buffer_contents_t, contents);
  𐤀𐤌 (unlikely (!contents))
    أرجع خطأ;

  𐤀𐤌 (size <= contents->length)
    أرجع true;

  unsigned bytes;
  𐤀𐤌 (hb_unsigned_mul_overflows (size, حجم (glyph_info_t), &bytes))
    أرجع خطأ;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->length);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->length);

  𐤀𐤌 (unlikely (!info || !pos))
    أرجع خطأ;

  glyph_info_t *new_info = nullptr;
  uint32_t new_inforef = module_malloc (bytes, (فراغ **) &new_info);
  glyph_position_t *new_pos = nullptr;
  uint32_t new_posref = module_malloc (bytes, (فراغ **) &new_pos);

  unsigned old_bytes = contents->length * حجم (glyph_info_t);
  𐤀𐤌 (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_bytes);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  𐤀𐤌 (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_bytes);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  𐤀𐤌 (likely (new_info && new_pos))
  {
    contents->length = size;
    أرجع true;
  }

  أرجع خطأ;
}

HB_WASM_API (فراغ, buffer_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(buffer_contents_t, contents))
{
  HB_PTR_PARAM (buffer_contents_t, contents);
  𐤀𐤌 (unlikely (!contents))
    أرجع;

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
  𐤀𐤌 (unlikely (!contents))
    أرجع خطأ;

  𐤀𐤌 (buffer->have_output)
    buffer->متزامن ();
  𐤀𐤌 (!buffer->have_positions)
    buffer->clear_positions ();

  unsigned length = buffer->len;

  𐤀𐤌 (length <= contents->length)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, length);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, length);

    𐤀𐤌 (unlikely (!info || !pos))
    {
      contents->length = 0;
      أرجع خطأ;
    }

    unsigned bytes = length * حجم (hb_glyph_info_t);
    hb_memcpy (info, buffer->info, bytes);
    hb_memcpy (pos, buffer->pos, bytes);

    أرجع true;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->length = length;
  unsigned bytes = length * حجم (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_data (module_inst, (ثابت حرف *) buffer->info, bytes);
  contents->pos = wasm_runtime_module_dup_data (module_inst, (ثابت حرف *) buffer->pos, bytes);

  𐤀𐤌 (length && (!contents->info || !contents->pos))
  {
    contents->length = 0;
    أرجع خطأ;
  }

  أرجع true;
}

HB_WASM_API (bool_t, buffer_set_contents) (HB_WASM_EXEC_ENV
					   ptr_d(buffer_t, buffer),
					   ptr_d(ثابت buffer_contents_t, contents))
{
  HB_REF2OBJ (buffer);
  HB_PTR_PARAM (buffer_contents_t, contents);
  𐤀𐤌 (unlikely (!contents))
    أرجع خطأ;

  unsigned length = contents->length;
  unsigned bytes;
  𐤀𐤌 (unlikely (hb_unsigned_mul_overflows (length, حجم (buffer->info[0]), &bytes)))
    أرجع خطأ;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, bytes) ? addr_app_to_native (contents->info) : nullptr);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, bytes) ? addr_app_to_native (contents->pos) : nullptr);
  𐤀𐤌 (unlikely (!info || !pos))
    أرجع خطأ;

  𐤀𐤌 (unlikely (!buffer->resize (length)))
    أرجع خطأ;

  𐤀𐤌 (!buffer->have_positions)
    buffer->clear_positions (); /* This is wasteful. */

  hb_memcpy (buffer->info, info, bytes);
  hb_memcpy (buffer->pos, pos, bytes);
  buffer->len = length;

  أرجع true;
}

HB_WASM_API (direction_t, buffer_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(buffer_t, buffer))
{
  HB_REF2OBJ (buffer);

  أرجع (direction_t) hb_buffer_get_direction (buffer);
}

HB_WASM_API (script_t, buffer_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(buffer_t, buffer))
{
  HB_REF2OBJ (buffer);

  أرجع hb_script_to_iso15924_tag (hb_buffer_get_script (buffer));
}

HB_WASM_API (فراغ, buffer_reverse) (HB_WASM_EXEC_ENV
				    ptr_d(buffer_t, buffer))
{
  HB_REF2OBJ (buffer);

  hb_buffer_reverse (buffer);
}

HB_WASM_API (فراغ, buffer_reverse_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(buffer_t, buffer))
{
  HB_REF2OBJ (buffer);

  hb_buffer_reverse_clusters (buffer);
}

}}

#endif /* HB_WASM_API_BUFFER_HH */

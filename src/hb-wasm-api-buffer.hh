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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_ਬਫਰ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_ਬਫਰ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"

ਨਾਮਾਕਾਸ਼ hb {
ਨਾਮਾਕਾਸ਼ wasm {

static_assert (ਆਕਾਰ (glyph_info_t) == ਆਕਾਰ (hb_glyph_info_t), "");
static_assert (ਆਕਾਰ (glyph_position_t) == ਆਕਾਰ (hb_glyph_position_t), "");

HB_WASM_API (bool_t, ਬਫਰ_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(ਬਫਰ_contents_t, contents),
					       uint32_t ਮਾਪ)
{
  HB_PTR_PARAM (ਬਫਰ_contents_t, contents);
  ਜੇ (unlikely (!contents))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (ਮਾਪ <= contents->ਲੰਮਾਈ)
    ਵਾਪਸ ਸੱਚ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ;
  ਜੇ (hb_unsigned_mul_overflows (ਮਾਪ, ਆਕਾਰ (glyph_info_t), &ਬਾਈਟਾਂ))
    ਵਾਪਸ ਝੂਠ;

  glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, contents->ਲੰਮਾਈ);
  glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, contents->ਲੰਮਾਈ);

  ਜੇ (unlikely (!info || !pos))
    ਵਾਪਸ ਝੂਠ;

  glyph_info_t *new_info = ਖਾਲੀ_ਸੂਚਕ;
  uint32_t new_inforef = module_malloc (ਬਾਈਟਾਂ, (ਖਾਲੀ **) &new_info);
  glyph_position_t *new_pos = ਖਾਲੀ_ਸੂਚਕ;
  uint32_t new_posref = module_malloc (ਬਾਈਟਾਂ, (ਖਾਲੀ **) &new_pos);

  ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਬਾਈਟਾਂ = contents->ਲੰਮਾਈ * ਆਕਾਰ (glyph_info_t);
  ਜੇ (likely (new_inforef))
  {
    hb_memcpy (new_info, info, old_ਬਾਈਟਾਂ);
    module_free (contents->info);
    contents->info = new_inforef;
  }
  ਜੇ (likely (new_posref))
  {
    hb_memcpy (new_pos, pos, old_ਬਾਈਟਾਂ);
    module_free (contents->pos);
    contents->pos = new_posref;
  }

  ਜੇ (likely (new_info && new_pos))
  {
    contents->ਲੰਮਾਈ = ਮਾਪ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਵਾਪਸ ਝੂਠ;
}

HB_WASM_API (ਖਾਲੀ, ਬਫਰ_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(ਬਫਰ_contents_t, contents))
{
  HB_PTR_PARAM (ਬਫਰ_contents_t, contents);
  ਜੇ (unlikely (!contents))
    ਵਾਪਸ;

  module_free (contents->info);
  module_free (contents->pos);

  contents->info = nullref;
  contents->pos = nullref;
  contents->ਲੰਮਾਈ = 0;
}

HB_WASM_API (bool_t, ਬਫਰ_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(ਬਫਰ_t, ਬਫਰ),
					    ptr_d(ਬਫਰ_contents_t, contents))
{
  HB_REF2OBJ (ਬਫਰ);
  HB_PTR_PARAM (ਬਫਰ_contents_t, contents);
  ਜੇ (unlikely (!contents))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (ਬਫਰ->have_output)
    ਬਫਰ->ਸਮਕਾਲੀ ();
  ਜੇ (!ਬਫਰ->have_positions)
    ਬਫਰ->clear_positions ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = ਬਫਰ->len;

  ਜੇ (ਲੰਮਾਈ <= contents->ਲੰਮਾਈ)
  {
    glyph_info_t *info = HB_ARRAY_APP2NATIVE (glyph_info_t, contents->info, ਲੰਮਾਈ);
    glyph_position_t *pos = HB_ARRAY_APP2NATIVE (glyph_position_t, contents->pos, ਲੰਮਾਈ);

    ਜੇ (unlikely (!info || !pos))
    {
      contents->ਲੰਮਾਈ = 0;
      ਵਾਪਸ ਝੂਠ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ = ਲੰਮਾਈ * ਆਕਾਰ (hb_glyph_info_t);
    hb_memcpy (info, ਬਫਰ->info, ਬਾਈਟਾਂ);
    hb_memcpy (pos, ਬਫਰ->pos, ਬਾਈਟਾਂ);

    ਵਾਪਸ ਸੱਚ;
  }

  module_free (contents->info);
  module_free (contents->pos);

  contents->ਲੰਮਾਈ = ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ = ਲੰਮਾਈ * ਆਕਾਰ (hb_glyph_info_t);
  contents->info = wasm_runtime_module_dup_ਅੰਕੜੇ (module_inst, (ਅਚਲ ਅੱਖਰ *) ਬਫਰ->info, ਬਾਈਟਾਂ);
  contents->pos = wasm_runtime_module_dup_ਅੰਕੜੇ (module_inst, (ਅਚਲ ਅੱਖਰ *) ਬਫਰ->pos, ਬਾਈਟਾਂ);

  ਜੇ (ਲੰਮਾਈ && (!contents->info || !contents->pos))
  {
    contents->ਲੰਮਾਈ = 0;
    ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ ਸੱਚ;
}

HB_WASM_API (bool_t, ਬਫਰ_ਸੈੱਟ_contents) (HB_WASM_EXEC_ENV
					   ptr_d(ਬਫਰ_t, ਬਫਰ),
					   ptr_d(ਅਚਲ ਬਫਰ_contents_t, contents))
{
  HB_REF2OBJ (ਬਫਰ);
  HB_PTR_PARAM (ਬਫਰ_contents_t, contents);
  ਜੇ (unlikely (!contents))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = contents->ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ;
  ਜੇ (unlikely (hb_unsigned_mul_overflows (ਲੰਮਾਈ, ਆਕਾਰ (ਬਫਰ->info[0]), &ਬਾਈਟਾਂ)))
    ਵਾਪਸ ਝੂਠ;

  glyph_info_t *info = (glyph_info_t *) (validate_app_addr (contents->info, ਬਾਈਟਾਂ) ? addr_app_ਮੰਜ਼ਿਲ_native (contents->info) : ਖਾਲੀ_ਸੂਚਕ);
  glyph_position_t *pos = (glyph_position_t *) (validate_app_addr (contents->pos, ਬਾਈਟਾਂ) ? addr_app_ਮੰਜ਼ਿਲ_native (contents->pos) : ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (!info || !pos))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (unlikely (!ਬਫਰ->resize (ਲੰਮਾਈ)))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (!ਬਫਰ->have_positions)
    ਬਫਰ->clear_positions (); /* This is wasteful. */

  hb_memcpy (ਬਫਰ->info, info, ਬਾਈਟਾਂ);
  hb_memcpy (ਬਫਰ->pos, pos, ਬਾਈਟਾਂ);
  ਬਫਰ->len = ਲੰਮਾਈ;

  ਵਾਪਸ ਸੱਚ;
}

HB_WASM_API (direction_t, ਬਫਰ_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(ਬਫਰ_t, ਬਫਰ))
{
  HB_REF2OBJ (ਬਫਰ);

  ਵਾਪਸ (direction_t) hb_ਬਫਰ_get_direction (ਬਫਰ);
}

HB_WASM_API (script_t, ਬਫਰ_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(ਬਫਰ_t, ਬਫਰ))
{
  HB_REF2OBJ (ਬਫਰ);

  ਵਾਪਸ hb_script_ਮੰਜ਼ਿਲ_iso15924_ਟੈਗ (hb_ਬਫਰ_get_script (ਬਫਰ));
}

HB_WASM_API (ਖਾਲੀ, ਬਫਰ_ਉਲਟ) (HB_WASM_EXEC_ENV
				    ptr_d(ਬਫਰ_t, ਬਫਰ))
{
  HB_REF2OBJ (ਬਫਰ);

  hb_ਬਫਰ_ਉਲਟ (ਬਫਰ);
}

HB_WASM_API (ਖਾਲੀ, ਬਫਰ_ਉਲਟ_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(ਬਫਰ_t, ਬਫਰ))
{
  HB_REF2OBJ (ਬਫਰ);

  hb_ਬਫਰ_ਉਲਟ_clusters (ਬਫਰ);
}

}}

#ਜੇ_ਅੰਤ /* HB_WASM_API_BUFFER_HH */

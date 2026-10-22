/*
 * Copyright © 2016  Google, Inc.
 * Copyright © 2018  Ebrahim Byagowi
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
 *
 * Google Author(s): Sascha Brawer
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_COLOR_CPAL_CPAL_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_COLOR_CPAL_CPAL_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_CPAL HB_ਟੈਗ('C','P','A','L')

ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ CPALV1_ਪੂਛ
{
  ਮਿੱਤਰ ਬਣਤਰ CPAL;

  ਨਿਜੀ:
  hb_ot_color_palette_flags_t get_palette_flags (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
						 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ,
						 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਗਿਣਤੀ) ਅਚਲ
  {
    ਜੇ (!paletteFlagsZ) ਵਾਪਸ HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    ਵਾਪਸ (hb_ot_color_palette_flags_t) (uint32_t)
	   (ਅਧਾਰ+paletteFlagsZ).as_array (palette_ਗਿਣਤੀ)[palette_ਸੂਚਕਾਂਕ];
  }

  hb_ot_name_id_t get_palette_name_id (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਗਿਣਤੀ) ਅਚਲ
  {
    ਜੇ (!paletteLabelsZ) ਵਾਪਸ HB_OT_NAME_ID_INVALID;
    ਵਾਪਸ (ਅਧਾਰ+paletteLabelsZ).as_array (palette_ਗਿਣਤੀ)[palette_ਸੂਚਕਾਂਕ];
  }

  hb_ot_name_id_t get_color_name_id (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਸੂਚਕਾਂਕ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਗਿਣਤੀ) ਅਚਲ
  {
    ਜੇ (!colorLabelsZ) ਵਾਪਸ HB_OT_NAME_ID_INVALID;
    ਵਾਪਸ (ਅਧਾਰ+colorLabelsZ).as_array (color_ਗਿਣਤੀ)[color_ਸੂਚਕਾਂਕ];
  }

  ਜਨਤਕ:
  ਖਾਲੀ collect_name_ids (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ palette_ਗਿਣਤੀ,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ color_ਗਿਣਤੀ,
                         ਅਚਲ hb_map_t *color_ਸੂਚਕਾਂਕ_map,
                         hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (paletteLabelsZ)
    {
      + (ਅਧਾਰ+paletteLabelsZ).as_array (palette_ਗਿਣਤੀ)
      | hb_sink (nameids_ਮੰਜ਼ਿਲ_retain)
      ;
    }

    ਜੇ (colorLabelsZ)
    {
      ਅਚਲ hb_array_t<ਅਚਲ NameID> colorLabels = (ਅਧਾਰ+colorLabelsZ).as_array (color_ਗਿਣਤੀ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < color_ਗਿਣਤੀ; i++)
      {
        ਜੇ (!color_ਸੂਚਕਾਂਕ_map->has (i)) ਜਾਰੀ;
        nameids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (colorLabels[i]);
      }
    }
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ palette_ਗਿਣਤੀ,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ color_ਗਿਣਤੀ,
                  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                  ਅਚਲ hb_map_t *color_ਸੂਚਕਾਂਕ_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->allocate_ਮਾਪ<CPALV1_ਪੂਛ> (static_ਮਾਪ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->paletteFlagsZ = 0;
    ਜੇ (paletteFlagsZ)
      ਬਾਹਰਲਾ->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, ਅਧਾਰ, 0, hb_serialize_context_t::ਸਿਰ, palette_ਗਿਣਤੀ);

    ਬਾਹਰਲਾ->paletteLabelsZ = 0;
    ਜੇ (paletteLabelsZ)
      ਬਾਹਰਲਾ->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, ਅਧਾਰ, 0, hb_serialize_context_t::ਸਿਰ, palette_ਗਿਣਤੀ);

    ਅਚਲ hb_array_t<ਅਚਲ NameID> colorLabels = (ਅਧਾਰ+colorLabelsZ).as_array (color_ਗਿਣਤੀ);
    ਜੇ (colorLabelsZ)
    {
      c->push ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < color_ਗਿਣਤੀ; i++)
      {
        ਜੇ (!color_ਸੂਚਕਾਂਕ_map->has (i)) ਜਾਰੀ;
        ਜੇ (!c->copy<NameID> (colorLabels[i]))
        {
          c->ਕੱਢੋ_discard ();
          return_trace (ਝੂਠ);
        }
      }
      c->ਜੋੜੋ_link (ਬਾਹਰਲਾ->colorLabelsZ, c->ਕੱਢੋ_pack ());
    }
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਗਿਣਤੀ,
		 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਗਿਣਤੀ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  (!paletteFlagsZ  || (ਅਧਾਰ+paletteFlagsZ).sanitize (c, palette_ਗਿਣਤੀ)) &&
		  (!paletteLabelsZ || (ਅਧਾਰ+paletteLabelsZ).sanitize (c, palette_ਗਿਣਤੀ)) &&
		  (!colorLabelsZ   || (ਅਧਾਰ+colorLabelsZ).sanitize (c, color_ਗਿਣਤੀ)));
  }

  ਸੁਰੱਖਿਅਤ:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBUINT32 BGRAColor;

ਬਣਤਰ CPAL
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_CPAL;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਅੰਕ_Palettes; }

  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ min_ਮਾਪ + ਅੰਕ_Palettes * ਆਕਾਰ (color_ਰਿਕਾਰਡ_Indices_Z[0]); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_palette_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ ਅੰਕ_Palettes; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   get_color_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ ਅੰਕ_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ) ਅਚਲ
  { ਵਾਪਸ v1 ().get_palette_flags (ਇਹ, palette_ਸੂਚਕਾਂਕ, ਅੰਕ_Palettes); }

  hb_ot_name_id_t get_palette_name_id (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ) ਅਚਲ
  { ਵਾਪਸ v1 ().get_palette_name_id (ਇਹ, palette_ਸੂਚਕਾਂਕ, ਅੰਕ_Palettes); }

  hb_ot_name_id_t get_color_name_id (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਸੂਚਕਾਂਕ) ਅਚਲ
  { ਵਾਪਸ v1 ().get_color_name_id (ਇਹ, color_ਸੂਚਕਾਂਕ, ਅੰਕ_Colors); }

  hb_array_t<ਅਚਲ BGRAColor> get_palette_colors (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (unlikely (palette_ਸੂਚਕਾਂਕ >= ਅੰਕ_Palettes))
      ਵਾਪਸ hb_array_t<ਅਚਲ BGRAColor> ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ = color_ਰਿਕਾਰਡ_Indices_Z[palette_ਸੂਚਕਾਂਕ];
    hb_array_t<ਅਚਲ BGRAColor> all_colors ((ਇਹ+color_ਰਿਕਾਰਡਾਂ_Z).arrayZ, ਅੰਕ_Color_ਰਿਕਾਰਡਾਂ);
    ਵਾਪਸ all_colors.sub_array (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ, ਅੰਕ_Colors);
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_palette_colors (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  palette_ਸੂਚਕਾਂਕ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *color_ਗਿਣਤੀ, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ਅਚਲ
  {
    ਜੇ (unlikely (palette_ਸੂਚਕਾਂਕ >= ਅੰਕ_Palettes))
    {
      ਜੇ (color_ਗਿਣਤੀ) *color_ਗਿਣਤੀ = 0;
      ਵਾਪਸ 0;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ = color_ਰਿਕਾਰਡ_Indices_Z[palette_ਸੂਚਕਾਂਕ];
    hb_array_t<ਅਚਲ BGRAColor> all_colors ((ਇਹ+color_ਰਿਕਾਰਡਾਂ_Z).arrayZ, ਅੰਕ_Color_ਰਿਕਾਰਡਾਂ);
    hb_array_t<ਅਚਲ BGRAColor> palette_colors = all_colors.sub_array (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ,
								       ਅੰਕ_Colors);
    ਜੇ (color_ਗਿਣਤੀ && colors)
    {
      + palette_colors.sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, color_ਗਿਣਤੀ)
      | hb_sink (hb_array (colors, *color_ਗਿਣਤੀ))
      ;
    }
    ਵਾਪਸ ਅੰਕ_Colors;
  }

  ਖਾਲੀ collect_name_ids (ਅਚਲ hb_map_t *color_ਸੂਚਕਾਂਕ_map,
                         hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (ਇਹ, ਅੰਕ_Palettes, ਅੰਕ_Colors, color_ਸੂਚਕਾਂਕ_map, nameids_ਮੰਜ਼ਿਲ_retain);
    }
  }

  ਨਿਜੀ:
  ਅਚਲ CPALV1_ਪੂਛ& v1 () ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ == 0) ਵਾਪਸ Null (CPALV1_ਪੂਛ);
    hb_barrier ();
    ਵਾਪਸ StructAfter<CPALV1_ਪੂਛ> (*ਇਹ);
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  ਅਚਲ hb_array_t<ਅਚਲ HBUINT16> &color_ਰਿਕਾਰਡ_indices,
                  ਅਚਲ hb_array_t<ਅਚਲ BGRAColor> &color_ਰਿਕਾਰਡਾਂ,
                  ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ_for_layer,
                  ਅਚਲ hb_map_t& ਪਹਿਲਾ_color_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ,
                  ਅਚਲ hb_ਸੈੱਟ_t &retained_color_indices) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    // TODO(grieger): limit total final size.

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ idx : color_ਰਿਕਾਰਡ_indices)
    {
      hb_codepoint_t layer_ਸੂਚਕਾਂਕ = ਪਹਿਲਾ_color_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ[idx];

      HBUINT16 new_idx;
      new_idx = layer_ਸੂਚਕਾਂਕ * retained_color_indices.get_population ();
      ਜੇ (!c->copy<HBUINT16> (new_idx)) return_trace (ਝੂਠ);
    }

    c->push ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ : ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ_for_layer)
    {
      ਲਈ (hb_codepoint_t color_ਸੂਚਕਾਂਕ : retained_color_indices)
      {
        ਜੇ (!c->copy<BGRAColor> (color_ਰਿਕਾਰਡਾਂ[ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ + color_ਸੂਚਕਾਂਕ]))
        {
          c->ਕੱਢੋ_discard ();
          return_trace (ਝੂਠ);
        }
      }
    }

    c->ਜੋੜੋ_link (color_ਰਿਕਾਰਡਾਂ_Z, c->ਕੱਢੋ_pack ());
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (!ਅੰਕ_Palettes) return_trace (ਝੂਠ);

    ਅਚਲ hb_map_t *color_ਸੂਚਕਾਂਕ_map = &c->plan->colr_palettes;
    ਜੇ (color_ਸੂਚਕਾਂਕ_map->is_empty ()) return_trace (ਝੂਠ);

    hb_ਸੈੱਟ_t retained_color_indices;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : color_ਸੂਚਕਾਂਕ_map->ਕੁੰਜੀਆਂ ())
    {
      ਜੇ (_ == 0xFFFF) ਜਾਰੀ;
      retained_color_indices.ਜੋੜੋ (_);
    }
    ਜੇ (retained_color_indices.is_empty ()) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);


    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ = ਸੰਸਕਰਣ;
    ਬਾਹਰਲਾ->ਅੰਕ_Colors = retained_color_indices.get_population ();
    ਬਾਹਰਲਾ->ਅੰਕ_Palettes = ਅੰਕ_Palettes;

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ_for_layer;
    hb_map_t ਪਹਿਲਾ_color_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ਅਚਲ BGRAColor>, ਬਿਨਾਨਿਸ਼ਾਨੀ> retained_colors_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ;
    hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<BGRAColor>> layer_retained_colors;

    ਅਚਲ hb_array_t<ਅਚਲ BGRAColor> all_color_ਰਿਕਾਰਡਾਂ = (ਇਹ+color_ਰਿਕਾਰਡਾਂ_Z).as_array (ਅੰਕ_Color_ਰਿਕਾਰਡਾਂ);

    ਅਚਲ hb_array_t<ਅਚਲ HBUINT16> color_ਰਿਕਾਰਡ_Indices = color_ਰਿਕਾਰਡ_Indices_Z.as_array (ਅੰਕ_Palettes);
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ ਪਹਿਲਾ_color_ਰਿਕਾਰਡ_idx : color_ਰਿਕਾਰਡ_Indices)
    {
      ਜੇ (ਪਹਿਲਾ_color_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ.has (ਪਹਿਲਾ_color_ਰਿਕਾਰਡ_idx)) ਜਾਰੀ;

      hb_ਸਦਿਸ਼_t<BGRAColor> retained_colors;
      ਜੇ (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (ਝੂਠ);
      ਲਈ (hb_codepoint_t color_ਸੂਚਕਾਂਕ : retained_color_indices)
	retained_colors.push (all_color_ਰਿਕਾਰਡਾਂ[ਪਹਿਲਾ_color_ਰਿਕਾਰਡ_idx + color_ਸੂਚਕਾਂਕ]);

      ਬਿਨਾਨਿਸ਼ਾਨੀ layer_ਸੂਚਕਾਂਕ = ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ_for_layer.ਲੰਮਾਈ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ *candidate_layer_ਸੂਚਕਾਂਕ = ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (retained_colors_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ.has (retained_colors.as_array (), &candidate_layer_ਸੂਚਕਾਂਕ))
	layer_ਸੂਚਕਾਂਕ = *candidate_layer_ਸੂਚਕਾਂਕ;
      ਨਹੀਂਤਾਂ
      {
	ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ_for_layer.push (ਪਹਿਲਾ_color_ਰਿਕਾਰਡ_idx);
	ਜੇ (unlikely (!c->serializer->propagate_error (ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ_for_layer))) return_trace (ਝੂਠ);

	ਜੇ (unlikely (!layer_retained_colors.ਧੱਕੋ_or_fail (std::move (retained_colors)))) return_trace (ਝੂਠ);
	ਜੇ (unlikely (!retained_colors_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ.ਸੈੱਟ (layer_retained_colors.ਪੂਛ ().as_array (), layer_ਸੂਚਕਾਂਕ)))
	  return_trace (ਝੂਠ);
      }
      ਪਹਿਲਾ_color_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ.ਸੈੱਟ (ਪਹਿਲਾ_color_ਰਿਕਾਰਡ_idx, layer_ਸੂਚਕਾਂਕ);
      ਜੇ (unlikely (!c->serializer->propagate_error (ਪਹਿਲਾ_color_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ))) return_trace (ਝੂਠ);
    }

    ਬਾਹਰਲਾ->ਅੰਕ_Color_ਰਿਕਾਰਡਾਂ = ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ_for_layer.ਲੰਮਾਈ
                           * retained_color_indices.get_population ();

    ਜੇ (!ਬਾਹਰਲਾ->serialize (c->serializer,
                         color_ਰਿਕਾਰਡ_Indices,
                         all_color_ਰਿਕਾਰਡਾਂ,
                         ਪਹਿਲਾ_color_ਸੂਚਕਾਂਕ_for_layer,
                         ਪਹਿਲਾ_color_ਮੰਜ਼ਿਲ_layer_ਸੂਚਕਾਂਕ,
                         retained_color_indices))
      return_trace (ਝੂਠ);

    ਜੇ (ਸੰਸਕਰਣ == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, ਅੰਕ_Palettes, ਅੰਕ_Colors, ਇਹ, color_ਸੂਚਕਾਂਕ_map));
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  (ਇਹ+color_ਰਿਕਾਰਡਾਂ_Z).sanitize (c, ਅੰਕ_Color_ਰਿਕਾਰਡਾਂ) &&
		  color_ਰਿਕਾਰਡ_Indices_Z.sanitize (c, ਅੰਕ_Palettes) &&
		  (ਸੰਸਕਰਣ == 0 || v1 ().sanitize (c, ਇਹ, ਅੰਕ_Palettes, ਅੰਕ_Colors)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;		/* Table version number */
  /* Version 0 */
  HBUINT16	ਅੰਕ_Colors;		/* Number of colors in each palette. */
  HBUINT16	ਅੰਕ_Palettes;		/* Number of palettes in the table. */
  HBUINT16	ਅੰਕ_Color_ਰਿਕਾਰਡਾਂ;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<BGRAColor>>
		color_ਰਿਕਾਰਡਾਂ_Z;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  Unsized_Array_ਦਾ<HBUINT16>
		color_ਰਿਕਾਰਡ_Indices_Z;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (12, color_ਰਿਕਾਰਡ_Indices_Z);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_COLOR_CPAL_CPAL_HH */

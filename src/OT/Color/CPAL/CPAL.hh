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

#ifndef OT_COLOR_CPAL_CPAL_HH
#define OT_COLOR_CPAL_CPAL_HH

#અન્તર્ભાવ "../../../hb-open-type.hh"
#અન્તર્ભાવ "../../../hb-ot-color.h"
#અન્તર્ભાવ "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

નામાકાશ OT {


સંરચના CPALV1Tail
{
  friend સંરચના CPAL;

  ખાનગી:
  hb_ot_color_palette_flags_t get_palette_flags (અચલ શૂન્ય *base,
						 unsigned પૂર્ણાંક palette_index,
						 unsigned પૂર્ણાંક palette_ગણતરી) અચલ
  {
    જો (!paletteFlagsZ) પાછા HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    પાછા (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_ગણતરી)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (અચલ શૂન્ય *base,
				       unsigned પૂર્ણાંક palette_index,
				       unsigned પૂર્ણાંક palette_ગણતરી) અચલ
  {
    જો (!paletteLabelsZ) પાછા HB_OT_NAME_ID_INVALID;
    પાછા (base+paletteLabelsZ).as_array (palette_ગણતરી)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (અચલ શૂન્ય *base,
				     unsigned પૂર્ણાંક color_index,
				     unsigned પૂર્ણાંક color_ગણતરી) અચલ
  {
    જો (!colorLabelsZ) પાછા HB_OT_NAME_ID_INVALID;
    પાછા (base+colorLabelsZ).as_array (color_ગણતરી)[color_index];
  }

  જાહેર:
  શૂન્ય collect_name_ids (અચલ શૂન્ય *base,
                         unsigned palette_ગણતરી,
                         unsigned color_ગણતરી,
                         અચલ hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) અચલ
  {
    જો (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_ગણતરી)
      | hb_sink (nameids_to_retain)
      ;
    }

    જો (colorLabelsZ)
    {
      અચલ hb_array_t<અચલ NameID> colorLabels = (base+colorLabelsZ).as_array (color_ગણતરી);
      માટે (unsigned i = 0; i < color_ગણતરી; i++)
      {
        જો (!color_index_map->has (i)) ચાલુ;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
                  unsigned palette_ગણતરી,
                  unsigned color_ગણતરી,
                  અચલ શૂન્ય *base,
                  અચલ hb_map_t *color_index_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    જો (unlikely (!out)) return_trace (ખોટું);

    out->paletteFlagsZ = 0;
    જો (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_ગણતરી);

    out->paletteLabelsZ = 0;
    જો (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_ગણતરી);

    અચલ hb_array_t<અચલ NameID> colorLabels = (base+colorLabelsZ).as_array (color_ગણતરી);
    જો (colorLabelsZ)
    {
      c->push ();
      માટે (unsigned i = 0; i < color_ગણતરી; i++)
      {
        જો (!color_index_map->has (i)) ચાલુ;
        જો (!c->copy<NameID> (colorLabels[i]))
        {
          c->કાઢો_discard ();
          return_trace (ખોટું);
        }
      }
      c->add_link (out->colorLabelsZ, c->કાઢો_pack ());
    }
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ શૂન્ય *base,
		 unsigned પૂર્ણાંક palette_ગણતરી,
		 unsigned પૂર્ણાંક color_ગણતરી) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_ગણતરી)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_ગણતરી)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_ગણતરી)));
  }

  સુરક્ષિત:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32To<UnsizedArrayOf<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32To<UnsizedArrayOf<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32To<UnsizedArrayOf<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  જાહેર:
  DEFINE_SIZE_STATIC (12);
};

પ્રકારવ્યાખ્યા HBUINT32 BGRAColor;

સંરચના CPAL
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  તાર્કિક has_data () અચલ { પાછા અંક_Palettes; }

  size_t get_size () અચલ
  { પાછા min_size + અંક_Palettes * કદ (colorRecordIndicesZ[0]); }

  unsigned પૂર્ણાંક get_palette_ગણતરી () અચલ { પાછા અંક_Palettes; }
  unsigned પૂર્ણાંક   get_color_ગણતરી () અચલ { પાછા અંક_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned પૂર્ણાંક palette_index) અચલ
  { પાછા v1 ().get_palette_flags (આ, palette_index, અંક_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned પૂર્ણાંક palette_index) અચલ
  { પાછા v1 ().get_palette_name_id (આ, palette_index, અંક_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned પૂર્ણાંક color_index) અચલ
  { પાછા v1 ().get_color_name_id (આ, color_index, અંક_Colors); }

  hb_array_t<અચલ BGRAColor> get_palette_colors (unsigned પૂર્ણાંક palette_index) અચલ
  {
    જો (unlikely (palette_index >= અંક_Palettes))
      પાછા hb_array_t<અચલ BGRAColor> ();
    unsigned પૂર્ણાંક start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<અચલ BGRAColor> all_colors ((આ+colorRecordsZ).arrayZ, અંક_Color_Records);
    પાછા all_colors.sub_array (start_index, અંક_Colors);
  }
  unsigned પૂર્ણાંક get_palette_colors (unsigned પૂર્ણાંક  palette_index,
				   unsigned પૂર્ણાંક  start_offset,
				   unsigned પૂર્ણાંક *color_ગણતરી, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) અચલ
  {
    જો (unlikely (palette_index >= અંક_Palettes))
    {
      જો (color_ગણતરી) *color_ગણતરી = 0;
      પાછા 0;
    }
    unsigned પૂર્ણાંક start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<અચલ BGRAColor> all_colors ((આ+colorRecordsZ).arrayZ, અંક_Color_Records);
    hb_array_t<અચલ BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       અંક_Colors);
    જો (color_ગણતરી && colors)
    {
      + palette_colors.sub_array (start_offset, color_ગણતરી)
      | hb_sink (hb_array (colors, *color_ગણતરી))
      ;
    }
    પાછા અંક_Colors;
  }

  શૂન્ય collect_name_ids (અચલ hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) અચલ
  {
    જો (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (આ, અંક_Palettes, અંક_Colors, color_index_map, nameids_to_retain);
    }
  }

  ખાનગી:
  અચલ CPALV1Tail& v1 () અચલ
  {
    જો (version == 0) પાછા Null (CPALV1Tail);
    hb_barrier ();
    પાછા StructAfter<CPALV1Tail> (*આ);
  }

  જાહેર:
  તાર્કિક serialize (hb_serialize_context_t *c,
                  અચલ hb_array_t<અચલ HBUINT16> &color_record_indices,
                  અચલ hb_array_t<અચલ BGRAColor> &color_records,
                  અચલ hb_vector_t<unsigned>& first_color_index_for_layer,
                  અચલ hb_map_t& first_color_to_layer_index,
                  અચલ hb_set_t &retained_color_indices) અચલ
  {
    TRACE_SERIALIZE (આ);

    // TODO(grieger): limit total final size.

    માટે (અચલ auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      જો (!c->copy<HBUINT16> (new_idx)) return_trace (ખોટું);
    }

    c->push ();
    માટે (unsigned first_color_index : first_color_index_for_layer)
    {
      માટે (hb_codepoint_t color_index : retained_color_indices)
      {
        જો (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->કાઢો_discard ();
          return_trace (ખોટું);
        }
      }
    }

    c->add_link (colorRecordsZ, c->કાઢો_pack ());
    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    જો (!અંક_Palettes) return_trace (ખોટું);

    અચલ hb_map_t *color_index_map = &c->plan->colr_palettes;
    જો (color_index_map->is_empty ()) return_trace (ખોટું);

    hb_set_t retained_color_indices;
    માટે (અચલ auto _ : color_index_map->ચાવીઓ ())
    {
      જો (_ == 0xFFFF) ચાલુ;
      retained_color_indices.add (_);
    }
    જો (retained_color_indices.is_empty ()) return_trace (ખોટું);

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);


    out->version = version;
    out->અંક_Colors = retained_color_indices.get_population ();
    out->અંક_Palettes = અંક_Palettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<અચલ BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    અચલ hb_array_t<અચલ BGRAColor> all_color_records = (આ+colorRecordsZ).as_array (અંક_Color_Records);

    અચલ hb_array_t<અચલ HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (અંક_Palettes);
    માટે (અચલ auto first_color_record_idx : colorRecordIndices)
    {
      જો (first_color_to_layer_index.has (first_color_record_idx)) ચાલુ;

      hb_vector_t<BGRAColor> retained_colors;
      જો (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (ખોટું);
      માટે (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      જો (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      નહીંતર
      {
	first_color_index_for_layer.push (first_color_record_idx);
	જો (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (ખોટું);

	જો (unlikely (!layer_retained_colors.ધકેલો_or_fail (std::move (retained_colors)))) return_trace (ખોટું);
	જો (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (ખોટું);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      જો (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (ખોટું);
    }

    out->અંક_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    જો (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (ખોટું);

    જો (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, અંક_Palettes, અંક_Colors, આ, color_index_map));
    }

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  (આ+colorRecordsZ).sanitize (c, અંક_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, અંક_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, આ, અંક_Palettes, અંક_Colors)));
  }

  સુરક્ષિત:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	અંક_Colors;		/* Number of colors in each palette. */
  HBUINT16	અંક_Palettes;		/* Number of palettes in the table. */
  HBUINT16	અંક_Color_Records;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  જાહેર:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

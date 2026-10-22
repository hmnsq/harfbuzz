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

#ಅನ್ತರ್ಭಾವ "../../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-color.h"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

ನಾಮಾಕಾಶ OT {


ಸಂರಚನಾ CPALV1Tail
{
  friend ಸಂರಚನಾ CPAL;

  ಗುಪ್ತ:
  hb_ot_color_palette_flags_t get_palette_flags (ಧ್ರುವ ಶೂನ್ಯ *base,
						 unsigned ಪೂರ್ಣಾಙ್ಕ palette_index,
						 unsigned ಪೂರ್ಣಾಙ್ಕ palette_ಎಣಿಕೆ) ಧ್ರುವ
  {
    ಯದಿ (!paletteFlagsZ) ಪ್ರತ್ಯಾಗಮ HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    ಪ್ರತ್ಯಾಗಮ (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_ಎಣಿಕೆ)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (ಧ್ರುವ ಶೂನ್ಯ *base,
				       unsigned ಪೂರ್ಣಾಙ್ಕ palette_index,
				       unsigned ಪೂರ್ಣಾಙ್ಕ palette_ಎಣಿಕೆ) ಧ್ರುವ
  {
    ಯದಿ (!paletteLabelsZ) ಪ್ರತ್ಯಾಗಮ HB_OT_NAME_ID_INVALID;
    ಪ್ರತ್ಯಾಗಮ (base+paletteLabelsZ).as_array (palette_ಎಣಿಕೆ)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (ಧ್ರುವ ಶೂನ್ಯ *base,
				     unsigned ಪೂರ್ಣಾಙ್ಕ color_index,
				     unsigned ಪೂರ್ಣಾಙ್ಕ color_ಎಣಿಕೆ) ಧ್ರುವ
  {
    ಯದಿ (!colorLabelsZ) ಪ್ರತ್ಯಾಗಮ HB_OT_NAME_ID_INVALID;
    ಪ್ರತ್ಯಾಗಮ (base+colorLabelsZ).as_array (color_ಎಣಿಕೆ)[color_index];
  }

  ಸಾರ್ವಜನಿಕ:
  ಶೂನ್ಯ collect_name_ids (ಧ್ರುವ ಶೂನ್ಯ *base,
                         unsigned palette_ಎಣಿಕೆ,
                         unsigned color_ಎಣಿಕೆ,
                         ಧ್ರುವ hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_ಎಣಿಕೆ)
      | hb_sink (nameids_to_retain)
      ;
    }

    ಯದಿ (colorLabelsZ)
    {
      ಧ್ರುವ hb_array_t<ಧ್ರುವ NameID> colorLabels = (base+colorLabelsZ).as_array (color_ಎಣಿಕೆ);
      ಪ್ರತಿ (unsigned i = 0; i < color_ಎಣಿಕೆ; i++)
      {
        ಯದಿ (!color_index_map->has (i)) ಅನುವರ್ತ;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  unsigned palette_ಎಣಿಕೆ,
                  unsigned color_ಎಣಿಕೆ,
                  ಧ್ರುವ ಶೂನ್ಯ *base,
                  ಧ್ರುವ hb_map_t *color_index_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->paletteFlagsZ = 0;
    ಯದಿ (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_ಎಣಿಕೆ);

    out->paletteLabelsZ = 0;
    ಯದಿ (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_ಎಣಿಕೆ);

    ಧ್ರುವ hb_array_t<ಧ್ರುವ NameID> colorLabels = (base+colorLabelsZ).as_array (color_ಎಣಿಕೆ);
    ಯದಿ (colorLabelsZ)
    {
      c->push ();
      ಪ್ರತಿ (unsigned i = 0; i < color_ಎಣಿಕೆ; i++)
      {
        ಯದಿ (!color_index_map->has (i)) ಅನುವರ್ತ;
        ಯದಿ (!c->copy<NameID> (colorLabels[i]))
        {
          c->ತೆಗೆ_discard ();
          return_trace (ಅಸತ್ಯ);
        }
      }
      c->add_link (out->colorLabelsZ, c->ತೆಗೆ_pack ());
    }
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ ಶೂನ್ಯ *base,
		 unsigned ಪೂರ್ಣಾಙ್ಕ palette_ಎಣಿಕೆ,
		 unsigned ಪೂರ್ಣಾಙ್ಕ color_ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_ಎಣಿಕೆ)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_ಎಣಿಕೆ)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_ಎಣಿಕೆ)));
  }

  ರಕ್ಷಿತ:
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

ಪ್ರಕಾರನಿರ್ದೇಶ HBUINT32 BGRAColor;

ಸಂರಚನಾ CPAL
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Palettes; }

  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ min_size + ಅಂಕೆ_Palettes * ಪರಿಮಾಣ (colorRecordIndicesZ[0]); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_palette_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Palettes; }
  unsigned ಪೂರ್ಣಾಙ್ಕ   get_color_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned ಪೂರ್ಣಾಙ್ಕ palette_index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ v1 ().get_palette_flags (ಏತತ್, palette_index, ಅಂಕೆ_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned ಪೂರ್ಣಾಙ್ಕ palette_index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ v1 ().get_palette_name_id (ಏತತ್, palette_index, ಅಂಕೆ_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned ಪೂರ್ಣಾಙ್ಕ color_index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ v1 ().get_color_name_id (ಏತತ್, color_index, ಅಂಕೆ_Colors); }

  hb_array_t<ಧ್ರುವ BGRAColor> get_palette_colors (unsigned ಪೂರ್ಣಾಙ್ಕ palette_index) ಧ್ರುವ
  {
    ಯದಿ (unlikely (palette_index >= ಅಂಕೆ_Palettes))
      ಪ್ರತ್ಯಾಗಮ hb_array_t<ಧ್ರುವ BGRAColor> ();
    unsigned ಪೂರ್ಣಾಙ್ಕ start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ಧ್ರುವ BGRAColor> all_colors ((ಏತತ್+colorRecordsZ).arrayZ, ಅಂಕೆ_Color_Records);
    ಪ್ರತ್ಯಾಗಮ all_colors.sub_array (start_index, ಅಂಕೆ_Colors);
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_palette_colors (unsigned ಪೂರ್ಣಾಙ್ಕ  palette_index,
				   unsigned ಪೂರ್ಣಾಙ್ಕ  start_offset,
				   unsigned ಪೂರ್ಣಾಙ್ಕ *color_ಎಣಿಕೆ, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ಧ್ರುವ
  {
    ಯದಿ (unlikely (palette_index >= ಅಂಕೆ_Palettes))
    {
      ಯದಿ (color_ಎಣಿಕೆ) *color_ಎಣಿಕೆ = 0;
      ಪ್ರತ್ಯಾಗಮ 0;
    }
    unsigned ಪೂರ್ಣಾಙ್ಕ start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ಧ್ರುವ BGRAColor> all_colors ((ಏತತ್+colorRecordsZ).arrayZ, ಅಂಕೆ_Color_Records);
    hb_array_t<ಧ್ರುವ BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       ಅಂಕೆ_Colors);
    ಯದಿ (color_ಎಣಿಕೆ && colors)
    {
      + palette_colors.sub_array (start_offset, color_ಎಣಿಕೆ)
      | hb_sink (hb_array (colors, *color_ಎಣಿಕೆ))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_Colors;
  }

  ಶೂನ್ಯ collect_name_ids (ಧ್ರುವ hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (ಏತತ್, ಅಂಕೆ_Palettes, ಅಂಕೆ_Colors, color_index_map, nameids_to_retain);
    }
  }

  ಗುಪ್ತ:
  ಧ್ರುವ CPALV1Tail& v1 () ಧ್ರುವ
  {
    ಯದಿ (version == 0) ಪ್ರತ್ಯಾಗಮ Null (CPALV1Tail);
    hb_barrier ();
    ಪ್ರತ್ಯಾಗಮ StructAfter<CPALV1Tail> (*ಏತತ್);
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  ಧ್ರುವ hb_array_t<ಧ್ರುವ HBUINT16> &color_record_indices,
                  ಧ್ರುವ hb_array_t<ಧ್ರುವ BGRAColor> &color_records,
                  ಧ್ರುವ hb_vector_t<unsigned>& first_color_index_for_layer,
                  ಧ್ರುವ hb_map_t& first_color_to_layer_index,
                  ಧ್ರುವ hb_set_t &retained_color_indices) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    // TODO(grieger): limit total final size.

    ಪ್ರತಿ (ಧ್ರುವ auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      ಯದಿ (!c->copy<HBUINT16> (new_idx)) return_trace (ಅಸತ್ಯ);
    }

    c->push ();
    ಪ್ರತಿ (unsigned first_color_index : first_color_index_for_layer)
    {
      ಪ್ರತಿ (hb_codepoint_t color_index : retained_color_indices)
      {
        ಯದಿ (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->ತೆಗೆ_discard ();
          return_trace (ಅಸತ್ಯ);
        }
      }
    }

    c->add_link (colorRecordsZ, c->ತೆಗೆ_pack ());
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (!ಅಂಕೆ_Palettes) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_map_t *color_index_map = &c->plan->colr_palettes;
    ಯದಿ (color_index_map->is_empty ()) return_trace (ಅಸತ್ಯ);

    hb_set_t retained_color_indices;
    ಪ್ರತಿ (ಧ್ರುವ auto _ : color_index_map->ಕೀಲಿಗಳು ())
    {
      ಯದಿ (_ == 0xFFFF) ಅನುವರ್ತ;
      retained_color_indices.add (_);
    }
    ಯದಿ (retained_color_indices.is_empty ()) return_trace (ಅಸತ್ಯ);

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);


    out->version = version;
    out->ಅಂಕೆ_Colors = retained_color_indices.get_population ();
    out->ಅಂಕೆ_Palettes = ಅಂಕೆ_Palettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ಧ್ರುವ BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    ಧ್ರುವ hb_array_t<ಧ್ರುವ BGRAColor> all_color_records = (ಏತತ್+colorRecordsZ).as_array (ಅಂಕೆ_Color_Records);

    ಧ್ರುವ hb_array_t<ಧ್ರುವ HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (ಅಂಕೆ_Palettes);
    ಪ್ರತಿ (ಧ್ರುವ auto first_color_record_idx : colorRecordIndices)
    {
      ಯದಿ (first_color_to_layer_index.has (first_color_record_idx)) ಅನುವರ್ತ;

      hb_vector_t<BGRAColor> retained_colors;
      ಯದಿ (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (ಅಸತ್ಯ);
      ಪ್ರತಿ (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      ಯದಿ (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      ಅನ್ಯಥಾ
      {
	first_color_index_for_layer.push (first_color_record_idx);
	ಯದಿ (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (ಅಸತ್ಯ);

	ಯದಿ (unlikely (!layer_retained_colors.ತಳ್ಳು_or_fail (std::move (retained_colors)))) return_trace (ಅಸತ್ಯ);
	ಯದಿ (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (ಅಸತ್ಯ);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      ಯದಿ (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (ಅಸತ್ಯ);
    }

    out->ಅಂಕೆ_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    ಯದಿ (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, ಅಂಕೆ_Palettes, ಅಂಕೆ_Colors, ಏತತ್, color_index_map));
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  (ಏತತ್+colorRecordsZ).sanitize (c, ಅಂಕೆ_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, ಅಂಕೆ_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, ಏತತ್, ಅಂಕೆ_Palettes, ಅಂಕೆ_Colors)));
  }

  ರಕ್ಷಿತ:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	ಅಂಕೆ_Colors;		/* Number of colors in each palette. */
  HBUINT16	ಅಂಕೆ_Palettes;		/* Number of palettes in the table. */
  HBUINT16	ಅಂಕೆ_Color_Records;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

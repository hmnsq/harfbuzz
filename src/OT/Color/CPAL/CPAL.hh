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

#অন্তর্ভাৱ "../../../hb-open-type.hh"
#অন্তর্ভাৱ "../../../hb-ot-color.h"
#অন্তর্ভাৱ "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

নামাকাশ OT {


সংরচনা CPALV1Tail
{
  friend সংরচনা CPAL;

  গুপ্ত:
  hb_ot_color_palette_flags_t get_palette_flags (ধ্রুৱ শূন্য *base,
						 unsigned পূর্ণাঙ্ক palette_index,
						 unsigned পূর্ণাঙ্ক palette_গুনতি) ধ্রুৱ
  {
    যদি (!paletteFlagsZ) ঘূৰাই HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    ঘূৰাই (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_গুনতি)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (ধ্রুৱ শূন্য *base,
				       unsigned পূর্ণাঙ্ক palette_index,
				       unsigned পূর্ণাঙ্ক palette_গুনতি) ধ্রুৱ
  {
    যদি (!paletteLabelsZ) ঘূৰাই HB_OT_NAME_ID_INVALID;
    ঘূৰাই (base+paletteLabelsZ).as_array (palette_গুনতি)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (ধ্রুৱ শূন্য *base,
				     unsigned পূর্ণাঙ্ক color_index,
				     unsigned পূর্ণাঙ্ক color_গুনতি) ধ্রুৱ
  {
    যদি (!colorLabelsZ) ঘূৰাই HB_OT_NAME_ID_INVALID;
    ঘূৰাই (base+colorLabelsZ).as_array (color_গুনতি)[color_index];
  }

  সার্ৱজনিক:
  শূন্য collect_name_ids (ধ্রুৱ শূন্য *base,
                         unsigned palette_গুনতি,
                         unsigned color_গুনতি,
                         ধ্রুৱ hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ধ্রুৱ
  {
    যদি (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_গুনতি)
      | hb_sink (nameids_to_retain)
      ;
    }

    যদি (colorLabelsZ)
    {
      ধ্রুৱ hb_array_t<ধ্রুৱ NameID> colorLabels = (base+colorLabelsZ).as_array (color_গুনতি);
      বাবে (unsigned i = 0; i < color_গুনতি; i++)
      {
        যদি (!color_index_map->has (i)) অনুৱর্ত;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  তার্কিক serialize (hb_serialize_context_t *c,
                  unsigned palette_গুনতি,
                  unsigned color_গুনতি,
                  ধ্রুৱ শূন্য *base,
                  ধ্রুৱ hb_map_t *color_index_map) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    যদি (unlikely (!out)) return_trace (মিছা);

    out->paletteFlagsZ = 0;
    যদি (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_গুনতি);

    out->paletteLabelsZ = 0;
    যদি (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_গুনতি);

    ধ্রুৱ hb_array_t<ধ্রুৱ NameID> colorLabels = (base+colorLabelsZ).as_array (color_গুনতি);
    যদি (colorLabelsZ)
    {
      c->push ();
      বাবে (unsigned i = 0; i < color_গুনতি; i++)
      {
        যদি (!color_index_map->has (i)) অনুৱর্ত;
        যদি (!c->copy<NameID> (colorLabels[i]))
        {
          c->বেৰ_কৰো_discard ();
          return_trace (মিছা);
        }
      }
      c->add_link (out->colorLabelsZ, c->বেৰ_কৰো_pack ());
    }
    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুৱ শূন্য *base,
		 unsigned পূর্ণাঙ্ক palette_গুনতি,
		 unsigned পূর্ণাঙ্ক color_গুনতি) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_গুনতি)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_গুনতি)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_গুনতি)));
  }

  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (12);
};

প্রকারনির্দেশ HBUINT32 BGRAColor;

সংরচনা CPAL
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই অঙ্ক_Palettes; }

  size_t get_size () ধ্রুৱ
  { ঘূৰাই min_size + অঙ্ক_Palettes * পরিমাণ (colorRecordIndicesZ[0]); }

  unsigned পূর্ণাঙ্ক get_palette_গুনতি () ধ্রুৱ { ঘূৰাই অঙ্ক_Palettes; }
  unsigned পূর্ণাঙ্ক   get_color_গুনতি () ধ্রুৱ { ঘূৰাই অঙ্ক_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned পূর্ণাঙ্ক palette_index) ধ্রুৱ
  { ঘূৰাই v1 ().get_palette_flags (এতত্, palette_index, অঙ্ক_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned পূর্ণাঙ্ক palette_index) ধ্রুৱ
  { ঘূৰাই v1 ().get_palette_name_id (এতত্, palette_index, অঙ্ক_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned পূর্ণাঙ্ক color_index) ধ্রুৱ
  { ঘূৰাই v1 ().get_color_name_id (এতত্, color_index, অঙ্ক_Colors); }

  hb_array_t<ধ্রুৱ BGRAColor> get_palette_colors (unsigned পূর্ণাঙ্ক palette_index) ধ্রুৱ
  {
    যদি (unlikely (palette_index >= অঙ্ক_Palettes))
      ঘূৰাই hb_array_t<ধ্রুৱ BGRAColor> ();
    unsigned পূর্ণাঙ্ক start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ধ্রুৱ BGRAColor> all_colors ((এতত্+colorRecordsZ).arrayZ, অঙ্ক_Color_Records);
    ঘূৰাই all_colors.sub_array (start_index, অঙ্ক_Colors);
  }
  unsigned পূর্ণাঙ্ক get_palette_colors (unsigned পূর্ণাঙ্ক  palette_index,
				   unsigned পূর্ণাঙ্ক  start_offset,
				   unsigned পূর্ণাঙ্ক *color_গুনতি, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ধ্রুৱ
  {
    যদি (unlikely (palette_index >= অঙ্ক_Palettes))
    {
      যদি (color_গুনতি) *color_গুনতি = 0;
      ঘূৰাই 0;
    }
    unsigned পূর্ণাঙ্ক start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ধ্রুৱ BGRAColor> all_colors ((এতত্+colorRecordsZ).arrayZ, অঙ্ক_Color_Records);
    hb_array_t<ধ্রুৱ BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       অঙ্ক_Colors);
    যদি (color_গুনতি && colors)
    {
      + palette_colors.sub_array (start_offset, color_গুনতি)
      | hb_sink (hb_array (colors, *color_গুনতি))
      ;
    }
    ঘূৰাই অঙ্ক_Colors;
  }

  শূন্য collect_name_ids (ধ্রুৱ hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ধ্রুৱ
  {
    যদি (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (এতত্, অঙ্ক_Palettes, অঙ্ক_Colors, color_index_map, nameids_to_retain);
    }
  }

  গুপ্ত:
  ধ্রুৱ CPALV1Tail& v1 () ধ্রুৱ
  {
    যদি (version == 0) ঘূৰাই Null (CPALV1Tail);
    hb_barrier ();
    ঘূৰাই StructAfter<CPALV1Tail> (*এতত্);
  }

  সার্ৱজনিক:
  তার্কিক serialize (hb_serialize_context_t *c,
                  ধ্রুৱ hb_array_t<ধ্রুৱ HBUINT16> &color_record_indices,
                  ধ্রুৱ hb_array_t<ধ্রুৱ BGRAColor> &color_records,
                  ধ্রুৱ hb_vector_t<unsigned>& first_color_index_for_layer,
                  ধ্রুৱ hb_map_t& first_color_to_layer_index,
                  ধ্রুৱ hb_set_t &retained_color_indices) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    // TODO(grieger): limit total final size.

    বাবে (ধ্রুৱ auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      যদি (!c->copy<HBUINT16> (new_idx)) return_trace (মিছা);
    }

    c->push ();
    বাবে (unsigned first_color_index : first_color_index_for_layer)
    {
      বাবে (hb_codepoint_t color_index : retained_color_indices)
      {
        যদি (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->বেৰ_কৰো_discard ();
          return_trace (মিছা);
        }
      }
    }

    c->add_link (colorRecordsZ, c->বেৰ_কৰো_pack ());
    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (!অঙ্ক_Palettes) return_trace (মিছা);

    ধ্রুৱ hb_map_t *color_index_map = &c->plan->colr_palettes;
    যদি (color_index_map->is_empty ()) return_trace (মিছা);

    hb_set_t retained_color_indices;
    বাবে (ধ্রুৱ auto _ : color_index_map->চাবিগুলি ())
    {
      যদি (_ == 0xFFFF) অনুৱর্ত;
      retained_color_indices.add (_);
    }
    যদি (retained_color_indices.is_empty ()) return_trace (মিছা);

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);


    out->version = version;
    out->অঙ্ক_Colors = retained_color_indices.get_population ();
    out->অঙ্ক_Palettes = অঙ্ক_Palettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ধ্রুৱ BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    ধ্রুৱ hb_array_t<ধ্রুৱ BGRAColor> all_color_records = (এতত্+colorRecordsZ).as_array (অঙ্ক_Color_Records);

    ধ্রুৱ hb_array_t<ধ্রুৱ HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (অঙ্ক_Palettes);
    বাবে (ধ্রুৱ auto first_color_record_idx : colorRecordIndices)
    {
      যদি (first_color_to_layer_index.has (first_color_record_idx)) অনুৱর্ত;

      hb_vector_t<BGRAColor> retained_colors;
      যদি (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (মিছা);
      বাবে (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      যদি (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      অন্যথা
      {
	first_color_index_for_layer.push (first_color_record_idx);
	যদি (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (মিছা);

	যদি (unlikely (!layer_retained_colors.ঠেলো_or_fail (std::move (retained_colors)))) return_trace (মিছা);
	যদি (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (মিছা);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      যদি (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (মিছা);
    }

    out->অঙ্ক_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    যদি (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (মিছা);

    যদি (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, অঙ্ক_Palettes, অঙ্ক_Colors, এতত্, color_index_map));
    }

    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  (এতত্+colorRecordsZ).sanitize (c, অঙ্ক_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, অঙ্ক_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, এতত্, অঙ্ক_Palettes, অঙ্ক_Colors)));
  }

  রক্ষিত:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	অঙ্ক_Colors;		/* Number of colors in each palette. */
  HBUINT16	অঙ্ক_Palettes;		/* Number of palettes in the table. */
  HBUINT16	অঙ্ক_Color_Records;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

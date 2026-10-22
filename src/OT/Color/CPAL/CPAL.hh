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

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-ot-color.h"
#ضمن "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

نطاق OT {


بنية CPALV1Tail
{
  friend بنية CPAL;

  خاص:
  hb_ot_color_palette_flags_t get_palette_flags (ثابت فراغ *base,
						 unsigned صحيح palette_index,
						 unsigned صحيح palette_count) ثابت
  {
    ܐܢ (!paletteFlagsZ) ܦܢܐ HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    ܦܢܐ (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_count)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (ثابت فراغ *base,
				       unsigned صحيح palette_index,
				       unsigned صحيح palette_count) ثابت
  {
    ܐܢ (!paletteLabelsZ) ܦܢܐ HB_OT_NAME_ID_INVALID;
    ܦܢܐ (base+paletteLabelsZ).as_array (palette_count)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (ثابت فراغ *base,
				     unsigned صحيح color_index,
				     unsigned صحيح color_count) ثابت
  {
    ܐܢ (!colorLabelsZ) ܦܢܐ HB_OT_NAME_ID_INVALID;
    ܦܢܐ (base+colorLabelsZ).as_array (color_count)[color_index];
  }

  عام:
  فراغ collect_name_ids (ثابت فراغ *base,
                         unsigned palette_count,
                         unsigned color_count,
                         ثابت hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    ܐܢ (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_count)
      | hb_sink (nameids_to_retain)
      ;
    }

    ܐܢ (colorLabelsZ)
    {
      ثابت hb_array_t<ثابت NameID> colorLabels = (base+colorLabelsZ).as_array (color_count);
      لكل (unsigned i = 0; i < color_count; i++)
      {
        ܐܢ (!color_index_map->has (i)) استمر;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
                  unsigned palette_count,
                  unsigned color_count,
                  ثابت فراغ *base,
                  ثابت hb_map_t *color_index_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    ܐܢ (unlikely (!out)) return_trace (ܕܓܠܐ);

    out->paletteFlagsZ = 0;
    ܐܢ (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_count);

    out->paletteLabelsZ = 0;
    ܐܢ (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_count);

    ثابت hb_array_t<ثابت NameID> colorLabels = (base+colorLabelsZ).as_array (color_count);
    ܐܢ (colorLabelsZ)
    {
      c->push ();
      لكل (unsigned i = 0; i < color_count; i++)
      {
        ܐܢ (!color_index_map->has (i)) استمر;
        ܐܢ (!c->copy<NameID> (colorLabels[i]))
        {
          c->pop_discard ();
          return_trace (ܕܓܠܐ);
        }
      }
      c->add_link (out->colorLabelsZ, c->pop_pack ());
    }
    return_trace (ܫܪܪܐ);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *base,
		 unsigned صحيح palette_count,
		 unsigned صحيح color_count) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_count)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_count)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_count)));
  }

  محمي:
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
  عام:
  DEFINE_SIZE_STATIC (12);
};

تعريفنوع HBUINT32 BGRAColor;

بنية CPAL
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  منطقي has_data () ثابت { ܦܢܐ numPalettes; }

  size_t get_size () ثابت
  { ܦܢܐ min_size + numPalettes * حجم (colorRecordIndicesZ[0]); }

  unsigned صحيح get_palette_count () ثابت { ܦܢܐ numPalettes; }
  unsigned صحيح   get_color_count () ثابت { ܦܢܐ numColors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned صحيح palette_index) ثابت
  { ܦܢܐ v1 ().get_palette_flags (هذا, palette_index, numPalettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned صحيح palette_index) ثابت
  { ܦܢܐ v1 ().get_palette_name_id (هذا, palette_index, numPalettes); }

  hb_ot_name_id_t get_color_name_id (unsigned صحيح color_index) ثابت
  { ܦܢܐ v1 ().get_color_name_id (هذا, color_index, numColors); }

  hb_array_t<ثابت BGRAColor> get_palette_colors (unsigned صحيح palette_index) ثابت
  {
    ܐܢ (unlikely (palette_index >= numPalettes))
      ܦܢܐ hb_array_t<ثابت BGRAColor> ();
    unsigned صحيح start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+colorRecordsZ).arrayZ, numColorRecords);
    ܦܢܐ all_colors.sub_array (start_index, numColors);
  }
  unsigned صحيح get_palette_colors (unsigned صحيح  palette_index,
				   unsigned صحيح  start_offset,
				   unsigned صحيح *color_count, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ثابت
  {
    ܐܢ (unlikely (palette_index >= numPalettes))
    {
      ܐܢ (color_count) *color_count = 0;
      ܦܢܐ 0;
    }
    unsigned صحيح start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+colorRecordsZ).arrayZ, numColorRecords);
    hb_array_t<ثابت BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       numColors);
    ܐܢ (color_count && colors)
    {
      + palette_colors.sub_array (start_offset, color_count)
      | hb_sink (hb_array (colors, *color_count))
      ;
    }
    ܦܢܐ numColors;
  }

  فراغ collect_name_ids (ثابت hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    ܐܢ (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (هذا, numPalettes, numColors, color_index_map, nameids_to_retain);
    }
  }

  خاص:
  ثابت CPALV1Tail& v1 () ثابت
  {
    ܐܢ (version == 0) ܦܢܐ Null (CPALV1Tail);
    hb_barrier ();
    ܦܢܐ StructAfter<CPALV1Tail> (*هذا);
  }

  عام:
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_array_t<ثابت HBUINT16> &color_record_indices,
                  ثابت hb_array_t<ثابت BGRAColor> &color_records,
                  ثابت hb_vector_t<unsigned>& first_color_index_for_layer,
                  ثابت hb_map_t& first_color_to_layer_index,
                  ثابت hb_set_t &retained_color_indices) ثابت
  {
    TRACE_SERIALIZE (هذا);

    // TODO(grieger): limit total final size.

    لكل (ثابت auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      ܐܢ (!c->copy<HBUINT16> (new_idx)) return_trace (ܕܓܠܐ);
    }

    c->push ();
    لكل (unsigned first_color_index : first_color_index_for_layer)
    {
      لكل (hb_codepoint_t color_index : retained_color_indices)
      {
        ܐܢ (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->pop_discard ();
          return_trace (ܕܓܠܐ);
        }
      }
    }

    c->add_link (colorRecordsZ, c->pop_pack ());
    return_trace (ܫܪܪܐ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ܐܢ (!numPalettes) return_trace (ܕܓܠܐ);

    ثابت hb_map_t *color_index_map = &c->plan->colr_palettes;
    ܐܢ (color_index_map->is_empty ()) return_trace (ܕܓܠܐ);

    hb_set_t retained_color_indices;
    لكل (ثابت auto _ : color_index_map->keys ())
    {
      ܐܢ (_ == 0xFFFF) استمر;
      retained_color_indices.add (_);
    }
    ܐܢ (retained_color_indices.is_empty ()) return_trace (ܕܓܠܐ);

    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);


    out->version = version;
    out->numColors = retained_color_indices.get_population ();
    out->numPalettes = numPalettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ثابت BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    ثابت hb_array_t<ثابت BGRAColor> all_color_records = (هذا+colorRecordsZ).as_array (numColorRecords);

    ثابت hb_array_t<ثابت HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (numPalettes);
    لكل (ثابت auto first_color_record_idx : colorRecordIndices)
    {
      ܐܢ (first_color_to_layer_index.has (first_color_record_idx)) استمر;

      hb_vector_t<BGRAColor> retained_colors;
      ܐܢ (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (ܕܓܠܐ);
      لكل (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      ܐܢ (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      ܐܚܪܢܐ
      {
	first_color_index_for_layer.push (first_color_record_idx);
	ܐܢ (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (ܕܓܠܐ);

	ܐܢ (unlikely (!layer_retained_colors.push_or_fail (std::move (retained_colors)))) return_trace (ܕܓܠܐ);
	ܐܢ (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (ܕܓܠܐ);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      ܐܢ (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (ܕܓܠܐ);
    }

    out->numColorRecords = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    ܐܢ (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (ܕܓܠܐ);

    ܐܢ (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, numPalettes, numColors, هذا, color_index_map));
    }

    return_trace (ܫܪܪܐ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (هذا+colorRecordsZ).sanitize (c, numColorRecords) &&
		  colorRecordIndicesZ.sanitize (c, numPalettes) &&
		  (version == 0 || v1 ().sanitize (c, هذا, numPalettes, numColors)));
  }

  محمي:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	numColors;		/* Number of colors in each palette. */
  HBUINT16	numPalettes;		/* Number of palettes in the table. */
  HBUINT16	numColorRecords;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  عام:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

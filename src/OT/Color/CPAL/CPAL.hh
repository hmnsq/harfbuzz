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

#ଅନ୍ତର୍ଭାଵ "../../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-color.h"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

ନାମାକାଶ OT {


ସଂରଚନା CPALV1Tail
{
  friend ସଂରଚନା CPAL;

  ବ୍ୟକ୍ତିଗତ:
  hb_ot_color_palette_flags_t get_palette_flags (ଅଚଳ ଶୂନ୍ୟ *base,
						 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index,
						 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_ଗଣନା) ଅଚଳ
  {
    ଯଦି (!paletteFlagsZ) ଫେରନ୍ତୁ HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    ଫେରନ୍ତୁ (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_ଗଣନା)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (ଅଚଳ ଶୂନ୍ୟ *base,
				       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index,
				       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_ଗଣନା) ଅଚଳ
  {
    ଯଦି (!paletteLabelsZ) ଫେରନ୍ତୁ HB_OT_NAME_ID_INVALID;
    ଫେରନ୍ତୁ (base+paletteLabelsZ).as_array (palette_ଗଣନା)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (ଅଚଳ ଶୂନ୍ୟ *base,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_index,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_ଗଣନା) ଅଚଳ
  {
    ଯଦି (!colorLabelsZ) ଫେରନ୍ତୁ HB_OT_NAME_ID_INVALID;
    ଫେରନ୍ତୁ (base+colorLabelsZ).as_array (color_ଗଣନା)[color_index];
  }

  ସର୍ବସାଧାରଣ:
  ଶୂନ୍ୟ collect_name_ids (ଅଚଳ ଶୂନ୍ୟ *base,
                         unsigned palette_ଗଣନା,
                         unsigned color_ଗଣନା,
                         ଅଚଳ hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ଅଚଳ
  {
    ଯଦି (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_ଗଣନା)
      | hb_sink (nameids_to_retain)
      ;
    }

    ଯଦି (colorLabelsZ)
    {
      ଅଚଳ hb_array_t<ଅଚଳ NameID> colorLabels = (base+colorLabelsZ).as_array (color_ଗଣନା);
      ପାଇଁ (unsigned i = 0; i < color_ଗଣନା; i++)
      {
        ଯଦି (!color_index_map->has (i)) ଜାରି;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  unsigned palette_ଗଣନା,
                  unsigned color_ଗଣନା,
                  ଅଚଳ ଶୂନ୍ୟ *base,
                  ଅଚଳ hb_map_t *color_index_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->paletteFlagsZ = 0;
    ଯଦି (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_ଗଣନା);

    out->paletteLabelsZ = 0;
    ଯଦି (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_ଗଣନା);

    ଅଚଳ hb_array_t<ଅଚଳ NameID> colorLabels = (base+colorLabelsZ).as_array (color_ଗଣନା);
    ଯଦି (colorLabelsZ)
    {
      c->push ();
      ପାଇଁ (unsigned i = 0; i < color_ଗଣନା; i++)
      {
        ଯଦି (!color_index_map->has (i)) ଜାରି;
        ଯଦି (!c->copy<NameID> (colorLabels[i]))
        {
          c->କାଢ଼_discard ();
          return_trace (ମିଥ୍ୟା);
        }
      }
      c->add_link (out->colorLabelsZ, c->କାଢ଼_pack ());
    }
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ ଶୂନ୍ୟ *base,
		 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_ଗଣନା,
		 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_ଗଣନା) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_ଗଣନା)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_ଗଣନା)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_ଗଣନା)));
  }

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା HBUINT32 BGRAColor;

ସଂରଚନା CPAL
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_Palettes; }

  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ min_size + ଅଙ୍କ_Palettes * ଆକାର (colorRecordIndicesZ[0]); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_palette_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_Palettes; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା   get_color_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index) ଅଚଳ
  { ଫେରନ୍ତୁ v1 ().get_palette_flags (ଏହି, palette_index, ଅଙ୍କ_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index) ଅଚଳ
  { ଫେରନ୍ତୁ v1 ().get_palette_name_id (ଏହି, palette_index, ଅଙ୍କ_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_index) ଅଚଳ
  { ଫେରନ୍ତୁ v1 ().get_color_name_id (ଏହି, color_index, ଅଙ୍କ_Colors); }

  hb_array_t<ଅଚଳ BGRAColor> get_palette_colors (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index) ଅଚଳ
  {
    ଯଦି (unlikely (palette_index >= ଅଙ୍କ_Palettes))
      ଫେରନ୍ତୁ hb_array_t<ଅଚଳ BGRAColor> ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ଅଚଳ BGRAColor> all_colors ((ଏହି+colorRecordsZ).arrayZ, ଅଙ୍କ_Color_Records);
    ଫେରନ୍ତୁ all_colors.sub_array (start_index, ଅଙ୍କ_Colors);
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_palette_colors (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  palette_index,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  start_offset,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *color_ଗଣନା, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ଅଚଳ
  {
    ଯଦି (unlikely (palette_index >= ଅଙ୍କ_Palettes))
    {
      ଯଦି (color_ଗଣନା) *color_ଗଣନା = 0;
      ଫେରନ୍ତୁ 0;
    }
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ଅଚଳ BGRAColor> all_colors ((ଏହି+colorRecordsZ).arrayZ, ଅଙ୍କ_Color_Records);
    hb_array_t<ଅଚଳ BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       ଅଙ୍କ_Colors);
    ଯଦି (color_ଗଣନା && colors)
    {
      + palette_colors.sub_array (start_offset, color_ଗଣନା)
      | hb_sink (hb_array (colors, *color_ଗଣନା))
      ;
    }
    ଫେରନ୍ତୁ ଅଙ୍କ_Colors;
  }

  ଶୂନ୍ୟ collect_name_ids (ଅଚଳ hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ଅଚଳ
  {
    ଯଦି (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (ଏହି, ଅଙ୍କ_Palettes, ଅଙ୍କ_Colors, color_index_map, nameids_to_retain);
    }
  }

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ CPALV1Tail& v1 () ଅଚଳ
  {
    ଯଦି (version == 0) ଫେରନ୍ତୁ Null (CPALV1Tail);
    hb_barrier ();
    ଫେରନ୍ତୁ StructAfter<CPALV1Tail> (*ଏହି);
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  ଅଚଳ hb_array_t<ଅଚଳ HBUINT16> &color_record_indices,
                  ଅଚଳ hb_array_t<ଅଚଳ BGRAColor> &color_records,
                  ଅଚଳ hb_vector_t<unsigned>& first_color_index_for_layer,
                  ଅଚଳ hb_map_t& first_color_to_layer_index,
                  ଅଚଳ hb_set_t &retained_color_indices) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    // TODO(grieger): limit total final size.

    ପାଇଁ (ଅଚଳ auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      ଯଦି (!c->copy<HBUINT16> (new_idx)) return_trace (ମିଥ୍ୟା);
    }

    c->push ();
    ପାଇଁ (unsigned first_color_index : first_color_index_for_layer)
    {
      ପାଇଁ (hb_codepoint_t color_index : retained_color_indices)
      {
        ଯଦି (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->କାଢ଼_discard ();
          return_trace (ମିଥ୍ୟା);
        }
      }
    }

    c->add_link (colorRecordsZ, c->କାଢ଼_pack ());
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (!ଅଙ୍କ_Palettes) return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_map_t *color_index_map = &c->plan->colr_palettes;
    ଯଦି (color_index_map->is_empty ()) return_trace (ମିଥ୍ୟା);

    hb_set_t retained_color_indices;
    ପାଇଁ (ଅଚଳ auto _ : color_index_map->ଚାବିଗୁଡ଼ିକ ())
    {
      ଯଦି (_ == 0xFFFF) ଜାରି;
      retained_color_indices.add (_);
    }
    ଯଦି (retained_color_indices.is_empty ()) return_trace (ମିଥ୍ୟା);

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);


    out->version = version;
    out->ଅଙ୍କ_Colors = retained_color_indices.get_population ();
    out->ଅଙ୍କ_Palettes = ଅଙ୍କ_Palettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ଅଚଳ BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    ଅଚଳ hb_array_t<ଅଚଳ BGRAColor> all_color_records = (ଏହି+colorRecordsZ).as_array (ଅଙ୍କ_Color_Records);

    ଅଚଳ hb_array_t<ଅଚଳ HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (ଅଙ୍କ_Palettes);
    ପାଇଁ (ଅଚଳ auto first_color_record_idx : colorRecordIndices)
    {
      ଯଦି (first_color_to_layer_index.has (first_color_record_idx)) ଜାରି;

      hb_vector_t<BGRAColor> retained_colors;
      ଯଦି (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (ମିଥ୍ୟା);
      ପାଇଁ (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      ଯଦି (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      ଅନ୍ୟଥା
      {
	first_color_index_for_layer.push (first_color_record_idx);
	ଯଦି (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (ମିଥ୍ୟା);

	ଯଦି (unlikely (!layer_retained_colors.ଠେଲ_or_fail (std::move (retained_colors)))) return_trace (ମିଥ୍ୟା);
	ଯଦି (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (ମିଥ୍ୟା);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      ଯଦି (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (ମିଥ୍ୟା);
    }

    out->ଅଙ୍କ_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    ଯଦି (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, ଅଙ୍କ_Palettes, ଅଙ୍କ_Colors, ଏହି, color_index_map));
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  (ଏହି+colorRecordsZ).sanitize (c, ଅଙ୍କ_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, ଅଙ୍କ_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, ଏହି, ଅଙ୍କ_Palettes, ଅଙ୍କ_Colors)));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	ଅଙ୍କ_Colors;		/* Number of colors in each palette. */
  HBUINT16	ଅଙ୍କ_Palettes;		/* Number of palettes in the table. */
  HBUINT16	ଅଙ୍କ_Color_Records;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

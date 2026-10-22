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

#அந்தர்பாவ "../../../hb-open-type.hh"
#அந்தர்பாவ "../../../hb-ot-color.h"
#அந்தர்பாவ "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

நாமாகாஶ OT {


கட்டமைப்பு CPALV1Tail
{
  friend கட்டமைப்பு CPAL;

  தனிப்பட்ட:
  hb_ot_color_palette_flags_t get_palette_flags (மாறிலி வெறுமை *base,
						 unsigned முழு palette_index,
						 unsigned முழு palette_மொத்தம்) மாறிலி
  {
    எனில் (!paletteFlagsZ) திரும்பு HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    திரும்பு (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_மொத்தம்)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (மாறிலி வெறுமை *base,
				       unsigned முழு palette_index,
				       unsigned முழு palette_மொத்தம்) மாறிலி
  {
    எனில் (!paletteLabelsZ) திரும்பு HB_OT_NAME_ID_INVALID;
    திரும்பு (base+paletteLabelsZ).as_array (palette_மொத்தம்)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (மாறிலி வெறுமை *base,
				     unsigned முழு color_index,
				     unsigned முழு color_மொத்தம்) மாறிலி
  {
    எனில் (!colorLabelsZ) திரும்பு HB_OT_NAME_ID_INVALID;
    திரும்பு (base+colorLabelsZ).as_array (color_மொத்தம்)[color_index];
  }

  பொது:
  வெறுமை collect_name_ids (மாறிலி வெறுமை *base,
                         unsigned palette_மொத்தம்,
                         unsigned color_மொத்தம்,
                         மாறிலி hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) மாறிலி
  {
    எனில் (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_மொத்தம்)
      | hb_sink (nameids_to_retain)
      ;
    }

    எனில் (colorLabelsZ)
    {
      மாறிலி hb_array_t<மாறிலி NameID> colorLabels = (base+colorLabelsZ).as_array (color_மொத்தம்);
      க்கு (unsigned i = 0; i < color_மொத்தம்; i++)
      {
        எனில் (!color_index_map->has (i)) தொடர்;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  தருக்க serialize (hb_serialize_context_t *c,
                  unsigned palette_மொத்தம்,
                  unsigned color_மொத்தம்,
                  மாறிலி வெறுமை *base,
                  மாறிலி hb_map_t *color_index_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->paletteFlagsZ = 0;
    எனில் (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_மொத்தம்);

    out->paletteLabelsZ = 0;
    எனில் (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_மொத்தம்);

    மாறிலி hb_array_t<மாறிலி NameID> colorLabels = (base+colorLabelsZ).as_array (color_மொத்தம்);
    எனில் (colorLabelsZ)
    {
      c->push ();
      க்கு (unsigned i = 0; i < color_மொத்தம்; i++)
      {
        எனில் (!color_index_map->has (i)) தொடர்;
        எனில் (!c->copy<NameID> (colorLabels[i]))
        {
          c->எடு_discard ();
          return_trace (பொய்);
        }
      }
      c->add_link (out->colorLabelsZ, c->எடு_pack ());
    }
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி வெறுமை *base,
		 unsigned முழு palette_மொத்தம்,
		 unsigned முழு color_மொத்தம்) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_மொத்தம்)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_மொத்தம்)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_மொத்தம்)));
  }

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_STATIC (12);
};

வகைவரை HBUINT32 BGRAColor;

கட்டமைப்பு CPAL
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  தருக்க has_data () மாறிலி { திரும்பு இலக்கம்_Palettes; }

  size_t get_size () மாறிலி
  { திரும்பு min_size + இலக்கம்_Palettes * அளவு (colorRecordIndicesZ[0]); }

  unsigned முழு get_palette_மொத்தம் () மாறிலி { திரும்பு இலக்கம்_Palettes; }
  unsigned முழு   get_color_மொத்தம் () மாறிலி { திரும்பு இலக்கம்_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned முழு palette_index) மாறிலி
  { திரும்பு v1 ().get_palette_flags (இது, palette_index, இலக்கம்_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned முழு palette_index) மாறிலி
  { திரும்பு v1 ().get_palette_name_id (இது, palette_index, இலக்கம்_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned முழு color_index) மாறிலி
  { திரும்பு v1 ().get_color_name_id (இது, color_index, இலக்கம்_Colors); }

  hb_array_t<மாறிலி BGRAColor> get_palette_colors (unsigned முழு palette_index) மாறிலி
  {
    எனில் (unlikely (palette_index >= இலக்கம்_Palettes))
      திரும்பு hb_array_t<மாறிலி BGRAColor> ();
    unsigned முழு start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<மாறிலி BGRAColor> all_colors ((இது+colorRecordsZ).arrayZ, இலக்கம்_Color_Records);
    திரும்பு all_colors.sub_array (start_index, இலக்கம்_Colors);
  }
  unsigned முழு get_palette_colors (unsigned முழு  palette_index,
				   unsigned முழு  start_offset,
				   unsigned முழு *color_மொத்தம், /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) மாறிலி
  {
    எனில் (unlikely (palette_index >= இலக்கம்_Palettes))
    {
      எனில் (color_மொத்தம்) *color_மொத்தம் = 0;
      திரும்பு 0;
    }
    unsigned முழு start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<மாறிலி BGRAColor> all_colors ((இது+colorRecordsZ).arrayZ, இலக்கம்_Color_Records);
    hb_array_t<மாறிலி BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       இலக்கம்_Colors);
    எனில் (color_மொத்தம் && colors)
    {
      + palette_colors.sub_array (start_offset, color_மொத்தம்)
      | hb_sink (hb_array (colors, *color_மொத்தம்))
      ;
    }
    திரும்பு இலக்கம்_Colors;
  }

  வெறுமை collect_name_ids (மாறிலி hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) மாறிலி
  {
    எனில் (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (இது, இலக்கம்_Palettes, இலக்கம்_Colors, color_index_map, nameids_to_retain);
    }
  }

  தனிப்பட்ட:
  மாறிலி CPALV1Tail& v1 () மாறிலி
  {
    எனில் (version == 0) திரும்பு Null (CPALV1Tail);
    hb_barrier ();
    திரும்பு StructAfter<CPALV1Tail> (*இது);
  }

  பொது:
  தருக்க serialize (hb_serialize_context_t *c,
                  மாறிலி hb_array_t<மாறிலி HBUINT16> &color_record_indices,
                  மாறிலி hb_array_t<மாறிலி BGRAColor> &color_records,
                  மாறிலி hb_vector_t<unsigned>& first_color_index_for_layer,
                  மாறிலி hb_map_t& first_color_to_layer_index,
                  மாறிலி hb_set_t &retained_color_indices) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    // TODO(grieger): limit total final size.

    க்கு (மாறிலி auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      எனில் (!c->copy<HBUINT16> (new_idx)) return_trace (பொய்);
    }

    c->push ();
    க்கு (unsigned first_color_index : first_color_index_for_layer)
    {
      க்கு (hb_codepoint_t color_index : retained_color_indices)
      {
        எனில் (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->எடு_discard ();
          return_trace (பொய்);
        }
      }
    }

    c->add_link (colorRecordsZ, c->எடு_pack ());
    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (!இலக்கம்_Palettes) return_trace (பொய்);

    மாறிலி hb_map_t *color_index_map = &c->plan->colr_palettes;
    எனில் (color_index_map->is_empty ()) return_trace (பொய்);

    hb_set_t retained_color_indices;
    க்கு (மாறிலி auto _ : color_index_map->சாவிகள் ())
    {
      எனில் (_ == 0xFFFF) தொடர்;
      retained_color_indices.add (_);
    }
    எனில் (retained_color_indices.is_empty ()) return_trace (பொய்);

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);


    out->version = version;
    out->இலக்கம்_Colors = retained_color_indices.get_population ();
    out->இலக்கம்_Palettes = இலக்கம்_Palettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<மாறிலி BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    மாறிலி hb_array_t<மாறிலி BGRAColor> all_color_records = (இது+colorRecordsZ).as_array (இலக்கம்_Color_Records);

    மாறிலி hb_array_t<மாறிலி HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (இலக்கம்_Palettes);
    க்கு (மாறிலி auto first_color_record_idx : colorRecordIndices)
    {
      எனில் (first_color_to_layer_index.has (first_color_record_idx)) தொடர்;

      hb_vector_t<BGRAColor> retained_colors;
      எனில் (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (பொய்);
      க்கு (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      எனில் (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      இல்லையெனில்
      {
	first_color_index_for_layer.push (first_color_record_idx);
	எனில் (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (பொய்);

	எனில் (unlikely (!layer_retained_colors.தள்ளு_or_fail (std::move (retained_colors)))) return_trace (பொய்);
	எனில் (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (பொய்);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      எனில் (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (பொய்);
    }

    out->இலக்கம்_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    எனில் (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (பொய்);

    எனில் (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, இலக்கம்_Palettes, இலக்கம்_Colors, இது, color_index_map));
    }

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  (இது+colorRecordsZ).sanitize (c, இலக்கம்_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, இலக்கம்_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, இது, இலக்கம்_Palettes, இலக்கம்_Colors)));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	இலக்கம்_Colors;		/* Number of colors in each palette. */
  HBUINT16	இலக்கம்_Palettes;		/* Number of palettes in the table. */
  HBUINT16	இலக்கம்_Color_Records;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  பொது:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

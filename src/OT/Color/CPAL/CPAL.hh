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

#अन्तर्भाव "../../../hb-open-type.hh"
#अन्तर्भाव "../../../hb-ot-color.h"
#अन्तर्भाव "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

नामाकाश OT {


संरचना CPALV1Tail
{
  friend संरचना CPAL;

  गुप्त:
  hb_ot_color_palette_flags_t get_palette_flags (ध्रुव शून्य *base,
						 unsigned पूर्णाङ्क palette_index,
						 unsigned पूर्णाङ्क palette_मोजणी) ध्रुव
  {
    यदि (!paletteFlagsZ) प्रत्यागम HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    प्रत्यागम (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_मोजणी)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (ध्रुव शून्य *base,
				       unsigned पूर्णाङ्क palette_index,
				       unsigned पूर्णाङ्क palette_मोजणी) ध्रुव
  {
    यदि (!paletteLabelsZ) प्रत्यागम HB_OT_NAME_ID_INVALID;
    प्रत्यागम (base+paletteLabelsZ).as_array (palette_मोजणी)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (ध्रुव शून्य *base,
				     unsigned पूर्णाङ्क color_index,
				     unsigned पूर्णाङ्क color_मोजणी) ध्रुव
  {
    यदि (!colorLabelsZ) प्रत्यागम HB_OT_NAME_ID_INVALID;
    प्रत्यागम (base+colorLabelsZ).as_array (color_मोजणी)[color_index];
  }

  सार्वजनिक:
  शून्य collect_name_ids (ध्रुव शून्य *base,
                         unsigned palette_मोजणी,
                         unsigned color_मोजणी,
                         ध्रुव hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ध्रुव
  {
    यदि (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_मोजणी)
      | hb_sink (nameids_to_retain)
      ;
    }

    यदि (colorLabelsZ)
    {
      ध्रुव hb_array_t<ध्रुव NameID> colorLabels = (base+colorLabelsZ).as_array (color_मोजणी);
      प्रति (unsigned i = 0; i < color_मोजणी; i++)
      {
        यदि (!color_index_map->has (i)) अनुवर्त;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  unsigned palette_मोजणी,
                  unsigned color_मोजणी,
                  ध्रुव शून्य *base,
                  ध्रुव hb_map_t *color_index_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    यदि (unlikely (!out)) return_trace (असत्य);

    out->paletteFlagsZ = 0;
    यदि (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_मोजणी);

    out->paletteLabelsZ = 0;
    यदि (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_मोजणी);

    ध्रुव hb_array_t<ध्रुव NameID> colorLabels = (base+colorLabelsZ).as_array (color_मोजणी);
    यदि (colorLabelsZ)
    {
      c->push ();
      प्रति (unsigned i = 0; i < color_मोजणी; i++)
      {
        यदि (!color_index_map->has (i)) अनुवर्त;
        यदि (!c->copy<NameID> (colorLabels[i]))
        {
          c->काढा_discard ();
          return_trace (असत्य);
        }
      }
      c->add_link (out->colorLabelsZ, c->काढा_pack ());
    }
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव शून्य *base,
		 unsigned पूर्णाङ्क palette_मोजणी,
		 unsigned पूर्णाङ्क color_मोजणी) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_मोजणी)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_मोजणी)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_मोजणी)));
  }

  रक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

प्रकारनिर्देश HBUINT32 BGRAColor;

संरचना CPAL
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  तार्किक has_data () ध्रुव { प्रत्यागम अंक_Palettes; }

  size_t get_size () ध्रुव
  { प्रत्यागम min_size + अंक_Palettes * परिमाण (colorRecordIndicesZ[0]); }

  unsigned पूर्णाङ्क get_palette_मोजणी () ध्रुव { प्रत्यागम अंक_Palettes; }
  unsigned पूर्णाङ्क   get_color_मोजणी () ध्रुव { प्रत्यागम अंक_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned पूर्णाङ्क palette_index) ध्रुव
  { प्रत्यागम v1 ().get_palette_flags (एतत्, palette_index, अंक_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned पूर्णाङ्क palette_index) ध्रुव
  { प्रत्यागम v1 ().get_palette_name_id (एतत्, palette_index, अंक_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned पूर्णाङ्क color_index) ध्रुव
  { प्रत्यागम v1 ().get_color_name_id (एतत्, color_index, अंक_Colors); }

  hb_array_t<ध्रुव BGRAColor> get_palette_colors (unsigned पूर्णाङ्क palette_index) ध्रुव
  {
    यदि (unlikely (palette_index >= अंक_Palettes))
      प्रत्यागम hb_array_t<ध्रुव BGRAColor> ();
    unsigned पूर्णाङ्क start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ध्रुव BGRAColor> all_colors ((एतत्+colorRecordsZ).arrayZ, अंक_Color_Records);
    प्रत्यागम all_colors.sub_array (start_index, अंक_Colors);
  }
  unsigned पूर्णाङ्क get_palette_colors (unsigned पूर्णाङ्क  palette_index,
				   unsigned पूर्णाङ्क  start_offset,
				   unsigned पूर्णाङ्क *color_मोजणी, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ध्रुव
  {
    यदि (unlikely (palette_index >= अंक_Palettes))
    {
      यदि (color_मोजणी) *color_मोजणी = 0;
      प्रत्यागम 0;
    }
    unsigned पूर्णाङ्क start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ध्रुव BGRAColor> all_colors ((एतत्+colorRecordsZ).arrayZ, अंक_Color_Records);
    hb_array_t<ध्रुव BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       अंक_Colors);
    यदि (color_मोजणी && colors)
    {
      + palette_colors.sub_array (start_offset, color_मोजणी)
      | hb_sink (hb_array (colors, *color_मोजणी))
      ;
    }
    प्रत्यागम अंक_Colors;
  }

  शून्य collect_name_ids (ध्रुव hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ध्रुव
  {
    यदि (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (एतत्, अंक_Palettes, अंक_Colors, color_index_map, nameids_to_retain);
    }
  }

  गुप्त:
  ध्रुव CPALV1Tail& v1 () ध्रुव
  {
    यदि (version == 0) प्रत्यागम Null (CPALV1Tail);
    hb_barrier ();
    प्रत्यागम StructAfter<CPALV1Tail> (*एतत्);
  }

  सार्वजनिक:
  तार्किक serialize (hb_serialize_context_t *c,
                  ध्रुव hb_array_t<ध्रुव HBUINT16> &color_record_indices,
                  ध्रुव hb_array_t<ध्रुव BGRAColor> &color_records,
                  ध्रुव hb_vector_t<unsigned>& first_color_index_for_layer,
                  ध्रुव hb_map_t& first_color_to_layer_index,
                  ध्रुव hb_set_t &retained_color_indices) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    // TODO(grieger): limit total final size.

    प्रति (ध्रुव auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      यदि (!c->copy<HBUINT16> (new_idx)) return_trace (असत्य);
    }

    c->push ();
    प्रति (unsigned first_color_index : first_color_index_for_layer)
    {
      प्रति (hb_codepoint_t color_index : retained_color_indices)
      {
        यदि (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->काढा_discard ();
          return_trace (असत्य);
        }
      }
    }

    c->add_link (colorRecordsZ, c->काढा_pack ());
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    यदि (!अंक_Palettes) return_trace (असत्य);

    ध्रुव hb_map_t *color_index_map = &c->plan->colr_palettes;
    यदि (color_index_map->is_empty ()) return_trace (असत्य);

    hb_set_t retained_color_indices;
    प्रति (ध्रुव auto _ : color_index_map->किल्ल्या ())
    {
      यदि (_ == 0xFFFF) अनुवर्त;
      retained_color_indices.add (_);
    }
    यदि (retained_color_indices.is_empty ()) return_trace (असत्य);

    auto *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);


    out->version = version;
    out->अंक_Colors = retained_color_indices.get_population ();
    out->अंक_Palettes = अंक_Palettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ध्रुव BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    ध्रुव hb_array_t<ध्रुव BGRAColor> all_color_records = (एतत्+colorRecordsZ).as_array (अंक_Color_Records);

    ध्रुव hb_array_t<ध्रुव HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (अंक_Palettes);
    प्रति (ध्रुव auto first_color_record_idx : colorRecordIndices)
    {
      यदि (first_color_to_layer_index.has (first_color_record_idx)) अनुवर्त;

      hb_vector_t<BGRAColor> retained_colors;
      यदि (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (असत्य);
      प्रति (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      यदि (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      अन्यथा
      {
	first_color_index_for_layer.push (first_color_record_idx);
	यदि (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (असत्य);

	यदि (unlikely (!layer_retained_colors.ढकला_or_fail (std::move (retained_colors)))) return_trace (असत्य);
	यदि (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (असत्य);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      यदि (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (असत्य);
    }

    out->अंक_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    यदि (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (असत्य);

    यदि (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, अंक_Palettes, अंक_Colors, एतत्, color_index_map));
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  (एतत्+colorRecordsZ).sanitize (c, अंक_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, अंक_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, एतत्, अंक_Palettes, अंक_Colors)));
  }

  रक्षित:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	अंक_Colors;		/* Number of colors in each palette. */
  HBUINT16	अंक_Palettes;		/* Number of palettes in the table. */
  HBUINT16	अंक_Color_Records;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

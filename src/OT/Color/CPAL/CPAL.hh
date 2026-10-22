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
  मित्र संरचना CPAL;

  गुप्त:
  hb_ot_color_palette_flags_t get_palette_flags (अचर शून्य *base,
						 अचिन्हित पूर्णाङ्क palette_index,
						 अचिन्हित पूर्णाङ्क palette_गन्ती) अचर
  {
    यदि (!paletteFlagsZ) फर्काऊ HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    फर्काऊ (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_गन्ती)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (अचर शून्य *base,
				       अचिन्हित पूर्णाङ्क palette_index,
				       अचिन्हित पूर्णाङ्क palette_गन्ती) अचर
  {
    यदि (!paletteLabelsZ) फर्काऊ HB_OT_NAME_ID_INVALID;
    फर्काऊ (base+paletteLabelsZ).as_array (palette_गन्ती)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (अचर शून्य *base,
				     अचिन्हित पूर्णाङ्क color_index,
				     अचिन्हित पूर्णाङ्क color_गन्ती) अचर
  {
    यदि (!colorLabelsZ) फर्काऊ HB_OT_NAME_ID_INVALID;
    फर्काऊ (base+colorLabelsZ).as_array (color_गन्ती)[color_index];
  }

  सार्वजनिक:
  शून्य collect_name_ids (अचर शून्य *base,
                         अचिन्हित palette_गन्ती,
                         अचिन्हित color_गन्ती,
                         अचर hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) अचर
  {
    यदि (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_गन्ती)
      | hb_sink (nameids_to_retain)
      ;
    }

    यदि (colorLabelsZ)
    {
      अचर hb_array_t<अचर NameID> colorLabels = (base+colorLabelsZ).as_array (color_गन्ती);
      लागि (अचिन्हित i = 0; i < color_गन्ती; i++)
      {
        यदि (!color_index_map->has (i)) जारी;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  अचिन्हित palette_गन्ती,
                  अचिन्हित color_गन्ती,
                  अचर शून्य *base,
                  अचर hb_map_t *color_index_map) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->allocate_size<CPALV1Tail> (static_size);
    यदि (unlikely (!out)) return_trace (असत्य);

    out->paletteFlagsZ = 0;
    यदि (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_गन्ती);

    out->paletteLabelsZ = 0;
    यदि (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_गन्ती);

    अचर hb_array_t<अचर NameID> colorLabels = (base+colorLabelsZ).as_array (color_गन्ती);
    यदि (colorLabelsZ)
    {
      c->push ();
      लागि (अचिन्हित i = 0; i < color_गन्ती; i++)
      {
        यदि (!color_index_map->has (i)) जारी;
        यदि (!c->copy<NameID> (colorLabels[i]))
        {
          c->झिक_discard ();
          return_trace (असत्य);
        }
      }
      c->add_link (out->colorLabelsZ, c->झिक_pack ());
    }
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 अचर शून्य *base,
		 अचिन्हित पूर्णाङ्क palette_गन्ती,
		 अचिन्हित पूर्णाङ्क color_गन्ती) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_गन्ती)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_गन्ती)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_गन्ती)));
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

  तार्किक has_data () अचर { फर्काऊ अङ्क_Palettes; }

  माप_प्रकार get_size () अचर
  { फर्काऊ min_size + अङ्क_Palettes * परिमाण (colorRecordIndicesZ[0]); }

  अचिन्हित पूर्णाङ्क get_palette_गन्ती () अचर { फर्काऊ अङ्क_Palettes; }
  अचिन्हित पूर्णाङ्क   get_color_गन्ती () अचर { फर्काऊ अङ्क_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (अचिन्हित पूर्णाङ्क palette_index) अचर
  { फर्काऊ v1 ().get_palette_flags (एतत्, palette_index, अङ्क_Palettes); }

  hb_ot_name_id_t get_palette_name_id (अचिन्हित पूर्णाङ्क palette_index) अचर
  { फर्काऊ v1 ().get_palette_name_id (एतत्, palette_index, अङ्क_Palettes); }

  hb_ot_name_id_t get_color_name_id (अचिन्हित पूर्णाङ्क color_index) अचर
  { फर्काऊ v1 ().get_color_name_id (एतत्, color_index, अङ्क_Colors); }

  hb_array_t<अचर BGRAColor> get_palette_colors (अचिन्हित पूर्णाङ्क palette_index) अचर
  {
    यदि (unlikely (palette_index >= अङ्क_Palettes))
      फर्काऊ hb_array_t<अचर BGRAColor> ();
    अचिन्हित पूर्णाङ्क start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<अचर BGRAColor> all_colors ((एतत्+colorRecordsZ).arrayZ, अङ्क_Color_Records);
    फर्काऊ all_colors.sub_array (start_index, अङ्क_Colors);
  }
  अचिन्हित पूर्णाङ्क get_palette_colors (अचिन्हित पूर्णाङ्क  palette_index,
				   अचिन्हित पूर्णाङ्क  start_offset,
				   अचिन्हित पूर्णाङ्क *color_गन्ती, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) अचर
  {
    यदि (unlikely (palette_index >= अङ्क_Palettes))
    {
      यदि (color_गन्ती) *color_गन्ती = 0;
      फर्काऊ 0;
    }
    अचिन्हित पूर्णाङ्क start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<अचर BGRAColor> all_colors ((एतत्+colorRecordsZ).arrayZ, अङ्क_Color_Records);
    hb_array_t<अचर BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       अङ्क_Colors);
    यदि (color_गन्ती && colors)
    {
      + palette_colors.sub_array (start_offset, color_गन्ती)
      | hb_sink (hb_array (colors, *color_गन्ती))
      ;
    }
    फर्काऊ अङ्क_Colors;
  }

  शून्य collect_name_ids (अचर hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) अचर
  {
    यदि (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (एतत्, अङ्क_Palettes, अङ्क_Colors, color_index_map, nameids_to_retain);
    }
  }

  गुप्त:
  अचर CPALV1Tail& v1 () अचर
  {
    यदि (version == 0) फर्काऊ Null (CPALV1Tail);
    hb_barrier ();
    फर्काऊ StructAfter<CPALV1Tail> (*एतत्);
  }

  सार्वजनिक:
  तार्किक serialize (hb_serialize_context_t *c,
                  अचर hb_array_t<अचर HBUINT16> &color_record_indices,
                  अचर hb_array_t<अचर BGRAColor> &color_records,
                  अचर hb_vector_t<अचिन्हित>& first_color_index_for_layer,
                  अचर hb_map_t& first_color_to_layer_index,
                  अचर hb_set_t &retained_color_indices) अचर
  {
    TRACE_SERIALIZE (एतत्);

    // TODO(grieger): limit total final size.

    लागि (अचर स्वतः idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      यदि (!c->copy<HBUINT16> (new_idx)) return_trace (असत्य);
    }

    c->push ();
    लागि (अचिन्हित first_color_index : first_color_index_for_layer)
    {
      लागि (hb_codepoint_t color_index : retained_color_indices)
      {
        यदि (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->झिक_discard ();
          return_trace (असत्य);
        }
      }
    }

    c->add_link (colorRecordsZ, c->झिक_pack ());
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    यदि (!अङ्क_Palettes) return_trace (असत्य);

    अचर hb_map_t *color_index_map = &c->plan->colr_palettes;
    यदि (color_index_map->is_empty ()) return_trace (असत्य);

    hb_set_t retained_color_indices;
    लागि (अचर स्वतः _ : color_index_map->साँचोहरू ())
    {
      यदि (_ == 0xFFFF) जारी;
      retained_color_indices.add (_);
    }
    यदि (retained_color_indices.is_empty ()) return_trace (असत्य);

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);


    out->version = version;
    out->अङ्क_Colors = retained_color_indices.get_population ();
    out->अङ्क_Palettes = अङ्क_Palettes;

    hb_vector_t<अचिन्हित> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<अचर BGRAColor>, अचिन्हित> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    अचर hb_array_t<अचर BGRAColor> all_color_records = (एतत्+colorRecordsZ).as_array (अङ्क_Color_Records);

    अचर hb_array_t<अचर HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (अङ्क_Palettes);
    लागि (अचर स्वतः first_color_record_idx : colorRecordIndices)
    {
      यदि (first_color_to_layer_index.has (first_color_record_idx)) जारी;

      hb_vector_t<BGRAColor> retained_colors;
      यदि (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (असत्य);
      लागि (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      अचिन्हित layer_index = first_color_index_for_layer.length;
      अचिन्हित *candidate_layer_index = nullptr;
      यदि (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      अन्यथा
      {
	first_color_index_for_layer.push (first_color_record_idx);
	यदि (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (असत्य);

	यदि (unlikely (!layer_retained_colors.थप_or_fail (std::move (retained_colors)))) return_trace (असत्य);
	यदि (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (असत्य);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      यदि (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (असत्य);
    }

    out->अङ्क_Color_Records = first_color_index_for_layer.length
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
      return_trace (v1 ().serialize (c->serializer, अङ्क_Palettes, अङ्क_Colors, एतत्, color_index_map));
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  (एतत्+colorRecordsZ).sanitize (c, अङ्क_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, अङ्क_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, एतत्, अङ्क_Palettes, अङ्क_Colors)));
  }

  रक्षित:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	अङ्क_Colors;		/* Number of colors in each palette. */
  HBUINT16	अङ्क_Palettes;		/* Number of palettes in the table. */
  HBUINT16	अङ्क_Color_Records;	/* Total number of color records, combined for
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

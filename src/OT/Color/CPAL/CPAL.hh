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

  खाजगी:
  hb_ot_color_palette_flags_t get_palette_flags (स्थिरांक शून्य *base,
						 अचिन्हित पूर्णांक palette_index,
						 अचिन्हित पूर्णांक palette_मोजणी) स्थिरांक
  {
    जर (!paletteFlagsZ) परत HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    परत (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_मोजणी)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (स्थिरांक शून्य *base,
				       अचिन्हित पूर्णांक palette_index,
				       अचिन्हित पूर्णांक palette_मोजणी) स्थिरांक
  {
    जर (!paletteLabelsZ) परत HB_OT_NAME_ID_INVALID;
    परत (base+paletteLabelsZ).as_array (palette_मोजणी)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (स्थिरांक शून्य *base,
				     अचिन्हित पूर्णांक color_index,
				     अचिन्हित पूर्णांक color_मोजणी) स्थिरांक
  {
    जर (!colorLabelsZ) परत HB_OT_NAME_ID_INVALID;
    परत (base+colorLabelsZ).as_array (color_मोजणी)[color_index];
  }

  सार्वजनिक:
  शून्य collect_name_ids (स्थिरांक शून्य *base,
                         अचिन्हित palette_मोजणी,
                         अचिन्हित color_मोजणी,
                         स्थिरांक hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) स्थिरांक
  {
    जर (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_मोजणी)
      | hb_sink (nameids_to_retain)
      ;
    }

    जर (colorLabelsZ)
    {
      स्थिरांक hb_array_t<स्थिरांक NameID> colorLabels = (base+colorLabelsZ).as_array (color_मोजणी);
      साठी (अचिन्हित i = 0; i < color_मोजणी; i++)
      {
        जर (!color_index_map->has (i)) सुरूठेवा;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  अचिन्हित palette_मोजणी,
                  अचिन्हित color_मोजणी,
                  स्थिरांक शून्य *base,
                  स्थिरांक hb_map_t *color_index_map) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->allocate_size<CPALV1Tail> (static_size);
    जर (unlikely (!out)) return_trace (असत्य);

    out->paletteFlagsZ = 0;
    जर (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_मोजणी);

    out->paletteLabelsZ = 0;
    जर (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_मोजणी);

    स्थिरांक hb_array_t<स्थिरांक NameID> colorLabels = (base+colorLabelsZ).as_array (color_मोजणी);
    जर (colorLabelsZ)
    {
      c->push ();
      साठी (अचिन्हित i = 0; i < color_मोजणी; i++)
      {
        जर (!color_index_map->has (i)) सुरूठेवा;
        जर (!c->copy<NameID> (colorLabels[i]))
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
		 स्थिरांक शून्य *base,
		 अचिन्हित पूर्णांक palette_मोजणी,
		 अचिन्हित पूर्णांक color_मोजणी) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_मोजणी)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_मोजणी)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_मोजणी)));
  }

  संरक्षित:
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

प्रकारव्याख्या HBUINT32 BGRAColor;

संरचना CPAL
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  तार्किक has_data () स्थिरांक { परत अंक_Palettes; }

  माप_प्रकार get_size () स्थिरांक
  { परत min_size + अंक_Palettes * आकार (colorRecordIndicesZ[0]); }

  अचिन्हित पूर्णांक get_palette_मोजणी () स्थिरांक { परत अंक_Palettes; }
  अचिन्हित पूर्णांक   get_color_मोजणी () स्थिरांक { परत अंक_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (अचिन्हित पूर्णांक palette_index) स्थिरांक
  { परत v1 ().get_palette_flags (हे, palette_index, अंक_Palettes); }

  hb_ot_name_id_t get_palette_name_id (अचिन्हित पूर्णांक palette_index) स्थिरांक
  { परत v1 ().get_palette_name_id (हे, palette_index, अंक_Palettes); }

  hb_ot_name_id_t get_color_name_id (अचिन्हित पूर्णांक color_index) स्थिरांक
  { परत v1 ().get_color_name_id (हे, color_index, अंक_Colors); }

  hb_array_t<स्थिरांक BGRAColor> get_palette_colors (अचिन्हित पूर्णांक palette_index) स्थिरांक
  {
    जर (unlikely (palette_index >= अंक_Palettes))
      परत hb_array_t<स्थिरांक BGRAColor> ();
    अचिन्हित पूर्णांक start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<स्थिरांक BGRAColor> all_colors ((हे+colorRecordsZ).arrayZ, अंक_Color_Records);
    परत all_colors.sub_array (start_index, अंक_Colors);
  }
  अचिन्हित पूर्णांक get_palette_colors (अचिन्हित पूर्णांक  palette_index,
				   अचिन्हित पूर्णांक  start_offset,
				   अचिन्हित पूर्णांक *color_मोजणी, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) स्थिरांक
  {
    जर (unlikely (palette_index >= अंक_Palettes))
    {
      जर (color_मोजणी) *color_मोजणी = 0;
      परत 0;
    }
    अचिन्हित पूर्णांक start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<स्थिरांक BGRAColor> all_colors ((हे+colorRecordsZ).arrayZ, अंक_Color_Records);
    hb_array_t<स्थिरांक BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       अंक_Colors);
    जर (color_मोजणी && colors)
    {
      + palette_colors.sub_array (start_offset, color_मोजणी)
      | hb_sink (hb_array (colors, *color_मोजणी))
      ;
    }
    परत अंक_Colors;
  }

  शून्य collect_name_ids (स्थिरांक hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) स्थिरांक
  {
    जर (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (हे, अंक_Palettes, अंक_Colors, color_index_map, nameids_to_retain);
    }
  }

  खाजगी:
  स्थिरांक CPALV1Tail& v1 () स्थिरांक
  {
    जर (version == 0) परत Null (CPALV1Tail);
    hb_barrier ();
    परत StructAfter<CPALV1Tail> (*हे);
  }

  सार्वजनिक:
  तार्किक serialize (hb_serialize_context_t *c,
                  स्थिरांक hb_array_t<स्थिरांक HBUINT16> &color_record_indices,
                  स्थिरांक hb_array_t<स्थिरांक BGRAColor> &color_records,
                  स्थिरांक hb_vector_t<अचिन्हित>& first_color_index_for_layer,
                  स्थिरांक hb_map_t& first_color_to_layer_index,
                  स्थिरांक hb_set_t &retained_color_indices) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    // TODO(grieger): limit total final size.

    साठी (स्थिरांक स्वतः idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      जर (!c->copy<HBUINT16> (new_idx)) return_trace (असत्य);
    }

    c->push ();
    साठी (अचिन्हित first_color_index : first_color_index_for_layer)
    {
      साठी (hb_codepoint_t color_index : retained_color_indices)
      {
        जर (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->काढा_discard ();
          return_trace (असत्य);
        }
      }
    }

    c->add_link (colorRecordsZ, c->काढा_pack ());
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (!अंक_Palettes) return_trace (असत्य);

    स्थिरांक hb_map_t *color_index_map = &c->plan->colr_palettes;
    जर (color_index_map->is_empty ()) return_trace (असत्य);

    hb_set_t retained_color_indices;
    साठी (स्थिरांक स्वतः _ : color_index_map->किल्ल्या ())
    {
      जर (_ == 0xFFFF) सुरूठेवा;
      retained_color_indices.add (_);
    }
    जर (retained_color_indices.is_empty ()) return_trace (असत्य);

    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);


    out->version = version;
    out->अंक_Colors = retained_color_indices.get_population ();
    out->अंक_Palettes = अंक_Palettes;

    hb_vector_t<अचिन्हित> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<स्थिरांक BGRAColor>, अचिन्हित> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    स्थिरांक hb_array_t<स्थिरांक BGRAColor> all_color_records = (हे+colorRecordsZ).as_array (अंक_Color_Records);

    स्थिरांक hb_array_t<स्थिरांक HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (अंक_Palettes);
    साठी (स्थिरांक स्वतः first_color_record_idx : colorRecordIndices)
    {
      जर (first_color_to_layer_index.has (first_color_record_idx)) सुरूठेवा;

      hb_vector_t<BGRAColor> retained_colors;
      जर (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (असत्य);
      साठी (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      अचिन्हित layer_index = first_color_index_for_layer.length;
      अचिन्हित *candidate_layer_index = nullptr;
      जर (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      अन्यथा
      {
	first_color_index_for_layer.push (first_color_record_idx);
	जर (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (असत्य);

	जर (unlikely (!layer_retained_colors.ढकला_or_fail (std::move (retained_colors)))) return_trace (असत्य);
	जर (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (असत्य);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      जर (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (असत्य);
    }

    out->अंक_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    जर (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (असत्य);

    जर (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, अंक_Palettes, अंक_Colors, हे, color_index_map));
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  (हे+colorRecordsZ).sanitize (c, अंक_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, अंक_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, हे, अंक_Palettes, अंक_Colors)));
  }

  संरक्षित:
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

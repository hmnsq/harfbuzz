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

#अगर_अघोषित OT_COLOR_CPAL_CPAL_HH
#घोषणा OT_COLOR_CPAL_CPAL_HH

#समावेश "../../../hb-open-type.hh"
#समावेश "../../../hb-ot-color.h"
#समावेश "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#घोषणा HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

namespace OT {


काष्ठा CPALV1_पुच्छ
{
  मित्र काष्ठा CPAL;

  निजी:
  hb_ot_color_palette_flags_t get_palette_flags (स्थिर व्योम *base,
						 अचिन्हित पूर्णांक palette_सूचकांक,
						 अचिन्हित पूर्णांक palette_गिनती) स्थिर
  {
    अगर (!paletteFlagsZ) वापस HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    वापस (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_गिनती)[palette_सूचकांक];
  }

  hb_ot_name_id_t get_palette_name_id (स्थिर व्योम *base,
				       अचिन्हित पूर्णांक palette_सूचकांक,
				       अचिन्हित पूर्णांक palette_गिनती) स्थिर
  {
    अगर (!paletteLabelsZ) वापस HB_OT_NAME_ID_INVALID;
    वापस (base+paletteLabelsZ).as_array (palette_गिनती)[palette_सूचकांक];
  }

  hb_ot_name_id_t get_color_name_id (स्थिर व्योम *base,
				     अचिन्हित पूर्णांक color_सूचकांक,
				     अचिन्हित पूर्णांक color_गिनती) स्थिर
  {
    अगर (!colorLabelsZ) वापस HB_OT_NAME_ID_INVALID;
    वापस (base+colorLabelsZ).as_array (color_गिनती)[color_सूचकांक];
  }

  खुला:
  व्योम collect_name_ids (स्थिर व्योम *base,
                         अचिन्हित palette_गिनती,
                         अचिन्हित color_गिनती,
                         स्थिर hb_map_t *color_सूचकांक_map,
                         hb_set_t *nameids_to_retain /* OUT */) स्थिर
  {
    अगर (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_गिनती)
      | hb_sink (nameids_to_retain)
      ;
    }

    अगर (colorLabelsZ)
    {
      स्थिर hb_array_t<स्थिर NameID> colorLabels = (base+colorLabelsZ).as_array (color_गिनती);
      क्रम (अचिन्हित i = 0; i < color_गिनती; i++)
      {
        अगर (!color_सूचकांक_map->has (i)) जारी;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  bool serialize (hb_serialize_context_t *c,
                  अचिन्हित palette_गिनती,
                  अचिन्हित color_गिनती,
                  स्थिर व्योम *base,
                  स्थिर hb_map_t *color_सूचकांक_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->allocate_आकार<CPALV1_पुच्छ> (static_आकार);
    अगर (unlikely (!out)) return_trace (false);

    out->paletteFlagsZ = 0;
    अगर (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::शीर्ष, palette_गिनती);

    out->paletteLabelsZ = 0;
    अगर (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::शीर्ष, palette_गिनती);

    स्थिर hb_array_t<स्थिर NameID> colorLabels = (base+colorLabelsZ).as_array (color_गिनती);
    अगर (colorLabelsZ)
    {
      c->push ();
      क्रम (अचिन्हित i = 0; i < color_गिनती; i++)
      {
        अगर (!color_सूचकांक_map->has (i)) जारी;
        अगर (!c->copy<NameID> (colorLabels[i]))
        {
          c->निकालो_discard ();
          return_trace (false);
        }
      }
      c->add_link (out->colorLabelsZ, c->निकालो_pack ());
    }
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर व्योम *base,
		 अचिन्हित पूर्णांक palette_गिनती,
		 अचिन्हित पूर्णांक color_गिनती) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_गिनती)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_गिनती)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_गिनती)));
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
  खुला:
  DEFINE_आकार_STATIC (12);
};

प्रकार HBUINT32 BGRAColor;

काष्ठा CPAL
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  bool has_तथ्य () स्थिर { वापस अंक_Palettes; }

  माप_प्रकार get_आकार () स्थिर
  { वापस min_आकार + अंक_Palettes * माप (colorRecordIndicesZ[0]); }

  अचिन्हित पूर्णांक get_palette_गिनती () स्थिर { वापस अंक_Palettes; }
  अचिन्हित पूर्णांक   get_color_गिनती () स्थिर { वापस अंक_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (अचिन्हित पूर्णांक palette_सूचकांक) स्थिर
  { वापस v1 ().get_palette_flags (यह, palette_सूचकांक, अंक_Palettes); }

  hb_ot_name_id_t get_palette_name_id (अचिन्हित पूर्णांक palette_सूचकांक) स्थिर
  { वापस v1 ().get_palette_name_id (यह, palette_सूचकांक, अंक_Palettes); }

  hb_ot_name_id_t get_color_name_id (अचिन्हित पूर्णांक color_सूचकांक) स्थिर
  { वापस v1 ().get_color_name_id (यह, color_सूचकांक, अंक_Colors); }

  hb_array_t<स्थिर BGRAColor> get_palette_colors (अचिन्हित पूर्णांक palette_सूचकांक) स्थिर
  {
    अगर (unlikely (palette_सूचकांक >= अंक_Palettes))
      वापस hb_array_t<स्थिर BGRAColor> ();
    अचिन्हित पूर्णांक start_सूचकांक = colorRecordIndicesZ[palette_सूचकांक];
    hb_array_t<स्थिर BGRAColor> all_colors ((यह+colorRecordsZ).arrayZ, अंक_Color_Records);
    वापस all_colors.sub_array (start_सूचकांक, अंक_Colors);
  }
  अचिन्हित पूर्णांक get_palette_colors (अचिन्हित पूर्णांक  palette_सूचकांक,
				   अचिन्हित पूर्णांक  start_offset,
				   अचिन्हित पूर्णांक *color_गिनती, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) स्थिर
  {
    अगर (unlikely (palette_सूचकांक >= अंक_Palettes))
    {
      अगर (color_गिनती) *color_गिनती = 0;
      वापस 0;
    }
    अचिन्हित पूर्णांक start_सूचकांक = colorRecordIndicesZ[palette_सूचकांक];
    hb_array_t<स्थिर BGRAColor> all_colors ((यह+colorRecordsZ).arrayZ, अंक_Color_Records);
    hb_array_t<स्थिर BGRAColor> palette_colors = all_colors.sub_array (start_सूचकांक,
								       अंक_Colors);
    अगर (color_गिनती && colors)
    {
      + palette_colors.sub_array (start_offset, color_गिनती)
      | hb_sink (hb_array (colors, *color_गिनती))
      ;
    }
    वापस अंक_Colors;
  }

  व्योम collect_name_ids (स्थिर hb_map_t *color_सूचकांक_map,
                         hb_set_t *nameids_to_retain /* OUT */) स्थिर
  {
    अगर (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (यह, अंक_Palettes, अंक_Colors, color_सूचकांक_map, nameids_to_retain);
    }
  }

  निजी:
  स्थिर CPALV1_पुच्छ& v1 () स्थिर
  {
    अगर (version == 0) वापस Null (CPALV1_पुच्छ);
    hb_barrier ();
    वापस StructAfter<CPALV1_पुच्छ> (*यह);
  }

  खुला:
  bool serialize (hb_serialize_context_t *c,
                  स्थिर hb_array_t<स्थिर HBUINT16> &color_record_indices,
                  स्थिर hb_array_t<स्थिर BGRAColor> &color_records,
                  स्थिर hb_vector_t<अचिन्हित>& first_color_सूचकांक_for_layer,
                  स्थिर hb_map_t& first_color_to_layer_सूचकांक,
                  स्थिर hb_set_t &retained_color_indices) स्थिर
  {
    TRACE_SERIALIZE (यह);

    // TODO(grieger): limit total final size.

    क्रम (स्थिर स्वतः idx : color_record_indices)
    {
      hb_codepoint_t layer_सूचकांक = first_color_to_layer_सूचकांक[idx];

      HBUINT16 new_idx;
      new_idx = layer_सूचकांक * retained_color_indices.get_population ();
      अगर (!c->copy<HBUINT16> (new_idx)) return_trace (false);
    }

    c->push ();
    क्रम (अचिन्हित first_color_सूचकांक : first_color_सूचकांक_for_layer)
    {
      क्रम (hb_codepoint_t color_सूचकांक : retained_color_indices)
      {
        अगर (!c->copy<BGRAColor> (color_records[first_color_सूचकांक + color_सूचकांक]))
        {
          c->निकालो_discard ();
          return_trace (false);
        }
      }
    }

    c->add_link (colorRecordsZ, c->निकालो_pack ());
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (!अंक_Palettes) return_trace (false);

    स्थिर hb_map_t *color_सूचकांक_map = &c->plan->colr_palettes;
    अगर (color_सूचकांक_map->is_empty ()) return_trace (false);

    hb_set_t retained_color_indices;
    क्रम (स्थिर स्वतः _ : color_सूचकांक_map->कुंजियाँ ())
    {
      अगर (_ == 0xFFFF) जारी;
      retained_color_indices.add (_);
    }
    अगर (retained_color_indices.is_empty ()) return_trace (false);

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);


    out->version = version;
    out->अंक_Colors = retained_color_indices.get_population ();
    out->अंक_Palettes = अंक_Palettes;

    hb_vector_t<अचिन्हित> first_color_सूचकांक_for_layer;
    hb_map_t first_color_to_layer_सूचकांक;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<स्थिर BGRAColor>, अचिन्हित> retained_colors_to_layer_सूचकांक;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    स्थिर hb_array_t<स्थिर BGRAColor> all_color_records = (यह+colorRecordsZ).as_array (अंक_Color_Records);

    स्थिर hb_array_t<स्थिर HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (अंक_Palettes);
    क्रम (स्थिर स्वतः first_color_record_idx : colorRecordIndices)
    {
      अगर (first_color_to_layer_सूचकांक.has (first_color_record_idx)) जारी;

      hb_vector_t<BGRAColor> retained_colors;
      अगर (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (false);
      क्रम (hb_codepoint_t color_सूचकांक : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_सूचकांक]);

      अचिन्हित layer_सूचकांक = first_color_सूचकांक_for_layer.लंबाई;
      अचिन्हित *candidate_layer_सूचकांक = nullptr;
      अगर (retained_colors_to_layer_सूचकांक.has (retained_colors.as_array (), &candidate_layer_सूचकांक))
	layer_सूचकांक = *candidate_layer_सूचकांक;
      अन्यथा
      {
	first_color_सूचकांक_for_layer.push (first_color_record_idx);
	अगर (unlikely (!c->serializer->propagate_error (first_color_सूचकांक_for_layer))) return_trace (false);

	अगर (unlikely (!layer_retained_colors.धकेलो_or_fail (std::move (retained_colors)))) return_trace (false);
	अगर (unlikely (!retained_colors_to_layer_सूचकांक.set (layer_retained_colors.पुच्छ ().as_array (), layer_सूचकांक)))
	  return_trace (false);
      }
      first_color_to_layer_सूचकांक.set (first_color_record_idx, layer_सूचकांक);
      अगर (unlikely (!c->serializer->propagate_error (first_color_to_layer_सूचकांक))) return_trace (false);
    }

    out->अंक_Color_Records = first_color_सूचकांक_for_layer.लंबाई
                           * retained_color_indices.get_population ();

    अगर (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_सूचकांक_for_layer,
                         first_color_to_layer_सूचकांक,
                         retained_color_indices))
      return_trace (false);

    अगर (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, अंक_Palettes, अंक_Colors, यह, color_सूचकांक_map));
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  (यह+colorRecordsZ).sanitize (c, अंक_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, अंक_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, यह, अंक_Palettes, अंक_Colors)));
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
  खुला:
  DEFINE_आकार_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#पूर्ण_अगर /* OT_COLOR_CPAL_CPAL_HH */

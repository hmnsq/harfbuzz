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

#അന്തര്ഭാവ "../../../hb-open-type.hh"
#അന്തര്ഭാവ "../../../hb-ot-color.h"
#അന്തര്ഭാവ "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

നാമാകാശ OT {


ഘടന CPALV1Tail
{
  friend ഘടന CPAL;

  സ്വകാര്യ:
  hb_ot_color_palette_flags_t get_palette_flags (സ്ഥിരം ശൂന്യം *base,
						 unsigned പൂർണ്ണസംഖ്യ palette_index,
						 unsigned പൂർണ്ണസംഖ്യ palette_എണ്ണം) സ്ഥിരം
  {
    എങ്കിൽ (!paletteFlagsZ) തിരികെ HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    തിരികെ (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_എണ്ണം)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (സ്ഥിരം ശൂന്യം *base,
				       unsigned പൂർണ്ണസംഖ്യ palette_index,
				       unsigned പൂർണ്ണസംഖ്യ palette_എണ്ണം) സ്ഥിരം
  {
    എങ്കിൽ (!paletteLabelsZ) തിരികെ HB_OT_NAME_ID_INVALID;
    തിരികെ (base+paletteLabelsZ).as_array (palette_എണ്ണം)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (സ്ഥിരം ശൂന്യം *base,
				     unsigned പൂർണ്ണസംഖ്യ color_index,
				     unsigned പൂർണ്ണസംഖ്യ color_എണ്ണം) സ്ഥിരം
  {
    എങ്കിൽ (!colorLabelsZ) തിരികെ HB_OT_NAME_ID_INVALID;
    തിരികെ (base+colorLabelsZ).as_array (color_എണ്ണം)[color_index];
  }

  പൊതു:
  ശൂന്യം collect_name_ids (സ്ഥിരം ശൂന്യം *base,
                         unsigned palette_എണ്ണം,
                         unsigned color_എണ്ണം,
                         സ്ഥിരം hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_എണ്ണം)
      | hb_sink (nameids_to_retain)
      ;
    }

    എങ്കിൽ (colorLabelsZ)
    {
      സ്ഥിരം hb_array_t<സ്ഥിരം NameID> colorLabels = (base+colorLabelsZ).as_array (color_എണ്ണം);
      വേണ്ടി (unsigned i = 0; i < color_എണ്ണം; i++)
      {
        എങ്കിൽ (!color_index_map->has (i)) തുടരുക;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  തർക്കം serialize (hb_serialize_context_t *c,
                  unsigned palette_എണ്ണം,
                  unsigned color_എണ്ണം,
                  സ്ഥിരം ശൂന്യം *base,
                  സ്ഥിരം hb_map_t *color_index_map) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->paletteFlagsZ = 0;
    എങ്കിൽ (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_എണ്ണം);

    out->paletteLabelsZ = 0;
    എങ്കിൽ (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_എണ്ണം);

    സ്ഥിരം hb_array_t<സ്ഥിരം NameID> colorLabels = (base+colorLabelsZ).as_array (color_എണ്ണം);
    എങ്കിൽ (colorLabelsZ)
    {
      c->push ();
      വേണ്ടി (unsigned i = 0; i < color_എണ്ണം; i++)
      {
        എങ്കിൽ (!color_index_map->has (i)) തുടരുക;
        എങ്കിൽ (!c->copy<NameID> (colorLabels[i]))
        {
          c->എടുക്കുക_discard ();
          return_trace (അസത്യം);
        }
      }
      c->add_link (out->colorLabelsZ, c->എടുക്കുക_pack ());
    }
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം ശൂന്യം *base,
		 unsigned പൂർണ്ണസംഖ്യ palette_എണ്ണം,
		 unsigned പൂർണ്ണസംഖ്യ color_എണ്ണം) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_എണ്ണം)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_എണ്ണം)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_എണ്ണം)));
  }

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

തരനിർവചനം HBUINT32 BGRAColor;

ഘടന CPAL
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  തർക്കം has_data () സ്ഥിരം { തിരികെ അക്കം_Palettes; }

  size_t get_size () സ്ഥിരം
  { തിരികെ min_size + അക്കം_Palettes * വലുപ്പം (colorRecordIndicesZ[0]); }

  unsigned പൂർണ്ണസംഖ്യ get_palette_എണ്ണം () സ്ഥിരം { തിരികെ അക്കം_Palettes; }
  unsigned പൂർണ്ണസംഖ്യ   get_color_എണ്ണം () സ്ഥിരം { തിരികെ അക്കം_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned പൂർണ്ണസംഖ്യ palette_index) സ്ഥിരം
  { തിരികെ v1 ().get_palette_flags (ഇത്, palette_index, അക്കം_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned പൂർണ്ണസംഖ്യ palette_index) സ്ഥിരം
  { തിരികെ v1 ().get_palette_name_id (ഇത്, palette_index, അക്കം_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned പൂർണ്ണസംഖ്യ color_index) സ്ഥിരം
  { തിരികെ v1 ().get_color_name_id (ഇത്, color_index, അക്കം_Colors); }

  hb_array_t<സ്ഥിരം BGRAColor> get_palette_colors (unsigned പൂർണ്ണസംഖ്യ palette_index) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (palette_index >= അക്കം_Palettes))
      തിരികെ hb_array_t<സ്ഥിരം BGRAColor> ();
    unsigned പൂർണ്ണസംഖ്യ start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<സ്ഥിരം BGRAColor> all_colors ((ഇത്+colorRecordsZ).arrayZ, അക്കം_Color_Records);
    തിരികെ all_colors.sub_array (start_index, അക്കം_Colors);
  }
  unsigned പൂർണ്ണസംഖ്യ get_palette_colors (unsigned പൂർണ്ണസംഖ്യ  palette_index,
				   unsigned പൂർണ്ണസംഖ്യ  start_offset,
				   unsigned പൂർണ്ണസംഖ്യ *color_എണ്ണം, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (palette_index >= അക്കം_Palettes))
    {
      എങ്കിൽ (color_എണ്ണം) *color_എണ്ണം = 0;
      തിരികെ 0;
    }
    unsigned പൂർണ്ണസംഖ്യ start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<സ്ഥിരം BGRAColor> all_colors ((ഇത്+colorRecordsZ).arrayZ, അക്കം_Color_Records);
    hb_array_t<സ്ഥിരം BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       അക്കം_Colors);
    എങ്കിൽ (color_എണ്ണം && colors)
    {
      + palette_colors.sub_array (start_offset, color_എണ്ണം)
      | hb_sink (hb_array (colors, *color_എണ്ണം))
      ;
    }
    തിരികെ അക്കം_Colors;
  }

  ശൂന്യം collect_name_ids (സ്ഥിരം hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (ഇത്, അക്കം_Palettes, അക്കം_Colors, color_index_map, nameids_to_retain);
    }
  }

  സ്വകാര്യ:
  സ്ഥിരം CPALV1Tail& v1 () സ്ഥിരം
  {
    എങ്കിൽ (version == 0) തിരികെ Null (CPALV1Tail);
    hb_barrier ();
    തിരികെ StructAfter<CPALV1Tail> (*ഇത്);
  }

  പൊതു:
  തർക്കം serialize (hb_serialize_context_t *c,
                  സ്ഥിരം hb_array_t<സ്ഥിരം HBUINT16> &color_record_indices,
                  സ്ഥിരം hb_array_t<സ്ഥിരം BGRAColor> &color_records,
                  സ്ഥിരം hb_vector_t<unsigned>& first_color_index_for_layer,
                  സ്ഥിരം hb_map_t& first_color_to_layer_index,
                  സ്ഥിരം hb_set_t &retained_color_indices) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    // TODO(grieger): limit total final size.

    വേണ്ടി (സ്ഥിരം auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      എങ്കിൽ (!c->copy<HBUINT16> (new_idx)) return_trace (അസത്യം);
    }

    c->push ();
    വേണ്ടി (unsigned first_color_index : first_color_index_for_layer)
    {
      വേണ്ടി (hb_codepoint_t color_index : retained_color_indices)
      {
        എങ്കിൽ (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->എടുക്കുക_discard ();
          return_trace (അസത്യം);
        }
      }
    }

    c->add_link (colorRecordsZ, c->എടുക്കുക_pack ());
    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (!അക്കം_Palettes) return_trace (അസത്യം);

    സ്ഥിരം hb_map_t *color_index_map = &c->plan->colr_palettes;
    എങ്കിൽ (color_index_map->is_empty ()) return_trace (അസത്യം);

    hb_set_t retained_color_indices;
    വേണ്ടി (സ്ഥിരം auto _ : color_index_map->താക്കോലുകൾ ())
    {
      എങ്കിൽ (_ == 0xFFFF) തുടരുക;
      retained_color_indices.add (_);
    }
    എങ്കിൽ (retained_color_indices.is_empty ()) return_trace (അസത്യം);

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);


    out->version = version;
    out->അക്കം_Colors = retained_color_indices.get_population ();
    out->അക്കം_Palettes = അക്കം_Palettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<സ്ഥിരം BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    സ്ഥിരം hb_array_t<സ്ഥിരം BGRAColor> all_color_records = (ഇത്+colorRecordsZ).as_array (അക്കം_Color_Records);

    സ്ഥിരം hb_array_t<സ്ഥിരം HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (അക്കം_Palettes);
    വേണ്ടി (സ്ഥിരം auto first_color_record_idx : colorRecordIndices)
    {
      എങ്കിൽ (first_color_to_layer_index.has (first_color_record_idx)) തുടരുക;

      hb_vector_t<BGRAColor> retained_colors;
      എങ്കിൽ (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (അസത്യം);
      വേണ്ടി (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      എങ്കിൽ (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      അല്ലെങ്കിൽ
      {
	first_color_index_for_layer.push (first_color_record_idx);
	എങ്കിൽ (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (അസത്യം);

	എങ്കിൽ (unlikely (!layer_retained_colors.തള്ളുക_or_fail (std::move (retained_colors)))) return_trace (അസത്യം);
	എങ്കിൽ (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (അസത്യം);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      എങ്കിൽ (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (അസത്യം);
    }

    out->അക്കം_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    എങ്കിൽ (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (അസത്യം);

    എങ്കിൽ (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, അക്കം_Palettes, അക്കം_Colors, ഇത്, color_index_map));
    }

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  (ഇത്+colorRecordsZ).sanitize (c, അക്കം_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, അക്കം_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, ഇത്, അക്കം_Palettes, അക്കം_Colors)));
  }

  സംരക്ഷിത:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	അക്കം_Colors;		/* Number of colors in each palette. */
  HBUINT16	അക്കം_Palettes;		/* Number of palettes in the table. */
  HBUINT16	അക്കം_Color_Records;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  പൊതു:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

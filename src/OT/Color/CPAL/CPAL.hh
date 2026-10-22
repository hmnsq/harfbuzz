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

#অন্তর্ভাব "../../../hb-open-type.hh"
#অন্তর্ভাব "../../../hb-ot-color.h"
#অন্তর্ভাব "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

নামাকাশ OT {


কাঠামো CPALV1_লেজ
{
  friend কাঠামো CPAL;

  ব্যক্তিগত:
  hb_ot_color_palette_flags_t get_palette_flags (ধ্রুবক শূন্য *base,
						 unsigned পূর্ণসংখ্যা palette_সূচক,
						 unsigned পূর্ণসংখ্যা palette_গুনতি) ধ্রুবক
  {
    যদি (!paletteFlagsZ) ফেরত HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    ফেরত (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_গুনতি)[palette_সূচক];
  }

  hb_ot_name_id_t get_palette_name_id (ধ্রুবক শূন্য *base,
				       unsigned পূর্ণসংখ্যা palette_সূচক,
				       unsigned পূর্ণসংখ্যা palette_গুনতি) ধ্রুবক
  {
    যদি (!paletteLabelsZ) ফেরত HB_OT_NAME_ID_INVALID;
    ফেরত (base+paletteLabelsZ).as_array (palette_গুনতি)[palette_সূচক];
  }

  hb_ot_name_id_t get_color_name_id (ধ্রুবক শূন্য *base,
				     unsigned পূর্ণসংখ্যা color_সূচক,
				     unsigned পূর্ণসংখ্যা color_গুনতি) ধ্রুবক
  {
    যদি (!colorLabelsZ) ফেরত HB_OT_NAME_ID_INVALID;
    ফেরত (base+colorLabelsZ).as_array (color_গুনতি)[color_সূচক];
  }

  সর্বজনীন:
  শূন্য collect_name_ids (ধ্রুবক শূন্য *base,
                         unsigned palette_গুনতি,
                         unsigned color_গুনতি,
                         ধ্রুবক hb_map_t *color_সূচক_map,
                         hb_set_t *nameids_to_retain /* OUT */) ধ্রুবক
  {
    যদি (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_গুনতি)
      | hb_sink (nameids_to_retain)
      ;
    }

    যদি (colorLabelsZ)
    {
      ধ্রুবক hb_array_t<ধ্রুবক NameID> colorLabels = (base+colorLabelsZ).as_array (color_গুনতি);
      জন্য (unsigned i = 0; i < color_গুনতি; i++)
      {
        যদি (!color_সূচক_map->has (i)) চালু;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
                  unsigned palette_গুনতি,
                  unsigned color_গুনতি,
                  ধ্রুবক শূন্য *base,
                  ধ্রুবক hb_map_t *color_সূচক_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->allocate_আকার<CPALV1_লেজ> (static_আকার);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->paletteFlagsZ = 0;
    যদি (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::শীর্ষ, palette_গুনতি);

    out->paletteLabelsZ = 0;
    যদি (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::শীর্ষ, palette_গুনতি);

    ধ্রুবক hb_array_t<ধ্রুবক NameID> colorLabels = (base+colorLabelsZ).as_array (color_গুনতি);
    যদি (colorLabelsZ)
    {
      c->push ();
      জন্য (unsigned i = 0; i < color_গুনতি; i++)
      {
        যদি (!color_সূচক_map->has (i)) চালু;
        যদি (!c->copy<NameID> (colorLabels[i]))
        {
          c->বের_করো_discard ();
          return_trace (মিথ্যা);
        }
      }
      c->add_link (out->colorLabelsZ, c->বের_করো_pack ());
    }
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক শূন্য *base,
		 unsigned পূর্ণসংখ্যা palette_গুনতি,
		 unsigned পূর্ণসংখ্যা color_গুনতি) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_গুনতি)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_গুনতি)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_গুনতি)));
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

প্রকারলেখ HBUINT32 BGRAColor;

কাঠামো CPAL
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত অঙ্ক_Palettes; }

  size_t get_আকার () ধ্রুবক
  { ফেরত min_আকার + অঙ্ক_Palettes * আকার (colorRecordIndicesZ[0]); }

  unsigned পূর্ণসংখ্যা get_palette_গুনতি () ধ্রুবক { ফেরত অঙ্ক_Palettes; }
  unsigned পূর্ণসংখ্যা   get_color_গুনতি () ধ্রুবক { ফেরত অঙ্ক_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned পূর্ণসংখ্যা palette_সূচক) ধ্রুবক
  { ফেরত v1 ().get_palette_flags (এই, palette_সূচক, অঙ্ক_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned পূর্ণসংখ্যা palette_সূচক) ধ্রুবক
  { ফেরত v1 ().get_palette_name_id (এই, palette_সূচক, অঙ্ক_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned পূর্ণসংখ্যা color_সূচক) ধ্রুবক
  { ফেরত v1 ().get_color_name_id (এই, color_সূচক, অঙ্ক_Colors); }

  hb_array_t<ধ্রুবক BGRAColor> get_palette_colors (unsigned পূর্ণসংখ্যা palette_সূচক) ধ্রুবক
  {
    যদি (unlikely (palette_সূচক >= অঙ্ক_Palettes))
      ফেরত hb_array_t<ধ্রুবক BGRAColor> ();
    unsigned পূর্ণসংখ্যা start_সূচক = colorRecordIndicesZ[palette_সূচক];
    hb_array_t<ধ্রুবক BGRAColor> all_colors ((এই+colorRecordsZ).arrayZ, অঙ্ক_Color_Records);
    ফেরত all_colors.sub_array (start_সূচক, অঙ্ক_Colors);
  }
  unsigned পূর্ণসংখ্যা get_palette_colors (unsigned পূর্ণসংখ্যা  palette_সূচক,
				   unsigned পূর্ণসংখ্যা  start_offset,
				   unsigned পূর্ণসংখ্যা *color_গুনতি, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ধ্রুবক
  {
    যদি (unlikely (palette_সূচক >= অঙ্ক_Palettes))
    {
      যদি (color_গুনতি) *color_গুনতি = 0;
      ফেরত 0;
    }
    unsigned পূর্ণসংখ্যা start_সূচক = colorRecordIndicesZ[palette_সূচক];
    hb_array_t<ধ্রুবক BGRAColor> all_colors ((এই+colorRecordsZ).arrayZ, অঙ্ক_Color_Records);
    hb_array_t<ধ্রুবক BGRAColor> palette_colors = all_colors.sub_array (start_সূচক,
								       অঙ্ক_Colors);
    যদি (color_গুনতি && colors)
    {
      + palette_colors.sub_array (start_offset, color_গুনতি)
      | hb_sink (hb_array (colors, *color_গুনতি))
      ;
    }
    ফেরত অঙ্ক_Colors;
  }

  শূন্য collect_name_ids (ধ্রুবক hb_map_t *color_সূচক_map,
                         hb_set_t *nameids_to_retain /* OUT */) ধ্রুবক
  {
    যদি (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (এই, অঙ্ক_Palettes, অঙ্ক_Colors, color_সূচক_map, nameids_to_retain);
    }
  }

  ব্যক্তিগত:
  ধ্রুবক CPALV1_লেজ& v1 () ধ্রুবক
  {
    যদি (version == 0) ফেরত Null (CPALV1_লেজ);
    hb_barrier ();
    ফেরত StructAfter<CPALV1_লেজ> (*এই);
  }

  সর্বজনীন:
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  ধ্রুবক hb_array_t<ধ্রুবক HBUINT16> &color_record_indices,
                  ধ্রুবক hb_array_t<ধ্রুবক BGRAColor> &color_records,
                  ধ্রুবক hb_vector_t<unsigned>& first_color_সূচক_for_layer,
                  ধ্রুবক hb_map_t& first_color_to_layer_সূচক,
                  ধ্রুবক hb_set_t &retained_color_indices) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    // TODO(grieger): limit total final size.

    জন্য (ধ্রুবক auto idx : color_record_indices)
    {
      hb_codepoint_t layer_সূচক = first_color_to_layer_সূচক[idx];

      HBUINT16 new_idx;
      new_idx = layer_সূচক * retained_color_indices.get_population ();
      যদি (!c->copy<HBUINT16> (new_idx)) return_trace (মিথ্যা);
    }

    c->push ();
    জন্য (unsigned first_color_সূচক : first_color_সূচক_for_layer)
    {
      জন্য (hb_codepoint_t color_সূচক : retained_color_indices)
      {
        যদি (!c->copy<BGRAColor> (color_records[first_color_সূচক + color_সূচক]))
        {
          c->বের_করো_discard ();
          return_trace (মিথ্যা);
        }
      }
    }

    c->add_link (colorRecordsZ, c->বের_করো_pack ());
    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (!অঙ্ক_Palettes) return_trace (মিথ্যা);

    ধ্রুবক hb_map_t *color_সূচক_map = &c->plan->colr_palettes;
    যদি (color_সূচক_map->is_empty ()) return_trace (মিথ্যা);

    hb_set_t retained_color_indices;
    জন্য (ধ্রুবক auto _ : color_সূচক_map->চাবিগুলি ())
    {
      যদি (_ == 0xFFFF) চালু;
      retained_color_indices.add (_);
    }
    যদি (retained_color_indices.is_empty ()) return_trace (মিথ্যা);

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);


    out->version = version;
    out->অঙ্ক_Colors = retained_color_indices.get_population ();
    out->অঙ্ক_Palettes = অঙ্ক_Palettes;

    hb_vector_t<unsigned> first_color_সূচক_for_layer;
    hb_map_t first_color_to_layer_সূচক;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ধ্রুবক BGRAColor>, unsigned> retained_colors_to_layer_সূচক;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    ধ্রুবক hb_array_t<ধ্রুবক BGRAColor> all_color_records = (এই+colorRecordsZ).as_array (অঙ্ক_Color_Records);

    ধ্রুবক hb_array_t<ধ্রুবক HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (অঙ্ক_Palettes);
    জন্য (ধ্রুবক auto first_color_record_idx : colorRecordIndices)
    {
      যদি (first_color_to_layer_সূচক.has (first_color_record_idx)) চালু;

      hb_vector_t<BGRAColor> retained_colors;
      যদি (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (মিথ্যা);
      জন্য (hb_codepoint_t color_সূচক : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_সূচক]);

      unsigned layer_সূচক = first_color_সূচক_for_layer.দৈর্ঘ্য;
      unsigned *candidate_layer_সূচক = nullptr;
      যদি (retained_colors_to_layer_সূচক.has (retained_colors.as_array (), &candidate_layer_সূচক))
	layer_সূচক = *candidate_layer_সূচক;
      অন্যথা
      {
	first_color_সূচক_for_layer.push (first_color_record_idx);
	যদি (unlikely (!c->serializer->propagate_error (first_color_সূচক_for_layer))) return_trace (মিথ্যা);

	যদি (unlikely (!layer_retained_colors.ঠেলো_or_fail (std::move (retained_colors)))) return_trace (মিথ্যা);
	যদি (unlikely (!retained_colors_to_layer_সূচক.set (layer_retained_colors.লেজ ().as_array (), layer_সূচক)))
	  return_trace (মিথ্যা);
      }
      first_color_to_layer_সূচক.set (first_color_record_idx, layer_সূচক);
      যদি (unlikely (!c->serializer->propagate_error (first_color_to_layer_সূচক))) return_trace (মিথ্যা);
    }

    out->অঙ্ক_Color_Records = first_color_সূচক_for_layer.দৈর্ঘ্য
                           * retained_color_indices.get_population ();

    যদি (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_সূচক_for_layer,
                         first_color_to_layer_সূচক,
                         retained_color_indices))
      return_trace (মিথ্যা);

    যদি (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, অঙ্ক_Palettes, অঙ্ক_Colors, এই, color_সূচক_map));
    }

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  (এই+colorRecordsZ).sanitize (c, অঙ্ক_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, অঙ্ক_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, এই, অঙ্ক_Palettes, অঙ্ক_Colors)));
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

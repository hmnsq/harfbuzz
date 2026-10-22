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

#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-ot-color.h"
#شامل "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

فضاینام OT {


ساختار CPALV1Tail
{
  friend ساختار CPAL;

  خصوصی:
  hb_ot_color_palette_flags_t get_palette_flags (ثابت تهی *base,
						 unsigned صحیح palette_index,
						 unsigned صحیح palette_شمار) ثابت
  {
    اگر (!paletteFlagsZ) بازگشت HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    بازگشت (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_شمار)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (ثابت تهی *base,
				       unsigned صحیح palette_index,
				       unsigned صحیح palette_شمار) ثابت
  {
    اگر (!paletteLabelsZ) بازگشت HB_OT_NAME_ID_INVALID;
    بازگشت (base+paletteLabelsZ).as_array (palette_شمار)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (ثابت تهی *base,
				     unsigned صحیح color_index,
				     unsigned صحیح color_شمار) ثابت
  {
    اگر (!colorLabelsZ) بازگشت HB_OT_NAME_ID_INVALID;
    بازگشت (base+colorLabelsZ).as_array (color_شمار)[color_index];
  }

  عمومی:
  تهی collect_name_ids (ثابت تهی *base,
                         unsigned palette_شمار,
                         unsigned color_شمار,
                         ثابت hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    اگر (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_شمار)
      | hb_sink (nameids_to_retain)
      ;
    }

    اگر (colorLabelsZ)
    {
      ثابت hb_array_t<ثابت NameID> colorLabels = (base+colorLabelsZ).as_array (color_شمار);
      برای (unsigned i = 0; i < color_شمار; i++)
      {
        اگر (!color_index_map->has (i)) ادامه;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  منطقی serialize (hb_serialize_context_t *c,
                  unsigned palette_شمار,
                  unsigned color_شمار,
                  ثابت تهی *base,
                  ثابت hb_map_t *color_index_map) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    اگر (unlikely (!out)) return_trace (نادرست);

    out->paletteFlagsZ = 0;
    اگر (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_شمار);

    out->paletteLabelsZ = 0;
    اگر (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_شمار);

    ثابت hb_array_t<ثابت NameID> colorLabels = (base+colorLabelsZ).as_array (color_شمار);
    اگر (colorLabelsZ)
    {
      c->push ();
      برای (unsigned i = 0; i < color_شمار; i++)
      {
        اگر (!color_index_map->has (i)) ادامه;
        اگر (!c->copy<NameID> (colorLabels[i]))
        {
          c->برداشت_discard ();
          return_trace (نادرست);
        }
      }
      c->add_link (out->colorLabelsZ, c->برداشت_pack ());
    }
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت تهی *base,
		 unsigned صحیح palette_شمار,
		 unsigned صحیح color_شمار) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_شمار)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_شمار)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_شمار)));
  }

  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_STATIC (12);
};

تعریفنوع HBUINT32 BGRAColor;

ساختار CPAL
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  منطقی has_data () ثابت { بازگشت رقم_Palettes; }

  size_t get_size () ثابت
  { بازگشت min_size + رقم_Palettes * اندازه (colorRecordIndicesZ[0]); }

  unsigned صحیح get_palette_شمار () ثابت { بازگشت رقم_Palettes; }
  unsigned صحیح   get_color_شمار () ثابت { بازگشت رقم_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned صحیح palette_index) ثابت
  { بازگشت v1 ().get_palette_flags (این, palette_index, رقم_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned صحیح palette_index) ثابت
  { بازگشت v1 ().get_palette_name_id (این, palette_index, رقم_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned صحیح color_index) ثابت
  { بازگشت v1 ().get_color_name_id (این, color_index, رقم_Colors); }

  hb_array_t<ثابت BGRAColor> get_palette_colors (unsigned صحیح palette_index) ثابت
  {
    اگر (unlikely (palette_index >= رقم_Palettes))
      بازگشت hb_array_t<ثابت BGRAColor> ();
    unsigned صحیح start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ثابت BGRAColor> all_colors ((این+colorRecordsZ).arrayZ, رقم_Color_Records);
    بازگشت all_colors.sub_array (start_index, رقم_Colors);
  }
  unsigned صحیح get_palette_colors (unsigned صحیح  palette_index,
				   unsigned صحیح  start_offset,
				   unsigned صحیح *color_شمار, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ثابت
  {
    اگر (unlikely (palette_index >= رقم_Palettes))
    {
      اگر (color_شمار) *color_شمار = 0;
      بازگشت 0;
    }
    unsigned صحیح start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ثابت BGRAColor> all_colors ((این+colorRecordsZ).arrayZ, رقم_Color_Records);
    hb_array_t<ثابت BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       رقم_Colors);
    اگر (color_شمار && colors)
    {
      + palette_colors.sub_array (start_offset, color_شمار)
      | hb_sink (hb_array (colors, *color_شمار))
      ;
    }
    بازگشت رقم_Colors;
  }

  تهی collect_name_ids (ثابت hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    اگر (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (این, رقم_Palettes, رقم_Colors, color_index_map, nameids_to_retain);
    }
  }

  خصوصی:
  ثابت CPALV1Tail& v1 () ثابت
  {
    اگر (version == 0) بازگشت Null (CPALV1Tail);
    hb_barrier ();
    بازگشت StructAfter<CPALV1Tail> (*این);
  }

  عمومی:
  منطقی serialize (hb_serialize_context_t *c,
                  ثابت hb_array_t<ثابت HBUINT16> &color_record_indices,
                  ثابت hb_array_t<ثابت BGRAColor> &color_records,
                  ثابت hb_vector_t<unsigned>& first_color_index_for_layer,
                  ثابت hb_map_t& first_color_to_layer_index,
                  ثابت hb_set_t &retained_color_indices) ثابت
  {
    TRACE_SERIALIZE (این);

    // TODO(grieger): limit total final size.

    برای (ثابت auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      اگر (!c->copy<HBUINT16> (new_idx)) return_trace (نادرست);
    }

    c->push ();
    برای (unsigned first_color_index : first_color_index_for_layer)
    {
      برای (hb_codepoint_t color_index : retained_color_indices)
      {
        اگر (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->برداشت_discard ();
          return_trace (نادرست);
        }
      }
    }

    c->add_link (colorRecordsZ, c->برداشت_pack ());
    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    اگر (!رقم_Palettes) return_trace (نادرست);

    ثابت hb_map_t *color_index_map = &c->plan->colr_palettes;
    اگر (color_index_map->is_empty ()) return_trace (نادرست);

    hb_set_t retained_color_indices;
    برای (ثابت auto _ : color_index_map->کلیدها ())
    {
      اگر (_ == 0xFFFF) ادامه;
      retained_color_indices.add (_);
    }
    اگر (retained_color_indices.is_empty ()) return_trace (نادرست);

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);


    out->version = version;
    out->رقم_Colors = retained_color_indices.get_population ();
    out->رقم_Palettes = رقم_Palettes;

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

    ثابت hb_array_t<ثابت BGRAColor> all_color_records = (این+colorRecordsZ).as_array (رقم_Color_Records);

    ثابت hb_array_t<ثابت HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (رقم_Palettes);
    برای (ثابت auto first_color_record_idx : colorRecordIndices)
    {
      اگر (first_color_to_layer_index.has (first_color_record_idx)) ادامه;

      hb_vector_t<BGRAColor> retained_colors;
      اگر (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (نادرست);
      برای (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      اگر (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      وگرنه
      {
	first_color_index_for_layer.push (first_color_record_idx);
	اگر (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (نادرست);

	اگر (unlikely (!layer_retained_colors.افزودن_or_fail (std::move (retained_colors)))) return_trace (نادرست);
	اگر (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (نادرست);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      اگر (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (نادرست);
    }

    out->رقم_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    اگر (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (نادرست);

    اگر (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, رقم_Palettes, رقم_Colors, این, color_index_map));
    }

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  (این+colorRecordsZ).sanitize (c, رقم_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, رقم_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, این, رقم_Palettes, رقم_Colors)));
  }

  محافظتشده:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	رقم_Colors;		/* Number of colors in each palette. */
  HBUINT16	رقم_Palettes;		/* Number of palettes in the table. */
  HBUINT16	رقم_Color_Records;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  عمومی:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */

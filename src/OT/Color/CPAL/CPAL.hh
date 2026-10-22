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
						 unsigned صحيح palette_گنتی) ثابت
  {
    إذا (!paletteFlagsZ) أرجع HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    أرجع (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_گنتی)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (ثابت فراغ *base,
				       unsigned صحيح palette_index,
				       unsigned صحيح palette_گنتی) ثابت
  {
    إذا (!paletteLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (base+paletteLabelsZ).as_array (palette_گنتی)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (ثابت فراغ *base,
				     unsigned صحيح color_index,
				     unsigned صحيح color_گنتی) ثابت
  {
    إذا (!colorLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (base+colorLabelsZ).as_array (color_گنتی)[color_index];
  }

  عام:
  فراغ collect_name_ids (ثابت فراغ *base,
                         unsigned palette_گنتی,
                         unsigned color_گنتی,
                         ثابت hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    إذا (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_گنتی)
      | hb_sink (nameids_to_retain)
      ;
    }

    إذا (colorLabelsZ)
    {
      ثابت hb_array_t<ثابت NameID> colorLabels = (base+colorLabelsZ).as_array (color_گنتی);
      لكل (unsigned i = 0; i < color_گنتی; i++)
      {
        إذا (!color_index_map->has (i)) استمر;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
                  unsigned palette_گنتی,
                  unsigned color_گنتی,
                  ثابت فراغ *base,
                  ثابت hb_map_t *color_index_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    إذا (unlikely (!out)) return_trace (خطأ);

    out->paletteFlagsZ = 0;
    إذا (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_گنتی);

    out->paletteLabelsZ = 0;
    إذا (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_گنتی);

    ثابت hb_array_t<ثابت NameID> colorLabels = (base+colorLabelsZ).as_array (color_گنتی);
    إذا (colorLabelsZ)
    {
      c->push ();
      لكل (unsigned i = 0; i < color_گنتی; i++)
      {
        إذا (!color_index_map->has (i)) استمر;
        إذا (!c->copy<NameID> (colorLabels[i]))
        {
          c->نکالو_discard ();
          return_trace (خطأ);
        }
      }
      c->add_link (out->colorLabelsZ, c->نکالو_pack ());
    }
    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *base,
		 unsigned صحيح palette_گنتی,
		 unsigned صحيح color_گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_گنتی)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_گنتی)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_گنتی)));
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

  منطقي has_data () ثابت { أرجع عدد_Palettes; }

  size_t get_size () ثابت
  { أرجع min_size + عدد_Palettes * حجم (colorRecordIndicesZ[0]); }

  unsigned صحيح get_palette_گنتی () ثابت { أرجع عدد_Palettes; }
  unsigned صحيح   get_color_گنتی () ثابت { أرجع عدد_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned صحيح palette_index) ثابت
  { أرجع v1 ().get_palette_flags (هذا, palette_index, عدد_Palettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned صحيح palette_index) ثابت
  { أرجع v1 ().get_palette_name_id (هذا, palette_index, عدد_Palettes); }

  hb_ot_name_id_t get_color_name_id (unsigned صحيح color_index) ثابت
  { أرجع v1 ().get_color_name_id (هذا, color_index, عدد_Colors); }

  hb_array_t<ثابت BGRAColor> get_palette_colors (unsigned صحيح palette_index) ثابت
  {
    إذا (unlikely (palette_index >= عدد_Palettes))
      أرجع hb_array_t<ثابت BGRAColor> ();
    unsigned صحيح start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+colorRecordsZ).arrayZ, عدد_Color_Records);
    أرجع all_colors.sub_array (start_index, عدد_Colors);
  }
  unsigned صحيح get_palette_colors (unsigned صحيح  palette_index,
				   unsigned صحيح  start_offset,
				   unsigned صحيح *color_گنتی, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ثابت
  {
    إذا (unlikely (palette_index >= عدد_Palettes))
    {
      إذا (color_گنتی) *color_گنتی = 0;
      أرجع 0;
    }
    unsigned صحيح start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+colorRecordsZ).arrayZ, عدد_Color_Records);
    hb_array_t<ثابت BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       عدد_Colors);
    إذا (color_گنتی && colors)
    {
      + palette_colors.sub_array (start_offset, color_گنتی)
      | hb_sink (hb_array (colors, *color_گنتی))
      ;
    }
    أرجع عدد_Colors;
  }

  فراغ collect_name_ids (ثابت hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    إذا (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (هذا, عدد_Palettes, عدد_Colors, color_index_map, nameids_to_retain);
    }
  }

  خاص:
  ثابت CPALV1Tail& v1 () ثابت
  {
    إذا (version == 0) أرجع Null (CPALV1Tail);
    hb_barrier ();
    أرجع StructAfter<CPALV1Tail> (*هذا);
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
      إذا (!c->copy<HBUINT16> (new_idx)) return_trace (خطأ);
    }

    c->push ();
    لكل (unsigned first_color_index : first_color_index_for_layer)
    {
      لكل (hb_codepoint_t color_index : retained_color_indices)
      {
        إذا (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->نکالو_discard ();
          return_trace (خطأ);
        }
      }
    }

    c->add_link (colorRecordsZ, c->نکالو_pack ());
    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!عدد_Palettes) return_trace (خطأ);

    ثابت hb_map_t *color_index_map = &c->plan->colr_palettes;
    إذا (color_index_map->is_empty ()) return_trace (خطأ);

    hb_set_t retained_color_indices;
    لكل (ثابت auto _ : color_index_map->کلیدیں ())
    {
      إذا (_ == 0xFFFF) استمر;
      retained_color_indices.add (_);
    }
    إذا (retained_color_indices.is_empty ()) return_trace (خطأ);

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);


    out->version = version;
    out->عدد_Colors = retained_color_indices.get_population ();
    out->عدد_Palettes = عدد_Palettes;

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

    ثابت hb_array_t<ثابت BGRAColor> all_color_records = (هذا+colorRecordsZ).as_array (عدد_Color_Records);

    ثابت hb_array_t<ثابت HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (عدد_Palettes);
    لكل (ثابت auto first_color_record_idx : colorRecordIndices)
    {
      إذا (first_color_to_layer_index.has (first_color_record_idx)) استمر;

      hb_vector_t<BGRAColor> retained_colors;
      إذا (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (خطأ);
      لكل (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      إذا (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      وإلا
      {
	first_color_index_for_layer.push (first_color_record_idx);
	إذا (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (خطأ);

	إذا (unlikely (!layer_retained_colors.ڈالو_or_fail (std::move (retained_colors)))) return_trace (خطأ);
	إذا (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (خطأ);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      إذا (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (خطأ);
    }

    out->عدد_Color_Records = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    إذا (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (خطأ);

    إذا (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, عدد_Palettes, عدد_Colors, هذا, color_index_map));
    }

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (هذا+colorRecordsZ).sanitize (c, عدد_Color_Records) &&
		  colorRecordIndicesZ.sanitize (c, عدد_Palettes) &&
		  (version == 0 || v1 ().sanitize (c, هذا, عدد_Palettes, عدد_Colors)));
  }

  محمي:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	عدد_Colors;		/* Number of colors in each palette. */
  HBUINT16	عدد_Palettes;		/* Number of palettes in the table. */
  HBUINT16	عدد_Color_Records;	/* Total number of color records, combined for
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

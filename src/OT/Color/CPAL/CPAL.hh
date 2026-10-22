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

#جيڪڏهن_وصف_نه_ٿيل OT_COLOR_CPAL_CPAL_HH
#وصف_ڪريو OT_COLOR_CPAL_CPAL_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-ot-color.h"
#ضمن "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#وصف_ڪريو HB_OT_ٽيگ_CPAL HB_ٽيگ('C','P','A','L')

نطاق OT {


بنية CPALV1_پڇ
{
  دوست بنية CPAL;

  خاص:
  hb_ot_color_palette_flags_t get_palette_flags (ثابت فراغ *بنياد,
						 بنانشان صحيح palette_اشاريو,
						 بنانشان صحيح palette_ڳڻپ) ثابت
  {
    إذا (!paletteFlagsZ) أرجع HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    أرجع (hb_ot_color_palette_flags_t) (uint32_t)
	   (بنياد+paletteFlagsZ).as_array (palette_ڳڻپ)[palette_اشاريو];
  }

  hb_ot_name_id_t get_palette_name_id (ثابت فراغ *بنياد,
				       بنانشان صحيح palette_اشاريو,
				       بنانشان صحيح palette_ڳڻپ) ثابت
  {
    إذا (!paletteLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (بنياد+paletteLabelsZ).as_array (palette_ڳڻپ)[palette_اشاريو];
  }

  hb_ot_name_id_t get_color_name_id (ثابت فراغ *بنياد,
				     بنانشان صحيح color_اشاريو,
				     بنانشان صحيح color_ڳڻپ) ثابت
  {
    إذا (!colorLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (بنياد+colorLabelsZ).as_array (color_ڳڻپ)[color_اشاريو];
  }

  عام:
  فراغ collect_name_ids (ثابت فراغ *بنياد,
                         بنانشان palette_ڳڻپ,
                         بنانشان color_ڳڻپ,
                         ثابت hb_map_t *color_اشاريو_map,
                         hb_سيٽ_t *nameids_منزل_retain /*ٻاهريون*/) ثابت
  {
    إذا (paletteLabelsZ)
    {
      + (بنياد+paletteLabelsZ).as_array (palette_ڳڻپ)
      | hb_sink (nameids_منزل_retain)
      ;
    }

    إذا (colorLabelsZ)
    {
      ثابت hb_array_t<ثابت NameID> colorLabels = (بنياد+colorLabelsZ).as_array (color_ڳڻپ);
      لكل (بنانشان i = 0; i < color_ڳڻپ; i++)
      {
        إذا (!color_اشاريو_map->has (i)) استمر;
        nameids_منزل_retain->جوڙيو (colorLabels[i]);
      }
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
                  بنانشان palette_ڳڻپ,
                  بنانشان color_ڳڻپ,
                  ثابت فراغ *بنياد,
                  ثابت hb_map_t *color_اشاريو_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->allocate_سائيز<CPALV1_پڇ> (static_سائيز);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->paletteFlagsZ = 0;
    إذا (paletteFlagsZ)
      ٻاهريون->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, بنياد, 0, hb_serialize_context_t::سر, palette_ڳڻپ);

    ٻاهريون->paletteLabelsZ = 0;
    إذا (paletteLabelsZ)
      ٻاهريون->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, بنياد, 0, hb_serialize_context_t::سر, palette_ڳڻپ);

    ثابت hb_array_t<ثابت NameID> colorLabels = (بنياد+colorLabelsZ).as_array (color_ڳڻپ);
    إذا (colorLabelsZ)
    {
      c->push ();
      لكل (بنانشان i = 0; i < color_ڳڻپ; i++)
      {
        إذا (!color_اشاريو_map->has (i)) استمر;
        إذا (!c->copy<NameID> (colorLabels[i]))
        {
          c->ڪڍو_discard ();
          return_trace (خطأ);
        }
      }
      c->جوڙيو_link (ٻاهريون->colorLabelsZ, c->ڪڍو_pack ());
    }
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *بنياد,
		 بنانشان صحيح palette_ڳڻپ,
		 بنانشان صحيح color_ڳڻپ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  (!paletteFlagsZ  || (بنياد+paletteFlagsZ).sanitize (c, palette_ڳڻپ)) &&
		  (!paletteLabelsZ || (بنياد+paletteLabelsZ).sanitize (c, palette_ڳڻپ)) &&
		  (!colorLabelsZ   || (بنياد+colorLabelsZ).sanitize (c, color_ڳڻپ)));
  }

  محمي:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32_منزل<Unsized_Array_جو<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32_منزل<Unsized_Array_جو<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32_منزل<Unsized_Array_جو<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  عام:
  DEFINE_سائيز_STATIC (12);
};

تعريفنوع HBUINT32 BGRAColor;

بنية CPAL
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_CPAL;

  منطقي has_معلومات () ثابت { أرجع انگ_Palettes; }

  size_t get_سائيز () ثابت
  { أرجع min_سائيز + انگ_Palettes * حجم (color_رڪارڊ_Indices_Z[0]); }

  بنانشان صحيح get_palette_ڳڻپ () ثابت { أرجع انگ_Palettes; }
  بنانشان صحيح   get_color_ڳڻپ () ثابت { أرجع انگ_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (بنانشان صحيح palette_اشاريو) ثابت
  { أرجع v1 ().get_palette_flags (هذا, palette_اشاريو, انگ_Palettes); }

  hb_ot_name_id_t get_palette_name_id (بنانشان صحيح palette_اشاريو) ثابت
  { أرجع v1 ().get_palette_name_id (هذا, palette_اشاريو, انگ_Palettes); }

  hb_ot_name_id_t get_color_name_id (بنانشان صحيح color_اشاريو) ثابت
  { أرجع v1 ().get_color_name_id (هذا, color_اشاريو, انگ_Colors); }

  hb_array_t<ثابت BGRAColor> get_palette_colors (بنانشان صحيح palette_اشاريو) ثابت
  {
    إذا (unlikely (palette_اشاريو >= انگ_Palettes))
      أرجع hb_array_t<ثابت BGRAColor> ();
    بنانشان صحيح شروعات_اشاريو = color_رڪارڊ_Indices_Z[palette_اشاريو];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+color_رڪارڊون_Z).arrayZ, انگ_Color_رڪارڊون);
    أرجع all_colors.sub_array (شروعات_اشاريو, انگ_Colors);
  }
  بنانشان صحيح get_palette_colors (بنانشان صحيح  palette_اشاريو,
				   بنانشان صحيح  شروعات_وٿي,
				   بنانشان صحيح *color_ڳڻپ, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ثابت
  {
    إذا (unlikely (palette_اشاريو >= انگ_Palettes))
    {
      إذا (color_ڳڻپ) *color_ڳڻپ = 0;
      أرجع 0;
    }
    بنانشان صحيح شروعات_اشاريو = color_رڪارڊ_Indices_Z[palette_اشاريو];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+color_رڪارڊون_Z).arrayZ, انگ_Color_رڪارڊون);
    hb_array_t<ثابت BGRAColor> palette_colors = all_colors.sub_array (شروعات_اشاريو,
								       انگ_Colors);
    إذا (color_ڳڻپ && colors)
    {
      + palette_colors.sub_array (شروعات_وٿي, color_ڳڻپ)
      | hb_sink (hb_array (colors, *color_ڳڻپ))
      ;
    }
    أرجع انگ_Colors;
  }

  فراغ collect_name_ids (ثابت hb_map_t *color_اشاريو_map,
                         hb_سيٽ_t *nameids_منزل_retain /*ٻاهريون*/) ثابت
  {
    إذا (نسخو == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (هذا, انگ_Palettes, انگ_Colors, color_اشاريو_map, nameids_منزل_retain);
    }
  }

  خاص:
  ثابت CPALV1_پڇ& v1 () ثابت
  {
    إذا (نسخو == 0) أرجع Null (CPALV1_پڇ);
    hb_barrier ();
    أرجع StructAfter<CPALV1_پڇ> (*هذا);
  }

  عام:
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_array_t<ثابت HBUINT16> &color_رڪارڊ_indices,
                  ثابت hb_array_t<ثابت BGRAColor> &color_رڪارڊون,
                  ثابت hb_ويڪٽر_t<بنانشان>& پهريون_color_اشاريو_for_layer,
                  ثابت hb_map_t& پهريون_color_منزل_layer_اشاريو,
                  ثابت hb_سيٽ_t &retained_color_indices) ثابت
  {
    TRACE_SERIALIZE (هذا);

    // TODO(grieger): limit total final size.

    لكل (ثابت خودڪار idx : color_رڪارڊ_indices)
    {
      hb_codepoint_t layer_اشاريو = پهريون_color_منزل_layer_اشاريو[idx];

      HBUINT16 new_idx;
      new_idx = layer_اشاريو * retained_color_indices.get_population ();
      إذا (!c->copy<HBUINT16> (new_idx)) return_trace (خطأ);
    }

    c->push ();
    لكل (بنانشان پهريون_color_اشاريو : پهريون_color_اشاريو_for_layer)
    {
      لكل (hb_codepoint_t color_اشاريو : retained_color_indices)
      {
        إذا (!c->copy<BGRAColor> (color_رڪارڊون[پهريون_color_اشاريو + color_اشاريو]))
        {
          c->ڪڍو_discard ();
          return_trace (خطأ);
        }
      }
    }

    c->جوڙيو_link (color_رڪارڊون_Z, c->ڪڍو_pack ());
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!انگ_Palettes) return_trace (خطأ);

    ثابت hb_map_t *color_اشاريو_map = &c->plan->colr_palettes;
    إذا (color_اشاريو_map->is_empty ()) return_trace (خطأ);

    hb_سيٽ_t retained_color_indices;
    لكل (ثابت خودڪار _ : color_اشاريو_map->ڪنجيون ())
    {
      إذا (_ == 0xFFFF) استمر;
      retained_color_indices.جوڙيو (_);
    }
    إذا (retained_color_indices.is_empty ()) return_trace (خطأ);

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);


    ٻاهريون->نسخو = نسخو;
    ٻاهريون->انگ_Colors = retained_color_indices.get_population ();
    ٻاهريون->انگ_Palettes = انگ_Palettes;

    hb_ويڪٽر_t<بنانشان> پهريون_color_اشاريو_for_layer;
    hb_map_t پهريون_color_منزل_layer_اشاريو;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ثابت BGRAColor>, بنانشان> retained_colors_منزل_layer_اشاريو;
    hb_ويڪٽر_t<hb_ويڪٽر_t<BGRAColor>> layer_retained_colors;

    ثابت hb_array_t<ثابت BGRAColor> all_color_رڪارڊون = (هذا+color_رڪارڊون_Z).as_array (انگ_Color_رڪارڊون);

    ثابت hb_array_t<ثابت HBUINT16> color_رڪارڊ_Indices = color_رڪارڊ_Indices_Z.as_array (انگ_Palettes);
    لكل (ثابت خودڪار پهريون_color_رڪارڊ_idx : color_رڪارڊ_Indices)
    {
      إذا (پهريون_color_منزل_layer_اشاريو.has (پهريون_color_رڪارڊ_idx)) استمر;

      hb_ويڪٽر_t<BGRAColor> retained_colors;
      إذا (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (خطأ);
      لكل (hb_codepoint_t color_اشاريو : retained_color_indices)
	retained_colors.push (all_color_رڪارڊون[پهريون_color_رڪارڊ_idx + color_اشاريو]);

      بنانشان layer_اشاريو = پهريون_color_اشاريو_for_layer.ڊيگهائي;
      بنانشان *candidate_layer_اشاريو = خالي_اشارو;
      إذا (retained_colors_منزل_layer_اشاريو.has (retained_colors.as_array (), &candidate_layer_اشاريو))
	layer_اشاريو = *candidate_layer_اشاريو;
      وإلا
      {
	پهريون_color_اشاريو_for_layer.push (پهريون_color_رڪارڊ_idx);
	إذا (unlikely (!c->serializer->propagate_error (پهريون_color_اشاريو_for_layer))) return_trace (خطأ);

	إذا (unlikely (!layer_retained_colors.ڌڪو_or_fail (std::move (retained_colors)))) return_trace (خطأ);
	إذا (unlikely (!retained_colors_منزل_layer_اشاريو.سيٽ (layer_retained_colors.پڇ ().as_array (), layer_اشاريو)))
	  return_trace (خطأ);
      }
      پهريون_color_منزل_layer_اشاريو.سيٽ (پهريون_color_رڪارڊ_idx, layer_اشاريو);
      إذا (unlikely (!c->serializer->propagate_error (پهريون_color_منزل_layer_اشاريو))) return_trace (خطأ);
    }

    ٻاهريون->انگ_Color_رڪارڊون = پهريون_color_اشاريو_for_layer.ڊيگهائي
                           * retained_color_indices.get_population ();

    إذا (!ٻاهريون->serialize (c->serializer,
                         color_رڪارڊ_Indices,
                         all_color_رڪارڊون,
                         پهريون_color_اشاريو_for_layer,
                         پهريون_color_منزل_layer_اشاريو,
                         retained_color_indices))
      return_trace (خطأ);

    إذا (نسخو == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, انگ_Palettes, انگ_Colors, هذا, color_اشاريو_map));
    }

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (هذا+color_رڪارڊون_Z).sanitize (c, انگ_Color_رڪارڊون) &&
		  color_رڪارڊ_Indices_Z.sanitize (c, انگ_Palettes) &&
		  (نسخو == 0 || v1 ().sanitize (c, هذا, انگ_Palettes, انگ_Colors)));
  }

  محمي:
  HBUINT16	نسخو;		/* Table version number */
  /* Version 0 */
  HBUINT16	انگ_Colors;		/* Number of colors in each palette. */
  HBUINT16	انگ_Palettes;		/* Number of palettes in the table. */
  HBUINT16	انگ_Color_رڪارڊون;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32_منزل<Unsized_Array_جو<BGRAColor>>
		color_رڪارڊون_Z;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  Unsized_Array_جو<HBUINT16>
		color_رڪارڊ_Indices_Z;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  عام:
  DEFINE_سائيز_ARRAY (12, color_رڪارڊ_Indices_Z);
};

} /* namespace OT */


#جيڪڏهن_پورو /* OT_COLOR_CPAL_CPAL_HH */

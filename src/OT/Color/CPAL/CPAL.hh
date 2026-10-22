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

#جے_بنا_تعریف OT_COLOR_CPAL_CPAL_HH
#تعریف_کرو OT_COLOR_CPAL_CPAL_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-ot-color.h"
#ضمن "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#تعریف_کرو HB_OT_ٹیگ_CPAL HB_ٹیگ('C','P','A','L')

نطاق OT {


بنية CPALV1_پوچھ
{
  یار بنية CPAL;

  خاص:
  hb_ot_color_palette_flags_t get_palette_flags (ثابت فراغ *ادھار,
						 بنانشانی صحيح palette_اشاریہ,
						 بنانشانی صحيح palette_گنتی) ثابت
  {
    إذا (!paletteFlagsZ) أرجع HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    أرجع (hb_ot_color_palette_flags_t) (uint32_t)
	   (ادھار+paletteFlagsZ).as_array (palette_گنتی)[palette_اشاریہ];
  }

  hb_ot_name_id_t get_palette_name_id (ثابت فراغ *ادھار,
				       بنانشانی صحيح palette_اشاریہ,
				       بنانشانی صحيح palette_گنتی) ثابت
  {
    إذا (!paletteLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (ادھار+paletteLabelsZ).as_array (palette_گنتی)[palette_اشاریہ];
  }

  hb_ot_name_id_t get_color_name_id (ثابت فراغ *ادھار,
				     بنانشانی صحيح color_اشاریہ,
				     بنانشانی صحيح color_گنتی) ثابت
  {
    إذا (!colorLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (ادھار+colorLabelsZ).as_array (color_گنتی)[color_اشاریہ];
  }

  عام:
  فراغ collect_name_ids (ثابت فراغ *ادھار,
                         بنانشانی palette_گنتی,
                         بنانشانی color_گنتی,
                         ثابت hb_map_t *color_اشاریہ_map,
                         hb_سیٹ_t *nameids_منزل_retain /*باہرلا*/) ثابت
  {
    إذا (paletteLabelsZ)
    {
      + (ادھار+paletteLabelsZ).as_array (palette_گنتی)
      | hb_sink (nameids_منزل_retain)
      ;
    }

    إذا (colorLabelsZ)
    {
      ثابت hb_array_t<ثابت NameID> colorLabels = (ادھار+colorLabelsZ).as_array (color_گنتی);
      لكل (بنانشانی i = 0; i < color_گنتی; i++)
      {
        إذا (!color_اشاریہ_map->has (i)) استمر;
        nameids_منزل_retain->جوڑو (colorLabels[i]);
      }
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
                  بنانشانی palette_گنتی,
                  بنانشانی color_گنتی,
                  ثابت فراغ *ادھار,
                  ثابت hb_map_t *color_اشاریہ_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->allocate_ناپ<CPALV1_پوچھ> (static_ناپ);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->paletteFlagsZ = 0;
    إذا (paletteFlagsZ)
      باہرلا->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, ادھار, 0, hb_serialize_context_t::سر, palette_گنتی);

    باہرلا->paletteLabelsZ = 0;
    إذا (paletteLabelsZ)
      باہرلا->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, ادھار, 0, hb_serialize_context_t::سر, palette_گنتی);

    ثابت hb_array_t<ثابت NameID> colorLabels = (ادھار+colorLabelsZ).as_array (color_گنتی);
    إذا (colorLabelsZ)
    {
      c->push ();
      لكل (بنانشانی i = 0; i < color_گنتی; i++)
      {
        إذا (!color_اشاریہ_map->has (i)) استمر;
        إذا (!c->copy<NameID> (colorLabels[i]))
        {
          c->نکالو_discard ();
          return_trace (خطأ);
        }
      }
      c->جوڑو_link (باہرلا->colorLabelsZ, c->نکالو_pack ());
    }
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *ادھار,
		 بنانشانی صحيح palette_گنتی,
		 بنانشانی صحيح color_گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  (!paletteFlagsZ  || (ادھار+paletteFlagsZ).sanitize (c, palette_گنتی)) &&
		  (!paletteLabelsZ || (ادھار+paletteLabelsZ).sanitize (c, palette_گنتی)) &&
		  (!colorLabelsZ   || (ادھار+colorLabelsZ).sanitize (c, color_گنتی)));
  }

  محمي:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32_منزل<Unsized_Array_دا<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32_منزل<Unsized_Array_دا<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32_منزل<Unsized_Array_دا<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  عام:
  DEFINE_ناپ_STATIC (12);
};

تعريفنوع HBUINT32 BGRAColor;

بنية CPAL
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_CPAL;

  منطقي has_انکڑے () ثابت { أرجع عدد_Palettes; }

  size_t get_ناپ () ثابت
  { أرجع min_ناپ + عدد_Palettes * حجم (color_ریکارڈ_Indices_Z[0]); }

  بنانشانی صحيح get_palette_گنتی () ثابت { أرجع عدد_Palettes; }
  بنانشانی صحيح   get_color_گنتی () ثابت { أرجع عدد_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (بنانشانی صحيح palette_اشاریہ) ثابت
  { أرجع v1 ().get_palette_flags (هذا, palette_اشاریہ, عدد_Palettes); }

  hb_ot_name_id_t get_palette_name_id (بنانشانی صحيح palette_اشاریہ) ثابت
  { أرجع v1 ().get_palette_name_id (هذا, palette_اشاریہ, عدد_Palettes); }

  hb_ot_name_id_t get_color_name_id (بنانشانی صحيح color_اشاریہ) ثابت
  { أرجع v1 ().get_color_name_id (هذا, color_اشاریہ, عدد_Colors); }

  hb_array_t<ثابت BGRAColor> get_palette_colors (بنانشانی صحيح palette_اشاریہ) ثابت
  {
    إذا (unlikely (palette_اشاریہ >= عدد_Palettes))
      أرجع hb_array_t<ثابت BGRAColor> ();
    بنانشانی صحيح شروع_اشاریہ = color_ریکارڈ_Indices_Z[palette_اشاریہ];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+color_ریکارڈاں_Z).arrayZ, عدد_Color_ریکارڈاں);
    أرجع all_colors.sub_array (شروع_اشاریہ, عدد_Colors);
  }
  بنانشانی صحيح get_palette_colors (بنانشانی صحيح  palette_اشاریہ,
				   بنانشانی صحيح  شروع_وتھ,
				   بنانشانی صحيح *color_گنتی, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ثابت
  {
    إذا (unlikely (palette_اشاریہ >= عدد_Palettes))
    {
      إذا (color_گنتی) *color_گنتی = 0;
      أرجع 0;
    }
    بنانشانی صحيح شروع_اشاریہ = color_ریکارڈ_Indices_Z[palette_اشاریہ];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+color_ریکارڈاں_Z).arrayZ, عدد_Color_ریکارڈاں);
    hb_array_t<ثابت BGRAColor> palette_colors = all_colors.sub_array (شروع_اشاریہ,
								       عدد_Colors);
    إذا (color_گنتی && colors)
    {
      + palette_colors.sub_array (شروع_وتھ, color_گنتی)
      | hb_sink (hb_array (colors, *color_گنتی))
      ;
    }
    أرجع عدد_Colors;
  }

  فراغ collect_name_ids (ثابت hb_map_t *color_اشاریہ_map,
                         hb_سیٹ_t *nameids_منزل_retain /*باہرلا*/) ثابت
  {
    إذا (نسخہ == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (هذا, عدد_Palettes, عدد_Colors, color_اشاریہ_map, nameids_منزل_retain);
    }
  }

  خاص:
  ثابت CPALV1_پوچھ& v1 () ثابت
  {
    إذا (نسخہ == 0) أرجع Null (CPALV1_پوچھ);
    hb_barrier ();
    أرجع StructAfter<CPALV1_پوچھ> (*هذا);
  }

  عام:
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_array_t<ثابت HBUINT16> &color_ریکارڈ_indices,
                  ثابت hb_array_t<ثابت BGRAColor> &color_ریکارڈاں,
                  ثابت hb_سمتیہ_t<بنانشانی>& پہلا_color_اشاریہ_for_layer,
                  ثابت hb_map_t& پہلا_color_منزل_layer_اشاریہ,
                  ثابت hb_سیٹ_t &retained_color_indices) ثابت
  {
    TRACE_SERIALIZE (هذا);

    // TODO(grieger): limit total final size.

    لكل (ثابت خودکار idx : color_ریکارڈ_indices)
    {
      hb_codepoint_t layer_اشاریہ = پہلا_color_منزل_layer_اشاریہ[idx];

      HBUINT16 new_idx;
      new_idx = layer_اشاریہ * retained_color_indices.get_population ();
      إذا (!c->copy<HBUINT16> (new_idx)) return_trace (خطأ);
    }

    c->push ();
    لكل (بنانشانی پہلا_color_اشاریہ : پہلا_color_اشاریہ_for_layer)
    {
      لكل (hb_codepoint_t color_اشاریہ : retained_color_indices)
      {
        إذا (!c->copy<BGRAColor> (color_ریکارڈاں[پہلا_color_اشاریہ + color_اشاریہ]))
        {
          c->نکالو_discard ();
          return_trace (خطأ);
        }
      }
    }

    c->جوڑو_link (color_ریکارڈاں_Z, c->نکالو_pack ());
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!عدد_Palettes) return_trace (خطأ);

    ثابت hb_map_t *color_اشاریہ_map = &c->plan->colr_palettes;
    إذا (color_اشاریہ_map->is_empty ()) return_trace (خطأ);

    hb_سیٹ_t retained_color_indices;
    لكل (ثابت خودکار _ : color_اشاریہ_map->کلیدیں ())
    {
      إذا (_ == 0xFFFF) استمر;
      retained_color_indices.جوڑو (_);
    }
    إذا (retained_color_indices.is_empty ()) return_trace (خطأ);

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);


    باہرلا->نسخہ = نسخہ;
    باہرلا->عدد_Colors = retained_color_indices.get_population ();
    باہرلا->عدد_Palettes = عدد_Palettes;

    hb_سمتیہ_t<بنانشانی> پہلا_color_اشاریہ_for_layer;
    hb_map_t پہلا_color_منزل_layer_اشاریہ;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ثابت BGRAColor>, بنانشانی> retained_colors_منزل_layer_اشاریہ;
    hb_سمتیہ_t<hb_سمتیہ_t<BGRAColor>> layer_retained_colors;

    ثابت hb_array_t<ثابت BGRAColor> all_color_ریکارڈاں = (هذا+color_ریکارڈاں_Z).as_array (عدد_Color_ریکارڈاں);

    ثابت hb_array_t<ثابت HBUINT16> color_ریکارڈ_Indices = color_ریکارڈ_Indices_Z.as_array (عدد_Palettes);
    لكل (ثابت خودکار پہلا_color_ریکارڈ_idx : color_ریکارڈ_Indices)
    {
      إذا (پہلا_color_منزل_layer_اشاریہ.has (پہلا_color_ریکارڈ_idx)) استمر;

      hb_سمتیہ_t<BGRAColor> retained_colors;
      إذا (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (خطأ);
      لكل (hb_codepoint_t color_اشاریہ : retained_color_indices)
	retained_colors.push (all_color_ریکارڈاں[پہلا_color_ریکارڈ_idx + color_اشاریہ]);

      بنانشانی layer_اشاریہ = پہلا_color_اشاریہ_for_layer.لمائی;
      بنانشانی *candidate_layer_اشاریہ = خالی_سوچک;
      إذا (retained_colors_منزل_layer_اشاریہ.has (retained_colors.as_array (), &candidate_layer_اشاریہ))
	layer_اشاریہ = *candidate_layer_اشاریہ;
      وإلا
      {
	پہلا_color_اشاریہ_for_layer.push (پہلا_color_ریکارڈ_idx);
	إذا (unlikely (!c->serializer->propagate_error (پہلا_color_اشاریہ_for_layer))) return_trace (خطأ);

	إذا (unlikely (!layer_retained_colors.ڈالو_or_fail (std::move (retained_colors)))) return_trace (خطأ);
	إذا (unlikely (!retained_colors_منزل_layer_اشاریہ.سیٹ (layer_retained_colors.پوچھ ().as_array (), layer_اشاریہ)))
	  return_trace (خطأ);
      }
      پہلا_color_منزل_layer_اشاریہ.سیٹ (پہلا_color_ریکارڈ_idx, layer_اشاریہ);
      إذا (unlikely (!c->serializer->propagate_error (پہلا_color_منزل_layer_اشاریہ))) return_trace (خطأ);
    }

    باہرلا->عدد_Color_ریکارڈاں = پہلا_color_اشاریہ_for_layer.لمائی
                           * retained_color_indices.get_population ();

    إذا (!باہرلا->serialize (c->serializer,
                         color_ریکارڈ_Indices,
                         all_color_ریکارڈاں,
                         پہلا_color_اشاریہ_for_layer,
                         پہلا_color_منزل_layer_اشاریہ,
                         retained_color_indices))
      return_trace (خطأ);

    إذا (نسخہ == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, عدد_Palettes, عدد_Colors, هذا, color_اشاریہ_map));
    }

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (هذا+color_ریکارڈاں_Z).sanitize (c, عدد_Color_ریکارڈاں) &&
		  color_ریکارڈ_Indices_Z.sanitize (c, عدد_Palettes) &&
		  (نسخہ == 0 || v1 ().sanitize (c, هذا, عدد_Palettes, عدد_Colors)));
  }

  محمي:
  HBUINT16	نسخہ;		/* Table version number */
  /* Version 0 */
  HBUINT16	عدد_Colors;		/* Number of colors in each palette. */
  HBUINT16	عدد_Palettes;		/* Number of palettes in the table. */
  HBUINT16	عدد_Color_ریکارڈاں;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32_منزل<Unsized_Array_دا<BGRAColor>>
		color_ریکارڈاں_Z;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  Unsized_Array_دا<HBUINT16>
		color_ریکارڈ_Indices_Z;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  عام:
  DEFINE_ناپ_ARRAY (12, color_ریکارڈ_Indices_Z);
};

} /* namespace OT */


#جے_انت /* OT_COLOR_CPAL_CPAL_HH */

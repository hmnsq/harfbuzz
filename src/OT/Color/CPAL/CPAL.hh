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

#که_نه_تعریف_شوی OT_COLOR_CPAL_CPAL_HH
#تعریف_کړه OT_COLOR_CPAL_CPAL_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-ot-color.h"
#ضمن "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#تعریف_کړه HB_OT_نښان_CPAL HB_نښان('C','P','A','L')

نطاق OT {


بنية CPALV1_لکۍ
{
  ملګری بنية CPAL;

  خاص:
  hb_ot_color_palette_flags_t get_palette_flags (ثابت فراغ *بنسټ,
						 بینښه صحيح palette_شاخص,
						 بینښه صحيح palette_شمېر) ثابت
  {
    إذا (!paletteFlagsZ) أرجع HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    أرجع (hb_ot_color_palette_flags_t) (uint32_t)
	   (بنسټ+paletteFlagsZ).as_array (palette_شمېر)[palette_شاخص];
  }

  hb_ot_name_id_t get_palette_name_id (ثابت فراغ *بنسټ,
				       بینښه صحيح palette_شاخص,
				       بینښه صحيح palette_شمېر) ثابت
  {
    إذا (!paletteLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (بنسټ+paletteLabelsZ).as_array (palette_شمېر)[palette_شاخص];
  }

  hb_ot_name_id_t get_color_name_id (ثابت فراغ *بنسټ,
				     بینښه صحيح color_شاخص,
				     بینښه صحيح color_شمېر) ثابت
  {
    إذا (!colorLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (بنسټ+colorLabelsZ).as_array (color_شمېر)[color_شاخص];
  }

  عام:
  فراغ collect_name_ids (ثابت فراغ *بنسټ,
                         بینښه palette_شمېر,
                         بینښه color_شمېر,
                         ثابت hb_map_t *color_شاخص_map,
                         hb_سېټ_t *nameids_منزل_retain /*بهر*/) ثابت
  {
    إذا (paletteLabelsZ)
    {
      + (بنسټ+paletteLabelsZ).as_array (palette_شمېر)
      | hb_sink (nameids_منزل_retain)
      ;
    }

    إذا (colorLabelsZ)
    {
      ثابت hb_array_t<ثابت NameID> colorLabels = (بنسټ+colorLabelsZ).as_array (color_شمېر);
      لكل (بینښه i = 0; i < color_شمېر; i++)
      {
        إذا (!color_شاخص_map->has (i)) استمر;
        nameids_منزل_retain->جمع_کړه (colorLabels[i]);
      }
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
                  بینښه palette_شمېر,
                  بینښه color_شمېر,
                  ثابت فراغ *بنسټ,
                  ثابت hb_map_t *color_شاخص_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->allocate_اندازه<CPALV1_لکۍ> (static_اندازه);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->paletteFlagsZ = 0;
    إذا (paletteFlagsZ)
      بهر->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, بنسټ, 0, hb_serialize_context_t::سر, palette_شمېر);

    بهر->paletteLabelsZ = 0;
    إذا (paletteLabelsZ)
      بهر->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, بنسټ, 0, hb_serialize_context_t::سر, palette_شمېر);

    ثابت hb_array_t<ثابت NameID> colorLabels = (بنسټ+colorLabelsZ).as_array (color_شمېر);
    إذا (colorLabelsZ)
    {
      c->push ();
      لكل (بینښه i = 0; i < color_شمېر; i++)
      {
        إذا (!color_شاخص_map->has (i)) استمر;
        إذا (!c->copy<NameID> (colorLabels[i]))
        {
          c->راوباسه_discard ();
          return_trace (خطأ);
        }
      }
      c->جمع_کړه_link (بهر->colorLabelsZ, c->راوباسه_pack ());
    }
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *بنسټ,
		 بینښه صحيح palette_شمېر,
		 بینښه صحيح color_شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  (!paletteFlagsZ  || (بنسټ+paletteFlagsZ).sanitize (c, palette_شمېر)) &&
		  (!paletteLabelsZ || (بنسټ+paletteLabelsZ).sanitize (c, palette_شمېر)) &&
		  (!colorLabelsZ   || (بنسټ+colorLabelsZ).sanitize (c, color_شمېر)));
  }

  محمي:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32_منزل<Unsized_Array_د<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32_منزل<Unsized_Array_د<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32_منزل<Unsized_Array_د<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

تعريفنوع HBUINT32 BGRAColor;

بنية CPAL
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_CPAL;

  منطقي has_معلومات () ثابت { أرجع عدد_Palettes; }

  size_t get_اندازه () ثابت
  { أرجع min_اندازه + عدد_Palettes * حجم (color_ریکارډ_Indices_Z[0]); }

  بینښه صحيح get_palette_شمېر () ثابت { أرجع عدد_Palettes; }
  بینښه صحيح   get_color_شمېر () ثابت { أرجع عدد_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (بینښه صحيح palette_شاخص) ثابت
  { أرجع v1 ().get_palette_flags (هذا, palette_شاخص, عدد_Palettes); }

  hb_ot_name_id_t get_palette_name_id (بینښه صحيح palette_شاخص) ثابت
  { أرجع v1 ().get_palette_name_id (هذا, palette_شاخص, عدد_Palettes); }

  hb_ot_name_id_t get_color_name_id (بینښه صحيح color_شاخص) ثابت
  { أرجع v1 ().get_color_name_id (هذا, color_شاخص, عدد_Colors); }

  hb_array_t<ثابت BGRAColor> get_palette_colors (بینښه صحيح palette_شاخص) ثابت
  {
    إذا (unlikely (palette_شاخص >= عدد_Palettes))
      أرجع hb_array_t<ثابت BGRAColor> ();
    بینښه صحيح پیل_شاخص = color_ریکارډ_Indices_Z[palette_شاخص];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+color_ریکارډونه_Z).arrayZ, عدد_Color_ریکارډونه);
    أرجع all_colors.sub_array (پیل_شاخص, عدد_Colors);
  }
  بینښه صحيح get_palette_colors (بینښه صحيح  palette_شاخص,
				   بینښه صحيح  پیل_دننه_واټن,
				   بینښه صحيح *color_شمېر, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ثابت
  {
    إذا (unlikely (palette_شاخص >= عدد_Palettes))
    {
      إذا (color_شمېر) *color_شمېر = 0;
      أرجع 0;
    }
    بینښه صحيح پیل_شاخص = color_ریکارډ_Indices_Z[palette_شاخص];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+color_ریکارډونه_Z).arrayZ, عدد_Color_ریکارډونه);
    hb_array_t<ثابت BGRAColor> palette_colors = all_colors.sub_array (پیل_شاخص,
								       عدد_Colors);
    إذا (color_شمېر && colors)
    {
      + palette_colors.sub_array (پیل_دننه_واټن, color_شمېر)
      | hb_sink (hb_array (colors, *color_شمېر))
      ;
    }
    أرجع عدد_Colors;
  }

  فراغ collect_name_ids (ثابت hb_map_t *color_شاخص_map,
                         hb_سېټ_t *nameids_منزل_retain /*بهر*/) ثابت
  {
    إذا (نسخه == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (هذا, عدد_Palettes, عدد_Colors, color_شاخص_map, nameids_منزل_retain);
    }
  }

  خاص:
  ثابت CPALV1_لکۍ& v1 () ثابت
  {
    إذا (نسخه == 0) أرجع Null (CPALV1_لکۍ);
    hb_barrier ();
    أرجع StructAfter<CPALV1_لکۍ> (*هذا);
  }

  عام:
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_array_t<ثابت HBUINT16> &color_ریکارډ_indices,
                  ثابت hb_array_t<ثابت BGRAColor> &color_ریکارډونه,
                  ثابت hb_ویکټور_t<بینښه>& لومړی_color_شاخص_for_layer,
                  ثابت hb_map_t& لومړی_color_منزل_layer_شاخص,
                  ثابت hb_سېټ_t &retained_color_indices) ثابت
  {
    TRACE_SERIALIZE (هذا);

    // TODO(grieger): limit total final size.

    لكل (ثابت خپلکاری idx : color_ریکارډ_indices)
    {
      hb_codepoint_t layer_شاخص = لومړی_color_منزل_layer_شاخص[idx];

      HBUINT16 new_idx;
      new_idx = layer_شاخص * retained_color_indices.get_population ();
      إذا (!c->copy<HBUINT16> (new_idx)) return_trace (خطأ);
    }

    c->push ();
    لكل (بینښه لومړی_color_شاخص : لومړی_color_شاخص_for_layer)
    {
      لكل (hb_codepoint_t color_شاخص : retained_color_indices)
      {
        إذا (!c->copy<BGRAColor> (color_ریکارډونه[لومړی_color_شاخص + color_شاخص]))
        {
          c->راوباسه_discard ();
          return_trace (خطأ);
        }
      }
    }

    c->جمع_کړه_link (color_ریکارډونه_Z, c->راوباسه_pack ());
    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!عدد_Palettes) return_trace (خطأ);

    ثابت hb_map_t *color_شاخص_map = &c->plan->colr_palettes;
    إذا (color_شاخص_map->is_empty ()) return_trace (خطأ);

    hb_سېټ_t retained_color_indices;
    لكل (ثابت خپلکاری _ : color_شاخص_map->کلیګانې ())
    {
      إذا (_ == 0xFFFF) استمر;
      retained_color_indices.جمع_کړه (_);
    }
    إذا (retained_color_indices.is_empty ()) return_trace (خطأ);

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);


    بهر->نسخه = نسخه;
    بهر->عدد_Colors = retained_color_indices.get_population ();
    بهر->عدد_Palettes = عدد_Palettes;

    hb_ویکټور_t<بینښه> لومړی_color_شاخص_for_layer;
    hb_map_t لومړی_color_منزل_layer_شاخص;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ثابت BGRAColor>, بینښه> retained_colors_منزل_layer_شاخص;
    hb_ویکټور_t<hb_ویکټور_t<BGRAColor>> layer_retained_colors;

    ثابت hb_array_t<ثابت BGRAColor> all_color_ریکارډونه = (هذا+color_ریکارډونه_Z).as_array (عدد_Color_ریکارډونه);

    ثابت hb_array_t<ثابت HBUINT16> color_ریکارډ_Indices = color_ریکارډ_Indices_Z.as_array (عدد_Palettes);
    لكل (ثابت خپلکاری لومړی_color_ریکارډ_idx : color_ریکارډ_Indices)
    {
      إذا (لومړی_color_منزل_layer_شاخص.has (لومړی_color_ریکارډ_idx)) استمر;

      hb_ویکټور_t<BGRAColor> retained_colors;
      إذا (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (خطأ);
      لكل (hb_codepoint_t color_شاخص : retained_color_indices)
	retained_colors.push (all_color_ریکارډونه[لومړی_color_ریکارډ_idx + color_شاخص]);

      بینښه layer_شاخص = لومړی_color_شاخص_for_layer.اوږدتیا;
      بینښه *candidate_layer_شاخص = تش_اشاره;
      إذا (retained_colors_منزل_layer_شاخص.has (retained_colors.as_array (), &candidate_layer_شاخص))
	layer_شاخص = *candidate_layer_شاخص;
      وإلا
      {
	لومړی_color_شاخص_for_layer.push (لومړی_color_ریکارډ_idx);
	إذا (unlikely (!c->serializer->propagate_error (لومړی_color_شاخص_for_layer))) return_trace (خطأ);

	إذا (unlikely (!layer_retained_colors.ټېل_or_fail (std::move (retained_colors)))) return_trace (خطأ);
	إذا (unlikely (!retained_colors_منزل_layer_شاخص.سېټ (layer_retained_colors.لکۍ ().as_array (), layer_شاخص)))
	  return_trace (خطأ);
      }
      لومړی_color_منزل_layer_شاخص.سېټ (لومړی_color_ریکارډ_idx, layer_شاخص);
      إذا (unlikely (!c->serializer->propagate_error (لومړی_color_منزل_layer_شاخص))) return_trace (خطأ);
    }

    بهر->عدد_Color_ریکارډونه = لومړی_color_شاخص_for_layer.اوږدتیا
                           * retained_color_indices.get_population ();

    إذا (!بهر->serialize (c->serializer,
                         color_ریکارډ_Indices,
                         all_color_ریکارډونه,
                         لومړی_color_شاخص_for_layer,
                         لومړی_color_منزل_layer_شاخص,
                         retained_color_indices))
      return_trace (خطأ);

    إذا (نسخه == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, عدد_Palettes, عدد_Colors, هذا, color_شاخص_map));
    }

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (هذا+color_ریکارډونه_Z).sanitize (c, عدد_Color_ریکارډونه) &&
		  color_ریکارډ_Indices_Z.sanitize (c, عدد_Palettes) &&
		  (نسخه == 0 || v1 ().sanitize (c, هذا, عدد_Palettes, عدد_Colors)));
  }

  محمي:
  HBUINT16	نسخه;		/* Table version number */
  /* Version 0 */
  HBUINT16	عدد_Colors;		/* Number of colors in each palette. */
  HBUINT16	عدد_Palettes;		/* Number of palettes in the table. */
  HBUINT16	عدد_Color_ریکارډونه;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32_منزل<Unsized_Array_د<BGRAColor>>
		color_ریکارډونه_Z;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  Unsized_Array_د<HBUINT16>
		color_ریکارډ_Indices_Z;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  عام:
  DEFINE_اندازه_ARRAY (12, color_ریکارډ_Indices_Z);
};

} /* namespace OT */


#د_که_پای /* OT_COLOR_CPAL_CPAL_HH */

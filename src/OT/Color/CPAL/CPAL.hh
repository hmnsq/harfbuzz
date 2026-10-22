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

#اگر_غیر_تعریف_شدہ OT_COLOR_CPAL_CPAL_HH
#تعریف_کرو OT_COLOR_CPAL_CPAL_HH

#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-ot-color.h"
#شامل "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#تعریف_کرو HB_OT_لیبل_CPAL HB_لیبل('C','P','A','L')

نامخلا OT {


ساخت CPALV1_دم
{
  دوست ساخت CPAL;

  نجی:
  hb_ot_color_palette_flags_t get_palette_flags (ثابت خالی *بنیاد,
						 غیرعلامتی صحیح palette_اشاریہ,
						 غیرعلامتی صحیح palette_گنتی) ثابت
  {
    اگر (!paletteFlagsZ) واپس HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    واپس (hb_ot_color_palette_flags_t) (uint32_t)
	   (بنیاد+paletteFlagsZ).as_array (palette_گنتی)[palette_اشاریہ];
  }

  hb_ot_name_id_t get_palette_name_id (ثابت خالی *بنیاد,
				       غیرعلامتی صحیح palette_اشاریہ,
				       غیرعلامتی صحیح palette_گنتی) ثابت
  {
    اگر (!paletteLabelsZ) واپس HB_OT_NAME_ID_INVALID;
    واپس (بنیاد+paletteLabelsZ).as_array (palette_گنتی)[palette_اشاریہ];
  }

  hb_ot_name_id_t get_color_name_id (ثابت خالی *بنیاد,
				     غیرعلامتی صحیح color_اشاریہ,
				     غیرعلامتی صحیح color_گنتی) ثابت
  {
    اگر (!colorLabelsZ) واپس HB_OT_NAME_ID_INVALID;
    واپس (بنیاد+colorLabelsZ).as_array (color_گنتی)[color_اشاریہ];
  }

  عوامی:
  خالی collect_name_ids (ثابت خالی *بنیاد,
                         غیرعلامتی palette_گنتی,
                         غیرعلامتی color_گنتی,
                         ثابت hb_map_t *color_اشاریہ_map,
                         hb_سیٹ_t *nameids_مقصد_retain /*برآمد*/) ثابت
  {
    اگر (paletteLabelsZ)
    {
      + (بنیاد+paletteLabelsZ).as_array (palette_گنتی)
      | hb_sink (nameids_مقصد_retain)
      ;
    }

    اگر (colorLabelsZ)
    {
      ثابت hb_array_t<ثابت NameID> colorLabels = (بنیاد+colorLabelsZ).as_array (color_گنتی);
      برائے (غیرعلامتی i = 0; i < color_گنتی; i++)
      {
        اگر (!color_اشاریہ_map->has (i)) جاری;
        nameids_مقصد_retain->جوڑو (colorLabels[i]);
      }
    }
  }

  منطقی serialize (hb_serialize_context_t *c,
                  غیرعلامتی palette_گنتی,
                  غیرعلامتی color_گنتی,
                  ثابت خالی *بنیاد,
                  ثابت hb_map_t *color_اشاریہ_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->allocate_سائز<CPALV1_دم> (static_سائز);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->paletteFlagsZ = 0;
    اگر (paletteFlagsZ)
      برآمد->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, بنیاد, 0, hb_serialize_context_t::سر, palette_گنتی);

    برآمد->paletteLabelsZ = 0;
    اگر (paletteLabelsZ)
      برآمد->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, بنیاد, 0, hb_serialize_context_t::سر, palette_گنتی);

    ثابت hb_array_t<ثابت NameID> colorLabels = (بنیاد+colorLabelsZ).as_array (color_گنتی);
    اگر (colorLabelsZ)
    {
      c->push ();
      برائے (غیرعلامتی i = 0; i < color_گنتی; i++)
      {
        اگر (!color_اشاریہ_map->has (i)) جاری;
        اگر (!c->copy<NameID> (colorLabels[i]))
        {
          c->نکالو_discard ();
          return_trace (جھوٹ);
        }
      }
      c->جوڑو_link (برآمد->colorLabelsZ, c->نکالو_pack ());
    }
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت خالی *بنیاد,
		 غیرعلامتی صحیح palette_گنتی,
		 غیرعلامتی صحیح color_گنتی) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  (!paletteFlagsZ  || (بنیاد+paletteFlagsZ).sanitize (c, palette_گنتی)) &&
		  (!paletteLabelsZ || (بنیاد+paletteLabelsZ).sanitize (c, palette_گنتی)) &&
		  (!colorLabelsZ   || (بنیاد+colorLabelsZ).sanitize (c, color_گنتی)));
  }

  محفوظ:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32_مقصد<Unsized_Array_کا<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32_مقصد<Unsized_Array_کا<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32_مقصد<Unsized_Array_کا<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  عوامی:
  DEFINE_سائز_STATIC (12);
};

قسمتعریف HBUINT32 BGRAColor;

ساخت CPAL
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_CPAL;

  منطقی has_ڈیٹا () ثابت { واپس عدد_Palettes; }

  size_t get_سائز () ثابت
  { واپس min_سائز + عدد_Palettes * حجم (color_ریکارڈ_Indices_Z[0]); }

  غیرعلامتی صحیح get_palette_گنتی () ثابت { واپس عدد_Palettes; }
  غیرعلامتی صحیح   get_color_گنتی () ثابت { واپس عدد_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (غیرعلامتی صحیح palette_اشاریہ) ثابت
  { واپس v1 ().get_palette_flags (یہ, palette_اشاریہ, عدد_Palettes); }

  hb_ot_name_id_t get_palette_name_id (غیرعلامتی صحیح palette_اشاریہ) ثابت
  { واپس v1 ().get_palette_name_id (یہ, palette_اشاریہ, عدد_Palettes); }

  hb_ot_name_id_t get_color_name_id (غیرعلامتی صحیح color_اشاریہ) ثابت
  { واپس v1 ().get_color_name_id (یہ, color_اشاریہ, عدد_Colors); }

  hb_array_t<ثابت BGRAColor> get_palette_colors (غیرعلامتی صحیح palette_اشاریہ) ثابت
  {
    اگر (unlikely (palette_اشاریہ >= عدد_Palettes))
      واپس hb_array_t<ثابت BGRAColor> ();
    غیرعلامتی صحیح آغاز_اشاریہ = color_ریکارڈ_Indices_Z[palette_اشاریہ];
    hb_array_t<ثابت BGRAColor> all_colors ((یہ+color_اندراجات_Z).arrayZ, عدد_Color_اندراجات);
    واپس all_colors.sub_array (آغاز_اشاریہ, عدد_Colors);
  }
  غیرعلامتی صحیح get_palette_colors (غیرعلامتی صحیح  palette_اشاریہ,
				   غیرعلامتی صحیح  آغاز_انحراف,
				   غیرعلامتی صحیح *color_گنتی, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ثابت
  {
    اگر (unlikely (palette_اشاریہ >= عدد_Palettes))
    {
      اگر (color_گنتی) *color_گنتی = 0;
      واپس 0;
    }
    غیرعلامتی صحیح آغاز_اشاریہ = color_ریکارڈ_Indices_Z[palette_اشاریہ];
    hb_array_t<ثابت BGRAColor> all_colors ((یہ+color_اندراجات_Z).arrayZ, عدد_Color_اندراجات);
    hb_array_t<ثابت BGRAColor> palette_colors = all_colors.sub_array (آغاز_اشاریہ,
								       عدد_Colors);
    اگر (color_گنتی && colors)
    {
      + palette_colors.sub_array (آغاز_انحراف, color_گنتی)
      | hb_sink (hb_array (colors, *color_گنتی))
      ;
    }
    واپس عدد_Colors;
  }

  خالی collect_name_ids (ثابت hb_map_t *color_اشاریہ_map,
                         hb_سیٹ_t *nameids_مقصد_retain /*برآمد*/) ثابت
  {
    اگر (نسخہ == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (یہ, عدد_Palettes, عدد_Colors, color_اشاریہ_map, nameids_مقصد_retain);
    }
  }

  نجی:
  ثابت CPALV1_دم& v1 () ثابت
  {
    اگر (نسخہ == 0) واپس Null (CPALV1_دم);
    hb_barrier ();
    واپس StructAfter<CPALV1_دم> (*یہ);
  }

  عوامی:
  منطقی serialize (hb_serialize_context_t *c,
                  ثابت hb_array_t<ثابت HBUINT16> &color_ریکارڈ_indices,
                  ثابت hb_array_t<ثابت BGRAColor> &color_اندراجات,
                  ثابت hb_سمتیہ_t<غیرعلامتی>& پہلا_color_اشاریہ_for_layer,
                  ثابت hb_map_t& پہلا_color_مقصد_layer_اشاریہ,
                  ثابت hb_سیٹ_t &retained_color_indices) ثابت
  {
    TRACE_SERIALIZE (یہ);

    // TODO(grieger): limit total final size.

    برائے (ثابت خودکار idx : color_ریکارڈ_indices)
    {
      hb_codepoint_t layer_اشاریہ = پہلا_color_مقصد_layer_اشاریہ[idx];

      HBUINT16 new_idx;
      new_idx = layer_اشاریہ * retained_color_indices.get_population ();
      اگر (!c->copy<HBUINT16> (new_idx)) return_trace (جھوٹ);
    }

    c->push ();
    برائے (غیرعلامتی پہلا_color_اشاریہ : پہلا_color_اشاریہ_for_layer)
    {
      برائے (hb_codepoint_t color_اشاریہ : retained_color_indices)
      {
        اگر (!c->copy<BGRAColor> (color_اندراجات[پہلا_color_اشاریہ + color_اشاریہ]))
        {
          c->نکالو_discard ();
          return_trace (جھوٹ);
        }
      }
    }

    c->جوڑو_link (color_اندراجات_Z, c->نکالو_pack ());
    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (!عدد_Palettes) return_trace (جھوٹ);

    ثابت hb_map_t *color_اشاریہ_map = &c->plan->colr_palettes;
    اگر (color_اشاریہ_map->is_empty ()) return_trace (جھوٹ);

    hb_سیٹ_t retained_color_indices;
    برائے (ثابت خودکار _ : color_اشاریہ_map->کلیدیں ())
    {
      اگر (_ == 0xFFFF) جاری;
      retained_color_indices.جوڑو (_);
    }
    اگر (retained_color_indices.is_empty ()) return_trace (جھوٹ);

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);


    برآمد->نسخہ = نسخہ;
    برآمد->عدد_Colors = retained_color_indices.get_population ();
    برآمد->عدد_Palettes = عدد_Palettes;

    hb_سمتیہ_t<غیرعلامتی> پہلا_color_اشاریہ_for_layer;
    hb_map_t پہلا_color_مقصد_layer_اشاریہ;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ثابت BGRAColor>, غیرعلامتی> retained_colors_مقصد_layer_اشاریہ;
    hb_سمتیہ_t<hb_سمتیہ_t<BGRAColor>> layer_retained_colors;

    ثابت hb_array_t<ثابت BGRAColor> all_color_اندراجات = (یہ+color_اندراجات_Z).as_array (عدد_Color_اندراجات);

    ثابت hb_array_t<ثابت HBUINT16> color_ریکارڈ_Indices = color_ریکارڈ_Indices_Z.as_array (عدد_Palettes);
    برائے (ثابت خودکار پہلا_color_ریکارڈ_idx : color_ریکارڈ_Indices)
    {
      اگر (پہلا_color_مقصد_layer_اشاریہ.has (پہلا_color_ریکارڈ_idx)) جاری;

      hb_سمتیہ_t<BGRAColor> retained_colors;
      اگر (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (جھوٹ);
      برائے (hb_codepoint_t color_اشاریہ : retained_color_indices)
	retained_colors.push (all_color_اندراجات[پہلا_color_ریکارڈ_idx + color_اشاریہ]);

      غیرعلامتی layer_اشاریہ = پہلا_color_اشاریہ_for_layer.لمبائی;
      غیرعلامتی *candidate_layer_اشاریہ = خالی_اشاریہ;
      اگر (retained_colors_مقصد_layer_اشاریہ.has (retained_colors.as_array (), &candidate_layer_اشاریہ))
	layer_اشاریہ = *candidate_layer_اشاریہ;
      ورنہ
      {
	پہلا_color_اشاریہ_for_layer.push (پہلا_color_ریکارڈ_idx);
	اگر (unlikely (!c->serializer->propagate_error (پہلا_color_اشاریہ_for_layer))) return_trace (جھوٹ);

	اگر (unlikely (!layer_retained_colors.ڈالو_or_fail (std::move (retained_colors)))) return_trace (جھوٹ);
	اگر (unlikely (!retained_colors_مقصد_layer_اشاریہ.سیٹ (layer_retained_colors.دم ().as_array (), layer_اشاریہ)))
	  return_trace (جھوٹ);
      }
      پہلا_color_مقصد_layer_اشاریہ.سیٹ (پہلا_color_ریکارڈ_idx, layer_اشاریہ);
      اگر (unlikely (!c->serializer->propagate_error (پہلا_color_مقصد_layer_اشاریہ))) return_trace (جھوٹ);
    }

    برآمد->عدد_Color_اندراجات = پہلا_color_اشاریہ_for_layer.لمبائی
                           * retained_color_indices.get_population ();

    اگر (!برآمد->serialize (c->serializer,
                         color_ریکارڈ_Indices,
                         all_color_اندراجات,
                         پہلا_color_اشاریہ_for_layer,
                         پہلا_color_مقصد_layer_اشاریہ,
                         retained_color_indices))
      return_trace (جھوٹ);

    اگر (نسخہ == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, عدد_Palettes, عدد_Colors, یہ, color_اشاریہ_map));
    }

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  (یہ+color_اندراجات_Z).sanitize (c, عدد_Color_اندراجات) &&
		  color_ریکارڈ_Indices_Z.sanitize (c, عدد_Palettes) &&
		  (نسخہ == 0 || v1 ().sanitize (c, یہ, عدد_Palettes, عدد_Colors)));
  }

  محفوظ:
  HBUINT16	نسخہ;		/* Table version number */
  /* Version 0 */
  HBUINT16	عدد_Colors;		/* Number of colors in each palette. */
  HBUINT16	عدد_Palettes;		/* Number of palettes in the table. */
  HBUINT16	عدد_Color_اندراجات;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32_مقصد<Unsized_Array_کا<BGRAColor>>
		color_اندراجات_Z;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  Unsized_Array_کا<HBUINT16>
		color_ریکارڈ_Indices_Z;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  عوامی:
  DEFINE_سائز_ARRAY (12, color_ریکارڈ_Indices_Z);
};

} /* namespace OT */


#اگر_ختم /* OT_COLOR_CPAL_CPAL_HH */

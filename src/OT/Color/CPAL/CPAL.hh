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

#اگر_تعریف_نشده OT_COLOR_CPAL_CPAL_HH
#تعریف_کن OT_COLOR_CPAL_CPAL_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-ot-color.h"
#ضمن "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#تعریف_کن HB_OT_برچسب_CPAL HB_برچسب('C','P','A','L')

نطاق OT {


بنية CPALV1_دم
{
  دوست بنية CPAL;

  خاص:
  hb_ot_color_palette_flags_t get_palette_flags (ثابت فراغ *پایه,
						 بیعلامت صحيح palette_شاخص,
						 بیعلامت صحيح palette_شمار) ثابت
  {
    إذا (!paletteFlagsZ) أرجع HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    أرجع (hb_ot_color_palette_flags_t) (uint32_t)
	   (پایه+paletteFlagsZ).as_array (palette_شمار)[palette_شاخص];
  }

  hb_ot_name_id_t get_palette_name_id (ثابت فراغ *پایه,
				       بیعلامت صحيح palette_شاخص,
				       بیعلامت صحيح palette_شمار) ثابت
  {
    إذا (!paletteLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (پایه+paletteLabelsZ).as_array (palette_شمار)[palette_شاخص];
  }

  hb_ot_name_id_t get_color_name_id (ثابت فراغ *پایه,
				     بیعلامت صحيح color_شاخص,
				     بیعلامت صحيح color_شمار) ثابت
  {
    إذا (!colorLabelsZ) أرجع HB_OT_NAME_ID_INVALID;
    أرجع (پایه+colorLabelsZ).as_array (color_شمار)[color_شاخص];
  }

  عام:
  فراغ collect_name_ids (ثابت فراغ *پایه,
                         بیعلامت palette_شمار,
                         بیعلامت color_شمار,
                         ثابت hb_map_t *color_شاخص_map,
                         hb_مجموعه_t *nameids_مقصد_retain /*بیرون*/) ثابت
  {
    إذا (paletteLabelsZ)
    {
      + (پایه+paletteLabelsZ).as_array (palette_شمار)
      | hb_sink (nameids_مقصد_retain)
      ;
    }

    إذا (colorLabelsZ)
    {
      ثابت hb_array_t<ثابت NameID> colorLabels = (پایه+colorLabelsZ).as_array (color_شمار);
      لكل (بیعلامت i = 0; i < color_شمار; i++)
      {
        إذا (!color_شاخص_map->has (i)) استمر;
        nameids_مقصد_retain->جمع_کن (colorLabels[i]);
      }
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
                  بیعلامت palette_شمار,
                  بیعلامت color_شمار,
                  ثابت فراغ *پایه,
                  ثابت hb_map_t *color_شاخص_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->allocate_اندازه<CPALV1_دم> (static_اندازه);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->paletteFlagsZ = 0;
    إذا (paletteFlagsZ)
      بیرون->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, پایه, 0, hb_serialize_context_t::سر, palette_شمار);

    بیرون->paletteLabelsZ = 0;
    إذا (paletteLabelsZ)
      بیرون->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, پایه, 0, hb_serialize_context_t::سر, palette_شمار);

    ثابت hb_array_t<ثابت NameID> colorLabels = (پایه+colorLabelsZ).as_array (color_شمار);
    إذا (colorLabelsZ)
    {
      c->push ();
      لكل (بیعلامت i = 0; i < color_شمار; i++)
      {
        إذا (!color_شاخص_map->has (i)) استمر;
        إذا (!c->copy<NameID> (colorLabels[i]))
        {
          c->برداشت_discard ();
          return_trace (خطأ);
        }
      }
      c->جمع_کن_link (بیرون->colorLabelsZ, c->برداشت_pack ());
    }
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *پایه,
		 بیعلامت صحيح palette_شمار,
		 بیعلامت صحيح color_شمار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  (!paletteFlagsZ  || (پایه+paletteFlagsZ).sanitize (c, palette_شمار)) &&
		  (!paletteLabelsZ || (پایه+paletteLabelsZ).sanitize (c, palette_شمار)) &&
		  (!colorLabelsZ   || (پایه+colorLabelsZ).sanitize (c, color_شمار)));
  }

  محمي:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32_مقصد<Unsized_Array_از<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32_مقصد<Unsized_Array_از<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32_مقصد<Unsized_Array_از<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

تعريفنوع HBUINT32 BGRAColor;

بنية CPAL
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_CPAL;

  منطقي has_داده () ثابت { أرجع رقم_Palettes; }

  size_t get_اندازه () ثابت
  { أرجع min_اندازه + رقم_Palettes * حجم (color_رکورد_Indices_Z[0]); }

  بیعلامت صحيح get_palette_شمار () ثابت { أرجع رقم_Palettes; }
  بیعلامت صحيح   get_color_شمار () ثابت { أرجع رقم_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (بیعلامت صحيح palette_شاخص) ثابت
  { أرجع v1 ().get_palette_flags (هذا, palette_شاخص, رقم_Palettes); }

  hb_ot_name_id_t get_palette_name_id (بیعلامت صحيح palette_شاخص) ثابت
  { أرجع v1 ().get_palette_name_id (هذا, palette_شاخص, رقم_Palettes); }

  hb_ot_name_id_t get_color_name_id (بیعلامت صحيح color_شاخص) ثابت
  { أرجع v1 ().get_color_name_id (هذا, color_شاخص, رقم_Colors); }

  hb_array_t<ثابت BGRAColor> get_palette_colors (بیعلامت صحيح palette_شاخص) ثابت
  {
    إذا (unlikely (palette_شاخص >= رقم_Palettes))
      أرجع hb_array_t<ثابت BGRAColor> ();
    بیعلامت صحيح شروع_شاخص = color_رکورد_Indices_Z[palette_شاخص];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+color_رکوردها_Z).arrayZ, رقم_Color_رکوردها);
    أرجع all_colors.sub_array (شروع_شاخص, رقم_Colors);
  }
  بیعلامت صحيح get_palette_colors (بیعلامت صحيح  palette_شاخص,
				   بیعلامت صحيح  شروع_جابه_جایی,
				   بیعلامت صحيح *color_شمار, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) ثابت
  {
    إذا (unlikely (palette_شاخص >= رقم_Palettes))
    {
      إذا (color_شمار) *color_شمار = 0;
      أرجع 0;
    }
    بیعلامت صحيح شروع_شاخص = color_رکورد_Indices_Z[palette_شاخص];
    hb_array_t<ثابت BGRAColor> all_colors ((هذا+color_رکوردها_Z).arrayZ, رقم_Color_رکوردها);
    hb_array_t<ثابت BGRAColor> palette_colors = all_colors.sub_array (شروع_شاخص,
								       رقم_Colors);
    إذا (color_شمار && colors)
    {
      + palette_colors.sub_array (شروع_جابه_جایی, color_شمار)
      | hb_sink (hb_array (colors, *color_شمار))
      ;
    }
    أرجع رقم_Colors;
  }

  فراغ collect_name_ids (ثابت hb_map_t *color_شاخص_map,
                         hb_مجموعه_t *nameids_مقصد_retain /*بیرون*/) ثابت
  {
    إذا (نسخه == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (هذا, رقم_Palettes, رقم_Colors, color_شاخص_map, nameids_مقصد_retain);
    }
  }

  خاص:
  ثابت CPALV1_دم& v1 () ثابت
  {
    إذا (نسخه == 0) أرجع Null (CPALV1_دم);
    hb_barrier ();
    أرجع StructAfter<CPALV1_دم> (*هذا);
  }

  عام:
  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_array_t<ثابت HBUINT16> &color_رکورد_indices,
                  ثابت hb_array_t<ثابت BGRAColor> &color_رکوردها,
                  ثابت hb_وکتور_t<بیعلامت>& اولی_color_شاخص_for_layer,
                  ثابت hb_map_t& اولی_color_مقصد_layer_شاخص,
                  ثابت hb_مجموعه_t &retained_color_indices) ثابت
  {
    TRACE_SERIALIZE (هذا);

    // TODO(grieger): limit total final size.

    لكل (ثابت خودکار idx : color_رکورد_indices)
    {
      hb_codepoint_t layer_شاخص = اولی_color_مقصد_layer_شاخص[idx];

      HBUINT16 new_idx;
      new_idx = layer_شاخص * retained_color_indices.get_population ();
      إذا (!c->copy<HBUINT16> (new_idx)) return_trace (خطأ);
    }

    c->push ();
    لكل (بیعلامت اولی_color_شاخص : اولی_color_شاخص_for_layer)
    {
      لكل (hb_codepoint_t color_شاخص : retained_color_indices)
      {
        إذا (!c->copy<BGRAColor> (color_رکوردها[اولی_color_شاخص + color_شاخص]))
        {
          c->برداشت_discard ();
          return_trace (خطأ);
        }
      }
    }

    c->جمع_کن_link (color_رکوردها_Z, c->برداشت_pack ());
    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!رقم_Palettes) return_trace (خطأ);

    ثابت hb_map_t *color_شاخص_map = &c->plan->colr_palettes;
    إذا (color_شاخص_map->is_empty ()) return_trace (خطأ);

    hb_مجموعه_t retained_color_indices;
    لكل (ثابت خودکار _ : color_شاخص_map->کلیدها ())
    {
      إذا (_ == 0xFFFF) استمر;
      retained_color_indices.جمع_کن (_);
    }
    إذا (retained_color_indices.is_empty ()) return_trace (خطأ);

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);


    بیرون->نسخه = نسخه;
    بیرون->رقم_Colors = retained_color_indices.get_population ();
    بیرون->رقم_Palettes = رقم_Palettes;

    hb_وکتور_t<بیعلامت> اولی_color_شاخص_for_layer;
    hb_map_t اولی_color_مقصد_layer_شاخص;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<ثابت BGRAColor>, بیعلامت> retained_colors_مقصد_layer_شاخص;
    hb_وکتور_t<hb_وکتور_t<BGRAColor>> layer_retained_colors;

    ثابت hb_array_t<ثابت BGRAColor> all_color_رکوردها = (هذا+color_رکوردها_Z).as_array (رقم_Color_رکوردها);

    ثابت hb_array_t<ثابت HBUINT16> color_رکورد_Indices = color_رکورد_Indices_Z.as_array (رقم_Palettes);
    لكل (ثابت خودکار اولی_color_رکورد_idx : color_رکورد_Indices)
    {
      إذا (اولی_color_مقصد_layer_شاخص.has (اولی_color_رکورد_idx)) استمر;

      hb_وکتور_t<BGRAColor> retained_colors;
      إذا (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (خطأ);
      لكل (hb_codepoint_t color_شاخص : retained_color_indices)
	retained_colors.push (all_color_رکوردها[اولی_color_رکورد_idx + color_شاخص]);

      بیعلامت layer_شاخص = اولی_color_شاخص_for_layer.درازی;
      بیعلامت *candidate_layer_شاخص = اشاره_تهی;
      إذا (retained_colors_مقصد_layer_شاخص.has (retained_colors.as_array (), &candidate_layer_شاخص))
	layer_شاخص = *candidate_layer_شاخص;
      وإلا
      {
	اولی_color_شاخص_for_layer.push (اولی_color_رکورد_idx);
	إذا (unlikely (!c->serializer->propagate_error (اولی_color_شاخص_for_layer))) return_trace (خطأ);

	إذا (unlikely (!layer_retained_colors.افزودن_or_fail (std::move (retained_colors)))) return_trace (خطأ);
	إذا (unlikely (!retained_colors_مقصد_layer_شاخص.مجموعه (layer_retained_colors.دم ().as_array (), layer_شاخص)))
	  return_trace (خطأ);
      }
      اولی_color_مقصد_layer_شاخص.مجموعه (اولی_color_رکورد_idx, layer_شاخص);
      إذا (unlikely (!c->serializer->propagate_error (اولی_color_مقصد_layer_شاخص))) return_trace (خطأ);
    }

    بیرون->رقم_Color_رکوردها = اولی_color_شاخص_for_layer.درازی
                           * retained_color_indices.get_population ();

    إذا (!بیرون->serialize (c->serializer,
                         color_رکورد_Indices,
                         all_color_رکوردها,
                         اولی_color_شاخص_for_layer,
                         اولی_color_مقصد_layer_شاخص,
                         retained_color_indices))
      return_trace (خطأ);

    إذا (نسخه == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, رقم_Palettes, رقم_Colors, هذا, color_شاخص_map));
    }

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (هذا+color_رکوردها_Z).sanitize (c, رقم_Color_رکوردها) &&
		  color_رکورد_Indices_Z.sanitize (c, رقم_Palettes) &&
		  (نسخه == 0 || v1 ().sanitize (c, هذا, رقم_Palettes, رقم_Colors)));
  }

  محمي:
  HBUINT16	نسخه;		/* Table version number */
  /* Version 0 */
  HBUINT16	رقم_Colors;		/* Number of colors in each palette. */
  HBUINT16	رقم_Palettes;		/* Number of palettes in the table. */
  HBUINT16	رقم_Color_رکوردها;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32_مقصد<Unsized_Array_از<BGRAColor>>
		color_رکوردها_Z;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  Unsized_Array_از<HBUINT16>
		color_رکورد_Indices_Z;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  عام:
  DEFINE_اندازه_ARRAY (12, color_رکورد_Indices_Z);
};

} /* namespace OT */


#پایان_اگر /* OT_COLOR_CPAL_CPAL_HH */

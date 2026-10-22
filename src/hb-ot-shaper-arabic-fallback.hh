/*
 * Copyright © 2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_تعریف_نشده HB_OT_SHAPER_ARABIC_FALLBACK_HH
#تعریف_کن HB_OT_SHAPER_ARABIC_FALLBACK_HH

#ضمن "hb.hh"

#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
ساكن ثابت hb_برچسب_t arabic_fallback_features[] =
{
  HB_برچسب('i','n','i','t'),
  HB_برچسب('m','e','d','i'),
  HB_برچسب('f','i','n','a'),
  HB_برچسب('i','s','o','l'),
  HB_برچسب('r','l','i','g'),
  HB_برچسب('r','l','i','g'),
  HB_برچسب('r','l','i','g'),
};

ساكن OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
					  hb_font_t *font,
					  بیعلامت صحيح feature_شاخص)
{
  OT::HBGlyphID16 glyphs[SHAPING_جدول_آخرین - SHAPING_جدول_اولی + 1];
  OT::HBGlyphID16 substitutes[SHAPING_جدول_آخرین - SHAPING_جدول_اولی + 1];
  بیعلامت صحيح رقم_glyphs = 0;

  /* Populate arrays */
  لكل (hb_codepoint_t u = SHAPING_جدول_اولی; u < SHAPING_جدول_آخرین + 1; u++)
  {
    hb_codepoint_t s = shaping_جدول[u - SHAPING_جدول_اولی][feature_شاخص];
    hb_codepoint_t u_glyph, s_glyph;

    إذا (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      استمر;

    glyphs[رقم_glyphs] = u_glyph;
    substitutes[رقم_glyphs] = s_glyph;

    رقم_glyphs++;
  }

  إذا (!رقم_glyphs)
    أرجع اشاره_تهی;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_مرتب_سازی (&glyphs[0], رقم_glyphs,
		  (صحيح(*)(ثابت OT::HBUINT16*, ثابت OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  حرف buf[(SHAPING_جدول_آخرین - SHAPING_جدول_اولی + 1) * 4 + 128];
  hb_serialize_context_t c (buf, حجم (buf));
  OT::SubstLookup *lookup = c.شروع_serialize<OT::SubstLookup> ();
  منطقي ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, رقم_glyphs),
				       hb_array (substitutes, رقم_glyphs));
  c.انتها_serialize ();

  أرجع ret && !c.درون_error () ? c.copy<OT::SubstLookup> () : اشاره_تهی;
}

قالب <نام_نوع T>
ساكن OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
					    hb_font_t *font,
					    ثابت T &ligature_جدول,
					    بیعلامت lookup_flags)
{
  OT::HBGlyphID16 اولی_glyphs[ARRAY_درازی_CONST (ligature_جدول)];
  بیعلامت صحيح اولی_glyphs_indirection[ARRAY_درازی_CONST (ligature_جدول)];
  بیعلامت صحيح ligature_در_هر_اولی_glyph_شمار_list[ARRAY_درازی_CONST (اولی_glyphs)];
  بیعلامت صحيح رقم_اولی_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_درازی_CONST (اولی_glyphs) * ARRAY_درازی_CONST(ligature_جدول[0].ligatures)];
  بیعلامت صحيح component_شمار_list[ARRAY_درازی_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_درازی_CONST (ligature_list) *
				 ARRAY_درازی_CONST (ligature_جدول[0].ligatures[0].components)];
  بیعلامت صحيح رقم_ligatures = 0;
  بیعلامت صحيح رقم_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  لكل (بیعلامت صحيح اولی_glyph_idx = 0; اولی_glyph_idx < ARRAY_درازی (اولی_glyphs); اولی_glyph_idx++)
  {
    hb_codepoint_t اولی_u = ligature_جدول[اولی_glyph_idx].اولی;
    hb_codepoint_t اولی_glyph;
    إذا (!hb_font_get_glyph (font, اولی_u, 0, &اولی_glyph))
      استمر;
    اولی_glyphs[رقم_اولی_glyphs] = اولی_glyph;
    ligature_در_هر_اولی_glyph_شمار_list[رقم_اولی_glyphs] = 0;
    اولی_glyphs_indirection[رقم_اولی_glyphs] = اولی_glyph_idx;
    رقم_اولی_glyphs++;
  }
  hb_stable_مرتب_سازی (&اولی_glyphs[0], رقم_اولی_glyphs,
		  (صحيح(*)(ثابت OT::HBUINT16*, ثابت OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &اولی_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  لكل (بیعلامت صحيح i = 0; i < رقم_اولی_glyphs; i++)
  {
    بیعلامت صحيح اولی_glyph_idx = اولی_glyphs_indirection[i];

    لكل (بیعلامت صحيح ligature_idx = 0; ligature_idx < ARRAY_درازی (ligature_جدول[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_جدول[اولی_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      إذا (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	استمر;

      ثابت خودکار &components = ligature_جدول[اولی_glyph_idx].ligatures[ligature_idx].components;
      بیعلامت component_شمار = ARRAY_درازی_CONST (components);

      منطقي matched = درست;
      لكل (بیعلامت j = 0; j < component_شمار; j++)
      {
	hb_codepoint_t component_u   = ligature_جدول[اولی_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	إذا (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = خطأ;
	  اكسر;
	}

	component_list[رقم_components++] = component_glyph;
      }
      إذا (!matched)
        استمر;

      component_شمار_list[رقم_ligatures] = 1 + component_شمار;
      ligature_list[رقم_ligatures] = ligature_glyph;

      ligature_در_هر_اولی_glyph_شمار_list[i]++;

      رقم_ligatures++;
    }
  }

  إذا (!رقم_ligatures)
    أرجع اشاره_تهی;


  /* 16 bytes per ligature ought to be enough... */
  حرف buf[ARRAY_درازی_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, حجم (buf));
  OT::SubstLookup *lookup = c.شروع_serialize<OT::SubstLookup> ();
  منطقي ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (اولی_glyphs, رقم_اولی_glyphs),
					 hb_array (ligature_در_هر_اولی_glyph_شمار_list, رقم_اولی_glyphs),
					 hb_array (ligature_list, رقم_ligatures),
					 hb_array (component_شمار_list, رقم_ligatures),
					 hb_array (component_list, رقم_components));
  c.انتها_serialize ();

  أرجع ret && !c.درون_error () ? c.copy<OT::SubstLookup> () : اشاره_تهی;
}

ساكن OT::SubstLookup *
arabic_fallback_synthesize_lookup (ثابت hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   بیعلامت صحيح feature_شاخص)
{
  إذا (feature_شاخص < 4)
    أرجع arabic_fallback_synthesize_lookup_single (plan, font, feature_شاخص);
  وإلا
  {
    بدل (feature_شاخص) {
      حالة 4: أرجع arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_جدول, OT::LookupFlag::IgnoreMarks);
      حالة 5: أرجع arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_جدول, OT::LookupFlag::IgnoreMarks);
      حالة 6: أرجع arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_جدول, 0);
    }
  }
  أكد (خطأ);
  أرجع اشاره_تهی;
}

#تعریف_کن ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_درازی_CONST (arabic_fallback_features)

بنية arabic_fallback_plan_t
{
  بیعلامت صحيح رقم_lookups;
  منطقي free_lookups;

  hb_ماسک_t ماسک_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#اگر_تعریف_نشده HB_NO_WIN1256
#ضمن "hb-ot-shaper-arabic-win1256.hh"
#پایان_اگر

بنية ManifestLookup
{
  عام:
  OT::برچسب برچسب_2;
  OT::Offset16_مقصد<OT::SubstLookup> lookup_جابه_جایی;
  عام:
  DEFINE_اندازه_STATIC (6);
};
تعريفنوع OT::Array16_از<ManifestLookup> Manifest;

ساكن منطقي
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_استفادهنشده,
				   ثابت hb_ot_shape_plan_t *plan HB_استفادهنشده,
				   hb_font_t *font HB_استفادهنشده)
{
#اگر_تعریف_نشده HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  إذا (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    أرجع خطأ;

  ثابت Manifest &manifest = reinterpret_cast<ثابت Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (حجم (arabic_win1256_gsub_lookups.manifest_داده) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * حجم (ManifestLookup), "");

  بیعلامت j = 0;
  بیعلامت صحيح شمار = manifest.len;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    fallback_plan->ماسک_array[j] = plan->map.get_1_ماسک (manifest[i].برچسب_2);
    إذا (fallback_plan->ماسک_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookup_جابه_جایی));
      إذا (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->رقم_lookups = j;
  fallback_plan->free_lookups = خطأ;

  أرجع j > 0;
#وإلا
  أرجع خطأ;
#پایان_اگر
}

ساكن منطقي
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   ثابت hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_درازی_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  بیعلامت صحيح j = 0;
  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی(arabic_fallback_features) ; i++)
  {
    fallback_plan->ماسک_array[j] = plan->map.get_1_ماسک (arabic_fallback_features[i]);
    إذا (fallback_plan->ماسک_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      إذا (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->رقم_lookups = j;
  fallback_plan->free_lookups = درست;

  أرجع j > 0;
}

ساكن arabic_fallback_plan_t *
arabic_fallback_plan_create (ثابت hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, حجم (arabic_fallback_plan_t));
  إذا (unlikely (!fallback_plan))
    أرجع const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->رقم_lookups = 0;
  fallback_plan->free_lookups = خطأ;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  إذا (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    أرجع fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  إذا (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    أرجع fallback_plan;

  أكد (fallback_plan->رقم_lookups == 0);
  hb_free (fallback_plan);
  أرجع const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

ساكن فراغ
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  إذا (!fallback_plan || fallback_plan->رقم_lookups == 0)
    أرجع;

  لكل (بیعلامت صحيح i = 0; i < fallback_plan->رقم_lookups; i++)
    إذا (fallback_plan->lookup_array[i])
    {
      إذا (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      إذا (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

ساكن فراغ
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_میانگیر_t *میانگیر)
{
  OT::hb_ot_apply_context_t c (0, font, میانگیر, hb_blob_get_خالی ());
  لكل (بیعلامت صحيح i = 0; i < fallback_plan->رقم_lookups; i++)
    إذا (fallback_plan->lookup_array[i]) {
      c.مجموعه_lookup_ماسک (fallback_plan->ماسک_array[i]);
      إذا (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#پایان_اگر /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

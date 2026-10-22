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

#جے_بنا_تعریف HB_OT_SHAPER_ARABIC_FALLBACK_HH
#تعریف_کرو HB_OT_SHAPER_ARABIC_FALLBACK_HH

#ضمن "hb.hh"

#ضمن "hb-ot-shape.hh"
#ضمن "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
ساكن ثابت hb_ٹیگ_t arabic_fallback_features[] =
{
  HB_ٹیگ('i','n','i','t'),
  HB_ٹیگ('m','e','d','i'),
  HB_ٹیگ('f','i','n','a'),
  HB_ٹیگ('i','s','o','l'),
  HB_ٹیگ('r','l','i','g'),
  HB_ٹیگ('r','l','i','g'),
  HB_ٹیگ('r','l','i','g'),
};

ساكن OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (ثابت hb_ot_shape_plan_t *plan HB_بن_ورتیا,
					  hb_font_t *font,
					  بنانشانی صحيح feature_اشاریہ)
{
  OT::HBGlyphID16 glyphs[SHAPING_جدول_آخری - SHAPING_جدول_پہلا + 1];
  OT::HBGlyphID16 substitutes[SHAPING_جدول_آخری - SHAPING_جدول_پہلا + 1];
  بنانشانی صحيح عدد_glyphs = 0;

  /* Populate arrays */
  لكل (hb_codepoint_t u = SHAPING_جدول_پہلا; u < SHAPING_جدول_آخری + 1; u++)
  {
    hb_codepoint_t s = shaping_جدول[u - SHAPING_جدول_پہلا][feature_اشاریہ];
    hb_codepoint_t u_glyph, s_glyph;

    إذا (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      استمر;

    glyphs[عدد_glyphs] = u_glyph;
    substitutes[عدد_glyphs] = s_glyph;

    عدد_glyphs++;
  }

  إذا (!عدد_glyphs)
    أرجع خالی_سوچک;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_ترتیب_دیو (&glyphs[0], عدد_glyphs,
		  (صحيح(*)(ثابت OT::HBUINT16*, ثابت OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  حرف buf[(SHAPING_جدول_آخری - SHAPING_جدول_پہلا + 1) * 4 + 128];
  hb_serialize_context_t c (buf, حجم (buf));
  OT::SubstLookup *lookup = c.شروع_serialize<OT::SubstLookup> ();
  منطقي ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, عدد_glyphs),
				       hb_array (substitutes, عدد_glyphs));
  c.سرا_serialize ();

  أرجع ret && !c.وچ_error () ? c.copy<OT::SubstLookup> () : خالی_سوچک;
}

سانچا <قسم_ناں T>
ساكن OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (ثابت hb_ot_shape_plan_t *plan HB_بن_ورتیا,
					    hb_font_t *font,
					    ثابت T &ligature_جدول,
					    بنانشانی lookup_flags)
{
  OT::HBGlyphID16 پہلا_glyphs[ARRAY_لمائی_CONST (ligature_جدول)];
  بنانشانی صحيح پہلا_glyphs_indirection[ARRAY_لمائی_CONST (ligature_جدول)];
  بنانشانی صحيح ligature_فی_پہلا_glyph_گنتی_list[ARRAY_لمائی_CONST (پہلا_glyphs)];
  بنانشانی صحيح عدد_پہلا_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_لمائی_CONST (پہلا_glyphs) * ARRAY_لمائی_CONST(ligature_جدول[0].ligatures)];
  بنانشانی صحيح component_گنتی_list[ARRAY_لمائی_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_لمائی_CONST (ligature_list) *
				 ARRAY_لمائی_CONST (ligature_جدول[0].ligatures[0].components)];
  بنانشانی صحيح عدد_ligatures = 0;
  بنانشانی صحيح عدد_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  لكل (بنانشانی صحيح پہلا_glyph_idx = 0; پہلا_glyph_idx < ARRAY_لمائی (پہلا_glyphs); پہلا_glyph_idx++)
  {
    hb_codepoint_t پہلا_u = ligature_جدول[پہلا_glyph_idx].پہلا;
    hb_codepoint_t پہلا_glyph;
    إذا (!hb_font_get_glyph (font, پہلا_u, 0, &پہلا_glyph))
      استمر;
    پہلا_glyphs[عدد_پہلا_glyphs] = پہلا_glyph;
    ligature_فی_پہلا_glyph_گنتی_list[عدد_پہلا_glyphs] = 0;
    پہلا_glyphs_indirection[عدد_پہلا_glyphs] = پہلا_glyph_idx;
    عدد_پہلا_glyphs++;
  }
  hb_stable_ترتیب_دیو (&پہلا_glyphs[0], عدد_پہلا_glyphs,
		  (صحيح(*)(ثابت OT::HBUINT16*, ثابت OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &پہلا_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  لكل (بنانشانی صحيح i = 0; i < عدد_پہلا_glyphs; i++)
  {
    بنانشانی صحيح پہلا_glyph_idx = پہلا_glyphs_indirection[i];

    لكل (بنانشانی صحيح ligature_idx = 0; ligature_idx < ARRAY_لمائی (ligature_جدول[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_جدول[پہلا_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      إذا (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	استمر;

      ثابت خودکار &components = ligature_جدول[پہلا_glyph_idx].ligatures[ligature_idx].components;
      بنانشانی component_گنتی = ARRAY_لمائی_CONST (components);

      منطقي matched = سچ;
      لكل (بنانشانی j = 0; j < component_گنتی; j++)
      {
	hb_codepoint_t component_u   = ligature_جدول[پہلا_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	إذا (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = خطأ;
	  اكسر;
	}

	component_list[عدد_components++] = component_glyph;
      }
      إذا (!matched)
        استمر;

      component_گنتی_list[عدد_ligatures] = 1 + component_گنتی;
      ligature_list[عدد_ligatures] = ligature_glyph;

      ligature_فی_پہلا_glyph_گنتی_list[i]++;

      عدد_ligatures++;
    }
  }

  إذا (!عدد_ligatures)
    أرجع خالی_سوچک;


  /* 16 bytes per ligature ought to be enough... */
  حرف buf[ARRAY_لمائی_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, حجم (buf));
  OT::SubstLookup *lookup = c.شروع_serialize<OT::SubstLookup> ();
  منطقي ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (پہلا_glyphs, عدد_پہلا_glyphs),
					 hb_array (ligature_فی_پہلا_glyph_گنتی_list, عدد_پہلا_glyphs),
					 hb_array (ligature_list, عدد_ligatures),
					 hb_array (component_گنتی_list, عدد_ligatures),
					 hb_array (component_list, عدد_components));
  c.سرا_serialize ();

  أرجع ret && !c.وچ_error () ? c.copy<OT::SubstLookup> () : خالی_سوچک;
}

ساكن OT::SubstLookup *
arabic_fallback_synthesize_lookup (ثابت hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   بنانشانی صحيح feature_اشاریہ)
{
  إذا (feature_اشاریہ < 4)
    أرجع arabic_fallback_synthesize_lookup_single (plan, font, feature_اشاریہ);
  وإلا
  {
    بدل (feature_اشاریہ) {
      حالة 4: أرجع arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_جدول, OT::LookupFlag::IgnoreMarks);
      حالة 5: أرجع arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_جدول, OT::LookupFlag::IgnoreMarks);
      حالة 6: أرجع arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_جدول, 0);
    }
  }
  أكد (خطأ);
  أرجع خالی_سوچک;
}

#تعریف_کرو ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_لمائی_CONST (arabic_fallback_features)

بنية arabic_fallback_plan_t
{
  بنانشانی صحيح عدد_lookups;
  منطقي free_lookups;

  hb_نقاب_t نقاب_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#جے_بنا_تعریف HB_NO_WIN1256
#ضمن "hb-ot-shaper-arabic-win1256.hh"
#جے_انت

بنية ManifestLookup
{
  عام:
  OT::ٹیگ ٹیگ_2;
  OT::Offset16_منزل<OT::SubstLookup> lookup_وتھ;
  عام:
  DEFINE_ناپ_STATIC (6);
};
تعريفنوع OT::Array16_دا<ManifestLookup> Manifest;

ساكن منطقي
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_بن_ورتیا,
				   ثابت hb_ot_shape_plan_t *plan HB_بن_ورتیا,
				   hb_font_t *font HB_بن_ورتیا)
{
#جے_بنا_تعریف HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  إذا (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    أرجع خطأ;

  ثابت Manifest &manifest = reinterpret_cast<ثابت Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (حجم (arabic_win1256_gsub_lookups.manifest_انکڑے) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * حجم (ManifestLookup), "");

  بنانشانی j = 0;
  بنانشانی صحيح گنتی = manifest.len;
  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
  {
    fallback_plan->نقاب_array[j] = plan->map.get_1_نقاب (manifest[i].ٹیگ_2);
    إذا (fallback_plan->نقاب_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookup_وتھ));
      إذا (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->عدد_lookups = j;
  fallback_plan->free_lookups = خطأ;

  أرجع j > 0;
#وإلا
  أرجع خطأ;
#جے_انت
}

ساكن منطقي
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   ثابت hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_لمائی_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  بنانشانی صحيح j = 0;
  لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی(arabic_fallback_features) ; i++)
  {
    fallback_plan->نقاب_array[j] = plan->map.get_1_نقاب (arabic_fallback_features[i]);
    إذا (fallback_plan->نقاب_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      إذا (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->عدد_lookups = j;
  fallback_plan->free_lookups = سچ;

  أرجع j > 0;
}

ساكن arabic_fallback_plan_t *
arabic_fallback_plan_create (ثابت hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, حجم (arabic_fallback_plan_t));
  إذا (unlikely (!fallback_plan))
    أرجع const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->عدد_lookups = 0;
  fallback_plan->free_lookups = خطأ;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  إذا (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    أرجع fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  إذا (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    أرجع fallback_plan;

  أكد (fallback_plan->عدد_lookups == 0);
  hb_free (fallback_plan);
  أرجع const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

ساكن فراغ
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  إذا (!fallback_plan || fallback_plan->عدد_lookups == 0)
    أرجع;

  لكل (بنانشانی صحيح i = 0; i < fallback_plan->عدد_lookups; i++)
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
			    hb_بفر_t *بفر)
{
  OT::hb_ot_apply_context_t c (0, font, بفر, hb_blob_get_خالی_تھاواں ());
  لكل (بنانشانی صحيح i = 0; i < fallback_plan->عدد_lookups; i++)
    إذا (fallback_plan->lookup_array[i]) {
      c.سیٹ_lookup_نقاب (fallback_plan->نقاب_array[i]);
      إذا (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#جے_انت /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

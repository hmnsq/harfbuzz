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

#నిర్వచితం_కాకపోతే HB_OT_SHAPER_ARABIC_FALLBACK_HH
#నిర్వచించు HB_OT_SHAPER_ARABIC_FALLBACK_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-ot-shape.hh"
#అన్తర్భావ "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
స్థిర స్థిరరాశి hb_ట్యాగ్_t arabic_fallback_features[] =
{
  HB_ట్యాగ్('i','n','i','t'),
  HB_ట్యాగ్('m','e','d','i'),
  HB_ట్యాగ్('f','i','n','a'),
  HB_ట్యాగ్('i','s','o','l'),
  HB_ట్యాగ్('r','l','i','g'),
  HB_ట్యాగ్('r','l','i','g'),
  HB_ట్యాగ్('r','l','i','g'),
};

స్థిర OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
					  hb_font_t *font,
					  గుర్తులేని పూర్ణాంకం feature_సూచీ)
{
  OT::HBGlyphID16 glyphs[SHAPING_పట్టిక_చివరి - SHAPING_పట్టిక_మొదటి + 1];
  OT::HBGlyphID16 substitutes[SHAPING_పట్టిక_చివరి - SHAPING_పట్టిక_మొదటి + 1];
  గుర్తులేని పూర్ణాంకం అంకె_glyphs = 0;

  /* Populate arrays */
  కొరకు (hb_codepoint_t u = SHAPING_పట్టిక_మొదటి; u < SHAPING_పట్టిక_చివరి + 1; u++)
  {
    hb_codepoint_t s = shaping_పట్టిక[u - SHAPING_పట్టిక_మొదటి][feature_సూచీ];
    hb_codepoint_t u_glyph, s_glyph;

    ఒకవేళ (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      కొనసాగు;

    glyphs[అంకె_glyphs] = u_glyph;
    substitutes[అంకె_glyphs] = s_glyph;

    అంకె_glyphs++;
  }

  ఒకవేళ (!అంకె_glyphs)
    తిరిగి శూన్యసూచిక;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_క్రమీకరించు (&glyphs[0], అంకె_glyphs,
		  (పూర్ణాంకం(*)(స్థిరరాశి OT::HBUINT16*, స్థిరరాశి OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  అక్షరం buf[(SHAPING_పట్టిక_చివరి - SHAPING_పట్టిక_మొదటి + 1) * 4 + 128];
  hb_serialize_context_t c (buf, పరిమాణం (buf));
  OT::SubstLookup *lookup = c.ప్రారంభం_serialize<OT::SubstLookup> ();
  తార్కిక ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, అంకె_glyphs),
				       hb_array (substitutes, అంకె_glyphs));
  c.చివర_serialize ();

  తిరిగి ret && !c.లోపల_error () ? c.copy<OT::SubstLookup> () : శూన్యసూచిక;
}

మూస <రకం_పేరు T>
స్థిర OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
					    hb_font_t *font,
					    స్థిరరాశి T &ligature_పట్టిక,
					    గుర్తులేని lookup_flags)
{
  OT::HBGlyphID16 మొదటి_glyphs[ARRAY_నిడివి_CONST (ligature_పట్టిక)];
  గుర్తులేని పూర్ణాంకం మొదటి_glyphs_indirection[ARRAY_నిడివి_CONST (ligature_పట్టిక)];
  గుర్తులేని పూర్ణాంకం ligature_చొప్పున_మొదటి_glyph_లెక్క_list[ARRAY_నిడివి_CONST (మొదటి_glyphs)];
  గుర్తులేని పూర్ణాంకం అంకె_మొదటి_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_నిడివి_CONST (మొదటి_glyphs) * ARRAY_నిడివి_CONST(ligature_పట్టిక[0].ligatures)];
  గుర్తులేని పూర్ణాంకం component_లెక్క_list[ARRAY_నిడివి_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_నిడివి_CONST (ligature_list) *
				 ARRAY_నిడివి_CONST (ligature_పట్టిక[0].ligatures[0].components)];
  గుర్తులేని పూర్ణాంకం అంకె_ligatures = 0;
  గుర్తులేని పూర్ణాంకం అంకె_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  కొరకు (గుర్తులేని పూర్ణాంకం మొదటి_glyph_idx = 0; మొదటి_glyph_idx < ARRAY_నిడివి (మొదటి_glyphs); మొదటి_glyph_idx++)
  {
    hb_codepoint_t మొదటి_u = ligature_పట్టిక[మొదటి_glyph_idx].మొదటి;
    hb_codepoint_t మొదటి_glyph;
    ఒకవేళ (!hb_font_get_glyph (font, మొదటి_u, 0, &మొదటి_glyph))
      కొనసాగు;
    మొదటి_glyphs[అంకె_మొదటి_glyphs] = మొదటి_glyph;
    ligature_చొప్పున_మొదటి_glyph_లెక్క_list[అంకె_మొదటి_glyphs] = 0;
    మొదటి_glyphs_indirection[అంకె_మొదటి_glyphs] = మొదటి_glyph_idx;
    అంకె_మొదటి_glyphs++;
  }
  hb_stable_క్రమీకరించు (&మొదటి_glyphs[0], అంకె_మొదటి_glyphs,
		  (పూర్ణాంకం(*)(స్థిరరాశి OT::HBUINT16*, స్థిరరాశి OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &మొదటి_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_మొదటి_glyphs; i++)
  {
    గుర్తులేని పూర్ణాంకం మొదటి_glyph_idx = మొదటి_glyphs_indirection[i];

    కొరకు (గుర్తులేని పూర్ణాంకం ligature_idx = 0; ligature_idx < ARRAY_నిడివి (ligature_పట్టిక[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_పట్టిక[మొదటి_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      ఒకవేళ (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	కొనసాగు;

      స్థిరరాశి స్వయంచాలక &components = ligature_పట్టిక[మొదటి_glyph_idx].ligatures[ligature_idx].components;
      గుర్తులేని component_లెక్క = ARRAY_నిడివి_CONST (components);

      తార్కిక matched = నిజం;
      కొరకు (గుర్తులేని j = 0; j < component_లెక్క; j++)
      {
	hb_codepoint_t component_u   = ligature_పట్టిక[మొదటి_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	ఒకవేళ (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = అబద్ధం;
	  ఆపు;
	}

	component_list[అంకె_components++] = component_glyph;
      }
      ఒకవేళ (!matched)
        కొనసాగు;

      component_లెక్క_list[అంకె_ligatures] = 1 + component_లెక్క;
      ligature_list[అంకె_ligatures] = ligature_glyph;

      ligature_చొప్పున_మొదటి_glyph_లెక్క_list[i]++;

      అంకె_ligatures++;
    }
  }

  ఒకవేళ (!అంకె_ligatures)
    తిరిగి శూన్యసూచిక;


  /* 16 bytes per ligature ought to be enough... */
  అక్షరం buf[ARRAY_నిడివి_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, పరిమాణం (buf));
  OT::SubstLookup *lookup = c.ప్రారంభం_serialize<OT::SubstLookup> ();
  తార్కిక ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (మొదటి_glyphs, అంకె_మొదటి_glyphs),
					 hb_array (ligature_చొప్పున_మొదటి_glyph_లెక్క_list, అంకె_మొదటి_glyphs),
					 hb_array (ligature_list, అంకె_ligatures),
					 hb_array (component_లెక్క_list, అంకె_ligatures),
					 hb_array (component_list, అంకె_components));
  c.చివర_serialize ();

  తిరిగి ret && !c.లోపల_error () ? c.copy<OT::SubstLookup> () : శూన్యసూచిక;
}

స్థిర OT::SubstLookup *
arabic_fallback_synthesize_lookup (స్థిరరాశి hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   గుర్తులేని పూర్ణాంకం feature_సూచీ)
{
  ఒకవేళ (feature_సూచీ < 4)
    తిరిగి arabic_fallback_synthesize_lookup_single (plan, font, feature_సూచీ);
  లేకపోతే
  {
    ఎంపిక (feature_సూచీ) {
      స్థితి 4: తిరిగి arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_పట్టిక, OT::LookupFlag::IgnoreMarks);
      స్థితి 5: తిరిగి arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_పట్టిక, OT::LookupFlag::IgnoreMarks);
      స్థితి 6: తిరిగి arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_పట్టిక, 0);
    }
  }
  ప్రతిజ్ఞా (అబద్ధం);
  తిరిగి శూన్యసూచిక;
}

#నిర్వచించు ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_నిడివి_CONST (arabic_fallback_features)

నిర్మాణం arabic_fallback_plan_t
{
  గుర్తులేని పూర్ణాంకం అంకె_lookups;
  తార్కిక free_lookups;

  hb_ముసుగు_t ముసుగు_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#నిర్వచితం_కాకపోతే HB_NO_WIN1256
#అన్తర్భావ "hb-ot-shaper-arabic-win1256.hh"
#అయితే_ముగింపు

నిర్మాణం ManifestLookup
{
  బహిరంగ:
  OT::ట్యాగ్ ట్యాగ్_2;
  OT::Offset16_గమ్యం<OT::SubstLookup> lookup_స్థానభ్రంశం;
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};
రకనిర్వచనం OT::Array16_యొక్క<ManifestLookup> Manifest;

స్థిర తార్కిక
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_వాడని,
				   స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
				   hb_font_t *font HB_వాడని)
{
#నిర్వచితం_కాకపోతే HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  ఒకవేళ (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    తిరిగి అబద్ధం;

  స్థిరరాశి Manifest &manifest = reinterpret_cast<స్థిరరాశి Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (పరిమాణం (arabic_win1256_gsub_lookups.manifest_దత్తాంశం) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * పరిమాణం (ManifestLookup), "");

  గుర్తులేని j = 0;
  గుర్తులేని పూర్ణాంకం లెక్క = manifest.len;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    fallback_plan->ముసుగు_array[j] = plan->map.get_1_ముసుగు (manifest[i].ట్యాగ్_2);
    ఒకవేళ (fallback_plan->ముసుగు_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookup_స్థానభ్రంశం));
      ఒకవేళ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->అంకె_lookups = j;
  fallback_plan->free_lookups = అబద్ధం;

  తిరిగి j > 0;
#లేకపోతే
  తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

స్థిర తార్కిక
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   స్థిరరాశి hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_నిడివి_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  గుర్తులేని పూర్ణాంకం j = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి(arabic_fallback_features) ; i++)
  {
    fallback_plan->ముసుగు_array[j] = plan->map.get_1_ముసుగు (arabic_fallback_features[i]);
    ఒకవేళ (fallback_plan->ముసుగు_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      ఒకవేళ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->అంకె_lookups = j;
  fallback_plan->free_lookups = నిజం;

  తిరిగి j > 0;
}

స్థిర arabic_fallback_plan_t *
arabic_fallback_plan_create (స్థిరరాశి hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, పరిమాణం (arabic_fallback_plan_t));
  ఒకవేళ (unlikely (!fallback_plan))
    తిరిగి const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->అంకె_lookups = 0;
  fallback_plan->free_lookups = అబద్ధం;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  ఒకవేళ (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    తిరిగి fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  ఒకవేళ (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    తిరిగి fallback_plan;

  ప్రతిజ్ఞా (fallback_plan->అంకె_lookups == 0);
  hb_free (fallback_plan);
  తిరిగి const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

స్థిర శూన్య
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  ఒకవేళ (!fallback_plan || fallback_plan->అంకె_lookups == 0)
    తిరిగి;

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < fallback_plan->అంకె_lookups; i++)
    ఒకవేళ (fallback_plan->lookup_array[i])
    {
      ఒకవేళ (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      ఒకవేళ (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

స్థిర శూన్య
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_బఫరు_t *బఫరు)
{
  OT::hb_ot_apply_context_t c (0, font, బఫరు, hb_blob_get_ఖాళీలు ());
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < fallback_plan->అంకె_lookups; i++)
    ఒకవేళ (fallback_plan->lookup_array[i]) {
      c.సమితి_lookup_ముసుగు (fallback_plan->ముసుగు_array[i]);
      ఒకవేళ (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#అయితే_ముగింపు /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

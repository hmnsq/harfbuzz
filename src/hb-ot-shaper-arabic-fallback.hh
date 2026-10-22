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

#ifndef HB_OT_SHAPER_ARABIC_FALLBACK_HH
#define HB_OT_SHAPER_ARABIC_FALLBACK_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-shape.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
ସ୍ଥିର ଅଚଳ hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

ସ୍ଥିର OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature_index)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = 0;

  /* Populate arrays */
  ପାଇଁ (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_index];
    hb_codepoint_t u_glyph, s_glyph;

    ଯଦି (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      ଜାରି;

    glyphs[ଅଙ୍କ_glyphs] = u_glyph;
    substitutes[ଅଙ୍କ_glyphs] = s_glyph;

    ଅଙ୍କ_glyphs++;
  }

  ଯଦି (!ଅଙ୍କ_glyphs)
    ଫେରନ୍ତୁ nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], ଅଙ୍କ_glyphs,
		  (ପୂର୍ଣ୍ଣସଂଖ୍ୟା(*)(ଅଚଳ OT::HBUINT16*, ଅଚଳ OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  ଅକ୍ଷର buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, ଆକାର (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  ତାର୍କିକ ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, ଅଙ୍କ_glyphs),
				       hb_array (substitutes, ଅଙ୍କ_glyphs));
  c.end_serialize ();

  ଫେରନ୍ତୁ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

template <typename T>
ସ୍ଥିର OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    ଅଚଳ T &ligature_table,
					    unsigned lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_glyphs_indirection[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ligature_per_first_glyph_ଗଣନା_list[ARRAY_LENGTH_CONST (first_glyphs)];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_LENGTH_CONST (first_glyphs) * ARRAY_LENGTH_CONST(ligature_table[0].ligatures)];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା component_ଗଣନା_list[ARRAY_LENGTH_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_LENGTH_CONST (ligature_list) *
				 ARRAY_LENGTH_CONST (ligature_table[0].ligatures[0].components)];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ligatures = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_glyph_idx = 0; first_glyph_idx < ARRAY_LENGTH (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    ଯଦି (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      ଜାରି;
    first_glyphs[ଅଙ୍କ_first_glyphs] = first_glyph;
    ligature_per_first_glyph_ଗଣନା_list[ଅଙ୍କ_first_glyphs] = 0;
    first_glyphs_indirection[ଅଙ୍କ_first_glyphs] = first_glyph_idx;
    ଅଙ୍କ_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], ଅଙ୍କ_first_glyphs,
		  (ପୂର୍ଣ୍ଣସଂଖ୍ୟା(*)(ଅଚଳ OT::HBUINT16*, ଅଚଳ OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_first_glyphs; i++)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_glyph_idx = first_glyphs_indirection[i];

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ligature_idx = 0; ligature_idx < ARRAY_LENGTH (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      ଯଦି (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	ଜାରି;

      ଅଚଳ auto &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      unsigned component_ଗଣନା = ARRAY_LENGTH_CONST (components);

      ତାର୍କିକ matched = ସତ୍ୟ;
      ପାଇଁ (unsigned j = 0; j < component_ଗଣନା; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	ଯଦି (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = ମିଥ୍ୟା;
	  ଭାଙ୍ଗ;
	}

	component_list[ଅଙ୍କ_components++] = component_glyph;
      }
      ଯଦି (!matched)
        ଜାରି;

      component_ଗଣନା_list[ଅଙ୍କ_ligatures] = 1 + component_ଗଣନା;
      ligature_list[ଅଙ୍କ_ligatures] = ligature_glyph;

      ligature_per_first_glyph_ଗଣନା_list[i]++;

      ଅଙ୍କ_ligatures++;
    }
  }

  ଯଦି (!ଅଙ୍କ_ligatures)
    ଫେରନ୍ତୁ nullptr;


  /* 16 bytes per ligature ought to be enough... */
  ଅକ୍ଷର buf[ARRAY_LENGTH_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, ଆକାର (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  ତାର୍କିକ ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, ଅଙ୍କ_first_glyphs),
					 hb_array (ligature_per_first_glyph_ଗଣନା_list, ଅଙ୍କ_first_glyphs),
					 hb_array (ligature_list, ଅଙ୍କ_ligatures),
					 hb_array (component_ଗଣନା_list, ଅଙ୍କ_ligatures),
					 hb_array (component_list, ଅଙ୍କ_components));
  c.end_serialize ();

  ଫେରନ୍ତୁ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

ସ୍ଥିର OT::SubstLookup *
arabic_fallback_synthesize_lookup (ଅଚଳ hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature_index)
{
  ଯଦି (feature_index < 4)
    ଫେରନ୍ତୁ arabic_fallback_synthesize_lookup_single (plan, font, feature_index);
  ଅନ୍ୟଥା
  {
    ବାଛ (feature_index) {
      ସ୍ଥିତି 4: ଫେରନ୍ତୁ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      ସ୍ଥିତି 5: ଫେରନ୍ତୁ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      ସ୍ଥିତି 6: ଫେରନ୍ତୁ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  ପ୍ରତିଜ୍ଞା (ମିଥ୍ୟା);
  ଫେରନ୍ତୁ nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_LENGTH_CONST (arabic_fallback_features)

ସଂରଚନା arabic_fallback_plan_t
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_lookups;
  ତାର୍କିକ free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ifndef HB_NO_WIN1256
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-arabic-win1256.hh"
#endif

ସଂରଚନା ManifestLookup
{
  ସର୍ବସାଧାରଣ:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା OT::Array16Of<ManifestLookup> Manifest;

ସ୍ଥିର ତାର୍କିକ
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   ଅଚଳ hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#ifndef HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  ଯଦି (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଅଚଳ Manifest &manifest = reinterpret_cast<ଅଚଳ Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (ଆକାର (arabic_win1256_gsub_lookups.manifestData) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * ଆକାର (ManifestLookup), "");

  unsigned j = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = manifest.len;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    ଯଦି (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      ଯଦି (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->ଅଙ୍କ_lookups = j;
  fallback_plan->free_lookups = ମିଥ୍ୟା;

  ଫେରନ୍ତୁ j > 0;
#ଅନ୍ୟଥା
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

ସ୍ଥିର ତାର୍କିକ
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   ଅଚଳ hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_LENGTH_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    ଯଦି (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      ଯଦି (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->ଅଙ୍କ_lookups = j;
  fallback_plan->free_lookups = ସତ୍ୟ;

  ଫେରନ୍ତୁ j > 0;
}

ସ୍ଥିର arabic_fallback_plan_t *
arabic_fallback_plan_create (ଅଚଳ hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, ଆକାର (arabic_fallback_plan_t));
  ଯଦି (unlikely (!fallback_plan))
    ଫେରନ୍ତୁ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->ଅଙ୍କ_lookups = 0;
  fallback_plan->free_lookups = ମିଥ୍ୟା;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  ଯଦି (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    ଫେରନ୍ତୁ fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  ଯଦି (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    ଫେରନ୍ତୁ fallback_plan;

  ପ୍ରତିଜ୍ଞା (fallback_plan->ଅଙ୍କ_lookups == 0);
  hb_free (fallback_plan);
  ଫେରନ୍ତୁ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

ସ୍ଥିର ଶୂନ୍ୟ
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  ଯଦି (!fallback_plan || fallback_plan->ଅଙ୍କ_lookups == 0)
    ଫେରନ୍ତୁ;

  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < fallback_plan->ଅଙ୍କ_lookups; i++)
    ଯଦି (fallback_plan->lookup_array[i])
    {
      ଯଦି (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      ଯଦି (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

ସ୍ଥିର ଶୂନ୍ୟ
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
  OT::hb_ot_apply_context_t c (0, font, buffer, hb_blob_get_empty ());
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < fallback_plan->ଅଙ୍କ_lookups; i++)
    ଯଦି (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      ଯଦି (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

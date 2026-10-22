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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_SHAPER_ARABIC_FALLBACK_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPER_ARABIC_FALLBACK_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
ਸਥਿਰ ਅਚਲ hb_ਟੈਗ_t arabic_fallback_features[] =
{
  HB_ਟੈਗ('i','n','i','t'),
  HB_ਟੈਗ('m','e','d','i'),
  HB_ਟੈਗ('f','i','n','a'),
  HB_ਟੈਗ('i','s','o','l'),
  HB_ਟੈਗ('r','l','i','g'),
  HB_ਟੈਗ('r','l','i','g'),
  HB_ਟੈਗ('r','l','i','g'),
};

ਸਥਿਰ OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
					  hb_font_t *font,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ)
{
  OT::HBGlyphID16 glyphs[SHAPING_ਸਾਰਣੀ_ਆਖਰੀ - SHAPING_ਸਾਰਣੀ_ਪਹਿਲਾ + 1];
  OT::HBGlyphID16 substitutes[SHAPING_ਸਾਰਣੀ_ਆਖਰੀ - SHAPING_ਸਾਰਣੀ_ਪਹਿਲਾ + 1];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = 0;

  /* Populate arrays */
  ਲਈ (hb_codepoint_t u = SHAPING_ਸਾਰਣੀ_ਪਹਿਲਾ; u < SHAPING_ਸਾਰਣੀ_ਆਖਰੀ + 1; u++)
  {
    hb_codepoint_t s = shaping_ਸਾਰਣੀ[u - SHAPING_ਸਾਰਣੀ_ਪਹਿਲਾ][feature_ਸੂਚਕਾਂਕ];
    hb_codepoint_t u_glyph, s_glyph;

    ਜੇ (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      ਜਾਰੀ;

    glyphs[ਅੰਕ_glyphs] = u_glyph;
    substitutes[ਅੰਕ_glyphs] = s_glyph;

    ਅੰਕ_glyphs++;
  }

  ਜੇ (!ਅੰਕ_glyphs)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_ਛਾਂਟੋ (&glyphs[0], ਅੰਕ_glyphs,
		  (ਪੂਰਨ(*)(ਅਚਲ OT::HBUINT16*, ਅਚਲ OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  ਅੱਖਰ buf[(SHAPING_ਸਾਰਣੀ_ਆਖਰੀ - SHAPING_ਸਾਰਣੀ_ਪਹਿਲਾ + 1) * 4 + 128];
  hb_serialize_context_t c (buf, ਆਕਾਰ (buf));
  OT::SubstLookup *lookup = c.ਸ਼ੁਰੂ_serialize<OT::SubstLookup> ();
  ਤਾਰਕਿਕ ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, ਅੰਕ_glyphs),
				       hb_array (substitutes, ਅੰਕ_glyphs));
  c.ਸਿਰਾ_serialize ();

  ਵਾਪਸ ret && !c.ਵਿੱਚ_error () ? c.copy<OT::SubstLookup> () : ਖਾਲੀ_ਸੂਚਕ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
					    hb_font_t *font,
					    ਅਚਲ T &ligature_ਸਾਰਣੀ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_flags)
{
  OT::HBGlyphID16 ਪਹਿਲਾ_glyphs[ARRAY_ਲੰਮਾਈ_CONST (ligature_ਸਾਰਣੀ)];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ_glyphs_indirection[ARRAY_ਲੰਮਾਈ_CONST (ligature_ਸਾਰਣੀ)];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ligature_ਪ੍ਰਤੀ_ਪਹਿਲਾ_glyph_ਗਿਣਤੀ_list[ARRAY_ਲੰਮਾਈ_CONST (ਪਹਿਲਾ_glyphs)];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਪਹਿਲਾ_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_ਲੰਮਾਈ_CONST (ਪਹਿਲਾ_glyphs) * ARRAY_ਲੰਮਾਈ_CONST(ligature_ਸਾਰਣੀ[0].ligatures)];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ component_ਗਿਣਤੀ_list[ARRAY_ਲੰਮਾਈ_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_ਲੰਮਾਈ_CONST (ligature_list) *
				 ARRAY_ਲੰਮਾਈ_CONST (ligature_ਸਾਰਣੀ[0].ligatures[0].components)];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ligatures = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ_glyph_idx = 0; ਪਹਿਲਾ_glyph_idx < ARRAY_ਲੰਮਾਈ (ਪਹਿਲਾ_glyphs); ਪਹਿਲਾ_glyph_idx++)
  {
    hb_codepoint_t ਪਹਿਲਾ_u = ligature_ਸਾਰਣੀ[ਪਹਿਲਾ_glyph_idx].ਪਹਿਲਾ;
    hb_codepoint_t ਪਹਿਲਾ_glyph;
    ਜੇ (!hb_font_get_glyph (font, ਪਹਿਲਾ_u, 0, &ਪਹਿਲਾ_glyph))
      ਜਾਰੀ;
    ਪਹਿਲਾ_glyphs[ਅੰਕ_ਪਹਿਲਾ_glyphs] = ਪਹਿਲਾ_glyph;
    ligature_ਪ੍ਰਤੀ_ਪਹਿਲਾ_glyph_ਗਿਣਤੀ_list[ਅੰਕ_ਪਹਿਲਾ_glyphs] = 0;
    ਪਹਿਲਾ_glyphs_indirection[ਅੰਕ_ਪਹਿਲਾ_glyphs] = ਪਹਿਲਾ_glyph_idx;
    ਅੰਕ_ਪਹਿਲਾ_glyphs++;
  }
  hb_stable_ਛਾਂਟੋ (&ਪਹਿਲਾ_glyphs[0], ਅੰਕ_ਪਹਿਲਾ_glyphs,
		  (ਪੂਰਨ(*)(ਅਚਲ OT::HBUINT16*, ਅਚਲ OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &ਪਹਿਲਾ_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_ਪਹਿਲਾ_glyphs; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ_glyph_idx = ਪਹਿਲਾ_glyphs_indirection[i];

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ligature_idx = 0; ligature_idx < ARRAY_ਲੰਮਾਈ (ligature_ਸਾਰਣੀ[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_ਸਾਰਣੀ[ਪਹਿਲਾ_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      ਜੇ (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	ਜਾਰੀ;

      ਅਚਲ ਸਵੈਚਾਲਿਤ &components = ligature_ਸਾਰਣੀ[ਪਹਿਲਾ_glyph_idx].ligatures[ligature_idx].components;
      ਬਿਨਾਨਿਸ਼ਾਨੀ component_ਗਿਣਤੀ = ARRAY_ਲੰਮਾਈ_CONST (components);

      ਤਾਰਕਿਕ matched = ਸੱਚ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < component_ਗਿਣਤੀ; j++)
      {
	hb_codepoint_t component_u   = ligature_ਸਾਰਣੀ[ਪਹਿਲਾ_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	ਜੇ (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = ਝੂਠ;
	  ਰੋਕੋ;
	}

	component_list[ਅੰਕ_components++] = component_glyph;
      }
      ਜੇ (!matched)
        ਜਾਰੀ;

      component_ਗਿਣਤੀ_list[ਅੰਕ_ligatures] = 1 + component_ਗਿਣਤੀ;
      ligature_list[ਅੰਕ_ligatures] = ligature_glyph;

      ligature_ਪ੍ਰਤੀ_ਪਹਿਲਾ_glyph_ਗਿਣਤੀ_list[i]++;

      ਅੰਕ_ligatures++;
    }
  }

  ਜੇ (!ਅੰਕ_ligatures)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;


  /* 16 bytes per ligature ought to be enough... */
  ਅੱਖਰ buf[ARRAY_ਲੰਮਾਈ_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, ਆਕਾਰ (buf));
  OT::SubstLookup *lookup = c.ਸ਼ੁਰੂ_serialize<OT::SubstLookup> ();
  ਤਾਰਕਿਕ ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (ਪਹਿਲਾ_glyphs, ਅੰਕ_ਪਹਿਲਾ_glyphs),
					 hb_array (ligature_ਪ੍ਰਤੀ_ਪਹਿਲਾ_glyph_ਗਿਣਤੀ_list, ਅੰਕ_ਪਹਿਲਾ_glyphs),
					 hb_array (ligature_list, ਅੰਕ_ligatures),
					 hb_array (component_ਗਿਣਤੀ_list, ਅੰਕ_ligatures),
					 hb_array (component_list, ਅੰਕ_components));
  c.ਸਿਰਾ_serialize ();

  ਵਾਪਸ ret && !c.ਵਿੱਚ_error () ? c.copy<OT::SubstLookup> () : ਖਾਲੀ_ਸੂਚਕ;
}

ਸਥਿਰ OT::SubstLookup *
arabic_fallback_synthesize_lookup (ਅਚਲ hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ)
{
  ਜੇ (feature_ਸੂਚਕਾਂਕ < 4)
    ਵਾਪਸ arabic_fallback_synthesize_lookup_single (plan, font, feature_ਸੂਚਕਾਂਕ);
  ਨਹੀਂਤਾਂ
  {
    ਚੋਣ (feature_ਸੂਚਕਾਂਕ) {
      ਹਾਲਤ 4: ਵਾਪਸ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_ਸਾਰਣੀ, OT::LookupFlag::IgnoreMarks);
      ਹਾਲਤ 5: ਵਾਪਸ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_ਸਾਰਣੀ, OT::LookupFlag::IgnoreMarks);
      ਹਾਲਤ 6: ਵਾਪਸ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_ਸਾਰਣੀ, 0);
    }
  }
  ਪ੍ਰਤਿਜ੍ਞਾ (ਝੂਠ);
  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_ਲੰਮਾਈ_CONST (arabic_fallback_features)

ਬਣਤਰ arabic_fallback_plan_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_lookups;
  ਤਾਰਕਿਕ free_lookups;

  hb_ਨਕਾਬ_t ਨਕਾਬ_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_WIN1256
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic-win1256.hh"
#ਜੇ_ਅੰਤ

ਬਣਤਰ ManifestLookup
{
  ਜਨਤਕ:
  OT::ਟੈਗ ਟੈਗ_2;
  OT::Offset16_ਮੰਜ਼ਿਲ<OT::SubstLookup> lookup_ਵਿੱਥ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ OT::Array16_ਦਾ<ManifestLookup> Manifest;

ਸਥਿਰ ਤਾਰਕਿਕ
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_ਅਣਵਰਤਿਆ,
				   ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
				   hb_font_t *font HB_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  ਜੇ (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    ਵਾਪਸ ਝੂਠ;

  ਅਚਲ Manifest &manifest = reinterpret_cast<ਅਚਲ Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (ਆਕਾਰ (arabic_win1256_gsub_lookups.manifest_ਅੰਕੜੇ) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * ਆਕਾਰ (ManifestLookup), "");

  ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = manifest.len;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    fallback_plan->ਨਕਾਬ_array[j] = plan->map.get_1_ਨਕਾਬ (manifest[i].ਟੈਗ_2);
    ਜੇ (fallback_plan->ਨਕਾਬ_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookup_ਵਿੱਥ));
      ਜੇ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->ਅੰਕ_lookups = j;
  fallback_plan->free_lookups = ਝੂਠ;

  ਵਾਪਸ j > 0;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਤਾਰਕਿਕ
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   ਅਚਲ hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_ਲੰਮਾਈ_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ(arabic_fallback_features) ; i++)
  {
    fallback_plan->ਨਕਾਬ_array[j] = plan->map.get_1_ਨਕਾਬ (arabic_fallback_features[i]);
    ਜੇ (fallback_plan->ਨਕਾਬ_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      ਜੇ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->ਅੰਕ_lookups = j;
  fallback_plan->free_lookups = ਸੱਚ;

  ਵਾਪਸ j > 0;
}

ਸਥਿਰ arabic_fallback_plan_t *
arabic_fallback_plan_create (ਅਚਲ hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, ਆਕਾਰ (arabic_fallback_plan_t));
  ਜੇ (unlikely (!fallback_plan))
    ਵਾਪਸ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->ਅੰਕ_lookups = 0;
  fallback_plan->free_lookups = ਝੂਠ;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  ਜੇ (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    ਵਾਪਸ fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  ਜੇ (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    ਵਾਪਸ fallback_plan;

  ਪ੍ਰਤਿਜ੍ਞਾ (fallback_plan->ਅੰਕ_lookups == 0);
  hb_free (fallback_plan);
  ਵਾਪਸ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

ਸਥਿਰ ਖਾਲੀ
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  ਜੇ (!fallback_plan || fallback_plan->ਅੰਕ_lookups == 0)
    ਵਾਪਸ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < fallback_plan->ਅੰਕ_lookups; i++)
    ਜੇ (fallback_plan->lookup_array[i])
    {
      ਜੇ (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      ਜੇ (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

ਸਥਿਰ ਖਾਲੀ
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_ਬਫਰ_t *ਬਫਰ)
{
  OT::hb_ot_apply_context_t c (0, font, ਬਫਰ, hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ());
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < fallback_plan->ਅੰਕ_lookups; i++)
    ਜੇ (fallback_plan->lookup_array[i]) {
      c.ਸੈੱਟ_lookup_ਨਕਾਬ (fallback_plan->ਨਕਾਬ_array[i]);
      ਜੇ (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#ਜੇ_ਅੰਤ /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

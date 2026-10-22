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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-shape.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
ಸ್ಥಿರ ಧ್ರುವ hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

ಸ್ಥಿರ OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (ಧ್ರುವ hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  unsigned ಪೂರ್ಣಾಙ್ಕ feature_index)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs = 0;

  /* Populate arrays */
  ಪ್ರತಿ (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_index];
    hb_codepoint_t u_glyph, s_glyph;

    ಯದಿ (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      ಅನುವರ್ತ;

    glyphs[ಅಂಕೆ_glyphs] = u_glyph;
    substitutes[ಅಂಕೆ_glyphs] = s_glyph;

    ಅಂಕೆ_glyphs++;
  }

  ಯದಿ (!ಅಂಕೆ_glyphs)
    ಪ್ರತ್ಯಾಗಮ nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], ಅಂಕೆ_glyphs,
		  (ಪೂರ್ಣಾಙ್ಕ(*)(ಧ್ರುವ OT::HBUINT16*, ಧ್ರುವ OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  ವರ್ಣ buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, ಪರಿಮಾಣ (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  ತಾರ್ಕಿಕ ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, ಅಂಕೆ_glyphs),
				       hb_array (substitutes, ಅಂಕೆ_glyphs));
  c.end_serialize ();

  ಪ್ರತ್ಯಾಗಮ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

template <typename T>
ಸ್ಥಿರ OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (ಧ್ರುವ hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    ಧ್ರುವ T &ligature_table,
					    unsigned lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned ಪೂರ್ಣಾಙ್ಕ first_glyphs_indirection[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned ಪೂರ್ಣಾಙ್ಕ ligature_per_first_glyph_ಎಣಿಕೆ_list[ARRAY_LENGTH_CONST (first_glyphs)];
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_LENGTH_CONST (first_glyphs) * ARRAY_LENGTH_CONST(ligature_table[0].ligatures)];
  unsigned ಪೂರ್ಣಾಙ್ಕ component_ಎಣಿಕೆ_list[ARRAY_LENGTH_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_LENGTH_CONST (ligature_list) *
				 ARRAY_LENGTH_CONST (ligature_table[0].ligatures[0].components)];
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ligatures = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ first_glyph_idx = 0; first_glyph_idx < ARRAY_LENGTH (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    ಯದಿ (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      ಅನುವರ್ತ;
    first_glyphs[ಅಂಕೆ_first_glyphs] = first_glyph;
    ligature_per_first_glyph_ಎಣಿಕೆ_list[ಅಂಕೆ_first_glyphs] = 0;
    first_glyphs_indirection[ಅಂಕೆ_first_glyphs] = first_glyph_idx;
    ಅಂಕೆ_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], ಅಂಕೆ_first_glyphs,
		  (ಪೂರ್ಣಾಙ್ಕ(*)(ಧ್ರುವ OT::HBUINT16*, ಧ್ರುವ OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_first_glyphs; i++)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ first_glyph_idx = first_glyphs_indirection[i];

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ ligature_idx = 0; ligature_idx < ARRAY_LENGTH (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      ಯದಿ (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	ಅನುವರ್ತ;

      ಧ್ರುವ auto &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      unsigned component_ಎಣಿಕೆ = ARRAY_LENGTH_CONST (components);

      ತಾರ್ಕಿಕ matched = ಸತ್ಯ;
      ಪ್ರತಿ (unsigned j = 0; j < component_ಎಣಿಕೆ; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	ಯದಿ (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = ಅಸತ್ಯ;
	  ಭಙ್ಗ;
	}

	component_list[ಅಂಕೆ_components++] = component_glyph;
      }
      ಯದಿ (!matched)
        ಅನುವರ್ತ;

      component_ಎಣಿಕೆ_list[ಅಂಕೆ_ligatures] = 1 + component_ಎಣಿಕೆ;
      ligature_list[ಅಂಕೆ_ligatures] = ligature_glyph;

      ligature_per_first_glyph_ಎಣಿಕೆ_list[i]++;

      ಅಂಕೆ_ligatures++;
    }
  }

  ಯದಿ (!ಅಂಕೆ_ligatures)
    ಪ್ರತ್ಯಾಗಮ nullptr;


  /* 16 bytes per ligature ought to be enough... */
  ವರ್ಣ buf[ARRAY_LENGTH_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, ಪರಿಮಾಣ (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  ತಾರ್ಕಿಕ ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, ಅಂಕೆ_first_glyphs),
					 hb_array (ligature_per_first_glyph_ಎಣಿಕೆ_list, ಅಂಕೆ_first_glyphs),
					 hb_array (ligature_list, ಅಂಕೆ_ligatures),
					 hb_array (component_ಎಣಿಕೆ_list, ಅಂಕೆ_ligatures),
					 hb_array (component_list, ಅಂಕೆ_components));
  c.end_serialize ();

  ಪ್ರತ್ಯಾಗಮ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

ಸ್ಥಿರ OT::SubstLookup *
arabic_fallback_synthesize_lookup (ಧ್ರುವ hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   unsigned ಪೂರ್ಣಾಙ್ಕ feature_index)
{
  ಯದಿ (feature_index < 4)
    ಪ್ರತ್ಯಾಗಮ arabic_fallback_synthesize_lookup_single (plan, font, feature_index);
  ಅನ್ಯಥಾ
  {
    ವಿಕಲ್ಪ (feature_index) {
      ಪಕ್ಷ 4: ಪ್ರತ್ಯಾಗಮ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      ಪಕ್ಷ 5: ಪ್ರತ್ಯಾಗಮ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      ಪಕ್ಷ 6: ಪ್ರತ್ಯಾಗಮ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  ಪ್ರತಿಜ್ಞಾ (ಅಸತ್ಯ);
  ಪ್ರತ್ಯಾಗಮ nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_LENGTH_CONST (arabic_fallback_features)

ಸಂರಚನಾ arabic_fallback_plan_t
{
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_lookups;
  ತಾರ್ಕಿಕ free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ifndef HB_NO_WIN1256
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-arabic-win1256.hh"
#endif

ಸಂರಚನಾ ManifestLookup
{
  ಸಾರ್ವಜನಿಕ:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};
ಪ್ರಕಾರನಿರ್ದೇಶ OT::Array16Of<ManifestLookup> Manifest;

ಸ್ಥಿರ ತಾರ್ಕಿಕ
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   ಧ್ರುವ hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#ifndef HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  ಯದಿ (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಧ್ರುವ Manifest &manifest = reinterpret_cast<ಧ್ರುವ Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (ಪರಿಮಾಣ (arabic_win1256_gsub_lookups.manifestData) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * ಪರಿಮಾಣ (ManifestLookup), "");

  unsigned j = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = manifest.len;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    ಯದಿ (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      ಯದಿ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->ಅಂಕೆ_lookups = j;
  fallback_plan->free_lookups = ಅಸತ್ಯ;

  ಪ್ರತ್ಯಾಗಮ j > 0;
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   ಧ್ರುವ hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_LENGTH_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  unsigned ಪೂರ್ಣಾಙ್ಕ j = 0;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    ಯದಿ (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      ಯದಿ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->ಅಂಕೆ_lookups = j;
  fallback_plan->free_lookups = ಸತ್ಯ;

  ಪ್ರತ್ಯಾಗಮ j > 0;
}

ಸ್ಥಿರ arabic_fallback_plan_t *
arabic_fallback_plan_create (ಧ್ರುವ hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, ಪರಿಮಾಣ (arabic_fallback_plan_t));
  ಯದಿ (unlikely (!fallback_plan))
    ಪ್ರತ್ಯಾಗಮ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->ಅಂಕೆ_lookups = 0;
  fallback_plan->free_lookups = ಅಸತ್ಯ;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  ಯದಿ (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    ಪ್ರತ್ಯಾಗಮ fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  ಯದಿ (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    ಪ್ರತ್ಯಾಗಮ fallback_plan;

  ಪ್ರತಿಜ್ಞಾ (fallback_plan->ಅಂಕೆ_lookups == 0);
  hb_free (fallback_plan);
  ಪ್ರತ್ಯಾಗಮ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

ಸ್ಥಿರ ಶೂನ್ಯ
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  ಯದಿ (!fallback_plan || fallback_plan->ಅಂಕೆ_lookups == 0)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < fallback_plan->ಅಂಕೆ_lookups; i++)
    ಯದಿ (fallback_plan->lookup_array[i])
    {
      ಯದಿ (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      ಯದಿ (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

ಸ್ಥಿರ ಶೂನ್ಯ
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
  OT::hb_ot_apply_context_t c (0, font, buffer, hb_blob_get_empty ());
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < fallback_plan->ಅಂಕೆ_lookups; i++)
    ಯದಿ (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      ಯದಿ (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

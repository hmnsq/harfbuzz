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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-ot-shape.hh"
#അന്തര്ഭാവ "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
സ്ഥിതം സ്ഥിരം hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

സ്ഥിതം OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  unsigned പൂർണ്ണസംഖ്യ feature_index)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = 0;

  /* Populate arrays */
  വേണ്ടി (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_index];
    hb_codepoint_t u_glyph, s_glyph;

    എങ്കിൽ (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      തുടരുക;

    glyphs[അക്കം_glyphs] = u_glyph;
    substitutes[അക്കം_glyphs] = s_glyph;

    അക്കം_glyphs++;
  }

  എങ്കിൽ (!അക്കം_glyphs)
    തിരികെ nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], അക്കം_glyphs,
		  (പൂർണ്ണസംഖ്യ(*)(സ്ഥിരം OT::HBUINT16*, സ്ഥിരം OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  അക്ഷരം buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, വലുപ്പം (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  തർക്കം ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, അക്കം_glyphs),
				       hb_array (substitutes, അക്കം_glyphs));
  c.end_serialize ();

  തിരികെ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

template <typename T>
സ്ഥിതം OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    സ്ഥിരം T &ligature_table,
					    unsigned lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned പൂർണ്ണസംഖ്യ first_glyphs_indirection[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned പൂർണ്ണസംഖ്യ ligature_per_first_glyph_എണ്ണം_list[ARRAY_LENGTH_CONST (first_glyphs)];
  unsigned പൂർണ്ണസംഖ്യ അക്കം_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_LENGTH_CONST (first_glyphs) * ARRAY_LENGTH_CONST(ligature_table[0].ligatures)];
  unsigned പൂർണ്ണസംഖ്യ component_എണ്ണം_list[ARRAY_LENGTH_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_LENGTH_CONST (ligature_list) *
				 ARRAY_LENGTH_CONST (ligature_table[0].ligatures[0].components)];
  unsigned പൂർണ്ണസംഖ്യ അക്കം_ligatures = 0;
  unsigned പൂർണ്ണസംഖ്യ അക്കം_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ first_glyph_idx = 0; first_glyph_idx < ARRAY_LENGTH (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    എങ്കിൽ (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      തുടരുക;
    first_glyphs[അക്കം_first_glyphs] = first_glyph;
    ligature_per_first_glyph_എണ്ണം_list[അക്കം_first_glyphs] = 0;
    first_glyphs_indirection[അക്കം_first_glyphs] = first_glyph_idx;
    അക്കം_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], അക്കം_first_glyphs,
		  (പൂർണ്ണസംഖ്യ(*)(സ്ഥിരം OT::HBUINT16*, സ്ഥിരം OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_first_glyphs; i++)
  {
    unsigned പൂർണ്ണസംഖ്യ first_glyph_idx = first_glyphs_indirection[i];

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ ligature_idx = 0; ligature_idx < ARRAY_LENGTH (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      എങ്കിൽ (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	തുടരുക;

      സ്ഥിരം auto &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      unsigned component_എണ്ണം = ARRAY_LENGTH_CONST (components);

      തർക്കം matched = സത്യം;
      വേണ്ടി (unsigned j = 0; j < component_എണ്ണം; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	എങ്കിൽ (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = അസത്യം;
	  നിർത്തുക;
	}

	component_list[അക്കം_components++] = component_glyph;
      }
      എങ്കിൽ (!matched)
        തുടരുക;

      component_എണ്ണം_list[അക്കം_ligatures] = 1 + component_എണ്ണം;
      ligature_list[അക്കം_ligatures] = ligature_glyph;

      ligature_per_first_glyph_എണ്ണം_list[i]++;

      അക്കം_ligatures++;
    }
  }

  എങ്കിൽ (!അക്കം_ligatures)
    തിരികെ nullptr;


  /* 16 bytes per ligature ought to be enough... */
  അക്ഷരം buf[ARRAY_LENGTH_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, വലുപ്പം (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  തർക്കം ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, അക്കം_first_glyphs),
					 hb_array (ligature_per_first_glyph_എണ്ണം_list, അക്കം_first_glyphs),
					 hb_array (ligature_list, അക്കം_ligatures),
					 hb_array (component_എണ്ണം_list, അക്കം_ligatures),
					 hb_array (component_list, അക്കം_components));
  c.end_serialize ();

  തിരികെ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

സ്ഥിതം OT::SubstLookup *
arabic_fallback_synthesize_lookup (സ്ഥിരം hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   unsigned പൂർണ്ണസംഖ്യ feature_index)
{
  എങ്കിൽ (feature_index < 4)
    തിരികെ arabic_fallback_synthesize_lookup_single (plan, font, feature_index);
  അല്ലെങ്കിൽ
  {
    തിരഞ്ഞെടുപ്പ് (feature_index) {
      അവസ്ഥ 4: തിരികെ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      അവസ്ഥ 5: തിരികെ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      അവസ്ഥ 6: തിരികെ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  പ്രതിജ്ഞാ (അസത്യം);
  തിരികെ nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_LENGTH_CONST (arabic_fallback_features)

ഘടന arabic_fallback_plan_t
{
  unsigned പൂർണ്ണസംഖ്യ അക്കം_lookups;
  തർക്കം free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ifndef HB_NO_WIN1256
#അന്തര്ഭാവ "hb-ot-shaper-arabic-win1256.hh"
#endif

ഘടന ManifestLookup
{
  പൊതു:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};
തരനിർവചനം OT::Array16Of<ManifestLookup> Manifest;

സ്ഥിതം തർക്കം
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   സ്ഥിരം hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#ifndef HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  എങ്കിൽ (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    തിരികെ അസത്യം;

  സ്ഥിരം Manifest &manifest = reinterpret_cast<സ്ഥിരം Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (വലുപ്പം (arabic_win1256_gsub_lookups.manifestData) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * വലുപ്പം (ManifestLookup), "");

  unsigned j = 0;
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = manifest.len;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    എങ്കിൽ (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      എങ്കിൽ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->അക്കം_lookups = j;
  fallback_plan->free_lookups = അസത്യം;

  തിരികെ j > 0;
#അല്ലെങ്കിൽ
  തിരികെ അസത്യം;
#endif
}

സ്ഥിതം തർക്കം
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   സ്ഥിരം hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_LENGTH_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  unsigned പൂർണ്ണസംഖ്യ j = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    എങ്കിൽ (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      എങ്കിൽ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->അക്കം_lookups = j;
  fallback_plan->free_lookups = സത്യം;

  തിരികെ j > 0;
}

സ്ഥിതം arabic_fallback_plan_t *
arabic_fallback_plan_create (സ്ഥിരം hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, വലുപ്പം (arabic_fallback_plan_t));
  എങ്കിൽ (unlikely (!fallback_plan))
    തിരികെ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->അക്കം_lookups = 0;
  fallback_plan->free_lookups = അസത്യം;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  എങ്കിൽ (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    തിരികെ fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  എങ്കിൽ (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    തിരികെ fallback_plan;

  പ്രതിജ്ഞാ (fallback_plan->അക്കം_lookups == 0);
  hb_free (fallback_plan);
  തിരികെ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

സ്ഥിതം ശൂന്യം
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  എങ്കിൽ (!fallback_plan || fallback_plan->അക്കം_lookups == 0)
    തിരികെ;

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < fallback_plan->അക്കം_lookups; i++)
    എങ്കിൽ (fallback_plan->lookup_array[i])
    {
      എങ്കിൽ (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      എങ്കിൽ (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

സ്ഥിതം ശൂന്യം
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
  OT::hb_ot_apply_context_t c (0, font, buffer, hb_blob_get_empty ());
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < fallback_plan->അക്കം_lookups; i++)
    എങ്കിൽ (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      എങ്കിൽ (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

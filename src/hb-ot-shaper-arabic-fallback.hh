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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-ot-shape.hh"
#अन्तर्भाव "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
स्थिर ध्रुव hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

स्थिर OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (ध्रुव hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  unsigned पूर्णाङ्क feature_index)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  unsigned पूर्णाङ्क अंक_glyphs = 0;

  /* Populate arrays */
  प्रति (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_index];
    hb_codepoint_t u_glyph, s_glyph;

    यदि (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      अनुवर्त;

    glyphs[अंक_glyphs] = u_glyph;
    substitutes[अंक_glyphs] = s_glyph;

    अंक_glyphs++;
  }

  यदि (!अंक_glyphs)
    प्रत्यागम nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], अंक_glyphs,
		  (पूर्णाङ्क(*)(ध्रुव OT::HBUINT16*, ध्रुव OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  वर्ण buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, परिमाण (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  तार्किक ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, अंक_glyphs),
				       hb_array (substitutes, अंक_glyphs));
  c.end_serialize ();

  प्रत्यागम ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

template <typename T>
स्थिर OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (ध्रुव hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    ध्रुव T &ligature_table,
					    unsigned lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned पूर्णाङ्क first_glyphs_indirection[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned पूर्णाङ्क ligature_per_first_glyph_मोजणी_list[ARRAY_LENGTH_CONST (first_glyphs)];
  unsigned पूर्णाङ्क अंक_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_LENGTH_CONST (first_glyphs) * ARRAY_LENGTH_CONST(ligature_table[0].ligatures)];
  unsigned पूर्णाङ्क component_मोजणी_list[ARRAY_LENGTH_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_LENGTH_CONST (ligature_list) *
				 ARRAY_LENGTH_CONST (ligature_table[0].ligatures[0].components)];
  unsigned पूर्णाङ्क अंक_ligatures = 0;
  unsigned पूर्णाङ्क अंक_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  प्रति (unsigned पूर्णाङ्क first_glyph_idx = 0; first_glyph_idx < ARRAY_LENGTH (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    यदि (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      अनुवर्त;
    first_glyphs[अंक_first_glyphs] = first_glyph;
    ligature_per_first_glyph_मोजणी_list[अंक_first_glyphs] = 0;
    first_glyphs_indirection[अंक_first_glyphs] = first_glyph_idx;
    अंक_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], अंक_first_glyphs,
		  (पूर्णाङ्क(*)(ध्रुव OT::HBUINT16*, ध्रुव OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  प्रति (unsigned पूर्णाङ्क i = 0; i < अंक_first_glyphs; i++)
  {
    unsigned पूर्णाङ्क first_glyph_idx = first_glyphs_indirection[i];

    प्रति (unsigned पूर्णाङ्क ligature_idx = 0; ligature_idx < ARRAY_LENGTH (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      यदि (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	अनुवर्त;

      ध्रुव auto &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      unsigned component_मोजणी = ARRAY_LENGTH_CONST (components);

      तार्किक matched = सत्य;
      प्रति (unsigned j = 0; j < component_मोजणी; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	यदि (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = असत्य;
	  भङ्ग;
	}

	component_list[अंक_components++] = component_glyph;
      }
      यदि (!matched)
        अनुवर्त;

      component_मोजणी_list[अंक_ligatures] = 1 + component_मोजणी;
      ligature_list[अंक_ligatures] = ligature_glyph;

      ligature_per_first_glyph_मोजणी_list[i]++;

      अंक_ligatures++;
    }
  }

  यदि (!अंक_ligatures)
    प्रत्यागम nullptr;


  /* 16 bytes per ligature ought to be enough... */
  वर्ण buf[ARRAY_LENGTH_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, परिमाण (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  तार्किक ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, अंक_first_glyphs),
					 hb_array (ligature_per_first_glyph_मोजणी_list, अंक_first_glyphs),
					 hb_array (ligature_list, अंक_ligatures),
					 hb_array (component_मोजणी_list, अंक_ligatures),
					 hb_array (component_list, अंक_components));
  c.end_serialize ();

  प्रत्यागम ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

स्थिर OT::SubstLookup *
arabic_fallback_synthesize_lookup (ध्रुव hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   unsigned पूर्णाङ्क feature_index)
{
  यदि (feature_index < 4)
    प्रत्यागम arabic_fallback_synthesize_lookup_single (plan, font, feature_index);
  अन्यथा
  {
    विकल्प (feature_index) {
      पक्ष 4: प्रत्यागम arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      पक्ष 5: प्रत्यागम arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      पक्ष 6: प्रत्यागम arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  प्रतिज्ञा (असत्य);
  प्रत्यागम nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_LENGTH_CONST (arabic_fallback_features)

संरचना arabic_fallback_plan_t
{
  unsigned पूर्णाङ्क अंक_lookups;
  तार्किक free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ifndef HB_NO_WIN1256
#अन्तर्भाव "hb-ot-shaper-arabic-win1256.hh"
#endif

संरचना ManifestLookup
{
  सार्वजनिक:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};
प्रकारनिर्देश OT::Array16Of<ManifestLookup> Manifest;

स्थिर तार्किक
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   ध्रुव hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#ifndef HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  यदि (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    प्रत्यागम असत्य;

  ध्रुव Manifest &manifest = reinterpret_cast<ध्रुव Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (परिमाण (arabic_win1256_gsub_lookups.manifestData) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * परिमाण (ManifestLookup), "");

  unsigned j = 0;
  unsigned पूर्णाङ्क मोजणी = manifest.len;
  प्रति (unsigned पूर्णाङ्क i = 0; i < मोजणी; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    यदि (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      यदि (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->अंक_lookups = j;
  fallback_plan->free_lookups = असत्य;

  प्रत्यागम j > 0;
#अन्यथा
  प्रत्यागम असत्य;
#endif
}

स्थिर तार्किक
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   ध्रुव hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_LENGTH_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  unsigned पूर्णाङ्क j = 0;
  प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    यदि (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      यदि (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->अंक_lookups = j;
  fallback_plan->free_lookups = सत्य;

  प्रत्यागम j > 0;
}

स्थिर arabic_fallback_plan_t *
arabic_fallback_plan_create (ध्रुव hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, परिमाण (arabic_fallback_plan_t));
  यदि (unlikely (!fallback_plan))
    प्रत्यागम const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->अंक_lookups = 0;
  fallback_plan->free_lookups = असत्य;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  यदि (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    प्रत्यागम fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  यदि (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    प्रत्यागम fallback_plan;

  प्रतिज्ञा (fallback_plan->अंक_lookups == 0);
  hb_free (fallback_plan);
  प्रत्यागम const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

स्थिर शून्य
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  यदि (!fallback_plan || fallback_plan->अंक_lookups == 0)
    प्रत्यागम;

  प्रति (unsigned पूर्णाङ्क i = 0; i < fallback_plan->अंक_lookups; i++)
    यदि (fallback_plan->lookup_array[i])
    {
      यदि (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      यदि (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

स्थिर शून्य
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
  OT::hb_ot_apply_context_t c (0, font, buffer, hb_blob_get_empty ());
  प्रति (unsigned पूर्णाङ्क i = 0; i < fallback_plan->अंक_lookups; i++)
    यदि (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      यदि (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

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
स्थिर अचर hb_tag_t arabic_fallback_features[] =
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
arabic_fallback_synthesize_lookup_single (अचर hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  अचिन्हित पूर्णाङ्क feature_index)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  अचिन्हित पूर्णाङ्क अङ्क_glyphs = 0;

  /* Populate arrays */
  लागि (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_index];
    hb_codepoint_t u_glyph, s_glyph;

    यदि (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      जारी;

    glyphs[अङ्क_glyphs] = u_glyph;
    substitutes[अङ्क_glyphs] = s_glyph;

    अङ्क_glyphs++;
  }

  यदि (!अङ्क_glyphs)
    फर्काऊ nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], अङ्क_glyphs,
		  (पूर्णाङ्क(*)(अचर OT::HBUINT16*, अचर OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  वर्ण buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, परिमाण (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  तार्किक ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, अङ्क_glyphs),
				       hb_array (substitutes, अङ्क_glyphs));
  c.end_serialize ();

  फर्काऊ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

ढाँचा <typename T>
स्थिर OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (अचर hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    अचर T &ligature_table,
					    अचिन्हित lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_LENGTH_CONST (ligature_table)];
  अचिन्हित पूर्णाङ्क first_glyphs_indirection[ARRAY_LENGTH_CONST (ligature_table)];
  अचिन्हित पूर्णाङ्क ligature_per_first_glyph_गन्ती_list[ARRAY_LENGTH_CONST (first_glyphs)];
  अचिन्हित पूर्णाङ्क अङ्क_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_LENGTH_CONST (first_glyphs) * ARRAY_LENGTH_CONST(ligature_table[0].ligatures)];
  अचिन्हित पूर्णाङ्क component_गन्ती_list[ARRAY_LENGTH_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_LENGTH_CONST (ligature_list) *
				 ARRAY_LENGTH_CONST (ligature_table[0].ligatures[0].components)];
  अचिन्हित पूर्णाङ्क अङ्क_ligatures = 0;
  अचिन्हित पूर्णाङ्क अङ्क_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  लागि (अचिन्हित पूर्णाङ्क first_glyph_idx = 0; first_glyph_idx < ARRAY_LENGTH (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    यदि (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      जारी;
    first_glyphs[अङ्क_first_glyphs] = first_glyph;
    ligature_per_first_glyph_गन्ती_list[अङ्क_first_glyphs] = 0;
    first_glyphs_indirection[अङ्क_first_glyphs] = first_glyph_idx;
    अङ्क_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], अङ्क_first_glyphs,
		  (पूर्णाङ्क(*)(अचर OT::HBUINT16*, अचर OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < अङ्क_first_glyphs; i++)
  {
    अचिन्हित पूर्णाङ्क first_glyph_idx = first_glyphs_indirection[i];

    लागि (अचिन्हित पूर्णाङ्क ligature_idx = 0; ligature_idx < ARRAY_LENGTH (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      यदि (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	जारी;

      अचर स्वतः &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      अचिन्हित component_गन्ती = ARRAY_LENGTH_CONST (components);

      तार्किक matched = सत्य;
      लागि (अचिन्हित j = 0; j < component_गन्ती; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	यदि (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = असत्य;
	  रोक;
	}

	component_list[अङ्क_components++] = component_glyph;
      }
      यदि (!matched)
        जारी;

      component_गन्ती_list[अङ्क_ligatures] = 1 + component_गन्ती;
      ligature_list[अङ्क_ligatures] = ligature_glyph;

      ligature_per_first_glyph_गन्ती_list[i]++;

      अङ्क_ligatures++;
    }
  }

  यदि (!अङ्क_ligatures)
    फर्काऊ nullptr;


  /* 16 bytes per ligature ought to be enough... */
  वर्ण buf[ARRAY_LENGTH_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, परिमाण (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  तार्किक ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, अङ्क_first_glyphs),
					 hb_array (ligature_per_first_glyph_गन्ती_list, अङ्क_first_glyphs),
					 hb_array (ligature_list, अङ्क_ligatures),
					 hb_array (component_गन्ती_list, अङ्क_ligatures),
					 hb_array (component_list, अङ्क_components));
  c.end_serialize ();

  फर्काऊ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

स्थिर OT::SubstLookup *
arabic_fallback_synthesize_lookup (अचर hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   अचिन्हित पूर्णाङ्क feature_index)
{
  यदि (feature_index < 4)
    फर्काऊ arabic_fallback_synthesize_lookup_single (plan, font, feature_index);
  अन्यथा
  {
    विकल्प (feature_index) {
      पक्ष 4: फर्काऊ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      पक्ष 5: फर्काऊ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      पक्ष 6: फर्काऊ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  प्रतिज्ञा (असत्य);
  फर्काऊ nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_LENGTH_CONST (arabic_fallback_features)

संरचना arabic_fallback_plan_t
{
  अचिन्हित पूर्णाङ्क अङ्क_lookups;
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
				   अचर hb_ot_shape_plan_t *plan HB_UNUSED,
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
    फर्काऊ असत्य;

  अचर Manifest &manifest = reinterpret_cast<अचर Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (परिमाण (arabic_win1256_gsub_lookups.manifestData) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * परिमाण (ManifestLookup), "");

  अचिन्हित j = 0;
  अचिन्हित पूर्णाङ्क गन्ती = manifest.len;
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
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

  fallback_plan->अङ्क_lookups = j;
  fallback_plan->free_lookups = असत्य;

  फर्काऊ j > 0;
#अन्यथा
  फर्काऊ असत्य;
#endif
}

स्थिर तार्किक
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   अचर hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_LENGTH_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  अचिन्हित पूर्णाङ्क j = 0;
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH(arabic_fallback_features) ; i++)
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

  fallback_plan->अङ्क_lookups = j;
  fallback_plan->free_lookups = सत्य;

  फर्काऊ j > 0;
}

स्थिर arabic_fallback_plan_t *
arabic_fallback_plan_create (अचर hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, परिमाण (arabic_fallback_plan_t));
  यदि (unlikely (!fallback_plan))
    फर्काऊ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->अङ्क_lookups = 0;
  fallback_plan->free_lookups = असत्य;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  यदि (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    फर्काऊ fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  यदि (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    फर्काऊ fallback_plan;

  प्रतिज्ञा (fallback_plan->अङ्क_lookups == 0);
  hb_free (fallback_plan);
  फर्काऊ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

स्थिर शून्य
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  यदि (!fallback_plan || fallback_plan->अङ्क_lookups == 0)
    फर्काऊ;

  लागि (अचिन्हित पूर्णाङ्क i = 0; i < fallback_plan->अङ्क_lookups; i++)
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
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < fallback_plan->अङ्क_lookups; i++)
    यदि (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      यदि (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

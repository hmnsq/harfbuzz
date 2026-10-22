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

#अगर_अघोषित HB_OT_SHAPER_ARABIC_FALLBACK_HH
#घोषणा HB_OT_SHAPER_ARABIC_FALLBACK_HH

#समावेश "hb.hh"

#समावेश "hb-ot-shape.hh"
#समावेश "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
अटल स्थिर hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

अटल OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  अचिन्हित पूर्णांक feature_सूचकांक)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  अचिन्हित पूर्णांक अंक_glyphs = 0;

  /* Populate arrays */
  क्रम (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_सूचकांक];
    hb_codepoint_t u_glyph, s_glyph;

    अगर (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      जारी;

    glyphs[अंक_glyphs] = u_glyph;
    substitutes[अंक_glyphs] = s_glyph;

    अंक_glyphs++;
  }

  अगर (!अंक_glyphs)
    वापस nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], अंक_glyphs,
		  (पूर्णांक(*)(स्थिर OT::HBUINT16*, स्थिर OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  अक्षर buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, माप (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  bool ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, अंक_glyphs),
				       hb_array (substitutes, अंक_glyphs));
  c.end_serialize ();

  वापस ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

ढाँचा <typename T>
अटल OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    स्थिर T &ligature_table,
					    अचिन्हित lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_लंबाई_CONST (ligature_table)];
  अचिन्हित पूर्णांक first_glyphs_indirection[ARRAY_लंबाई_CONST (ligature_table)];
  अचिन्हित पूर्णांक ligature_per_first_glyph_गिनती_list[ARRAY_लंबाई_CONST (first_glyphs)];
  अचिन्हित पूर्णांक अंक_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_लंबाई_CONST (first_glyphs) * ARRAY_लंबाई_CONST(ligature_table[0].ligatures)];
  अचिन्हित पूर्णांक component_गिनती_list[ARRAY_लंबाई_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_लंबाई_CONST (ligature_list) *
				 ARRAY_लंबाई_CONST (ligature_table[0].ligatures[0].components)];
  अचिन्हित पूर्णांक अंक_ligatures = 0;
  अचिन्हित पूर्णांक अंक_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  क्रम (अचिन्हित पूर्णांक first_glyph_idx = 0; first_glyph_idx < ARRAY_लंबाई (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    अगर (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      जारी;
    first_glyphs[अंक_first_glyphs] = first_glyph;
    ligature_per_first_glyph_गिनती_list[अंक_first_glyphs] = 0;
    first_glyphs_indirection[अंक_first_glyphs] = first_glyph_idx;
    अंक_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], अंक_first_glyphs,
		  (पूर्णांक(*)(स्थिर OT::HBUINT16*, स्थिर OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_first_glyphs; i++)
  {
    अचिन्हित पूर्णांक first_glyph_idx = first_glyphs_indirection[i];

    क्रम (अचिन्हित पूर्णांक ligature_idx = 0; ligature_idx < ARRAY_लंबाई (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      अगर (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	जारी;

      स्थिर स्वतः &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      अचिन्हित component_गिनती = ARRAY_लंबाई_CONST (components);

      bool matched = true;
      क्रम (अचिन्हित j = 0; j < component_गिनती; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	अगर (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = false;
	  अवरोध;
	}

	component_list[अंक_components++] = component_glyph;
      }
      अगर (!matched)
        जारी;

      component_गिनती_list[अंक_ligatures] = 1 + component_गिनती;
      ligature_list[अंक_ligatures] = ligature_glyph;

      ligature_per_first_glyph_गिनती_list[i]++;

      अंक_ligatures++;
    }
  }

  अगर (!अंक_ligatures)
    वापस nullptr;


  /* 16 bytes per ligature ought to be enough... */
  अक्षर buf[ARRAY_लंबाई_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, माप (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  bool ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, अंक_first_glyphs),
					 hb_array (ligature_per_first_glyph_गिनती_list, अंक_first_glyphs),
					 hb_array (ligature_list, अंक_ligatures),
					 hb_array (component_गिनती_list, अंक_ligatures),
					 hb_array (component_list, अंक_components));
  c.end_serialize ();

  वापस ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

अटल OT::SubstLookup *
arabic_fallback_synthesize_lookup (स्थिर hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   अचिन्हित पूर्णांक feature_सूचकांक)
{
  अगर (feature_सूचकांक < 4)
    वापस arabic_fallback_synthesize_lookup_single (plan, font, feature_सूचकांक);
  अन्यथा
  {
    चयन (feature_सूचकांक) {
      हाल 4: वापस arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      हाल 5: वापस arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      हाल 6: वापस arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  निश्चित (false);
  वापस nullptr;
}

#घोषणा ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_लंबाई_CONST (arabic_fallback_features)

काष्ठा arabic_fallback_plan_t
{
  अचिन्हित पूर्णांक अंक_lookups;
  bool free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#अगर_अघोषित HB_NO_WIN1256
#समावेश "hb-ot-shaper-arabic-win1256.hh"
#पूर्ण_अगर

काष्ठा ManifestLookup
{
  खुला:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  खुला:
  DEFINE_आकार_STATIC (6);
};
प्रकार OT::Array16Of<ManifestLookup> Manifest;

अटल bool
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   स्थिर hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#अगर_अघोषित HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  अगर (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    वापस false;

  स्थिर Manifest &manifest = reinterpret_cast<स्थिर Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (माप (arabic_win1256_gsub_lookups.manifest_तथ्य) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * माप (ManifestLookup), "");

  अचिन्हित j = 0;
  अचिन्हित पूर्णांक गिनती = manifest.len;
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    अगर (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      अगर (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->अंक_lookups = j;
  fallback_plan->free_lookups = false;

  वापस j > 0;
#अन्यथा
  वापस false;
#पूर्ण_अगर
}

अटल bool
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   स्थिर hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_लंबाई_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  अचिन्हित पूर्णांक j = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    अगर (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      अगर (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->अंक_lookups = j;
  fallback_plan->free_lookups = true;

  वापस j > 0;
}

अटल arabic_fallback_plan_t *
arabic_fallback_plan_create (स्थिर hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, माप (arabic_fallback_plan_t));
  अगर (unlikely (!fallback_plan))
    वापस const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->अंक_lookups = 0;
  fallback_plan->free_lookups = false;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  अगर (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    वापस fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  अगर (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    वापस fallback_plan;

  निश्चित (fallback_plan->अंक_lookups == 0);
  hb_free (fallback_plan);
  वापस const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

अटल व्योम
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  अगर (!fallback_plan || fallback_plan->अंक_lookups == 0)
    वापस;

  क्रम (अचिन्हित पूर्णांक i = 0; i < fallback_plan->अंक_lookups; i++)
    अगर (fallback_plan->lookup_array[i])
    {
      अगर (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      अगर (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

अटल व्योम
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_बफर_t *बफर)
{
  OT::hb_ot_apply_context_t c (0, font, बफर, hb_blob_get_empty ());
  क्रम (अचिन्हित पूर्णांक i = 0; i < fallback_plan->अंक_lookups; i++)
    अगर (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      अगर (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#पूर्ण_अगर /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

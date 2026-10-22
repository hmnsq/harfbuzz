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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-ot-shape.hh"
#অন্তর্ভাব "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
স্থির ধ্রুবক hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

স্থির OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  unsigned পূর্ণসংখ্যা feature_সূচক)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs = 0;

  /* Populate arrays */
  জন্য (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_সূচক];
    hb_codepoint_t u_glyph, s_glyph;

    যদি (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      চালু;

    glyphs[অঙ্ক_glyphs] = u_glyph;
    substitutes[অঙ্ক_glyphs] = s_glyph;

    অঙ্ক_glyphs++;
  }

  যদি (!অঙ্ক_glyphs)
    ফেরত nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], অঙ্ক_glyphs,
		  (পূর্ণসংখ্যা(*)(ধ্রুবক OT::HBUINT16*, ধ্রুবক OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  অক্ষর buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, আকার (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  যৌক্তিক ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, অঙ্ক_glyphs),
				       hb_array (substitutes, অঙ্ক_glyphs));
  c.end_serialize ();

  ফেরত ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

template <typename T>
স্থির OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    ধ্রুবক T &ligature_table,
					    unsigned lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_দৈর্ঘ্য_CONST (ligature_table)];
  unsigned পূর্ণসংখ্যা first_glyphs_indirection[ARRAY_দৈর্ঘ্য_CONST (ligature_table)];
  unsigned পূর্ণসংখ্যা ligature_per_first_glyph_গুনতি_list[ARRAY_দৈর্ঘ্য_CONST (first_glyphs)];
  unsigned পূর্ণসংখ্যা অঙ্ক_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_দৈর্ঘ্য_CONST (first_glyphs) * ARRAY_দৈর্ঘ্য_CONST(ligature_table[0].ligatures)];
  unsigned পূর্ণসংখ্যা component_গুনতি_list[ARRAY_দৈর্ঘ্য_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_দৈর্ঘ্য_CONST (ligature_list) *
				 ARRAY_দৈর্ঘ্য_CONST (ligature_table[0].ligatures[0].components)];
  unsigned পূর্ণসংখ্যা অঙ্ক_ligatures = 0;
  unsigned পূর্ণসংখ্যা অঙ্ক_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  জন্য (unsigned পূর্ণসংখ্যা first_glyph_idx = 0; first_glyph_idx < ARRAY_দৈর্ঘ্য (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    যদি (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      চালু;
    first_glyphs[অঙ্ক_first_glyphs] = first_glyph;
    ligature_per_first_glyph_গুনতি_list[অঙ্ক_first_glyphs] = 0;
    first_glyphs_indirection[অঙ্ক_first_glyphs] = first_glyph_idx;
    অঙ্ক_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], অঙ্ক_first_glyphs,
		  (পূর্ণসংখ্যা(*)(ধ্রুবক OT::HBUINT16*, ধ্রুবক OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_first_glyphs; i++)
  {
    unsigned পূর্ণসংখ্যা first_glyph_idx = first_glyphs_indirection[i];

    জন্য (unsigned পূর্ণসংখ্যা ligature_idx = 0; ligature_idx < ARRAY_দৈর্ঘ্য (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      যদি (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	চালু;

      ধ্রুবক auto &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      unsigned component_গুনতি = ARRAY_দৈর্ঘ্য_CONST (components);

      যৌক্তিক matched = সত্য;
      জন্য (unsigned j = 0; j < component_গুনতি; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	যদি (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = মিথ্যা;
	  বিরতি;
	}

	component_list[অঙ্ক_components++] = component_glyph;
      }
      যদি (!matched)
        চালু;

      component_গুনতি_list[অঙ্ক_ligatures] = 1 + component_গুনতি;
      ligature_list[অঙ্ক_ligatures] = ligature_glyph;

      ligature_per_first_glyph_গুনতি_list[i]++;

      অঙ্ক_ligatures++;
    }
  }

  যদি (!অঙ্ক_ligatures)
    ফেরত nullptr;


  /* 16 bytes per ligature ought to be enough... */
  অক্ষর buf[ARRAY_দৈর্ঘ্য_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, আকার (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  যৌক্তিক ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, অঙ্ক_first_glyphs),
					 hb_array (ligature_per_first_glyph_গুনতি_list, অঙ্ক_first_glyphs),
					 hb_array (ligature_list, অঙ্ক_ligatures),
					 hb_array (component_গুনতি_list, অঙ্ক_ligatures),
					 hb_array (component_list, অঙ্ক_components));
  c.end_serialize ();

  ফেরত ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

স্থির OT::SubstLookup *
arabic_fallback_synthesize_lookup (ধ্রুবক hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   unsigned পূর্ণসংখ্যা feature_সূচক)
{
  যদি (feature_সূচক < 4)
    ফেরত arabic_fallback_synthesize_lookup_single (plan, font, feature_সূচক);
  অন্যথা
  {
    নির্বাচন (feature_সূচক) {
      ক্ষেত্র 4: ফেরত arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      ক্ষেত্র 5: ফেরত arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      ক্ষেত্র 6: ফেরত arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  প্রতিজ্ঞা (মিথ্যা);
  ফেরত nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_দৈর্ঘ্য_CONST (arabic_fallback_features)

কাঠামো arabic_fallback_plan_t
{
  unsigned পূর্ণসংখ্যা অঙ্ক_lookups;
  যৌক্তিক free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ifndef HB_NO_WIN1256
#অন্তর্ভাব "hb-ot-shaper-arabic-win1256.hh"
#endif

কাঠামো ManifestLookup
{
  সর্বজনীন:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};
প্রকারলেখ OT::Array16Of<ManifestLookup> Manifest;

স্থির যৌক্তিক
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   ধ্রুবক hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#ifndef HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  যদি (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    ফেরত মিথ্যা;

  ধ্রুবক Manifest &manifest = reinterpret_cast<ধ্রুবক Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (আকার (arabic_win1256_gsub_lookups.manifest_তথ্য) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * আকার (ManifestLookup), "");

  unsigned j = 0;
  unsigned পূর্ণসংখ্যা গুনতি = manifest.len;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    যদি (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      যদি (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->অঙ্ক_lookups = j;
  fallback_plan->free_lookups = মিথ্যা;

  ফেরত j > 0;
#অন্যথা
  ফেরত মিথ্যা;
#endif
}

স্থির যৌক্তিক
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   ধ্রুবক hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_দৈর্ঘ্য_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  unsigned পূর্ণসংখ্যা j = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    যদি (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      যদি (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->অঙ্ক_lookups = j;
  fallback_plan->free_lookups = সত্য;

  ফেরত j > 0;
}

স্থির arabic_fallback_plan_t *
arabic_fallback_plan_create (ধ্রুবক hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, আকার (arabic_fallback_plan_t));
  যদি (unlikely (!fallback_plan))
    ফেরত const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->অঙ্ক_lookups = 0;
  fallback_plan->free_lookups = মিথ্যা;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  যদি (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    ফেরত fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  যদি (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    ফেরত fallback_plan;

  প্রতিজ্ঞা (fallback_plan->অঙ্ক_lookups == 0);
  hb_free (fallback_plan);
  ফেরত const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

স্থির শূন্য
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  যদি (!fallback_plan || fallback_plan->অঙ্ক_lookups == 0)
    ফেরত;

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < fallback_plan->অঙ্ক_lookups; i++)
    যদি (fallback_plan->lookup_array[i])
    {
      যদি (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      যদি (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

স্থির শূন্য
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_বাফার_t *বাফার)
{
  OT::hb_ot_apply_context_t c (0, font, বাফার, hb_blob_get_empty ());
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < fallback_plan->অঙ্ক_lookups; i++)
    যদি (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      যদি (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

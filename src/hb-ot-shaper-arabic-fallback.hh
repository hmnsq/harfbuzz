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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-ot-shape.hh"
#அந்தர்பாவ "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
நிலையான மாறிலி hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

நிலையான OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (மாறிலி hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  unsigned முழு feature_index)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  unsigned முழு இலக்கம்_glyphs = 0;

  /* Populate arrays */
  க்கு (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_index];
    hb_codepoint_t u_glyph, s_glyph;

    எனில் (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      தொடர்;

    glyphs[இலக்கம்_glyphs] = u_glyph;
    substitutes[இலக்கம்_glyphs] = s_glyph;

    இலக்கம்_glyphs++;
  }

  எனில் (!இலக்கம்_glyphs)
    திரும்பு nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], இலக்கம்_glyphs,
		  (முழு(*)(மாறிலி OT::HBUINT16*, மாறிலி OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  எழுத்து buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, அளவு (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  தருக்க ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, இலக்கம்_glyphs),
				       hb_array (substitutes, இலக்கம்_glyphs));
  c.end_serialize ();

  திரும்பு ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

template <typename T>
நிலையான OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (மாறிலி hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    மாறிலி T &ligature_table,
					    unsigned lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned முழு first_glyphs_indirection[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned முழு ligature_per_first_glyph_மொத்தம்_list[ARRAY_LENGTH_CONST (first_glyphs)];
  unsigned முழு இலக்கம்_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_LENGTH_CONST (first_glyphs) * ARRAY_LENGTH_CONST(ligature_table[0].ligatures)];
  unsigned முழு component_மொத்தம்_list[ARRAY_LENGTH_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_LENGTH_CONST (ligature_list) *
				 ARRAY_LENGTH_CONST (ligature_table[0].ligatures[0].components)];
  unsigned முழு இலக்கம்_ligatures = 0;
  unsigned முழு இலக்கம்_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  க்கு (unsigned முழு first_glyph_idx = 0; first_glyph_idx < ARRAY_LENGTH (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    எனில் (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      தொடர்;
    first_glyphs[இலக்கம்_first_glyphs] = first_glyph;
    ligature_per_first_glyph_மொத்தம்_list[இலக்கம்_first_glyphs] = 0;
    first_glyphs_indirection[இலக்கம்_first_glyphs] = first_glyph_idx;
    இலக்கம்_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], இலக்கம்_first_glyphs,
		  (முழு(*)(மாறிலி OT::HBUINT16*, மாறிலி OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  க்கு (unsigned முழு i = 0; i < இலக்கம்_first_glyphs; i++)
  {
    unsigned முழு first_glyph_idx = first_glyphs_indirection[i];

    க்கு (unsigned முழு ligature_idx = 0; ligature_idx < ARRAY_LENGTH (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      எனில் (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	தொடர்;

      மாறிலி auto &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      unsigned component_மொத்தம் = ARRAY_LENGTH_CONST (components);

      தருக்க matched = உண்மை;
      க்கு (unsigned j = 0; j < component_மொத்தம்; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	எனில் (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = பொய்;
	  நிறுத்து;
	}

	component_list[இலக்கம்_components++] = component_glyph;
      }
      எனில் (!matched)
        தொடர்;

      component_மொத்தம்_list[இலக்கம்_ligatures] = 1 + component_மொத்தம்;
      ligature_list[இலக்கம்_ligatures] = ligature_glyph;

      ligature_per_first_glyph_மொத்தம்_list[i]++;

      இலக்கம்_ligatures++;
    }
  }

  எனில் (!இலக்கம்_ligatures)
    திரும்பு nullptr;


  /* 16 bytes per ligature ought to be enough... */
  எழுத்து buf[ARRAY_LENGTH_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, அளவு (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  தருக்க ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, இலக்கம்_first_glyphs),
					 hb_array (ligature_per_first_glyph_மொத்தம்_list, இலக்கம்_first_glyphs),
					 hb_array (ligature_list, இலக்கம்_ligatures),
					 hb_array (component_மொத்தம்_list, இலக்கம்_ligatures),
					 hb_array (component_list, இலக்கம்_components));
  c.end_serialize ();

  திரும்பு ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

நிலையான OT::SubstLookup *
arabic_fallback_synthesize_lookup (மாறிலி hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   unsigned முழு feature_index)
{
  எனில் (feature_index < 4)
    திரும்பு arabic_fallback_synthesize_lookup_single (plan, font, feature_index);
  இல்லையெனில்
  {
    தேர்வு (feature_index) {
      நிலை 4: திரும்பு arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      நிலை 5: திரும்பு arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      நிலை 6: திரும்பு arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  ப்ரதிஜ்ஞா (பொய்);
  திரும்பு nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_LENGTH_CONST (arabic_fallback_features)

கட்டமைப்பு arabic_fallback_plan_t
{
  unsigned முழு இலக்கம்_lookups;
  தருக்க free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ifndef HB_NO_WIN1256
#அந்தர்பாவ "hb-ot-shaper-arabic-win1256.hh"
#endif

கட்டமைப்பு ManifestLookup
{
  பொது:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  பொது:
  DEFINE_SIZE_STATIC (6);
};
வகைவரை OT::Array16Of<ManifestLookup> Manifest;

நிலையான தருக்க
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   மாறிலி hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#ifndef HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  எனில் (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    திரும்பு பொய்;

  மாறிலி Manifest &manifest = reinterpret_cast<மாறிலி Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (அளவு (arabic_win1256_gsub_lookups.manifestData) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * அளவு (ManifestLookup), "");

  unsigned j = 0;
  unsigned முழு மொத்தம் = manifest.len;
  க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    எனில் (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      எனில் (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->இலக்கம்_lookups = j;
  fallback_plan->free_lookups = பொய்;

  திரும்பு j > 0;
#இல்லையெனில்
  திரும்பு பொய்;
#endif
}

நிலையான தருக்க
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   மாறிலி hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_LENGTH_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  unsigned முழு j = 0;
  க்கு (unsigned முழு i = 0; i < ARRAY_LENGTH(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    எனில் (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      எனில் (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->இலக்கம்_lookups = j;
  fallback_plan->free_lookups = உண்மை;

  திரும்பு j > 0;
}

நிலையான arabic_fallback_plan_t *
arabic_fallback_plan_create (மாறிலி hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, அளவு (arabic_fallback_plan_t));
  எனில் (unlikely (!fallback_plan))
    திரும்பு const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->இலக்கம்_lookups = 0;
  fallback_plan->free_lookups = பொய்;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  எனில் (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    திரும்பு fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  எனில் (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    திரும்பு fallback_plan;

  ப்ரதிஜ்ஞா (fallback_plan->இலக்கம்_lookups == 0);
  hb_free (fallback_plan);
  திரும்பு const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

நிலையான வெறுமை
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  எனில் (!fallback_plan || fallback_plan->இலக்கம்_lookups == 0)
    திரும்பு;

  க்கு (unsigned முழு i = 0; i < fallback_plan->இலக்கம்_lookups; i++)
    எனில் (fallback_plan->lookup_array[i])
    {
      எனில் (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      எனில் (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

நிலையான வெறுமை
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
  OT::hb_ot_apply_context_t c (0, font, buffer, hb_blob_get_empty ());
  க்கு (unsigned முழு i = 0; i < fallback_plan->இலக்கம்_lookups; i++)
    எனில் (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      எனில் (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

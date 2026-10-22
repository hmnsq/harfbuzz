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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-ot-shape.hh"
#અન્તર્ભાવ "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
સ્થિર અચલ hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

સ્થિર OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (અચલ hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  unsigned પૂર્ણાંક feature_index)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  unsigned પૂર્ણાંક અંક_glyphs = 0;

  /* Populate arrays */
  માટે (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_index];
    hb_codepoint_t u_glyph, s_glyph;

    જો (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      ચાલુ;

    glyphs[અંક_glyphs] = u_glyph;
    substitutes[અંક_glyphs] = s_glyph;

    અંક_glyphs++;
  }

  જો (!અંક_glyphs)
    પાછા nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], અંક_glyphs,
		  (પૂર્ણાંક(*)(અચલ OT::HBUINT16*, અચલ OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  અક્ષર buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, કદ (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  તાર્કિક ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, અંક_glyphs),
				       hb_array (substitutes, અંક_glyphs));
  c.end_serialize ();

  પાછા ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

template <typename T>
સ્થિર OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (અચલ hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    અચલ T &ligature_table,
					    unsigned lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned પૂર્ણાંક first_glyphs_indirection[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned પૂર્ણાંક ligature_per_first_glyph_ગણતરી_list[ARRAY_LENGTH_CONST (first_glyphs)];
  unsigned પૂર્ણાંક અંક_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_LENGTH_CONST (first_glyphs) * ARRAY_LENGTH_CONST(ligature_table[0].ligatures)];
  unsigned પૂર્ણાંક component_ગણતરી_list[ARRAY_LENGTH_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_LENGTH_CONST (ligature_list) *
				 ARRAY_LENGTH_CONST (ligature_table[0].ligatures[0].components)];
  unsigned પૂર્ણાંક અંક_ligatures = 0;
  unsigned પૂર્ણાંક અંક_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  માટે (unsigned પૂર્ણાંક first_glyph_idx = 0; first_glyph_idx < ARRAY_LENGTH (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    જો (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      ચાલુ;
    first_glyphs[અંક_first_glyphs] = first_glyph;
    ligature_per_first_glyph_ગણતરી_list[અંક_first_glyphs] = 0;
    first_glyphs_indirection[અંક_first_glyphs] = first_glyph_idx;
    અંક_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], અંક_first_glyphs,
		  (પૂર્ણાંક(*)(અચલ OT::HBUINT16*, અચલ OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  માટે (unsigned પૂર્ણાંક i = 0; i < અંક_first_glyphs; i++)
  {
    unsigned પૂર્ણાંક first_glyph_idx = first_glyphs_indirection[i];

    માટે (unsigned પૂર્ણાંક ligature_idx = 0; ligature_idx < ARRAY_LENGTH (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      જો (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	ચાલુ;

      અચલ auto &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      unsigned component_ગણતરી = ARRAY_LENGTH_CONST (components);

      તાર્કિક matched = સાચું;
      માટે (unsigned j = 0; j < component_ગણતરી; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	જો (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = ખોટું;
	  તોડો;
	}

	component_list[અંક_components++] = component_glyph;
      }
      જો (!matched)
        ચાલુ;

      component_ગણતરી_list[અંક_ligatures] = 1 + component_ગણતરી;
      ligature_list[અંક_ligatures] = ligature_glyph;

      ligature_per_first_glyph_ગણતરી_list[i]++;

      અંક_ligatures++;
    }
  }

  જો (!અંક_ligatures)
    પાછા nullptr;


  /* 16 bytes per ligature ought to be enough... */
  અક્ષર buf[ARRAY_LENGTH_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, કદ (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  તાર્કિક ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, અંક_first_glyphs),
					 hb_array (ligature_per_first_glyph_ગણતરી_list, અંક_first_glyphs),
					 hb_array (ligature_list, અંક_ligatures),
					 hb_array (component_ગણતરી_list, અંક_ligatures),
					 hb_array (component_list, અંક_components));
  c.end_serialize ();

  પાછા ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

સ્થિર OT::SubstLookup *
arabic_fallback_synthesize_lookup (અચલ hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   unsigned પૂર્ણાંક feature_index)
{
  જો (feature_index < 4)
    પાછા arabic_fallback_synthesize_lookup_single (plan, font, feature_index);
  નહીંતર
  {
    પસંદગી (feature_index) {
      સ્થિતિ 4: પાછા arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      સ્થિતિ 5: પાછા arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      સ્થિતિ 6: પાછા arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  પ્રતિજ્ઞા (ખોટું);
  પાછા nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_LENGTH_CONST (arabic_fallback_features)

સંરચના arabic_fallback_plan_t
{
  unsigned પૂર્ણાંક અંક_lookups;
  તાર્કિક free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ifndef HB_NO_WIN1256
#અન્તર્ભાવ "hb-ot-shaper-arabic-win1256.hh"
#endif

સંરચના ManifestLookup
{
  જાહેર:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};
પ્રકારવ્યાખ્યા OT::Array16Of<ManifestLookup> Manifest;

સ્થિર તાર્કિક
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   અચલ hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#ifndef HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  જો (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    પાછા ખોટું;

  અચલ Manifest &manifest = reinterpret_cast<અચલ Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (કદ (arabic_win1256_gsub_lookups.manifestData) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * કદ (ManifestLookup), "");

  unsigned j = 0;
  unsigned પૂર્ણાંક ગણતરી = manifest.len;
  માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    જો (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      જો (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->અંક_lookups = j;
  fallback_plan->free_lookups = ખોટું;

  પાછા j > 0;
#નહીંતર
  પાછા ખોટું;
#endif
}

સ્થિર તાર્કિક
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   અચલ hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_LENGTH_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  unsigned પૂર્ણાંક j = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    જો (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      જો (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->અંક_lookups = j;
  fallback_plan->free_lookups = સાચું;

  પાછા j > 0;
}

સ્થિર arabic_fallback_plan_t *
arabic_fallback_plan_create (અચલ hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, કદ (arabic_fallback_plan_t));
  જો (unlikely (!fallback_plan))
    પાછા const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->અંક_lookups = 0;
  fallback_plan->free_lookups = ખોટું;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  જો (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    પાછા fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  જો (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    પાછા fallback_plan;

  પ્રતિજ્ઞા (fallback_plan->અંક_lookups == 0);
  hb_free (fallback_plan);
  પાછા const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

સ્થિર શૂન્ય
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  જો (!fallback_plan || fallback_plan->અંક_lookups == 0)
    પાછા;

  માટે (unsigned પૂર્ણાંક i = 0; i < fallback_plan->અંક_lookups; i++)
    જો (fallback_plan->lookup_array[i])
    {
      જો (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      જો (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

સ્થિર શૂન્ય
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
  OT::hb_ot_apply_context_t c (0, font, buffer, hb_blob_get_empty ());
  માટે (unsigned પૂર્ણાંક i = 0; i < fallback_plan->અંક_lookups; i++)
    જો (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      જો (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

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

#include "hb.hh"

#include "hb-ot-shape.hh"
#include "hb-ot-layout-gsub-table.hh"


/* Features ordered the same as the entries in shaping_table rows,
 * followed by rlig.  Don't change.
 *
 * We currently support one subtable per lookup, and one lookup
 * per feature.  But we allow duplicate features, so we use that!
 */
static const hb_tag_t arabic_fallback_features[] =
{
  HB_TAG('i','n','i','t'),
  HB_TAG('m','e','d','i'),
  HB_TAG('f','i','n','a'),
  HB_TAG('i','s','o','l'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
  HB_TAG('r','l','i','g'),
};

static OT::SubstLookup *
arabic_fallback_synthesize_lookup_single (const hb_ot_shape_plan_t *plan HB_UNUSED,
					  hb_font_t *font,
					  unsigned puur_naa_nka feature_index)
{
  OT::HBGlyphID16 glyphs[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  OT::HBGlyphID16 substitutes[SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1];
  unsigned puur_naa_nka num_glyphs = 0;

  /* Populate arrays */
  ꦏꦁꦒꦺꦴ (hb_codepoint_t u = SHAPING_TABLE_FIRST; u < SHAPING_TABLE_LAST + 1; u++)
  {
    hb_codepoint_t s = shaping_table[u - SHAPING_TABLE_FIRST][feature_index];
    hb_codepoint_t u_glyph, s_glyph;

    ꦪꦼꦤ꧀ (!s ||
	!hb_font_get_glyph (font, u, 0, &u_glyph) ||
	!hb_font_get_glyph (font, s, 0, &s_glyph) ||
	u_glyph == s_glyph ||
	u_glyph > 0xFFFFu || s_glyph > 0xFFFFu)
      continue;

    glyphs[num_glyphs] = u_glyph;
    substitutes[num_glyphs] = s_glyph;

    num_glyphs++;
  }

  ꦪꦼꦤ꧀ (!num_glyphs)
    ꦮꦭꦶ nullptr;

  /* Bubble-sort or something equally good!
   * May not be good-enough for presidential candidate interviews, but good-enough for us... */
  hb_stable_sort (&glyphs[0], num_glyphs,
		  (puur_naa_nka(*)(const OT::HBUINT16*, const OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &substitutes[0]);


  /* Each glyph takes four bytes max, and there's some overhead. */
  var_na buf[(SHAPING_TABLE_LAST - SHAPING_TABLE_FIRST + 1) * 4 + 128];
  hb_serialize_context_t c (buf, parimaa_na (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  bool ret = lookup->serialize_single (&c,
				       OT::LookupFlag::IgnoreMarks,
				       hb_sorted_array (glyphs, num_glyphs),
				       hb_array (substitutes, num_glyphs));
  c.end_serialize ();

  ꦮꦭꦶ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

template <typename T>
static OT::SubstLookup *
arabic_fallback_synthesize_lookup_ligature (const hb_ot_shape_plan_t *plan HB_UNUSED,
					    hb_font_t *font,
					    const T &ligature_table,
					    unsigned lookup_flags)
{
  OT::HBGlyphID16 first_glyphs[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned puur_naa_nka first_glyphs_indirection[ARRAY_LENGTH_CONST (ligature_table)];
  unsigned puur_naa_nka ligature_per_first_glyph_count_list[ARRAY_LENGTH_CONST (first_glyphs)];
  unsigned puur_naa_nka num_first_glyphs = 0;

  /* We know that all our ligatures have the same number of components. */
  OT::HBGlyphID16 ligature_list[ARRAY_LENGTH_CONST (first_glyphs) * ARRAY_LENGTH_CONST(ligature_table[0].ligatures)];
  unsigned puur_naa_nka component_count_list[ARRAY_LENGTH_CONST (ligature_list)];
  OT::HBGlyphID16 component_list[ARRAY_LENGTH_CONST (ligature_list) *
				 ARRAY_LENGTH_CONST (ligature_table[0].ligatures[0].components)];
  unsigned puur_naa_nka num_ligatures = 0;
  unsigned puur_naa_nka num_components = 0;

  /* Populate arrays */

  /* Sort out the first-glyphs */
  ꦏꦁꦒꦺꦴ (unsigned puur_naa_nka first_glyph_idx = 0; first_glyph_idx < ARRAY_LENGTH (first_glyphs); first_glyph_idx++)
  {
    hb_codepoint_t first_u = ligature_table[first_glyph_idx].first;
    hb_codepoint_t first_glyph;
    ꦪꦼꦤ꧀ (!hb_font_get_glyph (font, first_u, 0, &first_glyph))
      continue;
    first_glyphs[num_first_glyphs] = first_glyph;
    ligature_per_first_glyph_count_list[num_first_glyphs] = 0;
    first_glyphs_indirection[num_first_glyphs] = first_glyph_idx;
    num_first_glyphs++;
  }
  hb_stable_sort (&first_glyphs[0], num_first_glyphs,
		  (puur_naa_nka(*)(const OT::HBUINT16*, const OT::HBUINT16 *)) OT::HBGlyphID16::cmp,
		  &first_glyphs_indirection[0]);

  /* Now that the first-glyphs are sorted, walk again, populate ligatures. */
  ꦏꦁꦒꦺꦴ (unsigned puur_naa_nka i = 0; i < num_first_glyphs; i++)
  {
    unsigned puur_naa_nka first_glyph_idx = first_glyphs_indirection[i];

    ꦏꦁꦒꦺꦴ (unsigned puur_naa_nka ligature_idx = 0; ligature_idx < ARRAY_LENGTH (ligature_table[0].ligatures); ligature_idx++)
    {
      hb_codepoint_t ligature_u = ligature_table[first_glyph_idx].ligatures[ligature_idx].ligature;
      hb_codepoint_t ligature_glyph;
      ꦪꦼꦤ꧀ (!hb_font_get_glyph (font, ligature_u, 0, &ligature_glyph))
	continue;

      const auto &components = ligature_table[first_glyph_idx].ligatures[ligature_idx].components;
      unsigned component_count = ARRAY_LENGTH_CONST (components);

      bool matched = true;
      ꦏꦁꦒꦺꦴ (unsigned j = 0; j < component_count; j++)
      {
	hb_codepoint_t component_u   = ligature_table[first_glyph_idx].ligatures[ligature_idx].components[j];
	hb_codepoint_t component_glyph;
	ꦪꦼꦤ꧀ (!component_u ||
	    !hb_font_get_nominal_glyph (font, component_u, &component_glyph))
	{
	  matched = false;
	  bha_nga;
	}

	component_list[num_components++] = component_glyph;
      }
      ꦪꦼꦤ꧀ (!matched)
        continue;

      component_count_list[num_ligatures] = 1 + component_count;
      ligature_list[num_ligatures] = ligature_glyph;

      ligature_per_first_glyph_count_list[i]++;

      num_ligatures++;
    }
  }

  ꦪꦼꦤ꧀ (!num_ligatures)
    ꦮꦭꦶ nullptr;


  /* 16 bytes per ligature ought to be enough... */
  var_na buf[ARRAY_LENGTH_CONST (ligature_list) * 16 + 128];
  hb_serialize_context_t c (buf, parimaa_na (buf));
  OT::SubstLookup *lookup = c.start_serialize<OT::SubstLookup> ();
  bool ret = lookup->serialize_ligature (&c,
					 lookup_flags,
					 hb_sorted_array (first_glyphs, num_first_glyphs),
					 hb_array (ligature_per_first_glyph_count_list, num_first_glyphs),
					 hb_array (ligature_list, num_ligatures),
					 hb_array (component_count_list, num_ligatures),
					 hb_array (component_list, num_components));
  c.end_serialize ();

  ꦮꦭꦶ ret && !c.in_error () ? c.copy<OT::SubstLookup> () : nullptr;
}

static OT::SubstLookup *
arabic_fallback_synthesize_lookup (const hb_ot_shape_plan_t *plan,
				   hb_font_t *font,
				   unsigned puur_naa_nka feature_index)
{
  ꦪꦼꦤ꧀ (feature_index < 4)
    ꦮꦭꦶ arabic_fallback_synthesize_lookup_single (plan, font, feature_index);
  ꦭꦶꦪ
  {
    switch (feature_index) {
      pak_sa 4: ꦮꦭꦶ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_3_table, OT::LookupFlag::IgnoreMarks);
      pak_sa 5: ꦮꦭꦶ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_table, OT::LookupFlag::IgnoreMarks);
      pak_sa 6: ꦮꦭꦶ arabic_fallback_synthesize_lookup_ligature (plan, font, ligature_mark_table, 0);
    }
  }
  assert (false);
  ꦮꦭꦶ nullptr;
}

#define ARABIC_FALLBACK_MAX_LOOKUPS ARRAY_LENGTH_CONST (arabic_fallback_features)

sa_nracanaa arabic_fallback_plan_t
{
  unsigned puur_naa_nka num_lookups;
  bool free_lookups;

  hb_mask_t mask_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::SubstLookup *lookup_array[ARABIC_FALLBACK_MAX_LOOKUPS];
  OT::hb_ot_layout_lookup_accelerator_t *accel_array[ARABIC_FALLBACK_MAX_LOOKUPS];
};

#ifndef HB_NO_WIN1256
#include "hb-ot-shaper-arabic-win1256.hh"
#endif

sa_nracanaa ManifestLookup
{
  public:
  OT::Tag tag;
  OT::Offset16To<OT::SubstLookup> lookupOffset;
  public:
  DEFINE_SIZE_STATIC (6);
};
typedef OT::Array16Of<ManifestLookup> Manifest;

static bool
arabic_fallback_plan_init_win1256 (arabic_fallback_plan_t *fallback_plan HB_UNUSED,
				   const hb_ot_shape_plan_t *plan HB_UNUSED,
				   hb_font_t *font HB_UNUSED)
{
#ifndef HB_NO_WIN1256
  /* Does this font look like it's Windows-1256-encoded? */
  hb_codepoint_t g;
  ꦪꦼꦤ꧀ (!(hb_font_get_glyph (font, 0x0627u, 0, &g) && g == 199 /* ALEF */ &&
	hb_font_get_glyph (font, 0x0644u, 0, &g) && g == 225 /* LAM */ &&
	hb_font_get_glyph (font, 0x0649u, 0, &g) && g == 236 /* ALEF MAKSURA */ &&
	hb_font_get_glyph (font, 0x064Au, 0, &g) && g == 237 /* YEH */ &&
	hb_font_get_glyph (font, 0x0652u, 0, &g) && g == 250 /* SUKUN */))
    ꦮꦭꦶ false;

  const Manifest &manifest = reinterpret_cast<const Manifest&> (arabic_win1256_gsub_lookups.manifest);
  static_assert (parimaa_na (arabic_win1256_gsub_lookups.manifestData) <=
		 ARABIC_FALLBACK_MAX_LOOKUPS * parimaa_na (ManifestLookup), "");

  unsigned j = 0;
  unsigned puur_naa_nka count = manifest.len;
  ꦏꦁꦒꦺꦴ (unsigned puur_naa_nka i = 0; i < count; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (manifest[i].tag);
    ꦪꦼꦤ꧀ (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = const_cast<OT::SubstLookup*> (&(&manifest+manifest[i].lookupOffset));
      ꦪꦼꦤ꧀ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->num_lookups = j;
  fallback_plan->free_lookups = false;

  ꦮꦭꦶ j > 0;
#ꦭꦶꦪ
  ꦮꦭꦶ false;
#endif
}

static bool
arabic_fallback_plan_init_unicode (arabic_fallback_plan_t *fallback_plan,
				   const hb_ot_shape_plan_t *plan,
				   hb_font_t *font)
{
  static_assert ((ARRAY_LENGTH_CONST (arabic_fallback_features) <= ARABIC_FALLBACK_MAX_LOOKUPS), "");
  unsigned puur_naa_nka j = 0;
  ꦏꦁꦒꦺꦴ (unsigned puur_naa_nka i = 0; i < ARRAY_LENGTH(arabic_fallback_features) ; i++)
  {
    fallback_plan->mask_array[j] = plan->map.get_1_mask (arabic_fallback_features[i]);
    ꦪꦼꦤ꧀ (fallback_plan->mask_array[j])
    {
      fallback_plan->lookup_array[j] = arabic_fallback_synthesize_lookup (plan, font, i);
      ꦪꦼꦤ꧀ (fallback_plan->lookup_array[j])
      {
	fallback_plan->accel_array[j] = OT::hb_ot_layout_lookup_accelerator_t::create (*fallback_plan->lookup_array[j]);
	j++;
      }
    }
  }

  fallback_plan->num_lookups = j;
  fallback_plan->free_lookups = true;

  ꦮꦭꦶ j > 0;
}

static arabic_fallback_plan_t *
arabic_fallback_plan_create (const hb_ot_shape_plan_t *plan,
			     hb_font_t *font)
{
  arabic_fallback_plan_t *fallback_plan = (arabic_fallback_plan_t *) hb_calloc (1, parimaa_na (arabic_fallback_plan_t));
  ꦪꦼꦤ꧀ (unlikely (!fallback_plan))
    ꦮꦭꦶ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));

  fallback_plan->num_lookups = 0;
  fallback_plan->free_lookups = false;

  /* Try synthesizing GSUB table using Unicode Arabic Presentation Forms,
   * in case the font has cmap entries for the presentation-forms characters. */
  ꦪꦼꦤ꧀ (arabic_fallback_plan_init_unicode (fallback_plan, plan, font))
    ꦮꦭꦶ fallback_plan;

  /* See if this looks like a Windows-1256-encoded font.  If it does, use a
   * hand-coded GSUB table. */
  ꦪꦼꦤ꧀ (arabic_fallback_plan_init_win1256 (fallback_plan, plan, font))
    ꦮꦭꦶ fallback_plan;

  assert (fallback_plan->num_lookups == 0);
  hb_free (fallback_plan);
  ꦮꦭꦶ const_cast<arabic_fallback_plan_t *> (&Null (arabic_fallback_plan_t));
}

static void
arabic_fallback_plan_destroy (arabic_fallback_plan_t *fallback_plan)
{
  ꦪꦼꦤ꧀ (!fallback_plan || fallback_plan->num_lookups == 0)
    ꦮꦭꦶ;

  ꦏꦁꦒꦺꦴ (unsigned puur_naa_nka i = 0; i < fallback_plan->num_lookups; i++)
    ꦪꦼꦤ꧀ (fallback_plan->lookup_array[i])
    {
      ꦪꦼꦤ꧀ (fallback_plan->accel_array[i])
	fallback_plan->accel_array[i]->fini ();
      hb_free (fallback_plan->accel_array[i]);
      ꦪꦼꦤ꧀ (fallback_plan->free_lookups)
	hb_free (fallback_plan->lookup_array[i]);
    }

  hb_free (fallback_plan);
}

static void
arabic_fallback_plan_shape (arabic_fallback_plan_t *fallback_plan,
			    hb_font_t *font,
			    hb_buffer_t *buffer)
{
  OT::hb_ot_apply_context_t c (0, font, buffer, hb_blob_get_empty ());
  ꦏꦁꦒꦺꦴ (unsigned puur_naa_nka i = 0; i < fallback_plan->num_lookups; i++)
    ꦪꦼꦤ꧀ (fallback_plan->lookup_array[i]) {
      c.set_lookup_mask (fallback_plan->mask_array[i]);
      ꦪꦼꦤ꧀ (fallback_plan->accel_array[i])
	hb_ot_layout_substitute_lookup (&c,
					*fallback_plan->lookup_array[i],
					*fallback_plan->accel_array[i]);
    }
}


#endif /* HB_OT_SHAPER_ARABIC_FALLBACK_HH */

/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Roderick Sheeter
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-accelerator.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-multimap.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-unicode.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cmap-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-glyf-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-base-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff1-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/COLR/COLR.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/COLR/colrv1-closure.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/CPAL/CPAL.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-fvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-varc-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-stat-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-math-table.hh"

hb_subset_accelerator_t::~hb_subset_accelerator_t ()
{
  ਜੇ (cmap_cache && destroy_cmap_cache)
    destroy_cmap_cache ((ਖਾਲੀ*) cmap_cache);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#ਜੇ_ਅੰਤ
  hb_face_destroy (ਸਰੋਤ);
}


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
ਜੋੜੋ_cff_seac_components (ਅਚਲ OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_ਸੈੱਟ_t *gids_ਮੰਜ਼ਿਲ_retain)
{
  hb_codepoint_t ਅਧਾਰ_gid, accent_gid;
  ਜੇ (cff.get_seac_components (gid, &ਅਧਾਰ_gid, &accent_gid))
  {
    gids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (ਅਧਾਰ_gid);
    gids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (accent_gid);
    ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ
_remap_palette_indexes (ਅਚਲ hb_ਸੈੱਟ_t *palette_indexes,
			hb_map_t       *mapping /*ਬਾਹਰਲਾ*/)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ new_idx = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ palette_ਸੂਚਕਾਂਕ : palette_indexes->iter ())
  {
    ਜੇ (palette_ਸੂਚਕਾਂਕ == 0xFFFF)
    {
      mapping->ਸੈੱਟ (palette_ਸੂਚਕਾਂਕ, palette_ਸੂਚਕਾਂਕ);
      ਜਾਰੀ;
    }
    mapping->ਸੈੱਟ (palette_ਸੂਚਕਾਂਕ, new_idx);
    new_idx++;
  }
}

ਖਾਲੀ
remap_indexes (ਅਚਲ hb_ਸੈੱਟ_t *indexes,
               hb_map_t       *mapping /*ਬਾਹਰਲਾ*/)
{
  ਲਈ (ਸਵੈਚਾਲਿਤ _ : + hb_enumerate (indexes->iter ()))
    mapping->ਸੈੱਟ (_.second, _.ਪਹਿਲਾ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cmap_ਘੇਰਾ (hb_face_t	   *face,
	       ਅਚਲ hb_ਸੈੱਟ_t	   *unicodes,
	       hb_ਸੈੱਟ_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.ਘੇਰਾ_glyphs (unicodes, glyphset);
}

ਸਥਿਰ ਖਾਲੀ colr_ਘੇਰਾ (hb_subset_plan_t* plan,
                           hb_ਸੈੱਟ_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->ਸਰੋਤ);
  ਜੇ (!colr.is_valid ()) ਵਾਪਸ;

  hb_ਸੈੱਟ_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_ਸੈੱਟ_t glyphset_colrv0;
  ਲਈ (hb_codepoint_t gid : *glyphs_colred)
    colr.ਘੇਰਾ_glyphs (gid, &glyphset_colrv0);

  glyphs_colred->union_ (glyphset_colrv0);

  //closure for COLRv1
  hb_ਸੈੱਟ_t variation_indices, delta_ਸੈੱਟ_indices;
  colr.ਘੇਰਾ_for_V1 (glyphs_colred, &layer_indices, &palette_indices, &variation_indices, &delta_ਸੈੱਟ_indices);

  colr.ਘੇਰਾ_V0palette_indices (glyphs_colred, &palette_indices);
  remap_indexes (&layer_indices, &plan->colrv1_layers);
  _remap_palette_indexes (&palette_indices, &plan->colr_palettes);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  /* colr variation indices mapping during planning phase:
   * generate colrv1_variation_idx_delta_map. When delta set index map is not
   * included, it's a mapping from varIdx-> (new varIdx,delta). Otherwise, it's
   * a mapping from old delta set idx-> (new delta set idx, delta). Mapping
   * delta set indices is the same as gid mapping.
   * Besides, we need to generate a delta set idx-> new var_idx map for updating
   * delta set index map if exists. This map will be updated again after
   * instancing. */
  ਜੇ (colr.has_var_store () && variation_indices)
  {
    ਅਚਲ OT::ਵਸਤੂ_Variation_Store &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਗਿਣਤੀ = var_store.get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ ();
    generate_varstore_inner_maps (variation_indices, subtable_ਗਿਣਤੀ, plan->colrv1_varstore_inner_maps);

    ਜੇ (!plan->all_axes_pinned)
      remap_variation_indices (var_store,
                              variation_indices,
                              plan->normalized_coords,
                              ਝੂਠ, /* no need to calculate delta for COLR during planning */
                              plan->all_axes_pinned,
                              plan->colrv1_variation_idx_delta_map);
  }

  /* Sentinel-only maps still need their delta-set indices remapped, even
   * when there are no live variation rows or no variation store. */
  ਜੇ (!plan->all_axes_pinned && colr.has_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map ())
    remap_colrv1_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_indices (colr.get_delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_map (),
                                         delta_ਸੈੱਟ_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
math_ਘੇਰਾ (hb_subset_plan_t *plan,
               hb_ਸੈੱਟ_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->ਸਰੋਤ_ਸਾਰਣੀ<OT::MATH> ();
  ਜੇ (math->has_ਅੰਕੜੇ ())
    math->ਘੇਰਾ_glyphs (glyphset);
  math.destroy ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_remove_invalid_gids (hb_ਸੈੱਟ_t *glyphs,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs)
{
  glyphs->del_range (ਅੰਕ_glyphs, HB_ਸੈੱਟ_ਮੁੱਲ_INVALID);
}

ਸਾਂਚਾ<ਤਾਰਕਿਕ GID_ALWAYS_EXISTS = ਝੂਠ, ਕਿਸਮ_ਨਾਂ I, ਕਿਸਮ_ਨਾਂ F, ਕਿਸਮ_ਨਾਂ G, hb_requires (hb_is_iterator (I))>
ਸਥਿਰ ਖਾਲੀ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_ਮੰਜ਼ਿਲ_gid_for_iterator,
                            G unicode_ਮੰਜ਼ਿਲ_gid_general)
{
  ਲਈ (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_ਮੰਜ਼ਿਲ_gid_for_iterator(cp);
    ਜੇ (!GID_ALWAYS_EXISTS && gid == HB_MAP_ਮੁੱਲ_INVALID)
    {
      DEBUG_ਸੁਨੇਹਾ(SUBSET, ਖਾਲੀ_ਸੂਚਕ, "Drop U+%04X; no gid", cp);
      ਜਾਰੀ;
    }

    plan->codepoint_ਮੰਜ਼ਿਲ_glyph->ਸੈੱਟ (cp, gid);
    plan->unicode_ਮੰਜ਼ਿਲ_new_gid_list.push (hb_pair (cp, gid));
  }
}

ਸਾਂਚਾ<ਤਾਰਕਿਕ GID_ALWAYS_EXISTS = ਝੂਠ, ਕਿਸਮ_ਨਾਂ I, ਕਿਸਮ_ਨਾਂ F, hb_requires (hb_is_iterator (I))>
ਸਥਿਰ ਖਾਲੀ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_ਮੰਜ਼ਿਲ_gid_for_iterator)
{
  _fill_unicode_and_glyph_map(plan, unicode_iterator, unicode_ਮੰਜ਼ਿਲ_gid_for_iterator, unicode_ਮੰਜ਼ਿਲ_gid_for_iterator);
}

/*
 * Finds additional unicode codepoints which are reachable from the input unicode set.
 * Currently this adds in mirrored variants (needed for bidi) of any input unicodes.
 */
ਸਥਿਰ hb_ਸੈੱਟ_t
unicode_ਘੇਰਾ (ਅਚਲ hb_ਸੈੱਟ_t* unicodes, ਤਾਰਕਿਕ bidi_ਘੇਰਾ) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_ਸੈੱਟ_t ਬਾਹਰਲਾ = *unicodes;
  ਜੇ (!bidi_ਘੇਰਾ) ਵਾਪਸ ਬਾਹਰਲਾ;

  ਜੇ (ਬਾਹਰਲਾ.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    ਵਾਪਸ ਬਾਹਰਲਾ;
  }

  ਸਵੈਚਾਲਿਤ unicode_funcs = hb_unicode_funcs_get_default ();
  ਲਈ (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   ਜੇ (unlikely (mirror != cp)) {
     ਬਾਹਰਲਾ.ਜੋੜੋ(mirror);
   }
  }

  ਵਾਪਸ ਬਾਹਰਲਾ;
}

ਸਥਿਰ ਖਾਲੀ
populate_unicodes_ਮੰਜ਼ਿਲ_retain (ਅਚਲ hb_ਸੈੱਟ_t *unicodes_ਵਿੱਚ,
                              ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_ਸੈੱਟ_t unicodes = unicode_ਘੇਰਾ(unicodes_ਵਿੱਚ,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_ਘੇਰਾ));

  OT::cmap_accelerator_t cmap (plan->ਸਰੋਤ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ_ਹੱਦ = plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ();

  ਜੇ (glyphs->is_empty () && unicodes.get_population () < ਮਾਪ_ਹੱਦ)
  {

    ਅਚਲ hb_map_t* unicode_ਮੰਜ਼ਿਲ_gid_2 = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (plan->accelerator)
      unicode_ਮੰਜ਼ਿਲ_gid_2 = &plan->accelerator->unicode_ਮੰਜ਼ਿਲ_gid_2;

    // This is approach to collection is faster, but can only be used  if glyphs
    // are not being explicitly added to the subset and the input unicodes set is
    // not excessively large (eg. an inverted set).
    plan->unicode_ਮੰਜ਼ਿਲ_new_gid_list.alloc (unicodes.get_population ());
    ਜੇ (!unicode_ਮੰਜ਼ਿਲ_gid_2) {
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        hb_codepoint_t gid;
        ਜੇ (!cmap.get_nominal_glyph (cp, &gid)) {
          ਵਾਪਸ HB_MAP_ਮੁੱਲ_INVALID;
        }
        ਵਾਪਸ gid;
      });
    } ਨਹੀਂਤਾਂ {
      // Use in memory unicode to gid map it's faster then looking up from
      // the map. This code is mostly duplicated from above to avoid doing
      // conditionals on the presence of the unicode_to_gid map each
      // iteration.
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        ਵਾਪਸ unicode_ਮੰਜ਼ਿਲ_gid_2->get (cp);
      });
    }
  }
  ਨਹੀਂਤਾਂ
  {
    // This approach is slower, but can handle adding in glyphs to the subset and will match
    // them with cmap entries.

    hb_map_t unicode_glyphid_map_storage;
    hb_ਸੈੱਟ_t cmap_unicodes_storage;
    ਅਚਲ hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    ਅਚਲ hb_ਸੈੱਟ_t* cmap_unicodes = &cmap_unicodes_storage;

    ਜੇ (!plan->accelerator) {
      cmap.collect_mapping (&cmap_unicodes_storage, &unicode_glyphid_map_storage);
      plan->unicode_ਮੰਜ਼ਿਲ_new_gid_list.alloc (hb_min(unicodes.get_population ()
                                                  + glyphs->get_population (),
                                                  cmap_unicodes->get_population ()));
    } ਨਹੀਂਤਾਂ {
      unicode_glyphid_map = &plan->accelerator->unicode_ਮੰਜ਼ਿਲ_gid_2;
      cmap_unicodes = &plan->accelerator->unicodes;
    }

    ਜੇ (plan->accelerator &&
	unicodes.get_population () < cmap_unicodes->get_population () &&
	glyphs->get_population () < cmap_unicodes->get_population ())
    {
      plan->codepoint_ਮੰਜ਼ਿਲ_glyph->alloc (unicodes.get_population () + glyphs->get_population ());

      ਸਵੈਚਾਲਿਤ &gid_ਮੰਜ਼ਿਲ_unicodes = plan->accelerator->gid_ਮੰਜ਼ਿਲ_unicodes;

      ਲਈ (hb_codepoint_t gid : *glyphs)
      {
        ਸਵੈਚਾਲਿਤ unicodes = gid_ਮੰਜ਼ਿਲ_unicodes.get (gid);
        _fill_unicode_and_glyph_map<ਸੱਚ>(plan, unicodes, [&] (hb_codepoint_t cp) {
          ਵਾਪਸ gid;
        },
        [&] (hb_codepoint_t cp) {
          ਵਾਪਸ unicode_glyphid_map->get(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	ਜੇ (plan->codepoint_ਮੰਜ਼ਿਲ_glyph->has (cp))
          ਵਾਪਸ HB_MAP_ਮੁੱਲ_INVALID;

        ਵਾਪਸ unicode_glyphid_map->get(cp);
      },
      [&] (hb_codepoint_t cp) {
          ਵਾਪਸ unicode_glyphid_map->get(cp);
      });

      plan->unicode_ਮੰਜ਼ਿਲ_new_gid_list.qsort ();
    }
    ਨਹੀਂਤਾਂ
    {
      plan->codepoint_ਮੰਜ਼ਿਲ_glyph->alloc (cmap_unicodes->get_population ());
      hb_codepoint_t ਪਹਿਲਾ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID, ਆਖਰੀ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      ਲਈ (; cmap_unicodes->ਅਗਲਾ_range (&ਪਹਿਲਾ, &ਆਖਰੀ); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(ਪਹਿਲਾ, ਆਖਰੀ + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  ਜੇ (!unicodes.has (cp) && !glyphs->has (gid))
	    ਵਾਪਸ HB_MAP_ਮੁੱਲ_INVALID;
          ਵਾਪਸ gid;
        },
        [&] (hb_codepoint_t cp) {
          ਵਾਪਸ unicode_glyphid_map->get(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ();
    hb_codepoint_t ਪਹਿਲਾ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID, ਆਖਰੀ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
    ਲਈ (; glyphs->ਅਗਲਾ_range (&ਪਹਿਲਾ, &ਆਖਰੀ); )
    {
      ਜੇ (ਪਹਿਲਾ >= ਅੰਕ_glyphs)
	ਰੋਕੋ;
      ਜੇ (ਆਖਰੀ >= ਅੰਕ_glyphs)
        ਆਖਰੀ = ਅੰਕ_glyphs - 1;
      plan->_glyphset_gsub.ਜੋੜੋ_range (ਪਹਿਲਾ, ਆਖਰੀ);
    }
  }

  ਸਵੈਚਾਲਿਤ &arr = plan->unicode_ਮੰਜ਼ਿਲ_new_gid_list;
  ਜੇ (arr.ਲੰਮਾਈ)
  {
    plan->unicodes.ਜੋੜੋ_sorted_array (&arr.arrayZ->ਪਹਿਲਾ, arr.ਲੰਮਾਈ, ਆਕਾਰ (*arr.arrayZ));
    plan->_glyphset_gsub.ਜੋੜੋ_array (&arr.arrayZ->second, arr.ਲੰਮਾਈ, ਆਕਾਰ (*arr.arrayZ));
  }

  // Variation selectors don't have glyphs associated with them in the cmap so they will have been filtered out above
  // but should still be retained. Add them back here.

  // However, the min and max codepoints for OS/2 should be calculated without considering variation selectors,
  // so record those first.
  plan->os2_info.min_cmap_codepoint = plan->unicodes.get_min();
  plan->os2_info.max_cmap_codepoint = plan->unicodes.get_max();

  hb_ਸੈੱਟ_t variation_selectors_ਮੰਜ਼ਿਲ_retain;
  cmap.collect_variation_selectors(&variation_selectors_ਮੰਜ਼ਿਲ_retain);
  + variation_selectors_ਮੰਜ਼ਿਲ_retain.iter()
  | hb_filter(unicodes)
  | hb_sink(&plan->unicodes)
  ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
glyf_ਜੋੜੋ_gid_and_children (ਅਚਲ OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_ਸੈੱਟ_t *gids_ਮੰਜ਼ਿਲ_retain,
			    ਪੂਰਨ operation_ਗਿਣਤੀ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ = 0)
{
  /* Check if is already visited */
  ਜੇ (gids_ਮੰਜ਼ਿਲ_retain->has (gid)) ਵਾਪਸ operation_ਗਿਣਤੀ;

  gids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (gid);

  ਜੇ (unlikely (ਡੂੰਘਾਈ++ > HB_MAX_NESTING_LEVEL)) ਵਾਪਸ operation_ਗਿਣਤੀ;
  ਜੇ (unlikely (--operation_ਗਿਣਤੀ < 0)) ਵਾਪਸ operation_ਗਿਣਤੀ;

  ਸਵੈਚਾਲਿਤ glyph = glyf.glyph_for_gid (gid);

  ਲਈ (ਸਵੈਚਾਲਿਤ &ਵਸਤੂ : glyph.get_ਸੰਯੁਕਤ_iterator ())
    operation_ਗਿਣਤੀ =
      glyf_ਜੋੜੋ_gid_and_children (glyf,
				  ਵਸਤੂ.get_gid (glyph.is_extended ()),
				  gids_ਮੰਜ਼ਿਲ_retain,
				  operation_ਗਿਣਤੀ,
				  ਡੂੰਘਾਈ);

  ਵਾਪਸ operation_ਗਿਣਤੀ;
}

ਸਥਿਰ ਖਾਲੀ
nameid_ਘੇਰਾ (hb_subset_plan_t* plan,
		 hb_ਸੈੱਟ_t* drop_tables)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_STYLE
  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_STAT))
    plan->ਸਰੋਤ->ਸਾਰਣੀ.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (!plan->all_axes_pinned)
    plan->ਸਰੋਤ->ਸਾਰਣੀ.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, &plan->name_ids);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_COLOR
  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_CPAL))
    plan->ਸਰੋਤ->ਸਾਰਣੀ.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_LAYOUT
  layout_nameid_ਘੇਰਾ(plan, drop_tables);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ
populate_gids_ਮੰਜ਼ਿਲ_retain (hb_subset_plan_t* plan,
		          hb_ਸੈੱਟ_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->ਸਰੋਤ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t ਢੇਰ_cff (plan->accelerator ? ਖਾਲੀ_ਸੂਚਕ : plan->ਸਰੋਤ);
  ਅਚਲ OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.get () : &ਢੇਰ_cff);
#ਜੇ_ਅੰਤ

  plan->_glyphset_gsub.ਜੋੜੋ (0); // Not-def

  cmap_ਘੇਰਾ (plan->ਸਰੋਤ, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_MATH))
  {
    math_ਘੇਰਾ (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_LAYOUT
  layout_populate_gids_ਮੰਜ਼ਿਲ_retain(plan, drop_tables);
#ਜੇ_ਅੰਤ

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ());

  hb_ਸੈੱਟ_t cur_glyphset = plan->_glyphset_gsub;
  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_COLR))
  {
    colr_ਘੇਰਾ (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
  ਜੇ (!drop_tables->has (OT::VARC::ਸਾਰਣੀ_ਟੈਗ))
  {
    ਜੇ (unlikely (!plan->ਸਰੋਤ->ਸਾਰਣੀ.VARC->ਘੇਰਾ_glyphs (&cur_glyphset)))
    {
      plan->check_success (ਝੂਠ);
      ਵਾਪਸ;
    }
    _remove_invalid_gids (&cur_glyphset, plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ());
  }
#ਜੇ_ਅੰਤ

  plan->_glyphset_varced = cur_glyphset;

  nameid_ਘੇਰਾ (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  ਜੇ (glyf.has_ਅੰਕੜੇ ())
    ਲਈ (hb_codepoint_t gid : cur_glyphset)
      glyf_ਜੋੜੋ_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_ਸੰਯੁਕਤ_OPERATIONS_ਪ੍ਰਤੀ_GLYPH);
  ਨਹੀਂਤਾਂ
    plan->_glyphset.union_ (cur_glyphset);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
  ਜੇ (!plan->accelerator || plan->accelerator->has_seac)
  {
    ਤਾਰਕਿਕ has_seac = ਝੂਠ;
    ਜੇ (cff->is_valid ())
      ਲਈ (hb_codepoint_t gid : cur_glyphset)
	ਜੇ (ਜੋੜੋ_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = ਸੱਚ;
    plan->has_seac = has_seac;
  }
#ਜੇ_ਅੰਤ

  _remove_invalid_gids (&plan->_glyphset, plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ());

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_LAYOUT
  ਜੇ (!drop_tables->has (HB_OT_ਟੈਗ_GDEF))
    collect_layout_variation_indices (plan);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਤਾਰਕਿਕ
_create_glyph_map_gsub (ਅਚਲ hb_ਸੈੱਟ_t* glyph_ਸੈੱਟ_gsub,
                        ਅਚਲ hb_map_t* glyph_map,
                        hb_map_t* ਬਾਹਰਲਾ,
                        hb_ਸਦਿਸ਼_t<hb_codepoint_t>* ਬਾਹਰਲਾ_flat)
{
  ਜੇ (unlikely (glyph_ਸੈੱਟ_gsub->ਵਿੱਚ_error ()))
    ਵਾਪਸ ਝੂਠ;

  hb_codepoint_t max_gid = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
  hb_ਸੈੱਟ_ਪਿਛਲਾ (glyph_ਸੈੱਟ_gsub, &max_gid);
  ਬਿਨਾਨਿਸ਼ਾਨੀ flat_ਮਾਪ = max_gid == HB_ਸੈੱਟ_ਮੁੱਲ_INVALID ? 0 : max_gid + 1;
  ਜੇ (unlikely (!ਬਾਹਰਲਾ_flat->resize_dirty (flat_ਮਾਪ)))
    ਵਾਪਸ ਝੂਠ;
  hb_memset (ਬਾਹਰਲਾ_flat->arrayZ, 0xFF, flat_ਮਾਪ * ਆਕਾਰ (hb_codepoint_t));

  ਬਾਹਰਲਾ->alloc (glyph_ਸੈੱਟ_gsub->get_population ());
  ਲਈ (ਸਵੈਚਾਲਿਤ gid : *glyph_ਸੈੱਟ_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->get (gid);
    ਬਾਹਰਲਾ->ਸੈੱਟ (gid, new_gid);
    ਬਾਹਰਲਾ_flat->arrayZ[gid] = new_gid;
  }
  ਵਾਪਸ !ਬਾਹਰਲਾ->ਵਿੱਚ_error ();
}

ਸਥਿਰ ਤਾਰਕਿਕ
create_old_gid_ਮੰਜ਼ਿਲ_new_gid_map (ਅਚਲ hb_face_t *face,
				ਤਾਰਕਿਕ		 retain_gids,
				ਅਚਲ hb_ਸੈੱਟ_t	*all_gids_ਮੰਜ਼ਿਲ_retain,
                                ਅਚਲ hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /*ਬਾਹਰਲਾ*/
				hb_map_t	*ਉਲਟ_glyph_map, /*ਬਾਹਰਲਾ*/
				hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_pair_t> *new_ਮੰਜ਼ਿਲ_old_gid_list /*ਬਾਹਰਲਾ*/,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	*ਅੰਕ_glyphs /*ਬਾਹਰਲਾ*/)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੱਢੋ = all_gids_ਮੰਜ਼ਿਲ_retain->get_population ();
  ਉਲਟ_glyph_map->alloc (ਕੱਢੋ);
  glyph_map->alloc (ਕੱਢੋ);
  new_ਮੰਜ਼ਿਲ_old_gid_list->alloc (ਕੱਢੋ);

  ਜੇ (*requested_glyph_map)
  {
    hb_ਸੈੱਟ_t new_gids(requested_glyph_map->values());
    ਜੇ (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "The provided custom glyph mapping is not unique.");
      ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (retain_gids)
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      ਵਾਪਸ ਝੂਠ;
    }

    hb_codepoint_t max_glyph = 0;
    hb_ਸੈੱਟ_t ਬਚਿਆ;
    ਲਈ (ਸਵੈਚਾਲਿਤ old_gid : all_gids_ਮੰਜ਼ਿਲ_retain->iter ())
    {
      ਜੇ (old_gid == 0) {
	new_ਮੰਜ਼ਿਲ_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        ਜਾਰੀ;
      }

      hb_codepoint_t* new_gid;
      ਜੇ (!requested_glyph_map->has (old_gid, &new_gid))
      {
        ਬਚਿਆ.ਜੋੜੋ(old_gid);
        ਜਾਰੀ;
      }

      ਜੇ (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_ਮੰਜ਼ਿਲ_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_ਮੰਜ਼ਿਲ_old_gid_list->qsort ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    ਲਈ (ਸਵੈਚਾਲਿਤ old_gid : ਬਚਿਆ)
      new_ਮੰਜ਼ਿਲ_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *ਅੰਕ_glyphs = max_glyph + 1;
  }
  ਨਹੀਂਤਾਂ ਜੇ (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_ਮੰਜ਼ਿਲ_retain), (hb_codepoint_t) 0)
    | hb_sink (new_ਮੰਜ਼ਿਲ_old_gid_list)
    ;
    *ਅੰਕ_glyphs = new_ਮੰਜ਼ਿਲ_old_gid_list->ਲੰਮਾਈ;
  }
  ਨਹੀਂਤਾਂ
  {
    + hb_iter (all_gids_ਮੰਜ਼ਿਲ_retain)
    | hb_map ([] (hb_codepoint_t _) {
		ਵਾਪਸ hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_ਮੰਜ਼ਿਲ_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
    hb_ਸੈੱਟ_ਪਿਛਲਾ (all_gids_ਮੰਜ਼ਿਲ_retain, &max_glyph);

    *ਅੰਕ_glyphs = max_glyph + 1;
  }

  ਉਲਟ_glyph_map->alloc (ਉਲਟ_glyph_map->get_population () + new_ਮੰਜ਼ਿਲ_old_gid_list->ਲੰਮਾਈ);
  + hb_iter (new_ਮੰਜ਼ਿਲ_old_gid_list)
  | hb_sink (ਉਲਟ_glyph_map)
  ;
  glyph_map->alloc (glyph_map->get_population () + new_ਮੰਜ਼ਿਲ_old_gid_list->ਲੰਮਾਈ);
  + hb_iter (new_ਮੰਜ਼ਿਲ_old_gid_list)
  | hb_map (&hb_codepoint_pair_t::reverse)
  | hb_sink (glyph_map)
  ;

  ਵਾਪਸ ਸੱਚ;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    ਅਚਲ hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  successful = ਸੱਚ;
  flags = ਨਿਵੇਸ਼->flags;

  unicode_ਮੰਜ਼ਿਲ_new_gid_list.init ();

  name_ids = *ਨਿਵੇਸ਼->sets.name_ids;
  name_languages = *ਨਿਵੇਸ਼->sets.name_languages;
  layout_features = *ਨਿਵੇਸ਼->sets.layout_features;
  layout_scripts = *ਨਿਵੇਸ਼->sets.layout_scripts;
  glyphs_requested = *ਨਿਵੇਸ਼->sets.glyphs;
  drop_tables = *ਨਿਵੇਸ਼->sets.drop_tables;
  no_subset_tables = *ਨਿਵੇਸ਼->sets.no_subset_tables;
  ਸਰੋਤ = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_ਮੰਜ਼ਿਲ_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  ਉਲਟ_glyph_map = hb_map_create ();

  gsub_ਪਾਓ_catch_all_feature_variation_rec = ਝੂਠ;
  gpos_ਪਾਓ_catch_all_feature_variation_rec = ਝੂਠ;
  gdef_varstore_inner_maps.init ();

  user_axes_location = ਨਿਵੇਸ਼->axes_location;
  all_axes_pinned = ਝੂਠ;
  pinned_at_default = ਸੱਚ;
  has_gdef_varstore = ਝੂਠ;
  has_avar2 = ਝੂਠ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
  ਲਈ (ਸਵੈਚਾਲਿਤ _ : ਨਿਵੇਸ਼->name_ਸਾਰਣੀ_overrides)
  {
    hb_ਬਾਈਟਾਂ_t name_ਬਾਈਟਾਂ = _.second;
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = name_ਬਾਈਟਾਂ.ਲੰਮਾਈ;
    ਅੱਖਰ *name_str = (ਅੱਖਰ *) hb_malloc (len);
    ਜੇ (unlikely (!check_success (name_str)))
      ਰੋਕੋ;

    hb_memcpy (name_str, name_ਬਾਈਟਾਂ.arrayZ, len);
    name_ਸਾਰਣੀ_overrides.ਸੈੱਟ (_.ਪਹਿਲਾ, hb_ਬਾਈਟਾਂ_t (name_str, len));
  }
#ਜੇ_ਅੰਤ

  ਖਾਲੀ* accel = hb_face_get_user_ਅੰਕੜੇ(face, hb_subset_accelerator_t::user_ਅੰਕੜੇ_ਕੁੰਜੀ());

  attach_accelerator_ਅੰਕੜੇ = ਨਿਵੇਸ਼->attach_accelerator_ਅੰਕੜੇ;
  force_long_loca = ਨਿਵੇਸ਼->force_long_loca;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#ਜੇ_ਅੰਤ

  ਜੇ (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  ਜੇ (unlikely (ਵਿੱਚ_error ()))
    ਵਾਪਸ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
  ਜੇ (!ਨਿਵੇਸ਼->sets.drop_tables->has (OT::VARC::ਸਾਰਣੀ_ਟੈਗ) &&
      ਨਿਵੇਸ਼->sets.no_subset_tables->has (OT::VARC::ਸਾਰਣੀ_ਟੈਗ) &&
      !(ਨਿਵੇਸ਼->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->ਸਾਰਣੀ.VARC->has_ਅੰਕੜੇ ())
  {
    check_success (ਝੂਠ);
    ਵਾਪਸ;
  }
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (!check_success (normalize_axes_location (face, ਇਹ)))
      ਵਾਪਸ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
  ਜੇ (!user_axes_location.is_empty () &&
      !ਨਿਵੇਸ਼->sets.drop_tables->has (OT::VARC::ਸਾਰਣੀ_ਟੈਗ) &&
      ਨਿਵੇਸ਼->sets.no_subset_tables->has (OT::VARC::ਸਾਰਣੀ_ਟੈਗ) &&
      face->ਸਾਰਣੀ.VARC->has_ਅੰਕੜੇ ())
  {
    check_success (ਝੂਠ);
    ਵਾਪਸ;
  }
#ਜੇ_ਅੰਤ

  populate_unicodes_ਮੰਜ਼ਿਲ_retain (ਨਿਵੇਸ਼->sets.unicodes, ਨਿਵੇਸ਼->sets.glyphs, ਇਹ);

  populate_gids_ਮੰਜ਼ਿਲ_retain (ਇਹ, ਨਿਵੇਸ਼->sets.drop_tables);
  ਜੇ (unlikely (ਵਿੱਚ_error ()))
    ਵਾਪਸ;

  ਜੇ (!check_success(create_old_gid_ਮੰਜ਼ਿਲ_new_gid_map(
          face,
          ਨਿਵੇਸ਼->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &ਨਿਵੇਸ਼->glyph_map,
          glyph_map,
          ਉਲਟ_glyph_map,
	  &new_ਮੰਜ਼ਿਲ_old_gid_list,
          &ਅੰਕ_output_glyphs_2))) {
    ਵਾਪਸ;
  }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API  
  ਜੇ ((ਨਿਵੇਸ਼->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (ਨਿਵੇਸ਼->flags & HB_SUBSET_FLAGS_RETAIN_ਅੰਕ_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    ਅੰਕ_output_glyphs_2 = ਸਰੋਤ->get_ਅੰਕ_glyphs ();
  }
#ਜੇ_ਅੰਤ

  ਜੇ (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    ਵਾਪਸ;

  // Now that we have old to new gid map update the unicode to new gid list.
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < unicode_ਮੰਜ਼ਿਲ_new_gid_list.ਲੰਮਾਈ; i++)
  {
    // Use raw array access for performance.
    unicode_ਮੰਜ਼ਿਲ_new_gid_list.arrayZ[i].second =
        glyph_map->get(unicode_ਮੰਜ਼ਿਲ_new_gid_list.arrayZ[i].second);
  }

  bounds_ਚੌੜਾਈ_vec.resize_dirty  (ਅੰਕ_output_glyphs_2);
  ਲਈ (ਸਵੈਚਾਲਿਤ &v : bounds_ਚੌੜਾਈ_vec)
    v = 0xFFFFFFFF;
  bounds_ਉਚਾਈ_vec.resize_dirty  (ਅੰਕ_output_glyphs_2);
  ਲਈ (ਸਵੈਚਾਲਿਤ &v : bounds_ਉਚਾਈ_vec)
    v = 0xFFFFFFFF;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_LAYOUT    
  ਜੇ (!drop_tables.has (HB_OT_ਟੈਗ_GDEF))
    remap_ਵਰਤਿਆ_mark_sets (ਇਹ, ਵਰਤਿਆ_mark_sets_map);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਅਧਾਰ
  ਜੇ (!drop_tables.has (HB_OT_ਟੈਗ_ਅਧਾਰ))
    collect_ਅਧਾਰ_variation_indices (ਇਹ);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

  ਜੇ (unlikely (ਵਿੱਚ_error ()))
    ਵਾਪਸ;

#ਜੇ !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (ਇਹ);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (!check_success (get_instance_glyphs_contour_points (ਇਹ)))
      ਵਾਪਸ;
#ਜੇ_ਅੰਤ

  ਜੇ (attach_accelerator_ਅੰਕੜੇ)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (ਸਰੋਤ,
				       *codepoint_ਮੰਜ਼ਿਲ_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.ਵਿੱਚ_error ());
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan-member-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SUBSET_PLAN_MEMBER
}

hb_subset_plan_t::~hb_subset_plan_t()
{
  hb_face_destroy (dest);

  hb_map_destroy (codepoint_ਮੰਜ਼ਿਲ_glyph);
  hb_map_destroy (glyph_map);
  hb_map_destroy (ਉਲਟ_glyph_map);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#ਜੇ_ਅੰਤ
  hb_face_destroy (ਸਰੋਤ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
  ਲਈ (ਸਵੈਚਾਲਿਤ _ : name_ਸਾਰਣੀ_overrides.iter_ref ())
    _.second.fini ();
#ਜੇ_ਅੰਤ

  ਜੇ (inprogress_accelerator)
    hb_subset_accelerator_t::destroy ((ਖਾਲੀ*) inprogress_accelerator);
}


/**
 * hb_subset_plan_create_or_fail:
 * @face: font face to create the plan for.
 * @input: a #hb_subset_input_t input.
 *
 * Computes a plan for subsetting the supplied face according
 * to a provided input. The plan describes
 * which tables and glyphs should be retained.
 *
 * Return value: (transfer full): New subset plan. Destroy with
 * hb_subset_plan_destroy(). If there is a failure creating the plan
 * nullptr will be returned.
 *
 * Since: 4.0.0
 **/
hb_subset_plan_t *
hb_subset_plan_create_or_fail (hb_face_t	 *face,
                               ਅਚਲ hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  hb_subset_plan_t *plan;
  ਜੇ (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, ਨਿਵੇਸ਼))))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (plan->ਵਿੱਚ_error ()))
  {
    hb_subset_plan_destroy (plan);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ plan;
}

/**
 * hb_subset_plan_destroy:
 * @plan: a #hb_subset_plan_t
 *
 * Decreases the reference count on @plan, and if it reaches zero, destroys
 * @plan, freeing all memory.
 *
 * Since: 4.0.0
 **/
ਖਾਲੀ
hb_subset_plan_destroy (hb_subset_plan_t *plan)
{
  ਜੇ (!hb_object_destroy (plan)) ਵਾਪਸ;

  hb_free (plan);
}

/**
 * hb_subset_plan_old_to_new_glyph_mapping:
 * @plan: a subsetting plan.
 *
 * Returns the mapping between glyphs in the original font to glyphs in the
 * subset that will be produced by @plan
 *
 * Return value: (transfer none):
 * A pointer to the #hb_map_t of the mapping.
 *
 * Since: 4.0.0
 **/
hb_map_t *
hb_subset_plan_old_ਮੰਜ਼ਿਲ_new_glyph_mapping (ਅਚਲ hb_subset_plan_t *plan)
{
  ਵਾਪਸ plan->glyph_map;
}

/**
 * hb_subset_plan_new_to_old_glyph_mapping:
 * @plan: a subsetting plan.
 *
 * Returns the mapping between glyphs in the subset that will be produced by
 * @plan and the glyph in the original font.
 *
 * Return value: (transfer none):
 * A pointer to the #hb_map_t of the mapping.
 *
 * Since: 4.0.0
 **/
hb_map_t *
hb_subset_plan_new_ਮੰਜ਼ਿਲ_old_glyph_mapping (ਅਚਲ hb_subset_plan_t *plan)
{
  ਵਾਪਸ plan->ਉਲਟ_glyph_map;
}

/**
 * hb_subset_plan_unicode_to_old_glyph_mapping:
 * @plan: a subsetting plan.
 *
 * Returns the mapping between codepoints in the original font and the
 * associated glyph id in the original font.
 *
 * Return value: (transfer none):
 * A pointer to the #hb_map_t of the mapping.
 *
 * Since: 4.0.0
 **/
hb_map_t *
hb_subset_plan_unicode_ਮੰਜ਼ਿਲ_old_glyph_mapping (ਅਚਲ hb_subset_plan_t *plan)
{
  ਵਾਪਸ plan->codepoint_ਮੰਜ਼ਿਲ_glyph;
}

/**
 * hb_subset_plan_reference: (skip)
 * @plan: a #hb_subset_plan_t object.
 *
 * Increases the reference count on @plan.
 *
 * Return value: @plan.
 *
 * Since: 4.0.0
 **/
hb_subset_plan_t *
hb_subset_plan_reference (hb_subset_plan_t *plan)
{
  ਵਾਪਸ hb_object_reference (plan);
}

/**
 * hb_subset_plan_set_user_data: (skip)
 * @plan: a #hb_subset_plan_t object.
 * @key: The user-data key to set
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the given subset plan object.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 4.0.0
 **/
hb_bool_t
hb_subset_plan_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_subset_plan_t   *plan,
                              hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
                              ਖਾਲੀ               *ਅੰਕੜੇ,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (plan, key, ਅੰਕੜੇ, destroy, replace);
}

/**
 * hb_subset_plan_get_user_data: (skip)
 * @plan: a #hb_subset_plan_t object.
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified subset plan object.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 4.0.0
 **/
ਖਾਲੀ *
hb_subset_plan_get_user_ਅੰਕੜੇ (ਅਚਲ hb_subset_plan_t *plan,
                              hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t     *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (plan, key);
}

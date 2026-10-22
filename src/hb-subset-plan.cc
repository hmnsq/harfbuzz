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

#అన్తర్భావ "hb-subset-plan.hh"
#అన్తర్భావ "hb-subset-accelerator.hh"
#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-multimap.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-subset.h"
#అన్తర్భావ "hb-unicode.h"

#అన్తర్భావ "hb-ot-cmap-table.hh"
#అన్తర్భావ "hb-ot-glyf-table.hh"
#అన్తర్భావ "hb-ot-layout-base-table.hh"
#అన్తర్భావ "hb-ot-cff1-table.hh"
#అన్తర్భావ "hb-ot-cff2-table.hh"
#అన్తర్భావ "OT/Color/COLR/COLR.hh"
#అన్తర్భావ "OT/Color/COLR/colrv1-closure.hh"
#అన్తర్భావ "OT/Color/CPAL/CPAL.hh"
#అన్తర్భావ "hb-ot-var-fvar-table.hh"
#అన్తర్భావ "hb-ot-var-varc-table.hh"
#అన్తర్భావ "hb-ot-stat-table.hh"
#అన్తర్భావ "hb-ot-math-table.hh"

hb_subset_accelerator_t::~hb_subset_accelerator_t ()
{
  ఒకవేళ (cmap_cache && destroy_cmap_cache)
    destroy_cmap_cache ((శూన్య*) cmap_cache);

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#అయితే_ముగింపు
  hb_face_destroy (ఆధారం);
}


#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
స్థిర అంతఃవరుస తార్కిక
కూడు_cff_seac_components (స్థిరరాశి OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_సమితి_t *gids_గమ్యం_retain)
{
  hb_codepoint_t పునాది_gid, accent_gid;
  ఒకవేళ (cff.get_seac_components (gid, &పునాది_gid, &accent_gid))
  {
    gids_గమ్యం_retain->కూడు (పునాది_gid);
    gids_గమ్యం_retain->కూడు (accent_gid);
    తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}
#అయితే_ముగింపు

స్థిర శూన్య
_remap_palette_indexes (స్థిరరాశి hb_సమితి_t *palette_indexes,
			hb_map_t       *mapping /*బహిర్గతం*/)
{
  గుర్తులేని new_idx = 0;
  కొరకు (గుర్తులేని palette_సూచీ : palette_indexes->iter ())
  {
    ఒకవేళ (palette_సూచీ == 0xFFFF)
    {
      mapping->సమితి (palette_సూచీ, palette_సూచీ);
      కొనసాగు;
    }
    mapping->సమితి (palette_సూచీ, new_idx);
    new_idx++;
  }
}

శూన్య
remap_indexes (స్థిరరాశి hb_సమితి_t *indexes,
               hb_map_t       *mapping /*బహిర్గతం*/)
{
  కొరకు (స్వయంచాలక _ : + hb_enumerate (indexes->iter ()))
    mapping->సమితి (_.second, _.మొదటి);
}

స్థిర అంతఃవరుస శూన్య
cmap_సంవృతం (hb_face_t	   *face,
	       స్థిరరాశి hb_సమితి_t	   *unicodes,
	       hb_సమితి_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.సంవృతం_glyphs (unicodes, glyphset);
}

స్థిర శూన్య colr_సంవృతం (hb_subset_plan_t* plan,
                           hb_సమితి_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->ఆధారం);
  ఒకవేళ (!colr.is_valid ()) తిరిగి;

  hb_సమితి_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_సమితి_t glyphset_colrv0;
  కొరకు (hb_codepoint_t gid : *glyphs_colred)
    colr.సంవృతం_glyphs (gid, &glyphset_colrv0);

  glyphs_colred->union_ (glyphset_colrv0);

  //closure for COLRv1
  hb_సమితి_t variation_indices, delta_సమితి_indices;
  colr.సంవృతం_for_V1 (glyphs_colred, &layer_indices, &palette_indices, &variation_indices, &delta_సమితి_indices);

  colr.సంవృతం_V0palette_indices (glyphs_colred, &palette_indices);
  remap_indexes (&layer_indices, &plan->colrv1_layers);
  _remap_palette_indexes (&palette_indices, &plan->colr_palettes);

#నిర్వచితం_కాకపోతే HB_NO_VAR
  /* colr variation indices mapping during planning phase:
   * generate colrv1_variation_idx_delta_map. When delta set index map is not
   * included, it's a mapping from varIdx-> (new varIdx,delta). Otherwise, it's
   * a mapping from old delta set idx-> (new delta set idx, delta). Mapping
   * delta set indices is the same as gid mapping.
   * Besides, we need to generate a delta set idx-> new var_idx map for updating
   * delta set index map if exists. This map will be updated again after
   * instancing. */
  ఒకవేళ (colr.has_var_store () && variation_indices)
  {
    స్థిరరాశి OT::అంశం_Variation_Store &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    గుర్తులేని subtable_లెక్క = var_store.get_sub_పట్టిక_లెక్క ();
    generate_varstore_inner_maps (variation_indices, subtable_లెక్క, plan->colrv1_varstore_inner_maps);

    ఒకవేళ (!plan->all_axes_pinned)
      remap_variation_indices (var_store,
                              variation_indices,
                              plan->normalized_coords,
                              అబద్ధం, /* no need to calculate delta for COLR during planning */
                              plan->all_axes_pinned,
                              plan->colrv1_variation_idx_delta_map);
  }

  /* Sentinel-only maps still need their delta-set indices remapped, even
   * when there are no live variation rows or no variation store. */
  ఒకవేళ (!plan->all_axes_pinned && colr.has_delta_సమితి_సూచీ_map ())
    remap_colrv1_delta_సమితి_సూచీ_indices (colr.get_delta_సమితి_సూచీ_map (),
                                         delta_సమితి_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస శూన్య
math_సంవృతం (hb_subset_plan_t *plan,
               hb_సమితి_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->ఆధారం_పట్టిక<OT::MATH> ();
  ఒకవేళ (math->has_దత్తాంశం ())
    math->సంవృతం_glyphs (glyphset);
  math.destroy ();
}

స్థిర అంతఃవరుస శూన్య
_remove_invalid_gids (hb_సమితి_t *glyphs,
		      గుర్తులేని పూర్ణాంకం అంకె_glyphs)
{
  glyphs->del_range (అంకె_glyphs, HB_సమితి_విలువ_INVALID);
}

మూస<తార్కిక GID_ALWAYS_EXISTS = అబద్ధం, రకం_పేరు I, రకం_పేరు F, రకం_పేరు G, hb_requires (hb_is_iterator (I))>
స్థిర శూన్య
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_గమ్యం_gid_for_iterator,
                            G unicode_గమ్యం_gid_general)
{
  కొరకు (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_గమ్యం_gid_for_iterator(cp);
    ఒకవేళ (!GID_ALWAYS_EXISTS && gid == HB_MAP_విలువ_INVALID)
    {
      DEBUG_సందేశం(SUBSET, శూన్యసూచిక, "Drop U+%04X; no gid", cp);
      కొనసాగు;
    }

    plan->codepoint_గమ్యం_glyph->సమితి (cp, gid);
    plan->unicode_గమ్యం_new_gid_list.push (hb_pair (cp, gid));
  }
}

మూస<తార్కిక GID_ALWAYS_EXISTS = అబద్ధం, రకం_పేరు I, రకం_పేరు F, hb_requires (hb_is_iterator (I))>
స్థిర శూన్య
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_గమ్యం_gid_for_iterator)
{
  _fill_unicode_and_glyph_map(plan, unicode_iterator, unicode_గమ్యం_gid_for_iterator, unicode_గమ్యం_gid_for_iterator);
}

/*
 * Finds additional unicode codepoints which are reachable from the input unicode set.
 * Currently this adds in mirrored variants (needed for bidi) of any input unicodes.
 */
స్థిర hb_సమితి_t
unicode_సంవృతం (స్థిరరాశి hb_సమితి_t* unicodes, తార్కిక bidi_సంవృతం) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_సమితి_t బహిర్గతం = *unicodes;
  ఒకవేళ (!bidi_సంవృతం) తిరిగి బహిర్గతం;

  ఒకవేళ (బహిర్గతం.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    తిరిగి బహిర్గతం;
  }

  స్వయంచాలక unicode_funcs = hb_unicode_funcs_get_default ();
  కొరకు (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   ఒకవేళ (unlikely (mirror != cp)) {
     బహిర్గతం.కూడు(mirror);
   }
  }

  తిరిగి బహిర్గతం;
}

స్థిర శూన్య
populate_unicodes_గమ్యం_retain (స్థిరరాశి hb_సమితి_t *unicodes_లోపల,
                              స్థిరరాశి hb_సమితి_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_సమితి_t unicodes = unicode_సంవృతం(unicodes_లోపల,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_సంవృతం));

  OT::cmap_accelerator_t cmap (plan->ఆధారం);
  గుర్తులేని కొలత_పరిమితి = plan->ఆధారం->get_అంకె_glyphs ();

  ఒకవేళ (glyphs->is_empty () && unicodes.get_population () < కొలత_పరిమితి)
  {

    స్థిరరాశి hb_map_t* unicode_గమ్యం_gid_2 = శూన్యసూచిక;
    ఒకవేళ (plan->accelerator)
      unicode_గమ్యం_gid_2 = &plan->accelerator->unicode_గమ్యం_gid_2;

    // This is approach to collection is faster, but can only be used  if glyphs
    // are not being explicitly added to the subset and the input unicodes set is
    // not excessively large (eg. an inverted set).
    plan->unicode_గమ్యం_new_gid_list.alloc (unicodes.get_population ());
    ఒకవేళ (!unicode_గమ్యం_gid_2) {
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        hb_codepoint_t gid;
        ఒకవేళ (!cmap.get_nominal_glyph (cp, &gid)) {
          తిరిగి HB_MAP_విలువ_INVALID;
        }
        తిరిగి gid;
      });
    } లేకపోతే {
      // Use in memory unicode to gid map it's faster then looking up from
      // the map. This code is mostly duplicated from above to avoid doing
      // conditionals on the presence of the unicode_to_gid map each
      // iteration.
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        తిరిగి unicode_గమ్యం_gid_2->get (cp);
      });
    }
  }
  లేకపోతే
  {
    // This approach is slower, but can handle adding in glyphs to the subset and will match
    // them with cmap entries.

    hb_map_t unicode_glyphid_map_storage;
    hb_సమితి_t cmap_unicodes_storage;
    స్థిరరాశి hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    స్థిరరాశి hb_సమితి_t* cmap_unicodes = &cmap_unicodes_storage;

    ఒకవేళ (!plan->accelerator) {
      cmap.collect_mapping (&cmap_unicodes_storage, &unicode_glyphid_map_storage);
      plan->unicode_గమ్యం_new_gid_list.alloc (hb_min(unicodes.get_population ()
                                                  + glyphs->get_population (),
                                                  cmap_unicodes->get_population ()));
    } లేకపోతే {
      unicode_glyphid_map = &plan->accelerator->unicode_గమ్యం_gid_2;
      cmap_unicodes = &plan->accelerator->unicodes;
    }

    ఒకవేళ (plan->accelerator &&
	unicodes.get_population () < cmap_unicodes->get_population () &&
	glyphs->get_population () < cmap_unicodes->get_population ())
    {
      plan->codepoint_గమ్యం_glyph->alloc (unicodes.get_population () + glyphs->get_population ());

      స్వయంచాలక &gid_గమ్యం_unicodes = plan->accelerator->gid_గమ్యం_unicodes;

      కొరకు (hb_codepoint_t gid : *glyphs)
      {
        స్వయంచాలక unicodes = gid_గమ్యం_unicodes.get (gid);
        _fill_unicode_and_glyph_map<నిజం>(plan, unicodes, [&] (hb_codepoint_t cp) {
          తిరిగి gid;
        },
        [&] (hb_codepoint_t cp) {
          తిరిగి unicode_glyphid_map->get(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	ఒకవేళ (plan->codepoint_గమ్యం_glyph->has (cp))
          తిరిగి HB_MAP_విలువ_INVALID;

        తిరిగి unicode_glyphid_map->get(cp);
      },
      [&] (hb_codepoint_t cp) {
          తిరిగి unicode_glyphid_map->get(cp);
      });

      plan->unicode_గమ్యం_new_gid_list.qsort ();
    }
    లేకపోతే
    {
      plan->codepoint_గమ్యం_glyph->alloc (cmap_unicodes->get_population ());
      hb_codepoint_t మొదటి = HB_సమితి_విలువ_INVALID, చివరి = HB_సమితి_విలువ_INVALID;
      కొరకు (; cmap_unicodes->తదుపరి_range (&మొదటి, &చివరి); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(మొదటి, చివరి + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  ఒకవేళ (!unicodes.has (cp) && !glyphs->has (gid))
	    తిరిగి HB_MAP_విలువ_INVALID;
          తిరిగి gid;
        },
        [&] (hb_codepoint_t cp) {
          తిరిగి unicode_glyphid_map->get(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    గుర్తులేని అంకె_glyphs = plan->ఆధారం->get_అంకె_glyphs ();
    hb_codepoint_t మొదటి = HB_సమితి_విలువ_INVALID, చివరి = HB_సమితి_విలువ_INVALID;
    కొరకు (; glyphs->తదుపరి_range (&మొదటి, &చివరి); )
    {
      ఒకవేళ (మొదటి >= అంకె_glyphs)
	ఆపు;
      ఒకవేళ (చివరి >= అంకె_glyphs)
        చివరి = అంకె_glyphs - 1;
      plan->_glyphset_gsub.కూడు_range (మొదటి, చివరి);
    }
  }

  స్వయంచాలక &arr = plan->unicode_గమ్యం_new_gid_list;
  ఒకవేళ (arr.నిడివి)
  {
    plan->unicodes.కూడు_sorted_array (&arr.arrayZ->మొదటి, arr.నిడివి, పరిమాణం (*arr.arrayZ));
    plan->_glyphset_gsub.కూడు_array (&arr.arrayZ->second, arr.నిడివి, పరిమాణం (*arr.arrayZ));
  }

  // Variation selectors don't have glyphs associated with them in the cmap so they will have been filtered out above
  // but should still be retained. Add them back here.

  // However, the min and max codepoints for OS/2 should be calculated without considering variation selectors,
  // so record those first.
  plan->os2_info.min_cmap_codepoint = plan->unicodes.get_min();
  plan->os2_info.max_cmap_codepoint = plan->unicodes.get_max();

  hb_సమితి_t variation_selectors_గమ్యం_retain;
  cmap.collect_variation_selectors(&variation_selectors_గమ్యం_retain);
  + variation_selectors_గమ్యం_retain.iter()
  | hb_filter(unicodes)
  | hb_sink(&plan->unicodes)
  ;
}

స్థిర గుర్తులేని
glyf_కూడు_gid_and_children (స్థిరరాశి OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_సమితి_t *gids_గమ్యం_retain,
			    పూర్ణాంకం operation_లెక్క,
			    గుర్తులేని లోతు = 0)
{
  /* Check if is already visited */
  ఒకవేళ (gids_గమ్యం_retain->has (gid)) తిరిగి operation_లెక్క;

  gids_గమ్యం_retain->కూడు (gid);

  ఒకవేళ (unlikely (లోతు++ > HB_MAX_NESTING_LEVEL)) తిరిగి operation_లెక్క;
  ఒకవేళ (unlikely (--operation_లెక్క < 0)) తిరిగి operation_లెక్క;

  స్వయంచాలక glyph = glyf.glyph_for_gid (gid);

  కొరకు (స్వయంచాలక &అంశం : glyph.get_సంయుక్త_iterator ())
    operation_లెక్క =
      glyf_కూడు_gid_and_children (glyf,
				  అంశం.get_gid (glyph.is_extended ()),
				  gids_గమ్యం_retain,
				  operation_లెక్క,
				  లోతు);

  తిరిగి operation_లెక్క;
}

స్థిర శూన్య
nameid_సంవృతం (hb_subset_plan_t* plan,
		 hb_సమితి_t* drop_tables)
{
#నిర్వచితం_కాకపోతే HB_NO_STYLE
  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_STAT))
    plan->ఆధారం->పట్టిక.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (!plan->all_axes_pinned)
    plan->ఆధారం->పట్టిక.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_సూచీ_ట్యాగ్_map, &plan->name_ids);
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_COLOR
  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_CPAL))
    plan->ఆధారం->పట్టిక.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_LAYOUT
  layout_nameid_సంవృతం(plan, drop_tables);
#అయితే_ముగింపు
}

స్థిర శూన్య
populate_gids_గమ్యం_retain (hb_subset_plan_t* plan,
		          hb_సమితి_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->ఆధారం);
#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t దొంతర_cff (plan->accelerator ? శూన్యసూచిక : plan->ఆధారం);
  స్థిరరాశి OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.get () : &దొంతర_cff);
#అయితే_ముగింపు

  plan->_glyphset_gsub.కూడు (0); // Not-def

  cmap_సంవృతం (plan->ఆధారం, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_MATH))
  {
    math_సంవృతం (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->ఆధారం->get_అంకె_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_LAYOUT
  layout_populate_gids_గమ్యం_retain(plan, drop_tables);
#అయితే_ముగింపు

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->ఆధారం->get_అంకె_glyphs ());

  hb_సమితి_t cur_glyphset = plan->_glyphset_gsub;
  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_COLR))
  {
    colr_సంవృతం (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->ఆధారం->get_అంకె_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
  ఒకవేళ (!drop_tables->has (OT::VARC::పట్టిక_ట్యాగ్))
  {
    ఒకవేళ (unlikely (!plan->ఆధారం->పట్టిక.VARC->సంవృతం_glyphs (&cur_glyphset)))
    {
      plan->check_success (అబద్ధం);
      తిరిగి;
    }
    _remove_invalid_gids (&cur_glyphset, plan->ఆధారం->get_అంకె_glyphs ());
  }
#అయితే_ముగింపు

  plan->_glyphset_varced = cur_glyphset;

  nameid_సంవృతం (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  ఒకవేళ (glyf.has_దత్తాంశం ())
    కొరకు (hb_codepoint_t gid : cur_glyphset)
      glyf_కూడు_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_సంయుక్త_OPERATIONS_చొప్పున_GLYPH);
  లేకపోతే
    plan->_glyphset.union_ (cur_glyphset);
#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
  ఒకవేళ (!plan->accelerator || plan->accelerator->has_seac)
  {
    తార్కిక has_seac = అబద్ధం;
    ఒకవేళ (cff->is_valid ())
      కొరకు (hb_codepoint_t gid : cur_glyphset)
	ఒకవేళ (కూడు_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = నిజం;
    plan->has_seac = has_seac;
  }
#అయితే_ముగింపు

  _remove_invalid_gids (&plan->_glyphset, plan->ఆధారం->get_అంకె_glyphs ());

#నిర్వచితం_కాకపోతే HB_NO_VAR
#నిర్వచితం_కాకపోతే HB_NO_SUBSET_LAYOUT
  ఒకవేళ (!drop_tables->has (HB_OT_ట్యాగ్_GDEF))
    collect_layout_variation_indices (plan);
#అయితే_ముగింపు
#అయితే_ముగింపు
}

స్థిర తార్కిక
_create_glyph_map_gsub (స్థిరరాశి hb_సమితి_t* glyph_సమితి_gsub,
                        స్థిరరాశి hb_map_t* glyph_map,
                        hb_map_t* బహిర్గతం,
                        hb_సదిశ_t<hb_codepoint_t>* బహిర్గతం_flat)
{
  ఒకవేళ (unlikely (glyph_సమితి_gsub->లోపల_error ()))
    తిరిగి అబద్ధం;

  hb_codepoint_t max_gid = HB_సమితి_విలువ_INVALID;
  hb_సమితి_మునుపటి (glyph_సమితి_gsub, &max_gid);
  గుర్తులేని flat_కొలత = max_gid == HB_సమితి_విలువ_INVALID ? 0 : max_gid + 1;
  ఒకవేళ (unlikely (!బహిర్గతం_flat->resize_dirty (flat_కొలత)))
    తిరిగి అబద్ధం;
  hb_memset (బహిర్గతం_flat->arrayZ, 0xFF, flat_కొలత * పరిమాణం (hb_codepoint_t));

  బహిర్గతం->alloc (glyph_సమితి_gsub->get_population ());
  కొరకు (స్వయంచాలక gid : *glyph_సమితి_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->get (gid);
    బహిర్గతం->సమితి (gid, new_gid);
    బహిర్గతం_flat->arrayZ[gid] = new_gid;
  }
  తిరిగి !బహిర్గతం->లోపల_error ();
}

స్థిర తార్కిక
create_old_gid_గమ్యం_new_gid_map (స్థిరరాశి hb_face_t *face,
				తార్కిక		 retain_gids,
				స్థిరరాశి hb_సమితి_t	*all_gids_గమ్యం_retain,
                                స్థిరరాశి hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /*బహిర్గతం*/
				hb_map_t	*తిరగేయి_glyph_map, /*బహిర్గతం*/
				hb_sorted_సదిశ_t<hb_codepoint_pair_t> *new_గమ్యం_old_gid_list /*బహిర్గతం*/,
				గుర్తులేని పూర్ణాంకం	*అంకె_glyphs /*బహిర్గతం*/)
{
  గుర్తులేని తీయి = all_gids_గమ్యం_retain->get_population ();
  తిరగేయి_glyph_map->alloc (తీయి);
  glyph_map->alloc (తీయి);
  new_గమ్యం_old_gid_list->alloc (తీయి);

  ఒకవేళ (*requested_glyph_map)
  {
    hb_సమితి_t new_gids(requested_glyph_map->values());
    ఒకవేళ (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "The provided custom glyph mapping is not unique.");
      తిరిగి అబద్ధం;
    }

    ఒకవేళ (retain_gids)
    {
      DEBUG_సందేశం (SUBSET, శూన్యసూచిక,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      తిరిగి అబద్ధం;
    }

    hb_codepoint_t max_glyph = 0;
    hb_సమితి_t మిగిలిన;
    కొరకు (స్వయంచాలక old_gid : all_gids_గమ్యం_retain->iter ())
    {
      ఒకవేళ (old_gid == 0) {
	new_గమ్యం_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        కొనసాగు;
      }

      hb_codepoint_t* new_gid;
      ఒకవేళ (!requested_glyph_map->has (old_gid, &new_gid))
      {
        మిగిలిన.కూడు(old_gid);
        కొనసాగు;
      }

      ఒకవేళ (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_గమ్యం_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_గమ్యం_old_gid_list->qsort ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    కొరకు (స్వయంచాలక old_gid : మిగిలిన)
      new_గమ్యం_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *అంకె_glyphs = max_glyph + 1;
  }
  లేకపోతే ఒకవేళ (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_గమ్యం_retain), (hb_codepoint_t) 0)
    | hb_sink (new_గమ్యం_old_gid_list)
    ;
    *అంకె_glyphs = new_గమ్యం_old_gid_list->నిడివి;
  }
  లేకపోతే
  {
    + hb_iter (all_gids_గమ్యం_retain)
    | hb_map ([] (hb_codepoint_t _) {
		తిరిగి hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_గమ్యం_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_సమితి_విలువ_INVALID;
    hb_సమితి_మునుపటి (all_gids_గమ్యం_retain, &max_glyph);

    *అంకె_glyphs = max_glyph + 1;
  }

  తిరగేయి_glyph_map->alloc (తిరగేయి_glyph_map->get_population () + new_గమ్యం_old_gid_list->నిడివి);
  + hb_iter (new_గమ్యం_old_gid_list)
  | hb_sink (తిరగేయి_glyph_map)
  ;
  glyph_map->alloc (glyph_map->get_population () + new_గమ్యం_old_gid_list->నిడివి);
  + hb_iter (new_గమ్యం_old_gid_list)
  | hb_map (&hb_codepoint_pair_t::reverse)
  | hb_sink (glyph_map)
  ;

  తిరిగి నిజం;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    స్థిరరాశి hb_subset_ఆదానం_t *ఆదానం)
{
  successful = నిజం;
  flags = ఆదానం->flags;

  unicode_గమ్యం_new_gid_list.init ();

  name_ids = *ఆదానం->sets.name_ids;
  name_languages = *ఆదానం->sets.name_languages;
  layout_features = *ఆదానం->sets.layout_features;
  layout_scripts = *ఆదానం->sets.layout_scripts;
  glyphs_requested = *ఆదానం->sets.glyphs;
  drop_tables = *ఆదానం->sets.drop_tables;
  no_subset_tables = *ఆదానం->sets.no_subset_tables;
  ఆధారం = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_గమ్యం_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  తిరగేయి_glyph_map = hb_map_create ();

  gsub_చొప్పించు_catch_all_feature_variation_rec = అబద్ధం;
  gpos_చొప్పించు_catch_all_feature_variation_rec = అబద్ధం;
  gdef_varstore_inner_maps.init ();

  user_axes_location = ఆదానం->axes_location;
  all_axes_pinned = అబద్ధం;
  pinned_at_default = నిజం;
  has_gdef_varstore = అబద్ధం;
  has_avar2 = అబద్ధం;

#నిర్వచితమైతే HB_EXPERIMENTAL_API
  కొరకు (స్వయంచాలక _ : ఆదానం->name_పట్టిక_overrides)
  {
    hb_బైట్లు_t name_బైట్లు = _.second;
    గుర్తులేని len = name_బైట్లు.నిడివి;
    అక్షరం *name_str = (అక్షరం *) hb_malloc (len);
    ఒకవేళ (unlikely (!check_success (name_str)))
      ఆపు;

    hb_memcpy (name_str, name_బైట్లు.arrayZ, len);
    name_పట్టిక_overrides.సమితి (_.మొదటి, hb_బైట్లు_t (name_str, len));
  }
#అయితే_ముగింపు

  శూన్య* accel = hb_face_get_user_దత్తాంశం(face, hb_subset_accelerator_t::user_దత్తాంశం_కీలకం());

  attach_accelerator_దత్తాంశం = ఆదానం->attach_accelerator_దత్తాంశం;
  force_long_loca = ఆదానం->force_long_loca;
#నిర్వచితమైతే HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#అయితే_ముగింపు

  ఒకవేళ (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  ఒకవేళ (unlikely (లోపల_error ()))
    తిరిగి;

#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
  ఒకవేళ (!ఆదానం->sets.drop_tables->has (OT::VARC::పట్టిక_ట్యాగ్) &&
      ఆదానం->sets.no_subset_tables->has (OT::VARC::పట్టిక_ట్యాగ్) &&
      !(ఆదానం->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->పట్టిక.VARC->has_దత్తాంశం ())
  {
    check_success (అబద్ధం);
    తిరిగి;
  }
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (!check_success (normalize_axes_location (face, ఇది)))
      తిరిగి;
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
  ఒకవేళ (!user_axes_location.is_empty () &&
      !ఆదానం->sets.drop_tables->has (OT::VARC::పట్టిక_ట్యాగ్) &&
      ఆదానం->sets.no_subset_tables->has (OT::VARC::పట్టిక_ట్యాగ్) &&
      face->పట్టిక.VARC->has_దత్తాంశం ())
  {
    check_success (అబద్ధం);
    తిరిగి;
  }
#అయితే_ముగింపు

  populate_unicodes_గమ్యం_retain (ఆదానం->sets.unicodes, ఆదానం->sets.glyphs, ఇది);

  populate_gids_గమ్యం_retain (ఇది, ఆదానం->sets.drop_tables);
  ఒకవేళ (unlikely (లోపల_error ()))
    తిరిగి;

  ఒకవేళ (!check_success(create_old_gid_గమ్యం_new_gid_map(
          face,
          ఆదానం->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &ఆదానం->glyph_map,
          glyph_map,
          తిరగేయి_glyph_map,
	  &new_గమ్యం_old_gid_list,
          &అంకె_output_glyphs_2))) {
    తిరిగి;
  }

#నిర్వచితమైతే HB_EXPERIMENTAL_API  
  ఒకవేళ ((ఆదానం->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (ఆదానం->flags & HB_SUBSET_FLAGS_RETAIN_అంకె_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    అంకె_output_glyphs_2 = ఆధారం->get_అంకె_glyphs ();
  }
#అయితే_ముగింపు

  ఒకవేళ (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    తిరిగి;

  // Now that we have old to new gid map update the unicode to new gid list.
  కొరకు (గుర్తులేని i = 0; i < unicode_గమ్యం_new_gid_list.నిడివి; i++)
  {
    // Use raw array access for performance.
    unicode_గమ్యం_new_gid_list.arrayZ[i].second =
        glyph_map->get(unicode_గమ్యం_new_gid_list.arrayZ[i].second);
  }

  bounds_వెడల్పు_vec.resize_dirty  (అంకె_output_glyphs_2);
  కొరకు (స్వయంచాలక &v : bounds_వెడల్పు_vec)
    v = 0xFFFFFFFF;
  bounds_ఎత్తు_vec.resize_dirty  (అంకె_output_glyphs_2);
  కొరకు (స్వయంచాలక &v : bounds_ఎత్తు_vec)
    v = 0xFFFFFFFF;

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_LAYOUT    
  ఒకవేళ (!drop_tables.has (HB_OT_ట్యాగ్_GDEF))
    remap_వాడిన_mark_sets (ఇది, వాడిన_mark_sets_map);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_VAR
#నిర్వచితం_కాకపోతే HB_NO_పునాది
  ఒకవేళ (!drop_tables.has (HB_OT_ట్యాగ్_పునాది))
    collect_పునాది_variation_indices (ఇది);
#అయితే_ముగింపు
#అయితే_ముగింపు

  ఒకవేళ (unlikely (లోపల_error ()))
    తిరిగి;

#ఒకవేళ !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (ఇది);
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (!check_success (get_instance_glyphs_contour_points (ఇది)))
      తిరిగి;
#అయితే_ముగింపు

  ఒకవేళ (attach_accelerator_దత్తాంశం)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (ఆధారం,
				       *codepoint_గమ్యం_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#నిర్వచించు HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.లోపల_error ());
#అన్తర్భావ "hb-subset-plan-member-list.hh"
#నిర్వచనం_రద్దు HB_SUBSET_PLAN_MEMBER
}

hb_subset_plan_t::~hb_subset_plan_t()
{
  hb_face_destroy (dest);

  hb_map_destroy (codepoint_గమ్యం_glyph);
  hb_map_destroy (glyph_map);
  hb_map_destroy (తిరగేయి_glyph_map);
#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#అయితే_ముగింపు
  hb_face_destroy (ఆధారం);

#నిర్వచితమైతే HB_EXPERIMENTAL_API
  కొరకు (స్వయంచాలక _ : name_పట్టిక_overrides.iter_ref ())
    _.second.fini ();
#అయితే_ముగింపు

  ఒకవేళ (inprogress_accelerator)
    hb_subset_accelerator_t::destroy ((శూన్య*) inprogress_accelerator);
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
                               స్థిరరాశి hb_subset_ఆదానం_t *ఆదానం)
{
  hb_subset_plan_t *plan;
  ఒకవేళ (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, ఆదానం))))
    తిరిగి శూన్యసూచిక;

  ఒకవేళ (unlikely (plan->లోపల_error ()))
  {
    hb_subset_plan_destroy (plan);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి plan;
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
శూన్య
hb_subset_plan_destroy (hb_subset_plan_t *plan)
{
  ఒకవేళ (!hb_object_destroy (plan)) తిరిగి;

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
hb_subset_plan_old_గమ్యం_new_glyph_mapping (స్థిరరాశి hb_subset_plan_t *plan)
{
  తిరిగి plan->glyph_map;
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
hb_subset_plan_new_గమ్యం_old_glyph_mapping (స్థిరరాశి hb_subset_plan_t *plan)
{
  తిరిగి plan->తిరగేయి_glyph_map;
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
hb_subset_plan_unicode_గమ్యం_old_glyph_mapping (స్థిరరాశి hb_subset_plan_t *plan)
{
  తిరిగి plan->codepoint_గమ్యం_glyph;
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
  తిరిగి hb_object_reference (plan);
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
hb_subset_plan_సమితి_user_దత్తాంశం (hb_subset_plan_t   *plan,
                              hb_user_దత్తాంశం_కీలకం_t *key,
                              శూన్య               *దత్తాంశం,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (plan, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_subset_plan_get_user_దత్తాంశం (స్థిరరాశి hb_subset_plan_t *plan,
                              hb_user_దత్తాంశం_కీలకం_t     *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (plan, key);
}

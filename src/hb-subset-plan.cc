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

#ଅନ୍ତର୍ଭାଵ "hb-subset-plan.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-accelerator.hh"
#ଅନ୍ତର୍ଭାଵ "hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-multimap.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset.h"
#ଅନ୍ତର୍ଭାଵ "hb-unicode.h"

#ଅନ୍ତର୍ଭାଵ "hb-ot-cmap-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-glyf-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-base-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff1-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/COLR/COLR.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/COLR/colrv1-closure.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/CPAL/CPAL.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-fvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-varc-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-stat-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-math-table.hh"

hb_subset_accelerator_t::~hb_subset_accelerator_t ()
{
  ଯଦି (cmap_cache && destroy_cmap_cache)
    destroy_cmap_cache ((ଶୂନ୍ୟ*) cmap_cache);

#ifndef HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#endif
  hb_face_destroy (source);
}


#ifndef HB_NO_SUBSET_CFF
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_add_cff_seac_components (ଅଚଳ OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_set_t *gids_to_retain)
{
  hb_codepoint_t base_gid, accent_gid;
  ଯଦି (cff.get_seac_components (gid, &base_gid, &accent_gid))
  {
    gids_to_retain->add (base_gid);
    gids_to_retain->add (accent_gid);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
#endif

ସ୍ଥିର ଶୂନ୍ୟ
_remap_palette_indexes (ଅଚଳ hb_set_t *palette_indexes,
			hb_map_t       *mapping /* OUT */)
{
  unsigned new_idx = 0;
  ପାଇଁ (unsigned palette_index : palette_indexes->iter ())
  {
    ଯଦି (palette_index == 0xFFFF)
    {
      mapping->set (palette_index, palette_index);
      ଜାରି;
    }
    mapping->set (palette_index, new_idx);
    new_idx++;
  }
}

ଶୂନ୍ୟ
remap_indexes (ଅଚଳ hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */)
{
  ପାଇଁ (auto _ : + hb_enumerate (indexes->iter ()))
    mapping->set (_.second, _.first);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_cmap_closure (hb_face_t	   *face,
	       ଅଚଳ hb_set_t	   *unicodes,
	       hb_set_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.closure_glyphs (unicodes, glyphset);
}

ସ୍ଥିର ଶୂନ୍ୟ _colr_closure (hb_subset_plan_t* plan,
                           hb_set_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->source);
  ଯଦି (!colr.is_valid ()) ଫେରନ୍ତୁ;

  hb_set_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_set_t glyphset_colrv0;
  ପାଇଁ (hb_codepoint_t gid : *glyphs_colred)
    colr.closure_glyphs (gid, &glyphset_colrv0);

  glyphs_colred->union_ (glyphset_colrv0);

  //closure for COLRv1
  hb_set_t variation_indices, delta_set_indices;
  colr.closure_forV1 (glyphs_colred, &layer_indices, &palette_indices, &variation_indices, &delta_set_indices);

  colr.closure_V0palette_indices (glyphs_colred, &palette_indices);
  remap_indexes (&layer_indices, &plan->colrv1_layers);
  _remap_palette_indexes (&palette_indices, &plan->colr_palettes);

#ifndef HB_NO_VAR
  /* colr variation indices mapping during planning phase:
   * generate colrv1_variation_idx_delta_map. When delta set index map is not
   * included, it's a mapping from varIdx-> (new varIdx,delta). Otherwise, it's
   * a mapping from old delta set idx-> (new delta set idx, delta). Mapping
   * delta set indices is the same as gid mapping.
   * Besides, we need to generate a delta set idx-> new var_idx map for updating
   * delta set index map if exists. This map will be updated again after
   * instancing. */
  ଯଦି (colr.has_var_store () && variation_indices)
  {
    ଅଚଳ OT::ItemVariationStore &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    unsigned subtable_ଗଣନା = var_store.get_sub_table_ଗଣନା ();
    generate_varstore_inner_maps (variation_indices, subtable_ଗଣନା, plan->colrv1_varstore_inner_maps);

    ଯଦି (!plan->all_axes_pinned)
      remap_variation_indices (var_store,
                              variation_indices,
                              plan->normalized_coords,
                              ମିଥ୍ୟା, /* no need to calculate delta for COLR during planning */
                              plan->all_axes_pinned,
                              plan->colrv1_variation_idx_delta_map);
  }

  /* Sentinel-only maps still need their delta-set indices remapped, even
   * when there are no live variation rows or no variation store. */
  ଯଦି (!plan->all_axes_pinned && colr.has_delta_set_index_map ())
    remap_colrv1_delta_set_index_indices (colr.get_delta_set_index_map (),
                                         delta_set_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_math_closure (hb_subset_plan_t *plan,
               hb_set_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->source_table<OT::MATH> ();
  ଯଦି (math->has_data ())
    math->closure_glyphs (glyphset);
  math.destroy ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_remove_invalid_gids (hb_set_t *glyphs,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs)
{
  glyphs->del_range (ଅଙ୍କ_glyphs, HB_SET_ମୂଲ୍ୟ_INVALID);
}

template<ତାର୍କିକ GID_ALWAYS_EXISTS = ମିଥ୍ୟା, typename I, typename F, typename G, hb_requires (hb_is_iterator (I))>
ସ୍ଥିର ଶୂନ୍ୟ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_to_gid_for_iterator,
                            G unicode_to_gid_general)
{
  ପାଇଁ (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_to_gid_for_iterator(cp);
    ଯଦି (!GID_ALWAYS_EXISTS && gid == HB_MAP_ମୂଲ୍ୟ_INVALID)
    {
      DEBUG_ସନ୍ଦେଶ(SUBSET, nullptr, "Drop U+%04X; no gid", cp);
      ଜାରି;
    }

    plan->codepoint_to_glyph->set (cp, gid);
    plan->unicode_to_new_gid_list.push (hb_pair (cp, gid));
  }
}

template<ତାର୍କିକ GID_ALWAYS_EXISTS = ମିଥ୍ୟା, typename I, typename F, hb_requires (hb_is_iterator (I))>
ସ୍ଥିର ଶୂନ୍ୟ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_to_gid_for_iterator)
{
  _fill_unicode_and_glyph_map(plan, unicode_iterator, unicode_to_gid_for_iterator, unicode_to_gid_for_iterator);
}

/*
 * Finds additional unicode codepoints which are reachable from the input unicode set.
 * Currently this adds in mirrored variants (needed for bidi) of any input unicodes.
 */
ସ୍ଥିର hb_set_t
_unicode_closure (ଅଚଳ hb_set_t* unicodes, ତାର୍କିକ bidi_closure) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_set_t out = *unicodes;
  ଯଦି (!bidi_closure) ଫେରନ୍ତୁ out;

  ଯଦି (out.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    ଫେରନ୍ତୁ out;
  }

  auto unicode_funcs = hb_unicode_funcs_get_default ();
  ପାଇଁ (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   ଯଦି (unlikely (mirror != cp)) {
     out.add(mirror);
   }
  }

  ଫେରନ୍ତୁ out;
}

ସ୍ଥିର ଶୂନ୍ୟ
_populate_unicodes_to_retain (ଅଚଳ hb_set_t *unicodes_in,
                              ଅଚଳ hb_set_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_set_t unicodes = _unicode_closure(unicodes_in,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_CLOSURE));

  OT::cmap_accelerator_t cmap (plan->source);
  unsigned size_threshold = plan->source->get_ଅଙ୍କ_glyphs ();

  ଯଦି (glyphs->is_empty () && unicodes.get_population () < size_threshold)
  {

    ଅଚଳ hb_map_t* unicode_to_gid = nullptr;
    ଯଦି (plan->accelerator)
      unicode_to_gid = &plan->accelerator->unicode_to_gid;

    // This is approach to collection is faster, but can only be used  if glyphs
    // are not being explicitly added to the subset and the input unicodes set is
    // not excessively large (eg. an inverted set).
    plan->unicode_to_new_gid_list.alloc (unicodes.get_population ());
    ଯଦି (!unicode_to_gid) {
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        hb_codepoint_t gid;
        ଯଦି (!cmap.get_nominal_glyph (cp, &gid)) {
          ଫେରନ୍ତୁ HB_MAP_ମୂଲ୍ୟ_INVALID;
        }
        ଫେରନ୍ତୁ gid;
      });
    } ଅନ୍ୟଥା {
      // Use in memory unicode to gid map it's faster then looking up from
      // the map. This code is mostly duplicated from above to avoid doing
      // conditionals on the presence of the unicode_to_gid map each
      // iteration.
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        ଫେରନ୍ତୁ unicode_to_gid->get (cp);
      });
    }
  }
  ଅନ୍ୟଥା
  {
    // This approach is slower, but can handle adding in glyphs to the subset and will match
    // them with cmap entries.

    hb_map_t unicode_glyphid_map_storage;
    hb_set_t cmap_unicodes_storage;
    ଅଚଳ hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    ଅଚଳ hb_set_t* cmap_unicodes = &cmap_unicodes_storage;

    ଯଦି (!plan->accelerator) {
      cmap.collect_mapping (&cmap_unicodes_storage, &unicode_glyphid_map_storage);
      plan->unicode_to_new_gid_list.alloc (hb_min(unicodes.get_population ()
                                                  + glyphs->get_population (),
                                                  cmap_unicodes->get_population ()));
    } ଅନ୍ୟଥା {
      unicode_glyphid_map = &plan->accelerator->unicode_to_gid;
      cmap_unicodes = &plan->accelerator->unicodes;
    }

    ଯଦି (plan->accelerator &&
	unicodes.get_population () < cmap_unicodes->get_population () &&
	glyphs->get_population () < cmap_unicodes->get_population ())
    {
      plan->codepoint_to_glyph->alloc (unicodes.get_population () + glyphs->get_population ());

      auto &gid_to_unicodes = plan->accelerator->gid_to_unicodes;

      ପାଇଁ (hb_codepoint_t gid : *glyphs)
      {
        auto unicodes = gid_to_unicodes.get (gid);
        _fill_unicode_and_glyph_map<ସତ୍ୟ>(plan, unicodes, [&] (hb_codepoint_t cp) {
          ଫେରନ୍ତୁ gid;
        },
        [&] (hb_codepoint_t cp) {
          ଫେରନ୍ତୁ unicode_glyphid_map->get(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	ଯଦି (plan->codepoint_to_glyph->has (cp))
          ଫେରନ୍ତୁ HB_MAP_ମୂଲ୍ୟ_INVALID;

        ଫେରନ୍ତୁ unicode_glyphid_map->get(cp);
      },
      [&] (hb_codepoint_t cp) {
          ଫେରନ୍ତୁ unicode_glyphid_map->get(cp);
      });

      plan->unicode_to_new_gid_list.qsort ();
    }
    ଅନ୍ୟଥା
    {
      plan->codepoint_to_glyph->alloc (cmap_unicodes->get_population ());
      hb_codepoint_t first = HB_SET_ମୂଲ୍ୟ_INVALID, last = HB_SET_ମୂଲ୍ୟ_INVALID;
      ପାଇଁ (; cmap_unicodes->next_range (&first, &last); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(first, last + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  ଯଦି (!unicodes.has (cp) && !glyphs->has (gid))
	    ଫେରନ୍ତୁ HB_MAP_ମୂଲ୍ୟ_INVALID;
          ଫେରନ୍ତୁ gid;
        },
        [&] (hb_codepoint_t cp) {
          ଫେରନ୍ତୁ unicode_glyphid_map->get(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    unsigned ଅଙ୍କ_glyphs = plan->source->get_ଅଙ୍କ_glyphs ();
    hb_codepoint_t first = HB_SET_ମୂଲ୍ୟ_INVALID, last = HB_SET_ମୂଲ୍ୟ_INVALID;
    ପାଇଁ (; glyphs->next_range (&first, &last); )
    {
      ଯଦି (first >= ଅଙ୍କ_glyphs)
	ଭାଙ୍ଗ;
      ଯଦି (last >= ଅଙ୍କ_glyphs)
        last = ଅଙ୍କ_glyphs - 1;
      plan->_glyphset_gsub.add_range (first, last);
    }
  }

  auto &arr = plan->unicode_to_new_gid_list;
  ଯଦି (arr.length)
  {
    plan->unicodes.add_sorted_array (&arr.arrayZ->first, arr.length, ଆକାର (*arr.arrayZ));
    plan->_glyphset_gsub.add_array (&arr.arrayZ->second, arr.length, ଆକାର (*arr.arrayZ));
  }

  // Variation selectors don't have glyphs associated with them in the cmap so they will have been filtered out above
  // but should still be retained. Add them back here.

  // However, the min and max codepoints for OS/2 should be calculated without considering variation selectors,
  // so record those first.
  plan->os2_info.min_cmap_codepoint = plan->unicodes.get_min();
  plan->os2_info.max_cmap_codepoint = plan->unicodes.get_max();

  hb_set_t variation_selectors_to_retain;
  cmap.collect_variation_selectors(&variation_selectors_to_retain);
  + variation_selectors_to_retain.iter()
  | hb_filter(unicodes)
  | hb_sink(&plan->unicodes)
  ;
}

ସ୍ଥିର unsigned
_glyf_add_gid_and_children (ଅଚଳ OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_set_t *gids_to_retain,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operation_ଗଣନା,
			    unsigned depth = 0)
{
  /* Check if is already visited */
  ଯଦି (gids_to_retain->has (gid)) ଫେରନ୍ତୁ operation_ଗଣନା;

  gids_to_retain->add (gid);

  ଯଦି (unlikely (depth++ > HB_MAX_NESTING_LEVEL)) ଫେରନ୍ତୁ operation_ଗଣନା;
  ଯଦି (unlikely (--operation_ଗଣନା < 0)) ଫେରନ୍ତୁ operation_ଗଣନା;

  auto glyph = glyf.glyph_for_gid (gid);

  ପାଇଁ (auto &item : glyph.get_composite_iterator ())
    operation_ଗଣନା =
      _glyf_add_gid_and_children (glyf,
				  item.get_gid (glyph.is_extended ()),
				  gids_to_retain,
				  operation_ଗଣନା,
				  depth);

  ଫେରନ୍ତୁ operation_ଗଣନା;
}

ସ୍ଥିର ଶୂନ୍ୟ
_nameid_closure (hb_subset_plan_t* plan,
		 hb_set_t* drop_tables)
{
#ifndef HB_NO_STYLE
  ଯଦି (!drop_tables->has (HB_OT_TAG_STAT))
    plan->source->table.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#endif
#ifndef HB_NO_VAR
  ଯଦି (!plan->all_axes_pinned)
    plan->source->table.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_index_tag_map, &plan->name_ids);
#endif
#ifndef HB_NO_COLOR
  ଯଦି (!drop_tables->has (HB_OT_TAG_CPAL))
    plan->source->table.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#endif

#ifndef HB_NO_SUBSET_LAYOUT
  layout_nameid_closure(plan, drop_tables);
#endif
}

ସ୍ଥିର ଶୂନ୍ୟ
_populate_gids_to_retain (hb_subset_plan_t* plan,
		          hb_set_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->source);
#ifndef HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t ଗଦା_cff (plan->accelerator ? nullptr : plan->source);
  ଅଚଳ OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.get () : &ଗଦା_cff);
#endif

  plan->_glyphset_gsub.add (0); // Not-def

  _cmap_closure (plan->source, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  ଯଦି (!drop_tables->has (HB_OT_TAG_MATH))
  {
    _math_closure (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->source->get_ଅଙ୍କ_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#ifndef HB_NO_SUBSET_LAYOUT
  layout_populate_gids_to_retain(plan, drop_tables);
#endif

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->source->get_ଅଙ୍କ_glyphs ());

  hb_set_t cur_glyphset = plan->_glyphset_gsub;
  ଯଦି (!drop_tables->has (HB_OT_TAG_COLR))
  {
    _colr_closure (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->source->get_ଅଙ୍କ_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#ifndef HB_NO_VAR_COMPOSITES
  ଯଦି (!drop_tables->has (OT::VARC::tableTag))
  {
    ଯଦି (unlikely (!plan->source->table.VARC->closure_glyphs (&cur_glyphset)))
    {
      plan->check_success (ମିଥ୍ୟା);
      ଫେରନ୍ତୁ;
    }
    _remove_invalid_gids (&cur_glyphset, plan->source->get_ଅଙ୍କ_glyphs ());
  }
#endif

  plan->_glyphset_varced = cur_glyphset;

  _nameid_closure (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  ଯଦି (glyf.has_data ())
    ପାଇଁ (hb_codepoint_t gid : cur_glyphset)
      _glyf_add_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_COMPOSITE_OPERATIONS_PER_GLYPH);
  ଅନ୍ୟଥା
    plan->_glyphset.union_ (cur_glyphset);
#ifndef HB_NO_SUBSET_CFF
  ଯଦି (!plan->accelerator || plan->accelerator->has_seac)
  {
    ତାର୍କିକ has_seac = ମିଥ୍ୟା;
    ଯଦି (cff->is_valid ())
      ପାଇଁ (hb_codepoint_t gid : cur_glyphset)
	ଯଦି (_add_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = ସତ୍ୟ;
    plan->has_seac = has_seac;
  }
#endif

  _remove_invalid_gids (&plan->_glyphset, plan->source->get_ଅଙ୍କ_glyphs ());

#ifndef HB_NO_VAR
#ifndef HB_NO_SUBSET_LAYOUT
  ଯଦି (!drop_tables->has (HB_OT_TAG_GDEF))
    collect_layout_variation_indices (plan);
#endif
#endif
}

ସ୍ଥିର ତାର୍କିକ
_create_glyph_map_gsub (ଅଚଳ hb_set_t* glyph_set_gsub,
                        ଅଚଳ hb_map_t* glyph_map,
                        hb_map_t* out,
                        hb_vector_t<hb_codepoint_t>* out_flat)
{
  ଯଦି (unlikely (glyph_set_gsub->in_error ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_codepoint_t max_gid = HB_SET_ମୂଲ୍ୟ_INVALID;
  hb_set_previous (glyph_set_gsub, &max_gid);
  unsigned flat_size = max_gid == HB_SET_ମୂଲ୍ୟ_INVALID ? 0 : max_gid + 1;
  ଯଦି (unlikely (!out_flat->resize_dirty (flat_size)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  hb_memset (out_flat->arrayZ, 0xFF, flat_size * ଆକାର (hb_codepoint_t));

  out->alloc (glyph_set_gsub->get_population ());
  ପାଇଁ (auto gid : *glyph_set_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->get (gid);
    out->set (gid, new_gid);
    out_flat->arrayZ[gid] = new_gid;
  }
  ଫେରନ୍ତୁ !out->in_error ();
}

ସ୍ଥିର ତାର୍କିକ
_create_old_gid_to_new_gid_map (ଅଚଳ hb_face_t *face,
				ତାର୍କିକ		 retain_gids,
				ଅଚଳ hb_set_t	*all_gids_to_retain,
                                ଅଚଳ hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /* OUT */
				hb_map_t	*reverse_glyph_map, /* OUT */
				hb_sorted_vector_t<hb_codepoint_pair_t> *new_to_old_gid_list /* OUT */,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	*ଅଙ୍କ_glyphs /* OUT */)
{
  unsigned କାଢ଼ = all_gids_to_retain->get_population ();
  reverse_glyph_map->alloc (କାଢ଼);
  glyph_map->alloc (କାଢ଼);
  new_to_old_gid_list->alloc (କାଢ଼);

  ଯଦି (*requested_glyph_map)
  {
    hb_set_t new_gids(requested_glyph_map->values());
    ଯଦି (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "The provided custom glyph mapping is not unique.");
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଯଦି (retain_gids)
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    hb_codepoint_t max_glyph = 0;
    hb_set_t remaining;
    ପାଇଁ (auto old_gid : all_gids_to_retain->iter ())
    {
      ଯଦି (old_gid == 0) {
	new_to_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        ଜାରି;
      }

      hb_codepoint_t* new_gid;
      ଯଦି (!requested_glyph_map->has (old_gid, &new_gid))
      {
        remaining.add(old_gid);
        ଜାରି;
      }

      ଯଦି (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_to_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_to_old_gid_list->qsort ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    ପାଇଁ (auto old_gid : remaining)
      new_to_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *ଅଙ୍କ_glyphs = max_glyph + 1;
  }
  ଅନ୍ୟଥା ଯଦି (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_to_retain), (hb_codepoint_t) 0)
    | hb_sink (new_to_old_gid_list)
    ;
    *ଅଙ୍କ_glyphs = new_to_old_gid_list->length;
  }
  ଅନ୍ୟଥା
  {
    + hb_iter (all_gids_to_retain)
    | hb_map ([] (hb_codepoint_t _) {
		ଫେରନ୍ତୁ hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_to_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_SET_ମୂଲ୍ୟ_INVALID;
    hb_set_previous (all_gids_to_retain, &max_glyph);

    *ଅଙ୍କ_glyphs = max_glyph + 1;
  }

  reverse_glyph_map->alloc (reverse_glyph_map->get_population () + new_to_old_gid_list->length);
  + hb_iter (new_to_old_gid_list)
  | hb_sink (reverse_glyph_map)
  ;
  glyph_map->alloc (glyph_map->get_population () + new_to_old_gid_list->length);
  + hb_iter (new_to_old_gid_list)
  | hb_map (&hb_codepoint_pair_t::reverse)
  | hb_sink (glyph_map)
  ;

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    ଅଚଳ hb_subset_ନିବେଶ_t *ନିବେଶ)
{
  successful = ସତ୍ୟ;
  flags = ନିବେଶ->flags;

  unicode_to_new_gid_list.init ();

  name_ids = *ନିବେଶ->sets.name_ids;
  name_languages = *ନିବେଶ->sets.name_languages;
  layout_features = *ନିବେଶ->sets.layout_features;
  layout_scripts = *ନିବେଶ->sets.layout_scripts;
  glyphs_requested = *ନିବେଶ->sets.glyphs;
  drop_tables = *ନିବେଶ->sets.drop_tables;
  no_subset_tables = *ନିବେଶ->sets.no_subset_tables;
  source = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_to_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  reverse_glyph_map = hb_map_create ();

  gsub_ଭର୍ତ୍ତି_catch_all_feature_variation_rec = ମିଥ୍ୟା;
  gpos_ଭର୍ତ୍ତି_catch_all_feature_variation_rec = ମିଥ୍ୟା;
  gdef_varstore_inner_maps.init ();

  user_axes_location = ନିବେଶ->axes_location;
  all_axes_pinned = ମିଥ୍ୟା;
  pinned_at_default = ସତ୍ୟ;
  has_gdef_varstore = ମିଥ୍ୟା;
  has_avar2 = ମିଥ୍ୟା;

#ifdef HB_EXPERIMENTAL_API
  ପାଇଁ (auto _ : ନିବେଶ->name_table_overrides)
  {
    hb_bytes_t name_bytes = _.second;
    unsigned len = name_bytes.length;
    ଅକ୍ଷର *name_str = (ଅକ୍ଷର *) hb_malloc (len);
    ଯଦି (unlikely (!check_success (name_str)))
      ଭାଙ୍ଗ;

    hb_memcpy (name_str, name_bytes.arrayZ, len);
    name_table_overrides.set (_.first, hb_bytes_t (name_str, len));
  }
#endif

  ଶୂନ୍ୟ* accel = hb_face_get_user_data(face, hb_subset_accelerator_t::user_data_ଚାବି());

  attach_accelerator_data = ନିବେଶ->attach_accelerator_data;
  force_long_loca = ନିବେଶ->force_long_loca;
#ifdef HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif

  ଯଦି (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  ଯଦି (unlikely (in_error ()))
    ଫେରନ୍ତୁ;

#ifndef HB_NO_VAR_COMPOSITES
  ଯଦି (!ନିବେଶ->sets.drop_tables->has (OT::VARC::tableTag) &&
      ନିବେଶ->sets.no_subset_tables->has (OT::VARC::tableTag) &&
      !(ନିବେଶ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->table.VARC->has_data ())
  {
    check_success (ମିଥ୍ୟା);
    ଫେରନ୍ତୁ;
  }
#endif

#ifndef HB_NO_VAR
  ଯଦି (!check_success (normalize_axes_location (face, ଏହି)))
      ଫେରନ୍ତୁ;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  ଯଦି (!user_axes_location.is_empty () &&
      !ନିବେଶ->sets.drop_tables->has (OT::VARC::tableTag) &&
      ନିବେଶ->sets.no_subset_tables->has (OT::VARC::tableTag) &&
      face->table.VARC->has_data ())
  {
    check_success (ମିଥ୍ୟା);
    ଫେରନ୍ତୁ;
  }
#endif

  _populate_unicodes_to_retain (ନିବେଶ->sets.unicodes, ନିବେଶ->sets.glyphs, ଏହି);

  _populate_gids_to_retain (ଏହି, ନିବେଶ->sets.drop_tables);
  ଯଦି (unlikely (in_error ()))
    ଫେରନ୍ତୁ;

  ଯଦି (!check_success(_create_old_gid_to_new_gid_map(
          face,
          ନିବେଶ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &ନିବେଶ->glyph_map,
          glyph_map,
          reverse_glyph_map,
	  &new_to_old_gid_list,
          &ଅଙ୍କ_output_glyphs_2))) {
    ଫେରନ୍ତୁ;
  }

#ifdef HB_EXPERIMENTAL_API  
  ଯଦି ((ନିବେଶ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (ନିବେଶ->flags & HB_SUBSET_FLAGS_RETAIN_ଅଙ୍କ_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    ଅଙ୍କ_output_glyphs_2 = source->get_ଅଙ୍କ_glyphs ();
  }
#endif

  ଯଦି (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    ଫେରନ୍ତୁ;

  // Now that we have old to new gid map update the unicode to new gid list.
  ପାଇଁ (unsigned i = 0; i < unicode_to_new_gid_list.length; i++)
  {
    // Use raw array access for performance.
    unicode_to_new_gid_list.arrayZ[i].second =
        glyph_map->get(unicode_to_new_gid_list.arrayZ[i].second);
  }

  bounds_width_vec.resize_dirty  (ଅଙ୍କ_output_glyphs_2);
  ପାଇଁ (auto &v : bounds_width_vec)
    v = 0xFFFFFFFF;
  bounds_height_vec.resize_dirty  (ଅଙ୍କ_output_glyphs_2);
  ପାଇଁ (auto &v : bounds_height_vec)
    v = 0xFFFFFFFF;

#ifndef HB_NO_SUBSET_LAYOUT    
  ଯଦି (!drop_tables.has (HB_OT_TAG_GDEF))
    remap_used_mark_sets (ଏହି, used_mark_sets_map);
#endif

#ifndef HB_NO_VAR
#ifndef HB_NO_BASE
  ଯଦି (!drop_tables.has (HB_OT_TAG_BASE))
    collect_base_variation_indices (ଏହି);
#endif
#endif

  ଯଦି (unlikely (in_error ()))
    ଫେରନ୍ତୁ;

#ଯଦି !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (ଏହି);
#endif
#ifndef HB_NO_VAR
  ଯଦି (!check_success (get_instance_glyphs_contour_points (ଏହି)))
      ଫେରନ୍ତୁ;
#endif

  ଯଦି (attach_accelerator_data)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (source,
				       *codepoint_to_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#define HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.in_error ());
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan-member-list.hh"
#undef HB_SUBSET_PLAN_MEMBER
}

hb_subset_plan_t::~hb_subset_plan_t()
{
  hb_face_destroy (dest);

  hb_map_destroy (codepoint_to_glyph);
  hb_map_destroy (glyph_map);
  hb_map_destroy (reverse_glyph_map);
#ifndef HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#endif
  hb_face_destroy (source);

#ifdef HB_EXPERIMENTAL_API
  ପାଇଁ (auto _ : name_table_overrides.iter_ref ())
    _.second.fini ();
#endif

  ଯଦି (inprogress_accelerator)
    hb_subset_accelerator_t::destroy ((ଶୂନ୍ୟ*) inprogress_accelerator);
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
                               ଅଚଳ hb_subset_ନିବେଶ_t *ନିବେଶ)
{
  hb_subset_plan_t *plan;
  ଯଦି (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, ନିବେଶ))))
    ଫେରନ୍ତୁ nullptr;

  ଯଦି (unlikely (plan->in_error ()))
  {
    hb_subset_plan_destroy (plan);
    ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ plan;
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
ଶୂନ୍ୟ
hb_subset_plan_destroy (hb_subset_plan_t *plan)
{
  ଯଦି (!hb_object_destroy (plan)) ଫେରନ୍ତୁ;

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
hb_subset_plan_old_to_new_glyph_mapping (ଅଚଳ hb_subset_plan_t *plan)
{
  ଫେରନ୍ତୁ plan->glyph_map;
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
hb_subset_plan_new_to_old_glyph_mapping (ଅଚଳ hb_subset_plan_t *plan)
{
  ଫେରନ୍ତୁ plan->reverse_glyph_map;
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
hb_subset_plan_unicode_to_old_glyph_mapping (ଅଚଳ hb_subset_plan_t *plan)
{
  ଫେରନ୍ତୁ plan->codepoint_to_glyph;
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
  ଫେରନ୍ତୁ hb_object_reference (plan);
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
hb_subset_plan_set_user_data (hb_subset_plan_t   *plan,
                              hb_user_data_ଚାବି_t *key,
                              ଶୂନ୍ୟ               *data,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  ଫେରନ୍ତୁ hb_object_set_user_data (plan, key, data, destroy, replace);
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
ଶୂନ୍ୟ *
hb_subset_plan_get_user_data (ଅଚଳ hb_subset_plan_t *plan,
                              hb_user_data_ଚାବି_t     *key)
{
  ଫେରନ୍ତୁ hb_object_get_user_data (plan, key);
}

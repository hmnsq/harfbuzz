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

#অন্তর্ভাৱ "hb-subset-plan.hh"
#অন্তর্ভাৱ "hb-subset-accelerator.hh"
#অন্তর্ভাৱ "hb-map.hh"
#অন্তর্ভাৱ "hb-multimap.hh"
#অন্তর্ভাৱ "hb-set.hh"
#অন্তর্ভাৱ "hb-subset.h"
#অন্তর্ভাৱ "hb-unicode.h"

#অন্তর্ভাৱ "hb-ot-cmap-table.hh"
#অন্তর্ভাৱ "hb-ot-glyf-table.hh"
#অন্তর্ভাৱ "hb-ot-layout-base-table.hh"
#অন্তর্ভাৱ "hb-ot-cff1-table.hh"
#অন্তর্ভাৱ "hb-ot-cff2-table.hh"
#অন্তর্ভাৱ "OT/Color/COLR/COLR.hh"
#অন্তর্ভাৱ "OT/Color/COLR/colrv1-closure.hh"
#অন্তর্ভাৱ "OT/Color/CPAL/CPAL.hh"
#অন্তর্ভাৱ "hb-ot-var-fvar-table.hh"
#অন্তর্ভাৱ "hb-ot-var-varc-table.hh"
#অন্তর্ভাৱ "hb-ot-stat-table.hh"
#অন্তর্ভাৱ "hb-ot-math-table.hh"

hb_subset_accelerator_t::~hb_subset_accelerator_t ()
{
  যদি (cmap_cache && destroy_cmap_cache)
    destroy_cmap_cache ((শূন্য*) cmap_cache);

#ifndef HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#endif
  hb_face_destroy (source);
}


#ifndef HB_NO_SUBSET_CFF
স্থির অন্তঃস্থ তার্কিক
_add_cff_seac_components (ধ্রুৱ OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_set_t *gids_to_retain)
{
  hb_codepoint_t base_gid, accent_gid;
  যদি (cff.get_seac_components (gid, &base_gid, &accent_gid))
  {
    gids_to_retain->add (base_gid);
    gids_to_retain->add (accent_gid);
    ঘূৰাই সঁচা;
  }
  ঘূৰাই মিছা;
}
#endif

স্থির শূন্য
_remap_palette_indexes (ধ্রুৱ hb_set_t *palette_indexes,
			hb_map_t       *mapping /* OUT */)
{
  unsigned new_idx = 0;
  বাবে (unsigned palette_index : palette_indexes->iter ())
  {
    যদি (palette_index == 0xFFFF)
    {
      mapping->set (palette_index, palette_index);
      অনুৱর্ত;
    }
    mapping->set (palette_index, new_idx);
    new_idx++;
  }
}

শূন্য
remap_indexes (ধ্রুৱ hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */)
{
  বাবে (auto _ : + hb_enumerate (indexes->iter ()))
    mapping->set (_.second, _.first);
}

স্থির অন্তঃস্থ শূন্য
_cmap_closure (hb_face_t	   *face,
	       ধ্রুৱ hb_set_t	   *unicodes,
	       hb_set_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.closure_glyphs (unicodes, glyphset);
}

স্থির শূন্য _colr_closure (hb_subset_plan_t* plan,
                           hb_set_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->source);
  যদি (!colr.is_valid ()) ঘূৰাই;

  hb_set_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_set_t glyphset_colrv0;
  বাবে (hb_codepoint_t gid : *glyphs_colred)
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
  যদি (colr.has_var_store () && variation_indices)
  {
    ধ্রুৱ OT::ItemVariationStore &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    unsigned subtable_গুনতি = var_store.get_sub_table_গুনতি ();
    generate_varstore_inner_maps (variation_indices, subtable_গুনতি, plan->colrv1_varstore_inner_maps);

    যদি (!plan->all_axes_pinned)
      remap_variation_indices (var_store,
                              variation_indices,
                              plan->normalized_coords,
                              মিছা, /* no need to calculate delta for COLR during planning */
                              plan->all_axes_pinned,
                              plan->colrv1_variation_idx_delta_map);
  }

  /* Sentinel-only maps still need their delta-set indices remapped, even
   * when there are no live variation rows or no variation store. */
  যদি (!plan->all_axes_pinned && colr.has_delta_set_index_map ())
    remap_colrv1_delta_set_index_indices (colr.get_delta_set_index_map (),
                                         delta_set_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#endif
}

স্থির অন্তঃস্থ শূন্য
_math_closure (hb_subset_plan_t *plan,
               hb_set_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->source_table<OT::MATH> ();
  যদি (math->has_data ())
    math->closure_glyphs (glyphset);
  math.destroy ();
}

স্থির অন্তঃস্থ শূন্য
_remove_invalid_gids (hb_set_t *glyphs,
		      unsigned পূর্ণাঙ্ক অঙ্ক_glyphs)
{
  glyphs->del_range (অঙ্ক_glyphs, HB_SET_মান_INVALID);
}

template<তার্কিক GID_ALWAYS_EXISTS = মিছা, typename I, typename F, typename G, hb_requires (hb_is_iterator (I))>
স্থির শূন্য
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_to_gid_for_iterator,
                            G unicode_to_gid_general)
{
  বাবে (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_to_gid_for_iterator(cp);
    যদি (!GID_ALWAYS_EXISTS && gid == HB_MAP_মান_INVALID)
    {
      DEBUG_বাৰ্তা(SUBSET, nullptr, "Drop U+%04X; no gid", cp);
      অনুৱর্ত;
    }

    plan->codepoint_to_glyph->set (cp, gid);
    plan->unicode_to_new_gid_list.push (hb_pair (cp, gid));
  }
}

template<তার্কিক GID_ALWAYS_EXISTS = মিছা, typename I, typename F, hb_requires (hb_is_iterator (I))>
স্থির শূন্য
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
স্থির hb_set_t
_unicode_closure (ধ্রুৱ hb_set_t* unicodes, তার্কিক bidi_closure) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_set_t out = *unicodes;
  যদি (!bidi_closure) ঘূৰাই out;

  যদি (out.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    ঘূৰাই out;
  }

  auto unicode_funcs = hb_unicode_funcs_get_default ();
  বাবে (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   যদি (unlikely (mirror != cp)) {
     out.add(mirror);
   }
  }

  ঘূৰাই out;
}

স্থির শূন্য
_populate_unicodes_to_retain (ধ্রুৱ hb_set_t *unicodes_in,
                              ধ্রুৱ hb_set_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_set_t unicodes = _unicode_closure(unicodes_in,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_CLOSURE));

  OT::cmap_accelerator_t cmap (plan->source);
  unsigned size_threshold = plan->source->get_অঙ্ক_glyphs ();

  যদি (glyphs->is_empty () && unicodes.get_population () < size_threshold)
  {

    ধ্রুৱ hb_map_t* unicode_to_gid = nullptr;
    যদি (plan->accelerator)
      unicode_to_gid = &plan->accelerator->unicode_to_gid;

    // This is approach to collection is faster, but can only be used  if glyphs
    // are not being explicitly added to the subset and the input unicodes set is
    // not excessively large (eg. an inverted set).
    plan->unicode_to_new_gid_list.alloc (unicodes.get_population ());
    যদি (!unicode_to_gid) {
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        hb_codepoint_t gid;
        যদি (!cmap.get_nominal_glyph (cp, &gid)) {
          ঘূৰাই HB_MAP_মান_INVALID;
        }
        ঘূৰাই gid;
      });
    } অন্যথা {
      // Use in memory unicode to gid map it's faster then looking up from
      // the map. This code is mostly duplicated from above to avoid doing
      // conditionals on the presence of the unicode_to_gid map each
      // iteration.
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        ঘূৰাই unicode_to_gid->get (cp);
      });
    }
  }
  অন্যথা
  {
    // This approach is slower, but can handle adding in glyphs to the subset and will match
    // them with cmap entries.

    hb_map_t unicode_glyphid_map_storage;
    hb_set_t cmap_unicodes_storage;
    ধ্রুৱ hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    ধ্রুৱ hb_set_t* cmap_unicodes = &cmap_unicodes_storage;

    যদি (!plan->accelerator) {
      cmap.collect_mapping (&cmap_unicodes_storage, &unicode_glyphid_map_storage);
      plan->unicode_to_new_gid_list.alloc (hb_min(unicodes.get_population ()
                                                  + glyphs->get_population (),
                                                  cmap_unicodes->get_population ()));
    } অন্যথা {
      unicode_glyphid_map = &plan->accelerator->unicode_to_gid;
      cmap_unicodes = &plan->accelerator->unicodes;
    }

    যদি (plan->accelerator &&
	unicodes.get_population () < cmap_unicodes->get_population () &&
	glyphs->get_population () < cmap_unicodes->get_population ())
    {
      plan->codepoint_to_glyph->alloc (unicodes.get_population () + glyphs->get_population ());

      auto &gid_to_unicodes = plan->accelerator->gid_to_unicodes;

      বাবে (hb_codepoint_t gid : *glyphs)
      {
        auto unicodes = gid_to_unicodes.get (gid);
        _fill_unicode_and_glyph_map<সঁচা>(plan, unicodes, [&] (hb_codepoint_t cp) {
          ঘূৰাই gid;
        },
        [&] (hb_codepoint_t cp) {
          ঘূৰাই unicode_glyphid_map->get(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	যদি (plan->codepoint_to_glyph->has (cp))
          ঘূৰাই HB_MAP_মান_INVALID;

        ঘূৰাই unicode_glyphid_map->get(cp);
      },
      [&] (hb_codepoint_t cp) {
          ঘূৰাই unicode_glyphid_map->get(cp);
      });

      plan->unicode_to_new_gid_list.qsort ();
    }
    অন্যথা
    {
      plan->codepoint_to_glyph->alloc (cmap_unicodes->get_population ());
      hb_codepoint_t first = HB_SET_মান_INVALID, last = HB_SET_মান_INVALID;
      বাবে (; cmap_unicodes->next_range (&first, &last); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(first, last + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  যদি (!unicodes.has (cp) && !glyphs->has (gid))
	    ঘূৰাই HB_MAP_মান_INVALID;
          ঘূৰাই gid;
        },
        [&] (hb_codepoint_t cp) {
          ঘূৰাই unicode_glyphid_map->get(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    unsigned অঙ্ক_glyphs = plan->source->get_অঙ্ক_glyphs ();
    hb_codepoint_t first = HB_SET_মান_INVALID, last = HB_SET_মান_INVALID;
    বাবে (; glyphs->next_range (&first, &last); )
    {
      যদি (first >= অঙ্ক_glyphs)
	ভঙ্গ;
      যদি (last >= অঙ্ক_glyphs)
        last = অঙ্ক_glyphs - 1;
      plan->_glyphset_gsub.add_range (first, last);
    }
  }

  auto &arr = plan->unicode_to_new_gid_list;
  যদি (arr.length)
  {
    plan->unicodes.add_sorted_array (&arr.arrayZ->first, arr.length, পরিমাণ (*arr.arrayZ));
    plan->_glyphset_gsub.add_array (&arr.arrayZ->second, arr.length, পরিমাণ (*arr.arrayZ));
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

স্থির unsigned
_glyf_add_gid_and_children (ধ্রুৱ OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_set_t *gids_to_retain,
			    পূর্ণাঙ্ক operation_গুনতি,
			    unsigned depth = 0)
{
  /* Check if is already visited */
  যদি (gids_to_retain->has (gid)) ঘূৰাই operation_গুনতি;

  gids_to_retain->add (gid);

  যদি (unlikely (depth++ > HB_MAX_NESTING_LEVEL)) ঘূৰাই operation_গুনতি;
  যদি (unlikely (--operation_গুনতি < 0)) ঘূৰাই operation_গুনতি;

  auto glyph = glyf.glyph_for_gid (gid);

  বাবে (auto &item : glyph.get_composite_iterator ())
    operation_গুনতি =
      _glyf_add_gid_and_children (glyf,
				  item.get_gid (glyph.is_extended ()),
				  gids_to_retain,
				  operation_গুনতি,
				  depth);

  ঘূৰাই operation_গুনতি;
}

স্থির শূন্য
_nameid_closure (hb_subset_plan_t* plan,
		 hb_set_t* drop_tables)
{
#ifndef HB_NO_STYLE
  যদি (!drop_tables->has (HB_OT_TAG_STAT))
    plan->source->table.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#endif
#ifndef HB_NO_VAR
  যদি (!plan->all_axes_pinned)
    plan->source->table.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_index_tag_map, &plan->name_ids);
#endif
#ifndef HB_NO_COLOR
  যদি (!drop_tables->has (HB_OT_TAG_CPAL))
    plan->source->table.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#endif

#ifndef HB_NO_SUBSET_LAYOUT
  layout_nameid_closure(plan, drop_tables);
#endif
}

স্থির শূন্য
_populate_gids_to_retain (hb_subset_plan_t* plan,
		          hb_set_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->source);
#ifndef HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t স্তূপ_cff (plan->accelerator ? nullptr : plan->source);
  ধ্রুৱ OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.get () : &স্তূপ_cff);
#endif

  plan->_glyphset_gsub.add (0); // Not-def

  _cmap_closure (plan->source, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  যদি (!drop_tables->has (HB_OT_TAG_MATH))
  {
    _math_closure (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->source->get_অঙ্ক_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#ifndef HB_NO_SUBSET_LAYOUT
  layout_populate_gids_to_retain(plan, drop_tables);
#endif

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->source->get_অঙ্ক_glyphs ());

  hb_set_t cur_glyphset = plan->_glyphset_gsub;
  যদি (!drop_tables->has (HB_OT_TAG_COLR))
  {
    _colr_closure (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->source->get_অঙ্ক_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#ifndef HB_NO_VAR_COMPOSITES
  যদি (!drop_tables->has (OT::VARC::tableTag))
  {
    যদি (unlikely (!plan->source->table.VARC->closure_glyphs (&cur_glyphset)))
    {
      plan->check_success (মিছা);
      ঘূৰাই;
    }
    _remove_invalid_gids (&cur_glyphset, plan->source->get_অঙ্ক_glyphs ());
  }
#endif

  plan->_glyphset_varced = cur_glyphset;

  _nameid_closure (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  যদি (glyf.has_data ())
    বাবে (hb_codepoint_t gid : cur_glyphset)
      _glyf_add_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_COMPOSITE_OPERATIONS_PER_GLYPH);
  অন্যথা
    plan->_glyphset.union_ (cur_glyphset);
#ifndef HB_NO_SUBSET_CFF
  যদি (!plan->accelerator || plan->accelerator->has_seac)
  {
    তার্কিক has_seac = মিছা;
    যদি (cff->is_valid ())
      বাবে (hb_codepoint_t gid : cur_glyphset)
	যদি (_add_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = সঁচা;
    plan->has_seac = has_seac;
  }
#endif

  _remove_invalid_gids (&plan->_glyphset, plan->source->get_অঙ্ক_glyphs ());

#ifndef HB_NO_VAR
#ifndef HB_NO_SUBSET_LAYOUT
  যদি (!drop_tables->has (HB_OT_TAG_GDEF))
    collect_layout_variation_indices (plan);
#endif
#endif
}

স্থির তার্কিক
_create_glyph_map_gsub (ধ্রুৱ hb_set_t* glyph_set_gsub,
                        ধ্রুৱ hb_map_t* glyph_map,
                        hb_map_t* out,
                        hb_vector_t<hb_codepoint_t>* out_flat)
{
  যদি (unlikely (glyph_set_gsub->in_error ()))
    ঘূৰাই মিছা;

  hb_codepoint_t max_gid = HB_SET_মান_INVALID;
  hb_set_previous (glyph_set_gsub, &max_gid);
  unsigned flat_size = max_gid == HB_SET_মান_INVALID ? 0 : max_gid + 1;
  যদি (unlikely (!out_flat->resize_dirty (flat_size)))
    ঘূৰাই মিছা;
  hb_memset (out_flat->arrayZ, 0xFF, flat_size * পরিমাণ (hb_codepoint_t));

  out->alloc (glyph_set_gsub->get_population ());
  বাবে (auto gid : *glyph_set_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->get (gid);
    out->set (gid, new_gid);
    out_flat->arrayZ[gid] = new_gid;
  }
  ঘূৰাই !out->in_error ();
}

স্থির তার্কিক
_create_old_gid_to_new_gid_map (ধ্রুৱ hb_face_t *face,
				তার্কিক		 retain_gids,
				ধ্রুৱ hb_set_t	*all_gids_to_retain,
                                ধ্রুৱ hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /* OUT */
				hb_map_t	*reverse_glyph_map, /* OUT */
				hb_sorted_vector_t<hb_codepoint_pair_t> *new_to_old_gid_list /* OUT */,
				unsigned পূর্ণাঙ্ক	*অঙ্ক_glyphs /* OUT */)
{
  unsigned বেৰ_কৰো = all_gids_to_retain->get_population ();
  reverse_glyph_map->alloc (বেৰ_কৰো);
  glyph_map->alloc (বেৰ_কৰো);
  new_to_old_gid_list->alloc (বেৰ_কৰো);

  যদি (*requested_glyph_map)
  {
    hb_set_t new_gids(requested_glyph_map->values());
    যদি (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_বাৰ্তা (SUBSET, nullptr, "The provided custom glyph mapping is not unique.");
      ঘূৰাই মিছা;
    }

    যদি (retain_gids)
    {
      DEBUG_বাৰ্তা (SUBSET, nullptr,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      ঘূৰাই মিছা;
    }

    hb_codepoint_t max_glyph = 0;
    hb_set_t remaining;
    বাবে (auto old_gid : all_gids_to_retain->iter ())
    {
      যদি (old_gid == 0) {
	new_to_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        অনুৱর্ত;
      }

      hb_codepoint_t* new_gid;
      যদি (!requested_glyph_map->has (old_gid, &new_gid))
      {
        remaining.add(old_gid);
        অনুৱর্ত;
      }

      যদি (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_to_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_to_old_gid_list->qsort ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    বাবে (auto old_gid : remaining)
      new_to_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *অঙ্ক_glyphs = max_glyph + 1;
  }
  অন্যথা যদি (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_to_retain), (hb_codepoint_t) 0)
    | hb_sink (new_to_old_gid_list)
    ;
    *অঙ্ক_glyphs = new_to_old_gid_list->length;
  }
  অন্যথা
  {
    + hb_iter (all_gids_to_retain)
    | hb_map ([] (hb_codepoint_t _) {
		ঘূৰাই hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_to_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_SET_মান_INVALID;
    hb_set_previous (all_gids_to_retain, &max_glyph);

    *অঙ্ক_glyphs = max_glyph + 1;
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

  ঘূৰাই সঁচা;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    ধ্রুৱ hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  successful = সঁচা;
  flags = প্ৰবেশ->flags;

  unicode_to_new_gid_list.init ();

  name_ids = *প্ৰবেশ->sets.name_ids;
  name_languages = *প্ৰবেশ->sets.name_languages;
  layout_features = *প্ৰবেশ->sets.layout_features;
  layout_scripts = *প্ৰবেশ->sets.layout_scripts;
  glyphs_requested = *প্ৰবেশ->sets.glyphs;
  drop_tables = *প্ৰবেশ->sets.drop_tables;
  no_subset_tables = *প্ৰবেশ->sets.no_subset_tables;
  source = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_to_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  reverse_glyph_map = hb_map_create ();

  gsub_ঢোকাও_catch_all_feature_variation_rec = মিছা;
  gpos_ঢোকাও_catch_all_feature_variation_rec = মিছা;
  gdef_varstore_inner_maps.init ();

  user_axes_location = প্ৰবেশ->axes_location;
  all_axes_pinned = মিছা;
  pinned_at_default = সঁচা;
  has_gdef_varstore = মিছা;
  has_avar2 = মিছা;

#ifdef HB_EXPERIMENTAL_API
  বাবে (auto _ : প্ৰবেশ->name_table_overrides)
  {
    hb_bytes_t name_bytes = _.second;
    unsigned len = name_bytes.length;
    ৱর্ণ *name_str = (ৱর্ণ *) hb_malloc (len);
    যদি (unlikely (!check_success (name_str)))
      ভঙ্গ;

    hb_memcpy (name_str, name_bytes.arrayZ, len);
    name_table_overrides.set (_.first, hb_bytes_t (name_str, len));
  }
#endif

  শূন্য* accel = hb_face_get_user_data(face, hb_subset_accelerator_t::user_data_চাবি());

  attach_accelerator_data = প্ৰবেশ->attach_accelerator_data;
  force_long_loca = প্ৰবেশ->force_long_loca;
#ifdef HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#endif

  যদি (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  যদি (unlikely (in_error ()))
    ঘূৰাই;

#ifndef HB_NO_VAR_COMPOSITES
  যদি (!প্ৰবেশ->sets.drop_tables->has (OT::VARC::tableTag) &&
      প্ৰবেশ->sets.no_subset_tables->has (OT::VARC::tableTag) &&
      !(প্ৰবেশ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->table.VARC->has_data ())
  {
    check_success (মিছা);
    ঘূৰাই;
  }
#endif

#ifndef HB_NO_VAR
  যদি (!check_success (normalize_axes_location (face, এতত্)))
      ঘূৰাই;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  যদি (!user_axes_location.is_empty () &&
      !প্ৰবেশ->sets.drop_tables->has (OT::VARC::tableTag) &&
      প্ৰবেশ->sets.no_subset_tables->has (OT::VARC::tableTag) &&
      face->table.VARC->has_data ())
  {
    check_success (মিছা);
    ঘূৰাই;
  }
#endif

  _populate_unicodes_to_retain (প্ৰবেশ->sets.unicodes, প্ৰবেশ->sets.glyphs, এতত্);

  _populate_gids_to_retain (এতত্, প্ৰবেশ->sets.drop_tables);
  যদি (unlikely (in_error ()))
    ঘূৰাই;

  যদি (!check_success(_create_old_gid_to_new_gid_map(
          face,
          প্ৰবেশ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &প্ৰবেশ->glyph_map,
          glyph_map,
          reverse_glyph_map,
	  &new_to_old_gid_list,
          &অঙ্ক_output_glyphs_2))) {
    ঘূৰাই;
  }

#ifdef HB_EXPERIMENTAL_API  
  যদি ((প্ৰবেশ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (প্ৰবেশ->flags & HB_SUBSET_FLAGS_RETAIN_অঙ্ক_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    অঙ্ক_output_glyphs_2 = source->get_অঙ্ক_glyphs ();
  }
#endif

  যদি (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    ঘূৰাই;

  // Now that we have old to new gid map update the unicode to new gid list.
  বাবে (unsigned i = 0; i < unicode_to_new_gid_list.length; i++)
  {
    // Use raw array access for performance.
    unicode_to_new_gid_list.arrayZ[i].second =
        glyph_map->get(unicode_to_new_gid_list.arrayZ[i].second);
  }

  bounds_width_vec.resize_dirty  (অঙ্ক_output_glyphs_2);
  বাবে (auto &v : bounds_width_vec)
    v = 0xFFFFFFFF;
  bounds_height_vec.resize_dirty  (অঙ্ক_output_glyphs_2);
  বাবে (auto &v : bounds_height_vec)
    v = 0xFFFFFFFF;

#ifndef HB_NO_SUBSET_LAYOUT    
  যদি (!drop_tables.has (HB_OT_TAG_GDEF))
    remap_used_mark_sets (এতত্, used_mark_sets_map);
#endif

#ifndef HB_NO_VAR
#ifndef HB_NO_BASE
  যদি (!drop_tables.has (HB_OT_TAG_BASE))
    collect_base_variation_indices (এতত্);
#endif
#endif

  যদি (unlikely (in_error ()))
    ঘূৰাই;

#যদি !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (এতত্);
#endif
#ifndef HB_NO_VAR
  যদি (!check_success (get_instance_glyphs_contour_points (এতত্)))
      ঘূৰাই;
#endif

  যদি (attach_accelerator_data)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (source,
				       *codepoint_to_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#define HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.in_error ());
#অন্তর্ভাৱ "hb-subset-plan-member-list.hh"
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
  বাবে (auto _ : name_table_overrides.iter_ref ())
    _.second.fini ();
#endif

  যদি (inprogress_accelerator)
    hb_subset_accelerator_t::destroy ((শূন্য*) inprogress_accelerator);
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
                               ধ্রুৱ hb_subset_প্ৰবেশ_t *প্ৰবেশ)
{
  hb_subset_plan_t *plan;
  যদি (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, প্ৰবেশ))))
    ঘূৰাই nullptr;

  যদি (unlikely (plan->in_error ()))
  {
    hb_subset_plan_destroy (plan);
    ঘূৰাই nullptr;
  }

  ঘূৰাই plan;
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
শূন্য
hb_subset_plan_destroy (hb_subset_plan_t *plan)
{
  যদি (!hb_object_destroy (plan)) ঘূৰাই;

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
hb_subset_plan_old_to_new_glyph_mapping (ধ্রুৱ hb_subset_plan_t *plan)
{
  ঘূৰাই plan->glyph_map;
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
hb_subset_plan_new_to_old_glyph_mapping (ধ্রুৱ hb_subset_plan_t *plan)
{
  ঘূৰাই plan->reverse_glyph_map;
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
hb_subset_plan_unicode_to_old_glyph_mapping (ধ্রুৱ hb_subset_plan_t *plan)
{
  ঘূৰাই plan->codepoint_to_glyph;
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
  ঘূৰাই hb_object_reference (plan);
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
                              hb_user_data_চাবি_t *key,
                              শূন্য               *data,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  ঘূৰাই hb_object_set_user_data (plan, key, data, destroy, replace);
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
শূন্য *
hb_subset_plan_get_user_data (ধ্রুৱ hb_subset_plan_t *plan,
                              hb_user_data_চাবি_t     *key)
{
  ঘূৰাই hb_object_get_user_data (plan, key);
}

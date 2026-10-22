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

#समावेश "hb-subset-plan.hh"
#समावेश "hb-subset-accelerator.hh"
#समावेश "hb-map.hh"
#समावेश "hb-multimap.hh"
#समावेश "hb-set.hh"
#समावेश "hb-subset.h"
#समावेश "hb-unicode.h"

#समावेश "hb-ot-cmap-table.hh"
#समावेश "hb-ot-glyf-table.hh"
#समावेश "hb-ot-layout-base-table.hh"
#समावेश "hb-ot-cff1-table.hh"
#समावेश "hb-ot-cff2-table.hh"
#समावेश "OT/Color/COLR/COLR.hh"
#समावेश "OT/Color/COLR/colrv1-closure.hh"
#समावेश "OT/Color/CPAL/CPAL.hh"
#समावेश "hb-ot-var-fvar-table.hh"
#समावेश "hb-ot-var-varc-table.hh"
#समावेश "hb-ot-stat-table.hh"
#समावेश "hb-ot-math-table.hh"

hb_subset_accelerator_t::~hb_subset_accelerator_t ()
{
  अगर (cmap_cache && destroy_cmap_cache)
    destroy_cmap_cache ((व्योम*) cmap_cache);

#अगर_अघोषित HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#पूर्ण_अगर
  hb_face_destroy (source);
}


#अगर_अघोषित HB_NO_SUBSET_CFF
अटल अंतरभूत bool
_add_cff_seac_components (स्थिर OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_set_t *gids_to_retain)
{
  hb_codepoint_t base_gid, accent_gid;
  अगर (cff.get_seac_components (gid, &base_gid, &accent_gid))
  {
    gids_to_retain->add (base_gid);
    gids_to_retain->add (accent_gid);
    वापस true;
  }
  वापस false;
}
#पूर्ण_अगर

अटल व्योम
_remap_palette_indexes (स्थिर hb_set_t *palette_indexes,
			hb_map_t       *mapping /* OUT */)
{
  अचिन्हित new_idx = 0;
  क्रम (अचिन्हित palette_सूचकांक : palette_indexes->iter ())
  {
    अगर (palette_सूचकांक == 0xFFFF)
    {
      mapping->set (palette_सूचकांक, palette_सूचकांक);
      जारी;
    }
    mapping->set (palette_सूचकांक, new_idx);
    new_idx++;
  }
}

व्योम
remap_indexes (स्थिर hb_set_t *indexes,
               hb_map_t       *mapping /* OUT */)
{
  क्रम (स्वतः _ : + hb_enumerate (indexes->iter ()))
    mapping->set (_.second, _.first);
}

अटल अंतरभूत व्योम
_cmap_closure (hb_face_t	   *face,
	       स्थिर hb_set_t	   *unicodes,
	       hb_set_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.closure_glyphs (unicodes, glyphset);
}

अटल व्योम _colr_closure (hb_subset_plan_t* plan,
                           hb_set_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->source);
  अगर (!colr.is_valid ()) वापस;

  hb_set_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_set_t glyphset_colrv0;
  क्रम (hb_codepoint_t gid : *glyphs_colred)
    colr.closure_glyphs (gid, &glyphset_colrv0);

  glyphs_colred->union_ (glyphset_colrv0);

  //closure for COLRv1
  hb_set_t variation_indices, delta_set_indices;
  colr.closure_forV1 (glyphs_colred, &layer_indices, &palette_indices, &variation_indices, &delta_set_indices);

  colr.closure_V0palette_indices (glyphs_colred, &palette_indices);
  remap_indexes (&layer_indices, &plan->colrv1_layers);
  _remap_palette_indexes (&palette_indices, &plan->colr_palettes);

#अगर_अघोषित HB_NO_VAR
  /* colr variation indices mapping during planning phase:
   * generate colrv1_variation_idx_delta_map. When delta set index map is not
   * included, it's a mapping from varIdx-> (new varIdx,delta). Otherwise, it's
   * a mapping from old delta set idx-> (new delta set idx, delta). Mapping
   * delta set indices is the same as gid mapping.
   * Besides, we need to generate a delta set idx-> new var_idx map for updating
   * delta set index map if exists. This map will be updated again after
   * instancing. */
  अगर (colr.has_var_store () && variation_indices)
  {
    स्थिर OT::ItemVariationStore &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    अचिन्हित subtable_गिनती = var_store.get_sub_table_गिनती ();
    generate_varstore_inner_maps (variation_indices, subtable_गिनती, plan->colrv1_varstore_inner_maps);

    अगर (!plan->all_axes_pinned)
      remap_variation_indices (var_store,
                              variation_indices,
                              plan->normalized_coords,
                              false, /* no need to calculate delta for COLR during planning */
                              plan->all_axes_pinned,
                              plan->colrv1_variation_idx_delta_map);
  }

  /* Sentinel-only maps still need their delta-set indices remapped, even
   * when there are no live variation rows or no variation store. */
  अगर (!plan->all_axes_pinned && colr.has_delta_set_सूचकांक_map ())
    remap_colrv1_delta_set_सूचकांक_indices (colr.get_delta_set_सूचकांक_map (),
                                         delta_set_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#पूर्ण_अगर
}

अटल अंतरभूत व्योम
_math_closure (hb_subset_plan_t *plan,
               hb_set_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->source_table<OT::MATH> ();
  अगर (math->has_तथ्य ())
    math->closure_glyphs (glyphset);
  math.destroy ();
}

अटल अंतरभूत व्योम
_remove_invalid_gids (hb_set_t *glyphs,
		      अचिन्हित पूर्णांक अंक_glyphs)
{
  glyphs->del_range (अंक_glyphs, HB_SET_मूल्य_INVALID);
}

ढाँचा<bool GID_ALWAYS_EXISTS = false, typename I, typename F, typename G, hb_requires (hb_is_iterator (I))>
अटल व्योम
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_to_gid_for_iterator,
                            G unicode_to_gid_general)
{
  क्रम (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_to_gid_for_iterator(cp);
    अगर (!GID_ALWAYS_EXISTS && gid == HB_MAP_मूल्य_INVALID)
    {
      DEBUG_संदेश(SUBSET, nullptr, "Drop U+%04X; no gid", cp);
      जारी;
    }

    plan->codepoint_to_glyph->set (cp, gid);
    plan->unicode_to_new_gid_list.push (hb_pair (cp, gid));
  }
}

ढाँचा<bool GID_ALWAYS_EXISTS = false, typename I, typename F, hb_requires (hb_is_iterator (I))>
अटल व्योम
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
अटल hb_set_t
_unicode_closure (स्थिर hb_set_t* unicodes, bool bidi_closure) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_set_t out = *unicodes;
  अगर (!bidi_closure) वापस out;

  अगर (out.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    वापस out;
  }

  स्वतः unicode_funcs = hb_unicode_funcs_get_default ();
  क्रम (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   अगर (unlikely (mirror != cp)) {
     out.add(mirror);
   }
  }

  वापस out;
}

अटल व्योम
_populate_unicodes_to_retain (स्थिर hb_set_t *unicodes_in,
                              स्थिर hb_set_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_set_t unicodes = _unicode_closure(unicodes_in,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_CLOSURE));

  OT::cmap_accelerator_t cmap (plan->source);
  अचिन्हित आकार_threshold = plan->source->get_अंक_glyphs ();

  अगर (glyphs->is_empty () && unicodes.get_population () < आकार_threshold)
  {

    स्थिर hb_map_t* unicode_to_gid = nullptr;
    अगर (plan->accelerator)
      unicode_to_gid = &plan->accelerator->unicode_to_gid;

    // This is approach to collection is faster, but can only be used  if glyphs
    // are not being explicitly added to the subset and the input unicodes set is
    // not excessively large (eg. an inverted set).
    plan->unicode_to_new_gid_list.alloc (unicodes.get_population ());
    अगर (!unicode_to_gid) {
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        hb_codepoint_t gid;
        अगर (!cmap.get_nominal_glyph (cp, &gid)) {
          वापस HB_MAP_मूल्य_INVALID;
        }
        वापस gid;
      });
    } अन्यथा {
      // Use in memory unicode to gid map it's faster then looking up from
      // the map. This code is mostly duplicated from above to avoid doing
      // conditionals on the presence of the unicode_to_gid map each
      // iteration.
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        वापस unicode_to_gid->लो (cp);
      });
    }
  }
  अन्यथा
  {
    // This approach is slower, but can handle adding in glyphs to the subset and will match
    // them with cmap entries.

    hb_map_t unicode_glyphid_map_storage;
    hb_set_t cmap_unicodes_storage;
    स्थिर hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    स्थिर hb_set_t* cmap_unicodes = &cmap_unicodes_storage;

    अगर (!plan->accelerator) {
      cmap.collect_mapping (&cmap_unicodes_storage, &unicode_glyphid_map_storage);
      plan->unicode_to_new_gid_list.alloc (hb_min(unicodes.get_population ()
                                                  + glyphs->get_population (),
                                                  cmap_unicodes->get_population ()));
    } अन्यथा {
      unicode_glyphid_map = &plan->accelerator->unicode_to_gid;
      cmap_unicodes = &plan->accelerator->unicodes;
    }

    अगर (plan->accelerator &&
	unicodes.get_population () < cmap_unicodes->get_population () &&
	glyphs->get_population () < cmap_unicodes->get_population ())
    {
      plan->codepoint_to_glyph->alloc (unicodes.get_population () + glyphs->get_population ());

      स्वतः &gid_to_unicodes = plan->accelerator->gid_to_unicodes;

      क्रम (hb_codepoint_t gid : *glyphs)
      {
        स्वतः unicodes = gid_to_unicodes.लो (gid);
        _fill_unicode_and_glyph_map<true>(plan, unicodes, [&] (hb_codepoint_t cp) {
          वापस gid;
        },
        [&] (hb_codepoint_t cp) {
          वापस unicode_glyphid_map->लो(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	अगर (plan->codepoint_to_glyph->has (cp))
          वापस HB_MAP_मूल्य_INVALID;

        वापस unicode_glyphid_map->लो(cp);
      },
      [&] (hb_codepoint_t cp) {
          वापस unicode_glyphid_map->लो(cp);
      });

      plan->unicode_to_new_gid_list.क्विक ();
    }
    अन्यथा
    {
      plan->codepoint_to_glyph->alloc (cmap_unicodes->get_population ());
      hb_codepoint_t first = HB_SET_मूल्य_INVALID, last = HB_SET_मूल्य_INVALID;
      क्रम (; cmap_unicodes->अग्र_range (&first, &last); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(first, last + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  अगर (!unicodes.has (cp) && !glyphs->has (gid))
	    वापस HB_MAP_मूल्य_INVALID;
          वापस gid;
        },
        [&] (hb_codepoint_t cp) {
          वापस unicode_glyphid_map->लो(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    अचिन्हित अंक_glyphs = plan->source->get_अंक_glyphs ();
    hb_codepoint_t first = HB_SET_मूल्य_INVALID, last = HB_SET_मूल्य_INVALID;
    क्रम (; glyphs->अग्र_range (&first, &last); )
    {
      अगर (first >= अंक_glyphs)
	अवरोध;
      अगर (last >= अंक_glyphs)
        last = अंक_glyphs - 1;
      plan->_glyphset_gsub.add_range (first, last);
    }
  }

  स्वतः &arr = plan->unicode_to_new_gid_list;
  अगर (arr.लंबाई)
  {
    plan->unicodes.add_sorted_array (&arr.arrayZ->first, arr.लंबाई, माप (*arr.arrayZ));
    plan->_glyphset_gsub.add_array (&arr.arrayZ->second, arr.लंबाई, माप (*arr.arrayZ));
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

अटल अचिन्हित
_glyf_add_gid_and_children (स्थिर OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_set_t *gids_to_retain,
			    पूर्णांक operation_गिनती,
			    अचिन्हित depth = 0)
{
  /* Check if is already visited */
  अगर (gids_to_retain->has (gid)) वापस operation_गिनती;

  gids_to_retain->add (gid);

  अगर (unlikely (depth++ > HB_MAX_NESTING_LEVEL)) वापस operation_गिनती;
  अगर (unlikely (--operation_गिनती < 0)) वापस operation_गिनती;

  स्वतः glyph = glyf.glyph_for_gid (gid);

  क्रम (स्वतः &item : glyph.get_composite_iterator ())
    operation_गिनती =
      _glyf_add_gid_and_children (glyf,
				  item.get_gid (glyph.is_extended ()),
				  gids_to_retain,
				  operation_गिनती,
				  depth);

  वापस operation_गिनती;
}

अटल व्योम
_nameid_closure (hb_subset_plan_t* plan,
		 hb_set_t* drop_tables)
{
#अगर_अघोषित HB_NO_STYLE
  अगर (!drop_tables->has (HB_OT_TAG_STAT))
    plan->source->table.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR
  अगर (!plan->all_axes_pinned)
    plan->source->table.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_सूचकांक_tag_map, &plan->name_ids);
#पूर्ण_अगर
#अगर_अघोषित HB_NO_COLOR
  अगर (!drop_tables->has (HB_OT_TAG_CPAL))
    plan->source->table.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#पूर्ण_अगर

#अगर_अघोषित HB_NO_SUBSET_LAYOUT
  layout_nameid_closure(plan, drop_tables);
#पूर्ण_अगर
}

अटल व्योम
_populate_gids_to_retain (hb_subset_plan_t* plan,
		          hb_set_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->source);
#अगर_अघोषित HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t ढेर_cff (plan->accelerator ? nullptr : plan->source);
  स्थिर OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.लो () : &ढेर_cff);
#पूर्ण_अगर

  plan->_glyphset_gsub.add (0); // Not-def

  _cmap_closure (plan->source, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  अगर (!drop_tables->has (HB_OT_TAG_MATH))
  {
    _math_closure (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->source->get_अंक_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#अगर_अघोषित HB_NO_SUBSET_LAYOUT
  layout_populate_gids_to_retain(plan, drop_tables);
#पूर्ण_अगर

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->source->get_अंक_glyphs ());

  hb_set_t cur_glyphset = plan->_glyphset_gsub;
  अगर (!drop_tables->has (HB_OT_TAG_COLR))
  {
    _colr_closure (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->source->get_अंक_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#अगर_अघोषित HB_NO_VAR_COMPOSITES
  अगर (!drop_tables->has (OT::VARC::tableTag))
  {
    अगर (unlikely (!plan->source->table.VARC->closure_glyphs (&cur_glyphset)))
    {
      plan->check_success (false);
      वापस;
    }
    _remove_invalid_gids (&cur_glyphset, plan->source->get_अंक_glyphs ());
  }
#पूर्ण_अगर

  plan->_glyphset_varced = cur_glyphset;

  _nameid_closure (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  अगर (glyf.has_तथ्य ())
    क्रम (hb_codepoint_t gid : cur_glyphset)
      _glyf_add_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_COMPOSITE_OPERATIONS_PER_GLYPH);
  अन्यथा
    plan->_glyphset.union_ (cur_glyphset);
#अगर_अघोषित HB_NO_SUBSET_CFF
  अगर (!plan->accelerator || plan->accelerator->has_seac)
  {
    bool has_seac = false;
    अगर (cff->is_valid ())
      क्रम (hb_codepoint_t gid : cur_glyphset)
	अगर (_add_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = true;
    plan->has_seac = has_seac;
  }
#पूर्ण_अगर

  _remove_invalid_gids (&plan->_glyphset, plan->source->get_अंक_glyphs ());

#अगर_अघोषित HB_NO_VAR
#अगर_अघोषित HB_NO_SUBSET_LAYOUT
  अगर (!drop_tables->has (HB_OT_TAG_GDEF))
    collect_layout_variation_indices (plan);
#पूर्ण_अगर
#पूर्ण_अगर
}

अटल bool
_create_glyph_map_gsub (स्थिर hb_set_t* glyph_set_gsub,
                        स्थिर hb_map_t* glyph_map,
                        hb_map_t* out,
                        hb_vector_t<hb_codepoint_t>* out_flat)
{
  अगर (unlikely (glyph_set_gsub->in_error ()))
    वापस false;

  hb_codepoint_t max_gid = HB_SET_मूल्य_INVALID;
  hb_set_previous (glyph_set_gsub, &max_gid);
  अचिन्हित flat_आकार = max_gid == HB_SET_मूल्य_INVALID ? 0 : max_gid + 1;
  अगर (unlikely (!out_flat->resize_dirty (flat_आकार)))
    वापस false;
  hb_memset (out_flat->arrayZ, 0xFF, flat_आकार * माप (hb_codepoint_t));

  out->alloc (glyph_set_gsub->get_population ());
  क्रम (स्वतः gid : *glyph_set_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->लो (gid);
    out->set (gid, new_gid);
    out_flat->arrayZ[gid] = new_gid;
  }
  वापस !out->in_error ();
}

अटल bool
_create_old_gid_to_new_gid_map (स्थिर hb_face_t *face,
				bool		 retain_gids,
				स्थिर hb_set_t	*all_gids_to_retain,
                                स्थिर hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /* OUT */
				hb_map_t	*reverse_glyph_map, /* OUT */
				hb_sorted_vector_t<hb_codepoint_pair_t> *new_to_old_gid_list /* OUT */,
				अचिन्हित पूर्णांक	*अंक_glyphs /* OUT */)
{
  अचिन्हित निकालो = all_gids_to_retain->get_population ();
  reverse_glyph_map->alloc (निकालो);
  glyph_map->alloc (निकालो);
  new_to_old_gid_list->alloc (निकालो);

  अगर (*requested_glyph_map)
  {
    hb_set_t new_gids(requested_glyph_map->values());
    अगर (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_संदेश (SUBSET, nullptr, "The provided custom glyph mapping is not unique.");
      वापस false;
    }

    अगर (retain_gids)
    {
      DEBUG_संदेश (SUBSET, nullptr,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      वापस false;
    }

    hb_codepoint_t max_glyph = 0;
    hb_set_t remaining;
    क्रम (स्वतः old_gid : all_gids_to_retain->iter ())
    {
      अगर (old_gid == 0) {
	new_to_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        जारी;
      }

      hb_codepoint_t* new_gid;
      अगर (!requested_glyph_map->has (old_gid, &new_gid))
      {
        remaining.add(old_gid);
        जारी;
      }

      अगर (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_to_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_to_old_gid_list->क्विक ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    क्रम (स्वतः old_gid : remaining)
      new_to_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *अंक_glyphs = max_glyph + 1;
  }
  अन्यथा अगर (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_to_retain), (hb_codepoint_t) 0)
    | hb_sink (new_to_old_gid_list)
    ;
    *अंक_glyphs = new_to_old_gid_list->लंबाई;
  }
  अन्यथा
  {
    + hb_iter (all_gids_to_retain)
    | hb_map ([] (hb_codepoint_t _) {
		वापस hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_to_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_SET_मूल्य_INVALID;
    hb_set_previous (all_gids_to_retain, &max_glyph);

    *अंक_glyphs = max_glyph + 1;
  }

  reverse_glyph_map->alloc (reverse_glyph_map->get_population () + new_to_old_gid_list->लंबाई);
  + hb_iter (new_to_old_gid_list)
  | hb_sink (reverse_glyph_map)
  ;
  glyph_map->alloc (glyph_map->get_population () + new_to_old_gid_list->लंबाई);
  + hb_iter (new_to_old_gid_list)
  | hb_map (&hb_codepoint_pair_t::reverse)
  | hb_sink (glyph_map)
  ;

  वापस true;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    स्थिर hb_subset_निवेश_t *निवेश)
{
  successful = true;
  flags = निवेश->flags;

  unicode_to_new_gid_list.init ();

  name_ids = *निवेश->sets.name_ids;
  name_languages = *निवेश->sets.name_languages;
  layout_features = *निवेश->sets.layout_features;
  layout_scripts = *निवेश->sets.layout_scripts;
  glyphs_requested = *निवेश->sets.glyphs;
  drop_tables = *निवेश->sets.drop_tables;
  no_subset_tables = *निवेश->sets.no_subset_tables;
  source = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_to_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  reverse_glyph_map = hb_map_create ();

  gsub_जोड़ो_catch_all_feature_variation_rec = false;
  gpos_जोड़ो_catch_all_feature_variation_rec = false;
  gdef_varstore_inner_maps.init ();

  user_axes_location = निवेश->axes_location;
  all_axes_pinned = false;
  pinned_at_default = true;
  has_gdef_varstore = false;
  has_avar2 = false;

#अगर_घोषित HB_EXPERIMENTAL_API
  क्रम (स्वतः _ : निवेश->name_table_overrides)
  {
    hb_bytes_t name_bytes = _.second;
    अचिन्हित len = name_bytes.लंबाई;
    अक्षर *name_str = (अक्षर *) hb_malloc (len);
    अगर (unlikely (!check_success (name_str)))
      अवरोध;

    hb_memcpy (name_str, name_bytes.arrayZ, len);
    name_table_overrides.set (_.first, hb_bytes_t (name_str, len));
  }
#पूर्ण_अगर

  व्योम* accel = hb_face_get_user_तथ्य(face, hb_subset_accelerator_t::user_तथ्य_कुंजी());

  attach_accelerator_तथ्य = निवेश->attach_accelerator_तथ्य;
  force_long_loca = निवेश->force_long_loca;
#अगर_घोषित HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#पूर्ण_अगर

  अगर (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  अगर (unlikely (in_error ()))
    वापस;

#अगर_अघोषित HB_NO_VAR_COMPOSITES
  अगर (!निवेश->sets.drop_tables->has (OT::VARC::tableTag) &&
      निवेश->sets.no_subset_tables->has (OT::VARC::tableTag) &&
      !(निवेश->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->table.VARC->has_तथ्य ())
  {
    check_success (false);
    वापस;
  }
#पूर्ण_अगर

#अगर_अघोषित HB_NO_VAR
  अगर (!check_success (normalize_axes_location (face, यह)))
      वापस;
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR_COMPOSITES
  अगर (!user_axes_location.is_empty () &&
      !निवेश->sets.drop_tables->has (OT::VARC::tableTag) &&
      निवेश->sets.no_subset_tables->has (OT::VARC::tableTag) &&
      face->table.VARC->has_तथ्य ())
  {
    check_success (false);
    वापस;
  }
#पूर्ण_अगर

  _populate_unicodes_to_retain (निवेश->sets.unicodes, निवेश->sets.glyphs, यह);

  _populate_gids_to_retain (यह, निवेश->sets.drop_tables);
  अगर (unlikely (in_error ()))
    वापस;

  अगर (!check_success(_create_old_gid_to_new_gid_map(
          face,
          निवेश->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &निवेश->glyph_map,
          glyph_map,
          reverse_glyph_map,
	  &new_to_old_gid_list,
          &अंक_output_glyphs_2))) {
    वापस;
  }

#अगर_घोषित HB_EXPERIMENTAL_API  
  अगर ((निवेश->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (निवेश->flags & HB_SUBSET_FLAGS_RETAIN_अंक_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    अंक_output_glyphs_2 = source->get_अंक_glyphs ();
  }
#पूर्ण_अगर

  अगर (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    वापस;

  // Now that we have old to new gid map update the unicode to new gid list.
  क्रम (अचिन्हित i = 0; i < unicode_to_new_gid_list.लंबाई; i++)
  {
    // Use raw array access for performance.
    unicode_to_new_gid_list.arrayZ[i].second =
        glyph_map->लो(unicode_to_new_gid_list.arrayZ[i].second);
  }

  bounds_width_vec.resize_dirty  (अंक_output_glyphs_2);
  क्रम (स्वतः &v : bounds_width_vec)
    v = 0xFFFFFFFF;
  bounds_height_vec.resize_dirty  (अंक_output_glyphs_2);
  क्रम (स्वतः &v : bounds_height_vec)
    v = 0xFFFFFFFF;

#अगर_अघोषित HB_NO_SUBSET_LAYOUT    
  अगर (!drop_tables.has (HB_OT_TAG_GDEF))
    remap_used_mark_sets (यह, used_mark_sets_map);
#पूर्ण_अगर

#अगर_अघोषित HB_NO_VAR
#अगर_अघोषित HB_NO_BASE
  अगर (!drop_tables.has (HB_OT_TAG_BASE))
    collect_base_variation_indices (यह);
#पूर्ण_अगर
#पूर्ण_अगर

  अगर (unlikely (in_error ()))
    वापस;

#अगर !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (यह);
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR
  अगर (!check_success (get_instance_glyphs_contour_points (यह)))
      वापस;
#पूर्ण_अगर

  अगर (attach_accelerator_तथ्य)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (source,
				       *codepoint_to_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#घोषणा HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.in_error ());
#समावेश "hb-subset-plan-member-list.hh"
#अघोषित HB_SUBSET_PLAN_MEMBER
}

hb_subset_plan_t::~hb_subset_plan_t()
{
  hb_face_destroy (dest);

  hb_map_destroy (codepoint_to_glyph);
  hb_map_destroy (glyph_map);
  hb_map_destroy (reverse_glyph_map);
#अगर_अघोषित HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#पूर्ण_अगर
  hb_face_destroy (source);

#अगर_घोषित HB_EXPERIMENTAL_API
  क्रम (स्वतः _ : name_table_overrides.iter_ref ())
    _.second.fini ();
#पूर्ण_अगर

  अगर (inprogress_accelerator)
    hb_subset_accelerator_t::destroy ((व्योम*) inprogress_accelerator);
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
                               स्थिर hb_subset_निवेश_t *निवेश)
{
  hb_subset_plan_t *plan;
  अगर (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, निवेश))))
    वापस nullptr;

  अगर (unlikely (plan->in_error ()))
  {
    hb_subset_plan_destroy (plan);
    वापस nullptr;
  }

  वापस plan;
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
व्योम
hb_subset_plan_destroy (hb_subset_plan_t *plan)
{
  अगर (!hb_object_destroy (plan)) वापस;

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
hb_subset_plan_old_to_new_glyph_mapping (स्थिर hb_subset_plan_t *plan)
{
  वापस plan->glyph_map;
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
hb_subset_plan_new_to_old_glyph_mapping (स्थिर hb_subset_plan_t *plan)
{
  वापस plan->reverse_glyph_map;
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
hb_subset_plan_unicode_to_old_glyph_mapping (स्थिर hb_subset_plan_t *plan)
{
  वापस plan->codepoint_to_glyph;
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
  वापस hb_object_reference (plan);
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
hb_subset_plan_set_user_तथ्य (hb_subset_plan_t   *plan,
                              hb_user_तथ्य_कुंजी_t *key,
                              व्योम               *तथ्य,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  वापस hb_object_set_user_तथ्य (plan, key, तथ्य, destroy, replace);
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
व्योम *
hb_subset_plan_get_user_तथ्य (स्थिर hb_subset_plan_t *plan,
                              hb_user_तथ्य_कुंजी_t     *key)
{
  वापस hb_object_get_user_तथ्य (plan, key);
}

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

#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-accelerator.hh"
#ضمن "hb-map.hh"
#ضمن "hb-multimap.hh"
#ضمن "hb-set.hh"
#ضمن "hb-subset.h"
#ضمن "hb-unicode.h"

#ضمن "hb-ot-cmap-table.hh"
#ضمن "hb-ot-glyf-table.hh"
#ضمن "hb-ot-layout-base-table.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/COLR/colrv1-closure.hh"
#ضمن "OT/Color/CPAL/CPAL.hh"
#ضمن "hb-ot-var-fvar-table.hh"
#ضمن "hb-ot-var-varc-table.hh"
#ضمن "hb-ot-stat-table.hh"
#ضمن "hb-ot-math-table.hh"

hb_subset_accelerator_t::~hb_subset_accelerator_t ()
{
  إذا (cmap_cache && destroy_cmap_cache)
    destroy_cmap_cache ((فراغ*) cmap_cache);

#که_نه_تعریف_شوی HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#د_که_پای
  hb_face_destroy (سرچینه);
}


#که_نه_تعریف_شوی HB_NO_SUBSET_CFF
ساكن مضمن منطقي
جمع_کړه_cff_seac_components (ثابت OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_سېټ_t *gids_منزل_retain)
{
  hb_codepoint_t بنسټ_gid, accent_gid;
  إذا (cff.get_seac_components (gid, &بنسټ_gid, &accent_gid))
  {
    gids_منزل_retain->جمع_کړه (بنسټ_gid);
    gids_منزل_retain->جمع_کړه (accent_gid);
    أرجع ریښتیا;
  }
  أرجع خطأ;
}
#د_که_پای

ساكن فراغ
_remap_palette_indexes (ثابت hb_سېټ_t *palette_indexes,
			hb_map_t       *mapping /*بهر*/)
{
  بینښه new_idx = 0;
  لكل (بینښه palette_شاخص : palette_indexes->iter ())
  {
    إذا (palette_شاخص == 0xFFFF)
    {
      mapping->سېټ (palette_شاخص, palette_شاخص);
      استمر;
    }
    mapping->سېټ (palette_شاخص, new_idx);
    new_idx++;
  }
}

فراغ
remap_indexes (ثابت hb_سېټ_t *indexes,
               hb_map_t       *mapping /*بهر*/)
{
  لكل (خپلکاری _ : + hb_enumerate (indexes->iter ()))
    mapping->سېټ (_.second, _.لومړی);
}

ساكن مضمن فراغ
cmap_تړون (hb_face_t	   *face,
	       ثابت hb_سېټ_t	   *unicodes,
	       hb_سېټ_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.تړون_glyphs (unicodes, glyphset);
}

ساكن فراغ colr_تړون (hb_subset_plan_t* plan,
                           hb_سېټ_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->سرچینه);
  إذا (!colr.is_valid ()) أرجع;

  hb_سېټ_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_سېټ_t glyphset_colrv0;
  لكل (hb_codepoint_t gid : *glyphs_colred)
    colr.تړون_glyphs (gid, &glyphset_colrv0);

  glyphs_colred->union_ (glyphset_colrv0);

  //closure for COLRv1
  hb_سېټ_t variation_indices, delta_سېټ_indices;
  colr.تړون_for_V1 (glyphs_colred, &layer_indices, &palette_indices, &variation_indices, &delta_سېټ_indices);

  colr.تړون_V0palette_indices (glyphs_colred, &palette_indices);
  remap_indexes (&layer_indices, &plan->colrv1_layers);
  _remap_palette_indexes (&palette_indices, &plan->colr_palettes);

#که_نه_تعریف_شوی HB_NO_VAR
  /* colr variation indices mapping during planning phase:
   * generate colrv1_variation_idx_delta_map. When delta set index map is not
   * included, it's a mapping from varIdx-> (new varIdx,delta). Otherwise, it's
   * a mapping from old delta set idx-> (new delta set idx, delta). Mapping
   * delta set indices is the same as gid mapping.
   * Besides, we need to generate a delta set idx-> new var_idx map for updating
   * delta set index map if exists. This map will be updated again after
   * instancing. */
  إذا (colr.has_var_store () && variation_indices)
  {
    ثابت OT::توکی_Variation_Store &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    بینښه subtable_شمېر = var_store.get_sub_جدول_شمېر ();
    generate_varstore_inner_maps (variation_indices, subtable_شمېر, plan->colrv1_varstore_inner_maps);

    إذا (!plan->all_axes_pinned)
      remap_variation_indices (var_store,
                              variation_indices,
                              plan->normalized_coords,
                              خطأ, /* no need to calculate delta for COLR during planning */
                              plan->all_axes_pinned,
                              plan->colrv1_variation_idx_delta_map);
  }

  /* Sentinel-only maps still need their delta-set indices remapped, even
   * when there are no live variation rows or no variation store. */
  إذا (!plan->all_axes_pinned && colr.has_delta_سېټ_شاخص_map ())
    remap_colrv1_delta_سېټ_شاخص_indices (colr.get_delta_سېټ_شاخص_map (),
                                         delta_سېټ_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#د_که_پای
}

ساكن مضمن فراغ
math_تړون (hb_subset_plan_t *plan,
               hb_سېټ_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->سرچینه_جدول<OT::MATH> ();
  إذا (math->has_معلومات ())
    math->تړون_glyphs (glyphset);
  math.destroy ();
}

ساكن مضمن فراغ
_remove_invalid_gids (hb_سېټ_t *glyphs,
		      بینښه صحيح عدد_glyphs)
{
  glyphs->del_range (عدد_glyphs, HB_سېټ_ارزښت_INVALID);
}

کينډۍ<منطقي GID_ALWAYS_EXISTS = خطأ, ډول_نوم I, ډول_نوم F, ډول_نوم G, hb_requires (hb_is_iterator (I))>
ساكن فراغ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_منزل_gid_for_iterator,
                            G unicode_منزل_gid_general)
{
  لكل (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_منزل_gid_for_iterator(cp);
    إذا (!GID_ALWAYS_EXISTS && gid == HB_MAP_ارزښت_INVALID)
    {
      DEBUG_پیغام(SUBSET, تش_اشاره, "Drop U+%04X; no gid", cp);
      استمر;
    }

    plan->codepoint_منزل_glyph->سېټ (cp, gid);
    plan->unicode_منزل_new_gid_list.push (hb_pair (cp, gid));
  }
}

کينډۍ<منطقي GID_ALWAYS_EXISTS = خطأ, ډول_نوم I, ډول_نوم F, hb_requires (hb_is_iterator (I))>
ساكن فراغ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_منزل_gid_for_iterator)
{
  _fill_unicode_and_glyph_map(plan, unicode_iterator, unicode_منزل_gid_for_iterator, unicode_منزل_gid_for_iterator);
}

/*
 * Finds additional unicode codepoints which are reachable from the input unicode set.
 * Currently this adds in mirrored variants (needed for bidi) of any input unicodes.
 */
ساكن hb_سېټ_t
unicode_تړون (ثابت hb_سېټ_t* unicodes, منطقي bidi_تړون) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_سېټ_t بهر = *unicodes;
  إذا (!bidi_تړون) أرجع بهر;

  إذا (بهر.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    أرجع بهر;
  }

  خپلکاری unicode_funcs = hb_unicode_funcs_get_default ();
  لكل (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   إذا (unlikely (mirror != cp)) {
     بهر.جمع_کړه(mirror);
   }
  }

  أرجع بهر;
}

ساكن فراغ
populate_unicodes_منزل_retain (ثابت hb_سېټ_t *unicodes_دننه,
                              ثابت hb_سېټ_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_سېټ_t unicodes = unicode_تړون(unicodes_دننه,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_تړون));

  OT::cmap_accelerator_t cmap (plan->سرچینه);
  بینښه اندازه_پوله = plan->سرچینه->get_عدد_glyphs ();

  إذا (glyphs->is_empty () && unicodes.get_population () < اندازه_پوله)
  {

    ثابت hb_map_t* unicode_منزل_gid_2 = تش_اشاره;
    إذا (plan->accelerator)
      unicode_منزل_gid_2 = &plan->accelerator->unicode_منزل_gid_2;

    // This is approach to collection is faster, but can only be used  if glyphs
    // are not being explicitly added to the subset and the input unicodes set is
    // not excessively large (eg. an inverted set).
    plan->unicode_منزل_new_gid_list.alloc (unicodes.get_population ());
    إذا (!unicode_منزل_gid_2) {
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        hb_codepoint_t gid;
        إذا (!cmap.get_nominal_glyph (cp, &gid)) {
          أرجع HB_MAP_ارزښت_INVALID;
        }
        أرجع gid;
      });
    } وإلا {
      // Use in memory unicode to gid map it's faster then looking up from
      // the map. This code is mostly duplicated from above to avoid doing
      // conditionals on the presence of the unicode_to_gid map each
      // iteration.
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        أرجع unicode_منزل_gid_2->get (cp);
      });
    }
  }
  وإلا
  {
    // This approach is slower, but can handle adding in glyphs to the subset and will match
    // them with cmap entries.

    hb_map_t unicode_glyphid_map_storage;
    hb_سېټ_t cmap_unicodes_storage;
    ثابت hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    ثابت hb_سېټ_t* cmap_unicodes = &cmap_unicodes_storage;

    إذا (!plan->accelerator) {
      cmap.collect_mapping (&cmap_unicodes_storage, &unicode_glyphid_map_storage);
      plan->unicode_منزل_new_gid_list.alloc (hb_min(unicodes.get_population ()
                                                  + glyphs->get_population (),
                                                  cmap_unicodes->get_population ()));
    } وإلا {
      unicode_glyphid_map = &plan->accelerator->unicode_منزل_gid_2;
      cmap_unicodes = &plan->accelerator->unicodes;
    }

    إذا (plan->accelerator &&
	unicodes.get_population () < cmap_unicodes->get_population () &&
	glyphs->get_population () < cmap_unicodes->get_population ())
    {
      plan->codepoint_منزل_glyph->alloc (unicodes.get_population () + glyphs->get_population ());

      خپلکاری &gid_منزل_unicodes = plan->accelerator->gid_منزل_unicodes;

      لكل (hb_codepoint_t gid : *glyphs)
      {
        خپلکاری unicodes = gid_منزل_unicodes.get (gid);
        _fill_unicode_and_glyph_map<ریښتیا>(plan, unicodes, [&] (hb_codepoint_t cp) {
          أرجع gid;
        },
        [&] (hb_codepoint_t cp) {
          أرجع unicode_glyphid_map->get(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	إذا (plan->codepoint_منزل_glyph->has (cp))
          أرجع HB_MAP_ارزښت_INVALID;

        أرجع unicode_glyphid_map->get(cp);
      },
      [&] (hb_codepoint_t cp) {
          أرجع unicode_glyphid_map->get(cp);
      });

      plan->unicode_منزل_new_gid_list.qsort ();
    }
    وإلا
    {
      plan->codepoint_منزل_glyph->alloc (cmap_unicodes->get_population ());
      hb_codepoint_t لومړی = HB_سېټ_ارزښت_INVALID, وروستی = HB_سېټ_ارزښت_INVALID;
      لكل (; cmap_unicodes->بل_range (&لومړی, &وروستی); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(لومړی, وروستی + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  إذا (!unicodes.has (cp) && !glyphs->has (gid))
	    أرجع HB_MAP_ارزښت_INVALID;
          أرجع gid;
        },
        [&] (hb_codepoint_t cp) {
          أرجع unicode_glyphid_map->get(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    بینښه عدد_glyphs = plan->سرچینه->get_عدد_glyphs ();
    hb_codepoint_t لومړی = HB_سېټ_ارزښت_INVALID, وروستی = HB_سېټ_ارزښت_INVALID;
    لكل (; glyphs->بل_range (&لومړی, &وروستی); )
    {
      إذا (لومړی >= عدد_glyphs)
	اكسر;
      إذا (وروستی >= عدد_glyphs)
        وروستی = عدد_glyphs - 1;
      plan->_glyphset_gsub.جمع_کړه_range (لومړی, وروستی);
    }
  }

  خپلکاری &arr = plan->unicode_منزل_new_gid_list;
  إذا (arr.اوږدتیا)
  {
    plan->unicodes.جمع_کړه_sorted_array (&arr.arrayZ->لومړی, arr.اوږدتیا, حجم (*arr.arrayZ));
    plan->_glyphset_gsub.جمع_کړه_array (&arr.arrayZ->second, arr.اوږدتیا, حجم (*arr.arrayZ));
  }

  // Variation selectors don't have glyphs associated with them in the cmap so they will have been filtered out above
  // but should still be retained. Add them back here.

  // However, the min and max codepoints for OS/2 should be calculated without considering variation selectors,
  // so record those first.
  plan->os2_info.min_cmap_codepoint = plan->unicodes.get_min();
  plan->os2_info.max_cmap_codepoint = plan->unicodes.get_max();

  hb_سېټ_t variation_selectors_منزل_retain;
  cmap.collect_variation_selectors(&variation_selectors_منزل_retain);
  + variation_selectors_منزل_retain.iter()
  | hb_filter(unicodes)
  | hb_sink(&plan->unicodes)
  ;
}

ساكن بینښه
glyf_جمع_کړه_gid_and_children (ثابت OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_سېټ_t *gids_منزل_retain,
			    صحيح operation_شمېر,
			    بینښه ژوروالی = 0)
{
  /* Check if is already visited */
  إذا (gids_منزل_retain->has (gid)) أرجع operation_شمېر;

  gids_منزل_retain->جمع_کړه (gid);

  إذا (unlikely (ژوروالی++ > HB_MAX_NESTING_LEVEL)) أرجع operation_شمېر;
  إذا (unlikely (--operation_شمېر < 0)) أرجع operation_شمېر;

  خپلکاری glyph = glyf.glyph_for_gid (gid);

  لكل (خپلکاری &توکی : glyph.get_مرکب_iterator ())
    operation_شمېر =
      glyf_جمع_کړه_gid_and_children (glyf,
				  توکی.get_gid (glyph.is_extended ()),
				  gids_منزل_retain,
				  operation_شمېر,
				  ژوروالی);

  أرجع operation_شمېر;
}

ساكن فراغ
nameid_تړون (hb_subset_plan_t* plan,
		 hb_سېټ_t* drop_tables)
{
#که_نه_تعریف_شوی HB_NO_STYLE
  إذا (!drop_tables->has (HB_OT_نښان_STAT))
    plan->سرچینه->جدول.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR
  إذا (!plan->all_axes_pinned)
    plan->سرچینه->جدول.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_شاخص_نښان_map, &plan->name_ids);
#د_که_پای
#که_نه_تعریف_شوی HB_NO_COLOR
  إذا (!drop_tables->has (HB_OT_نښان_CPAL))
    plan->سرچینه->جدول.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#د_که_پای

#که_نه_تعریف_شوی HB_NO_SUBSET_LAYOUT
  layout_nameid_تړون(plan, drop_tables);
#د_که_پای
}

ساكن فراغ
populate_gids_منزل_retain (hb_subset_plan_t* plan,
		          hb_سېټ_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->سرچینه);
#که_نه_تعریف_شوی HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t ډېری_cff (plan->accelerator ? تش_اشاره : plan->سرچینه);
  ثابت OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.get () : &ډېری_cff);
#د_که_پای

  plan->_glyphset_gsub.جمع_کړه (0); // Not-def

  cmap_تړون (plan->سرچینه, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  إذا (!drop_tables->has (HB_OT_نښان_MATH))
  {
    math_تړون (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->سرچینه->get_عدد_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#که_نه_تعریف_شوی HB_NO_SUBSET_LAYOUT
  layout_populate_gids_منزل_retain(plan, drop_tables);
#د_که_پای

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->سرچینه->get_عدد_glyphs ());

  hb_سېټ_t cur_glyphset = plan->_glyphset_gsub;
  إذا (!drop_tables->has (HB_OT_نښان_COLR))
  {
    colr_تړون (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->سرچینه->get_عدد_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES
  إذا (!drop_tables->has (OT::VARC::جدول_نښان))
  {
    إذا (unlikely (!plan->سرچینه->جدول.VARC->تړون_glyphs (&cur_glyphset)))
    {
      plan->check_success (خطأ);
      أرجع;
    }
    _remove_invalid_gids (&cur_glyphset, plan->سرچینه->get_عدد_glyphs ());
  }
#د_که_پای

  plan->_glyphset_varced = cur_glyphset;

  nameid_تړون (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  إذا (glyf.has_معلومات ())
    لكل (hb_codepoint_t gid : cur_glyphset)
      glyf_جمع_کړه_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_مرکب_OPERATIONS_فی_GLYPH);
  وإلا
    plan->_glyphset.union_ (cur_glyphset);
#که_نه_تعریف_شوی HB_NO_SUBSET_CFF
  إذا (!plan->accelerator || plan->accelerator->has_seac)
  {
    منطقي has_seac = خطأ;
    إذا (cff->is_valid ())
      لكل (hb_codepoint_t gid : cur_glyphset)
	إذا (جمع_کړه_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = ریښتیا;
    plan->has_seac = has_seac;
  }
#د_که_پای

  _remove_invalid_gids (&plan->_glyphset, plan->سرچینه->get_عدد_glyphs ());

#که_نه_تعریف_شوی HB_NO_VAR
#که_نه_تعریف_شوی HB_NO_SUBSET_LAYOUT
  إذا (!drop_tables->has (HB_OT_نښان_GDEF))
    collect_layout_variation_indices (plan);
#د_که_پای
#د_که_پای
}

ساكن منطقي
_create_glyph_map_gsub (ثابت hb_سېټ_t* glyph_سېټ_gsub,
                        ثابت hb_map_t* glyph_map,
                        hb_map_t* بهر,
                        hb_ویکټور_t<hb_codepoint_t>* بهر_flat)
{
  إذا (unlikely (glyph_سېټ_gsub->دننه_error ()))
    أرجع خطأ;

  hb_codepoint_t max_gid = HB_سېټ_ارزښت_INVALID;
  hb_سېټ_پخوانی (glyph_سېټ_gsub, &max_gid);
  بینښه flat_اندازه = max_gid == HB_سېټ_ارزښت_INVALID ? 0 : max_gid + 1;
  إذا (unlikely (!بهر_flat->resize_dirty (flat_اندازه)))
    أرجع خطأ;
  hb_memset (بهر_flat->arrayZ, 0xFF, flat_اندازه * حجم (hb_codepoint_t));

  بهر->alloc (glyph_سېټ_gsub->get_population ());
  لكل (خپلکاری gid : *glyph_سېټ_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->get (gid);
    بهر->سېټ (gid, new_gid);
    بهر_flat->arrayZ[gid] = new_gid;
  }
  أرجع !بهر->دننه_error ();
}

ساكن منطقي
create_old_gid_منزل_new_gid_map (ثابت hb_face_t *face,
				منطقي		 retain_gids,
				ثابت hb_سېټ_t	*all_gids_منزل_retain,
                                ثابت hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /*بهر*/
				hb_map_t	*سرچپه_glyph_map, /*بهر*/
				hb_sorted_ویکټور_t<hb_codepoint_pair_t> *new_منزل_old_gid_list /*بهر*/,
				بینښه صحيح	*عدد_glyphs /*بهر*/)
{
  بینښه راوباسه = all_gids_منزل_retain->get_population ();
  سرچپه_glyph_map->alloc (راوباسه);
  glyph_map->alloc (راوباسه);
  new_منزل_old_gid_list->alloc (راوباسه);

  إذا (*requested_glyph_map)
  {
    hb_سېټ_t new_gids(requested_glyph_map->values());
    إذا (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_پیغام (SUBSET, تش_اشاره, "The provided custom glyph mapping is not unique.");
      أرجع خطأ;
    }

    إذا (retain_gids)
    {
      DEBUG_پیغام (SUBSET, تش_اشاره,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      أرجع خطأ;
    }

    hb_codepoint_t max_glyph = 0;
    hb_سېټ_t پاتې_شوی;
    لكل (خپلکاری old_gid : all_gids_منزل_retain->iter ())
    {
      إذا (old_gid == 0) {
	new_منزل_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        استمر;
      }

      hb_codepoint_t* new_gid;
      إذا (!requested_glyph_map->has (old_gid, &new_gid))
      {
        پاتې_شوی.جمع_کړه(old_gid);
        استمر;
      }

      إذا (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_منزل_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_منزل_old_gid_list->qsort ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    لكل (خپلکاری old_gid : پاتې_شوی)
      new_منزل_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *عدد_glyphs = max_glyph + 1;
  }
  وإلا إذا (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_منزل_retain), (hb_codepoint_t) 0)
    | hb_sink (new_منزل_old_gid_list)
    ;
    *عدد_glyphs = new_منزل_old_gid_list->اوږدتیا;
  }
  وإلا
  {
    + hb_iter (all_gids_منزل_retain)
    | hb_map ([] (hb_codepoint_t _) {
		أرجع hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_منزل_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_سېټ_ارزښت_INVALID;
    hb_سېټ_پخوانی (all_gids_منزل_retain, &max_glyph);

    *عدد_glyphs = max_glyph + 1;
  }

  سرچپه_glyph_map->alloc (سرچپه_glyph_map->get_population () + new_منزل_old_gid_list->اوږدتیا);
  + hb_iter (new_منزل_old_gid_list)
  | hb_sink (سرچپه_glyph_map)
  ;
  glyph_map->alloc (glyph_map->get_population () + new_منزل_old_gid_list->اوږدتیا);
  + hb_iter (new_منزل_old_gid_list)
  | hb_map (&hb_codepoint_pair_t::reverse)
  | hb_sink (glyph_map)
  ;

  أرجع ریښتیا;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    ثابت hb_subset_ننوت_t *ننوت)
{
  successful = ریښتیا;
  flags = ننوت->flags;

  unicode_منزل_new_gid_list.init ();

  name_ids = *ننوت->sets.name_ids;
  name_languages = *ننوت->sets.name_languages;
  layout_features = *ننوت->sets.layout_features;
  layout_scripts = *ننوت->sets.layout_scripts;
  glyphs_requested = *ننوت->sets.glyphs;
  drop_tables = *ننوت->sets.drop_tables;
  no_subset_tables = *ننوت->sets.no_subset_tables;
  سرچینه = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_منزل_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  سرچپه_glyph_map = hb_map_create ();

  gsub_ورننباسه_catch_all_feature_variation_rec = خطأ;
  gpos_ورننباسه_catch_all_feature_variation_rec = خطأ;
  gdef_varstore_inner_maps.init ();

  user_axes_location = ننوت->axes_location;
  all_axes_pinned = خطأ;
  pinned_at_default = ریښتیا;
  has_gdef_varstore = خطأ;
  has_avar2 = خطأ;

#که_تعریف_شوی HB_EXPERIMENTAL_API
  لكل (خپلکاری _ : ننوت->name_جدول_overrides)
  {
    hb_بایټونه_t name_بایټونه = _.second;
    بینښه len = name_بایټونه.اوږدتیا;
    حرف *name_str = (حرف *) hb_malloc (len);
    إذا (unlikely (!check_success (name_str)))
      اكسر;

    hb_memcpy (name_str, name_بایټونه.arrayZ, len);
    name_جدول_overrides.سېټ (_.لومړی, hb_بایټونه_t (name_str, len));
  }
#د_که_پای

  فراغ* accel = hb_face_get_user_معلومات(face, hb_subset_accelerator_t::user_معلومات_کلی());

  attach_accelerator_معلومات = ننوت->attach_accelerator_معلومات;
  force_long_loca = ننوت->force_long_loca;
#که_تعریف_شوی HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#د_که_پای

  إذا (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  إذا (unlikely (دننه_error ()))
    أرجع;

#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES
  إذا (!ننوت->sets.drop_tables->has (OT::VARC::جدول_نښان) &&
      ننوت->sets.no_subset_tables->has (OT::VARC::جدول_نښان) &&
      !(ننوت->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->جدول.VARC->has_معلومات ())
  {
    check_success (خطأ);
    أرجع;
  }
#د_که_پای

#که_نه_تعریف_شوی HB_NO_VAR
  إذا (!check_success (normalize_axes_location (face, هذا)))
      أرجع;
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES
  إذا (!user_axes_location.is_empty () &&
      !ننوت->sets.drop_tables->has (OT::VARC::جدول_نښان) &&
      ننوت->sets.no_subset_tables->has (OT::VARC::جدول_نښان) &&
      face->جدول.VARC->has_معلومات ())
  {
    check_success (خطأ);
    أرجع;
  }
#د_که_پای

  populate_unicodes_منزل_retain (ننوت->sets.unicodes, ننوت->sets.glyphs, هذا);

  populate_gids_منزل_retain (هذا, ننوت->sets.drop_tables);
  إذا (unlikely (دننه_error ()))
    أرجع;

  إذا (!check_success(create_old_gid_منزل_new_gid_map(
          face,
          ننوت->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &ننوت->glyph_map,
          glyph_map,
          سرچپه_glyph_map,
	  &new_منزل_old_gid_list,
          &عدد_output_glyphs_2))) {
    أرجع;
  }

#که_تعریف_شوی HB_EXPERIMENTAL_API  
  إذا ((ننوت->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (ننوت->flags & HB_SUBSET_FLAGS_RETAIN_عدد_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    عدد_output_glyphs_2 = سرچینه->get_عدد_glyphs ();
  }
#د_که_پای

  إذا (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    أرجع;

  // Now that we have old to new gid map update the unicode to new gid list.
  لكل (بینښه i = 0; i < unicode_منزل_new_gid_list.اوږدتیا; i++)
  {
    // Use raw array access for performance.
    unicode_منزل_new_gid_list.arrayZ[i].second =
        glyph_map->get(unicode_منزل_new_gid_list.arrayZ[i].second);
  }

  bounds_پلنوالی_vec.resize_dirty  (عدد_output_glyphs_2);
  لكل (خپلکاری &v : bounds_پلنوالی_vec)
    v = 0xFFFFFFFF;
  bounds_لوړوالی_vec.resize_dirty  (عدد_output_glyphs_2);
  لكل (خپلکاری &v : bounds_لوړوالی_vec)
    v = 0xFFFFFFFF;

#که_نه_تعریف_شوی HB_NO_SUBSET_LAYOUT    
  إذا (!drop_tables.has (HB_OT_نښان_GDEF))
    remap_کارول_شوی_mark_sets (هذا, کارول_شوی_mark_sets_map);
#د_که_پای

#که_نه_تعریف_شوی HB_NO_VAR
#که_نه_تعریف_شوی HB_NO_بنسټ
  إذا (!drop_tables.has (HB_OT_نښان_بنسټ))
    collect_بنسټ_variation_indices (هذا);
#د_که_پای
#د_که_پای

  إذا (unlikely (دننه_error ()))
    أرجع;

#إذا !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (هذا);
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR
  إذا (!check_success (get_instance_glyphs_contour_points (هذا)))
      أرجع;
#د_که_پای

  إذا (attach_accelerator_معلومات)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (سرچینه,
				       *codepoint_منزل_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#تعریف_کړه HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.دننه_error ());
#ضمن "hb-subset-plan-member-list.hh"
#تعریف_لغوه HB_SUBSET_PLAN_MEMBER
}

hb_subset_plan_t::~hb_subset_plan_t()
{
  hb_face_destroy (dest);

  hb_map_destroy (codepoint_منزل_glyph);
  hb_map_destroy (glyph_map);
  hb_map_destroy (سرچپه_glyph_map);
#که_نه_تعریف_شوی HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#د_که_پای
  hb_face_destroy (سرچینه);

#که_تعریف_شوی HB_EXPERIMENTAL_API
  لكل (خپلکاری _ : name_جدول_overrides.iter_ref ())
    _.second.fini ();
#د_که_پای

  إذا (inprogress_accelerator)
    hb_subset_accelerator_t::destroy ((فراغ*) inprogress_accelerator);
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
                               ثابت hb_subset_ننوت_t *ننوت)
{
  hb_subset_plan_t *plan;
  إذا (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, ننوت))))
    أرجع تش_اشاره;

  إذا (unlikely (plan->دننه_error ()))
  {
    hb_subset_plan_destroy (plan);
    أرجع تش_اشاره;
  }

  أرجع plan;
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
فراغ
hb_subset_plan_destroy (hb_subset_plan_t *plan)
{
  إذا (!hb_object_destroy (plan)) أرجع;

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
hb_subset_plan_old_منزل_new_glyph_mapping (ثابت hb_subset_plan_t *plan)
{
  أرجع plan->glyph_map;
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
hb_subset_plan_new_منزل_old_glyph_mapping (ثابت hb_subset_plan_t *plan)
{
  أرجع plan->سرچپه_glyph_map;
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
hb_subset_plan_unicode_منزل_old_glyph_mapping (ثابت hb_subset_plan_t *plan)
{
  أرجع plan->codepoint_منزل_glyph;
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
  أرجع hb_object_reference (plan);
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
hb_subset_plan_سېټ_user_معلومات (hb_subset_plan_t   *plan,
                              hb_user_معلومات_کلی_t *key,
                              فراغ               *معلومات,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  أرجع hb_object_سېټ_user_معلومات (plan, key, معلومات, destroy, replace);
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
فراغ *
hb_subset_plan_get_user_معلومات (ثابت hb_subset_plan_t *plan,
                              hb_user_معلومات_کلی_t     *key)
{
  أرجع hb_object_get_user_معلومات (plan, key);
}

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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#جيڪڏهن_پورو
  hb_face_destroy (ذريعو);
}


#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF
ساكن مضمن منطقي
جوڙيو_cff_seac_components (ثابت OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_سيٽ_t *gids_منزل_retain)
{
  hb_codepoint_t بنياد_gid, accent_gid;
  إذا (cff.get_seac_components (gid, &بنياد_gid, &accent_gid))
  {
    gids_منزل_retain->جوڙيو (بنياد_gid);
    gids_منزل_retain->جوڙيو (accent_gid);
    أرجع سچ;
  }
  أرجع خطأ;
}
#جيڪڏهن_پورو

ساكن فراغ
_remap_palette_indexes (ثابت hb_سيٽ_t *palette_indexes,
			hb_map_t       *mapping /*ٻاهريون*/)
{
  بنانشان new_idx = 0;
  لكل (بنانشان palette_اشاريو : palette_indexes->iter ())
  {
    إذا (palette_اشاريو == 0xFFFF)
    {
      mapping->سيٽ (palette_اشاريو, palette_اشاريو);
      استمر;
    }
    mapping->سيٽ (palette_اشاريو, new_idx);
    new_idx++;
  }
}

فراغ
remap_indexes (ثابت hb_سيٽ_t *indexes,
               hb_map_t       *mapping /*ٻاهريون*/)
{
  لكل (خودڪار _ : + hb_enumerate (indexes->iter ()))
    mapping->سيٽ (_.second, _.پهريون);
}

ساكن مضمن فراغ
cmap_گهيرو (hb_face_t	   *face,
	       ثابت hb_سيٽ_t	   *unicodes,
	       hb_سيٽ_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.گهيرو_glyphs (unicodes, glyphset);
}

ساكن فراغ colr_گهيرو (hb_subset_plan_t* plan,
                           hb_سيٽ_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->ذريعو);
  إذا (!colr.is_valid ()) أرجع;

  hb_سيٽ_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_سيٽ_t glyphset_colrv0;
  لكل (hb_codepoint_t gid : *glyphs_colred)
    colr.گهيرو_glyphs (gid, &glyphset_colrv0);

  glyphs_colred->union_ (glyphset_colrv0);

  //closure for COLRv1
  hb_سيٽ_t variation_indices, delta_سيٽ_indices;
  colr.گهيرو_for_V1 (glyphs_colred, &layer_indices, &palette_indices, &variation_indices, &delta_سيٽ_indices);

  colr.گهيرو_V0palette_indices (glyphs_colred, &palette_indices);
  remap_indexes (&layer_indices, &plan->colrv1_layers);
  _remap_palette_indexes (&palette_indices, &plan->colr_palettes);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
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
    ثابت OT::شي_Variation_Store &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    بنانشان subtable_ڳڻپ = var_store.get_sub_جدول_ڳڻپ ();
    generate_varstore_inner_maps (variation_indices, subtable_ڳڻپ, plan->colrv1_varstore_inner_maps);

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
  إذا (!plan->all_axes_pinned && colr.has_delta_سيٽ_اشاريو_map ())
    remap_colrv1_delta_سيٽ_اشاريو_indices (colr.get_delta_سيٽ_اشاريو_map (),
                                         delta_سيٽ_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#جيڪڏهن_پورو
}

ساكن مضمن فراغ
math_گهيرو (hb_subset_plan_t *plan,
               hb_سيٽ_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->ذريعو_جدول<OT::MATH> ();
  إذا (math->has_معلومات ())
    math->گهيرو_glyphs (glyphset);
  math.destroy ();
}

ساكن مضمن فراغ
_remove_invalid_gids (hb_سيٽ_t *glyphs,
		      بنانشان صحيح انگ_glyphs)
{
  glyphs->del_range (انگ_glyphs, HB_سيٽ_قدر_INVALID);
}

سانچو<منطقي GID_ALWAYS_EXISTS = خطأ, قسم_نالو I, قسم_نالو F, قسم_نالو G, hb_requires (hb_is_iterator (I))>
ساكن فراغ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_منزل_gid_for_iterator,
                            G unicode_منزل_gid_general)
{
  لكل (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_منزل_gid_for_iterator(cp);
    إذا (!GID_ALWAYS_EXISTS && gid == HB_MAP_قدر_INVALID)
    {
      DEBUG_نياپو(SUBSET, خالي_اشارو, "Drop U+%04X; no gid", cp);
      استمر;
    }

    plan->codepoint_منزل_glyph->سيٽ (cp, gid);
    plan->unicode_منزل_new_gid_list.push (hb_pair (cp, gid));
  }
}

سانچو<منطقي GID_ALWAYS_EXISTS = خطأ, قسم_نالو I, قسم_نالو F, hb_requires (hb_is_iterator (I))>
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
ساكن hb_سيٽ_t
unicode_گهيرو (ثابت hb_سيٽ_t* unicodes, منطقي bidi_گهيرو) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_سيٽ_t ٻاهريون = *unicodes;
  إذا (!bidi_گهيرو) أرجع ٻاهريون;

  إذا (ٻاهريون.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    أرجع ٻاهريون;
  }

  خودڪار unicode_funcs = hb_unicode_funcs_get_default ();
  لكل (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   إذا (unlikely (mirror != cp)) {
     ٻاهريون.جوڙيو(mirror);
   }
  }

  أرجع ٻاهريون;
}

ساكن فراغ
populate_unicodes_منزل_retain (ثابت hb_سيٽ_t *unicodes_اندريون,
                              ثابت hb_سيٽ_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_سيٽ_t unicodes = unicode_گهيرو(unicodes_اندريون,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_گهيرو));

  OT::cmap_accelerator_t cmap (plan->ذريعو);
  بنانشان سائيز_حد = plan->ذريعو->get_انگ_glyphs ();

  إذا (glyphs->is_empty () && unicodes.get_population () < سائيز_حد)
  {

    ثابت hb_map_t* unicode_منزل_gid_2 = خالي_اشارو;
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
          أرجع HB_MAP_قدر_INVALID;
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
    hb_سيٽ_t cmap_unicodes_storage;
    ثابت hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    ثابت hb_سيٽ_t* cmap_unicodes = &cmap_unicodes_storage;

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

      خودڪار &gid_منزل_unicodes = plan->accelerator->gid_منزل_unicodes;

      لكل (hb_codepoint_t gid : *glyphs)
      {
        خودڪار unicodes = gid_منزل_unicodes.get (gid);
        _fill_unicode_and_glyph_map<سچ>(plan, unicodes, [&] (hb_codepoint_t cp) {
          أرجع gid;
        },
        [&] (hb_codepoint_t cp) {
          أرجع unicode_glyphid_map->get(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	إذا (plan->codepoint_منزل_glyph->has (cp))
          أرجع HB_MAP_قدر_INVALID;

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
      hb_codepoint_t پهريون = HB_سيٽ_قدر_INVALID, آخري = HB_سيٽ_قدر_INVALID;
      لكل (; cmap_unicodes->ايندڙ_range (&پهريون, &آخري); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(پهريون, آخري + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  إذا (!unicodes.has (cp) && !glyphs->has (gid))
	    أرجع HB_MAP_قدر_INVALID;
          أرجع gid;
        },
        [&] (hb_codepoint_t cp) {
          أرجع unicode_glyphid_map->get(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    بنانشان انگ_glyphs = plan->ذريعو->get_انگ_glyphs ();
    hb_codepoint_t پهريون = HB_سيٽ_قدر_INVALID, آخري = HB_سيٽ_قدر_INVALID;
    لكل (; glyphs->ايندڙ_range (&پهريون, &آخري); )
    {
      إذا (پهريون >= انگ_glyphs)
	اكسر;
      إذا (آخري >= انگ_glyphs)
        آخري = انگ_glyphs - 1;
      plan->_glyphset_gsub.جوڙيو_range (پهريون, آخري);
    }
  }

  خودڪار &arr = plan->unicode_منزل_new_gid_list;
  إذا (arr.ڊيگهائي)
  {
    plan->unicodes.جوڙيو_sorted_array (&arr.arrayZ->پهريون, arr.ڊيگهائي, حجم (*arr.arrayZ));
    plan->_glyphset_gsub.جوڙيو_array (&arr.arrayZ->second, arr.ڊيگهائي, حجم (*arr.arrayZ));
  }

  // Variation selectors don't have glyphs associated with them in the cmap so they will have been filtered out above
  // but should still be retained. Add them back here.

  // However, the min and max codepoints for OS/2 should be calculated without considering variation selectors,
  // so record those first.
  plan->os2_info.min_cmap_codepoint = plan->unicodes.get_min();
  plan->os2_info.max_cmap_codepoint = plan->unicodes.get_max();

  hb_سيٽ_t variation_selectors_منزل_retain;
  cmap.collect_variation_selectors(&variation_selectors_منزل_retain);
  + variation_selectors_منزل_retain.iter()
  | hb_filter(unicodes)
  | hb_sink(&plan->unicodes)
  ;
}

ساكن بنانشان
glyf_جوڙيو_gid_and_children (ثابت OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_سيٽ_t *gids_منزل_retain,
			    صحيح operation_ڳڻپ,
			    بنانشان اونهائي = 0)
{
  /* Check if is already visited */
  إذا (gids_منزل_retain->has (gid)) أرجع operation_ڳڻپ;

  gids_منزل_retain->جوڙيو (gid);

  إذا (unlikely (اونهائي++ > HB_MAX_NESTING_LEVEL)) أرجع operation_ڳڻپ;
  إذا (unlikely (--operation_ڳڻپ < 0)) أرجع operation_ڳڻپ;

  خودڪار glyph = glyf.glyph_for_gid (gid);

  لكل (خودڪار &شي : glyph.get_مرکب_iterator ())
    operation_ڳڻپ =
      glyf_جوڙيو_gid_and_children (glyf,
				  شي.get_gid (glyph.is_extended ()),
				  gids_منزل_retain,
				  operation_ڳڻپ,
				  اونهائي);

  أرجع operation_ڳڻپ;
}

ساكن فراغ
nameid_گهيرو (hb_subset_plan_t* plan,
		 hb_سيٽ_t* drop_tables)
{
#جيڪڏهن_وصف_نه_ٿيل HB_NO_STYLE
  إذا (!drop_tables->has (HB_OT_ٽيگ_STAT))
    plan->ذريعو->جدول.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  إذا (!plan->all_axes_pinned)
    plan->ذريعو->جدول.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_اشاريو_ٽيگ_map, &plan->name_ids);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_COLOR
  إذا (!drop_tables->has (HB_OT_ٽيگ_CPAL))
    plan->ذريعو->جدول.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_LAYOUT
  layout_nameid_گهيرو(plan, drop_tables);
#جيڪڏهن_پورو
}

ساكن فراغ
populate_gids_منزل_retain (hb_subset_plan_t* plan,
		          hb_سيٽ_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->ذريعو);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t ڍير_cff (plan->accelerator ? خالي_اشارو : plan->ذريعو);
  ثابت OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.get () : &ڍير_cff);
#جيڪڏهن_پورو

  plan->_glyphset_gsub.جوڙيو (0); // Not-def

  cmap_گهيرو (plan->ذريعو, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  إذا (!drop_tables->has (HB_OT_ٽيگ_MATH))
  {
    math_گهيرو (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->ذريعو->get_انگ_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_LAYOUT
  layout_populate_gids_منزل_retain(plan, drop_tables);
#جيڪڏهن_پورو

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->ذريعو->get_انگ_glyphs ());

  hb_سيٽ_t cur_glyphset = plan->_glyphset_gsub;
  إذا (!drop_tables->has (HB_OT_ٽيگ_COLR))
  {
    colr_گهيرو (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->ذريعو->get_انگ_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR_COMPOSITES
  إذا (!drop_tables->has (OT::VARC::جدول_ٽيگ))
  {
    إذا (unlikely (!plan->ذريعو->جدول.VARC->گهيرو_glyphs (&cur_glyphset)))
    {
      plan->check_success (خطأ);
      أرجع;
    }
    _remove_invalid_gids (&cur_glyphset, plan->ذريعو->get_انگ_glyphs ());
  }
#جيڪڏهن_پورو

  plan->_glyphset_varced = cur_glyphset;

  nameid_گهيرو (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  إذا (glyf.has_معلومات ())
    لكل (hb_codepoint_t gid : cur_glyphset)
      glyf_جوڙيو_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_مرکب_OPERATIONS_في_GLYPH);
  وإلا
    plan->_glyphset.union_ (cur_glyphset);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF
  إذا (!plan->accelerator || plan->accelerator->has_seac)
  {
    منطقي has_seac = خطأ;
    إذا (cff->is_valid ())
      لكل (hb_codepoint_t gid : cur_glyphset)
	إذا (جوڙيو_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = سچ;
    plan->has_seac = has_seac;
  }
#جيڪڏهن_پورو

  _remove_invalid_gids (&plan->_glyphset, plan->ذريعو->get_انگ_glyphs ());

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_LAYOUT
  إذا (!drop_tables->has (HB_OT_ٽيگ_GDEF))
    collect_layout_variation_indices (plan);
#جيڪڏهن_پورو
#جيڪڏهن_پورو
}

ساكن منطقي
_create_glyph_map_gsub (ثابت hb_سيٽ_t* glyph_سيٽ_gsub,
                        ثابت hb_map_t* glyph_map,
                        hb_map_t* ٻاهريون,
                        hb_ويڪٽر_t<hb_codepoint_t>* ٻاهريون_flat)
{
  إذا (unlikely (glyph_سيٽ_gsub->اندريون_error ()))
    أرجع خطأ;

  hb_codepoint_t max_gid = HB_سيٽ_قدر_INVALID;
  hb_سيٽ_اڳيون (glyph_سيٽ_gsub, &max_gid);
  بنانشان flat_سائيز = max_gid == HB_سيٽ_قدر_INVALID ? 0 : max_gid + 1;
  إذا (unlikely (!ٻاهريون_flat->resize_dirty (flat_سائيز)))
    أرجع خطأ;
  hb_memset (ٻاهريون_flat->arrayZ, 0xFF, flat_سائيز * حجم (hb_codepoint_t));

  ٻاهريون->alloc (glyph_سيٽ_gsub->get_population ());
  لكل (خودڪار gid : *glyph_سيٽ_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->get (gid);
    ٻاهريون->سيٽ (gid, new_gid);
    ٻاهريون_flat->arrayZ[gid] = new_gid;
  }
  أرجع !ٻاهريون->اندريون_error ();
}

ساكن منطقي
create_old_gid_منزل_new_gid_map (ثابت hb_face_t *face,
				منطقي		 retain_gids,
				ثابت hb_سيٽ_t	*all_gids_منزل_retain,
                                ثابت hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /*ٻاهريون*/
				hb_map_t	*ابتو_glyph_map, /*ٻاهريون*/
				hb_sorted_ويڪٽر_t<hb_codepoint_pair_t> *new_منزل_old_gid_list /*ٻاهريون*/,
				بنانشان صحيح	*انگ_glyphs /*ٻاهريون*/)
{
  بنانشان ڪڍو = all_gids_منزل_retain->get_population ();
  ابتو_glyph_map->alloc (ڪڍو);
  glyph_map->alloc (ڪڍو);
  new_منزل_old_gid_list->alloc (ڪڍو);

  إذا (*requested_glyph_map)
  {
    hb_سيٽ_t new_gids(requested_glyph_map->values());
    إذا (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_نياپو (SUBSET, خالي_اشارو, "The provided custom glyph mapping is not unique.");
      أرجع خطأ;
    }

    إذا (retain_gids)
    {
      DEBUG_نياپو (SUBSET, خالي_اشارو,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      أرجع خطأ;
    }

    hb_codepoint_t max_glyph = 0;
    hb_سيٽ_t بچيل;
    لكل (خودڪار old_gid : all_gids_منزل_retain->iter ())
    {
      إذا (old_gid == 0) {
	new_منزل_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        استمر;
      }

      hb_codepoint_t* new_gid;
      إذا (!requested_glyph_map->has (old_gid, &new_gid))
      {
        بچيل.جوڙيو(old_gid);
        استمر;
      }

      إذا (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_منزل_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_منزل_old_gid_list->qsort ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    لكل (خودڪار old_gid : بچيل)
      new_منزل_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *انگ_glyphs = max_glyph + 1;
  }
  وإلا إذا (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_منزل_retain), (hb_codepoint_t) 0)
    | hb_sink (new_منزل_old_gid_list)
    ;
    *انگ_glyphs = new_منزل_old_gid_list->ڊيگهائي;
  }
  وإلا
  {
    + hb_iter (all_gids_منزل_retain)
    | hb_map ([] (hb_codepoint_t _) {
		أرجع hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_منزل_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_سيٽ_قدر_INVALID;
    hb_سيٽ_اڳيون (all_gids_منزل_retain, &max_glyph);

    *انگ_glyphs = max_glyph + 1;
  }

  ابتو_glyph_map->alloc (ابتو_glyph_map->get_population () + new_منزل_old_gid_list->ڊيگهائي);
  + hb_iter (new_منزل_old_gid_list)
  | hb_sink (ابتو_glyph_map)
  ;
  glyph_map->alloc (glyph_map->get_population () + new_منزل_old_gid_list->ڊيگهائي);
  + hb_iter (new_منزل_old_gid_list)
  | hb_map (&hb_codepoint_pair_t::reverse)
  | hb_sink (glyph_map)
  ;

  أرجع سچ;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    ثابت hb_subset_داخلا_t *داخلا_2)
{
  successful = سچ;
  flags = داخلا_2->flags;

  unicode_منزل_new_gid_list.init ();

  name_ids = *داخلا_2->sets.name_ids;
  name_languages = *داخلا_2->sets.name_languages;
  layout_features = *داخلا_2->sets.layout_features;
  layout_scripts = *داخلا_2->sets.layout_scripts;
  glyphs_requested = *داخلا_2->sets.glyphs;
  drop_tables = *داخلا_2->sets.drop_tables;
  no_subset_tables = *داخلا_2->sets.no_subset_tables;
  ذريعو = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_منزل_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  ابتو_glyph_map = hb_map_create ();

  gsub_وجهو_catch_all_feature_variation_rec = خطأ;
  gpos_وجهو_catch_all_feature_variation_rec = خطأ;
  gdef_varstore_inner_maps.init ();

  user_axes_location = داخلا_2->axes_location;
  all_axes_pinned = خطأ;
  pinned_at_default = سچ;
  has_gdef_varstore = خطأ;
  has_avar2 = خطأ;

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
  لكل (خودڪار _ : داخلا_2->name_جدول_overrides)
  {
    hb_بائيٽون_t name_بائيٽون = _.second;
    بنانشان len = name_بائيٽون.ڊيگهائي;
    حرف *name_str = (حرف *) hb_malloc (len);
    إذا (unlikely (!check_success (name_str)))
      اكسر;

    hb_memcpy (name_str, name_بائيٽون.arrayZ, len);
    name_جدول_overrides.سيٽ (_.پهريون, hb_بائيٽون_t (name_str, len));
  }
#جيڪڏهن_پورو

  فراغ* accel = hb_face_get_user_معلومات(face, hb_subset_accelerator_t::user_معلومات_ڪنجي());

  attach_accelerator_معلومات = داخلا_2->attach_accelerator_معلومات;
  force_long_loca = داخلا_2->force_long_loca;
#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#جيڪڏهن_پورو

  إذا (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  إذا (unlikely (اندريون_error ()))
    أرجع;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR_COMPOSITES
  إذا (!داخلا_2->sets.drop_tables->has (OT::VARC::جدول_ٽيگ) &&
      داخلا_2->sets.no_subset_tables->has (OT::VARC::جدول_ٽيگ) &&
      !(داخلا_2->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->جدول.VARC->has_معلومات ())
  {
    check_success (خطأ);
    أرجع;
  }
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  إذا (!check_success (normalize_axes_location (face, هذا)))
      أرجع;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR_COMPOSITES
  إذا (!user_axes_location.is_empty () &&
      !داخلا_2->sets.drop_tables->has (OT::VARC::جدول_ٽيگ) &&
      داخلا_2->sets.no_subset_tables->has (OT::VARC::جدول_ٽيگ) &&
      face->جدول.VARC->has_معلومات ())
  {
    check_success (خطأ);
    أرجع;
  }
#جيڪڏهن_پورو

  populate_unicodes_منزل_retain (داخلا_2->sets.unicodes, داخلا_2->sets.glyphs, هذا);

  populate_gids_منزل_retain (هذا, داخلا_2->sets.drop_tables);
  إذا (unlikely (اندريون_error ()))
    أرجع;

  إذا (!check_success(create_old_gid_منزل_new_gid_map(
          face,
          داخلا_2->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &داخلا_2->glyph_map,
          glyph_map,
          ابتو_glyph_map,
	  &new_منزل_old_gid_list,
          &انگ_output_glyphs_2))) {
    أرجع;
  }

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API  
  إذا ((داخلا_2->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (داخلا_2->flags & HB_SUBSET_FLAGS_RETAIN_انگ_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    انگ_output_glyphs_2 = ذريعو->get_انگ_glyphs ();
  }
#جيڪڏهن_پورو

  إذا (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    أرجع;

  // Now that we have old to new gid map update the unicode to new gid list.
  لكل (بنانشان i = 0; i < unicode_منزل_new_gid_list.ڊيگهائي; i++)
  {
    // Use raw array access for performance.
    unicode_منزل_new_gid_list.arrayZ[i].second =
        glyph_map->get(unicode_منزل_new_gid_list.arrayZ[i].second);
  }

  bounds_ويڪر_vec.resize_dirty  (انگ_output_glyphs_2);
  لكل (خودڪار &v : bounds_ويڪر_vec)
    v = 0xFFFFFFFF;
  bounds_اوچائي_vec.resize_dirty  (انگ_output_glyphs_2);
  لكل (خودڪار &v : bounds_اوچائي_vec)
    v = 0xFFFFFFFF;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_LAYOUT    
  إذا (!drop_tables.has (HB_OT_ٽيگ_GDEF))
    remap_استعمال_ٿيل_mark_sets (هذا, استعمال_ٿيل_mark_sets_map);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
#جيڪڏهن_وصف_نه_ٿيل HB_NO_بنياد
  إذا (!drop_tables.has (HB_OT_ٽيگ_بنياد))
    collect_بنياد_variation_indices (هذا);
#جيڪڏهن_پورو
#جيڪڏهن_پورو

  إذا (unlikely (اندريون_error ()))
    أرجع;

#إذا !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (هذا);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  إذا (!check_success (get_instance_glyphs_contour_points (هذا)))
      أرجع;
#جيڪڏهن_پورو

  إذا (attach_accelerator_معلومات)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (ذريعو,
				       *codepoint_منزل_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#وصف_ڪريو HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.اندريون_error ());
#ضمن "hb-subset-plan-member-list.hh"
#وصف_رد HB_SUBSET_PLAN_MEMBER
}

hb_subset_plan_t::~hb_subset_plan_t()
{
  hb_face_destroy (dest);

  hb_map_destroy (codepoint_منزل_glyph);
  hb_map_destroy (glyph_map);
  hb_map_destroy (ابتو_glyph_map);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#جيڪڏهن_پورو
  hb_face_destroy (ذريعو);

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
  لكل (خودڪار _ : name_جدول_overrides.iter_ref ())
    _.second.fini ();
#جيڪڏهن_پورو

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
                               ثابت hb_subset_داخلا_t *داخلا_2)
{
  hb_subset_plan_t *plan;
  إذا (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, داخلا_2))))
    أرجع خالي_اشارو;

  إذا (unlikely (plan->اندريون_error ()))
  {
    hb_subset_plan_destroy (plan);
    أرجع خالي_اشارو;
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
  أرجع plan->ابتو_glyph_map;
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
hb_subset_plan_سيٽ_user_معلومات (hb_subset_plan_t   *plan,
                              hb_user_معلومات_ڪنجي_t *key,
                              فراغ               *معلومات,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  أرجع hb_object_سيٽ_user_معلومات (plan, key, معلومات, destroy, replace);
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
                              hb_user_معلومات_ڪنجي_t     *key)
{
  أرجع hb_object_get_user_معلومات (plan, key);
}

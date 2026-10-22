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

#اگر_تعریف_نشده HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#پایان_اگر
  hb_face_destroy (منبع);
}


#اگر_تعریف_نشده HB_NO_SUBSET_CFF
ساكن مضمن منطقي
جمع_کن_cff_seac_components (ثابت OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_مجموعه_t *gids_مقصد_retain)
{
  hb_codepoint_t پایه_gid, accent_gid;
  إذا (cff.get_seac_components (gid, &پایه_gid, &accent_gid))
  {
    gids_مقصد_retain->جمع_کن (پایه_gid);
    gids_مقصد_retain->جمع_کن (accent_gid);
    أرجع درست;
  }
  أرجع خطأ;
}
#پایان_اگر

ساكن فراغ
_remap_palette_indexes (ثابت hb_مجموعه_t *palette_indexes,
			hb_map_t       *mapping /*بیرون*/)
{
  بیعلامت new_idx = 0;
  لكل (بیعلامت palette_شاخص : palette_indexes->iter ())
  {
    إذا (palette_شاخص == 0xFFFF)
    {
      mapping->مجموعه (palette_شاخص, palette_شاخص);
      استمر;
    }
    mapping->مجموعه (palette_شاخص, new_idx);
    new_idx++;
  }
}

فراغ
remap_indexes (ثابت hb_مجموعه_t *indexes,
               hb_map_t       *mapping /*بیرون*/)
{
  لكل (خودکار _ : + hb_enumerate (indexes->iter ()))
    mapping->مجموعه (_.second, _.اولی);
}

ساكن مضمن فراغ
cmap_بستگی (hb_face_t	   *face,
	       ثابت hb_مجموعه_t	   *unicodes,
	       hb_مجموعه_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.بستگی_glyphs (unicodes, glyphset);
}

ساكن فراغ colr_بستگی (hb_subset_plan_t* plan,
                           hb_مجموعه_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->منبع);
  إذا (!colr.is_valid ()) أرجع;

  hb_مجموعه_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_مجموعه_t glyphset_colrv0;
  لكل (hb_codepoint_t gid : *glyphs_colred)
    colr.بستگی_glyphs (gid, &glyphset_colrv0);

  glyphs_colred->union_ (glyphset_colrv0);

  //closure for COLRv1
  hb_مجموعه_t variation_indices, delta_مجموعه_indices;
  colr.بستگی_for_V1 (glyphs_colred, &layer_indices, &palette_indices, &variation_indices, &delta_مجموعه_indices);

  colr.بستگی_V0palette_indices (glyphs_colred, &palette_indices);
  remap_indexes (&layer_indices, &plan->colrv1_layers);
  _remap_palette_indexes (&palette_indices, &plan->colr_palettes);

#اگر_تعریف_نشده HB_NO_VAR
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
    ثابت OT::قلم_Variation_Store &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    بیعلامت subtable_شمار = var_store.get_sub_جدول_شمار ();
    generate_varstore_inner_maps (variation_indices, subtable_شمار, plan->colrv1_varstore_inner_maps);

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
  إذا (!plan->all_axes_pinned && colr.has_delta_مجموعه_شاخص_map ())
    remap_colrv1_delta_مجموعه_شاخص_indices (colr.get_delta_مجموعه_شاخص_map (),
                                         delta_مجموعه_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#پایان_اگر
}

ساكن مضمن فراغ
math_بستگی (hb_subset_plan_t *plan,
               hb_مجموعه_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->منبع_جدول<OT::MATH> ();
  إذا (math->has_داده ())
    math->بستگی_glyphs (glyphset);
  math.destroy ();
}

ساكن مضمن فراغ
_remove_invalid_gids (hb_مجموعه_t *glyphs,
		      بیعلامت صحيح رقم_glyphs)
{
  glyphs->del_range (رقم_glyphs, HB_مجموعه_مقدار_INVALID);
}

قالب<منطقي GID_ALWAYS_EXISTS = خطأ, نام_نوع I, نام_نوع F, نام_نوع G, hb_requires (hb_is_iterator (I))>
ساكن فراغ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_مقصد_gid_for_iterator,
                            G unicode_مقصد_gid_general)
{
  لكل (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_مقصد_gid_for_iterator(cp);
    إذا (!GID_ALWAYS_EXISTS && gid == HB_MAP_مقدار_INVALID)
    {
      DEBUG_پیام(SUBSET, اشاره_تهی, "Drop U+%04X; no gid", cp);
      استمر;
    }

    plan->codepoint_مقصد_glyph->مجموعه (cp, gid);
    plan->unicode_مقصد_new_gid_list.push (hb_pair (cp, gid));
  }
}

قالب<منطقي GID_ALWAYS_EXISTS = خطأ, نام_نوع I, نام_نوع F, hb_requires (hb_is_iterator (I))>
ساكن فراغ
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_مقصد_gid_for_iterator)
{
  _fill_unicode_and_glyph_map(plan, unicode_iterator, unicode_مقصد_gid_for_iterator, unicode_مقصد_gid_for_iterator);
}

/*
 * Finds additional unicode codepoints which are reachable from the input unicode set.
 * Currently this adds in mirrored variants (needed for bidi) of any input unicodes.
 */
ساكن hb_مجموعه_t
unicode_بستگی (ثابت hb_مجموعه_t* unicodes, منطقي bidi_بستگی) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_مجموعه_t بیرون = *unicodes;
  إذا (!bidi_بستگی) أرجع بیرون;

  إذا (بیرون.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    أرجع بیرون;
  }

  خودکار unicode_funcs = hb_unicode_funcs_get_default ();
  لكل (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   إذا (unlikely (mirror != cp)) {
     بیرون.جمع_کن(mirror);
   }
  }

  أرجع بیرون;
}

ساكن فراغ
populate_unicodes_مقصد_retain (ثابت hb_مجموعه_t *unicodes_درون,
                              ثابت hb_مجموعه_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_مجموعه_t unicodes = unicode_بستگی(unicodes_درون,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_بستگی));

  OT::cmap_accelerator_t cmap (plan->منبع);
  بیعلامت اندازه_آستانه = plan->منبع->get_رقم_glyphs ();

  إذا (glyphs->is_empty () && unicodes.get_population () < اندازه_آستانه)
  {

    ثابت hb_map_t* unicode_مقصد_gid_2 = اشاره_تهی;
    إذا (plan->accelerator)
      unicode_مقصد_gid_2 = &plan->accelerator->unicode_مقصد_gid_2;

    // This is approach to collection is faster, but can only be used  if glyphs
    // are not being explicitly added to the subset and the input unicodes set is
    // not excessively large (eg. an inverted set).
    plan->unicode_مقصد_new_gid_list.alloc (unicodes.get_population ());
    إذا (!unicode_مقصد_gid_2) {
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        hb_codepoint_t gid;
        إذا (!cmap.get_nominal_glyph (cp, &gid)) {
          أرجع HB_MAP_مقدار_INVALID;
        }
        أرجع gid;
      });
    } وإلا {
      // Use in memory unicode to gid map it's faster then looking up from
      // the map. This code is mostly duplicated from above to avoid doing
      // conditionals on the presence of the unicode_to_gid map each
      // iteration.
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        أرجع unicode_مقصد_gid_2->get (cp);
      });
    }
  }
  وإلا
  {
    // This approach is slower, but can handle adding in glyphs to the subset and will match
    // them with cmap entries.

    hb_map_t unicode_glyphid_map_storage;
    hb_مجموعه_t cmap_unicodes_storage;
    ثابت hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    ثابت hb_مجموعه_t* cmap_unicodes = &cmap_unicodes_storage;

    إذا (!plan->accelerator) {
      cmap.collect_mapping (&cmap_unicodes_storage, &unicode_glyphid_map_storage);
      plan->unicode_مقصد_new_gid_list.alloc (hb_min(unicodes.get_population ()
                                                  + glyphs->get_population (),
                                                  cmap_unicodes->get_population ()));
    } وإلا {
      unicode_glyphid_map = &plan->accelerator->unicode_مقصد_gid_2;
      cmap_unicodes = &plan->accelerator->unicodes;
    }

    إذا (plan->accelerator &&
	unicodes.get_population () < cmap_unicodes->get_population () &&
	glyphs->get_population () < cmap_unicodes->get_population ())
    {
      plan->codepoint_مقصد_glyph->alloc (unicodes.get_population () + glyphs->get_population ());

      خودکار &gid_مقصد_unicodes = plan->accelerator->gid_مقصد_unicodes;

      لكل (hb_codepoint_t gid : *glyphs)
      {
        خودکار unicodes = gid_مقصد_unicodes.get (gid);
        _fill_unicode_and_glyph_map<درست>(plan, unicodes, [&] (hb_codepoint_t cp) {
          أرجع gid;
        },
        [&] (hb_codepoint_t cp) {
          أرجع unicode_glyphid_map->get(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	إذا (plan->codepoint_مقصد_glyph->has (cp))
          أرجع HB_MAP_مقدار_INVALID;

        أرجع unicode_glyphid_map->get(cp);
      },
      [&] (hb_codepoint_t cp) {
          أرجع unicode_glyphid_map->get(cp);
      });

      plan->unicode_مقصد_new_gid_list.qsort ();
    }
    وإلا
    {
      plan->codepoint_مقصد_glyph->alloc (cmap_unicodes->get_population ());
      hb_codepoint_t اولی = HB_مجموعه_مقدار_INVALID, آخرین = HB_مجموعه_مقدار_INVALID;
      لكل (; cmap_unicodes->بعدی_range (&اولی, &آخرین); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(اولی, آخرین + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  إذا (!unicodes.has (cp) && !glyphs->has (gid))
	    أرجع HB_MAP_مقدار_INVALID;
          أرجع gid;
        },
        [&] (hb_codepoint_t cp) {
          أرجع unicode_glyphid_map->get(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    بیعلامت رقم_glyphs = plan->منبع->get_رقم_glyphs ();
    hb_codepoint_t اولی = HB_مجموعه_مقدار_INVALID, آخرین = HB_مجموعه_مقدار_INVALID;
    لكل (; glyphs->بعدی_range (&اولی, &آخرین); )
    {
      إذا (اولی >= رقم_glyphs)
	اكسر;
      إذا (آخرین >= رقم_glyphs)
        آخرین = رقم_glyphs - 1;
      plan->_glyphset_gsub.جمع_کن_range (اولی, آخرین);
    }
  }

  خودکار &arr = plan->unicode_مقصد_new_gid_list;
  إذا (arr.درازی)
  {
    plan->unicodes.جمع_کن_sorted_array (&arr.arrayZ->اولی, arr.درازی, حجم (*arr.arrayZ));
    plan->_glyphset_gsub.جمع_کن_array (&arr.arrayZ->second, arr.درازی, حجم (*arr.arrayZ));
  }

  // Variation selectors don't have glyphs associated with them in the cmap so they will have been filtered out above
  // but should still be retained. Add them back here.

  // However, the min and max codepoints for OS/2 should be calculated without considering variation selectors,
  // so record those first.
  plan->os2_info.min_cmap_codepoint = plan->unicodes.get_min();
  plan->os2_info.max_cmap_codepoint = plan->unicodes.get_max();

  hb_مجموعه_t variation_selectors_مقصد_retain;
  cmap.collect_variation_selectors(&variation_selectors_مقصد_retain);
  + variation_selectors_مقصد_retain.iter()
  | hb_filter(unicodes)
  | hb_sink(&plan->unicodes)
  ;
}

ساكن بیعلامت
glyf_جمع_کن_gid_and_children (ثابت OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_مجموعه_t *gids_مقصد_retain,
			    صحيح operation_شمار,
			    بیعلامت عمق = 0)
{
  /* Check if is already visited */
  إذا (gids_مقصد_retain->has (gid)) أرجع operation_شمار;

  gids_مقصد_retain->جمع_کن (gid);

  إذا (unlikely (عمق++ > HB_MAX_NESTING_LEVEL)) أرجع operation_شمار;
  إذا (unlikely (--operation_شمار < 0)) أرجع operation_شمار;

  خودکار glyph = glyf.glyph_for_gid (gid);

  لكل (خودکار &قلم : glyph.get_مرکب_iterator ())
    operation_شمار =
      glyf_جمع_کن_gid_and_children (glyf,
				  قلم.get_gid (glyph.is_extended ()),
				  gids_مقصد_retain,
				  operation_شمار,
				  عمق);

  أرجع operation_شمار;
}

ساكن فراغ
nameid_بستگی (hb_subset_plan_t* plan,
		 hb_مجموعه_t* drop_tables)
{
#اگر_تعریف_نشده HB_NO_STYLE
  إذا (!drop_tables->has (HB_OT_برچسب_STAT))
    plan->منبع->جدول.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR
  إذا (!plan->all_axes_pinned)
    plan->منبع->جدول.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_شاخص_برچسب_map, &plan->name_ids);
#پایان_اگر
#اگر_تعریف_نشده HB_NO_COLOR
  إذا (!drop_tables->has (HB_OT_برچسب_CPAL))
    plan->منبع->جدول.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_SUBSET_LAYOUT
  layout_nameid_بستگی(plan, drop_tables);
#پایان_اگر
}

ساكن فراغ
populate_gids_مقصد_retain (hb_subset_plan_t* plan,
		          hb_مجموعه_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->منبع);
#اگر_تعریف_نشده HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t پشته_cff (plan->accelerator ? اشاره_تهی : plan->منبع);
  ثابت OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.get () : &پشته_cff);
#پایان_اگر

  plan->_glyphset_gsub.جمع_کن (0); // Not-def

  cmap_بستگی (plan->منبع, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  إذا (!drop_tables->has (HB_OT_برچسب_MATH))
  {
    math_بستگی (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->منبع->get_رقم_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#اگر_تعریف_نشده HB_NO_SUBSET_LAYOUT
  layout_populate_gids_مقصد_retain(plan, drop_tables);
#پایان_اگر

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->منبع->get_رقم_glyphs ());

  hb_مجموعه_t cur_glyphset = plan->_glyphset_gsub;
  إذا (!drop_tables->has (HB_OT_برچسب_COLR))
  {
    colr_بستگی (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->منبع->get_رقم_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES
  إذا (!drop_tables->has (OT::VARC::جدول_برچسب))
  {
    إذا (unlikely (!plan->منبع->جدول.VARC->بستگی_glyphs (&cur_glyphset)))
    {
      plan->check_success (خطأ);
      أرجع;
    }
    _remove_invalid_gids (&cur_glyphset, plan->منبع->get_رقم_glyphs ());
  }
#پایان_اگر

  plan->_glyphset_varced = cur_glyphset;

  nameid_بستگی (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  إذا (glyf.has_داده ())
    لكل (hb_codepoint_t gid : cur_glyphset)
      glyf_جمع_کن_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_مرکب_OPERATIONS_در_هر_GLYPH);
  وإلا
    plan->_glyphset.union_ (cur_glyphset);
#اگر_تعریف_نشده HB_NO_SUBSET_CFF
  إذا (!plan->accelerator || plan->accelerator->has_seac)
  {
    منطقي has_seac = خطأ;
    إذا (cff->is_valid ())
      لكل (hb_codepoint_t gid : cur_glyphset)
	إذا (جمع_کن_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = درست;
    plan->has_seac = has_seac;
  }
#پایان_اگر

  _remove_invalid_gids (&plan->_glyphset, plan->منبع->get_رقم_glyphs ());

#اگر_تعریف_نشده HB_NO_VAR
#اگر_تعریف_نشده HB_NO_SUBSET_LAYOUT
  إذا (!drop_tables->has (HB_OT_برچسب_GDEF))
    collect_layout_variation_indices (plan);
#پایان_اگر
#پایان_اگر
}

ساكن منطقي
_create_glyph_map_gsub (ثابت hb_مجموعه_t* glyph_مجموعه_gsub,
                        ثابت hb_map_t* glyph_map,
                        hb_map_t* بیرون,
                        hb_وکتور_t<hb_codepoint_t>* بیرون_flat)
{
  إذا (unlikely (glyph_مجموعه_gsub->درون_error ()))
    أرجع خطأ;

  hb_codepoint_t max_gid = HB_مجموعه_مقدار_INVALID;
  hb_مجموعه_قبلی (glyph_مجموعه_gsub, &max_gid);
  بیعلامت flat_اندازه = max_gid == HB_مجموعه_مقدار_INVALID ? 0 : max_gid + 1;
  إذا (unlikely (!بیرون_flat->resize_dirty (flat_اندازه)))
    أرجع خطأ;
  hb_memset (بیرون_flat->arrayZ, 0xFF, flat_اندازه * حجم (hb_codepoint_t));

  بیرون->alloc (glyph_مجموعه_gsub->get_population ());
  لكل (خودکار gid : *glyph_مجموعه_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->get (gid);
    بیرون->مجموعه (gid, new_gid);
    بیرون_flat->arrayZ[gid] = new_gid;
  }
  أرجع !بیرون->درون_error ();
}

ساكن منطقي
create_old_gid_مقصد_new_gid_map (ثابت hb_face_t *face,
				منطقي		 retain_gids,
				ثابت hb_مجموعه_t	*all_gids_مقصد_retain,
                                ثابت hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /*بیرون*/
				hb_map_t	*معکوس_glyph_map, /*بیرون*/
				hb_sorted_وکتور_t<hb_codepoint_pair_t> *new_مقصد_old_gid_list /*بیرون*/,
				بیعلامت صحيح	*رقم_glyphs /*بیرون*/)
{
  بیعلامت برداشت = all_gids_مقصد_retain->get_population ();
  معکوس_glyph_map->alloc (برداشت);
  glyph_map->alloc (برداشت);
  new_مقصد_old_gid_list->alloc (برداشت);

  إذا (*requested_glyph_map)
  {
    hb_مجموعه_t new_gids(requested_glyph_map->values());
    إذا (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_پیام (SUBSET, اشاره_تهی, "The provided custom glyph mapping is not unique.");
      أرجع خطأ;
    }

    إذا (retain_gids)
    {
      DEBUG_پیام (SUBSET, اشاره_تهی,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      أرجع خطأ;
    }

    hb_codepoint_t max_glyph = 0;
    hb_مجموعه_t مانده;
    لكل (خودکار old_gid : all_gids_مقصد_retain->iter ())
    {
      إذا (old_gid == 0) {
	new_مقصد_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        استمر;
      }

      hb_codepoint_t* new_gid;
      إذا (!requested_glyph_map->has (old_gid, &new_gid))
      {
        مانده.جمع_کن(old_gid);
        استمر;
      }

      إذا (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_مقصد_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_مقصد_old_gid_list->qsort ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    لكل (خودکار old_gid : مانده)
      new_مقصد_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *رقم_glyphs = max_glyph + 1;
  }
  وإلا إذا (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_مقصد_retain), (hb_codepoint_t) 0)
    | hb_sink (new_مقصد_old_gid_list)
    ;
    *رقم_glyphs = new_مقصد_old_gid_list->درازی;
  }
  وإلا
  {
    + hb_iter (all_gids_مقصد_retain)
    | hb_map ([] (hb_codepoint_t _) {
		أرجع hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_مقصد_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_مجموعه_مقدار_INVALID;
    hb_مجموعه_قبلی (all_gids_مقصد_retain, &max_glyph);

    *رقم_glyphs = max_glyph + 1;
  }

  معکوس_glyph_map->alloc (معکوس_glyph_map->get_population () + new_مقصد_old_gid_list->درازی);
  + hb_iter (new_مقصد_old_gid_list)
  | hb_sink (معکوس_glyph_map)
  ;
  glyph_map->alloc (glyph_map->get_population () + new_مقصد_old_gid_list->درازی);
  + hb_iter (new_مقصد_old_gid_list)
  | hb_map (&hb_codepoint_pair_t::reverse)
  | hb_sink (glyph_map)
  ;

  أرجع درست;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    ثابت hb_subset_درون_داد_t *درون_داد)
{
  successful = درست;
  flags = درون_داد->flags;

  unicode_مقصد_new_gid_list.init ();

  name_ids = *درون_داد->sets.name_ids;
  name_languages = *درون_داد->sets.name_languages;
  layout_features = *درون_داد->sets.layout_features;
  layout_scripts = *درون_داد->sets.layout_scripts;
  glyphs_requested = *درون_داد->sets.glyphs;
  drop_tables = *درون_داد->sets.drop_tables;
  no_subset_tables = *درون_داد->sets.no_subset_tables;
  منبع = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_مقصد_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  معکوس_glyph_map = hb_map_create ();

  gsub_درج_catch_all_feature_variation_rec = خطأ;
  gpos_درج_catch_all_feature_variation_rec = خطأ;
  gdef_varstore_inner_maps.init ();

  user_axes_location = درون_داد->axes_location;
  all_axes_pinned = خطأ;
  pinned_at_default = درست;
  has_gdef_varstore = خطأ;
  has_avar2 = خطأ;

#اگر_تعریف_شده HB_EXPERIMENTAL_API
  لكل (خودکار _ : درون_داد->name_جدول_overrides)
  {
    hb_بایتها_t name_بایتها = _.second;
    بیعلامت len = name_بایتها.درازی;
    حرف *name_str = (حرف *) hb_malloc (len);
    إذا (unlikely (!check_success (name_str)))
      اكسر;

    hb_memcpy (name_str, name_بایتها.arrayZ, len);
    name_جدول_overrides.مجموعه (_.اولی, hb_بایتها_t (name_str, len));
  }
#پایان_اگر

  فراغ* accel = hb_face_get_user_داده(face, hb_subset_accelerator_t::user_داده_کلید());

  attach_accelerator_داده = درون_داد->attach_accelerator_داده;
  force_long_loca = درون_داد->force_long_loca;
#اگر_تعریف_شده HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#پایان_اگر

  إذا (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  إذا (unlikely (درون_error ()))
    أرجع;

#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES
  إذا (!درون_داد->sets.drop_tables->has (OT::VARC::جدول_برچسب) &&
      درون_داد->sets.no_subset_tables->has (OT::VARC::جدول_برچسب) &&
      !(درون_داد->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->جدول.VARC->has_داده ())
  {
    check_success (خطأ);
    أرجع;
  }
#پایان_اگر

#اگر_تعریف_نشده HB_NO_VAR
  إذا (!check_success (normalize_axes_location (face, هذا)))
      أرجع;
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES
  إذا (!user_axes_location.is_empty () &&
      !درون_داد->sets.drop_tables->has (OT::VARC::جدول_برچسب) &&
      درون_داد->sets.no_subset_tables->has (OT::VARC::جدول_برچسب) &&
      face->جدول.VARC->has_داده ())
  {
    check_success (خطأ);
    أرجع;
  }
#پایان_اگر

  populate_unicodes_مقصد_retain (درون_داد->sets.unicodes, درون_داد->sets.glyphs, هذا);

  populate_gids_مقصد_retain (هذا, درون_داد->sets.drop_tables);
  إذا (unlikely (درون_error ()))
    أرجع;

  إذا (!check_success(create_old_gid_مقصد_new_gid_map(
          face,
          درون_داد->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &درون_داد->glyph_map,
          glyph_map,
          معکوس_glyph_map,
	  &new_مقصد_old_gid_list,
          &رقم_output_glyphs_2))) {
    أرجع;
  }

#اگر_تعریف_شده HB_EXPERIMENTAL_API  
  إذا ((درون_داد->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (درون_داد->flags & HB_SUBSET_FLAGS_RETAIN_رقم_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    رقم_output_glyphs_2 = منبع->get_رقم_glyphs ();
  }
#پایان_اگر

  إذا (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    أرجع;

  // Now that we have old to new gid map update the unicode to new gid list.
  لكل (بیعلامت i = 0; i < unicode_مقصد_new_gid_list.درازی; i++)
  {
    // Use raw array access for performance.
    unicode_مقصد_new_gid_list.arrayZ[i].second =
        glyph_map->get(unicode_مقصد_new_gid_list.arrayZ[i].second);
  }

  bounds_عرض_vec.resize_dirty  (رقم_output_glyphs_2);
  لكل (خودکار &v : bounds_عرض_vec)
    v = 0xFFFFFFFF;
  bounds_ارتفاع_vec.resize_dirty  (رقم_output_glyphs_2);
  لكل (خودکار &v : bounds_ارتفاع_vec)
    v = 0xFFFFFFFF;

#اگر_تعریف_نشده HB_NO_SUBSET_LAYOUT    
  إذا (!drop_tables.has (HB_OT_برچسب_GDEF))
    remap_استفاده_شده_mark_sets (هذا, استفاده_شده_mark_sets_map);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_VAR
#اگر_تعریف_نشده HB_NO_پایه
  إذا (!drop_tables.has (HB_OT_برچسب_پایه))
    collect_پایه_variation_indices (هذا);
#پایان_اگر
#پایان_اگر

  إذا (unlikely (درون_error ()))
    أرجع;

#إذا !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (هذا);
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR
  إذا (!check_success (get_instance_glyphs_contour_points (هذا)))
      أرجع;
#پایان_اگر

  إذا (attach_accelerator_داده)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (منبع,
				       *codepoint_مقصد_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#تعریف_کن HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.درون_error ());
#ضمن "hb-subset-plan-member-list.hh"
#لغو_تعریف HB_SUBSET_PLAN_MEMBER
}

hb_subset_plan_t::~hb_subset_plan_t()
{
  hb_face_destroy (dest);

  hb_map_destroy (codepoint_مقصد_glyph);
  hb_map_destroy (glyph_map);
  hb_map_destroy (معکوس_glyph_map);
#اگر_تعریف_نشده HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#پایان_اگر
  hb_face_destroy (منبع);

#اگر_تعریف_شده HB_EXPERIMENTAL_API
  لكل (خودکار _ : name_جدول_overrides.iter_ref ())
    _.second.fini ();
#پایان_اگر

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
                               ثابت hb_subset_درون_داد_t *درون_داد)
{
  hb_subset_plan_t *plan;
  إذا (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, درون_داد))))
    أرجع اشاره_تهی;

  إذا (unlikely (plan->درون_error ()))
  {
    hb_subset_plan_destroy (plan);
    أرجع اشاره_تهی;
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
hb_subset_plan_old_مقصد_new_glyph_mapping (ثابت hb_subset_plan_t *plan)
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
hb_subset_plan_new_مقصد_old_glyph_mapping (ثابت hb_subset_plan_t *plan)
{
  أرجع plan->معکوس_glyph_map;
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
hb_subset_plan_unicode_مقصد_old_glyph_mapping (ثابت hb_subset_plan_t *plan)
{
  أرجع plan->codepoint_مقصد_glyph;
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
hb_subset_plan_مجموعه_user_داده (hb_subset_plan_t   *plan,
                              hb_user_داده_کلید_t *key,
                              فراغ               *داده,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  أرجع hb_object_مجموعه_user_داده (plan, key, داده, destroy, replace);
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
hb_subset_plan_get_user_داده (ثابت hb_subset_plan_t *plan,
                              hb_user_داده_کلید_t     *key)
{
  أرجع hb_object_get_user_داده (plan, key);
}

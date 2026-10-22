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

#شامل "hb-subset-plan.hh"
#شامل "hb-subset-accelerator.hh"
#شامل "hb-map.hh"
#شامل "hb-multimap.hh"
#شامل "hb-set.hh"
#شامل "hb-subset.h"
#شامل "hb-unicode.h"

#شامل "hb-ot-cmap-table.hh"
#شامل "hb-ot-glyf-table.hh"
#شامل "hb-ot-layout-base-table.hh"
#شامل "hb-ot-cff1-table.hh"
#شامل "hb-ot-cff2-table.hh"
#شامل "OT/Color/COLR/COLR.hh"
#شامل "OT/Color/COLR/colrv1-closure.hh"
#شامل "OT/Color/CPAL/CPAL.hh"
#شامل "hb-ot-var-fvar-table.hh"
#شامل "hb-ot-var-varc-table.hh"
#شامل "hb-ot-stat-table.hh"
#شامل "hb-ot-math-table.hh"

hb_subset_accelerator_t::~hb_subset_accelerator_t ()
{
  اگر (cmap_cache && destroy_cmap_cache)
    destroy_cmap_cache ((خالی*) cmap_cache);

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#اگر_ختم
  hb_face_destroy (ماخذ);
}


#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
ساکن اندرونی منطقی
جوڑو_cff_seac_components (ثابت OT::cff1::accelerator_subset_t &cff,
			  hb_codepoint_t gid,
			  hb_سیٹ_t *gids_مقصد_retain)
{
  hb_codepoint_t بنیاد_gid, accent_gid;
  اگر (cff.get_seac_components (gid, &بنیاد_gid, &accent_gid))
  {
    gids_مقصد_retain->جوڑو (بنیاد_gid);
    gids_مقصد_retain->جوڑو (accent_gid);
    واپس سچ;
  }
  واپس جھوٹ;
}
#اگر_ختم

ساکن خالی
_remap_palette_indexes (ثابت hb_سیٹ_t *palette_indexes,
			hb_map_t       *mapping /*برآمد*/)
{
  غیرعلامتی new_idx = 0;
  برائے (غیرعلامتی palette_اشاریہ : palette_indexes->iter ())
  {
    اگر (palette_اشاریہ == 0xFFFF)
    {
      mapping->سیٹ (palette_اشاریہ, palette_اشاریہ);
      جاری;
    }
    mapping->سیٹ (palette_اشاریہ, new_idx);
    new_idx++;
  }
}

خالی
remap_indexes (ثابت hb_سیٹ_t *indexes,
               hb_map_t       *mapping /*برآمد*/)
{
  برائے (خودکار _ : + hb_enumerate (indexes->iter ()))
    mapping->سیٹ (_.second, _.پہلا);
}

ساکن اندرونی خالی
cmap_احاطہ (hb_face_t	   *face,
	       ثابت hb_سیٹ_t	   *unicodes,
	       hb_سیٹ_t		   *glyphset)
{
  OT::cmap_accelerator_t cmap (face);
  cmap.احاطہ_glyphs (unicodes, glyphset);
}

ساکن خالی colr_احاطہ (hb_subset_plan_t* plan,
                           hb_سیٹ_t *glyphs_colred)
{
  OT::COLR::accelerator_t colr (plan->ماخذ);
  اگر (!colr.is_valid ()) واپس;

  hb_سیٹ_t palette_indices, layer_indices;
  // Collect all glyphs referenced by COLRv0
  hb_سیٹ_t glyphset_colrv0;
  برائے (hb_codepoint_t gid : *glyphs_colred)
    colr.احاطہ_glyphs (gid, &glyphset_colrv0);

  glyphs_colred->union_ (glyphset_colrv0);

  //closure for COLRv1
  hb_سیٹ_t variation_indices, delta_سیٹ_indices;
  colr.احاطہ_for_V1 (glyphs_colred, &layer_indices, &palette_indices, &variation_indices, &delta_سیٹ_indices);

  colr.احاطہ_V0palette_indices (glyphs_colred, &palette_indices);
  remap_indexes (&layer_indices, &plan->colrv1_layers);
  _remap_palette_indexes (&palette_indices, &plan->colr_palettes);

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  /* colr variation indices mapping during planning phase:
   * generate colrv1_variation_idx_delta_map. When delta set index map is not
   * included, it's a mapping from varIdx-> (new varIdx,delta). Otherwise, it's
   * a mapping from old delta set idx-> (new delta set idx, delta). Mapping
   * delta set indices is the same as gid mapping.
   * Besides, we need to generate a delta set idx-> new var_idx map for updating
   * delta set index map if exists. This map will be updated again after
   * instancing. */
  اگر (colr.has_var_store () && variation_indices)
  {
    ثابت OT::شے_Variation_Store &var_store = colr.get_var_store ();
    // generated inner_maps is used by ItemVariationStore serialize(), which is subset only
    غیرعلامتی subtable_گنتی = var_store.get_sub_جدول_گنتی ();
    generate_varstore_inner_maps (variation_indices, subtable_گنتی, plan->colrv1_varstore_inner_maps);

    اگر (!plan->all_axes_pinned)
      remap_variation_indices (var_store,
                              variation_indices,
                              plan->normalized_coords,
                              جھوٹ, /* no need to calculate delta for COLR during planning */
                              plan->all_axes_pinned,
                              plan->colrv1_variation_idx_delta_map);
  }

  /* Sentinel-only maps still need their delta-set indices remapped, even
   * when there are no live variation rows or no variation store. */
  اگر (!plan->all_axes_pinned && colr.has_delta_سیٹ_اشاریہ_map ())
    remap_colrv1_delta_سیٹ_اشاریہ_indices (colr.get_delta_سیٹ_اشاریہ_map (),
                                         delta_سیٹ_indices,
                                         plan->colrv1_variation_idx_delta_map,
                                         plan->colrv1_new_deltaset_idx_varidx_map);
#اگر_ختم
}

ساکن اندرونی خالی
math_احاطہ (hb_subset_plan_t *plan,
               hb_سیٹ_t         *glyphset)
{
  hb_blob_ptr_t<OT::MATH> math = plan->ماخذ_جدول<OT::MATH> ();
  اگر (math->has_ڈیٹا ())
    math->احاطہ_glyphs (glyphset);
  math.destroy ();
}

ساکن اندرونی خالی
_remove_invalid_gids (hb_سیٹ_t *glyphs,
		      غیرعلامتی صحیح عدد_glyphs)
{
  glyphs->del_range (عدد_glyphs, HB_سیٹ_قدر_INVALID);
}

سانچہ<منطقی GID_ALWAYS_EXISTS = جھوٹ, قسم_نام I, قسم_نام F, قسم_نام G, hb_requires (hb_is_iterator (I))>
ساکن خالی
_fill_unicode_and_glyph_map(hb_subset_plan_t *plan,
                            I unicode_iterator,
                            F unicode_مقصد_gid_for_iterator,
                            G unicode_مقصد_gid_general)
{
  برائے (hb_codepoint_t cp : unicode_iterator)
  {
    hb_codepoint_t gid = unicode_مقصد_gid_for_iterator(cp);
    اگر (!GID_ALWAYS_EXISTS && gid == HB_MAP_قدر_INVALID)
    {
      DEBUG_پیغام(SUBSET, خالی_اشاریہ, "Drop U+%04X; no gid", cp);
      جاری;
    }

    plan->codepoint_مقصد_glyph->سیٹ (cp, gid);
    plan->unicode_مقصد_new_gid_list.push (hb_pair (cp, gid));
  }
}

سانچہ<منطقی GID_ALWAYS_EXISTS = جھوٹ, قسم_نام I, قسم_نام F, hb_requires (hb_is_iterator (I))>
ساکن خالی
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
ساکن hb_سیٹ_t
unicode_احاطہ (ثابت hb_سیٹ_t* unicodes, منطقی bidi_احاطہ) {
  // TODO: we may want to also consider pulling in reachable unicode composition and decompositions.
  //       see: https://github.com/harfbuzz/harfbuzz/issues/2283
  hb_سیٹ_t برآمد = *unicodes;
  اگر (!bidi_احاطہ) واپس برآمد;

  اگر (برآمد.is_inverted()) {
    // don't closure inverted sets, they are asking to specifically exclude certain codepoints.
    // otherwise everything is already included.
    واپس برآمد;
  }

  خودکار unicode_funcs = hb_unicode_funcs_get_default ();
  برائے (hb_codepoint_t cp : *unicodes) {
   hb_codepoint_t mirror = hb_unicode_mirroring(unicode_funcs, cp);
   اگر (unlikely (mirror != cp)) {
     برآمد.جوڑو(mirror);
   }
  }

  واپس برآمد;
}

ساکن خالی
populate_unicodes_مقصد_retain (ثابت hb_سیٹ_t *unicodes_اندر,
                              ثابت hb_سیٹ_t *glyphs,
                              hb_subset_plan_t *plan)
{
  hb_سیٹ_t unicodes = unicode_احاطہ(unicodes_اندر,
    !(plan->flags & HB_SUBSET_FLAGS_NO_BIDI_احاطہ));

  OT::cmap_accelerator_t cmap (plan->ماخذ);
  غیرعلامتی سائز_حد = plan->ماخذ->get_عدد_glyphs ();

  اگر (glyphs->is_empty () && unicodes.get_population () < سائز_حد)
  {

    ثابت hb_map_t* unicode_مقصد_gid_2 = خالی_اشاریہ;
    اگر (plan->accelerator)
      unicode_مقصد_gid_2 = &plan->accelerator->unicode_مقصد_gid_2;

    // This is approach to collection is faster, but can only be used  if glyphs
    // are not being explicitly added to the subset and the input unicodes set is
    // not excessively large (eg. an inverted set).
    plan->unicode_مقصد_new_gid_list.alloc (unicodes.get_population ());
    اگر (!unicode_مقصد_gid_2) {
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        hb_codepoint_t gid;
        اگر (!cmap.get_nominal_glyph (cp, &gid)) {
          واپس HB_MAP_قدر_INVALID;
        }
        واپس gid;
      });
    } ورنہ {
      // Use in memory unicode to gid map it's faster then looking up from
      // the map. This code is mostly duplicated from above to avoid doing
      // conditionals on the presence of the unicode_to_gid map each
      // iteration.
      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
        واپس unicode_مقصد_gid_2->get (cp);
      });
    }
  }
  ورنہ
  {
    // This approach is slower, but can handle adding in glyphs to the subset and will match
    // them with cmap entries.

    hb_map_t unicode_glyphid_map_storage;
    hb_سیٹ_t cmap_unicodes_storage;
    ثابت hb_map_t* unicode_glyphid_map = &unicode_glyphid_map_storage;
    ثابت hb_سیٹ_t* cmap_unicodes = &cmap_unicodes_storage;

    اگر (!plan->accelerator) {
      cmap.collect_mapping (&cmap_unicodes_storage, &unicode_glyphid_map_storage);
      plan->unicode_مقصد_new_gid_list.alloc (hb_min(unicodes.get_population ()
                                                  + glyphs->get_population (),
                                                  cmap_unicodes->get_population ()));
    } ورنہ {
      unicode_glyphid_map = &plan->accelerator->unicode_مقصد_gid_2;
      cmap_unicodes = &plan->accelerator->unicodes;
    }

    اگر (plan->accelerator &&
	unicodes.get_population () < cmap_unicodes->get_population () &&
	glyphs->get_population () < cmap_unicodes->get_population ())
    {
      plan->codepoint_مقصد_glyph->alloc (unicodes.get_population () + glyphs->get_population ());

      خودکار &gid_مقصد_unicodes = plan->accelerator->gid_مقصد_unicodes;

      برائے (hb_codepoint_t gid : *glyphs)
      {
        خودکار unicodes = gid_مقصد_unicodes.get (gid);
        _fill_unicode_and_glyph_map<سچ>(plan, unicodes, [&] (hb_codepoint_t cp) {
          واپس gid;
        },
        [&] (hb_codepoint_t cp) {
          واپس unicode_glyphid_map->get(cp);
        });
      }

      _fill_unicode_and_glyph_map(plan, unicodes.iter(), [&] (hb_codepoint_t cp) {
          /* Don't double-add entry. */
	اگر (plan->codepoint_مقصد_glyph->has (cp))
          واپس HB_MAP_قدر_INVALID;

        واپس unicode_glyphid_map->get(cp);
      },
      [&] (hb_codepoint_t cp) {
          واپس unicode_glyphid_map->get(cp);
      });

      plan->unicode_مقصد_new_gid_list.qsort ();
    }
    ورنہ
    {
      plan->codepoint_مقصد_glyph->alloc (cmap_unicodes->get_population ());
      hb_codepoint_t پہلا = HB_سیٹ_قدر_INVALID, آخری = HB_سیٹ_قدر_INVALID;
      برائے (; cmap_unicodes->اگلا_range (&پہلا, &آخری); )
      {
        _fill_unicode_and_glyph_map(plan, hb_range(پہلا, آخری + 1), [&] (hb_codepoint_t cp) {
          hb_codepoint_t gid = (*unicode_glyphid_map)[cp];
	  اگر (!unicodes.has (cp) && !glyphs->has (gid))
	    واپس HB_MAP_قدر_INVALID;
          واپس gid;
        },
        [&] (hb_codepoint_t cp) {
          واپس unicode_glyphid_map->get(cp);
        });
      }
    }

    /* Add gids which where requested, but not mapped in cmap */
    غیرعلامتی عدد_glyphs = plan->ماخذ->get_عدد_glyphs ();
    hb_codepoint_t پہلا = HB_سیٹ_قدر_INVALID, آخری = HB_سیٹ_قدر_INVALID;
    برائے (; glyphs->اگلا_range (&پہلا, &آخری); )
    {
      اگر (پہلا >= عدد_glyphs)
	توڑو;
      اگر (آخری >= عدد_glyphs)
        آخری = عدد_glyphs - 1;
      plan->_glyphset_gsub.جوڑو_range (پہلا, آخری);
    }
  }

  خودکار &arr = plan->unicode_مقصد_new_gid_list;
  اگر (arr.لمبائی)
  {
    plan->unicodes.جوڑو_sorted_array (&arr.arrayZ->پہلا, arr.لمبائی, حجم (*arr.arrayZ));
    plan->_glyphset_gsub.جوڑو_array (&arr.arrayZ->second, arr.لمبائی, حجم (*arr.arrayZ));
  }

  // Variation selectors don't have glyphs associated with them in the cmap so they will have been filtered out above
  // but should still be retained. Add them back here.

  // However, the min and max codepoints for OS/2 should be calculated without considering variation selectors,
  // so record those first.
  plan->os2_info.min_cmap_codepoint = plan->unicodes.get_min();
  plan->os2_info.max_cmap_codepoint = plan->unicodes.get_max();

  hb_سیٹ_t variation_selectors_مقصد_retain;
  cmap.collect_variation_selectors(&variation_selectors_مقصد_retain);
  + variation_selectors_مقصد_retain.iter()
  | hb_filter(unicodes)
  | hb_sink(&plan->unicodes)
  ;
}

ساکن غیرعلامتی
glyf_جوڑو_gid_and_children (ثابت OT::glyf_accelerator_t &glyf,
			    hb_codepoint_t gid,
			    hb_سیٹ_t *gids_مقصد_retain,
			    صحیح operation_گنتی,
			    غیرعلامتی گہرائی = 0)
{
  /* Check if is already visited */
  اگر (gids_مقصد_retain->has (gid)) واپس operation_گنتی;

  gids_مقصد_retain->جوڑو (gid);

  اگر (unlikely (گہرائی++ > HB_MAX_NESTING_LEVEL)) واپس operation_گنتی;
  اگر (unlikely (--operation_گنتی < 0)) واپس operation_گنتی;

  خودکار glyph = glyf.glyph_for_gid (gid);

  برائے (خودکار &شے : glyph.get_مرکب_iterator ())
    operation_گنتی =
      glyf_جوڑو_gid_and_children (glyf,
				  شے.get_gid (glyph.is_extended ()),
				  gids_مقصد_retain,
				  operation_گنتی,
				  گہرائی);

  واپس operation_گنتی;
}

ساکن خالی
nameid_احاطہ (hb_subset_plan_t* plan,
		 hb_سیٹ_t* drop_tables)
{
#اگر_غیر_تعریف_شدہ HB_NO_STYLE
  اگر (!drop_tables->has (HB_OT_لیبل_STAT))
    plan->ماخذ->جدول.STAT->collect_name_ids (&plan->user_axes_location, &plan->name_ids);
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (!plan->all_axes_pinned)
    plan->ماخذ->جدول.fvar->collect_name_ids (&plan->user_axes_location, &plan->axes_old_اشاریہ_لیبل_map, &plan->name_ids);
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_COLOR
  اگر (!drop_tables->has (HB_OT_لیبل_CPAL))
    plan->ماخذ->جدول.CPAL->collect_name_ids (&plan->colr_palettes, &plan->name_ids);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_LAYOUT
  layout_nameid_احاطہ(plan, drop_tables);
#اگر_ختم
}

ساکن خالی
populate_gids_مقصد_retain (hb_subset_plan_t* plan,
		          hb_سیٹ_t* drop_tables)
{
  OT::glyf_accelerator_t glyf (plan->ماخذ);
#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
  // Note: we cannot use inprogress_accelerator here, since it has not been
  // created yet. So in case of preprocessed-face (and otherwise), we do an
  // extra sanitize pass here, which is not ideal.
  OT::cff1::accelerator_subset_t ڈھیر_cff (plan->accelerator ? خالی_اشاریہ : plan->ماخذ);
  ثابت OT::cff1::accelerator_subset_t *cff (plan->accelerator ? plan->accelerator->cff1_accel.get () : &ڈھیر_cff);
#اگر_ختم

  plan->_glyphset_gsub.جوڑو (0); // Not-def

  cmap_احاطہ (plan->ماخذ, &plan->unicodes, &plan->_glyphset_gsub);
  plan->_glyphset_cmaped = plan->_glyphset_gsub;

  اگر (!drop_tables->has (HB_OT_لیبل_MATH))
  {
    math_احاطہ (plan, &plan->_glyphset_gsub);
    _remove_invalid_gids (&plan->_glyphset_gsub, plan->ماخذ->get_عدد_glyphs ());
  }
  plan->_glyphset_mathed = plan->_glyphset_gsub;

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_LAYOUT
  layout_populate_gids_مقصد_retain(plan, drop_tables);
#اگر_ختم

  _remove_invalid_gids (&plan->_glyphset_gsub, plan->ماخذ->get_عدد_glyphs ());

  hb_سیٹ_t cur_glyphset = plan->_glyphset_gsub;
  اگر (!drop_tables->has (HB_OT_لیبل_COLR))
  {
    colr_احاطہ (plan, &cur_glyphset);
    _remove_invalid_gids (&cur_glyphset, plan->ماخذ->get_عدد_glyphs ());
  }

  plan->_glyphset_colred = cur_glyphset;

#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES
  اگر (!drop_tables->has (OT::VARC::جدول_لیبل))
  {
    اگر (unlikely (!plan->ماخذ->جدول.VARC->احاطہ_glyphs (&cur_glyphset)))
    {
      plan->check_success (جھوٹ);
      واپس;
    }
    _remove_invalid_gids (&cur_glyphset, plan->ماخذ->get_عدد_glyphs ());
  }
#اگر_ختم

  plan->_glyphset_varced = cur_glyphset;

  nameid_احاطہ (plan, drop_tables);
  /* Populate a full set of glyphs to retain by adding all referenced
   * composite glyphs. */
  اگر (glyf.has_ڈیٹا ())
    برائے (hb_codepoint_t gid : cur_glyphset)
      glyf_جوڑو_gid_and_children (glyf, gid, &plan->_glyphset,
				  cur_glyphset.get_population () * HB_MAX_مرکب_OPERATIONS_فی_GLYPH);
  ورنہ
    plan->_glyphset.union_ (cur_glyphset);
#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
  اگر (!plan->accelerator || plan->accelerator->has_seac)
  {
    منطقی has_seac = جھوٹ;
    اگر (cff->is_valid ())
      برائے (hb_codepoint_t gid : cur_glyphset)
	اگر (جوڑو_cff_seac_components (*cff, gid, &plan->_glyphset))
	  has_seac = سچ;
    plan->has_seac = has_seac;
  }
#اگر_ختم

  _remove_invalid_gids (&plan->_glyphset, plan->ماخذ->get_عدد_glyphs ());

#اگر_غیر_تعریف_شدہ HB_NO_VAR
#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_LAYOUT
  اگر (!drop_tables->has (HB_OT_لیبل_GDEF))
    collect_layout_variation_indices (plan);
#اگر_ختم
#اگر_ختم
}

ساکن منطقی
_create_glyph_map_gsub (ثابت hb_سیٹ_t* glyph_سیٹ_gsub,
                        ثابت hb_map_t* glyph_map,
                        hb_map_t* برآمد,
                        hb_سمتیہ_t<hb_codepoint_t>* برآمد_flat)
{
  اگر (unlikely (glyph_سیٹ_gsub->اندر_error ()))
    واپس جھوٹ;

  hb_codepoint_t max_gid = HB_سیٹ_قدر_INVALID;
  hb_سیٹ_پچھلا (glyph_سیٹ_gsub, &max_gid);
  غیرعلامتی flat_سائز = max_gid == HB_سیٹ_قدر_INVALID ? 0 : max_gid + 1;
  اگر (unlikely (!برآمد_flat->resize_dirty (flat_سائز)))
    واپس جھوٹ;
  hb_memset (برآمد_flat->arrayZ, 0xFF, flat_سائز * حجم (hb_codepoint_t));

  برآمد->alloc (glyph_سیٹ_gsub->get_population ());
  برائے (خودکار gid : *glyph_سیٹ_gsub)
  {
    hb_codepoint_t new_gid = glyph_map->get (gid);
    برآمد->سیٹ (gid, new_gid);
    برآمد_flat->arrayZ[gid] = new_gid;
  }
  واپس !برآمد->اندر_error ();
}

ساکن منطقی
create_old_gid_مقصد_new_gid_map (ثابت hb_face_t *face,
				منطقی		 retain_gids,
				ثابت hb_سیٹ_t	*all_gids_مقصد_retain,
                                ثابت hb_map_t  *requested_glyph_map,
				hb_map_t	*glyph_map, /*برآمد*/
				hb_map_t	*الٹ_glyph_map, /*برآمد*/
				hb_sorted_سمتیہ_t<hb_codepoint_pair_t> *new_مقصد_old_gid_list /*برآمد*/,
				غیرعلامتی صحیح	*عدد_glyphs /*برآمد*/)
{
  غیرعلامتی نکالو = all_gids_مقصد_retain->get_population ();
  الٹ_glyph_map->alloc (نکالو);
  glyph_map->alloc (نکالو);
  new_مقصد_old_gid_list->alloc (نکالو);

  اگر (*requested_glyph_map)
  {
    hb_سیٹ_t new_gids(requested_glyph_map->values());
    اگر (new_gids.get_population() != requested_glyph_map->get_population())
    {
      DEBUG_پیغام (SUBSET, خالی_اشاریہ, "The provided custom glyph mapping is not unique.");
      واپس جھوٹ;
    }

    اگر (retain_gids)
    {
      DEBUG_پیغام (SUBSET, خالی_اشاریہ,
        "HB_SUBSET_FLAGS_RETAIN_GIDS cannot be set if "
        "a custom glyph mapping has been provided.");
      واپس جھوٹ;
    }

    hb_codepoint_t max_glyph = 0;
    hb_سیٹ_t بقایا;
    برائے (خودکار old_gid : all_gids_مقصد_retain->iter ())
    {
      اگر (old_gid == 0) {
	new_مقصد_old_gid_list->push (hb_pair<hb_codepoint_t, hb_codepoint_t> (0u, 0u));
        جاری;
      }

      hb_codepoint_t* new_gid;
      اگر (!requested_glyph_map->has (old_gid, &new_gid))
      {
        بقایا.جوڑو(old_gid);
        جاری;
      }

      اگر (*new_gid > max_glyph)
        max_glyph = *new_gid;
      new_مقصد_old_gid_list->push (hb_pair (*new_gid, old_gid));
    }
    new_مقصد_old_gid_list->qsort ();

    // Anything that wasn't mapped by the requested mapping should
    // be placed after the requested mapping.
    برائے (خودکار old_gid : بقایا)
      new_مقصد_old_gid_list->push (hb_pair (++max_glyph, old_gid));

    *عدد_glyphs = max_glyph + 1;
  }
  ورنہ اگر (!retain_gids)
  {
    + hb_enumerate (hb_iter (all_gids_مقصد_retain), (hb_codepoint_t) 0)
    | hb_sink (new_مقصد_old_gid_list)
    ;
    *عدد_glyphs = new_مقصد_old_gid_list->لمبائی;
  }
  ورنہ
  {
    + hb_iter (all_gids_مقصد_retain)
    | hb_map ([] (hb_codepoint_t _) {
		واپس hb_codepoint_pair_t (_, _);
	      })
    | hb_sink (new_مقصد_old_gid_list)
    ;

    hb_codepoint_t max_glyph = HB_سیٹ_قدر_INVALID;
    hb_سیٹ_پچھلا (all_gids_مقصد_retain, &max_glyph);

    *عدد_glyphs = max_glyph + 1;
  }

  الٹ_glyph_map->alloc (الٹ_glyph_map->get_population () + new_مقصد_old_gid_list->لمبائی);
  + hb_iter (new_مقصد_old_gid_list)
  | hb_sink (الٹ_glyph_map)
  ;
  glyph_map->alloc (glyph_map->get_population () + new_مقصد_old_gid_list->لمبائی);
  + hb_iter (new_مقصد_old_gid_list)
  | hb_map (&hb_codepoint_pair_t::reverse)
  | hb_sink (glyph_map)
  ;

  واپس سچ;
}

hb_subset_plan_t::hb_subset_plan_t (hb_face_t *face,
				    ثابت hb_subset_داخلہ_t *داخلہ)
{
  successful = سچ;
  flags = داخلہ->flags;

  unicode_مقصد_new_gid_list.init ();

  name_ids = *داخلہ->sets.name_ids;
  name_languages = *داخلہ->sets.name_languages;
  layout_features = *داخلہ->sets.layout_features;
  layout_scripts = *داخلہ->sets.layout_scripts;
  glyphs_requested = *داخلہ->sets.glyphs;
  drop_tables = *داخلہ->sets.drop_tables;
  no_subset_tables = *داخلہ->sets.no_subset_tables;
  ماخذ = hb_face_reference (face);
  dest = hb_face_builder_create ();

  codepoint_مقصد_glyph = hb_map_create ();
  glyph_map = hb_map_create ();
  الٹ_glyph_map = hb_map_create ();

  gsub_داخل_کرو_catch_all_feature_variation_rec = جھوٹ;
  gpos_داخل_کرو_catch_all_feature_variation_rec = جھوٹ;
  gdef_varstore_inner_maps.init ();

  user_axes_location = داخلہ->axes_location;
  all_axes_pinned = جھوٹ;
  pinned_at_default = سچ;
  has_gdef_varstore = جھوٹ;
  has_avar2 = جھوٹ;

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
  برائے (خودکار _ : داخلہ->name_جدول_overrides)
  {
    hb_بائٹیں_t name_بائٹیں = _.second;
    غیرعلامتی len = name_بائٹیں.لمبائی;
    حرف *name_str = (حرف *) hb_malloc (len);
    اگر (unlikely (!check_success (name_str)))
      توڑو;

    hb_memcpy (name_str, name_بائٹیں.arrayZ, len);
    name_جدول_overrides.سیٹ (_.پہلا, hb_بائٹیں_t (name_str, len));
  }
#اگر_ختم

  خالی* accel = hb_face_get_user_ڈیٹا(face, hb_subset_accelerator_t::user_ڈیٹا_کلید());

  attach_accelerator_ڈیٹا = داخلہ->attach_accelerator_ڈیٹا;
  force_long_loca = داخلہ->force_long_loca;
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
  force_long_loca = force_long_loca || (flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#اگر_ختم

  اگر (accel)
    accelerator = (hb_subset_accelerator_t*) accel;

  اگر (unlikely (اندر_error ()))
    واپس;

#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES
  اگر (!داخلہ->sets.drop_tables->has (OT::VARC::جدول_لیبل) &&
      داخلہ->sets.no_subset_tables->has (OT::VARC::جدول_لیبل) &&
      !(داخلہ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      face->جدول.VARC->has_ڈیٹا ())
  {
    check_success (جھوٹ);
    واپس;
  }
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (!check_success (normalize_axes_location (face, یہ)))
      واپس;
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES
  اگر (!user_axes_location.is_empty () &&
      !داخلہ->sets.drop_tables->has (OT::VARC::جدول_لیبل) &&
      داخلہ->sets.no_subset_tables->has (OT::VARC::جدول_لیبل) &&
      face->جدول.VARC->has_ڈیٹا ())
  {
    check_success (جھوٹ);
    واپس;
  }
#اگر_ختم

  populate_unicodes_مقصد_retain (داخلہ->sets.unicodes, داخلہ->sets.glyphs, یہ);

  populate_gids_مقصد_retain (یہ, داخلہ->sets.drop_tables);
  اگر (unlikely (اندر_error ()))
    واپس;

  اگر (!check_success(create_old_gid_مقصد_new_gid_map(
          face,
          داخلہ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS,
          &_glyphset,
          &داخلہ->glyph_map,
          glyph_map,
          الٹ_glyph_map,
	  &new_مقصد_old_gid_list,
          &عدد_output_glyphs_2))) {
    واپس;
  }

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API  
  اگر ((داخلہ->flags & HB_SUBSET_FLAGS_RETAIN_GIDS) &&
      (داخلہ->flags & HB_SUBSET_FLAGS_RETAIN_عدد_GLYPHS)) {
    // We've been requested to maintain the num glyphs count from the
    // input face.
    عدد_output_glyphs_2 = ماخذ->get_عدد_glyphs ();
  }
#اگر_ختم

  اگر (!check_success (_create_glyph_map_gsub (
      &_glyphset_gsub,
      glyph_map,
      &glyph_map_gsub,
      &glyph_map_gsub_flat)))
    واپس;

  // Now that we have old to new gid map update the unicode to new gid list.
  برائے (غیرعلامتی i = 0; i < unicode_مقصد_new_gid_list.لمبائی; i++)
  {
    // Use raw array access for performance.
    unicode_مقصد_new_gid_list.arrayZ[i].second =
        glyph_map->get(unicode_مقصد_new_gid_list.arrayZ[i].second);
  }

  bounds_چوڑائی_vec.resize_dirty  (عدد_output_glyphs_2);
  برائے (خودکار &v : bounds_چوڑائی_vec)
    v = 0xFFFFFFFF;
  bounds_اونچائی_vec.resize_dirty  (عدد_output_glyphs_2);
  برائے (خودکار &v : bounds_اونچائی_vec)
    v = 0xFFFFFFFF;

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_LAYOUT    
  اگر (!drop_tables.has (HB_OT_لیبل_GDEF))
    remap_مستعمل_mark_sets (یہ, مستعمل_mark_sets_map);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_VAR
#اگر_غیر_تعریف_شدہ HB_NO_بنیاد
  اگر (!drop_tables.has (HB_OT_لیبل_بنیاد))
    collect_بنیاد_variation_indices (یہ);
#اگر_ختم
#اگر_ختم

  اگر (unlikely (اندر_error ()))
    واپس;

#اگر !defined(HB_NO_VAR) && !defined(HB_NO_OT_FONT_CFF)
  update_instance_metrics_map_from_cff2 (یہ);
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (!check_success (get_instance_glyphs_contour_points (یہ)))
      واپس;
#اگر_ختم

  اگر (attach_accelerator_ڈیٹا)
  {
    inprogress_accelerator =
      hb_subset_accelerator_t::create (ماخذ,
				       *codepoint_مقصد_glyph,
                                       unicodes,
				       has_seac);

    check_success (inprogress_accelerator);
  }

#تعریف_کرو HB_SUBSET_PLAN_MEMBER(Type, Name) check_success (!Name.اندر_error ());
#شامل "hb-subset-plan-member-list.hh"
#تعریف_منسوخ HB_SUBSET_PLAN_MEMBER
}

hb_subset_plan_t::~hb_subset_plan_t()
{
  hb_face_destroy (dest);

  hb_map_destroy (codepoint_مقصد_glyph);
  hb_map_destroy (glyph_map);
  hb_map_destroy (الٹ_glyph_map);
#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
  cff1_accel.fini ();
  cff2_accel.fini ();
#اگر_ختم
  hb_face_destroy (ماخذ);

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
  برائے (خودکار _ : name_جدول_overrides.iter_ref ())
    _.second.fini ();
#اگر_ختم

  اگر (inprogress_accelerator)
    hb_subset_accelerator_t::destroy ((خالی*) inprogress_accelerator);
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
                               ثابت hb_subset_داخلہ_t *داخلہ)
{
  hb_subset_plan_t *plan;
  اگر (unlikely (!(plan = hb_object_create<hb_subset_plan_t> (face, داخلہ))))
    واپس خالی_اشاریہ;

  اگر (unlikely (plan->اندر_error ()))
  {
    hb_subset_plan_destroy (plan);
    واپس خالی_اشاریہ;
  }

  واپس plan;
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
خالی
hb_subset_plan_destroy (hb_subset_plan_t *plan)
{
  اگر (!hb_object_destroy (plan)) واپس;

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
  واپس plan->glyph_map;
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
  واپس plan->الٹ_glyph_map;
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
  واپس plan->codepoint_مقصد_glyph;
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
  واپس hb_object_reference (plan);
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
hb_subset_plan_سیٹ_user_ڈیٹا (hb_subset_plan_t   *plan,
                              hb_user_ڈیٹا_کلید_t *key,
                              خالی               *ڈیٹا,
                              hb_destroy_func_t   destroy,
                              hb_bool_t	          replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (plan, key, ڈیٹا, destroy, replace);
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
خالی *
hb_subset_plan_get_user_ڈیٹا (ثابت hb_subset_plan_t *plan,
                              hb_user_ڈیٹا_کلید_t     *key)
{
  واپس hb_object_get_user_ڈیٹا (plan, key);
}

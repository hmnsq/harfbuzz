/*
 * Copyright © 2011,2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod, Roderick Sheeter
 */

#جيڪڏهن_وصف_نه_ٿيل HB_OT_HMTX_جدول_HH
#وصف_ڪريو HB_OT_HMTX_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"
#ضمن "hb-ot-maxp-table.hh"
#ضمن "hb-ot-hhea-table.hh"
#ضمن "hb-ot-os2-table.hh"
#ضمن "hb-ot-var-hvar-table.hh"
#ضمن "hb-ot-var-mvar-table.hh"
#ضمن "hb-ot-metrics.hh"

/*
 * hmtx -- Horizontal Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hmtx
 * vmtx -- Vertical Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vmtx
 */
#وصف_ڪريو HB_OT_ٽيگ_hmtx HB_ٽيگ('h','m','t','x')
#وصف_ڪريو HB_OT_ٽيگ_HMTX HB_ٽيگ('H','M','T','X')
#وصف_ڪريو HB_OT_ٽيگ_vmtx HB_ٽيگ('v','m','t','x')
#وصف_ڪريو HB_OT_ٽيگ_VMTX HB_ٽيگ('V','M','T','X')


نطاق OT {


بنية LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  عام:
  DEFINE_سائيز_STATIC (4);
};


سانچو <قسم_نالو T/*Data table type*/, قسم_نالو H/*Header table type*/, قسم_نالو V/*Var table type*/>
بنية hmtxvmtx
{
  منطقي sanitize (hb_sanitize_context_t *c HB_اڻ_استعمال_ٿيل) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (سچ);
  }

  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشان, صحيح>>* get_mtx_map (ثابت hb_subset_plan_t *plan) ثابت
  { أرجع T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  منطقي subset_update_سرو (hb_subset_context_t *c,
			     بنانشان صحيح انگ_hmetrics,
			     ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشان, صحيح>> *mtx_map,
			     ثابت hb_ويڪٽر_t<بنانشان> &bounds_vec) ثابت
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_جدول<H> (c->plan->ذريعو, H::جدول_ٽيگ);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    إذا (unlikely (!dest_blob)) {
      أرجع خطأ;
    }

    بنانشان صحيح ڊيگهائي;
    H *جدول = (H *) hb_blob_get_معلومات (dest_blob, &ڊيگهائي);
    c->serializer->check_سونپيو (جدول->عدد_جو_Long_Metrics, انگ_hmetrics, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودڪار &MVAR = *c->plan->ذريعو->جدول.MVAR;
      إذا (T::is_horizontal)
      {
	HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_RISE,   caret_لاهي_Rise);
	HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_لاڳيتو,    caret_لاهي_لاڳيتو);
	HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_وٿي, caret_وٿي);
      }
      وإلا
      {
	HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_VERTICAL_CARET_RISE,     caret_لاهي_Rise);
	HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_VERTICAL_CARET_لاڳيتو,      caret_لاهي_لاڳيتو);
	HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_VERTICAL_CARET_وٿي,   caret_وٿي);
      }

      منطقي خالي_جايون_2 = سچ;
      صحيح min_lsb = 0x7FFF;
      صحيح min_rsb = 0x7FFF;
      صحيح max_extent = -0x7FFF;
      بنانشان max_adv = 0;
      لكل (ثابت خودڪار _ : *mtx_map)
      {
        hb_codepoint_t gid = _.پهريون;
        بنانشان adv = _.second.پهريون;
        صحيح lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        إذا (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  خالي_جايون_2 = خطأ;
          بنانشان bound_ويڪر = bounds_vec[gid];
          صحيح rsb = adv - lsb - bound_ويڪر;
          صحيح extent = lsb + bound_ويڪر;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      جدول->advanceMax = max_adv;
      إذا (!خالي_جايون_2)
      {
        جدول->minLeadingBearing = min_lsb;
        جدول->minTrailingBearing = min_rsb;
        جدول->maxExtent = max_extent;
      }

      إذا (T::is_horizontal)
      {
        ثابت خودڪار &OS2 = *c->plan->ذريعو->جدول.OS2;
        إذا (OS2.has_معلومات () &&
            جدول->ascender == OS2.sTypoAscender &&
            جدول->descender == OS2.sTypoDescender &&
            جدول->سٽ_Gap == OS2.s_Typo_سٽ_Gap)
        {
          جدول->ascender = static_cast<صحيح> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.ڊيگهائي)));
          جدول->descender = static_cast<صحيح> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.ڊيگهائي)));
          جدول->سٽ_Gap = static_cast<صحيح> (roundf (OS2.s_Typo_سٽ_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.ڊيگهائي)));
        }
      }
    }
#جيڪڏهن_پورو

    منطقي نتيجو = c->plan->جوڙيو_جدول (H::جدول_ٽيگ, dest_blob);
    hb_blob_destroy (dest_blob);

    أرجع نتيجو;
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بنانشان انگ_long_metrics,
                  بنانشان ڪل_انگ_metrics)
  {
    LongMetric* long_metrics = c->allocate_سائيز<LongMetric> (انگ_long_metrics * LongMetric::static_سائيز);
    FWORD* short_metrics = c->allocate_سائيز<FWORD> ((ڪل_انگ_metrics - انگ_long_metrics) * FWORD::static_سائيز);
    إذا (!long_metrics || !short_metrics) أرجع;

    short_metrics -= انگ_long_metrics;

    لكل (خودڪار _ : new_منزل_old_gid_list)
    {
      hb_codepoint_t gid = _.پهريون;
      خودڪار mtx = *it++;

      إذا (gid < انگ_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.پهريون;
	lm.sb = mtx.second;
      }
      وإلا
        short_metrics[gid] = mtx.second;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خودڪار *جدول_مفرد = c->serializer->شروعات_embed <T> ();

    accelerator_t _mtx (c->plan->ذريعو);
    بنانشان انگ_long_metrics;
    ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشان, صحيح>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      خودڪار& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      انگ_long_metrics = hb_min (plan->انگ_output_glyphs (), H::long_metric_ڳڻپ_max ());
      بنانشان صحيح آخري_advance = get_new_gid_advance_unscaled (plan, mtx_map, انگ_long_metrics - 1, _mtx);
      طالما (انگ_long_metrics > 1 &&
	     آخري_advance == get_new_gid_advance_unscaled (plan, mtx_map, انگ_long_metrics - 2, _mtx))
      {
	انگ_long_metrics--;
      }
    }

    خودڪار it =
    + hb_iter (c->plan->new_منزل_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.پهريون;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<بنانشان, صحيح> *v = خالي_اشارو;
		إذا (!mtx_map->has (new_gid, &v))
		{
		  صحيح lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  أرجع hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		أرجع *v;
	      })
    ;

    جدول_مفرد->serialize (c->serializer,
			    it,
			    c->plan->new_منزل_old_gid_list,
			    انگ_long_metrics,
			    c->plan->انگ_output_glyphs ());

    إذا (unlikely (c->serializer->اندريون_error ()))
      return_trace (خطأ);

    // Amend header num hmetrics
    إذا (unlikely (!subset_update_سرو (c, انگ_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_ويڪر_vec : c->plan->bounds_اوچائي_vec)))
      return_trace (خطأ);

    return_trace (سچ);
  }

  بنية accelerator_t
  {
    دوست بنية hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::جدول_ٽيگ, get_انگ_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_ٽيگ_t جدول_ٽيگ, بنانشان انگ_long_metrics_2)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<hmtxvmtx> (face, جدول_ٽيگ);
      var_جدول = hb_sanitize_context_t ().reference_جدول<V> (face, T::variations_ٽيگ);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      بنانشان صحيح len = جدول.get_ڊيگهائي ();
      إذا (len & 1)
        len--;

      انگ_long_metrics = انگ_long_metrics_2;
      إذا (unlikely (انگ_long_metrics > len / 4))
	انگ_long_metrics = len / 4;
      len -= انگ_long_metrics * 4;

      انگ_bearings = face->جدول.maxp->get_انگ_glyphs ();

      إذا (unlikely (انگ_bearings < انگ_long_metrics))
        انگ_bearings = انگ_long_metrics;
      إذا (unlikely ((انگ_bearings - انگ_long_metrics) * 2 > len))
        انگ_bearings = انگ_long_metrics + len / 2;
      len -= (انگ_bearings - انگ_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      إذا (unlikely (!انگ_long_metrics))
	انگ_bearings = انگ_long_metrics = 0;

      انگ_advances = انگ_bearings + len / 2;
      انگ_glyphs = face->get_انگ_glyphs ();
      إذا (انگ_glyphs < انگ_advances)
        انگ_glyphs = انگ_advances;
    }
    ~accelerator_t ()
    {
      جدول.destroy ();
      var_جدول.destroy ();
    }

    منطقي has_معلومات () ثابت { أرجع (منطقي) انگ_bearings; }

    فراغ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   صحيح *lsb) ثابت
    {
      إذا (glyph < انگ_long_metrics)
      {
	*lsb = جدول->longMetricZ[glyph].sb;
	أرجع;
      }

      إذا (unlikely (glyph >= انگ_bearings))
      {
        *lsb = 0;
	أرجع;
      }

      ثابت FWORD *bearings = (ثابت FWORD *) &جدول->longMetricZ[انگ_long_metrics];
      *lsb = bearings[glyph - انگ_long_metrics];
    }

    بنانشان صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
    {
      إذا (glyph < انگ_bearings)
	أرجع جدول->longMetricZ[hb_min (glyph, (uint32_t) انگ_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      إذا (unlikely (!انگ_advances))
	أرجع default_advance;

      أرجع 0;
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    بنانشان get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
    {
      بنانشان صحيح advance = get_advance_without_var_unscaled (glyph);
      أرجع hb_clamp_منزل<بنانشان> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								      font->coords, font->انگ_coords,
								      store_cache)));
    }
#جيڪڏهن_پورو

    محمي:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    بنانشان انگ_long_metrics;
    بنانشان انگ_bearings;
    بنانشان انگ_advances;
    بنانشان انگ_glyphs;

    بنانشان صحيح default_advance;

    عام:
    hb_blob_ptr_t<hmtxvmtx> جدول;
    hb_blob_ptr_t<V> var_جدول;

    خاص:
    ساكن بنانشان get_انگ_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_جدول<H> (face);
      بنانشان ret = hea->عدد_جو_Long_Metrics;
      hea.destroy ();
      أرجع ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  بنانشان get_new_gid_advance_unscaled (ثابت hb_subset_plan_t *plan,
                                         ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشان, صحيح>> *mtx_map,
                                         بنانشان new_gid,
                                         ثابت accelerator_t &_mtx) ثابت
  {
    إذا (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      أرجع plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    أرجع mtx_map->get (new_gid).پهريون;
  }

  محمي:
  Unsized_Array_جو<LongMetric>
		longMetricZ;	/* Paired advance width and leading
				 * bearing values for each glyph. The
				 * value numOfHMetrics comes from
				 * the 'hhea' table. If the font is
				 * monospaced, only one entry need
				 * be in the array, but that entry is
				 * required. The last entry applies to
				 * all subsequent glyphs. */
/*UnsizedArrayOf<FWORD>	leadingBearingX;*/
				/* Here the advance is assumed
				 * to be the same as the advance
				 * for the last entry above. The
				 * number of entries in this array is
				 * derived from numGlyphs (from 'maxp'
				 * table) minus numberOfLongMetrics.
				 * This generally is used with a run
				 * of monospaced glyphs (e.g., Kanji
				 * fonts or Courier fonts). Only one
				 * run is allowed and it must be at
				 * the end. This allows a monospaced
				 * font to vary the side bearing
				 * values for each glyph. */
/*UnsizedArrayOf<UFWORD>advancesX;*/
				/* TODO Document. */
  عام:
  DEFINE_سائيز_ARRAY (0, longMetricZ);
};

بنية hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_hmtx;
  ساكن constexpr hb_ٽيگ_t variations_ٽيگ = HB_OT_ٽيگ_HVAR;
  ساكن constexpr منطقي is_horizontal = سچ;
};
بنية HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_HMTX;
  ساكن constexpr hb_ٽيگ_t variations_ٽيگ = HB_OT_ٽيگ_HVAR;
  ساكن constexpr منطقي is_horizontal = سچ;
};
بنية vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_vmtx;
  ساكن constexpr hb_ٽيگ_t variations_ٽيگ = HB_OT_ٽيگ_VVAR;
  ساكن constexpr منطقي is_horizontal = خطأ;
};
بنية VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_VMTX;
  ساكن constexpr hb_ٽيگ_t variations_ٽيگ = HB_OT_ٽيگ_VVAR;
  ساكن constexpr منطقي is_horizontal = خطأ;
};

بنية hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
						  HMTX::accelerator_t>
{
  hmtx_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<hmtx::accelerator_t, HMTX::accelerator_t> (face)
  {
    var_جدول = hb_sanitize_context_t ().reference_جدول<HVAR> (face);
  }

  ~hmtx_accelerator_t ()
  {
    var_جدول.destroy ();
  }

  فراغ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 صحيح *lsb) ثابت
  {
    HB_DUAL_GET (*هذا, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  بنانشان صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  بنانشان get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
    بنانشان صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_منزل<بنانشان> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->انگ_coords,
								    store_cache)));
  }
#جيڪڏهن_پورو

  hb_blob_ptr_t<HVAR> var_جدول;
};

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
بنية vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
						  VMTX::accelerator_t>
{
  vmtx_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<vmtx::accelerator_t, VMTX::accelerator_t> (face)
  {
    var_جدول = hb_sanitize_context_t ().reference_جدول<VVAR> (face);
  }

  ~vmtx_accelerator_t ()
  {
    var_جدول.destroy ();
  }

  فراغ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 صحيح *lsb) ثابت
  {
    HB_DUAL_GET (*هذا, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  بنانشان صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  بنانشان get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
    بنانشان صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_منزل<بنانشان> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->انگ_coords,
								    store_cache)));
  }
#جيڪڏهن_پورو

  hb_blob_ptr_t<VVAR> var_جدول;
};
#وإلا
بنية vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#جيڪڏهن_پورو

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_HMTX_TABLE_HH */

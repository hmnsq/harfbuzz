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

#اگر_غیر_تعریف_شدہ HB_OT_HMTX_جدول_HH
#تعریف_کرو HB_OT_HMTX_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-dual.hh"
#شامل "hb-ot-maxp-table.hh"
#شامل "hb-ot-hhea-table.hh"
#شامل "hb-ot-os2-table.hh"
#شامل "hb-ot-var-hvar-table.hh"
#شامل "hb-ot-var-mvar-table.hh"
#شامل "hb-ot-metrics.hh"

/*
 * hmtx -- Horizontal Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hmtx
 * vmtx -- Vertical Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vmtx
 */
#تعریف_کرو HB_OT_لیبل_hmtx HB_لیبل('h','m','t','x')
#تعریف_کرو HB_OT_لیبل_HMTX HB_لیبل('H','M','T','X')
#تعریف_کرو HB_OT_لیبل_vmtx HB_لیبل('v','m','t','x')
#تعریف_کرو HB_OT_لیبل_VMTX HB_لیبل('V','M','T','X')


نامخلا OT {


ساخت LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  عوامی:
  DEFINE_سائز_STATIC (4);
};


سانچہ <قسم_نام T/*Data table type*/, قسم_نام H/*Header table type*/, قسم_نام V/*Var table type*/>
ساخت hmtxvmtx
{
  منطقی sanitize (hb_sanitize_context_t *c HB_غیر_مستعمل) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (سچ);
  }

  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<غیرعلامتی, صحیح>>* get_mtx_map (ثابت hb_subset_plan_t *plan) ثابت
  { واپس T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  منطقی subset_update_سرخی (hb_subset_context_t *c,
			     غیرعلامتی صحیح عدد_hmetrics,
			     ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<غیرعلامتی, صحیح>> *mtx_map,
			     ثابت hb_سمتیہ_t<غیرعلامتی> &bounds_vec) ثابت
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_جدول<H> (c->plan->ماخذ, H::جدول_لیبل);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    اگر (unlikely (!dest_blob)) {
      واپس جھوٹ;
    }

    غیرعلامتی صحیح لمبائی;
    H *جدول = (H *) hb_blob_get_ڈیٹا (dest_blob, &لمبائی);
    c->serializer->check_تفویض (جدول->نمبر_کا_Long_Metrics, عدد_hmetrics, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->ماخذ->جدول.MVAR;
      اگر (T::is_horizontal)
      {
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_HORIZONTAL_CARET_RISE,   caret_ڈھلان_Rise);
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_HORIZONTAL_CARET_تسلسل,    caret_ڈھلان_تسلسل);
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_HORIZONTAL_CARET_انحراف, caret_انحراف);
      }
      ورنہ
      {
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_VERTICAL_CARET_RISE,     caret_ڈھلان_Rise);
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_VERTICAL_CARET_تسلسل,      caret_ڈھلان_تسلسل);
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_VERTICAL_CARET_انحراف,   caret_انحراف);
      }

      منطقی خالی_خانے_2 = سچ;
      صحیح min_lsb = 0x7FFF;
      صحیح min_rsb = 0x7FFF;
      صحیح max_extent = -0x7FFF;
      غیرعلامتی max_adv = 0;
      برائے (ثابت خودکار _ : *mtx_map)
      {
        hb_codepoint_t gid = _.پہلا;
        غیرعلامتی adv = _.second.پہلا;
        صحیح lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        اگر (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  خالی_خانے_2 = جھوٹ;
          غیرعلامتی bound_چوڑائی = bounds_vec[gid];
          صحیح rsb = adv - lsb - bound_چوڑائی;
          صحیح extent = lsb + bound_چوڑائی;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      جدول->advanceMax = max_adv;
      اگر (!خالی_خانے_2)
      {
        جدول->minLeadingBearing = min_lsb;
        جدول->minTrailingBearing = min_rsb;
        جدول->maxExtent = max_extent;
      }

      اگر (T::is_horizontal)
      {
        ثابت خودکار &OS2 = *c->plan->ماخذ->جدول.OS2;
        اگر (OS2.has_ڈیٹا () &&
            جدول->ascender == OS2.sTypoAscender &&
            جدول->descender == OS2.sTypoDescender &&
            جدول->سطر_Gap == OS2.s_Typo_سطر_Gap)
        {
          جدول->ascender = static_cast<صحیح> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.لمبائی)));
          جدول->descender = static_cast<صحیح> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.لمبائی)));
          جدول->سطر_Gap = static_cast<صحیح> (roundf (OS2.s_Typo_سطر_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.لمبائی)));
        }
      }
    }
#اگر_ختم

    منطقی نتیجہ = c->plan->جوڑو_جدول (H::جدول_لیبل, dest_blob);
    hb_blob_destroy (dest_blob);

    واپس نتیجہ;
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_مقصد_old_gid_list,
		  غیرعلامتی عدد_long_metrics,
                  غیرعلامتی کل_عدد_metrics)
  {
    LongMetric* long_metrics = c->allocate_سائز<LongMetric> (عدد_long_metrics * LongMetric::static_سائز);
    FWORD* short_metrics = c->allocate_سائز<FWORD> ((کل_عدد_metrics - عدد_long_metrics) * FWORD::static_سائز);
    اگر (!long_metrics || !short_metrics) واپس;

    short_metrics -= عدد_long_metrics;

    برائے (خودکار _ : new_مقصد_old_gid_list)
    {
      hb_codepoint_t gid = _.پہلا;
      خودکار mtx = *it++;

      اگر (gid < عدد_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.پہلا;
	lm.sb = mtx.second;
      }
      ورنہ
        short_metrics[gid] = mtx.second;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);

    خودکار *جدول_مفرد = c->serializer->آغاز_embed <T> ();

    accelerator_t _mtx (c->plan->ماخذ);
    غیرعلامتی عدد_long_metrics;
    ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<غیرعلامتی, صحیح>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      خودکار& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      عدد_long_metrics = hb_min (plan->عدد_output_glyphs (), H::long_metric_گنتی_max ());
      غیرعلامتی صحیح آخری_advance = get_new_gid_advance_unscaled (plan, mtx_map, عدد_long_metrics - 1, _mtx);
      جب (عدد_long_metrics > 1 &&
	     آخری_advance == get_new_gid_advance_unscaled (plan, mtx_map, عدد_long_metrics - 2, _mtx))
      {
	عدد_long_metrics--;
      }
    }

    خودکار it =
    + hb_iter (c->plan->new_مقصد_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.پہلا;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<غیرعلامتی, صحیح> *v = خالی_اشاریہ;
		اگر (!mtx_map->has (new_gid, &v))
		{
		  صحیح lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  واپس hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		واپس *v;
	      })
    ;

    جدول_مفرد->serialize (c->serializer,
			    it,
			    c->plan->new_مقصد_old_gid_list,
			    عدد_long_metrics,
			    c->plan->عدد_output_glyphs ());

    اگر (unlikely (c->serializer->اندر_error ()))
      return_trace (جھوٹ);

    // Amend header num hmetrics
    اگر (unlikely (!subset_update_سرخی (c, عدد_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_چوڑائی_vec : c->plan->bounds_اونچائی_vec)))
      return_trace (جھوٹ);

    return_trace (سچ);
  }

  ساخت accelerator_t
  {
    دوست ساخت hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::جدول_لیبل, get_عدد_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_لیبل_t جدول_لیبل, غیرعلامتی عدد_long_metrics_2)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<hmtxvmtx> (face, جدول_لیبل);
      var_جدول = hb_sanitize_context_t ().reference_جدول<V> (face, T::variations_لیبل);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      غیرعلامتی صحیح len = جدول.get_لمبائی ();
      اگر (len & 1)
        len--;

      عدد_long_metrics = عدد_long_metrics_2;
      اگر (unlikely (عدد_long_metrics > len / 4))
	عدد_long_metrics = len / 4;
      len -= عدد_long_metrics * 4;

      عدد_bearings = face->جدول.maxp->get_عدد_glyphs ();

      اگر (unlikely (عدد_bearings < عدد_long_metrics))
        عدد_bearings = عدد_long_metrics;
      اگر (unlikely ((عدد_bearings - عدد_long_metrics) * 2 > len))
        عدد_bearings = عدد_long_metrics + len / 2;
      len -= (عدد_bearings - عدد_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      اگر (unlikely (!عدد_long_metrics))
	عدد_bearings = عدد_long_metrics = 0;

      عدد_advances = عدد_bearings + len / 2;
      عدد_glyphs = face->get_عدد_glyphs ();
      اگر (عدد_glyphs < عدد_advances)
        عدد_glyphs = عدد_advances;
    }
    ~accelerator_t ()
    {
      جدول.destroy ();
      var_جدول.destroy ();
    }

    منطقی has_ڈیٹا () ثابت { واپس (منطقی) عدد_bearings; }

    خالی get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   صحیح *lsb) ثابت
    {
      اگر (glyph < عدد_long_metrics)
      {
	*lsb = جدول->longMetricZ[glyph].sb;
	واپس;
      }

      اگر (unlikely (glyph >= عدد_bearings))
      {
        *lsb = 0;
	واپس;
      }

      ثابت FWORD *bearings = (ثابت FWORD *) &جدول->longMetricZ[عدد_long_metrics];
      *lsb = bearings[glyph - عدد_long_metrics];
    }

    غیرعلامتی صحیح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
    {
      اگر (glyph < عدد_bearings)
	واپس جدول->longMetricZ[hb_min (glyph, (uint32_t) عدد_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      اگر (unlikely (!عدد_advances))
	واپس default_advance;

      واپس 0;
    }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    غیرعلامتی get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
    {
      غیرعلامتی صحیح advance = get_advance_without_var_unscaled (glyph);
      واپس hb_clamp_مقصد<غیرعلامتی> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								      font->coords, font->عدد_coords,
								      store_cache)));
    }
#اگر_ختم

    محفوظ:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    غیرعلامتی عدد_long_metrics;
    غیرعلامتی عدد_bearings;
    غیرعلامتی عدد_advances;
    غیرعلامتی عدد_glyphs;

    غیرعلامتی صحیح default_advance;

    عوامی:
    hb_blob_ptr_t<hmtxvmtx> جدول;
    hb_blob_ptr_t<V> var_جدول;

    نجی:
    ساکن غیرعلامتی get_عدد_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_جدول<H> (face);
      غیرعلامتی ret = hea->نمبر_کا_Long_Metrics;
      hea.destroy ();
      واپس ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  غیرعلامتی get_new_gid_advance_unscaled (ثابت hb_subset_plan_t *plan,
                                         ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<غیرعلامتی, صحیح>> *mtx_map,
                                         غیرعلامتی new_gid,
                                         ثابت accelerator_t &_mtx) ثابت
  {
    اگر (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      واپس plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    واپس mtx_map->get (new_gid).پہلا;
  }

  محفوظ:
  Unsized_Array_کا<LongMetric>
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
  عوامی:
  DEFINE_سائز_ARRAY (0, longMetricZ);
};

ساخت hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_hmtx;
  ساکن constexpr hb_لیبل_t variations_لیبل = HB_OT_لیبل_HVAR;
  ساکن constexpr منطقی is_horizontal = سچ;
};
ساخت HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_HMTX;
  ساکن constexpr hb_لیبل_t variations_لیبل = HB_OT_لیبل_HVAR;
  ساکن constexpr منطقی is_horizontal = سچ;
};
ساخت vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_vmtx;
  ساکن constexpr hb_لیبل_t variations_لیبل = HB_OT_لیبل_VVAR;
  ساکن constexpr منطقی is_horizontal = جھوٹ;
};
ساخت VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_VMTX;
  ساکن constexpr hb_لیبل_t variations_لیبل = HB_OT_لیبل_VVAR;
  ساکن constexpr منطقی is_horizontal = جھوٹ;
};

ساخت hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  خالی get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 صحیح *lsb) ثابت
  {
    HB_DUAL_GET (*یہ, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  غیرعلامتی صحیح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    واپس HB_DUAL_GET (*یہ, get_advance_without_var_unscaled (glyph));
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  غیرعلامتی get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
    غیرعلامتی صحیح advance = get_advance_without_var_unscaled (glyph);
    واپس hb_clamp_مقصد<غیرعلامتی> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->عدد_coords,
								    store_cache)));
  }
#اگر_ختم

  hb_blob_ptr_t<HVAR> var_جدول;
};

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
ساخت vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  خالی get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 صحیح *lsb) ثابت
  {
    HB_DUAL_GET (*یہ, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  غیرعلامتی صحیح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    واپس HB_DUAL_GET (*یہ, get_advance_without_var_unscaled (glyph));
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  غیرعلامتی get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
    غیرعلامتی صحیح advance = get_advance_without_var_unscaled (glyph);
    واپس hb_clamp_مقصد<غیرعلامتی> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->عدد_coords,
								    store_cache)));
  }
#اگر_ختم

  hb_blob_ptr_t<VVAR> var_جدول;
};
#ورنہ
ساخت vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#اگر_ختم

} /* namespace OT */


#اگر_ختم /* HB_OT_HMTX_TABLE_HH */

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

#اگر_تعریف_نشده HB_OT_HMTX_جدول_HH
#تعریف_کن HB_OT_HMTX_جدول_HH

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
#تعریف_کن HB_OT_برچسب_hmtx HB_برچسب('h','m','t','x')
#تعریف_کن HB_OT_برچسب_HMTX HB_برچسب('H','M','T','X')
#تعریف_کن HB_OT_برچسب_vmtx HB_برچسب('v','m','t','x')
#تعریف_کن HB_OT_برچسب_VMTX HB_برچسب('V','M','T','X')


نطاق OT {


بنية LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  عام:
  DEFINE_اندازه_STATIC (4);
};


قالب <نام_نوع T/*Data table type*/, نام_نوع H/*Header table type*/, نام_نوع V/*Var table type*/>
بنية hmtxvmtx
{
  منطقي sanitize (hb_sanitize_context_t *c HB_استفادهنشده) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (درست);
  }

  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بیعلامت, صحيح>>* get_mtx_map (ثابت hb_subset_plan_t *plan) ثابت
  { أرجع T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  منطقي subset_update_سرآیند (hb_subset_context_t *c,
			     بیعلامت صحيح رقم_hmetrics,
			     ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بیعلامت, صحيح>> *mtx_map,
			     ثابت hb_وکتور_t<بیعلامت> &bounds_vec) ثابت
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_جدول<H> (c->plan->منبع, H::جدول_برچسب);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    إذا (unlikely (!dest_blob)) {
      أرجع خطأ;
    }

    بیعلامت صحيح درازی;
    H *جدول = (H *) hb_blob_get_داده (dest_blob, &درازی);
    c->serializer->check_تخصیص_بده (جدول->عدد_از_Long_Metrics, رقم_hmetrics, HB_SERIALIZE_ERROR_INT_سرریز);

#اگر_تعریف_نشده HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->منبع->جدول.MVAR;
      إذا (T::is_horizontal)
      {
	HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_HORIZONTAL_CARET_RISE,   caret_شیب_Rise);
	HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_HORIZONTAL_CARET_پیاپی,    caret_شیب_پیاپی);
	HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_HORIZONTAL_CARET_جابه_جایی, caret_جابه_جایی);
      }
      وإلا
      {
	HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_VERTICAL_CARET_RISE,     caret_شیب_Rise);
	HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_VERTICAL_CARET_پیاپی,      caret_شیب_پیاپی);
	HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_VERTICAL_CARET_جابه_جایی,   caret_جابه_جایی);
      }

      منطقي خالی_2 = درست;
      صحيح min_lsb = 0x7FFF;
      صحيح min_rsb = 0x7FFF;
      صحيح max_extent = -0x7FFF;
      بیعلامت max_adv = 0;
      لكل (ثابت خودکار _ : *mtx_map)
      {
        hb_codepoint_t gid = _.اولی;
        بیعلامت adv = _.second.اولی;
        صحيح lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        إذا (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  خالی_2 = خطأ;
          بیعلامت bound_عرض = bounds_vec[gid];
          صحيح rsb = adv - lsb - bound_عرض;
          صحيح extent = lsb + bound_عرض;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      جدول->advanceMax = max_adv;
      إذا (!خالی_2)
      {
        جدول->minLeadingBearing = min_lsb;
        جدول->minTrailingBearing = min_rsb;
        جدول->maxExtent = max_extent;
      }

      إذا (T::is_horizontal)
      {
        ثابت خودکار &OS2 = *c->plan->منبع->جدول.OS2;
        إذا (OS2.has_داده () &&
            جدول->ascender == OS2.sTypoAscender &&
            جدول->descender == OS2.sTypoDescender &&
            جدول->سطر_Gap == OS2.s_Typo_سطر_Gap)
        {
          جدول->ascender = static_cast<صحيح> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_برچسب_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.درازی)));
          جدول->descender = static_cast<صحيح> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_برچسب_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.درازی)));
          جدول->سطر_Gap = static_cast<صحيح> (roundf (OS2.s_Typo_سطر_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_برچسب_HORIZONTAL_سطر_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.درازی)));
        }
      }
    }
#پایان_اگر

    منطقي نتیجه = c->plan->جمع_کن_جدول (H::جدول_برچسب, dest_blob);
    hb_blob_destroy (dest_blob);

    أرجع نتیجه;
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_مقصد_old_gid_list,
		  بیعلامت رقم_long_metrics,
                  بیعلامت مجموع_رقم_metrics)
  {
    LongMetric* long_metrics = c->allocate_اندازه<LongMetric> (رقم_long_metrics * LongMetric::static_اندازه);
    FWORD* short_metrics = c->allocate_اندازه<FWORD> ((مجموع_رقم_metrics - رقم_long_metrics) * FWORD::static_اندازه);
    إذا (!long_metrics || !short_metrics) أرجع;

    short_metrics -= رقم_long_metrics;

    لكل (خودکار _ : new_مقصد_old_gid_list)
    {
      hb_codepoint_t gid = _.اولی;
      خودکار mtx = *it++;

      إذا (gid < رقم_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.اولی;
	lm.sb = mtx.second;
      }
      وإلا
        short_metrics[gid] = mtx.second;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار *جدول_اول = c->serializer->شروع_embed <T> ();

    accelerator_t _mtx (c->plan->منبع);
    بیعلامت رقم_long_metrics;
    ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بیعلامت, صحيح>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      خودکار& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      رقم_long_metrics = hb_min (plan->رقم_output_glyphs (), H::long_metric_شمار_max ());
      بیعلامت صحيح آخرین_advance = get_new_gid_advance_unscaled (plan, mtx_map, رقم_long_metrics - 1, _mtx);
      طالما (رقم_long_metrics > 1 &&
	     آخرین_advance == get_new_gid_advance_unscaled (plan, mtx_map, رقم_long_metrics - 2, _mtx))
      {
	رقم_long_metrics--;
      }
    }

    خودکار it =
    + hb_iter (c->plan->new_مقصد_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.اولی;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<بیعلامت, صحيح> *v = اشاره_تهی;
		إذا (!mtx_map->has (new_gid, &v))
		{
		  صحيح lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  أرجع hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		أرجع *v;
	      })
    ;

    جدول_اول->serialize (c->serializer,
			    it,
			    c->plan->new_مقصد_old_gid_list,
			    رقم_long_metrics,
			    c->plan->رقم_output_glyphs ());

    إذا (unlikely (c->serializer->درون_error ()))
      return_trace (خطأ);

    // Amend header num hmetrics
    إذا (unlikely (!subset_update_سرآیند (c, رقم_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_عرض_vec : c->plan->bounds_ارتفاع_vec)))
      return_trace (خطأ);

    return_trace (درست);
  }

  بنية accelerator_t
  {
    دوست بنية hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::جدول_برچسب, get_رقم_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_برچسب_t جدول_برچسب, بیعلامت رقم_long_metrics_2)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<hmtxvmtx> (face, جدول_برچسب);
      var_جدول = hb_sanitize_context_t ().reference_جدول<V> (face, T::variations_برچسب);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      بیعلامت صحيح len = جدول.get_درازی ();
      إذا (len & 1)
        len--;

      رقم_long_metrics = رقم_long_metrics_2;
      إذا (unlikely (رقم_long_metrics > len / 4))
	رقم_long_metrics = len / 4;
      len -= رقم_long_metrics * 4;

      رقم_bearings = face->جدول.maxp->get_رقم_glyphs ();

      إذا (unlikely (رقم_bearings < رقم_long_metrics))
        رقم_bearings = رقم_long_metrics;
      إذا (unlikely ((رقم_bearings - رقم_long_metrics) * 2 > len))
        رقم_bearings = رقم_long_metrics + len / 2;
      len -= (رقم_bearings - رقم_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      إذا (unlikely (!رقم_long_metrics))
	رقم_bearings = رقم_long_metrics = 0;

      رقم_advances = رقم_bearings + len / 2;
      رقم_glyphs = face->get_رقم_glyphs ();
      إذا (رقم_glyphs < رقم_advances)
        رقم_glyphs = رقم_advances;
    }
    ~accelerator_t ()
    {
      جدول.destroy ();
      var_جدول.destroy ();
    }

    منطقي has_داده () ثابت { أرجع (منطقي) رقم_bearings; }

    فراغ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   صحيح *lsb) ثابت
    {
      إذا (glyph < رقم_long_metrics)
      {
	*lsb = جدول->longMetricZ[glyph].sb;
	أرجع;
      }

      إذا (unlikely (glyph >= رقم_bearings))
      {
        *lsb = 0;
	أرجع;
      }

      ثابت FWORD *bearings = (ثابت FWORD *) &جدول->longMetricZ[رقم_long_metrics];
      *lsb = bearings[glyph - رقم_long_metrics];
    }

    بیعلامت صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
    {
      إذا (glyph < رقم_bearings)
	أرجع جدول->longMetricZ[hb_min (glyph, (uint32_t) رقم_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      إذا (unlikely (!رقم_advances))
	أرجع default_advance;

      أرجع 0;
    }

#اگر_تعریف_نشده HB_NO_VAR
    بیعلامت get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
    {
      بیعلامت صحيح advance = get_advance_without_var_unscaled (glyph);
      أرجع hb_clamp_مقصد<بیعلامت> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								      font->coords, font->رقم_coords,
								      store_cache)));
    }
#پایان_اگر

    محمي:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    بیعلامت رقم_long_metrics;
    بیعلامت رقم_bearings;
    بیعلامت رقم_advances;
    بیعلامت رقم_glyphs;

    بیعلامت صحيح default_advance;

    عام:
    hb_blob_ptr_t<hmtxvmtx> جدول;
    hb_blob_ptr_t<V> var_جدول;

    خاص:
    ساكن بیعلامت get_رقم_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_جدول<H> (face);
      بیعلامت ret = hea->عدد_از_Long_Metrics;
      hea.destroy ();
      أرجع ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  بیعلامت get_new_gid_advance_unscaled (ثابت hb_subset_plan_t *plan,
                                         ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بیعلامت, صحيح>> *mtx_map,
                                         بیعلامت new_gid,
                                         ثابت accelerator_t &_mtx) ثابت
  {
    إذا (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      أرجع plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    أرجع mtx_map->get (new_gid).اولی;
  }

  محمي:
  Unsized_Array_از<LongMetric>
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
  DEFINE_اندازه_ARRAY (0, longMetricZ);
};

بنية hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_hmtx;
  ساكن constexpr hb_برچسب_t variations_برچسب = HB_OT_برچسب_HVAR;
  ساكن constexpr منطقي is_horizontal = درست;
};
بنية HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_HMTX;
  ساكن constexpr hb_برچسب_t variations_برچسب = HB_OT_برچسب_HVAR;
  ساكن constexpr منطقي is_horizontal = درست;
};
بنية vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_vmtx;
  ساكن constexpr hb_برچسب_t variations_برچسب = HB_OT_برچسب_VVAR;
  ساكن constexpr منطقي is_horizontal = خطأ;
};
بنية VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_VMTX;
  ساكن constexpr hb_برچسب_t variations_برچسب = HB_OT_برچسب_VVAR;
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

  بیعلامت صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#اگر_تعریف_نشده HB_NO_VAR
  بیعلامت get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
    بیعلامت صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_مقصد<بیعلامت> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->رقم_coords,
								    store_cache)));
  }
#پایان_اگر

  hb_blob_ptr_t<HVAR> var_جدول;
};

#اگر_تعریف_نشده HB_NO_VERTICAL
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

  بیعلامت صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#اگر_تعریف_نشده HB_NO_VAR
  بیعلامت get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
    بیعلامت صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_مقصد<بیعلامت> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->رقم_coords,
								    store_cache)));
  }
#پایان_اگر

  hb_blob_ptr_t<VVAR> var_جدول;
};
#وإلا
بنية vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#پایان_اگر

} /* namespace OT */


#پایان_اگر /* HB_OT_HMTX_TABLE_HH */

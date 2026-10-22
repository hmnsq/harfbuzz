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

#که_نه_تعریف_شوی HB_OT_HMTX_جدول_HH
#تعریف_کړه HB_OT_HMTX_جدول_HH

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
#تعریف_کړه HB_OT_نښان_hmtx HB_نښان('h','m','t','x')
#تعریف_کړه HB_OT_نښان_HMTX HB_نښان('H','M','T','X')
#تعریف_کړه HB_OT_نښان_vmtx HB_نښان('v','m','t','x')
#تعریف_کړه HB_OT_نښان_VMTX HB_نښان('V','M','T','X')


نطاق OT {


بنية LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  عام:
  DEFINE_اندازه_STATIC (4);
};


کينډۍ <ډول_نوم T/*Data table type*/, ډول_نوم H/*Header table type*/, ډول_نوم V/*Var table type*/>
بنية hmtxvmtx
{
  منطقي sanitize (hb_sanitize_context_t *c HB_ناکارول_شوی) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (ریښتیا);
  }

  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بینښه, صحيح>>* get_mtx_map (ثابت hb_subset_plan_t *plan) ثابت
  { أرجع T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  منطقي subset_update_سرلیک (hb_subset_context_t *c,
			     بینښه صحيح عدد_hmetrics,
			     ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بینښه, صحيح>> *mtx_map,
			     ثابت hb_ویکټور_t<بینښه> &bounds_vec) ثابت
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_جدول<H> (c->plan->سرچینه, H::جدول_نښان);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    إذا (unlikely (!dest_blob)) {
      أرجع خطأ;
    }

    بینښه صحيح اوږدتیا;
    H *جدول = (H *) hb_blob_get_معلومات (dest_blob, &اوږدتیا);
    c->serializer->check_ټاکل (جدول->شمېره_د_Long_Metrics, عدد_hmetrics, HB_SERIALIZE_ERROR_INT_ډکېدنه);

#که_نه_تعریف_شوی HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خپلکاری &MVAR = *c->plan->سرچینه->جدول.MVAR;
      إذا (T::is_horizontal)
      {
	HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_HORIZONTAL_CARET_RISE,   caret_میلان_Rise);
	HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_HORIZONTAL_CARET_پرلپسې,    caret_میلان_پرلپسې);
	HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_HORIZONTAL_CARET_دننه_واټن, caret_دننه_واټن);
      }
      وإلا
      {
	HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_VERTICAL_CARET_RISE,     caret_میلان_Rise);
	HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_VERTICAL_CARET_پرلپسې,      caret_میلان_پرلپسې);
	HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_VERTICAL_CARET_دننه_واټن,   caret_دننه_واټن);
      }

      منطقي تشه_2 = ریښتیا;
      صحيح min_lsb = 0x7FFF;
      صحيح min_rsb = 0x7FFF;
      صحيح max_extent = -0x7FFF;
      بینښه max_adv = 0;
      لكل (ثابت خپلکاری _ : *mtx_map)
      {
        hb_codepoint_t gid = _.لومړی;
        بینښه adv = _.second.لومړی;
        صحيح lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        إذا (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  تشه_2 = خطأ;
          بینښه bound_پلنوالی = bounds_vec[gid];
          صحيح rsb = adv - lsb - bound_پلنوالی;
          صحيح extent = lsb + bound_پلنوالی;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      جدول->advanceMax = max_adv;
      إذا (!تشه_2)
      {
        جدول->minLeadingBearing = min_lsb;
        جدول->minTrailingBearing = min_rsb;
        جدول->maxExtent = max_extent;
      }

      إذا (T::is_horizontal)
      {
        ثابت خپلکاری &OS2 = *c->plan->سرچینه->جدول.OS2;
        إذا (OS2.has_معلومات () &&
            جدول->ascender == OS2.sTypoAscender &&
            جدول->descender == OS2.sTypoDescender &&
            جدول->کرښه_Gap == OS2.s_Typo_کرښه_Gap)
        {
          جدول->ascender = static_cast<صحيح> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_نښان_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.اوږدتیا)));
          جدول->descender = static_cast<صحيح> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_نښان_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.اوږدتیا)));
          جدول->کرښه_Gap = static_cast<صحيح> (roundf (OS2.s_Typo_کرښه_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_نښان_HORIZONTAL_کرښه_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.اوږدتیا)));
        }
      }
    }
#د_که_پای

    منطقي پایله = c->plan->جمع_کړه_جدول (H::جدول_نښان, dest_blob);
    hb_blob_destroy (dest_blob);

    أرجع پایله;
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بینښه عدد_long_metrics,
                  بینښه ټول_عدد_metrics)
  {
    LongMetric* long_metrics = c->allocate_اندازه<LongMetric> (عدد_long_metrics * LongMetric::static_اندازه);
    FWORD* short_metrics = c->allocate_اندازه<FWORD> ((ټول_عدد_metrics - عدد_long_metrics) * FWORD::static_اندازه);
    إذا (!long_metrics || !short_metrics) أرجع;

    short_metrics -= عدد_long_metrics;

    لكل (خپلکاری _ : new_منزل_old_gid_list)
    {
      hb_codepoint_t gid = _.لومړی;
      خپلکاری mtx = *it++;

      إذا (gid < عدد_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.لومړی;
	lm.sb = mtx.second;
      }
      وإلا
        short_metrics[gid] = mtx.second;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خپلکاری *جدول_اولي = c->serializer->پیل_embed <T> ();

    accelerator_t _mtx (c->plan->سرچینه);
    بینښه عدد_long_metrics;
    ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بینښه, صحيح>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      خپلکاری& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      عدد_long_metrics = hb_min (plan->عدد_output_glyphs (), H::long_metric_شمېر_max ());
      بینښه صحيح وروستی_advance = get_new_gid_advance_unscaled (plan, mtx_map, عدد_long_metrics - 1, _mtx);
      طالما (عدد_long_metrics > 1 &&
	     وروستی_advance == get_new_gid_advance_unscaled (plan, mtx_map, عدد_long_metrics - 2, _mtx))
      {
	عدد_long_metrics--;
      }
    }

    خپلکاری it =
    + hb_iter (c->plan->new_منزل_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.لومړی;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<بینښه, صحيح> *v = تش_اشاره;
		إذا (!mtx_map->has (new_gid, &v))
		{
		  صحيح lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  أرجع hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		أرجع *v;
	      })
    ;

    جدول_اولي->serialize (c->serializer,
			    it,
			    c->plan->new_منزل_old_gid_list,
			    عدد_long_metrics,
			    c->plan->عدد_output_glyphs ());

    إذا (unlikely (c->serializer->دننه_error ()))
      return_trace (خطأ);

    // Amend header num hmetrics
    إذا (unlikely (!subset_update_سرلیک (c, عدد_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_پلنوالی_vec : c->plan->bounds_لوړوالی_vec)))
      return_trace (خطأ);

    return_trace (ریښتیا);
  }

  بنية accelerator_t
  {
    ملګری بنية hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::جدول_نښان, get_عدد_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_نښان_t جدول_نښان, بینښه عدد_long_metrics_2)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<hmtxvmtx> (face, جدول_نښان);
      var_جدول = hb_sanitize_context_t ().reference_جدول<V> (face, T::variations_نښان);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      بینښه صحيح len = جدول.get_اوږدتیا ();
      إذا (len & 1)
        len--;

      عدد_long_metrics = عدد_long_metrics_2;
      إذا (unlikely (عدد_long_metrics > len / 4))
	عدد_long_metrics = len / 4;
      len -= عدد_long_metrics * 4;

      عدد_bearings = face->جدول.maxp->get_عدد_glyphs ();

      إذا (unlikely (عدد_bearings < عدد_long_metrics))
        عدد_bearings = عدد_long_metrics;
      إذا (unlikely ((عدد_bearings - عدد_long_metrics) * 2 > len))
        عدد_bearings = عدد_long_metrics + len / 2;
      len -= (عدد_bearings - عدد_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      إذا (unlikely (!عدد_long_metrics))
	عدد_bearings = عدد_long_metrics = 0;

      عدد_advances = عدد_bearings + len / 2;
      عدد_glyphs = face->get_عدد_glyphs ();
      إذا (عدد_glyphs < عدد_advances)
        عدد_glyphs = عدد_advances;
    }
    ~accelerator_t ()
    {
      جدول.destroy ();
      var_جدول.destroy ();
    }

    منطقي has_معلومات () ثابت { أرجع (منطقي) عدد_bearings; }

    فراغ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   صحيح *lsb) ثابت
    {
      إذا (glyph < عدد_long_metrics)
      {
	*lsb = جدول->longMetricZ[glyph].sb;
	أرجع;
      }

      إذا (unlikely (glyph >= عدد_bearings))
      {
        *lsb = 0;
	أرجع;
      }

      ثابت FWORD *bearings = (ثابت FWORD *) &جدول->longMetricZ[عدد_long_metrics];
      *lsb = bearings[glyph - عدد_long_metrics];
    }

    بینښه صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
    {
      إذا (glyph < عدد_bearings)
	أرجع جدول->longMetricZ[hb_min (glyph, (uint32_t) عدد_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      إذا (unlikely (!عدد_advances))
	أرجع default_advance;

      أرجع 0;
    }

#که_نه_تعریف_شوی HB_NO_VAR
    بینښه get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
    {
      بینښه صحيح advance = get_advance_without_var_unscaled (glyph);
      أرجع hb_clamp_منزل<بینښه> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								      font->coords, font->عدد_coords,
								      store_cache)));
    }
#د_که_پای

    محمي:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    بینښه عدد_long_metrics;
    بینښه عدد_bearings;
    بینښه عدد_advances;
    بینښه عدد_glyphs;

    بینښه صحيح default_advance;

    عام:
    hb_blob_ptr_t<hmtxvmtx> جدول;
    hb_blob_ptr_t<V> var_جدول;

    خاص:
    ساكن بینښه get_عدد_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_جدول<H> (face);
      بینښه ret = hea->شمېره_د_Long_Metrics;
      hea.destroy ();
      أرجع ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  بینښه get_new_gid_advance_unscaled (ثابت hb_subset_plan_t *plan,
                                         ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بینښه, صحيح>> *mtx_map,
                                         بینښه new_gid,
                                         ثابت accelerator_t &_mtx) ثابت
  {
    إذا (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      أرجع plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    أرجع mtx_map->get (new_gid).لومړی;
  }

  محمي:
  Unsized_Array_د<LongMetric>
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
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_hmtx;
  ساكن constexpr hb_نښان_t variations_نښان = HB_OT_نښان_HVAR;
  ساكن constexpr منطقي is_horizontal = ریښتیا;
};
بنية HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_HMTX;
  ساكن constexpr hb_نښان_t variations_نښان = HB_OT_نښان_HVAR;
  ساكن constexpr منطقي is_horizontal = ریښتیا;
};
بنية vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_vmtx;
  ساكن constexpr hb_نښان_t variations_نښان = HB_OT_نښان_VVAR;
  ساكن constexpr منطقي is_horizontal = خطأ;
};
بنية VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_VMTX;
  ساكن constexpr hb_نښان_t variations_نښان = HB_OT_نښان_VVAR;
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

  بینښه صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#که_نه_تعریف_شوی HB_NO_VAR
  بینښه get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
    بینښه صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_منزل<بینښه> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->عدد_coords,
								    store_cache)));
  }
#د_که_پای

  hb_blob_ptr_t<HVAR> var_جدول;
};

#که_نه_تعریف_شوی HB_NO_VERTICAL
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

  بینښه صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#که_نه_تعریف_شوی HB_NO_VAR
  بینښه get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
    بینښه صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_منزل<بینښه> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->عدد_coords,
								    store_cache)));
  }
#د_که_پای

  hb_blob_ptr_t<VVAR> var_جدول;
};
#وإلا
بنية vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#د_که_پای

} /* namespace OT */


#د_که_پای /* HB_OT_HMTX_TABLE_HH */

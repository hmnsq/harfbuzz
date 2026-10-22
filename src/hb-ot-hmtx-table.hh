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

#جے_بنا_تعریف HB_OT_HMTX_جدول_HH
#تعریف_کرو HB_OT_HMTX_جدول_HH

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
#تعریف_کرو HB_OT_ٹیگ_hmtx HB_ٹیگ('h','m','t','x')
#تعریف_کرو HB_OT_ٹیگ_HMTX HB_ٹیگ('H','M','T','X')
#تعریف_کرو HB_OT_ٹیگ_vmtx HB_ٹیگ('v','m','t','x')
#تعریف_کرو HB_OT_ٹیگ_VMTX HB_ٹیگ('V','M','T','X')


نطاق OT {


بنية LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  عام:
  DEFINE_ناپ_STATIC (4);
};


سانچا <قسم_ناں T/*Data table type*/, قسم_ناں H/*Header table type*/, قسم_ناں V/*Var table type*/>
بنية hmtxvmtx
{
  منطقي sanitize (hb_sanitize_context_t *c HB_بن_ورتیا) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (سچ);
  }

  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشانی, صحيح>>* get_mtx_map (ثابت hb_subset_plan_t *plan) ثابت
  { أرجع T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  منطقي subset_update_سرخی (hb_subset_context_t *c,
			     بنانشانی صحيح عدد_hmetrics,
			     ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشانی, صحيح>> *mtx_map,
			     ثابت hb_سمتیہ_t<بنانشانی> &bounds_vec) ثابت
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_جدول<H> (c->plan->سروت, H::جدول_ٹیگ);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    إذا (unlikely (!dest_blob)) {
      أرجع خطأ;
    }

    بنانشانی صحيح لمائی;
    H *جدول = (H *) hb_blob_get_انکڑے (dest_blob, &لمائی);
    c->serializer->check_سونپو (جدول->نمبر_دا_Long_Metrics, عدد_hmetrics, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);

#جے_بنا_تعریف HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->سروت->جدول.MVAR;
      إذا (T::is_horizontal)
      {
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_HORIZONTAL_CARET_RISE,   caret_ڈھلوان_Rise);
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_HORIZONTAL_CARET_لگاتار,    caret_ڈھلوان_لگاتار);
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_HORIZONTAL_CARET_وتھ, caret_وتھ);
      }
      وإلا
      {
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_VERTICAL_CARET_RISE,     caret_ڈھلوان_Rise);
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_VERTICAL_CARET_لگاتار,      caret_ڈھلوان_لگاتار);
	HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_VERTICAL_CARET_وتھ,   caret_وتھ);
      }

      منطقي خالی_تھاواں_2 = سچ;
      صحيح min_lsb = 0x7FFF;
      صحيح min_rsb = 0x7FFF;
      صحيح max_extent = -0x7FFF;
      بنانشانی max_adv = 0;
      لكل (ثابت خودکار _ : *mtx_map)
      {
        hb_codepoint_t gid = _.پہلا;
        بنانشانی adv = _.second.پہلا;
        صحيح lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        إذا (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  خالی_تھاواں_2 = خطأ;
          بنانشانی bound_چوڑائی = bounds_vec[gid];
          صحيح rsb = adv - lsb - bound_چوڑائی;
          صحيح extent = lsb + bound_چوڑائی;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      جدول->advanceMax = max_adv;
      إذا (!خالی_تھاواں_2)
      {
        جدول->minLeadingBearing = min_lsb;
        جدول->minTrailingBearing = min_rsb;
        جدول->maxExtent = max_extent;
      }

      إذا (T::is_horizontal)
      {
        ثابت خودکار &OS2 = *c->plan->سروت->جدول.OS2;
        إذا (OS2.has_انکڑے () &&
            جدول->ascender == OS2.sTypoAscender &&
            جدول->descender == OS2.sTypoDescender &&
            جدول->سطر_Gap == OS2.s_Typo_سطر_Gap)
        {
          جدول->ascender = static_cast<صحيح> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_ٹیگ_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.لمائی)));
          جدول->descender = static_cast<صحيح> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_ٹیگ_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.لمائی)));
          جدول->سطر_Gap = static_cast<صحيح> (roundf (OS2.s_Typo_سطر_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_ٹیگ_HORIZONTAL_سطر_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.لمائی)));
        }
      }
    }
#جے_انت

    منطقي نتیجہ = c->plan->جوڑو_جدول (H::جدول_ٹیگ, dest_blob);
    hb_blob_destroy (dest_blob);

    أرجع نتیجہ;
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بنانشانی عدد_long_metrics,
                  بنانشانی کل_عدد_metrics)
  {
    LongMetric* long_metrics = c->allocate_ناپ<LongMetric> (عدد_long_metrics * LongMetric::static_ناپ);
    FWORD* short_metrics = c->allocate_ناپ<FWORD> ((کل_عدد_metrics - عدد_long_metrics) * FWORD::static_ناپ);
    إذا (!long_metrics || !short_metrics) أرجع;

    short_metrics -= عدد_long_metrics;

    لكل (خودکار _ : new_منزل_old_gid_list)
    {
      hb_codepoint_t gid = _.پہلا;
      خودکار mtx = *it++;

      إذا (gid < عدد_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.پہلا;
	lm.sb = mtx.second;
      }
      وإلا
        short_metrics[gid] = mtx.second;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار *جدول_مفرد = c->serializer->شروع_embed <T> ();

    accelerator_t _mtx (c->plan->سروت);
    بنانشانی عدد_long_metrics;
    ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشانی, صحيح>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      خودکار& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      عدد_long_metrics = hb_min (plan->عدد_output_glyphs (), H::long_metric_گنتی_max ());
      بنانشانی صحيح آخری_advance = get_new_gid_advance_unscaled (plan, mtx_map, عدد_long_metrics - 1, _mtx);
      طالما (عدد_long_metrics > 1 &&
	     آخری_advance == get_new_gid_advance_unscaled (plan, mtx_map, عدد_long_metrics - 2, _mtx))
      {
	عدد_long_metrics--;
      }
    }

    خودکار it =
    + hb_iter (c->plan->new_منزل_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.پہلا;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<بنانشانی, صحيح> *v = خالی_سوچک;
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
			    عدد_long_metrics,
			    c->plan->عدد_output_glyphs ());

    إذا (unlikely (c->serializer->وچ_error ()))
      return_trace (خطأ);

    // Amend header num hmetrics
    إذا (unlikely (!subset_update_سرخی (c, عدد_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_چوڑائی_vec : c->plan->bounds_اچائی_vec)))
      return_trace (خطأ);

    return_trace (سچ);
  }

  بنية accelerator_t
  {
    یار بنية hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::جدول_ٹیگ, get_عدد_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_ٹیگ_t جدول_ٹیگ, بنانشانی عدد_long_metrics_2)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<hmtxvmtx> (face, جدول_ٹیگ);
      var_جدول = hb_sanitize_context_t ().reference_جدول<V> (face, T::variations_ٹیگ);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      بنانشانی صحيح len = جدول.get_لمائی ();
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

    منطقي has_انکڑے () ثابت { أرجع (منطقي) عدد_bearings; }

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

    بنانشانی صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
    {
      إذا (glyph < عدد_bearings)
	أرجع جدول->longMetricZ[hb_min (glyph, (uint32_t) عدد_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      إذا (unlikely (!عدد_advances))
	أرجع default_advance;

      أرجع 0;
    }

#جے_بنا_تعریف HB_NO_VAR
    بنانشانی get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
    {
      بنانشانی صحيح advance = get_advance_without_var_unscaled (glyph);
      أرجع hb_clamp_منزل<بنانشانی> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								      font->coords, font->عدد_coords,
								      store_cache)));
    }
#جے_انت

    محمي:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    بنانشانی عدد_long_metrics;
    بنانشانی عدد_bearings;
    بنانشانی عدد_advances;
    بنانشانی عدد_glyphs;

    بنانشانی صحيح default_advance;

    عام:
    hb_blob_ptr_t<hmtxvmtx> جدول;
    hb_blob_ptr_t<V> var_جدول;

    خاص:
    ساكن بنانشانی get_عدد_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_جدول<H> (face);
      بنانشانی ret = hea->نمبر_دا_Long_Metrics;
      hea.destroy ();
      أرجع ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  بنانشانی get_new_gid_advance_unscaled (ثابت hb_subset_plan_t *plan,
                                         ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشانی, صحيح>> *mtx_map,
                                         بنانشانی new_gid,
                                         ثابت accelerator_t &_mtx) ثابت
  {
    إذا (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      أرجع plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    أرجع mtx_map->get (new_gid).پہلا;
  }

  محمي:
  Unsized_Array_دا<LongMetric>
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
  DEFINE_ناپ_ARRAY (0, longMetricZ);
};

بنية hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_hmtx;
  ساكن constexpr hb_ٹیگ_t variations_ٹیگ = HB_OT_ٹیگ_HVAR;
  ساكن constexpr منطقي is_horizontal = سچ;
};
بنية HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_HMTX;
  ساكن constexpr hb_ٹیگ_t variations_ٹیگ = HB_OT_ٹیگ_HVAR;
  ساكن constexpr منطقي is_horizontal = سچ;
};
بنية vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_vmtx;
  ساكن constexpr hb_ٹیگ_t variations_ٹیگ = HB_OT_ٹیگ_VVAR;
  ساكن constexpr منطقي is_horizontal = خطأ;
};
بنية VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_VMTX;
  ساكن constexpr hb_ٹیگ_t variations_ٹیگ = HB_OT_ٹیگ_VVAR;
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

  بنانشانی صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#جے_بنا_تعریف HB_NO_VAR
  بنانشانی get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
    بنانشانی صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_منزل<بنانشانی> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->عدد_coords,
								    store_cache)));
  }
#جے_انت

  hb_blob_ptr_t<HVAR> var_جدول;
};

#جے_بنا_تعریف HB_NO_VERTICAL
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

  بنانشانی صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#جے_بنا_تعریف HB_NO_VAR
  بنانشانی get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
    بنانشانی صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_منزل<بنانشانی> (advance + roundf (var_جدول->get_advance_delta_unscaled (glyph,
								    font->coords, font->عدد_coords,
								    store_cache)));
  }
#جے_انت

  hb_blob_ptr_t<VVAR> var_جدول;
};
#وإلا
بنية vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#جے_انت

} /* namespace OT */


#جے_انت /* HB_OT_HMTX_TABLE_HH */

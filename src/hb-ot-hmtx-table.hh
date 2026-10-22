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

#నిర్వచితం_కాకపోతే HB_OT_HMTX_పట్టిక_HH
#నిర్వచించు HB_OT_HMTX_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-dual.hh"
#అన్తర్భావ "hb-ot-maxp-table.hh"
#అన్తర్భావ "hb-ot-hhea-table.hh"
#అన్తర్భావ "hb-ot-os2-table.hh"
#అన్తర్భావ "hb-ot-var-hvar-table.hh"
#అన్తర్భావ "hb-ot-var-mvar-table.hh"
#అన్తర్భావ "hb-ot-metrics.hh"

/*
 * hmtx -- Horizontal Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hmtx
 * vmtx -- Vertical Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vmtx
 */
#నిర్వచించు HB_OT_ట్యాగ్_hmtx HB_ట్యాగ్('h','m','t','x')
#నిర్వచించు HB_OT_ట్యాగ్_HMTX HB_ట్యాగ్('H','M','T','X')
#నిర్వచించు HB_OT_ట్యాగ్_vmtx HB_ట్యాగ్('v','m','t','x')
#నిర్వచించు HB_OT_ట్యాగ్_VMTX HB_ట్యాగ్('V','M','T','X')


నామాకాశ OT {


నిర్మాణం LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};


మూస <రకం_పేరు T/*Data table type*/, రకం_పేరు H/*Header table type*/, రకం_పేరు V/*Var table type*/>
నిర్మాణం hmtxvmtx
{
  తార్కిక sanitize (hb_sanitize_context_t *c HB_వాడని) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (నిజం);
  }

  స్థిరరాశి hb_hashmap_t<hb_codepoint_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>>* get_mtx_map (స్థిరరాశి hb_subset_plan_t *plan) స్థిరరాశి
  { తిరిగి T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  తార్కిక subset_update_శీర్షిక (hb_subset_context_t *c,
			     గుర్తులేని పూర్ణాంకం అంకె_hmetrics,
			     స్థిరరాశి hb_hashmap_t<hb_codepoint_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *mtx_map,
			     స్థిరరాశి hb_సదిశ_t<గుర్తులేని> &bounds_vec) స్థిరరాశి
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_పట్టిక<H> (c->plan->ఆధారం, H::పట్టిక_ట్యాగ్);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    ఒకవేళ (unlikely (!dest_blob)) {
      తిరిగి అబద్ధం;
    }

    గుర్తులేని పూర్ణాంకం నిడివి;
    H *పట్టిక = (H *) hb_blob_get_దత్తాంశం (dest_blob, &నిడివి);
    c->serializer->check_కేటాయించు (పట్టిక->సంఖ్య_యొక్క_Long_Metrics, అంకె_hmetrics, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);

#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (c->plan->normalized_coords)
    {
      స్వయంచాలక &MVAR = *c->plan->ఆధారం->పట్టిక.MVAR;
      ఒకవేళ (T::is_horizontal)
      {
	HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_RISE,   caret_వాలు_Rise);
	HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_నిరంతరం,    caret_వాలు_నిరంతరం);
	HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_స్థానభ్రంశం, caret_స్థానభ్రంశం);
      }
      లేకపోతే
      {
	HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_RISE,     caret_వాలు_Rise);
	HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_నిరంతరం,      caret_వాలు_నిరంతరం);
	HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_స్థానభ్రంశం,   caret_స్థానభ్రంశం);
      }

      తార్కిక ఖాళీలు_2 = నిజం;
      పూర్ణాంకం min_lsb = 0x7FFF;
      పూర్ణాంకం min_rsb = 0x7FFF;
      పూర్ణాంకం max_extent = -0x7FFF;
      గుర్తులేని max_adv = 0;
      కొరకు (స్థిరరాశి స్వయంచాలక _ : *mtx_map)
      {
        hb_codepoint_t gid = _.మొదటి;
        గుర్తులేని adv = _.second.మొదటి;
        పూర్ణాంకం lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        ఒకవేళ (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  ఖాళీలు_2 = అబద్ధం;
          గుర్తులేని bound_వెడల్పు = bounds_vec[gid];
          పూర్ణాంకం rsb = adv - lsb - bound_వెడల్పు;
          పూర్ణాంకం extent = lsb + bound_వెడల్పు;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      పట్టిక->advanceMax = max_adv;
      ఒకవేళ (!ఖాళీలు_2)
      {
        పట్టిక->minLeadingBearing = min_lsb;
        పట్టిక->minTrailingBearing = min_rsb;
        పట్టిక->maxExtent = max_extent;
      }

      ఒకవేళ (T::is_horizontal)
      {
        స్థిరరాశి స్వయంచాలక &OS2 = *c->plan->ఆధారం->పట్టిక.OS2;
        ఒకవేళ (OS2.has_దత్తాంశం () &&
            పట్టిక->ascender == OS2.sTypoAscender &&
            పట్టిక->descender == OS2.sTypoDescender &&
            పట్టిక->పంక్తి_Gap == OS2.s_Typo_పంక్తి_Gap)
        {
          పట్టిక->ascender = static_cast<పూర్ణాంకం> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.నిడివి)));
          పట్టిక->descender = static_cast<పూర్ణాంకం> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.నిడివి)));
          పట్టిక->పంక్తి_Gap = static_cast<పూర్ణాంకం> (roundf (OS2.s_Typo_పంక్తి_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.నిడివి)));
        }
      }
    }
#అయితే_ముగింపు

    తార్కిక ఫలితం = c->plan->కూడు_పట్టిక (H::పట్టిక_ట్యాగ్, dest_blob);
    hb_blob_destroy (dest_blob);

    తిరిగి ఫలితం;
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<స్థిరరాశి hb_codepoint_pair_t> new_గమ్యం_old_gid_list,
		  గుర్తులేని అంకె_long_metrics,
                  గుర్తులేని మొత్తం_అంకె_metrics)
  {
    LongMetric* long_metrics = c->allocate_కొలత<LongMetric> (అంకె_long_metrics * LongMetric::static_కొలత);
    FWORD* short_metrics = c->allocate_కొలత<FWORD> ((మొత్తం_అంకె_metrics - అంకె_long_metrics) * FWORD::static_కొలత);
    ఒకవేళ (!long_metrics || !short_metrics) తిరిగి;

    short_metrics -= అంకె_long_metrics;

    కొరకు (స్వయంచాలక _ : new_గమ్యం_old_gid_list)
    {
      hb_codepoint_t gid = _.మొదటి;
      స్వయంచాలక mtx = *it++;

      ఒకవేళ (gid < అంకె_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.మొదటి;
	lm.sb = mtx.second;
      }
      లేకపోతే
        short_metrics[gid] = mtx.second;
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్వయంచాలక *పట్టిక_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed <T> ();

    accelerator_t _mtx (c->plan->ఆధారం);
    గుర్తులేని అంకె_long_metrics;
    స్థిరరాశి hb_hashmap_t<hb_codepoint_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      స్వయంచాలక& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      అంకె_long_metrics = hb_min (plan->అంకె_output_glyphs (), H::long_metric_లెక్క_max ());
      గుర్తులేని పూర్ణాంకం చివరి_advance = get_new_gid_advance_unscaled (plan, mtx_map, అంకె_long_metrics - 1, _mtx);
      అయితే (అంకె_long_metrics > 1 &&
	     చివరి_advance == get_new_gid_advance_unscaled (plan, mtx_map, అంకె_long_metrics - 2, _mtx))
      {
	అంకె_long_metrics--;
      }
    }

    స్వయంచాలక it =
    + hb_iter (c->plan->new_గమ్యం_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.మొదటి;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<గుర్తులేని, పూర్ణాంకం> *v = శూన్యసూచిక;
		ఒకవేళ (!mtx_map->has (new_gid, &v))
		{
		  పూర్ణాంకం lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  తిరిగి hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		తిరిగి *v;
	      })
    ;

    పట్టిక_ప్రధానసంఖ్య->serialize (c->serializer,
			    it,
			    c->plan->new_గమ్యం_old_gid_list,
			    అంకె_long_metrics,
			    c->plan->అంకె_output_glyphs ());

    ఒకవేళ (unlikely (c->serializer->లోపల_error ()))
      return_trace (అబద్ధం);

    // Amend header num hmetrics
    ఒకవేళ (unlikely (!subset_update_శీర్షిక (c, అంకె_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_వెడల్పు_vec : c->plan->bounds_ఎత్తు_vec)))
      return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  నిర్మాణం accelerator_t
  {
    మిత్ర నిర్మాణం hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::పట్టిక_ట్యాగ్, get_అంకె_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_ట్యాగ్_t పట్టిక_ట్యాగ్, గుర్తులేని అంకె_long_metrics_2)
    {
      పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<hmtxvmtx> (face, పట్టిక_ట్యాగ్);
      var_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<V> (face, T::variations_ట్యాగ్);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      గుర్తులేని పూర్ణాంకం len = పట్టిక.get_నిడివి ();
      ఒకవేళ (len & 1)
        len--;

      అంకె_long_metrics = అంకె_long_metrics_2;
      ఒకవేళ (unlikely (అంకె_long_metrics > len / 4))
	అంకె_long_metrics = len / 4;
      len -= అంకె_long_metrics * 4;

      అంకె_bearings = face->పట్టిక.maxp->get_అంకె_glyphs ();

      ఒకవేళ (unlikely (అంకె_bearings < అంకె_long_metrics))
        అంకె_bearings = అంకె_long_metrics;
      ఒకవేళ (unlikely ((అంకె_bearings - అంకె_long_metrics) * 2 > len))
        అంకె_bearings = అంకె_long_metrics + len / 2;
      len -= (అంకె_bearings - అంకె_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      ఒకవేళ (unlikely (!అంకె_long_metrics))
	అంకె_bearings = అంకె_long_metrics = 0;

      అంకె_advances = అంకె_bearings + len / 2;
      అంకె_glyphs = face->get_అంకె_glyphs ();
      ఒకవేళ (అంకె_glyphs < అంకె_advances)
        అంకె_glyphs = అంకె_advances;
    }
    ~accelerator_t ()
    {
      పట్టిక.destroy ();
      var_పట్టిక.destroy ();
    }

    తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి (తార్కిక) అంకె_bearings; }

    శూన్య get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   పూర్ణాంకం *lsb) స్థిరరాశి
    {
      ఒకవేళ (glyph < అంకె_long_metrics)
      {
	*lsb = పట్టిక->longMetricZ[glyph].sb;
	తిరిగి;
      }

      ఒకవేళ (unlikely (glyph >= అంకె_bearings))
      {
        *lsb = 0;
	తిరిగి;
      }

      స్థిరరాశి FWORD *bearings = (స్థిరరాశి FWORD *) &పట్టిక->longMetricZ[అంకె_long_metrics];
      *lsb = bearings[glyph - అంకె_long_metrics];
    }

    గుర్తులేని పూర్ణాంకం get_advance_without_var_unscaled (hb_codepoint_t glyph) స్థిరరాశి
    {
      ఒకవేళ (glyph < అంకె_bearings)
	తిరిగి పట్టిక->longMetricZ[hb_min (glyph, (uint32_t) అంకె_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      ఒకవేళ (unlikely (!అంకె_advances))
	తిరిగి default_advance;

      తిరిగి 0;
    }

#నిర్వచితం_కాకపోతే HB_NO_VAR
    గుర్తులేని get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
    {
      గుర్తులేని పూర్ణాంకం advance = get_advance_without_var_unscaled (glyph);
      తిరిగి hb_clamp_గమ్యం<గుర్తులేని> (advance + roundf (var_పట్టిక->get_advance_delta_unscaled (glyph,
								      font->coords, font->అంకె_coords,
								      store_cache)));
    }
#అయితే_ముగింపు

    రక్షిత:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    గుర్తులేని అంకె_long_metrics;
    గుర్తులేని అంకె_bearings;
    గుర్తులేని అంకె_advances;
    గుర్తులేని అంకె_glyphs;

    గుర్తులేని పూర్ణాంకం default_advance;

    బహిరంగ:
    hb_blob_ptr_t<hmtxvmtx> పట్టిక;
    hb_blob_ptr_t<V> var_పట్టిక;

    అంతరంగ:
    స్థిర గుర్తులేని get_అంకె_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_పట్టిక<H> (face);
      గుర్తులేని ret = hea->సంఖ్య_యొక్క_Long_Metrics;
      hea.destroy ();
      తిరిగి ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  గుర్తులేని get_new_gid_advance_unscaled (స్థిరరాశి hb_subset_plan_t *plan,
                                         స్థిరరాశి hb_hashmap_t<hb_codepoint_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *mtx_map,
                                         గుర్తులేని new_gid,
                                         స్థిరరాశి accelerator_t &_mtx) స్థిరరాశి
  {
    ఒకవేళ (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      తిరిగి plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    తిరిగి mtx_map->get (new_gid).మొదటి;
  }

  రక్షిత:
  Unsized_Array_యొక్క<LongMetric>
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
  బహిరంగ:
  DEFINE_కొలత_ARRAY (0, longMetricZ);
};

నిర్మాణం hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_hmtx;
  స్థిర constexpr hb_ట్యాగ్_t variations_ట్యాగ్ = HB_OT_ట్యాగ్_HVAR;
  స్థిర constexpr తార్కిక is_horizontal = నిజం;
};
నిర్మాణం HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_HMTX;
  స్థిర constexpr hb_ట్యాగ్_t variations_ట్యాగ్ = HB_OT_ట్యాగ్_HVAR;
  స్థిర constexpr తార్కిక is_horizontal = నిజం;
};
నిర్మాణం vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_vmtx;
  స్థిర constexpr hb_ట్యాగ్_t variations_ట్యాగ్ = HB_OT_ట్యాగ్_VVAR;
  స్థిర constexpr తార్కిక is_horizontal = అబద్ధం;
};
నిర్మాణం VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_VMTX;
  స్థిర constexpr hb_ట్యాగ్_t variations_ట్యాగ్ = HB_OT_ట్యాగ్_VVAR;
  స్థిర constexpr తార్కిక is_horizontal = అబద్ధం;
};

నిర్మాణం hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
						  HMTX::accelerator_t>
{
  hmtx_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<hmtx::accelerator_t, HMTX::accelerator_t> (face)
  {
    var_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<HVAR> (face);
  }

  ~hmtx_accelerator_t ()
  {
    var_పట్టిక.destroy ();
  }

  శూన్య get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 పూర్ణాంకం *lsb) స్థిరరాశి
  {
    HB_DUAL_GET (*ఇది, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  గుర్తులేని పూర్ణాంకం get_advance_without_var_unscaled (hb_codepoint_t glyph) స్థిరరాశి
  {
    తిరిగి HB_DUAL_GET (*ఇది, get_advance_without_var_unscaled (glyph));
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  గుర్తులేని get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం advance = get_advance_without_var_unscaled (glyph);
    తిరిగి hb_clamp_గమ్యం<గుర్తులేని> (advance + roundf (var_పట్టిక->get_advance_delta_unscaled (glyph,
								    font->coords, font->అంకె_coords,
								    store_cache)));
  }
#అయితే_ముగింపు

  hb_blob_ptr_t<HVAR> var_పట్టిక;
};

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
నిర్మాణం vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
						  VMTX::accelerator_t>
{
  vmtx_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<vmtx::accelerator_t, VMTX::accelerator_t> (face)
  {
    var_పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<VVAR> (face);
  }

  ~vmtx_accelerator_t ()
  {
    var_పట్టిక.destroy ();
  }

  శూన్య get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 పూర్ణాంకం *lsb) స్థిరరాశి
  {
    HB_DUAL_GET (*ఇది, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  గుర్తులేని పూర్ణాంకం get_advance_without_var_unscaled (hb_codepoint_t glyph) స్థిరరాశి
  {
    తిరిగి HB_DUAL_GET (*ఇది, get_advance_without_var_unscaled (glyph));
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  గుర్తులేని get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం advance = get_advance_without_var_unscaled (glyph);
    తిరిగి hb_clamp_గమ్యం<గుర్తులేని> (advance + roundf (var_పట్టిక->get_advance_delta_unscaled (glyph,
								    font->coords, font->అంకె_coords,
								    store_cache)));
  }
#అయితే_ముగింపు

  hb_blob_ptr_t<VVAR> var_పట్టిక;
};
#లేకపోతే
నిర్మాణం vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#అయితే_ముగింపు

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_HMTX_TABLE_HH */

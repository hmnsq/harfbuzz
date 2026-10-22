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

#ifndef HB_OT_HMTX_TABLE_HH
#define HB_OT_HMTX_TABLE_HH

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
#define HB_OT_TAG_hmtx HB_TAG('h','m','t','x')
#define HB_OT_TAG_HMTX HB_TAG('H','M','T','X')
#define HB_OT_TAG_vmtx HB_TAG('v','m','t','x')
#define HB_OT_TAG_VMTX HB_TAG('V','M','T','X')


فضاینام OT {


ساختار LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  عمومی:
  DEFINE_SIZE_STATIC (4);
};


template <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
ساختار hmtxvmtx
{
  منطقی sanitize (hb_sanitize_context_t *c HB_UNUSED) ثابت
  {
    TRACE_SANITIZE (این);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (درست);
  }

  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحیح>>* get_mtx_map (ثابت hb_subset_plan_t *plan) ثابت
  { بازگشت T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  منطقی subset_update_header (hb_subset_context_t *c,
			     unsigned صحیح رقم_hmetrics,
			     ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحیح>> *mtx_map,
			     ثابت hb_vector_t<unsigned> &bounds_vec) ثابت
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    اگر (unlikely (!dest_blob)) {
      بازگشت نادرست;
    }

    unsigned صحیح length;
    H *table = (H *) hb_blob_get_data (dest_blob, &length);
    c->serializer->check_assign (table->عدد_Of_Long_Metrics, رقم_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#ifndef HB_NO_VAR
    اگر (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      اگر (T::is_horizontal)
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE,   caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN,    caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET, caretOffset);
      }
      وگرنه
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RISE,     caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RUN,      caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET,   caretOffset);
      }

      منطقی empty = درست;
      صحیح min_lsb = 0x7FFF;
      صحیح min_rsb = 0x7FFF;
      صحیح max_extent = -0x7FFF;
      unsigned max_adv = 0;
      برای (ثابت auto _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        unsigned adv = _.second.first;
        صحیح lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        اگر (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = نادرست;
          unsigned bound_width = bounds_vec[gid];
          صحیح rsb = adv - lsb - bound_width;
          صحیح extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      اگر (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      اگر (T::is_horizontal)
      {
        ثابت auto &OS2 = *c->plan->source->table.OS2;
        اگر (OS2.has_data () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->سطر_Gap == OS2.s_Typo_سطر_Gap)
        {
          table->ascender = static_cast<صحیح> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.length)));
          table->descender = static_cast<صحیح> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.length)));
          table->سطر_Gap = static_cast<صحیح> (roundf (OS2.s_Typo_سطر_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.length)));
        }
      }
    }
#endif

    منطقی result = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    بازگشت result;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  تهی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned رقم_long_metrics,
                  unsigned total_رقم_metrics)
  {
    LongMetric* long_metrics = c->allocate_size<LongMetric> (رقم_long_metrics * LongMetric::static_size);
    FWORD* short_metrics = c->allocate_size<FWORD> ((total_رقم_metrics - رقم_long_metrics) * FWORD::static_size);
    اگر (!long_metrics || !short_metrics) بازگشت;

    short_metrics -= رقم_long_metrics;

    برای (auto _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      auto mtx = *it++;

      اگر (gid < رقم_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      وگرنه
        short_metrics[gid] = mtx.second;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);

    auto *table_اول = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    unsigned رقم_long_metrics;
    ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحیح>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      auto& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      رقم_long_metrics = hb_min (plan->رقم_output_glyphs (), H::long_metric_شمار_max ());
      unsigned صحیح last_advance = get_new_gid_advance_unscaled (plan, mtx_map, رقم_long_metrics - 1, _mtx);
      وقتی (رقم_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, رقم_long_metrics - 2, _mtx))
      {
	رقم_long_metrics--;
      }
    }

    auto it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<unsigned, صحیح> *v = nullptr;
		اگر (!mtx_map->has (new_gid, &v))
		{
		  صحیح lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  بازگشت hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		بازگشت *v;
	      })
    ;

    table_اول->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    رقم_long_metrics,
			    c->plan->رقم_output_glyphs ());

    اگر (unlikely (c->serializer->in_error ()))
      return_trace (نادرست);

    // Amend header num hmetrics
    اگر (unlikely (!subset_update_header (c, رقم_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (نادرست);

    return_trace (درست);
  }

  ساختار accelerator_t
  {
    friend ساختار hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_رقم_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, unsigned رقم_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      unsigned صحیح len = table.get_length ();
      اگر (len & 1)
        len--;

      رقم_long_metrics = رقم_long_metrics_2;
      اگر (unlikely (رقم_long_metrics > len / 4))
	رقم_long_metrics = len / 4;
      len -= رقم_long_metrics * 4;

      رقم_bearings = face->table.maxp->get_رقم_glyphs ();

      اگر (unlikely (رقم_bearings < رقم_long_metrics))
        رقم_bearings = رقم_long_metrics;
      اگر (unlikely ((رقم_bearings - رقم_long_metrics) * 2 > len))
        رقم_bearings = رقم_long_metrics + len / 2;
      len -= (رقم_bearings - رقم_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      اگر (unlikely (!رقم_long_metrics))
	رقم_bearings = رقم_long_metrics = 0;

      رقم_advances = رقم_bearings + len / 2;
      رقم_glyphs = face->get_رقم_glyphs ();
      اگر (رقم_glyphs < رقم_advances)
        رقم_glyphs = رقم_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    منطقی has_data () ثابت { بازگشت (منطقی) رقم_bearings; }

    تهی get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   صحیح *lsb) ثابت
    {
      اگر (glyph < رقم_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	بازگشت;
      }

      اگر (unlikely (glyph >= رقم_bearings))
      {
        *lsb = 0;
	بازگشت;
      }

      ثابت FWORD *bearings = (ثابت FWORD *) &table->longMetricZ[رقم_long_metrics];
      *lsb = bearings[glyph - رقم_long_metrics];
    }

    unsigned صحیح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
    {
      اگر (glyph < رقم_bearings)
	بازگشت table->longMetricZ[hb_min (glyph, (uint32_t) رقم_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      اگر (unlikely (!رقم_advances))
	بازگشت default_advance;

      بازگشت 0;
    }

#ifndef HB_NO_VAR
    unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) ثابت
    {
      unsigned صحیح advance = get_advance_without_var_unscaled (glyph);
      بازگشت hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->رقم_coords,
								      store_cache)));
    }
#endif

    محافظتشده:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    unsigned رقم_long_metrics;
    unsigned رقم_bearings;
    unsigned رقم_advances;
    unsigned رقم_glyphs;

    unsigned صحیح default_advance;

    عمومی:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    خصوصی:
    ایستا unsigned get_رقم_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      unsigned ret = hea->عدد_Of_Long_Metrics;
      hea.destroy ();
      بازگشت ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  unsigned get_new_gid_advance_unscaled (ثابت hb_subset_plan_t *plan,
                                         ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحیح>> *mtx_map,
                                         unsigned new_gid,
                                         ثابت accelerator_t &_mtx) ثابت
  {
    اگر (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      بازگشت plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    بازگشت mtx_map->get (new_gid).first;
  }

  محافظتشده:
  UnsizedArrayOf<LongMetric>
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
  عمومی:
  DEFINE_SIZE_ARRAY (0, longMetricZ);
};

ساختار hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  ایستا constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  ایستا constexpr منطقی is_horizontal = درست;
};
ساختار HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  ایستا constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  ایستا constexpr منطقی is_horizontal = درست;
};
ساختار vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  ایستا constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  ایستا constexpr منطقی is_horizontal = نادرست;
};
ساختار VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  ایستا constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  ایستا constexpr منطقی is_horizontal = نادرست;
};

ساختار hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
						  HMTX::accelerator_t>
{
  hmtx_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<hmtx::accelerator_t, HMTX::accelerator_t> (face)
  {
    var_table = hb_sanitize_context_t ().reference_table<HVAR> (face);
  }

  ~hmtx_accelerator_t ()
  {
    var_table.destroy ();
  }

  تهی get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 صحیح *lsb) ثابت
  {
    HB_DUAL_GET (*این, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned صحیح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    بازگشت HB_DUAL_GET (*این, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    unsigned صحیح advance = get_advance_without_var_unscaled (glyph);
    بازگشت hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->رقم_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<HVAR> var_table;
};

#ifndef HB_NO_VERTICAL
ساختار vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
						  VMTX::accelerator_t>
{
  vmtx_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<vmtx::accelerator_t, VMTX::accelerator_t> (face)
  {
    var_table = hb_sanitize_context_t ().reference_table<VVAR> (face);
  }

  ~vmtx_accelerator_t ()
  {
    var_table.destroy ();
  }

  تهی get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 صحیح *lsb) ثابت
  {
    HB_DUAL_GET (*این, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned صحیح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    بازگشت HB_DUAL_GET (*این, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    unsigned صحیح advance = get_advance_without_var_unscaled (glyph);
    بازگشت hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->رقم_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<VVAR> var_table;
};
#وگرنه
ساختار vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#endif

} /* namespace OT */


#endif /* HB_OT_HMTX_TABLE_HH */

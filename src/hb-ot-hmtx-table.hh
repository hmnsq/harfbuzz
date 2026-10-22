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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-dual.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-maxp-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-hhea-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-os2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-hvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-mvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-metrics.hh"

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


ନାମାକାଶ OT {


ସଂରଚନା LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};


template <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
ସଂରଚନା hmtxvmtx
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c HB_UNUSED) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (ସତ୍ୟ);
  }

  ଅଚଳ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>* get_mtx_map (ଅଚଳ hb_subset_plan_t *plan) ଅଚଳ
  { ଫେରନ୍ତୁ T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  ତାର୍କିକ subset_update_header (hb_subset_context_t *c,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_hmetrics,
			     ଅଚଳ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *mtx_map,
			     ଅଚଳ hb_vector_t<unsigned> &bounds_vec) ଅଚଳ
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    ଯଦି (unlikely (!dest_blob)) {
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length;
    H *table = (H *) hb_blob_get_data (dest_blob, &length);
    c->serializer->check_assign (table->ସଂଖ୍ୟା_Of_Long_Metrics, ଅଙ୍କ_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#ifndef HB_NO_VAR
    ଯଦି (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      ଯଦି (T::is_horizontal)
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE,   caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN,    caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET, caretOffset);
      }
      ଅନ୍ୟଥା
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RISE,     caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RUN,      caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET,   caretOffset);
      }

      ତାର୍କିକ empty = ସତ୍ୟ;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_lsb = 0x7FFF;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_rsb = 0x7FFF;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_extent = -0x7FFF;
      unsigned max_adv = 0;
      ପାଇଁ (ଅଚଳ auto _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        unsigned adv = _.second.first;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        ଯଦି (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = ମିଥ୍ୟା;
          unsigned bound_width = bounds_vec[gid];
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା rsb = adv - lsb - bound_width;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      ଯଦି (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      ଯଦି (T::is_horizontal)
      {
        ଅଚଳ auto &OS2 = *c->plan->source->table.OS2;
        ଯଦି (OS2.has_data () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->ଧାଡ଼ି_Gap == OS2.s_Typo_ଧାଡ଼ି_Gap)
        {
          table->ascender = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.length)));
          table->descender = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.length)));
          table->ଧାଡ଼ି_Gap = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf (OS2.s_Typo_ଧାଡ଼ି_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.length)));
        }
      }
    }
#endif

    ତାର୍କିକ result = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    ଫେରନ୍ତୁ result;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ଅଚଳ hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned ଅଙ୍କ_long_metrics,
                  unsigned total_ଅଙ୍କ_metrics)
  {
    LongMetric* long_metrics = c->allocate_size<LongMetric> (ଅଙ୍କ_long_metrics * LongMetric::static_size);
    FWORD* short_metrics = c->allocate_size<FWORD> ((total_ଅଙ୍କ_metrics - ଅଙ୍କ_long_metrics) * FWORD::static_size);
    ଯଦି (!long_metrics || !short_metrics) ଫେରନ୍ତୁ;

    short_metrics -= ଅଙ୍କ_long_metrics;

    ପାଇଁ (auto _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      auto mtx = *it++;

      ଯଦି (gid < ଅଙ୍କ_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      ଅନ୍ୟଥା
        short_metrics[gid] = mtx.second;
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    auto *table_ମୌଳିକ = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    unsigned ଅଙ୍କ_long_metrics;
    ଅଚଳ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      auto& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      ଅଙ୍କ_long_metrics = hb_min (plan->ଅଙ୍କ_output_glyphs (), H::long_metric_ଗଣନା_max ());
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_advance = get_new_gid_advance_unscaled (plan, mtx_map, ଅଙ୍କ_long_metrics - 1, _mtx);
      ଯେତେବେଳେ (ଅଙ୍କ_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, ଅଙ୍କ_long_metrics - 2, _mtx))
      {
	ଅଙ୍କ_long_metrics--;
      }
    }

    auto it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *v = nullptr;
		ଯଦି (!mtx_map->has (new_gid, &v))
		{
		  ପୂର୍ଣ୍ଣସଂଖ୍ୟା lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  ଫେରନ୍ତୁ hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		ଫେରନ୍ତୁ *v;
	      })
    ;

    table_ମୌଳିକ->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    ଅଙ୍କ_long_metrics,
			    c->plan->ଅଙ୍କ_output_glyphs ());

    ଯଦି (unlikely (c->serializer->in_error ()))
      return_trace (ମିଥ୍ୟା);

    // Amend header num hmetrics
    ଯଦି (unlikely (!subset_update_header (c, ଅଙ୍କ_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  ସଂରଚନା accelerator_t
  {
    friend ସଂରଚନା hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_ଅଙ୍କ_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, unsigned ଅଙ୍କ_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = table.get_length ();
      ଯଦି (len & 1)
        len--;

      ଅଙ୍କ_long_metrics = ଅଙ୍କ_long_metrics_2;
      ଯଦି (unlikely (ଅଙ୍କ_long_metrics > len / 4))
	ଅଙ୍କ_long_metrics = len / 4;
      len -= ଅଙ୍କ_long_metrics * 4;

      ଅଙ୍କ_bearings = face->table.maxp->get_ଅଙ୍କ_glyphs ();

      ଯଦି (unlikely (ଅଙ୍କ_bearings < ଅଙ୍କ_long_metrics))
        ଅଙ୍କ_bearings = ଅଙ୍କ_long_metrics;
      ଯଦି (unlikely ((ଅଙ୍କ_bearings - ଅଙ୍କ_long_metrics) * 2 > len))
        ଅଙ୍କ_bearings = ଅଙ୍କ_long_metrics + len / 2;
      len -= (ଅଙ୍କ_bearings - ଅଙ୍କ_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      ଯଦି (unlikely (!ଅଙ୍କ_long_metrics))
	ଅଙ୍କ_bearings = ଅଙ୍କ_long_metrics = 0;

      ଅଙ୍କ_advances = ଅଙ୍କ_bearings + len / 2;
      ଅଙ୍କ_glyphs = face->get_ଅଙ୍କ_glyphs ();
      ଯଦି (ଅଙ୍କ_glyphs < ଅଙ୍କ_advances)
        ଅଙ୍କ_glyphs = ଅଙ୍କ_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ (ତାର୍କିକ) ଅଙ୍କ_bearings; }

    ଶୂନ୍ୟ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lsb) ଅଚଳ
    {
      ଯଦି (glyph < ଅଙ୍କ_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	ଫେରନ୍ତୁ;
      }

      ଯଦି (unlikely (glyph >= ଅଙ୍କ_bearings))
      {
        *lsb = 0;
	ଫେରନ୍ତୁ;
      }

      ଅଚଳ FWORD *bearings = (ଅଚଳ FWORD *) &table->longMetricZ[ଅଙ୍କ_long_metrics];
      *lsb = bearings[glyph - ଅଙ୍କ_long_metrics];
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_advance_without_var_unscaled (hb_codepoint_t glyph) ଅଚଳ
    {
      ଯଦି (glyph < ଅଙ୍କ_bearings)
	ଫେରନ୍ତୁ table->longMetricZ[hb_min (glyph, (uint32_t) ଅଙ୍କ_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      ଯଦି (unlikely (!ଅଙ୍କ_advances))
	ଫେରନ୍ତୁ default_advance;

      ଫେରନ୍ତୁ 0;
    }

#ifndef HB_NO_VAR
    unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା advance = get_advance_without_var_unscaled (glyph);
      ଫେରନ୍ତୁ hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->ଅଙ୍କ_coords,
								      store_cache)));
    }
#endif

    ସୁରକ୍ଷିତ:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    unsigned ଅଙ୍କ_long_metrics;
    unsigned ଅଙ୍କ_bearings;
    unsigned ଅଙ୍କ_advances;
    unsigned ଅଙ୍କ_glyphs;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_advance;

    ସର୍ବସାଧାରଣ:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    ବ୍ୟକ୍ତିଗତ:
    ସ୍ଥିର unsigned get_ଅଙ୍କ_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      unsigned ret = hea->ସଂଖ୍ୟା_Of_Long_Metrics;
      hea.destroy ();
      ଫେରନ୍ତୁ ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  unsigned get_new_gid_advance_unscaled (ଅଚଳ hb_subset_plan_t *plan,
                                         ଅଚଳ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *mtx_map,
                                         unsigned new_gid,
                                         ଅଚଳ accelerator_t &_mtx) ଅଚଳ
  {
    ଯଦି (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      ଫେରନ୍ତୁ plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    ଫେରନ୍ତୁ mtx_map->get (new_gid).first;
  }

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (0, longMetricZ);
};

ସଂରଚନା hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  ସ୍ଥିର constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  ସ୍ଥିର constexpr ତାର୍କିକ is_horizontal = ସତ୍ୟ;
};
ସଂରଚନା HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  ସ୍ଥିର constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  ସ୍ଥିର constexpr ତାର୍କିକ is_horizontal = ସତ୍ୟ;
};
ସଂରଚନା vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  ସ୍ଥିର constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  ସ୍ଥିର constexpr ତାର୍କିକ is_horizontal = ମିଥ୍ୟା;
};
ସଂରଚନା VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  ସ୍ଥିର constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  ସ୍ଥିର constexpr ତାର୍କିକ is_horizontal = ମିଥ୍ୟା;
};

ସଂରଚନା hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  ଶୂନ୍ୟ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lsb) ଅଚଳ
  {
    HB_DUAL_GET (*ଏହି, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_advance_without_var_unscaled (hb_codepoint_t glyph) ଅଚଳ
  {
    ଫେରନ୍ତୁ HB_DUAL_GET (*ଏହି, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା advance = get_advance_without_var_unscaled (glyph);
    ଫେରନ୍ତୁ hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->ଅଙ୍କ_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<HVAR> var_table;
};

#ifndef HB_NO_VERTICAL
ସଂରଚନା vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  ଶୂନ୍ୟ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lsb) ଅଚଳ
  {
    HB_DUAL_GET (*ଏହି, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_advance_without_var_unscaled (hb_codepoint_t glyph) ଅଚଳ
  {
    ଫେରନ୍ତୁ HB_DUAL_GET (*ଏହି, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା advance = get_advance_without_var_unscaled (glyph);
    ଫେରନ୍ତୁ hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->ଅଙ୍କ_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<VVAR> var_table;
};
#ଅନ୍ୟଥା
ସଂରଚନା vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#endif

} /* namespace OT */


#endif /* HB_OT_HMTX_TABLE_HH */

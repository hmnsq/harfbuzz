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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-dual.hh"
#அந்தர்பாவ "hb-ot-maxp-table.hh"
#அந்தர்பாவ "hb-ot-hhea-table.hh"
#அந்தர்பாவ "hb-ot-os2-table.hh"
#அந்தர்பாவ "hb-ot-var-hvar-table.hh"
#அந்தர்பாவ "hb-ot-var-mvar-table.hh"
#அந்தர்பாவ "hb-ot-metrics.hh"

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


நாமாகாஶ OT {


கட்டமைப்பு LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  பொது:
  DEFINE_SIZE_STATIC (4);
};


template <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
கட்டமைப்பு hmtxvmtx
{
  தருக்க sanitize (hb_sanitize_context_t *c HB_UNUSED) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (உண்மை);
  }

  மாறிலி hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, முழு>>* get_mtx_map (மாறிலி hb_subset_plan_t *plan) மாறிலி
  { திரும்பு T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  தருக்க subset_update_header (hb_subset_context_t *c,
			     unsigned முழு இலக்கம்_hmetrics,
			     மாறிலி hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, முழு>> *mtx_map,
			     மாறிலி hb_vector_t<unsigned> &bounds_vec) மாறிலி
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    எனில் (unlikely (!dest_blob)) {
      திரும்பு பொய்;
    }

    unsigned முழு length;
    H *table = (H *) hb_blob_get_data (dest_blob, &length);
    c->serializer->check_assign (table->எண்_Of_Long_Metrics, இலக்கம்_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#ifndef HB_NO_VAR
    எனில் (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      எனில் (T::is_horizontal)
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE,   caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN,    caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET, caretOffset);
      }
      இல்லையெனில்
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RISE,     caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RUN,      caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET,   caretOffset);
      }

      தருக்க empty = உண்மை;
      முழு min_lsb = 0x7FFF;
      முழு min_rsb = 0x7FFF;
      முழு max_extent = -0x7FFF;
      unsigned max_adv = 0;
      க்கு (மாறிலி auto _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        unsigned adv = _.second.first;
        முழு lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        எனில் (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = பொய்;
          unsigned bound_width = bounds_vec[gid];
          முழு rsb = adv - lsb - bound_width;
          முழு extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      எனில் (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      எனில் (T::is_horizontal)
      {
        மாறிலி auto &OS2 = *c->plan->source->table.OS2;
        எனில் (OS2.has_data () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->வரி_Gap == OS2.s_Typo_வரி_Gap)
        {
          table->ascender = static_cast<முழு> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.length)));
          table->descender = static_cast<முழு> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.length)));
          table->வரி_Gap = static_cast<முழு> (roundf (OS2.s_Typo_வரி_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.length)));
        }
      }
    }
#endif

    தருக்க result = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    திரும்பு result;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<மாறிலி hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned இலக்கம்_long_metrics,
                  unsigned total_இலக்கம்_metrics)
  {
    LongMetric* long_metrics = c->allocate_size<LongMetric> (இலக்கம்_long_metrics * LongMetric::static_size);
    FWORD* short_metrics = c->allocate_size<FWORD> ((total_இலக்கம்_metrics - இலக்கம்_long_metrics) * FWORD::static_size);
    எனில் (!long_metrics || !short_metrics) திரும்பு;

    short_metrics -= இலக்கம்_long_metrics;

    க்கு (auto _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      auto mtx = *it++;

      எனில் (gid < இலக்கம்_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      இல்லையெனில்
        short_metrics[gid] = mtx.second;
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);

    auto *table_பகாஎண் = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    unsigned இலக்கம்_long_metrics;
    மாறிலி hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, முழு>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      auto& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      இலக்கம்_long_metrics = hb_min (plan->இலக்கம்_output_glyphs (), H::long_metric_மொத்தம்_max ());
      unsigned முழு last_advance = get_new_gid_advance_unscaled (plan, mtx_map, இலக்கம்_long_metrics - 1, _mtx);
      போது (இலக்கம்_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, இலக்கம்_long_metrics - 2, _mtx))
      {
	இலக்கம்_long_metrics--;
      }
    }

    auto it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<unsigned, முழு> *v = nullptr;
		எனில் (!mtx_map->has (new_gid, &v))
		{
		  முழு lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  திரும்பு hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		திரும்பு *v;
	      })
    ;

    table_பகாஎண்->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    இலக்கம்_long_metrics,
			    c->plan->இலக்கம்_output_glyphs ());

    எனில் (unlikely (c->serializer->in_error ()))
      return_trace (பொய்);

    // Amend header num hmetrics
    எனில் (unlikely (!subset_update_header (c, இலக்கம்_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (பொய்);

    return_trace (உண்மை);
  }

  கட்டமைப்பு accelerator_t
  {
    friend கட்டமைப்பு hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_இலக்கம்_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, unsigned இலக்கம்_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      unsigned முழு len = table.get_length ();
      எனில் (len & 1)
        len--;

      இலக்கம்_long_metrics = இலக்கம்_long_metrics_2;
      எனில் (unlikely (இலக்கம்_long_metrics > len / 4))
	இலக்கம்_long_metrics = len / 4;
      len -= இலக்கம்_long_metrics * 4;

      இலக்கம்_bearings = face->table.maxp->get_இலக்கம்_glyphs ();

      எனில் (unlikely (இலக்கம்_bearings < இலக்கம்_long_metrics))
        இலக்கம்_bearings = இலக்கம்_long_metrics;
      எனில் (unlikely ((இலக்கம்_bearings - இலக்கம்_long_metrics) * 2 > len))
        இலக்கம்_bearings = இலக்கம்_long_metrics + len / 2;
      len -= (இலக்கம்_bearings - இலக்கம்_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      எனில் (unlikely (!இலக்கம்_long_metrics))
	இலக்கம்_bearings = இலக்கம்_long_metrics = 0;

      இலக்கம்_advances = இலக்கம்_bearings + len / 2;
      இலக்கம்_glyphs = face->get_இலக்கம்_glyphs ();
      எனில் (இலக்கம்_glyphs < இலக்கம்_advances)
        இலக்கம்_glyphs = இலக்கம்_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    தருக்க has_data () மாறிலி { திரும்பு (தருக்க) இலக்கம்_bearings; }

    வெறுமை get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   முழு *lsb) மாறிலி
    {
      எனில் (glyph < இலக்கம்_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	திரும்பு;
      }

      எனில் (unlikely (glyph >= இலக்கம்_bearings))
      {
        *lsb = 0;
	திரும்பு;
      }

      மாறிலி FWORD *bearings = (மாறிலி FWORD *) &table->longMetricZ[இலக்கம்_long_metrics];
      *lsb = bearings[glyph - இலக்கம்_long_metrics];
    }

    unsigned முழு get_advance_without_var_unscaled (hb_codepoint_t glyph) மாறிலி
    {
      எனில் (glyph < இலக்கம்_bearings)
	திரும்பு table->longMetricZ[hb_min (glyph, (uint32_t) இலக்கம்_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      எனில் (unlikely (!இலக்கம்_advances))
	திரும்பு default_advance;

      திரும்பு 0;
    }

#ifndef HB_NO_VAR
    unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) மாறிலி
    {
      unsigned முழு advance = get_advance_without_var_unscaled (glyph);
      திரும்பு hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->இலக்கம்_coords,
								      store_cache)));
    }
#endif

    பாதுகாக்கப்பட்ட:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    unsigned இலக்கம்_long_metrics;
    unsigned இலக்கம்_bearings;
    unsigned இலக்கம்_advances;
    unsigned இலக்கம்_glyphs;

    unsigned முழு default_advance;

    பொது:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    தனிப்பட்ட:
    நிலையான unsigned get_இலக்கம்_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      unsigned ret = hea->எண்_Of_Long_Metrics;
      hea.destroy ();
      திரும்பு ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  unsigned get_new_gid_advance_unscaled (மாறிலி hb_subset_plan_t *plan,
                                         மாறிலி hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, முழு>> *mtx_map,
                                         unsigned new_gid,
                                         மாறிலி accelerator_t &_mtx) மாறிலி
  {
    எனில் (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      திரும்பு plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    திரும்பு mtx_map->get (new_gid).first;
  }

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_ARRAY (0, longMetricZ);
};

கட்டமைப்பு hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  நிலையான constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  நிலையான constexpr தருக்க is_horizontal = உண்மை;
};
கட்டமைப்பு HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  நிலையான constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  நிலையான constexpr தருக்க is_horizontal = உண்மை;
};
கட்டமைப்பு vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  நிலையான constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  நிலையான constexpr தருக்க is_horizontal = பொய்;
};
கட்டமைப்பு VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  நிலையான constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  நிலையான constexpr தருக்க is_horizontal = பொய்;
};

கட்டமைப்பு hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  வெறுமை get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 முழு *lsb) மாறிலி
  {
    HB_DUAL_GET (*இது, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned முழு get_advance_without_var_unscaled (hb_codepoint_t glyph) மாறிலி
  {
    திரும்பு HB_DUAL_GET (*இது, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
    unsigned முழு advance = get_advance_without_var_unscaled (glyph);
    திரும்பு hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->இலக்கம்_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<HVAR> var_table;
};

#ifndef HB_NO_VERTICAL
கட்டமைப்பு vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  வெறுமை get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 முழு *lsb) மாறிலி
  {
    HB_DUAL_GET (*இது, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned முழு get_advance_without_var_unscaled (hb_codepoint_t glyph) மாறிலி
  {
    திரும்பு HB_DUAL_GET (*இது, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
    unsigned முழு advance = get_advance_without_var_unscaled (glyph);
    திரும்பு hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->இலக்கம்_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<VVAR> var_table;
};
#இல்லையெனில்
கட்டமைப்பு vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#endif

} /* namespace OT */


#endif /* HB_OT_HMTX_TABLE_HH */

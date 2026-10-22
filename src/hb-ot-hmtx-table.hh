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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-dual.hh"
#অন্তর্ভাব "hb-ot-maxp-table.hh"
#অন্তর্ভাব "hb-ot-hhea-table.hh"
#অন্তর্ভাব "hb-ot-os2-table.hh"
#অন্তর্ভাব "hb-ot-var-hvar-table.hh"
#অন্তর্ভাব "hb-ot-var-mvar-table.hh"
#অন্তর্ভাব "hb-ot-metrics.hh"

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


নামাকাশ OT {


কাঠামো LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};


template <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
কাঠামো hmtxvmtx
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c HB_UNUSED) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (সত্য);
  }

  ধ্রুবক hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>>* get_mtx_map (ধ্রুবক hb_subset_plan_t *plan) ধ্রুবক
  { ফেরত T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  যৌক্তিক subset_update_header (hb_subset_context_t *c,
			     unsigned পূর্ণসংখ্যা অঙ্ক_hmetrics,
			     ধ্রুবক hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *mtx_map,
			     ধ্রুবক hb_vector_t<unsigned> &bounds_vec) ধ্রুবক
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    যদি (unlikely (!dest_blob)) {
      ফেরত মিথ্যা;
    }

    unsigned পূর্ণসংখ্যা দৈর্ঘ্য;
    H *table = (H *) hb_blob_get_তথ্য (dest_blob, &দৈর্ঘ্য);
    c->serializer->check_assign (table->সংখ্যা_Of_Long_Metrics, অঙ্ক_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#ifndef HB_NO_VAR
    যদি (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      যদি (T::is_horizontal)
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE,   caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN,    caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET, caretOffset);
      }
      অন্যথা
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RISE,     caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RUN,      caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET,   caretOffset);
      }

      যৌক্তিক empty = সত্য;
      পূর্ণসংখ্যা min_lsb = 0x7FFF;
      পূর্ণসংখ্যা min_rsb = 0x7FFF;
      পূর্ণসংখ্যা max_extent = -0x7FFF;
      unsigned max_adv = 0;
      জন্য (ধ্রুবক auto _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        unsigned adv = _.second.first;
        পূর্ণসংখ্যা lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        যদি (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = মিথ্যা;
          unsigned bound_width = bounds_vec[gid];
          পূর্ণসংখ্যা rsb = adv - lsb - bound_width;
          পূর্ণসংখ্যা extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      যদি (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      যদি (T::is_horizontal)
      {
        ধ্রুবক auto &OS2 = *c->plan->source->table.OS2;
        যদি (OS2.has_তথ্য () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->সারি_Gap == OS2.s_Typo_সারি_Gap)
        {
          table->ascender = static_cast<পূর্ণসংখ্যা> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.দৈর্ঘ্য)));
          table->descender = static_cast<পূর্ণসংখ্যা> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.দৈর্ঘ্য)));
          table->সারি_Gap = static_cast<পূর্ণসংখ্যা> (roundf (OS2.s_Typo_সারি_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_সারি_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.দৈর্ঘ্য)));
        }
      }
    }
#endif

    যৌক্তিক ফলাফল = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    ফেরত ফলাফল;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ধ্রুবক hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned অঙ্ক_long_metrics,
                  unsigned যোগ_অঙ্ক_metrics)
  {
    LongMetric* long_metrics = c->allocate_আকার<LongMetric> (অঙ্ক_long_metrics * LongMetric::static_আকার);
    FWORD* short_metrics = c->allocate_আকার<FWORD> ((যোগ_অঙ্ক_metrics - অঙ্ক_long_metrics) * FWORD::static_আকার);
    যদি (!long_metrics || !short_metrics) ফেরত;

    short_metrics -= অঙ্ক_long_metrics;

    জন্য (auto _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      auto mtx = *it++;

      যদি (gid < অঙ্ক_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      অন্যথা
        short_metrics[gid] = mtx.second;
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    auto *table_মৌলিক = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    unsigned অঙ্ক_long_metrics;
    ধ্রুবক hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      auto& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      অঙ্ক_long_metrics = hb_min (plan->অঙ্ক_output_glyphs (), H::long_metric_গুনতি_max ());
      unsigned পূর্ণসংখ্যা last_advance = get_new_gid_advance_unscaled (plan, mtx_map, অঙ্ক_long_metrics - 1, _mtx);
      যখন (অঙ্ক_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, অঙ্ক_long_metrics - 2, _mtx))
      {
	অঙ্ক_long_metrics--;
      }
    }

    auto it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<unsigned, পূর্ণসংখ্যা> *v = nullptr;
		যদি (!mtx_map->has (new_gid, &v))
		{
		  পূর্ণসংখ্যা lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  ফেরত hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		ফেরত *v;
	      })
    ;

    table_মৌলিক->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    অঙ্ক_long_metrics,
			    c->plan->অঙ্ক_output_glyphs ());

    যদি (unlikely (c->serializer->in_error ()))
      return_trace (মিথ্যা);

    // Amend header num hmetrics
    যদি (unlikely (!subset_update_header (c, অঙ্ক_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  কাঠামো accelerator_t
  {
    friend কাঠামো hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_অঙ্ক_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, unsigned অঙ্ক_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      unsigned পূর্ণসংখ্যা len = table.get_দৈর্ঘ্য ();
      যদি (len & 1)
        len--;

      অঙ্ক_long_metrics = অঙ্ক_long_metrics_2;
      যদি (unlikely (অঙ্ক_long_metrics > len / 4))
	অঙ্ক_long_metrics = len / 4;
      len -= অঙ্ক_long_metrics * 4;

      অঙ্ক_bearings = face->table.maxp->get_অঙ্ক_glyphs ();

      যদি (unlikely (অঙ্ক_bearings < অঙ্ক_long_metrics))
        অঙ্ক_bearings = অঙ্ক_long_metrics;
      যদি (unlikely ((অঙ্ক_bearings - অঙ্ক_long_metrics) * 2 > len))
        অঙ্ক_bearings = অঙ্ক_long_metrics + len / 2;
      len -= (অঙ্ক_bearings - অঙ্ক_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      যদি (unlikely (!অঙ্ক_long_metrics))
	অঙ্ক_bearings = অঙ্ক_long_metrics = 0;

      অঙ্ক_advances = অঙ্ক_bearings + len / 2;
      অঙ্ক_glyphs = face->get_অঙ্ক_glyphs ();
      যদি (অঙ্ক_glyphs < অঙ্ক_advances)
        অঙ্ক_glyphs = অঙ্ক_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত (যৌক্তিক) অঙ্ক_bearings; }

    শূন্য get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   পূর্ণসংখ্যা *lsb) ধ্রুবক
    {
      যদি (glyph < অঙ্ক_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	ফেরত;
      }

      যদি (unlikely (glyph >= অঙ্ক_bearings))
      {
        *lsb = 0;
	ফেরত;
      }

      ধ্রুবক FWORD *bearings = (ধ্রুবক FWORD *) &table->longMetricZ[অঙ্ক_long_metrics];
      *lsb = bearings[glyph - অঙ্ক_long_metrics];
    }

    unsigned পূর্ণসংখ্যা get_advance_without_var_unscaled (hb_codepoint_t glyph) ধ্রুবক
    {
      যদি (glyph < অঙ্ক_bearings)
	ফেরত table->longMetricZ[hb_min (glyph, (uint32_t) অঙ্ক_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      যদি (unlikely (!অঙ্ক_advances))
	ফেরত default_advance;

      ফেরত 0;
    }

#ifndef HB_NO_VAR
    unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
    {
      unsigned পূর্ণসংখ্যা advance = get_advance_without_var_unscaled (glyph);
      ফেরত hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->অঙ্ক_coords,
								      store_cache)));
    }
#endif

    সুরক্ষিত:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    unsigned অঙ্ক_long_metrics;
    unsigned অঙ্ক_bearings;
    unsigned অঙ্ক_advances;
    unsigned অঙ্ক_glyphs;

    unsigned পূর্ণসংখ্যা default_advance;

    সর্বজনীন:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    ব্যক্তিগত:
    স্থির unsigned get_অঙ্ক_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      unsigned ret = hea->সংখ্যা_Of_Long_Metrics;
      hea.destroy ();
      ফেরত ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  unsigned get_new_gid_advance_unscaled (ধ্রুবক hb_subset_plan_t *plan,
                                         ধ্রুবক hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *mtx_map,
                                         unsigned new_gid,
                                         ধ্রুবক accelerator_t &_mtx) ধ্রুবক
  {
    যদি (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      ফেরত plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    ফেরত mtx_map->get (new_gid).first;
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_ARRAY (0, longMetricZ);
};

কাঠামো hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  স্থির constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  স্থির constexpr যৌক্তিক is_horizontal = সত্য;
};
কাঠামো HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  স্থির constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  স্থির constexpr যৌক্তিক is_horizontal = সত্য;
};
কাঠামো vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  স্থির constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  স্থির constexpr যৌক্তিক is_horizontal = মিথ্যা;
};
কাঠামো VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  স্থির constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  স্থির constexpr যৌক্তিক is_horizontal = মিথ্যা;
};

কাঠামো hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  শূন্য get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 পূর্ণসংখ্যা *lsb) ধ্রুবক
  {
    HB_DUAL_GET (*এই, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned পূর্ণসংখ্যা get_advance_without_var_unscaled (hb_codepoint_t glyph) ধ্রুবক
  {
    ফেরত HB_DUAL_GET (*এই, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা advance = get_advance_without_var_unscaled (glyph);
    ফেরত hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->অঙ্ক_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<HVAR> var_table;
};

#ifndef HB_NO_VERTICAL
কাঠামো vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  শূন্য get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 পূর্ণসংখ্যা *lsb) ধ্রুবক
  {
    HB_DUAL_GET (*এই, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned পূর্ণসংখ্যা get_advance_without_var_unscaled (hb_codepoint_t glyph) ধ্রুবক
  {
    ফেরত HB_DUAL_GET (*এই, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা advance = get_advance_without_var_unscaled (glyph);
    ফেরত hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->অঙ্ক_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<VVAR> var_table;
};
#অন্যথা
কাঠামো vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#endif

} /* namespace OT */


#endif /* HB_OT_HMTX_TABLE_HH */

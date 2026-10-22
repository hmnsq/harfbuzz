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
#define HB_OT_TAG_hmtx HB_TAG('h','m','t','x')
#define HB_OT_TAG_HMTX HB_TAG('H','M','T','X')
#define HB_OT_TAG_vmtx HB_TAG('v','m','t','x')
#define HB_OT_TAG_VMTX HB_TAG('V','M','T','X')


نطاق OT {


بنية LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  عام:
  DEFINE_SIZE_STATIC (4);
};


template <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
بنية hmtxvmtx
{
  منطقي sanitize (hb_sanitize_context_t *c HB_UNUSED) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (true);
  }

  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحيح>>* get_mtx_map (ثابت hb_subset_plan_t *plan) ثابت
  { أرجع T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  منطقي subset_update_header (hb_subset_context_t *c,
			     unsigned صحيح لمب_hmetrics,
			     ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحيح>> *mtx_map,
			     ثابت hb_vector_t<unsigned> &bounds_vec) ثابت
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    إذا (unlikely (!dest_blob)) {
      أرجع خطأ;
    }

    unsigned صحيح length;
    H *table = (H *) hb_blob_get_data (dest_blob, &length);
    c->serializer->check_assign (table->لمب_Of_Long_Metrics, لمب_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#ifndef HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      إذا (T::is_horizontal)
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE,   caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN,    caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET, caretOffset);
      }
      وإلا
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RISE,     caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RUN,      caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET,   caretOffset);
      }

      منطقي empty = true;
      صحيح min_lsb = 0x7FFF;
      صحيح min_rsb = 0x7FFF;
      صحيح max_extent = -0x7FFF;
      unsigned max_adv = 0;
      لكل (ثابت auto _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        unsigned adv = _.second.first;
        صحيح lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        إذا (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = خطأ;
          unsigned bound_width = bounds_vec[gid];
          صحيح rsb = adv - lsb - bound_width;
          صحيح extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      إذا (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      إذا (T::is_horizontal)
      {
        ثابت auto &OS2 = *c->plan->source->table.OS2;
        إذا (OS2.has_data () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->لي_Gap == OS2.s_Typo_لي_Gap)
        {
          table->ascender = static_cast<صحيح> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.length)));
          table->descender = static_cast<صحيح> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.length)));
          table->لي_Gap = static_cast<صحيح> (roundf (OS2.s_Typo_لي_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_لي_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.length)));
        }
      }
    }
#endif

    منطقي result = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    أرجع result;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned لمب_long_metrics,
                  unsigned total_لمب_metrics)
  {
    LongMetric* long_metrics = c->allocate_size<LongMetric> (لمب_long_metrics * LongMetric::static_size);
    FWORD* short_metrics = c->allocate_size<FWORD> ((total_لمب_metrics - لمب_long_metrics) * FWORD::static_size);
    إذا (!long_metrics || !short_metrics) أرجع;

    short_metrics -= لمب_long_metrics;

    لكل (auto _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      auto mtx = *it++;

      إذا (gid < لمب_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      وإلا
        short_metrics[gid] = mtx.second;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    auto *table_فرك = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    unsigned لمب_long_metrics;
    ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحيح>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      auto& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      لمب_long_metrics = hb_min (plan->لمب_output_glyphs (), H::long_metric_كدي_max ());
      unsigned صحيح last_advance = get_new_gid_advance_unscaled (plan, mtx_map, لمب_long_metrics - 1, _mtx);
      طالما (لمب_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, لمب_long_metrics - 2, _mtx))
      {
	لمب_long_metrics--;
      }
    }

    auto it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<unsigned, صحيح> *v = nullptr;
		إذا (!mtx_map->has (new_gid, &v))
		{
		  صحيح lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  أرجع hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		أرجع *v;
	      })
    ;

    table_فرك->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    لمب_long_metrics,
			    c->plan->لمب_output_glyphs ());

    إذا (unlikely (c->serializer->in_error ()))
      return_trace (خطأ);

    // Amend header num hmetrics
    إذا (unlikely (!subset_update_header (c, لمب_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (خطأ);

    return_trace (true);
  }

  بنية accelerator_t
  {
    friend بنية hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_لمب_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, unsigned لمب_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      unsigned صحيح len = table.get_length ();
      إذا (len & 1)
        len--;

      لمب_long_metrics = لمب_long_metrics_2;
      إذا (unlikely (لمب_long_metrics > len / 4))
	لمب_long_metrics = len / 4;
      len -= لمب_long_metrics * 4;

      لمب_bearings = face->table.maxp->get_لمب_glyphs ();

      إذا (unlikely (لمب_bearings < لمب_long_metrics))
        لمب_bearings = لمب_long_metrics;
      إذا (unlikely ((لمب_bearings - لمب_long_metrics) * 2 > len))
        لمب_bearings = لمب_long_metrics + len / 2;
      len -= (لمب_bearings - لمب_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      إذا (unlikely (!لمب_long_metrics))
	لمب_bearings = لمب_long_metrics = 0;

      لمب_advances = لمب_bearings + len / 2;
      لمب_glyphs = face->get_لمب_glyphs ();
      إذا (لمب_glyphs < لمب_advances)
        لمب_glyphs = لمب_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    منطقي has_data () ثابت { أرجع (منطقي) لمب_bearings; }

    فراغ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   صحيح *lsb) ثابت
    {
      إذا (glyph < لمب_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	أرجع;
      }

      إذا (unlikely (glyph >= لمب_bearings))
      {
        *lsb = 0;
	أرجع;
      }

      ثابت FWORD *bearings = (ثابت FWORD *) &table->longMetricZ[لمب_long_metrics];
      *lsb = bearings[glyph - لمب_long_metrics];
    }

    unsigned صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
    {
      إذا (glyph < لمب_bearings)
	أرجع table->longMetricZ[hb_min (glyph, (uint32_t) لمب_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      إذا (unlikely (!لمب_advances))
	أرجع default_advance;

      أرجع 0;
    }

#ifndef HB_NO_VAR
    unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) ثابت
    {
      unsigned صحيح advance = get_advance_without_var_unscaled (glyph);
      أرجع hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->لمب_coords,
								      store_cache)));
    }
#endif

    محمي:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    unsigned لمب_long_metrics;
    unsigned لمب_bearings;
    unsigned لمب_advances;
    unsigned لمب_glyphs;

    unsigned صحيح default_advance;

    عام:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    خاص:
    ساكن unsigned get_لمب_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      unsigned ret = hea->لمب_Of_Long_Metrics;
      hea.destroy ();
      أرجع ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  unsigned get_new_gid_advance_unscaled (ثابت hb_subset_plan_t *plan,
                                         ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحيح>> *mtx_map,
                                         unsigned new_gid,
                                         ثابت accelerator_t &_mtx) ثابت
  {
    إذا (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      أرجع plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    أرجع mtx_map->get (new_gid).first;
  }

  محمي:
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
  عام:
  DEFINE_SIZE_ARRAY (0, longMetricZ);
};

بنية hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  ساكن constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  ساكن constexpr منطقي is_horizontal = true;
};
بنية HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  ساكن constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  ساكن constexpr منطقي is_horizontal = true;
};
بنية vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  ساكن constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  ساكن constexpr منطقي is_horizontal = خطأ;
};
بنية VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  ساكن constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  ساكن constexpr منطقي is_horizontal = خطأ;
};

بنية hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  فراغ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 صحيح *lsb) ثابت
  {
    HB_DUAL_GET (*هذا, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    unsigned صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->لمب_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<HVAR> var_table;
};

#ifndef HB_NO_VERTICAL
بنية vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  فراغ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 صحيح *lsb) ثابت
  {
    HB_DUAL_GET (*هذا, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned صحيح get_advance_without_var_unscaled (hb_codepoint_t glyph) ثابت
  {
    أرجع HB_DUAL_GET (*هذا, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    unsigned صحيح advance = get_advance_without_var_unscaled (glyph);
    أرجع hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->لمب_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<VVAR> var_table;
};
#وإلا
بنية vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#endif

} /* namespace OT */


#endif /* HB_OT_HMTX_TABLE_HH */

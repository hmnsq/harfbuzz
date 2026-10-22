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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-dual.hh"
#अन्तर्भाव "hb-ot-maxp-table.hh"
#अन्तर्भाव "hb-ot-hhea-table.hh"
#अन्तर्भाव "hb-ot-os2-table.hh"
#अन्तर्भाव "hb-ot-var-hvar-table.hh"
#अन्तर्भाव "hb-ot-var-mvar-table.hh"
#अन्तर्भाव "hb-ot-metrics.hh"

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


नामाकाश OT {


संरचना LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};


template <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
संरचना hmtxvmtx
{
  तार्किक sanitize (hb_sanitize_context_t *c HB_UNUSED) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (सत्य);
  }

  ध्रुव hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, पूर्णाङ्क>>* get_mtx_map (ध्रुव hb_subset_plan_t *plan) ध्रुव
  { प्रत्यागम T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  तार्किक subset_update_header (hb_subset_context_t *c,
			     unsigned पूर्णाङ्क अंक_hmetrics,
			     ध्रुव hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, पूर्णाङ्क>> *mtx_map,
			     ध्रुव hb_vector_t<unsigned> &bounds_vec) ध्रुव
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    यदि (unlikely (!dest_blob)) {
      प्रत्यागम असत्य;
    }

    unsigned पूर्णाङ्क length;
    H *table = (H *) hb_blob_get_data (dest_blob, &length);
    c->serializer->check_assign (table->संख्या_Of_Long_Metrics, अंक_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#ifndef HB_NO_VAR
    यदि (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      यदि (T::is_horizontal)
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE,   caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN,    caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET, caretOffset);
      }
      अन्यथा
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RISE,     caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RUN,      caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET,   caretOffset);
      }

      तार्किक empty = सत्य;
      पूर्णाङ्क min_lsb = 0x7FFF;
      पूर्णाङ्क min_rsb = 0x7FFF;
      पूर्णाङ्क max_extent = -0x7FFF;
      unsigned max_adv = 0;
      प्रति (ध्रुव auto _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        unsigned adv = _.second.first;
        पूर्णाङ्क lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        यदि (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = असत्य;
          unsigned bound_width = bounds_vec[gid];
          पूर्णाङ्क rsb = adv - lsb - bound_width;
          पूर्णाङ्क extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      यदि (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      यदि (T::is_horizontal)
      {
        ध्रुव auto &OS2 = *c->plan->source->table.OS2;
        यदि (OS2.has_data () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->पंक्ति_Gap == OS2.s_Typo_पंक्ति_Gap)
        {
          table->ascender = static_cast<पूर्णाङ्क> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.length)));
          table->descender = static_cast<पूर्णाङ्क> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.length)));
          table->पंक्ति_Gap = static_cast<पूर्णाङ्क> (roundf (OS2.s_Typo_पंक्ति_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.length)));
        }
      }
    }
#endif

    तार्किक result = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    प्रत्यागम result;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ध्रुव hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned अंक_long_metrics,
                  unsigned total_अंक_metrics)
  {
    LongMetric* long_metrics = c->allocate_size<LongMetric> (अंक_long_metrics * LongMetric::static_size);
    FWORD* short_metrics = c->allocate_size<FWORD> ((total_अंक_metrics - अंक_long_metrics) * FWORD::static_size);
    यदि (!long_metrics || !short_metrics) प्रत्यागम;

    short_metrics -= अंक_long_metrics;

    प्रति (auto _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      auto mtx = *it++;

      यदि (gid < अंक_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      अन्यथा
        short_metrics[gid] = mtx.second;
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    auto *table_अभाज्य = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    unsigned अंक_long_metrics;
    ध्रुव hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, पूर्णाङ्क>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      auto& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      अंक_long_metrics = hb_min (plan->अंक_output_glyphs (), H::long_metric_गिनती_max ());
      unsigned पूर्णाङ्क last_advance = get_new_gid_advance_unscaled (plan, mtx_map, अंक_long_metrics - 1, _mtx);
      यावत् (अंक_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, अंक_long_metrics - 2, _mtx))
      {
	अंक_long_metrics--;
      }
    }

    auto it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<unsigned, पूर्णाङ्क> *v = nullptr;
		यदि (!mtx_map->has (new_gid, &v))
		{
		  पूर्णाङ्क lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  प्रत्यागम hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		प्रत्यागम *v;
	      })
    ;

    table_अभाज्य->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    अंक_long_metrics,
			    c->plan->अंक_output_glyphs ());

    यदि (unlikely (c->serializer->in_error ()))
      return_trace (असत्य);

    // Amend header num hmetrics
    यदि (unlikely (!subset_update_header (c, अंक_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (असत्य);

    return_trace (सत्य);
  }

  संरचना accelerator_t
  {
    friend संरचना hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_अंक_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, unsigned अंक_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      unsigned पूर्णाङ्क len = table.get_length ();
      यदि (len & 1)
        len--;

      अंक_long_metrics = अंक_long_metrics_2;
      यदि (unlikely (अंक_long_metrics > len / 4))
	अंक_long_metrics = len / 4;
      len -= अंक_long_metrics * 4;

      अंक_bearings = face->table.maxp->get_अंक_glyphs ();

      यदि (unlikely (अंक_bearings < अंक_long_metrics))
        अंक_bearings = अंक_long_metrics;
      यदि (unlikely ((अंक_bearings - अंक_long_metrics) * 2 > len))
        अंक_bearings = अंक_long_metrics + len / 2;
      len -= (अंक_bearings - अंक_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      यदि (unlikely (!अंक_long_metrics))
	अंक_bearings = अंक_long_metrics = 0;

      अंक_advances = अंक_bearings + len / 2;
      अंक_glyphs = face->get_अंक_glyphs ();
      यदि (अंक_glyphs < अंक_advances)
        अंक_glyphs = अंक_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    तार्किक has_data () ध्रुव { प्रत्यागम (तार्किक) अंक_bearings; }

    शून्य get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   पूर्णाङ्क *lsb) ध्रुव
    {
      यदि (glyph < अंक_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	प्रत्यागम;
      }

      यदि (unlikely (glyph >= अंक_bearings))
      {
        *lsb = 0;
	प्रत्यागम;
      }

      ध्रुव FWORD *bearings = (ध्रुव FWORD *) &table->longMetricZ[अंक_long_metrics];
      *lsb = bearings[glyph - अंक_long_metrics];
    }

    unsigned पूर्णाङ्क get_advance_without_var_unscaled (hb_codepoint_t glyph) ध्रुव
    {
      यदि (glyph < अंक_bearings)
	प्रत्यागम table->longMetricZ[hb_min (glyph, (uint32_t) अंक_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      यदि (unlikely (!अंक_advances))
	प्रत्यागम default_advance;

      प्रत्यागम 0;
    }

#ifndef HB_NO_VAR
    unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) ध्रुव
    {
      unsigned पूर्णाङ्क advance = get_advance_without_var_unscaled (glyph);
      प्रत्यागम hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->अंक_coords,
								      store_cache)));
    }
#endif

    रक्षित:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    unsigned अंक_long_metrics;
    unsigned अंक_bearings;
    unsigned अंक_advances;
    unsigned अंक_glyphs;

    unsigned पूर्णाङ्क default_advance;

    सार्वजनिक:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    गुप्त:
    स्थिर unsigned get_अंक_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      unsigned ret = hea->संख्या_Of_Long_Metrics;
      hea.destroy ();
      प्रत्यागम ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  unsigned get_new_gid_advance_unscaled (ध्रुव hb_subset_plan_t *plan,
                                         ध्रुव hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, पूर्णाङ्क>> *mtx_map,
                                         unsigned new_gid,
                                         ध्रुव accelerator_t &_mtx) ध्रुव
  {
    यदि (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      प्रत्यागम plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    प्रत्यागम mtx_map->get (new_gid).first;
  }

  रक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (0, longMetricZ);
};

संरचना hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  स्थिर constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  स्थिर constexpr तार्किक is_horizontal = सत्य;
};
संरचना HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  स्थिर constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  स्थिर constexpr तार्किक is_horizontal = सत्य;
};
संरचना vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  स्थिर constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  स्थिर constexpr तार्किक is_horizontal = असत्य;
};
संरचना VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  स्थिर constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  स्थिर constexpr तार्किक is_horizontal = असत्य;
};

संरचना hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  शून्य get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 पूर्णाङ्क *lsb) ध्रुव
  {
    HB_DUAL_GET (*एतत्, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned पूर्णाङ्क get_advance_without_var_unscaled (hb_codepoint_t glyph) ध्रुव
  {
    प्रत्यागम HB_DUAL_GET (*एतत्, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
    unsigned पूर्णाङ्क advance = get_advance_without_var_unscaled (glyph);
    प्रत्यागम hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->अंक_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<HVAR> var_table;
};

#ifndef HB_NO_VERTICAL
संरचना vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  शून्य get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 पूर्णाङ्क *lsb) ध्रुव
  {
    HB_DUAL_GET (*एतत्, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned पूर्णाङ्क get_advance_without_var_unscaled (hb_codepoint_t glyph) ध्रुव
  {
    प्रत्यागम HB_DUAL_GET (*एतत्, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
    unsigned पूर्णाङ्क advance = get_advance_without_var_unscaled (glyph);
    प्रत्यागम hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->अंक_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<VVAR> var_table;
};
#अन्यथा
संरचना vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#endif

} /* namespace OT */


#endif /* HB_OT_HMTX_TABLE_HH */

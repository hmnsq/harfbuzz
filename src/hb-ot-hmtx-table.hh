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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-dual.hh"
#અન્તર્ભાવ "hb-ot-maxp-table.hh"
#અન્તર્ભાવ "hb-ot-hhea-table.hh"
#અન્તર્ભાવ "hb-ot-os2-table.hh"
#અન્તર્ભાવ "hb-ot-var-hvar-table.hh"
#અન્તર્ભાવ "hb-ot-var-mvar-table.hh"
#અન્તર્ભાવ "hb-ot-metrics.hh"

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


નામાકાશ OT {


સંરચના LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};


template <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
સંરચના hmtxvmtx
{
  તાર્કિક sanitize (hb_sanitize_context_t *c HB_UNUSED) અચલ
  {
    TRACE_SANITIZE (આ);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (સાચું);
  }

  અચલ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, પૂર્ણાંક>>* get_mtx_map (અચલ hb_subset_plan_t *plan) અચલ
  { પાછા T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  તાર્કિક subset_update_header (hb_subset_context_t *c,
			     unsigned પૂર્ણાંક અંક_hmetrics,
			     અચલ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, પૂર્ણાંક>> *mtx_map,
			     અચલ hb_vector_t<unsigned> &bounds_vec) અચલ
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    જો (unlikely (!dest_blob)) {
      પાછા ખોટું;
    }

    unsigned પૂર્ણાંક length;
    H *table = (H *) hb_blob_get_data (dest_blob, &length);
    c->serializer->check_assign (table->સંખ્યા_Of_Long_Metrics, અંક_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#ifndef HB_NO_VAR
    જો (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      જો (T::is_horizontal)
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE,   caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN,    caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET, caretOffset);
      }
      નહીંતર
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RISE,     caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RUN,      caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET,   caretOffset);
      }

      તાર્કિક empty = સાચું;
      પૂર્ણાંક min_lsb = 0x7FFF;
      પૂર્ણાંક min_rsb = 0x7FFF;
      પૂર્ણાંક max_extent = -0x7FFF;
      unsigned max_adv = 0;
      માટે (અચલ auto _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        unsigned adv = _.second.first;
        પૂર્ણાંક lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        જો (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = ખોટું;
          unsigned bound_width = bounds_vec[gid];
          પૂર્ણાંક rsb = adv - lsb - bound_width;
          પૂર્ણાંક extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      જો (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      જો (T::is_horizontal)
      {
        અચલ auto &OS2 = *c->plan->source->table.OS2;
        જો (OS2.has_data () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->લીટી_Gap == OS2.s_Typo_લીટી_Gap)
        {
          table->ascender = static_cast<પૂર્ણાંક> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.length)));
          table->descender = static_cast<પૂર્ણાંક> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.length)));
          table->લીટી_Gap = static_cast<પૂર્ણાંક> (roundf (OS2.s_Typo_લીટી_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.length)));
        }
      }
    }
#endif

    તાર્કિક result = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    પાછા result;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<અચલ hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned અંક_long_metrics,
                  unsigned total_અંક_metrics)
  {
    LongMetric* long_metrics = c->allocate_size<LongMetric> (અંક_long_metrics * LongMetric::static_size);
    FWORD* short_metrics = c->allocate_size<FWORD> ((total_અંક_metrics - અંક_long_metrics) * FWORD::static_size);
    જો (!long_metrics || !short_metrics) પાછા;

    short_metrics -= અંક_long_metrics;

    માટે (auto _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      auto mtx = *it++;

      જો (gid < અંક_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      નહીંતર
        short_metrics[gid] = mtx.second;
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);

    auto *table_અવિભાજ્ય = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    unsigned અંક_long_metrics;
    અચલ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, પૂર્ણાંક>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      auto& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      અંક_long_metrics = hb_min (plan->અંક_output_glyphs (), H::long_metric_ગણતરી_max ());
      unsigned પૂર્ણાંક last_advance = get_new_gid_advance_unscaled (plan, mtx_map, અંક_long_metrics - 1, _mtx);
      જ્યારે (અંક_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, અંક_long_metrics - 2, _mtx))
      {
	અંક_long_metrics--;
      }
    }

    auto it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<unsigned, પૂર્ણાંક> *v = nullptr;
		જો (!mtx_map->has (new_gid, &v))
		{
		  પૂર્ણાંક lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  પાછા hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		પાછા *v;
	      })
    ;

    table_અવિભાજ્ય->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    અંક_long_metrics,
			    c->plan->અંક_output_glyphs ());

    જો (unlikely (c->serializer->in_error ()))
      return_trace (ખોટું);

    // Amend header num hmetrics
    જો (unlikely (!subset_update_header (c, અંક_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (ખોટું);

    return_trace (સાચું);
  }

  સંરચના accelerator_t
  {
    friend સંરચના hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_અંક_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, unsigned અંક_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      unsigned પૂર્ણાંક len = table.get_length ();
      જો (len & 1)
        len--;

      અંક_long_metrics = અંક_long_metrics_2;
      જો (unlikely (અંક_long_metrics > len / 4))
	અંક_long_metrics = len / 4;
      len -= અંક_long_metrics * 4;

      અંક_bearings = face->table.maxp->get_અંક_glyphs ();

      જો (unlikely (અંક_bearings < અંક_long_metrics))
        અંક_bearings = અંક_long_metrics;
      જો (unlikely ((અંક_bearings - અંક_long_metrics) * 2 > len))
        અંક_bearings = અંક_long_metrics + len / 2;
      len -= (અંક_bearings - અંક_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      જો (unlikely (!અંક_long_metrics))
	અંક_bearings = અંક_long_metrics = 0;

      અંક_advances = અંક_bearings + len / 2;
      અંક_glyphs = face->get_અંક_glyphs ();
      જો (અંક_glyphs < અંક_advances)
        અંક_glyphs = અંક_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    તાર્કિક has_data () અચલ { પાછા (તાર્કિક) અંક_bearings; }

    શૂન્ય get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   પૂર્ણાંક *lsb) અચલ
    {
      જો (glyph < અંક_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	પાછા;
      }

      જો (unlikely (glyph >= અંક_bearings))
      {
        *lsb = 0;
	પાછા;
      }

      અચલ FWORD *bearings = (અચલ FWORD *) &table->longMetricZ[અંક_long_metrics];
      *lsb = bearings[glyph - અંક_long_metrics];
    }

    unsigned પૂર્ણાંક get_advance_without_var_unscaled (hb_codepoint_t glyph) અચલ
    {
      જો (glyph < અંક_bearings)
	પાછા table->longMetricZ[hb_min (glyph, (uint32_t) અંક_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      જો (unlikely (!અંક_advances))
	પાછા default_advance;

      પાછા 0;
    }

#ifndef HB_NO_VAR
    unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) અચલ
    {
      unsigned પૂર્ણાંક advance = get_advance_without_var_unscaled (glyph);
      પાછા hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->અંક_coords,
								      store_cache)));
    }
#endif

    સુરક્ષિત:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    unsigned અંક_long_metrics;
    unsigned અંક_bearings;
    unsigned અંક_advances;
    unsigned અંક_glyphs;

    unsigned પૂર્ણાંક default_advance;

    જાહેર:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    ખાનગી:
    સ્થિર unsigned get_અંક_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      unsigned ret = hea->સંખ્યા_Of_Long_Metrics;
      hea.destroy ();
      પાછા ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  unsigned get_new_gid_advance_unscaled (અચલ hb_subset_plan_t *plan,
                                         અચલ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, પૂર્ણાંક>> *mtx_map,
                                         unsigned new_gid,
                                         અચલ accelerator_t &_mtx) અચલ
  {
    જો (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      પાછા plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    પાછા mtx_map->get (new_gid).first;
  }

  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_ARRAY (0, longMetricZ);
};

સંરચના hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  સ્થિર constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  સ્થિર constexpr તાર્કિક is_horizontal = સાચું;
};
સંરચના HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  સ્થિર constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  સ્થિર constexpr તાર્કિક is_horizontal = સાચું;
};
સંરચના vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  સ્થિર constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  સ્થિર constexpr તાર્કિક is_horizontal = ખોટું;
};
સંરચના VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  સ્થિર constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  સ્થિર constexpr તાર્કિક is_horizontal = ખોટું;
};

સંરચના hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  શૂન્ય get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 પૂર્ણાંક *lsb) અચલ
  {
    HB_DUAL_GET (*આ, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned પૂર્ણાંક get_advance_without_var_unscaled (hb_codepoint_t glyph) અચલ
  {
    પાછા HB_DUAL_GET (*આ, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
    unsigned પૂર્ણાંક advance = get_advance_without_var_unscaled (glyph);
    પાછા hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->અંક_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<HVAR> var_table;
};

#ifndef HB_NO_VERTICAL
સંરચના vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  શૂન્ય get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 પૂર્ણાંક *lsb) અચલ
  {
    HB_DUAL_GET (*આ, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned પૂર્ણાંક get_advance_without_var_unscaled (hb_codepoint_t glyph) અચલ
  {
    પાછા HB_DUAL_GET (*આ, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
    unsigned પૂર્ણાંક advance = get_advance_without_var_unscaled (glyph);
    પાછા hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->અંક_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<VVAR> var_table;
};
#નહીંતર
સંરચના vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#endif

} /* namespace OT */


#endif /* HB_OT_HMTX_TABLE_HH */

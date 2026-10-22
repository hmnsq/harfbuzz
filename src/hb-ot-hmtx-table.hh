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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-dual.hh"
#അന്തര്ഭാവ "hb-ot-maxp-table.hh"
#അന്തര്ഭാവ "hb-ot-hhea-table.hh"
#അന്തര്ഭാവ "hb-ot-os2-table.hh"
#അന്തര്ഭാവ "hb-ot-var-hvar-table.hh"
#അന്തര്ഭാവ "hb-ot-var-mvar-table.hh"
#അന്തര്ഭാവ "hb-ot-metrics.hh"

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


നാമാകാശ OT {


ഘടന LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};


template <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
ഘടന hmtxvmtx
{
  തർക്കം sanitize (hb_sanitize_context_t *c HB_UNUSED) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (സത്യം);
  }

  സ്ഥിരം hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>>* get_mtx_map (സ്ഥിരം hb_subset_plan_t *plan) സ്ഥിരം
  { തിരികെ T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  തർക്കം subset_update_header (hb_subset_context_t *c,
			     unsigned പൂർണ്ണസംഖ്യ അക്കം_hmetrics,
			     സ്ഥിരം hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *mtx_map,
			     സ്ഥിരം hb_vector_t<unsigned> &bounds_vec) സ്ഥിരം
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    എങ്കിൽ (unlikely (!dest_blob)) {
      തിരികെ അസത്യം;
    }

    unsigned പൂർണ്ണസംഖ്യ length;
    H *table = (H *) hb_blob_get_data (dest_blob, &length);
    c->serializer->check_assign (table->സംഖ്യ_Of_Long_Metrics, അക്കം_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#ifndef HB_NO_VAR
    എങ്കിൽ (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      എങ്കിൽ (T::is_horizontal)
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE,   caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN,    caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET, caretOffset);
      }
      അല്ലെങ്കിൽ
      {
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RISE,     caretSlopeRise);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_RUN,      caretSlopeRun);
	HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET,   caretOffset);
      }

      തർക്കം empty = സത്യം;
      പൂർണ്ണസംഖ്യ min_lsb = 0x7FFF;
      പൂർണ്ണസംഖ്യ min_rsb = 0x7FFF;
      പൂർണ്ണസംഖ്യ max_extent = -0x7FFF;
      unsigned max_adv = 0;
      വേണ്ടി (സ്ഥിരം auto _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        unsigned adv = _.second.first;
        പൂർണ്ണസംഖ്യ lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        എങ്കിൽ (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = അസത്യം;
          unsigned bound_width = bounds_vec[gid];
          പൂർണ്ണസംഖ്യ rsb = adv - lsb - bound_width;
          പൂർണ്ണസംഖ്യ extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      എങ്കിൽ (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      എങ്കിൽ (T::is_horizontal)
      {
        സ്ഥിരം auto &OS2 = *c->plan->source->table.OS2;
        എങ്കിൽ (OS2.has_data () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->വരി_Gap == OS2.s_Typo_വരി_Gap)
        {
          table->ascender = static_cast<പൂർണ്ണസംഖ്യ> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.length)));
          table->descender = static_cast<പൂർണ്ണസംഖ്യ> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.length)));
          table->വരി_Gap = static_cast<പൂർണ്ണസംഖ്യ> (roundf (OS2.s_Typo_വരി_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.length)));
        }
      }
    }
#endif

    തർക്കം result = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    തിരികെ result;
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<സ്ഥിരം hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned അക്കം_long_metrics,
                  unsigned total_അക്കം_metrics)
  {
    LongMetric* long_metrics = c->allocate_size<LongMetric> (അക്കം_long_metrics * LongMetric::static_size);
    FWORD* short_metrics = c->allocate_size<FWORD> ((total_അക്കം_metrics - അക്കം_long_metrics) * FWORD::static_size);
    എങ്കിൽ (!long_metrics || !short_metrics) തിരികെ;

    short_metrics -= അക്കം_long_metrics;

    വേണ്ടി (auto _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      auto mtx = *it++;

      എങ്കിൽ (gid < അക്കം_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      അല്ലെങ്കിൽ
        short_metrics[gid] = mtx.second;
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    auto *table_അഭാജ്യം = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    unsigned അക്കം_long_metrics;
    സ്ഥിരം hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      auto& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      അക്കം_long_metrics = hb_min (plan->അക്കം_output_glyphs (), H::long_metric_എണ്ണം_max ());
      unsigned പൂർണ്ണസംഖ്യ last_advance = get_new_gid_advance_unscaled (plan, mtx_map, അക്കം_long_metrics - 1, _mtx);
      ആകുമ്പോൾ (അക്കം_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, അക്കം_long_metrics - 2, _mtx))
      {
	അക്കം_long_metrics--;
      }
    }

    auto it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *v = nullptr;
		എങ്കിൽ (!mtx_map->has (new_gid, &v))
		{
		  പൂർണ്ണസംഖ്യ lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  തിരികെ hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		തിരികെ *v;
	      })
    ;

    table_അഭാജ്യം->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    അക്കം_long_metrics,
			    c->plan->അക്കം_output_glyphs ());

    എങ്കിൽ (unlikely (c->serializer->in_error ()))
      return_trace (അസത്യം);

    // Amend header num hmetrics
    എങ്കിൽ (unlikely (!subset_update_header (c, അക്കം_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  ഘടന accelerator_t
  {
    friend ഘടന hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_അക്കം_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, unsigned അക്കം_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      unsigned പൂർണ്ണസംഖ്യ len = table.get_length ();
      എങ്കിൽ (len & 1)
        len--;

      അക്കം_long_metrics = അക്കം_long_metrics_2;
      എങ്കിൽ (unlikely (അക്കം_long_metrics > len / 4))
	അക്കം_long_metrics = len / 4;
      len -= അക്കം_long_metrics * 4;

      അക്കം_bearings = face->table.maxp->get_അക്കം_glyphs ();

      എങ്കിൽ (unlikely (അക്കം_bearings < അക്കം_long_metrics))
        അക്കം_bearings = അക്കം_long_metrics;
      എങ്കിൽ (unlikely ((അക്കം_bearings - അക്കം_long_metrics) * 2 > len))
        അക്കം_bearings = അക്കം_long_metrics + len / 2;
      len -= (അക്കം_bearings - അക്കം_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      എങ്കിൽ (unlikely (!അക്കം_long_metrics))
	അക്കം_bearings = അക്കം_long_metrics = 0;

      അക്കം_advances = അക്കം_bearings + len / 2;
      അക്കം_glyphs = face->get_അക്കം_glyphs ();
      എങ്കിൽ (അക്കം_glyphs < അക്കം_advances)
        അക്കം_glyphs = അക്കം_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    തർക്കം has_data () സ്ഥിരം { തിരികെ (തർക്കം) അക്കം_bearings; }

    ശൂന്യം get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   പൂർണ്ണസംഖ്യ *lsb) സ്ഥിരം
    {
      എങ്കിൽ (glyph < അക്കം_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	തിരികെ;
      }

      എങ്കിൽ (unlikely (glyph >= അക്കം_bearings))
      {
        *lsb = 0;
	തിരികെ;
      }

      സ്ഥിരം FWORD *bearings = (സ്ഥിരം FWORD *) &table->longMetricZ[അക്കം_long_metrics];
      *lsb = bearings[glyph - അക്കം_long_metrics];
    }

    unsigned പൂർണ്ണസംഖ്യ get_advance_without_var_unscaled (hb_codepoint_t glyph) സ്ഥിരം
    {
      എങ്കിൽ (glyph < അക്കം_bearings)
	തിരികെ table->longMetricZ[hb_min (glyph, (uint32_t) അക്കം_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      എങ്കിൽ (unlikely (!അക്കം_advances))
	തിരികെ default_advance;

      തിരികെ 0;
    }

#ifndef HB_NO_VAR
    unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
    {
      unsigned പൂർണ്ണസംഖ്യ advance = get_advance_without_var_unscaled (glyph);
      തിരികെ hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->അക്കം_coords,
								      store_cache)));
    }
#endif

    സംരക്ഷിത:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    unsigned അക്കം_long_metrics;
    unsigned അക്കം_bearings;
    unsigned അക്കം_advances;
    unsigned അക്കം_glyphs;

    unsigned പൂർണ്ണസംഖ്യ default_advance;

    പൊതു:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    സ്വകാര്യ:
    സ്ഥിതം unsigned get_അക്കം_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      unsigned ret = hea->സംഖ്യ_Of_Long_Metrics;
      hea.destroy ();
      തിരികെ ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  unsigned get_new_gid_advance_unscaled (സ്ഥിരം hb_subset_plan_t *plan,
                                         സ്ഥിരം hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *mtx_map,
                                         unsigned new_gid,
                                         സ്ഥിരം accelerator_t &_mtx) സ്ഥിരം
  {
    എങ്കിൽ (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      തിരികെ plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    തിരികെ mtx_map->get (new_gid).first;
  }

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_ARRAY (0, longMetricZ);
};

ഘടന hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  സ്ഥിതം constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  സ്ഥിതം constexpr തർക്കം is_horizontal = സത്യം;
};
ഘടന HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  സ്ഥിതം constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  സ്ഥിതം constexpr തർക്കം is_horizontal = സത്യം;
};
ഘടന vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  സ്ഥിതം constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  സ്ഥിതം constexpr തർക്കം is_horizontal = അസത്യം;
};
ഘടന VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  സ്ഥിതം constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  സ്ഥിതം constexpr തർക്കം is_horizontal = അസത്യം;
};

ഘടന hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  ശൂന്യം get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 പൂർണ്ണസംഖ്യ *lsb) സ്ഥിരം
  {
    HB_DUAL_GET (*ഇത്, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned പൂർണ്ണസംഖ്യ get_advance_without_var_unscaled (hb_codepoint_t glyph) സ്ഥിരം
  {
    തിരികെ HB_DUAL_GET (*ഇത്, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ advance = get_advance_without_var_unscaled (glyph);
    തിരികെ hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->അക്കം_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<HVAR> var_table;
};

#ifndef HB_NO_VERTICAL
ഘടന vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  ശൂന്യം get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 പൂർണ്ണസംഖ്യ *lsb) സ്ഥിരം
  {
    HB_DUAL_GET (*ഇത്, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  unsigned പൂർണ്ണസംഖ്യ get_advance_without_var_unscaled (hb_codepoint_t glyph) സ്ഥിരം
  {
    തിരികെ HB_DUAL_GET (*ഇത്, get_advance_without_var_unscaled (glyph));
  }

#ifndef HB_NO_VAR
  unsigned get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ advance = get_advance_without_var_unscaled (glyph);
    തിരികെ hb_clamp_to<unsigned> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->അക്കം_coords,
								    store_cache)));
  }
#endif

  hb_blob_ptr_t<VVAR> var_table;
};
#അല്ലെങ്കിൽ
ഘടന vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#endif

} /* namespace OT */


#endif /* HB_OT_HMTX_TABLE_HH */

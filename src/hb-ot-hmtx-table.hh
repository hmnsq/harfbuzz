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

#अगर_अघोषित HB_OT_HMTX_TABLE_HH
#घोषणा HB_OT_HMTX_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-dual.hh"
#समावेश "hb-ot-maxp-table.hh"
#समावेश "hb-ot-hhea-table.hh"
#समावेश "hb-ot-os2-table.hh"
#समावेश "hb-ot-var-hvar-table.hh"
#समावेश "hb-ot-var-mvar-table.hh"
#समावेश "hb-ot-metrics.hh"

/*
 * hmtx -- Horizontal Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hmtx
 * vmtx -- Vertical Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vmtx
 */
#घोषणा HB_OT_TAG_hmtx HB_TAG('h','m','t','x')
#घोषणा HB_OT_TAG_HMTX HB_TAG('H','M','T','X')
#घोषणा HB_OT_TAG_vmtx HB_TAG('v','m','t','x')
#घोषणा HB_OT_TAG_VMTX HB_TAG('V','M','T','X')


namespace OT {


काष्ठा LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  खुला:
  DEFINE_आकार_STATIC (4);
};


ढाँचा <typename T/*Data table type*/, typename H/*Header table type*/, typename V/*Var table type*/>
काष्ठा hmtxvmtx
{
  bool sanitize (hb_sanitize_context_t *c HB_UNUSED) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (true);
  }

  स्थिर hb_hashmap_t<hb_codepoint_t, hb_pair_t<अचिन्हित, पूर्णांक>>* get_mtx_map (स्थिर hb_subset_plan_t *plan) स्थिर
  { वापस T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  bool subset_update_header (hb_subset_context_t *c,
			     अचिन्हित पूर्णांक अंक_hmetrics,
			     स्थिर hb_hashmap_t<hb_codepoint_t, hb_pair_t<अचिन्हित, पूर्णांक>> *mtx_map,
			     स्थिर hb_vector_t<अचिन्हित> &bounds_vec) स्थिर
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_table<H> (c->plan->source, H::tableTag);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    अगर (unlikely (!dest_blob)) {
      वापस false;
    }

    अचिन्हित पूर्णांक लंबाई;
    H *table = (H *) hb_blob_get_तथ्य (dest_blob, &लंबाई);
    c->serializer->check_assign (table->संख्या_Of_Long_Metrics, अंक_hmetrics, HB_SERIALIZE_ERROR_INT_OVERFLOW);

#अगर_अघोषित HB_NO_VAR
    अगर (c->plan->normalized_coords)
    {
      स्वतः &MVAR = *c->plan->source->table.MVAR;
      अगर (T::is_horizontal)
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

      bool empty = true;
      पूर्णांक min_lsb = 0x7FFF;
      पूर्णांक min_rsb = 0x7FFF;
      पूर्णांक max_extent = -0x7FFF;
      अचिन्हित max_adv = 0;
      क्रम (स्थिर स्वतः _ : *mtx_map)
      {
        hb_codepoint_t gid = _.first;
        अचिन्हित adv = _.second.first;
        पूर्णांक lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        अगर (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  empty = false;
          अचिन्हित bound_width = bounds_vec[gid];
          पूर्णांक rsb = adv - lsb - bound_width;
          पूर्णांक extent = lsb + bound_width;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      table->advanceMax = max_adv;
      अगर (!empty)
      {
        table->minLeadingBearing = min_lsb;
        table->minTrailingBearing = min_rsb;
        table->maxExtent = max_extent;
      }

      अगर (T::is_horizontal)
      {
        स्थिर स्वतः &OS2 = *c->plan->source->table.OS2;
        अगर (OS2.has_तथ्य () &&
            table->ascender == OS2.sTypoAscender &&
            table->descender == OS2.sTypoDescender &&
            table->पंक्ति_Gap == OS2.s_Typo_पंक्ति_Gap)
        {
          table->ascender = static_cast<पूर्णांक> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.लंबाई)));
          table->descender = static_cast<पूर्णांक> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.लंबाई)));
          table->पंक्ति_Gap = static_cast<पूर्णांक> (roundf (OS2.s_Typo_पंक्ति_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.लंबाई)));
        }
      }
    }
#पूर्ण_अगर

    bool परिणाम = c->plan->add_table (H::tableTag, dest_blob);
    hb_blob_destroy (dest_blob);

    वापस परिणाम;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<स्थिर hb_codepoint_pair_t> new_to_old_gid_list,
		  अचिन्हित अंक_long_metrics,
                  अचिन्हित योग_अंक_metrics)
  {
    LongMetric* long_metrics = c->allocate_आकार<LongMetric> (अंक_long_metrics * LongMetric::static_आकार);
    FWORD* short_metrics = c->allocate_आकार<FWORD> ((योग_अंक_metrics - अंक_long_metrics) * FWORD::static_आकार);
    अगर (!long_metrics || !short_metrics) वापस;

    short_metrics -= अंक_long_metrics;

    क्रम (स्वतः _ : new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      स्वतः mtx = *it++;

      अगर (gid < अंक_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.first;
	lm.sb = mtx.second;
      }
      अन्यथा
        short_metrics[gid] = mtx.second;
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);

    स्वतः *table_अभाज्य = c->serializer->start_embed <T> ();

    accelerator_t _mtx (c->plan->source);
    अचिन्हित अंक_long_metrics;
    स्थिर hb_hashmap_t<hb_codepoint_t, hb_pair_t<अचिन्हित, पूर्णांक>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      स्वतः& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      अंक_long_metrics = hb_min (plan->अंक_output_glyphs (), H::long_metric_गिनती_max ());
      अचिन्हित पूर्णांक last_advance = get_new_gid_advance_unscaled (plan, mtx_map, अंक_long_metrics - 1, _mtx);
      जबतक (अंक_long_metrics > 1 &&
	     last_advance == get_new_gid_advance_unscaled (plan, mtx_map, अंक_long_metrics - 2, _mtx))
      {
	अंक_long_metrics--;
      }
    }

    स्वतः it =
    + hb_iter (c->plan->new_to_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.first;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<अचिन्हित, पूर्णांक> *v = nullptr;
		अगर (!mtx_map->has (new_gid, &v))
		{
		  पूर्णांक lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  वापस hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		वापस *v;
	      })
    ;

    table_अभाज्य->serialize (c->serializer,
			    it,
			    c->plan->new_to_old_gid_list,
			    अंक_long_metrics,
			    c->plan->अंक_output_glyphs ());

    अगर (unlikely (c->serializer->in_error ()))
      return_trace (false);

    // Amend header num hmetrics
    अगर (unlikely (!subset_update_header (c, अंक_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_width_vec : c->plan->bounds_height_vec)))
      return_trace (false);

    return_trace (true);
  }

  काष्ठा accelerator_t
  {
    मित्र काष्ठा hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::tableTag, get_अंक_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_tag_t tableTag, अचिन्हित अंक_long_metrics_2)
    {
      table = hb_sanitize_context_t ().reference_table<hmtxvmtx> (face, tableTag);
      var_table = hb_sanitize_context_t ().reference_table<V> (face, T::variationsTag);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      अचिन्हित पूर्णांक len = table.get_लंबाई ();
      अगर (len & 1)
        len--;

      अंक_long_metrics = अंक_long_metrics_2;
      अगर (unlikely (अंक_long_metrics > len / 4))
	अंक_long_metrics = len / 4;
      len -= अंक_long_metrics * 4;

      अंक_bearings = face->table.maxp->get_अंक_glyphs ();

      अगर (unlikely (अंक_bearings < अंक_long_metrics))
        अंक_bearings = अंक_long_metrics;
      अगर (unlikely ((अंक_bearings - अंक_long_metrics) * 2 > len))
        अंक_bearings = अंक_long_metrics + len / 2;
      len -= (अंक_bearings - अंक_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      अगर (unlikely (!अंक_long_metrics))
	अंक_bearings = अंक_long_metrics = 0;

      अंक_advances = अंक_bearings + len / 2;
      अंक_glyphs = face->get_अंक_glyphs ();
      अगर (अंक_glyphs < अंक_advances)
        अंक_glyphs = अंक_advances;
    }
    ~accelerator_t ()
    {
      table.destroy ();
      var_table.destroy ();
    }

    bool has_तथ्य () स्थिर { वापस (bool) अंक_bearings; }

    व्योम get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   पूर्णांक *lsb) स्थिर
    {
      अगर (glyph < अंक_long_metrics)
      {
	*lsb = table->longMetricZ[glyph].sb;
	वापस;
      }

      अगर (unlikely (glyph >= अंक_bearings))
      {
        *lsb = 0;
	वापस;
      }

      स्थिर FWORD *bearings = (स्थिर FWORD *) &table->longMetricZ[अंक_long_metrics];
      *lsb = bearings[glyph - अंक_long_metrics];
    }

    अचिन्हित पूर्णांक get_advance_without_var_unscaled (hb_codepoint_t glyph) स्थिर
    {
      अगर (glyph < अंक_bearings)
	वापस table->longMetricZ[hb_min (glyph, (uint32_t) अंक_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      अगर (unlikely (!अंक_advances))
	वापस default_advance;

      वापस 0;
    }

#अगर_अघोषित HB_NO_VAR
    अचिन्हित get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = nullptr) स्थिर
    {
      अचिन्हित पूर्णांक advance = get_advance_without_var_unscaled (glyph);
      वापस hb_clamp_to<अचिन्हित> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								      font->coords, font->अंक_coords,
								      store_cache)));
    }
#पूर्ण_अगर

    रक्षित:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    अचिन्हित अंक_long_metrics;
    अचिन्हित अंक_bearings;
    अचिन्हित अंक_advances;
    अचिन्हित अंक_glyphs;

    अचिन्हित पूर्णांक default_advance;

    खुला:
    hb_blob_ptr_t<hmtxvmtx> table;
    hb_blob_ptr_t<V> var_table;

    निजी:
    अटल अचिन्हित get_अंक_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_table<H> (face);
      अचिन्हित ret = hea->संख्या_Of_Long_Metrics;
      hea.destroy ();
      वापस ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  अचिन्हित get_new_gid_advance_unscaled (स्थिर hb_subset_plan_t *plan,
                                         स्थिर hb_hashmap_t<hb_codepoint_t, hb_pair_t<अचिन्हित, पूर्णांक>> *mtx_map,
                                         अचिन्हित new_gid,
                                         स्थिर accelerator_t &_mtx) स्थिर
  {
    अगर (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      वापस plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    वापस mtx_map->लो (new_gid).first;
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
  खुला:
  DEFINE_आकार_ARRAY (0, longMetricZ);
};

काष्ठा hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_hmtx;
  अटल constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  अटल constexpr bool is_horizontal = true;
};
काष्ठा HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_HMTX;
  अटल constexpr hb_tag_t variationsTag = HB_OT_TAG_HVAR;
  अटल constexpr bool is_horizontal = true;
};
काष्ठा vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_vmtx;
  अटल constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  अटल constexpr bool is_horizontal = false;
};
काष्ठा VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_VMTX;
  अटल constexpr hb_tag_t variationsTag = HB_OT_TAG_VVAR;
  अटल constexpr bool is_horizontal = false;
};

काष्ठा hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
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

  व्योम get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 पूर्णांक *lsb) स्थिर
  {
    HB_DUAL_GET (*यह, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  अचिन्हित पूर्णांक get_advance_without_var_unscaled (hb_codepoint_t glyph) स्थिर
  {
    वापस HB_DUAL_GET (*यह, get_advance_without_var_unscaled (glyph));
  }

#अगर_अघोषित HB_NO_VAR
  अचिन्हित get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
    अचिन्हित पूर्णांक advance = get_advance_without_var_unscaled (glyph);
    वापस hb_clamp_to<अचिन्हित> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->अंक_coords,
								    store_cache)));
  }
#पूर्ण_अगर

  hb_blob_ptr_t<HVAR> var_table;
};

#अगर_अघोषित HB_NO_VERTICAL
काष्ठा vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
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

  व्योम get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 पूर्णांक *lsb) स्थिर
  {
    HB_DUAL_GET (*यह, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  अचिन्हित पूर्णांक get_advance_without_var_unscaled (hb_codepoint_t glyph) स्थिर
  {
    वापस HB_DUAL_GET (*यह, get_advance_without_var_unscaled (glyph));
  }

#अगर_अघोषित HB_NO_VAR
  अचिन्हित get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
    अचिन्हित पूर्णांक advance = get_advance_without_var_unscaled (glyph);
    वापस hb_clamp_to<अचिन्हित> (advance + roundf (var_table->get_advance_delta_unscaled (glyph,
								    font->coords, font->अंक_coords,
								    store_cache)));
  }
#पूर्ण_अगर

  hb_blob_ptr_t<VVAR> var_table;
};
#अन्यथा
काष्ठा vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#पूर्ण_अगर

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_HMTX_TABLE_HH */

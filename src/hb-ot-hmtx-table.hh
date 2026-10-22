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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_HMTX_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_HMTX_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-dual.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-maxp-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hhea-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-hvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-mvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-metrics.hh"

/*
 * hmtx -- Horizontal Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hmtx
 * vmtx -- Vertical Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vmtx
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_hmtx HB_ਟੈਗ('h','m','t','x')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_HMTX HB_ਟੈਗ('H','M','T','X')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_vmtx HB_ਟੈਗ('v','m','t','x')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VMTX HB_ਟੈਗ('V','M','T','X')


ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ LongMetric
{
  UFWORD	advance; /* Advance width/height. */
  FWORD		sb; /* Leading (left/top) side bearing. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T/*Data table type*/, ਕਿਸਮ_ਨਾਂ H/*Header table type*/, ਕਿਸਮ_ਨਾਂ V/*Var table type*/>
ਬਣਤਰ hmtxvmtx
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* We don't check for anything specific here.  The users of the
     * struct do all the hard work... */
    return_trace (ਸੱਚ);
  }

  ਅਚਲ hb_hashmap_t<hb_codepoint_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>>* get_mtx_map (ਅਚਲ hb_subset_plan_t *plan) ਅਚਲ
  { ਵਾਪਸ T::is_horizontal ? &plan->hmtx_map : &plan->vmtx_map; }

  ਤਾਰਕਿਕ subset_update_ਸਿਰਲੇਖ (hb_subset_context_t *c,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_hmetrics,
			     ਅਚਲ hb_hashmap_t<hb_codepoint_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *mtx_map,
			     ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &bounds_vec) ਅਚਲ
  {
    hb_blob_t *src_blob = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<H> (c->plan->ਸਰੋਤ, H::ਸਾਰਣੀ_ਟੈਗ);
    hb_blob_t *dest_blob = hb_blob_copy_writable_or_fail (src_blob);
    hb_blob_destroy (src_blob);

    ਜੇ (unlikely (!dest_blob)) {
      ਵਾਪਸ ਝੂਠ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ;
    H *ਸਾਰਣੀ = (H *) hb_blob_get_ਅੰਕੜੇ (dest_blob, &ਲੰਮਾਈ);
    c->serializer->check_ਸੌਂਪੋ (ਸਾਰਣੀ->ਸੰਖਿਆ_ਦਾ_Long_Metrics, ਅੰਕ_hmetrics, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (c->plan->normalized_coords)
    {
      ਸਵੈਚਾਲਿਤ &MVAR = *c->plan->ਸਰੋਤ->ਸਾਰਣੀ.MVAR;
      ਜੇ (T::is_horizontal)
      {
	HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_RISE,   caret_ਢਲਾਨ_Rise);
	HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_ਲੜੀਵਾਰ,    caret_ਢਲਾਨ_ਲੜੀਵਾਰ);
	HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_CARET_ਵਿੱਥ, caret_ਵਿੱਥ);
      }
      ਨਹੀਂਤਾਂ
      {
	HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_RISE,     caret_ਢਲਾਨ_Rise);
	HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_ਲੜੀਵਾਰ,      caret_ਢਲਾਨ_ਲੜੀਵਾਰ);
	HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_VERTICAL_CARET_ਵਿੱਥ,   caret_ਵਿੱਥ);
      }

      ਤਾਰਕਿਕ ਖਾਲੀ_ਥਾਵਾਂ_2 = ਸੱਚ;
      ਪੂਰਨ min_lsb = 0x7FFF;
      ਪੂਰਨ min_rsb = 0x7FFF;
      ਪੂਰਨ max_extent = -0x7FFF;
      ਬਿਨਾਨਿਸ਼ਾਨੀ max_adv = 0;
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : *mtx_map)
      {
        hb_codepoint_t gid = _.ਪਹਿਲਾ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ adv = _.second.ਪਹਿਲਾ;
        ਪੂਰਨ lsb = _.second.second;
        max_adv = hb_max (max_adv, adv);

        ਜੇ (bounds_vec[gid] != 0xFFFFFFFF)
        {
	  ਖਾਲੀ_ਥਾਵਾਂ_2 = ਝੂਠ;
          ਬਿਨਾਨਿਸ਼ਾਨੀ bound_ਚੌੜਾਈ = bounds_vec[gid];
          ਪੂਰਨ rsb = adv - lsb - bound_ਚੌੜਾਈ;
          ਪੂਰਨ extent = lsb + bound_ਚੌੜਾਈ;
          min_lsb = hb_min (min_lsb, lsb);
          min_rsb = hb_min (min_rsb, rsb);
          max_extent = hb_max (max_extent, extent);
        }
      }

      ਸਾਰਣੀ->advanceMax = max_adv;
      ਜੇ (!ਖਾਲੀ_ਥਾਵਾਂ_2)
      {
        ਸਾਰਣੀ->minLeadingBearing = min_lsb;
        ਸਾਰਣੀ->minTrailingBearing = min_rsb;
        ਸਾਰਣੀ->maxExtent = max_extent;
      }

      ਜੇ (T::is_horizontal)
      {
        ਅਚਲ ਸਵੈਚਾਲਿਤ &OS2 = *c->plan->ਸਰੋਤ->ਸਾਰਣੀ.OS2;
        ਜੇ (OS2.has_ਅੰਕੜੇ () &&
            ਸਾਰਣੀ->ascender == OS2.sTypoAscender &&
            ਸਾਰਣੀ->descender == OS2.sTypoDescender &&
            ਸਾਰਣੀ->ਸਤਰ_Gap == OS2.s_Typo_ਸਤਰ_Gap)
        {
          ਸਾਰਣੀ->ascender = static_cast<ਪੂਰਨ> (roundf (OS2.sTypoAscender +
                                                      MVAR.get_var (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER,
                                                                    c->plan->normalized_coords.arrayZ,
                                                                    c->plan->normalized_coords.ਲੰਮਾਈ)));
          ਸਾਰਣੀ->descender = static_cast<ਪੂਰਨ> (roundf (OS2.sTypoDescender +
                                                       MVAR.get_var (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER,
                                                                     c->plan->normalized_coords.arrayZ,
                                                                     c->plan->normalized_coords.ਲੰਮਾਈ)));
          ਸਾਰਣੀ->ਸਤਰ_Gap = static_cast<ਪੂਰਨ> (roundf (OS2.s_Typo_ਸਤਰ_Gap +
                                                     MVAR.get_var (HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP,
                                                                   c->plan->normalized_coords.arrayZ,
                                                                   c->plan->normalized_coords.ਲੰਮਾਈ)));
        }
      }
    }
#ਜੇ_ਅੰਤ

    ਤਾਰਕਿਕ ਨਤੀਜਾ = c->plan->ਜੋੜੋ_ਸਾਰਣੀ (H::ਸਾਰਣੀ_ਟੈਗ, dest_blob);
    hb_blob_destroy (dest_blob);

    ਵਾਪਸ ਨਤੀਜਾ;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  hb_array_t<ਅਚਲ hb_codepoint_pair_t> new_ਮੰਜ਼ਿਲ_old_gid_list,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_long_metrics,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਅੰਕ_metrics)
  {
    LongMetric* long_metrics = c->allocate_ਮਾਪ<LongMetric> (ਅੰਕ_long_metrics * LongMetric::static_ਮਾਪ);
    FWORD* short_metrics = c->allocate_ਮਾਪ<FWORD> ((ਕੁੱਲ_ਅੰਕ_metrics - ਅੰਕ_long_metrics) * FWORD::static_ਮਾਪ);
    ਜੇ (!long_metrics || !short_metrics) ਵਾਪਸ;

    short_metrics -= ਅੰਕ_long_metrics;

    ਲਈ (ਸਵੈਚਾਲਿਤ _ : new_ਮੰਜ਼ਿਲ_old_gid_list)
    {
      hb_codepoint_t gid = _.ਪਹਿਲਾ;
      ਸਵੈਚਾਲਿਤ mtx = *it++;

      ਜੇ (gid < ਅੰਕ_long_metrics)
      {
	LongMetric& lm = long_metrics[gid];
	lm.advance = mtx.ਪਹਿਲਾ;
	lm.sb = mtx.second;
      }
      ਨਹੀਂਤਾਂ
        short_metrics[gid] = mtx.second;
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਸਵੈਚਾਲਿਤ *ਸਾਰਣੀ_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed <T> ();

    accelerator_t _mtx (c->plan->ਸਰੋਤ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_long_metrics;
    ਅਚਲ hb_hashmap_t<hb_codepoint_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *mtx_map = get_mtx_map (c->plan);
    {
      /* Determine num_long_metrics to encode. */
      ਸਵੈਚਾਲਿਤ& plan = c->plan;

      // TODO Don't consider retaingid holes here.

      ਅੰਕ_long_metrics = hb_min (plan->ਅੰਕ_output_glyphs (), H::long_metric_ਗਿਣਤੀ_max ());
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_advance = get_new_gid_advance_unscaled (plan, mtx_map, ਅੰਕ_long_metrics - 1, _mtx);
      ਜਦੋਂ (ਅੰਕ_long_metrics > 1 &&
	     ਆਖਰੀ_advance == get_new_gid_advance_unscaled (plan, mtx_map, ਅੰਕ_long_metrics - 2, _mtx))
      {
	ਅੰਕ_long_metrics--;
      }
    }

    ਸਵੈਚਾਲਿਤ it =
    + hb_iter (c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
    | hb_map ([&_mtx, mtx_map] (hb_codepoint_pair_t _)
	      {
		hb_codepoint_t new_gid = _.ਪਹਿਲਾ;
		hb_codepoint_t old_gid = _.second;

		hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *v = ਖਾਲੀ_ਸੂਚਕ;
		ਜੇ (!mtx_map->has (new_gid, &v))
		{
		  ਪੂਰਨ lsb = 0;
		  _mtx.get_leading_bearing_without_var_unscaled (old_gid, &lsb);
		  ਵਾਪਸ hb_pair (_mtx.get_advance_without_var_unscaled (old_gid), +lsb);
		}
		ਵਾਪਸ *v;
	      })
    ;

    ਸਾਰਣੀ_ਅਭਾਜ->serialize (c->serializer,
			    it,
			    c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list,
			    ਅੰਕ_long_metrics,
			    c->plan->ਅੰਕ_output_glyphs ());

    ਜੇ (unlikely (c->serializer->ਵਿੱਚ_error ()))
      return_trace (ਝੂਠ);

    // Amend header num hmetrics
    ਜੇ (unlikely (!subset_update_ਸਿਰਲੇਖ (c, ਅੰਕ_long_metrics, mtx_map,
                                         T::is_horizontal ? c->plan->bounds_ਚੌੜਾਈ_vec : c->plan->bounds_ਉਚਾਈ_vec)))
      return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਬਣਤਰ accelerator_t
  {
    ਮਿੱਤਰ ਬਣਤਰ hmtxvmtx;

    accelerator_t (hb_face_t *face) :
      accelerator_t (face, T::ਸਾਰਣੀ_ਟੈਗ, get_ਅੰਕ_long_metrics (face))
    {}

    accelerator_t (hb_face_t *face, hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_long_metrics_2)
    {
      ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<hmtxvmtx> (face, ਸਾਰਣੀ_ਟੈਗ);
      var_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<V> (face, T::variations_ਟੈਗ);

      default_advance = T::is_horizontal ? hb_face_get_upem (face) / 2 : hb_face_get_upem (face);

      /* Populate count variables and sort them out as we go */

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = ਸਾਰਣੀ.get_ਲੰਮਾਈ ();
      ਜੇ (len & 1)
        len--;

      ਅੰਕ_long_metrics = ਅੰਕ_long_metrics_2;
      ਜੇ (unlikely (ਅੰਕ_long_metrics > len / 4))
	ਅੰਕ_long_metrics = len / 4;
      len -= ਅੰਕ_long_metrics * 4;

      ਅੰਕ_bearings = face->ਸਾਰਣੀ.maxp->get_ਅੰਕ_glyphs ();

      ਜੇ (unlikely (ਅੰਕ_bearings < ਅੰਕ_long_metrics))
        ਅੰਕ_bearings = ਅੰਕ_long_metrics;
      ਜੇ (unlikely ((ਅੰਕ_bearings - ਅੰਕ_long_metrics) * 2 > len))
        ਅੰਕ_bearings = ਅੰਕ_long_metrics + len / 2;
      len -= (ਅੰਕ_bearings - ਅੰਕ_long_metrics) * 2;

      /* We MUST set num_bearings to zero if num_long_metrics is zero.
       * Our get_advance() depends on that. */
      ਜੇ (unlikely (!ਅੰਕ_long_metrics))
	ਅੰਕ_bearings = ਅੰਕ_long_metrics = 0;

      ਅੰਕ_advances = ਅੰਕ_bearings + len / 2;
      ਅੰਕ_glyphs = face->get_ਅੰਕ_glyphs ();
      ਜੇ (ਅੰਕ_glyphs < ਅੰਕ_advances)
        ਅੰਕ_glyphs = ਅੰਕ_advances;
    }
    ~accelerator_t ()
    {
      ਸਾਰਣੀ.destroy ();
      var_ਸਾਰਣੀ.destroy ();
    }

    ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ (ਤਾਰਕਿਕ) ਅੰਕ_bearings; }

    ਖਾਲੀ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						   ਪੂਰਨ *lsb) ਅਚਲ
    {
      ਜੇ (glyph < ਅੰਕ_long_metrics)
      {
	*lsb = ਸਾਰਣੀ->longMetricZ[glyph].sb;
	ਵਾਪਸ;
      }

      ਜੇ (unlikely (glyph >= ਅੰਕ_bearings))
      {
        *lsb = 0;
	ਵਾਪਸ;
      }

      ਅਚਲ FWORD *bearings = (ਅਚਲ FWORD *) &ਸਾਰਣੀ->longMetricZ[ਅੰਕ_long_metrics];
      *lsb = bearings[glyph - ਅੰਕ_long_metrics];
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_advance_without_var_unscaled (hb_codepoint_t glyph) ਅਚਲ
    {
      ਜੇ (glyph < ਅੰਕ_bearings)
	ਵਾਪਸ ਸਾਰਣੀ->longMetricZ[hb_min (glyph, (uint32_t) ਅੰਕ_long_metrics - 1)].advance;

      /* If num_advances is zero, it means we don't have the metrics table
       * for this direction: return default advance. */
      ਜੇ (unlikely (!ਅੰਕ_advances))
	ਵਾਪਸ default_advance;

      ਵਾਪਸ 0;
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਬਿਨਾਨਿਸ਼ਾਨੀ get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					    hb_font_t         *font,
					    hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ advance = get_advance_without_var_unscaled (glyph);
      ਵਾਪਸ hb_clamp_ਮੰਜ਼ਿਲ<ਬਿਨਾਨਿਸ਼ਾਨੀ> (advance + roundf (var_ਸਾਰਣੀ->get_advance_delta_unscaled (glyph,
								      font->coords, font->ਅੰਕ_coords,
								      store_cache)));
    }
#ਜੇ_ਅੰਤ

    ਸੁਰੱਖਿਅਤ:
    // 0 <= num_long_metrics <= num_bearings <= num_advances <= num_glyphs
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_long_metrics;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_bearings;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_advances;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ default_advance;

    ਜਨਤਕ:
    hb_blob_ptr_t<hmtxvmtx> ਸਾਰਣੀ;
    hb_blob_ptr_t<V> var_ਸਾਰਣੀ;

    ਨਿਜੀ:
    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਅੰਕ_long_metrics (hb_face_t *face)
    {
      hb_blob_ptr_t<H> hea = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<H> (face);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ret = hea->ਸੰਖਿਆ_ਦਾ_Long_Metrics;
      hea.destroy ();
      ਵਾਪਸ ret;
    }
  };

  /* get advance: when no variations, call get_advance_without_var_unscaled.
   * when there're variations, get advance value from mtx_map in subset_plan*/
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_new_gid_advance_unscaled (ਅਚਲ hb_subset_plan_t *plan,
                                         ਅਚਲ hb_hashmap_t<hb_codepoint_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *mtx_map,
                                         ਬਿਨਾਨਿਸ਼ਾਨੀ new_gid,
                                         ਅਚਲ accelerator_t &_mtx) ਅਚਲ
  {
    ਜੇ (mtx_map->is_empty ())
    {
      hb_codepoint_t old_gid = 0;
      ਵਾਪਸ plan->old_gid_for_new_gid (new_gid, &old_gid) ?
             _mtx.get_advance_without_var_unscaled (old_gid) : 0;
    }
    ਵਾਪਸ mtx_map->get (new_gid).ਪਹਿਲਾ;
  }

  ਸੁਰੱਖਿਅਤ:
  Unsized_Array_ਦਾ<LongMetric>
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
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (0, longMetricZ);
};

ਬਣਤਰ hmtx : hmtxvmtx<hmtx, hhea, HVAR> {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_hmtx;
  ਸਥਿਰ constexpr hb_ਟੈਗ_t variations_ਟੈਗ = HB_OT_ਟੈਗ_HVAR;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_horizontal = ਸੱਚ;
};
ਬਣਤਰ HMTX : hmtxvmtx<HMTX, HHEA, HVAR> {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_HMTX;
  ਸਥਿਰ constexpr hb_ਟੈਗ_t variations_ਟੈਗ = HB_OT_ਟੈਗ_HVAR;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_horizontal = ਸੱਚ;
};
ਬਣਤਰ vmtx : hmtxvmtx<vmtx, vhea, VVAR> {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_vmtx;
  ਸਥਿਰ constexpr hb_ਟੈਗ_t variations_ਟੈਗ = HB_OT_ਟੈਗ_VVAR;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_horizontal = ਝੂਠ;
};
ਬਣਤਰ VMTX : hmtxvmtx<VMTX, VHEA, VVAR> {
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_VMTX;
  ਸਥਿਰ constexpr hb_ਟੈਗ_t variations_ਟੈਗ = HB_OT_ਟੈਗ_VVAR;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_horizontal = ਝੂਠ;
};

ਬਣਤਰ hmtx_accelerator_t : hb_dual_accelerator_t<hmtx::accelerator_t,
						  HMTX::accelerator_t>
{
  hmtx_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<hmtx::accelerator_t, HMTX::accelerator_t> (face)
  {
    var_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<HVAR> (face);
  }

  ~hmtx_accelerator_t ()
  {
    var_ਸਾਰਣੀ.destroy ();
  }

  ਖਾਲੀ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 ਪੂਰਨ *lsb) ਅਚਲ
  {
    HB_DUAL_GET (*ਇਹ, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_advance_without_var_unscaled (hb_codepoint_t glyph) ਅਚਲ
  {
    ਵਾਪਸ HB_DUAL_GET (*ਇਹ, get_advance_without_var_unscaled (glyph));
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ advance = get_advance_without_var_unscaled (glyph);
    ਵਾਪਸ hb_clamp_ਮੰਜ਼ਿਲ<ਬਿਨਾਨਿਸ਼ਾਨੀ> (advance + roundf (var_ਸਾਰਣੀ->get_advance_delta_unscaled (glyph,
								    font->coords, font->ਅੰਕ_coords,
								    store_cache)));
  }
#ਜੇ_ਅੰਤ

  hb_blob_ptr_t<HVAR> var_ਸਾਰਣੀ;
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
ਬਣਤਰ vmtx_accelerator_t : hb_dual_accelerator_t<vmtx::accelerator_t,
						  VMTX::accelerator_t>
{
  vmtx_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<vmtx::accelerator_t, VMTX::accelerator_t> (face)
  {
    var_ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<VVAR> (face);
  }

  ~vmtx_accelerator_t ()
  {
    var_ਸਾਰਣੀ.destroy ();
  }

  ਖਾਲੀ get_leading_bearing_without_var_unscaled (hb_codepoint_t glyph,
						 ਪੂਰਨ *lsb) ਅਚਲ
  {
    HB_DUAL_GET (*ਇਹ, get_leading_bearing_without_var_unscaled (glyph, lsb));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_advance_without_var_unscaled (hb_codepoint_t glyph) ਅਚਲ
  {
    ਵਾਪਸ HB_DUAL_GET (*ਇਹ, get_advance_without_var_unscaled (glyph));
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_advance_with_var_unscaled (hb_codepoint_t     glyph,
					  hb_font_t         *font,
					  hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ advance = get_advance_without_var_unscaled (glyph);
    ਵਾਪਸ hb_clamp_ਮੰਜ਼ਿਲ<ਬਿਨਾਨਿਸ਼ਾਨੀ> (advance + roundf (var_ਸਾਰਣੀ->get_advance_delta_unscaled (glyph,
								    font->coords, font->ਅੰਕ_coords,
								    store_cache)));
  }
#ਜੇ_ਅੰਤ

  hb_blob_ptr_t<VVAR> var_ਸਾਰਣੀ;
};
#ਨਹੀਂਤਾਂ
ਬਣਤਰ vmtx_accelerator_t : vmtx::accelerator_t {
  vmtx_accelerator_t (hb_face_t *face) : vmtx::accelerator_t (face) {}
};
#ਜੇ_ਅੰਤ

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_HMTX_TABLE_HH */

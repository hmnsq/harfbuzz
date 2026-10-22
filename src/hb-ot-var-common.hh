/*
 * Copyright © 2021  Google, Inc.
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
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_COMMON_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-alloc-pool.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-priority-queue.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-instancer-iup.hh"


ਨਾਮਾਕਾਸ਼ OT {

ਵਰਤੋਂ rebase_tent_ਨਤੀਜਾ_scratch_t = hb_pair_t<rebase_tent_ਨਤੀਜਾ_t, rebase_tent_ਨਤੀਜਾ_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
ਬਣਤਰ Tuple_Variation_ਸਿਰਲੇਖ
{
  ਮਿੱਤਰ ਬਣਤਰ tuple_delta_t;
  size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ_times_2) ਅਚਲ
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    ਬਿਨਾਨਿਸ਼ਾਨੀ ti = tuple_ਸੂਚਕਾਂਕ;
    ਜੇ (unlikely ((ti & (Tuple_ਸੂਚਕਾਂਕ::EmbeddedPeakTuple | Tuple_ਸੂਚਕਾਂਕ::IntermediateRegion))))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ((ti & Tuple_ਸੂਚਕਾਂਕ::EmbeddedPeakTuple) != 0) + ((ti & Tuple_ਸੂਚਕਾਂਕ::IntermediateRegion) != 0) * 2;
      ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (ਗਿਣਤੀ_2, axis_ਗਿਣਤੀ_times_2, min_ਮਾਪ);
    }
    ਵਾਪਸ min_ਮਾਪ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਅੰਕੜੇ_ਮਾਪ () ਅਚਲ { ਵਾਪਸ var_ਅੰਕੜੇ_ਮਾਪ; }

  ਅਚਲ Tuple_Variation_ਸਿਰਲੇਖ &get_ਅਗਲਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ_times_2) ਅਚਲ
  { ਵਾਪਸ Struct_At_ਵਿੱਥ<Tuple_Variation_ਸਿਰਲੇਖ> (ਇਹ, get_ਮਾਪ (axis_ਗਿਣਤੀ_times_2)); }

  ਤਾਰਕਿਕ unpack_axis_tuples (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
                           ਅਚਲ hb_array_t<ਅਚਲ F2DOT14> shared_tuples,
                           ਅਚਲ hb_map_t *axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                           hb_hashmap_t<hb_ਟੈਗ_t, Triple>& axis_tuples /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਅਚਲ F2DOT14 *peak_tuple = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (has_peak ())
      peak_tuple = get_peak_tuple (axis_ਗਿਣਤੀ);
    ਨਹੀਂਤਾਂ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = get_ਸੂਚਕਾਂਕ ();
      ਜੇ (unlikely ((ਸੂਚਕਾਂਕ_2 + 1) * axis_ਗਿਣਤੀ > shared_tuples.ਲੰਮਾਈ))
        ਵਾਪਸ ਝੂਠ;
      peak_tuple = shared_tuples.sub_array (axis_ਗਿਣਤੀ * ਸੂਚਕਾਂਕ_2, axis_ਗਿਣਤੀ).arrayZ;
    }

    ਅਚਲ F2DOT14 *ਸ਼ੁਰੂ_tuple = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ F2DOT14 *ਸਿਰਾ_tuple = ਖਾਲੀ_ਸੂਚਕ;
    ਤਾਰਕਿਕ has_interm = has_intermediate ();

    ਜੇ (has_interm)
    {
      ਸ਼ੁਰੂ_tuple = get_ਸ਼ੁਰੂ_tuple (axis_ਗਿਣਤੀ);
      ਸਿਰਾ_tuple = get_ਸਿਰਾ_tuple (axis_ਗਿਣਤੀ);
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ; i++)
    {
      ਦਸ਼ਮਲਵ peak = peak_tuple[i].ਮੰਜ਼ਿਲ_float ();
      ਜੇ (peak == 0.f) ਜਾਰੀ;

      hb_ਟੈਗ_t *axis_ਟੈਗ_2;
      ਜੇ (!axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map->has (i, &axis_ਟੈਗ_2))
        ਵਾਪਸ ਝੂਠ;

      ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ, ਸਿਰਾ;
      ਜੇ (has_interm)
      {
        ਸ਼ੁਰੂ = ਸ਼ੁਰੂ_tuple[i].ਮੰਜ਼ਿਲ_float ();
        ਸਿਰਾ = ਸਿਰਾ_tuple[i].ਮੰਜ਼ਿਲ_float ();
      }
      ਨਹੀਂਤਾਂ
      {
        ਸ਼ੁਰੂ = hb_min (peak, 0.f);
        ਸਿਰਾ = hb_max (peak, 0.f);
      }
      axis_tuples.ਸੈੱਟ (*axis_ਟੈਗ_2, Triple (ਸ਼ੁਰੂ, peak, ਸਿਰਾ));
    }

    ਵਾਪਸ ਸੱਚ;
  }

  HB_ALWAYS_INLINE
  ਦਸ਼ਮਲਵ calculate_scalar (hb_array_t<ਅਚਲ ਪੂਰਨ> coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
			   ਅਚਲ hb_array_t<ਅਚਲ F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ tuple_ਸੂਚਕਾਂਕ_2 = tuple_ਸੂਚਕਾਂਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = tuple_ਸੂਚਕਾਂਕ_2 & Tuple_ਸੂਚਕਾਂਕ::Tuple_ਸੂਚਕਾਂਕ_ਨਕਾਬ;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    ਤਾਰਕਿਕ plain_shared_tuple = (tuple_ਸੂਚਕਾਂਕ_2 & ~Tuple_ਸੂਚਕਾਂਕ::ਨਿੱਜੀ_ਬਿੰਦੂ_Numbers) < shared_tuple_scalar_cache->ਲੰਮਾਈ;
    ਜੇ (likely (plain_shared_tuple))
    {
      ਦਸ਼ਮਲਵ scalar;
      ਜੇ (likely (shared_tuple_scalar_cache->get (ਸੂਚਕਾਂਕ_2, &scalar)))
	ਵਾਪਸ scalar;
    }

    ਅਚਲ F2DOT14 *peak_tuple;

    ਤਾਰਕਿਕ has_interm = tuple_ਸੂਚਕਾਂਕ_2 & Tuple_ਸੂਚਕਾਂਕ::IntermediateRegion; // Inlined for performance

    ਜੇ (unlikely (tuple_ਸੂਚਕਾਂਕ_2 & Tuple_ਸੂਚਕਾਂਕ::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_ਗਿਣਤੀ);
      shared_tuple_scalar_cache = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਨਹੀਂਤਾਂ
    {
      ਦਸ਼ਮਲਵ scalar;
      ਜੇ (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->ਲੰਮਾਈ &&
	  shared_tuple_scalar_cache->get (ਸੂਚਕਾਂਕ_2, &scalar))
      {
        ਜੇ (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = ਖਾਲੀ_ਸੂਚਕ;
	ਨਹੀਂਤਾਂ
	  ਵਾਪਸ scalar;
      }

      ਜੇ (unlikely ((ਸੂਚਕਾਂਕ_2 + 1) * coord_ਗਿਣਤੀ > shared_tuples.ਲੰਮਾਈ))
        ਵਾਪਸ 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_ਗਿਣਤੀ * ਸੂਚਕਾਂਕ_2);

    }

    ਅਚਲ F2DOT14 *ਸ਼ੁਰੂ_tuple = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ F2DOT14 *ਸਿਰਾ_tuple = ਖਾਲੀ_ਸੂਚਕ;

    ਜੇ (has_interm)
    {
      ਸ਼ੁਰੂ_tuple = get_ਸ਼ੁਰੂ_tuple (coord_ਗਿਣਤੀ);
      ਸਿਰਾ_tuple = get_ਸਿਰਾ_tuple (coord_ਗਿਣਤੀ);
    }

    ਦਸ਼ਮਲਵ scalar = 1.f;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
    ਤਾਰਕਿਕ skip = coord_ਗਿਣਤੀ >= 16;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < coord_ਗਿਣਤੀ; i++)
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
#ਜੇ HB_FAST_ਅੰਕ_ACCESS
      ਜੇ (skip)
      {
	ਜਦੋਂ (i + 4 <= coord_ਗਿਣਤੀ && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	ਜਦੋਂ (i < coord_ਗਿਣਤੀ && peak_tuple[i].ਮੰਜ਼ਿਲ_int () == 0)
	  i += 1;
	ਜੇ (i >= coord_ਗਿਣਤੀ)
	  ਰੋਕੋ;
      }
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

      ਪੂਰਨ peak = peak_tuple[i].ਮੰਜ਼ਿਲ_int ();
      ਜੇ (!peak) ਜਾਰੀ;

      ਪੂਰਨ v = coords[i];
      ਜੇ (!v) { scalar = 0.f; ਰੋਕੋ; }
      ਜੇ (v == peak) ਜਾਰੀ;

      ਜੇ (has_interm)
      {
	shared_tuple_scalar_cache = ਖਾਲੀ_ਸੂਚਕ;
        ਪੂਰਨ ਸ਼ੁਰੂ = ਸ਼ੁਰੂ_tuple[i].ਮੰਜ਼ਿਲ_int ();
        ਪੂਰਨ ਸਿਰਾ = ਸਿਰਾ_tuple[i].ਮੰਜ਼ਿਲ_int ();
        ਜੇ (unlikely (ਸ਼ੁਰੂ > peak || peak > ਸਿਰਾ ||
                      (ਸ਼ੁਰੂ < 0 && ਸਿਰਾ > 0 && peak))) ਜਾਰੀ;
        ਜੇ (v < ਸ਼ੁਰੂ || v > ਸਿਰਾ) { scalar = 0.f; ਰੋਕੋ; }
        ਜੇ (v < peak)
        { ਜੇ (peak != ਸ਼ੁਰੂ) scalar *= (ਦਸ਼ਮਲਵ) (v - ਸ਼ੁਰੂ) / (peak - ਸ਼ੁਰੂ); }
        ਨਹੀਂਤਾਂ
        { ਜੇ (peak != ਸਿਰਾ) scalar *= (ਦਸ਼ਮਲਵ) (ਸਿਰਾ - v) / (ਸਿਰਾ - peak); }
      }
      ਨਹੀਂਤਾਂ ਜੇ (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; ਰੋਕੋ; }
      ਨਹੀਂਤਾਂ
        scalar *= (ਦਸ਼ਮਲਵ) v / peak;
    }
    ਜੇ (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->ਸੈੱਟ (ਸੂਚਕਾਂਕ_2, scalar);
    ਵਾਪਸ scalar;
  }

  ਤਾਰਕਿਕ           has_peak () ਅਚਲ { ਵਾਪਸ tuple_ਸੂਚਕਾਂਕ & Tuple_ਸੂਚਕਾਂਕ::EmbeddedPeakTuple; }
  ਤਾਰਕਿਕ   has_intermediate () ਅਚਲ { ਵਾਪਸ tuple_ਸੂਚਕਾਂਕ & Tuple_ਸੂਚਕਾਂਕ::IntermediateRegion; }
  ਤਾਰਕਿਕ has_ਨਿੱਜੀ_points () ਅਚਲ { ਵਾਪਸ tuple_ਸੂਚਕਾਂਕ & Tuple_ਸੂਚਕਾਂਕ::ਨਿੱਜੀ_ਬਿੰਦੂ_Numbers; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ      get_ਸੂਚਕਾਂਕ () ਅਚਲ { ਵਾਪਸ tuple_ਸੂਚਕਾਂਕ & Tuple_ਸੂਚਕਾਂਕ::Tuple_ਸੂਚਕਾਂਕ_ਨਕਾਬ; }
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ max_shared_tuple_ਗਿਣਤੀ = 0x1000u;

  ਸੁਰੱਖਿਅਤ:
  ਬਣਤਰ Tuple_ਸੂਚਕਾਂਕ : HBUINT16
  {
    ਗਿਣਤੀ Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      ਨਿੱਜੀ_ਬਿੰਦੂ_Numbers = 0x2000u,
      Tuple_ਸੂਚਕਾਂਕ_ਨਕਾਬ      = 0x0FFFu
    };

    Tuple_ਸੂਚਕਾਂਕ& ਸੰਚਾਲਕ = (uint16_t i) { HBUINT16::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }
    DEFINE_ਮਾਪ_STATIC (2);
  };

  hb_array_t<ਅਚਲ F2DOT14> get_all_tuples (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ) ਅਚਲ
  { ਵਾਪਸ StructAfter<Unsized_Array_ਦਾ<F2DOT14>> (tuple_ਸੂਚਕਾਂਕ).as_array ((has_peak () + has_intermediate () * 2) * axis_ਗਿਣਤੀ); }
  ਅਚਲ F2DOT14* get_all_tuples_ਅਧਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ) ਅਚਲ
  { ਵਾਪਸ StructAfter<Unsized_Array_ਦਾ<F2DOT14>> (tuple_ਸੂਚਕਾਂਕ).arrayZ; }
  ਅਚਲ F2DOT14* get_peak_tuple (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ) ਅਚਲ
  { ਵਾਪਸ get_all_tuples_ਅਧਾਰ (axis_ਗਿਣਤੀ); }
  ਅਚਲ F2DOT14* get_ਸ਼ੁਰੂ_tuple (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ) ਅਚਲ
  { ਵਾਪਸ get_all_tuples_ਅਧਾਰ (axis_ਗਿਣਤੀ) + has_peak () * axis_ਗਿਣਤੀ; }
  ਅਚਲ F2DOT14* get_ਸਿਰਾ_tuple (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ) ਅਚਲ
  { ਵਾਪਸ get_all_tuples_ਅਧਾਰ (axis_ਗਿਣਤੀ) + has_peak () * axis_ਗਿਣਤੀ + axis_ਗਿਣਤੀ; }

  HBUINT16      var_ਅੰਕੜੇ_ਮਾਪ;    /* The size in bytes of the serialized
                                 * data for this tuple variation table. */
  Tuple_ਸੂਚਕਾਂਕ    tuple_ਸੂਚਕਾਂਕ;     /* A packed field. The high 4 bits are flags (see below).
                                   The low 12 bits are an index into a shared tuple
                                   records array. */
  /* UnsizedArrayOf<F2DOT14> peakTuple - optional */
                                /* Peak tuple record for this tuple variation table — optional,
                                 * determined by flags in the tupleIndex value.
                                 *
                                 * Note that this must always be included in the 'cvar' table. */
  /* UnsizedArrayOf<F2DOT14> intermediateStartTuple - optional */
                                /* Intermediate start tuple record for this tuple variation table — optional,
                                   determined by flags in the tupleIndex value. */
  /* UnsizedArrayOf<F2DOT14> intermediateEndTuple - optional */
                                /* Intermediate end tuple record for this tuple variation table — optional,
                                 * determined by flags in the tupleIndex value. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (4);
};

ਬਣਤਰ optimize_scratch_t
{
  iup_scratch_t iup;
  hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ> opt_indices;
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> rounded_x_deltas;
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> rounded_y_deltas;
  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> opt_deltas_x;
  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> opt_deltas_y;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> opt_ਬਿੰਦੂ_ਅੰਕੜੇ;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> opt_deltas_ਅੰਕੜੇ;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> ਬਿੰਦੂ_ਅੰਕੜੇ;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> deltas_ਅੰਕੜੇ;
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> rounded_deltas;
};

ਬਣਤਰ tuple_delta_t
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ realloc_move = ਸੱਚ;  // Watch out when adding new members!

  ਜਨਤਕ:
  hb_hashmap_t<hb_ਟੈਗ_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ> indices;

  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> deltas_x;
  /* empty for cvar tuples */
  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> compiled_tuple_ਸਿਰਲੇਖ;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> compiled_deltas;

  hb_ਸਦਿਸ਼_t<F2DOT14> compiled_peak_coords;
  hb_ਸਦਿਸ਼_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ) {}
  tuple_delta_t (ਅਚਲ tuple_delta_t& o) = ਮੂਲ;
  tuple_delta_t& ਸੰਚਾਲਕ = (ਅਚਲ tuple_delta_t& o) = ਮੂਲ;

  ਮਿੱਤਰ ਖਾਲੀ swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
  {
    hb_ਅਦਲਾ_ਬਦਲੀ (a.axis_tuples, b.axis_tuples);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.indices, b.indices);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.deltas_x, b.deltas_x);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.deltas_y, b.deltas_y);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.compiled_tuple_ਸਿਰਲੇਖ, b.compiled_tuple_ਸਿਰਲੇਖ);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.compiled_deltas, b.compiled_deltas);
    hb_ਅਦਲਾ_ਬਦਲੀ (a.compiled_peak_coords, b.compiled_peak_coords);
  }

  tuple_delta_t (tuple_delta_t&& o)  noexcept : tuple_delta_t ()
  { hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, o); }

  tuple_delta_t& ਸੰਚਾਲਕ = (tuple_delta_t&& o) noexcept
  {
    hb_ਅਦਲਾ_ਬਦਲੀ (*ਇਹ, o);
    ਵਾਪਸ *ਇਹ;
  }

  ਖਾਲੀ copy_from (ਅਚਲ tuple_delta_t& o, hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ)
  {
    axis_tuples = o.axis_tuples;
    indices.duplicate_ਸਦਿਸ਼_from_pool (pool, o.indices);
    deltas_x.duplicate_ਸਦਿਸ਼_from_pool (pool, o.deltas_x);
    deltas_y.duplicate_ਸਦਿਸ਼_from_pool (pool, o.deltas_y);
    compiled_tuple_ਸਿਰਲੇਖ.duplicate_ਸਦਿਸ਼_from_pool (pool, o.compiled_tuple_ਸਿਰਲੇਖ);
    compiled_deltas.duplicate_ਸਦਿਸ਼_from_pool (pool, o.compiled_deltas);
    compiled_peak_coords.duplicate_ਸਦਿਸ਼_from_pool (pool, o.compiled_peak_coords);
    compiled_interm_coords.duplicate_ਸਦਿਸ਼_from_pool (pool, o.compiled_interm_coords);
  }

  ਖਾਲੀ remove_axis (hb_ਟੈਗ_t axis_ਟੈਗ_2)
  { axis_tuples.del (axis_ਟੈਗ_2); }

  ਤਾਰਕਿਕ ਸੈੱਟ_tent (hb_ਟੈਗ_t axis_ਟੈਗ_2, Triple tent)
  { ਵਾਪਸ axis_tuples.ਸੈੱਟ (axis_ਟੈਗ_2, tent); }

  tuple_delta_t& ਸੰਚਾਲਕ += (ਅਚਲ tuple_delta_t& o)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ = indices.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ; i++)
    {
      ਜੇ (indices.arrayZ[i])
      {
        ਜੇ (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          ਜੇ (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      ਨਹੀਂਤਾਂ
      {
        ਜੇ (!o.indices.arrayZ[i]) ਜਾਰੀ;
        indices.arrayZ[i] = ਸੱਚ;
        deltas_x[i] = o.deltas_x[i];
        ਜੇ (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    ਵਾਪਸ *ਇਹ;
  }

  tuple_delta_t& ਸੰਚਾਲਕ *= (ਦਸ਼ਮਲਵ scalar)
  {
    ਜੇ (scalar == 1.0f)
      ਵਾਪਸ *ਇਹ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ = indices.ਲੰਮਾਈ;
    ਜੇ (deltas_y)
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ; i++)
      {
	ਜੇ (!indices.arrayZ[i]) ਜਾਰੀ;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    ਨਹੀਂਤਾਂ
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ; i++)
      {
	ਜੇ (!indices.arrayZ[i]) ਜਾਰੀ;
	deltas_x[i] *= scalar;
      }
    ਵਾਪਸ *ਇਹ;
  }

  ਖਾਲੀ change_tuple_var_axis_ਹੱਦਬੰਦੀ (hb_ਟੈਗ_t axis_ਟੈਗ_2, Triple axis_ਹੱਦਬੰਦੀ,
				    TripleDistances axis_triple_distances,
				    hb_ਸਦਿਸ਼_t<tuple_delta_t>& ਬਾਹਰਲਾ,
				    rebase_tent_ਨਤੀਜਾ_scratch_t &scratch,
				    hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ)
  {
    // May move *this out.

    ਬਾਹਰਲਾ.reset ();
    Triple *tent;
    ਜੇ (!axis_tuples.has (axis_ਟੈਗ_2, &tent))
    {
      ਬਾਹਰਲਾ.push (std::move (*ਇਹ));
      ਵਾਪਸ;
    }

    ਜੇ ((tent->minimum < 0.f && tent->ਵੱਧ_ਤੋਂ_ਵੱਧ > 0.f) ||
        !(tent->minimum <= tent->ਵਿਚਕਾਰ && tent->ਵਿਚਕਾਰ <= tent->ਵੱਧ_ਤੋਂ_ਵੱਧ))
      ਵਾਪਸ;

    ਜੇ (tent->ਵਿਚਕਾਰ == 0.f)
    {
      ਬਾਹਰਲਾ.push (std::move (*ਇਹ));
      ਵਾਪਸ;
    }

    ਜੇ (!axis_ਹੱਦਬੰਦੀ.is_ਬਿੰਦੂ () &&
        !(-1.f <= axis_ਹੱਦਬੰਦੀ.minimum &&
          axis_ਹੱਦਬੰਦੀ.minimum <= axis_ਹੱਦਬੰਦੀ.ਵਿਚਕਾਰ &&
          axis_ਹੱਦਬੰਦੀ.ਵਿਚਕਾਰ <= axis_ਹੱਦਬੰਦੀ.ਵੱਧ_ਤੋਂ_ਵੱਧ &&
          axis_ਹੱਦਬੰਦੀ.ਵੱਧ_ਤੋਂ_ਵੱਧ <= +1.f))
      ਵਾਪਸ;

    rebase_tent_ਨਤੀਜਾ_t &ਹੱਲ = scratch.ਪਹਿਲਾ;
    rebase_tent (*tent, axis_ਹੱਦਬੰਦੀ, axis_triple_distances, ਹੱਲ, scratch.second);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਹੱਲ.ਲੰਮਾਈ; i++)
    {
      ਸਵੈਚਾਲਿਤ &t = ਹੱਲ.arrayZ[i];

      tuple_delta_t new_var;
      ਜੇ (i < ਹੱਲ.ਲੰਮਾਈ - 1)
	new_var.copy_from (*ਇਹ, pool);
      ਨਹੀਂਤਾਂ
	new_var = std::move (*ਇਹ);

      ਜੇ (t.second == Triple ())
        new_var.remove_axis (axis_ਟੈਗ_2);
      ਨਹੀਂਤਾਂ
        new_var.ਸੈੱਟ_tent (axis_ਟੈਗ_2, t.second);

      new_var *= t.ਪਹਿਲਾ;
      ਬਾਹਰਲਾ.push (std::move (new_var));
    }
  }

  ਤਾਰਕਿਕ compile_coords (ਅਚਲ hb_map_t& axes_ਸੂਚਕਾਂਕ_map,
		       ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
		       hb_alloc_pool_t *pool= ਖਾਲੀ_ਸੂਚਕ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ cur_axis_ਗਿਣਤੀ = axes_ਸੂਚਕਾਂਕ_map.get_population ();
    ਜੇ (pool)
    {
      ਜੇ (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_ਗਿਣਤੀ)))
	ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (!compiled_peak_coords.resize (cur_axis_ਗਿਣਤੀ)))
      ਵਾਪਸ ਝੂਠ;

    hb_array_t<F2DOT14> ਸ਼ੁਰੂ_coords, ਸਿਰਾ_coords;

    ਬਿਨਾਨਿਸ਼ਾਨੀ orig_axis_ਗਿਣਤੀ = axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.get_population ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < orig_axis_ਗਿਣਤੀ; i++)
    {
      ਜੇ (!axes_ਸੂਚਕਾਂਕ_map.has (i))
        ਜਾਰੀ;

      hb_ਟੈਗ_t axis_ਟੈਗ_2 = axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.get (i);
      Triple *coords = ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (axis_tuples.has (axis_ਟੈਗ_2, &coords))
      {
	ਦਸ਼ਮਲਵ min_val = coords->minimum;
	ਦਸ਼ਮਲਵ val = coords->ਵਿਚਕਾਰ;
	ਦਸ਼ਮਲਵ max_val = coords->ਵੱਧ_ਤੋਂ_ਵੱਧ;

	compiled_peak_coords.arrayZ[j].ਸੈੱਟ_float (val);

	ਜੇ (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  ਜੇ (!compiled_interm_coords)
	  {
	    ਜੇ (pool)
	    {
	      ਜੇ (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_ਗਿਣਤੀ)))
		ਵਾਪਸ ਝੂਠ;
	    }
	    ਨਹੀਂਤਾਂ ਜੇ (unlikely (!compiled_interm_coords.resize (2 * cur_axis_ਗਿਣਤੀ)))
	      ਵਾਪਸ ਝੂਠ;
	    ਸ਼ੁਰੂ_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_ਗਿਣਤੀ);
	    ਸਿਰਾ_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_ਗਿਣਤੀ);

	    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ k = 0; k < j; k++)
	    {
	      ਨਿਸ਼ਾਨੀਵਾਲਾ peak = compiled_peak_coords.arrayZ[k].ਮੰਜ਼ਿਲ_int ();
	      ਜੇ (!peak) ਜਾਰੀ;
	      ਸ਼ੁਰੂ_coords.arrayZ[k].ਸੈੱਟ_int (hb_min (peak, 0));
	      ਸਿਰਾ_coords.arrayZ[k].ਸੈੱਟ_int (hb_max (peak, 0));
	    }
	  }

	}

	ਜੇ (compiled_interm_coords)
	{
	  ਸ਼ੁਰੂ_coords.arrayZ[j].ਸੈੱਟ_float (min_val);
	  ਸਿਰਾ_coords.arrayZ[j].ਸੈੱਟ_float (max_val);
	}
      }

      j++;
    }

    ਵਾਪਸ !compiled_peak_coords.ਵਿੱਚ_error () && !compiled_interm_coords.ਵਿੱਚ_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  ਤਾਰਕਿਕ compile_tuple_var_ਸਿਰਲੇਖ (ਅਚਲ hb_map_t& axes_ਸੂਚਕਾਂਕ_map,
                                 ਬਿਨਾਨਿਸ਼ਾਨੀ points_ਅੰਕੜੇ_ਲੰਮਾਈ,
                                 ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                 ਅਚਲ hb_hashmap_t<ਅਚਲ hb_ਸਦਿਸ਼_t<F2DOT14>*, ਬਿਨਾਨਿਸ਼ਾਨੀ>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    ਜੇ (!compiled_deltas) ਵਾਪਸ ਸੱਚ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ cur_axis_ਗਿਣਤੀ = axes_ਸੂਚਕਾਂਕ_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    ਬਿਨਾਨਿਸ਼ਾਨੀ alloc_len = 3 * cur_axis_ਗਿਣਤੀ * (F2DOT14::static_ਮਾਪ) + 4;
    ਜੇ (unlikely (!compiled_tuple_ਸਿਰਲੇਖ.allocate_from_pool (pool, alloc_len, ਝੂਠ))) ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_ਸਿਰਲੇਖ.begin () + 4);
    F2DOT14* ਸਿਰਾ = reinterpret_cast<F2DOT14 *> (compiled_tuple_ਸਿਰਲੇਖ.ਸਿਰਾ ());
    hb_array_t<F2DOT14> coords (p, ਸਿਰਾ - p);

    ਜੇ (!shared_tuples_idx_map)
      compile_coords (axes_ਸੂਚਕਾਂਕ_map, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    ਬਿਨਾਨਿਸ਼ਾਨੀ peak_ਗਿਣਤੀ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ *shared_tuple_idx;
    ਜੇ (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    ਨਹੀਂਤਾਂ
    {
      peak_ਗਿਣਤੀ = encode_peak_coords(coords, flag);
      ਜੇ (!peak_ਗਿਣਤੀ) ਵਾਪਸ ਝੂਠ;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    ਬਿਨਾਨਿਸ਼ਾਨੀ interim_ਗਿਣਤੀ = encode_interm_coords (coords.sub_array (peak_ਗਿਣਤੀ), flag);

    /* pointdata length = 0 implies "use shared points" */
    ਜੇ (points_ਅੰਕੜੇ_ਲੰਮਾਈ)
      flag |= Tuple_Variation_ਸਿਰਲੇਖ::Tuple_ਸੂਚਕਾਂਕ::ਨਿੱਜੀ_ਬਿੰਦੂ_Numbers;

    ਬਿਨਾਨਿਸ਼ਾਨੀ serialized_ਅੰਕੜੇ_ਮਾਪ = points_ਅੰਕੜੇ_ਲੰਮਾਈ + compiled_deltas.ਲੰਮਾਈ;
    Tuple_Variation_ਸਿਰਲੇਖ *o = reinterpret_cast<Tuple_Variation_ਸਿਰਲੇਖ *> (compiled_tuple_ਸਿਰਲੇਖ.begin ());
    o->var_ਅੰਕੜੇ_ਮਾਪ = serialized_ਅੰਕੜੇ_ਮਾਪ;
    o->tuple_ਸੂਚਕਾਂਕ = flag;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਸਿਰਲੇਖ_len = 4 + (peak_ਗਿਣਤੀ + interim_ਗਿਣਤੀ) * (F2DOT14::static_ਮਾਪ);
    compiled_tuple_ਸਿਰਲੇਖ.shrink_ਪਿੱਠ_ਮੰਜ਼ਿਲ_pool (pool, ਕੁੱਲ_ਸਿਰਲੇਖ_len);
    ਵਾਪਸ ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               ਬਿਨਾਨਿਸ਼ਾਨੀ& flag) ਅਚਲ
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.ਲੰਮਾਈ * ਆਕਾਰ (compiled_peak_coords[0]));
    flag |= Tuple_Variation_ਸਿਰਲੇਖ::Tuple_ਸੂਚਕਾਂਕ::EmbeddedPeakTuple;
    ਵਾਪਸ compiled_peak_coords.ਲੰਮਾਈ;
  }

  /* if no need to encode intermediate coords, then just return p */
  ਬਿਨਾਨਿਸ਼ਾਨੀ encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 ਬਿਨਾਨਿਸ਼ਾਨੀ& flag) ਅਚਲ
  {
    ਜੇ (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.ਲੰਮਾਈ * ਆਕਾਰ (compiled_interm_coords[0]));
      flag |= Tuple_Variation_ਸਿਰਲੇਖ::Tuple_ਸੂਚਕਾਂਕ::IntermediateRegion;
    }
    ਵਾਪਸ compiled_interm_coords.ਲੰਮਾਈ;
  }

  ਤਾਰਕਿਕ compile_deltas (hb_ਸਦਿਸ਼_t<ਪੂਰਨ> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ)
  { ਵਾਪਸ compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ਸਥਿਰ ਤਾਰਕਿਕ compile_deltas (hb_array_t<ਅਚਲ ਤਾਰਕਿਕ> ਬਿੰਦੂ_indices,
			      hb_array_t<ਅਚਲ ਦਸ਼ਮਲਵ> x_deltas,
			      hb_array_t<ਅਚਲ ਦਸ਼ਮਲਵ> y_deltas,
			      hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> &compiled_deltas, /*ਬਾਹਰਲਾ*/
			      hb_ਸਦਿਸ਼_t<ਪੂਰਨ> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (unlikely (!rounded_deltas.resize_dirty  (ਬਿੰਦੂ_indices.ਲੰਮਾਈ)))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਿੰਦੂ_indices.ਲੰਮਾਈ; i++)
    {
      ਜੇ (!ਬਿੰਦੂ_indices[i]) ਜਾਰੀ;
      rounded_deltas.arrayZ[j++] = hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (roundf ((ਦੁੱਗਣਾ) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    ਜੇ (!rounded_deltas) ਵਾਪਸ ਸੱਚ;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ alloc_len = 2 + 4 * rounded_deltas.ਲੰਮਾਈ + (rounded_deltas.ਲੰਮਾਈ + 63) / 64;
    ਜੇ (y_deltas)
      alloc_len *= 2;

    ਜੇ (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, ਝੂਠ))) ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    ਜੇ (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ idx = 0; idx < ਬਿੰਦੂ_indices.ਲੰਮਾਈ; idx++)
      {
        ਜੇ (!ਬਿੰਦੂ_indices[idx]) ਜਾਰੀ;
        ਪੂਰਨ rounded_delta = hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (roundf ((ਦੁੱਗਣਾ) y_deltas.arrayZ[idx]));

        ਜੇ (j >= rounded_deltas.ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;

        rounded_deltas[j++] = rounded_delta;
      }

      ਜੇ (j != rounded_deltas.ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_ਪਿੱਠ_ਮੰਜ਼ਿਲ_pool (pool, encoded_len);
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ compile_deltas (hb_array_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> encoded_ਬਾਈਟਾਂ,
				  hb_array_t<ਅਚਲ ਪੂਰਨ> deltas)
  {
    ਵਾਪਸ Tuple_ਮੁੱਲਾਂ::compile_unsafe (deltas, encoded_ਬਾਈਟਾਂ);
  }

  ਤਾਰਕਿਕ calc_inferred_deltas (ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t& orig_points,
			     hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &scratch)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਿੰਦੂ_ਗਿਣਤੀ = orig_points.ਲੰਮਾਈ;
    ਜੇ (ਬਿੰਦੂ_ਗਿਣਤੀ != indices.ਲੰਮਾਈ)
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ref_ਗਿਣਤੀ_2 = 0;

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &ਸਿਰਾ_points = scratch.reset ();

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਿੰਦੂ_ਗਿਣਤੀ; i++)
    {
      ref_ਗਿਣਤੀ_2 += indices.arrayZ[i];
      ਜੇ (orig_points.arrayZ[i].is_ਸਿਰਾ_ਬਿੰਦੂ)
        ਸਿਰਾ_points.push (i);
    }
    /* all points are referenced, nothing to do */
    ਜੇ (ref_ਗਿਣਤੀ_2 == ਬਿੰਦੂ_ਗਿਣਤੀ)
      ਵਾਪਸ ਸੱਚ;
    ਜੇ (unlikely (ਸਿਰਾ_points.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

    hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t inferred_idxes;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ਬਿੰਦੂ_2 = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਬਿੰਦੂ : ਸਿਰਾ_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ unref_ਗਿਣਤੀ = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ_ਬਿੰਦੂ_2; i < ਸਿਰਾ_ਬਿੰਦੂ + 1; i++)
        unref_ਗਿਣਤੀ += indices.arrayZ[i];
      unref_ਗਿਣਤੀ = (ਸਿਰਾ_ਬਿੰਦੂ - ਸ਼ੁਰੂ_ਬਿੰਦੂ_2 + 1) - unref_ਗਿਣਤੀ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ j = ਸ਼ੁਰੂ_ਬਿੰਦੂ_2;
      ਜੇ (unref_ਗਿਣਤੀ == 0 || unref_ਗਿਣਤੀ > ਸਿਰਾ_ਬਿੰਦੂ - ਸ਼ੁਰੂ_ਬਿੰਦੂ_2)
        ਜਾਓ no_ਹੋਰ_gaps;
      ਲਈ (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prev, next, i;
        ਲਈ (;;)
        {
          i = j;
          j = ਅਗਲਾ_ਸੂਚਕਾਂਕ (i, ਸ਼ੁਰੂ_ਬਿੰਦੂ_2, ਸਿਰਾ_ਬਿੰਦੂ);
          ਜੇ (indices.arrayZ[i] && !indices.arrayZ[j]) ਰੋਕੋ;
        }
        prev = j = i;
        ਲਈ (;;)
        {
          i = j;
          j = ਅਗਲਾ_ਸੂਚਕਾਂਕ (i, ਸ਼ੁਰੂ_ਬਿੰਦੂ_2, ਸਿਰਾ_ਬਿੰਦੂ);
          ਜੇ (!indices.arrayZ[i] && indices.arrayZ[j]) ਰੋਕੋ;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        ਲਈ (;;)
        {
          i = ਅਗਲਾ_ਸੂਚਕਾਂਕ (i, ਸ਼ੁਰੂ_ਬਿੰਦੂ_2, ਸਿਰਾ_ਬਿੰਦੂ);
          ਜੇ (i == next) ਰੋਕੋ;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.ਜੋੜੋ (i);
          ਜੇ (--unref_ਗਿਣਤੀ == 0) ਜਾਓ no_ਹੋਰ_gaps;
        }
      }
    no_ਹੋਰ_gaps:
      ਸ਼ੁਰੂ_ਬਿੰਦੂ_2 = ਸਿਰਾ_ਬਿੰਦੂ + 1;
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਿੰਦੂ_ਗਿਣਤੀ; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      ਜੇ ( !indices[i])
      {
        ਜੇ (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = ਸੱਚ;
      }
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ optimize (ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t& contour_points,
                 ਤਾਰਕਿਕ is_ਸੰਯੁਕਤ,
		 optimize_scratch_t &scratch,
                 ਦੁੱਗਣਾ tolerance = 0.5 + 1e-10)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = contour_points.ਲੰਮਾਈ;
    ਜੇ (deltas_x.ਲੰਮਾਈ != ਗਿਣਤੀ_2 ||
        deltas_y.ਲੰਮਾਈ != ਗਿਣਤੀ_2)
      ਵਾਪਸ ਝੂਠ;

    hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ> &opt_indices = scratch.opt_indices.reset ();
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> &rounded_y_deltas = scratch.rounded_y_deltas;

    ਜੇ (unlikely (!rounded_x_deltas.resize_dirty  (ਗਿਣਤੀ_2) ||
                  !rounded_y_deltas.resize_dirty  (ਗਿਣਤੀ_2)))
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (roundf ((ਦੁੱਗਣਾ) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (roundf ((ਦੁੱਗਣਾ) deltas_y.arrayZ[i]));
    }

    ਜੇ (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ref_ਗਿਣਤੀ_2 = 0;
    ਲਈ (ਤਾਰਕਿਕ ref_flag : opt_indices)
       ref_ਗਿਣਤੀ_2 += ref_flag;

    ਜੇ (ref_ਗਿਣਤੀ_2 == ਗਿਣਤੀ_2) ਵਾਪਸ ਸੱਚ;

    hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    ਤਾਰਕਿਕ is_comp_glyph_wo_deltas = (is_ਸੰਯੁਕਤ && ref_ਗਿਣਤੀ_2 == 0);
    ਜੇ (is_comp_glyph_wo_deltas)
    {
      ਜੇ (unlikely (!opt_deltas_x.resize (ਗਿਣਤੀ_2) ||
                    !opt_deltas_y.resize (ਗਿਣਤੀ_2)))
        ਵਾਪਸ ਝੂਠ;

      opt_indices.arrayZ[0] = ਸੱਚ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < ਗਿਣਤੀ_2; i++)
        opt_indices.arrayZ[i] = ਝੂਠ;
    }

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> &opt_ਬਿੰਦੂ_ਅੰਕੜੇ = scratch.opt_ਬਿੰਦੂ_ਅੰਕੜੇ.reset ();
    ਜੇ (!compile_ਬਿੰਦੂ_ਸੈੱਟ (opt_indices, opt_ਬਿੰਦੂ_ਅੰਕੜੇ))
      ਵਾਪਸ ਝੂਠ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> &opt_deltas_ਅੰਕੜੇ = scratch.opt_deltas_ਅੰਕੜੇ.reset ();
    ਜੇ (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_ਅੰਕੜੇ,
			 scratch.rounded_deltas))
      ਵਾਪਸ ਝੂਠ;

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> &ਬਿੰਦੂ_ਅੰਕੜੇ = scratch.ਬਿੰਦੂ_ਅੰਕੜੇ.reset ();
    ਜੇ (!compile_ਬਿੰਦੂ_ਸੈੱਟ (indices, ਬਿੰਦੂ_ਅੰਕੜੇ))
      ਵਾਪਸ ਝੂਠ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> &deltas_ਅੰਕੜੇ = scratch.deltas_ਅੰਕੜੇ.reset ();
    ਜੇ (!compile_deltas (indices, deltas_x, deltas_y, deltas_ਅੰਕੜੇ, scratch.rounded_deltas))
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (opt_ਬਿੰਦੂ_ਅੰਕੜੇ.ਲੰਮਾਈ + opt_deltas_ਅੰਕੜੇ.ਲੰਮਾਈ < ਬਿੰਦੂ_ਅੰਕੜੇ.ਲੰਮਾਈ + deltas_ਅੰਕੜੇ.ਲੰਮਾਈ)
    {
      indices = std::move (opt_indices);

      ਜੇ (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    ਵਾਪਸ !indices.ਵਿੱਚ_error () && !deltas_x.ਵਿੱਚ_error () && !deltas_y.ਵਿੱਚ_error ();
  }

  ਸਥਿਰ ਤਾਰਕਿਕ compile_ਬਿੰਦੂ_ਸੈੱਟ (ਅਚਲ hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ> &ਬਿੰਦੂ_indices,
                                 hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>& compiled_points /*ਬਾਹਰਲਾ*/)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_points = 0;
    ਲਈ (ਤਾਰਕਿਕ i : ਬਿੰਦੂ_indices)
      ਜੇ (i) ਅੰਕ_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    ਜੇ (!ਅੰਕ_points) ਵਾਪਸ ਸੱਚ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ indices_ਲੰਮਾਈ = ਬਿੰਦੂ_indices.ਲੰਮਾਈ;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    ਜੇ (ਅੰਕ_points == indices_ਲੰਮਾਈ)
      ਵਾਪਸ compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਬਾਈਟਾਂ = 2 + 3 *ਅੰਕ_points;
    ਜੇ (unlikely (!compiled_points.resize_dirty  (ਅੰਕ_ਬਾਈਟਾਂ)))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ pos = 0;
    /* binary data starts with the total number of reference points */
    ਜੇ (ਅੰਕ_points < 0x80)
      compiled_points.arrayZ[pos++] = ਅੰਕ_points;
    ਨਹੀਂਤਾਂ
    {
      compiled_points.arrayZ[pos++] = ((ਅੰਕ_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = ਅੰਕ_points & 0xFF;
    }

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਲੜੀਵਾਰ_ਲੰਮਾਈ = 0x7F;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_ਮੁੱਲ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_encoded = 0;
    ਜਦੋਂ (i < indices_ਲੰਮਾਈ && ਅੰਕ_encoded < ਅੰਕ_points)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਲੰਮਾਈ = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਲੇਖ_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      ਤਾਰਕਿਕ use_byte_encoding = ਝੂਠ;
      ਤਾਰਕਿਕ new_ਲੜੀਵਾਰ_2 = ਸੱਚ;
      ਜਦੋਂ (i < indices_ਲੰਮਾਈ && ਅੰਕ_encoded < ਅੰਕ_points &&
             ਲੜੀਵਾਰ_ਲੰਮਾਈ <= max_ਲੜੀਵਾਰ_ਲੰਮਾਈ)
      {
        // find out next referenced point index
        ਜਦੋਂ (i < indices_ਲੰਮਾਈ && !ਬਿੰਦੂ_indices[i])
          i++;

        ਜੇ (i >= indices_ਲੰਮਾਈ) ਰੋਕੋ;

        ਬਿਨਾਨਿਸ਼ਾਨੀ cur_ਮੁੱਲ = i;
        ਬਿਨਾਨਿਸ਼ਾਨੀ delta = cur_ਮੁੱਲ - ਆਖਰੀ_ਮੁੱਲ;

        ਜੇ (new_ਲੜੀਵਾਰ_2)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_ਲੜੀਵਾਰ_2 = ਝੂਠ;
        }

        ਜੇ (use_byte_encoding && delta > 0xFF)
          ਰੋਕੋ;

        ਜੇ (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        ਨਹੀਂਤਾਂ
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        ਆਖਰੀ_ਮੁੱਲ = cur_ਮੁੱਲ;
        ਲੜੀਵਾਰ_ਲੰਮਾਈ++;
        ਅੰਕ_encoded++;
      }

      ਜੇ (use_byte_encoding)
        compiled_points.arrayZ[ਸਿਰਲੇਖ_pos] = ਲੜੀਵਾਰ_ਲੰਮਾਈ - 1;
      ਨਹੀਂਤਾਂ
        compiled_points.arrayZ[ਸਿਰਲੇਖ_pos] = (ਲੜੀਵਾਰ_ਲੰਮਾਈ - 1) | 0x80;
    }
    ਵਾਪਸ compiled_points.resize_dirty  (pos);
  }

  ਸਥਿਰ ਦਸ਼ਮਲਵ infer_delta (ਦਸ਼ਮਲਵ ਨਿਸ਼ਾਨਾ_val, ਦਸ਼ਮਲਵ prev_val, ਦਸ਼ਮਲਵ ਅਗਲਾ_val, ਦਸ਼ਮਲਵ prev_delta, ਦਸ਼ਮਲਵ ਅਗਲਾ_delta)
  {
    ਜੇ (prev_val == ਅਗਲਾ_val)
      ਵਾਪਸ (prev_delta == ਅਗਲਾ_delta) ? prev_delta : 0.f;
    ਨਹੀਂਤਾਂ ਜੇ (ਨਿਸ਼ਾਨਾ_val <= hb_min (prev_val, ਅਗਲਾ_val))
      ਵਾਪਸ (prev_val < ਅਗਲਾ_val) ? prev_delta : ਅਗਲਾ_delta;
    ਨਹੀਂਤਾਂ ਜੇ (ਨਿਸ਼ਾਨਾ_val >= hb_max (prev_val, ਅਗਲਾ_val))
      ਵਾਪਸ (prev_val > ਅਗਲਾ_val) ? prev_delta : ਅਗਲਾ_delta;

    ਦਸ਼ਮਲਵ r = (ਨਿਸ਼ਾਨਾ_val - prev_val) / (ਅਗਲਾ_val - prev_val);
    ਵਾਪਸ prev_delta + r * (ਅਗਲਾ_delta - prev_delta);
  }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
  { ਵਾਪਸ (i >= ਸਿਰਾ) ? ਸ਼ੁਰੂ : (i + 1); }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OffType = HBUINT16>
ਬਣਤਰ Tuple_Variation_ਅੰਕੜੇ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (ਇਹ));
  }

  size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ_times_2) ਅਚਲ
  {
    size_t ਕੁੱਲ_ਮਾਪ = min_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = tuple_Var_ਗਿਣਤੀ.get_ਗਿਣਤੀ ();
    ਅਚਲ Tuple_Variation_ਸਿਰਲੇਖ *tuple_var_ਸਿਰਲੇਖ = &(get_tuple_var_ਸਿਰਲੇਖ());
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਕੁੱਲ_ਮਾਪ = hb_unsigned_ਜੋੜੋ_saturate (ਕੁੱਲ_ਮਾਪ,
					     tuple_var_ਸਿਰਲੇਖ->get_ਮਾਪ (axis_ਗਿਣਤੀ_times_2),
					     (size_t) tuple_var_ਸਿਰਲੇਖ->get_ਅੰਕੜੇ_ਮਾਪ ());
      tuple_var_ਸਿਰਲੇਖ = &tuple_var_ਸਿਰਲੇਖ->get_ਅਗਲਾ (axis_ਗਿਣਤੀ_times_2);
    }

    ਵਾਪਸ ਕੁੱਲ_ਮਾਪ;
  }

  ਅਚਲ Tuple_Variation_ਸਿਰਲੇਖ &get_tuple_var_ਸਿਰਲੇਖ (ਖਾਲੀ) ਅਚਲ
  { ਵਾਪਸ StructAfter<Tuple_Variation_ਸਿਰਲੇਖ> (ਅੰਕੜੇ); }

  ਬਣਤਰ tuple_iterator_t;
  ਬਣਤਰ tuple_variations_t
  {
    hb_ਸਦਿਸ਼_t<tuple_delta_t> tuple_vars;

    ਨਿਜੀ:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ>*, hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>> ਬਿੰਦੂ_ਅੰਕੜੇ_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ>*, ਬਿਨਾਨਿਸ਼ਾਨੀ> ਬਿੰਦੂ_ਸੈੱਟ_ਗਿਣਤੀ_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> *shared_points_ਬਾਈਟਾਂ = ਖਾਲੀ_ਸੂਚਕ;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    ਬਿਨਾਨਿਸ਼ਾਨੀ compiled_byte_ਮਾਪ = 0;
    ਤਾਰਕਿਕ needs_padding = ਝੂਠ;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    ਤਾਰਕਿਕ is_ਸੰਯੁਕਤ = ਝੂਠ;

    ਜਨਤਕ:
    tuple_variations_t () = ਮੂਲ;
    tuple_variations_t (ਅਚਲ tuple_variations_t&) = ਮਿਟਾਓ;
    tuple_variations_t& ਸੰਚਾਲਕ=(ਅਚਲ tuple_variations_t&) = ਮਿਟਾਓ;
    tuple_variations_t (tuple_variations_t&&) = ਮੂਲ;
    tuple_variations_t& ਸੰਚਾਲਕ=(tuple_variations_t&&) = ਮੂਲ;
    ~tuple_variations_t () = ਮੂਲ;

    ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ ਤਾਰਕਿਕ (tuple_vars); }
    ਬਿਨਾਨਿਸ਼ਾਨੀ get_var_ਗਿਣਤੀ () ਅਚਲ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      ਲਈ (ਸਵੈਚਾਲਿਤ& tuple: tuple_vars)
        ਜੇ (tuple.compiled_deltas) ਗਿਣਤੀ_2++;

      ਜੇ (shared_points_ਬਾਈਟਾਂ && shared_points_ਬਾਈਟਾਂ->ਲੰਮਾਈ)
        ਗਿਣਤੀ_2 |= Tuple_Var_ਗਿਣਤੀ::Shared_ਬਿੰਦੂ_Numbers;
      ਵਾਪਸ ਗਿਣਤੀ_2;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ get_compiled_byte_ਮਾਪ () ਅਚਲ
    { ਵਾਪਸ compiled_byte_ਮਾਪ; }

    ਤਾਰਕਿਕ create_from_tuple_var_ਅੰਕੜੇ (tuple_iterator_t iterator,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ tuple_var_ਗਿਣਤੀ,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਿੰਦੂ_ਗਿਣਤੀ,
                                     ਤਾਰਕਿਕ is_gvar,
                                     ਅਚਲ hb_map_t *axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                     ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &shared_indices,
                                     ਅਚਲ hb_array_t<ਅਚਲ F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ,
                                     ਤਾਰਕਿਕ is_ਸੰਯੁਕਤ_glyph = ਝੂਠ)
    {
      hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਨਿੱਜੀ_indices;
      hb_ਸਦਿਸ਼_t<ਪੂਰਨ> deltas_x;
      hb_ਸਦਿਸ਼_t<ਪੂਰਨ> deltas_y;
      ਕਰੋ
      {
        ਅਚਲ HBUINT8 *p = iterator.get_serialized_ਅੰਕੜੇ ();
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ = iterator.current_tuple->get_ਅੰਕੜੇ_ਮਾਪ ();
        ਜੇ (unlikely (!iterator.var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.check_range (p, ਲੰਮਾਈ)))
          ਵਾਪਸ ਝੂਠ;

	hb_hashmap_t<hb_ਟੈਗ_t, Triple> axis_tuples;
        ਜੇ (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_ਗਿਣਤੀ (), shared_tuples, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, axis_tuples)
            || axis_tuples.is_empty ())
          ਵਾਪਸ ਝੂਠ;

	ਨਿੱਜੀ_indices.reset ();
        ਤਾਰਕਿਕ has_ਨਿੱਜੀ_points = iterator.current_tuple->has_ਨਿੱਜੀ_points ();
        ਅਚਲ HBUINT8 *ਸਿਰਾ = p + ਲੰਮਾਈ;
        ਜੇ (has_ਨਿੱਜੀ_points &&
            !Tuple_Variation_ਅੰਕੜੇ::decompile_points (p, ਨਿੱਜੀ_indices, ਸਿਰਾ))
          ਵਾਪਸ ਝੂਠ;

        ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &indices = has_ਨਿੱਜੀ_points ? ਨਿੱਜੀ_indices : shared_indices;
        ਤਾਰਕਿਕ apply_ਮੰਜ਼ਿਲ_all = (indices.ਲੰਮਾਈ == 0);
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_deltas = apply_ਮੰਜ਼ਿਲ_all ? ਬਿੰਦੂ_ਗਿਣਤੀ : indices.ਲੰਮਾਈ;

        ਜੇ (unlikely (!deltas_x.resize_dirty  (ਅੰਕ_deltas) ||
                      !Tuple_Variation_ਅੰਕੜੇ::decompile_deltas (p, deltas_x, ਸਿਰਾ)))
          ਵਾਪਸ ਝੂਠ;

        ਜੇ (is_gvar)
        {
          ਜੇ (unlikely (!deltas_y.resize_dirty  (ਅੰਕ_deltas) ||
                        !Tuple_Variation_ਅੰਕੜੇ::decompile_deltas (p, deltas_y, ਸਿਰਾ)))
            ਵਾਪਸ ਝੂਠ;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        ਜੇ (unlikely (!var.indices.allocate_from_pool (pool, ਬਿੰਦੂ_ਗਿਣਤੀ) ||
                      !var.deltas_x.allocate_from_pool (pool, ਬਿੰਦੂ_ਗਿਣਤੀ, ਝੂਠ)))
          ਵਾਪਸ ਝੂਠ;

        ਜੇ (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, ਬਿੰਦੂ_ਗਿਣਤੀ, ਝੂਠ)))
          ਵਾਪਸ ਝੂਠ;

        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_deltas; i++)
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ idx = apply_ਮੰਜ਼ਿਲ_all ? i : indices[i];
          ਜੇ (idx >= ਬਿੰਦੂ_ਗਿਣਤੀ) ਜਾਰੀ;
          var.indices[idx] = ਸੱਚ;
          var.deltas_x[idx] = deltas_x[i];
          ਜੇ (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } ਜਦੋਂ (iterator.move_ਮੰਜ਼ਿਲ_ਅਗਲਾ ());

      is_ਸੰਯੁਕਤ = is_ਸੰਯੁਕਤ_glyph;
      ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ create_from_ਵਸਤੂ_var_ਅੰਕੜੇ (ਅਚਲ Var_ਅੰਕੜੇ &var_ਅੰਕੜੇ,
                                    ਅਚਲ hb_ਸਦਿਸ਼_t<hb_hashmap_t<hb_ਟੈਗ_t, Triple>>& regions,
                                    ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ& ਵਸਤੂ_ਗਿਣਤੀ_2,
                                    ਅਚਲ hb_inc_bimap_t* inner_map = ਖਾਲੀ_ਸੂਚਕ)
    {
      /* NULL offset, to keep original varidx valid, just return */
      ਜੇ (&var_ਅੰਕੜੇ == &Null (Var_ਅੰਕੜੇ))
        ਵਾਪਸ ਸੱਚ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_regions = var_ਅੰਕੜੇ.get_region_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ ();
      ਜੇ (!tuple_vars.alloc (ਅੰਕ_regions)) ਵਾਪਸ ਝੂਠ;

      ਵਸਤੂ_ਗਿਣਤੀ_2 = inner_map ? inner_map->get_population () : var_ਅੰਕੜੇ.get_ਵਸਤੂ_ਗਿਣਤੀ ();
      ਜੇ (!ਵਸਤੂ_ਗਿਣਤੀ_2) ਵਾਪਸ ਸੱਚ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_ਮਾਪ = var_ਅੰਕੜੇ.get_ਪੰਕਤੀ_ਮਾਪ ();
      ਅਚਲ HBUINT8 *delta_ਬਾਈਟਾਂ = var_ਅੰਕੜੇ.get_delta_ਬਾਈਟਾਂ ();

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < ਅੰਕ_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        ਜੇ (!tuple.deltas_x.resize_dirty  (ਵਸਤੂ_ਗਿਣਤੀ_2) ||
            !tuple.indices.resize_dirty  (ਵਸਤੂ_ਗਿਣਤੀ_2))
          ਵਾਪਸ ਝੂਠ;

        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਵਸਤੂ_ਗਿਣਤੀ_2; i++)
        {
          tuple.indices.arrayZ[i] = ਸੱਚ;
          tuple.deltas_x.arrayZ[i] = var_ਅੰਕੜੇ.get_ਵਸਤੂ_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_ਬਾਈਟਾਂ, ਪੰਕਤੀ_ਮਾਪ);
        }

        ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਸੂਚਕਾਂਕ = var_ਅੰਕੜੇ.get_region_ਸੂਚਕਾਂਕ (r);
        ਜੇ (region_ਸੂਚਕਾਂਕ >= regions.ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;
        tuple.axis_tuples = regions.arrayZ[region_ਸੂਚਕਾਂਕ];

        tuple_vars.push (std::move (tuple));
      }
      ਵਾਪਸ !tuple_vars.ਵਿੱਚ_error ();
    }

    ਨਿਜੀ:
    ਸਥਿਰ ਪੂਰਨ cmp_axis_ਟੈਗ (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
    {
      ਅਚਲ hb_ਟੈਗ_t *a = (ਅਚਲ hb_ਟੈਗ_t*) pa;
      ਅਚਲ hb_ਟੈਗ_t *b = (ਅਚਲ hb_ਟੈਗ_t*) pb;
      ਵਾਪਸ (ਪੂਰਨ)(*a) - (ਪੂਰਨ)(*b);
    }

    ਤਾਰਕਿਕ change_tuple_variations_axis_limits (ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>& normalized_axes_location,
                                              ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> axis_ਟੈਗ_ਸੂਚੀ;
      ਜੇ (!axis_ਟੈਗ_ਸੂਚੀ.alloc (normalized_axes_location.get_population ()))
        ਵਾਪਸ ਝੂਠ;
      ਲਈ (ਸਵੈਚਾਲਿਤ t : normalized_axes_location.ਕੁੰਜੀਆਂ ())
        axis_ਟੈਗ_ਸੂਚੀ.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_ਨਤੀਜਾ_scratch_t scratch;
      hb_ਸਦਿਸ਼_t<tuple_delta_t> ਬਾਹਰਲਾ;

      axis_ਟੈਗ_ਸੂਚੀ.qsort (cmp_axis_ਟੈਗ);
      ਲਈ (ਸਵੈਚਾਲਿਤ axis_ਟੈਗ_2 : axis_ਟੈਗ_ਸੂਚੀ)
      {
        Triple *axis_ਹੱਦਬੰਦੀ;
        ਜੇ (!normalized_axes_location.has (axis_ਟੈਗ_2, &axis_ਹੱਦਬੰਦੀ))
          ਵਾਪਸ ਝੂਠ;
        TripleDistances axis_triple_distances{1.f, 1.f};
        ਜੇ (axes_triple_distances.has (axis_ਟੈਗ_2))
          axis_triple_distances = axes_triple_distances.get (axis_ਟੈਗ_2);

        hb_ਸਦਿਸ਼_t<tuple_delta_t> new_vars;
        ਲਈ (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_ਹੱਦਬੰਦੀ (axis_ਟੈਗ_2, *axis_ਹੱਦਬੰਦੀ, axis_triple_distances, ਬਾਹਰਲਾ, scratch, pool);
          ਜੇ (!ਬਾਹਰਲਾ) ਜਾਰੀ;

          ਬਿਨਾਨਿਸ਼ਾਨੀ new_len = new_vars.ਲੰਮਾਈ + ਬਾਹਰਲਾ.ਲੰਮਾਈ;

          ਜੇ (unlikely (!new_vars.alloc (new_len, ਝੂਠ)))
            ਵਾਪਸ ਝੂਠ;

          ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਾਹਰਲਾ.ਲੰਮਾਈ; i++)
            new_vars.push (std::move (ਬਾਹਰਲਾ[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      ਵਾਪਸ ਸੱਚ;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    ਤਾਰਕਿਕ ਮਿਲਾਓ_tuple_variations (contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t* contour_points = ਖਾਲੀ_ਸੂਚਕ)
    {
      hb_ਸਦਿਸ਼_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      ਜੇ (unlikely (!new_vars.alloc (tuple_vars.ਲੰਮਾਈ)))
	ਵਾਪਸ ਝੂਠ;
      hb_hashmap_t<ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>*, ਬਿਨਾਨਿਸ਼ਾਨੀ> m;
      ਲਈ (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        ਜੇ (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          ਜੇ (contour_points && !contour_points->ਜੋੜੋ_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            ਵਾਪਸ ਝੂਠ;
          ਜਾਰੀ;
        }

        ਬਿਨਾਨਿਸ਼ਾਨੀ *idx;
        ਜੇ (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        ਨਹੀਂਤਾਂ
        {
	  ਸਵੈਚਾਲਿਤ *new_var = new_vars.push ();
	  ਜੇ (unlikely (new_vars.ਵਿੱਚ_error ()))
	    ਵਾਪਸ ਝੂਠ;
          hb_ਅਦਲਾ_ਬਦਲੀ (*new_var, var);
          ਜੇ (unlikely (!m.ਸੈੱਟ (&(new_var->axis_tuples), new_vars.ਲੰਮਾਈ - 1)))
            ਵਾਪਸ ਝੂਠ;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      ਵਾਪਸ ਸੱਚ;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    ਤਾਰਕਿਕ compile_all_ਬਿੰਦੂ_sets ()
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& tuple: tuple_vars)
      {
        ਅਚਲ hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ>* points_ਸੈੱਟ = &(tuple.indices);
        ਜੇ (ਬਿੰਦੂ_ਅੰਕੜੇ_map.has (points_ਸੈੱਟ))
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ *ਗਿਣਤੀ_2;
          ਜੇ (unlikely (!ਬਿੰਦੂ_ਸੈੱਟ_ਗਿਣਤੀ_map.has (points_ਸੈੱਟ, &ਗਿਣਤੀ_2) ||
                        !ਬਿੰਦੂ_ਸੈੱਟ_ਗਿਣਤੀ_map.ਸੈੱਟ (points_ਸੈੱਟ, (*ਗਿਣਤੀ_2) + 1)))
            ਵਾਪਸ ਝੂਠ;
          ਜਾਰੀ;
        }

        hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> compiled_ਬਿੰਦੂ_ਅੰਕੜੇ;
        ਜੇ (!tuple_delta_t::compile_ਬਿੰਦੂ_ਸੈੱਟ (*points_ਸੈੱਟ, compiled_ਬਿੰਦੂ_ਅੰਕੜੇ))
          ਵਾਪਸ ਝੂਠ;

        ਜੇ (!ਬਿੰਦੂ_ਅੰਕੜੇ_map.ਸੈੱਟ (points_ਸੈੱਟ, std::move (compiled_ਬਿੰਦੂ_ਅੰਕੜੇ)) ||
            !ਬਿੰਦੂ_ਸੈੱਟ_ਗਿਣਤੀ_map.ਸੈੱਟ (points_ਸੈੱਟ, 1))
          ਵਾਪਸ ਝੂਠ;
      }
      ਵਾਪਸ ਸੱਚ;
    }

    /* find shared points set which saves most bytes */
    ਖਾਲੀ ਲੱਭੋ_shared_points ()
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ max_saved_ਬਾਈਟਾਂ = 0;

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : ਬਿੰਦੂ_ਅੰਕੜੇ_map.iter_ref ())
      {
        ਅਚਲ hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ>* points_ਸੈੱਟ = _.ਪਹਿਲਾ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਲੰਮਾਈ = _.second.ਲੰਮਾਈ;
        ਜੇ (!ਅੰਕੜੇ_ਲੰਮਾਈ) ਜਾਰੀ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ *ਗਿਣਤੀ_2;
        ਜੇ (unlikely (!ਬਿੰਦੂ_ਸੈੱਟ_ਗਿਣਤੀ_map.has (points_ਸੈੱਟ, &ਗਿਣਤੀ_2) ||
                      *ਗਿਣਤੀ_2 <= 1))
        {
          shared_points_ਬਾਈਟਾਂ = ਖਾਲੀ_ਸੂਚਕ;
          ਵਾਪਸ;
        }

        ਬਿਨਾਨਿਸ਼ਾਨੀ saved_ਬਾਈਟਾਂ = ਅੰਕੜੇ_ਲੰਮਾਈ * ((*ਗਿਣਤੀ_2) -1);
        ਜੇ (saved_ਬਾਈਟਾਂ > max_saved_ਬਾਈਟਾਂ)
        {
          max_saved_ਬਾਈਟਾਂ = saved_ਬਾਈਟਾਂ;
          shared_points_ਬਾਈਟਾਂ = &(_.second);
        }
      }
    }

    ਤਾਰਕਿਕ calc_inferred_deltas (ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t& contour_points,
			       hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &scratch)
    {
      ਲਈ (tuple_delta_t& var : tuple_vars)
        ਜੇ (!var.calc_inferred_deltas (contour_points, scratch))
          ਵਾਪਸ ਝੂਠ;

      ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ iup_optimize (ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      ਲਈ (tuple_delta_t& var : tuple_vars)
      {
        ਜੇ (!var.optimize (contour_points, is_ਸੰਯੁਕਤ, scratch))
          ਵਾਪਸ ਝੂਠ;
      }
      ਵਾਪਸ ਸੱਚ;
    }

    ਜਨਤਕ:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    ਖਾਲੀ cull_unreachable (ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> &reachable_ranges)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < tuple_vars.ਲੰਮਾਈ; i++)
      {
	ਜੇ (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  ਜਾਰੀ;
	ਜੇ (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    ਤਾਰਕਿਕ instantiate (ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>& normalized_axes_location,
                      ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ,
                      contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t* contour_points = ਖਾਲੀ_ਸੂਚਕ,
                      ਤਾਰਕਿਕ optimize = ਝੂਠ)
    {
      ਜੇ (!tuple_vars) ਵਾਪਸ ਸੱਚ;
      ਜੇ (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        ਵਾਪਸ ਝੂਠ;
      /* compute inferred deltas only for gvar */
      ਜੇ (contour_points)
      {
	hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> scratch;
	ਜੇ (!calc_inferred_deltas (*contour_points, scratch))
	  ਵਾਪਸ ਝੂਠ;
      }

      /* if iup delta opt is on, contour_points can't be null */
      ਜੇ (optimize && !contour_points)
        ਵਾਪਸ ਝੂਠ;

      ਜੇ (!ਮਿਲਾਓ_tuple_variations (optimize ? contour_points : ਖਾਲੀ_ਸੂਚਕ))
        ਵਾਪਸ ਝੂਠ;

      ਜੇ (optimize && !iup_optimize (*contour_points, scratch)) ਵਾਪਸ ਝੂਠ;
      ਵਾਪਸ !tuple_vars.ਵਿੱਚ_error ();
    }

    ਤਾਰਕਿਕ compile_ਬਾਈਟਾਂ (ਅਚਲ hb_map_t& axes_ਸੂਚਕਾਂਕ_map,
                        ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                        ਤਾਰਕਿਕ use_shared_points,
                        ਤਾਰਕਿਕ is_gvar = ਝੂਠ,
                        ਅਚਲ hb_hashmap_t<ਅਚਲ hb_ਸਦਿਸ਼_t<F2DOT14>*, ਬਿਨਾਨਿਸ਼ਾਨੀ>* shared_tuples_idx_map = ਖਾਲੀ_ਸੂਚਕ,
			hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ)
    {
      // return true for empty glyph
      ਜੇ (!tuple_vars)
        ਵਾਪਸ ਸੱਚ;

      // compile points set and store data in hashmap
      ਜੇ (!compile_all_ਬਿੰਦੂ_sets ())
        ਵਾਪਸ ਝੂਠ;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_ਮਾਪ += 4;

      ਜੇ (use_shared_points)
      {
        ਲੱਭੋ_shared_points ();
        ਜੇ (shared_points_ਬਾਈਟਾਂ)
          compiled_byte_ਮਾਪ += shared_points_ਬਾਈਟਾਂ->ਲੰਮਾਈ;
      }
      hb_ਸਦਿਸ਼_t<ਪੂਰਨ> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      ਲਈ (ਸਵੈਚਾਲਿਤ& tuple: tuple_vars)
      {
        ਅਚਲ hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ>* points_ਸੈੱਟ = &(tuple.indices);
        hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> *points_ਅੰਕੜੇ;
        ਜੇ (unlikely (!ਬਿੰਦੂ_ਅੰਕੜੇ_map.has (points_ਸੈੱਟ, &points_ਅੰਕੜੇ)))
          ਵਾਪਸ ਝੂਠ;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        ਜੇ (!points_ਅੰਕੜੇ->ਲੰਮਾਈ)
          ਜਾਰੀ;
        ਜੇ (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          ਵਾਪਸ ਝੂਠ;

        ਬਿਨਾਨਿਸ਼ਾਨੀ points_ਅੰਕੜੇ_ਲੰਮਾਈ = (points_ਅੰਕੜੇ != shared_points_ਬਾਈਟਾਂ) ? points_ਅੰਕੜੇ->ਲੰਮਾਈ : 0;
        ਜੇ (!tuple.compile_tuple_var_ਸਿਰਲੇਖ (axes_ਸੂਚਕਾਂਕ_map, points_ਅੰਕੜੇ_ਲੰਮਾਈ, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                             shared_tuples_idx_map,
					     pool))
          ਵਾਪਸ ਝੂਠ;
        compiled_byte_ਮਾਪ += tuple.compiled_tuple_ਸਿਰਲੇਖ.ਲੰਮਾਈ + points_ਅੰਕੜੇ_ਲੰਮਾਈ + tuple.compiled_deltas.ਲੰਮਾਈ;
      }

      ਜੇ (is_gvar && (compiled_byte_ਮਾਪ % 2))
      {
        needs_padding = ਸੱਚ;
        compiled_byte_ਮਾਪ += 1;
      }

      ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ serialize_var_ਸਿਰਲੇਖਾਂ (hb_serialize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ& ਕੁੱਲ_ਸਿਰਲੇਖ_len) ਅਚਲ
    {
      TRACE_SERIALIZE (ਇਹ);
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& tuple: tuple_vars)
      {
        tuple.compiled_tuple_ਸਿਰਲੇਖ.as_array ().copy (c);
        ਜੇ (c->ਵਿੱਚ_error ()) return_trace (ਝੂਠ);
        ਕੁੱਲ_ਸਿਰਲੇਖ_len += tuple.compiled_tuple_ਸਿਰਲੇਖ.ਲੰਮਾਈ;
      }
      return_trace (ਸੱਚ);
    }

    ਤਾਰਕਿਕ serialize_var_ਅੰਕੜੇ (hb_serialize_context_t *c, ਤਾਰਕਿਕ is_gvar) ਅਚਲ
    {
      TRACE_SERIALIZE (ਇਹ);
      ਜੇ (is_gvar && shared_points_ਬਾਈਟਾਂ)
      {
        hb_ubytes_t s (shared_points_ਬਾਈਟਾਂ->arrayZ, shared_points_ਬਾਈਟਾਂ->ਲੰਮਾਈ);
        s.copy (c);
      }

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& tuple: tuple_vars)
      {
        ਅਚਲ hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ>* points_ਸੈੱਟ = &(tuple.indices);
        hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> *ਬਿੰਦੂ_ਅੰਕੜੇ;
        ਜੇ (!ਬਿੰਦੂ_ਅੰਕੜੇ_map.has (points_ਸੈੱਟ, &ਬਿੰਦੂ_ਅੰਕੜੇ))
          return_trace (ਝੂਠ);

        ਜੇ (!is_gvar || ਬਿੰਦੂ_ਅੰਕੜੇ != shared_points_ਬਾਈਟਾਂ)
        {
          hb_ubytes_t s (ਬਿੰਦੂ_ਅੰਕੜੇ->arrayZ, ਬਿੰਦੂ_ਅੰਕੜੇ->ਲੰਮਾਈ);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        ਜੇ (c->ਵਿੱਚ_error ()) return_trace (ਝੂਠ);
      }

      /* padding for gvar */
      ਜੇ (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        ਜੇ (!c->embed (pad)) return_trace (ਝੂਠ);
      }
      return_trace (ਸੱਚ);
    }
  };

  ਬਣਤਰ tuple_iterator_t
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ get_axis_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ axis_ਗਿਣਤੀ; }

    ਖਾਲੀ init (hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਗਿਣਤੀ_3, ਅਚਲ ਖਾਲੀ *ਸਾਰਣੀ_ਅਧਾਰ)
    {
      var_ਅੰਕੜੇ_ਬਾਈਟਾਂ = var_ਅੰਕੜੇ_ਬਾਈਟਾਂ_2;
      var_ਅੰਕੜੇ = var_ਅੰਕੜੇ_ਬਾਈਟਾਂ_2.as<Tuple_Variation_ਅੰਕੜੇ> ();
      tuples_ਖੱਬਾ = var_ਅੰਕੜੇ->tuple_Var_ਗਿਣਤੀ.get_ਗਿਣਤੀ ();
      axis_ਗਿਣਤੀ = axis_ਗਿਣਤੀ_3;
      axis_ਗਿਣਤੀ_times_2 = axis_ਗਿਣਤੀ_3 * 2;
      current_tuple = &var_ਅੰਕੜੇ->get_tuple_var_ਸਿਰਲੇਖ ();
      ਅੰਕੜੇ_ਵਿੱਥ_2 = 0;
      ਸਾਰਣੀ_ਅਧਾਰ_2 = ਸਾਰਣੀ_ਅਧਾਰ;
    }

    ਤਾਰਕਿਕ get_shared_indices (hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> &shared_indices /*ਬਾਹਰਲਾ*/)
    {
      ਜੇ (var_ਅੰਕੜੇ->has_shared_ਬਿੰਦੂ_numbers ())
      {
        ਅਚਲ HBUINT8 *ਅਧਾਰ = &(ਸਾਰਣੀ_ਅਧਾਰ_2+var_ਅੰਕੜੇ->ਅੰਕੜੇ);
        ਅਚਲ HBUINT8 *p = ਅਧਾਰ;
        ਜੇ (!decompile_points (p, shared_indices, (ਅਚਲ HBUINT8 *) (var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.arrayZ + var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.ਲੰਮਾਈ))) ਵਾਪਸ ਝੂਠ;
        ਅੰਕੜੇ_ਵਿੱਥ_2 = p - ਅਧਾਰ;
      }
      ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ is_valid ()
    {
      ਜੇ (unlikely (tuples_ਖੱਬਾ <= 0))
	ਵਾਪਸ ਝੂਠ;

      current_tuple_ਮਾਪ = Tuple_Variation_ਸਿਰਲੇਖ::min_ਮਾਪ;
      ਜੇ (unlikely (!var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.check_ਸਿਰਾ ((ਅਚਲ ਅੱਖਰ *) current_tuple + current_tuple_ਮਾਪ)))
	ਵਾਪਸ ਝੂਠ;

      current_tuple_ਮਾਪ = current_tuple->get_ਮਾਪ (axis_ਗਿਣਤੀ_times_2);
      ਜੇ (unlikely (!var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.check_ਸਿਰਾ ((ਅਚਲ ਅੱਖਰ *) current_tuple + current_tuple_ਮਾਪ)))
	ਵਾਪਸ ਝੂਠ;

      ਵਾਪਸ ਸੱਚ;
    }

    HB_ALWAYS_INLINE
    ਤਾਰਕਿਕ move_ਮੰਜ਼ਿਲ_ਅਗਲਾ ()
    {
      ਅੰਕੜੇ_ਵਿੱਥ_2 += current_tuple->get_ਅੰਕੜੇ_ਮਾਪ ();
      current_tuple = &Struct_At_ਵਿੱਥ<Tuple_Variation_ਸਿਰਲੇਖ> (current_tuple, current_tuple_ਮਾਪ);
      tuples_ਖੱਬਾ--;
      ਵਾਪਸ is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ਅਚਲ HBUINT8 *get_serialized_ਅੰਕੜੇ () ਅਚਲ
    { ਵਾਪਸ &(ਸਾਰਣੀ_ਅਧਾਰ_2+var_ਅੰਕੜੇ->ਅੰਕੜੇ) + ਅੰਕੜੇ_ਵਿੱਥ_2; }

    ਨਿਜੀ:
    ਨਿਸ਼ਾਨੀਵਾਲਾ tuples_ਖੱਬਾ;
    ਅਚਲ Tuple_Variation_ਅੰਕੜੇ *var_ਅੰਕੜੇ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਗਿਣਤੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਗਿਣਤੀ_times_2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕੜੇ_ਵਿੱਥ_2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ current_tuple_ਮਾਪ;
    ਅਚਲ ਖਾਲੀ *ਸਾਰਣੀ_ਅਧਾਰ_2;

    ਜਨਤਕ:
    hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ;
    ਅਚਲ Tuple_Variation_ਸਿਰਲੇਖ *current_tuple;
  };

  ਸਥਿਰ ਤਾਰਕਿਕ get_tuple_iterator (hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
                                  ਅਚਲ ਖਾਲੀ *ਸਾਰਣੀ_ਅਧਾਰ_2,
                                  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> &shared_indices /*ਬਾਹਰਲਾ*/,
                                  tuple_iterator_t *iterator /*ਬਾਹਰਲਾ*/)
  {
    iterator->init (var_ਅੰਕੜੇ_ਬਾਈਟਾਂ, axis_ਗਿਣਤੀ, ਸਾਰਣੀ_ਅਧਾਰ_2);
    ਜੇ (!iterator->get_shared_indices (shared_indices))
      ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ iterator->is_valid ();
  }

  ਤਾਰਕਿਕ has_shared_ਬਿੰਦੂ_numbers () ਅਚਲ { ਵਾਪਸ tuple_Var_ਗਿਣਤੀ.has_shared_ਬਿੰਦੂ_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  ਤਾਰਕਿਕ cull_tuple_variations (hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
			      hb_array_t<ਅਚਲ F2DOT14> shared_tuples,
			      ਅਚਲ hb_map_t *axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
			      ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> &reachable_ranges,
			      hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &ਬਾਹਰਲਾ /*ਬਾਹਰਲਾ*/,
			      ਤਾਰਕਿਕ *ਬਦਲਿਆ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    *ਬਦਲਿਆ = ਝੂਠ;

    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> shared_indices;
    tuple_iterator_t iterator;
    ਜੇ (!get_tuple_iterator (var_ਅੰਕੜੇ_ਬਾਈਟਾਂ, axis_ਗਿਣਤੀ, ਇਹ,
			     shared_indices, &iterator))
      ਵਾਪਸ ਸੱਚ; /* no tuples or malformed: keep original */

    ਅਚਲ ਅੱਖਰ *ਬਾਈਟਾਂ_ਸ਼ੁਰੂ = var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.arrayZ;
    ਅਚਲ ਅੱਖਰ *ਬਾਈਟਾਂ_ਸਿਰਾ = ਬਾਈਟਾਂ_ਸ਼ੁਰੂ + var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.ਲੰਮਾਈ;
    ਅਚਲ ਅੱਖਰ *serialized_ਅਧਾਰ = (ਅਚਲ ਅੱਖਰ *) &(ਇਹ+ਅੰਕੜੇ);
    ਅਚਲ ਅੱਖਰ *shared_points_ਸਿਰਾ = (ਅਚਲ ਅੱਖਰ *) iterator.get_serialized_ਅੰਕੜੇ ();
    ਜੇ (unlikely (serialized_ਅਧਾਰ < ਬਾਈਟਾਂ_ਸ਼ੁਰੂ || serialized_ਅਧਾਰ > ਬਾਈਟਾਂ_ਸਿਰਾ ||
		  shared_points_ਸਿਰਾ < serialized_ਅਧਾਰ || shared_points_ਸਿਰਾ > ਬਾਈਟਾਂ_ਸਿਰਾ))
      ਵਾਪਸ ਸੱਚ;

    ਬਣਤਰ kept_tuple_t
    {
      ਅਚਲ ਅੱਖਰ *ਸਿਰਲੇਖ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਲੇਖ_ਮਾਪ;
      ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ;
    };
    hb_ਸਦਿਸ਼_t<kept_tuple_t> kept;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ kept_ਸਿਰਲੇਖਾਂ_ਮਾਪ = 0, kept_ਅੰਕੜੇ_ਮਾਪ = 0;
    ਕਰੋ
    {
      ਅਚਲ Tuple_Variation_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ = iterator.current_tuple;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਲੇਖ_ਮਾਪ = ਸਿਰਲੇਖ->get_ਮਾਪ (axis_ਗਿਣਤੀ * 2);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ = ਸਿਰਲੇਖ->get_ਅੰਕੜੇ_ਮਾਪ ();
      ਅਚਲ ਅੱਖਰ *tuple_ਅੰਕੜੇ = (ਅਚਲ ਅੱਖਰ *) iterator.get_serialized_ਅੰਕੜੇ ();
      ਜੇ (unlikely (tuple_ਅੰਕੜੇ < serialized_ਅਧਾਰ ||
		    ਅੰਕੜੇ_ਮਾਪ > (ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਬਾਈਟਾਂ_ਸਿਰਾ - tuple_ਅੰਕੜੇ)))
	ਵਾਪਸ ਸੱਚ; /* malformed: keep original */

      hb_hashmap_t<hb_ਟੈਗ_t, Triple> axis_tuples;
      ਜੇ (!ਸਿਰਲੇਖ->unpack_axis_tuples (axis_ਗਿਣਤੀ, shared_tuples,
				       axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, axis_tuples))
	ਵਾਪਸ ਸੱਚ;

      ਕੁੱਲ++;
      ਜੇ (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ਅਚਲ ਅੱਖਰ *) ਸਿਰਲੇਖ, ਸਿਰਲੇਖ_ਮਾਪ,
				 tuple_ਅੰਕੜੇ, ਅੰਕੜੇ_ਮਾਪ});
	kept_ਸਿਰਲੇਖਾਂ_ਮਾਪ += ਸਿਰਲੇਖ_ਮਾਪ;
	kept_ਅੰਕੜੇ_ਮਾਪ += ਅੰਕੜੇ_ਮਾਪ;
      }
    } ਜਦੋਂ (iterator.move_ਮੰਜ਼ਿਲ_ਅਗਲਾ ());

    ਜੇ (unlikely (kept.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
    ਜੇ (kept.ਲੰਮਾਈ == ਕੁੱਲ) ਵਾਪਸ ਸੱਚ; /* nothing to cull */

    *ਬਦਲਿਆ = ਸੱਚ;
    ਜੇ (!kept.ਲੰਮਾਈ) ਵਾਪਸ ਸੱਚ; /* everything culled: no variation data */

    ਬਿਨਾਨਿਸ਼ਾਨੀ shared_points_ਮਾਪ = shared_points_ਸਿਰਾ - serialized_ਅਧਾਰ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਵਿੱਥ_2 = min_ਮਾਪ + kept_ਸਿਰਲੇਖਾਂ_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਮਾਪ = ਅੰਕੜੇ_ਵਿੱਥ_2 + shared_points_ਮਾਪ + kept_ਅੰਕੜੇ_ਮਾਪ;
    ਜੇ (unlikely (!ਬਾਹਰਲਾ.resize (new_ਮਾਪ)))
      ਵਾਪਸ ਝੂਠ;

    Tuple_Variation_ਅੰਕੜੇ *ਬਾਹਰਲਾ_ਅੰਕੜੇ = (Tuple_Variation_ਅੰਕੜੇ *) ਬਾਹਰਲਾ.arrayZ;
    ਬਾਹਰਲਾ_ਅੰਕੜੇ->tuple_Var_ਗਿਣਤੀ = (uint16_t) (kept.ਲੰਮਾਈ |
					  (has_shared_ਬਿੰਦੂ_numbers () ? 0x8000u : 0u));
    ਬਾਹਰਲਾ_ਅੰਕੜੇ->ਅੰਕੜੇ = ਅੰਕੜੇ_ਵਿੱਥ_2;

    ਅੱਖਰ *p = ਬਾਹਰਲਾ.arrayZ + min_ਮਾਪ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &t : kept)
    {
      hb_memcpy (p, t.ਸਿਰਲੇਖ, t.ਸਿਰਲੇਖ_ਮਾਪ);
      p += t.ਸਿਰਲੇਖ_ਮਾਪ;
    }
    hb_memcpy (p, serialized_ਅਧਾਰ, shared_points_ਮਾਪ);
    p += shared_points_ਮਾਪ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &t : kept)
    {
      hb_memcpy (p, t.ਅੰਕੜੇ, t.ਅੰਕੜੇ_ਮਾਪ);
      p += t.ਅੰਕੜੇ_ਮਾਪ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ decompile_points (ਅਚਲ HBUINT8 *&p /* IN/OUT */,
				hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> &points /*ਬਾਹਰਲਾ*/,
				ਅਚਲ HBUINT8 *ਸਿਰਾ)
  {
    ਗਿਣਤੀ packed_ਬਿੰਦੂ_flag_t
    {
      POINTS_ARE_ਸ਼ਬਦ     = 0x80,
      ਬਿੰਦੂ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ = 0x7F
    };

    ਜੇ (unlikely (p + 1 > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = *p++;
    ਜੇ (ਗਿਣਤੀ_2 & POINTS_ARE_ਸ਼ਬਦ)
    {
      ਜੇ (unlikely (p + 1 > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
      ਗਿਣਤੀ_2 = ((ਗਿਣਤੀ_2 & ਬਿੰਦੂ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ) << 8) | *p++;
    }
    ਜੇ (unlikely (!points.resize_dirty  (ਗਿਣਤੀ_2))) ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ n = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਜਦੋਂ (i < ਗਿਣਤੀ_2)
    {
      ਜੇ (unlikely (p + 1 > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ control = *p++;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਗਿਣਤੀ = (control & ਬਿੰਦੂ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ) + 1;
      ਬਿਨਾਨਿਸ਼ਾਨੀ stop = i + ਲੜੀਵਾਰ_ਗਿਣਤੀ;
      ਜੇ (unlikely (stop > ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;
      ਜੇ (control & POINTS_ARE_ਸ਼ਬਦ)
      {
        ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ * HBUINT16::static_ਮਾਪ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
        ਲਈ (; i < stop; i++)
        {
          n += *(ਅਚਲ HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_ਮਾਪ;
        }
      }
      ਨਹੀਂਤਾਂ
      {
        ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
        ਲਈ (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  HB_ALWAYS_INLINE
  ਸਥਿਰ ਤਾਰਕਿਕ decompile_deltas (ਅਚਲ HBUINT8 *&p /* IN/OUT */,
				hb_ਸਦਿਸ਼_t<T> &deltas /* IN/OUT */,
				ਅਚਲ HBUINT8 *ਸਿਰਾ,
				ਤਾਰਕਿਕ consume_all = ਝੂਠ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0)
  {
    ਵਾਪਸ Tuple_ਮੁੱਲਾਂ::decompile (p, deltas, ਸਿਰਾ, consume_all, ਸ਼ੁਰੂ);
  }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ tuple_Var_ਗਿਣਤੀ; }

  ਤਾਰਕਿਕ decompile_tuple_variations (ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਿੰਦੂ_ਗਿਣਤੀ,
                                   ਤਾਰਕਿਕ is_gvar,
                                   tuple_iterator_t iterator,
                                   ਅਚਲ hb_map_t *axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                   ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &shared_indices,
                                   ਅਚਲ hb_array_t<ਅਚਲ F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /*ਬਾਹਰਲਾ*/
				   hb_alloc_pool_t *pool = ਖਾਲੀ_ਸੂਚਕ,
                                   ਤਾਰਕਿਕ is_ਸੰਯੁਕਤ_glyph = ਝੂਠ) ਅਚਲ
  {
    ਵਾਪਸ tuple_variations.create_from_tuple_var_ਅੰਕੜੇ (iterator, tuple_Var_ਗਿਣਤੀ,
                                                        ਬਿੰਦੂ_ਗਿਣਤੀ, is_gvar,
                                                        axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_ਸੰਯੁਕਤ_glyph);
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  ਤਾਰਕਿਕ is_gvar,
                  ਅਚਲ tuple_variations_t& tuple_variations) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    /* empty tuple variations, just return and skip serialization. */
    ਜੇ (!tuple_variations) return_trace (ਸੱਚ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਜੇ (!c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->tuple_Var_ਗਿਣਤੀ, tuple_variations.get_var_ਗਿਣਤੀ (),
                          HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਸਿਰਲੇਖ_len = 0;

    ਜੇ (!tuple_variations.serialize_var_ਸਿਰਲੇਖਾਂ (c, ਕੁੱਲ_ਸਿਰਲੇਖ_len))
      return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਵਿੱਥ_2 = min_ਮਾਪ + ਕੁੱਲ_ਸਿਰਲੇਖ_len;
    ਜੇ (!is_gvar) ਅੰਕੜੇ_ਵਿੱਥ_2 += 4;
    ਜੇ (!c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->ਅੰਕੜੇ, ਅੰਕੜੇ_ਵਿੱਥ_2, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)) return_trace (ਝੂਠ);

    ਵਾਪਸ tuple_variations.serialize_var_ਅੰਕੜੇ (c, is_gvar);
  }

  ਸੁਰੱਖਿਅਤ:
  ਬਣਤਰ Tuple_Var_ਗਿਣਤੀ : HBUINT16
  {
    ਮਿੱਤਰ ਬਣਤਰ tuple_variations_t;
    ਤਾਰਕਿਕ has_shared_ਬਿੰਦੂ_numbers () ਅਚਲ { ਵਾਪਸ ((*ਇਹ) & Shared_ਬਿੰਦੂ_Numbers); }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ (*ਇਹ) & ਗਿਣਤੀ_ਨਕਾਬ; }
    Tuple_Var_ਗਿਣਤੀ& ਸੰਚਾਲਕ = (uint16_t i) { HBUINT16::ਸੰਚਾਲਕ= (i); ਵਾਪਸ *ਇਹ; }
    ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ get_ਗਿਣਤੀ (); }

    ਸੁਰੱਖਿਅਤ:
    ਗਿਣਤੀ Flags
    {
      Shared_ਬਿੰਦੂ_Numbers= 0x8000u,
      ਗਿਣਤੀ_ਨਕਾਬ         = 0x0FFFu
    };
    ਜਨਤਕ:
    DEFINE_ਮਾਪ_STATIC (2);
  };

  Tuple_Var_ਗਿਣਤੀ tuple_Var_ਗਿਣਤੀ;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  ਵਿੱਥ_ਮੰਜ਼ਿਲ<HBUINT8, OffType>
                ਅੰਕੜੇ;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (2 + OffType::static_ਮਾਪ);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
ਵਰਤੋਂ tuple_variations_t = Tuple_Variation_ਅੰਕੜੇ<HBUINT16>::tuple_variations_t;
ਬਣਤਰ ਵਸਤੂ_variations_t
{
  ਵਰਤੋਂ region_t = ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>*;
  ਨਿਜੀ:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_ਸਦਿਸ਼_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_ਸਦਿਸ਼_t<hb_hashmap_t<hb_ਟੈਗ_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_ਸਦਿਸ਼_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> region_map;

  /* all delta rows after instantiation */
  hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਪੂਰਨ>> delta_ਪੰਕਤੀਆਂ;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_ਸਦਿਸ਼_t<delta_ਪੰਕਤੀ_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  ਤਾਰਕਿਕ has_long = ਝੂਠ;

  ਜਨਤਕ:
  ਤਾਰਕਿਕ has_long_ਸ਼ਬਦ () ਅਚਲ
  { ਵਾਪਸ has_long; }

  ਅਚਲ hb_ਸਦਿਸ਼_t<region_t>& get_region_list () ਅਚਲ
  { ਵਾਪਸ region_list; }

  ਅਚਲ hb_ਸਦਿਸ਼_t<delta_ਪੰਕਤੀ_encoding_t>& get_vardata_encodings () ਅਚਲ
  { ਵਾਪਸ encodings; }

  ਅਚਲ hb_map_t& get_varidx_map () ਅਚਲ
  { ਵਾਪਸ varidx_map; }

  ਤਾਰਕਿਕ ਜੋੜੋ_vardata_encoding_for_testing (hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*> &&ਪੰਕਤੀਆਂ,
                                         ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ)
  {
    encodings.push (delta_ਪੰਕਤੀ_encoding_t (std::move (ਪੰਕਤੀਆਂ), ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ));
    ਵਾਪਸ !encodings.ਵਿੱਚ_error ();
  }

  ਤਾਰਕਿਕ compile_varidx_map_for_testing (ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*>& front_mapping)
  { ਵਾਪਸ compile_varidx_map (front_mapping); }

  ਤਾਰਕਿਕ instantiate (ਅਚਲ ਵਸਤੂ_Variation_Store& varStore,
                    ਅਚਲ hb_subset_plan_t *plan,
                    ਤਾਰਕਿਕ optimize=ਸੱਚ,
                    ਤਾਰਕਿਕ use_no_variation_idx=ਸੱਚ,
                    ਅਚਲ hb_array_t <ਅਚਲ hb_inc_bimap_t> inner_maps = hb_array_t<ਅਚਲ hb_inc_bimap_t> (),
                    ਤਾਰਕਿਕ layout_conditions = ਝੂਠ)
  {
    ਜੇ (!create_from_ਵਸਤੂ_varstore (varStore, plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, inner_maps))
      ਵਾਪਸ ਝੂਠ;
    /* avar2 partial instancing: cull unreachable regions. */
    ਜੇ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      ਲਈ (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    ਜੇ (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (layout_conditions &&
        (!instantiate_condition_ਮੁੱਲਾਂ (plan, varStore) || !build_region_list ()))
      ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ as_ਵਸਤੂ_varstore (optimize, use_no_variation_idx);
  }

  ਤਾਰਕਿਕ instantiate_condition_ਮੁੱਲਾਂ (ਅਚਲ hb_subset_plan_t *plan,
                                      ਅਚਲ ਵਸਤੂ_Variation_Store &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    ਸਵੈਚਾਲਿਤ &condition_map = plan->layout_condition_idx_ਮੁੱਲ_map;
    condition_map.clear ();
    ਜੇ (!plan->layout_condition_ਮੁੱਲਾਂ) ਵਾਪਸ ਸੱਚ;
    ਵਰਤੋਂ condition_ਮੁੱਲ_t = hb_pair_t<uintptr_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>>;
    hb_ਸਦਿਸ਼_t<condition_ਮੁੱਲ_t> conditions;
    ਲਈ (ਸਵੈਚਾਲਿਤ condition : plan->layout_condition_ਮੁੱਲਾਂ.iter ())
      conditions.push (condition);
    ਜੇ (unlikely (conditions.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ਅਚਲ condition_ਮੁੱਲ_t &a, ਅਚਲ condition_ਮੁੱਲ_t &b) {
      ਜੇ (a.second.ਪਹਿਲਾ != b.second.ਪਹਿਲਾ)
        ਵਾਪਸ a.second.ਪਹਿਲਾ < b.second.ਪਹਿਲਾ ? -1 : 1;
      ਵਾਪਸ a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_outer = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < plan->gdef_varstore_inner_maps.ਲੰਮਾਈ; i++)
      ਜੇ (plan->gdef_varstore_inner_maps[i].get_population ())
        ਜੇ (!outer_map.ਸੈੱਟ (i, new_outer++)) ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਸਵੈਚਾਲਿਤ condition : conditions)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ old_idx = condition.second.ਪਹਿਲਾ;
      ਦੁੱਗਣਾ value = condition.second.second +
                     varStore.get_delta<ਦੁੱਗਣਾ> (old_idx, plan->normalized_coords.as_array ());
      ਬਿਨਾਨਿਸ਼ਾਨੀ old_outer = old_idx >> 16;
      ਜੇ (!outer_map.has (old_outer))
        ਜਾਰੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ outer = outer_map.get (old_outer);
      ਬਿਨਾਨਿਸ਼ਾਨੀ inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      ਬਿਨਾਨਿਸ਼ਾਨੀ idx = (outer << 16) | inner;
      ਜੇ (outer >= vars.ਲੰਮਾਈ || inner >= var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ[outer])
        ਜਾਰੀ;

      ਦੁੱਗਣਾ ਵੱਧ_ਤੋਂ_ਵੱਧ = fabs ((ਦੁੱਗਣਾ) value);
      ਤਾਰਕਿਕ variable = ਝੂਠ;
      ਲਈ (ਅਚਲ tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        ਜੇ (unlikely (inner >= tuple.deltas_x.ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;
        ਦਸ਼ਮਲਵ delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        ਵੱਧ_ਤੋਂ_ਵੱਧ = hb_max (ਵੱਧ_ਤੋਂ_ਵੱਧ, fabs ((ਦੁੱਗਣਾ) delta));
      }
      ਜੇ (!variable)
      {
        ਜੇ (!condition_map.ਸੈੱਟ (condition.ਪਹਿਲਾ,
                                hb_pair (ਬਿਨਾਨਿਸ਼ਾਨੀ (VarIdx::NO_VARIATION), ਪੂਰਨ (value > 0))))
          ਵਾਪਸ ਝੂਠ;
        ਜਾਰੀ;
      }

      ਦੁੱਗਣਾ scale = 1.;
      ਜਦੋਂ (ਵੱਧ_ਤੋਂ_ਵੱਧ * scale > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      ਜਦੋਂ (ਵੱਧ_ਤੋਂ_ਵੱਧ * scale * 2 <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
      {
        ਤਾਰਕਿਕ integral = (ਦੁੱਗਣਾ) value * scale == round ((ਦੁੱਗਣਾ) value * scale);
        ਲਈ (ਅਚਲ tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (ਦੁੱਗਣਾ) tuple.deltas_x[inner] * scale ==
                      round ((ਦੁੱਗਣਾ) tuple.deltas_x[inner] * scale);
        ਜੇ (integral) ਰੋਕੋ;
        scale *= 2;
      }
      ਪੂਰਨ default_ਮੁੱਲ = (ਪੂਰਨ) round ((ਦੁੱਗਣਾ) value * scale);
      ਜੇ (scale != 1. || default_ਮੁੱਲ < -32768 || default_ਮੁੱਲ > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t ਨਿੱਜੀ_ਪੰਕਤੀ;
        ਲਈ (ਅਚਲ tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          ਜੇ (!tuple.deltas_x[inner]) ਜਾਰੀ;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (ਸੱਚ);
          copy.deltas_x.push ((ਦਸ਼ਮਲਵ) round ((ਦੁੱਗਣਾ) tuple.deltas_x[inner] * scale));
          ਜੇ (unlikely (copy.axis_tuples.ਵਿੱਚ_error () || copy.indices.ਵਿੱਚ_error () ||
                        copy.deltas_x.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
          ਨਿੱਜੀ_ਪੰਕਤੀ.tuple_vars.push (std::move (copy));
        }
        ਜੇ (unlikely (vars.ਲੰਮਾਈ >= 0xFFFFu)) ਵਾਪਸ ਝੂਠ;
        ਜੇ (unlikely (ਨਿੱਜੀ_ਪੰਕਤੀ.tuple_vars.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
        idx = vars.ਲੰਮਾਈ << 16;
        vars.push (std::move (ਨਿੱਜੀ_ਪੰਕਤੀ));
        var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.push (1);
        ਜੇ (unlikely (vars.ਵਿੱਚ_error () || var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
        ਜੇ (default_ਮੁੱਲ < -32768 || default_ਮੁੱਲ > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          ਪੂਰਨ bias = default_ਮੁੱਲ - hb_clamp (default_ਮੁੱਲ, -32768, 32767);
          default_ਮੁੱਲ -= bias;
          ਪੂਰਨ ਉੱਚਾ = bias / 65536 * 65536;
          ਪੂਰਨ parts[] = {ਉੱਚਾ, bias - ਉੱਚਾ};
          ਲਈ (ਪੂਰਨ part : parts)
          {
            ਜੇ (!part) ਜਾਰੀ;
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = vars[idx >> 16].tuple_vars.ਲੰਮਾਈ;
            ਜੋੜੋ_tuple (idx >> 16, hb_hashmap_t<hb_ਟੈਗ_t, Triple> (),
                       0, part, 1);
            ਅਚਲ ਸਵੈਚਾਲਿਤ &tuples = vars[idx >> 16].tuple_vars;
            ਜੇ (unlikely (tuples.ਵਿੱਚ_error () || tuples.ਲੰਮਾਈ != ਗਿਣਤੀ_2 + 1 ||
                          tuples.ਪੂਛ ().indices.ਵਿੱਚ_error () ||
                          tuples.ਪੂਛ ().deltas_x.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
          }
        }
      }
      ਜੇ (!condition_map.ਸੈੱਟ (condition.ਪਹਿਲਾ, hb_pair (idx, default_ਮੁੱਲ)))
        ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ !condition_map.ਵਿੱਚ_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  ਤਾਰਕਿਕ create_from_ਵਸਤੂ_varstore (ਅਚਲ ਵਸਤੂ_Variation_Store& varStore,
                                  ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                  ਅਚਲ hb_array_t <ਅਚਲ hb_inc_bimap_t> inner_maps = hb_array_t<ਅਚਲ hb_inc_bimap_t> ())
  {
    ਅਚਲ VarRegionList& regionList = varStore.get_region_list ();
    ਜੇ (!regionList.get_var_regions (axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, orig_region_list))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_var_ਅੰਕੜੇ = varStore.get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ ();
    ਜੇ (inner_maps && inner_maps.ਲੰਮਾਈ != ਅੰਕ_var_ਅੰਕੜੇ) ਵਾਪਸ ਝੂਠ;
    ਜੇ (!vars.alloc (ਅੰਕ_var_ਅੰਕੜੇ) ||
        !var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.alloc (ਅੰਕ_var_ਅੰਕੜੇ)) ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_var_ਅੰਕੜੇ; i++)
    {
      ਜੇ (inner_maps && !inner_maps.arrayZ[i].get_population ())
          ਜਾਰੀ;
      tuple_variations_t var_ਅੰਕੜੇ_tuples;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਗਿਣਤੀ_2 = 0;
      ਜੇ (!var_ਅੰਕੜੇ_tuples.create_from_ਵਸਤੂ_var_ਅੰਕੜੇ (varStore.get_sub_ਸਾਰਣੀ (i),
                                                      orig_region_list,
                                                      axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                                      ਵਸਤੂ_ਗਿਣਤੀ_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : ਖਾਲੀ_ਸੂਚਕ))
        ਵਾਪਸ ਝੂਠ;

      var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.push (ਵਸਤੂ_ਗਿਣਤੀ_2);
      vars.push (std::move (var_ਅੰਕੜੇ_tuples));
    }
    ਵਾਪਸ !vars.ਵਿੱਚ_error () && !var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.ਵਿੱਚ_error () && vars.ਲੰਮਾਈ == var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.ਲੰਮਾਈ;
  }

  ਤਾਰਕਿਕ instantiate_tuple_vars (ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>& normalized_axes_location,
                               ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, TripleDistances>& axes_triple_distances,
                               ਤਾਰਕਿਕ build_regions = ਸੱਚ)
  {
    optimize_scratch_t scratch;
    ਲਈ (tuple_variations_t& tuple_vars : vars)
      ਜੇ (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_vardata (ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਗਿਣਤੀ_2)
  {
    vars.push (tuple_variations_t ());
    var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.push (ਵਸਤੂ_ਗਿਣਤੀ_2);
    ਵਾਪਸ vars.ਲੰਮਾਈ - 1;
  }

  /* Get item count for a VarData subtable. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਵਸਤੂ_ਗਿਣਤੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ outer) ਅਚਲ
  { ਵਾਪਸ outer < var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.ਲੰਮਾਈ ? var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ duplicate_ਪੰਕਤੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ outer, ਬਿਨਾਨਿਸ਼ਾਨੀ inner)
  {
    ਜੇ (unlikely (outer >= vars.ਲੰਮਾਈ ||
                  inner >= var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ[outer] ||
                  var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ[outer] >= 0xFFFFu))
      ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
    ਲਈ (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      ਜੇ (unlikely (inner >= tuple.deltas_x.ਲੰਮਾਈ ||
                    inner >= tuple.indices.ਲੰਮਾਈ))
        ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      ਜੇ (inner < tuple.deltas_y.ਲੰਮਾਈ)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      ਜੇ (unlikely (tuple.indices.ਵਿੱਚ_error () ||
                    tuple.deltas_x.ਵਿੱਚ_error () ||
                    tuple.deltas_y.ਵਿੱਚ_error ()))
        ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;
    }
    ਵਾਪਸ var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  ਖਾਲੀ ਜੋੜੋ_tuple (ਬਿਨਾਨਿਸ਼ਾਨੀ outer,
                  hb_hashmap_t<hb_ਟੈਗ_t, Triple>&& axis_tuples,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ inner, ਪੂਰਨ delta, ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਗਿਣਤੀ_2)
  {
    ਜੇ (outer >= vars.ਲੰਮਾਈ) ਵਾਪਸ;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    ਜੇ (!tuple.indices.resize (ਵਸਤੂ_ਗਿਣਤੀ_2) ||
        !tuple.deltas_x.resize (ਵਸਤੂ_ਗਿਣਤੀ_2)) ਵਾਪਸ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਵਸਤੂ_ਗਿਣਤੀ_2; i++)
    {
      tuple.indices.arrayZ[i] = ਸੱਚ;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (ਦਸ਼ਮਲਵ) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  ਤਾਰਕਿਕ build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> all_regions;
    hb_hashmap_t<region_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> ਵਰਤਿਆ_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_ਸਦਿਸ਼_t<region_t> all_unique_regions;
    ਲਈ (ਅਚਲ tuple_variations_t& sub_ਸਾਰਣੀ_2 : vars)
    {
      ਲਈ (ਅਚਲ tuple_delta_t& tuple : sub_ਸਾਰਣੀ_2.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        ਜੇ (!ਵਰਤਿਆ_regions.has (r))
        {
          ਤਾਰਕਿਕ all_zeros = ਸੱਚ;
          ਲਈ (ਦਸ਼ਮਲਵ d : tuple.deltas_x)
          {
            ਪੂਰਨ delta = hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (roundf ((ਦੁੱਗਣਾ) d));
            ਜੇ (delta != 0)
            {
              all_zeros = ਝੂਠ;
              ਰੋਕੋ;
            }
          }
          ਜੇ (!all_zeros)
          {
            ਜੇ (!ਵਰਤਿਆ_regions.ਸੈੱਟ (r, 1))
              ਵਾਪਸ ਝੂਠ;
          }
        }
        ਜੇ (all_regions.has (r))
          ਜਾਰੀ;
        ਜੇ (!all_regions.ਸੈੱਟ (r, 1))
          ਵਾਪਸ ਝੂਠ;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    ਜੇ (!all_regions || !all_unique_regions) ਵਾਪਸ ਸੱਚ;

    ਜੇ (!region_list.alloc (all_regions.get_population ()))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ idx = 0;
    /* append the original regions that pre-existed */
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& r : orig_region_list)
    {
      ਜੇ (!all_regions.has (&r) || !ਵਰਤਿਆ_regions.has (&r))
        ਜਾਰੀ;

      region_list.push (&r);
      ਜੇ (!region_map.ਸੈੱਟ (&r, idx))
        ਵਾਪਸ ਝੂਠ;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& r: all_unique_regions)
    {
      ਜੇ (!all_regions.has (r) || !ਵਰਤਿਆ_regions.has (r))
        ਜਾਰੀ;
      region_list.push (r);
      ਜੇ (!region_map.ਸੈੱਟ (r, idx))
        ਵਾਪਸ ਝੂਠ;
      all_regions.del (r);
      idx++;
    }
    ਵਾਪਸ (!region_list.ਵਿੱਚ_error ()) && (!region_map.ਵਿੱਚ_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  ਬਣਤਰ combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = ਮੂਲ;
    combined_gain_idx_tuple_t (ਬਿਨਾਨਿਸ਼ਾਨੀ gain, ਬਿਨਾਨਿਸ਼ਾਨੀ i, ਬਿਨਾਨਿਸ਼ਾਨੀ j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (gain < 0xFFFFFF);
      ਪ੍ਰਤਿਜ੍ਞਾ (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ < (ਅਚਲ combined_gain_idx_tuple_t& o)
    {
      ਵਾਪਸ encoded < o.encoded;
    }

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ <= (ਅਚਲ combined_gain_idx_tuple_t& o)
    {
      ਵਾਪਸ encoded <= o.encoded;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ idx_1 () ਅਚਲ { ਵਾਪਸ (encoded >> 20) & 0xFFFFF; };
    ਬਿਨਾਨਿਸ਼ਾਨੀ idx_2 () ਅਚਲ { ਵਾਪਸ encoded & 0xFFFFF; };
  };

  ਤਾਰਕਿਕ as_ਵਸਤੂ_varstore (ਤਾਰਕਿਕ optimize=ਸੱਚ, ਤਾਰਕਿਕ use_no_variation_idx=ਸੱਚ)
  {
    /* return true if no variation data */
    ਜੇ (!region_list) ਵਾਪਸ ਸੱਚ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ = region_list.ਲੰਮਾਈ;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਪੰਕਤੀਆਂ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ major = 0; major < var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ.ਲੰਮਾਈ; major++)
      ਕੁੱਲ_ਪੰਕਤੀਆਂ += var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ[major];

    ਜੇ (!delta_ਪੰਕਤੀਆਂ.resize (ਕੁੱਲ_ਪੰਕਤੀਆਂ)) ਵਾਪਸ ਝੂਠ;
    /* init all rows to [0]*num_cols */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਕੁੱਲ_ਪੰਕਤੀਆਂ; i++)
      ਜੇ (!(delta_ਪੰਕਤੀਆਂ[i].resize (ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ))) ਵਾਪਸ ਝੂਠ;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*> front_mapping;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ਪੰਕਤੀ = 0;
    hb_ਸਦਿਸ਼_t<delta_ਪੰਕਤੀ_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ> *> major_ਪੰਕਤੀਆਂ;
    hb_hashmap_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*, ਬਿਨਾਨਿਸ਼ਾਨੀ> delta_ਪੰਕਤੀਆਂ_map;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ major = 0; major < vars.ਲੰਮਾਈ; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ਅਚਲ tuple_variations_t& tuples = vars[major];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਪੰਕਤੀਆਂ = var_ਅੰਕੜੇ_ਅੰਕ_ਪੰਕਤੀਆਂ[major];

      ਜੇ (!ਅੰਕ_ਪੰਕਤੀਆਂ) ਜਾਰੀ;

      ਲਈ (ਅਚਲ tuple_delta_t& tuple: tuples.tuple_vars)
      {
        ਜੇ (tuple.deltas_x.ਲੰਮਾਈ != ਅੰਕ_ਪੰਕਤੀਆਂ)
          ਵਾਪਸ ਝੂਠ;

        /* skip unused regions */
        ਬਿਨਾਨਿਸ਼ਾਨੀ *ਖਾਨਾ_idx;
        ਜੇ (!region_map.has (&(tuple.axis_tuples), &ਖਾਨਾ_idx))
          ਜਾਰੀ;

        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_ਪੰਕਤੀਆਂ; i++)
        {
          ਪੂਰਨ rounded_delta = hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (roundf ((ਦੁੱਗਣਾ) tuple.deltas_x[i]));
          ਪੂਰਨ &delta = delta_ਪੰਕਤੀਆਂ[ਸ਼ੁਰੂ_ਪੰਕਤੀ + i][*ਖਾਨਾ_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          ਜੇ (unlikely (sum < INT_MIN || sum > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)) ਵਾਪਸ ਝੂਠ;
          delta = (ਪੂਰਨ) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_ਪੰਕਤੀਆਂ.reset ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ minor = 0; minor < ਅੰਕ_ਪੰਕਤੀਆਂ; minor++)
      {
	ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& ਪੰਕਤੀ = delta_ਪੰਕਤੀਆਂ[ਸ਼ੁਰੂ_ਪੰਕਤੀ + minor];
	ਜੇ (use_no_variation_idx)
	{
	  ਤਾਰਕਿਕ all_zeros = ਸੱਚ;
	  ਲਈ (ਪੂਰਨ delta : ਪੰਕਤੀ)
	  {
	    ਜੇ (delta != 0)
	    {
	      all_zeros = ਝੂਠ;
	      ਰੋਕੋ;
	    }
	  }
	  ਜੇ (all_zeros)
	    ਜਾਰੀ;
	}

	ਜੇ (!front_mapping.ਸੈੱਟ ((major<<16) + minor, &ਪੰਕਤੀ))
	  ਵਾਪਸ ਝੂਠ;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	ਜੇ (optimize)
	{
	  ਜੇ (delta_ਪੰਕਤੀਆਂ_map.has (&ਪੰਕਤੀ))
	    ਜਾਰੀ;

	  delta_ਪੰਕਤੀਆਂ_map.ਸੈੱਟ (&ਪੰਕਤੀ, 1);
	}

	major_ਪੰਕਤੀਆਂ.push (&ਪੰਕਤੀ);
      }

      ਜੇ (unlikely (major_ਪੰਕਤੀਆਂ.ਵਿੱਚ_error () || delta_ਪੰਕਤੀਆਂ_map.ਵਿੱਚ_error ()))
        ਵਾਪਸ ਝੂਠ;
      ਜੇ (major_ਪੰਕਤੀਆਂ)
      {
	ਸਵੈਚਾਲਿਤ *encoding = encoding_objs.push (std::move (major_ਪੰਕਤੀਆਂ), ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ);
	ਜੇ (unlikely (encoding_objs.ਵਿੱਚ_error () || encoding->ਵਿੱਚ_error ()))
	  ਵਾਪਸ ਝੂਠ;
      }

      ਸ਼ੁਰੂ_ਪੰਕਤੀ += ਅੰਕ_ਪੰਕਤੀਆਂ;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    ਜੇ (!optimize)
    {
      encodings = std::move (encoding_objs);
      ਵਾਪਸ !encodings.ਵਿੱਚ_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.qsort ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    ਵਰਤੋਂ ਵਸਤੂ_t = hb_ਤਰਜੀਹ_ਕਤਾਰ_t<combined_gain_idx_tuple_t>::ਵਸਤੂ_t;
    hb_ਸਦਿਸ਼_t<ਵਸਤੂ_t> ਕਤਾਰ_ਚੀਜ਼ਾਂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_todos = encoding_objs.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_todos; i++)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = i + 1; j < ਅੰਕ_todos; j++)
      {
        ਪੂਰਨ combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        ਜੇ (combining_gain > 0)
          ਕਤਾਰ_ਚੀਜ਼ਾਂ.push (ਵਸਤੂ_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_ਤਰਜੀਹ_ਕਤਾਰ_t<combined_gain_idx_tuple_t> ਕਤਾਰ (std::move (ਕਤਾਰ_ਚੀਜ਼ਾਂ));

    hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t removed_todo_idxes;
    ਜਦੋਂ (ਕਤਾਰ)
    {
      ਸਵੈਚਾਲਿਤ t = ਕਤਾਰ.ਕੱਢੋ_minimum ().ਪਹਿਲਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ i = t.idx_1 ();
      ਬਿਨਾਨਿਸ਼ਾਨੀ j = t.idx_2 ();

      ਜੇ (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        ਜਾਰੀ;

      delta_ਪੰਕਤੀ_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_ਪੰਕਤੀ_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.ਜੋੜੋ (i);
      removed_todo_idxes.ਜੋੜੋ (j);
      ਜੇ (unlikely (removed_todo_idxes.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

      encoding.ਮਿਲਾਓ (other_encoding);
      ਜੇ (unlikely (encoding.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ idx = 0; idx < encoding_objs.ਲੰਮਾਈ; idx++)
      {
        ਜੇ (removed_todo_idxes.has (idx)) ਜਾਰੀ;

        ਅਚਲ delta_ਪੰਕਤੀ_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        ਜੇ (obj.ਚੌੜਾਈ == encoding.ਚੌੜਾਈ && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& ਪੰਕਤੀ : obj.ਚੀਜ਼ਾਂ)
            encoding.ਜੋੜੋ_ਪੰਕਤੀ (ਪੰਕਤੀ);
          ਜੇ (unlikely (encoding.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

          removed_todo_idxes.ਜੋੜੋ (idx);
          ਜੇ (unlikely (removed_todo_idxes.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
          ਜਾਰੀ;
        }

        ਪੂਰਨ combined_gain = encoding.gain_from_merging (obj);
        ਜੇ (combined_gain > 0)
          ਕਤਾਰ.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.ਲੰਮਾਈ), 0);
      }

      ਸਵੈਚਾਲਿਤ moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      ਜੇ (unlikely (encoding_objs.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
    }

    ਪੂਰਨ ਅੰਕ_final_encodings = (ਪੂਰਨ) encoding_objs.ਲੰਮਾਈ - (ਪੂਰਨ) removed_todo_idxes.get_population ();
    ਜੇ (ਅੰਕ_final_encodings <= 0) ਵਾਪਸ ਝੂਠ;

    ਜੇ (!encodings.alloc (ਅੰਕ_final_encodings)) ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < encoding_objs.ਲੰਮਾਈ; i++)
    {
      ਜੇ (removed_todo_idxes.has (i)) ਜਾਰੀ;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    ਵਾਪਸ compile_varidx_map (front_mapping);
  }

  ਨਿਜੀ:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  ਤਾਰਕਿਕ compile_varidx_map (ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*, ਬਿਨਾਨਿਸ਼ਾਨੀ> ਪਿੱਠ_mapping;
    hb_ਸਦਿਸ਼_t<delta_ਪੰਕਤੀ_encoding_t> ਵੰਡੋ_encodings;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < encodings.ਲੰਮਾਈ; i++)
    {
      delta_ਪੰਕਤੀ_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      ਜੇ (encoding.is_empty () || encoding.ਵਿੱਚ_error ())
        ਵਾਪਸ ਝੂਠ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਪੰਕਤੀਆਂ = encoding.ਚੀਜ਼ਾਂ.ਲੰਮਾਈ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ = encoding.chars.ਲੰਮਾਈ;

      /* sort rows, make result deterministic */
      encoding.ਚੀਜ਼ਾਂ.qsort (cmp_ਪੰਕਤੀ);

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0; ਸ਼ੁਰੂ < ਅੰਕ_ਪੰਕਤੀਆਂ; ਸ਼ੁਰੂ += 0xFFFFu)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ chunk_len = hb_min (ਅੰਕ_ਪੰਕਤੀਆਂ - ਸ਼ੁਰੂ, 0xFFFFu);
        hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*> ਪੰਕਤੀਆਂ;

        ਜੇ (!ਪੰਕਤੀਆਂ.alloc (chunk_len))
          ਵਾਪਸ ਝੂਠ;

        ਬਿਨਾਨਿਸ਼ਾਨੀ major = ਵੰਡੋ_encodings.ਲੰਮਾਈ;
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ minor = 0; minor < chunk_len; minor++)
        {
          ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ> *ਪੰਕਤੀ = encoding.ਚੀਜ਼ਾਂ.arrayZ[ਸ਼ੁਰੂ + minor];
          ਪੰਕਤੀਆਂ.push (ਪੰਕਤੀ);
          ਜੇ (!ਪਿੱਠ_mapping.ਸੈੱਟ (ਪੰਕਤੀ, (major << 16) + minor))
            ਵਾਪਸ ਝੂਠ;
        }

        ਸਵੈਚਾਲਿਤ *ਵੰਡੋ = ਵੰਡੋ_encodings.push (std::move (ਪੰਕਤੀਆਂ), ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ);
        ਜੇ (unlikely (ਵੰਡੋ_encodings.ਵਿੱਚ_error () || ਵੰਡੋ->ਵਿੱਚ_error ()))
          ਵਾਪਸ ਝੂਠ;
      }
    }

    encodings = std::move (ਵੰਡੋ_encodings);
    ਜੇ (encodings.ਵਿੱਚ_error () || ਪਿੱਠ_mapping.ਵਿੱਚ_error ())
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਸਵੈਚਾਲਿਤ _ : front_mapping.iter ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ old_varidx = _.ਪਹਿਲਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ *new_varidx;
      ਜੇ (ਪਿੱਠ_mapping.has (_.second, &new_varidx))
        varidx_map.ਸੈੱਟ (old_varidx, *new_varidx);
      ਨਹੀਂਤਾਂ
        varidx_map.ਸੈੱਟ (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ);
    }
    ਵਾਪਸ !varidx_map.ਵਿੱਚ_error ();
  }

  ਸਥਿਰ ਪੂਰਨ cmp_ਪੰਕਤੀ (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>** a = (ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>**) pa;
    ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>** b = (ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>**) pb;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < (*b)->ਲੰਮਾਈ; i++)
    {
      ਪੂਰਨ va = (*a)->arrayZ[i];
      ਪੂਰਨ vb = (*b)->arrayZ[i];
      ਜੇ (va != vb)
        ਵਾਪਸ va < vb ? -1 : 1;
    }
    ਵਾਪਸ 0;
  }
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_VAR_COMMON_HH */

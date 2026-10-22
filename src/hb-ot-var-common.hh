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

#ifndef HB_OT_VAR_COMMON_HH
#define HB_OT_VAR_COMMON_HH

#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-alloc-pool.hh"
#ಅನ್ತರ್ಭಾವ "hb-priority-queue.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-instancer-iup.hh"


ನಾಮಾಕಾಶ OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
ಸಂರಚನಾ TupleVariationHeader
{
  friend ಸಂರಚನಾ tuple_delta_t;
  size_t get_size (unsigned axis_ಎಣಿಕೆ_times_2) ಧ್ರುವ
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    ಯದಿ (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned ಎಣಿಕೆ = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (ಎಣಿಕೆ, axis_ಎಣಿಕೆ_times_2, min_size);
    }
    ಪ್ರತ್ಯಾಗಮ min_size;
  }

  unsigned get_data_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ varDataSize; }

  ಧ್ರುವ TupleVariationHeader &get_next (unsigned axis_ಎಣಿಕೆ_times_2) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ StructAtOffset<TupleVariationHeader> (ಏತತ್, get_size (axis_ಎಣಿಕೆ_times_2)); }

  ತಾರ್ಕಿಕ unpack_axis_tuples (unsigned axis_ಎಣಿಕೆ,
                           ಧ್ರುವ hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples,
                           ಧ್ರುವ hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ಧ್ರುವ
  {
    ಧ್ರುವ F2DOT14 *peak_tuple = nullptr;
    ಯದಿ (has_peak ())
      peak_tuple = get_peak_tuple (axis_ಎಣಿಕೆ);
    ಅನ್ಯಥಾ
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ index = get_index ();
      ಯದಿ (unlikely ((index + 1) * axis_ಎಣಿಕೆ > shared_tuples.length))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      peak_tuple = shared_tuples.sub_array (axis_ಎಣಿಕೆ * index, axis_ಎಣಿಕೆ).arrayZ;
    }

    ಧ್ರುವ F2DOT14 *start_tuple = nullptr;
    ಧ್ರುವ F2DOT14 *end_tuple = nullptr;
    ತಾರ್ಕಿಕ has_interm = has_intermediate ();

    ಯದಿ (has_interm)
    {
      start_tuple = get_start_tuple (axis_ಎಣಿಕೆ);
      end_tuple = get_end_tuple (axis_ಎಣಿಕೆ);
    }

    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ; i++)
    {
      ಭಿನ್ನ peak = peak_tuple[i].to_float ();
      ಯದಿ (peak == 0.f) ಅನುವರ್ತ;

      hb_tag_t *axis_tag;
      ಯದಿ (!axes_old_index_tag_map->has (i, &axis_tag))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಭಿನ್ನ start, end;
      ಯದಿ (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      ಅನ್ಯಥಾ
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  HB_ALWAYS_INLINE
  ಭಿನ್ನ calculate_scalar (hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
			   ಧ್ರುವ hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ಧ್ರುವ
  {
    unsigned tuple_index = tupleIndex;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    ತಾರ್ಕಿಕ plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    ಯದಿ (likely (plain_shared_tuple))
    {
      ಭಿನ್ನ scalar;
      ಯದಿ (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	ಪ್ರತ್ಯಾಗಮ scalar;
    }

    ಧ್ರುವ F2DOT14 *peak_tuple;

    ತಾರ್ಕಿಕ has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    ಯದಿ (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_ಎಣಿಕೆ);
      shared_tuple_scalar_cache = nullptr;
    }
    ಅನ್ಯಥಾ
    {
      ಭಿನ್ನ scalar;
      ಯದಿ (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        ಯದಿ (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	ಅನ್ಯಥಾ
	  ಪ್ರತ್ಯಾಗಮ scalar;
      }

      ಯದಿ (unlikely ((index + 1) * coord_ಎಣಿಕೆ > shared_tuples.length))
        ಪ್ರತ್ಯಾಗಮ 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_ಎಣಿಕೆ * index);

    }

    ಧ್ರುವ F2DOT14 *start_tuple = nullptr;
    ಧ್ರುವ F2DOT14 *end_tuple = nullptr;

    ಯದಿ (has_interm)
    {
      start_tuple = get_start_tuple (coord_ಎಣಿಕೆ);
      end_tuple = get_end_tuple (coord_ಎಣಿಕೆ);
    }

    ಭಿನ್ನ scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
    ತಾರ್ಕಿಕ skip = coord_ಎಣಿಕೆ >= 16;
#endif
#endif
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < coord_ಎಣಿಕೆ; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#ಯದಿ HB_FAST_ಅಂಕೆ_ACCESS
      ಯದಿ (skip)
      {
	ಯಾವತ್ (i + 4 <= coord_ಎಣಿಕೆ && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	ಯಾವತ್ (i < coord_ಎಣಿಕೆ && peak_tuple[i].to_int () == 0)
	  i += 1;
	ಯದಿ (i >= coord_ಎಣಿಕೆ)
	  ಭಙ್ಗ;
      }
#endif
#endif

      ಪೂರ್ಣಾಙ್ಕ peak = peak_tuple[i].to_int ();
      ಯದಿ (!peak) ಅನುವರ್ತ;

      ಪೂರ್ಣಾಙ್ಕ v = coords[i];
      ಯದಿ (!v) { scalar = 0.f; ಭಙ್ಗ; }
      ಯದಿ (v == peak) ಅನುವರ್ತ;

      ಯದಿ (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        ಪೂರ್ಣಾಙ್ಕ start = start_tuple[i].to_int ();
        ಪೂರ್ಣಾಙ್ಕ end = end_tuple[i].to_int ();
        ಯದಿ (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) ಅನುವರ್ತ;
        ಯದಿ (v < start || v > end) { scalar = 0.f; ಭಙ್ಗ; }
        ಯದಿ (v < peak)
        { ಯದಿ (peak != start) scalar *= (ಭಿನ್ನ) (v - start) / (peak - start); }
        ಅನ್ಯಥಾ
        { ಯದಿ (peak != end) scalar *= (ಭಿನ್ನ) (end - v) / (end - peak); }
      }
      ಅನ್ಯಥಾ ಯದಿ (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; ಭಙ್ಗ; }
      ಅನ್ಯಥಾ
        scalar *= (ಭಿನ್ನ) v / peak;
    }
    ಯದಿ (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    ಪ್ರತ್ಯಾಗಮ scalar;
  }

  ತಾರ್ಕಿಕ           has_peak () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  ತಾರ್ಕಿಕ   has_intermediate () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tupleIndex & TupleIndex::IntermediateRegion; }
  ತಾರ್ಕಿಕ has_private_points () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tupleIndex & TupleIndex::TupleIndexMask; }
  ಸ್ಥಿರ constexpr unsigned max_shared_tuple_ಎಣಿಕೆ = 0x1000u;

  ರಕ್ಷಿತ:
  ಸಂರಚನಾ TupleIndex : HBUINT16
  {
    ಗಣನಾ Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<ಧ್ರುವ F2DOT14> get_all_tuples (unsigned axis_ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_ಎಣಿಕೆ); }
  ಧ್ರುವ F2DOT14* get_all_tuples_base (unsigned axis_ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  ಧ್ರುವ F2DOT14* get_peak_tuple (unsigned axis_ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_all_tuples_base (axis_ಎಣಿಕೆ); }
  ಧ್ರುವ F2DOT14* get_start_tuple (unsigned axis_ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_all_tuples_base (axis_ಎಣಿಕೆ) + has_peak () * axis_ಎಣಿಕೆ; }
  ಧ್ರುವ F2DOT14* get_end_tuple (unsigned axis_ಎಣಿಕೆ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_all_tuples_base (axis_ಎಣಿಕೆ) + has_peak () * axis_ಎಣಿಕೆ + axis_ಎಣಿಕೆ; }

  HBUINT16      varDataSize;    /* The size in bytes of the serialized
                                 * data for this tuple variation table. */
  TupleIndex    tupleIndex;     /* A packed field. The high 4 bits are flags (see below).
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (4);
};

ಸಂರಚನಾ optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<ತಾರ್ಕಿಕ> opt_indices;
  hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> rounded_x_deltas;
  hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> rounded_y_deltas;
  hb_vector_t<ಭಿನ್ನ> opt_deltas_x;
  hb_vector_t<ಭಿನ್ನ> opt_deltas_y;
  hb_vector_t<unsigned ವರ್ಣ> opt_point_data;
  hb_vector_t<unsigned ವರ್ಣ> opt_deltas_data;
  hb_vector_t<unsigned ವರ್ಣ> point_data;
  hb_vector_t<unsigned ವರ್ಣ> deltas_data;
  hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> rounded_deltas;
};

ಸಂರಚನಾ tuple_delta_t
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ realloc_move = ಸತ್ಯ;  // Watch out when adding new members!

  ಸಾರ್ವಜನಿಕ:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<ತಾರ್ಕಿಕ> indices;

  hb_vector_t<ಭಿನ್ನ> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<ಭಿನ್ನ> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned ವರ್ಣ> compiled_tuple_header;
  hb_vector_t<unsigned ವರ್ಣ> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (ಧ್ರುವ tuple_delta_t& o) = ಸ್ವಭಾವ;
  tuple_delta_t& operator = (ಧ್ರುವ tuple_delta_t& o) = ಸ್ವಭಾವ;

  friend ಶೂನ್ಯ swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
  {
    hb_swap (a.axis_tuples, b.axis_tuples);
    hb_swap (a.indices, b.indices);
    hb_swap (a.deltas_x, b.deltas_x);
    hb_swap (a.deltas_y, b.deltas_y);
    hb_swap (a.compiled_tuple_header, b.compiled_tuple_header);
    hb_swap (a.compiled_deltas, b.compiled_deltas);
    hb_swap (a.compiled_peak_coords, b.compiled_peak_coords);
  }

  tuple_delta_t (tuple_delta_t&& o)  noexcept : tuple_delta_t ()
  { hb_swap (*ಏತತ್, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*ಏತತ್, o);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  ಶೂನ್ಯ copy_from (ಧ್ರುವ tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
  {
    axis_tuples = o.axis_tuples;
    indices.duplicate_vector_from_pool (pool, o.indices);
    deltas_x.duplicate_vector_from_pool (pool, o.deltas_x);
    deltas_y.duplicate_vector_from_pool (pool, o.deltas_y);
    compiled_tuple_header.duplicate_vector_from_pool (pool, o.compiled_tuple_header);
    compiled_deltas.duplicate_vector_from_pool (pool, o.compiled_deltas);
    compiled_peak_coords.duplicate_vector_from_pool (pool, o.compiled_peak_coords);
    compiled_interm_coords.duplicate_vector_from_pool (pool, o.compiled_interm_coords);
  }

  ಶೂನ್ಯ remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  ತಾರ್ಕಿಕ set_tent (hb_tag_t axis_tag, Triple tent)
  { ಪ್ರತ್ಯಾಗಮ axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (ಧ್ರುವ tuple_delta_t& o)
  {
    unsigned ಅಂಕೆ = indices.length;
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ; i++)
    {
      ಯದಿ (indices.arrayZ[i])
      {
        ಯದಿ (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          ಯದಿ (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      ಅನ್ಯಥಾ
      {
        ಯದಿ (!o.indices.arrayZ[i]) ಅನುವರ್ತ;
        indices.arrayZ[i] = ಸತ್ಯ;
        deltas_x[i] = o.deltas_x[i];
        ಯದಿ (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  tuple_delta_t& operator *= (ಭಿನ್ನ scalar)
  {
    ಯದಿ (scalar == 1.0f)
      ಪ್ರತ್ಯಾಗಮ *ಏತತ್;

    unsigned ಅಂಕೆ = indices.length;
    ಯದಿ (deltas_y)
      ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ; i++)
      {
	ಯದಿ (!indices.arrayZ[i]) ಅನುವರ್ತ;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    ಅನ್ಯಥಾ
      ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ; i++)
      {
	ಯದಿ (!indices.arrayZ[i]) ಅನುವರ್ತ;
	deltas_x[i] *= scalar;
      }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  ಶೂನ್ಯ change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    ಯದಿ (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*ಏತತ್));
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->ಮಧ್ಯ && tent->ಮಧ್ಯ <= tent->maximum))
      ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (tent->ಮಧ್ಯ == 0.f)
    {
      out.push (std::move (*ಏತತ್));
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.ಮಧ್ಯ &&
          axis_limit.ಮಧ್ಯ <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      ಪ್ರತ್ಯಾಗಮ;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    ಪ್ರತಿ (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      ಯದಿ (i < solutions.length - 1)
	new_var.copy_from (*ಏತತ್, pool);
      ಅನ್ಯಥಾ
	new_var = std::move (*ಏತತ್);

      ಯದಿ (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      ಅನ್ಯಥಾ
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  ತಾರ್ಕಿಕ compile_coords (ಧ್ರುವ hb_map_t& axes_index_map,
		       ಧ್ರುವ hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_ಎಣಿಕೆ = axes_index_map.get_population ();
    ಯದಿ (pool)
    {
      ಯದಿ (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_ಎಣಿಕೆ)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಅನ್ಯಥಾ ಯದಿ (unlikely (!compiled_peak_coords.resize (cur_axis_ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_ಎಣಿಕೆ = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    ಪ್ರತಿ (unsigned i = 0; i < orig_axis_ಎಣಿಕೆ; i++)
    {
      ಯದಿ (!axes_index_map.has (i))
        ಅನುವರ್ತ;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      ಯದಿ (axis_tuples.has (axis_tag, &coords))
      {
	ಭಿನ್ನ min_val = coords->minimum;
	ಭಿನ್ನ val = coords->ಮಧ್ಯ;
	ಭಿನ್ನ max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	ಯದಿ (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  ಯದಿ (!compiled_interm_coords)
	  {
	    ಯದಿ (pool)
	    {
	      ಯದಿ (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_ಎಣಿಕೆ)))
		ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	    }
	    ಅನ್ಯಥಾ ಯದಿ (unlikely (!compiled_interm_coords.resize (2 * cur_axis_ಎಣಿಕೆ)))
	      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_ಎಣಿಕೆ);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_ಎಣಿಕೆ);

	    ಪ್ರತಿ (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      ಯದಿ (!peak) ಅನುವರ್ತ;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	ಯದಿ (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    ಪ್ರತ್ಯಾಗಮ !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  ತಾರ್ಕಿಕ compile_tuple_var_header (ಧ್ರುವ hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 ಧ್ರುವ hb_map_t& axes_old_index_tag_map,
                                 ಧ್ರುವ hb_hashmap_t<ಧ್ರುವ hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    ಯದಿ (!compiled_deltas) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    unsigned cur_axis_ಎಣಿಕೆ = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_ಎಣಿಕೆ * (F2DOT14::static_size) + 4;
    ಯದಿ (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, ಅಸತ್ಯ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    ಯದಿ (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_ಎಣಿಕೆ = 0;
    unsigned *shared_tuple_idx;
    ಯದಿ (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    ಅನ್ಯಥಾ
    {
      peak_ಎಣಿಕೆ = encode_peak_coords(coords, flag);
      ಯದಿ (!peak_ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_ಎಣಿಕೆ = encode_interm_coords (coords.sub_array (peak_ಎಣಿಕೆ), flag);

    /* pointdata length = 0 implies "use shared points" */
    ಯದಿ (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_ಎಣಿಕೆ + interim_ಎಣಿಕೆ) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) ಧ್ರುವ
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * ಪರಿಮಾಣ (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    ಪ್ರತ್ಯಾಗಮ compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) ಧ್ರುವ
  {
    ಯದಿ (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * ಪರಿಮಾಣ (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    ಪ್ರತ್ಯಾಗಮ compiled_interm_coords.length;
  }

  ತಾರ್ಕಿಕ compile_deltas (hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { ಪ್ರತ್ಯಾಗಮ compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ compile_deltas (hb_array_t<ಧ್ರುವ ತಾರ್ಕಿಕ> point_indices,
			      hb_array_t<ಧ್ರುವ ಭಿನ್ನ> x_deltas,
			      hb_array_t<ಧ್ರುವ ಭಿನ್ನ> y_deltas,
			      hb_vector_t<unsigned ವರ್ಣ> &compiled_deltas, /* OUT */
			      hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    ಯದಿ (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned j = 0;
    ಪ್ರತಿ (unsigned i = 0; i < point_indices.length; i++)
    {
      ಯದಿ (!point_indices[i]) ಅನುವರ್ತ;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (roundf ((ದ್ವಿಗುಣ) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    ಯದಿ (!rounded_deltas) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    ಯದಿ (y_deltas)
      alloc_len *= 2;

    ಯದಿ (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, ಅಸತ್ಯ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    ಯದಿ (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      ಪ್ರತಿ (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        ಯದಿ (!point_indices[idx]) ಅನುವರ್ತ;
        ಪೂರ್ಣಾಙ್ಕ rounded_delta = hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (roundf ((ದ್ವಿಗುಣ) y_deltas.arrayZ[idx]));

        ಯದಿ (j >= rounded_deltas.length) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        rounded_deltas[j++] = rounded_delta;
      }

      ಯದಿ (j != rounded_deltas.length) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸ್ಥಿರ unsigned compile_deltas (hb_array_t<unsigned ವರ್ಣ> encoded_bytes,
				  hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> deltas)
  {
    ಪ್ರತ್ಯಾಗಮ Tuple_ಮೌಲ್ಯಗಳು::compile_unsafe (deltas, encoded_bytes);
  }

  ತಾರ್ಕಿಕ calc_inferred_deltas (ಧ್ರುವ contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_ಎಣಿಕೆ = orig_points.length;
    ಯದಿ (point_ಎಣಿಕೆ != indices.length)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ref_ಎಣಿಕೆ_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    ಪ್ರತಿ (unsigned i = 0; i < point_ಎಣಿಕೆ; i++)
    {
      ref_ಎಣಿಕೆ_2 += indices.arrayZ[i];
      ಯದಿ (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    ಯದಿ (ref_ಎಣಿಕೆ_2 == point_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಯದಿ (unlikely (end_points.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    ಪ್ರತಿ (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_ಎಣಿಕೆ = 0;
      ಪ್ರತಿ (unsigned i = start_point; i < end_point + 1; i++)
        unref_ಎಣಿಕೆ += indices.arrayZ[i];
      unref_ಎಣಿಕೆ = (end_point - start_point + 1) - unref_ಎಣಿಕೆ;

      unsigned j = start_point;
      ಯದಿ (unref_ಎಣಿಕೆ == 0 || unref_ಎಣಿಕೆ > end_point - start_point)
        ಗಚ್ಛ no_more_gaps;
      ಪ್ರತಿ (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned ಪೂರ್ಣಾಙ್ಕ prev, next, i;
        ಪ್ರತಿ (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          ಯದಿ (indices.arrayZ[i] && !indices.arrayZ[j]) ಭಙ್ಗ;
        }
        prev = j = i;
        ಪ್ರತಿ (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          ಯದಿ (!indices.arrayZ[i] && indices.arrayZ[j]) ಭಙ್ಗ;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        ಪ್ರತಿ (;;)
        {
          i = next_index (i, start_point, end_point);
          ಯದಿ (i == next) ಭಙ್ಗ;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          ಯದಿ (--unref_ಎಣಿಕೆ == 0) ಗಚ್ಛ no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    ಪ್ರತಿ (unsigned i = 0; i < point_ಎಣಿಕೆ; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      ಯದಿ ( !indices[i])
      {
        ಯದಿ (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = ಸತ್ಯ;
      }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ optimize (ಧ್ರುವ contour_point_vector_t& contour_points,
                 ತಾರ್ಕಿಕ is_composite,
		 optimize_scratch_t &scratch,
                 ದ್ವಿಗುಣ tolerance = 0.5 + 1e-10)
  {
    unsigned ಎಣಿಕೆ = contour_points.length;
    ಯದಿ (deltas_x.length != ಎಣಿಕೆ ||
        deltas_y.length != ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_vector_t<ತಾರ್ಕಿಕ> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> &rounded_y_deltas = scratch.rounded_y_deltas;

    ಯದಿ (unlikely (!rounded_x_deltas.resize_dirty  (ಎಣಿಕೆ) ||
                  !rounded_y_deltas.resize_dirty  (ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (roundf ((ದ್ವಿಗುಣ) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (roundf ((ದ್ವಿಗುಣ) deltas_y.arrayZ[i]));
    }

    ಯದಿ (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ref_ಎಣಿಕೆ_2 = 0;
    ಪ್ರತಿ (ತಾರ್ಕಿಕ ref_flag : opt_indices)
       ref_ಎಣಿಕೆ_2 += ref_flag;

    ಯದಿ (ref_ಎಣಿಕೆ_2 == ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    hb_vector_t<ಭಿನ್ನ> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<ಭಿನ್ನ> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    ತಾರ್ಕಿಕ is_comp_glyph_wo_deltas = (is_composite && ref_ಎಣಿಕೆ_2 == 0);
    ಯದಿ (is_comp_glyph_wo_deltas)
    {
      ಯದಿ (unlikely (!opt_deltas_x.resize (ಎಣಿಕೆ) ||
                    !opt_deltas_y.resize (ಎಣಿಕೆ)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      opt_indices.arrayZ[0] = ಸತ್ಯ;
      ಪ್ರತಿ (unsigned i = 1; i < ಎಣಿಕೆ; i++)
        opt_indices.arrayZ[i] = ಅಸತ್ಯ;
    }

    hb_vector_t<unsigned ವರ್ಣ> &opt_point_data = scratch.opt_point_data.reset ();
    ಯದಿ (!compile_point_set (opt_indices, opt_point_data))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_vector_t<unsigned ವರ್ಣ> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    ಯದಿ (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_vector_t<unsigned ವರ್ಣ> &point_data = scratch.point_data.reset ();
    ಯದಿ (!compile_point_set (indices, point_data))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_vector_t<unsigned ವರ್ಣ> &deltas_data = scratch.deltas_data.reset ();
    ಯದಿ (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      ಯದಿ (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    ಪ್ರತ್ಯಾಗಮ !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ compile_point_set (ಧ್ರುವ hb_vector_t<ತಾರ್ಕಿಕ> &point_indices,
                                 hb_vector_t<unsigned ವರ್ಣ>& compiled_points /* OUT */)
  {
    unsigned ಅಂಕೆ_points = 0;
    ಪ್ರತಿ (ತಾರ್ಕಿಕ i : point_indices)
      ಯದಿ (i) ಅಂಕೆ_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    ಯದಿ (!ಅಂಕೆ_points) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    ಯದಿ (ಅಂಕೆ_points == indices_length)
      ಪ್ರತ್ಯಾಗಮ compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned ಅಂಕೆ_bytes = 2 + 3 *ಅಂಕೆ_points;
    ಯದಿ (unlikely (!compiled_points.resize_dirty  (ಅಂಕೆ_bytes)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    ಯದಿ (ಅಂಕೆ_points < 0x80)
      compiled_points.arrayZ[pos++] = ಅಂಕೆ_points;
    ಅನ್ಯಥಾ
    {
      compiled_points.arrayZ[pos++] = ((ಅಂಕೆ_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = ಅಂಕೆ_points & 0xFF;
    }

    ಧ್ರುವ unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_ಮೌಲ್ಯ = 0;
    unsigned ಅಂಕೆ_encoded = 0;
    ಯಾವತ್ (i < indices_length && ಅಂಕೆ_encoded < ಅಂಕೆ_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      ತಾರ್ಕಿಕ use_byte_encoding = ಅಸತ್ಯ;
      ತಾರ್ಕಿಕ new_run = ಸತ್ಯ;
      ಯಾವತ್ (i < indices_length && ಅಂಕೆ_encoded < ಅಂಕೆ_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        ಯಾವತ್ (i < indices_length && !point_indices[i])
          i++;

        ಯದಿ (i >= indices_length) ಭಙ್ಗ;

        unsigned cur_ಮೌಲ್ಯ = i;
        unsigned delta = cur_ಮೌಲ್ಯ - last_ಮೌಲ್ಯ;

        ಯದಿ (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = ಅಸತ್ಯ;
        }

        ಯದಿ (use_byte_encoding && delta > 0xFF)
          ಭಙ್ಗ;

        ಯದಿ (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        ಅನ್ಯಥಾ
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_ಮೌಲ್ಯ = cur_ಮೌಲ್ಯ;
        run_length++;
        ಅಂಕೆ_encoded++;
      }

      ಯದಿ (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      ಅನ್ಯಥಾ
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    ಪ್ರತ್ಯಾಗಮ compiled_points.resize_dirty  (pos);
  }

  ಸ್ಥಿರ ಭಿನ್ನ infer_delta (ಭಿನ್ನ target_val, ಭಿನ್ನ prev_val, ಭಿನ್ನ next_val, ಭಿನ್ನ prev_delta, ಭಿನ್ನ next_delta)
  {
    ಯದಿ (prev_val == next_val)
      ಪ್ರತ್ಯಾಗಮ (prev_delta == next_delta) ? prev_delta : 0.f;
    ಅನ್ಯಥಾ ಯದಿ (target_val <= hb_min (prev_val, next_val))
      ಪ್ರತ್ಯಾಗಮ (prev_val < next_val) ? prev_delta : next_delta;
    ಅನ್ಯಥಾ ಯದಿ (target_val >= hb_max (prev_val, next_val))
      ಪ್ರತ್ಯಾಗಮ (prev_val > next_val) ? prev_delta : next_delta;

    ಭಿನ್ನ r = (target_val - prev_val) / (next_val - prev_val);
    ಪ್ರತ್ಯಾಗಮ prev_delta + r * (next_delta - prev_delta);
  }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ next_index (unsigned ಪೂರ್ಣಾಙ್ಕ i, unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಪೂರ್ಣಾಙ್ಕ end)
  { ಪ್ರತ್ಯಾಗಮ (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
ಸಂರಚನಾ TupleVariationData
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (ಏತತ್));
  }

  size_t get_size (unsigned axis_ಎಣಿಕೆ_times_2) ಧ್ರುವ
  {
    size_t total_size = min_size;
    unsigned ಎಣಿಕೆ = tuple_Var_ಎಣಿಕೆ.get_ಎಣಿಕೆ ();
    ಧ್ರುವ TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_ಎಣಿಕೆ_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_ಎಣಿಕೆ_times_2);
    }

    ಪ್ರತ್ಯಾಗಮ total_size;
  }

  ಧ್ರುವ TupleVariationHeader &get_tuple_var_header (ಶೂನ್ಯ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ StructAfter<TupleVariationHeader> (data); }

  ಸಂರಚನಾ tuple_iterator_t;
  ಸಂರಚನಾ tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    ಗುಪ್ತ:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ಧ್ರುವ hb_vector_t<ತಾರ್ಕಿಕ>*, hb_vector_t<unsigned ವರ್ಣ>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ಧ್ರುವ hb_vector_t<ತಾರ್ಕಿಕ>*, unsigned> point_set_ಎಣಿಕೆ_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned ವರ್ಣ> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    ತಾರ್ಕಿಕ needs_padding = ಅಸತ್ಯ;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    ತಾರ್ಕಿಕ is_composite = ಅಸತ್ಯ;

    ಸಾರ್ವಜನಿಕ:
    tuple_variations_t () = ಸ್ವಭಾವ;
    tuple_variations_t (ಧ್ರುವ tuple_variations_t&) = delete;
    tuple_variations_t& operator=(ಧ್ರುವ tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = ಸ್ವಭಾವ;
    tuple_variations_t& operator=(tuple_variations_t&&) = ಸ್ವಭಾವ;
    ~tuple_variations_t () = ಸ್ವಭಾವ;

    explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ತಾರ್ಕಿಕ (tuple_vars); }
    unsigned get_var_ಎಣಿಕೆ () ಧ್ರುವ
    {
      unsigned ಎಣಿಕೆ = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      ಪ್ರತಿ (auto& tuple: tuple_vars)
        ಯದಿ (tuple.compiled_deltas) ಎಣಿಕೆ++;

      ಯದಿ (shared_points_bytes && shared_points_bytes->length)
        ಎಣಿಕೆ |= Tuple_Var_ಎಣಿಕೆ::SharedPointNumbers;
      ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
    }

    unsigned get_compiled_byte_size () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ compiled_byte_size; }

    ತಾರ್ಕಿಕ create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_ಎಣಿಕೆ,
                                     unsigned point_ಎಣಿಕೆ,
                                     ತಾರ್ಕಿಕ is_gvar,
                                     ಧ್ರುವ hb_map_t *axes_old_index_tag_map,
                                     ಧ್ರುವ hb_vector_t<unsigned> &shared_indices,
                                     ಧ್ರುವ hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     ತಾರ್ಕಿಕ is_composite_glyph = ಅಸತ್ಯ)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> deltas_x;
      hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> deltas_y;
      ಕುರು
      {
        ಧ್ರುವ HBUINT8 *p = iterator.get_serialized_data ();
        unsigned ಪೂರ್ಣಾಙ್ಕ length = iterator.current_tuple->get_data_size ();
        ಯದಿ (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        ಯದಿ (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_ಎಣಿಕೆ (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	private_indices.reset ();
        ತಾರ್ಕಿಕ has_private_points = iterator.current_tuple->has_private_points ();
        ಧ್ರುವ HBUINT8 *end = p + length;
        ಯದಿ (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಧ್ರುವ hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        ತಾರ್ಕಿಕ apply_to_all = (indices.length == 0);
        unsigned ಅಂಕೆ_deltas = apply_to_all ? point_ಎಣಿಕೆ : indices.length;

        ಯದಿ (unlikely (!deltas_x.resize_dirty  (ಅಂಕೆ_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಯದಿ (is_gvar)
        {
          ಯದಿ (unlikely (!deltas_y.resize_dirty  (ಅಂಕೆ_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        ಯದಿ (unlikely (!var.indices.allocate_from_pool (pool, point_ಎಣಿಕೆ) ||
                      !var.deltas_x.allocate_from_pool (pool, point_ಎಣಿಕೆ, ಅಸತ್ಯ)))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಯದಿ (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_ಎಣಿಕೆ, ಅಸತ್ಯ)))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          ಯದಿ (idx >= point_ಎಣಿಕೆ) ಅನುವರ್ತ;
          var.indices[idx] = ಸತ್ಯ;
          var.deltas_x[idx] = deltas_x[i];
          ಯದಿ (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } ಯಾವತ್ (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ create_from_item_var_data (ಧ್ರುವ VarData &var_data,
                                    ಧ್ರುವ hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    ಧ್ರುವ hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_ಎಣಿಕೆ_2,
                                    ಧ್ರುವ hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      ಯದಿ (&var_data == &Null (VarData))
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      unsigned ಅಂಕೆ_regions = var_data.get_region_index_ಎಣಿಕೆ ();
      ಯದಿ (!tuple_vars.alloc (ಅಂಕೆ_regions)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      item_ಎಣಿಕೆ_2 = inner_map ? inner_map->get_population () : var_data.get_item_ಎಣಿಕೆ ();
      ಯದಿ (!item_ಎಣಿಕೆ_2) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      unsigned row_size = var_data.get_row_size ();
      ಧ್ರುವ HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      ಪ್ರತಿ (unsigned r = 0; r < ಅಂಕೆ_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        ಯದಿ (!tuple.deltas_x.resize_dirty  (item_ಎಣಿಕೆ_2) ||
            !tuple.indices.resize_dirty  (item_ಎಣಿಕೆ_2))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಪ್ರತಿ (unsigned i = 0; i < item_ಎಣಿಕೆ_2; i++)
        {
          tuple.indices.arrayZ[i] = ಸತ್ಯ;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        ಯದಿ (region_index >= regions.length) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      ಪ್ರತ್ಯಾಗಮ !tuple_vars.in_error ();
    }

    ಗುಪ್ತ:
    ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ _cmp_axis_tag (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
    {
      ಧ್ರುವ hb_tag_t *a = (ಧ್ರುವ hb_tag_t*) pa;
      ಧ್ರುವ hb_tag_t *b = (ಧ್ರುವ hb_tag_t*) pb;
      ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)(*a) - (ಪೂರ್ಣಾಙ್ಕ)(*b);
    }

    ತಾರ್ಕಿಕ change_tuple_variations_axis_limits (ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              ಧ್ರುವ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      ಯದಿ (!axis_tags.alloc (normalized_axes_location.get_population ()))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತಿ (auto t : normalized_axes_location.ಕೀಲಿಗಳು ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      ಪ್ರತಿ (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        ಯದಿ (!normalized_axes_location.has (axis_tag, &axis_limit))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        TripleDistances axis_triple_distances{1.f, 1.f};
        ಯದಿ (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        ಪ್ರತಿ (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          ಯದಿ (!out) ಅನುವರ್ತ;

          unsigned new_len = new_vars.length + out.length;

          ಯದಿ (unlikely (!new_vars.alloc (new_len, ಅಸತ್ಯ)))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

          ಪ್ರತಿ (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    ತಾರ್ಕಿಕ merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      ಯದಿ (unlikely (!new_vars.alloc (tuple_vars.length)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      hb_hashmap_t<ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      ಪ್ರತಿ (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        ಯದಿ (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          ಯದಿ (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          ಅನುವರ್ತ;
        }

        unsigned *idx;
        ಯದಿ (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        ಅನ್ಯಥಾ
        {
	  auto *new_var = new_vars.push ();
	  ಯದಿ (unlikely (new_vars.in_error ()))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          hb_swap (*new_var, var);
          ಯದಿ (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    ತಾರ್ಕಿಕ compile_all_point_sets ()
    {
      ಪ್ರತಿ (ಧ್ರುವ auto& tuple: tuple_vars)
      {
        ಧ್ರುವ hb_vector_t<ತಾರ್ಕಿಕ>* points_set = &(tuple.indices);
        ಯದಿ (point_data_map.has (points_set))
        {
          unsigned *ಎಣಿಕೆ;
          ಯದಿ (unlikely (!point_set_ಎಣಿಕೆ_map.has (points_set, &ಎಣಿಕೆ) ||
                        !point_set_ಎಣಿಕೆ_map.set (points_set, (*ಎಣಿಕೆ) + 1)))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          ಅನುವರ್ತ;
        }

        hb_vector_t<unsigned ವರ್ಣ> compiled_point_data;
        ಯದಿ (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಯದಿ (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_ಎಣಿಕೆ_map.set (points_set, 1))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    /* find shared points set which saves most bytes */
    ಶೂನ್ಯ find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      ಪ್ರತಿ (ಧ್ರುವ auto& _ : point_data_map.iter_ref ())
      {
        ಧ್ರುವ hb_vector_t<ತಾರ್ಕಿಕ>* points_set = _.first;
        unsigned data_length = _.second.length;
        ಯದಿ (!data_length) ಅನುವರ್ತ;
        unsigned *ಎಣಿಕೆ;
        ಯದಿ (unlikely (!point_set_ಎಣಿಕೆ_map.has (points_set, &ಎಣಿಕೆ) ||
                      *ಎಣಿಕೆ <= 1))
        {
          shared_points_bytes = nullptr;
          ಪ್ರತ್ಯಾಗಮ;
        }

        unsigned saved_bytes = data_length * ((*ಎಣಿಕೆ) -1);
        ಯದಿ (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    ತಾರ್ಕಿಕ calc_inferred_deltas (ಧ್ರುವ contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      ಪ್ರತಿ (tuple_delta_t& var : tuple_vars)
        ಯದಿ (!var.calc_inferred_deltas (contour_points, scratch))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ iup_optimize (ಧ್ರುವ contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      ಪ್ರತಿ (tuple_delta_t& var : tuple_vars)
      {
        ಯದಿ (!var.optimize (contour_points, is_composite, scratch))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಸಾರ್ವಜನಿಕ:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    ಶೂನ್ಯ cull_unreachable (ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      ಪ್ರತಿ (unsigned i = 0; i < tuple_vars.length; i++)
      {
	ಯದಿ (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  ಅನುವರ್ತ;
	ಯದಿ (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    ತಾರ್ಕಿಕ instantiate (ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      ಧ್ರುವ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      ತಾರ್ಕಿಕ optimize = ಅಸತ್ಯ)
    {
      ಯದಿ (!tuple_vars) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಯದಿ (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      /* compute inferred deltas only for gvar */
      ಯದಿ (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	ಯದಿ (!calc_inferred_deltas (*contour_points, scratch))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      /* if iup delta opt is on, contour_points can't be null */
      ಯದಿ (optimize && !contour_points)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (!merge_tuple_variations (optimize ? contour_points : nullptr))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (optimize && !iup_optimize (*contour_points, scratch)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ !tuple_vars.in_error ();
    }

    ತಾರ್ಕಿಕ compile_bytes (ಧ್ರುವ hb_map_t& axes_index_map,
                        ಧ್ರುವ hb_map_t& axes_old_index_tag_map,
                        ತಾರ್ಕಿಕ use_shared_points,
                        ತಾರ್ಕಿಕ is_gvar = ಅಸತ್ಯ,
                        ಧ್ರುವ hb_hashmap_t<ಧ್ರುವ hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      ಯದಿ (!tuple_vars)
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      // compile points set and store data in hashmap
      ಯದಿ (!compile_all_point_sets ())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      ಯದಿ (use_shared_points)
      {
        find_shared_points ();
        ಯದಿ (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      ಪ್ರತಿ (auto& tuple: tuple_vars)
      {
        ಧ್ರುವ hb_vector_t<ತಾರ್ಕಿಕ>* points_set = &(tuple.indices);
        hb_vector_t<unsigned ವರ್ಣ> *points_data;
        ಯದಿ (unlikely (!point_data_map.has (points_set, &points_data)))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        ಯದಿ (!points_data->length)
          ಅನುವರ್ತ;
        ಯದಿ (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        ಯದಿ (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      ಯದಿ (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = ಸತ್ಯ;
        compiled_byte_size += 1;
      }

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) ಧ್ರುವ
    {
      TRACE_SERIALIZE (ಏತತ್);
      ಪ್ರತಿ (ಧ್ರುವ auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        ಯದಿ (c->in_error ()) return_trace (ಅಸತ್ಯ);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (ಸತ್ಯ);
    }

    ತಾರ್ಕಿಕ serialize_var_data (hb_serialize_context_t *c, ತಾರ್ಕಿಕ is_gvar) ಧ್ರುವ
    {
      TRACE_SERIALIZE (ಏತತ್);
      ಯದಿ (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      ಪ್ರತಿ (ಧ್ರುವ auto& tuple: tuple_vars)
      {
        ಧ್ರುವ hb_vector_t<ತಾರ್ಕಿಕ>* points_set = &(tuple.indices);
        hb_vector_t<unsigned ವರ್ಣ> *point_data;
        ಯದಿ (!point_data_map.has (points_set, &point_data))
          return_trace (ಅಸತ್ಯ);

        ಯದಿ (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        ಯದಿ (c->in_error ()) return_trace (ಅಸತ್ಯ);
      }

      /* padding for gvar */
      ಯದಿ (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        ಯದಿ (!c->embed (pad)) return_trace (ಅಸತ್ಯ);
      }
      return_trace (ಸತ್ಯ);
    }
  };

  ಸಂರಚನಾ tuple_iterator_t
  {
    unsigned get_axis_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ axis_ಎಣಿಕೆ; }

    ಶೂನ್ಯ init (hb_bytes_t var_data_bytes_, unsigned ಪೂರ್ಣಾಙ್ಕ axis_ಎಣಿಕೆ_3, ಧ್ರುವ ಶೂನ್ಯ *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_ಎಡ = var_data->tuple_Var_ಎಣಿಕೆ.get_ಎಣಿಕೆ ();
      axis_ಎಣಿಕೆ = axis_ಎಣಿಕೆ_3;
      axis_ಎಣಿಕೆ_times_2 = axis_ಎಣಿಕೆ_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    ತಾರ್ಕಿಕ get_shared_indices (hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> &shared_indices /* OUT */)
    {
      ಯದಿ (var_data->has_shared_point_numbers ())
      {
        ಧ್ರುವ HBUINT8 *base = &(table_base+var_data->data);
        ಧ್ರುವ HBUINT8 *p = base;
        ಯದಿ (!decompile_points (p, shared_indices, (ಧ್ರುವ HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        data_offset = p - base;
      }
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ is_valid ()
    {
      ಯದಿ (unlikely (tuples_ಎಡ <= 0))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      current_tuple_size = TupleVariationHeader::min_size;
      ಯದಿ (unlikely (!var_data_bytes.check_end ((ಧ್ರುವ ವರ್ಣ *) current_tuple + current_tuple_size)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      current_tuple_size = current_tuple->get_size (axis_ಎಣಿಕೆ_times_2);
      ಯದಿ (unlikely (!var_data_bytes.check_end ((ಧ್ರುವ ವರ್ಣ *) current_tuple + current_tuple_size)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    HB_ALWAYS_INLINE
    ತಾರ್ಕಿಕ move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_ಎಡ--;
      ಪ್ರತ್ಯಾಗಮ is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ಧ್ರುವ HBUINT8 *get_serialized_data () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ &(table_base+var_data->data) + data_offset; }

    ಗುಪ್ತ:
    signed tuples_ಎಡ;
    ಧ್ರುವ TupleVariationData *var_data;
    unsigned ಪೂರ್ಣಾಙ್ಕ axis_ಎಣಿಕೆ;
    unsigned ಪೂರ್ಣಾಙ್ಕ axis_ಎಣಿಕೆ_times_2;
    unsigned ಪೂರ್ಣಾಙ್ಕ data_offset;
    unsigned ಪೂರ್ಣಾಙ್ಕ current_tuple_size;
    ಧ್ರುವ ಶೂನ್ಯ *table_base;

    ಸಾರ್ವಜನಿಕ:
    hb_bytes_t var_data_bytes;
    ಧ್ರುವ TupleVariationHeader *current_tuple;
  };

  ಸ್ಥಿರ ತಾರ್ಕಿಕ get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_ಎಣಿಕೆ,
                                  ಧ್ರುವ ಶೂನ್ಯ *table_base,
                                  hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_ಎಣಿಕೆ, table_base);
    ಯದಿ (!iterator->get_shared_indices (shared_indices))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ iterator->is_valid ();
  }

  ತಾರ್ಕಿಕ has_shared_point_numbers () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tuple_Var_ಎಣಿಕೆ.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  ತಾರ್ಕಿಕ cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_ಎಣಿಕೆ,
			      hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples,
			      ಧ್ರುವ hb_map_t *axes_old_index_tag_map,
			      ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<ವರ್ಣ> &out /* OUT */,
			      ತಾರ್ಕಿಕ *changed /* OUT */) ಧ್ರುವ
  {
    *changed = ಅಸತ್ಯ;

    hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> shared_indices;
    tuple_iterator_t iterator;
    ಯದಿ (!get_tuple_iterator (var_data_bytes, axis_ಎಣಿಕೆ, ಏತತ್,
			     shared_indices, &iterator))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* no tuples or malformed: keep original */

    ಧ್ರುವ ವರ್ಣ *bytes_start = var_data_bytes.arrayZ;
    ಧ್ರುವ ವರ್ಣ *bytes_end = bytes_start + var_data_bytes.length;
    ಧ್ರುವ ವರ್ಣ *serialized_base = (ಧ್ರುವ ವರ್ಣ *) &(ಏತತ್+data);
    ಧ್ರುವ ವರ್ಣ *shared_points_end = (ಧ್ರುವ ವರ್ಣ *) iterator.get_serialized_data ();
    ಯದಿ (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಸಂರಚನಾ kept_tuple_t
    {
      ಧ್ರುವ ವರ್ಣ *header;
      unsigned header_size;
      ಧ್ರುವ ವರ್ಣ *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    ಕುರು
    {
      ಧ್ರುವ TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_ಎಣಿಕೆ * 2);
      unsigned data_size = header->get_data_size ();
      ಧ್ರುವ ವರ್ಣ *tuple_data = (ಧ್ರುವ ವರ್ಣ *) iterator.get_serialized_data ();
      ಯದಿ (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      ಯದಿ (!header->unpack_axis_tuples (axis_ಎಣಿಕೆ, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      total++;
      ಯದಿ (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ಧ್ರುವ ವರ್ಣ *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } ಯಾವತ್ (iterator.move_to_next ());

    ಯದಿ (unlikely (kept.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (kept.length == total) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* nothing to cull */

    *changed = ಸತ್ಯ;
    ಯದಿ (!kept.length) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    ಯದಿ (unlikely (!out.resize (new_size)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_ಎಣಿಕೆ = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    ವರ್ಣ *p = out.arrayZ + min_size;
    ಪ್ರತಿ (ಧ್ರುವ auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    ಪ್ರತಿ (ಧ್ರುವ auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ decompile_points (ಧ್ರುವ HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> &points /* OUT */,
				ಧ್ರುವ HBUINT8 *end)
  {
    ಗಣನಾ packed_point_flag_t
    {
      POINTS_ARE_ಪದಗಳು     = 0x80,
      POINT_RUN_ಎಣಿಕೆ_MASK = 0x7F
    };

    ಯದಿ (unlikely (p + 1 > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ಎಣಿಕೆ = *p++;
    ಯದಿ (ಎಣಿಕೆ & POINTS_ARE_ಪದಗಳು)
    {
      ಯದಿ (unlikely (p + 1 > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಎಣಿಕೆ = ((ಎಣಿಕೆ & POINT_RUN_ಎಣಿಕೆ_MASK) << 8) | *p++;
    }
    ಯದಿ (unlikely (!points.resize_dirty  (ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned n = 0;
    unsigned i = 0;
    ಯಾವತ್ (i < ಎಣಿಕೆ)
    {
      ಯದಿ (unlikely (p + 1 > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      unsigned control = *p++;
      unsigned run_ಎಣಿಕೆ = (control & POINT_RUN_ಎಣಿಕೆ_MASK) + 1;
      unsigned stop = i + run_ಎಣಿಕೆ;
      ಯದಿ (unlikely (stop > ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (control & POINTS_ARE_ಪದಗಳು)
      {
        ಯದಿ (unlikely (p + run_ಎಣಿಕೆ * HBUINT16::static_size > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಪ್ರತಿ (; i < stop; i++)
        {
          n += *(ಧ್ರುವ HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      ಅನ್ಯಥಾ
      {
        ಯದಿ (unlikely (p + run_ಎಣಿಕೆ > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಪ್ರತಿ (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  ಸ್ಥಿರ ತಾರ್ಕಿಕ decompile_deltas (ಧ್ರುವ HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				ಧ್ರುವ HBUINT8 *end,
				ತಾರ್ಕಿಕ consume_all = ಅಸತ್ಯ,
				unsigned start = 0)
  {
    ಪ್ರತ್ಯಾಗಮ Tuple_ಮೌಲ್ಯಗಳು::decompile (p, deltas, end, consume_all, start);
  }

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tuple_Var_ಎಣಿಕೆ; }

  ತಾರ್ಕಿಕ decompile_tuple_variations (unsigned point_ಎಣಿಕೆ,
                                   ತಾರ್ಕಿಕ is_gvar,
                                   tuple_iterator_t iterator,
                                   ಧ್ರುವ hb_map_t *axes_old_index_tag_map,
                                   ಧ್ರುವ hb_vector_t<unsigned> &shared_indices,
                                   ಧ್ರುವ hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   ತಾರ್ಕಿಕ is_composite_glyph = ಅಸತ್ಯ) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_ಎಣಿಕೆ,
                                                        point_ಎಣಿಕೆ, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  ತಾರ್ಕಿಕ is_gvar,
                  ಧ್ರುವ tuple_variations_t& tuple_variations) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    /* empty tuple variations, just return and skip serialization. */
    ಯದಿ (!tuple_variations) return_trace (ಸತ್ಯ);

    auto *out = c->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (!c->check_assign (out->tuple_Var_ಎಣಿಕೆ, tuple_variations.get_var_ಎಣಿಕೆ (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ಅಸತ್ಯ);

    unsigned total_header_len = 0;

    ಯದಿ (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (ಅಸತ್ಯ);

    unsigned data_offset = min_size + total_header_len;
    ಯದಿ (!is_gvar) data_offset += 4;
    ಯದಿ (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ಅಸತ್ಯ);

    ಪ್ರತ್ಯಾಗಮ tuple_variations.serialize_var_data (c, is_gvar);
  }

  ರಕ್ಷಿತ:
  ಸಂರಚನಾ Tuple_Var_ಎಣಿಕೆ : HBUINT16
  {
    friend ಸಂರಚನಾ tuple_variations_t;
    ತಾರ್ಕಿಕ has_shared_point_numbers () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ((*ಏತತ್) & SharedPointNumbers); }
    unsigned ಪೂರ್ಣಾಙ್ಕ get_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್) & ಎಣಿಕೆ_Mask; }
    Tuple_Var_ಎಣಿಕೆ& operator = (uint16_t i) { HBUINT16::operator= (i); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
    explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_ಎಣಿಕೆ (); }

    ರಕ್ಷಿತ:
    ಗಣನಾ Flags
    {
      SharedPointNumbers= 0x8000u,
      ಎಣಿಕೆ_Mask         = 0x0FFFu
    };
    ಸಾರ್ವಜನಿಕ:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_ಎಣಿಕೆ tuple_Var_ಎಣಿಕೆ;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
ಸಂರಚನಾ item_variations_t
{
  using region_t = ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>*;
  ಗುಪ್ತ:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_data_ಅಂಕೆ_rows;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_vector_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, unsigned> region_map;

  /* all delta rows after instantiation */
  hb_vector_t<hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  ತಾರ್ಕಿಕ has_long = ಅಸತ್ಯ;

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ has_long_word () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ has_long; }

  ಧ್ರುವ hb_vector_t<region_t>& get_region_list () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ region_list; }

  ಧ್ರುವ hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ encodings; }

  ಧ್ರುವ hb_map_t& get_varidx_map () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ varidx_map; }

  ತಾರ್ಕಿಕ add_vardata_encoding_for_testing (hb_vector_t<ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*> &&rows,
                                         unsigned ಅಂಕೆ_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), ಅಂಕೆ_cols));
    ಪ್ರತ್ಯಾಗಮ !encodings.in_error ();
  }

  ತಾರ್ಕಿಕ compile_varidx_map_for_testing (ಧ್ರುವ hb_hashmap_t<unsigned, ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*>& front_mapping)
  { ಪ್ರತ್ಯಾಗಮ compile_varidx_map (front_mapping); }

  ತಾರ್ಕಿಕ instantiate (ಧ್ರುವ ItemVariationStore& varStore,
                    ಧ್ರುವ hb_subset_plan_t *plan,
                    ತಾರ್ಕಿಕ optimize=ಸತ್ಯ,
                    ತಾರ್ಕಿಕ use_no_variation_idx=ಸತ್ಯ,
                    ಧ್ರುವ hb_array_t <ಧ್ರುವ hb_inc_bimap_t> inner_maps = hb_array_t<ಧ್ರುವ hb_inc_bimap_t> (),
                    ತಾರ್ಕಿಕ layout_conditions = ಅಸತ್ಯ)
  {
    ಯದಿ (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    /* avar2 partial instancing: cull unreachable regions. */
    ಯದಿ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      ಪ್ರತಿ (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    ಯದಿ (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (layout_conditions &&
        (!instantiate_condition_ಮೌಲ್ಯಗಳು (plan, varStore) || !build_region_list ()))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ as_item_varstore (optimize, use_no_variation_idx);
  }

  ತಾರ್ಕಿಕ instantiate_condition_ಮೌಲ್ಯಗಳು (ಧ್ರುವ hb_subset_plan_t *plan,
                                      ಧ್ರುವ ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_ಮೌಲ್ಯ_map;
    condition_map.clear ();
    ಯದಿ (!plan->layout_condition_ಮೌಲ್ಯಗಳು) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    using condition_ಮೌಲ್ಯ_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>>;
    hb_vector_t<condition_ಮೌಲ್ಯ_t> conditions;
    ಪ್ರತಿ (auto condition : plan->layout_condition_ಮೌಲ್ಯಗಳು.iter ())
      conditions.push (condition);
    ಯದಿ (unlikely (conditions.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ಧ್ರುವ condition_ಮೌಲ್ಯ_t &a, ಧ್ರುವ condition_ಮೌಲ್ಯ_t &b) {
      ಯದಿ (a.second.first != b.second.first)
        ಪ್ರತ್ಯಾಗಮ a.second.first < b.second.first ? -1 : 1;
      ಪ್ರತ್ಯಾಗಮ a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    ಪ್ರತಿ (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      ಯದಿ (plan->gdef_varstore_inner_maps[i].get_population ())
        ಯದಿ (!outer_map.set (i, new_outer++)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      ದ್ವಿಗುಣ value = condition.second.second +
                     varStore.get_delta<ದ್ವಿಗುಣ> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      ಯದಿ (!outer_map.has (old_outer))
        ಅನುವರ್ತ;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      ಯದಿ (outer >= vars.length || inner >= var_data_ಅಂಕೆ_rows[outer])
        ಅನುವರ್ತ;

      ದ್ವಿಗುಣ maximum = fabs ((ದ್ವಿಗುಣ) value);
      ತಾರ್ಕಿಕ variable = ಅಸತ್ಯ;
      ಪ್ರತಿ (ಧ್ರುವ tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        ಯದಿ (unlikely (inner >= tuple.deltas_x.length)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಭಿನ್ನ delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((ದ್ವಿಗುಣ) delta));
      }
      ಯದಿ (!variable)
      {
        ಯದಿ (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), ಪೂರ್ಣಾಙ್ಕ (value > 0))))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಅನುವರ್ತ;
      }

      ದ್ವಿಗುಣ scale = 1.;
      ಯಾವತ್ (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      ಯಾವತ್ (maximum * scale * 2 <= INT_MAX)
      {
        ತಾರ್ಕಿಕ integral = (ದ್ವಿಗುಣ) value * scale == round ((ದ್ವಿಗುಣ) value * scale);
        ಪ್ರತಿ (ಧ್ರುವ tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (ದ್ವಿಗುಣ) tuple.deltas_x[inner] * scale ==
                      round ((ದ್ವಿಗುಣ) tuple.deltas_x[inner] * scale);
        ಯದಿ (integral) ಭಙ್ಗ;
        scale *= 2;
      }
      ಪೂರ್ಣಾಙ್ಕ default_ಮೌಲ್ಯ = (ಪೂರ್ಣಾಙ್ಕ) round ((ದ್ವಿಗುಣ) value * scale);
      ಯದಿ (scale != 1. || default_ಮೌಲ್ಯ < -32768 || default_ಮೌಲ್ಯ > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        ಪ್ರತಿ (ಧ್ರುವ tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          ಯದಿ (!tuple.deltas_x[inner]) ಅನುವರ್ತ;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (ಸತ್ಯ);
          copy.deltas_x.push ((ಭಿನ್ನ) round ((ದ್ವಿಗುಣ) tuple.deltas_x[inner] * scale));
          ಯದಿ (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          private_row.tuple_vars.push (std::move (copy));
        }
        ಯದಿ (unlikely (vars.length >= 0xFFFFu)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಯದಿ (unlikely (private_row.tuple_vars.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_ಅಂಕೆ_rows.push (1);
        ಯದಿ (unlikely (vars.in_error () || var_data_ಅಂಕೆ_rows.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಯದಿ (default_ಮೌಲ್ಯ < -32768 || default_ಮೌಲ್ಯ > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          ಪೂರ್ಣಾಙ್ಕ bias = default_ಮೌಲ್ಯ - hb_clamp (default_ಮೌಲ್ಯ, -32768, 32767);
          default_ಮೌಲ್ಯ -= bias;
          ಪೂರ್ಣಾಙ್ಕ ಮೇಲಿನ = bias / 65536 * 65536;
          ಪೂರ್ಣಾಙ್ಕ parts[] = {ಮೇಲಿನ, bias - ಮೇಲಿನ};
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ part : parts)
          {
            ಯದಿ (!part) ಅನುವರ್ತ;
            unsigned ಎಣಿಕೆ = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            ಧ್ರುವ auto &tuples = vars[idx >> 16].tuple_vars;
            ಯದಿ (unlikely (tuples.in_error () || tuples.length != ಎಣಿಕೆ + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          }
        }
      }
      ಯದಿ (!condition_map.set (condition.first, hb_pair (idx, default_ಮೌಲ್ಯ)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  ತಾರ್ಕಿಕ create_from_item_varstore (ಧ್ರುವ ItemVariationStore& varStore,
                                  ಧ್ರುವ hb_map_t& axes_old_index_tag_map,
                                  ಧ್ರುವ hb_array_t <ಧ್ರುವ hb_inc_bimap_t> inner_maps = hb_array_t<ಧ್ರುವ hb_inc_bimap_t> ())
  {
    ಧ್ರುವ VarRegionList& regionList = varStore.get_region_list ();
    ಯದಿ (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ಅಂಕೆ_var_data = varStore.get_sub_table_ಎಣಿಕೆ ();
    ಯದಿ (inner_maps && inner_maps.length != ಅಂಕೆ_var_data) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!vars.alloc (ಅಂಕೆ_var_data) ||
        !var_data_ಅಂಕೆ_rows.alloc (ಅಂಕೆ_var_data)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_var_data; i++)
    {
      ಯದಿ (inner_maps && !inner_maps.arrayZ[i].get_population ())
          ಅನುವರ್ತ;
      tuple_variations_t var_data_tuples;
      unsigned item_ಎಣಿಕೆ_2 = 0;
      ಯದಿ (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_ಎಣಿಕೆ_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      var_data_ಅಂಕೆ_rows.push (item_ಎಣಿಕೆ_2);
      vars.push (std::move (var_data_tuples));
    }
    ಪ್ರತ್ಯಾಗಮ !vars.in_error () && !var_data_ಅಂಕೆ_rows.in_error () && vars.length == var_data_ಅಂಕೆ_rows.length;
  }

  ತಾರ್ಕಿಕ instantiate_tuple_vars (ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               ಧ್ರುವ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               ತಾರ್ಕಿಕ build_regions = ಸತ್ಯ)
  {
    optimize_scratch_t scratch;
    ಪ್ರತಿ (tuple_variations_t& tuple_vars : vars)
      ಯದಿ (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_ಎಣಿಕೆ_2)
  {
    vars.push (tuple_variations_t ());
    var_data_ಅಂಕೆ_rows.push (item_ಎಣಿಕೆ_2);
    ಪ್ರತ್ಯಾಗಮ vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_ಎಣಿಕೆ (unsigned outer) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ outer < var_data_ಅಂಕೆ_rows.length ? var_data_ಅಂಕೆ_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    ಯದಿ (unlikely (outer >= vars.length ||
                  inner >= var_data_ಅಂಕೆ_rows[outer] ||
                  var_data_ಅಂಕೆ_rows[outer] >= 0xFFFFu))
      ಪ್ರತ್ಯಾಗಮ (unsigned) -1;
    ಪ್ರತಿ (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      ಯದಿ (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        ಪ್ರತ್ಯಾಗಮ (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      ಯದಿ (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      ಯದಿ (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        ಪ್ರತ್ಯಾಗಮ (unsigned) -1;
    }
    ಪ್ರತ್ಯಾಗಮ var_data_ಅಂಕೆ_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  ಶೂನ್ಯ add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, ಪೂರ್ಣಾಙ್ಕ delta, unsigned item_ಎಣಿಕೆ_2)
  {
    ಯದಿ (outer >= vars.length) ಪ್ರತ್ಯಾಗಮ;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    ಯದಿ (!tuple.indices.resize (item_ಎಣಿಕೆ_2) ||
        !tuple.deltas_x.resize (item_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ (unsigned i = 0; i < item_ಎಣಿಕೆ_2; i++)
    {
      tuple.indices.arrayZ[i] = ಸತ್ಯ;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (ಭಿನ್ನ) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  ತಾರ್ಕಿಕ build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    ಪ್ರತಿ (ಧ್ರುವ tuple_variations_t& sub_table : vars)
    {
      ಪ್ರತಿ (ಧ್ರುವ tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        ಯದಿ (!used_regions.has (r))
        {
          ತಾರ್ಕಿಕ all_zeros = ಸತ್ಯ;
          ಪ್ರತಿ (ಭಿನ್ನ d : tuple.deltas_x)
          {
            ಪೂರ್ಣಾಙ್ಕ delta = hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (roundf ((ದ್ವಿಗುಣ) d));
            ಯದಿ (delta != 0)
            {
              all_zeros = ಅಸತ್ಯ;
              ಭಙ್ಗ;
            }
          }
          ಯದಿ (!all_zeros)
          {
            ಯದಿ (!used_regions.set (r, 1))
              ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          }
        }
        ಯದಿ (all_regions.has (r))
          ಅನುವರ್ತ;
        ಯದಿ (!all_regions.set (r, 1))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    ಯದಿ (!all_regions || !all_unique_regions) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಯದಿ (!region_list.alloc (all_regions.get_population ()))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    ಪ್ರತಿ (ಧ್ರುವ auto& r : orig_region_list)
    {
      ಯದಿ (!all_regions.has (&r) || !used_regions.has (&r))
        ಅನುವರ್ತ;

      region_list.push (&r);
      ಯದಿ (!region_map.set (&r, idx))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    ಪ್ರತಿ (ಧ್ರುವ auto& r: all_unique_regions)
    {
      ಯದಿ (!all_regions.has (r) || !used_regions.has (r))
        ಅನುವರ್ತ;
      region_list.push (r);
      ಯದಿ (!region_map.set (r, idx))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      all_regions.del (r);
      idx++;
    }
    ಪ್ರತ್ಯಾಗಮ (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  ಸಂರಚನಾ combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = ಸ್ವಭಾವ;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      ಪ್ರತಿಜ್ಞಾ (gain < 0xFFFFFF);
      ಪ್ರತಿಜ್ಞಾ (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    ತಾರ್ಕಿಕ operator < (ಧ್ರುವ combined_gain_idx_tuple_t& o)
    {
      ಪ್ರತ್ಯಾಗಮ encoded < o.encoded;
    }

    ತಾರ್ಕಿಕ operator <= (ಧ್ರುವ combined_gain_idx_tuple_t& o)
    {
      ಪ್ರತ್ಯಾಗಮ encoded <= o.encoded;
    }

    unsigned idx_1 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ encoded & 0xFFFFF; };
  };

  ತಾರ್ಕಿಕ as_item_varstore (ತಾರ್ಕಿಕ optimize=ಸತ್ಯ, ತಾರ್ಕಿಕ use_no_variation_idx=ಸತ್ಯ)
  {
    /* return true if no variation data */
    ಯದಿ (!region_list) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    unsigned ಅಂಕೆ_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    ಪ್ರತಿ (unsigned major = 0; major < var_data_ಅಂಕೆ_rows.length; major++)
      total_rows += var_data_ಅಂಕೆ_rows[major];

    ಯದಿ (!delta_rows.resize (total_rows)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    /* init all rows to [0]*num_cols */
    ಪ್ರತಿ (unsigned i = 0; i < total_rows; i++)
      ಯದಿ (!(delta_rows[i].resize (ಅಂಕೆ_cols))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> *> major_rows;
    hb_hashmap_t<ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*, unsigned> delta_rows_map;
    ಪ್ರತಿ (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ಧ್ರುವ tuple_variations_t& tuples = vars[major];
      unsigned ಅಂಕೆ_rows = var_data_ಅಂಕೆ_rows[major];

      ಯದಿ (!ಅಂಕೆ_rows) ಅನುವರ್ತ;

      ಪ್ರತಿ (ಧ್ರುವ tuple_delta_t& tuple: tuples.tuple_vars)
      {
        ಯದಿ (tuple.deltas_x.length != ಅಂಕೆ_rows)
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        /* skip unused regions */
        unsigned *col_idx;
        ಯದಿ (!region_map.has (&(tuple.axis_tuples), &col_idx))
          ಅನುವರ್ತ;

        ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_rows; i++)
        {
          ಪೂರ್ಣಾಙ್ಕ rounded_delta = hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (roundf ((ದ್ವಿಗುಣ) tuple.deltas_x[i]));
          ಪೂರ್ಣಾಙ್ಕ &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          ಯದಿ (unlikely (sum < INT_MIN || sum > INT_MAX)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          delta = (ಪೂರ್ಣಾಙ್ಕ) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      ಪ್ರತಿ (unsigned minor = 0; minor < ಅಂಕೆ_rows; minor++)
      {
	ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>& row = delta_rows[start_row + minor];
	ಯದಿ (use_no_variation_idx)
	{
	  ತಾರ್ಕಿಕ all_zeros = ಸತ್ಯ;
	  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ delta : row)
	  {
	    ಯದಿ (delta != 0)
	    {
	      all_zeros = ಅಸತ್ಯ;
	      ಭಙ್ಗ;
	    }
	  }
	  ಯದಿ (all_zeros)
	    ಅನುವರ್ತ;
	}

	ಯದಿ (!front_mapping.set ((major<<16) + minor, &row))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	ಯದಿ (optimize)
	{
	  ಯದಿ (delta_rows_map.has (&row))
	    ಅನುವರ್ತ;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      ಯದಿ (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), ಅಂಕೆ_cols);
	ಯದಿ (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      start_row += ಅಂಕೆ_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    ಯದಿ (!optimize)
    {
      encodings = std::move (encoding_objs);
      ಪ್ರತ್ಯಾಗಮ !encodings.in_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.qsort ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    using item_t = hb_priority_queue_t<combined_gain_idx_tuple_t>::item_t;
    hb_vector_t<item_t> queue_ವಸ್ತುಗಳು;
    unsigned ಅಂಕೆ_todos = encoding_objs.length;
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_todos; i++)
    {
      ಪ್ರತಿ (unsigned j = i + 1; j < ಅಂಕೆ_todos; j++)
      {
        ಪೂರ್ಣಾಙ್ಕ combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        ಯದಿ (combining_gain > 0)
          queue_ವಸ್ತುಗಳು.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_ವಸ್ತುಗಳು));

    hb_bit_set_t removed_todo_idxes;
    ಯಾವತ್ (queue)
    {
      auto t = queue.ತೆಗೆ_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      ಯದಿ (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        ಅನುವರ್ತ;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      ಯದಿ (unlikely (removed_todo_idxes.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      encoding.merge (other_encoding);
      ಯದಿ (unlikely (encoding.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪ್ರತಿ (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        ಯದಿ (removed_todo_idxes.has (idx)) ಅನುವರ್ತ;

        ಧ್ರುವ delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        ಯದಿ (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          ಪ್ರತಿ (ಧ್ರುವ auto& row : obj.ವಸ್ತುಗಳು)
            encoding.add_row (row);
          ಯದಿ (unlikely (encoding.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

          removed_todo_idxes.add (idx);
          ಯದಿ (unlikely (removed_todo_idxes.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          ಅನುವರ್ತ;
        }

        ಪೂರ್ಣಾಙ್ಕ combined_gain = encoding.gain_from_merging (obj);
        ಯದಿ (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      ಯದಿ (unlikely (encoding_objs.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_final_encodings = (ಪೂರ್ಣಾಙ್ಕ) encoding_objs.length - (ಪೂರ್ಣಾಙ್ಕ) removed_todo_idxes.get_population ();
    ಯದಿ (ಅಂಕೆ_final_encodings <= 0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (!encodings.alloc (ಅಂಕೆ_final_encodings)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned i = 0; i < encoding_objs.length; i++)
    {
      ಯದಿ (removed_todo_idxes.has (i)) ಅನುವರ್ತ;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    ಪ್ರತ್ಯಾಗಮ compile_varidx_map (front_mapping);
  }

  ಗುಪ್ತ:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  ತಾರ್ಕಿಕ compile_varidx_map (ಧ್ರುವ hb_hashmap_t<unsigned, ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    ಪ್ರತಿ (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      ಯದಿ (encoding.is_empty () || encoding.in_error ())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      unsigned ಅಂಕೆ_rows = encoding.ವಸ್ತುಗಳು.length;
      unsigned ಅಂಕೆ_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.ವಸ್ತುಗಳು.qsort (_cmp_row);

      ಪ್ರತಿ (unsigned start = 0; start < ಅಂಕೆ_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (ಅಂಕೆ_rows - start, 0xFFFFu);
        hb_vector_t<ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*> rows;

        ಯದಿ (!rows.alloc (chunk_len))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        unsigned major = split_encodings.length;
        ಪ್ರತಿ (unsigned minor = 0; minor < chunk_len; minor++)
        {
          ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> *row = encoding.ವಸ್ತುಗಳು.arrayZ[start + minor];
          rows.push (row);
          ಯದಿ (!back_mapping.set (row, (major << 16) + minor))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }

        auto *split = split_encodings.push (std::move (rows), ಅಂಕೆ_cols);
        ಯದಿ (unlikely (split_encodings.in_error () || split->in_error ()))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }

    encodings = std::move (split_encodings);
    ಯದಿ (encodings.in_error () || back_mapping.in_error ())
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      ಯದಿ (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      ಅನ್ಯಥಾ
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    ಪ್ರತ್ಯಾಗಮ !varidx_map.in_error ();
  }

  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ _cmp_row (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>** a = (ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>**) pa;
    ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>** b = (ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>**) pb;

    ಪ್ರತಿ (unsigned i = 0; i < (*b)->length; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ va = (*a)->arrayZ[i];
      ಪೂರ್ಣಾಙ್ಕ vb = (*b)->arrayZ[i];
      ಯದಿ (va != vb)
        ಪ್ರತ್ಯಾಗಮ va < vb ? -1 : 1;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

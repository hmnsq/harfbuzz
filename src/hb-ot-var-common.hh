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

#include "hb-ot-layout-common.hh"
#include "hb-alloc-pool.hh"
#include "hb-priority-queue.hh"
#include "hb-subset-instancer-iup.hh"


namespace OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
struct TupleVariationHeader
{
  friend struct tuple_delta_t;
  size_t get_size (unsigned axis_count_times_2) const
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    якщо (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned count = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      повернути hb_unsigned_mul_add_saturate (count, axis_count_times_2, min_size);
    }
    повернути min_size;
  }

  unsigned get_data_size () const { повернути varDataSize; }

  const TupleVariationHeader &get_next (unsigned axis_count_times_2) const
  { повернути StructAtOffset<TupleVariationHeader> (this, get_size (axis_count_times_2)); }

  bool unpack_axis_tuples (unsigned axis_count,
                           const hb_array_t<const F2DOT14> shared_tuples,
                           const hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) const
  {
    const F2DOT14 *peak_tuple = nullptr;
    якщо (has_peak ())
      peak_tuple = get_peak_tuple (axis_count);
    інакше
    {
      unsigned ціле index = get_index ();
      якщо (unlikely ((index + 1) * axis_count > shared_tuples.length))
        повернути false;
      peak_tuple = shared_tuples.sub_array (axis_count * index, axis_count).arrayZ;
    }

    const F2DOT14 *start_tuple = nullptr;
    const F2DOT14 *end_tuple = nullptr;
    bool has_interm = has_intermediate ();

    якщо (has_interm)
    {
      start_tuple = get_start_tuple (axis_count);
      end_tuple = get_end_tuple (axis_count);
    }

    для (unsigned i = 0; i < axis_count; i++)
    {
      float peak = peak_tuple[i].to_float ();
      якщо (peak == 0.f) continue;

      hb_tag_t *axis_tag;
      якщо (!axes_old_index_tag_map->has (i, &axis_tag))
        повернути false;

      float start, end;
      якщо (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      інакше
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    повернути true;
  }

  HB_ALWAYS_INLINE
  float calculate_scalar (hb_array_t<const ціле> coords, unsigned ціле coord_count,
			   const hb_array_t<const F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) const
  {
    unsigned tuple_index = tupleIndex;
    unsigned ціле index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    bool plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    якщо (likely (plain_shared_tuple))
    {
      float scalar;
      якщо (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	повернути scalar;
    }

    const F2DOT14 *peak_tuple;

    bool has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    якщо (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_count);
      shared_tuple_scalar_cache = nullptr;
    }
    інакше
    {
      float scalar;
      якщо (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        якщо (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	інакше
	  повернути scalar;
      }

      якщо (unlikely ((index + 1) * coord_count > shared_tuples.length))
        повернути 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_count * index);

    }

    const F2DOT14 *start_tuple = nullptr;
    const F2DOT14 *end_tuple = nullptr;

    якщо (has_interm)
    {
      start_tuple = get_start_tuple (coord_count);
      end_tuple = get_end_tuple (coord_count);
    }

    float scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#якщо HB_FAST_NUM_ACCESS
    bool skip = coord_count >= 16;
#endif
#endif
    для (unsigned ціле i = 0; i < coord_count; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#якщо HB_FAST_NUM_ACCESS
      якщо (skip)
      {
	поки (i + 4 <= coord_count && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	поки (i < coord_count && peak_tuple[i].to_int () == 0)
	  i += 1;
	якщо (i >= coord_count)
	  break;
      }
#endif
#endif

      ціле peak = peak_tuple[i].to_int ();
      якщо (!peak) continue;

      ціле v = coords[i];
      якщо (!v) { scalar = 0.f; break; }
      якщо (v == peak) continue;

      якщо (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        ціле start = start_tuple[i].to_int ();
        ціле end = end_tuple[i].to_int ();
        якщо (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) continue;
        якщо (v < start || v > end) { scalar = 0.f; break; }
        якщо (v < peak)
        { якщо (peak != start) scalar *= (float) (v - start) / (peak - start); }
        інакше
        { якщо (peak != end) scalar *= (float) (end - v) / (end - peak); }
      }
      інакше якщо (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; break; }
      інакше
        scalar *= (float) v / peak;
    }
    якщо (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    повернути scalar;
  }

  bool           has_peak () const { повернути tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  bool   has_intermediate () const { повернути tupleIndex & TupleIndex::IntermediateRegion; }
  bool has_private_points () const { повернути tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () const { повернути tupleIndex & TupleIndex::TupleIndexMask; }
  static constexpr unsigned max_shared_tuple_count = 0x1000u;

  protected:
  struct TupleIndex : HBUINT16
  {
    enum Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); повернути *this; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<const F2DOT14> get_all_tuples (unsigned axis_count) const
  { повернути StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_count); }
  const F2DOT14* get_all_tuples_base (unsigned axis_count) const
  { повернути StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  const F2DOT14* get_peak_tuple (unsigned axis_count) const
  { повернути get_all_tuples_base (axis_count); }
  const F2DOT14* get_start_tuple (unsigned axis_count) const
  { повернути get_all_tuples_base (axis_count) + has_peak () * axis_count; }
  const F2DOT14* get_end_tuple (unsigned axis_count) const
  { повернути get_all_tuples_base (axis_count) + has_peak () * axis_count + axis_count; }

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
  public:
  DEFINE_SIZE_MIN (4);
};

struct optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<bool> opt_indices;
  hb_vector_t<ціле> rounded_x_deltas;
  hb_vector_t<ціле> rounded_y_deltas;
  hb_vector_t<float> opt_deltas_x;
  hb_vector_t<float> opt_deltas_y;
  hb_vector_t<unsigned char> opt_point_data;
  hb_vector_t<unsigned char> opt_deltas_data;
  hb_vector_t<unsigned char> point_data;
  hb_vector_t<unsigned char> deltas_data;
  hb_vector_t<ціле> rounded_deltas;
};

struct tuple_delta_t
{
  static constexpr bool realloc_move = true;  // Watch out when adding new members!

  public:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<bool> indices;

  hb_vector_t<float> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<float> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned char> compiled_tuple_header;
  hb_vector_t<unsigned char> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (const tuple_delta_t& o) = default;
  tuple_delta_t& operator = (const tuple_delta_t& o) = default;

  friend void swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*this, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*this, o);
    повернути *this;
  }

  void copy_from (const tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  void remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  bool set_tent (hb_tag_t axis_tag, Triple tent)
  { повернути axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (const tuple_delta_t& o)
  {
    unsigned num = indices.length;
    для (unsigned i = 0; i < num; i++)
    {
      якщо (indices.arrayZ[i])
      {
        якщо (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          якщо (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      інакше
      {
        якщо (!o.indices.arrayZ[i]) continue;
        indices.arrayZ[i] = true;
        deltas_x[i] = o.deltas_x[i];
        якщо (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    повернути *this;
  }

  tuple_delta_t& operator *= (float scalar)
  {
    якщо (scalar == 1.0f)
      повернути *this;

    unsigned num = indices.length;
    якщо (deltas_y)
      для (unsigned i = 0; i < num; i++)
      {
	якщо (!indices.arrayZ[i]) continue;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    інакше
      для (unsigned i = 0; i < num; i++)
      {
	якщо (!indices.arrayZ[i]) continue;
	deltas_x[i] *= scalar;
      }
    повернути *this;
  }

  void change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    якщо (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*this));
      повернути;
    }

    якщо ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->middle && tent->middle <= tent->maximum))
      повернути;

    якщо (tent->middle == 0.f)
    {
      out.push (std::move (*this));
      повернути;
    }

    якщо (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.middle &&
          axis_limit.middle <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      повернути;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    для (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      якщо (i < solutions.length - 1)
	new_var.copy_from (*this, pool);
      інакше
	new_var = std::move (*this);

      якщо (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      інакше
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  bool compile_coords (const hb_map_t& axes_index_map,
		       const hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_count = axes_index_map.get_population ();
    якщо (pool)
    {
      якщо (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_count)))
	повернути false;
    }
    інакше якщо (unlikely (!compiled_peak_coords.resize (cur_axis_count)))
      повернути false;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_count = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    для (unsigned i = 0; i < orig_axis_count; i++)
    {
      якщо (!axes_index_map.has (i))
        continue;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      якщо (axis_tuples.has (axis_tag, &coords))
      {
	float min_val = coords->minimum;
	float val = coords->middle;
	float max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	якщо (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  якщо (!compiled_interm_coords)
	  {
	    якщо (pool)
	    {
	      якщо (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_count)))
		повернути false;
	    }
	    інакше якщо (unlikely (!compiled_interm_coords.resize (2 * cur_axis_count)))
	      повернути false;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_count);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_count);

	    для (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      якщо (!peak) continue;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	якщо (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    повернути !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  bool compile_tuple_var_header (const hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 const hb_map_t& axes_old_index_tag_map,
                                 const hb_hashmap_t<const hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    якщо (!compiled_deltas) повернути true;

    unsigned cur_axis_count = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_count * (F2DOT14::static_size) + 4;
    якщо (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, false))) повернути false;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    якщо (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_count = 0;
    unsigned *shared_tuple_idx;
    якщо (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    інакше
    {
      peak_count = encode_peak_coords(coords, flag);
      якщо (!peak_count) повернути false;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_count = encode_interm_coords (coords.sub_array (peak_count), flag);

    /* pointdata length = 0 implies "use shared points" */
    якщо (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_count + interim_count) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    повернути true;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) const
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * sizeof (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    повернути compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) const
  {
    якщо (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * sizeof (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    повернути compiled_interm_coords.length;
  }

  bool compile_deltas (hb_vector_t<ціле> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { повернути compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  static bool compile_deltas (hb_array_t<const bool> point_indices,
			      hb_array_t<const float> x_deltas,
			      hb_array_t<const float> y_deltas,
			      hb_vector_t<unsigned char> &compiled_deltas, /* OUT */
			      hb_vector_t<ціле> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    якщо (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      повернути false;

    unsigned j = 0;
    для (unsigned i = 0; i < point_indices.length; i++)
    {
      якщо (!point_indices[i]) continue;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<ціле> (roundf ((double) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    якщо (!rounded_deltas) повернути true;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    якщо (y_deltas)
      alloc_len *= 2;

    якщо (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, false))) повернути false;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    якщо (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      для (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        якщо (!point_indices[idx]) continue;
        ціле rounded_delta = hb_clamp_to<ціле> (roundf ((double) y_deltas.arrayZ[idx]));

        якщо (j >= rounded_deltas.length) повернути false;

        rounded_deltas[j++] = rounded_delta;
      }

      якщо (j != rounded_deltas.length) повернути false;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    повернути true;
  }

  static unsigned compile_deltas (hb_array_t<unsigned char> encoded_bytes,
				  hb_array_t<const ціле> deltas)
  {
    повернути TupleValues::compile_unsafe (deltas, encoded_bytes);
  }

  bool calc_inferred_deltas (const contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_count = orig_points.length;
    якщо (point_count != indices.length)
      повернути false;

    unsigned ref_count = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    для (unsigned i = 0; i < point_count; i++)
    {
      ref_count += indices.arrayZ[i];
      якщо (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    якщо (ref_count == point_count)
      повернути true;
    якщо (unlikely (end_points.in_error ())) повернути false;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    для (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_count = 0;
      для (unsigned i = start_point; i < end_point + 1; i++)
        unref_count += indices.arrayZ[i];
      unref_count = (end_point - start_point + 1) - unref_count;

      unsigned j = start_point;
      якщо (unref_count == 0 || unref_count > end_point - start_point)
        goto no_more_gaps;
      для (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned ціле prev, next, i;
        для (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          якщо (indices.arrayZ[i] && !indices.arrayZ[j]) break;
        }
        prev = j = i;
        для (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          якщо (!indices.arrayZ[i] && indices.arrayZ[j]) break;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        для (;;)
        {
          i = next_index (i, start_point, end_point);
          якщо (i == next) break;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          якщо (--unref_count == 0) goto no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    для (unsigned i = 0; i < point_count; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      якщо ( !indices[i])
      {
        якщо (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = true;
      }
    }
    повернути true;
  }

  bool optimize (const contour_point_vector_t& contour_points,
                 bool is_composite,
		 optimize_scratch_t &scratch,
                 double tolerance = 0.5 + 1e-10)
  {
    unsigned count = contour_points.length;
    якщо (deltas_x.length != count ||
        deltas_y.length != count)
      повернути false;

    hb_vector_t<bool> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<ціле> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<ціле> &rounded_y_deltas = scratch.rounded_y_deltas;

    якщо (unlikely (!rounded_x_deltas.resize_dirty  (count) ||
                  !rounded_y_deltas.resize_dirty  (count)))
      повернути false;

    для (unsigned i = 0; i < count; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<ціле> (roundf ((double) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<ціле> (roundf ((double) deltas_y.arrayZ[i]));
    }

    якщо (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      повернути false;

    unsigned ref_count = 0;
    для (bool ref_flag : opt_indices)
       ref_count += ref_flag;

    якщо (ref_count == count) повернути true;

    hb_vector_t<float> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<float> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    bool is_comp_glyph_wo_deltas = (is_composite && ref_count == 0);
    якщо (is_comp_glyph_wo_deltas)
    {
      якщо (unlikely (!opt_deltas_x.resize (count) ||
                    !opt_deltas_y.resize (count)))
        повернути false;

      opt_indices.arrayZ[0] = true;
      для (unsigned i = 1; i < count; i++)
        opt_indices.arrayZ[i] = false;
    }

    hb_vector_t<unsigned char> &opt_point_data = scratch.opt_point_data.reset ();
    якщо (!compile_point_set (opt_indices, opt_point_data))
      повернути false;
    hb_vector_t<unsigned char> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    якщо (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      повернути false;

    hb_vector_t<unsigned char> &point_data = scratch.point_data.reset ();
    якщо (!compile_point_set (indices, point_data))
      повернути false;
    hb_vector_t<unsigned char> &deltas_data = scratch.deltas_data.reset ();
    якщо (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      повернути false;

    якщо (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      якщо (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    повернути !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  static bool compile_point_set (const hb_vector_t<bool> &point_indices,
                                 hb_vector_t<unsigned char>& compiled_points /* OUT */)
  {
    unsigned num_points = 0;
    для (bool i : point_indices)
      якщо (i) num_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    якщо (!num_points) повернути true;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    якщо (num_points == indices_length)
      повернути compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned num_bytes = 2 + 3 *num_points;
    якщо (unlikely (!compiled_points.resize_dirty  (num_bytes)))
      повернути false;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    якщо (num_points < 0x80)
      compiled_points.arrayZ[pos++] = num_points;
    інакше
    {
      compiled_points.arrayZ[pos++] = ((num_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = num_points & 0xFF;
    }

    const unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_value = 0;
    unsigned num_encoded = 0;
    поки (i < indices_length && num_encoded < num_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      bool use_byte_encoding = false;
      bool new_run = true;
      поки (i < indices_length && num_encoded < num_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        поки (i < indices_length && !point_indices[i])
          i++;

        якщо (i >= indices_length) break;

        unsigned cur_value = i;
        unsigned delta = cur_value - last_value;

        якщо (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = false;
        }

        якщо (use_byte_encoding && delta > 0xFF)
          break;

        якщо (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        інакше
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_value = cur_value;
        run_length++;
        num_encoded++;
      }

      якщо (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      інакше
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    повернути compiled_points.resize_dirty  (pos);
  }

  static float infer_delta (float target_val, float prev_val, float next_val, float prev_delta, float next_delta)
  {
    якщо (prev_val == next_val)
      повернути (prev_delta == next_delta) ? prev_delta : 0.f;
    інакше якщо (target_val <= hb_min (prev_val, next_val))
      повернути (prev_val < next_val) ? prev_delta : next_delta;
    інакше якщо (target_val >= hb_max (prev_val, next_val))
      повернути (prev_val > next_val) ? prev_delta : next_delta;

    float r = (target_val - prev_val) / (next_val - prev_val);
    повернути prev_delta + r * (next_delta - prev_delta);
  }

  static unsigned ціле next_index (unsigned ціле i, unsigned ціле start, unsigned ціле end)
  { повернути (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
struct TupleVariationData
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (this));
  }

  size_t get_size (unsigned axis_count_times_2) const
  {
    size_t total_size = min_size;
    unsigned count = tupleVarCount.get_count ();
    const TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    для (unsigned i = 0; i < count; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_count_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_count_times_2);
    }

    повернути total_size;
  }

  const TupleVariationHeader &get_tuple_var_header (void) const
  { повернути StructAfter<TupleVariationHeader> (data); }

  struct tuple_iterator_t;
  struct tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    private:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<const hb_vector_t<bool>*, hb_vector_t<unsigned char>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<const hb_vector_t<bool>*, unsigned> point_set_count_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned char> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    bool needs_padding = false;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    bool is_composite = false;

    public:
    tuple_variations_t () = default;
    tuple_variations_t (const tuple_variations_t&) = delete;
    tuple_variations_t& operator=(const tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = default;
    tuple_variations_t& operator=(tuple_variations_t&&) = default;
    ~tuple_variations_t () = default;

    explicit operator bool () const { повернути bool (tuple_vars); }
    unsigned get_var_count () const
    {
      unsigned count = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      для (auto& tuple: tuple_vars)
        якщо (tuple.compiled_deltas) count++;

      якщо (shared_points_bytes && shared_points_bytes->length)
        count |= TupleVarCount::SharedPointNumbers;
      повернути count;
    }

    unsigned get_compiled_byte_size () const
    { повернути compiled_byte_size; }

    bool create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_count,
                                     unsigned point_count,
                                     bool is_gvar,
                                     const hb_map_t *axes_old_index_tag_map,
                                     const hb_vector_t<unsigned> &shared_indices,
                                     const hb_array_t<const F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     bool is_composite_glyph = false)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<ціле> deltas_x;
      hb_vector_t<ціле> deltas_y;
      do
      {
        const HBUINT8 *p = iterator.get_serialized_data ();
        unsigned ціле length = iterator.current_tuple->get_data_size ();
        якщо (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          повернути false;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        якщо (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_count (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          повернути false;

	private_indices.reset ();
        bool has_private_points = iterator.current_tuple->has_private_points ();
        const HBUINT8 *end = p + length;
        якщо (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          повернути false;

        const hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        bool apply_to_all = (indices.length == 0);
        unsigned num_deltas = apply_to_all ? point_count : indices.length;

        якщо (unlikely (!deltas_x.resize_dirty  (num_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          повернути false;

        якщо (is_gvar)
        {
          якщо (unlikely (!deltas_y.resize_dirty  (num_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            повернути false;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        якщо (unlikely (!var.indices.allocate_from_pool (pool, point_count) ||
                      !var.deltas_x.allocate_from_pool (pool, point_count, false)))
          повернути false;

        якщо (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_count, false)))
          повернути false;

        для (unsigned i = 0; i < num_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          якщо (idx >= point_count) continue;
          var.indices[idx] = true;
          var.deltas_x[idx] = deltas_x[i];
          якщо (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } поки (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      повернути true;
    }

    bool create_from_item_var_data (const VarData &var_data,
                                    const hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    const hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_count,
                                    const hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      якщо (&var_data == &Null (VarData))
        повернути true;

      unsigned num_regions = var_data.get_region_index_count ();
      якщо (!tuple_vars.alloc (num_regions)) повернути false;

      item_count = inner_map ? inner_map->get_population () : var_data.get_item_count ();
      якщо (!item_count) повернути true;
      unsigned row_size = var_data.get_row_size ();
      const HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      для (unsigned r = 0; r < num_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        якщо (!tuple.deltas_x.resize_dirty  (item_count) ||
            !tuple.indices.resize_dirty  (item_count))
          повернути false;

        для (unsigned i = 0; i < item_count; i++)
        {
          tuple.indices.arrayZ[i] = true;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        якщо (region_index >= regions.length) повернути false;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      повернути !tuple_vars.in_error ();
    }

    private:
    static ціле _cmp_axis_tag (const void *pa, const void *pb)
    {
      const hb_tag_t *a = (const hb_tag_t*) pa;
      const hb_tag_t *b = (const hb_tag_t*) pb;
      повернути (ціле)(*a) - (ціле)(*b);
    }

    bool change_tuple_variations_axis_limits (const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      якщо (!axis_tags.alloc (normalized_axes_location.get_population ()))
        повернути false;
      для (auto t : normalized_axes_location.keys ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      для (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        якщо (!normalized_axes_location.has (axis_tag, &axis_limit))
          повернути false;
        TripleDistances axis_triple_distances{1.f, 1.f};
        якщо (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        для (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          якщо (!out) continue;

          unsigned new_len = new_vars.length + out.length;

          якщо (unlikely (!new_vars.alloc (new_len, false)))
            повернути false;

          для (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      повернути true;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    bool merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      якщо (unlikely (!new_vars.alloc (tuple_vars.length)))
	повернути false;
      hb_hashmap_t<const hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      для (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        якщо (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          якщо (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            повернути false;
          continue;
        }

        unsigned *idx;
        якщо (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        інакше
        {
	  auto *new_var = new_vars.push ();
	  якщо (unlikely (new_vars.in_error ()))
	    повернути false;
          hb_swap (*new_var, var);
          якщо (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            повернути false;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      повернути true;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    bool compile_all_point_sets ()
    {
      для (const auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        якщо (point_data_map.has (points_set))
        {
          unsigned *count;
          якщо (unlikely (!point_set_count_map.has (points_set, &count) ||
                        !point_set_count_map.set (points_set, (*count) + 1)))
            повернути false;
          continue;
        }

        hb_vector_t<unsigned char> compiled_point_data;
        якщо (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          повернути false;

        якщо (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_count_map.set (points_set, 1))
          повернути false;
      }
      повернути true;
    }

    /* find shared points set which saves most bytes */
    void find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      для (const auto& _ : point_data_map.iter_ref ())
      {
        const hb_vector_t<bool>* points_set = _.first;
        unsigned data_length = _.second.length;
        якщо (!data_length) continue;
        unsigned *count;
        якщо (unlikely (!point_set_count_map.has (points_set, &count) ||
                      *count <= 1))
        {
          shared_points_bytes = nullptr;
          повернути;
        }

        unsigned saved_bytes = data_length * ((*count) -1);
        якщо (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    bool calc_inferred_deltas (const contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      для (tuple_delta_t& var : tuple_vars)
        якщо (!var.calc_inferred_deltas (contour_points, scratch))
          повернути false;

      повернути true;
    }

    bool iup_optimize (const contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      для (tuple_delta_t& var : tuple_vars)
      {
        якщо (!var.optimize (contour_points, is_composite, scratch))
          повернути false;
      }
      повернути true;
    }

    public:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    void cull_unreachable (const hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      для (unsigned i = 0; i < tuple_vars.length; i++)
      {
	якщо (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  continue;
	якщо (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    bool instantiate (const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      bool optimize = false)
    {
      якщо (!tuple_vars) повернути true;
      якщо (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        повернути false;
      /* compute inferred deltas only for gvar */
      якщо (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	якщо (!calc_inferred_deltas (*contour_points, scratch))
	  повернути false;
      }

      /* if iup delta opt is on, contour_points can't be null */
      якщо (optimize && !contour_points)
        повернути false;

      якщо (!merge_tuple_variations (optimize ? contour_points : nullptr))
        повернути false;

      якщо (optimize && !iup_optimize (*contour_points, scratch)) повернути false;
      повернути !tuple_vars.in_error ();
    }

    bool compile_bytes (const hb_map_t& axes_index_map,
                        const hb_map_t& axes_old_index_tag_map,
                        bool use_shared_points,
                        bool is_gvar = false,
                        const hb_hashmap_t<const hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      якщо (!tuple_vars)
        повернути true;

      // compile points set and store data in hashmap
      якщо (!compile_all_point_sets ())
        повернути false;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      якщо (use_shared_points)
      {
        find_shared_points ();
        якщо (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<ціле> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      для (auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        hb_vector_t<unsigned char> *points_data;
        якщо (unlikely (!point_data_map.has (points_set, &points_data)))
          повернути false;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        якщо (!points_data->length)
          continue;
        якщо (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          повернути false;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        якщо (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          повернути false;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      якщо (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = true;
        compiled_byte_size += 1;
      }

      повернути true;
    }

    bool serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) const
    {
      TRACE_SERIALIZE (this);
      для (const auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        якщо (c->in_error ()) return_trace (false);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (true);
    }

    bool serialize_var_data (hb_serialize_context_t *c, bool is_gvar) const
    {
      TRACE_SERIALIZE (this);
      якщо (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      для (const auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        hb_vector_t<unsigned char> *point_data;
        якщо (!point_data_map.has (points_set, &point_data))
          return_trace (false);

        якщо (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        якщо (c->in_error ()) return_trace (false);
      }

      /* padding for gvar */
      якщо (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        якщо (!c->embed (pad)) return_trace (false);
      }
      return_trace (true);
    }
  };

  struct tuple_iterator_t
  {
    unsigned get_axis_count () const { повернути axis_count; }

    void init (hb_bytes_t var_data_bytes_, unsigned ціле axis_count_, const void *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_left = var_data->tupleVarCount.get_count ();
      axis_count = axis_count_;
      axis_count_times_2 = axis_count_ * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    bool get_shared_indices (hb_vector_t<unsigned ціле> &shared_indices /* OUT */)
    {
      якщо (var_data->has_shared_point_numbers ())
      {
        const HBUINT8 *base = &(table_base+var_data->data);
        const HBUINT8 *p = base;
        якщо (!decompile_points (p, shared_indices, (const HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) повернути false;
        data_offset = p - base;
      }
      повернути true;
    }

    bool is_valid ()
    {
      якщо (unlikely (tuples_left <= 0))
	повернути false;

      current_tuple_size = TupleVariationHeader::min_size;
      якщо (unlikely (!var_data_bytes.check_end ((const char *) current_tuple + current_tuple_size)))
	повернути false;

      current_tuple_size = current_tuple->get_size (axis_count_times_2);
      якщо (unlikely (!var_data_bytes.check_end ((const char *) current_tuple + current_tuple_size)))
	повернути false;

      повернути true;
    }

    HB_ALWAYS_INLINE
    bool move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_left--;
      повернути is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    const HBUINT8 *get_serialized_data () const
    { повернути &(table_base+var_data->data) + data_offset; }

    private:
    signed tuples_left;
    const TupleVariationData *var_data;
    unsigned ціле axis_count;
    unsigned ціле axis_count_times_2;
    unsigned ціле data_offset;
    unsigned ціле current_tuple_size;
    const void *table_base;

    public:
    hb_bytes_t var_data_bytes;
    const TupleVariationHeader *current_tuple;
  };

  static bool get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_count,
                                  const void *table_base,
                                  hb_vector_t<unsigned ціле> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_count, table_base);
    якщо (!iterator->get_shared_indices (shared_indices))
      повернути false;
    повернути iterator->is_valid ();
  }

  bool has_shared_point_numbers () const { повернути tupleVarCount.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  bool cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_count,
			      hb_array_t<const F2DOT14> shared_tuples,
			      const hb_map_t *axes_old_index_tag_map,
			      const hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<char> &out /* OUT */,
			      bool *changed /* OUT */) const
  {
    *changed = false;

    hb_vector_t<unsigned ціле> shared_indices;
    tuple_iterator_t iterator;
    якщо (!get_tuple_iterator (var_data_bytes, axis_count, this,
			     shared_indices, &iterator))
      повернути true; /* no tuples or malformed: keep original */

    const char *bytes_start = var_data_bytes.arrayZ;
    const char *bytes_end = bytes_start + var_data_bytes.length;
    const char *serialized_base = (const char *) &(this+data);
    const char *shared_points_end = (const char *) iterator.get_serialized_data ();
    якщо (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      повернути true;

    struct kept_tuple_t
    {
      const char *header;
      unsigned header_size;
      const char *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    do
    {
      const TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_count * 2);
      unsigned data_size = header->get_data_size ();
      const char *tuple_data = (const char *) iterator.get_serialized_data ();
      якщо (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	повернути true; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      якщо (!header->unpack_axis_tuples (axis_count, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	повернути true;

      total++;
      якщо (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(const char *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } поки (iterator.move_to_next ());

    якщо (unlikely (kept.in_error ())) повернути false;
    якщо (kept.length == total) повернути true; /* nothing to cull */

    *changed = true;
    якщо (!kept.length) повернути true; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    якщо (unlikely (!out.resize (new_size)))
      повернути false;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tupleVarCount = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    char *p = out.arrayZ + min_size;
    для (const auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    для (const auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    повернути true;
  }

  static bool decompile_points (const HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned ціле> &points /* OUT */,
				const HBUINT8 *end)
  {
    enum packed_point_flag_t
    {
      POINTS_ARE_WORDS     = 0x80,
      POINT_RUN_COUNT_MASK = 0x7F
    };

    якщо (unlikely (p + 1 > end)) повернути false;

    unsigned count = *p++;
    якщо (count & POINTS_ARE_WORDS)
    {
      якщо (unlikely (p + 1 > end)) повернути false;
      count = ((count & POINT_RUN_COUNT_MASK) << 8) | *p++;
    }
    якщо (unlikely (!points.resize_dirty  (count))) повернути false;

    unsigned n = 0;
    unsigned i = 0;
    поки (i < count)
    {
      якщо (unlikely (p + 1 > end)) повернути false;
      unsigned control = *p++;
      unsigned run_count = (control & POINT_RUN_COUNT_MASK) + 1;
      unsigned stop = i + run_count;
      якщо (unlikely (stop > count)) повернути false;
      якщо (control & POINTS_ARE_WORDS)
      {
        якщо (unlikely (p + run_count * HBUINT16::static_size > end)) повернути false;
        для (; i < stop; i++)
        {
          n += *(const HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      інакше
      {
        якщо (unlikely (p + run_count > end)) повернути false;
        для (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    повернути true;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  static bool decompile_deltas (const HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				const HBUINT8 *end,
				bool consume_all = false,
				unsigned start = 0)
  {
    повернути TupleValues::decompile (p, deltas, end, consume_all, start);
  }

  bool has_data () const { повернути tupleVarCount; }

  bool decompile_tuple_variations (unsigned point_count,
                                   bool is_gvar,
                                   tuple_iterator_t iterator,
                                   const hb_map_t *axes_old_index_tag_map,
                                   const hb_vector_t<unsigned> &shared_indices,
                                   const hb_array_t<const F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   bool is_composite_glyph = false) const
  {
    повернути tuple_variations.create_from_tuple_var_data (iterator, tupleVarCount,
                                                        point_count, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  bool serialize (hb_serialize_context_t *c,
                  bool is_gvar,
                  const tuple_variations_t& tuple_variations) const
  {
    TRACE_SERIALIZE (this);
    /* empty tuple variations, just return and skip serialization. */
    якщо (!tuple_variations) return_trace (true);

    auto *out = c->start_embed (this);
    якщо (unlikely (!c->extend_min (out))) return_trace (false);

    якщо (!c->check_assign (out->tupleVarCount, tuple_variations.get_var_count (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    unsigned total_header_len = 0;

    якщо (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (false);

    unsigned data_offset = min_size + total_header_len;
    якщо (!is_gvar) data_offset += 4;
    якщо (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    повернути tuple_variations.serialize_var_data (c, is_gvar);
  }

  protected:
  struct TupleVarCount : HBUINT16
  {
    friend struct tuple_variations_t;
    bool has_shared_point_numbers () const { повернути ((*this) & SharedPointNumbers); }
    unsigned ціле get_count () const { повернути (*this) & CountMask; }
    TupleVarCount& operator = (uint16_t i) { HBUINT16::operator= (i); повернути *this; }
    explicit operator bool () const { повернути get_count (); }

    protected:
    enum Flags
    {
      SharedPointNumbers= 0x8000u,
      CountMask         = 0x0FFFu
    };
    public:
    DEFINE_SIZE_STATIC (2);
  };

  TupleVarCount tupleVarCount;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  public:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
struct item_variations_t
{
  using region_t = const hb_hashmap_t<hb_tag_t, Triple>*;
  private:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_data_num_rows;

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
  hb_vector_t<hb_vector_t<ціле>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  bool has_long = false;

  public:
  bool has_long_word () const
  { повернути has_long; }

  const hb_vector_t<region_t>& get_region_list () const
  { повернути region_list; }

  const hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () const
  { повернути encodings; }

  const hb_map_t& get_varidx_map () const
  { повернути varidx_map; }

  bool add_vardata_encoding_for_testing (hb_vector_t<const hb_vector_t<ціле>*> &&rows,
                                         unsigned num_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), num_cols));
    повернути !encodings.in_error ();
  }

  bool compile_varidx_map_for_testing (const hb_hashmap_t<unsigned, const hb_vector_t<ціле>*>& front_mapping)
  { повернути compile_varidx_map (front_mapping); }

  bool instantiate (const ItemVariationStore& varStore,
                    const hb_subset_plan_t *plan,
                    bool optimize=true,
                    bool use_no_variation_idx=true,
                    const hb_array_t <const hb_inc_bimap_t> inner_maps = hb_array_t<const hb_inc_bimap_t> (),
                    bool layout_conditions = false)
  {
    якщо (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      повернути false;
    /* avar2 partial instancing: cull unreachable regions. */
    якщо (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      для (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    якщо (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      повернути false;
    якщо (layout_conditions &&
        (!instantiate_condition_values (plan, varStore) || !build_region_list ()))
      повернути false;
    повернути as_item_varstore (optimize, use_no_variation_idx);
  }

  bool instantiate_condition_values (const hb_subset_plan_t *plan,
                                      const ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_value_map;
    condition_map.clear ();
    якщо (!plan->layout_condition_values) повернути true;
    using condition_value_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, ціле>>;
    hb_vector_t<condition_value_t> conditions;
    для (auto condition : plan->layout_condition_values.iter ())
      conditions.push (condition);
    якщо (unlikely (conditions.in_error ())) повернути false;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (const condition_value_t &a, const condition_value_t &b) {
      якщо (a.second.first != b.second.first)
        повернути a.second.first < b.second.first ? -1 : 1;
      повернути a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    для (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      якщо (plan->gdef_varstore_inner_maps[i].get_population ())
        якщо (!outer_map.set (i, new_outer++)) повернути false;
    для (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      double value = condition.second.second +
                     varStore.get_delta<double> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      якщо (!outer_map.has (old_outer))
        continue;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      якщо (outer >= vars.length || inner >= var_data_num_rows[outer])
        continue;

      double maximum = fabs ((double) value);
      bool variable = false;
      для (const tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        якщо (unlikely (inner >= tuple.deltas_x.length)) повернути false;
        float delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((double) delta));
      }
      якщо (!variable)
      {
        якщо (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), ціле (value > 0))))
          повернути false;
        continue;
      }

      double scale = 1.;
      поки (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      поки (maximum * scale * 2 <= INT_MAX)
      {
        bool integral = (double) value * scale == round ((double) value * scale);
        для (const tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (double) tuple.deltas_x[inner] * scale ==
                      round ((double) tuple.deltas_x[inner] * scale);
        якщо (integral) break;
        scale *= 2;
      }
      ціле default_value = (ціле) round ((double) value * scale);
      якщо (scale != 1. || default_value < -32768 || default_value > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        для (const tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          якщо (!tuple.deltas_x[inner]) continue;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (true);
          copy.deltas_x.push ((float) round ((double) tuple.deltas_x[inner] * scale));
          якщо (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) повернути false;
          private_row.tuple_vars.push (std::move (copy));
        }
        якщо (unlikely (vars.length >= 0xFFFFu)) повернути false;
        якщо (unlikely (private_row.tuple_vars.in_error ())) повернути false;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_num_rows.push (1);
        якщо (unlikely (vars.in_error () || var_data_num_rows.in_error ())) повернути false;
        якщо (default_value < -32768 || default_value > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          ціле bias = default_value - hb_clamp (default_value, -32768, 32767);
          default_value -= bias;
          ціле high = bias / 65536 * 65536;
          ціле parts[] = {high, bias - high};
          для (ціле part : parts)
          {
            якщо (!part) continue;
            unsigned count = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            const auto &tuples = vars[idx >> 16].tuple_vars;
            якщо (unlikely (tuples.in_error () || tuples.length != count + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) повернути false;
          }
        }
      }
      якщо (!condition_map.set (condition.first, hb_pair (idx, default_value)))
        повернути false;
    }
    повернути !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  bool create_from_item_varstore (const ItemVariationStore& varStore,
                                  const hb_map_t& axes_old_index_tag_map,
                                  const hb_array_t <const hb_inc_bimap_t> inner_maps = hb_array_t<const hb_inc_bimap_t> ())
  {
    const VarRegionList& regionList = varStore.get_region_list ();
    якщо (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      повернути false;

    unsigned num_var_data = varStore.get_sub_table_count ();
    якщо (inner_maps && inner_maps.length != num_var_data) повернути false;
    якщо (!vars.alloc (num_var_data) ||
        !var_data_num_rows.alloc (num_var_data)) повернути false;

    для (unsigned i = 0; i < num_var_data; i++)
    {
      якщо (inner_maps && !inner_maps.arrayZ[i].get_population ())
          continue;
      tuple_variations_t var_data_tuples;
      unsigned item_count = 0;
      якщо (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_count,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        повернути false;

      var_data_num_rows.push (item_count);
      vars.push (std::move (var_data_tuples));
    }
    повернути !vars.in_error () && !var_data_num_rows.in_error () && vars.length == var_data_num_rows.length;
  }

  bool instantiate_tuple_vars (const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               bool build_regions = true)
  {
    optimize_scratch_t scratch;
    для (tuple_variations_t& tuple_vars : vars)
      якщо (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        повернути false;

    повернути !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_count)
  {
    vars.push (tuple_variations_t ());
    var_data_num_rows.push (item_count);
    повернути vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_count (unsigned outer) const
  { повернути outer < var_data_num_rows.length ? var_data_num_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    якщо (unlikely (outer >= vars.length ||
                  inner >= var_data_num_rows[outer] ||
                  var_data_num_rows[outer] >= 0xFFFFu))
      повернути (unsigned) -1;
    для (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      якщо (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        повернути (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      якщо (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      якщо (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        повернути (unsigned) -1;
    }
    повернути var_data_num_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  void add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, ціле delta, unsigned item_count)
  {
    якщо (outer >= vars.length) повернути;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    якщо (!tuple.indices.resize (item_count) ||
        !tuple.deltas_x.resize (item_count)) повернути;
    для (unsigned i = 0; i < item_count; i++)
    {
      tuple.indices.arrayZ[i] = true;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (float) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  bool build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    для (const tuple_variations_t& sub_table : vars)
    {
      для (const tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        якщо (!used_regions.has (r))
        {
          bool all_zeros = true;
          для (float d : tuple.deltas_x)
          {
            ціле delta = hb_clamp_to<ціле> (roundf ((double) d));
            якщо (delta != 0)
            {
              all_zeros = false;
              break;
            }
          }
          якщо (!all_zeros)
          {
            якщо (!used_regions.set (r, 1))
              повернути false;
          }
        }
        якщо (all_regions.has (r))
          continue;
        якщо (!all_regions.set (r, 1))
          повернути false;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    якщо (!all_regions || !all_unique_regions) повернути true;

    якщо (!region_list.alloc (all_regions.get_population ()))
      повернути false;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    для (const auto& r : orig_region_list)
    {
      якщо (!all_regions.has (&r) || !used_regions.has (&r))
        continue;

      region_list.push (&r);
      якщо (!region_map.set (&r, idx))
        повернути false;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    для (const auto& r: all_unique_regions)
    {
      якщо (!all_regions.has (r) || !used_regions.has (r))
        continue;
      region_list.push (r);
      якщо (!region_map.set (r, idx))
        повернути false;
      all_regions.del (r);
      idx++;
    }
    повернути (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  struct combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = default;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      assert (gain < 0xFFFFFF);
      assert (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    bool operator < (const combined_gain_idx_tuple_t& o)
    {
      повернути encoded < o.encoded;
    }

    bool operator <= (const combined_gain_idx_tuple_t& o)
    {
      повернути encoded <= o.encoded;
    }

    unsigned idx_1 () const { повернути (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () const { повернути encoded & 0xFFFFF; };
  };

  bool as_item_varstore (bool optimize=true, bool use_no_variation_idx=true)
  {
    /* return true if no variation data */
    якщо (!region_list) повернути true;
    unsigned num_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    для (unsigned major = 0; major < var_data_num_rows.length; major++)
      total_rows += var_data_num_rows[major];

    якщо (!delta_rows.resize (total_rows)) повернути false;
    /* init all rows to [0]*num_cols */
    для (unsigned i = 0; i < total_rows; i++)
      якщо (!(delta_rows[i].resize (num_cols))) повернути false;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, const hb_vector_t<ціле>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<const hb_vector_t<ціле> *> major_rows;
    hb_hashmap_t<const hb_vector_t<ціле>*, unsigned> delta_rows_map;
    для (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      const tuple_variations_t& tuples = vars[major];
      unsigned num_rows = var_data_num_rows[major];

      якщо (!num_rows) continue;

      для (const tuple_delta_t& tuple: tuples.tuple_vars)
      {
        якщо (tuple.deltas_x.length != num_rows)
          повернути false;

        /* skip unused regions */
        unsigned *col_idx;
        якщо (!region_map.has (&(tuple.axis_tuples), &col_idx))
          continue;

        для (unsigned i = 0; i < num_rows; i++)
        {
          ціле rounded_delta = hb_clamp_to<ціле> (roundf ((double) tuple.deltas_x[i]));
          ціле &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          якщо (unlikely (sum < INT_MIN || sum > INT_MAX)) повернути false;
          delta = (ціле) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      для (unsigned minor = 0; minor < num_rows; minor++)
      {
	const hb_vector_t<ціле>& row = delta_rows[start_row + minor];
	якщо (use_no_variation_idx)
	{
	  bool all_zeros = true;
	  для (ціле delta : row)
	  {
	    якщо (delta != 0)
	    {
	      all_zeros = false;
	      break;
	    }
	  }
	  якщо (all_zeros)
	    continue;
	}

	якщо (!front_mapping.set ((major<<16) + minor, &row))
	  повернути false;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	якщо (optimize)
	{
	  якщо (delta_rows_map.has (&row))
	    continue;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      якщо (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        повернути false;
      якщо (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), num_cols);
	якщо (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  повернути false;
      }

      start_row += num_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    якщо (!optimize)
    {
      encodings = std::move (encoding_objs);
      повернути !encodings.in_error ();
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
    hb_vector_t<item_t> queue_items;
    unsigned num_todos = encoding_objs.length;
    для (unsigned i = 0; i < num_todos; i++)
    {
      для (unsigned j = i + 1; j < num_todos; j++)
      {
        ціле combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        якщо (combining_gain > 0)
          queue_items.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_items));

    hb_bit_set_t removed_todo_idxes;
    поки (queue)
    {
      auto t = queue.pop_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      якщо (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        continue;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      якщо (unlikely (removed_todo_idxes.in_error ())) повернути false;

      encoding.merge (other_encoding);
      якщо (unlikely (encoding.in_error ())) повернути false;

      для (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        якщо (removed_todo_idxes.has (idx)) continue;

        const delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        якщо (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          для (const auto& row : obj.items)
            encoding.add_row (row);
          якщо (unlikely (encoding.in_error ())) повернути false;

          removed_todo_idxes.add (idx);
          якщо (unlikely (removed_todo_idxes.in_error ())) повернути false;
          continue;
        }

        ціле combined_gain = encoding.gain_from_merging (obj);
        якщо (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      якщо (unlikely (encoding_objs.in_error ())) повернути false;
    }

    ціле num_final_encodings = (ціле) encoding_objs.length - (ціле) removed_todo_idxes.get_population ();
    якщо (num_final_encodings <= 0) повернути false;

    якщо (!encodings.alloc (num_final_encodings)) повернути false;
    для (unsigned i = 0; i < encoding_objs.length; i++)
    {
      якщо (removed_todo_idxes.has (i)) continue;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    повернути compile_varidx_map (front_mapping);
  }

  private:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  bool compile_varidx_map (const hb_hashmap_t<unsigned, const hb_vector_t<ціле>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<const hb_vector_t<ціле>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    для (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      якщо (encoding.is_empty () || encoding.in_error ())
        повернути false;

      unsigned num_rows = encoding.items.length;
      unsigned num_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.items.qsort (_cmp_row);

      для (unsigned start = 0; start < num_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (num_rows - start, 0xFFFFu);
        hb_vector_t<const hb_vector_t<ціле>*> rows;

        якщо (!rows.alloc (chunk_len))
          повернути false;

        unsigned major = split_encodings.length;
        для (unsigned minor = 0; minor < chunk_len; minor++)
        {
          const hb_vector_t<ціле> *row = encoding.items.arrayZ[start + minor];
          rows.push (row);
          якщо (!back_mapping.set (row, (major << 16) + minor))
            повернути false;
        }

        auto *split = split_encodings.push (std::move (rows), num_cols);
        якщо (unlikely (split_encodings.in_error () || split->in_error ()))
          повернути false;
      }
    }

    encodings = std::move (split_encodings);
    якщо (encodings.in_error () || back_mapping.in_error ())
      повернути false;

    для (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      якщо (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      інакше
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    повернути !varidx_map.in_error ();
  }

  static ціле _cmp_row (const void *pa, const void *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    const hb_vector_t<ціле>** a = (const hb_vector_t<ціле>**) pa;
    const hb_vector_t<ціле>** b = (const hb_vector_t<ціле>**) pb;

    для (unsigned i = 0; i < (*b)->length; i++)
    {
      ціле va = (*a)->arrayZ[i];
      ціле vb = (*b)->arrayZ[i];
      якщо (va != vb)
        повернути va < vb ? -1 : 1;
    }
    повернути 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

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
    만약 (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned count = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      반환 hb_unsigned_mul_add_saturate (count, axis_count_times_2, min_size);
    }
    반환 min_size;
  }

  unsigned get_data_size () const { 반환 varDataSize; }

  const TupleVariationHeader &get_next (unsigned axis_count_times_2) const
  { 반환 StructAtOffset<TupleVariationHeader> (this, get_size (axis_count_times_2)); }

  bool unpack_axis_tuples (unsigned axis_count,
                           const hb_array_t<const F2DOT14> shared_tuples,
                           const hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) const
  {
    const F2DOT14 *peak_tuple = nullptr;
    만약 (has_peak ())
      peak_tuple = get_peak_tuple (axis_count);
    아니면
    {
      unsigned 정수 index = get_index ();
      만약 (unlikely ((index + 1) * axis_count > shared_tuples.length))
        반환 false;
      peak_tuple = shared_tuples.sub_array (axis_count * index, axis_count).arrayZ;
    }

    const F2DOT14 *start_tuple = nullptr;
    const F2DOT14 *end_tuple = nullptr;
    bool has_interm = has_intermediate ();

    만약 (has_interm)
    {
      start_tuple = get_start_tuple (axis_count);
      end_tuple = get_end_tuple (axis_count);
    }

    위해 (unsigned i = 0; i < axis_count; i++)
    {
      float peak = peak_tuple[i].to_float ();
      만약 (peak == 0.f) continue;

      hb_tag_t *axis_tag;
      만약 (!axes_old_index_tag_map->has (i, &axis_tag))
        반환 false;

      float start, end;
      만약 (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      아니면
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    반환 true;
  }

  HB_ALWAYS_INLINE
  float calculate_scalar (hb_array_t<const 정수> coords, unsigned 정수 coord_count,
			   const hb_array_t<const F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) const
  {
    unsigned tuple_index = tupleIndex;
    unsigned 정수 index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    bool plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    만약 (likely (plain_shared_tuple))
    {
      float scalar;
      만약 (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	반환 scalar;
    }

    const F2DOT14 *peak_tuple;

    bool has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    만약 (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_count);
      shared_tuple_scalar_cache = nullptr;
    }
    아니면
    {
      float scalar;
      만약 (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        만약 (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	아니면
	  반환 scalar;
      }

      만약 (unlikely ((index + 1) * coord_count > shared_tuples.length))
        반환 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_count * index);

    }

    const F2DOT14 *start_tuple = nullptr;
    const F2DOT14 *end_tuple = nullptr;

    만약 (has_interm)
    {
      start_tuple = get_start_tuple (coord_count);
      end_tuple = get_end_tuple (coord_count);
    }

    float scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#만약 HB_FAST_NUM_ACCESS
    bool skip = coord_count >= 16;
#endif
#endif
    위해 (unsigned 정수 i = 0; i < coord_count; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#만약 HB_FAST_NUM_ACCESS
      만약 (skip)
      {
	동안 (i + 4 <= coord_count && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	동안 (i < coord_count && peak_tuple[i].to_int () == 0)
	  i += 1;
	만약 (i >= coord_count)
	  break;
      }
#endif
#endif

      정수 peak = peak_tuple[i].to_int ();
      만약 (!peak) continue;

      정수 v = coords[i];
      만약 (!v) { scalar = 0.f; break; }
      만약 (v == peak) continue;

      만약 (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        정수 start = start_tuple[i].to_int ();
        정수 end = end_tuple[i].to_int ();
        만약 (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) continue;
        만약 (v < start || v > end) { scalar = 0.f; break; }
        만약 (v < peak)
        { 만약 (peak != start) scalar *= (float) (v - start) / (peak - start); }
        아니면
        { 만약 (peak != end) scalar *= (float) (end - v) / (end - peak); }
      }
      아니면 만약 (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; break; }
      아니면
        scalar *= (float) v / peak;
    }
    만약 (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    반환 scalar;
  }

  bool           has_peak () const { 반환 tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  bool   has_intermediate () const { 반환 tupleIndex & TupleIndex::IntermediateRegion; }
  bool has_private_points () const { 반환 tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () const { 반환 tupleIndex & TupleIndex::TupleIndexMask; }
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

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); 반환 *this; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<const F2DOT14> get_all_tuples (unsigned axis_count) const
  { 반환 StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_count); }
  const F2DOT14* get_all_tuples_base (unsigned axis_count) const
  { 반환 StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  const F2DOT14* get_peak_tuple (unsigned axis_count) const
  { 반환 get_all_tuples_base (axis_count); }
  const F2DOT14* get_start_tuple (unsigned axis_count) const
  { 반환 get_all_tuples_base (axis_count) + has_peak () * axis_count; }
  const F2DOT14* get_end_tuple (unsigned axis_count) const
  { 반환 get_all_tuples_base (axis_count) + has_peak () * axis_count + axis_count; }

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
  hb_vector_t<정수> rounded_x_deltas;
  hb_vector_t<정수> rounded_y_deltas;
  hb_vector_t<float> opt_deltas_x;
  hb_vector_t<float> opt_deltas_y;
  hb_vector_t<unsigned char> opt_point_data;
  hb_vector_t<unsigned char> opt_deltas_data;
  hb_vector_t<unsigned char> point_data;
  hb_vector_t<unsigned char> deltas_data;
  hb_vector_t<정수> rounded_deltas;
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
    반환 *this;
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
  { 반환 axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (const tuple_delta_t& o)
  {
    unsigned num = indices.length;
    위해 (unsigned i = 0; i < num; i++)
    {
      만약 (indices.arrayZ[i])
      {
        만약 (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          만약 (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      아니면
      {
        만약 (!o.indices.arrayZ[i]) continue;
        indices.arrayZ[i] = true;
        deltas_x[i] = o.deltas_x[i];
        만약 (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    반환 *this;
  }

  tuple_delta_t& operator *= (float scalar)
  {
    만약 (scalar == 1.0f)
      반환 *this;

    unsigned num = indices.length;
    만약 (deltas_y)
      위해 (unsigned i = 0; i < num; i++)
      {
	만약 (!indices.arrayZ[i]) continue;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    아니면
      위해 (unsigned i = 0; i < num; i++)
      {
	만약 (!indices.arrayZ[i]) continue;
	deltas_x[i] *= scalar;
      }
    반환 *this;
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
    만약 (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*this));
      반환;
    }

    만약 ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->middle && tent->middle <= tent->maximum))
      반환;

    만약 (tent->middle == 0.f)
    {
      out.push (std::move (*this));
      반환;
    }

    만약 (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.middle &&
          axis_limit.middle <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      반환;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    위해 (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      만약 (i < solutions.length - 1)
	new_var.copy_from (*this, pool);
      아니면
	new_var = std::move (*this);

      만약 (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      아니면
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
    만약 (pool)
    {
      만약 (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_count)))
	반환 false;
    }
    아니면 만약 (unlikely (!compiled_peak_coords.resize (cur_axis_count)))
      반환 false;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_count = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    위해 (unsigned i = 0; i < orig_axis_count; i++)
    {
      만약 (!axes_index_map.has (i))
        continue;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      만약 (axis_tuples.has (axis_tag, &coords))
      {
	float min_val = coords->minimum;
	float val = coords->middle;
	float max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	만약 (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  만약 (!compiled_interm_coords)
	  {
	    만약 (pool)
	    {
	      만약 (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_count)))
		반환 false;
	    }
	    아니면 만약 (unlikely (!compiled_interm_coords.resize (2 * cur_axis_count)))
	      반환 false;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_count);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_count);

	    위해 (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      만약 (!peak) continue;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	만약 (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    반환 !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
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
    만약 (!compiled_deltas) 반환 true;

    unsigned cur_axis_count = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_count * (F2DOT14::static_size) + 4;
    만약 (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, false))) 반환 false;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    만약 (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_count = 0;
    unsigned *shared_tuple_idx;
    만약 (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    아니면
    {
      peak_count = encode_peak_coords(coords, flag);
      만약 (!peak_count) 반환 false;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_count = encode_interm_coords (coords.sub_array (peak_count), flag);

    /* pointdata length = 0 implies "use shared points" */
    만약 (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_count + interim_count) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    반환 true;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) const
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * sizeof (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    반환 compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) const
  {
    만약 (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * sizeof (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    반환 compiled_interm_coords.length;
  }

  bool compile_deltas (hb_vector_t<정수> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { 반환 compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  static bool compile_deltas (hb_array_t<const bool> point_indices,
			      hb_array_t<const float> x_deltas,
			      hb_array_t<const float> y_deltas,
			      hb_vector_t<unsigned char> &compiled_deltas, /* OUT */
			      hb_vector_t<정수> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    만약 (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      반환 false;

    unsigned j = 0;
    위해 (unsigned i = 0; i < point_indices.length; i++)
    {
      만약 (!point_indices[i]) continue;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<정수> (roundf ((double) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    만약 (!rounded_deltas) 반환 true;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    만약 (y_deltas)
      alloc_len *= 2;

    만약 (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, false))) 반환 false;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    만약 (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      위해 (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        만약 (!point_indices[idx]) continue;
        정수 rounded_delta = hb_clamp_to<정수> (roundf ((double) y_deltas.arrayZ[idx]));

        만약 (j >= rounded_deltas.length) 반환 false;

        rounded_deltas[j++] = rounded_delta;
      }

      만약 (j != rounded_deltas.length) 반환 false;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    반환 true;
  }

  static unsigned compile_deltas (hb_array_t<unsigned char> encoded_bytes,
				  hb_array_t<const 정수> deltas)
  {
    반환 TupleValues::compile_unsafe (deltas, encoded_bytes);
  }

  bool calc_inferred_deltas (const contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_count = orig_points.length;
    만약 (point_count != indices.length)
      반환 false;

    unsigned ref_count = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    위해 (unsigned i = 0; i < point_count; i++)
    {
      ref_count += indices.arrayZ[i];
      만약 (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    만약 (ref_count == point_count)
      반환 true;
    만약 (unlikely (end_points.in_error ())) 반환 false;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    위해 (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_count = 0;
      위해 (unsigned i = start_point; i < end_point + 1; i++)
        unref_count += indices.arrayZ[i];
      unref_count = (end_point - start_point + 1) - unref_count;

      unsigned j = start_point;
      만약 (unref_count == 0 || unref_count > end_point - start_point)
        goto no_more_gaps;
      위해 (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned 정수 prev, next, i;
        위해 (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          만약 (indices.arrayZ[i] && !indices.arrayZ[j]) break;
        }
        prev = j = i;
        위해 (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          만약 (!indices.arrayZ[i] && indices.arrayZ[j]) break;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        위해 (;;)
        {
          i = next_index (i, start_point, end_point);
          만약 (i == next) break;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          만약 (--unref_count == 0) goto no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    위해 (unsigned i = 0; i < point_count; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      만약 ( !indices[i])
      {
        만약 (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = true;
      }
    }
    반환 true;
  }

  bool optimize (const contour_point_vector_t& contour_points,
                 bool is_composite,
		 optimize_scratch_t &scratch,
                 double tolerance = 0.5 + 1e-10)
  {
    unsigned count = contour_points.length;
    만약 (deltas_x.length != count ||
        deltas_y.length != count)
      반환 false;

    hb_vector_t<bool> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<정수> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<정수> &rounded_y_deltas = scratch.rounded_y_deltas;

    만약 (unlikely (!rounded_x_deltas.resize_dirty  (count) ||
                  !rounded_y_deltas.resize_dirty  (count)))
      반환 false;

    위해 (unsigned i = 0; i < count; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<정수> (roundf ((double) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<정수> (roundf ((double) deltas_y.arrayZ[i]));
    }

    만약 (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      반환 false;

    unsigned ref_count = 0;
    위해 (bool ref_flag : opt_indices)
       ref_count += ref_flag;

    만약 (ref_count == count) 반환 true;

    hb_vector_t<float> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<float> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    bool is_comp_glyph_wo_deltas = (is_composite && ref_count == 0);
    만약 (is_comp_glyph_wo_deltas)
    {
      만약 (unlikely (!opt_deltas_x.resize (count) ||
                    !opt_deltas_y.resize (count)))
        반환 false;

      opt_indices.arrayZ[0] = true;
      위해 (unsigned i = 1; i < count; i++)
        opt_indices.arrayZ[i] = false;
    }

    hb_vector_t<unsigned char> &opt_point_data = scratch.opt_point_data.reset ();
    만약 (!compile_point_set (opt_indices, opt_point_data))
      반환 false;
    hb_vector_t<unsigned char> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    만약 (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      반환 false;

    hb_vector_t<unsigned char> &point_data = scratch.point_data.reset ();
    만약 (!compile_point_set (indices, point_data))
      반환 false;
    hb_vector_t<unsigned char> &deltas_data = scratch.deltas_data.reset ();
    만약 (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      반환 false;

    만약 (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      만약 (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    반환 !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  static bool compile_point_set (const hb_vector_t<bool> &point_indices,
                                 hb_vector_t<unsigned char>& compiled_points /* OUT */)
  {
    unsigned num_points = 0;
    위해 (bool i : point_indices)
      만약 (i) num_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    만약 (!num_points) 반환 true;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    만약 (num_points == indices_length)
      반환 compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned num_bytes = 2 + 3 *num_points;
    만약 (unlikely (!compiled_points.resize_dirty  (num_bytes)))
      반환 false;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    만약 (num_points < 0x80)
      compiled_points.arrayZ[pos++] = num_points;
    아니면
    {
      compiled_points.arrayZ[pos++] = ((num_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = num_points & 0xFF;
    }

    const unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_value = 0;
    unsigned num_encoded = 0;
    동안 (i < indices_length && num_encoded < num_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      bool use_byte_encoding = false;
      bool new_run = true;
      동안 (i < indices_length && num_encoded < num_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        동안 (i < indices_length && !point_indices[i])
          i++;

        만약 (i >= indices_length) break;

        unsigned cur_value = i;
        unsigned delta = cur_value - last_value;

        만약 (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = false;
        }

        만약 (use_byte_encoding && delta > 0xFF)
          break;

        만약 (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        아니면
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_value = cur_value;
        run_length++;
        num_encoded++;
      }

      만약 (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      아니면
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    반환 compiled_points.resize_dirty  (pos);
  }

  static float infer_delta (float target_val, float prev_val, float next_val, float prev_delta, float next_delta)
  {
    만약 (prev_val == next_val)
      반환 (prev_delta == next_delta) ? prev_delta : 0.f;
    아니면 만약 (target_val <= hb_min (prev_val, next_val))
      반환 (prev_val < next_val) ? prev_delta : next_delta;
    아니면 만약 (target_val >= hb_max (prev_val, next_val))
      반환 (prev_val > next_val) ? prev_delta : next_delta;

    float r = (target_val - prev_val) / (next_val - prev_val);
    반환 prev_delta + r * (next_delta - prev_delta);
  }

  static unsigned 정수 next_index (unsigned 정수 i, unsigned 정수 start, unsigned 정수 end)
  { 반환 (i >= end) ? start : (i + 1); }
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
    위해 (unsigned i = 0; i < count; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_count_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_count_times_2);
    }

    반환 total_size;
  }

  const TupleVariationHeader &get_tuple_var_header (void) const
  { 반환 StructAfter<TupleVariationHeader> (data); }

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

    explicit operator bool () const { 반환 bool (tuple_vars); }
    unsigned get_var_count () const
    {
      unsigned count = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      위해 (auto& tuple: tuple_vars)
        만약 (tuple.compiled_deltas) count++;

      만약 (shared_points_bytes && shared_points_bytes->length)
        count |= TupleVarCount::SharedPointNumbers;
      반환 count;
    }

    unsigned get_compiled_byte_size () const
    { 반환 compiled_byte_size; }

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
      hb_vector_t<정수> deltas_x;
      hb_vector_t<정수> deltas_y;
      do
      {
        const HBUINT8 *p = iterator.get_serialized_data ();
        unsigned 정수 length = iterator.current_tuple->get_data_size ();
        만약 (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          반환 false;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        만약 (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_count (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          반환 false;

	private_indices.reset ();
        bool has_private_points = iterator.current_tuple->has_private_points ();
        const HBUINT8 *end = p + length;
        만약 (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          반환 false;

        const hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        bool apply_to_all = (indices.length == 0);
        unsigned num_deltas = apply_to_all ? point_count : indices.length;

        만약 (unlikely (!deltas_x.resize_dirty  (num_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          반환 false;

        만약 (is_gvar)
        {
          만약 (unlikely (!deltas_y.resize_dirty  (num_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            반환 false;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        만약 (unlikely (!var.indices.allocate_from_pool (pool, point_count) ||
                      !var.deltas_x.allocate_from_pool (pool, point_count, false)))
          반환 false;

        만약 (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_count, false)))
          반환 false;

        위해 (unsigned i = 0; i < num_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          만약 (idx >= point_count) continue;
          var.indices[idx] = true;
          var.deltas_x[idx] = deltas_x[i];
          만약 (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } 동안 (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      반환 true;
    }

    bool create_from_item_var_data (const VarData &var_data,
                                    const hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    const hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_count,
                                    const hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      만약 (&var_data == &Null (VarData))
        반환 true;

      unsigned num_regions = var_data.get_region_index_count ();
      만약 (!tuple_vars.alloc (num_regions)) 반환 false;

      item_count = inner_map ? inner_map->get_population () : var_data.get_item_count ();
      만약 (!item_count) 반환 true;
      unsigned row_size = var_data.get_row_size ();
      const HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      위해 (unsigned r = 0; r < num_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        만약 (!tuple.deltas_x.resize_dirty  (item_count) ||
            !tuple.indices.resize_dirty  (item_count))
          반환 false;

        위해 (unsigned i = 0; i < item_count; i++)
        {
          tuple.indices.arrayZ[i] = true;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        만약 (region_index >= regions.length) 반환 false;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      반환 !tuple_vars.in_error ();
    }

    private:
    static 정수 _cmp_axis_tag (const void *pa, const void *pb)
    {
      const hb_tag_t *a = (const hb_tag_t*) pa;
      const hb_tag_t *b = (const hb_tag_t*) pb;
      반환 (정수)(*a) - (정수)(*b);
    }

    bool change_tuple_variations_axis_limits (const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      만약 (!axis_tags.alloc (normalized_axes_location.get_population ()))
        반환 false;
      위해 (auto t : normalized_axes_location.keys ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      위해 (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        만약 (!normalized_axes_location.has (axis_tag, &axis_limit))
          반환 false;
        TripleDistances axis_triple_distances{1.f, 1.f};
        만약 (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        위해 (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          만약 (!out) continue;

          unsigned new_len = new_vars.length + out.length;

          만약 (unlikely (!new_vars.alloc (new_len, false)))
            반환 false;

          위해 (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      반환 true;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    bool merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      만약 (unlikely (!new_vars.alloc (tuple_vars.length)))
	반환 false;
      hb_hashmap_t<const hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      위해 (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        만약 (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          만약 (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            반환 false;
          continue;
        }

        unsigned *idx;
        만약 (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        아니면
        {
	  auto *new_var = new_vars.push ();
	  만약 (unlikely (new_vars.in_error ()))
	    반환 false;
          hb_swap (*new_var, var);
          만약 (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            반환 false;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      반환 true;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    bool compile_all_point_sets ()
    {
      위해 (const auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        만약 (point_data_map.has (points_set))
        {
          unsigned *count;
          만약 (unlikely (!point_set_count_map.has (points_set, &count) ||
                        !point_set_count_map.set (points_set, (*count) + 1)))
            반환 false;
          continue;
        }

        hb_vector_t<unsigned char> compiled_point_data;
        만약 (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          반환 false;

        만약 (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_count_map.set (points_set, 1))
          반환 false;
      }
      반환 true;
    }

    /* find shared points set which saves most bytes */
    void find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      위해 (const auto& _ : point_data_map.iter_ref ())
      {
        const hb_vector_t<bool>* points_set = _.first;
        unsigned data_length = _.second.length;
        만약 (!data_length) continue;
        unsigned *count;
        만약 (unlikely (!point_set_count_map.has (points_set, &count) ||
                      *count <= 1))
        {
          shared_points_bytes = nullptr;
          반환;
        }

        unsigned saved_bytes = data_length * ((*count) -1);
        만약 (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    bool calc_inferred_deltas (const contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      위해 (tuple_delta_t& var : tuple_vars)
        만약 (!var.calc_inferred_deltas (contour_points, scratch))
          반환 false;

      반환 true;
    }

    bool iup_optimize (const contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      위해 (tuple_delta_t& var : tuple_vars)
      {
        만약 (!var.optimize (contour_points, is_composite, scratch))
          반환 false;
      }
      반환 true;
    }

    public:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    void cull_unreachable (const hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      위해 (unsigned i = 0; i < tuple_vars.length; i++)
      {
	만약 (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  continue;
	만약 (i != j)
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
      만약 (!tuple_vars) 반환 true;
      만약 (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        반환 false;
      /* compute inferred deltas only for gvar */
      만약 (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	만약 (!calc_inferred_deltas (*contour_points, scratch))
	  반환 false;
      }

      /* if iup delta opt is on, contour_points can't be null */
      만약 (optimize && !contour_points)
        반환 false;

      만약 (!merge_tuple_variations (optimize ? contour_points : nullptr))
        반환 false;

      만약 (optimize && !iup_optimize (*contour_points, scratch)) 반환 false;
      반환 !tuple_vars.in_error ();
    }

    bool compile_bytes (const hb_map_t& axes_index_map,
                        const hb_map_t& axes_old_index_tag_map,
                        bool use_shared_points,
                        bool is_gvar = false,
                        const hb_hashmap_t<const hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      만약 (!tuple_vars)
        반환 true;

      // compile points set and store data in hashmap
      만약 (!compile_all_point_sets ())
        반환 false;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      만약 (use_shared_points)
      {
        find_shared_points ();
        만약 (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<정수> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      위해 (auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        hb_vector_t<unsigned char> *points_data;
        만약 (unlikely (!point_data_map.has (points_set, &points_data)))
          반환 false;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        만약 (!points_data->length)
          continue;
        만약 (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          반환 false;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        만약 (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          반환 false;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      만약 (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = true;
        compiled_byte_size += 1;
      }

      반환 true;
    }

    bool serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) const
    {
      TRACE_SERIALIZE (this);
      위해 (const auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        만약 (c->in_error ()) return_trace (false);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (true);
    }

    bool serialize_var_data (hb_serialize_context_t *c, bool is_gvar) const
    {
      TRACE_SERIALIZE (this);
      만약 (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      위해 (const auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        hb_vector_t<unsigned char> *point_data;
        만약 (!point_data_map.has (points_set, &point_data))
          return_trace (false);

        만약 (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        만약 (c->in_error ()) return_trace (false);
      }

      /* padding for gvar */
      만약 (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        만약 (!c->embed (pad)) return_trace (false);
      }
      return_trace (true);
    }
  };

  struct tuple_iterator_t
  {
    unsigned get_axis_count () const { 반환 axis_count; }

    void init (hb_bytes_t var_data_bytes_, unsigned 정수 axis_count_, const void *table_base_)
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

    bool get_shared_indices (hb_vector_t<unsigned 정수> &shared_indices /* OUT */)
    {
      만약 (var_data->has_shared_point_numbers ())
      {
        const HBUINT8 *base = &(table_base+var_data->data);
        const HBUINT8 *p = base;
        만약 (!decompile_points (p, shared_indices, (const HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) 반환 false;
        data_offset = p - base;
      }
      반환 true;
    }

    bool is_valid ()
    {
      만약 (unlikely (tuples_left <= 0))
	반환 false;

      current_tuple_size = TupleVariationHeader::min_size;
      만약 (unlikely (!var_data_bytes.check_end ((const char *) current_tuple + current_tuple_size)))
	반환 false;

      current_tuple_size = current_tuple->get_size (axis_count_times_2);
      만약 (unlikely (!var_data_bytes.check_end ((const char *) current_tuple + current_tuple_size)))
	반환 false;

      반환 true;
    }

    HB_ALWAYS_INLINE
    bool move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_left--;
      반환 is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    const HBUINT8 *get_serialized_data () const
    { 반환 &(table_base+var_data->data) + data_offset; }

    private:
    signed tuples_left;
    const TupleVariationData *var_data;
    unsigned 정수 axis_count;
    unsigned 정수 axis_count_times_2;
    unsigned 정수 data_offset;
    unsigned 정수 current_tuple_size;
    const void *table_base;

    public:
    hb_bytes_t var_data_bytes;
    const TupleVariationHeader *current_tuple;
  };

  static bool get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_count,
                                  const void *table_base,
                                  hb_vector_t<unsigned 정수> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_count, table_base);
    만약 (!iterator->get_shared_indices (shared_indices))
      반환 false;
    반환 iterator->is_valid ();
  }

  bool has_shared_point_numbers () const { 반환 tupleVarCount.has_shared_point_numbers (); }

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

    hb_vector_t<unsigned 정수> shared_indices;
    tuple_iterator_t iterator;
    만약 (!get_tuple_iterator (var_data_bytes, axis_count, this,
			     shared_indices, &iterator))
      반환 true; /* no tuples or malformed: keep original */

    const char *bytes_start = var_data_bytes.arrayZ;
    const char *bytes_end = bytes_start + var_data_bytes.length;
    const char *serialized_base = (const char *) &(this+data);
    const char *shared_points_end = (const char *) iterator.get_serialized_data ();
    만약 (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      반환 true;

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
      만약 (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	반환 true; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      만약 (!header->unpack_axis_tuples (axis_count, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	반환 true;

      total++;
      만약 (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(const char *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } 동안 (iterator.move_to_next ());

    만약 (unlikely (kept.in_error ())) 반환 false;
    만약 (kept.length == total) 반환 true; /* nothing to cull */

    *changed = true;
    만약 (!kept.length) 반환 true; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    만약 (unlikely (!out.resize (new_size)))
      반환 false;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tupleVarCount = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    char *p = out.arrayZ + min_size;
    위해 (const auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    위해 (const auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    반환 true;
  }

  static bool decompile_points (const HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned 정수> &points /* OUT */,
				const HBUINT8 *end)
  {
    enum packed_point_flag_t
    {
      POINTS_ARE_WORDS     = 0x80,
      POINT_RUN_COUNT_MASK = 0x7F
    };

    만약 (unlikely (p + 1 > end)) 반환 false;

    unsigned count = *p++;
    만약 (count & POINTS_ARE_WORDS)
    {
      만약 (unlikely (p + 1 > end)) 반환 false;
      count = ((count & POINT_RUN_COUNT_MASK) << 8) | *p++;
    }
    만약 (unlikely (!points.resize_dirty  (count))) 반환 false;

    unsigned n = 0;
    unsigned i = 0;
    동안 (i < count)
    {
      만약 (unlikely (p + 1 > end)) 반환 false;
      unsigned control = *p++;
      unsigned run_count = (control & POINT_RUN_COUNT_MASK) + 1;
      unsigned stop = i + run_count;
      만약 (unlikely (stop > count)) 반환 false;
      만약 (control & POINTS_ARE_WORDS)
      {
        만약 (unlikely (p + run_count * HBUINT16::static_size > end)) 반환 false;
        위해 (; i < stop; i++)
        {
          n += *(const HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      아니면
      {
        만약 (unlikely (p + run_count > end)) 반환 false;
        위해 (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    반환 true;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  static bool decompile_deltas (const HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				const HBUINT8 *end,
				bool consume_all = false,
				unsigned start = 0)
  {
    반환 TupleValues::decompile (p, deltas, end, consume_all, start);
  }

  bool has_data () const { 반환 tupleVarCount; }

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
    반환 tuple_variations.create_from_tuple_var_data (iterator, tupleVarCount,
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
    만약 (!tuple_variations) return_trace (true);

    auto *out = c->start_embed (this);
    만약 (unlikely (!c->extend_min (out))) return_trace (false);

    만약 (!c->check_assign (out->tupleVarCount, tuple_variations.get_var_count (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    unsigned total_header_len = 0;

    만약 (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (false);

    unsigned data_offset = min_size + total_header_len;
    만약 (!is_gvar) data_offset += 4;
    만약 (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    반환 tuple_variations.serialize_var_data (c, is_gvar);
  }

  protected:
  struct TupleVarCount : HBUINT16
  {
    friend struct tuple_variations_t;
    bool has_shared_point_numbers () const { 반환 ((*this) & SharedPointNumbers); }
    unsigned 정수 get_count () const { 반환 (*this) & CountMask; }
    TupleVarCount& operator = (uint16_t i) { HBUINT16::operator= (i); 반환 *this; }
    explicit operator bool () const { 반환 get_count (); }

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
  hb_vector_t<hb_vector_t<정수>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  bool has_long = false;

  public:
  bool has_long_word () const
  { 반환 has_long; }

  const hb_vector_t<region_t>& get_region_list () const
  { 반환 region_list; }

  const hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () const
  { 반환 encodings; }

  const hb_map_t& get_varidx_map () const
  { 반환 varidx_map; }

  bool add_vardata_encoding_for_testing (hb_vector_t<const hb_vector_t<정수>*> &&rows,
                                         unsigned num_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), num_cols));
    반환 !encodings.in_error ();
  }

  bool compile_varidx_map_for_testing (const hb_hashmap_t<unsigned, const hb_vector_t<정수>*>& front_mapping)
  { 반환 compile_varidx_map (front_mapping); }

  bool instantiate (const ItemVariationStore& varStore,
                    const hb_subset_plan_t *plan,
                    bool optimize=true,
                    bool use_no_variation_idx=true,
                    const hb_array_t <const hb_inc_bimap_t> inner_maps = hb_array_t<const hb_inc_bimap_t> (),
                    bool layout_conditions = false)
  {
    만약 (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      반환 false;
    /* avar2 partial instancing: cull unreachable regions. */
    만약 (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      위해 (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    만약 (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      반환 false;
    만약 (layout_conditions &&
        (!instantiate_condition_values (plan, varStore) || !build_region_list ()))
      반환 false;
    반환 as_item_varstore (optimize, use_no_variation_idx);
  }

  bool instantiate_condition_values (const hb_subset_plan_t *plan,
                                      const ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_value_map;
    condition_map.clear ();
    만약 (!plan->layout_condition_values) 반환 true;
    using condition_value_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, 정수>>;
    hb_vector_t<condition_value_t> conditions;
    위해 (auto condition : plan->layout_condition_values.iter ())
      conditions.push (condition);
    만약 (unlikely (conditions.in_error ())) 반환 false;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (const condition_value_t &a, const condition_value_t &b) {
      만약 (a.second.first != b.second.first)
        반환 a.second.first < b.second.first ? -1 : 1;
      반환 a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    위해 (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      만약 (plan->gdef_varstore_inner_maps[i].get_population ())
        만약 (!outer_map.set (i, new_outer++)) 반환 false;
    위해 (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      double value = condition.second.second +
                     varStore.get_delta<double> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      만약 (!outer_map.has (old_outer))
        continue;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      만약 (outer >= vars.length || inner >= var_data_num_rows[outer])
        continue;

      double maximum = fabs ((double) value);
      bool variable = false;
      위해 (const tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        만약 (unlikely (inner >= tuple.deltas_x.length)) 반환 false;
        float delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((double) delta));
      }
      만약 (!variable)
      {
        만약 (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), 정수 (value > 0))))
          반환 false;
        continue;
      }

      double scale = 1.;
      동안 (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      동안 (maximum * scale * 2 <= INT_MAX)
      {
        bool integral = (double) value * scale == round ((double) value * scale);
        위해 (const tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (double) tuple.deltas_x[inner] * scale ==
                      round ((double) tuple.deltas_x[inner] * scale);
        만약 (integral) break;
        scale *= 2;
      }
      정수 default_value = (정수) round ((double) value * scale);
      만약 (scale != 1. || default_value < -32768 || default_value > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        위해 (const tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          만약 (!tuple.deltas_x[inner]) continue;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (true);
          copy.deltas_x.push ((float) round ((double) tuple.deltas_x[inner] * scale));
          만약 (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) 반환 false;
          private_row.tuple_vars.push (std::move (copy));
        }
        만약 (unlikely (vars.length >= 0xFFFFu)) 반환 false;
        만약 (unlikely (private_row.tuple_vars.in_error ())) 반환 false;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_num_rows.push (1);
        만약 (unlikely (vars.in_error () || var_data_num_rows.in_error ())) 반환 false;
        만약 (default_value < -32768 || default_value > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          정수 bias = default_value - hb_clamp (default_value, -32768, 32767);
          default_value -= bias;
          정수 high = bias / 65536 * 65536;
          정수 parts[] = {high, bias - high};
          위해 (정수 part : parts)
          {
            만약 (!part) continue;
            unsigned count = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            const auto &tuples = vars[idx >> 16].tuple_vars;
            만약 (unlikely (tuples.in_error () || tuples.length != count + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) 반환 false;
          }
        }
      }
      만약 (!condition_map.set (condition.first, hb_pair (idx, default_value)))
        반환 false;
    }
    반환 !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  bool create_from_item_varstore (const ItemVariationStore& varStore,
                                  const hb_map_t& axes_old_index_tag_map,
                                  const hb_array_t <const hb_inc_bimap_t> inner_maps = hb_array_t<const hb_inc_bimap_t> ())
  {
    const VarRegionList& regionList = varStore.get_region_list ();
    만약 (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      반환 false;

    unsigned num_var_data = varStore.get_sub_table_count ();
    만약 (inner_maps && inner_maps.length != num_var_data) 반환 false;
    만약 (!vars.alloc (num_var_data) ||
        !var_data_num_rows.alloc (num_var_data)) 반환 false;

    위해 (unsigned i = 0; i < num_var_data; i++)
    {
      만약 (inner_maps && !inner_maps.arrayZ[i].get_population ())
          continue;
      tuple_variations_t var_data_tuples;
      unsigned item_count = 0;
      만약 (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_count,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        반환 false;

      var_data_num_rows.push (item_count);
      vars.push (std::move (var_data_tuples));
    }
    반환 !vars.in_error () && !var_data_num_rows.in_error () && vars.length == var_data_num_rows.length;
  }

  bool instantiate_tuple_vars (const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               bool build_regions = true)
  {
    optimize_scratch_t scratch;
    위해 (tuple_variations_t& tuple_vars : vars)
      만약 (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        반환 false;

    반환 !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_count)
  {
    vars.push (tuple_variations_t ());
    var_data_num_rows.push (item_count);
    반환 vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_count (unsigned outer) const
  { 반환 outer < var_data_num_rows.length ? var_data_num_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    만약 (unlikely (outer >= vars.length ||
                  inner >= var_data_num_rows[outer] ||
                  var_data_num_rows[outer] >= 0xFFFFu))
      반환 (unsigned) -1;
    위해 (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      만약 (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        반환 (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      만약 (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      만약 (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        반환 (unsigned) -1;
    }
    반환 var_data_num_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  void add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, 정수 delta, unsigned item_count)
  {
    만약 (outer >= vars.length) 반환;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    만약 (!tuple.indices.resize (item_count) ||
        !tuple.deltas_x.resize (item_count)) 반환;
    위해 (unsigned i = 0; i < item_count; i++)
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
    위해 (const tuple_variations_t& sub_table : vars)
    {
      위해 (const tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        만약 (!used_regions.has (r))
        {
          bool all_zeros = true;
          위해 (float d : tuple.deltas_x)
          {
            정수 delta = hb_clamp_to<정수> (roundf ((double) d));
            만약 (delta != 0)
            {
              all_zeros = false;
              break;
            }
          }
          만약 (!all_zeros)
          {
            만약 (!used_regions.set (r, 1))
              반환 false;
          }
        }
        만약 (all_regions.has (r))
          continue;
        만약 (!all_regions.set (r, 1))
          반환 false;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    만약 (!all_regions || !all_unique_regions) 반환 true;

    만약 (!region_list.alloc (all_regions.get_population ()))
      반환 false;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    위해 (const auto& r : orig_region_list)
    {
      만약 (!all_regions.has (&r) || !used_regions.has (&r))
        continue;

      region_list.push (&r);
      만약 (!region_map.set (&r, idx))
        반환 false;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    위해 (const auto& r: all_unique_regions)
    {
      만약 (!all_regions.has (r) || !used_regions.has (r))
        continue;
      region_list.push (r);
      만약 (!region_map.set (r, idx))
        반환 false;
      all_regions.del (r);
      idx++;
    }
    반환 (!region_list.in_error ()) && (!region_map.in_error ());
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
      반환 encoded < o.encoded;
    }

    bool operator <= (const combined_gain_idx_tuple_t& o)
    {
      반환 encoded <= o.encoded;
    }

    unsigned idx_1 () const { 반환 (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () const { 반환 encoded & 0xFFFFF; };
  };

  bool as_item_varstore (bool optimize=true, bool use_no_variation_idx=true)
  {
    /* return true if no variation data */
    만약 (!region_list) 반환 true;
    unsigned num_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    위해 (unsigned major = 0; major < var_data_num_rows.length; major++)
      total_rows += var_data_num_rows[major];

    만약 (!delta_rows.resize (total_rows)) 반환 false;
    /* init all rows to [0]*num_cols */
    위해 (unsigned i = 0; i < total_rows; i++)
      만약 (!(delta_rows[i].resize (num_cols))) 반환 false;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, const hb_vector_t<정수>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<const hb_vector_t<정수> *> major_rows;
    hb_hashmap_t<const hb_vector_t<정수>*, unsigned> delta_rows_map;
    위해 (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      const tuple_variations_t& tuples = vars[major];
      unsigned num_rows = var_data_num_rows[major];

      만약 (!num_rows) continue;

      위해 (const tuple_delta_t& tuple: tuples.tuple_vars)
      {
        만약 (tuple.deltas_x.length != num_rows)
          반환 false;

        /* skip unused regions */
        unsigned *col_idx;
        만약 (!region_map.has (&(tuple.axis_tuples), &col_idx))
          continue;

        위해 (unsigned i = 0; i < num_rows; i++)
        {
          정수 rounded_delta = hb_clamp_to<정수> (roundf ((double) tuple.deltas_x[i]));
          정수 &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          만약 (unlikely (sum < INT_MIN || sum > INT_MAX)) 반환 false;
          delta = (정수) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      위해 (unsigned minor = 0; minor < num_rows; minor++)
      {
	const hb_vector_t<정수>& row = delta_rows[start_row + minor];
	만약 (use_no_variation_idx)
	{
	  bool all_zeros = true;
	  위해 (정수 delta : row)
	  {
	    만약 (delta != 0)
	    {
	      all_zeros = false;
	      break;
	    }
	  }
	  만약 (all_zeros)
	    continue;
	}

	만약 (!front_mapping.set ((major<<16) + minor, &row))
	  반환 false;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	만약 (optimize)
	{
	  만약 (delta_rows_map.has (&row))
	    continue;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      만약 (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        반환 false;
      만약 (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), num_cols);
	만약 (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  반환 false;
      }

      start_row += num_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    만약 (!optimize)
    {
      encodings = std::move (encoding_objs);
      반환 !encodings.in_error ();
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
    위해 (unsigned i = 0; i < num_todos; i++)
    {
      위해 (unsigned j = i + 1; j < num_todos; j++)
      {
        정수 combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        만약 (combining_gain > 0)
          queue_items.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_items));

    hb_bit_set_t removed_todo_idxes;
    동안 (queue)
    {
      auto t = queue.pop_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      만약 (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        continue;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      만약 (unlikely (removed_todo_idxes.in_error ())) 반환 false;

      encoding.merge (other_encoding);
      만약 (unlikely (encoding.in_error ())) 반환 false;

      위해 (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        만약 (removed_todo_idxes.has (idx)) continue;

        const delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        만약 (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          위해 (const auto& row : obj.items)
            encoding.add_row (row);
          만약 (unlikely (encoding.in_error ())) 반환 false;

          removed_todo_idxes.add (idx);
          만약 (unlikely (removed_todo_idxes.in_error ())) 반환 false;
          continue;
        }

        정수 combined_gain = encoding.gain_from_merging (obj);
        만약 (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      만약 (unlikely (encoding_objs.in_error ())) 반환 false;
    }

    정수 num_final_encodings = (정수) encoding_objs.length - (정수) removed_todo_idxes.get_population ();
    만약 (num_final_encodings <= 0) 반환 false;

    만약 (!encodings.alloc (num_final_encodings)) 반환 false;
    위해 (unsigned i = 0; i < encoding_objs.length; i++)
    {
      만약 (removed_todo_idxes.has (i)) continue;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    반환 compile_varidx_map (front_mapping);
  }

  private:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  bool compile_varidx_map (const hb_hashmap_t<unsigned, const hb_vector_t<정수>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<const hb_vector_t<정수>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    위해 (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      만약 (encoding.is_empty () || encoding.in_error ())
        반환 false;

      unsigned num_rows = encoding.items.length;
      unsigned num_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.items.qsort (_cmp_row);

      위해 (unsigned start = 0; start < num_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (num_rows - start, 0xFFFFu);
        hb_vector_t<const hb_vector_t<정수>*> rows;

        만약 (!rows.alloc (chunk_len))
          반환 false;

        unsigned major = split_encodings.length;
        위해 (unsigned minor = 0; minor < chunk_len; minor++)
        {
          const hb_vector_t<정수> *row = encoding.items.arrayZ[start + minor];
          rows.push (row);
          만약 (!back_mapping.set (row, (major << 16) + minor))
            반환 false;
        }

        auto *split = split_encodings.push (std::move (rows), num_cols);
        만약 (unlikely (split_encodings.in_error () || split->in_error ()))
          반환 false;
      }
    }

    encodings = std::move (split_encodings);
    만약 (encodings.in_error () || back_mapping.in_error ())
      반환 false;

    위해 (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      만약 (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      아니면
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    반환 !varidx_map.in_error ();
  }

  static 정수 _cmp_row (const void *pa, const void *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    const hb_vector_t<정수>** a = (const hb_vector_t<정수>**) pa;
    const hb_vector_t<정수>** b = (const hb_vector_t<정수>**) pb;

    위해 (unsigned i = 0; i < (*b)->length; i++)
    {
      정수 va = (*a)->arrayZ[i];
      정수 vb = (*b)->arrayZ[i];
      만약 (va != vb)
        반환 va < vb ? -1 : 1;
    }
    반환 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

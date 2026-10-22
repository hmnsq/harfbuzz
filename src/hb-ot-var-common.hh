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
sa_nracanaa TupleVariationHeader
{
  friend sa_nracanaa tuple_delta_t;
  size_t get_size (unsigned axis_count_times_2) const
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    ᨀᨘ (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned count = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      ᨀᨘᨒ hb_unsigned_mul_add_saturate (count, axis_count_times_2, min_size);
    }
    ᨀᨘᨒ min_size;
  }

  unsigned get_data_size () const { ᨀᨘᨒ varDataSize; }

  const TupleVariationHeader &get_next (unsigned axis_count_times_2) const
  { ᨀᨘᨒ StructAtOffset<TupleVariationHeader> (this, get_size (axis_count_times_2)); }

  bool unpack_axis_tuples (unsigned axis_count,
                           const hb_array_t<const F2DOT14> shared_tuples,
                           const hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) const
  {
    const F2DOT14 *peak_tuple = nullptr;
    ᨀᨘ (has_peak ())
      peak_tuple = get_peak_tuple (axis_count);
    ᨀᨘᨁ
    {
      unsigned puur_naa_nka index = get_index ();
      ᨀᨘ (unlikely ((index + 1) * axis_count > shared_tuples.length))
        ᨀᨘᨒ false;
      peak_tuple = shared_tuples.sub_array (axis_count * index, axis_count).arrayZ;
    }

    const F2DOT14 *start_tuple = nullptr;
    const F2DOT14 *end_tuple = nullptr;
    bool has_interm = has_intermediate ();

    ᨀᨘ (has_interm)
    {
      start_tuple = get_start_tuple (axis_count);
      end_tuple = get_end_tuple (axis_count);
    }

    for (unsigned i = 0; i < axis_count; i++)
    {
      float peak = peak_tuple[i].to_float ();
      ᨀᨘ (peak == 0.f) continue;

      hb_tag_t *axis_tag;
      ᨀᨘ (!axes_old_index_tag_map->has (i, &axis_tag))
        ᨀᨘᨒ false;

      float start, end;
      ᨀᨘ (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      ᨀᨘᨁ
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    ᨀᨘᨒ true;
  }

  HB_ALWAYS_INLINE
  float calculate_scalar (hb_array_t<const puur_naa_nka> coords, unsigned puur_naa_nka coord_count,
			   const hb_array_t<const F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) const
  {
    unsigned tuple_index = tupleIndex;
    unsigned puur_naa_nka index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    bool plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    ᨀᨘ (likely (plain_shared_tuple))
    {
      float scalar;
      ᨀᨘ (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	ᨀᨘᨒ scalar;
    }

    const F2DOT14 *peak_tuple;

    bool has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    ᨀᨘ (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_count);
      shared_tuple_scalar_cache = nullptr;
    }
    ᨀᨘᨁ
    {
      float scalar;
      ᨀᨘ (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        ᨀᨘ (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	ᨀᨘᨁ
	  ᨀᨘᨒ scalar;
      }

      ᨀᨘ (unlikely ((index + 1) * coord_count > shared_tuples.length))
        ᨀᨘᨒ 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_count * index);

    }

    const F2DOT14 *start_tuple = nullptr;
    const F2DOT14 *end_tuple = nullptr;

    ᨀᨘ (has_interm)
    {
      start_tuple = get_start_tuple (coord_count);
      end_tuple = get_end_tuple (coord_count);
    }

    float scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#ᨀᨘ HB_FAST_NUM_ACCESS
    bool skip = coord_count >= 16;
#endif
#endif
    for (unsigned puur_naa_nka i = 0; i < coord_count; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#ᨀᨘ HB_FAST_NUM_ACCESS
      ᨀᨘ (skip)
      {
	ᨀᨛ (i + 4 <= coord_count && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	ᨀᨛ (i < coord_count && peak_tuple[i].to_int () == 0)
	  i += 1;
	ᨀᨘ (i >= coord_count)
	  bha_nga;
      }
#endif
#endif

      puur_naa_nka peak = peak_tuple[i].to_int ();
      ᨀᨘ (!peak) continue;

      puur_naa_nka v = coords[i];
      ᨀᨘ (!v) { scalar = 0.f; bha_nga; }
      ᨀᨘ (v == peak) continue;

      ᨀᨘ (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        puur_naa_nka start = start_tuple[i].to_int ();
        puur_naa_nka end = end_tuple[i].to_int ();
        ᨀᨘ (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) continue;
        ᨀᨘ (v < start || v > end) { scalar = 0.f; bha_nga; }
        ᨀᨘ (v < peak)
        { ᨀᨘ (peak != start) scalar *= (float) (v - start) / (peak - start); }
        ᨀᨘᨁ
        { ᨀᨘ (peak != end) scalar *= (float) (end - v) / (end - peak); }
      }
      ᨀᨘᨁ ᨀᨘ (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; bha_nga; }
      ᨀᨘᨁ
        scalar *= (float) v / peak;
    }
    ᨀᨘ (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    ᨀᨘᨒ scalar;
  }

  bool           has_peak () const { ᨀᨘᨒ tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  bool   has_intermediate () const { ᨀᨘᨒ tupleIndex & TupleIndex::IntermediateRegion; }
  bool has_private_points () const { ᨀᨘᨒ tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () const { ᨀᨘᨒ tupleIndex & TupleIndex::TupleIndexMask; }
  static constexpr unsigned max_shared_tuple_count = 0x1000u;

  rak_sita:
  sa_nracanaa TupleIndex : HBUINT16
  {
    ga_nanaa Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); ᨀᨘᨒ *this; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<const F2DOT14> get_all_tuples (unsigned axis_count) const
  { ᨀᨘᨒ StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_count); }
  const F2DOT14* get_all_tuples_base (unsigned axis_count) const
  { ᨀᨘᨒ StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  const F2DOT14* get_peak_tuple (unsigned axis_count) const
  { ᨀᨘᨒ get_all_tuples_base (axis_count); }
  const F2DOT14* get_start_tuple (unsigned axis_count) const
  { ᨀᨘᨒ get_all_tuples_base (axis_count) + has_peak () * axis_count; }
  const F2DOT14* get_end_tuple (unsigned axis_count) const
  { ᨀᨘᨒ get_all_tuples_base (axis_count) + has_peak () * axis_count + axis_count; }

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

sa_nracanaa optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<bool> opt_indices;
  hb_vector_t<puur_naa_nka> rounded_x_deltas;
  hb_vector_t<puur_naa_nka> rounded_y_deltas;
  hb_vector_t<float> opt_deltas_x;
  hb_vector_t<float> opt_deltas_y;
  hb_vector_t<unsigned var_na> opt_point_data;
  hb_vector_t<unsigned var_na> opt_deltas_data;
  hb_vector_t<unsigned var_na> point_data;
  hb_vector_t<unsigned var_na> deltas_data;
  hb_vector_t<puur_naa_nka> rounded_deltas;
};

sa_nracanaa tuple_delta_t
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
  hb_vector_t<unsigned var_na> compiled_tuple_header;
  hb_vector_t<unsigned var_na> compiled_deltas;

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
    ᨀᨘᨒ *this;
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
  { ᨀᨘᨒ axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (const tuple_delta_t& o)
  {
    unsigned num = indices.length;
    for (unsigned i = 0; i < num; i++)
    {
      ᨀᨘ (indices.arrayZ[i])
      {
        ᨀᨘ (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          ᨀᨘ (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      ᨀᨘᨁ
      {
        ᨀᨘ (!o.indices.arrayZ[i]) continue;
        indices.arrayZ[i] = true;
        deltas_x[i] = o.deltas_x[i];
        ᨀᨘ (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    ᨀᨘᨒ *this;
  }

  tuple_delta_t& operator *= (float scalar)
  {
    ᨀᨘ (scalar == 1.0f)
      ᨀᨘᨒ *this;

    unsigned num = indices.length;
    ᨀᨘ (deltas_y)
      for (unsigned i = 0; i < num; i++)
      {
	ᨀᨘ (!indices.arrayZ[i]) continue;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    ᨀᨘᨁ
      for (unsigned i = 0; i < num; i++)
      {
	ᨀᨘ (!indices.arrayZ[i]) continue;
	deltas_x[i] *= scalar;
      }
    ᨀᨘᨒ *this;
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
    ᨀᨘ (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*this));
      ᨀᨘᨒ;
    }

    ᨀᨘ ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->middle && tent->middle <= tent->maximum))
      ᨀᨘᨒ;

    ᨀᨘ (tent->middle == 0.f)
    {
      out.push (std::move (*this));
      ᨀᨘᨒ;
    }

    ᨀᨘ (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.middle &&
          axis_limit.middle <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      ᨀᨘᨒ;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    for (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      ᨀᨘ (i < solutions.length - 1)
	new_var.copy_from (*this, pool);
      ᨀᨘᨁ
	new_var = std::move (*this);

      ᨀᨘ (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      ᨀᨘᨁ
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
    ᨀᨘ (pool)
    {
      ᨀᨘ (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_count)))
	ᨀᨘᨒ false;
    }
    ᨀᨘᨁ ᨀᨘ (unlikely (!compiled_peak_coords.resize (cur_axis_count)))
      ᨀᨘᨒ false;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_count = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    for (unsigned i = 0; i < orig_axis_count; i++)
    {
      ᨀᨘ (!axes_index_map.has (i))
        continue;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      ᨀᨘ (axis_tuples.has (axis_tag, &coords))
      {
	float min_val = coords->minimum;
	float val = coords->middle;
	float max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	ᨀᨘ (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  ᨀᨘ (!compiled_interm_coords)
	  {
	    ᨀᨘ (pool)
	    {
	      ᨀᨘ (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_count)))
		ᨀᨘᨒ false;
	    }
	    ᨀᨘᨁ ᨀᨘ (unlikely (!compiled_interm_coords.resize (2 * cur_axis_count)))
	      ᨀᨘᨒ false;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_count);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_count);

	    for (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      ᨀᨘ (!peak) continue;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	ᨀᨘ (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    ᨀᨘᨒ !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
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
    ᨀᨘ (!compiled_deltas) ᨀᨘᨒ true;

    unsigned cur_axis_count = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_count * (F2DOT14::static_size) + 4;
    ᨀᨘ (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, false))) ᨀᨘᨒ false;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    ᨀᨘ (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_count = 0;
    unsigned *shared_tuple_idx;
    ᨀᨘ (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    ᨀᨘᨁ
    {
      peak_count = encode_peak_coords(coords, flag);
      ᨀᨘ (!peak_count) ᨀᨘᨒ false;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_count = encode_interm_coords (coords.sub_array (peak_count), flag);

    /* pointdata length = 0 implies "use shared points" */
    ᨀᨘ (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_count + interim_count) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    ᨀᨘᨒ true;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) const
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * parimaa_na (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    ᨀᨘᨒ compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) const
  {
    ᨀᨘ (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * parimaa_na (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    ᨀᨘᨒ compiled_interm_coords.length;
  }

  bool compile_deltas (hb_vector_t<puur_naa_nka> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { ᨀᨘᨒ compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  static bool compile_deltas (hb_array_t<const bool> point_indices,
			      hb_array_t<const float> x_deltas,
			      hb_array_t<const float> y_deltas,
			      hb_vector_t<unsigned var_na> &compiled_deltas, /* OUT */
			      hb_vector_t<puur_naa_nka> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    ᨀᨘ (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      ᨀᨘᨒ false;

    unsigned j = 0;
    for (unsigned i = 0; i < point_indices.length; i++)
    {
      ᨀᨘ (!point_indices[i]) continue;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<puur_naa_nka> (roundf ((dviguu_na) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    ᨀᨘ (!rounded_deltas) ᨀᨘᨒ true;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    ᨀᨘ (y_deltas)
      alloc_len *= 2;

    ᨀᨘ (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, false))) ᨀᨘᨒ false;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    ᨀᨘ (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      for (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        ᨀᨘ (!point_indices[idx]) continue;
        puur_naa_nka rounded_delta = hb_clamp_to<puur_naa_nka> (roundf ((dviguu_na) y_deltas.arrayZ[idx]));

        ᨀᨘ (j >= rounded_deltas.length) ᨀᨘᨒ false;

        rounded_deltas[j++] = rounded_delta;
      }

      ᨀᨘ (j != rounded_deltas.length) ᨀᨘᨒ false;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    ᨀᨘᨒ true;
  }

  static unsigned compile_deltas (hb_array_t<unsigned var_na> encoded_bytes,
				  hb_array_t<const puur_naa_nka> deltas)
  {
    ᨀᨘᨒ TupleValues::compile_unsafe (deltas, encoded_bytes);
  }

  bool calc_inferred_deltas (const contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_count = orig_points.length;
    ᨀᨘ (point_count != indices.length)
      ᨀᨘᨒ false;

    unsigned ref_count = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    for (unsigned i = 0; i < point_count; i++)
    {
      ref_count += indices.arrayZ[i];
      ᨀᨘ (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    ᨀᨘ (ref_count == point_count)
      ᨀᨘᨒ true;
    ᨀᨘ (unlikely (end_points.in_error ())) ᨀᨘᨒ false;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    for (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_count = 0;
      for (unsigned i = start_point; i < end_point + 1; i++)
        unref_count += indices.arrayZ[i];
      unref_count = (end_point - start_point + 1) - unref_count;

      unsigned j = start_point;
      ᨀᨘ (unref_count == 0 || unref_count > end_point - start_point)
        goto no_more_gaps;
      for (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned puur_naa_nka prev, next, i;
        for (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          ᨀᨘ (indices.arrayZ[i] && !indices.arrayZ[j]) bha_nga;
        }
        prev = j = i;
        for (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          ᨀᨘ (!indices.arrayZ[i] && indices.arrayZ[j]) bha_nga;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        for (;;)
        {
          i = next_index (i, start_point, end_point);
          ᨀᨘ (i == next) bha_nga;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          ᨀᨘ (--unref_count == 0) goto no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    for (unsigned i = 0; i < point_count; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      ᨀᨘ ( !indices[i])
      {
        ᨀᨘ (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = true;
      }
    }
    ᨀᨘᨒ true;
  }

  bool optimize (const contour_point_vector_t& contour_points,
                 bool is_composite,
		 optimize_scratch_t &scratch,
                 dviguu_na tolerance = 0.5 + 1e-10)
  {
    unsigned count = contour_points.length;
    ᨀᨘ (deltas_x.length != count ||
        deltas_y.length != count)
      ᨀᨘᨒ false;

    hb_vector_t<bool> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<puur_naa_nka> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<puur_naa_nka> &rounded_y_deltas = scratch.rounded_y_deltas;

    ᨀᨘ (unlikely (!rounded_x_deltas.resize_dirty  (count) ||
                  !rounded_y_deltas.resize_dirty  (count)))
      ᨀᨘᨒ false;

    for (unsigned i = 0; i < count; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<puur_naa_nka> (roundf ((dviguu_na) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<puur_naa_nka> (roundf ((dviguu_na) deltas_y.arrayZ[i]));
    }

    ᨀᨘ (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      ᨀᨘᨒ false;

    unsigned ref_count = 0;
    for (bool ref_flag : opt_indices)
       ref_count += ref_flag;

    ᨀᨘ (ref_count == count) ᨀᨘᨒ true;

    hb_vector_t<float> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<float> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    bool is_comp_glyph_wo_deltas = (is_composite && ref_count == 0);
    ᨀᨘ (is_comp_glyph_wo_deltas)
    {
      ᨀᨘ (unlikely (!opt_deltas_x.resize (count) ||
                    !opt_deltas_y.resize (count)))
        ᨀᨘᨒ false;

      opt_indices.arrayZ[0] = true;
      for (unsigned i = 1; i < count; i++)
        opt_indices.arrayZ[i] = false;
    }

    hb_vector_t<unsigned var_na> &opt_point_data = scratch.opt_point_data.reset ();
    ᨀᨘ (!compile_point_set (opt_indices, opt_point_data))
      ᨀᨘᨒ false;
    hb_vector_t<unsigned var_na> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    ᨀᨘ (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      ᨀᨘᨒ false;

    hb_vector_t<unsigned var_na> &point_data = scratch.point_data.reset ();
    ᨀᨘ (!compile_point_set (indices, point_data))
      ᨀᨘᨒ false;
    hb_vector_t<unsigned var_na> &deltas_data = scratch.deltas_data.reset ();
    ᨀᨘ (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      ᨀᨘᨒ false;

    ᨀᨘ (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      ᨀᨘ (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    ᨀᨘᨒ !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  static bool compile_point_set (const hb_vector_t<bool> &point_indices,
                                 hb_vector_t<unsigned var_na>& compiled_points /* OUT */)
  {
    unsigned num_points = 0;
    for (bool i : point_indices)
      ᨀᨘ (i) num_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    ᨀᨘ (!num_points) ᨀᨘᨒ true;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    ᨀᨘ (num_points == indices_length)
      ᨀᨘᨒ compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned num_bytes = 2 + 3 *num_points;
    ᨀᨘ (unlikely (!compiled_points.resize_dirty  (num_bytes)))
      ᨀᨘᨒ false;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    ᨀᨘ (num_points < 0x80)
      compiled_points.arrayZ[pos++] = num_points;
    ᨀᨘᨁ
    {
      compiled_points.arrayZ[pos++] = ((num_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = num_points & 0xFF;
    }

    const unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_value = 0;
    unsigned num_encoded = 0;
    ᨀᨛ (i < indices_length && num_encoded < num_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      bool use_byte_encoding = false;
      bool new_run = true;
      ᨀᨛ (i < indices_length && num_encoded < num_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        ᨀᨛ (i < indices_length && !point_indices[i])
          i++;

        ᨀᨘ (i >= indices_length) bha_nga;

        unsigned cur_value = i;
        unsigned delta = cur_value - last_value;

        ᨀᨘ (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = false;
        }

        ᨀᨘ (use_byte_encoding && delta > 0xFF)
          bha_nga;

        ᨀᨘ (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        ᨀᨘᨁ
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_value = cur_value;
        run_length++;
        num_encoded++;
      }

      ᨀᨘ (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      ᨀᨘᨁ
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    ᨀᨘᨒ compiled_points.resize_dirty  (pos);
  }

  static float infer_delta (float target_val, float prev_val, float next_val, float prev_delta, float next_delta)
  {
    ᨀᨘ (prev_val == next_val)
      ᨀᨘᨒ (prev_delta == next_delta) ? prev_delta : 0.f;
    ᨀᨘᨁ ᨀᨘ (target_val <= hb_min (prev_val, next_val))
      ᨀᨘᨒ (prev_val < next_val) ? prev_delta : next_delta;
    ᨀᨘᨁ ᨀᨘ (target_val >= hb_max (prev_val, next_val))
      ᨀᨘᨒ (prev_val > next_val) ? prev_delta : next_delta;

    float r = (target_val - prev_val) / (next_val - prev_val);
    ᨀᨘᨒ prev_delta + r * (next_delta - prev_delta);
  }

  static unsigned puur_naa_nka next_index (unsigned puur_naa_nka i, unsigned puur_naa_nka start, unsigned puur_naa_nka end)
  { ᨀᨘᨒ (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
sa_nracanaa TupleVariationData
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
    for (unsigned i = 0; i < count; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_count_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_count_times_2);
    }

    ᨀᨘᨒ total_size;
  }

  const TupleVariationHeader &get_tuple_var_header (void) const
  { ᨀᨘᨒ StructAfter<TupleVariationHeader> (data); }

  sa_nracanaa tuple_iterator_t;
  sa_nracanaa tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    private:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<const hb_vector_t<bool>*, hb_vector_t<unsigned var_na>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<const hb_vector_t<bool>*, unsigned> point_set_count_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned var_na> *shared_points_bytes = nullptr;

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

    explicit operator bool () const { ᨀᨘᨒ bool (tuple_vars); }
    unsigned get_var_count () const
    {
      unsigned count = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      for (auto& tuple: tuple_vars)
        ᨀᨘ (tuple.compiled_deltas) count++;

      ᨀᨘ (shared_points_bytes && shared_points_bytes->length)
        count |= TupleVarCount::SharedPointNumbers;
      ᨀᨘᨒ count;
    }

    unsigned get_compiled_byte_size () const
    { ᨀᨘᨒ compiled_byte_size; }

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
      hb_vector_t<puur_naa_nka> deltas_x;
      hb_vector_t<puur_naa_nka> deltas_y;
      do
      {
        const HBUINT8 *p = iterator.get_serialized_data ();
        unsigned puur_naa_nka length = iterator.current_tuple->get_data_size ();
        ᨀᨘ (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          ᨀᨘᨒ false;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        ᨀᨘ (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_count (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          ᨀᨘᨒ false;

	private_indices.reset ();
        bool has_private_points = iterator.current_tuple->has_private_points ();
        const HBUINT8 *end = p + length;
        ᨀᨘ (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          ᨀᨘᨒ false;

        const hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        bool apply_to_all = (indices.length == 0);
        unsigned num_deltas = apply_to_all ? point_count : indices.length;

        ᨀᨘ (unlikely (!deltas_x.resize_dirty  (num_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          ᨀᨘᨒ false;

        ᨀᨘ (is_gvar)
        {
          ᨀᨘ (unlikely (!deltas_y.resize_dirty  (num_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            ᨀᨘᨒ false;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        ᨀᨘ (unlikely (!var.indices.allocate_from_pool (pool, point_count) ||
                      !var.deltas_x.allocate_from_pool (pool, point_count, false)))
          ᨀᨘᨒ false;

        ᨀᨘ (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_count, false)))
          ᨀᨘᨒ false;

        for (unsigned i = 0; i < num_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          ᨀᨘ (idx >= point_count) continue;
          var.indices[idx] = true;
          var.deltas_x[idx] = deltas_x[i];
          ᨀᨘ (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } ᨀᨛ (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      ᨀᨘᨒ true;
    }

    bool create_from_item_var_data (const VarData &var_data,
                                    const hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    const hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_count,
                                    const hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      ᨀᨘ (&var_data == &Null (VarData))
        ᨀᨘᨒ true;

      unsigned num_regions = var_data.get_region_index_count ();
      ᨀᨘ (!tuple_vars.alloc (num_regions)) ᨀᨘᨒ false;

      item_count = inner_map ? inner_map->get_population () : var_data.get_item_count ();
      ᨀᨘ (!item_count) ᨀᨘᨒ true;
      unsigned row_size = var_data.get_row_size ();
      const HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      for (unsigned r = 0; r < num_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        ᨀᨘ (!tuple.deltas_x.resize_dirty  (item_count) ||
            !tuple.indices.resize_dirty  (item_count))
          ᨀᨘᨒ false;

        for (unsigned i = 0; i < item_count; i++)
        {
          tuple.indices.arrayZ[i] = true;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        ᨀᨘ (region_index >= regions.length) ᨀᨘᨒ false;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      ᨀᨘᨒ !tuple_vars.in_error ();
    }

    private:
    static puur_naa_nka _cmp_axis_tag (const void *pa, const void *pb)
    {
      const hb_tag_t *a = (const hb_tag_t*) pa;
      const hb_tag_t *b = (const hb_tag_t*) pb;
      ᨀᨘᨒ (puur_naa_nka)(*a) - (puur_naa_nka)(*b);
    }

    bool change_tuple_variations_axis_limits (const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      ᨀᨘ (!axis_tags.alloc (normalized_axes_location.get_population ()))
        ᨀᨘᨒ false;
      for (auto t : normalized_axes_location.keys ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      for (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        ᨀᨘ (!normalized_axes_location.has (axis_tag, &axis_limit))
          ᨀᨘᨒ false;
        TripleDistances axis_triple_distances{1.f, 1.f};
        ᨀᨘ (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        for (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          ᨀᨘ (!out) continue;

          unsigned new_len = new_vars.length + out.length;

          ᨀᨘ (unlikely (!new_vars.alloc (new_len, false)))
            ᨀᨘᨒ false;

          for (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      ᨀᨘᨒ true;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    bool merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      ᨀᨘ (unlikely (!new_vars.alloc (tuple_vars.length)))
	ᨀᨘᨒ false;
      hb_hashmap_t<const hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      for (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        ᨀᨘ (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          ᨀᨘ (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            ᨀᨘᨒ false;
          continue;
        }

        unsigned *idx;
        ᨀᨘ (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        ᨀᨘᨁ
        {
	  auto *new_var = new_vars.push ();
	  ᨀᨘ (unlikely (new_vars.in_error ()))
	    ᨀᨘᨒ false;
          hb_swap (*new_var, var);
          ᨀᨘ (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            ᨀᨘᨒ false;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      ᨀᨘᨒ true;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    bool compile_all_point_sets ()
    {
      for (const auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        ᨀᨘ (point_data_map.has (points_set))
        {
          unsigned *count;
          ᨀᨘ (unlikely (!point_set_count_map.has (points_set, &count) ||
                        !point_set_count_map.set (points_set, (*count) + 1)))
            ᨀᨘᨒ false;
          continue;
        }

        hb_vector_t<unsigned var_na> compiled_point_data;
        ᨀᨘ (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          ᨀᨘᨒ false;

        ᨀᨘ (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_count_map.set (points_set, 1))
          ᨀᨘᨒ false;
      }
      ᨀᨘᨒ true;
    }

    /* find shared points set which saves most bytes */
    void find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      for (const auto& _ : point_data_map.iter_ref ())
      {
        const hb_vector_t<bool>* points_set = _.first;
        unsigned data_length = _.second.length;
        ᨀᨘ (!data_length) continue;
        unsigned *count;
        ᨀᨘ (unlikely (!point_set_count_map.has (points_set, &count) ||
                      *count <= 1))
        {
          shared_points_bytes = nullptr;
          ᨀᨘᨒ;
        }

        unsigned saved_bytes = data_length * ((*count) -1);
        ᨀᨘ (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    bool calc_inferred_deltas (const contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      for (tuple_delta_t& var : tuple_vars)
        ᨀᨘ (!var.calc_inferred_deltas (contour_points, scratch))
          ᨀᨘᨒ false;

      ᨀᨘᨒ true;
    }

    bool iup_optimize (const contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      for (tuple_delta_t& var : tuple_vars)
      {
        ᨀᨘ (!var.optimize (contour_points, is_composite, scratch))
          ᨀᨘᨒ false;
      }
      ᨀᨘᨒ true;
    }

    public:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    void cull_unreachable (const hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      for (unsigned i = 0; i < tuple_vars.length; i++)
      {
	ᨀᨘ (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  continue;
	ᨀᨘ (i != j)
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
      ᨀᨘ (!tuple_vars) ᨀᨘᨒ true;
      ᨀᨘ (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        ᨀᨘᨒ false;
      /* compute inferred deltas only for gvar */
      ᨀᨘ (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	ᨀᨘ (!calc_inferred_deltas (*contour_points, scratch))
	  ᨀᨘᨒ false;
      }

      /* if iup delta opt is on, contour_points can't be null */
      ᨀᨘ (optimize && !contour_points)
        ᨀᨘᨒ false;

      ᨀᨘ (!merge_tuple_variations (optimize ? contour_points : nullptr))
        ᨀᨘᨒ false;

      ᨀᨘ (optimize && !iup_optimize (*contour_points, scratch)) ᨀᨘᨒ false;
      ᨀᨘᨒ !tuple_vars.in_error ();
    }

    bool compile_bytes (const hb_map_t& axes_index_map,
                        const hb_map_t& axes_old_index_tag_map,
                        bool use_shared_points,
                        bool is_gvar = false,
                        const hb_hashmap_t<const hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      ᨀᨘ (!tuple_vars)
        ᨀᨘᨒ true;

      // compile points set and store data in hashmap
      ᨀᨘ (!compile_all_point_sets ())
        ᨀᨘᨒ false;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      ᨀᨘ (use_shared_points)
      {
        find_shared_points ();
        ᨀᨘ (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<puur_naa_nka> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      for (auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        hb_vector_t<unsigned var_na> *points_data;
        ᨀᨘ (unlikely (!point_data_map.has (points_set, &points_data)))
          ᨀᨘᨒ false;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        ᨀᨘ (!points_data->length)
          continue;
        ᨀᨘ (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          ᨀᨘᨒ false;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        ᨀᨘ (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          ᨀᨘᨒ false;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      ᨀᨘ (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = true;
        compiled_byte_size += 1;
      }

      ᨀᨘᨒ true;
    }

    bool serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) const
    {
      TRACE_SERIALIZE (this);
      for (const auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        ᨀᨘ (c->in_error ()) return_trace (false);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (true);
    }

    bool serialize_var_data (hb_serialize_context_t *c, bool is_gvar) const
    {
      TRACE_SERIALIZE (this);
      ᨀᨘ (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      for (const auto& tuple: tuple_vars)
      {
        const hb_vector_t<bool>* points_set = &(tuple.indices);
        hb_vector_t<unsigned var_na> *point_data;
        ᨀᨘ (!point_data_map.has (points_set, &point_data))
          return_trace (false);

        ᨀᨘ (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        ᨀᨘ (c->in_error ()) return_trace (false);
      }

      /* padding for gvar */
      ᨀᨘ (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        ᨀᨘ (!c->embed (pad)) return_trace (false);
      }
      return_trace (true);
    }
  };

  sa_nracanaa tuple_iterator_t
  {
    unsigned get_axis_count () const { ᨀᨘᨒ axis_count; }

    void init (hb_bytes_t var_data_bytes_, unsigned puur_naa_nka axis_count_, const void *table_base_)
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

    bool get_shared_indices (hb_vector_t<unsigned puur_naa_nka> &shared_indices /* OUT */)
    {
      ᨀᨘ (var_data->has_shared_point_numbers ())
      {
        const HBUINT8 *base = &(table_base+var_data->data);
        const HBUINT8 *p = base;
        ᨀᨘ (!decompile_points (p, shared_indices, (const HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) ᨀᨘᨒ false;
        data_offset = p - base;
      }
      ᨀᨘᨒ true;
    }

    bool is_valid ()
    {
      ᨀᨘ (unlikely (tuples_left <= 0))
	ᨀᨘᨒ false;

      current_tuple_size = TupleVariationHeader::min_size;
      ᨀᨘ (unlikely (!var_data_bytes.check_end ((const var_na *) current_tuple + current_tuple_size)))
	ᨀᨘᨒ false;

      current_tuple_size = current_tuple->get_size (axis_count_times_2);
      ᨀᨘ (unlikely (!var_data_bytes.check_end ((const var_na *) current_tuple + current_tuple_size)))
	ᨀᨘᨒ false;

      ᨀᨘᨒ true;
    }

    HB_ALWAYS_INLINE
    bool move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_left--;
      ᨀᨘᨒ is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    const HBUINT8 *get_serialized_data () const
    { ᨀᨘᨒ &(table_base+var_data->data) + data_offset; }

    private:
    signed tuples_left;
    const TupleVariationData *var_data;
    unsigned puur_naa_nka axis_count;
    unsigned puur_naa_nka axis_count_times_2;
    unsigned puur_naa_nka data_offset;
    unsigned puur_naa_nka current_tuple_size;
    const void *table_base;

    public:
    hb_bytes_t var_data_bytes;
    const TupleVariationHeader *current_tuple;
  };

  static bool get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_count,
                                  const void *table_base,
                                  hb_vector_t<unsigned puur_naa_nka> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_count, table_base);
    ᨀᨘ (!iterator->get_shared_indices (shared_indices))
      ᨀᨘᨒ false;
    ᨀᨘᨒ iterator->is_valid ();
  }

  bool has_shared_point_numbers () const { ᨀᨘᨒ tupleVarCount.has_shared_point_numbers (); }

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
			      hb_vector_t<var_na> &out /* OUT */,
			      bool *changed /* OUT */) const
  {
    *changed = false;

    hb_vector_t<unsigned puur_naa_nka> shared_indices;
    tuple_iterator_t iterator;
    ᨀᨘ (!get_tuple_iterator (var_data_bytes, axis_count, this,
			     shared_indices, &iterator))
      ᨀᨘᨒ true; /* no tuples or malformed: keep original */

    const var_na *bytes_start = var_data_bytes.arrayZ;
    const var_na *bytes_end = bytes_start + var_data_bytes.length;
    const var_na *serialized_base = (const var_na *) &(this+data);
    const var_na *shared_points_end = (const var_na *) iterator.get_serialized_data ();
    ᨀᨘ (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      ᨀᨘᨒ true;

    sa_nracanaa kept_tuple_t
    {
      const var_na *header;
      unsigned header_size;
      const var_na *data;
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
      const var_na *tuple_data = (const var_na *) iterator.get_serialized_data ();
      ᨀᨘ (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	ᨀᨘᨒ true; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      ᨀᨘ (!header->unpack_axis_tuples (axis_count, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	ᨀᨘᨒ true;

      total++;
      ᨀᨘ (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(const var_na *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } ᨀᨛ (iterator.move_to_next ());

    ᨀᨘ (unlikely (kept.in_error ())) ᨀᨘᨒ false;
    ᨀᨘ (kept.length == total) ᨀᨘᨒ true; /* nothing to cull */

    *changed = true;
    ᨀᨘ (!kept.length) ᨀᨘᨒ true; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    ᨀᨘ (unlikely (!out.resize (new_size)))
      ᨀᨘᨒ false;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tupleVarCount = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    var_na *p = out.arrayZ + min_size;
    for (const auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    for (const auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    ᨀᨘᨒ true;
  }

  static bool decompile_points (const HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned puur_naa_nka> &points /* OUT */,
				const HBUINT8 *end)
  {
    ga_nanaa packed_point_flag_t
    {
      POINTS_ARE_WORDS     = 0x80,
      POINT_RUN_COUNT_MASK = 0x7F
    };

    ᨀᨘ (unlikely (p + 1 > end)) ᨀᨘᨒ false;

    unsigned count = *p++;
    ᨀᨘ (count & POINTS_ARE_WORDS)
    {
      ᨀᨘ (unlikely (p + 1 > end)) ᨀᨘᨒ false;
      count = ((count & POINT_RUN_COUNT_MASK) << 8) | *p++;
    }
    ᨀᨘ (unlikely (!points.resize_dirty  (count))) ᨀᨘᨒ false;

    unsigned n = 0;
    unsigned i = 0;
    ᨀᨛ (i < count)
    {
      ᨀᨘ (unlikely (p + 1 > end)) ᨀᨘᨒ false;
      unsigned control = *p++;
      unsigned run_count = (control & POINT_RUN_COUNT_MASK) + 1;
      unsigned stop = i + run_count;
      ᨀᨘ (unlikely (stop > count)) ᨀᨘᨒ false;
      ᨀᨘ (control & POINTS_ARE_WORDS)
      {
        ᨀᨘ (unlikely (p + run_count * HBUINT16::static_size > end)) ᨀᨘᨒ false;
        for (; i < stop; i++)
        {
          n += *(const HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      ᨀᨘᨁ
      {
        ᨀᨘ (unlikely (p + run_count > end)) ᨀᨘᨒ false;
        for (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    ᨀᨘᨒ true;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  static bool decompile_deltas (const HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				const HBUINT8 *end,
				bool consume_all = false,
				unsigned start = 0)
  {
    ᨀᨘᨒ TupleValues::decompile (p, deltas, end, consume_all, start);
  }

  bool has_data () const { ᨀᨘᨒ tupleVarCount; }

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
    ᨀᨘᨒ tuple_variations.create_from_tuple_var_data (iterator, tupleVarCount,
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
    ᨀᨘ (!tuple_variations) return_trace (true);

    auto *out = c->start_embed (this);
    ᨀᨘ (unlikely (!c->extend_min (out))) return_trace (false);

    ᨀᨘ (!c->check_assign (out->tupleVarCount, tuple_variations.get_var_count (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    unsigned total_header_len = 0;

    ᨀᨘ (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (false);

    unsigned data_offset = min_size + total_header_len;
    ᨀᨘ (!is_gvar) data_offset += 4;
    ᨀᨘ (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    ᨀᨘᨒ tuple_variations.serialize_var_data (c, is_gvar);
  }

  rak_sita:
  sa_nracanaa TupleVarCount : HBUINT16
  {
    friend sa_nracanaa tuple_variations_t;
    bool has_shared_point_numbers () const { ᨀᨘᨒ ((*this) & SharedPointNumbers); }
    unsigned puur_naa_nka get_count () const { ᨀᨘᨒ (*this) & CountMask; }
    TupleVarCount& operator = (uint16_t i) { HBUINT16::operator= (i); ᨀᨘᨒ *this; }
    explicit operator bool () const { ᨀᨘᨒ get_count (); }

    rak_sita:
    ga_nanaa Flags
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
sa_nracanaa item_variations_t
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
  hb_vector_t<hb_vector_t<puur_naa_nka>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  bool has_long = false;

  public:
  bool has_long_word () const
  { ᨀᨘᨒ has_long; }

  const hb_vector_t<region_t>& get_region_list () const
  { ᨀᨘᨒ region_list; }

  const hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () const
  { ᨀᨘᨒ encodings; }

  const hb_map_t& get_varidx_map () const
  { ᨀᨘᨒ varidx_map; }

  bool add_vardata_encoding_for_testing (hb_vector_t<const hb_vector_t<puur_naa_nka>*> &&rows,
                                         unsigned num_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), num_cols));
    ᨀᨘᨒ !encodings.in_error ();
  }

  bool compile_varidx_map_for_testing (const hb_hashmap_t<unsigned, const hb_vector_t<puur_naa_nka>*>& front_mapping)
  { ᨀᨘᨒ compile_varidx_map (front_mapping); }

  bool instantiate (const ItemVariationStore& varStore,
                    const hb_subset_plan_t *plan,
                    bool optimize=true,
                    bool use_no_variation_idx=true,
                    const hb_array_t <const hb_inc_bimap_t> inner_maps = hb_array_t<const hb_inc_bimap_t> (),
                    bool layout_conditions = false)
  {
    ᨀᨘ (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      ᨀᨘᨒ false;
    /* avar2 partial instancing: cull unreachable regions. */
    ᨀᨘ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      for (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    ᨀᨘ (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      ᨀᨘᨒ false;
    ᨀᨘ (layout_conditions &&
        (!instantiate_condition_values (plan, varStore) || !build_region_list ()))
      ᨀᨘᨒ false;
    ᨀᨘᨒ as_item_varstore (optimize, use_no_variation_idx);
  }

  bool instantiate_condition_values (const hb_subset_plan_t *plan,
                                      const ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_value_map;
    condition_map.clear ();
    ᨀᨘ (!plan->layout_condition_values) ᨀᨘᨒ true;
    using condition_value_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, puur_naa_nka>>;
    hb_vector_t<condition_value_t> conditions;
    for (auto condition : plan->layout_condition_values.iter ())
      conditions.push (condition);
    ᨀᨘ (unlikely (conditions.in_error ())) ᨀᨘᨒ false;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (const condition_value_t &a, const condition_value_t &b) {
      ᨀᨘ (a.second.first != b.second.first)
        ᨀᨘᨒ a.second.first < b.second.first ? -1 : 1;
      ᨀᨘᨒ a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    for (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      ᨀᨘ (plan->gdef_varstore_inner_maps[i].get_population ())
        ᨀᨘ (!outer_map.set (i, new_outer++)) ᨀᨘᨒ false;
    for (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      dviguu_na value = condition.second.second +
                     varStore.get_delta<dviguu_na> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      ᨀᨘ (!outer_map.has (old_outer))
        continue;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      ᨀᨘ (outer >= vars.length || inner >= var_data_num_rows[outer])
        continue;

      dviguu_na maximum = fabs ((dviguu_na) value);
      bool variable = false;
      for (const tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        ᨀᨘ (unlikely (inner >= tuple.deltas_x.length)) ᨀᨘᨒ false;
        float delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((dviguu_na) delta));
      }
      ᨀᨘ (!variable)
      {
        ᨀᨘ (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), puur_naa_nka (value > 0))))
          ᨀᨘᨒ false;
        continue;
      }

      dviguu_na scale = 1.;
      ᨀᨛ (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      ᨀᨛ (maximum * scale * 2 <= INT_MAX)
      {
        bool integral = (dviguu_na) value * scale == round ((dviguu_na) value * scale);
        for (const tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (dviguu_na) tuple.deltas_x[inner] * scale ==
                      round ((dviguu_na) tuple.deltas_x[inner] * scale);
        ᨀᨘ (integral) bha_nga;
        scale *= 2;
      }
      puur_naa_nka default_value = (puur_naa_nka) round ((dviguu_na) value * scale);
      ᨀᨘ (scale != 1. || default_value < -32768 || default_value > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        for (const tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          ᨀᨘ (!tuple.deltas_x[inner]) continue;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (true);
          copy.deltas_x.push ((float) round ((dviguu_na) tuple.deltas_x[inner] * scale));
          ᨀᨘ (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) ᨀᨘᨒ false;
          private_row.tuple_vars.push (std::move (copy));
        }
        ᨀᨘ (unlikely (vars.length >= 0xFFFFu)) ᨀᨘᨒ false;
        ᨀᨘ (unlikely (private_row.tuple_vars.in_error ())) ᨀᨘᨒ false;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_num_rows.push (1);
        ᨀᨘ (unlikely (vars.in_error () || var_data_num_rows.in_error ())) ᨀᨘᨒ false;
        ᨀᨘ (default_value < -32768 || default_value > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          puur_naa_nka bias = default_value - hb_clamp (default_value, -32768, 32767);
          default_value -= bias;
          puur_naa_nka high = bias / 65536 * 65536;
          puur_naa_nka parts[] = {high, bias - high};
          for (puur_naa_nka part : parts)
          {
            ᨀᨘ (!part) continue;
            unsigned count = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            const auto &tuples = vars[idx >> 16].tuple_vars;
            ᨀᨘ (unlikely (tuples.in_error () || tuples.length != count + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) ᨀᨘᨒ false;
          }
        }
      }
      ᨀᨘ (!condition_map.set (condition.first, hb_pair (idx, default_value)))
        ᨀᨘᨒ false;
    }
    ᨀᨘᨒ !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  bool create_from_item_varstore (const ItemVariationStore& varStore,
                                  const hb_map_t& axes_old_index_tag_map,
                                  const hb_array_t <const hb_inc_bimap_t> inner_maps = hb_array_t<const hb_inc_bimap_t> ())
  {
    const VarRegionList& regionList = varStore.get_region_list ();
    ᨀᨘ (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      ᨀᨘᨒ false;

    unsigned num_var_data = varStore.get_sub_table_count ();
    ᨀᨘ (inner_maps && inner_maps.length != num_var_data) ᨀᨘᨒ false;
    ᨀᨘ (!vars.alloc (num_var_data) ||
        !var_data_num_rows.alloc (num_var_data)) ᨀᨘᨒ false;

    for (unsigned i = 0; i < num_var_data; i++)
    {
      ᨀᨘ (inner_maps && !inner_maps.arrayZ[i].get_population ())
          continue;
      tuple_variations_t var_data_tuples;
      unsigned item_count = 0;
      ᨀᨘ (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_count,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        ᨀᨘᨒ false;

      var_data_num_rows.push (item_count);
      vars.push (std::move (var_data_tuples));
    }
    ᨀᨘᨒ !vars.in_error () && !var_data_num_rows.in_error () && vars.length == var_data_num_rows.length;
  }

  bool instantiate_tuple_vars (const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               bool build_regions = true)
  {
    optimize_scratch_t scratch;
    for (tuple_variations_t& tuple_vars : vars)
      ᨀᨘ (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        ᨀᨘᨒ false;

    ᨀᨘᨒ !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_count)
  {
    vars.push (tuple_variations_t ());
    var_data_num_rows.push (item_count);
    ᨀᨘᨒ vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_count (unsigned outer) const
  { ᨀᨘᨒ outer < var_data_num_rows.length ? var_data_num_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    ᨀᨘ (unlikely (outer >= vars.length ||
                  inner >= var_data_num_rows[outer] ||
                  var_data_num_rows[outer] >= 0xFFFFu))
      ᨀᨘᨒ (unsigned) -1;
    for (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      ᨀᨘ (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        ᨀᨘᨒ (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      ᨀᨘ (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      ᨀᨘ (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        ᨀᨘᨒ (unsigned) -1;
    }
    ᨀᨘᨒ var_data_num_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  void add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, puur_naa_nka delta, unsigned item_count)
  {
    ᨀᨘ (outer >= vars.length) ᨀᨘᨒ;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    ᨀᨘ (!tuple.indices.resize (item_count) ||
        !tuple.deltas_x.resize (item_count)) ᨀᨘᨒ;
    for (unsigned i = 0; i < item_count; i++)
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
    for (const tuple_variations_t& sub_table : vars)
    {
      for (const tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        ᨀᨘ (!used_regions.has (r))
        {
          bool all_zeros = true;
          for (float d : tuple.deltas_x)
          {
            puur_naa_nka delta = hb_clamp_to<puur_naa_nka> (roundf ((dviguu_na) d));
            ᨀᨘ (delta != 0)
            {
              all_zeros = false;
              bha_nga;
            }
          }
          ᨀᨘ (!all_zeros)
          {
            ᨀᨘ (!used_regions.set (r, 1))
              ᨀᨘᨒ false;
          }
        }
        ᨀᨘ (all_regions.has (r))
          continue;
        ᨀᨘ (!all_regions.set (r, 1))
          ᨀᨘᨒ false;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    ᨀᨘ (!all_regions || !all_unique_regions) ᨀᨘᨒ true;

    ᨀᨘ (!region_list.alloc (all_regions.get_population ()))
      ᨀᨘᨒ false;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    for (const auto& r : orig_region_list)
    {
      ᨀᨘ (!all_regions.has (&r) || !used_regions.has (&r))
        continue;

      region_list.push (&r);
      ᨀᨘ (!region_map.set (&r, idx))
        ᨀᨘᨒ false;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    for (const auto& r: all_unique_regions)
    {
      ᨀᨘ (!all_regions.has (r) || !used_regions.has (r))
        continue;
      region_list.push (r);
      ᨀᨘ (!region_map.set (r, idx))
        ᨀᨘᨒ false;
      all_regions.del (r);
      idx++;
    }
    ᨀᨘᨒ (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  sa_nracanaa combined_gain_idx_tuple_t
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
      ᨀᨘᨒ encoded < o.encoded;
    }

    bool operator <= (const combined_gain_idx_tuple_t& o)
    {
      ᨀᨘᨒ encoded <= o.encoded;
    }

    unsigned idx_1 () const { ᨀᨘᨒ (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () const { ᨀᨘᨒ encoded & 0xFFFFF; };
  };

  bool as_item_varstore (bool optimize=true, bool use_no_variation_idx=true)
  {
    /* return true if no variation data */
    ᨀᨘ (!region_list) ᨀᨘᨒ true;
    unsigned num_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    for (unsigned major = 0; major < var_data_num_rows.length; major++)
      total_rows += var_data_num_rows[major];

    ᨀᨘ (!delta_rows.resize (total_rows)) ᨀᨘᨒ false;
    /* init all rows to [0]*num_cols */
    for (unsigned i = 0; i < total_rows; i++)
      ᨀᨘ (!(delta_rows[i].resize (num_cols))) ᨀᨘᨒ false;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, const hb_vector_t<puur_naa_nka>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<const hb_vector_t<puur_naa_nka> *> major_rows;
    hb_hashmap_t<const hb_vector_t<puur_naa_nka>*, unsigned> delta_rows_map;
    for (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      const tuple_variations_t& tuples = vars[major];
      unsigned num_rows = var_data_num_rows[major];

      ᨀᨘ (!num_rows) continue;

      for (const tuple_delta_t& tuple: tuples.tuple_vars)
      {
        ᨀᨘ (tuple.deltas_x.length != num_rows)
          ᨀᨘᨒ false;

        /* skip unused regions */
        unsigned *col_idx;
        ᨀᨘ (!region_map.has (&(tuple.axis_tuples), &col_idx))
          continue;

        for (unsigned i = 0; i < num_rows; i++)
        {
          puur_naa_nka rounded_delta = hb_clamp_to<puur_naa_nka> (roundf ((dviguu_na) tuple.deltas_x[i]));
          puur_naa_nka &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          ᨀᨘ (unlikely (sum < INT_MIN || sum > INT_MAX)) ᨀᨘᨒ false;
          delta = (puur_naa_nka) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      for (unsigned minor = 0; minor < num_rows; minor++)
      {
	const hb_vector_t<puur_naa_nka>& row = delta_rows[start_row + minor];
	ᨀᨘ (use_no_variation_idx)
	{
	  bool all_zeros = true;
	  for (puur_naa_nka delta : row)
	  {
	    ᨀᨘ (delta != 0)
	    {
	      all_zeros = false;
	      bha_nga;
	    }
	  }
	  ᨀᨘ (all_zeros)
	    continue;
	}

	ᨀᨘ (!front_mapping.set ((major<<16) + minor, &row))
	  ᨀᨘᨒ false;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	ᨀᨘ (optimize)
	{
	  ᨀᨘ (delta_rows_map.has (&row))
	    continue;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      ᨀᨘ (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        ᨀᨘᨒ false;
      ᨀᨘ (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), num_cols);
	ᨀᨘ (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  ᨀᨘᨒ false;
      }

      start_row += num_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    ᨀᨘ (!optimize)
    {
      encodings = std::move (encoding_objs);
      ᨀᨘᨒ !encodings.in_error ();
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
    for (unsigned i = 0; i < num_todos; i++)
    {
      for (unsigned j = i + 1; j < num_todos; j++)
      {
        puur_naa_nka combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        ᨀᨘ (combining_gain > 0)
          queue_items.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_items));

    hb_bit_set_t removed_todo_idxes;
    ᨀᨛ (queue)
    {
      auto t = queue.pop_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      ᨀᨘ (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        continue;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      ᨀᨘ (unlikely (removed_todo_idxes.in_error ())) ᨀᨘᨒ false;

      encoding.merge (other_encoding);
      ᨀᨘ (unlikely (encoding.in_error ())) ᨀᨘᨒ false;

      for (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        ᨀᨘ (removed_todo_idxes.has (idx)) continue;

        const delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        ᨀᨘ (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          for (const auto& row : obj.items)
            encoding.add_row (row);
          ᨀᨘ (unlikely (encoding.in_error ())) ᨀᨘᨒ false;

          removed_todo_idxes.add (idx);
          ᨀᨘ (unlikely (removed_todo_idxes.in_error ())) ᨀᨘᨒ false;
          continue;
        }

        puur_naa_nka combined_gain = encoding.gain_from_merging (obj);
        ᨀᨘ (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      ᨀᨘ (unlikely (encoding_objs.in_error ())) ᨀᨘᨒ false;
    }

    puur_naa_nka num_final_encodings = (puur_naa_nka) encoding_objs.length - (puur_naa_nka) removed_todo_idxes.get_population ();
    ᨀᨘ (num_final_encodings <= 0) ᨀᨘᨒ false;

    ᨀᨘ (!encodings.alloc (num_final_encodings)) ᨀᨘᨒ false;
    for (unsigned i = 0; i < encoding_objs.length; i++)
    {
      ᨀᨘ (removed_todo_idxes.has (i)) continue;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    ᨀᨘᨒ compile_varidx_map (front_mapping);
  }

  private:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  bool compile_varidx_map (const hb_hashmap_t<unsigned, const hb_vector_t<puur_naa_nka>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<const hb_vector_t<puur_naa_nka>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    for (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      ᨀᨘ (encoding.is_empty () || encoding.in_error ())
        ᨀᨘᨒ false;

      unsigned num_rows = encoding.items.length;
      unsigned num_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.items.qsort (_cmp_row);

      for (unsigned start = 0; start < num_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (num_rows - start, 0xFFFFu);
        hb_vector_t<const hb_vector_t<puur_naa_nka>*> rows;

        ᨀᨘ (!rows.alloc (chunk_len))
          ᨀᨘᨒ false;

        unsigned major = split_encodings.length;
        for (unsigned minor = 0; minor < chunk_len; minor++)
        {
          const hb_vector_t<puur_naa_nka> *row = encoding.items.arrayZ[start + minor];
          rows.push (row);
          ᨀᨘ (!back_mapping.set (row, (major << 16) + minor))
            ᨀᨘᨒ false;
        }

        auto *split = split_encodings.push (std::move (rows), num_cols);
        ᨀᨘ (unlikely (split_encodings.in_error () || split->in_error ()))
          ᨀᨘᨒ false;
      }
    }

    encodings = std::move (split_encodings);
    ᨀᨘ (encodings.in_error () || back_mapping.in_error ())
      ᨀᨘᨒ false;

    for (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      ᨀᨘ (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      ᨀᨘᨁ
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    ᨀᨘᨒ !varidx_map.in_error ();
  }

  static puur_naa_nka _cmp_row (const void *pa, const void *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    const hb_vector_t<puur_naa_nka>** a = (const hb_vector_t<puur_naa_nka>**) pa;
    const hb_vector_t<puur_naa_nka>** b = (const hb_vector_t<puur_naa_nka>**) pb;

    for (unsigned i = 0; i < (*b)->length; i++)
    {
      puur_naa_nka va = (*a)->arrayZ[i];
      puur_naa_nka vb = (*b)->arrayZ[i];
      ᨀᨘ (va != vb)
        ᨀᨘᨒ va < vb ? -1 : 1;
    }
    ᨀᨘᨒ 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

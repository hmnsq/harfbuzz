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

#அந்தர்பாவ "hb-ot-layout-common.hh"
#அந்தர்பாவ "hb-alloc-pool.hh"
#அந்தர்பாவ "hb-priority-queue.hh"
#அந்தர்பாவ "hb-subset-instancer-iup.hh"


நாமாகாஶ OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
கட்டமைப்பு TupleVariationHeader
{
  friend கட்டமைப்பு tuple_delta_t;
  size_t get_size (unsigned axis_மொத்தம்_times_2) மாறிலி
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    எனில் (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned மொத்தம் = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      திரும்பு hb_unsigned_mul_add_saturate (மொத்தம், axis_மொத்தம்_times_2, min_size);
    }
    திரும்பு min_size;
  }

  unsigned get_data_size () மாறிலி { திரும்பு varDataSize; }

  மாறிலி TupleVariationHeader &get_next (unsigned axis_மொத்தம்_times_2) மாறிலி
  { திரும்பு StructAtOffset<TupleVariationHeader> (இது, get_size (axis_மொத்தம்_times_2)); }

  தருக்க unpack_axis_tuples (unsigned axis_மொத்தம்,
                           மாறிலி hb_array_t<மாறிலி F2DOT14> shared_tuples,
                           மாறிலி hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) மாறிலி
  {
    மாறிலி F2DOT14 *peak_tuple = nullptr;
    எனில் (has_peak ())
      peak_tuple = get_peak_tuple (axis_மொத்தம்);
    இல்லையெனில்
    {
      unsigned முழு index = get_index ();
      எனில் (unlikely ((index + 1) * axis_மொத்தம் > shared_tuples.length))
        திரும்பு பொய்;
      peak_tuple = shared_tuples.sub_array (axis_மொத்தம் * index, axis_மொத்தம்).arrayZ;
    }

    மாறிலி F2DOT14 *start_tuple = nullptr;
    மாறிலி F2DOT14 *end_tuple = nullptr;
    தருக்க has_interm = has_intermediate ();

    எனில் (has_interm)
    {
      start_tuple = get_start_tuple (axis_மொத்தம்);
      end_tuple = get_end_tuple (axis_மொத்தம்);
    }

    க்கு (unsigned i = 0; i < axis_மொத்தம்; i++)
    {
      தசம peak = peak_tuple[i].to_float ();
      எனில் (peak == 0.f) தொடர்;

      hb_tag_t *axis_tag;
      எனில் (!axes_old_index_tag_map->has (i, &axis_tag))
        திரும்பு பொய்;

      தசம start, end;
      எனில் (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      இல்லையெனில்
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    திரும்பு உண்மை;
  }

  HB_ALWAYS_INLINE
  தசம calculate_scalar (hb_array_t<மாறிலி முழு> coords, unsigned முழு coord_மொத்தம்,
			   மாறிலி hb_array_t<மாறிலி F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) மாறிலி
  {
    unsigned tuple_index = tupleIndex;
    unsigned முழு index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    தருக்க plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    எனில் (likely (plain_shared_tuple))
    {
      தசம scalar;
      எனில் (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	திரும்பு scalar;
    }

    மாறிலி F2DOT14 *peak_tuple;

    தருக்க has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    எனில் (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_மொத்தம்);
      shared_tuple_scalar_cache = nullptr;
    }
    இல்லையெனில்
    {
      தசம scalar;
      எனில் (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        எனில் (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	இல்லையெனில்
	  திரும்பு scalar;
      }

      எனில் (unlikely ((index + 1) * coord_மொத்தம் > shared_tuples.length))
        திரும்பு 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_மொத்தம் * index);

    }

    மாறிலி F2DOT14 *start_tuple = nullptr;
    மாறிலி F2DOT14 *end_tuple = nullptr;

    எனில் (has_interm)
    {
      start_tuple = get_start_tuple (coord_மொத்தம்);
      end_tuple = get_end_tuple (coord_மொத்தம்);
    }

    தசம scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#எனில் HB_FAST_இலக்கம்_ACCESS
    தருக்க skip = coord_மொத்தம் >= 16;
#endif
#endif
    க்கு (unsigned முழு i = 0; i < coord_மொத்தம்; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#எனில் HB_FAST_இலக்கம்_ACCESS
      எனில் (skip)
      {
	போது (i + 4 <= coord_மொத்தம் && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	போது (i < coord_மொத்தம் && peak_tuple[i].to_int () == 0)
	  i += 1;
	எனில் (i >= coord_மொத்தம்)
	  நிறுத்து;
      }
#endif
#endif

      முழு peak = peak_tuple[i].to_int ();
      எனில் (!peak) தொடர்;

      முழு v = coords[i];
      எனில் (!v) { scalar = 0.f; நிறுத்து; }
      எனில் (v == peak) தொடர்;

      எனில் (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        முழு start = start_tuple[i].to_int ();
        முழு end = end_tuple[i].to_int ();
        எனில் (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) தொடர்;
        எனில் (v < start || v > end) { scalar = 0.f; நிறுத்து; }
        எனில் (v < peak)
        { எனில் (peak != start) scalar *= (தசம) (v - start) / (peak - start); }
        இல்லையெனில்
        { எனில் (peak != end) scalar *= (தசம) (end - v) / (end - peak); }
      }
      இல்லையெனில் எனில் (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; நிறுத்து; }
      இல்லையெனில்
        scalar *= (தசம) v / peak;
    }
    எனில் (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    திரும்பு scalar;
  }

  தருக்க           has_peak () மாறிலி { திரும்பு tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  தருக்க   has_intermediate () மாறிலி { திரும்பு tupleIndex & TupleIndex::IntermediateRegion; }
  தருக்க has_private_points () மாறிலி { திரும்பு tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () மாறிலி { திரும்பு tupleIndex & TupleIndex::TupleIndexMask; }
  நிலையான constexpr unsigned max_shared_tuple_மொத்தம் = 0x1000u;

  பாதுகாக்கப்பட்ட:
  கட்டமைப்பு TupleIndex : HBUINT16
  {
    எண்ணிக்கை Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); திரும்பு *இது; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<மாறிலி F2DOT14> get_all_tuples (unsigned axis_மொத்தம்) மாறிலி
  { திரும்பு StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_மொத்தம்); }
  மாறிலி F2DOT14* get_all_tuples_base (unsigned axis_மொத்தம்) மாறிலி
  { திரும்பு StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  மாறிலி F2DOT14* get_peak_tuple (unsigned axis_மொத்தம்) மாறிலி
  { திரும்பு get_all_tuples_base (axis_மொத்தம்); }
  மாறிலி F2DOT14* get_start_tuple (unsigned axis_மொத்தம்) மாறிலி
  { திரும்பு get_all_tuples_base (axis_மொத்தம்) + has_peak () * axis_மொத்தம்; }
  மாறிலி F2DOT14* get_end_tuple (unsigned axis_மொத்தம்) மாறிலி
  { திரும்பு get_all_tuples_base (axis_மொத்தம்) + has_peak () * axis_மொத்தம் + axis_மொத்தம்; }

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
  பொது:
  DEFINE_SIZE_MIN (4);
};

கட்டமைப்பு optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<தருக்க> opt_indices;
  hb_vector_t<முழு> rounded_x_deltas;
  hb_vector_t<முழு> rounded_y_deltas;
  hb_vector_t<தசம> opt_deltas_x;
  hb_vector_t<தசம> opt_deltas_y;
  hb_vector_t<unsigned எழுத்து> opt_point_data;
  hb_vector_t<unsigned எழுத்து> opt_deltas_data;
  hb_vector_t<unsigned எழுத்து> point_data;
  hb_vector_t<unsigned எழுத்து> deltas_data;
  hb_vector_t<முழு> rounded_deltas;
};

கட்டமைப்பு tuple_delta_t
{
  நிலையான constexpr தருக்க realloc_move = உண்மை;  // Watch out when adding new members!

  பொது:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<தருக்க> indices;

  hb_vector_t<தசம> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<தசம> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned எழுத்து> compiled_tuple_header;
  hb_vector_t<unsigned எழுத்து> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (மாறிலி tuple_delta_t& o) = இயல்பு;
  tuple_delta_t& operator = (மாறிலி tuple_delta_t& o) = இயல்பு;

  friend வெறுமை swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*இது, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*இது, o);
    திரும்பு *இது;
  }

  வெறுமை copy_from (மாறிலி tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  வெறுமை remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  தருக்க set_tent (hb_tag_t axis_tag, Triple tent)
  { திரும்பு axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (மாறிலி tuple_delta_t& o)
  {
    unsigned இலக்கம் = indices.length;
    க்கு (unsigned i = 0; i < இலக்கம்; i++)
    {
      எனில் (indices.arrayZ[i])
      {
        எனில் (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          எனில் (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      இல்லையெனில்
      {
        எனில் (!o.indices.arrayZ[i]) தொடர்;
        indices.arrayZ[i] = உண்மை;
        deltas_x[i] = o.deltas_x[i];
        எனில் (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    திரும்பு *இது;
  }

  tuple_delta_t& operator *= (தசம scalar)
  {
    எனில் (scalar == 1.0f)
      திரும்பு *இது;

    unsigned இலக்கம் = indices.length;
    எனில் (deltas_y)
      க்கு (unsigned i = 0; i < இலக்கம்; i++)
      {
	எனில் (!indices.arrayZ[i]) தொடர்;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    இல்லையெனில்
      க்கு (unsigned i = 0; i < இலக்கம்; i++)
      {
	எனில் (!indices.arrayZ[i]) தொடர்;
	deltas_x[i] *= scalar;
      }
    திரும்பு *இது;
  }

  வெறுமை change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    எனில் (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*இது));
      திரும்பு;
    }

    எனில் ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->நடு && tent->நடு <= tent->maximum))
      திரும்பு;

    எனில் (tent->நடு == 0.f)
    {
      out.push (std::move (*இது));
      திரும்பு;
    }

    எனில் (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.நடு &&
          axis_limit.நடு <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      திரும்பு;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    க்கு (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      எனில் (i < solutions.length - 1)
	new_var.copy_from (*இது, pool);
      இல்லையெனில்
	new_var = std::move (*இது);

      எனில் (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      இல்லையெனில்
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  தருக்க compile_coords (மாறிலி hb_map_t& axes_index_map,
		       மாறிலி hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_மொத்தம் = axes_index_map.get_population ();
    எனில் (pool)
    {
      எனில் (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_மொத்தம்)))
	திரும்பு பொய்;
    }
    இல்லையெனில் எனில் (unlikely (!compiled_peak_coords.resize (cur_axis_மொத்தம்)))
      திரும்பு பொய்;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_மொத்தம் = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    க்கு (unsigned i = 0; i < orig_axis_மொத்தம்; i++)
    {
      எனில் (!axes_index_map.has (i))
        தொடர்;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      எனில் (axis_tuples.has (axis_tag, &coords))
      {
	தசம min_val = coords->minimum;
	தசம val = coords->நடு;
	தசம max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	எனில் (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  எனில் (!compiled_interm_coords)
	  {
	    எனில் (pool)
	    {
	      எனில் (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_மொத்தம்)))
		திரும்பு பொய்;
	    }
	    இல்லையெனில் எனில் (unlikely (!compiled_interm_coords.resize (2 * cur_axis_மொத்தம்)))
	      திரும்பு பொய்;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_மொத்தம்);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_மொத்தம்);

	    க்கு (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      எனில் (!peak) தொடர்;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	எனில் (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    திரும்பு !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  தருக்க compile_tuple_var_header (மாறிலி hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 மாறிலி hb_map_t& axes_old_index_tag_map,
                                 மாறிலி hb_hashmap_t<மாறிலி hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    எனில் (!compiled_deltas) திரும்பு உண்மை;

    unsigned cur_axis_மொத்தம் = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_மொத்தம் * (F2DOT14::static_size) + 4;
    எனில் (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, பொய்))) திரும்பு பொய்;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    எனில் (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_மொத்தம் = 0;
    unsigned *shared_tuple_idx;
    எனில் (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    இல்லையெனில்
    {
      peak_மொத்தம் = encode_peak_coords(coords, flag);
      எனில் (!peak_மொத்தம்) திரும்பு பொய்;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_மொத்தம் = encode_interm_coords (coords.sub_array (peak_மொத்தம்), flag);

    /* pointdata length = 0 implies "use shared points" */
    எனில் (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_மொத்தம் + interim_மொத்தம்) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    திரும்பு உண்மை;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) மாறிலி
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * அளவு (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    திரும்பு compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) மாறிலி
  {
    எனில் (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * அளவு (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    திரும்பு compiled_interm_coords.length;
  }

  தருக்க compile_deltas (hb_vector_t<முழு> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { திரும்பு compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  நிலையான தருக்க compile_deltas (hb_array_t<மாறிலி தருக்க> point_indices,
			      hb_array_t<மாறிலி தசம> x_deltas,
			      hb_array_t<மாறிலி தசம> y_deltas,
			      hb_vector_t<unsigned எழுத்து> &compiled_deltas, /* OUT */
			      hb_vector_t<முழு> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    எனில் (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      திரும்பு பொய்;

    unsigned j = 0;
    க்கு (unsigned i = 0; i < point_indices.length; i++)
    {
      எனில் (!point_indices[i]) தொடர்;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<முழு> (roundf ((இரட்டை) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    எனில் (!rounded_deltas) திரும்பு உண்மை;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    எனில் (y_deltas)
      alloc_len *= 2;

    எனில் (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, பொய்))) திரும்பு பொய்;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    எனில் (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      க்கு (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        எனில் (!point_indices[idx]) தொடர்;
        முழு rounded_delta = hb_clamp_to<முழு> (roundf ((இரட்டை) y_deltas.arrayZ[idx]));

        எனில் (j >= rounded_deltas.length) திரும்பு பொய்;

        rounded_deltas[j++] = rounded_delta;
      }

      எனில் (j != rounded_deltas.length) திரும்பு பொய்;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    திரும்பு உண்மை;
  }

  நிலையான unsigned compile_deltas (hb_array_t<unsigned எழுத்து> encoded_bytes,
				  hb_array_t<மாறிலி முழு> deltas)
  {
    திரும்பு Tuple_மதிப்புகள்::compile_unsafe (deltas, encoded_bytes);
  }

  தருக்க calc_inferred_deltas (மாறிலி contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_மொத்தம் = orig_points.length;
    எனில் (point_மொத்தம் != indices.length)
      திரும்பு பொய்;

    unsigned ref_மொத்தம்_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    க்கு (unsigned i = 0; i < point_மொத்தம்; i++)
    {
      ref_மொத்தம்_2 += indices.arrayZ[i];
      எனில் (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    எனில் (ref_மொத்தம்_2 == point_மொத்தம்)
      திரும்பு உண்மை;
    எனில் (unlikely (end_points.in_error ())) திரும்பு பொய்;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    க்கு (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_மொத்தம் = 0;
      க்கு (unsigned i = start_point; i < end_point + 1; i++)
        unref_மொத்தம் += indices.arrayZ[i];
      unref_மொத்தம் = (end_point - start_point + 1) - unref_மொத்தம்;

      unsigned j = start_point;
      எனில் (unref_மொத்தம் == 0 || unref_மொத்தம் > end_point - start_point)
        செல் no_more_gaps;
      க்கு (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned முழு prev, next, i;
        க்கு (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          எனில் (indices.arrayZ[i] && !indices.arrayZ[j]) நிறுத்து;
        }
        prev = j = i;
        க்கு (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          எனில் (!indices.arrayZ[i] && indices.arrayZ[j]) நிறுத்து;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        க்கு (;;)
        {
          i = next_index (i, start_point, end_point);
          எனில் (i == next) நிறுத்து;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          எனில் (--unref_மொத்தம் == 0) செல் no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    க்கு (unsigned i = 0; i < point_மொத்தம்; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      எனில் ( !indices[i])
      {
        எனில் (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = உண்மை;
      }
    }
    திரும்பு உண்மை;
  }

  தருக்க optimize (மாறிலி contour_point_vector_t& contour_points,
                 தருக்க is_composite,
		 optimize_scratch_t &scratch,
                 இரட்டை tolerance = 0.5 + 1e-10)
  {
    unsigned மொத்தம் = contour_points.length;
    எனில் (deltas_x.length != மொத்தம் ||
        deltas_y.length != மொத்தம்)
      திரும்பு பொய்;

    hb_vector_t<தருக்க> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<முழு> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<முழு> &rounded_y_deltas = scratch.rounded_y_deltas;

    எனில் (unlikely (!rounded_x_deltas.resize_dirty  (மொத்தம்) ||
                  !rounded_y_deltas.resize_dirty  (மொத்தம்)))
      திரும்பு பொய்;

    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<முழு> (roundf ((இரட்டை) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<முழு> (roundf ((இரட்டை) deltas_y.arrayZ[i]));
    }

    எனில் (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      திரும்பு பொய்;

    unsigned ref_மொத்தம்_2 = 0;
    க்கு (தருக்க ref_flag : opt_indices)
       ref_மொத்தம்_2 += ref_flag;

    எனில் (ref_மொத்தம்_2 == மொத்தம்) திரும்பு உண்மை;

    hb_vector_t<தசம> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<தசம> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    தருக்க is_comp_glyph_wo_deltas = (is_composite && ref_மொத்தம்_2 == 0);
    எனில் (is_comp_glyph_wo_deltas)
    {
      எனில் (unlikely (!opt_deltas_x.resize (மொத்தம்) ||
                    !opt_deltas_y.resize (மொத்தம்)))
        திரும்பு பொய்;

      opt_indices.arrayZ[0] = உண்மை;
      க்கு (unsigned i = 1; i < மொத்தம்; i++)
        opt_indices.arrayZ[i] = பொய்;
    }

    hb_vector_t<unsigned எழுத்து> &opt_point_data = scratch.opt_point_data.reset ();
    எனில் (!compile_point_set (opt_indices, opt_point_data))
      திரும்பு பொய்;
    hb_vector_t<unsigned எழுத்து> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    எனில் (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      திரும்பு பொய்;

    hb_vector_t<unsigned எழுத்து> &point_data = scratch.point_data.reset ();
    எனில் (!compile_point_set (indices, point_data))
      திரும்பு பொய்;
    hb_vector_t<unsigned எழுத்து> &deltas_data = scratch.deltas_data.reset ();
    எனில் (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      திரும்பு பொய்;

    எனில் (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      எனில் (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    திரும்பு !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  நிலையான தருக்க compile_point_set (மாறிலி hb_vector_t<தருக்க> &point_indices,
                                 hb_vector_t<unsigned எழுத்து>& compiled_points /* OUT */)
  {
    unsigned இலக்கம்_points = 0;
    க்கு (தருக்க i : point_indices)
      எனில் (i) இலக்கம்_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    எனில் (!இலக்கம்_points) திரும்பு உண்மை;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    எனில் (இலக்கம்_points == indices_length)
      திரும்பு compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned இலக்கம்_bytes = 2 + 3 *இலக்கம்_points;
    எனில் (unlikely (!compiled_points.resize_dirty  (இலக்கம்_bytes)))
      திரும்பு பொய்;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    எனில் (இலக்கம்_points < 0x80)
      compiled_points.arrayZ[pos++] = இலக்கம்_points;
    இல்லையெனில்
    {
      compiled_points.arrayZ[pos++] = ((இலக்கம்_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = இலக்கம்_points & 0xFF;
    }

    மாறிலி unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_மதிப்பு = 0;
    unsigned இலக்கம்_encoded = 0;
    போது (i < indices_length && இலக்கம்_encoded < இலக்கம்_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      தருக்க use_byte_encoding = பொய்;
      தருக்க new_run = உண்மை;
      போது (i < indices_length && இலக்கம்_encoded < இலக்கம்_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        போது (i < indices_length && !point_indices[i])
          i++;

        எனில் (i >= indices_length) நிறுத்து;

        unsigned cur_மதிப்பு = i;
        unsigned delta = cur_மதிப்பு - last_மதிப்பு;

        எனில் (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = பொய்;
        }

        எனில் (use_byte_encoding && delta > 0xFF)
          நிறுத்து;

        எனில் (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        இல்லையெனில்
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_மதிப்பு = cur_மதிப்பு;
        run_length++;
        இலக்கம்_encoded++;
      }

      எனில் (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      இல்லையெனில்
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    திரும்பு compiled_points.resize_dirty  (pos);
  }

  நிலையான தசம infer_delta (தசம target_val, தசம prev_val, தசம next_val, தசம prev_delta, தசம next_delta)
  {
    எனில் (prev_val == next_val)
      திரும்பு (prev_delta == next_delta) ? prev_delta : 0.f;
    இல்லையெனில் எனில் (target_val <= hb_min (prev_val, next_val))
      திரும்பு (prev_val < next_val) ? prev_delta : next_delta;
    இல்லையெனில் எனில் (target_val >= hb_max (prev_val, next_val))
      திரும்பு (prev_val > next_val) ? prev_delta : next_delta;

    தசம r = (target_val - prev_val) / (next_val - prev_val);
    திரும்பு prev_delta + r * (next_delta - prev_delta);
  }

  நிலையான unsigned முழு next_index (unsigned முழு i, unsigned முழு start, unsigned முழு end)
  { திரும்பு (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
கட்டமைப்பு TupleVariationData
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (இது));
  }

  size_t get_size (unsigned axis_மொத்தம்_times_2) மாறிலி
  {
    size_t total_size = min_size;
    unsigned மொத்தம் = tuple_Var_மொத்தம்.get_மொத்தம் ();
    மாறிலி TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_மொத்தம்_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_மொத்தம்_times_2);
    }

    திரும்பு total_size;
  }

  மாறிலி TupleVariationHeader &get_tuple_var_header (வெறுமை) மாறிலி
  { திரும்பு StructAfter<TupleVariationHeader> (data); }

  கட்டமைப்பு tuple_iterator_t;
  கட்டமைப்பு tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    தனிப்பட்ட:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<மாறிலி hb_vector_t<தருக்க>*, hb_vector_t<unsigned எழுத்து>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<மாறிலி hb_vector_t<தருக்க>*, unsigned> point_set_மொத்தம்_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned எழுத்து> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    தருக்க needs_padding = பொய்;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    தருக்க is_composite = பொய்;

    பொது:
    tuple_variations_t () = இயல்பு;
    tuple_variations_t (மாறிலி tuple_variations_t&) = delete;
    tuple_variations_t& operator=(மாறிலி tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = இயல்பு;
    tuple_variations_t& operator=(tuple_variations_t&&) = இயல்பு;
    ~tuple_variations_t () = இயல்பு;

    explicit operator தருக்க () மாறிலி { திரும்பு தருக்க (tuple_vars); }
    unsigned get_var_மொத்தம் () மாறிலி
    {
      unsigned மொத்தம் = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      க்கு (auto& tuple: tuple_vars)
        எனில் (tuple.compiled_deltas) மொத்தம்++;

      எனில் (shared_points_bytes && shared_points_bytes->length)
        மொத்தம் |= Tuple_Var_மொத்தம்::SharedPointNumbers;
      திரும்பு மொத்தம்;
    }

    unsigned get_compiled_byte_size () மாறிலி
    { திரும்பு compiled_byte_size; }

    தருக்க create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_மொத்தம்,
                                     unsigned point_மொத்தம்,
                                     தருக்க is_gvar,
                                     மாறிலி hb_map_t *axes_old_index_tag_map,
                                     மாறிலி hb_vector_t<unsigned> &shared_indices,
                                     மாறிலி hb_array_t<மாறிலி F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     தருக்க is_composite_glyph = பொய்)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<முழு> deltas_x;
      hb_vector_t<முழு> deltas_y;
      செய்
      {
        மாறிலி HBUINT8 *p = iterator.get_serialized_data ();
        unsigned முழு length = iterator.current_tuple->get_data_size ();
        எனில் (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          திரும்பு பொய்;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        எனில் (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_மொத்தம் (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          திரும்பு பொய்;

	private_indices.reset ();
        தருக்க has_private_points = iterator.current_tuple->has_private_points ();
        மாறிலி HBUINT8 *end = p + length;
        எனில் (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          திரும்பு பொய்;

        மாறிலி hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        தருக்க apply_to_all = (indices.length == 0);
        unsigned இலக்கம்_deltas = apply_to_all ? point_மொத்தம் : indices.length;

        எனில் (unlikely (!deltas_x.resize_dirty  (இலக்கம்_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          திரும்பு பொய்;

        எனில் (is_gvar)
        {
          எனில் (unlikely (!deltas_y.resize_dirty  (இலக்கம்_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            திரும்பு பொய்;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        எனில் (unlikely (!var.indices.allocate_from_pool (pool, point_மொத்தம்) ||
                      !var.deltas_x.allocate_from_pool (pool, point_மொத்தம், பொய்)))
          திரும்பு பொய்;

        எனில் (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_மொத்தம், பொய்)))
          திரும்பு பொய்;

        க்கு (unsigned i = 0; i < இலக்கம்_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          எனில் (idx >= point_மொத்தம்) தொடர்;
          var.indices[idx] = உண்மை;
          var.deltas_x[idx] = deltas_x[i];
          எனில் (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } போது (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      திரும்பு உண்மை;
    }

    தருக்க create_from_item_var_data (மாறிலி VarData &var_data,
                                    மாறிலி hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    மாறிலி hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_மொத்தம்_2,
                                    மாறிலி hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      எனில் (&var_data == &Null (VarData))
        திரும்பு உண்மை;

      unsigned இலக்கம்_regions = var_data.get_region_index_மொத்தம் ();
      எனில் (!tuple_vars.alloc (இலக்கம்_regions)) திரும்பு பொய்;

      item_மொத்தம்_2 = inner_map ? inner_map->get_population () : var_data.get_item_மொத்தம் ();
      எனில் (!item_மொத்தம்_2) திரும்பு உண்மை;
      unsigned row_size = var_data.get_row_size ();
      மாறிலி HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      க்கு (unsigned r = 0; r < இலக்கம்_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        எனில் (!tuple.deltas_x.resize_dirty  (item_மொத்தம்_2) ||
            !tuple.indices.resize_dirty  (item_மொத்தம்_2))
          திரும்பு பொய்;

        க்கு (unsigned i = 0; i < item_மொத்தம்_2; i++)
        {
          tuple.indices.arrayZ[i] = உண்மை;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        எனில் (region_index >= regions.length) திரும்பு பொய்;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      திரும்பு !tuple_vars.in_error ();
    }

    தனிப்பட்ட:
    நிலையான முழு _cmp_axis_tag (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
    {
      மாறிலி hb_tag_t *a = (மாறிலி hb_tag_t*) pa;
      மாறிலி hb_tag_t *b = (மாறிலி hb_tag_t*) pb;
      திரும்பு (முழு)(*a) - (முழு)(*b);
    }

    தருக்க change_tuple_variations_axis_limits (மாறிலி hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              மாறிலி hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      எனில் (!axis_tags.alloc (normalized_axes_location.get_population ()))
        திரும்பு பொய்;
      க்கு (auto t : normalized_axes_location.சாவிகள் ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      க்கு (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        எனில் (!normalized_axes_location.has (axis_tag, &axis_limit))
          திரும்பு பொய்;
        TripleDistances axis_triple_distances{1.f, 1.f};
        எனில் (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        க்கு (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          எனில் (!out) தொடர்;

          unsigned new_len = new_vars.length + out.length;

          எனில் (unlikely (!new_vars.alloc (new_len, பொய்)))
            திரும்பு பொய்;

          க்கு (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      திரும்பு உண்மை;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    தருக்க merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      எனில் (unlikely (!new_vars.alloc (tuple_vars.length)))
	திரும்பு பொய்;
      hb_hashmap_t<மாறிலி hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      க்கு (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        எனில் (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          எனில் (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            திரும்பு பொய்;
          தொடர்;
        }

        unsigned *idx;
        எனில் (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        இல்லையெனில்
        {
	  auto *new_var = new_vars.push ();
	  எனில் (unlikely (new_vars.in_error ()))
	    திரும்பு பொய்;
          hb_swap (*new_var, var);
          எனில் (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            திரும்பு பொய்;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      திரும்பு உண்மை;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    தருக்க compile_all_point_sets ()
    {
      க்கு (மாறிலி auto& tuple: tuple_vars)
      {
        மாறிலி hb_vector_t<தருக்க>* points_set = &(tuple.indices);
        எனில் (point_data_map.has (points_set))
        {
          unsigned *மொத்தம்;
          எனில் (unlikely (!point_set_மொத்தம்_map.has (points_set, &மொத்தம்) ||
                        !point_set_மொத்தம்_map.set (points_set, (*மொத்தம்) + 1)))
            திரும்பு பொய்;
          தொடர்;
        }

        hb_vector_t<unsigned எழுத்து> compiled_point_data;
        எனில் (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          திரும்பு பொய்;

        எனில் (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_மொத்தம்_map.set (points_set, 1))
          திரும்பு பொய்;
      }
      திரும்பு உண்மை;
    }

    /* find shared points set which saves most bytes */
    வெறுமை find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      க்கு (மாறிலி auto& _ : point_data_map.iter_ref ())
      {
        மாறிலி hb_vector_t<தருக்க>* points_set = _.first;
        unsigned data_length = _.second.length;
        எனில் (!data_length) தொடர்;
        unsigned *மொத்தம்;
        எனில் (unlikely (!point_set_மொத்தம்_map.has (points_set, &மொத்தம்) ||
                      *மொத்தம் <= 1))
        {
          shared_points_bytes = nullptr;
          திரும்பு;
        }

        unsigned saved_bytes = data_length * ((*மொத்தம்) -1);
        எனில் (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    தருக்க calc_inferred_deltas (மாறிலி contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      க்கு (tuple_delta_t& var : tuple_vars)
        எனில் (!var.calc_inferred_deltas (contour_points, scratch))
          திரும்பு பொய்;

      திரும்பு உண்மை;
    }

    தருக்க iup_optimize (மாறிலி contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      க்கு (tuple_delta_t& var : tuple_vars)
      {
        எனில் (!var.optimize (contour_points, is_composite, scratch))
          திரும்பு பொய்;
      }
      திரும்பு உண்மை;
    }

    பொது:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    வெறுமை cull_unreachable (மாறிலி hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      க்கு (unsigned i = 0; i < tuple_vars.length; i++)
      {
	எனில் (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  தொடர்;
	எனில் (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    தருக்க instantiate (மாறிலி hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      மாறிலி hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      தருக்க optimize = பொய்)
    {
      எனில் (!tuple_vars) திரும்பு உண்மை;
      எனில் (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        திரும்பு பொய்;
      /* compute inferred deltas only for gvar */
      எனில் (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	எனில் (!calc_inferred_deltas (*contour_points, scratch))
	  திரும்பு பொய்;
      }

      /* if iup delta opt is on, contour_points can't be null */
      எனில் (optimize && !contour_points)
        திரும்பு பொய்;

      எனில் (!merge_tuple_variations (optimize ? contour_points : nullptr))
        திரும்பு பொய்;

      எனில் (optimize && !iup_optimize (*contour_points, scratch)) திரும்பு பொய்;
      திரும்பு !tuple_vars.in_error ();
    }

    தருக்க compile_bytes (மாறிலி hb_map_t& axes_index_map,
                        மாறிலி hb_map_t& axes_old_index_tag_map,
                        தருக்க use_shared_points,
                        தருக்க is_gvar = பொய்,
                        மாறிலி hb_hashmap_t<மாறிலி hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      எனில் (!tuple_vars)
        திரும்பு உண்மை;

      // compile points set and store data in hashmap
      எனில் (!compile_all_point_sets ())
        திரும்பு பொய்;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      எனில் (use_shared_points)
      {
        find_shared_points ();
        எனில் (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<முழு> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      க்கு (auto& tuple: tuple_vars)
      {
        மாறிலி hb_vector_t<தருக்க>* points_set = &(tuple.indices);
        hb_vector_t<unsigned எழுத்து> *points_data;
        எனில் (unlikely (!point_data_map.has (points_set, &points_data)))
          திரும்பு பொய்;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        எனில் (!points_data->length)
          தொடர்;
        எனில் (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          திரும்பு பொய்;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        எனில் (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          திரும்பு பொய்;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      எனில் (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = உண்மை;
        compiled_byte_size += 1;
      }

      திரும்பு உண்மை;
    }

    தருக்க serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) மாறிலி
    {
      TRACE_SERIALIZE (இது);
      க்கு (மாறிலி auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        எனில் (c->in_error ()) return_trace (பொய்);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (உண்மை);
    }

    தருக்க serialize_var_data (hb_serialize_context_t *c, தருக்க is_gvar) மாறிலி
    {
      TRACE_SERIALIZE (இது);
      எனில் (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      க்கு (மாறிலி auto& tuple: tuple_vars)
      {
        மாறிலி hb_vector_t<தருக்க>* points_set = &(tuple.indices);
        hb_vector_t<unsigned எழுத்து> *point_data;
        எனில் (!point_data_map.has (points_set, &point_data))
          return_trace (பொய்);

        எனில் (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        எனில் (c->in_error ()) return_trace (பொய்);
      }

      /* padding for gvar */
      எனில் (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        எனில் (!c->embed (pad)) return_trace (பொய்);
      }
      return_trace (உண்மை);
    }
  };

  கட்டமைப்பு tuple_iterator_t
  {
    unsigned get_axis_மொத்தம் () மாறிலி { திரும்பு axis_மொத்தம்; }

    வெறுமை init (hb_bytes_t var_data_bytes_, unsigned முழு axis_மொத்தம்_3, மாறிலி வெறுமை *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_இடது = var_data->tuple_Var_மொத்தம்.get_மொத்தம் ();
      axis_மொத்தம் = axis_மொத்தம்_3;
      axis_மொத்தம்_times_2 = axis_மொத்தம்_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    தருக்க get_shared_indices (hb_vector_t<unsigned முழு> &shared_indices /* OUT */)
    {
      எனில் (var_data->has_shared_point_numbers ())
      {
        மாறிலி HBUINT8 *base = &(table_base+var_data->data);
        மாறிலி HBUINT8 *p = base;
        எனில் (!decompile_points (p, shared_indices, (மாறிலி HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) திரும்பு பொய்;
        data_offset = p - base;
      }
      திரும்பு உண்மை;
    }

    தருக்க is_valid ()
    {
      எனில் (unlikely (tuples_இடது <= 0))
	திரும்பு பொய்;

      current_tuple_size = TupleVariationHeader::min_size;
      எனில் (unlikely (!var_data_bytes.check_end ((மாறிலி எழுத்து *) current_tuple + current_tuple_size)))
	திரும்பு பொய்;

      current_tuple_size = current_tuple->get_size (axis_மொத்தம்_times_2);
      எனில் (unlikely (!var_data_bytes.check_end ((மாறிலி எழுத்து *) current_tuple + current_tuple_size)))
	திரும்பு பொய்;

      திரும்பு உண்மை;
    }

    HB_ALWAYS_INLINE
    தருக்க move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_இடது--;
      திரும்பு is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    மாறிலி HBUINT8 *get_serialized_data () மாறிலி
    { திரும்பு &(table_base+var_data->data) + data_offset; }

    தனிப்பட்ட:
    signed tuples_இடது;
    மாறிலி TupleVariationData *var_data;
    unsigned முழு axis_மொத்தம்;
    unsigned முழு axis_மொத்தம்_times_2;
    unsigned முழு data_offset;
    unsigned முழு current_tuple_size;
    மாறிலி வெறுமை *table_base;

    பொது:
    hb_bytes_t var_data_bytes;
    மாறிலி TupleVariationHeader *current_tuple;
  };

  நிலையான தருக்க get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_மொத்தம்,
                                  மாறிலி வெறுமை *table_base,
                                  hb_vector_t<unsigned முழு> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_மொத்தம், table_base);
    எனில் (!iterator->get_shared_indices (shared_indices))
      திரும்பு பொய்;
    திரும்பு iterator->is_valid ();
  }

  தருக்க has_shared_point_numbers () மாறிலி { திரும்பு tuple_Var_மொத்தம்.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  தருக்க cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_மொத்தம்,
			      hb_array_t<மாறிலி F2DOT14> shared_tuples,
			      மாறிலி hb_map_t *axes_old_index_tag_map,
			      மாறிலி hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<எழுத்து> &out /* OUT */,
			      தருக்க *changed /* OUT */) மாறிலி
  {
    *changed = பொய்;

    hb_vector_t<unsigned முழு> shared_indices;
    tuple_iterator_t iterator;
    எனில் (!get_tuple_iterator (var_data_bytes, axis_மொத்தம், இது,
			     shared_indices, &iterator))
      திரும்பு உண்மை; /* no tuples or malformed: keep original */

    மாறிலி எழுத்து *bytes_start = var_data_bytes.arrayZ;
    மாறிலி எழுத்து *bytes_end = bytes_start + var_data_bytes.length;
    மாறிலி எழுத்து *serialized_base = (மாறிலி எழுத்து *) &(இது+data);
    மாறிலி எழுத்து *shared_points_end = (மாறிலி எழுத்து *) iterator.get_serialized_data ();
    எனில் (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      திரும்பு உண்மை;

    கட்டமைப்பு kept_tuple_t
    {
      மாறிலி எழுத்து *header;
      unsigned header_size;
      மாறிலி எழுத்து *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    செய்
    {
      மாறிலி TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_மொத்தம் * 2);
      unsigned data_size = header->get_data_size ();
      மாறிலி எழுத்து *tuple_data = (மாறிலி எழுத்து *) iterator.get_serialized_data ();
      எனில் (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	திரும்பு உண்மை; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      எனில் (!header->unpack_axis_tuples (axis_மொத்தம், shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	திரும்பு உண்மை;

      total++;
      எனில் (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(மாறிலி எழுத்து *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } போது (iterator.move_to_next ());

    எனில் (unlikely (kept.in_error ())) திரும்பு பொய்;
    எனில் (kept.length == total) திரும்பு உண்மை; /* nothing to cull */

    *changed = உண்மை;
    எனில் (!kept.length) திரும்பு உண்மை; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    எனில் (unlikely (!out.resize (new_size)))
      திரும்பு பொய்;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_மொத்தம் = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    எழுத்து *p = out.arrayZ + min_size;
    க்கு (மாறிலி auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    க்கு (மாறிலி auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    திரும்பு உண்மை;
  }

  நிலையான தருக்க decompile_points (மாறிலி HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned முழு> &points /* OUT */,
				மாறிலி HBUINT8 *end)
  {
    எண்ணிக்கை packed_point_flag_t
    {
      POINTS_ARE_சொற்கள்     = 0x80,
      POINT_RUN_மொத்தம்_MASK = 0x7F
    };

    எனில் (unlikely (p + 1 > end)) திரும்பு பொய்;

    unsigned மொத்தம் = *p++;
    எனில் (மொத்தம் & POINTS_ARE_சொற்கள்)
    {
      எனில் (unlikely (p + 1 > end)) திரும்பு பொய்;
      மொத்தம் = ((மொத்தம் & POINT_RUN_மொத்தம்_MASK) << 8) | *p++;
    }
    எனில் (unlikely (!points.resize_dirty  (மொத்தம்))) திரும்பு பொய்;

    unsigned n = 0;
    unsigned i = 0;
    போது (i < மொத்தம்)
    {
      எனில் (unlikely (p + 1 > end)) திரும்பு பொய்;
      unsigned control = *p++;
      unsigned run_மொத்தம் = (control & POINT_RUN_மொத்தம்_MASK) + 1;
      unsigned stop = i + run_மொத்தம்;
      எனில் (unlikely (stop > மொத்தம்)) திரும்பு பொய்;
      எனில் (control & POINTS_ARE_சொற்கள்)
      {
        எனில் (unlikely (p + run_மொத்தம் * HBUINT16::static_size > end)) திரும்பு பொய்;
        க்கு (; i < stop; i++)
        {
          n += *(மாறிலி HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      இல்லையெனில்
      {
        எனில் (unlikely (p + run_மொத்தம் > end)) திரும்பு பொய்;
        க்கு (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    திரும்பு உண்மை;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  நிலையான தருக்க decompile_deltas (மாறிலி HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				மாறிலி HBUINT8 *end,
				தருக்க consume_all = பொய்,
				unsigned start = 0)
  {
    திரும்பு Tuple_மதிப்புகள்::decompile (p, deltas, end, consume_all, start);
  }

  தருக்க has_data () மாறிலி { திரும்பு tuple_Var_மொத்தம்; }

  தருக்க decompile_tuple_variations (unsigned point_மொத்தம்,
                                   தருக்க is_gvar,
                                   tuple_iterator_t iterator,
                                   மாறிலி hb_map_t *axes_old_index_tag_map,
                                   மாறிலி hb_vector_t<unsigned> &shared_indices,
                                   மாறிலி hb_array_t<மாறிலி F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   தருக்க is_composite_glyph = பொய்) மாறிலி
  {
    திரும்பு tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_மொத்தம்,
                                                        point_மொத்தம், is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  தருக்க serialize (hb_serialize_context_t *c,
                  தருக்க is_gvar,
                  மாறிலி tuple_variations_t& tuple_variations) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    /* empty tuple variations, just return and skip serialization. */
    எனில் (!tuple_variations) return_trace (உண்மை);

    auto *out = c->start_embed (இது);
    எனில் (unlikely (!c->extend_min (out))) return_trace (பொய்);

    எனில் (!c->check_assign (out->tuple_Var_மொத்தம், tuple_variations.get_var_மொத்தம் (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (பொய்);

    unsigned total_header_len = 0;

    எனில் (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (பொய்);

    unsigned data_offset = min_size + total_header_len;
    எனில் (!is_gvar) data_offset += 4;
    எனில் (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (பொய்);

    திரும்பு tuple_variations.serialize_var_data (c, is_gvar);
  }

  பாதுகாக்கப்பட்ட:
  கட்டமைப்பு Tuple_Var_மொத்தம் : HBUINT16
  {
    friend கட்டமைப்பு tuple_variations_t;
    தருக்க has_shared_point_numbers () மாறிலி { திரும்பு ((*இது) & SharedPointNumbers); }
    unsigned முழு get_மொத்தம் () மாறிலி { திரும்பு (*இது) & மொத்தம்_Mask; }
    Tuple_Var_மொத்தம்& operator = (uint16_t i) { HBUINT16::operator= (i); திரும்பு *இது; }
    explicit operator தருக்க () மாறிலி { திரும்பு get_மொத்தம் (); }

    பாதுகாக்கப்பட்ட:
    எண்ணிக்கை Flags
    {
      SharedPointNumbers= 0x8000u,
      மொத்தம்_Mask         = 0x0FFFu
    };
    பொது:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_மொத்தம் tuple_Var_மொத்தம்;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  பொது:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
கட்டமைப்பு item_variations_t
{
  using region_t = மாறிலி hb_hashmap_t<hb_tag_t, Triple>*;
  தனிப்பட்ட:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_data_இலக்கம்_rows;

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
  hb_vector_t<hb_vector_t<முழு>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  தருக்க has_long = பொய்;

  பொது:
  தருக்க has_long_word () மாறிலி
  { திரும்பு has_long; }

  மாறிலி hb_vector_t<region_t>& get_region_list () மாறிலி
  { திரும்பு region_list; }

  மாறிலி hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () மாறிலி
  { திரும்பு encodings; }

  மாறிலி hb_map_t& get_varidx_map () மாறிலி
  { திரும்பு varidx_map; }

  தருக்க add_vardata_encoding_for_testing (hb_vector_t<மாறிலி hb_vector_t<முழு>*> &&rows,
                                         unsigned இலக்கம்_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), இலக்கம்_cols));
    திரும்பு !encodings.in_error ();
  }

  தருக்க compile_varidx_map_for_testing (மாறிலி hb_hashmap_t<unsigned, மாறிலி hb_vector_t<முழு>*>& front_mapping)
  { திரும்பு compile_varidx_map (front_mapping); }

  தருக்க instantiate (மாறிலி ItemVariationStore& varStore,
                    மாறிலி hb_subset_plan_t *plan,
                    தருக்க optimize=உண்மை,
                    தருக்க use_no_variation_idx=உண்மை,
                    மாறிலி hb_array_t <மாறிலி hb_inc_bimap_t> inner_maps = hb_array_t<மாறிலி hb_inc_bimap_t> (),
                    தருக்க layout_conditions = பொய்)
  {
    எனில் (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      திரும்பு பொய்;
    /* avar2 partial instancing: cull unreachable regions. */
    எனில் (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      க்கு (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    எனில் (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      திரும்பு பொய்;
    எனில் (layout_conditions &&
        (!instantiate_condition_மதிப்புகள் (plan, varStore) || !build_region_list ()))
      திரும்பு பொய்;
    திரும்பு as_item_varstore (optimize, use_no_variation_idx);
  }

  தருக்க instantiate_condition_மதிப்புகள் (மாறிலி hb_subset_plan_t *plan,
                                      மாறிலி ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_மதிப்பு_map;
    condition_map.clear ();
    எனில் (!plan->layout_condition_மதிப்புகள்) திரும்பு உண்மை;
    using condition_மதிப்பு_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, முழு>>;
    hb_vector_t<condition_மதிப்பு_t> conditions;
    க்கு (auto condition : plan->layout_condition_மதிப்புகள்.iter ())
      conditions.push (condition);
    எனில் (unlikely (conditions.in_error ())) திரும்பு பொய்;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (மாறிலி condition_மதிப்பு_t &a, மாறிலி condition_மதிப்பு_t &b) {
      எனில் (a.second.first != b.second.first)
        திரும்பு a.second.first < b.second.first ? -1 : 1;
      திரும்பு a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    க்கு (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      எனில் (plan->gdef_varstore_inner_maps[i].get_population ())
        எனில் (!outer_map.set (i, new_outer++)) திரும்பு பொய்;
    க்கு (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      இரட்டை value = condition.second.second +
                     varStore.get_delta<இரட்டை> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      எனில் (!outer_map.has (old_outer))
        தொடர்;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      எனில் (outer >= vars.length || inner >= var_data_இலக்கம்_rows[outer])
        தொடர்;

      இரட்டை maximum = fabs ((இரட்டை) value);
      தருக்க variable = பொய்;
      க்கு (மாறிலி tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        எனில் (unlikely (inner >= tuple.deltas_x.length)) திரும்பு பொய்;
        தசம delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((இரட்டை) delta));
      }
      எனில் (!variable)
      {
        எனில் (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), முழு (value > 0))))
          திரும்பு பொய்;
        தொடர்;
      }

      இரட்டை scale = 1.;
      போது (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      போது (maximum * scale * 2 <= INT_MAX)
      {
        தருக்க integral = (இரட்டை) value * scale == round ((இரட்டை) value * scale);
        க்கு (மாறிலி tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (இரட்டை) tuple.deltas_x[inner] * scale ==
                      round ((இரட்டை) tuple.deltas_x[inner] * scale);
        எனில் (integral) நிறுத்து;
        scale *= 2;
      }
      முழு default_மதிப்பு = (முழு) round ((இரட்டை) value * scale);
      எனில் (scale != 1. || default_மதிப்பு < -32768 || default_மதிப்பு > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        க்கு (மாறிலி tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          எனில் (!tuple.deltas_x[inner]) தொடர்;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (உண்மை);
          copy.deltas_x.push ((தசம) round ((இரட்டை) tuple.deltas_x[inner] * scale));
          எனில் (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) திரும்பு பொய்;
          private_row.tuple_vars.push (std::move (copy));
        }
        எனில் (unlikely (vars.length >= 0xFFFFu)) திரும்பு பொய்;
        எனில் (unlikely (private_row.tuple_vars.in_error ())) திரும்பு பொய்;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_இலக்கம்_rows.push (1);
        எனில் (unlikely (vars.in_error () || var_data_இலக்கம்_rows.in_error ())) திரும்பு பொய்;
        எனில் (default_மதிப்பு < -32768 || default_மதிப்பு > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          முழு bias = default_மதிப்பு - hb_clamp (default_மதிப்பு, -32768, 32767);
          default_மதிப்பு -= bias;
          முழு உயர்வு = bias / 65536 * 65536;
          முழு parts[] = {உயர்வு, bias - உயர்வு};
          க்கு (முழு part : parts)
          {
            எனில் (!part) தொடர்;
            unsigned மொத்தம் = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            மாறிலி auto &tuples = vars[idx >> 16].tuple_vars;
            எனில் (unlikely (tuples.in_error () || tuples.length != மொத்தம் + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) திரும்பு பொய்;
          }
        }
      }
      எனில் (!condition_map.set (condition.first, hb_pair (idx, default_மதிப்பு)))
        திரும்பு பொய்;
    }
    திரும்பு !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  தருக்க create_from_item_varstore (மாறிலி ItemVariationStore& varStore,
                                  மாறிலி hb_map_t& axes_old_index_tag_map,
                                  மாறிலி hb_array_t <மாறிலி hb_inc_bimap_t> inner_maps = hb_array_t<மாறிலி hb_inc_bimap_t> ())
  {
    மாறிலி VarRegionList& regionList = varStore.get_region_list ();
    எனில் (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      திரும்பு பொய்;

    unsigned இலக்கம்_var_data = varStore.get_sub_table_மொத்தம் ();
    எனில் (inner_maps && inner_maps.length != இலக்கம்_var_data) திரும்பு பொய்;
    எனில் (!vars.alloc (இலக்கம்_var_data) ||
        !var_data_இலக்கம்_rows.alloc (இலக்கம்_var_data)) திரும்பு பொய்;

    க்கு (unsigned i = 0; i < இலக்கம்_var_data; i++)
    {
      எனில் (inner_maps && !inner_maps.arrayZ[i].get_population ())
          தொடர்;
      tuple_variations_t var_data_tuples;
      unsigned item_மொத்தம்_2 = 0;
      எனில் (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_மொத்தம்_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        திரும்பு பொய்;

      var_data_இலக்கம்_rows.push (item_மொத்தம்_2);
      vars.push (std::move (var_data_tuples));
    }
    திரும்பு !vars.in_error () && !var_data_இலக்கம்_rows.in_error () && vars.length == var_data_இலக்கம்_rows.length;
  }

  தருக்க instantiate_tuple_vars (மாறிலி hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               மாறிலி hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               தருக்க build_regions = உண்மை)
  {
    optimize_scratch_t scratch;
    க்கு (tuple_variations_t& tuple_vars : vars)
      எனில் (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        திரும்பு பொய்;

    திரும்பு !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_மொத்தம்_2)
  {
    vars.push (tuple_variations_t ());
    var_data_இலக்கம்_rows.push (item_மொத்தம்_2);
    திரும்பு vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_மொத்தம் (unsigned outer) மாறிலி
  { திரும்பு outer < var_data_இலக்கம்_rows.length ? var_data_இலக்கம்_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    எனில் (unlikely (outer >= vars.length ||
                  inner >= var_data_இலக்கம்_rows[outer] ||
                  var_data_இலக்கம்_rows[outer] >= 0xFFFFu))
      திரும்பு (unsigned) -1;
    க்கு (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      எனில் (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        திரும்பு (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      எனில் (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      எனில் (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        திரும்பு (unsigned) -1;
    }
    திரும்பு var_data_இலக்கம்_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  வெறுமை add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, முழு delta, unsigned item_மொத்தம்_2)
  {
    எனில் (outer >= vars.length) திரும்பு;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    எனில் (!tuple.indices.resize (item_மொத்தம்_2) ||
        !tuple.deltas_x.resize (item_மொத்தம்_2)) திரும்பு;
    க்கு (unsigned i = 0; i < item_மொத்தம்_2; i++)
    {
      tuple.indices.arrayZ[i] = உண்மை;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (தசம) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  தருக்க build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    க்கு (மாறிலி tuple_variations_t& sub_table : vars)
    {
      க்கு (மாறிலி tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        எனில் (!used_regions.has (r))
        {
          தருக்க all_zeros = உண்மை;
          க்கு (தசம d : tuple.deltas_x)
          {
            முழு delta = hb_clamp_to<முழு> (roundf ((இரட்டை) d));
            எனில் (delta != 0)
            {
              all_zeros = பொய்;
              நிறுத்து;
            }
          }
          எனில் (!all_zeros)
          {
            எனில் (!used_regions.set (r, 1))
              திரும்பு பொய்;
          }
        }
        எனில் (all_regions.has (r))
          தொடர்;
        எனில் (!all_regions.set (r, 1))
          திரும்பு பொய்;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    எனில் (!all_regions || !all_unique_regions) திரும்பு உண்மை;

    எனில் (!region_list.alloc (all_regions.get_population ()))
      திரும்பு பொய்;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    க்கு (மாறிலி auto& r : orig_region_list)
    {
      எனில் (!all_regions.has (&r) || !used_regions.has (&r))
        தொடர்;

      region_list.push (&r);
      எனில் (!region_map.set (&r, idx))
        திரும்பு பொய்;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    க்கு (மாறிலி auto& r: all_unique_regions)
    {
      எனில் (!all_regions.has (r) || !used_regions.has (r))
        தொடர்;
      region_list.push (r);
      எனில் (!region_map.set (r, idx))
        திரும்பு பொய்;
      all_regions.del (r);
      idx++;
    }
    திரும்பு (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  கட்டமைப்பு combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = இயல்பு;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      ப்ரதிஜ்ஞா (gain < 0xFFFFFF);
      ப்ரதிஜ்ஞா (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    தருக்க operator < (மாறிலி combined_gain_idx_tuple_t& o)
    {
      திரும்பு encoded < o.encoded;
    }

    தருக்க operator <= (மாறிலி combined_gain_idx_tuple_t& o)
    {
      திரும்பு encoded <= o.encoded;
    }

    unsigned idx_1 () மாறிலி { திரும்பு (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () மாறிலி { திரும்பு encoded & 0xFFFFF; };
  };

  தருக்க as_item_varstore (தருக்க optimize=உண்மை, தருக்க use_no_variation_idx=உண்மை)
  {
    /* return true if no variation data */
    எனில் (!region_list) திரும்பு உண்மை;
    unsigned இலக்கம்_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    க்கு (unsigned major = 0; major < var_data_இலக்கம்_rows.length; major++)
      total_rows += var_data_இலக்கம்_rows[major];

    எனில் (!delta_rows.resize (total_rows)) திரும்பு பொய்;
    /* init all rows to [0]*num_cols */
    க்கு (unsigned i = 0; i < total_rows; i++)
      எனில் (!(delta_rows[i].resize (இலக்கம்_cols))) திரும்பு பொய்;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, மாறிலி hb_vector_t<முழு>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<மாறிலி hb_vector_t<முழு> *> major_rows;
    hb_hashmap_t<மாறிலி hb_vector_t<முழு>*, unsigned> delta_rows_map;
    க்கு (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      மாறிலி tuple_variations_t& tuples = vars[major];
      unsigned இலக்கம்_rows = var_data_இலக்கம்_rows[major];

      எனில் (!இலக்கம்_rows) தொடர்;

      க்கு (மாறிலி tuple_delta_t& tuple: tuples.tuple_vars)
      {
        எனில் (tuple.deltas_x.length != இலக்கம்_rows)
          திரும்பு பொய்;

        /* skip unused regions */
        unsigned *col_idx;
        எனில் (!region_map.has (&(tuple.axis_tuples), &col_idx))
          தொடர்;

        க்கு (unsigned i = 0; i < இலக்கம்_rows; i++)
        {
          முழு rounded_delta = hb_clamp_to<முழு> (roundf ((இரட்டை) tuple.deltas_x[i]));
          முழு &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          எனில் (unlikely (sum < INT_MIN || sum > INT_MAX)) திரும்பு பொய்;
          delta = (முழு) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      க்கு (unsigned minor = 0; minor < இலக்கம்_rows; minor++)
      {
	மாறிலி hb_vector_t<முழு>& row = delta_rows[start_row + minor];
	எனில் (use_no_variation_idx)
	{
	  தருக்க all_zeros = உண்மை;
	  க்கு (முழு delta : row)
	  {
	    எனில் (delta != 0)
	    {
	      all_zeros = பொய்;
	      நிறுத்து;
	    }
	  }
	  எனில் (all_zeros)
	    தொடர்;
	}

	எனில் (!front_mapping.set ((major<<16) + minor, &row))
	  திரும்பு பொய்;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	எனில் (optimize)
	{
	  எனில் (delta_rows_map.has (&row))
	    தொடர்;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      எனில் (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        திரும்பு பொய்;
      எனில் (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), இலக்கம்_cols);
	எனில் (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  திரும்பு பொய்;
      }

      start_row += இலக்கம்_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    எனில் (!optimize)
    {
      encodings = std::move (encoding_objs);
      திரும்பு !encodings.in_error ();
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
    hb_vector_t<item_t> queue_பொருட்கள்;
    unsigned இலக்கம்_todos = encoding_objs.length;
    க்கு (unsigned i = 0; i < இலக்கம்_todos; i++)
    {
      க்கு (unsigned j = i + 1; j < இலக்கம்_todos; j++)
      {
        முழு combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        எனில் (combining_gain > 0)
          queue_பொருட்கள்.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_பொருட்கள்));

    hb_bit_set_t removed_todo_idxes;
    போது (queue)
    {
      auto t = queue.எடு_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      எனில் (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        தொடர்;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      எனில் (unlikely (removed_todo_idxes.in_error ())) திரும்பு பொய்;

      encoding.merge (other_encoding);
      எனில் (unlikely (encoding.in_error ())) திரும்பு பொய்;

      க்கு (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        எனில் (removed_todo_idxes.has (idx)) தொடர்;

        மாறிலி delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        எனில் (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          க்கு (மாறிலி auto& row : obj.பொருட்கள்)
            encoding.add_row (row);
          எனில் (unlikely (encoding.in_error ())) திரும்பு பொய்;

          removed_todo_idxes.add (idx);
          எனில் (unlikely (removed_todo_idxes.in_error ())) திரும்பு பொய்;
          தொடர்;
        }

        முழு combined_gain = encoding.gain_from_merging (obj);
        எனில் (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      எனில் (unlikely (encoding_objs.in_error ())) திரும்பு பொய்;
    }

    முழு இலக்கம்_final_encodings = (முழு) encoding_objs.length - (முழு) removed_todo_idxes.get_population ();
    எனில் (இலக்கம்_final_encodings <= 0) திரும்பு பொய்;

    எனில் (!encodings.alloc (இலக்கம்_final_encodings)) திரும்பு பொய்;
    க்கு (unsigned i = 0; i < encoding_objs.length; i++)
    {
      எனில் (removed_todo_idxes.has (i)) தொடர்;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    திரும்பு compile_varidx_map (front_mapping);
  }

  தனிப்பட்ட:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  தருக்க compile_varidx_map (மாறிலி hb_hashmap_t<unsigned, மாறிலி hb_vector_t<முழு>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<மாறிலி hb_vector_t<முழு>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    க்கு (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      எனில் (encoding.is_empty () || encoding.in_error ())
        திரும்பு பொய்;

      unsigned இலக்கம்_rows = encoding.பொருட்கள்.length;
      unsigned இலக்கம்_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.பொருட்கள்.qsort (_cmp_row);

      க்கு (unsigned start = 0; start < இலக்கம்_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (இலக்கம்_rows - start, 0xFFFFu);
        hb_vector_t<மாறிலி hb_vector_t<முழு>*> rows;

        எனில் (!rows.alloc (chunk_len))
          திரும்பு பொய்;

        unsigned major = split_encodings.length;
        க்கு (unsigned minor = 0; minor < chunk_len; minor++)
        {
          மாறிலி hb_vector_t<முழு> *row = encoding.பொருட்கள்.arrayZ[start + minor];
          rows.push (row);
          எனில் (!back_mapping.set (row, (major << 16) + minor))
            திரும்பு பொய்;
        }

        auto *split = split_encodings.push (std::move (rows), இலக்கம்_cols);
        எனில் (unlikely (split_encodings.in_error () || split->in_error ()))
          திரும்பு பொய்;
      }
    }

    encodings = std::move (split_encodings);
    எனில் (encodings.in_error () || back_mapping.in_error ())
      திரும்பு பொய்;

    க்கு (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      எனில் (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      இல்லையெனில்
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    திரும்பு !varidx_map.in_error ();
  }

  நிலையான முழு _cmp_row (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    மாறிலி hb_vector_t<முழு>** a = (மாறிலி hb_vector_t<முழு>**) pa;
    மாறிலி hb_vector_t<முழு>** b = (மாறிலி hb_vector_t<முழு>**) pb;

    க்கு (unsigned i = 0; i < (*b)->length; i++)
    {
      முழு va = (*a)->arrayZ[i];
      முழு vb = (*b)->arrayZ[i];
      எனில் (va != vb)
        திரும்பு va < vb ? -1 : 1;
    }
    திரும்பு 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

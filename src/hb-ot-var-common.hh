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

#شامل "hb-ot-layout-common.hh"
#شامل "hb-alloc-pool.hh"
#شامل "hb-priority-queue.hh"
#شامل "hb-subset-instancer-iup.hh"


فضاینام OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
ساختار TupleVariationHeader
{
  friend ساختار tuple_delta_t;
  size_t get_size (unsigned axis_شمار_times_2) ثابت
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    اگر (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned شمار = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      بازگشت hb_unsigned_mul_add_saturate (شمار, axis_شمار_times_2, min_size);
    }
    بازگشت min_size;
  }

  unsigned get_data_size () ثابت { بازگشت varDataSize; }

  ثابت TupleVariationHeader &get_next (unsigned axis_شمار_times_2) ثابت
  { بازگشت StructAtOffset<TupleVariationHeader> (این, get_size (axis_شمار_times_2)); }

  منطقی unpack_axis_tuples (unsigned axis_شمار,
                           ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                           ثابت hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ثابت
  {
    ثابت F2DOT14 *peak_tuple = nullptr;
    اگر (has_peak ())
      peak_tuple = get_peak_tuple (axis_شمار);
    وگرنه
    {
      unsigned صحیح index = get_index ();
      اگر (unlikely ((index + 1) * axis_شمار > shared_tuples.length))
        بازگشت نادرست;
      peak_tuple = shared_tuples.sub_array (axis_شمار * index, axis_شمار).arrayZ;
    }

    ثابت F2DOT14 *start_tuple = nullptr;
    ثابت F2DOT14 *end_tuple = nullptr;
    منطقی has_interm = has_intermediate ();

    اگر (has_interm)
    {
      start_tuple = get_start_tuple (axis_شمار);
      end_tuple = get_end_tuple (axis_شمار);
    }

    برای (unsigned i = 0; i < axis_شمار; i++)
    {
      اعشاری peak = peak_tuple[i].to_float ();
      اگر (peak == 0.f) ادامه;

      hb_tag_t *axis_tag;
      اگر (!axes_old_index_tag_map->has (i, &axis_tag))
        بازگشت نادرست;

      اعشاری start, end;
      اگر (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      وگرنه
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    بازگشت درست;
  }

  HB_ALWAYS_INLINE
  اعشاری calculate_scalar (hb_array_t<ثابت صحیح> coords, unsigned صحیح coord_شمار,
			   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ثابت
  {
    unsigned tuple_index = tupleIndex;
    unsigned صحیح index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    منطقی plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    اگر (likely (plain_shared_tuple))
    {
      اعشاری scalar;
      اگر (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	بازگشت scalar;
    }

    ثابت F2DOT14 *peak_tuple;

    منطقی has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    اگر (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_شمار);
      shared_tuple_scalar_cache = nullptr;
    }
    وگرنه
    {
      اعشاری scalar;
      اگر (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        اگر (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	وگرنه
	  بازگشت scalar;
      }

      اگر (unlikely ((index + 1) * coord_شمار > shared_tuples.length))
        بازگشت 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_شمار * index);

    }

    ثابت F2DOT14 *start_tuple = nullptr;
    ثابت F2DOT14 *end_tuple = nullptr;

    اگر (has_interm)
    {
      start_tuple = get_start_tuple (coord_شمار);
      end_tuple = get_end_tuple (coord_شمار);
    }

    اعشاری scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#اگر HB_FAST_رقم_ACCESS
    منطقی skip = coord_شمار >= 16;
#endif
#endif
    برای (unsigned صحیح i = 0; i < coord_شمار; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#اگر HB_FAST_رقم_ACCESS
      اگر (skip)
      {
	وقتی (i + 4 <= coord_شمار && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	وقتی (i < coord_شمار && peak_tuple[i].to_int () == 0)
	  i += 1;
	اگر (i >= coord_شمار)
	  بشکن;
      }
#endif
#endif

      صحیح peak = peak_tuple[i].to_int ();
      اگر (!peak) ادامه;

      صحیح v = coords[i];
      اگر (!v) { scalar = 0.f; بشکن; }
      اگر (v == peak) ادامه;

      اگر (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        صحیح start = start_tuple[i].to_int ();
        صحیح end = end_tuple[i].to_int ();
        اگر (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) ادامه;
        اگر (v < start || v > end) { scalar = 0.f; بشکن; }
        اگر (v < peak)
        { اگر (peak != start) scalar *= (اعشاری) (v - start) / (peak - start); }
        وگرنه
        { اگر (peak != end) scalar *= (اعشاری) (end - v) / (end - peak); }
      }
      وگرنه اگر (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; بشکن; }
      وگرنه
        scalar *= (اعشاری) v / peak;
    }
    اگر (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    بازگشت scalar;
  }

  منطقی           has_peak () ثابت { بازگشت tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  منطقی   has_intermediate () ثابت { بازگشت tupleIndex & TupleIndex::IntermediateRegion; }
  منطقی has_private_points () ثابت { بازگشت tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () ثابت { بازگشت tupleIndex & TupleIndex::TupleIndexMask; }
  ایستا constexpr unsigned max_shared_tuple_شمار = 0x1000u;

  محافظتشده:
  ساختار TupleIndex : HBUINT16
  {
    شمارش Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); بازگشت *این; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<ثابت F2DOT14> get_all_tuples (unsigned axis_شمار) ثابت
  { بازگشت StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_شمار); }
  ثابت F2DOT14* get_all_tuples_base (unsigned axis_شمار) ثابت
  { بازگشت StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  ثابت F2DOT14* get_peak_tuple (unsigned axis_شمار) ثابت
  { بازگشت get_all_tuples_base (axis_شمار); }
  ثابت F2DOT14* get_start_tuple (unsigned axis_شمار) ثابت
  { بازگشت get_all_tuples_base (axis_شمار) + has_peak () * axis_شمار; }
  ثابت F2DOT14* get_end_tuple (unsigned axis_شمار) ثابت
  { بازگشت get_all_tuples_base (axis_شمار) + has_peak () * axis_شمار + axis_شمار; }

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
  عمومی:
  DEFINE_SIZE_MIN (4);
};

ساختار optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<منطقی> opt_indices;
  hb_vector_t<صحیح> rounded_x_deltas;
  hb_vector_t<صحیح> rounded_y_deltas;
  hb_vector_t<اعشاری> opt_deltas_x;
  hb_vector_t<اعشاری> opt_deltas_y;
  hb_vector_t<unsigned نویسه> opt_point_data;
  hb_vector_t<unsigned نویسه> opt_deltas_data;
  hb_vector_t<unsigned نویسه> point_data;
  hb_vector_t<unsigned نویسه> deltas_data;
  hb_vector_t<صحیح> rounded_deltas;
};

ساختار tuple_delta_t
{
  ایستا constexpr منطقی realloc_move = درست;  // Watch out when adding new members!

  عمومی:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<منطقی> indices;

  hb_vector_t<اعشاری> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<اعشاری> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned نویسه> compiled_tuple_header;
  hb_vector_t<unsigned نویسه> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (ثابت tuple_delta_t& o) = پیشفرض;
  tuple_delta_t& operator = (ثابت tuple_delta_t& o) = پیشفرض;

  friend تهی swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*این, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*این, o);
    بازگشت *این;
  }

  تهی copy_from (ثابت tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  تهی remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  منطقی set_tent (hb_tag_t axis_tag, Triple tent)
  { بازگشت axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (ثابت tuple_delta_t& o)
  {
    unsigned رقم = indices.length;
    برای (unsigned i = 0; i < رقم; i++)
    {
      اگر (indices.arrayZ[i])
      {
        اگر (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          اگر (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      وگرنه
      {
        اگر (!o.indices.arrayZ[i]) ادامه;
        indices.arrayZ[i] = درست;
        deltas_x[i] = o.deltas_x[i];
        اگر (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    بازگشت *این;
  }

  tuple_delta_t& operator *= (اعشاری scalar)
  {
    اگر (scalar == 1.0f)
      بازگشت *این;

    unsigned رقم = indices.length;
    اگر (deltas_y)
      برای (unsigned i = 0; i < رقم; i++)
      {
	اگر (!indices.arrayZ[i]) ادامه;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    وگرنه
      برای (unsigned i = 0; i < رقم; i++)
      {
	اگر (!indices.arrayZ[i]) ادامه;
	deltas_x[i] *= scalar;
      }
    بازگشت *این;
  }

  تهی change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    اگر (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*این));
      بازگشت;
    }

    اگر ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->میانه && tent->میانه <= tent->maximum))
      بازگشت;

    اگر (tent->میانه == 0.f)
    {
      out.push (std::move (*این));
      بازگشت;
    }

    اگر (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.میانه &&
          axis_limit.میانه <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      بازگشت;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    برای (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      اگر (i < solutions.length - 1)
	new_var.copy_from (*این, pool);
      وگرنه
	new_var = std::move (*این);

      اگر (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      وگرنه
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  منطقی compile_coords (ثابت hb_map_t& axes_index_map,
		       ثابت hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_شمار = axes_index_map.get_population ();
    اگر (pool)
    {
      اگر (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_شمار)))
	بازگشت نادرست;
    }
    وگرنه اگر (unlikely (!compiled_peak_coords.resize (cur_axis_شمار)))
      بازگشت نادرست;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_شمار = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    برای (unsigned i = 0; i < orig_axis_شمار; i++)
    {
      اگر (!axes_index_map.has (i))
        ادامه;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      اگر (axis_tuples.has (axis_tag, &coords))
      {
	اعشاری min_val = coords->minimum;
	اعشاری val = coords->میانه;
	اعشاری max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	اگر (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  اگر (!compiled_interm_coords)
	  {
	    اگر (pool)
	    {
	      اگر (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_شمار)))
		بازگشت نادرست;
	    }
	    وگرنه اگر (unlikely (!compiled_interm_coords.resize (2 * cur_axis_شمار)))
	      بازگشت نادرست;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_شمار);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_شمار);

	    برای (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      اگر (!peak) ادامه;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	اگر (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    بازگشت !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  منطقی compile_tuple_var_header (ثابت hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 ثابت hb_map_t& axes_old_index_tag_map,
                                 ثابت hb_hashmap_t<ثابت hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    اگر (!compiled_deltas) بازگشت درست;

    unsigned cur_axis_شمار = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_شمار * (F2DOT14::static_size) + 4;
    اگر (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, نادرست))) بازگشت نادرست;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    اگر (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_شمار = 0;
    unsigned *shared_tuple_idx;
    اگر (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    وگرنه
    {
      peak_شمار = encode_peak_coords(coords, flag);
      اگر (!peak_شمار) بازگشت نادرست;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_شمار = encode_interm_coords (coords.sub_array (peak_شمار), flag);

    /* pointdata length = 0 implies "use shared points" */
    اگر (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_شمار + interim_شمار) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    بازگشت درست;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) ثابت
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * اندازه (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    بازگشت compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) ثابت
  {
    اگر (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * اندازه (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    بازگشت compiled_interm_coords.length;
  }

  منطقی compile_deltas (hb_vector_t<صحیح> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { بازگشت compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ایستا منطقی compile_deltas (hb_array_t<ثابت منطقی> point_indices,
			      hb_array_t<ثابت اعشاری> x_deltas,
			      hb_array_t<ثابت اعشاری> y_deltas,
			      hb_vector_t<unsigned نویسه> &compiled_deltas, /* OUT */
			      hb_vector_t<صحیح> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    اگر (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      بازگشت نادرست;

    unsigned j = 0;
    برای (unsigned i = 0; i < point_indices.length; i++)
    {
      اگر (!point_indices[i]) ادامه;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<صحیح> (roundf ((دوگانه) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    اگر (!rounded_deltas) بازگشت درست;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    اگر (y_deltas)
      alloc_len *= 2;

    اگر (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, نادرست))) بازگشت نادرست;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    اگر (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      برای (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        اگر (!point_indices[idx]) ادامه;
        صحیح rounded_delta = hb_clamp_to<صحیح> (roundf ((دوگانه) y_deltas.arrayZ[idx]));

        اگر (j >= rounded_deltas.length) بازگشت نادرست;

        rounded_deltas[j++] = rounded_delta;
      }

      اگر (j != rounded_deltas.length) بازگشت نادرست;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    بازگشت درست;
  }

  ایستا unsigned compile_deltas (hb_array_t<unsigned نویسه> encoded_bytes,
				  hb_array_t<ثابت صحیح> deltas)
  {
    بازگشت Tuple_مقادیر::compile_unsafe (deltas, encoded_bytes);
  }

  منطقی calc_inferred_deltas (ثابت contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_شمار = orig_points.length;
    اگر (point_شمار != indices.length)
      بازگشت نادرست;

    unsigned ref_شمار_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    برای (unsigned i = 0; i < point_شمار; i++)
    {
      ref_شمار_2 += indices.arrayZ[i];
      اگر (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    اگر (ref_شمار_2 == point_شمار)
      بازگشت درست;
    اگر (unlikely (end_points.in_error ())) بازگشت نادرست;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    برای (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_شمار = 0;
      برای (unsigned i = start_point; i < end_point + 1; i++)
        unref_شمار += indices.arrayZ[i];
      unref_شمار = (end_point - start_point + 1) - unref_شمار;

      unsigned j = start_point;
      اگر (unref_شمار == 0 || unref_شمار > end_point - start_point)
        برو no_more_gaps;
      برای (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned صحیح prev, next, i;
        برای (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          اگر (indices.arrayZ[i] && !indices.arrayZ[j]) بشکن;
        }
        prev = j = i;
        برای (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          اگر (!indices.arrayZ[i] && indices.arrayZ[j]) بشکن;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        برای (;;)
        {
          i = next_index (i, start_point, end_point);
          اگر (i == next) بشکن;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          اگر (--unref_شمار == 0) برو no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    برای (unsigned i = 0; i < point_شمار; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      اگر ( !indices[i])
      {
        اگر (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = درست;
      }
    }
    بازگشت درست;
  }

  منطقی optimize (ثابت contour_point_vector_t& contour_points,
                 منطقی is_composite,
		 optimize_scratch_t &scratch,
                 دوگانه tolerance = 0.5 + 1e-10)
  {
    unsigned شمار = contour_points.length;
    اگر (deltas_x.length != شمار ||
        deltas_y.length != شمار)
      بازگشت نادرست;

    hb_vector_t<منطقی> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<صحیح> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<صحیح> &rounded_y_deltas = scratch.rounded_y_deltas;

    اگر (unlikely (!rounded_x_deltas.resize_dirty  (شمار) ||
                  !rounded_y_deltas.resize_dirty  (شمار)))
      بازگشت نادرست;

    برای (unsigned i = 0; i < شمار; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<صحیح> (roundf ((دوگانه) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<صحیح> (roundf ((دوگانه) deltas_y.arrayZ[i]));
    }

    اگر (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      بازگشت نادرست;

    unsigned ref_شمار_2 = 0;
    برای (منطقی ref_flag : opt_indices)
       ref_شمار_2 += ref_flag;

    اگر (ref_شمار_2 == شمار) بازگشت درست;

    hb_vector_t<اعشاری> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<اعشاری> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    منطقی is_comp_glyph_wo_deltas = (is_composite && ref_شمار_2 == 0);
    اگر (is_comp_glyph_wo_deltas)
    {
      اگر (unlikely (!opt_deltas_x.resize (شمار) ||
                    !opt_deltas_y.resize (شمار)))
        بازگشت نادرست;

      opt_indices.arrayZ[0] = درست;
      برای (unsigned i = 1; i < شمار; i++)
        opt_indices.arrayZ[i] = نادرست;
    }

    hb_vector_t<unsigned نویسه> &opt_point_data = scratch.opt_point_data.reset ();
    اگر (!compile_point_set (opt_indices, opt_point_data))
      بازگشت نادرست;
    hb_vector_t<unsigned نویسه> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    اگر (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      بازگشت نادرست;

    hb_vector_t<unsigned نویسه> &point_data = scratch.point_data.reset ();
    اگر (!compile_point_set (indices, point_data))
      بازگشت نادرست;
    hb_vector_t<unsigned نویسه> &deltas_data = scratch.deltas_data.reset ();
    اگر (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      بازگشت نادرست;

    اگر (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      اگر (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    بازگشت !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  ایستا منطقی compile_point_set (ثابت hb_vector_t<منطقی> &point_indices,
                                 hb_vector_t<unsigned نویسه>& compiled_points /* OUT */)
  {
    unsigned رقم_points = 0;
    برای (منطقی i : point_indices)
      اگر (i) رقم_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    اگر (!رقم_points) بازگشت درست;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    اگر (رقم_points == indices_length)
      بازگشت compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned رقم_bytes = 2 + 3 *رقم_points;
    اگر (unlikely (!compiled_points.resize_dirty  (رقم_bytes)))
      بازگشت نادرست;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    اگر (رقم_points < 0x80)
      compiled_points.arrayZ[pos++] = رقم_points;
    وگرنه
    {
      compiled_points.arrayZ[pos++] = ((رقم_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = رقم_points & 0xFF;
    }

    ثابت unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_مقدار = 0;
    unsigned رقم_encoded = 0;
    وقتی (i < indices_length && رقم_encoded < رقم_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      منطقی use_byte_encoding = نادرست;
      منطقی new_run = درست;
      وقتی (i < indices_length && رقم_encoded < رقم_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        وقتی (i < indices_length && !point_indices[i])
          i++;

        اگر (i >= indices_length) بشکن;

        unsigned cur_مقدار = i;
        unsigned delta = cur_مقدار - last_مقدار;

        اگر (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = نادرست;
        }

        اگر (use_byte_encoding && delta > 0xFF)
          بشکن;

        اگر (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        وگرنه
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_مقدار = cur_مقدار;
        run_length++;
        رقم_encoded++;
      }

      اگر (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      وگرنه
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    بازگشت compiled_points.resize_dirty  (pos);
  }

  ایستا اعشاری infer_delta (اعشاری target_val, اعشاری prev_val, اعشاری next_val, اعشاری prev_delta, اعشاری next_delta)
  {
    اگر (prev_val == next_val)
      بازگشت (prev_delta == next_delta) ? prev_delta : 0.f;
    وگرنه اگر (target_val <= hb_min (prev_val, next_val))
      بازگشت (prev_val < next_val) ? prev_delta : next_delta;
    وگرنه اگر (target_val >= hb_max (prev_val, next_val))
      بازگشت (prev_val > next_val) ? prev_delta : next_delta;

    اعشاری r = (target_val - prev_val) / (next_val - prev_val);
    بازگشت prev_delta + r * (next_delta - prev_delta);
  }

  ایستا unsigned صحیح next_index (unsigned صحیح i, unsigned صحیح start, unsigned صحیح end)
  { بازگشت (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
ساختار TupleVariationData
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (این));
  }

  size_t get_size (unsigned axis_شمار_times_2) ثابت
  {
    size_t total_size = min_size;
    unsigned شمار = tuple_Var_شمار.get_شمار ();
    ثابت TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    برای (unsigned i = 0; i < شمار; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_شمار_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_شمار_times_2);
    }

    بازگشت total_size;
  }

  ثابت TupleVariationHeader &get_tuple_var_header (تهی) ثابت
  { بازگشت StructAfter<TupleVariationHeader> (data); }

  ساختار tuple_iterator_t;
  ساختار tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    خصوصی:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ثابت hb_vector_t<منطقی>*, hb_vector_t<unsigned نویسه>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ثابت hb_vector_t<منطقی>*, unsigned> point_set_شمار_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned نویسه> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    منطقی needs_padding = نادرست;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    منطقی is_composite = نادرست;

    عمومی:
    tuple_variations_t () = پیشفرض;
    tuple_variations_t (ثابت tuple_variations_t&) = delete;
    tuple_variations_t& operator=(ثابت tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = پیشفرض;
    tuple_variations_t& operator=(tuple_variations_t&&) = پیشفرض;
    ~tuple_variations_t () = پیشفرض;

    explicit operator منطقی () ثابت { بازگشت منطقی (tuple_vars); }
    unsigned get_var_شمار () ثابت
    {
      unsigned شمار = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      برای (auto& tuple: tuple_vars)
        اگر (tuple.compiled_deltas) شمار++;

      اگر (shared_points_bytes && shared_points_bytes->length)
        شمار |= Tuple_Var_شمار::SharedPointNumbers;
      بازگشت شمار;
    }

    unsigned get_compiled_byte_size () ثابت
    { بازگشت compiled_byte_size; }

    منطقی create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_شمار,
                                     unsigned point_شمار,
                                     منطقی is_gvar,
                                     ثابت hb_map_t *axes_old_index_tag_map,
                                     ثابت hb_vector_t<unsigned> &shared_indices,
                                     ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     منطقی is_composite_glyph = نادرست)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<صحیح> deltas_x;
      hb_vector_t<صحیح> deltas_y;
      انجامبده
      {
        ثابت HBUINT8 *p = iterator.get_serialized_data ();
        unsigned صحیح length = iterator.current_tuple->get_data_size ();
        اگر (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          بازگشت نادرست;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        اگر (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_شمار (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          بازگشت نادرست;

	private_indices.reset ();
        منطقی has_private_points = iterator.current_tuple->has_private_points ();
        ثابت HBUINT8 *end = p + length;
        اگر (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          بازگشت نادرست;

        ثابت hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        منطقی apply_to_all = (indices.length == 0);
        unsigned رقم_deltas = apply_to_all ? point_شمار : indices.length;

        اگر (unlikely (!deltas_x.resize_dirty  (رقم_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          بازگشت نادرست;

        اگر (is_gvar)
        {
          اگر (unlikely (!deltas_y.resize_dirty  (رقم_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            بازگشت نادرست;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        اگر (unlikely (!var.indices.allocate_from_pool (pool, point_شمار) ||
                      !var.deltas_x.allocate_from_pool (pool, point_شمار, نادرست)))
          بازگشت نادرست;

        اگر (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_شمار, نادرست)))
          بازگشت نادرست;

        برای (unsigned i = 0; i < رقم_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          اگر (idx >= point_شمار) ادامه;
          var.indices[idx] = درست;
          var.deltas_x[idx] = deltas_x[i];
          اگر (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } وقتی (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      بازگشت درست;
    }

    منطقی create_from_item_var_data (ثابت VarData &var_data,
                                    ثابت hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    ثابت hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_شمار_2,
                                    ثابت hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      اگر (&var_data == &Null (VarData))
        بازگشت درست;

      unsigned رقم_regions = var_data.get_region_index_شمار ();
      اگر (!tuple_vars.alloc (رقم_regions)) بازگشت نادرست;

      item_شمار_2 = inner_map ? inner_map->get_population () : var_data.get_item_شمار ();
      اگر (!item_شمار_2) بازگشت درست;
      unsigned row_size = var_data.get_row_size ();
      ثابت HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      برای (unsigned r = 0; r < رقم_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        اگر (!tuple.deltas_x.resize_dirty  (item_شمار_2) ||
            !tuple.indices.resize_dirty  (item_شمار_2))
          بازگشت نادرست;

        برای (unsigned i = 0; i < item_شمار_2; i++)
        {
          tuple.indices.arrayZ[i] = درست;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        اگر (region_index >= regions.length) بازگشت نادرست;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      بازگشت !tuple_vars.in_error ();
    }

    خصوصی:
    ایستا صحیح _cmp_axis_tag (ثابت تهی *pa, ثابت تهی *pb)
    {
      ثابت hb_tag_t *a = (ثابت hb_tag_t*) pa;
      ثابت hb_tag_t *b = (ثابت hb_tag_t*) pb;
      بازگشت (صحیح)(*a) - (صحیح)(*b);
    }

    منطقی change_tuple_variations_axis_limits (ثابت hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              ثابت hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      اگر (!axis_tags.alloc (normalized_axes_location.get_population ()))
        بازگشت نادرست;
      برای (auto t : normalized_axes_location.کلیدها ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      برای (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        اگر (!normalized_axes_location.has (axis_tag, &axis_limit))
          بازگشت نادرست;
        TripleDistances axis_triple_distances{1.f, 1.f};
        اگر (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        برای (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          اگر (!out) ادامه;

          unsigned new_len = new_vars.length + out.length;

          اگر (unlikely (!new_vars.alloc (new_len, نادرست)))
            بازگشت نادرست;

          برای (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      بازگشت درست;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    منطقی merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      اگر (unlikely (!new_vars.alloc (tuple_vars.length)))
	بازگشت نادرست;
      hb_hashmap_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      برای (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        اگر (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          اگر (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            بازگشت نادرست;
          ادامه;
        }

        unsigned *idx;
        اگر (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        وگرنه
        {
	  auto *new_var = new_vars.push ();
	  اگر (unlikely (new_vars.in_error ()))
	    بازگشت نادرست;
          hb_swap (*new_var, var);
          اگر (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            بازگشت نادرست;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      بازگشت درست;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    منطقی compile_all_point_sets ()
    {
      برای (ثابت auto& tuple: tuple_vars)
      {
        ثابت hb_vector_t<منطقی>* points_set = &(tuple.indices);
        اگر (point_data_map.has (points_set))
        {
          unsigned *شمار;
          اگر (unlikely (!point_set_شمار_map.has (points_set, &شمار) ||
                        !point_set_شمار_map.set (points_set, (*شمار) + 1)))
            بازگشت نادرست;
          ادامه;
        }

        hb_vector_t<unsigned نویسه> compiled_point_data;
        اگر (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          بازگشت نادرست;

        اگر (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_شمار_map.set (points_set, 1))
          بازگشت نادرست;
      }
      بازگشت درست;
    }

    /* find shared points set which saves most bytes */
    تهی find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      برای (ثابت auto& _ : point_data_map.iter_ref ())
      {
        ثابت hb_vector_t<منطقی>* points_set = _.first;
        unsigned data_length = _.second.length;
        اگر (!data_length) ادامه;
        unsigned *شمار;
        اگر (unlikely (!point_set_شمار_map.has (points_set, &شمار) ||
                      *شمار <= 1))
        {
          shared_points_bytes = nullptr;
          بازگشت;
        }

        unsigned saved_bytes = data_length * ((*شمار) -1);
        اگر (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    منطقی calc_inferred_deltas (ثابت contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      برای (tuple_delta_t& var : tuple_vars)
        اگر (!var.calc_inferred_deltas (contour_points, scratch))
          بازگشت نادرست;

      بازگشت درست;
    }

    منطقی iup_optimize (ثابت contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      برای (tuple_delta_t& var : tuple_vars)
      {
        اگر (!var.optimize (contour_points, is_composite, scratch))
          بازگشت نادرست;
      }
      بازگشت درست;
    }

    عمومی:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    تهی cull_unreachable (ثابت hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      برای (unsigned i = 0; i < tuple_vars.length; i++)
      {
	اگر (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  ادامه;
	اگر (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    منطقی instantiate (ثابت hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      ثابت hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      منطقی optimize = نادرست)
    {
      اگر (!tuple_vars) بازگشت درست;
      اگر (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        بازگشت نادرست;
      /* compute inferred deltas only for gvar */
      اگر (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	اگر (!calc_inferred_deltas (*contour_points, scratch))
	  بازگشت نادرست;
      }

      /* if iup delta opt is on, contour_points can't be null */
      اگر (optimize && !contour_points)
        بازگشت نادرست;

      اگر (!merge_tuple_variations (optimize ? contour_points : nullptr))
        بازگشت نادرست;

      اگر (optimize && !iup_optimize (*contour_points, scratch)) بازگشت نادرست;
      بازگشت !tuple_vars.in_error ();
    }

    منطقی compile_bytes (ثابت hb_map_t& axes_index_map,
                        ثابت hb_map_t& axes_old_index_tag_map,
                        منطقی use_shared_points,
                        منطقی is_gvar = نادرست,
                        ثابت hb_hashmap_t<ثابت hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      اگر (!tuple_vars)
        بازگشت درست;

      // compile points set and store data in hashmap
      اگر (!compile_all_point_sets ())
        بازگشت نادرست;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      اگر (use_shared_points)
      {
        find_shared_points ();
        اگر (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<صحیح> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      برای (auto& tuple: tuple_vars)
      {
        ثابت hb_vector_t<منطقی>* points_set = &(tuple.indices);
        hb_vector_t<unsigned نویسه> *points_data;
        اگر (unlikely (!point_data_map.has (points_set, &points_data)))
          بازگشت نادرست;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        اگر (!points_data->length)
          ادامه;
        اگر (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          بازگشت نادرست;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        اگر (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          بازگشت نادرست;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      اگر (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = درست;
        compiled_byte_size += 1;
      }

      بازگشت درست;
    }

    منطقی serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) ثابت
    {
      TRACE_SERIALIZE (این);
      برای (ثابت auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        اگر (c->in_error ()) return_trace (نادرست);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (درست);
    }

    منطقی serialize_var_data (hb_serialize_context_t *c, منطقی is_gvar) ثابت
    {
      TRACE_SERIALIZE (این);
      اگر (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      برای (ثابت auto& tuple: tuple_vars)
      {
        ثابت hb_vector_t<منطقی>* points_set = &(tuple.indices);
        hb_vector_t<unsigned نویسه> *point_data;
        اگر (!point_data_map.has (points_set, &point_data))
          return_trace (نادرست);

        اگر (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        اگر (c->in_error ()) return_trace (نادرست);
      }

      /* padding for gvar */
      اگر (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        اگر (!c->embed (pad)) return_trace (نادرست);
      }
      return_trace (درست);
    }
  };

  ساختار tuple_iterator_t
  {
    unsigned get_axis_شمار () ثابت { بازگشت axis_شمار; }

    تهی init (hb_bytes_t var_data_bytes_, unsigned صحیح axis_شمار_3, ثابت تهی *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_چپ = var_data->tuple_Var_شمار.get_شمار ();
      axis_شمار = axis_شمار_3;
      axis_شمار_times_2 = axis_شمار_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    منطقی get_shared_indices (hb_vector_t<unsigned صحیح> &shared_indices /* OUT */)
    {
      اگر (var_data->has_shared_point_numbers ())
      {
        ثابت HBUINT8 *base = &(table_base+var_data->data);
        ثابت HBUINT8 *p = base;
        اگر (!decompile_points (p, shared_indices, (ثابت HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) بازگشت نادرست;
        data_offset = p - base;
      }
      بازگشت درست;
    }

    منطقی is_valid ()
    {
      اگر (unlikely (tuples_چپ <= 0))
	بازگشت نادرست;

      current_tuple_size = TupleVariationHeader::min_size;
      اگر (unlikely (!var_data_bytes.check_end ((ثابت نویسه *) current_tuple + current_tuple_size)))
	بازگشت نادرست;

      current_tuple_size = current_tuple->get_size (axis_شمار_times_2);
      اگر (unlikely (!var_data_bytes.check_end ((ثابت نویسه *) current_tuple + current_tuple_size)))
	بازگشت نادرست;

      بازگشت درست;
    }

    HB_ALWAYS_INLINE
    منطقی move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_چپ--;
      بازگشت is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ثابت HBUINT8 *get_serialized_data () ثابت
    { بازگشت &(table_base+var_data->data) + data_offset; }

    خصوصی:
    signed tuples_چپ;
    ثابت TupleVariationData *var_data;
    unsigned صحیح axis_شمار;
    unsigned صحیح axis_شمار_times_2;
    unsigned صحیح data_offset;
    unsigned صحیح current_tuple_size;
    ثابت تهی *table_base;

    عمومی:
    hb_bytes_t var_data_bytes;
    ثابت TupleVariationHeader *current_tuple;
  };

  ایستا منطقی get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_شمار,
                                  ثابت تهی *table_base,
                                  hb_vector_t<unsigned صحیح> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_شمار, table_base);
    اگر (!iterator->get_shared_indices (shared_indices))
      بازگشت نادرست;
    بازگشت iterator->is_valid ();
  }

  منطقی has_shared_point_numbers () ثابت { بازگشت tuple_Var_شمار.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  منطقی cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_شمار,
			      hb_array_t<ثابت F2DOT14> shared_tuples,
			      ثابت hb_map_t *axes_old_index_tag_map,
			      ثابت hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<نویسه> &out /* OUT */,
			      منطقی *changed /* OUT */) ثابت
  {
    *changed = نادرست;

    hb_vector_t<unsigned صحیح> shared_indices;
    tuple_iterator_t iterator;
    اگر (!get_tuple_iterator (var_data_bytes, axis_شمار, این,
			     shared_indices, &iterator))
      بازگشت درست; /* no tuples or malformed: keep original */

    ثابت نویسه *bytes_start = var_data_bytes.arrayZ;
    ثابت نویسه *bytes_end = bytes_start + var_data_bytes.length;
    ثابت نویسه *serialized_base = (ثابت نویسه *) &(این+data);
    ثابت نویسه *shared_points_end = (ثابت نویسه *) iterator.get_serialized_data ();
    اگر (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      بازگشت درست;

    ساختار kept_tuple_t
    {
      ثابت نویسه *header;
      unsigned header_size;
      ثابت نویسه *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    انجامبده
    {
      ثابت TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_شمار * 2);
      unsigned data_size = header->get_data_size ();
      ثابت نویسه *tuple_data = (ثابت نویسه *) iterator.get_serialized_data ();
      اگر (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	بازگشت درست; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      اگر (!header->unpack_axis_tuples (axis_شمار, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	بازگشت درست;

      total++;
      اگر (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ثابت نویسه *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } وقتی (iterator.move_to_next ());

    اگر (unlikely (kept.in_error ())) بازگشت نادرست;
    اگر (kept.length == total) بازگشت درست; /* nothing to cull */

    *changed = درست;
    اگر (!kept.length) بازگشت درست; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    اگر (unlikely (!out.resize (new_size)))
      بازگشت نادرست;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_شمار = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    نویسه *p = out.arrayZ + min_size;
    برای (ثابت auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    برای (ثابت auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    بازگشت درست;
  }

  ایستا منطقی decompile_points (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned صحیح> &points /* OUT */,
				ثابت HBUINT8 *end)
  {
    شمارش packed_point_flag_t
    {
      POINTS_ARE_واژگان     = 0x80,
      POINT_RUN_شمار_MASK = 0x7F
    };

    اگر (unlikely (p + 1 > end)) بازگشت نادرست;

    unsigned شمار = *p++;
    اگر (شمار & POINTS_ARE_واژگان)
    {
      اگر (unlikely (p + 1 > end)) بازگشت نادرست;
      شمار = ((شمار & POINT_RUN_شمار_MASK) << 8) | *p++;
    }
    اگر (unlikely (!points.resize_dirty  (شمار))) بازگشت نادرست;

    unsigned n = 0;
    unsigned i = 0;
    وقتی (i < شمار)
    {
      اگر (unlikely (p + 1 > end)) بازگشت نادرست;
      unsigned control = *p++;
      unsigned run_شمار = (control & POINT_RUN_شمار_MASK) + 1;
      unsigned stop = i + run_شمار;
      اگر (unlikely (stop > شمار)) بازگشت نادرست;
      اگر (control & POINTS_ARE_واژگان)
      {
        اگر (unlikely (p + run_شمار * HBUINT16::static_size > end)) بازگشت نادرست;
        برای (; i < stop; i++)
        {
          n += *(ثابت HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      وگرنه
      {
        اگر (unlikely (p + run_شمار > end)) بازگشت نادرست;
        برای (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    بازگشت درست;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  ایستا منطقی decompile_deltas (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				ثابت HBUINT8 *end,
				منطقی consume_all = نادرست,
				unsigned start = 0)
  {
    بازگشت Tuple_مقادیر::decompile (p, deltas, end, consume_all, start);
  }

  منطقی has_data () ثابت { بازگشت tuple_Var_شمار; }

  منطقی decompile_tuple_variations (unsigned point_شمار,
                                   منطقی is_gvar,
                                   tuple_iterator_t iterator,
                                   ثابت hb_map_t *axes_old_index_tag_map,
                                   ثابت hb_vector_t<unsigned> &shared_indices,
                                   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   منطقی is_composite_glyph = نادرست) ثابت
  {
    بازگشت tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_شمار,
                                                        point_شمار, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  منطقی serialize (hb_serialize_context_t *c,
                  منطقی is_gvar,
                  ثابت tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (این);
    /* empty tuple variations, just return and skip serialization. */
    اگر (!tuple_variations) return_trace (درست);

    auto *out = c->start_embed (این);
    اگر (unlikely (!c->extend_min (out))) return_trace (نادرست);

    اگر (!c->check_assign (out->tuple_Var_شمار, tuple_variations.get_var_شمار (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (نادرست);

    unsigned total_header_len = 0;

    اگر (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (نادرست);

    unsigned data_offset = min_size + total_header_len;
    اگر (!is_gvar) data_offset += 4;
    اگر (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (نادرست);

    بازگشت tuple_variations.serialize_var_data (c, is_gvar);
  }

  محافظتشده:
  ساختار Tuple_Var_شمار : HBUINT16
  {
    friend ساختار tuple_variations_t;
    منطقی has_shared_point_numbers () ثابت { بازگشت ((*این) & SharedPointNumbers); }
    unsigned صحیح get_شمار () ثابت { بازگشت (*این) & شمار_Mask; }
    Tuple_Var_شمار& operator = (uint16_t i) { HBUINT16::operator= (i); بازگشت *این; }
    explicit operator منطقی () ثابت { بازگشت get_شمار (); }

    محافظتشده:
    شمارش Flags
    {
      SharedPointNumbers= 0x8000u,
      شمار_Mask         = 0x0FFFu
    };
    عمومی:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_شمار tuple_Var_شمار;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  عمومی:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
ساختار item_variations_t
{
  using region_t = ثابت hb_hashmap_t<hb_tag_t, Triple>*;
  خصوصی:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_data_رقم_rows;

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
  hb_vector_t<hb_vector_t<صحیح>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  منطقی has_long = نادرست;

  عمومی:
  منطقی has_long_word () ثابت
  { بازگشت has_long; }

  ثابت hb_vector_t<region_t>& get_region_list () ثابت
  { بازگشت region_list; }

  ثابت hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () ثابت
  { بازگشت encodings; }

  ثابت hb_map_t& get_varidx_map () ثابت
  { بازگشت varidx_map; }

  منطقی add_vardata_encoding_for_testing (hb_vector_t<ثابت hb_vector_t<صحیح>*> &&rows,
                                         unsigned رقم_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), رقم_cols));
    بازگشت !encodings.in_error ();
  }

  منطقی compile_varidx_map_for_testing (ثابت hb_hashmap_t<unsigned, ثابت hb_vector_t<صحیح>*>& front_mapping)
  { بازگشت compile_varidx_map (front_mapping); }

  منطقی instantiate (ثابت ItemVariationStore& varStore,
                    ثابت hb_subset_plan_t *plan,
                    منطقی optimize=درست,
                    منطقی use_no_variation_idx=درست,
                    ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> (),
                    منطقی layout_conditions = نادرست)
  {
    اگر (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      بازگشت نادرست;
    /* avar2 partial instancing: cull unreachable regions. */
    اگر (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      برای (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    اگر (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      بازگشت نادرست;
    اگر (layout_conditions &&
        (!instantiate_condition_مقادیر (plan, varStore) || !build_region_list ()))
      بازگشت نادرست;
    بازگشت as_item_varstore (optimize, use_no_variation_idx);
  }

  منطقی instantiate_condition_مقادیر (ثابت hb_subset_plan_t *plan,
                                      ثابت ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_مقدار_map;
    condition_map.clear ();
    اگر (!plan->layout_condition_مقادیر) بازگشت درست;
    using condition_مقدار_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, صحیح>>;
    hb_vector_t<condition_مقدار_t> conditions;
    برای (auto condition : plan->layout_condition_مقادیر.iter ())
      conditions.push (condition);
    اگر (unlikely (conditions.in_error ())) بازگشت نادرست;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ثابت condition_مقدار_t &a, ثابت condition_مقدار_t &b) {
      اگر (a.second.first != b.second.first)
        بازگشت a.second.first < b.second.first ? -1 : 1;
      بازگشت a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    برای (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      اگر (plan->gdef_varstore_inner_maps[i].get_population ())
        اگر (!outer_map.set (i, new_outer++)) بازگشت نادرست;
    برای (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      دوگانه value = condition.second.second +
                     varStore.get_delta<دوگانه> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      اگر (!outer_map.has (old_outer))
        ادامه;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      اگر (outer >= vars.length || inner >= var_data_رقم_rows[outer])
        ادامه;

      دوگانه maximum = fabs ((دوگانه) value);
      منطقی variable = نادرست;
      برای (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        اگر (unlikely (inner >= tuple.deltas_x.length)) بازگشت نادرست;
        اعشاری delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((دوگانه) delta));
      }
      اگر (!variable)
      {
        اگر (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), صحیح (value > 0))))
          بازگشت نادرست;
        ادامه;
      }

      دوگانه scale = 1.;
      وقتی (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      وقتی (maximum * scale * 2 <= INT_MAX)
      {
        منطقی integral = (دوگانه) value * scale == round ((دوگانه) value * scale);
        برای (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (دوگانه) tuple.deltas_x[inner] * scale ==
                      round ((دوگانه) tuple.deltas_x[inner] * scale);
        اگر (integral) بشکن;
        scale *= 2;
      }
      صحیح default_مقدار = (صحیح) round ((دوگانه) value * scale);
      اگر (scale != 1. || default_مقدار < -32768 || default_مقدار > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        برای (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          اگر (!tuple.deltas_x[inner]) ادامه;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (درست);
          copy.deltas_x.push ((اعشاری) round ((دوگانه) tuple.deltas_x[inner] * scale));
          اگر (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) بازگشت نادرست;
          private_row.tuple_vars.push (std::move (copy));
        }
        اگر (unlikely (vars.length >= 0xFFFFu)) بازگشت نادرست;
        اگر (unlikely (private_row.tuple_vars.in_error ())) بازگشت نادرست;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_رقم_rows.push (1);
        اگر (unlikely (vars.in_error () || var_data_رقم_rows.in_error ())) بازگشت نادرست;
        اگر (default_مقدار < -32768 || default_مقدار > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          صحیح bias = default_مقدار - hb_clamp (default_مقدار, -32768, 32767);
          default_مقدار -= bias;
          صحیح بالا = bias / 65536 * 65536;
          صحیح parts[] = {بالا, bias - بالا};
          برای (صحیح part : parts)
          {
            اگر (!part) ادامه;
            unsigned شمار = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            ثابت auto &tuples = vars[idx >> 16].tuple_vars;
            اگر (unlikely (tuples.in_error () || tuples.length != شمار + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) بازگشت نادرست;
          }
        }
      }
      اگر (!condition_map.set (condition.first, hb_pair (idx, default_مقدار)))
        بازگشت نادرست;
    }
    بازگشت !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  منطقی create_from_item_varstore (ثابت ItemVariationStore& varStore,
                                  ثابت hb_map_t& axes_old_index_tag_map,
                                  ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> ())
  {
    ثابت VarRegionList& regionList = varStore.get_region_list ();
    اگر (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      بازگشت نادرست;

    unsigned رقم_var_data = varStore.get_sub_table_شمار ();
    اگر (inner_maps && inner_maps.length != رقم_var_data) بازگشت نادرست;
    اگر (!vars.alloc (رقم_var_data) ||
        !var_data_رقم_rows.alloc (رقم_var_data)) بازگشت نادرست;

    برای (unsigned i = 0; i < رقم_var_data; i++)
    {
      اگر (inner_maps && !inner_maps.arrayZ[i].get_population ())
          ادامه;
      tuple_variations_t var_data_tuples;
      unsigned item_شمار_2 = 0;
      اگر (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_شمار_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        بازگشت نادرست;

      var_data_رقم_rows.push (item_شمار_2);
      vars.push (std::move (var_data_tuples));
    }
    بازگشت !vars.in_error () && !var_data_رقم_rows.in_error () && vars.length == var_data_رقم_rows.length;
  }

  منطقی instantiate_tuple_vars (ثابت hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               ثابت hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               منطقی build_regions = درست)
  {
    optimize_scratch_t scratch;
    برای (tuple_variations_t& tuple_vars : vars)
      اگر (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        بازگشت نادرست;

    بازگشت !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_شمار_2)
  {
    vars.push (tuple_variations_t ());
    var_data_رقم_rows.push (item_شمار_2);
    بازگشت vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_شمار (unsigned outer) ثابت
  { بازگشت outer < var_data_رقم_rows.length ? var_data_رقم_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    اگر (unlikely (outer >= vars.length ||
                  inner >= var_data_رقم_rows[outer] ||
                  var_data_رقم_rows[outer] >= 0xFFFFu))
      بازگشت (unsigned) -1;
    برای (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      اگر (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        بازگشت (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      اگر (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      اگر (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        بازگشت (unsigned) -1;
    }
    بازگشت var_data_رقم_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  تهی add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, صحیح delta, unsigned item_شمار_2)
  {
    اگر (outer >= vars.length) بازگشت;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    اگر (!tuple.indices.resize (item_شمار_2) ||
        !tuple.deltas_x.resize (item_شمار_2)) بازگشت;
    برای (unsigned i = 0; i < item_شمار_2; i++)
    {
      tuple.indices.arrayZ[i] = درست;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (اعشاری) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  منطقی build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    برای (ثابت tuple_variations_t& sub_table : vars)
    {
      برای (ثابت tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        اگر (!used_regions.has (r))
        {
          منطقی all_zeros = درست;
          برای (اعشاری d : tuple.deltas_x)
          {
            صحیح delta = hb_clamp_to<صحیح> (roundf ((دوگانه) d));
            اگر (delta != 0)
            {
              all_zeros = نادرست;
              بشکن;
            }
          }
          اگر (!all_zeros)
          {
            اگر (!used_regions.set (r, 1))
              بازگشت نادرست;
          }
        }
        اگر (all_regions.has (r))
          ادامه;
        اگر (!all_regions.set (r, 1))
          بازگشت نادرست;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    اگر (!all_regions || !all_unique_regions) بازگشت درست;

    اگر (!region_list.alloc (all_regions.get_population ()))
      بازگشت نادرست;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    برای (ثابت auto& r : orig_region_list)
    {
      اگر (!all_regions.has (&r) || !used_regions.has (&r))
        ادامه;

      region_list.push (&r);
      اگر (!region_map.set (&r, idx))
        بازگشت نادرست;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    برای (ثابت auto& r: all_unique_regions)
    {
      اگر (!all_regions.has (r) || !used_regions.has (r))
        ادامه;
      region_list.push (r);
      اگر (!region_map.set (r, idx))
        بازگشت نادرست;
      all_regions.del (r);
      idx++;
    }
    بازگشت (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  ساختار combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = پیشفرض;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      أكد (gain < 0xFFFFFF);
      أكد (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    منطقی operator < (ثابت combined_gain_idx_tuple_t& o)
    {
      بازگشت encoded < o.encoded;
    }

    منطقی operator <= (ثابت combined_gain_idx_tuple_t& o)
    {
      بازگشت encoded <= o.encoded;
    }

    unsigned idx_1 () ثابت { بازگشت (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () ثابت { بازگشت encoded & 0xFFFFF; };
  };

  منطقی as_item_varstore (منطقی optimize=درست, منطقی use_no_variation_idx=درست)
  {
    /* return true if no variation data */
    اگر (!region_list) بازگشت درست;
    unsigned رقم_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    برای (unsigned major = 0; major < var_data_رقم_rows.length; major++)
      total_rows += var_data_رقم_rows[major];

    اگر (!delta_rows.resize (total_rows)) بازگشت نادرست;
    /* init all rows to [0]*num_cols */
    برای (unsigned i = 0; i < total_rows; i++)
      اگر (!(delta_rows[i].resize (رقم_cols))) بازگشت نادرست;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, ثابت hb_vector_t<صحیح>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<ثابت hb_vector_t<صحیح> *> major_rows;
    hb_hashmap_t<ثابت hb_vector_t<صحیح>*, unsigned> delta_rows_map;
    برای (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ثابت tuple_variations_t& tuples = vars[major];
      unsigned رقم_rows = var_data_رقم_rows[major];

      اگر (!رقم_rows) ادامه;

      برای (ثابت tuple_delta_t& tuple: tuples.tuple_vars)
      {
        اگر (tuple.deltas_x.length != رقم_rows)
          بازگشت نادرست;

        /* skip unused regions */
        unsigned *col_idx;
        اگر (!region_map.has (&(tuple.axis_tuples), &col_idx))
          ادامه;

        برای (unsigned i = 0; i < رقم_rows; i++)
        {
          صحیح rounded_delta = hb_clamp_to<صحیح> (roundf ((دوگانه) tuple.deltas_x[i]));
          صحیح &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          اگر (unlikely (sum < INT_MIN || sum > INT_MAX)) بازگشت نادرست;
          delta = (صحیح) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      برای (unsigned minor = 0; minor < رقم_rows; minor++)
      {
	ثابت hb_vector_t<صحیح>& row = delta_rows[start_row + minor];
	اگر (use_no_variation_idx)
	{
	  منطقی all_zeros = درست;
	  برای (صحیح delta : row)
	  {
	    اگر (delta != 0)
	    {
	      all_zeros = نادرست;
	      بشکن;
	    }
	  }
	  اگر (all_zeros)
	    ادامه;
	}

	اگر (!front_mapping.set ((major<<16) + minor, &row))
	  بازگشت نادرست;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	اگر (optimize)
	{
	  اگر (delta_rows_map.has (&row))
	    ادامه;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      اگر (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        بازگشت نادرست;
      اگر (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), رقم_cols);
	اگر (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  بازگشت نادرست;
      }

      start_row += رقم_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    اگر (!optimize)
    {
      encodings = std::move (encoding_objs);
      بازگشت !encodings.in_error ();
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
    hb_vector_t<item_t> queue_اقلام;
    unsigned رقم_todos = encoding_objs.length;
    برای (unsigned i = 0; i < رقم_todos; i++)
    {
      برای (unsigned j = i + 1; j < رقم_todos; j++)
      {
        صحیح combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        اگر (combining_gain > 0)
          queue_اقلام.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_اقلام));

    hb_bit_set_t removed_todo_idxes;
    وقتی (queue)
    {
      auto t = queue.برداشت_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      اگر (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        ادامه;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      اگر (unlikely (removed_todo_idxes.in_error ())) بازگشت نادرست;

      encoding.merge (other_encoding);
      اگر (unlikely (encoding.in_error ())) بازگشت نادرست;

      برای (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        اگر (removed_todo_idxes.has (idx)) ادامه;

        ثابت delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        اگر (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          برای (ثابت auto& row : obj.اقلام)
            encoding.add_row (row);
          اگر (unlikely (encoding.in_error ())) بازگشت نادرست;

          removed_todo_idxes.add (idx);
          اگر (unlikely (removed_todo_idxes.in_error ())) بازگشت نادرست;
          ادامه;
        }

        صحیح combined_gain = encoding.gain_from_merging (obj);
        اگر (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      اگر (unlikely (encoding_objs.in_error ())) بازگشت نادرست;
    }

    صحیح رقم_final_encodings = (صحیح) encoding_objs.length - (صحیح) removed_todo_idxes.get_population ();
    اگر (رقم_final_encodings <= 0) بازگشت نادرست;

    اگر (!encodings.alloc (رقم_final_encodings)) بازگشت نادرست;
    برای (unsigned i = 0; i < encoding_objs.length; i++)
    {
      اگر (removed_todo_idxes.has (i)) ادامه;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    بازگشت compile_varidx_map (front_mapping);
  }

  خصوصی:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  منطقی compile_varidx_map (ثابت hb_hashmap_t<unsigned, ثابت hb_vector_t<صحیح>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ثابت hb_vector_t<صحیح>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    برای (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      اگر (encoding.is_empty () || encoding.in_error ())
        بازگشت نادرست;

      unsigned رقم_rows = encoding.اقلام.length;
      unsigned رقم_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.اقلام.qsort (_cmp_row);

      برای (unsigned start = 0; start < رقم_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (رقم_rows - start, 0xFFFFu);
        hb_vector_t<ثابت hb_vector_t<صحیح>*> rows;

        اگر (!rows.alloc (chunk_len))
          بازگشت نادرست;

        unsigned major = split_encodings.length;
        برای (unsigned minor = 0; minor < chunk_len; minor++)
        {
          ثابت hb_vector_t<صحیح> *row = encoding.اقلام.arrayZ[start + minor];
          rows.push (row);
          اگر (!back_mapping.set (row, (major << 16) + minor))
            بازگشت نادرست;
        }

        auto *split = split_encodings.push (std::move (rows), رقم_cols);
        اگر (unlikely (split_encodings.in_error () || split->in_error ()))
          بازگشت نادرست;
      }
    }

    encodings = std::move (split_encodings);
    اگر (encodings.in_error () || back_mapping.in_error ())
      بازگشت نادرست;

    برای (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      اگر (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      وگرنه
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    بازگشت !varidx_map.in_error ();
  }

  ایستا صحیح _cmp_row (ثابت تهی *pa, ثابت تهی *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ثابت hb_vector_t<صحیح>** a = (ثابت hb_vector_t<صحیح>**) pa;
    ثابت hb_vector_t<صحیح>** b = (ثابت hb_vector_t<صحیح>**) pb;

    برای (unsigned i = 0; i < (*b)->length; i++)
    {
      صحیح va = (*a)->arrayZ[i];
      صحیح vb = (*b)->arrayZ[i];
      اگر (va != vb)
        بازگشت va < vb ? -1 : 1;
    }
    بازگشت 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

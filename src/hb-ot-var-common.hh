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

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-alloc-pool.hh"
#ضمن "hb-priority-queue.hh"
#ضمن "hb-subset-instancer-iup.hh"


نطاق OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
بنية TupleVariationHeader
{
  friend بنية tuple_delta_t;
  size_t get_size (unsigned axis_تعداد_times_2) ثابت
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    إذا (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned تعداد_2 = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      إرجاع hb_unsigned_mul_add_saturate (تعداد_2, axis_تعداد_times_2, min_size);
    }
    إرجاع min_size;
  }

  unsigned get_data_size () ثابت { إرجاع varDataSize; }

  ثابت TupleVariationHeader &get_next (unsigned axis_تعداد_times_2) ثابت
  { إرجاع StructAtOffset<TupleVariationHeader> (هذا, get_size (axis_تعداد_times_2)); }

  منطقي unpack_axis_tuples (unsigned axis_تعداد,
                           ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                           ثابت hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ثابت
  {
    ثابت F2DOT14 *peak_tuple = nullptr;
    إذا (has_peak ())
      peak_tuple = get_peak_tuple (axis_تعداد);
    وإلا
    {
      unsigned صحيح index = get_index ();
      إذا (unlikely ((index + 1) * axis_تعداد > shared_tuples.length))
        إرجاع خطأ;
      peak_tuple = shared_tuples.sub_array (axis_تعداد * index, axis_تعداد).arrayZ;
    }

    ثابت F2DOT14 *start_tuple = nullptr;
    ثابت F2DOT14 *end_tuple = nullptr;
    منطقي has_interm = has_intermediate ();

    إذا (has_interm)
    {
      start_tuple = get_start_tuple (axis_تعداد);
      end_tuple = get_end_tuple (axis_تعداد);
    }

    لكل (unsigned i = 0; i < axis_تعداد; i++)
    {
      عائم peak = peak_tuple[i].to_float ();
      إذا (peak == 0.f) استمر;

      hb_tag_t *axis_tag;
      إذا (!axes_old_index_tag_map->has (i, &axis_tag))
        إرجاع خطأ;

      عائم start, end;
      إذا (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      وإلا
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    إرجاع true;
  }

  HB_ALWAYS_INLINE
  عائم calculate_scalar (hb_array_t<ثابت صحيح> coords, unsigned صحيح coord_تعداد,
			   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ثابت
  {
    unsigned tuple_index = tupleIndex;
    unsigned صحيح index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    منطقي plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    إذا (likely (plain_shared_tuple))
    {
      عائم scalar;
      إذا (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	إرجاع scalar;
    }

    ثابت F2DOT14 *peak_tuple;

    منطقي has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    إذا (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_تعداد);
      shared_tuple_scalar_cache = nullptr;
    }
    وإلا
    {
      عائم scalar;
      إذا (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        إذا (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	وإلا
	  إرجاع scalar;
      }

      إذا (unlikely ((index + 1) * coord_تعداد > shared_tuples.length))
        إرجاع 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_تعداد * index);

    }

    ثابت F2DOT14 *start_tuple = nullptr;
    ثابت F2DOT14 *end_tuple = nullptr;

    إذا (has_interm)
    {
      start_tuple = get_start_tuple (coord_تعداد);
      end_tuple = get_end_tuple (coord_تعداد);
    }

    عائم scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#إذا HB_FAST_رقم_ACCESS
    منطقي skip = coord_تعداد >= 16;
#endif
#endif
    لكل (unsigned صحيح i = 0; i < coord_تعداد; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#إذا HB_FAST_رقم_ACCESS
      إذا (skip)
      {
	بينما (i + 4 <= coord_تعداد && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	بينما (i < coord_تعداد && peak_tuple[i].to_int () == 0)
	  i += 1;
	إذا (i >= coord_تعداد)
	  اكسر;
      }
#endif
#endif

      صحيح peak = peak_tuple[i].to_int ();
      إذا (!peak) استمر;

      صحيح v = coords[i];
      إذا (!v) { scalar = 0.f; اكسر; }
      إذا (v == peak) استمر;

      إذا (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        صحيح start = start_tuple[i].to_int ();
        صحيح end = end_tuple[i].to_int ();
        إذا (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) استمر;
        إذا (v < start || v > end) { scalar = 0.f; اكسر; }
        إذا (v < peak)
        { إذا (peak != start) scalar *= (عائم) (v - start) / (peak - start); }
        وإلا
        { إذا (peak != end) scalar *= (عائم) (end - v) / (end - peak); }
      }
      وإلا إذا (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; اكسر; }
      وإلا
        scalar *= (عائم) v / peak;
    }
    إذا (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    إرجاع scalar;
  }

  منطقي           has_peak () ثابت { إرجاع tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  منطقي   has_intermediate () ثابت { إرجاع tupleIndex & TupleIndex::IntermediateRegion; }
  منطقي has_private_points () ثابت { إرجاع tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () ثابت { إرجاع tupleIndex & TupleIndex::TupleIndexMask; }
  ساكن constexpr unsigned max_shared_tuple_تعداد = 0x1000u;

  محمي:
  بنية TupleIndex : HBUINT16
  {
    تعداد Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); إرجاع *هذا; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<ثابت F2DOT14> get_all_tuples (unsigned axis_تعداد) ثابت
  { إرجاع StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_تعداد); }
  ثابت F2DOT14* get_all_tuples_base (unsigned axis_تعداد) ثابت
  { إرجاع StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  ثابت F2DOT14* get_peak_tuple (unsigned axis_تعداد) ثابت
  { إرجاع get_all_tuples_base (axis_تعداد); }
  ثابت F2DOT14* get_start_tuple (unsigned axis_تعداد) ثابت
  { إرجاع get_all_tuples_base (axis_تعداد) + has_peak () * axis_تعداد; }
  ثابت F2DOT14* get_end_tuple (unsigned axis_تعداد) ثابت
  { إرجاع get_all_tuples_base (axis_تعداد) + has_peak () * axis_تعداد + axis_تعداد; }

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
  عام:
  DEFINE_SIZE_MIN (4);
};

بنية optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<منطقي> opt_indices;
  hb_vector_t<صحيح> rounded_x_deltas;
  hb_vector_t<صحيح> rounded_y_deltas;
  hb_vector_t<عائم> opt_deltas_x;
  hb_vector_t<عائم> opt_deltas_y;
  hb_vector_t<unsigned حرف> opt_point_data;
  hb_vector_t<unsigned حرف> opt_deltas_data;
  hb_vector_t<unsigned حرف> point_data;
  hb_vector_t<unsigned حرف> deltas_data;
  hb_vector_t<صحيح> rounded_deltas;
};

بنية tuple_delta_t
{
  ساكن constexpr منطقي realloc_move = true;  // Watch out when adding new members!

  عام:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<منطقي> indices;

  hb_vector_t<عائم> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<عائم> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned حرف> compiled_tuple_header;
  hb_vector_t<unsigned حرف> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (ثابت tuple_delta_t& o) = افتراضي;
  tuple_delta_t& operator = (ثابت tuple_delta_t& o) = افتراضي;

  friend فراغ swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*هذا, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*هذا, o);
    إرجاع *هذا;
  }

  فراغ copy_from (ثابت tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  فراغ remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  منطقي set_tent (hb_tag_t axis_tag, Triple tent)
  { إرجاع axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (ثابت tuple_delta_t& o)
  {
    unsigned رقم = indices.length;
    لكل (unsigned i = 0; i < رقم; i++)
    {
      إذا (indices.arrayZ[i])
      {
        إذا (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          إذا (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      وإلا
      {
        إذا (!o.indices.arrayZ[i]) استمر;
        indices.arrayZ[i] = true;
        deltas_x[i] = o.deltas_x[i];
        إذا (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    إرجاع *هذا;
  }

  tuple_delta_t& operator *= (عائم scalar)
  {
    إذا (scalar == 1.0f)
      إرجاع *هذا;

    unsigned رقم = indices.length;
    إذا (deltas_y)
      لكل (unsigned i = 0; i < رقم; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    وإلا
      لكل (unsigned i = 0; i < رقم; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
      }
    إرجاع *هذا;
  }

  فراغ change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    إذا (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*هذا));
      إرجاع;
    }

    إذا ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->وسط && tent->وسط <= tent->maximum))
      إرجاع;

    إذا (tent->وسط == 0.f)
    {
      out.push (std::move (*هذا));
      إرجاع;
    }

    إذا (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.وسط &&
          axis_limit.وسط <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      إرجاع;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    لكل (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      إذا (i < solutions.length - 1)
	new_var.copy_from (*هذا, pool);
      وإلا
	new_var = std::move (*هذا);

      إذا (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      وإلا
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  منطقي compile_coords (ثابت hb_map_t& axes_index_map,
		       ثابت hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_تعداد = axes_index_map.get_population ();
    إذا (pool)
    {
      إذا (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_تعداد)))
	إرجاع خطأ;
    }
    وإلا إذا (unlikely (!compiled_peak_coords.resize (cur_axis_تعداد)))
      إرجاع خطأ;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_تعداد = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    لكل (unsigned i = 0; i < orig_axis_تعداد; i++)
    {
      إذا (!axes_index_map.has (i))
        استمر;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      إذا (axis_tuples.has (axis_tag, &coords))
      {
	عائم min_val = coords->minimum;
	عائم val = coords->وسط;
	عائم max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	إذا (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  إذا (!compiled_interm_coords)
	  {
	    إذا (pool)
	    {
	      إذا (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_تعداد)))
		إرجاع خطأ;
	    }
	    وإلا إذا (unlikely (!compiled_interm_coords.resize (2 * cur_axis_تعداد)))
	      إرجاع خطأ;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_تعداد);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_تعداد);

	    لكل (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      إذا (!peak) استمر;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	إذا (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    إرجاع !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  منطقي compile_tuple_var_header (ثابت hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 ثابت hb_map_t& axes_old_index_tag_map,
                                 ثابت hb_hashmap_t<ثابت hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    إذا (!compiled_deltas) إرجاع true;

    unsigned cur_axis_تعداد = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_تعداد * (F2DOT14::static_size) + 4;
    إذا (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, خطأ))) إرجاع خطأ;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    إذا (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_تعداد = 0;
    unsigned *shared_tuple_idx;
    إذا (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    وإلا
    {
      peak_تعداد = encode_peak_coords(coords, flag);
      إذا (!peak_تعداد) إرجاع خطأ;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_تعداد = encode_interm_coords (coords.sub_array (peak_تعداد), flag);

    /* pointdata length = 0 implies "use shared points" */
    إذا (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_تعداد + interim_تعداد) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    إرجاع true;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) ثابت
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * حجم (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    إرجاع compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) ثابت
  {
    إذا (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * حجم (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    إرجاع compiled_interm_coords.length;
  }

  منطقي compile_deltas (hb_vector_t<صحيح> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { إرجاع compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ساكن منطقي compile_deltas (hb_array_t<ثابت منطقي> point_indices,
			      hb_array_t<ثابت عائم> x_deltas,
			      hb_array_t<ثابت عائم> y_deltas,
			      hb_vector_t<unsigned حرف> &compiled_deltas, /* OUT */
			      hb_vector_t<صحيح> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    إذا (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      إرجاع خطأ;

    unsigned j = 0;
    لكل (unsigned i = 0; i < point_indices.length; i++)
    {
      إذا (!point_indices[i]) استمر;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<صحيح> (roundf ((مزدوج) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    إذا (!rounded_deltas) إرجاع true;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    إذا (y_deltas)
      alloc_len *= 2;

    إذا (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, خطأ))) إرجاع خطأ;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    إذا (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      لكل (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        إذا (!point_indices[idx]) استمر;
        صحيح rounded_delta = hb_clamp_to<صحيح> (roundf ((مزدوج) y_deltas.arrayZ[idx]));

        إذا (j >= rounded_deltas.length) إرجاع خطأ;

        rounded_deltas[j++] = rounded_delta;
      }

      إذا (j != rounded_deltas.length) إرجاع خطأ;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    إرجاع true;
  }

  ساكن unsigned compile_deltas (hb_array_t<unsigned حرف> encoded_bytes,
				  hb_array_t<ثابت صحيح> deltas)
  {
    إرجاع Tuple_قيم::compile_unsafe (deltas, encoded_bytes);
  }

  منطقي calc_inferred_deltas (ثابت contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_تعداد = orig_points.length;
    إذا (point_تعداد != indices.length)
      إرجاع خطأ;

    unsigned ref_تعداد_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    لكل (unsigned i = 0; i < point_تعداد; i++)
    {
      ref_تعداد_2 += indices.arrayZ[i];
      إذا (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    إذا (ref_تعداد_2 == point_تعداد)
      إرجاع true;
    إذا (unlikely (end_points.in_error ())) إرجاع خطأ;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    لكل (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_تعداد = 0;
      لكل (unsigned i = start_point; i < end_point + 1; i++)
        unref_تعداد += indices.arrayZ[i];
      unref_تعداد = (end_point - start_point + 1) - unref_تعداد;

      unsigned j = start_point;
      إذا (unref_تعداد == 0 || unref_تعداد > end_point - start_point)
        اذهب no_more_gaps;
      لكل (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned صحيح prev, next, i;
        لكل (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          إذا (indices.arrayZ[i] && !indices.arrayZ[j]) اكسر;
        }
        prev = j = i;
        لكل (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          إذا (!indices.arrayZ[i] && indices.arrayZ[j]) اكسر;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        لكل (;;)
        {
          i = next_index (i, start_point, end_point);
          إذا (i == next) اكسر;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          إذا (--unref_تعداد == 0) اذهب no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    لكل (unsigned i = 0; i < point_تعداد; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      إذا ( !indices[i])
      {
        إذا (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = true;
      }
    }
    إرجاع true;
  }

  منطقي optimize (ثابت contour_point_vector_t& contour_points,
                 منطقي is_composite,
		 optimize_scratch_t &scratch,
                 مزدوج tolerance = 0.5 + 1e-10)
  {
    unsigned تعداد_2 = contour_points.length;
    إذا (deltas_x.length != تعداد_2 ||
        deltas_y.length != تعداد_2)
      إرجاع خطأ;

    hb_vector_t<منطقي> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<صحيح> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<صحيح> &rounded_y_deltas = scratch.rounded_y_deltas;

    إذا (unlikely (!rounded_x_deltas.resize_dirty  (تعداد_2) ||
                  !rounded_y_deltas.resize_dirty  (تعداد_2)))
      إرجاع خطأ;

    لكل (unsigned i = 0; i < تعداد_2; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<صحيح> (roundf ((مزدوج) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<صحيح> (roundf ((مزدوج) deltas_y.arrayZ[i]));
    }

    إذا (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      إرجاع خطأ;

    unsigned ref_تعداد_2 = 0;
    لكل (منطقي ref_flag : opt_indices)
       ref_تعداد_2 += ref_flag;

    إذا (ref_تعداد_2 == تعداد_2) إرجاع true;

    hb_vector_t<عائم> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<عائم> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    منطقي is_comp_glyph_wo_deltas = (is_composite && ref_تعداد_2 == 0);
    إذا (is_comp_glyph_wo_deltas)
    {
      إذا (unlikely (!opt_deltas_x.resize (تعداد_2) ||
                    !opt_deltas_y.resize (تعداد_2)))
        إرجاع خطأ;

      opt_indices.arrayZ[0] = true;
      لكل (unsigned i = 1; i < تعداد_2; i++)
        opt_indices.arrayZ[i] = خطأ;
    }

    hb_vector_t<unsigned حرف> &opt_point_data = scratch.opt_point_data.reset ();
    إذا (!compile_point_set (opt_indices, opt_point_data))
      إرجاع خطأ;
    hb_vector_t<unsigned حرف> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    إذا (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      إرجاع خطأ;

    hb_vector_t<unsigned حرف> &point_data = scratch.point_data.reset ();
    إذا (!compile_point_set (indices, point_data))
      إرجاع خطأ;
    hb_vector_t<unsigned حرف> &deltas_data = scratch.deltas_data.reset ();
    إذا (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      إرجاع خطأ;

    إذا (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      إذا (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    إرجاع !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  ساكن منطقي compile_point_set (ثابت hb_vector_t<منطقي> &point_indices,
                                 hb_vector_t<unsigned حرف>& compiled_points /* OUT */)
  {
    unsigned رقم_points = 0;
    لكل (منطقي i : point_indices)
      إذا (i) رقم_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    إذا (!رقم_points) إرجاع true;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    إذا (رقم_points == indices_length)
      إرجاع compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned رقم_bytes = 2 + 3 *رقم_points;
    إذا (unlikely (!compiled_points.resize_dirty  (رقم_bytes)))
      إرجاع خطأ;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    إذا (رقم_points < 0x80)
      compiled_points.arrayZ[pos++] = رقم_points;
    وإلا
    {
      compiled_points.arrayZ[pos++] = ((رقم_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = رقم_points & 0xFF;
    }

    ثابت unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_قيمة = 0;
    unsigned رقم_encoded = 0;
    بينما (i < indices_length && رقم_encoded < رقم_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      منطقي use_byte_encoding = خطأ;
      منطقي new_run = true;
      بينما (i < indices_length && رقم_encoded < رقم_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        بينما (i < indices_length && !point_indices[i])
          i++;

        إذا (i >= indices_length) اكسر;

        unsigned cur_قيمة = i;
        unsigned delta = cur_قيمة - last_قيمة;

        إذا (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = خطأ;
        }

        إذا (use_byte_encoding && delta > 0xFF)
          اكسر;

        إذا (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        وإلا
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_قيمة = cur_قيمة;
        run_length++;
        رقم_encoded++;
      }

      إذا (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      وإلا
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    إرجاع compiled_points.resize_dirty  (pos);
  }

  ساكن عائم infer_delta (عائم target_val, عائم prev_val, عائم next_val, عائم prev_delta, عائم next_delta)
  {
    إذا (prev_val == next_val)
      إرجاع (prev_delta == next_delta) ? prev_delta : 0.f;
    وإلا إذا (target_val <= hb_min (prev_val, next_val))
      إرجاع (prev_val < next_val) ? prev_delta : next_delta;
    وإلا إذا (target_val >= hb_max (prev_val, next_val))
      إرجاع (prev_val > next_val) ? prev_delta : next_delta;

    عائم r = (target_val - prev_val) / (next_val - prev_val);
    إرجاع prev_delta + r * (next_delta - prev_delta);
  }

  ساكن unsigned صحيح next_index (unsigned صحيح i, unsigned صحيح start, unsigned صحيح end)
  { إرجاع (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
بنية TupleVariationData
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (هذا));
  }

  size_t get_size (unsigned axis_تعداد_times_2) ثابت
  {
    size_t total_size = min_size;
    unsigned تعداد_2 = tuple_Var_تعداد.get_تعداد ();
    ثابت TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    لكل (unsigned i = 0; i < تعداد_2; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_تعداد_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_تعداد_times_2);
    }

    إرجاع total_size;
  }

  ثابت TupleVariationHeader &get_tuple_var_header (فراغ) ثابت
  { إرجاع StructAfter<TupleVariationHeader> (data); }

  بنية tuple_iterator_t;
  بنية tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    خاص:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ثابت hb_vector_t<منطقي>*, hb_vector_t<unsigned حرف>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ثابت hb_vector_t<منطقي>*, unsigned> point_set_تعداد_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned حرف> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    منطقي needs_padding = خطأ;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    منطقي is_composite = خطأ;

    عام:
    tuple_variations_t () = افتراضي;
    tuple_variations_t (ثابت tuple_variations_t&) = delete;
    tuple_variations_t& operator=(ثابت tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = افتراضي;
    tuple_variations_t& operator=(tuple_variations_t&&) = افتراضي;
    ~tuple_variations_t () = افتراضي;

    explicit operator منطقي () ثابت { إرجاع منطقي (tuple_vars); }
    unsigned get_var_تعداد () ثابت
    {
      unsigned تعداد_2 = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      لكل (auto& tuple: tuple_vars)
        إذا (tuple.compiled_deltas) تعداد_2++;

      إذا (shared_points_bytes && shared_points_bytes->length)
        تعداد_2 |= Tuple_Var_تعداد::SharedPointNumbers;
      إرجاع تعداد_2;
    }

    unsigned get_compiled_byte_size () ثابت
    { إرجاع compiled_byte_size; }

    منطقي create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_تعداد,
                                     unsigned point_تعداد,
                                     منطقي is_gvar,
                                     ثابت hb_map_t *axes_old_index_tag_map,
                                     ثابت hb_vector_t<unsigned> &shared_indices,
                                     ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     منطقي is_composite_glyph = خطأ)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<صحيح> deltas_x;
      hb_vector_t<صحيح> deltas_y;
      افعل
      {
        ثابت HBUINT8 *p = iterator.get_serialized_data ();
        unsigned صحيح length = iterator.current_tuple->get_data_size ();
        إذا (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          إرجاع خطأ;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        إذا (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_تعداد (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          إرجاع خطأ;

	private_indices.reset ();
        منطقي has_private_points = iterator.current_tuple->has_private_points ();
        ثابت HBUINT8 *end = p + length;
        إذا (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          إرجاع خطأ;

        ثابت hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        منطقي apply_to_all = (indices.length == 0);
        unsigned رقم_deltas = apply_to_all ? point_تعداد : indices.length;

        إذا (unlikely (!deltas_x.resize_dirty  (رقم_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          إرجاع خطأ;

        إذا (is_gvar)
        {
          إذا (unlikely (!deltas_y.resize_dirty  (رقم_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            إرجاع خطأ;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        إذا (unlikely (!var.indices.allocate_from_pool (pool, point_تعداد) ||
                      !var.deltas_x.allocate_from_pool (pool, point_تعداد, خطأ)))
          إرجاع خطأ;

        إذا (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_تعداد, خطأ)))
          إرجاع خطأ;

        لكل (unsigned i = 0; i < رقم_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          إذا (idx >= point_تعداد) استمر;
          var.indices[idx] = true;
          var.deltas_x[idx] = deltas_x[i];
          إذا (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } بينما (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      إرجاع true;
    }

    منطقي create_from_item_var_data (ثابت VarData &var_data,
                                    ثابت hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    ثابت hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_تعداد_2,
                                    ثابت hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      إذا (&var_data == &Null (VarData))
        إرجاع true;

      unsigned رقم_regions = var_data.get_region_index_تعداد ();
      إذا (!tuple_vars.alloc (رقم_regions)) إرجاع خطأ;

      item_تعداد_2 = inner_map ? inner_map->get_population () : var_data.get_item_تعداد ();
      إذا (!item_تعداد_2) إرجاع true;
      unsigned row_size = var_data.get_row_size ();
      ثابت HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      لكل (unsigned r = 0; r < رقم_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        إذا (!tuple.deltas_x.resize_dirty  (item_تعداد_2) ||
            !tuple.indices.resize_dirty  (item_تعداد_2))
          إرجاع خطأ;

        لكل (unsigned i = 0; i < item_تعداد_2; i++)
        {
          tuple.indices.arrayZ[i] = true;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        إذا (region_index >= regions.length) إرجاع خطأ;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      إرجاع !tuple_vars.in_error ();
    }

    خاص:
    ساكن صحيح _cmp_axis_tag (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت hb_tag_t *a = (ثابت hb_tag_t*) pa;
      ثابت hb_tag_t *b = (ثابت hb_tag_t*) pb;
      إرجاع (صحيح)(*a) - (صحيح)(*b);
    }

    منطقي change_tuple_variations_axis_limits (ثابت hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              ثابت hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      إذا (!axis_tags.alloc (normalized_axes_location.get_population ()))
        إرجاع خطأ;
      لكل (auto t : normalized_axes_location.مفاتيح ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      لكل (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        إذا (!normalized_axes_location.has (axis_tag, &axis_limit))
          إرجاع خطأ;
        TripleDistances axis_triple_distances{1.f, 1.f};
        إذا (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        لكل (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          إذا (!out) استمر;

          unsigned new_len = new_vars.length + out.length;

          إذا (unlikely (!new_vars.alloc (new_len, خطأ)))
            إرجاع خطأ;

          لكل (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      إرجاع true;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    منطقي merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      إذا (unlikely (!new_vars.alloc (tuple_vars.length)))
	إرجاع خطأ;
      hb_hashmap_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      لكل (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        إذا (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          إذا (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            إرجاع خطأ;
          استمر;
        }

        unsigned *idx;
        إذا (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        وإلا
        {
	  auto *new_var = new_vars.push ();
	  إذا (unlikely (new_vars.in_error ()))
	    إرجاع خطأ;
          hb_swap (*new_var, var);
          إذا (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            إرجاع خطأ;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      إرجاع true;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    منطقي compile_all_point_sets ()
    {
      لكل (ثابت auto& tuple: tuple_vars)
      {
        ثابت hb_vector_t<منطقي>* points_set = &(tuple.indices);
        إذا (point_data_map.has (points_set))
        {
          unsigned *تعداد_2;
          إذا (unlikely (!point_set_تعداد_map.has (points_set, &تعداد_2) ||
                        !point_set_تعداد_map.set (points_set, (*تعداد_2) + 1)))
            إرجاع خطأ;
          استمر;
        }

        hb_vector_t<unsigned حرف> compiled_point_data;
        إذا (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          إرجاع خطأ;

        إذا (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_تعداد_map.set (points_set, 1))
          إرجاع خطأ;
      }
      إرجاع true;
    }

    /* find shared points set which saves most bytes */
    فراغ find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      لكل (ثابت auto& _ : point_data_map.iter_ref ())
      {
        ثابت hb_vector_t<منطقي>* points_set = _.first;
        unsigned data_length = _.second.length;
        إذا (!data_length) استمر;
        unsigned *تعداد_2;
        إذا (unlikely (!point_set_تعداد_map.has (points_set, &تعداد_2) ||
                      *تعداد_2 <= 1))
        {
          shared_points_bytes = nullptr;
          إرجاع;
        }

        unsigned saved_bytes = data_length * ((*تعداد_2) -1);
        إذا (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    منطقي calc_inferred_deltas (ثابت contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
        إذا (!var.calc_inferred_deltas (contour_points, scratch))
          إرجاع خطأ;

      إرجاع true;
    }

    منطقي iup_optimize (ثابت contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
      {
        إذا (!var.optimize (contour_points, is_composite, scratch))
          إرجاع خطأ;
      }
      إرجاع true;
    }

    عام:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    فراغ cull_unreachable (ثابت hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      لكل (unsigned i = 0; i < tuple_vars.length; i++)
      {
	إذا (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  استمر;
	إذا (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    منطقي instantiate (ثابت hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      ثابت hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      منطقي optimize = خطأ)
    {
      إذا (!tuple_vars) إرجاع true;
      إذا (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        إرجاع خطأ;
      /* compute inferred deltas only for gvar */
      إذا (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	إذا (!calc_inferred_deltas (*contour_points, scratch))
	  إرجاع خطأ;
      }

      /* if iup delta opt is on, contour_points can't be null */
      إذا (optimize && !contour_points)
        إرجاع خطأ;

      إذا (!merge_tuple_variations (optimize ? contour_points : nullptr))
        إرجاع خطأ;

      إذا (optimize && !iup_optimize (*contour_points, scratch)) إرجاع خطأ;
      إرجاع !tuple_vars.in_error ();
    }

    منطقي compile_bytes (ثابت hb_map_t& axes_index_map,
                        ثابت hb_map_t& axes_old_index_tag_map,
                        منطقي use_shared_points,
                        منطقي is_gvar = خطأ,
                        ثابت hb_hashmap_t<ثابت hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      إذا (!tuple_vars)
        إرجاع true;

      // compile points set and store data in hashmap
      إذا (!compile_all_point_sets ())
        إرجاع خطأ;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      إذا (use_shared_points)
      {
        find_shared_points ();
        إذا (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<صحيح> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      لكل (auto& tuple: tuple_vars)
      {
        ثابت hb_vector_t<منطقي>* points_set = &(tuple.indices);
        hb_vector_t<unsigned حرف> *points_data;
        إذا (unlikely (!point_data_map.has (points_set, &points_data)))
          إرجاع خطأ;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        إذا (!points_data->length)
          استمر;
        إذا (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          إرجاع خطأ;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        إذا (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          إرجاع خطأ;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      إذا (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = true;
        compiled_byte_size += 1;
      }

      إرجاع true;
    }

    منطقي serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) ثابت
    {
      TRACE_SERIALIZE (هذا);
      لكل (ثابت auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        إذا (c->in_error ()) return_trace (خطأ);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (true);
    }

    منطقي serialize_var_data (hb_serialize_context_t *c, منطقي is_gvar) ثابت
    {
      TRACE_SERIALIZE (هذا);
      إذا (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      لكل (ثابت auto& tuple: tuple_vars)
      {
        ثابت hb_vector_t<منطقي>* points_set = &(tuple.indices);
        hb_vector_t<unsigned حرف> *point_data;
        إذا (!point_data_map.has (points_set, &point_data))
          return_trace (خطأ);

        إذا (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        إذا (c->in_error ()) return_trace (خطأ);
      }

      /* padding for gvar */
      إذا (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        إذا (!c->embed (pad)) return_trace (خطأ);
      }
      return_trace (true);
    }
  };

  بنية tuple_iterator_t
  {
    unsigned get_axis_تعداد () ثابت { إرجاع axis_تعداد; }

    فراغ init (hb_bytes_t var_data_bytes_, unsigned صحيح axis_تعداد_3, ثابت فراغ *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_يسار = var_data->tuple_Var_تعداد.get_تعداد ();
      axis_تعداد = axis_تعداد_3;
      axis_تعداد_times_2 = axis_تعداد_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    منطقي get_shared_indices (hb_vector_t<unsigned صحيح> &shared_indices /* OUT */)
    {
      إذا (var_data->has_shared_point_numbers ())
      {
        ثابت HBUINT8 *base = &(table_base+var_data->data);
        ثابت HBUINT8 *p = base;
        إذا (!decompile_points (p, shared_indices, (ثابت HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) إرجاع خطأ;
        data_offset = p - base;
      }
      إرجاع true;
    }

    منطقي is_valid ()
    {
      إذا (unlikely (tuples_يسار <= 0))
	إرجاع خطأ;

      current_tuple_size = TupleVariationHeader::min_size;
      إذا (unlikely (!var_data_bytes.check_end ((ثابت حرف *) current_tuple + current_tuple_size)))
	إرجاع خطأ;

      current_tuple_size = current_tuple->get_size (axis_تعداد_times_2);
      إذا (unlikely (!var_data_bytes.check_end ((ثابت حرف *) current_tuple + current_tuple_size)))
	إرجاع خطأ;

      إرجاع true;
    }

    HB_ALWAYS_INLINE
    منطقي move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_يسار--;
      إرجاع is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ثابت HBUINT8 *get_serialized_data () ثابت
    { إرجاع &(table_base+var_data->data) + data_offset; }

    خاص:
    signed tuples_يسار;
    ثابت TupleVariationData *var_data;
    unsigned صحيح axis_تعداد;
    unsigned صحيح axis_تعداد_times_2;
    unsigned صحيح data_offset;
    unsigned صحيح current_tuple_size;
    ثابت فراغ *table_base;

    عام:
    hb_bytes_t var_data_bytes;
    ثابت TupleVariationHeader *current_tuple;
  };

  ساكن منطقي get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_تعداد,
                                  ثابت فراغ *table_base,
                                  hb_vector_t<unsigned صحيح> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_تعداد, table_base);
    إذا (!iterator->get_shared_indices (shared_indices))
      إرجاع خطأ;
    إرجاع iterator->is_valid ();
  }

  منطقي has_shared_point_numbers () ثابت { إرجاع tuple_Var_تعداد.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  منطقي cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_تعداد,
			      hb_array_t<ثابت F2DOT14> shared_tuples,
			      ثابت hb_map_t *axes_old_index_tag_map,
			      ثابت hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<حرف> &out /* OUT */,
			      منطقي *changed /* OUT */) ثابت
  {
    *changed = خطأ;

    hb_vector_t<unsigned صحيح> shared_indices;
    tuple_iterator_t iterator;
    إذا (!get_tuple_iterator (var_data_bytes, axis_تعداد, هذا,
			     shared_indices, &iterator))
      إرجاع true; /* no tuples or malformed: keep original */

    ثابت حرف *bytes_start = var_data_bytes.arrayZ;
    ثابت حرف *bytes_end = bytes_start + var_data_bytes.length;
    ثابت حرف *serialized_base = (ثابت حرف *) &(هذا+data);
    ثابت حرف *shared_points_end = (ثابت حرف *) iterator.get_serialized_data ();
    إذا (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      إرجاع true;

    بنية kept_tuple_t
    {
      ثابت حرف *header;
      unsigned header_size;
      ثابت حرف *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    افعل
    {
      ثابت TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_تعداد * 2);
      unsigned data_size = header->get_data_size ();
      ثابت حرف *tuple_data = (ثابت حرف *) iterator.get_serialized_data ();
      إذا (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	إرجاع true; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      إذا (!header->unpack_axis_tuples (axis_تعداد, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	إرجاع true;

      total++;
      إذا (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ثابت حرف *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } بينما (iterator.move_to_next ());

    إذا (unlikely (kept.in_error ())) إرجاع خطأ;
    إذا (kept.length == total) إرجاع true; /* nothing to cull */

    *changed = true;
    إذا (!kept.length) إرجاع true; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    إذا (unlikely (!out.resize (new_size)))
      إرجاع خطأ;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_تعداد = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    حرف *p = out.arrayZ + min_size;
    لكل (ثابت auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    لكل (ثابت auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    إرجاع true;
  }

  ساكن منطقي decompile_points (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned صحيح> &points /* OUT */,
				ثابت HBUINT8 *end)
  {
    تعداد packed_point_flag_t
    {
      POINTS_ARE_كلمات     = 0x80,
      POINT_RUN_تعداد_MASK = 0x7F
    };

    إذا (unlikely (p + 1 > end)) إرجاع خطأ;

    unsigned تعداد_2 = *p++;
    إذا (تعداد_2 & POINTS_ARE_كلمات)
    {
      إذا (unlikely (p + 1 > end)) إرجاع خطأ;
      تعداد_2 = ((تعداد_2 & POINT_RUN_تعداد_MASK) << 8) | *p++;
    }
    إذا (unlikely (!points.resize_dirty  (تعداد_2))) إرجاع خطأ;

    unsigned n = 0;
    unsigned i = 0;
    بينما (i < تعداد_2)
    {
      إذا (unlikely (p + 1 > end)) إرجاع خطأ;
      unsigned control = *p++;
      unsigned run_تعداد = (control & POINT_RUN_تعداد_MASK) + 1;
      unsigned stop = i + run_تعداد;
      إذا (unlikely (stop > تعداد_2)) إرجاع خطأ;
      إذا (control & POINTS_ARE_كلمات)
      {
        إذا (unlikely (p + run_تعداد * HBUINT16::static_size > end)) إرجاع خطأ;
        لكل (; i < stop; i++)
        {
          n += *(ثابت HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      وإلا
      {
        إذا (unlikely (p + run_تعداد > end)) إرجاع خطأ;
        لكل (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    إرجاع true;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  ساكن منطقي decompile_deltas (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				ثابت HBUINT8 *end,
				منطقي consume_all = خطأ,
				unsigned start = 0)
  {
    إرجاع Tuple_قيم::decompile (p, deltas, end, consume_all, start);
  }

  منطقي has_data () ثابت { إرجاع tuple_Var_تعداد; }

  منطقي decompile_tuple_variations (unsigned point_تعداد,
                                   منطقي is_gvar,
                                   tuple_iterator_t iterator,
                                   ثابت hb_map_t *axes_old_index_tag_map,
                                   ثابت hb_vector_t<unsigned> &shared_indices,
                                   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   منطقي is_composite_glyph = خطأ) ثابت
  {
    إرجاع tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_تعداد,
                                                        point_تعداد, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي is_gvar,
                  ثابت tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (هذا);
    /* empty tuple variations, just return and skip serialization. */
    إذا (!tuple_variations) return_trace (true);

    auto *out = c->start_embed (هذا);
    إذا (unlikely (!c->extend_min (out))) return_trace (خطأ);

    إذا (!c->check_assign (out->tuple_Var_تعداد, tuple_variations.get_var_تعداد (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (خطأ);

    unsigned total_header_len = 0;

    إذا (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (خطأ);

    unsigned data_offset = min_size + total_header_len;
    إذا (!is_gvar) data_offset += 4;
    إذا (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (خطأ);

    إرجاع tuple_variations.serialize_var_data (c, is_gvar);
  }

  محمي:
  بنية Tuple_Var_تعداد : HBUINT16
  {
    friend بنية tuple_variations_t;
    منطقي has_shared_point_numbers () ثابت { إرجاع ((*هذا) & SharedPointNumbers); }
    unsigned صحيح get_تعداد () ثابت { إرجاع (*هذا) & تعداد_Mask; }
    Tuple_Var_تعداد& operator = (uint16_t i) { HBUINT16::operator= (i); إرجاع *هذا; }
    explicit operator منطقي () ثابت { إرجاع get_تعداد (); }

    محمي:
    تعداد Flags
    {
      SharedPointNumbers= 0x8000u,
      تعداد_Mask         = 0x0FFFu
    };
    عام:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_تعداد tuple_Var_تعداد;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  عام:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
بنية item_variations_t
{
  using region_t = ثابت hb_hashmap_t<hb_tag_t, Triple>*;
  خاص:
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
  hb_vector_t<hb_vector_t<صحيح>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  منطقي has_long = خطأ;

  عام:
  منطقي has_long_word () ثابت
  { إرجاع has_long; }

  ثابت hb_vector_t<region_t>& get_region_list () ثابت
  { إرجاع region_list; }

  ثابت hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () ثابت
  { إرجاع encodings; }

  ثابت hb_map_t& get_varidx_map () ثابت
  { إرجاع varidx_map; }

  منطقي add_vardata_encoding_for_testing (hb_vector_t<ثابت hb_vector_t<صحيح>*> &&rows,
                                         unsigned رقم_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), رقم_cols));
    إرجاع !encodings.in_error ();
  }

  منطقي compile_varidx_map_for_testing (ثابت hb_hashmap_t<unsigned, ثابت hb_vector_t<صحيح>*>& front_mapping)
  { إرجاع compile_varidx_map (front_mapping); }

  منطقي instantiate (ثابت ItemVariationStore& varStore,
                    ثابت hb_subset_plan_t *plan,
                    منطقي optimize=true,
                    منطقي use_no_variation_idx=true,
                    ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> (),
                    منطقي layout_conditions = خطأ)
  {
    إذا (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      إرجاع خطأ;
    /* avar2 partial instancing: cull unreachable regions. */
    إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      لكل (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    إذا (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      إرجاع خطأ;
    إذا (layout_conditions &&
        (!instantiate_condition_قيم (plan, varStore) || !build_region_list ()))
      إرجاع خطأ;
    إرجاع as_item_varstore (optimize, use_no_variation_idx);
  }

  منطقي instantiate_condition_قيم (ثابت hb_subset_plan_t *plan,
                                      ثابت ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_قيمة_map;
    condition_map.clear ();
    إذا (!plan->layout_condition_قيم) إرجاع true;
    using condition_قيمة_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, صحيح>>;
    hb_vector_t<condition_قيمة_t> conditions;
    لكل (auto condition : plan->layout_condition_قيم.iter ())
      conditions.push (condition);
    إذا (unlikely (conditions.in_error ())) إرجاع خطأ;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ثابت condition_قيمة_t &a, ثابت condition_قيمة_t &b) {
      إذا (a.second.first != b.second.first)
        إرجاع a.second.first < b.second.first ? -1 : 1;
      إرجاع a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    لكل (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      إذا (plan->gdef_varstore_inner_maps[i].get_population ())
        إذا (!outer_map.set (i, new_outer++)) إرجاع خطأ;
    لكل (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      مزدوج value = condition.second.second +
                     varStore.get_delta<مزدوج> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      إذا (!outer_map.has (old_outer))
        استمر;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      إذا (outer >= vars.length || inner >= var_data_رقم_rows[outer])
        استمر;

      مزدوج maximum = fabs ((مزدوج) value);
      منطقي variable = خطأ;
      لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        إذا (unlikely (inner >= tuple.deltas_x.length)) إرجاع خطأ;
        عائم delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((مزدوج) delta));
      }
      إذا (!variable)
      {
        إذا (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), صحيح (value > 0))))
          إرجاع خطأ;
        استمر;
      }

      مزدوج scale = 1.;
      بينما (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      بينما (maximum * scale * 2 <= INT_MAX)
      {
        منطقي integral = (مزدوج) value * scale == round ((مزدوج) value * scale);
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (مزدوج) tuple.deltas_x[inner] * scale ==
                      round ((مزدوج) tuple.deltas_x[inner] * scale);
        إذا (integral) اكسر;
        scale *= 2;
      }
      صحيح default_قيمة = (صحيح) round ((مزدوج) value * scale);
      إذا (scale != 1. || default_قيمة < -32768 || default_قيمة > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          إذا (!tuple.deltas_x[inner]) استمر;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (true);
          copy.deltas_x.push ((عائم) round ((مزدوج) tuple.deltas_x[inner] * scale));
          إذا (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) إرجاع خطأ;
          private_row.tuple_vars.push (std::move (copy));
        }
        إذا (unlikely (vars.length >= 0xFFFFu)) إرجاع خطأ;
        إذا (unlikely (private_row.tuple_vars.in_error ())) إرجاع خطأ;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_رقم_rows.push (1);
        إذا (unlikely (vars.in_error () || var_data_رقم_rows.in_error ())) إرجاع خطأ;
        إذا (default_قيمة < -32768 || default_قيمة > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          صحيح bias = default_قيمة - hb_clamp (default_قيمة, -32768, 32767);
          default_قيمة -= bias;
          صحيح أعلى = bias / 65536 * 65536;
          صحيح parts[] = {أعلى, bias - أعلى};
          لكل (صحيح part : parts)
          {
            إذا (!part) استمر;
            unsigned تعداد_2 = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            ثابت auto &tuples = vars[idx >> 16].tuple_vars;
            إذا (unlikely (tuples.in_error () || tuples.length != تعداد_2 + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) إرجاع خطأ;
          }
        }
      }
      إذا (!condition_map.set (condition.first, hb_pair (idx, default_قيمة)))
        إرجاع خطأ;
    }
    إرجاع !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  منطقي create_from_item_varstore (ثابت ItemVariationStore& varStore,
                                  ثابت hb_map_t& axes_old_index_tag_map,
                                  ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> ())
  {
    ثابت VarRegionList& regionList = varStore.get_region_list ();
    إذا (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      إرجاع خطأ;

    unsigned رقم_var_data = varStore.get_sub_table_تعداد ();
    إذا (inner_maps && inner_maps.length != رقم_var_data) إرجاع خطأ;
    إذا (!vars.alloc (رقم_var_data) ||
        !var_data_رقم_rows.alloc (رقم_var_data)) إرجاع خطأ;

    لكل (unsigned i = 0; i < رقم_var_data; i++)
    {
      إذا (inner_maps && !inner_maps.arrayZ[i].get_population ())
          استمر;
      tuple_variations_t var_data_tuples;
      unsigned item_تعداد_2 = 0;
      إذا (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_تعداد_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        إرجاع خطأ;

      var_data_رقم_rows.push (item_تعداد_2);
      vars.push (std::move (var_data_tuples));
    }
    إرجاع !vars.in_error () && !var_data_رقم_rows.in_error () && vars.length == var_data_رقم_rows.length;
  }

  منطقي instantiate_tuple_vars (ثابت hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               ثابت hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               منطقي build_regions = true)
  {
    optimize_scratch_t scratch;
    لكل (tuple_variations_t& tuple_vars : vars)
      إذا (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        إرجاع خطأ;

    إرجاع !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_تعداد_2)
  {
    vars.push (tuple_variations_t ());
    var_data_رقم_rows.push (item_تعداد_2);
    إرجاع vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_تعداد (unsigned outer) ثابت
  { إرجاع outer < var_data_رقم_rows.length ? var_data_رقم_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    إذا (unlikely (outer >= vars.length ||
                  inner >= var_data_رقم_rows[outer] ||
                  var_data_رقم_rows[outer] >= 0xFFFFu))
      إرجاع (unsigned) -1;
    لكل (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      إذا (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        إرجاع (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      إذا (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      إذا (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        إرجاع (unsigned) -1;
    }
    إرجاع var_data_رقم_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  فراغ add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, صحيح delta, unsigned item_تعداد_2)
  {
    إذا (outer >= vars.length) إرجاع;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    إذا (!tuple.indices.resize (item_تعداد_2) ||
        !tuple.deltas_x.resize (item_تعداد_2)) إرجاع;
    لكل (unsigned i = 0; i < item_تعداد_2; i++)
    {
      tuple.indices.arrayZ[i] = true;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (عائم) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  منطقي build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    لكل (ثابت tuple_variations_t& sub_table : vars)
    {
      لكل (ثابت tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        إذا (!used_regions.has (r))
        {
          منطقي all_zeros = true;
          لكل (عائم d : tuple.deltas_x)
          {
            صحيح delta = hb_clamp_to<صحيح> (roundf ((مزدوج) d));
            إذا (delta != 0)
            {
              all_zeros = خطأ;
              اكسر;
            }
          }
          إذا (!all_zeros)
          {
            إذا (!used_regions.set (r, 1))
              إرجاع خطأ;
          }
        }
        إذا (all_regions.has (r))
          استمر;
        إذا (!all_regions.set (r, 1))
          إرجاع خطأ;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    إذا (!all_regions || !all_unique_regions) إرجاع true;

    إذا (!region_list.alloc (all_regions.get_population ()))
      إرجاع خطأ;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    لكل (ثابت auto& r : orig_region_list)
    {
      إذا (!all_regions.has (&r) || !used_regions.has (&r))
        استمر;

      region_list.push (&r);
      إذا (!region_map.set (&r, idx))
        إرجاع خطأ;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    لكل (ثابت auto& r: all_unique_regions)
    {
      إذا (!all_regions.has (r) || !used_regions.has (r))
        استمر;
      region_list.push (r);
      إذا (!region_map.set (r, idx))
        إرجاع خطأ;
      all_regions.del (r);
      idx++;
    }
    إرجاع (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  بنية combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = افتراضي;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      أكد (gain < 0xFFFFFF);
      أكد (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    منطقي operator < (ثابت combined_gain_idx_tuple_t& o)
    {
      إرجاع encoded < o.encoded;
    }

    منطقي operator <= (ثابت combined_gain_idx_tuple_t& o)
    {
      إرجاع encoded <= o.encoded;
    }

    unsigned idx_1 () ثابت { إرجاع (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () ثابت { إرجاع encoded & 0xFFFFF; };
  };

  منطقي as_item_varstore (منطقي optimize=true, منطقي use_no_variation_idx=true)
  {
    /* return true if no variation data */
    إذا (!region_list) إرجاع true;
    unsigned رقم_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    لكل (unsigned major = 0; major < var_data_رقم_rows.length; major++)
      total_rows += var_data_رقم_rows[major];

    إذا (!delta_rows.resize (total_rows)) إرجاع خطأ;
    /* init all rows to [0]*num_cols */
    لكل (unsigned i = 0; i < total_rows; i++)
      إذا (!(delta_rows[i].resize (رقم_cols))) إرجاع خطأ;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, ثابت hb_vector_t<صحيح>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<ثابت hb_vector_t<صحيح> *> major_rows;
    hb_hashmap_t<ثابت hb_vector_t<صحيح>*, unsigned> delta_rows_map;
    لكل (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ثابت tuple_variations_t& tuples = vars[major];
      unsigned رقم_rows = var_data_رقم_rows[major];

      إذا (!رقم_rows) استمر;

      لكل (ثابت tuple_delta_t& tuple: tuples.tuple_vars)
      {
        إذا (tuple.deltas_x.length != رقم_rows)
          إرجاع خطأ;

        /* skip unused regions */
        unsigned *col_idx;
        إذا (!region_map.has (&(tuple.axis_tuples), &col_idx))
          استمر;

        لكل (unsigned i = 0; i < رقم_rows; i++)
        {
          صحيح rounded_delta = hb_clamp_to<صحيح> (roundf ((مزدوج) tuple.deltas_x[i]));
          صحيح &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          إذا (unlikely (sum < INT_MIN || sum > INT_MAX)) إرجاع خطأ;
          delta = (صحيح) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      لكل (unsigned minor = 0; minor < رقم_rows; minor++)
      {
	ثابت hb_vector_t<صحيح>& row = delta_rows[start_row + minor];
	إذا (use_no_variation_idx)
	{
	  منطقي all_zeros = true;
	  لكل (صحيح delta : row)
	  {
	    إذا (delta != 0)
	    {
	      all_zeros = خطأ;
	      اكسر;
	    }
	  }
	  إذا (all_zeros)
	    استمر;
	}

	إذا (!front_mapping.set ((major<<16) + minor, &row))
	  إرجاع خطأ;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	إذا (optimize)
	{
	  إذا (delta_rows_map.has (&row))
	    استمر;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      إذا (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        إرجاع خطأ;
      إذا (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), رقم_cols);
	إذا (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  إرجاع خطأ;
      }

      start_row += رقم_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    إذا (!optimize)
    {
      encodings = std::move (encoding_objs);
      إرجاع !encodings.in_error ();
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
    hb_vector_t<item_t> queue_عناصر;
    unsigned رقم_todos = encoding_objs.length;
    لكل (unsigned i = 0; i < رقم_todos; i++)
    {
      لكل (unsigned j = i + 1; j < رقم_todos; j++)
      {
        صحيح combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        إذا (combining_gain > 0)
          queue_عناصر.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_عناصر));

    hb_bit_set_t removed_todo_idxes;
    بينما (queue)
    {
      auto t = queue.اسحب_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      إذا (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        استمر;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      إذا (unlikely (removed_todo_idxes.in_error ())) إرجاع خطأ;

      encoding.merge (other_encoding);
      إذا (unlikely (encoding.in_error ())) إرجاع خطأ;

      لكل (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        إذا (removed_todo_idxes.has (idx)) استمر;

        ثابت delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        إذا (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          لكل (ثابت auto& row : obj.عناصر)
            encoding.add_row (row);
          إذا (unlikely (encoding.in_error ())) إرجاع خطأ;

          removed_todo_idxes.add (idx);
          إذا (unlikely (removed_todo_idxes.in_error ())) إرجاع خطأ;
          استمر;
        }

        صحيح combined_gain = encoding.gain_from_merging (obj);
        إذا (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      إذا (unlikely (encoding_objs.in_error ())) إرجاع خطأ;
    }

    صحيح رقم_final_encodings = (صحيح) encoding_objs.length - (صحيح) removed_todo_idxes.get_population ();
    إذا (رقم_final_encodings <= 0) إرجاع خطأ;

    إذا (!encodings.alloc (رقم_final_encodings)) إرجاع خطأ;
    لكل (unsigned i = 0; i < encoding_objs.length; i++)
    {
      إذا (removed_todo_idxes.has (i)) استمر;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    إرجاع compile_varidx_map (front_mapping);
  }

  خاص:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  منطقي compile_varidx_map (ثابت hb_hashmap_t<unsigned, ثابت hb_vector_t<صحيح>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ثابت hb_vector_t<صحيح>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    لكل (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      إذا (encoding.is_empty () || encoding.in_error ())
        إرجاع خطأ;

      unsigned رقم_rows = encoding.عناصر.length;
      unsigned رقم_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.عناصر.qsort (_cmp_row);

      لكل (unsigned start = 0; start < رقم_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (رقم_rows - start, 0xFFFFu);
        hb_vector_t<ثابت hb_vector_t<صحيح>*> rows;

        إذا (!rows.alloc (chunk_len))
          إرجاع خطأ;

        unsigned major = split_encodings.length;
        لكل (unsigned minor = 0; minor < chunk_len; minor++)
        {
          ثابت hb_vector_t<صحيح> *row = encoding.عناصر.arrayZ[start + minor];
          rows.push (row);
          إذا (!back_mapping.set (row, (major << 16) + minor))
            إرجاع خطأ;
        }

        auto *split = split_encodings.push (std::move (rows), رقم_cols);
        إذا (unlikely (split_encodings.in_error () || split->in_error ()))
          إرجاع خطأ;
      }
    }

    encodings = std::move (split_encodings);
    إذا (encodings.in_error () || back_mapping.in_error ())
      إرجاع خطأ;

    لكل (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      إذا (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      وإلا
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    إرجاع !varidx_map.in_error ();
  }

  ساكن صحيح _cmp_row (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ثابت hb_vector_t<صحيح>** a = (ثابت hb_vector_t<صحيح>**) pa;
    ثابت hb_vector_t<صحيح>** b = (ثابت hb_vector_t<صحيح>**) pb;

    لكل (unsigned i = 0; i < (*b)->length; i++)
    {
      صحيح va = (*a)->arrayZ[i];
      صحيح vb = (*b)->arrayZ[i];
      إذا (va != vb)
        إرجاع va < vb ? -1 : 1;
    }
    إرجاع 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

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

#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-alloc-pool.hh"
#अन्तर्भाव "hb-priority-queue.hh"
#अन्तर्भाव "hb-subset-instancer-iup.hh"


नामाकाश OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
संरचना TupleVariationHeader
{
  friend संरचना tuple_delta_t;
  size_t get_size (unsigned axis_गणनम्_times_2) ध्रुव
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    सचे (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned गणनम् = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      पटिनिवत्त hb_unsigned_mul_add_saturate (गणनम्, axis_गणनम्_times_2, min_size);
    }
    पटिनिवत्त min_size;
  }

  unsigned get_data_size () ध्रुव { पटिनिवत्त varDataSize; }

  ध्रुव TupleVariationHeader &get_next (unsigned axis_गणनम्_times_2) ध्रुव
  { पटिनिवत्त StructAtOffset<TupleVariationHeader> (एतत्, get_size (axis_गणनम्_times_2)); }

  तार्किक unpack_axis_tuples (unsigned axis_गणनम्,
                           ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
                           ध्रुव hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ध्रुव
  {
    ध्रुव F2DOT14 *peak_tuple = nullptr;
    सचे (has_peak ())
      peak_tuple = get_peak_tuple (axis_गणनम्);
    अञ्ञथा
    {
      unsigned पूर्णाङ्क index = get_index ();
      सचे (unlikely ((index + 1) * axis_गणनम् > shared_tuples.length))
        पटिनिवत्त मिच्छा;
      peak_tuple = shared_tuples.sub_array (axis_गणनम् * index, axis_गणनम्).arrayZ;
    }

    ध्रुव F2DOT14 *start_tuple = nullptr;
    ध्रुव F2DOT14 *end_tuple = nullptr;
    तार्किक has_interm = has_intermediate ();

    सचे (has_interm)
    {
      start_tuple = get_start_tuple (axis_गणनम्);
      end_tuple = get_end_tuple (axis_गणनम्);
    }

    अत्थाय (unsigned i = 0; i < axis_गणनम्; i++)
    {
      भिन्न peak = peak_tuple[i].to_float ();
      सचे (peak == 0.f) अनुवर्त;

      hb_tag_t *axis_tag;
      सचे (!axes_old_index_tag_map->has (i, &axis_tag))
        पटिनिवत्त मिच्छा;

      भिन्न start, end;
      सचे (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      अञ्ञथा
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    पटिनिवत्त सच्च;
  }

  HB_ALWAYS_INLINE
  भिन्न calculate_scalar (hb_array_t<ध्रुव पूर्णाङ्क> coords, unsigned पूर्णाङ्क coord_गणनम्,
			   ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ध्रुव
  {
    unsigned tuple_index = tupleIndex;
    unsigned पूर्णाङ्क index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    तार्किक plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    सचे (likely (plain_shared_tuple))
    {
      भिन्न scalar;
      सचे (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	पटिनिवत्त scalar;
    }

    ध्रुव F2DOT14 *peak_tuple;

    तार्किक has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    सचे (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_गणनम्);
      shared_tuple_scalar_cache = nullptr;
    }
    अञ्ञथा
    {
      भिन्न scalar;
      सचे (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        सचे (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	अञ्ञथा
	  पटिनिवत्त scalar;
      }

      सचे (unlikely ((index + 1) * coord_गणनम् > shared_tuples.length))
        पटिनिवत्त 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_गणनम् * index);

    }

    ध्रुव F2DOT14 *start_tuple = nullptr;
    ध्रुव F2DOT14 *end_tuple = nullptr;

    सचे (has_interm)
    {
      start_tuple = get_start_tuple (coord_गणनम्);
      end_tuple = get_end_tuple (coord_गणनम्);
    }

    भिन्न scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#सचे HB_FAST_अङ्कः_ACCESS
    तार्किक skip = coord_गणनम् >= 16;
#endif
#endif
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < coord_गणनम्; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#सचे HB_FAST_अङ्कः_ACCESS
      सचे (skip)
      {
	याव (i + 4 <= coord_गणनम् && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	याव (i < coord_गणनम् && peak_tuple[i].to_int () == 0)
	  i += 1;
	सचे (i >= coord_गणनम्)
	  भङ्ग;
      }
#endif
#endif

      पूर्णाङ्क peak = peak_tuple[i].to_int ();
      सचे (!peak) अनुवर्त;

      पूर्णाङ्क v = coords[i];
      सचे (!v) { scalar = 0.f; भङ्ग; }
      सचे (v == peak) अनुवर्त;

      सचे (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        पूर्णाङ्क start = start_tuple[i].to_int ();
        पूर्णाङ्क end = end_tuple[i].to_int ();
        सचे (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) अनुवर्त;
        सचे (v < start || v > end) { scalar = 0.f; भङ्ग; }
        सचे (v < peak)
        { सचे (peak != start) scalar *= (भिन्न) (v - start) / (peak - start); }
        अञ्ञथा
        { सचे (peak != end) scalar *= (भिन्न) (end - v) / (end - peak); }
      }
      अञ्ञथा सचे (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; भङ्ग; }
      अञ्ञथा
        scalar *= (भिन्न) v / peak;
    }
    सचे (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    पटिनिवत्त scalar;
  }

  तार्किक           has_peak () ध्रुव { पटिनिवत्त tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  तार्किक   has_intermediate () ध्रुव { पटिनिवत्त tupleIndex & TupleIndex::IntermediateRegion; }
  तार्किक has_private_points () ध्रुव { पटिनिवत्त tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () ध्रुव { पटिनिवत्त tupleIndex & TupleIndex::TupleIndexMask; }
  स्थिर constexpr unsigned max_shared_tuple_गणनम् = 0x1000u;

  रक्षित:
  संरचना TupleIndex : HBUINT16
  {
    गणना Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); पटिनिवत्त *एतत्; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<ध्रुव F2DOT14> get_all_tuples (unsigned axis_गणनम्) ध्रुव
  { पटिनिवत्त StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_गणनम्); }
  ध्रुव F2DOT14* get_all_tuples_base (unsigned axis_गणनम्) ध्रुव
  { पटिनिवत्त StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  ध्रुव F2DOT14* get_peak_tuple (unsigned axis_गणनम्) ध्रुव
  { पटिनिवत्त get_all_tuples_base (axis_गणनम्); }
  ध्रुव F2DOT14* get_start_tuple (unsigned axis_गणनम्) ध्रुव
  { पटिनिवत्त get_all_tuples_base (axis_गणनम्) + has_peak () * axis_गणनम्; }
  ध्रुव F2DOT14* get_end_tuple (unsigned axis_गणनम्) ध्रुव
  { पटिनिवत्त get_all_tuples_base (axis_गणनम्) + has_peak () * axis_गणनम् + axis_गणनम्; }

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
  सार्वजनिक:
  DEFINE_SIZE_MIN (4);
};

संरचना optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<तार्किक> opt_indices;
  hb_vector_t<पूर्णाङ्क> rounded_x_deltas;
  hb_vector_t<पूर्णाङ्क> rounded_y_deltas;
  hb_vector_t<भिन्न> opt_deltas_x;
  hb_vector_t<भिन्न> opt_deltas_y;
  hb_vector_t<unsigned वर्ण> opt_point_data;
  hb_vector_t<unsigned वर्ण> opt_deltas_data;
  hb_vector_t<unsigned वर्ण> point_data;
  hb_vector_t<unsigned वर्ण> deltas_data;
  hb_vector_t<पूर्णाङ्क> rounded_deltas;
};

संरचना tuple_delta_t
{
  स्थिर constexpr तार्किक realloc_move = सच्च;  // Watch out when adding new members!

  सार्वजनिक:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<तार्किक> indices;

  hb_vector_t<भिन्न> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<भिन्न> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned वर्ण> compiled_tuple_header;
  hb_vector_t<unsigned वर्ण> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (ध्रुव tuple_delta_t& o) = स्वभाव;
  tuple_delta_t& operator = (ध्रुव tuple_delta_t& o) = स्वभाव;

  friend शून्य swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*एतत्, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*एतत्, o);
    पटिनिवत्त *एतत्;
  }

  शून्य copy_from (ध्रुव tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  शून्य remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  तार्किक set_tent (hb_tag_t axis_tag, Triple tent)
  { पटिनिवत्त axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (ध्रुव tuple_delta_t& o)
  {
    unsigned अङ्कः = indices.length;
    अत्थाय (unsigned i = 0; i < अङ्कः; i++)
    {
      सचे (indices.arrayZ[i])
      {
        सचे (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          सचे (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      अञ्ञथा
      {
        सचे (!o.indices.arrayZ[i]) अनुवर्त;
        indices.arrayZ[i] = सच्च;
        deltas_x[i] = o.deltas_x[i];
        सचे (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    पटिनिवत्त *एतत्;
  }

  tuple_delta_t& operator *= (भिन्न scalar)
  {
    सचे (scalar == 1.0f)
      पटिनिवत्त *एतत्;

    unsigned अङ्कः = indices.length;
    सचे (deltas_y)
      अत्थाय (unsigned i = 0; i < अङ्कः; i++)
      {
	सचे (!indices.arrayZ[i]) अनुवर्त;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    अञ्ञथा
      अत्थाय (unsigned i = 0; i < अङ्कः; i++)
      {
	सचे (!indices.arrayZ[i]) अनुवर्त;
	deltas_x[i] *= scalar;
      }
    पटिनिवत्त *एतत्;
  }

  शून्य change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    सचे (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*एतत्));
      पटिनिवत्त;
    }

    सचे ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->मध्यः && tent->मध्यः <= tent->maximum))
      पटिनिवत्त;

    सचे (tent->मध्यः == 0.f)
    {
      out.push (std::move (*एतत्));
      पटिनिवत्त;
    }

    सचे (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.मध्यः &&
          axis_limit.मध्यः <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      पटिनिवत्त;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    अत्थाय (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      सचे (i < solutions.length - 1)
	new_var.copy_from (*एतत्, pool);
      अञ्ञथा
	new_var = std::move (*एतत्);

      सचे (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      अञ्ञथा
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  तार्किक compile_coords (ध्रुव hb_map_t& axes_index_map,
		       ध्रुव hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_गणनम् = axes_index_map.get_population ();
    सचे (pool)
    {
      सचे (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_गणनम्)))
	पटिनिवत्त मिच्छा;
    }
    अञ्ञथा सचे (unlikely (!compiled_peak_coords.resize (cur_axis_गणनम्)))
      पटिनिवत्त मिच्छा;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_गणनम् = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    अत्थाय (unsigned i = 0; i < orig_axis_गणनम्; i++)
    {
      सचे (!axes_index_map.has (i))
        अनुवर्त;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      सचे (axis_tuples.has (axis_tag, &coords))
      {
	भिन्न min_val = coords->minimum;
	भिन्न val = coords->मध्यः;
	भिन्न max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	सचे (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  सचे (!compiled_interm_coords)
	  {
	    सचे (pool)
	    {
	      सचे (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_गणनम्)))
		पटिनिवत्त मिच्छा;
	    }
	    अञ्ञथा सचे (unlikely (!compiled_interm_coords.resize (2 * cur_axis_गणनम्)))
	      पटिनिवत्त मिच्छा;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_गणनम्);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_गणनम्);

	    अत्थाय (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      सचे (!peak) अनुवर्त;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	सचे (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    पटिनिवत्त !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  तार्किक compile_tuple_var_header (ध्रुव hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 ध्रुव hb_map_t& axes_old_index_tag_map,
                                 ध्रुव hb_hashmap_t<ध्रुव hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    सचे (!compiled_deltas) पटिनिवत्त सच्च;

    unsigned cur_axis_गणनम् = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_गणनम् * (F2DOT14::static_size) + 4;
    सचे (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, मिच्छा))) पटिनिवत्त मिच्छा;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    सचे (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_गणनम् = 0;
    unsigned *shared_tuple_idx;
    सचे (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    अञ्ञथा
    {
      peak_गणनम् = encode_peak_coords(coords, flag);
      सचे (!peak_गणनम्) पटिनिवत्त मिच्छा;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_गणनम् = encode_interm_coords (coords.sub_array (peak_गणनम्), flag);

    /* pointdata length = 0 implies "use shared points" */
    सचे (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_गणनम् + interim_गणनम्) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    पटिनिवत्त सच्च;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) ध्रुव
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * परिमाण (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    पटिनिवत्त compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) ध्रुव
  {
    सचे (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * परिमाण (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    पटिनिवत्त compiled_interm_coords.length;
  }

  तार्किक compile_deltas (hb_vector_t<पूर्णाङ्क> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { पटिनिवत्त compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  स्थिर तार्किक compile_deltas (hb_array_t<ध्रुव तार्किक> point_indices,
			      hb_array_t<ध्रुव भिन्न> x_deltas,
			      hb_array_t<ध्रुव भिन्न> y_deltas,
			      hb_vector_t<unsigned वर्ण> &compiled_deltas, /* OUT */
			      hb_vector_t<पूर्णाङ्क> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    सचे (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      पटिनिवत्त मिच्छा;

    unsigned j = 0;
    अत्थाय (unsigned i = 0; i < point_indices.length; i++)
    {
      सचे (!point_indices[i]) अनुवर्त;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    सचे (!rounded_deltas) पटिनिवत्त सच्च;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    सचे (y_deltas)
      alloc_len *= 2;

    सचे (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, मिच्छा))) पटिनिवत्त मिच्छा;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    सचे (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      अत्थाय (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        सचे (!point_indices[idx]) अनुवर्त;
        पूर्णाङ्क rounded_delta = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) y_deltas.arrayZ[idx]));

        सचे (j >= rounded_deltas.length) पटिनिवत्त मिच्छा;

        rounded_deltas[j++] = rounded_delta;
      }

      सचे (j != rounded_deltas.length) पटिनिवत्त मिच्छा;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    पटिनिवत्त सच्च;
  }

  स्थिर unsigned compile_deltas (hb_array_t<unsigned वर्ण> encoded_bytes,
				  hb_array_t<ध्रुव पूर्णाङ्क> deltas)
  {
    पटिनिवत्त Tuple_मानानि::compile_unsafe (deltas, encoded_bytes);
  }

  तार्किक calc_inferred_deltas (ध्रुव contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_गणनम् = orig_points.length;
    सचे (point_गणनम् != indices.length)
      पटिनिवत्त मिच्छा;

    unsigned ref_गणनम्_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    अत्थाय (unsigned i = 0; i < point_गणनम्; i++)
    {
      ref_गणनम्_2 += indices.arrayZ[i];
      सचे (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    सचे (ref_गणनम्_2 == point_गणनम्)
      पटिनिवत्त सच्च;
    सचे (unlikely (end_points.in_error ())) पटिनिवत्त मिच्छा;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    अत्थाय (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_गणनम् = 0;
      अत्थाय (unsigned i = start_point; i < end_point + 1; i++)
        unref_गणनम् += indices.arrayZ[i];
      unref_गणनम् = (end_point - start_point + 1) - unref_गणनम्;

      unsigned j = start_point;
      सचे (unref_गणनम् == 0 || unref_गणनम् > end_point - start_point)
        गच्छ no_more_gaps;
      अत्थाय (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned पूर्णाङ्क prev, next, i;
        अत्थाय (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          सचे (indices.arrayZ[i] && !indices.arrayZ[j]) भङ्ग;
        }
        prev = j = i;
        अत्थाय (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          सचे (!indices.arrayZ[i] && indices.arrayZ[j]) भङ्ग;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        अत्थाय (;;)
        {
          i = next_index (i, start_point, end_point);
          सचे (i == next) भङ्ग;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          सचे (--unref_गणनम् == 0) गच्छ no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    अत्थाय (unsigned i = 0; i < point_गणनम्; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      सचे ( !indices[i])
      {
        सचे (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = सच्च;
      }
    }
    पटिनिवत्त सच्च;
  }

  तार्किक optimize (ध्रुव contour_point_vector_t& contour_points,
                 तार्किक is_composite,
		 optimize_scratch_t &scratch,
                 द्विगुण tolerance = 0.5 + 1e-10)
  {
    unsigned गणनम् = contour_points.length;
    सचे (deltas_x.length != गणनम् ||
        deltas_y.length != गणनम्)
      पटिनिवत्त मिच्छा;

    hb_vector_t<तार्किक> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<पूर्णाङ्क> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<पूर्णाङ्क> &rounded_y_deltas = scratch.rounded_y_deltas;

    सचे (unlikely (!rounded_x_deltas.resize_dirty  (गणनम्) ||
                  !rounded_y_deltas.resize_dirty  (गणनम्)))
      पटिनिवत्त मिच्छा;

    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) deltas_y.arrayZ[i]));
    }

    सचे (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      पटिनिवत्त मिच्छा;

    unsigned ref_गणनम्_2 = 0;
    अत्थाय (तार्किक ref_flag : opt_indices)
       ref_गणनम्_2 += ref_flag;

    सचे (ref_गणनम्_2 == गणनम्) पटिनिवत्त सच्च;

    hb_vector_t<भिन्न> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<भिन्न> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    तार्किक is_comp_glyph_wo_deltas = (is_composite && ref_गणनम्_2 == 0);
    सचे (is_comp_glyph_wo_deltas)
    {
      सचे (unlikely (!opt_deltas_x.resize (गणनम्) ||
                    !opt_deltas_y.resize (गणनम्)))
        पटिनिवत्त मिच्छा;

      opt_indices.arrayZ[0] = सच्च;
      अत्थाय (unsigned i = 1; i < गणनम्; i++)
        opt_indices.arrayZ[i] = मिच्छा;
    }

    hb_vector_t<unsigned वर्ण> &opt_point_data = scratch.opt_point_data.reset ();
    सचे (!compile_point_set (opt_indices, opt_point_data))
      पटिनिवत्त मिच्छा;
    hb_vector_t<unsigned वर्ण> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    सचे (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      पटिनिवत्त मिच्छा;

    hb_vector_t<unsigned वर्ण> &point_data = scratch.point_data.reset ();
    सचे (!compile_point_set (indices, point_data))
      पटिनिवत्त मिच्छा;
    hb_vector_t<unsigned वर्ण> &deltas_data = scratch.deltas_data.reset ();
    सचे (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      पटिनिवत्त मिच्छा;

    सचे (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      सचे (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    पटिनिवत्त !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  स्थिर तार्किक compile_point_set (ध्रुव hb_vector_t<तार्किक> &point_indices,
                                 hb_vector_t<unsigned वर्ण>& compiled_points /* OUT */)
  {
    unsigned अङ्कः_points = 0;
    अत्थाय (तार्किक i : point_indices)
      सचे (i) अङ्कः_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    सचे (!अङ्कः_points) पटिनिवत्त सच्च;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    सचे (अङ्कः_points == indices_length)
      पटिनिवत्त compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned अङ्कः_bytes = 2 + 3 *अङ्कः_points;
    सचे (unlikely (!compiled_points.resize_dirty  (अङ्कः_bytes)))
      पटिनिवत्त मिच्छा;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    सचे (अङ्कः_points < 0x80)
      compiled_points.arrayZ[pos++] = अङ्कः_points;
    अञ्ञथा
    {
      compiled_points.arrayZ[pos++] = ((अङ्कः_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = अङ्कः_points & 0xFF;
    }

    ध्रुव unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_मानम् = 0;
    unsigned अङ्कः_encoded = 0;
    याव (i < indices_length && अङ्कः_encoded < अङ्कः_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      तार्किक use_byte_encoding = मिच्छा;
      तार्किक new_run = सच्च;
      याव (i < indices_length && अङ्कः_encoded < अङ्कः_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        याव (i < indices_length && !point_indices[i])
          i++;

        सचे (i >= indices_length) भङ्ग;

        unsigned cur_मानम् = i;
        unsigned delta = cur_मानम् - last_मानम्;

        सचे (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = मिच्छा;
        }

        सचे (use_byte_encoding && delta > 0xFF)
          भङ्ग;

        सचे (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        अञ्ञथा
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_मानम् = cur_मानम्;
        run_length++;
        अङ्कः_encoded++;
      }

      सचे (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      अञ्ञथा
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    पटिनिवत्त compiled_points.resize_dirty  (pos);
  }

  स्थिर भिन्न infer_delta (भिन्न target_val, भिन्न prev_val, भिन्न next_val, भिन्न prev_delta, भिन्न next_delta)
  {
    सचे (prev_val == next_val)
      पटिनिवत्त (prev_delta == next_delta) ? prev_delta : 0.f;
    अञ्ञथा सचे (target_val <= hb_min (prev_val, next_val))
      पटिनिवत्त (prev_val < next_val) ? prev_delta : next_delta;
    अञ्ञथा सचे (target_val >= hb_max (prev_val, next_val))
      पटिनिवत्त (prev_val > next_val) ? prev_delta : next_delta;

    भिन्न r = (target_val - prev_val) / (next_val - prev_val);
    पटिनिवत्त prev_delta + r * (next_delta - prev_delta);
  }

  स्थिर unsigned पूर्णाङ्क next_index (unsigned पूर्णाङ्क i, unsigned पूर्णाङ्क start, unsigned पूर्णाङ्क end)
  { पटिनिवत्त (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
संरचना TupleVariationData
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (एतत्));
  }

  size_t get_size (unsigned axis_गणनम्_times_2) ध्रुव
  {
    size_t total_size = min_size;
    unsigned गणनम् = tuple_Var_गणनम्.get_गणनम् ();
    ध्रुव TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_गणनम्_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_गणनम्_times_2);
    }

    पटिनिवत्त total_size;
  }

  ध्रुव TupleVariationHeader &get_tuple_var_header (शून्य) ध्रुव
  { पटिनिवत्त StructAfter<TupleVariationHeader> (data); }

  संरचना tuple_iterator_t;
  संरचना tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    गुप्त:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ध्रुव hb_vector_t<तार्किक>*, hb_vector_t<unsigned वर्ण>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ध्रुव hb_vector_t<तार्किक>*, unsigned> point_set_गणनम्_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned वर्ण> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    तार्किक needs_padding = मिच्छा;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    तार्किक is_composite = मिच्छा;

    सार्वजनिक:
    tuple_variations_t () = स्वभाव;
    tuple_variations_t (ध्रुव tuple_variations_t&) = delete;
    tuple_variations_t& operator=(ध्रुव tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = स्वभाव;
    tuple_variations_t& operator=(tuple_variations_t&&) = स्वभाव;
    ~tuple_variations_t () = स्वभाव;

    explicit operator तार्किक () ध्रुव { पटिनिवत्त तार्किक (tuple_vars); }
    unsigned get_var_गणनम् () ध्रुव
    {
      unsigned गणनम् = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      अत्थाय (auto& tuple: tuple_vars)
        सचे (tuple.compiled_deltas) गणनम्++;

      सचे (shared_points_bytes && shared_points_bytes->length)
        गणनम् |= Tuple_Var_गणनम्::SharedPointNumbers;
      पटिनिवत्त गणनम्;
    }

    unsigned get_compiled_byte_size () ध्रुव
    { पटिनिवत्त compiled_byte_size; }

    तार्किक create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_गणनम्,
                                     unsigned point_गणनम्,
                                     तार्किक is_gvar,
                                     ध्रुव hb_map_t *axes_old_index_tag_map,
                                     ध्रुव hb_vector_t<unsigned> &shared_indices,
                                     ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     तार्किक is_composite_glyph = मिच्छा)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<पूर्णाङ्क> deltas_x;
      hb_vector_t<पूर्णाङ्क> deltas_y;
      करोहि
      {
        ध्रुव HBUINT8 *p = iterator.get_serialized_data ();
        unsigned पूर्णाङ्क length = iterator.current_tuple->get_data_size ();
        सचे (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          पटिनिवत्त मिच्छा;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        सचे (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_गणनम् (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          पटिनिवत्त मिच्छा;

	private_indices.reset ();
        तार्किक has_private_points = iterator.current_tuple->has_private_points ();
        ध्रुव HBUINT8 *end = p + length;
        सचे (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          पटिनिवत्त मिच्छा;

        ध्रुव hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        तार्किक apply_to_all = (indices.length == 0);
        unsigned अङ्कः_deltas = apply_to_all ? point_गणनम् : indices.length;

        सचे (unlikely (!deltas_x.resize_dirty  (अङ्कः_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          पटिनिवत्त मिच्छा;

        सचे (is_gvar)
        {
          सचे (unlikely (!deltas_y.resize_dirty  (अङ्कः_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            पटिनिवत्त मिच्छा;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        सचे (unlikely (!var.indices.allocate_from_pool (pool, point_गणनम्) ||
                      !var.deltas_x.allocate_from_pool (pool, point_गणनम्, मिच्छा)))
          पटिनिवत्त मिच्छा;

        सचे (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_गणनम्, मिच्छा)))
          पटिनिवत्त मिच्छा;

        अत्थाय (unsigned i = 0; i < अङ्कः_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          सचे (idx >= point_गणनम्) अनुवर्त;
          var.indices[idx] = सच्च;
          var.deltas_x[idx] = deltas_x[i];
          सचे (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } याव (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      पटिनिवत्त सच्च;
    }

    तार्किक create_from_item_var_data (ध्रुव VarData &var_data,
                                    ध्रुव hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    ध्रुव hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_गणनम्_2,
                                    ध्रुव hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      सचे (&var_data == &Null (VarData))
        पटिनिवत्त सच्च;

      unsigned अङ्कः_regions = var_data.get_region_index_गणनम् ();
      सचे (!tuple_vars.alloc (अङ्कः_regions)) पटिनिवत्त मिच्छा;

      item_गणनम्_2 = inner_map ? inner_map->get_population () : var_data.get_item_गणनम् ();
      सचे (!item_गणनम्_2) पटिनिवत्त सच्च;
      unsigned row_size = var_data.get_row_size ();
      ध्रुव HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      अत्थाय (unsigned r = 0; r < अङ्कः_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        सचे (!tuple.deltas_x.resize_dirty  (item_गणनम्_2) ||
            !tuple.indices.resize_dirty  (item_गणनम्_2))
          पटिनिवत्त मिच्छा;

        अत्थाय (unsigned i = 0; i < item_गणनम्_2; i++)
        {
          tuple.indices.arrayZ[i] = सच्च;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        सचे (region_index >= regions.length) पटिनिवत्त मिच्छा;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      पटिनिवत्त !tuple_vars.in_error ();
    }

    गुप्त:
    स्थिर पूर्णाङ्क _cmp_axis_tag (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
    {
      ध्रुव hb_tag_t *a = (ध्रुव hb_tag_t*) pa;
      ध्रुव hb_tag_t *b = (ध्रुव hb_tag_t*) pb;
      पटिनिवत्त (पूर्णाङ्क)(*a) - (पूर्णाङ्क)(*b);
    }

    तार्किक change_tuple_variations_axis_limits (ध्रुव hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              ध्रुव hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      सचे (!axis_tags.alloc (normalized_axes_location.get_population ()))
        पटिनिवत्त मिच्छा;
      अत्थाय (auto t : normalized_axes_location.कुञ्चिकाः ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      अत्थाय (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        सचे (!normalized_axes_location.has (axis_tag, &axis_limit))
          पटिनिवत्त मिच्छा;
        TripleDistances axis_triple_distances{1.f, 1.f};
        सचे (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        अत्थाय (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          सचे (!out) अनुवर्त;

          unsigned new_len = new_vars.length + out.length;

          सचे (unlikely (!new_vars.alloc (new_len, मिच्छा)))
            पटिनिवत्त मिच्छा;

          अत्थाय (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      पटिनिवत्त सच्च;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    तार्किक merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      सचे (unlikely (!new_vars.alloc (tuple_vars.length)))
	पटिनिवत्त मिच्छा;
      hb_hashmap_t<ध्रुव hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      अत्थाय (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        सचे (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          सचे (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            पटिनिवत्त मिच्छा;
          अनुवर्त;
        }

        unsigned *idx;
        सचे (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        अञ्ञथा
        {
	  auto *new_var = new_vars.push ();
	  सचे (unlikely (new_vars.in_error ()))
	    पटिनिवत्त मिच्छा;
          hb_swap (*new_var, var);
          सचे (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            पटिनिवत्त मिच्छा;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      पटिनिवत्त सच्च;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    तार्किक compile_all_point_sets ()
    {
      अत्थाय (ध्रुव auto& tuple: tuple_vars)
      {
        ध्रुव hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        सचे (point_data_map.has (points_set))
        {
          unsigned *गणनम्;
          सचे (unlikely (!point_set_गणनम्_map.has (points_set, &गणनम्) ||
                        !point_set_गणनम्_map.set (points_set, (*गणनम्) + 1)))
            पटिनिवत्त मिच्छा;
          अनुवर्त;
        }

        hb_vector_t<unsigned वर्ण> compiled_point_data;
        सचे (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          पटिनिवत्त मिच्छा;

        सचे (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_गणनम्_map.set (points_set, 1))
          पटिनिवत्त मिच्छा;
      }
      पटिनिवत्त सच्च;
    }

    /* find shared points set which saves most bytes */
    शून्य find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      अत्थाय (ध्रुव auto& _ : point_data_map.iter_ref ())
      {
        ध्रुव hb_vector_t<तार्किक>* points_set = _.first;
        unsigned data_length = _.second.length;
        सचे (!data_length) अनुवर्त;
        unsigned *गणनम्;
        सचे (unlikely (!point_set_गणनम्_map.has (points_set, &गणनम्) ||
                      *गणनम् <= 1))
        {
          shared_points_bytes = nullptr;
          पटिनिवत्त;
        }

        unsigned saved_bytes = data_length * ((*गणनम्) -1);
        सचे (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    तार्किक calc_inferred_deltas (ध्रुव contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      अत्थाय (tuple_delta_t& var : tuple_vars)
        सचे (!var.calc_inferred_deltas (contour_points, scratch))
          पटिनिवत्त मिच्छा;

      पटिनिवत्त सच्च;
    }

    तार्किक iup_optimize (ध्रुव contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      अत्थाय (tuple_delta_t& var : tuple_vars)
      {
        सचे (!var.optimize (contour_points, is_composite, scratch))
          पटिनिवत्त मिच्छा;
      }
      पटिनिवत्त सच्च;
    }

    सार्वजनिक:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    शून्य cull_unreachable (ध्रुव hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      अत्थाय (unsigned i = 0; i < tuple_vars.length; i++)
      {
	सचे (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  अनुवर्त;
	सचे (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    तार्किक instantiate (ध्रुव hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      ध्रुव hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      तार्किक optimize = मिच्छा)
    {
      सचे (!tuple_vars) पटिनिवत्त सच्च;
      सचे (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        पटिनिवत्त मिच्छा;
      /* compute inferred deltas only for gvar */
      सचे (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	सचे (!calc_inferred_deltas (*contour_points, scratch))
	  पटिनिवत्त मिच्छा;
      }

      /* if iup delta opt is on, contour_points can't be null */
      सचे (optimize && !contour_points)
        पटिनिवत्त मिच्छा;

      सचे (!merge_tuple_variations (optimize ? contour_points : nullptr))
        पटिनिवत्त मिच्छा;

      सचे (optimize && !iup_optimize (*contour_points, scratch)) पटिनिवत्त मिच्छा;
      पटिनिवत्त !tuple_vars.in_error ();
    }

    तार्किक compile_bytes (ध्रुव hb_map_t& axes_index_map,
                        ध्रुव hb_map_t& axes_old_index_tag_map,
                        तार्किक use_shared_points,
                        तार्किक is_gvar = मिच्छा,
                        ध्रुव hb_hashmap_t<ध्रुव hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      सचे (!tuple_vars)
        पटिनिवत्त सच्च;

      // compile points set and store data in hashmap
      सचे (!compile_all_point_sets ())
        पटिनिवत्त मिच्छा;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      सचे (use_shared_points)
      {
        find_shared_points ();
        सचे (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<पूर्णाङ्क> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      अत्थाय (auto& tuple: tuple_vars)
      {
        ध्रुव hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        hb_vector_t<unsigned वर्ण> *points_data;
        सचे (unlikely (!point_data_map.has (points_set, &points_data)))
          पटिनिवत्त मिच्छा;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        सचे (!points_data->length)
          अनुवर्त;
        सचे (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          पटिनिवत्त मिच्छा;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        सचे (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          पटिनिवत्त मिच्छा;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      सचे (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = सच्च;
        compiled_byte_size += 1;
      }

      पटिनिवत्त सच्च;
    }

    तार्किक serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) ध्रुव
    {
      TRACE_SERIALIZE (एतत्);
      अत्थाय (ध्रुव auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        सचे (c->in_error ()) return_trace (मिच्छा);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (सच्च);
    }

    तार्किक serialize_var_data (hb_serialize_context_t *c, तार्किक is_gvar) ध्रुव
    {
      TRACE_SERIALIZE (एतत्);
      सचे (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      अत्थाय (ध्रुव auto& tuple: tuple_vars)
      {
        ध्रुव hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        hb_vector_t<unsigned वर्ण> *point_data;
        सचे (!point_data_map.has (points_set, &point_data))
          return_trace (मिच्छा);

        सचे (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        सचे (c->in_error ()) return_trace (मिच्छा);
      }

      /* padding for gvar */
      सचे (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        सचे (!c->embed (pad)) return_trace (मिच्छा);
      }
      return_trace (सच्च);
    }
  };

  संरचना tuple_iterator_t
  {
    unsigned get_axis_गणनम् () ध्रुव { पटिनिवत्त axis_गणनम्; }

    शून्य init (hb_bytes_t var_data_bytes_, unsigned पूर्णाङ्क axis_गणनम्_3, ध्रुव शून्य *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_वामः = var_data->tuple_Var_गणनम्.get_गणनम् ();
      axis_गणनम् = axis_गणनम्_3;
      axis_गणनम्_times_2 = axis_गणनम्_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    तार्किक get_shared_indices (hb_vector_t<unsigned पूर्णाङ्क> &shared_indices /* OUT */)
    {
      सचे (var_data->has_shared_point_numbers ())
      {
        ध्रुव HBUINT8 *base = &(table_base+var_data->data);
        ध्रुव HBUINT8 *p = base;
        सचे (!decompile_points (p, shared_indices, (ध्रुव HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) पटिनिवत्त मिच्छा;
        data_offset = p - base;
      }
      पटिनिवत्त सच्च;
    }

    तार्किक is_valid ()
    {
      सचे (unlikely (tuples_वामः <= 0))
	पटिनिवत्त मिच्छा;

      current_tuple_size = TupleVariationHeader::min_size;
      सचे (unlikely (!var_data_bytes.check_end ((ध्रुव वर्ण *) current_tuple + current_tuple_size)))
	पटिनिवत्त मिच्छा;

      current_tuple_size = current_tuple->get_size (axis_गणनम्_times_2);
      सचे (unlikely (!var_data_bytes.check_end ((ध्रुव वर्ण *) current_tuple + current_tuple_size)))
	पटिनिवत्त मिच्छा;

      पटिनिवत्त सच्च;
    }

    HB_ALWAYS_INLINE
    तार्किक move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_वामः--;
      पटिनिवत्त is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ध्रुव HBUINT8 *get_serialized_data () ध्रुव
    { पटिनिवत्त &(table_base+var_data->data) + data_offset; }

    गुप्त:
    signed tuples_वामः;
    ध्रुव TupleVariationData *var_data;
    unsigned पूर्णाङ्क axis_गणनम्;
    unsigned पूर्णाङ्क axis_गणनम्_times_2;
    unsigned पूर्णाङ्क data_offset;
    unsigned पूर्णाङ्क current_tuple_size;
    ध्रुव शून्य *table_base;

    सार्वजनिक:
    hb_bytes_t var_data_bytes;
    ध्रुव TupleVariationHeader *current_tuple;
  };

  स्थिर तार्किक get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_गणनम्,
                                  ध्रुव शून्य *table_base,
                                  hb_vector_t<unsigned पूर्णाङ्क> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_गणनम्, table_base);
    सचे (!iterator->get_shared_indices (shared_indices))
      पटिनिवत्त मिच्छा;
    पटिनिवत्त iterator->is_valid ();
  }

  तार्किक has_shared_point_numbers () ध्रुव { पटिनिवत्त tuple_Var_गणनम्.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  तार्किक cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_गणनम्,
			      hb_array_t<ध्रुव F2DOT14> shared_tuples,
			      ध्रुव hb_map_t *axes_old_index_tag_map,
			      ध्रुव hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<वर्ण> &out /* OUT */,
			      तार्किक *changed /* OUT */) ध्रुव
  {
    *changed = मिच्छा;

    hb_vector_t<unsigned पूर्णाङ्क> shared_indices;
    tuple_iterator_t iterator;
    सचे (!get_tuple_iterator (var_data_bytes, axis_गणनम्, एतत्,
			     shared_indices, &iterator))
      पटिनिवत्त सच्च; /* no tuples or malformed: keep original */

    ध्रुव वर्ण *bytes_start = var_data_bytes.arrayZ;
    ध्रुव वर्ण *bytes_end = bytes_start + var_data_bytes.length;
    ध्रुव वर्ण *serialized_base = (ध्रुव वर्ण *) &(एतत्+data);
    ध्रुव वर्ण *shared_points_end = (ध्रुव वर्ण *) iterator.get_serialized_data ();
    सचे (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      पटिनिवत्त सच्च;

    संरचना kept_tuple_t
    {
      ध्रुव वर्ण *header;
      unsigned header_size;
      ध्रुव वर्ण *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    करोहि
    {
      ध्रुव TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_गणनम् * 2);
      unsigned data_size = header->get_data_size ();
      ध्रुव वर्ण *tuple_data = (ध्रुव वर्ण *) iterator.get_serialized_data ();
      सचे (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	पटिनिवत्त सच्च; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      सचे (!header->unpack_axis_tuples (axis_गणनम्, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	पटिनिवत्त सच्च;

      total++;
      सचे (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ध्रुव वर्ण *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } याव (iterator.move_to_next ());

    सचे (unlikely (kept.in_error ())) पटिनिवत्त मिच्छा;
    सचे (kept.length == total) पटिनिवत्त सच्च; /* nothing to cull */

    *changed = सच्च;
    सचे (!kept.length) पटिनिवत्त सच्च; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    सचे (unlikely (!out.resize (new_size)))
      पटिनिवत्त मिच्छा;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_गणनम् = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    वर्ण *p = out.arrayZ + min_size;
    अत्थाय (ध्रुव auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    अत्थाय (ध्रुव auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    पटिनिवत्त सच्च;
  }

  स्थिर तार्किक decompile_points (ध्रुव HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned पूर्णाङ्क> &points /* OUT */,
				ध्रुव HBUINT8 *end)
  {
    गणना packed_point_flag_t
    {
      POINTS_ARE_शब्दाः     = 0x80,
      POINT_RUN_गणनम्_MASK = 0x7F
    };

    सचे (unlikely (p + 1 > end)) पटिनिवत्त मिच्छा;

    unsigned गणनम् = *p++;
    सचे (गणनम् & POINTS_ARE_शब्दाः)
    {
      सचे (unlikely (p + 1 > end)) पटिनिवत्त मिच्छा;
      गणनम् = ((गणनम् & POINT_RUN_गणनम्_MASK) << 8) | *p++;
    }
    सचे (unlikely (!points.resize_dirty  (गणनम्))) पटिनिवत्त मिच्छा;

    unsigned n = 0;
    unsigned i = 0;
    याव (i < गणनम्)
    {
      सचे (unlikely (p + 1 > end)) पटिनिवत्त मिच्छा;
      unsigned control = *p++;
      unsigned run_गणनम् = (control & POINT_RUN_गणनम्_MASK) + 1;
      unsigned stop = i + run_गणनम्;
      सचे (unlikely (stop > गणनम्)) पटिनिवत्त मिच्छा;
      सचे (control & POINTS_ARE_शब्दाः)
      {
        सचे (unlikely (p + run_गणनम् * HBUINT16::static_size > end)) पटिनिवत्त मिच्छा;
        अत्थाय (; i < stop; i++)
        {
          n += *(ध्रुव HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      अञ्ञथा
      {
        सचे (unlikely (p + run_गणनम् > end)) पटिनिवत्त मिच्छा;
        अत्थाय (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    पटिनिवत्त सच्च;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  स्थिर तार्किक decompile_deltas (ध्रुव HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				ध्रुव HBUINT8 *end,
				तार्किक consume_all = मिच्छा,
				unsigned start = 0)
  {
    पटिनिवत्त Tuple_मानानि::decompile (p, deltas, end, consume_all, start);
  }

  तार्किक has_data () ध्रुव { पटिनिवत्त tuple_Var_गणनम्; }

  तार्किक decompile_tuple_variations (unsigned point_गणनम्,
                                   तार्किक is_gvar,
                                   tuple_iterator_t iterator,
                                   ध्रुव hb_map_t *axes_old_index_tag_map,
                                   ध्रुव hb_vector_t<unsigned> &shared_indices,
                                   ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   तार्किक is_composite_glyph = मिच्छा) ध्रुव
  {
    पटिनिवत्त tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_गणनम्,
                                                        point_गणनम्, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  तार्किक is_gvar,
                  ध्रुव tuple_variations_t& tuple_variations) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    /* empty tuple variations, just return and skip serialization. */
    सचे (!tuple_variations) return_trace (सच्च);

    auto *out = c->start_embed (एतत्);
    सचे (unlikely (!c->extend_min (out))) return_trace (मिच्छा);

    सचे (!c->check_assign (out->tuple_Var_गणनम्, tuple_variations.get_var_गणनम् (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (मिच्छा);

    unsigned total_header_len = 0;

    सचे (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (मिच्छा);

    unsigned data_offset = min_size + total_header_len;
    सचे (!is_gvar) data_offset += 4;
    सचे (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (मिच्छा);

    पटिनिवत्त tuple_variations.serialize_var_data (c, is_gvar);
  }

  रक्षित:
  संरचना Tuple_Var_गणनम् : HBUINT16
  {
    friend संरचना tuple_variations_t;
    तार्किक has_shared_point_numbers () ध्रुव { पटिनिवत्त ((*एतत्) & SharedPointNumbers); }
    unsigned पूर्णाङ्क get_गणनम् () ध्रुव { पटिनिवत्त (*एतत्) & गणनम्_Mask; }
    Tuple_Var_गणनम्& operator = (uint16_t i) { HBUINT16::operator= (i); पटिनिवत्त *एतत्; }
    explicit operator तार्किक () ध्रुव { पटिनिवत्त get_गणनम् (); }

    रक्षित:
    गणना Flags
    {
      SharedPointNumbers= 0x8000u,
      गणनम्_Mask         = 0x0FFFu
    };
    सार्वजनिक:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_गणनम् tuple_Var_गणनम्;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
संरचना item_variations_t
{
  using region_t = ध्रुव hb_hashmap_t<hb_tag_t, Triple>*;
  गुप्त:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_data_अङ्कः_rows;

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
  hb_vector_t<hb_vector_t<पूर्णाङ्क>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  तार्किक has_long = मिच्छा;

  सार्वजनिक:
  तार्किक has_long_word () ध्रुव
  { पटिनिवत्त has_long; }

  ध्रुव hb_vector_t<region_t>& get_region_list () ध्रुव
  { पटिनिवत्त region_list; }

  ध्रुव hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () ध्रुव
  { पटिनिवत्त encodings; }

  ध्रुव hb_map_t& get_varidx_map () ध्रुव
  { पटिनिवत्त varidx_map; }

  तार्किक add_vardata_encoding_for_testing (hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क>*> &&rows,
                                         unsigned अङ्कः_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), अङ्कः_cols));
    पटिनिवत्त !encodings.in_error ();
  }

  तार्किक compile_varidx_map_for_testing (ध्रुव hb_hashmap_t<unsigned, ध्रुव hb_vector_t<पूर्णाङ्क>*>& front_mapping)
  { पटिनिवत्त compile_varidx_map (front_mapping); }

  तार्किक instantiate (ध्रुव ItemVariationStore& varStore,
                    ध्रुव hb_subset_plan_t *plan,
                    तार्किक optimize=सच्च,
                    तार्किक use_no_variation_idx=सच्च,
                    ध्रुव hb_array_t <ध्रुव hb_inc_bimap_t> inner_maps = hb_array_t<ध्रुव hb_inc_bimap_t> (),
                    तार्किक layout_conditions = मिच्छा)
  {
    सचे (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      पटिनिवत्त मिच्छा;
    /* avar2 partial instancing: cull unreachable regions. */
    सचे (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      अत्थाय (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    सचे (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      पटिनिवत्त मिच्छा;
    सचे (layout_conditions &&
        (!instantiate_condition_मानानि (plan, varStore) || !build_region_list ()))
      पटिनिवत्त मिच्छा;
    पटिनिवत्त as_item_varstore (optimize, use_no_variation_idx);
  }

  तार्किक instantiate_condition_मानानि (ध्रुव hb_subset_plan_t *plan,
                                      ध्रुव ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_मानम्_map;
    condition_map.clear ();
    सचे (!plan->layout_condition_मानानि) पटिनिवत्त सच्च;
    using condition_मानम्_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, पूर्णाङ्क>>;
    hb_vector_t<condition_मानम्_t> conditions;
    अत्थाय (auto condition : plan->layout_condition_मानानि.iter ())
      conditions.push (condition);
    सचे (unlikely (conditions.in_error ())) पटिनिवत्त मिच्छा;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ध्रुव condition_मानम्_t &a, ध्रुव condition_मानम्_t &b) {
      सचे (a.second.first != b.second.first)
        पटिनिवत्त a.second.first < b.second.first ? -1 : 1;
      पटिनिवत्त a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    अत्थाय (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      सचे (plan->gdef_varstore_inner_maps[i].get_population ())
        सचे (!outer_map.set (i, new_outer++)) पटिनिवत्त मिच्छा;
    अत्थाय (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      द्विगुण value = condition.second.second +
                     varStore.get_delta<द्विगुण> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      सचे (!outer_map.has (old_outer))
        अनुवर्त;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      सचे (outer >= vars.length || inner >= var_data_अङ्कः_rows[outer])
        अनुवर्त;

      द्विगुण maximum = fabs ((द्विगुण) value);
      तार्किक variable = मिच्छा;
      अत्थाय (ध्रुव tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        सचे (unlikely (inner >= tuple.deltas_x.length)) पटिनिवत्त मिच्छा;
        भिन्न delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((द्विगुण) delta));
      }
      सचे (!variable)
      {
        सचे (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), पूर्णाङ्क (value > 0))))
          पटिनिवत्त मिच्छा;
        अनुवर्त;
      }

      द्विगुण scale = 1.;
      याव (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      याव (maximum * scale * 2 <= INT_MAX)
      {
        तार्किक integral = (द्विगुण) value * scale == round ((द्विगुण) value * scale);
        अत्थाय (ध्रुव tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (द्विगुण) tuple.deltas_x[inner] * scale ==
                      round ((द्विगुण) tuple.deltas_x[inner] * scale);
        सचे (integral) भङ्ग;
        scale *= 2;
      }
      पूर्णाङ्क default_मानम् = (पूर्णाङ्क) round ((द्विगुण) value * scale);
      सचे (scale != 1. || default_मानम् < -32768 || default_मानम् > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        अत्थाय (ध्रुव tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          सचे (!tuple.deltas_x[inner]) अनुवर्त;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (सच्च);
          copy.deltas_x.push ((भिन्न) round ((द्विगुण) tuple.deltas_x[inner] * scale));
          सचे (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) पटिनिवत्त मिच्छा;
          private_row.tuple_vars.push (std::move (copy));
        }
        सचे (unlikely (vars.length >= 0xFFFFu)) पटिनिवत्त मिच्छा;
        सचे (unlikely (private_row.tuple_vars.in_error ())) पटिनिवत्त मिच्छा;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_अङ्कः_rows.push (1);
        सचे (unlikely (vars.in_error () || var_data_अङ्कः_rows.in_error ())) पटिनिवत्त मिच्छा;
        सचे (default_मानम् < -32768 || default_मानम् > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          पूर्णाङ्क bias = default_मानम् - hb_clamp (default_मानम्, -32768, 32767);
          default_मानम् -= bias;
          पूर्णाङ्क उच्चः = bias / 65536 * 65536;
          पूर्णाङ्क parts[] = {उच्चः, bias - उच्चः};
          अत्थाय (पूर्णाङ्क part : parts)
          {
            सचे (!part) अनुवर्त;
            unsigned गणनम् = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            ध्रुव auto &tuples = vars[idx >> 16].tuple_vars;
            सचे (unlikely (tuples.in_error () || tuples.length != गणनम् + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) पटिनिवत्त मिच्छा;
          }
        }
      }
      सचे (!condition_map.set (condition.first, hb_pair (idx, default_मानम्)))
        पटिनिवत्त मिच्छा;
    }
    पटिनिवत्त !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  तार्किक create_from_item_varstore (ध्रुव ItemVariationStore& varStore,
                                  ध्रुव hb_map_t& axes_old_index_tag_map,
                                  ध्रुव hb_array_t <ध्रुव hb_inc_bimap_t> inner_maps = hb_array_t<ध्रुव hb_inc_bimap_t> ())
  {
    ध्रुव VarRegionList& regionList = varStore.get_region_list ();
    सचे (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      पटिनिवत्त मिच्छा;

    unsigned अङ्कः_var_data = varStore.get_sub_table_गणनम् ();
    सचे (inner_maps && inner_maps.length != अङ्कः_var_data) पटिनिवत्त मिच्छा;
    सचे (!vars.alloc (अङ्कः_var_data) ||
        !var_data_अङ्कः_rows.alloc (अङ्कः_var_data)) पटिनिवत्त मिच्छा;

    अत्थाय (unsigned i = 0; i < अङ्कः_var_data; i++)
    {
      सचे (inner_maps && !inner_maps.arrayZ[i].get_population ())
          अनुवर्त;
      tuple_variations_t var_data_tuples;
      unsigned item_गणनम्_2 = 0;
      सचे (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_गणनम्_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        पटिनिवत्त मिच्छा;

      var_data_अङ्कः_rows.push (item_गणनम्_2);
      vars.push (std::move (var_data_tuples));
    }
    पटिनिवत्त !vars.in_error () && !var_data_अङ्कः_rows.in_error () && vars.length == var_data_अङ्कः_rows.length;
  }

  तार्किक instantiate_tuple_vars (ध्रुव hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               ध्रुव hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               तार्किक build_regions = सच्च)
  {
    optimize_scratch_t scratch;
    अत्थाय (tuple_variations_t& tuple_vars : vars)
      सचे (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        पटिनिवत्त मिच्छा;

    पटिनिवत्त !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_गणनम्_2)
  {
    vars.push (tuple_variations_t ());
    var_data_अङ्कः_rows.push (item_गणनम्_2);
    पटिनिवत्त vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_गणनम् (unsigned outer) ध्रुव
  { पटिनिवत्त outer < var_data_अङ्कः_rows.length ? var_data_अङ्कः_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    सचे (unlikely (outer >= vars.length ||
                  inner >= var_data_अङ्कः_rows[outer] ||
                  var_data_अङ्कः_rows[outer] >= 0xFFFFu))
      पटिनिवत्त (unsigned) -1;
    अत्थाय (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      सचे (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        पटिनिवत्त (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      सचे (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      सचे (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        पटिनिवत्त (unsigned) -1;
    }
    पटिनिवत्त var_data_अङ्कः_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  शून्य add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, पूर्णाङ्क delta, unsigned item_गणनम्_2)
  {
    सचे (outer >= vars.length) पटिनिवत्त;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    सचे (!tuple.indices.resize (item_गणनम्_2) ||
        !tuple.deltas_x.resize (item_गणनम्_2)) पटिनिवत्त;
    अत्थाय (unsigned i = 0; i < item_गणनम्_2; i++)
    {
      tuple.indices.arrayZ[i] = सच्च;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (भिन्न) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  तार्किक build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    अत्थाय (ध्रुव tuple_variations_t& sub_table : vars)
    {
      अत्थाय (ध्रुव tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        सचे (!used_regions.has (r))
        {
          तार्किक all_zeros = सच्च;
          अत्थाय (भिन्न d : tuple.deltas_x)
          {
            पूर्णाङ्क delta = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) d));
            सचे (delta != 0)
            {
              all_zeros = मिच्छा;
              भङ्ग;
            }
          }
          सचे (!all_zeros)
          {
            सचे (!used_regions.set (r, 1))
              पटिनिवत्त मिच्छा;
          }
        }
        सचे (all_regions.has (r))
          अनुवर्त;
        सचे (!all_regions.set (r, 1))
          पटिनिवत्त मिच्छा;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    सचे (!all_regions || !all_unique_regions) पटिनिवत्त सच्च;

    सचे (!region_list.alloc (all_regions.get_population ()))
      पटिनिवत्त मिच्छा;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    अत्थाय (ध्रुव auto& r : orig_region_list)
    {
      सचे (!all_regions.has (&r) || !used_regions.has (&r))
        अनुवर्त;

      region_list.push (&r);
      सचे (!region_map.set (&r, idx))
        पटिनिवत्त मिच्छा;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    अत्थाय (ध्रुव auto& r: all_unique_regions)
    {
      सचे (!all_regions.has (r) || !used_regions.has (r))
        अनुवर्त;
      region_list.push (r);
      सचे (!region_map.set (r, idx))
        पटिनिवत्त मिच्छा;
      all_regions.del (r);
      idx++;
    }
    पटिनिवत्त (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  संरचना combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = स्वभाव;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      प्रतिज्ञा (gain < 0xFFFFFF);
      प्रतिज्ञा (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    तार्किक operator < (ध्रुव combined_gain_idx_tuple_t& o)
    {
      पटिनिवत्त encoded < o.encoded;
    }

    तार्किक operator <= (ध्रुव combined_gain_idx_tuple_t& o)
    {
      पटिनिवत्त encoded <= o.encoded;
    }

    unsigned idx_1 () ध्रुव { पटिनिवत्त (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () ध्रुव { पटिनिवत्त encoded & 0xFFFFF; };
  };

  तार्किक as_item_varstore (तार्किक optimize=सच्च, तार्किक use_no_variation_idx=सच्च)
  {
    /* return true if no variation data */
    सचे (!region_list) पटिनिवत्त सच्च;
    unsigned अङ्कः_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    अत्थाय (unsigned major = 0; major < var_data_अङ्कः_rows.length; major++)
      total_rows += var_data_अङ्कः_rows[major];

    सचे (!delta_rows.resize (total_rows)) पटिनिवत्त मिच्छा;
    /* init all rows to [0]*num_cols */
    अत्थाय (unsigned i = 0; i < total_rows; i++)
      सचे (!(delta_rows[i].resize (अङ्कः_cols))) पटिनिवत्त मिच्छा;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, ध्रुव hb_vector_t<पूर्णाङ्क>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क> *> major_rows;
    hb_hashmap_t<ध्रुव hb_vector_t<पूर्णाङ्क>*, unsigned> delta_rows_map;
    अत्थाय (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ध्रुव tuple_variations_t& tuples = vars[major];
      unsigned अङ्कः_rows = var_data_अङ्कः_rows[major];

      सचे (!अङ्कः_rows) अनुवर्त;

      अत्थाय (ध्रुव tuple_delta_t& tuple: tuples.tuple_vars)
      {
        सचे (tuple.deltas_x.length != अङ्कः_rows)
          पटिनिवत्त मिच्छा;

        /* skip unused regions */
        unsigned *col_idx;
        सचे (!region_map.has (&(tuple.axis_tuples), &col_idx))
          अनुवर्त;

        अत्थाय (unsigned i = 0; i < अङ्कः_rows; i++)
        {
          पूर्णाङ्क rounded_delta = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) tuple.deltas_x[i]));
          पूर्णाङ्क &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          सचे (unlikely (sum < INT_MIN || sum > INT_MAX)) पटिनिवत्त मिच्छा;
          delta = (पूर्णाङ्क) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      अत्थाय (unsigned minor = 0; minor < अङ्कः_rows; minor++)
      {
	ध्रुव hb_vector_t<पूर्णाङ्क>& row = delta_rows[start_row + minor];
	सचे (use_no_variation_idx)
	{
	  तार्किक all_zeros = सच्च;
	  अत्थाय (पूर्णाङ्क delta : row)
	  {
	    सचे (delta != 0)
	    {
	      all_zeros = मिच्छा;
	      भङ्ग;
	    }
	  }
	  सचे (all_zeros)
	    अनुवर्त;
	}

	सचे (!front_mapping.set ((major<<16) + minor, &row))
	  पटिनिवत्त मिच्छा;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	सचे (optimize)
	{
	  सचे (delta_rows_map.has (&row))
	    अनुवर्त;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      सचे (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        पटिनिवत्त मिच्छा;
      सचे (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), अङ्कः_cols);
	सचे (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  पटिनिवत्त मिच्छा;
      }

      start_row += अङ्कः_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    सचे (!optimize)
    {
      encodings = std::move (encoding_objs);
      पटिनिवत्त !encodings.in_error ();
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
    hb_vector_t<item_t> queue_वस्तूनि;
    unsigned अङ्कः_todos = encoding_objs.length;
    अत्थाय (unsigned i = 0; i < अङ्कः_todos; i++)
    {
      अत्थाय (unsigned j = i + 1; j < अङ्कः_todos; j++)
      {
        पूर्णाङ्क combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        सचे (combining_gain > 0)
          queue_वस्तूनि.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_वस्तूनि));

    hb_bit_set_t removed_todo_idxes;
    याव (queue)
    {
      auto t = queue.निष्कासय_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      सचे (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        अनुवर्त;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      सचे (unlikely (removed_todo_idxes.in_error ())) पटिनिवत्त मिच्छा;

      encoding.merge (other_encoding);
      सचे (unlikely (encoding.in_error ())) पटिनिवत्त मिच्छा;

      अत्थाय (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        सचे (removed_todo_idxes.has (idx)) अनुवर्त;

        ध्रुव delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        सचे (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          अत्थाय (ध्रुव auto& row : obj.वस्तूनि)
            encoding.add_row (row);
          सचे (unlikely (encoding.in_error ())) पटिनिवत्त मिच्छा;

          removed_todo_idxes.add (idx);
          सचे (unlikely (removed_todo_idxes.in_error ())) पटिनिवत्त मिच्छा;
          अनुवर्त;
        }

        पूर्णाङ्क combined_gain = encoding.gain_from_merging (obj);
        सचे (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      सचे (unlikely (encoding_objs.in_error ())) पटिनिवत्त मिच्छा;
    }

    पूर्णाङ्क अङ्कः_final_encodings = (पूर्णाङ्क) encoding_objs.length - (पूर्णाङ्क) removed_todo_idxes.get_population ();
    सचे (अङ्कः_final_encodings <= 0) पटिनिवत्त मिच्छा;

    सचे (!encodings.alloc (अङ्कः_final_encodings)) पटिनिवत्त मिच्छा;
    अत्थाय (unsigned i = 0; i < encoding_objs.length; i++)
    {
      सचे (removed_todo_idxes.has (i)) अनुवर्त;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    पटिनिवत्त compile_varidx_map (front_mapping);
  }

  गुप्त:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  तार्किक compile_varidx_map (ध्रुव hb_hashmap_t<unsigned, ध्रुव hb_vector_t<पूर्णाङ्क>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ध्रुव hb_vector_t<पूर्णाङ्क>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    अत्थाय (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      सचे (encoding.is_empty () || encoding.in_error ())
        पटिनिवत्त मिच्छा;

      unsigned अङ्कः_rows = encoding.वस्तूनि.length;
      unsigned अङ्कः_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.वस्तूनि.qsort (_cmp_row);

      अत्थाय (unsigned start = 0; start < अङ्कः_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (अङ्कः_rows - start, 0xFFFFu);
        hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क>*> rows;

        सचे (!rows.alloc (chunk_len))
          पटिनिवत्त मिच्छा;

        unsigned major = split_encodings.length;
        अत्थाय (unsigned minor = 0; minor < chunk_len; minor++)
        {
          ध्रुव hb_vector_t<पूर्णाङ्क> *row = encoding.वस्तूनि.arrayZ[start + minor];
          rows.push (row);
          सचे (!back_mapping.set (row, (major << 16) + minor))
            पटिनिवत्त मिच्छा;
        }

        auto *split = split_encodings.push (std::move (rows), अङ्कः_cols);
        सचे (unlikely (split_encodings.in_error () || split->in_error ()))
          पटिनिवत्त मिच्छा;
      }
    }

    encodings = std::move (split_encodings);
    सचे (encodings.in_error () || back_mapping.in_error ())
      पटिनिवत्त मिच्छा;

    अत्थाय (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      सचे (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      अञ्ञथा
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    पटिनिवत्त !varidx_map.in_error ();
  }

  स्थिर पूर्णाङ्क _cmp_row (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ध्रुव hb_vector_t<पूर्णाङ्क>** a = (ध्रुव hb_vector_t<पूर्णाङ्क>**) pa;
    ध्रुव hb_vector_t<पूर्णाङ्क>** b = (ध्रुव hb_vector_t<पूर्णाङ्क>**) pb;

    अत्थाय (unsigned i = 0; i < (*b)->length; i++)
    {
      पूर्णाङ्क va = (*a)->arrayZ[i];
      पूर्णाङ्क vb = (*b)->arrayZ[i];
      सचे (va != vb)
        पटिनिवत्त va < vb ? -1 : 1;
    }
    पटिनिवत्त 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

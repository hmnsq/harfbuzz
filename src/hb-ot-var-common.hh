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
  मित्र संरचना tuple_delta_t;
  माप_प्रकार get_size (अचिन्हित axis_गिनती_times_2) ध्रुव
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    अचिन्हित ti = tupleIndex;
    यदि (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      अचिन्हित गिनती = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      प्रत्यागम hb_unsigned_mul_add_saturate (गिनती, axis_गिनती_times_2, min_size);
    }
    प्रत्यागम min_size;
  }

  अचिन्हित get_data_size () ध्रुव { प्रत्यागम varDataSize; }

  ध्रुव TupleVariationHeader &get_next (अचिन्हित axis_गिनती_times_2) ध्रुव
  { प्रत्यागम StructAtOffset<TupleVariationHeader> (एतत्, get_size (axis_गिनती_times_2)); }

  तार्किक unpack_axis_tuples (अचिन्हित axis_गिनती,
                           ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
                           ध्रुव hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ध्रुव
  {
    ध्रुव F2DOT14 *peak_tuple = nullptr;
    यदि (has_peak ())
      peak_tuple = get_peak_tuple (axis_गिनती);
    अन्यथा
    {
      अचिन्हित पूर्णाङ्क index = get_index ();
      यदि (unlikely ((index + 1) * axis_गिनती > shared_tuples.length))
        प्रत्यागम असत्य;
      peak_tuple = shared_tuples.sub_array (axis_गिनती * index, axis_गिनती).arrayZ;
    }

    ध्रुव F2DOT14 *start_tuple = nullptr;
    ध्रुव F2DOT14 *end_tuple = nullptr;
    तार्किक has_interm = has_intermediate ();

    यदि (has_interm)
    {
      start_tuple = get_start_tuple (axis_गिनती);
      end_tuple = get_end_tuple (axis_गिनती);
    }

    प्रति (अचिन्हित i = 0; i < axis_गिनती; i++)
    {
      भिन्न peak = peak_tuple[i].to_float ();
      यदि (peak == 0.f) अनुवर्त;

      hb_tag_t *axis_tag;
      यदि (!axes_old_index_tag_map->has (i, &axis_tag))
        प्रत्यागम असत्य;

      भिन्न start, end;
      यदि (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      अन्यथा
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    प्रत्यागम सत्य;
  }

  HB_ALWAYS_INLINE
  भिन्न calculate_scalar (hb_array_t<ध्रुव पूर्णाङ्क> coords, अचिन्हित पूर्णाङ्क coord_गिनती,
			   ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ध्रुव
  {
    अचिन्हित tuple_index = tupleIndex;
    अचिन्हित पूर्णाङ्क index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    तार्किक plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    यदि (likely (plain_shared_tuple))
    {
      भिन्न scalar;
      यदि (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	प्रत्यागम scalar;
    }

    ध्रुव F2DOT14 *peak_tuple;

    तार्किक has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    यदि (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_गिनती);
      shared_tuple_scalar_cache = nullptr;
    }
    अन्यथा
    {
      भिन्न scalar;
      यदि (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        यदि (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	अन्यथा
	  प्रत्यागम scalar;
      }

      यदि (unlikely ((index + 1) * coord_गिनती > shared_tuples.length))
        प्रत्यागम 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_गिनती * index);

    }

    ध्रुव F2DOT14 *start_tuple = nullptr;
    ध्रुव F2DOT14 *end_tuple = nullptr;

    यदि (has_interm)
    {
      start_tuple = get_start_tuple (coord_गिनती);
      end_tuple = get_end_tuple (coord_गिनती);
    }

    भिन्न scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#यदि HB_FAST_अंक_ACCESS
    तार्किक skip = coord_गिनती >= 16;
#endif
#endif
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < coord_गिनती; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#यदि HB_FAST_अंक_ACCESS
      यदि (skip)
      {
	यावत् (i + 4 <= coord_गिनती && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	यावत् (i < coord_गिनती && peak_tuple[i].to_int () == 0)
	  i += 1;
	यदि (i >= coord_गिनती)
	  भङ्ग;
      }
#endif
#endif

      पूर्णाङ्क peak = peak_tuple[i].to_int ();
      यदि (!peak) अनुवर्त;

      पूर्णाङ्क v = coords[i];
      यदि (!v) { scalar = 0.f; भङ्ग; }
      यदि (v == peak) अनुवर्त;

      यदि (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        पूर्णाङ्क start = start_tuple[i].to_int ();
        पूर्णाङ्क end = end_tuple[i].to_int ();
        यदि (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) अनुवर्त;
        यदि (v < start || v > end) { scalar = 0.f; भङ्ग; }
        यदि (v < peak)
        { यदि (peak != start) scalar *= (भिन्न) (v - start) / (peak - start); }
        अन्यथा
        { यदि (peak != end) scalar *= (भिन्न) (end - v) / (end - peak); }
      }
      अन्यथा यदि (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; भङ्ग; }
      अन्यथा
        scalar *= (भिन्न) v / peak;
    }
    यदि (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    प्रत्यागम scalar;
  }

  तार्किक           has_peak () ध्रुव { प्रत्यागम tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  तार्किक   has_intermediate () ध्रुव { प्रत्यागम tupleIndex & TupleIndex::IntermediateRegion; }
  तार्किक has_private_points () ध्रुव { प्रत्यागम tupleIndex & TupleIndex::PrivatePointNumbers; }
  अचिन्हित      get_index () ध्रुव { प्रत्यागम tupleIndex & TupleIndex::TupleIndexMask; }
  स्थिर constexpr अचिन्हित max_shared_tuple_गिनती = 0x1000u;

  रक्षित:
  संरचना TupleIndex : HBUINT16
  {
    गणना Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& चालक = (uint16_t i) { HBUINT16::चालक= (i); प्रत्यागम *एतत्; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<ध्रुव F2DOT14> get_all_tuples (अचिन्हित axis_गिनती) ध्रुव
  { प्रत्यागम StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_गिनती); }
  ध्रुव F2DOT14* get_all_tuples_base (अचिन्हित axis_गिनती) ध्रुव
  { प्रत्यागम StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  ध्रुव F2DOT14* get_peak_tuple (अचिन्हित axis_गिनती) ध्रुव
  { प्रत्यागम get_all_tuples_base (axis_गिनती); }
  ध्रुव F2DOT14* get_start_tuple (अचिन्हित axis_गिनती) ध्रुव
  { प्रत्यागम get_all_tuples_base (axis_गिनती) + has_peak () * axis_गिनती; }
  ध्रुव F2DOT14* get_end_tuple (अचिन्हित axis_गिनती) ध्रुव
  { प्रत्यागम get_all_tuples_base (axis_गिनती) + has_peak () * axis_गिनती + axis_गिनती; }

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
  hb_vector_t<अचिन्हित वर्ण> opt_point_data;
  hb_vector_t<अचिन्हित वर्ण> opt_deltas_data;
  hb_vector_t<अचिन्हित वर्ण> point_data;
  hb_vector_t<अचिन्हित वर्ण> deltas_data;
  hb_vector_t<पूर्णाङ्क> rounded_deltas;
};

संरचना tuple_delta_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;  // Watch out when adding new members!

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
  hb_vector_t<अचिन्हित वर्ण> compiled_tuple_header;
  hb_vector_t<अचिन्हित वर्ण> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (ध्रुव tuple_delta_t& o) = स्वभाव;
  tuple_delta_t& चालक = (ध्रुव tuple_delta_t& o) = स्वभाव;

  मित्र शून्य swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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

  tuple_delta_t& चालक = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*एतत्, o);
    प्रत्यागम *एतत्;
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
  { प्रत्यागम axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& चालक += (ध्रुव tuple_delta_t& o)
  {
    अचिन्हित अंक = indices.length;
    प्रति (अचिन्हित i = 0; i < अंक; i++)
    {
      यदि (indices.arrayZ[i])
      {
        यदि (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          यदि (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      अन्यथा
      {
        यदि (!o.indices.arrayZ[i]) अनुवर्त;
        indices.arrayZ[i] = सत्य;
        deltas_x[i] = o.deltas_x[i];
        यदि (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    प्रत्यागम *एतत्;
  }

  tuple_delta_t& चालक *= (भिन्न scalar)
  {
    यदि (scalar == 1.0f)
      प्रत्यागम *एतत्;

    अचिन्हित अंक = indices.length;
    यदि (deltas_y)
      प्रति (अचिन्हित i = 0; i < अंक; i++)
      {
	यदि (!indices.arrayZ[i]) अनुवर्त;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    अन्यथा
      प्रति (अचिन्हित i = 0; i < अंक; i++)
      {
	यदि (!indices.arrayZ[i]) अनुवर्त;
	deltas_x[i] *= scalar;
      }
    प्रत्यागम *एतत्;
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
    यदि (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*एतत्));
      प्रत्यागम;
    }

    यदि ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->मध्य && tent->मध्य <= tent->maximum))
      प्रत्यागम;

    यदि (tent->मध्य == 0.f)
    {
      out.push (std::move (*एतत्));
      प्रत्यागम;
    }

    यदि (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.मध्य &&
          axis_limit.मध्य <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      प्रत्यागम;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    प्रति (अचिन्हित i = 0; i < solutions.length; i++)
    {
      स्वतः &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      यदि (i < solutions.length - 1)
	new_var.copy_from (*एतत्, pool);
      अन्यथा
	new_var = std::move (*एतत्);

      यदि (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      अन्यथा
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  तार्किक compile_coords (ध्रुव hb_map_t& axes_index_map,
		       ध्रुव hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    अचिन्हित cur_axis_गिनती = axes_index_map.get_population ();
    यदि (pool)
    {
      यदि (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_गिनती)))
	प्रत्यागम असत्य;
    }
    अन्यथा यदि (unlikely (!compiled_peak_coords.resize (cur_axis_गिनती)))
      प्रत्यागम असत्य;

    hb_array_t<F2DOT14> start_coords, end_coords;

    अचिन्हित orig_axis_गिनती = axes_old_index_tag_map.get_population ();
    अचिन्हित j = 0;
    प्रति (अचिन्हित i = 0; i < orig_axis_गिनती; i++)
    {
      यदि (!axes_index_map.has (i))
        अनुवर्त;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      यदि (axis_tuples.has (axis_tag, &coords))
      {
	भिन्न min_val = coords->minimum;
	भिन्न val = coords->मध्य;
	भिन्न max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	यदि (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  यदि (!compiled_interm_coords)
	  {
	    यदि (pool)
	    {
	      यदि (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_गिनती)))
		प्रत्यागम असत्य;
	    }
	    अन्यथा यदि (unlikely (!compiled_interm_coords.resize (2 * cur_axis_गिनती)))
	      प्रत्यागम असत्य;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_गिनती);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_गिनती);

	    प्रति (अचिन्हित k = 0; k < j; k++)
	    {
	      चिन्हित peak = compiled_peak_coords.arrayZ[k].to_int ();
	      यदि (!peak) अनुवर्त;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	यदि (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    प्रत्यागम !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  तार्किक compile_tuple_var_header (ध्रुव hb_map_t& axes_index_map,
                                 अचिन्हित points_data_length,
                                 ध्रुव hb_map_t& axes_old_index_tag_map,
                                 ध्रुव hb_hashmap_t<ध्रुव hb_vector_t<F2DOT14>*, अचिन्हित>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    यदि (!compiled_deltas) प्रत्यागम सत्य;

    अचिन्हित cur_axis_गिनती = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    अचिन्हित alloc_len = 3 * cur_axis_गिनती * (F2DOT14::static_size) + 4;
    यदि (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, असत्य))) प्रत्यागम असत्य;

    अचिन्हित flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    यदि (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    अचिन्हित peak_गिनती = 0;
    अचिन्हित *shared_tuple_idx;
    यदि (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    अन्यथा
    {
      peak_गिनती = encode_peak_coords(coords, flag);
      यदि (!peak_गिनती) प्रत्यागम असत्य;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    अचिन्हित interim_गिनती = encode_interm_coords (coords.sub_array (peak_गिनती), flag);

    /* pointdata length = 0 implies "use shared points" */
    यदि (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    अचिन्हित serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    अचिन्हित total_header_len = 4 + (peak_गिनती + interim_गिनती) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    प्रत्यागम सत्य;
  }

  अचिन्हित encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               अचिन्हित& flag) ध्रुव
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * परिमाण (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    प्रत्यागम compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  अचिन्हित encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 अचिन्हित& flag) ध्रुव
  {
    यदि (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * परिमाण (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    प्रत्यागम compiled_interm_coords.length;
  }

  तार्किक compile_deltas (hb_vector_t<पूर्णाङ्क> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { प्रत्यागम compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  स्थिर तार्किक compile_deltas (hb_array_t<ध्रुव तार्किक> point_indices,
			      hb_array_t<ध्रुव भिन्न> x_deltas,
			      hb_array_t<ध्रुव भिन्न> y_deltas,
			      hb_vector_t<अचिन्हित वर्ण> &compiled_deltas, /* OUT */
			      hb_vector_t<पूर्णाङ्क> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    यदि (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      प्रत्यागम असत्य;

    अचिन्हित j = 0;
    प्रति (अचिन्हित i = 0; i < point_indices.length; i++)
    {
      यदि (!point_indices[i]) अनुवर्त;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    यदि (!rounded_deltas) प्रत्यागम सत्य;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    अचिन्हित alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    यदि (y_deltas)
      alloc_len *= 2;

    यदि (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, असत्य))) प्रत्यागम असत्य;

    अचिन्हित encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    यदि (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      अचिन्हित j = 0;
      प्रति (अचिन्हित idx = 0; idx < point_indices.length; idx++)
      {
        यदि (!point_indices[idx]) अनुवर्त;
        पूर्णाङ्क rounded_delta = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) y_deltas.arrayZ[idx]));

        यदि (j >= rounded_deltas.length) प्रत्यागम असत्य;

        rounded_deltas[j++] = rounded_delta;
      }

      यदि (j != rounded_deltas.length) प्रत्यागम असत्य;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    प्रत्यागम सत्य;
  }

  स्थिर अचिन्हित compile_deltas (hb_array_t<अचिन्हित वर्ण> encoded_bytes,
				  hb_array_t<ध्रुव पूर्णाङ्क> deltas)
  {
    प्रत्यागम Tuple_मूल्यावली::compile_unsafe (deltas, encoded_bytes);
  }

  तार्किक calc_inferred_deltas (ध्रुव contour_point_vector_t& orig_points,
			     hb_vector_t<अचिन्हित> &scratch)
  {
    अचिन्हित point_गिनती = orig_points.length;
    यदि (point_गिनती != indices.length)
      प्रत्यागम असत्य;

    अचिन्हित ref_गिनती_2 = 0;

    hb_vector_t<अचिन्हित> &end_points = scratch.reset ();

    प्रति (अचिन्हित i = 0; i < point_गिनती; i++)
    {
      ref_गिनती_2 += indices.arrayZ[i];
      यदि (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    यदि (ref_गिनती_2 == point_गिनती)
      प्रत्यागम सत्य;
    यदि (unlikely (end_points.in_error ())) प्रत्यागम असत्य;

    hb_bit_set_t inferred_idxes;
    अचिन्हित start_point = 0;
    प्रति (अचिन्हित end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      अचिन्हित unref_गिनती = 0;
      प्रति (अचिन्हित i = start_point; i < end_point + 1; i++)
        unref_गिनती += indices.arrayZ[i];
      unref_गिनती = (end_point - start_point + 1) - unref_गिनती;

      अचिन्हित j = start_point;
      यदि (unref_गिनती == 0 || unref_गिनती > end_point - start_point)
        गच्छ no_more_gaps;
      प्रति (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        अचिन्हित पूर्णाङ्क prev, next, i;
        प्रति (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          यदि (indices.arrayZ[i] && !indices.arrayZ[j]) भङ्ग;
        }
        prev = j = i;
        प्रति (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          यदि (!indices.arrayZ[i] && indices.arrayZ[j]) भङ्ग;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        प्रति (;;)
        {
          i = next_index (i, start_point, end_point);
          यदि (i == next) भङ्ग;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          यदि (--unref_गिनती == 0) गच्छ no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    प्रति (अचिन्हित i = 0; i < point_गिनती; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      यदि ( !indices[i])
      {
        यदि (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = सत्य;
      }
    }
    प्रत्यागम सत्य;
  }

  तार्किक optimize (ध्रुव contour_point_vector_t& contour_points,
                 तार्किक is_composite,
		 optimize_scratch_t &scratch,
                 द्विगुण tolerance = 0.5 + 1e-10)
  {
    अचिन्हित गिनती = contour_points.length;
    यदि (deltas_x.length != गिनती ||
        deltas_y.length != गिनती)
      प्रत्यागम असत्य;

    hb_vector_t<तार्किक> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<पूर्णाङ्क> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<पूर्णाङ्क> &rounded_y_deltas = scratch.rounded_y_deltas;

    यदि (unlikely (!rounded_x_deltas.resize_dirty  (गिनती) ||
                  !rounded_y_deltas.resize_dirty  (गिनती)))
      प्रत्यागम असत्य;

    प्रति (अचिन्हित i = 0; i < गिनती; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) deltas_y.arrayZ[i]));
    }

    यदि (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      प्रत्यागम असत्य;

    अचिन्हित ref_गिनती_2 = 0;
    प्रति (तार्किक ref_flag : opt_indices)
       ref_गिनती_2 += ref_flag;

    यदि (ref_गिनती_2 == गिनती) प्रत्यागम सत्य;

    hb_vector_t<भिन्न> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<भिन्न> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    तार्किक is_comp_glyph_wo_deltas = (is_composite && ref_गिनती_2 == 0);
    यदि (is_comp_glyph_wo_deltas)
    {
      यदि (unlikely (!opt_deltas_x.resize (गिनती) ||
                    !opt_deltas_y.resize (गिनती)))
        प्रत्यागम असत्य;

      opt_indices.arrayZ[0] = सत्य;
      प्रति (अचिन्हित i = 1; i < गिनती; i++)
        opt_indices.arrayZ[i] = असत्य;
    }

    hb_vector_t<अचिन्हित वर्ण> &opt_point_data = scratch.opt_point_data.reset ();
    यदि (!compile_point_set (opt_indices, opt_point_data))
      प्रत्यागम असत्य;
    hb_vector_t<अचिन्हित वर्ण> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    यदि (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      प्रत्यागम असत्य;

    hb_vector_t<अचिन्हित वर्ण> &point_data = scratch.point_data.reset ();
    यदि (!compile_point_set (indices, point_data))
      प्रत्यागम असत्य;
    hb_vector_t<अचिन्हित वर्ण> &deltas_data = scratch.deltas_data.reset ();
    यदि (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      प्रत्यागम असत्य;

    यदि (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      यदि (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    प्रत्यागम !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  स्थिर तार्किक compile_point_set (ध्रुव hb_vector_t<तार्किक> &point_indices,
                                 hb_vector_t<अचिन्हित वर्ण>& compiled_points /* OUT */)
  {
    अचिन्हित अंक_points = 0;
    प्रति (तार्किक i : point_indices)
      यदि (i) अंक_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    यदि (!अंक_points) प्रत्यागम सत्य;

    अचिन्हित indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    यदि (अंक_points == indices_length)
      प्रत्यागम compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    अचिन्हित अंक_bytes = 2 + 3 *अंक_points;
    यदि (unlikely (!compiled_points.resize_dirty  (अंक_bytes)))
      प्रत्यागम असत्य;

    अचिन्हित pos = 0;
    /* binary data starts with the total number of reference points */
    यदि (अंक_points < 0x80)
      compiled_points.arrayZ[pos++] = अंक_points;
    अन्यथा
    {
      compiled_points.arrayZ[pos++] = ((अंक_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = अंक_points & 0xFF;
    }

    ध्रुव अचिन्हित max_run_length = 0x7F;
    अचिन्हित i = 0;
    अचिन्हित last_मूल्य = 0;
    अचिन्हित अंक_encoded = 0;
    यावत् (i < indices_length && अंक_encoded < अंक_points)
    {
      अचिन्हित run_length = 0;
      अचिन्हित header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      तार्किक use_byte_encoding = असत्य;
      तार्किक new_run = सत्य;
      यावत् (i < indices_length && अंक_encoded < अंक_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        यावत् (i < indices_length && !point_indices[i])
          i++;

        यदि (i >= indices_length) भङ्ग;

        अचिन्हित cur_मूल्य = i;
        अचिन्हित delta = cur_मूल्य - last_मूल्य;

        यदि (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = असत्य;
        }

        यदि (use_byte_encoding && delta > 0xFF)
          भङ्ग;

        यदि (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        अन्यथा
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_मूल्य = cur_मूल्य;
        run_length++;
        अंक_encoded++;
      }

      यदि (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      अन्यथा
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    प्रत्यागम compiled_points.resize_dirty  (pos);
  }

  स्थिर भिन्न infer_delta (भिन्न target_val, भिन्न prev_val, भिन्न next_val, भिन्न prev_delta, भिन्न next_delta)
  {
    यदि (prev_val == next_val)
      प्रत्यागम (prev_delta == next_delta) ? prev_delta : 0.f;
    अन्यथा यदि (target_val <= hb_min (prev_val, next_val))
      प्रत्यागम (prev_val < next_val) ? prev_delta : next_delta;
    अन्यथा यदि (target_val >= hb_max (prev_val, next_val))
      प्रत्यागम (prev_val > next_val) ? prev_delta : next_delta;

    भिन्न r = (target_val - prev_val) / (next_val - prev_val);
    प्रत्यागम prev_delta + r * (next_delta - prev_delta);
  }

  स्थिर अचिन्हित पूर्णाङ्क next_index (अचिन्हित पूर्णाङ्क i, अचिन्हित पूर्णाङ्क start, अचिन्हित पूर्णाङ्क end)
  { प्रत्यागम (i >= end) ? start : (i + 1); }
};

ढाँचा <typename OffType = HBUINT16>
संरचना TupleVariationData
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (एतत्));
  }

  माप_प्रकार get_size (अचिन्हित axis_गिनती_times_2) ध्रुव
  {
    माप_प्रकार total_size = min_size;
    अचिन्हित गिनती = tuple_Var_गिनती.get_गिनती ();
    ध्रुव TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_गिनती_times_2),
					     (माप_प्रकार) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_गिनती_times_2);
    }

    प्रत्यागम total_size;
  }

  ध्रुव TupleVariationHeader &get_tuple_var_header (शून्य) ध्रुव
  { प्रत्यागम StructAfter<TupleVariationHeader> (data); }

  संरचना tuple_iterator_t;
  संरचना tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    गुप्त:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ध्रुव hb_vector_t<तार्किक>*, hb_vector_t<अचिन्हित वर्ण>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ध्रुव hb_vector_t<तार्किक>*, अचिन्हित> point_set_गिनती_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<अचिन्हित वर्ण> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    अचिन्हित compiled_byte_size = 0;
    तार्किक needs_padding = असत्य;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    तार्किक is_composite = असत्य;

    सार्वजनिक:
    tuple_variations_t () = स्वभाव;
    tuple_variations_t (ध्रुव tuple_variations_t&) = delete;
    tuple_variations_t& चालक=(ध्रुव tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = स्वभाव;
    tuple_variations_t& चालक=(tuple_variations_t&&) = स्वभाव;
    ~tuple_variations_t () = स्वभाव;

    explicit चालक तार्किक () ध्रुव { प्रत्यागम तार्किक (tuple_vars); }
    अचिन्हित get_var_गिनती () ध्रुव
    {
      अचिन्हित गिनती = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      प्रति (स्वतः& tuple: tuple_vars)
        यदि (tuple.compiled_deltas) गिनती++;

      यदि (shared_points_bytes && shared_points_bytes->length)
        गिनती |= Tuple_Var_गिनती::SharedPointNumbers;
      प्रत्यागम गिनती;
    }

    अचिन्हित get_compiled_byte_size () ध्रुव
    { प्रत्यागम compiled_byte_size; }

    तार्किक create_from_tuple_var_data (tuple_iterator_t iterator,
                                     अचिन्हित tuple_var_गिनती,
                                     अचिन्हित point_गिनती,
                                     तार्किक is_gvar,
                                     ध्रुव hb_map_t *axes_old_index_tag_map,
                                     ध्रुव hb_vector_t<अचिन्हित> &shared_indices,
                                     ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     तार्किक is_composite_glyph = असत्य)
    {
      hb_vector_t<अचिन्हित> private_indices;
      hb_vector_t<पूर्णाङ्क> deltas_x;
      hb_vector_t<पूर्णाङ्क> deltas_y;
      कुरु
      {
        ध्रुव HBUINT8 *p = iterator.get_serialized_data ();
        अचिन्हित पूर्णाङ्क length = iterator.current_tuple->get_data_size ();
        यदि (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          प्रत्यागम असत्य;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        यदि (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_गिनती (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          प्रत्यागम असत्य;

	private_indices.reset ();
        तार्किक has_private_points = iterator.current_tuple->has_private_points ();
        ध्रुव HBUINT8 *end = p + length;
        यदि (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          प्रत्यागम असत्य;

        ध्रुव hb_vector_t<अचिन्हित> &indices = has_private_points ? private_indices : shared_indices;
        तार्किक apply_to_all = (indices.length == 0);
        अचिन्हित अंक_deltas = apply_to_all ? point_गिनती : indices.length;

        यदि (unlikely (!deltas_x.resize_dirty  (अंक_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          प्रत्यागम असत्य;

        यदि (is_gvar)
        {
          यदि (unlikely (!deltas_y.resize_dirty  (अंक_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            प्रत्यागम असत्य;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        यदि (unlikely (!var.indices.allocate_from_pool (pool, point_गिनती) ||
                      !var.deltas_x.allocate_from_pool (pool, point_गिनती, असत्य)))
          प्रत्यागम असत्य;

        यदि (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_गिनती, असत्य)))
          प्रत्यागम असत्य;

        प्रति (अचिन्हित i = 0; i < अंक_deltas; i++)
        {
          अचिन्हित idx = apply_to_all ? i : indices[i];
          यदि (idx >= point_गिनती) अनुवर्त;
          var.indices[idx] = सत्य;
          var.deltas_x[idx] = deltas_x[i];
          यदि (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } यावत् (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      प्रत्यागम सत्य;
    }

    तार्किक create_from_item_var_data (ध्रुव VarData &var_data,
                                    ध्रुव hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    ध्रुव hb_map_t& axes_old_index_tag_map,
                                    अचिन्हित& item_गिनती_2,
                                    ध्रुव hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      यदि (&var_data == &Null (VarData))
        प्रत्यागम सत्य;

      अचिन्हित अंक_regions = var_data.get_region_index_गिनती ();
      यदि (!tuple_vars.alloc (अंक_regions)) प्रत्यागम असत्य;

      item_गिनती_2 = inner_map ? inner_map->get_population () : var_data.get_item_गिनती ();
      यदि (!item_गिनती_2) प्रत्यागम सत्य;
      अचिन्हित row_size = var_data.get_row_size ();
      ध्रुव HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      प्रति (अचिन्हित r = 0; r < अंक_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        यदि (!tuple.deltas_x.resize_dirty  (item_गिनती_2) ||
            !tuple.indices.resize_dirty  (item_गिनती_2))
          प्रत्यागम असत्य;

        प्रति (अचिन्हित i = 0; i < item_गिनती_2; i++)
        {
          tuple.indices.arrayZ[i] = सत्य;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        अचिन्हित region_index = var_data.get_region_index (r);
        यदि (region_index >= regions.length) प्रत्यागम असत्य;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      प्रत्यागम !tuple_vars.in_error ();
    }

    गुप्त:
    स्थिर पूर्णाङ्क _cmp_axis_tag (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
    {
      ध्रुव hb_tag_t *a = (ध्रुव hb_tag_t*) pa;
      ध्रुव hb_tag_t *b = (ध्रुव hb_tag_t*) pb;
      प्रत्यागम (पूर्णाङ्क)(*a) - (पूर्णाङ्क)(*b);
    }

    तार्किक change_tuple_variations_axis_limits (ध्रुव hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              ध्रुव hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      यदि (!axis_tags.alloc (normalized_axes_location.get_population ()))
        प्रत्यागम असत्य;
      प्रति (स्वतः t : normalized_axes_location.कुंजियाँ ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.क्विक (_cmp_axis_tag);
      प्रति (स्वतः axis_tag : axis_tags)
      {
        Triple *axis_limit;
        यदि (!normalized_axes_location.has (axis_tag, &axis_limit))
          प्रत्यागम असत्य;
        TripleDistances axis_triple_distances{1.f, 1.f};
        यदि (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        प्रति (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          यदि (!out) अनुवर्त;

          अचिन्हित new_len = new_vars.length + out.length;

          यदि (unlikely (!new_vars.alloc (new_len, असत्य)))
            प्रत्यागम असत्य;

          प्रति (अचिन्हित i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      प्रत्यागम सत्य;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    तार्किक merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      यदि (unlikely (!new_vars.alloc (tuple_vars.length)))
	प्रत्यागम असत्य;
      hb_hashmap_t<ध्रुव hb_hashmap_t<hb_tag_t, Triple>*, अचिन्हित> m;
      प्रति (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        यदि (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          यदि (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            प्रत्यागम असत्य;
          अनुवर्त;
        }

        अचिन्हित *idx;
        यदि (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        अन्यथा
        {
	  स्वतः *new_var = new_vars.push ();
	  यदि (unlikely (new_vars.in_error ()))
	    प्रत्यागम असत्य;
          hb_swap (*new_var, var);
          यदि (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            प्रत्यागम असत्य;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      प्रत्यागम सत्य;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    तार्किक compile_all_point_sets ()
    {
      प्रति (ध्रुव स्वतः& tuple: tuple_vars)
      {
        ध्रुव hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        यदि (point_data_map.has (points_set))
        {
          अचिन्हित *गिनती;
          यदि (unlikely (!point_set_गिनती_map.has (points_set, &गिनती) ||
                        !point_set_गिनती_map.set (points_set, (*गिनती) + 1)))
            प्रत्यागम असत्य;
          अनुवर्त;
        }

        hb_vector_t<अचिन्हित वर्ण> compiled_point_data;
        यदि (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          प्रत्यागम असत्य;

        यदि (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_गिनती_map.set (points_set, 1))
          प्रत्यागम असत्य;
      }
      प्रत्यागम सत्य;
    }

    /* find shared points set which saves most bytes */
    शून्य find_shared_points ()
    {
      अचिन्हित max_saved_bytes = 0;

      प्रति (ध्रुव स्वतः& _ : point_data_map.iter_ref ())
      {
        ध्रुव hb_vector_t<तार्किक>* points_set = _.first;
        अचिन्हित data_length = _.second.length;
        यदि (!data_length) अनुवर्त;
        अचिन्हित *गिनती;
        यदि (unlikely (!point_set_गिनती_map.has (points_set, &गिनती) ||
                      *गिनती <= 1))
        {
          shared_points_bytes = nullptr;
          प्रत्यागम;
        }

        अचिन्हित saved_bytes = data_length * ((*गिनती) -1);
        यदि (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    तार्किक calc_inferred_deltas (ध्रुव contour_point_vector_t& contour_points,
			       hb_vector_t<अचिन्हित> &scratch)
    {
      प्रति (tuple_delta_t& var : tuple_vars)
        यदि (!var.calc_inferred_deltas (contour_points, scratch))
          प्रत्यागम असत्य;

      प्रत्यागम सत्य;
    }

    तार्किक iup_optimize (ध्रुव contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      प्रति (tuple_delta_t& var : tuple_vars)
      {
        यदि (!var.optimize (contour_points, is_composite, scratch))
          प्रत्यागम असत्य;
      }
      प्रत्यागम सत्य;
    }

    सार्वजनिक:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    शून्य cull_unreachable (ध्रुव hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      अचिन्हित j = 0;
      प्रति (अचिन्हित i = 0; i < tuple_vars.length; i++)
      {
	यदि (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  अनुवर्त;
	यदि (i != j)
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
                      तार्किक optimize = असत्य)
    {
      यदि (!tuple_vars) प्रत्यागम सत्य;
      यदि (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        प्रत्यागम असत्य;
      /* compute inferred deltas only for gvar */
      यदि (contour_points)
      {
	hb_vector_t<अचिन्हित> scratch;
	यदि (!calc_inferred_deltas (*contour_points, scratch))
	  प्रत्यागम असत्य;
      }

      /* if iup delta opt is on, contour_points can't be null */
      यदि (optimize && !contour_points)
        प्रत्यागम असत्य;

      यदि (!merge_tuple_variations (optimize ? contour_points : nullptr))
        प्रत्यागम असत्य;

      यदि (optimize && !iup_optimize (*contour_points, scratch)) प्रत्यागम असत्य;
      प्रत्यागम !tuple_vars.in_error ();
    }

    तार्किक compile_bytes (ध्रुव hb_map_t& axes_index_map,
                        ध्रुव hb_map_t& axes_old_index_tag_map,
                        तार्किक use_shared_points,
                        तार्किक is_gvar = असत्य,
                        ध्रुव hb_hashmap_t<ध्रुव hb_vector_t<F2DOT14>*, अचिन्हित>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      यदि (!tuple_vars)
        प्रत्यागम सत्य;

      // compile points set and store data in hashmap
      यदि (!compile_all_point_sets ())
        प्रत्यागम असत्य;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      यदि (use_shared_points)
      {
        find_shared_points ();
        यदि (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<पूर्णाङ्क> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      प्रति (स्वतः& tuple: tuple_vars)
      {
        ध्रुव hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        hb_vector_t<अचिन्हित वर्ण> *points_data;
        यदि (unlikely (!point_data_map.has (points_set, &points_data)))
          प्रत्यागम असत्य;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        यदि (!points_data->length)
          अनुवर्त;
        यदि (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          प्रत्यागम असत्य;

        अचिन्हित points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        यदि (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          प्रत्यागम असत्य;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      यदि (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = सत्य;
        compiled_byte_size += 1;
      }

      प्रत्यागम सत्य;
    }

    तार्किक serialize_var_headers (hb_serialize_context_t *c, अचिन्हित& total_header_len) ध्रुव
    {
      TRACE_SERIALIZE (एतत्);
      प्रति (ध्रुव स्वतः& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        यदि (c->in_error ()) return_trace (असत्य);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (सत्य);
    }

    तार्किक serialize_var_data (hb_serialize_context_t *c, तार्किक is_gvar) ध्रुव
    {
      TRACE_SERIALIZE (एतत्);
      यदि (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      प्रति (ध्रुव स्वतः& tuple: tuple_vars)
      {
        ध्रुव hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        hb_vector_t<अचिन्हित वर्ण> *point_data;
        यदि (!point_data_map.has (points_set, &point_data))
          return_trace (असत्य);

        यदि (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        यदि (c->in_error ()) return_trace (असत्य);
      }

      /* padding for gvar */
      यदि (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        यदि (!c->embed (pad)) return_trace (असत्य);
      }
      return_trace (सत्य);
    }
  };

  संरचना tuple_iterator_t
  {
    अचिन्हित get_axis_गिनती () ध्रुव { प्रत्यागम axis_गिनती; }

    शून्य init (hb_bytes_t var_data_bytes_, अचिन्हित पूर्णाङ्क axis_गिनती_3, ध्रुव शून्य *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_बायाँ = var_data->tuple_Var_गिनती.get_गिनती ();
      axis_गिनती = axis_गिनती_3;
      axis_गिनती_times_2 = axis_गिनती_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    तार्किक get_shared_indices (hb_vector_t<अचिन्हित पूर्णाङ्क> &shared_indices /* OUT */)
    {
      यदि (var_data->has_shared_point_numbers ())
      {
        ध्रुव HBUINT8 *base = &(table_base+var_data->data);
        ध्रुव HBUINT8 *p = base;
        यदि (!decompile_points (p, shared_indices, (ध्रुव HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) प्रत्यागम असत्य;
        data_offset = p - base;
      }
      प्रत्यागम सत्य;
    }

    तार्किक is_valid ()
    {
      यदि (unlikely (tuples_बायाँ <= 0))
	प्रत्यागम असत्य;

      current_tuple_size = TupleVariationHeader::min_size;
      यदि (unlikely (!var_data_bytes.check_end ((ध्रुव वर्ण *) current_tuple + current_tuple_size)))
	प्रत्यागम असत्य;

      current_tuple_size = current_tuple->get_size (axis_गिनती_times_2);
      यदि (unlikely (!var_data_bytes.check_end ((ध्रुव वर्ण *) current_tuple + current_tuple_size)))
	प्रत्यागम असत्य;

      प्रत्यागम सत्य;
    }

    HB_ALWAYS_INLINE
    तार्किक move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_बायाँ--;
      प्रत्यागम is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ध्रुव HBUINT8 *get_serialized_data () ध्रुव
    { प्रत्यागम &(table_base+var_data->data) + data_offset; }

    गुप्त:
    चिन्हित tuples_बायाँ;
    ध्रुव TupleVariationData *var_data;
    अचिन्हित पूर्णाङ्क axis_गिनती;
    अचिन्हित पूर्णाङ्क axis_गिनती_times_2;
    अचिन्हित पूर्णाङ्क data_offset;
    अचिन्हित पूर्णाङ्क current_tuple_size;
    ध्रुव शून्य *table_base;

    सार्वजनिक:
    hb_bytes_t var_data_bytes;
    ध्रुव TupleVariationHeader *current_tuple;
  };

  स्थिर तार्किक get_tuple_iterator (hb_bytes_t var_data_bytes, अचिन्हित axis_गिनती,
                                  ध्रुव शून्य *table_base,
                                  hb_vector_t<अचिन्हित पूर्णाङ्क> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_गिनती, table_base);
    यदि (!iterator->get_shared_indices (shared_indices))
      प्रत्यागम असत्य;
    प्रत्यागम iterator->is_valid ();
  }

  तार्किक has_shared_point_numbers () ध्रुव { प्रत्यागम tuple_Var_गिनती.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  तार्किक cull_tuple_variations (hb_bytes_t var_data_bytes,
			      अचिन्हित axis_गिनती,
			      hb_array_t<ध्रुव F2DOT14> shared_tuples,
			      ध्रुव hb_map_t *axes_old_index_tag_map,
			      ध्रुव hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<वर्ण> &out /* OUT */,
			      तार्किक *changed /* OUT */) ध्रुव
  {
    *changed = असत्य;

    hb_vector_t<अचिन्हित पूर्णाङ्क> shared_indices;
    tuple_iterator_t iterator;
    यदि (!get_tuple_iterator (var_data_bytes, axis_गिनती, एतत्,
			     shared_indices, &iterator))
      प्रत्यागम सत्य; /* no tuples or malformed: keep original */

    ध्रुव वर्ण *bytes_start = var_data_bytes.arrayZ;
    ध्रुव वर्ण *bytes_end = bytes_start + var_data_bytes.length;
    ध्रुव वर्ण *serialized_base = (ध्रुव वर्ण *) &(एतत्+data);
    ध्रुव वर्ण *shared_points_end = (ध्रुव वर्ण *) iterator.get_serialized_data ();
    यदि (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      प्रत्यागम सत्य;

    संरचना kept_tuple_t
    {
      ध्रुव वर्ण *header;
      अचिन्हित header_size;
      ध्रुव वर्ण *data;
      अचिन्हित data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    अचिन्हित total = 0;
    अचिन्हित kept_headers_size = 0, kept_data_size = 0;
    कुरु
    {
      ध्रुव TupleVariationHeader *header = iterator.current_tuple;
      अचिन्हित header_size = header->get_size (axis_गिनती * 2);
      अचिन्हित data_size = header->get_data_size ();
      ध्रुव वर्ण *tuple_data = (ध्रुव वर्ण *) iterator.get_serialized_data ();
      यदि (unlikely (tuple_data < serialized_base ||
		    data_size > (अचिन्हित) (bytes_end - tuple_data)))
	प्रत्यागम सत्य; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      यदि (!header->unpack_axis_tuples (axis_गिनती, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	प्रत्यागम सत्य;

      total++;
      यदि (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ध्रुव वर्ण *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } यावत् (iterator.move_to_next ());

    यदि (unlikely (kept.in_error ())) प्रत्यागम असत्य;
    यदि (kept.length == total) प्रत्यागम सत्य; /* nothing to cull */

    *changed = सत्य;
    यदि (!kept.length) प्रत्यागम सत्य; /* everything culled: no variation data */

    अचिन्हित shared_points_size = shared_points_end - serialized_base;
    अचिन्हित data_offset = min_size + kept_headers_size;
    अचिन्हित new_size = data_offset + shared_points_size + kept_data_size;
    यदि (unlikely (!out.resize (new_size)))
      प्रत्यागम असत्य;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_गिनती = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    वर्ण *p = out.arrayZ + min_size;
    प्रति (ध्रुव स्वतः &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    प्रति (ध्रुव स्वतः &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    प्रत्यागम सत्य;
  }

  स्थिर तार्किक decompile_points (ध्रुव HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<अचिन्हित पूर्णाङ्क> &points /* OUT */,
				ध्रुव HBUINT8 *end)
  {
    गणना packed_point_flag_t
    {
      POINTS_ARE_शब्दगण     = 0x80,
      POINT_RUN_गिनती_MASK = 0x7F
    };

    यदि (unlikely (p + 1 > end)) प्रत्यागम असत्य;

    अचिन्हित गिनती = *p++;
    यदि (गिनती & POINTS_ARE_शब्दगण)
    {
      यदि (unlikely (p + 1 > end)) प्रत्यागम असत्य;
      गिनती = ((गिनती & POINT_RUN_गिनती_MASK) << 8) | *p++;
    }
    यदि (unlikely (!points.resize_dirty  (गिनती))) प्रत्यागम असत्य;

    अचिन्हित n = 0;
    अचिन्हित i = 0;
    यावत् (i < गिनती)
    {
      यदि (unlikely (p + 1 > end)) प्रत्यागम असत्य;
      अचिन्हित control = *p++;
      अचिन्हित run_गिनती = (control & POINT_RUN_गिनती_MASK) + 1;
      अचिन्हित stop = i + run_गिनती;
      यदि (unlikely (stop > गिनती)) प्रत्यागम असत्य;
      यदि (control & POINTS_ARE_शब्दगण)
      {
        यदि (unlikely (p + run_गिनती * HBUINT16::static_size > end)) प्रत्यागम असत्य;
        प्रति (; i < stop; i++)
        {
          n += *(ध्रुव HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      अन्यथा
      {
        यदि (unlikely (p + run_गिनती > end)) प्रत्यागम असत्य;
        प्रति (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    प्रत्यागम सत्य;
  }

  ढाँचा <typename T>
  HB_ALWAYS_INLINE
  स्थिर तार्किक decompile_deltas (ध्रुव HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				ध्रुव HBUINT8 *end,
				तार्किक consume_all = असत्य,
				अचिन्हित start = 0)
  {
    प्रत्यागम Tuple_मूल्यावली::decompile (p, deltas, end, consume_all, start);
  }

  तार्किक has_data () ध्रुव { प्रत्यागम tuple_Var_गिनती; }

  तार्किक decompile_tuple_variations (अचिन्हित point_गिनती,
                                   तार्किक is_gvar,
                                   tuple_iterator_t iterator,
                                   ध्रुव hb_map_t *axes_old_index_tag_map,
                                   ध्रुव hb_vector_t<अचिन्हित> &shared_indices,
                                   ध्रुव hb_array_t<ध्रुव F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   तार्किक is_composite_glyph = असत्य) ध्रुव
  {
    प्रत्यागम tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_गिनती,
                                                        point_गिनती, is_gvar,
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
    यदि (!tuple_variations) return_trace (सत्य);

    स्वतः *out = c->start_embed (एतत्);
    यदि (unlikely (!c->extend_min (out))) return_trace (असत्य);

    यदि (!c->check_assign (out->tuple_Var_गिनती, tuple_variations.get_var_गिनती (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (असत्य);

    अचिन्हित total_header_len = 0;

    यदि (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (असत्य);

    अचिन्हित data_offset = min_size + total_header_len;
    यदि (!is_gvar) data_offset += 4;
    यदि (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (असत्य);

    प्रत्यागम tuple_variations.serialize_var_data (c, is_gvar);
  }

  रक्षित:
  संरचना Tuple_Var_गिनती : HBUINT16
  {
    मित्र संरचना tuple_variations_t;
    तार्किक has_shared_point_numbers () ध्रुव { प्रत्यागम ((*एतत्) & SharedPointNumbers); }
    अचिन्हित पूर्णाङ्क get_गिनती () ध्रुव { प्रत्यागम (*एतत्) & गिनती_Mask; }
    Tuple_Var_गिनती& चालक = (uint16_t i) { HBUINT16::चालक= (i); प्रत्यागम *एतत्; }
    explicit चालक तार्किक () ध्रुव { प्रत्यागम get_गिनती (); }

    रक्षित:
    गणना Flags
    {
      SharedPointNumbers= 0x8000u,
      गिनती_Mask         = 0x0FFFu
    };
    सार्वजनिक:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_गिनती tuple_Var_गिनती;  /* A packed field. The high 4 bits are flags, and the
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
  hb_vector_t<अचिन्हित> var_data_अंक_rows;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_vector_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, अचिन्हित> region_map;

  /* all delta rows after instantiation */
  hb_vector_t<hb_vector_t<पूर्णाङ्क>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  तार्किक has_long = असत्य;

  सार्वजनिक:
  तार्किक has_long_word () ध्रुव
  { प्रत्यागम has_long; }

  ध्रुव hb_vector_t<region_t>& get_region_list () ध्रुव
  { प्रत्यागम region_list; }

  ध्रुव hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () ध्रुव
  { प्रत्यागम encodings; }

  ध्रुव hb_map_t& get_varidx_map () ध्रुव
  { प्रत्यागम varidx_map; }

  तार्किक add_vardata_encoding_for_testing (hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क>*> &&rows,
                                         अचिन्हित अंक_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), अंक_cols));
    प्रत्यागम !encodings.in_error ();
  }

  तार्किक compile_varidx_map_for_testing (ध्रुव hb_hashmap_t<अचिन्हित, ध्रुव hb_vector_t<पूर्णाङ्क>*>& front_mapping)
  { प्रत्यागम compile_varidx_map (front_mapping); }

  तार्किक instantiate (ध्रुव ItemVariationStore& varStore,
                    ध्रुव hb_subset_plan_t *plan,
                    तार्किक optimize=सत्य,
                    तार्किक use_no_variation_idx=सत्य,
                    ध्रुव hb_array_t <ध्रुव hb_inc_bimap_t> inner_maps = hb_array_t<ध्रुव hb_inc_bimap_t> (),
                    तार्किक layout_conditions = असत्य)
  {
    यदि (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      प्रत्यागम असत्य;
    /* avar2 partial instancing: cull unreachable regions. */
    यदि (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      प्रति (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    यदि (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      प्रत्यागम असत्य;
    यदि (layout_conditions &&
        (!instantiate_condition_मूल्यावली (plan, varStore) || !build_region_list ()))
      प्रत्यागम असत्य;
    प्रत्यागम as_item_varstore (optimize, use_no_variation_idx);
  }

  तार्किक instantiate_condition_मूल्यावली (ध्रुव hb_subset_plan_t *plan,
                                      ध्रुव ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    स्वतः &condition_map = plan->layout_condition_idx_मूल्य_map;
    condition_map.clear ();
    यदि (!plan->layout_condition_मूल्यावली) प्रत्यागम सत्य;
    using condition_मूल्य_t = hb_pair_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णाङ्क>>;
    hb_vector_t<condition_मूल्य_t> conditions;
    प्रति (स्वतः condition : plan->layout_condition_मूल्यावली.iter ())
      conditions.push (condition);
    यदि (unlikely (conditions.in_error ())) प्रत्यागम असत्य;
    // Row order must not depend on the addresses of the source conditions.
    conditions.क्विक ([] (ध्रुव condition_मूल्य_t &a, ध्रुव condition_मूल्य_t &b) {
      यदि (a.second.first != b.second.first)
        प्रत्यागम a.second.first < b.second.first ? -1 : 1;
      प्रत्यागम a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    अचिन्हित new_outer = 0;
    प्रति (अचिन्हित i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      यदि (plan->gdef_varstore_inner_maps[i].get_population ())
        यदि (!outer_map.set (i, new_outer++)) प्रत्यागम असत्य;
    प्रति (स्वतः condition : conditions)
    {
      अचिन्हित old_idx = condition.second.first;
      द्विगुण value = condition.second.second +
                     varStore.get_delta<द्विगुण> (old_idx, plan->normalized_coords.as_array ());
      अचिन्हित old_outer = old_idx >> 16;
      यदि (!outer_map.has (old_outer))
        अनुवर्त;
      अचिन्हित outer = outer_map.get (old_outer);
      अचिन्हित inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      अचिन्हित idx = (outer << 16) | inner;
      यदि (outer >= vars.length || inner >= var_data_अंक_rows[outer])
        अनुवर्त;

      द्विगुण maximum = भ_असल ((द्विगुण) value);
      तार्किक variable = असत्य;
      प्रति (ध्रुव tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        यदि (unlikely (inner >= tuple.deltas_x.length)) प्रत्यागम असत्य;
        भिन्न delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, भ_असल ((द्विगुण) delta));
      }
      यदि (!variable)
      {
        यदि (!condition_map.set (condition.first,
                                hb_pair (अचिन्हित (VarIdx::NO_VARIATION), पूर्णाङ्क (value > 0))))
          प्रत्यागम असत्य;
        अनुवर्त;
      }

      द्विगुण scale = 1.;
      यावत् (maximum * scale > पूर्णांक_उच्च)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      यावत् (maximum * scale * 2 <= पूर्णांक_उच्च)
      {
        तार्किक integral = (द्विगुण) value * scale == round ((द्विगुण) value * scale);
        प्रति (ध्रुव tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (द्विगुण) tuple.deltas_x[inner] * scale ==
                      round ((द्विगुण) tuple.deltas_x[inner] * scale);
        यदि (integral) भङ्ग;
        scale *= 2;
      }
      पूर्णाङ्क default_मूल्य = (पूर्णाङ्क) round ((द्विगुण) value * scale);
      यदि (scale != 1. || default_मूल्य < -32768 || default_मूल्य > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        प्रति (ध्रुव tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          यदि (!tuple.deltas_x[inner]) अनुवर्त;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (सत्य);
          copy.deltas_x.push ((भिन्न) round ((द्विगुण) tuple.deltas_x[inner] * scale));
          यदि (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) प्रत्यागम असत्य;
          private_row.tuple_vars.push (std::move (copy));
        }
        यदि (unlikely (vars.length >= 0xFFFFu)) प्रत्यागम असत्य;
        यदि (unlikely (private_row.tuple_vars.in_error ())) प्रत्यागम असत्य;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_अंक_rows.push (1);
        यदि (unlikely (vars.in_error () || var_data_अंक_rows.in_error ())) प्रत्यागम असत्य;
        यदि (default_मूल्य < -32768 || default_मूल्य > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          पूर्णाङ्क bias = default_मूल्य - hb_clamp (default_मूल्य, -32768, 32767);
          default_मूल्य -= bias;
          पूर्णाङ्क ऊपरी = bias / 65536 * 65536;
          पूर्णाङ्क parts[] = {ऊपरी, bias - ऊपरी};
          प्रति (पूर्णाङ्क part : parts)
          {
            यदि (!part) अनुवर्त;
            अचिन्हित गिनती = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            ध्रुव स्वतः &tuples = vars[idx >> 16].tuple_vars;
            यदि (unlikely (tuples.in_error () || tuples.length != गिनती + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) प्रत्यागम असत्य;
          }
        }
      }
      यदि (!condition_map.set (condition.first, hb_pair (idx, default_मूल्य)))
        प्रत्यागम असत्य;
    }
    प्रत्यागम !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  तार्किक create_from_item_varstore (ध्रुव ItemVariationStore& varStore,
                                  ध्रुव hb_map_t& axes_old_index_tag_map,
                                  ध्रुव hb_array_t <ध्रुव hb_inc_bimap_t> inner_maps = hb_array_t<ध्रुव hb_inc_bimap_t> ())
  {
    ध्रुव VarRegionList& regionList = varStore.get_region_list ();
    यदि (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      प्रत्यागम असत्य;

    अचिन्हित अंक_var_data = varStore.get_sub_table_गिनती ();
    यदि (inner_maps && inner_maps.length != अंक_var_data) प्रत्यागम असत्य;
    यदि (!vars.alloc (अंक_var_data) ||
        !var_data_अंक_rows.alloc (अंक_var_data)) प्रत्यागम असत्य;

    प्रति (अचिन्हित i = 0; i < अंक_var_data; i++)
    {
      यदि (inner_maps && !inner_maps.arrayZ[i].get_population ())
          अनुवर्त;
      tuple_variations_t var_data_tuples;
      अचिन्हित item_गिनती_2 = 0;
      यदि (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_गिनती_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        प्रत्यागम असत्य;

      var_data_अंक_rows.push (item_गिनती_2);
      vars.push (std::move (var_data_tuples));
    }
    प्रत्यागम !vars.in_error () && !var_data_अंक_rows.in_error () && vars.length == var_data_अंक_rows.length;
  }

  तार्किक instantiate_tuple_vars (ध्रुव hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               ध्रुव hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               तार्किक build_regions = सत्य)
  {
    optimize_scratch_t scratch;
    प्रति (tuple_variations_t& tuple_vars : vars)
      यदि (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        प्रत्यागम असत्य;

    प्रत्यागम !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  अचिन्हित add_vardata (अचिन्हित item_गिनती_2)
  {
    vars.push (tuple_variations_t ());
    var_data_अंक_rows.push (item_गिनती_2);
    प्रत्यागम vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  अचिन्हित get_item_गिनती (अचिन्हित outer) ध्रुव
  { प्रत्यागम outer < var_data_अंक_rows.length ? var_data_अंक_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  अचिन्हित duplicate_row (अचिन्हित outer, अचिन्हित inner)
  {
    यदि (unlikely (outer >= vars.length ||
                  inner >= var_data_अंक_rows[outer] ||
                  var_data_अंक_rows[outer] >= 0xFFFFu))
      प्रत्यागम (अचिन्हित) -1;
    प्रति (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      यदि (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        प्रत्यागम (अचिन्हित) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      यदि (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      यदि (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        प्रत्यागम (अचिन्हित) -1;
    }
    प्रत्यागम var_data_अंक_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  शून्य add_tuple (अचिन्हित outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  अचिन्हित inner, पूर्णाङ्क delta, अचिन्हित item_गिनती_2)
  {
    यदि (outer >= vars.length) प्रत्यागम;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    यदि (!tuple.indices.resize (item_गिनती_2) ||
        !tuple.deltas_x.resize (item_गिनती_2)) प्रत्यागम;
    प्रति (अचिन्हित i = 0; i < item_गिनती_2; i++)
    {
      tuple.indices.arrayZ[i] = सत्य;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (भिन्न) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  तार्किक build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, अचिन्हित> all_regions;
    hb_hashmap_t<region_t, अचिन्हित> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    प्रति (ध्रुव tuple_variations_t& sub_table : vars)
    {
      प्रति (ध्रुव tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        यदि (!used_regions.has (r))
        {
          तार्किक all_zeros = सत्य;
          प्रति (भिन्न d : tuple.deltas_x)
          {
            पूर्णाङ्क delta = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) d));
            यदि (delta != 0)
            {
              all_zeros = असत्य;
              भङ्ग;
            }
          }
          यदि (!all_zeros)
          {
            यदि (!used_regions.set (r, 1))
              प्रत्यागम असत्य;
          }
        }
        यदि (all_regions.has (r))
          अनुवर्त;
        यदि (!all_regions.set (r, 1))
          प्रत्यागम असत्य;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    यदि (!all_regions || !all_unique_regions) प्रत्यागम सत्य;

    यदि (!region_list.alloc (all_regions.get_population ()))
      प्रत्यागम असत्य;

    अचिन्हित idx = 0;
    /* append the original regions that pre-existed */
    प्रति (ध्रुव स्वतः& r : orig_region_list)
    {
      यदि (!all_regions.has (&r) || !used_regions.has (&r))
        अनुवर्त;

      region_list.push (&r);
      यदि (!region_map.set (&r, idx))
        प्रत्यागम असत्य;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    प्रति (ध्रुव स्वतः& r: all_unique_regions)
    {
      यदि (!all_regions.has (r) || !used_regions.has (r))
        अनुवर्त;
      region_list.push (r);
      यदि (!region_map.set (r, idx))
        प्रत्यागम असत्य;
      all_regions.del (r);
      idx++;
    }
    प्रत्यागम (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  संरचना combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = स्वभाव;
    combined_gain_idx_tuple_t (अचिन्हित gain, अचिन्हित i, अचिन्हित j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      प्रतिज्ञा (gain < 0xFFFFFF);
      प्रतिज्ञा (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    तार्किक चालक < (ध्रुव combined_gain_idx_tuple_t& o)
    {
      प्रत्यागम encoded < o.encoded;
    }

    तार्किक चालक <= (ध्रुव combined_gain_idx_tuple_t& o)
    {
      प्रत्यागम encoded <= o.encoded;
    }

    अचिन्हित idx_1 () ध्रुव { प्रत्यागम (encoded >> 20) & 0xFFFFF; };
    अचिन्हित idx_2 () ध्रुव { प्रत्यागम encoded & 0xFFFFF; };
  };

  तार्किक as_item_varstore (तार्किक optimize=सत्य, तार्किक use_no_variation_idx=सत्य)
  {
    /* return true if no variation data */
    यदि (!region_list) प्रत्यागम सत्य;
    अचिन्हित अंक_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    अचिन्हित total_rows = 0;
    प्रति (अचिन्हित major = 0; major < var_data_अंक_rows.length; major++)
      total_rows += var_data_अंक_rows[major];

    यदि (!delta_rows.resize (total_rows)) प्रत्यागम असत्य;
    /* init all rows to [0]*num_cols */
    प्रति (अचिन्हित i = 0; i < total_rows; i++)
      यदि (!(delta_rows[i].resize (अंक_cols))) प्रत्यागम असत्य;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<अचिन्हित, ध्रुव hb_vector_t<पूर्णाङ्क>*> front_mapping;
    अचिन्हित start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क> *> major_rows;
    hb_hashmap_t<ध्रुव hb_vector_t<पूर्णाङ्क>*, अचिन्हित> delta_rows_map;
    प्रति (अचिन्हित major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ध्रुव tuple_variations_t& tuples = vars[major];
      अचिन्हित अंक_rows = var_data_अंक_rows[major];

      यदि (!अंक_rows) अनुवर्त;

      प्रति (ध्रुव tuple_delta_t& tuple: tuples.tuple_vars)
      {
        यदि (tuple.deltas_x.length != अंक_rows)
          प्रत्यागम असत्य;

        /* skip unused regions */
        अचिन्हित *col_idx;
        यदि (!region_map.has (&(tuple.axis_tuples), &col_idx))
          अनुवर्त;

        प्रति (अचिन्हित i = 0; i < अंक_rows; i++)
        {
          पूर्णाङ्क rounded_delta = hb_clamp_to<पूर्णाङ्क> (roundf ((द्विगुण) tuple.deltas_x[i]));
          पूर्णाङ्क &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          यदि (unlikely (sum < पूर्णांक_न्यून || sum > पूर्णांक_उच्च)) प्रत्यागम असत्य;
          delta = (पूर्णाङ्क) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      प्रति (अचिन्हित minor = 0; minor < अंक_rows; minor++)
      {
	ध्रुव hb_vector_t<पूर्णाङ्क>& row = delta_rows[start_row + minor];
	यदि (use_no_variation_idx)
	{
	  तार्किक all_zeros = सत्य;
	  प्रति (पूर्णाङ्क delta : row)
	  {
	    यदि (delta != 0)
	    {
	      all_zeros = असत्य;
	      भङ्ग;
	    }
	  }
	  यदि (all_zeros)
	    अनुवर्त;
	}

	यदि (!front_mapping.set ((major<<16) + minor, &row))
	  प्रत्यागम असत्य;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	यदि (optimize)
	{
	  यदि (delta_rows_map.has (&row))
	    अनुवर्त;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      यदि (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        प्रत्यागम असत्य;
      यदि (major_rows)
      {
	स्वतः *encoding = encoding_objs.push (std::move (major_rows), अंक_cols);
	यदि (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  प्रत्यागम असत्य;
      }

      start_row += अंक_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    यदि (!optimize)
    {
      encodings = std::move (encoding_objs);
      प्रत्यागम !encodings.in_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.क्विक ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    using item_t = hb_priority_queue_t<combined_gain_idx_tuple_t>::item_t;
    hb_vector_t<item_t> queue_वस्तुएँ;
    अचिन्हित अंक_todos = encoding_objs.length;
    प्रति (अचिन्हित i = 0; i < अंक_todos; i++)
    {
      प्रति (अचिन्हित j = i + 1; j < अंक_todos; j++)
      {
        पूर्णाङ्क combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        यदि (combining_gain > 0)
          queue_वस्तुएँ.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_वस्तुएँ));

    hb_bit_set_t removed_todo_idxes;
    यावत् (queue)
    {
      स्वतः t = queue.निकालो_minimum ().first;
      अचिन्हित i = t.idx_1 ();
      अचिन्हित j = t.idx_2 ();

      यदि (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        अनुवर्त;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      यदि (unlikely (removed_todo_idxes.in_error ())) प्रत्यागम असत्य;

      encoding.merge (other_encoding);
      यदि (unlikely (encoding.in_error ())) प्रत्यागम असत्य;

      प्रति (अचिन्हित idx = 0; idx < encoding_objs.length; idx++)
      {
        यदि (removed_todo_idxes.has (idx)) अनुवर्त;

        ध्रुव delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        यदि (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          प्रति (ध्रुव स्वतः& row : obj.वस्तुएँ)
            encoding.add_row (row);
          यदि (unlikely (encoding.in_error ())) प्रत्यागम असत्य;

          removed_todo_idxes.add (idx);
          यदि (unlikely (removed_todo_idxes.in_error ())) प्रत्यागम असत्य;
          अनुवर्त;
        }

        पूर्णाङ्क combined_gain = encoding.gain_from_merging (obj);
        यदि (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      स्वतः moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      यदि (unlikely (encoding_objs.in_error ())) प्रत्यागम असत्य;
    }

    पूर्णाङ्क अंक_final_encodings = (पूर्णाङ्क) encoding_objs.length - (पूर्णाङ्क) removed_todo_idxes.get_population ();
    यदि (अंक_final_encodings <= 0) प्रत्यागम असत्य;

    यदि (!encodings.alloc (अंक_final_encodings)) प्रत्यागम असत्य;
    प्रति (अचिन्हित i = 0; i < encoding_objs.length; i++)
    {
      यदि (removed_todo_idxes.has (i)) अनुवर्त;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    प्रत्यागम compile_varidx_map (front_mapping);
  }

  गुप्त:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  तार्किक compile_varidx_map (ध्रुव hb_hashmap_t<अचिन्हित, ध्रुव hb_vector_t<पूर्णाङ्क>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ध्रुव hb_vector_t<पूर्णाङ्क>*, अचिन्हित> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    प्रति (अचिन्हित i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      यदि (encoding.is_empty () || encoding.in_error ())
        प्रत्यागम असत्य;

      अचिन्हित अंक_rows = encoding.वस्तुएँ.length;
      अचिन्हित अंक_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.वस्तुएँ.क्विक (_cmp_row);

      प्रति (अचिन्हित start = 0; start < अंक_rows; start += 0xFFFFu)
      {
        अचिन्हित chunk_len = hb_min (अंक_rows - start, 0xFFFFu);
        hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क>*> rows;

        यदि (!rows.alloc (chunk_len))
          प्रत्यागम असत्य;

        अचिन्हित major = split_encodings.length;
        प्रति (अचिन्हित minor = 0; minor < chunk_len; minor++)
        {
          ध्रुव hb_vector_t<पूर्णाङ्क> *row = encoding.वस्तुएँ.arrayZ[start + minor];
          rows.push (row);
          यदि (!back_mapping.set (row, (major << 16) + minor))
            प्रत्यागम असत्य;
        }

        स्वतः *split = split_encodings.push (std::move (rows), अंक_cols);
        यदि (unlikely (split_encodings.in_error () || split->in_error ()))
          प्रत्यागम असत्य;
      }
    }

    encodings = std::move (split_encodings);
    यदि (encodings.in_error () || back_mapping.in_error ())
      प्रत्यागम असत्य;

    प्रति (स्वतः _ : front_mapping.iter ())
    {
      अचिन्हित old_varidx = _.first;
      अचिन्हित *new_varidx;
      यदि (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      अन्यथा
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    प्रत्यागम !varidx_map.in_error ();
  }

  स्थिर पूर्णाङ्क _cmp_row (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ध्रुव hb_vector_t<पूर्णाङ्क>** a = (ध्रुव hb_vector_t<पूर्णाङ्क>**) pa;
    ध्रुव hb_vector_t<पूर्णाङ्क>** b = (ध्रुव hb_vector_t<पूर्णाङ्क>**) pb;

    प्रति (अचिन्हित i = 0; i < (*b)->length; i++)
    {
      पूर्णाङ्क va = (*a)->arrayZ[i];
      पूर्णाङ्क vb = (*b)->arrayZ[i];
      यदि (va != vb)
        प्रत्यागम va < vb ? -1 : 1;
    }
    प्रत्यागम 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

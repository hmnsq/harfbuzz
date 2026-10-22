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
  माप_प्रकार get_size (अचिन्हित axis_मोजणी_times_2) स्थिरांक
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    अचिन्हित ti = tupleIndex;
    जर (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      अचिन्हित मोजणी = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      परत hb_unsigned_mul_add_saturate (मोजणी, axis_मोजणी_times_2, min_size);
    }
    परत min_size;
  }

  अचिन्हित get_data_size () स्थिरांक { परत varDataSize; }

  स्थिरांक TupleVariationHeader &get_next (अचिन्हित axis_मोजणी_times_2) स्थिरांक
  { परत StructAtOffset<TupleVariationHeader> (हे, get_size (axis_मोजणी_times_2)); }

  तार्किक unpack_axis_tuples (अचिन्हित axis_मोजणी,
                           स्थिरांक hb_array_t<स्थिरांक F2DOT14> shared_tuples,
                           स्थिरांक hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) स्थिरांक
  {
    स्थिरांक F2DOT14 *peak_tuple = nullptr;
    जर (has_peak ())
      peak_tuple = get_peak_tuple (axis_मोजणी);
    अन्यथा
    {
      अचिन्हित पूर्णांक index = get_index ();
      जर (unlikely ((index + 1) * axis_मोजणी > shared_tuples.length))
        परत असत्य;
      peak_tuple = shared_tuples.sub_array (axis_मोजणी * index, axis_मोजणी).arrayZ;
    }

    स्थिरांक F2DOT14 *start_tuple = nullptr;
    स्थिरांक F2DOT14 *end_tuple = nullptr;
    तार्किक has_interm = has_intermediate ();

    जर (has_interm)
    {
      start_tuple = get_start_tuple (axis_मोजणी);
      end_tuple = get_end_tuple (axis_मोजणी);
    }

    साठी (अचिन्हित i = 0; i < axis_मोजणी; i++)
    {
      दशांश peak = peak_tuple[i].to_float ();
      जर (peak == 0.f) सुरूठेवा;

      hb_tag_t *axis_tag;
      जर (!axes_old_index_tag_map->has (i, &axis_tag))
        परत असत्य;

      दशांश start, end;
      जर (has_interm)
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

    परत सत्य;
  }

  HB_ALWAYS_INLINE
  दशांश calculate_scalar (hb_array_t<स्थिरांक पूर्णांक> coords, अचिन्हित पूर्णांक coord_मोजणी,
			   स्थिरांक hb_array_t<स्थिरांक F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) स्थिरांक
  {
    अचिन्हित tuple_index = tupleIndex;
    अचिन्हित पूर्णांक index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    तार्किक plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    जर (likely (plain_shared_tuple))
    {
      दशांश scalar;
      जर (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	परत scalar;
    }

    स्थिरांक F2DOT14 *peak_tuple;

    तार्किक has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    जर (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_मोजणी);
      shared_tuple_scalar_cache = nullptr;
    }
    अन्यथा
    {
      दशांश scalar;
      जर (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        जर (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	अन्यथा
	  परत scalar;
      }

      जर (unlikely ((index + 1) * coord_मोजणी > shared_tuples.length))
        परत 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_मोजणी * index);

    }

    स्थिरांक F2DOT14 *start_tuple = nullptr;
    स्थिरांक F2DOT14 *end_tuple = nullptr;

    जर (has_interm)
    {
      start_tuple = get_start_tuple (coord_मोजणी);
      end_tuple = get_end_tuple (coord_मोजणी);
    }

    दशांश scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#जर HB_FAST_अंक_ACCESS
    तार्किक skip = coord_मोजणी >= 16;
#endif
#endif
    साठी (अचिन्हित पूर्णांक i = 0; i < coord_मोजणी; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#जर HB_FAST_अंक_ACCESS
      जर (skip)
      {
	पर्यंत (i + 4 <= coord_मोजणी && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	पर्यंत (i < coord_मोजणी && peak_tuple[i].to_int () == 0)
	  i += 1;
	जर (i >= coord_मोजणी)
	  थांबा;
      }
#endif
#endif

      पूर्णांक peak = peak_tuple[i].to_int ();
      जर (!peak) सुरूठेवा;

      पूर्णांक v = coords[i];
      जर (!v) { scalar = 0.f; थांबा; }
      जर (v == peak) सुरूठेवा;

      जर (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        पूर्णांक start = start_tuple[i].to_int ();
        पूर्णांक end = end_tuple[i].to_int ();
        जर (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) सुरूठेवा;
        जर (v < start || v > end) { scalar = 0.f; थांबा; }
        जर (v < peak)
        { जर (peak != start) scalar *= (दशांश) (v - start) / (peak - start); }
        अन्यथा
        { जर (peak != end) scalar *= (दशांश) (end - v) / (end - peak); }
      }
      अन्यथा जर (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; थांबा; }
      अन्यथा
        scalar *= (दशांश) v / peak;
    }
    जर (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    परत scalar;
  }

  तार्किक           has_peak () स्थिरांक { परत tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  तार्किक   has_intermediate () स्थिरांक { परत tupleIndex & TupleIndex::IntermediateRegion; }
  तार्किक has_private_points () स्थिरांक { परत tupleIndex & TupleIndex::PrivatePointNumbers; }
  अचिन्हित      get_index () स्थिरांक { परत tupleIndex & TupleIndex::TupleIndexMask; }
  स्थिर constexpr अचिन्हित max_shared_tuple_मोजणी = 0x1000u;

  संरक्षित:
  संरचना TupleIndex : HBUINT16
  {
    गणना Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& चालक = (uint16_t i) { HBUINT16::चालक= (i); परत *हे; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<स्थिरांक F2DOT14> get_all_tuples (अचिन्हित axis_मोजणी) स्थिरांक
  { परत StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_मोजणी); }
  स्थिरांक F2DOT14* get_all_tuples_base (अचिन्हित axis_मोजणी) स्थिरांक
  { परत StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  स्थिरांक F2DOT14* get_peak_tuple (अचिन्हित axis_मोजणी) स्थिरांक
  { परत get_all_tuples_base (axis_मोजणी); }
  स्थिरांक F2DOT14* get_start_tuple (अचिन्हित axis_मोजणी) स्थिरांक
  { परत get_all_tuples_base (axis_मोजणी) + has_peak () * axis_मोजणी; }
  स्थिरांक F2DOT14* get_end_tuple (अचिन्हित axis_मोजणी) स्थिरांक
  { परत get_all_tuples_base (axis_मोजणी) + has_peak () * axis_मोजणी + axis_मोजणी; }

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
  hb_vector_t<पूर्णांक> rounded_x_deltas;
  hb_vector_t<पूर्णांक> rounded_y_deltas;
  hb_vector_t<दशांश> opt_deltas_x;
  hb_vector_t<दशांश> opt_deltas_y;
  hb_vector_t<अचिन्हित अक्षर> opt_point_data;
  hb_vector_t<अचिन्हित अक्षर> opt_deltas_data;
  hb_vector_t<अचिन्हित अक्षर> point_data;
  hb_vector_t<अचिन्हित अक्षर> deltas_data;
  hb_vector_t<पूर्णांक> rounded_deltas;
};

संरचना tuple_delta_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;  // Watch out when adding new members!

  सार्वजनिक:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<तार्किक> indices;

  hb_vector_t<दशांश> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<दशांश> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<अचिन्हित अक्षर> compiled_tuple_header;
  hb_vector_t<अचिन्हित अक्षर> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (स्थिरांक tuple_delta_t& o) = पूर्वनिर्धारित;
  tuple_delta_t& चालक = (स्थिरांक tuple_delta_t& o) = पूर्वनिर्धारित;

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
  { hb_swap (*हे, o); }

  tuple_delta_t& चालक = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*हे, o);
    परत *हे;
  }

  शून्य copy_from (स्थिरांक tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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
  { परत axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& चालक += (स्थिरांक tuple_delta_t& o)
  {
    अचिन्हित अंक = indices.length;
    साठी (अचिन्हित i = 0; i < अंक; i++)
    {
      जर (indices.arrayZ[i])
      {
        जर (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          जर (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      अन्यथा
      {
        जर (!o.indices.arrayZ[i]) सुरूठेवा;
        indices.arrayZ[i] = सत्य;
        deltas_x[i] = o.deltas_x[i];
        जर (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    परत *हे;
  }

  tuple_delta_t& चालक *= (दशांश scalar)
  {
    जर (scalar == 1.0f)
      परत *हे;

    अचिन्हित अंक = indices.length;
    जर (deltas_y)
      साठी (अचिन्हित i = 0; i < अंक; i++)
      {
	जर (!indices.arrayZ[i]) सुरूठेवा;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    अन्यथा
      साठी (अचिन्हित i = 0; i < अंक; i++)
      {
	जर (!indices.arrayZ[i]) सुरूठेवा;
	deltas_x[i] *= scalar;
      }
    परत *हे;
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
    जर (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*हे));
      परत;
    }

    जर ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->मधला && tent->मधला <= tent->maximum))
      परत;

    जर (tent->मधला == 0.f)
    {
      out.push (std::move (*हे));
      परत;
    }

    जर (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.मधला &&
          axis_limit.मधला <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      परत;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    साठी (अचिन्हित i = 0; i < solutions.length; i++)
    {
      स्वतः &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      जर (i < solutions.length - 1)
	new_var.copy_from (*हे, pool);
      अन्यथा
	new_var = std::move (*हे);

      जर (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      अन्यथा
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  तार्किक compile_coords (स्थिरांक hb_map_t& axes_index_map,
		       स्थिरांक hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    अचिन्हित cur_axis_मोजणी = axes_index_map.get_population ();
    जर (pool)
    {
      जर (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_मोजणी)))
	परत असत्य;
    }
    अन्यथा जर (unlikely (!compiled_peak_coords.resize (cur_axis_मोजणी)))
      परत असत्य;

    hb_array_t<F2DOT14> start_coords, end_coords;

    अचिन्हित orig_axis_मोजणी = axes_old_index_tag_map.get_population ();
    अचिन्हित j = 0;
    साठी (अचिन्हित i = 0; i < orig_axis_मोजणी; i++)
    {
      जर (!axes_index_map.has (i))
        सुरूठेवा;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      जर (axis_tuples.has (axis_tag, &coords))
      {
	दशांश min_val = coords->minimum;
	दशांश val = coords->मधला;
	दशांश max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	जर (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  जर (!compiled_interm_coords)
	  {
	    जर (pool)
	    {
	      जर (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_मोजणी)))
		परत असत्य;
	    }
	    अन्यथा जर (unlikely (!compiled_interm_coords.resize (2 * cur_axis_मोजणी)))
	      परत असत्य;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_मोजणी);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_मोजणी);

	    साठी (अचिन्हित k = 0; k < j; k++)
	    {
	      चिन्हित peak = compiled_peak_coords.arrayZ[k].to_int ();
	      जर (!peak) सुरूठेवा;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	जर (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    परत !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  तार्किक compile_tuple_var_header (स्थिरांक hb_map_t& axes_index_map,
                                 अचिन्हित points_data_length,
                                 स्थिरांक hb_map_t& axes_old_index_tag_map,
                                 स्थिरांक hb_hashmap_t<स्थिरांक hb_vector_t<F2DOT14>*, अचिन्हित>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    जर (!compiled_deltas) परत सत्य;

    अचिन्हित cur_axis_मोजणी = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    अचिन्हित alloc_len = 3 * cur_axis_मोजणी * (F2DOT14::static_size) + 4;
    जर (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, असत्य))) परत असत्य;

    अचिन्हित flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    जर (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    अचिन्हित peak_मोजणी = 0;
    अचिन्हित *shared_tuple_idx;
    जर (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    अन्यथा
    {
      peak_मोजणी = encode_peak_coords(coords, flag);
      जर (!peak_मोजणी) परत असत्य;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    अचिन्हित interim_मोजणी = encode_interm_coords (coords.sub_array (peak_मोजणी), flag);

    /* pointdata length = 0 implies "use shared points" */
    जर (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    अचिन्हित serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    अचिन्हित total_header_len = 4 + (peak_मोजणी + interim_मोजणी) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    परत सत्य;
  }

  अचिन्हित encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               अचिन्हित& flag) स्थिरांक
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * आकार (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    परत compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  अचिन्हित encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 अचिन्हित& flag) स्थिरांक
  {
    जर (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * आकार (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    परत compiled_interm_coords.length;
  }

  तार्किक compile_deltas (hb_vector_t<पूर्णांक> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { परत compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  स्थिर तार्किक compile_deltas (hb_array_t<स्थिरांक तार्किक> point_indices,
			      hb_array_t<स्थिरांक दशांश> x_deltas,
			      hb_array_t<स्थिरांक दशांश> y_deltas,
			      hb_vector_t<अचिन्हित अक्षर> &compiled_deltas, /* OUT */
			      hb_vector_t<पूर्णांक> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    जर (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      परत असत्य;

    अचिन्हित j = 0;
    साठी (अचिन्हित i = 0; i < point_indices.length; i++)
    {
      जर (!point_indices[i]) सुरूठेवा;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<पूर्णांक> (roundf ((द्विगुण) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    जर (!rounded_deltas) परत सत्य;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    अचिन्हित alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    जर (y_deltas)
      alloc_len *= 2;

    जर (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, असत्य))) परत असत्य;

    अचिन्हित encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    जर (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      अचिन्हित j = 0;
      साठी (अचिन्हित idx = 0; idx < point_indices.length; idx++)
      {
        जर (!point_indices[idx]) सुरूठेवा;
        पूर्णांक rounded_delta = hb_clamp_to<पूर्णांक> (roundf ((द्विगुण) y_deltas.arrayZ[idx]));

        जर (j >= rounded_deltas.length) परत असत्य;

        rounded_deltas[j++] = rounded_delta;
      }

      जर (j != rounded_deltas.length) परत असत्य;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    परत सत्य;
  }

  स्थिर अचिन्हित compile_deltas (hb_array_t<अचिन्हित अक्षर> encoded_bytes,
				  hb_array_t<स्थिरांक पूर्णांक> deltas)
  {
    परत Tuple_मूल्ये::compile_unsafe (deltas, encoded_bytes);
  }

  तार्किक calc_inferred_deltas (स्थिरांक contour_point_vector_t& orig_points,
			     hb_vector_t<अचिन्हित> &scratch)
  {
    अचिन्हित point_मोजणी = orig_points.length;
    जर (point_मोजणी != indices.length)
      परत असत्य;

    अचिन्हित ref_मोजणी_2 = 0;

    hb_vector_t<अचिन्हित> &end_points = scratch.reset ();

    साठी (अचिन्हित i = 0; i < point_मोजणी; i++)
    {
      ref_मोजणी_2 += indices.arrayZ[i];
      जर (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    जर (ref_मोजणी_2 == point_मोजणी)
      परत सत्य;
    जर (unlikely (end_points.in_error ())) परत असत्य;

    hb_bit_set_t inferred_idxes;
    अचिन्हित start_point = 0;
    साठी (अचिन्हित end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      अचिन्हित unref_मोजणी = 0;
      साठी (अचिन्हित i = start_point; i < end_point + 1; i++)
        unref_मोजणी += indices.arrayZ[i];
      unref_मोजणी = (end_point - start_point + 1) - unref_मोजणी;

      अचिन्हित j = start_point;
      जर (unref_मोजणी == 0 || unref_मोजणी > end_point - start_point)
        जा no_more_gaps;
      साठी (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        अचिन्हित पूर्णांक prev, next, i;
        साठी (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          जर (indices.arrayZ[i] && !indices.arrayZ[j]) थांबा;
        }
        prev = j = i;
        साठी (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          जर (!indices.arrayZ[i] && indices.arrayZ[j]) थांबा;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        साठी (;;)
        {
          i = next_index (i, start_point, end_point);
          जर (i == next) थांबा;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          जर (--unref_मोजणी == 0) जा no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    साठी (अचिन्हित i = 0; i < point_मोजणी; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      जर ( !indices[i])
      {
        जर (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = सत्य;
      }
    }
    परत सत्य;
  }

  तार्किक optimize (स्थिरांक contour_point_vector_t& contour_points,
                 तार्किक is_composite,
		 optimize_scratch_t &scratch,
                 द्विगुण tolerance = 0.5 + 1e-10)
  {
    अचिन्हित मोजणी = contour_points.length;
    जर (deltas_x.length != मोजणी ||
        deltas_y.length != मोजणी)
      परत असत्य;

    hb_vector_t<तार्किक> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<पूर्णांक> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<पूर्णांक> &rounded_y_deltas = scratch.rounded_y_deltas;

    जर (unlikely (!rounded_x_deltas.resize_dirty  (मोजणी) ||
                  !rounded_y_deltas.resize_dirty  (मोजणी)))
      परत असत्य;

    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<पूर्णांक> (roundf ((द्विगुण) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<पूर्णांक> (roundf ((द्विगुण) deltas_y.arrayZ[i]));
    }

    जर (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      परत असत्य;

    अचिन्हित ref_मोजणी_2 = 0;
    साठी (तार्किक ref_flag : opt_indices)
       ref_मोजणी_2 += ref_flag;

    जर (ref_मोजणी_2 == मोजणी) परत सत्य;

    hb_vector_t<दशांश> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<दशांश> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    तार्किक is_comp_glyph_wo_deltas = (is_composite && ref_मोजणी_2 == 0);
    जर (is_comp_glyph_wo_deltas)
    {
      जर (unlikely (!opt_deltas_x.resize (मोजणी) ||
                    !opt_deltas_y.resize (मोजणी)))
        परत असत्य;

      opt_indices.arrayZ[0] = सत्य;
      साठी (अचिन्हित i = 1; i < मोजणी; i++)
        opt_indices.arrayZ[i] = असत्य;
    }

    hb_vector_t<अचिन्हित अक्षर> &opt_point_data = scratch.opt_point_data.reset ();
    जर (!compile_point_set (opt_indices, opt_point_data))
      परत असत्य;
    hb_vector_t<अचिन्हित अक्षर> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    जर (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      परत असत्य;

    hb_vector_t<अचिन्हित अक्षर> &point_data = scratch.point_data.reset ();
    जर (!compile_point_set (indices, point_data))
      परत असत्य;
    hb_vector_t<अचिन्हित अक्षर> &deltas_data = scratch.deltas_data.reset ();
    जर (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      परत असत्य;

    जर (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      जर (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    परत !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  स्थिर तार्किक compile_point_set (स्थिरांक hb_vector_t<तार्किक> &point_indices,
                                 hb_vector_t<अचिन्हित अक्षर>& compiled_points /* OUT */)
  {
    अचिन्हित अंक_points = 0;
    साठी (तार्किक i : point_indices)
      जर (i) अंक_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    जर (!अंक_points) परत सत्य;

    अचिन्हित indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    जर (अंक_points == indices_length)
      परत compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    अचिन्हित अंक_bytes = 2 + 3 *अंक_points;
    जर (unlikely (!compiled_points.resize_dirty  (अंक_bytes)))
      परत असत्य;

    अचिन्हित pos = 0;
    /* binary data starts with the total number of reference points */
    जर (अंक_points < 0x80)
      compiled_points.arrayZ[pos++] = अंक_points;
    अन्यथा
    {
      compiled_points.arrayZ[pos++] = ((अंक_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = अंक_points & 0xFF;
    }

    स्थिरांक अचिन्हित max_run_length = 0x7F;
    अचिन्हित i = 0;
    अचिन्हित last_मूल्य = 0;
    अचिन्हित अंक_encoded = 0;
    पर्यंत (i < indices_length && अंक_encoded < अंक_points)
    {
      अचिन्हित run_length = 0;
      अचिन्हित header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      तार्किक use_byte_encoding = असत्य;
      तार्किक new_run = सत्य;
      पर्यंत (i < indices_length && अंक_encoded < अंक_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        पर्यंत (i < indices_length && !point_indices[i])
          i++;

        जर (i >= indices_length) थांबा;

        अचिन्हित cur_मूल्य = i;
        अचिन्हित delta = cur_मूल्य - last_मूल्य;

        जर (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = असत्य;
        }

        जर (use_byte_encoding && delta > 0xFF)
          थांबा;

        जर (use_byte_encoding)
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

      जर (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      अन्यथा
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    परत compiled_points.resize_dirty  (pos);
  }

  स्थिर दशांश infer_delta (दशांश target_val, दशांश prev_val, दशांश next_val, दशांश prev_delta, दशांश next_delta)
  {
    जर (prev_val == next_val)
      परत (prev_delta == next_delta) ? prev_delta : 0.f;
    अन्यथा जर (target_val <= hb_min (prev_val, next_val))
      परत (prev_val < next_val) ? prev_delta : next_delta;
    अन्यथा जर (target_val >= hb_max (prev_val, next_val))
      परत (prev_val > next_val) ? prev_delta : next_delta;

    दशांश r = (target_val - prev_val) / (next_val - prev_val);
    परत prev_delta + r * (next_delta - prev_delta);
  }

  स्थिर अचिन्हित पूर्णांक next_index (अचिन्हित पूर्णांक i, अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
  { परत (i >= end) ? start : (i + 1); }
};

ढाँचा <typename OffType = HBUINT16>
संरचना TupleVariationData
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (हे));
  }

  माप_प्रकार get_size (अचिन्हित axis_मोजणी_times_2) स्थिरांक
  {
    माप_प्रकार total_size = min_size;
    अचिन्हित मोजणी = tuple_Var_मोजणी.get_मोजणी ();
    स्थिरांक TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_मोजणी_times_2),
					     (माप_प्रकार) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_मोजणी_times_2);
    }

    परत total_size;
  }

  स्थिरांक TupleVariationHeader &get_tuple_var_header (शून्य) स्थिरांक
  { परत StructAfter<TupleVariationHeader> (data); }

  संरचना tuple_iterator_t;
  संरचना tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    खाजगी:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<स्थिरांक hb_vector_t<तार्किक>*, hb_vector_t<अचिन्हित अक्षर>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<स्थिरांक hb_vector_t<तार्किक>*, अचिन्हित> point_set_मोजणी_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<अचिन्हित अक्षर> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    अचिन्हित compiled_byte_size = 0;
    तार्किक needs_padding = असत्य;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    तार्किक is_composite = असत्य;

    सार्वजनिक:
    tuple_variations_t () = पूर्वनिर्धारित;
    tuple_variations_t (स्थिरांक tuple_variations_t&) = delete;
    tuple_variations_t& चालक=(स्थिरांक tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = पूर्वनिर्धारित;
    tuple_variations_t& चालक=(tuple_variations_t&&) = पूर्वनिर्धारित;
    ~tuple_variations_t () = पूर्वनिर्धारित;

    explicit चालक तार्किक () स्थिरांक { परत तार्किक (tuple_vars); }
    अचिन्हित get_var_मोजणी () स्थिरांक
    {
      अचिन्हित मोजणी = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      साठी (स्वतः& tuple: tuple_vars)
        जर (tuple.compiled_deltas) मोजणी++;

      जर (shared_points_bytes && shared_points_bytes->length)
        मोजणी |= Tuple_Var_मोजणी::SharedPointNumbers;
      परत मोजणी;
    }

    अचिन्हित get_compiled_byte_size () स्थिरांक
    { परत compiled_byte_size; }

    तार्किक create_from_tuple_var_data (tuple_iterator_t iterator,
                                     अचिन्हित tuple_var_मोजणी,
                                     अचिन्हित point_मोजणी,
                                     तार्किक is_gvar,
                                     स्थिरांक hb_map_t *axes_old_index_tag_map,
                                     स्थिरांक hb_vector_t<अचिन्हित> &shared_indices,
                                     स्थिरांक hb_array_t<स्थिरांक F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     तार्किक is_composite_glyph = असत्य)
    {
      hb_vector_t<अचिन्हित> private_indices;
      hb_vector_t<पूर्णांक> deltas_x;
      hb_vector_t<पूर्णांक> deltas_y;
      करा
      {
        स्थिरांक HBUINT8 *p = iterator.get_serialized_data ();
        अचिन्हित पूर्णांक length = iterator.current_tuple->get_data_size ();
        जर (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          परत असत्य;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        जर (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_मोजणी (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          परत असत्य;

	private_indices.reset ();
        तार्किक has_private_points = iterator.current_tuple->has_private_points ();
        स्थिरांक HBUINT8 *end = p + length;
        जर (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          परत असत्य;

        स्थिरांक hb_vector_t<अचिन्हित> &indices = has_private_points ? private_indices : shared_indices;
        तार्किक apply_to_all = (indices.length == 0);
        अचिन्हित अंक_deltas = apply_to_all ? point_मोजणी : indices.length;

        जर (unlikely (!deltas_x.resize_dirty  (अंक_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          परत असत्य;

        जर (is_gvar)
        {
          जर (unlikely (!deltas_y.resize_dirty  (अंक_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            परत असत्य;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        जर (unlikely (!var.indices.allocate_from_pool (pool, point_मोजणी) ||
                      !var.deltas_x.allocate_from_pool (pool, point_मोजणी, असत्य)))
          परत असत्य;

        जर (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_मोजणी, असत्य)))
          परत असत्य;

        साठी (अचिन्हित i = 0; i < अंक_deltas; i++)
        {
          अचिन्हित idx = apply_to_all ? i : indices[i];
          जर (idx >= point_मोजणी) सुरूठेवा;
          var.indices[idx] = सत्य;
          var.deltas_x[idx] = deltas_x[i];
          जर (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } पर्यंत (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      परत सत्य;
    }

    तार्किक create_from_item_var_data (स्थिरांक VarData &var_data,
                                    स्थिरांक hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    स्थिरांक hb_map_t& axes_old_index_tag_map,
                                    अचिन्हित& item_मोजणी_2,
                                    स्थिरांक hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      जर (&var_data == &Null (VarData))
        परत सत्य;

      अचिन्हित अंक_regions = var_data.get_region_index_मोजणी ();
      जर (!tuple_vars.alloc (अंक_regions)) परत असत्य;

      item_मोजणी_2 = inner_map ? inner_map->get_population () : var_data.get_item_मोजणी ();
      जर (!item_मोजणी_2) परत सत्य;
      अचिन्हित row_size = var_data.get_row_size ();
      स्थिरांक HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      साठी (अचिन्हित r = 0; r < अंक_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        जर (!tuple.deltas_x.resize_dirty  (item_मोजणी_2) ||
            !tuple.indices.resize_dirty  (item_मोजणी_2))
          परत असत्य;

        साठी (अचिन्हित i = 0; i < item_मोजणी_2; i++)
        {
          tuple.indices.arrayZ[i] = सत्य;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        अचिन्हित region_index = var_data.get_region_index (r);
        जर (region_index >= regions.length) परत असत्य;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      परत !tuple_vars.in_error ();
    }

    खाजगी:
    स्थिर पूर्णांक _cmp_axis_tag (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
    {
      स्थिरांक hb_tag_t *a = (स्थिरांक hb_tag_t*) pa;
      स्थिरांक hb_tag_t *b = (स्थिरांक hb_tag_t*) pb;
      परत (पूर्णांक)(*a) - (पूर्णांक)(*b);
    }

    तार्किक change_tuple_variations_axis_limits (स्थिरांक hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              स्थिरांक hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      जर (!axis_tags.alloc (normalized_axes_location.get_population ()))
        परत असत्य;
      साठी (स्वतः t : normalized_axes_location.किल्ल्या ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.क्विक (_cmp_axis_tag);
      साठी (स्वतः axis_tag : axis_tags)
      {
        Triple *axis_limit;
        जर (!normalized_axes_location.has (axis_tag, &axis_limit))
          परत असत्य;
        TripleDistances axis_triple_distances{1.f, 1.f};
        जर (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        साठी (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          जर (!out) सुरूठेवा;

          अचिन्हित new_len = new_vars.length + out.length;

          जर (unlikely (!new_vars.alloc (new_len, असत्य)))
            परत असत्य;

          साठी (अचिन्हित i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      परत सत्य;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    तार्किक merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      जर (unlikely (!new_vars.alloc (tuple_vars.length)))
	परत असत्य;
      hb_hashmap_t<स्थिरांक hb_hashmap_t<hb_tag_t, Triple>*, अचिन्हित> m;
      साठी (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        जर (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          जर (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            परत असत्य;
          सुरूठेवा;
        }

        अचिन्हित *idx;
        जर (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        अन्यथा
        {
	  स्वतः *new_var = new_vars.push ();
	  जर (unlikely (new_vars.in_error ()))
	    परत असत्य;
          hb_swap (*new_var, var);
          जर (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            परत असत्य;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      परत सत्य;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    तार्किक compile_all_point_sets ()
    {
      साठी (स्थिरांक स्वतः& tuple: tuple_vars)
      {
        स्थिरांक hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        जर (point_data_map.has (points_set))
        {
          अचिन्हित *मोजणी;
          जर (unlikely (!point_set_मोजणी_map.has (points_set, &मोजणी) ||
                        !point_set_मोजणी_map.set (points_set, (*मोजणी) + 1)))
            परत असत्य;
          सुरूठेवा;
        }

        hb_vector_t<अचिन्हित अक्षर> compiled_point_data;
        जर (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          परत असत्य;

        जर (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_मोजणी_map.set (points_set, 1))
          परत असत्य;
      }
      परत सत्य;
    }

    /* find shared points set which saves most bytes */
    शून्य find_shared_points ()
    {
      अचिन्हित max_saved_bytes = 0;

      साठी (स्थिरांक स्वतः& _ : point_data_map.iter_ref ())
      {
        स्थिरांक hb_vector_t<तार्किक>* points_set = _.first;
        अचिन्हित data_length = _.second.length;
        जर (!data_length) सुरूठेवा;
        अचिन्हित *मोजणी;
        जर (unlikely (!point_set_मोजणी_map.has (points_set, &मोजणी) ||
                      *मोजणी <= 1))
        {
          shared_points_bytes = nullptr;
          परत;
        }

        अचिन्हित saved_bytes = data_length * ((*मोजणी) -1);
        जर (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    तार्किक calc_inferred_deltas (स्थिरांक contour_point_vector_t& contour_points,
			       hb_vector_t<अचिन्हित> &scratch)
    {
      साठी (tuple_delta_t& var : tuple_vars)
        जर (!var.calc_inferred_deltas (contour_points, scratch))
          परत असत्य;

      परत सत्य;
    }

    तार्किक iup_optimize (स्थिरांक contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      साठी (tuple_delta_t& var : tuple_vars)
      {
        जर (!var.optimize (contour_points, is_composite, scratch))
          परत असत्य;
      }
      परत सत्य;
    }

    सार्वजनिक:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    शून्य cull_unreachable (स्थिरांक hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      अचिन्हित j = 0;
      साठी (अचिन्हित i = 0; i < tuple_vars.length; i++)
      {
	जर (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  सुरूठेवा;
	जर (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    तार्किक instantiate (स्थिरांक hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      स्थिरांक hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      तार्किक optimize = असत्य)
    {
      जर (!tuple_vars) परत सत्य;
      जर (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        परत असत्य;
      /* compute inferred deltas only for gvar */
      जर (contour_points)
      {
	hb_vector_t<अचिन्हित> scratch;
	जर (!calc_inferred_deltas (*contour_points, scratch))
	  परत असत्य;
      }

      /* if iup delta opt is on, contour_points can't be null */
      जर (optimize && !contour_points)
        परत असत्य;

      जर (!merge_tuple_variations (optimize ? contour_points : nullptr))
        परत असत्य;

      जर (optimize && !iup_optimize (*contour_points, scratch)) परत असत्य;
      परत !tuple_vars.in_error ();
    }

    तार्किक compile_bytes (स्थिरांक hb_map_t& axes_index_map,
                        स्थिरांक hb_map_t& axes_old_index_tag_map,
                        तार्किक use_shared_points,
                        तार्किक is_gvar = असत्य,
                        स्थिरांक hb_hashmap_t<स्थिरांक hb_vector_t<F2DOT14>*, अचिन्हित>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      जर (!tuple_vars)
        परत सत्य;

      // compile points set and store data in hashmap
      जर (!compile_all_point_sets ())
        परत असत्य;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      जर (use_shared_points)
      {
        find_shared_points ();
        जर (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<पूर्णांक> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      साठी (स्वतः& tuple: tuple_vars)
      {
        स्थिरांक hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        hb_vector_t<अचिन्हित अक्षर> *points_data;
        जर (unlikely (!point_data_map.has (points_set, &points_data)))
          परत असत्य;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        जर (!points_data->length)
          सुरूठेवा;
        जर (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          परत असत्य;

        अचिन्हित points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        जर (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          परत असत्य;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      जर (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = सत्य;
        compiled_byte_size += 1;
      }

      परत सत्य;
    }

    तार्किक serialize_var_headers (hb_serialize_context_t *c, अचिन्हित& total_header_len) स्थिरांक
    {
      TRACE_SERIALIZE (हे);
      साठी (स्थिरांक स्वतः& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        जर (c->in_error ()) return_trace (असत्य);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (सत्य);
    }

    तार्किक serialize_var_data (hb_serialize_context_t *c, तार्किक is_gvar) स्थिरांक
    {
      TRACE_SERIALIZE (हे);
      जर (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      साठी (स्थिरांक स्वतः& tuple: tuple_vars)
      {
        स्थिरांक hb_vector_t<तार्किक>* points_set = &(tuple.indices);
        hb_vector_t<अचिन्हित अक्षर> *point_data;
        जर (!point_data_map.has (points_set, &point_data))
          return_trace (असत्य);

        जर (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        जर (c->in_error ()) return_trace (असत्य);
      }

      /* padding for gvar */
      जर (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        जर (!c->embed (pad)) return_trace (असत्य);
      }
      return_trace (सत्य);
    }
  };

  संरचना tuple_iterator_t
  {
    अचिन्हित get_axis_मोजणी () स्थिरांक { परत axis_मोजणी; }

    शून्य init (hb_bytes_t var_data_bytes_, अचिन्हित पूर्णांक axis_मोजणी_3, स्थिरांक शून्य *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_डावा = var_data->tuple_Var_मोजणी.get_मोजणी ();
      axis_मोजणी = axis_मोजणी_3;
      axis_मोजणी_times_2 = axis_मोजणी_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    तार्किक get_shared_indices (hb_vector_t<अचिन्हित पूर्णांक> &shared_indices /* OUT */)
    {
      जर (var_data->has_shared_point_numbers ())
      {
        स्थिरांक HBUINT8 *base = &(table_base+var_data->data);
        स्थिरांक HBUINT8 *p = base;
        जर (!decompile_points (p, shared_indices, (स्थिरांक HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) परत असत्य;
        data_offset = p - base;
      }
      परत सत्य;
    }

    तार्किक is_valid ()
    {
      जर (unlikely (tuples_डावा <= 0))
	परत असत्य;

      current_tuple_size = TupleVariationHeader::min_size;
      जर (unlikely (!var_data_bytes.check_end ((स्थिरांक अक्षर *) current_tuple + current_tuple_size)))
	परत असत्य;

      current_tuple_size = current_tuple->get_size (axis_मोजणी_times_2);
      जर (unlikely (!var_data_bytes.check_end ((स्थिरांक अक्षर *) current_tuple + current_tuple_size)))
	परत असत्य;

      परत सत्य;
    }

    HB_ALWAYS_INLINE
    तार्किक move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_डावा--;
      परत is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    स्थिरांक HBUINT8 *get_serialized_data () स्थिरांक
    { परत &(table_base+var_data->data) + data_offset; }

    खाजगी:
    चिन्हित tuples_डावा;
    स्थिरांक TupleVariationData *var_data;
    अचिन्हित पूर्णांक axis_मोजणी;
    अचिन्हित पूर्णांक axis_मोजणी_times_2;
    अचिन्हित पूर्णांक data_offset;
    अचिन्हित पूर्णांक current_tuple_size;
    स्थिरांक शून्य *table_base;

    सार्वजनिक:
    hb_bytes_t var_data_bytes;
    स्थिरांक TupleVariationHeader *current_tuple;
  };

  स्थिर तार्किक get_tuple_iterator (hb_bytes_t var_data_bytes, अचिन्हित axis_मोजणी,
                                  स्थिरांक शून्य *table_base,
                                  hb_vector_t<अचिन्हित पूर्णांक> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_मोजणी, table_base);
    जर (!iterator->get_shared_indices (shared_indices))
      परत असत्य;
    परत iterator->is_valid ();
  }

  तार्किक has_shared_point_numbers () स्थिरांक { परत tuple_Var_मोजणी.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  तार्किक cull_tuple_variations (hb_bytes_t var_data_bytes,
			      अचिन्हित axis_मोजणी,
			      hb_array_t<स्थिरांक F2DOT14> shared_tuples,
			      स्थिरांक hb_map_t *axes_old_index_tag_map,
			      स्थिरांक hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<अक्षर> &out /* OUT */,
			      तार्किक *changed /* OUT */) स्थिरांक
  {
    *changed = असत्य;

    hb_vector_t<अचिन्हित पूर्णांक> shared_indices;
    tuple_iterator_t iterator;
    जर (!get_tuple_iterator (var_data_bytes, axis_मोजणी, हे,
			     shared_indices, &iterator))
      परत सत्य; /* no tuples or malformed: keep original */

    स्थिरांक अक्षर *bytes_start = var_data_bytes.arrayZ;
    स्थिरांक अक्षर *bytes_end = bytes_start + var_data_bytes.length;
    स्थिरांक अक्षर *serialized_base = (स्थिरांक अक्षर *) &(हे+data);
    स्थिरांक अक्षर *shared_points_end = (स्थिरांक अक्षर *) iterator.get_serialized_data ();
    जर (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      परत सत्य;

    संरचना kept_tuple_t
    {
      स्थिरांक अक्षर *header;
      अचिन्हित header_size;
      स्थिरांक अक्षर *data;
      अचिन्हित data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    अचिन्हित total = 0;
    अचिन्हित kept_headers_size = 0, kept_data_size = 0;
    करा
    {
      स्थिरांक TupleVariationHeader *header = iterator.current_tuple;
      अचिन्हित header_size = header->get_size (axis_मोजणी * 2);
      अचिन्हित data_size = header->get_data_size ();
      स्थिरांक अक्षर *tuple_data = (स्थिरांक अक्षर *) iterator.get_serialized_data ();
      जर (unlikely (tuple_data < serialized_base ||
		    data_size > (अचिन्हित) (bytes_end - tuple_data)))
	परत सत्य; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      जर (!header->unpack_axis_tuples (axis_मोजणी, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	परत सत्य;

      total++;
      जर (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(स्थिरांक अक्षर *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } पर्यंत (iterator.move_to_next ());

    जर (unlikely (kept.in_error ())) परत असत्य;
    जर (kept.length == total) परत सत्य; /* nothing to cull */

    *changed = सत्य;
    जर (!kept.length) परत सत्य; /* everything culled: no variation data */

    अचिन्हित shared_points_size = shared_points_end - serialized_base;
    अचिन्हित data_offset = min_size + kept_headers_size;
    अचिन्हित new_size = data_offset + shared_points_size + kept_data_size;
    जर (unlikely (!out.resize (new_size)))
      परत असत्य;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_मोजणी = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    अक्षर *p = out.arrayZ + min_size;
    साठी (स्थिरांक स्वतः &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    साठी (स्थिरांक स्वतः &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    परत सत्य;
  }

  स्थिर तार्किक decompile_points (स्थिरांक HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<अचिन्हित पूर्णांक> &points /* OUT */,
				स्थिरांक HBUINT8 *end)
  {
    गणना packed_point_flag_t
    {
      POINTS_ARE_शब्द     = 0x80,
      POINT_RUN_मोजणी_MASK = 0x7F
    };

    जर (unlikely (p + 1 > end)) परत असत्य;

    अचिन्हित मोजणी = *p++;
    जर (मोजणी & POINTS_ARE_शब्द)
    {
      जर (unlikely (p + 1 > end)) परत असत्य;
      मोजणी = ((मोजणी & POINT_RUN_मोजणी_MASK) << 8) | *p++;
    }
    जर (unlikely (!points.resize_dirty  (मोजणी))) परत असत्य;

    अचिन्हित n = 0;
    अचिन्हित i = 0;
    पर्यंत (i < मोजणी)
    {
      जर (unlikely (p + 1 > end)) परत असत्य;
      अचिन्हित control = *p++;
      अचिन्हित run_मोजणी = (control & POINT_RUN_मोजणी_MASK) + 1;
      अचिन्हित stop = i + run_मोजणी;
      जर (unlikely (stop > मोजणी)) परत असत्य;
      जर (control & POINTS_ARE_शब्द)
      {
        जर (unlikely (p + run_मोजणी * HBUINT16::static_size > end)) परत असत्य;
        साठी (; i < stop; i++)
        {
          n += *(स्थिरांक HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      अन्यथा
      {
        जर (unlikely (p + run_मोजणी > end)) परत असत्य;
        साठी (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    परत सत्य;
  }

  ढाँचा <typename T>
  HB_ALWAYS_INLINE
  स्थिर तार्किक decompile_deltas (स्थिरांक HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				स्थिरांक HBUINT8 *end,
				तार्किक consume_all = असत्य,
				अचिन्हित start = 0)
  {
    परत Tuple_मूल्ये::decompile (p, deltas, end, consume_all, start);
  }

  तार्किक has_data () स्थिरांक { परत tuple_Var_मोजणी; }

  तार्किक decompile_tuple_variations (अचिन्हित point_मोजणी,
                                   तार्किक is_gvar,
                                   tuple_iterator_t iterator,
                                   स्थिरांक hb_map_t *axes_old_index_tag_map,
                                   स्थिरांक hb_vector_t<अचिन्हित> &shared_indices,
                                   स्थिरांक hb_array_t<स्थिरांक F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   तार्किक is_composite_glyph = असत्य) स्थिरांक
  {
    परत tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_मोजणी,
                                                        point_मोजणी, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  तार्किक is_gvar,
                  स्थिरांक tuple_variations_t& tuple_variations) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    /* empty tuple variations, just return and skip serialization. */
    जर (!tuple_variations) return_trace (सत्य);

    स्वतः *out = c->start_embed (हे);
    जर (unlikely (!c->extend_min (out))) return_trace (असत्य);

    जर (!c->check_assign (out->tuple_Var_मोजणी, tuple_variations.get_var_मोजणी (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (असत्य);

    अचिन्हित total_header_len = 0;

    जर (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (असत्य);

    अचिन्हित data_offset = min_size + total_header_len;
    जर (!is_gvar) data_offset += 4;
    जर (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (असत्य);

    परत tuple_variations.serialize_var_data (c, is_gvar);
  }

  संरक्षित:
  संरचना Tuple_Var_मोजणी : HBUINT16
  {
    मित्र संरचना tuple_variations_t;
    तार्किक has_shared_point_numbers () स्थिरांक { परत ((*हे) & SharedPointNumbers); }
    अचिन्हित पूर्णांक get_मोजणी () स्थिरांक { परत (*हे) & मोजणी_Mask; }
    Tuple_Var_मोजणी& चालक = (uint16_t i) { HBUINT16::चालक= (i); परत *हे; }
    explicit चालक तार्किक () स्थिरांक { परत get_मोजणी (); }

    संरक्षित:
    गणना Flags
    {
      SharedPointNumbers= 0x8000u,
      मोजणी_Mask         = 0x0FFFu
    };
    सार्वजनिक:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_मोजणी tuple_Var_मोजणी;  /* A packed field. The high 4 bits are flags, and the
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
  using region_t = स्थिरांक hb_hashmap_t<hb_tag_t, Triple>*;
  खाजगी:
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
  hb_vector_t<hb_vector_t<पूर्णांक>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  तार्किक has_long = असत्य;

  सार्वजनिक:
  तार्किक has_long_word () स्थिरांक
  { परत has_long; }

  स्थिरांक hb_vector_t<region_t>& get_region_list () स्थिरांक
  { परत region_list; }

  स्थिरांक hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () स्थिरांक
  { परत encodings; }

  स्थिरांक hb_map_t& get_varidx_map () स्थिरांक
  { परत varidx_map; }

  तार्किक add_vardata_encoding_for_testing (hb_vector_t<स्थिरांक hb_vector_t<पूर्णांक>*> &&rows,
                                         अचिन्हित अंक_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), अंक_cols));
    परत !encodings.in_error ();
  }

  तार्किक compile_varidx_map_for_testing (स्थिरांक hb_hashmap_t<अचिन्हित, स्थिरांक hb_vector_t<पूर्णांक>*>& front_mapping)
  { परत compile_varidx_map (front_mapping); }

  तार्किक instantiate (स्थिरांक ItemVariationStore& varStore,
                    स्थिरांक hb_subset_plan_t *plan,
                    तार्किक optimize=सत्य,
                    तार्किक use_no_variation_idx=सत्य,
                    स्थिरांक hb_array_t <स्थिरांक hb_inc_bimap_t> inner_maps = hb_array_t<स्थिरांक hb_inc_bimap_t> (),
                    तार्किक layout_conditions = असत्य)
  {
    जर (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      परत असत्य;
    /* avar2 partial instancing: cull unreachable regions. */
    जर (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      साठी (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    जर (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      परत असत्य;
    जर (layout_conditions &&
        (!instantiate_condition_मूल्ये (plan, varStore) || !build_region_list ()))
      परत असत्य;
    परत as_item_varstore (optimize, use_no_variation_idx);
  }

  तार्किक instantiate_condition_मूल्ये (स्थिरांक hb_subset_plan_t *plan,
                                      स्थिरांक ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    स्वतः &condition_map = plan->layout_condition_idx_मूल्य_map;
    condition_map.clear ();
    जर (!plan->layout_condition_मूल्ये) परत सत्य;
    using condition_मूल्य_t = hb_pair_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णांक>>;
    hb_vector_t<condition_मूल्य_t> conditions;
    साठी (स्वतः condition : plan->layout_condition_मूल्ये.iter ())
      conditions.push (condition);
    जर (unlikely (conditions.in_error ())) परत असत्य;
    // Row order must not depend on the addresses of the source conditions.
    conditions.क्विक ([] (स्थिरांक condition_मूल्य_t &a, स्थिरांक condition_मूल्य_t &b) {
      जर (a.second.first != b.second.first)
        परत a.second.first < b.second.first ? -1 : 1;
      परत a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    अचिन्हित new_outer = 0;
    साठी (अचिन्हित i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      जर (plan->gdef_varstore_inner_maps[i].get_population ())
        जर (!outer_map.set (i, new_outer++)) परत असत्य;
    साठी (स्वतः condition : conditions)
    {
      अचिन्हित old_idx = condition.second.first;
      द्विगुण value = condition.second.second +
                     varStore.get_delta<द्विगुण> (old_idx, plan->normalized_coords.as_array ());
      अचिन्हित old_outer = old_idx >> 16;
      जर (!outer_map.has (old_outer))
        सुरूठेवा;
      अचिन्हित outer = outer_map.get (old_outer);
      अचिन्हित inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      अचिन्हित idx = (outer << 16) | inner;
      जर (outer >= vars.length || inner >= var_data_अंक_rows[outer])
        सुरूठेवा;

      द्विगुण maximum = भ_असल ((द्विगुण) value);
      तार्किक variable = असत्य;
      साठी (स्थिरांक tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        जर (unlikely (inner >= tuple.deltas_x.length)) परत असत्य;
        दशांश delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, भ_असल ((द्विगुण) delta));
      }
      जर (!variable)
      {
        जर (!condition_map.set (condition.first,
                                hb_pair (अचिन्हित (VarIdx::NO_VARIATION), पूर्णांक (value > 0))))
          परत असत्य;
        सुरूठेवा;
      }

      द्विगुण scale = 1.;
      पर्यंत (maximum * scale > पूर्णांक_उच्च)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      पर्यंत (maximum * scale * 2 <= पूर्णांक_उच्च)
      {
        तार्किक integral = (द्विगुण) value * scale == round ((द्विगुण) value * scale);
        साठी (स्थिरांक tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (द्विगुण) tuple.deltas_x[inner] * scale ==
                      round ((द्विगुण) tuple.deltas_x[inner] * scale);
        जर (integral) थांबा;
        scale *= 2;
      }
      पूर्णांक default_मूल्य = (पूर्णांक) round ((द्विगुण) value * scale);
      जर (scale != 1. || default_मूल्य < -32768 || default_मूल्य > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        साठी (स्थिरांक tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          जर (!tuple.deltas_x[inner]) सुरूठेवा;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (सत्य);
          copy.deltas_x.push ((दशांश) round ((द्विगुण) tuple.deltas_x[inner] * scale));
          जर (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) परत असत्य;
          private_row.tuple_vars.push (std::move (copy));
        }
        जर (unlikely (vars.length >= 0xFFFFu)) परत असत्य;
        जर (unlikely (private_row.tuple_vars.in_error ())) परत असत्य;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_अंक_rows.push (1);
        जर (unlikely (vars.in_error () || var_data_अंक_rows.in_error ())) परत असत्य;
        जर (default_मूल्य < -32768 || default_मूल्य > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          पूर्णांक bias = default_मूल्य - hb_clamp (default_मूल्य, -32768, 32767);
          default_मूल्य -= bias;
          पूर्णांक वरचा = bias / 65536 * 65536;
          पूर्णांक parts[] = {वरचा, bias - वरचा};
          साठी (पूर्णांक part : parts)
          {
            जर (!part) सुरूठेवा;
            अचिन्हित मोजणी = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            स्थिरांक स्वतः &tuples = vars[idx >> 16].tuple_vars;
            जर (unlikely (tuples.in_error () || tuples.length != मोजणी + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) परत असत्य;
          }
        }
      }
      जर (!condition_map.set (condition.first, hb_pair (idx, default_मूल्य)))
        परत असत्य;
    }
    परत !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  तार्किक create_from_item_varstore (स्थिरांक ItemVariationStore& varStore,
                                  स्थिरांक hb_map_t& axes_old_index_tag_map,
                                  स्थिरांक hb_array_t <स्थिरांक hb_inc_bimap_t> inner_maps = hb_array_t<स्थिरांक hb_inc_bimap_t> ())
  {
    स्थिरांक VarRegionList& regionList = varStore.get_region_list ();
    जर (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      परत असत्य;

    अचिन्हित अंक_var_data = varStore.get_sub_table_मोजणी ();
    जर (inner_maps && inner_maps.length != अंक_var_data) परत असत्य;
    जर (!vars.alloc (अंक_var_data) ||
        !var_data_अंक_rows.alloc (अंक_var_data)) परत असत्य;

    साठी (अचिन्हित i = 0; i < अंक_var_data; i++)
    {
      जर (inner_maps && !inner_maps.arrayZ[i].get_population ())
          सुरूठेवा;
      tuple_variations_t var_data_tuples;
      अचिन्हित item_मोजणी_2 = 0;
      जर (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_मोजणी_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        परत असत्य;

      var_data_अंक_rows.push (item_मोजणी_2);
      vars.push (std::move (var_data_tuples));
    }
    परत !vars.in_error () && !var_data_अंक_rows.in_error () && vars.length == var_data_अंक_rows.length;
  }

  तार्किक instantiate_tuple_vars (स्थिरांक hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               स्थिरांक hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               तार्किक build_regions = सत्य)
  {
    optimize_scratch_t scratch;
    साठी (tuple_variations_t& tuple_vars : vars)
      जर (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        परत असत्य;

    परत !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  अचिन्हित add_vardata (अचिन्हित item_मोजणी_2)
  {
    vars.push (tuple_variations_t ());
    var_data_अंक_rows.push (item_मोजणी_2);
    परत vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  अचिन्हित get_item_मोजणी (अचिन्हित outer) स्थिरांक
  { परत outer < var_data_अंक_rows.length ? var_data_अंक_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  अचिन्हित duplicate_row (अचिन्हित outer, अचिन्हित inner)
  {
    जर (unlikely (outer >= vars.length ||
                  inner >= var_data_अंक_rows[outer] ||
                  var_data_अंक_rows[outer] >= 0xFFFFu))
      परत (अचिन्हित) -1;
    साठी (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      जर (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        परत (अचिन्हित) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      जर (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      जर (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        परत (अचिन्हित) -1;
    }
    परत var_data_अंक_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  शून्य add_tuple (अचिन्हित outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  अचिन्हित inner, पूर्णांक delta, अचिन्हित item_मोजणी_2)
  {
    जर (outer >= vars.length) परत;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    जर (!tuple.indices.resize (item_मोजणी_2) ||
        !tuple.deltas_x.resize (item_मोजणी_2)) परत;
    साठी (अचिन्हित i = 0; i < item_मोजणी_2; i++)
    {
      tuple.indices.arrayZ[i] = सत्य;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (दशांश) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  तार्किक build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, अचिन्हित> all_regions;
    hb_hashmap_t<region_t, अचिन्हित> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    साठी (स्थिरांक tuple_variations_t& sub_table : vars)
    {
      साठी (स्थिरांक tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        जर (!used_regions.has (r))
        {
          तार्किक all_zeros = सत्य;
          साठी (दशांश d : tuple.deltas_x)
          {
            पूर्णांक delta = hb_clamp_to<पूर्णांक> (roundf ((द्विगुण) d));
            जर (delta != 0)
            {
              all_zeros = असत्य;
              थांबा;
            }
          }
          जर (!all_zeros)
          {
            जर (!used_regions.set (r, 1))
              परत असत्य;
          }
        }
        जर (all_regions.has (r))
          सुरूठेवा;
        जर (!all_regions.set (r, 1))
          परत असत्य;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    जर (!all_regions || !all_unique_regions) परत सत्य;

    जर (!region_list.alloc (all_regions.get_population ()))
      परत असत्य;

    अचिन्हित idx = 0;
    /* append the original regions that pre-existed */
    साठी (स्थिरांक स्वतः& r : orig_region_list)
    {
      जर (!all_regions.has (&r) || !used_regions.has (&r))
        सुरूठेवा;

      region_list.push (&r);
      जर (!region_map.set (&r, idx))
        परत असत्य;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    साठी (स्थिरांक स्वतः& r: all_unique_regions)
    {
      जर (!all_regions.has (r) || !used_regions.has (r))
        सुरूठेवा;
      region_list.push (r);
      जर (!region_map.set (r, idx))
        परत असत्य;
      all_regions.del (r);
      idx++;
    }
    परत (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  संरचना combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = पूर्वनिर्धारित;
    combined_gain_idx_tuple_t (अचिन्हित gain, अचिन्हित i, अचिन्हित j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      प्रतिज्ञा (gain < 0xFFFFFF);
      प्रतिज्ञा (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    तार्किक चालक < (स्थिरांक combined_gain_idx_tuple_t& o)
    {
      परत encoded < o.encoded;
    }

    तार्किक चालक <= (स्थिरांक combined_gain_idx_tuple_t& o)
    {
      परत encoded <= o.encoded;
    }

    अचिन्हित idx_1 () स्थिरांक { परत (encoded >> 20) & 0xFFFFF; };
    अचिन्हित idx_2 () स्थिरांक { परत encoded & 0xFFFFF; };
  };

  तार्किक as_item_varstore (तार्किक optimize=सत्य, तार्किक use_no_variation_idx=सत्य)
  {
    /* return true if no variation data */
    जर (!region_list) परत सत्य;
    अचिन्हित अंक_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    अचिन्हित total_rows = 0;
    साठी (अचिन्हित major = 0; major < var_data_अंक_rows.length; major++)
      total_rows += var_data_अंक_rows[major];

    जर (!delta_rows.resize (total_rows)) परत असत्य;
    /* init all rows to [0]*num_cols */
    साठी (अचिन्हित i = 0; i < total_rows; i++)
      जर (!(delta_rows[i].resize (अंक_cols))) परत असत्य;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<अचिन्हित, स्थिरांक hb_vector_t<पूर्णांक>*> front_mapping;
    अचिन्हित start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<स्थिरांक hb_vector_t<पूर्णांक> *> major_rows;
    hb_hashmap_t<स्थिरांक hb_vector_t<पूर्णांक>*, अचिन्हित> delta_rows_map;
    साठी (अचिन्हित major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      स्थिरांक tuple_variations_t& tuples = vars[major];
      अचिन्हित अंक_rows = var_data_अंक_rows[major];

      जर (!अंक_rows) सुरूठेवा;

      साठी (स्थिरांक tuple_delta_t& tuple: tuples.tuple_vars)
      {
        जर (tuple.deltas_x.length != अंक_rows)
          परत असत्य;

        /* skip unused regions */
        अचिन्हित *col_idx;
        जर (!region_map.has (&(tuple.axis_tuples), &col_idx))
          सुरूठेवा;

        साठी (अचिन्हित i = 0; i < अंक_rows; i++)
        {
          पूर्णांक rounded_delta = hb_clamp_to<पूर्णांक> (roundf ((द्विगुण) tuple.deltas_x[i]));
          पूर्णांक &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          जर (unlikely (sum < पूर्णांक_न्यून || sum > पूर्णांक_उच्च)) परत असत्य;
          delta = (पूर्णांक) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      साठी (अचिन्हित minor = 0; minor < अंक_rows; minor++)
      {
	स्थिरांक hb_vector_t<पूर्णांक>& row = delta_rows[start_row + minor];
	जर (use_no_variation_idx)
	{
	  तार्किक all_zeros = सत्य;
	  साठी (पूर्णांक delta : row)
	  {
	    जर (delta != 0)
	    {
	      all_zeros = असत्य;
	      थांबा;
	    }
	  }
	  जर (all_zeros)
	    सुरूठेवा;
	}

	जर (!front_mapping.set ((major<<16) + minor, &row))
	  परत असत्य;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	जर (optimize)
	{
	  जर (delta_rows_map.has (&row))
	    सुरूठेवा;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      जर (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        परत असत्य;
      जर (major_rows)
      {
	स्वतः *encoding = encoding_objs.push (std::move (major_rows), अंक_cols);
	जर (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  परत असत्य;
      }

      start_row += अंक_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    जर (!optimize)
    {
      encodings = std::move (encoding_objs);
      परत !encodings.in_error ();
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
    hb_vector_t<item_t> queue_वस्तुसंच;
    अचिन्हित अंक_todos = encoding_objs.length;
    साठी (अचिन्हित i = 0; i < अंक_todos; i++)
    {
      साठी (अचिन्हित j = i + 1; j < अंक_todos; j++)
      {
        पूर्णांक combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        जर (combining_gain > 0)
          queue_वस्तुसंच.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_वस्तुसंच));

    hb_bit_set_t removed_todo_idxes;
    पर्यंत (queue)
    {
      स्वतः t = queue.काढा_minimum ().first;
      अचिन्हित i = t.idx_1 ();
      अचिन्हित j = t.idx_2 ();

      जर (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        सुरूठेवा;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      जर (unlikely (removed_todo_idxes.in_error ())) परत असत्य;

      encoding.merge (other_encoding);
      जर (unlikely (encoding.in_error ())) परत असत्य;

      साठी (अचिन्हित idx = 0; idx < encoding_objs.length; idx++)
      {
        जर (removed_todo_idxes.has (idx)) सुरूठेवा;

        स्थिरांक delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        जर (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          साठी (स्थिरांक स्वतः& row : obj.वस्तुसंच)
            encoding.add_row (row);
          जर (unlikely (encoding.in_error ())) परत असत्य;

          removed_todo_idxes.add (idx);
          जर (unlikely (removed_todo_idxes.in_error ())) परत असत्य;
          सुरूठेवा;
        }

        पूर्णांक combined_gain = encoding.gain_from_merging (obj);
        जर (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      स्वतः moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      जर (unlikely (encoding_objs.in_error ())) परत असत्य;
    }

    पूर्णांक अंक_final_encodings = (पूर्णांक) encoding_objs.length - (पूर्णांक) removed_todo_idxes.get_population ();
    जर (अंक_final_encodings <= 0) परत असत्य;

    जर (!encodings.alloc (अंक_final_encodings)) परत असत्य;
    साठी (अचिन्हित i = 0; i < encoding_objs.length; i++)
    {
      जर (removed_todo_idxes.has (i)) सुरूठेवा;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    परत compile_varidx_map (front_mapping);
  }

  खाजगी:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  तार्किक compile_varidx_map (स्थिरांक hb_hashmap_t<अचिन्हित, स्थिरांक hb_vector_t<पूर्णांक>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<स्थिरांक hb_vector_t<पूर्णांक>*, अचिन्हित> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    साठी (अचिन्हित i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      जर (encoding.is_empty () || encoding.in_error ())
        परत असत्य;

      अचिन्हित अंक_rows = encoding.वस्तुसंच.length;
      अचिन्हित अंक_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.वस्तुसंच.क्विक (_cmp_row);

      साठी (अचिन्हित start = 0; start < अंक_rows; start += 0xFFFFu)
      {
        अचिन्हित chunk_len = hb_min (अंक_rows - start, 0xFFFFu);
        hb_vector_t<स्थिरांक hb_vector_t<पूर्णांक>*> rows;

        जर (!rows.alloc (chunk_len))
          परत असत्य;

        अचिन्हित major = split_encodings.length;
        साठी (अचिन्हित minor = 0; minor < chunk_len; minor++)
        {
          स्थिरांक hb_vector_t<पूर्णांक> *row = encoding.वस्तुसंच.arrayZ[start + minor];
          rows.push (row);
          जर (!back_mapping.set (row, (major << 16) + minor))
            परत असत्य;
        }

        स्वतः *split = split_encodings.push (std::move (rows), अंक_cols);
        जर (unlikely (split_encodings.in_error () || split->in_error ()))
          परत असत्य;
      }
    }

    encodings = std::move (split_encodings);
    जर (encodings.in_error () || back_mapping.in_error ())
      परत असत्य;

    साठी (स्वतः _ : front_mapping.iter ())
    {
      अचिन्हित old_varidx = _.first;
      अचिन्हित *new_varidx;
      जर (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      अन्यथा
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    परत !varidx_map.in_error ();
  }

  स्थिर पूर्णांक _cmp_row (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    स्थिरांक hb_vector_t<पूर्णांक>** a = (स्थिरांक hb_vector_t<पूर्णांक>**) pa;
    स्थिरांक hb_vector_t<पूर्णांक>** b = (स्थिरांक hb_vector_t<पूर्णांक>**) pb;

    साठी (अचिन्हित i = 0; i < (*b)->length; i++)
    {
      पूर्णांक va = (*a)->arrayZ[i];
      पूर्णांक vb = (*b)->arrayZ[i];
      जर (va != vb)
        परत va < vb ? -1 : 1;
    }
    परत 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

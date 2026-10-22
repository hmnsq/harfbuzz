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

#अगर_अघोषित HB_OT_VAR_COMMON_HH
#घोषणा HB_OT_VAR_COMMON_HH

#समावेश "hb-ot-layout-common.hh"
#समावेश "hb-alloc-pool.hh"
#समावेश "hb-priority-queue.hh"
#समावेश "hb-subset-instancer-iup.hh"


namespace OT {

using rebase_tent_परिणाम_scratch_t = hb_pair_t<rebase_tent_परिणाम_t, rebase_tent_परिणाम_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
काष्ठा TupleVariationHeader
{
  मित्र काष्ठा tuple_delta_t;
  माप_प्रकार get_आकार (अचिन्हित axis_गिनती_times_2) स्थिर
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    अचिन्हित ti = tuple_सूचकांक;
    अगर (unlikely ((ti & (Tuple_सूचकांक::EmbeddedPeakTuple | Tuple_सूचकांक::IntermediateRegion))))
    {
      अचिन्हित गिनती = ((ti & Tuple_सूचकांक::EmbeddedPeakTuple) != 0) + ((ti & Tuple_सूचकांक::IntermediateRegion) != 0) * 2;
      वापस hb_unsigned_mul_add_saturate (गिनती, axis_गिनती_times_2, min_आकार);
    }
    वापस min_आकार;
  }

  अचिन्हित get_तथ्य_आकार () स्थिर { वापस var_तथ्य_आकार; }

  स्थिर TupleVariationHeader &get_अग्र (अचिन्हित axis_गिनती_times_2) स्थिर
  { वापस StructAtOffset<TupleVariationHeader> (यह, get_आकार (axis_गिनती_times_2)); }

  bool unpack_axis_tuples (अचिन्हित axis_गिनती,
                           स्थिर hb_array_t<स्थिर F2DOT14> shared_tuples,
                           स्थिर hb_map_t *axes_old_सूचकांक_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) स्थिर
  {
    स्थिर F2DOT14 *peak_tuple = nullptr;
    अगर (has_peak ())
      peak_tuple = get_peak_tuple (axis_गिनती);
    अन्यथा
    {
      अचिन्हित पूर्णांक सूचकांक_2 = get_सूचकांक ();
      अगर (unlikely ((सूचकांक_2 + 1) * axis_गिनती > shared_tuples.लंबाई))
        वापस false;
      peak_tuple = shared_tuples.sub_array (axis_गिनती * सूचकांक_2, axis_गिनती).arrayZ;
    }

    स्थिर F2DOT14 *start_tuple = nullptr;
    स्थिर F2DOT14 *end_tuple = nullptr;
    bool has_interm = has_intermediate ();

    अगर (has_interm)
    {
      start_tuple = get_start_tuple (axis_गिनती);
      end_tuple = get_end_tuple (axis_गिनती);
    }

    क्रम (अचिन्हित i = 0; i < axis_गिनती; i++)
    {
      भग्न peak = peak_tuple[i].to_float ();
      अगर (peak == 0.f) जारी;

      hb_tag_t *axis_tag;
      अगर (!axes_old_सूचकांक_tag_map->has (i, &axis_tag))
        वापस false;

      भग्न start, end;
      अगर (has_interm)
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

    वापस true;
  }

  HB_ALWAYS_INLINE
  भग्न calculate_scalar (hb_array_t<स्थिर पूर्णांक> coords, अचिन्हित पूर्णांक coord_गिनती,
			   स्थिर hb_array_t<स्थिर F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) स्थिर
  {
    अचिन्हित tuple_सूचकांक_2 = tuple_सूचकांक;
    अचिन्हित पूर्णांक सूचकांक_2 = tuple_सूचकांक_2 & Tuple_सूचकांक::Tuple_सूचकांक_Mask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    bool plain_shared_tuple = (tuple_सूचकांक_2 & ~Tuple_सूचकांक::PrivatePointNumbers) < shared_tuple_scalar_cache->लंबाई;
    अगर (likely (plain_shared_tuple))
    {
      भग्न scalar;
      अगर (likely (shared_tuple_scalar_cache->लो (सूचकांक_2, &scalar)))
	वापस scalar;
    }

    स्थिर F2DOT14 *peak_tuple;

    bool has_interm = tuple_सूचकांक_2 & Tuple_सूचकांक::IntermediateRegion; // Inlined for performance

    अगर (unlikely (tuple_सूचकांक_2 & Tuple_सूचकांक::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_गिनती);
      shared_tuple_scalar_cache = nullptr;
    }
    अन्यथा
    {
      भग्न scalar;
      अगर (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->लंबाई &&
	  shared_tuple_scalar_cache->लो (सूचकांक_2, &scalar))
      {
        अगर (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	अन्यथा
	  वापस scalar;
      }

      अगर (unlikely ((सूचकांक_2 + 1) * coord_गिनती > shared_tuples.लंबाई))
        वापस 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_गिनती * सूचकांक_2);

    }

    स्थिर F2DOT14 *start_tuple = nullptr;
    स्थिर F2DOT14 *end_tuple = nullptr;

    अगर (has_interm)
    {
      start_tuple = get_start_tuple (coord_गिनती);
      end_tuple = get_end_tuple (coord_गिनती);
    }

    भग्न scalar = 1.f;
#अगर_अघोषित HB_OPTIMIZE_आकार
#अगर HB_FAST_अंक_ACCESS
    bool skip = coord_गिनती >= 16;
#पूर्ण_अगर
#पूर्ण_अगर
    क्रम (अचिन्हित पूर्णांक i = 0; i < coord_गिनती; i++)
    {
#अगर_अघोषित HB_OPTIMIZE_आकार
#अगर HB_FAST_अंक_ACCESS
      अगर (skip)
      {
	जबतक (i + 4 <= coord_गिनती && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	जबतक (i < coord_गिनती && peak_tuple[i].to_int () == 0)
	  i += 1;
	अगर (i >= coord_गिनती)
	  अवरोध;
      }
#पूर्ण_अगर
#पूर्ण_अगर

      पूर्णांक peak = peak_tuple[i].to_int ();
      अगर (!peak) जारी;

      पूर्णांक v = coords[i];
      अगर (!v) { scalar = 0.f; अवरोध; }
      अगर (v == peak) जारी;

      अगर (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        पूर्णांक start = start_tuple[i].to_int ();
        पूर्णांक end = end_tuple[i].to_int ();
        अगर (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) जारी;
        अगर (v < start || v > end) { scalar = 0.f; अवरोध; }
        अगर (v < peak)
        { अगर (peak != start) scalar *= (भग्न) (v - start) / (peak - start); }
        अन्यथा
        { अगर (peak != end) scalar *= (भग्न) (end - v) / (end - peak); }
      }
      अन्यथा अगर (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; अवरोध; }
      अन्यथा
        scalar *= (भग्न) v / peak;
    }
    अगर (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (सूचकांक_2, scalar);
    वापस scalar;
  }

  bool           has_peak () स्थिर { वापस tuple_सूचकांक & Tuple_सूचकांक::EmbeddedPeakTuple; }
  bool   has_intermediate () स्थिर { वापस tuple_सूचकांक & Tuple_सूचकांक::IntermediateRegion; }
  bool has_private_points () स्थिर { वापस tuple_सूचकांक & Tuple_सूचकांक::PrivatePointNumbers; }
  अचिन्हित      get_सूचकांक () स्थिर { वापस tuple_सूचकांक & Tuple_सूचकांक::Tuple_सूचकांक_Mask; }
  अटल constexpr अचिन्हित max_shared_tuple_गिनती = 0x1000u;

  रक्षित:
  काष्ठा Tuple_सूचकांक : HBUINT16
  {
    क्रमागत Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      Tuple_सूचकांक_Mask      = 0x0FFFu
    };

    Tuple_सूचकांक& चालक = (uint16_t i) { HBUINT16::चालक= (i); वापस *यह; }
    DEFINE_आकार_STATIC (2);
  };

  hb_array_t<स्थिर F2DOT14> get_all_tuples (अचिन्हित axis_गिनती) स्थिर
  { वापस StructAfter<UnsizedArrayOf<F2DOT14>> (tuple_सूचकांक).as_array ((has_peak () + has_intermediate () * 2) * axis_गिनती); }
  स्थिर F2DOT14* get_all_tuples_base (अचिन्हित axis_गिनती) स्थिर
  { वापस StructAfter<UnsizedArrayOf<F2DOT14>> (tuple_सूचकांक).arrayZ; }
  स्थिर F2DOT14* get_peak_tuple (अचिन्हित axis_गिनती) स्थिर
  { वापस get_all_tuples_base (axis_गिनती); }
  स्थिर F2DOT14* get_start_tuple (अचिन्हित axis_गिनती) स्थिर
  { वापस get_all_tuples_base (axis_गिनती) + has_peak () * axis_गिनती; }
  स्थिर F2DOT14* get_end_tuple (अचिन्हित axis_गिनती) स्थिर
  { वापस get_all_tuples_base (axis_गिनती) + has_peak () * axis_गिनती + axis_गिनती; }

  HBUINT16      var_तथ्य_आकार;    /* The size in bytes of the serialized
                                 * data for this tuple variation table. */
  Tuple_सूचकांक    tuple_सूचकांक;     /* A packed field. The high 4 bits are flags (see below).
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
  खुला:
  DEFINE_आकार_MIN (4);
};

काष्ठा optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<bool> opt_indices;
  hb_vector_t<पूर्णांक> rounded_x_deltas;
  hb_vector_t<पूर्णांक> rounded_y_deltas;
  hb_vector_t<भग्न> opt_deltas_x;
  hb_vector_t<भग्न> opt_deltas_y;
  hb_vector_t<अचिन्हित अक्षर> opt_point_तथ्य;
  hb_vector_t<अचिन्हित अक्षर> opt_deltas_तथ्य;
  hb_vector_t<अचिन्हित अक्षर> point_तथ्य;
  hb_vector_t<अचिन्हित अक्षर> deltas_तथ्य;
  hb_vector_t<पूर्णांक> rounded_deltas;
};

काष्ठा tuple_delta_t
{
  अटल constexpr bool realloc_move = true;  // Watch out when adding new members!

  खुला:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<bool> indices;

  hb_vector_t<भग्न> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<भग्न> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<अचिन्हित अक्षर> compiled_tuple_header;
  hb_vector_t<अचिन्हित अक्षर> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (स्थिर tuple_delta_t& o) = शेष;
  tuple_delta_t& चालक = (स्थिर tuple_delta_t& o) = शेष;

  मित्र व्योम swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*यह, o); }

  tuple_delta_t& चालक = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*यह, o);
    वापस *यह;
  }

  व्योम copy_from (स्थिर tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  व्योम remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  bool set_tent (hb_tag_t axis_tag, Triple tent)
  { वापस axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& चालक += (स्थिर tuple_delta_t& o)
  {
    अचिन्हित अंक = indices.लंबाई;
    क्रम (अचिन्हित i = 0; i < अंक; i++)
    {
      अगर (indices.arrayZ[i])
      {
        अगर (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          अगर (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      अन्यथा
      {
        अगर (!o.indices.arrayZ[i]) जारी;
        indices.arrayZ[i] = true;
        deltas_x[i] = o.deltas_x[i];
        अगर (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    वापस *यह;
  }

  tuple_delta_t& चालक *= (भग्न scalar)
  {
    अगर (scalar == 1.0f)
      वापस *यह;

    अचिन्हित अंक = indices.लंबाई;
    अगर (deltas_y)
      क्रम (अचिन्हित i = 0; i < अंक; i++)
      {
	अगर (!indices.arrayZ[i]) जारी;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    अन्यथा
      क्रम (अचिन्हित i = 0; i < अंक; i++)
      {
	अगर (!indices.arrayZ[i]) जारी;
	deltas_x[i] *= scalar;
      }
    वापस *यह;
  }

  व्योम change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_परिणाम_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    अगर (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*यह));
      वापस;
    }

    अगर ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->मध्य && tent->मध्य <= tent->maximum))
      वापस;

    अगर (tent->मध्य == 0.f)
    {
      out.push (std::move (*यह));
      वापस;
    }

    अगर (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.मध्य &&
          axis_limit.मध्य <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      वापस;

    rebase_tent_परिणाम_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    क्रम (अचिन्हित i = 0; i < solutions.लंबाई; i++)
    {
      स्वतः &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      अगर (i < solutions.लंबाई - 1)
	new_var.copy_from (*यह, pool);
      अन्यथा
	new_var = std::move (*यह);

      अगर (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      अन्यथा
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  bool compile_coords (स्थिर hb_map_t& axes_सूचकांक_map,
		       स्थिर hb_map_t& axes_old_सूचकांक_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    अचिन्हित cur_axis_गिनती = axes_सूचकांक_map.get_population ();
    अगर (pool)
    {
      अगर (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_गिनती)))
	वापस false;
    }
    अन्यथा अगर (unlikely (!compiled_peak_coords.resize (cur_axis_गिनती)))
      वापस false;

    hb_array_t<F2DOT14> start_coords, end_coords;

    अचिन्हित orig_axis_गिनती = axes_old_सूचकांक_tag_map.get_population ();
    अचिन्हित j = 0;
    क्रम (अचिन्हित i = 0; i < orig_axis_गिनती; i++)
    {
      अगर (!axes_सूचकांक_map.has (i))
        जारी;

      hb_tag_t axis_tag = axes_old_सूचकांक_tag_map.लो (i);
      Triple *coords = nullptr;
      अगर (axis_tuples.has (axis_tag, &coords))
      {
	भग्न min_val = coords->minimum;
	भग्न val = coords->मध्य;
	भग्न max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	अगर (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  अगर (!compiled_interm_coords)
	  {
	    अगर (pool)
	    {
	      अगर (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_गिनती)))
		वापस false;
	    }
	    अन्यथा अगर (unlikely (!compiled_interm_coords.resize (2 * cur_axis_गिनती)))
	      वापस false;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_गिनती);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_गिनती);

	    क्रम (अचिन्हित k = 0; k < j; k++)
	    {
	      चिन्हित peak = compiled_peak_coords.arrayZ[k].to_int ();
	      अगर (!peak) जारी;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	अगर (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    वापस !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  bool compile_tuple_var_header (स्थिर hb_map_t& axes_सूचकांक_map,
                                 अचिन्हित points_तथ्य_लंबाई,
                                 स्थिर hb_map_t& axes_old_सूचकांक_tag_map,
                                 स्थिर hb_hashmap_t<स्थिर hb_vector_t<F2DOT14>*, अचिन्हित>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    अगर (!compiled_deltas) वापस true;

    अचिन्हित cur_axis_गिनती = axes_सूचकांक_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    अचिन्हित alloc_len = 3 * cur_axis_गिनती * (F2DOT14::static_आकार) + 4;
    अगर (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, false))) वापस false;

    अचिन्हित flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    अगर (!shared_tuples_idx_map)
      compile_coords (axes_सूचकांक_map, axes_old_सूचकांक_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    अचिन्हित peak_गिनती = 0;
    अचिन्हित *shared_tuple_idx;
    अगर (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    अन्यथा
    {
      peak_गिनती = encode_peak_coords(coords, flag);
      अगर (!peak_गिनती) वापस false;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    अचिन्हित interim_गिनती = encode_interm_coords (coords.sub_array (peak_गिनती), flag);

    /* pointdata length = 0 implies "use shared points" */
    अगर (points_तथ्य_लंबाई)
      flag |= TupleVariationHeader::Tuple_सूचकांक::PrivatePointNumbers;

    अचिन्हित serialized_तथ्य_आकार = points_तथ्य_लंबाई + compiled_deltas.लंबाई;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->var_तथ्य_आकार = serialized_तथ्य_आकार;
    o->tuple_सूचकांक = flag;

    अचिन्हित योग_header_len = 4 + (peak_गिनती + interim_गिनती) * (F2DOT14::static_आकार);
    compiled_tuple_header.shrink_back_to_pool (pool, योग_header_len);
    वापस true;
  }

  अचिन्हित encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               अचिन्हित& flag) स्थिर
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.लंबाई * माप (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::Tuple_सूचकांक::EmbeddedPeakTuple;
    वापस compiled_peak_coords.लंबाई;
  }

  /* if no need to encode intermediate coords, then just return p */
  अचिन्हित encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 अचिन्हित& flag) स्थिर
  {
    अगर (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.लंबाई * माप (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::Tuple_सूचकांक::IntermediateRegion;
    }
    वापस compiled_interm_coords.लंबाई;
  }

  bool compile_deltas (hb_vector_t<पूर्णांक> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { वापस compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  अटल bool compile_deltas (hb_array_t<स्थिर bool> point_indices,
			      hb_array_t<स्थिर भग्न> x_deltas,
			      hb_array_t<स्थिर भग्न> y_deltas,
			      hb_vector_t<अचिन्हित अक्षर> &compiled_deltas, /* OUT */
			      hb_vector_t<पूर्णांक> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    अगर (unlikely (!rounded_deltas.resize_dirty  (point_indices.लंबाई)))
      वापस false;

    अचिन्हित j = 0;
    क्रम (अचिन्हित i = 0; i < point_indices.लंबाई; i++)
    {
      अगर (!point_indices[i]) जारी;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<पूर्णांक> (roundf ((द्विगुन) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    अगर (!rounded_deltas) वापस true;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    अचिन्हित alloc_len = 2 + 4 * rounded_deltas.लंबाई + (rounded_deltas.लंबाई + 63) / 64;
    अगर (y_deltas)
      alloc_len *= 2;

    अगर (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, false))) वापस false;

    अचिन्हित encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    अगर (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      अचिन्हित j = 0;
      क्रम (अचिन्हित idx = 0; idx < point_indices.लंबाई; idx++)
      {
        अगर (!point_indices[idx]) जारी;
        पूर्णांक rounded_delta = hb_clamp_to<पूर्णांक> (roundf ((द्विगुन) y_deltas.arrayZ[idx]));

        अगर (j >= rounded_deltas.लंबाई) वापस false;

        rounded_deltas[j++] = rounded_delta;
      }

      अगर (j != rounded_deltas.लंबाई) वापस false;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    वापस true;
  }

  अटल अचिन्हित compile_deltas (hb_array_t<अचिन्हित अक्षर> encoded_bytes,
				  hb_array_t<स्थिर पूर्णांक> deltas)
  {
    वापस Tuple_मूल्यावली::compile_unsafe (deltas, encoded_bytes);
  }

  bool calc_inferred_deltas (स्थिर contour_point_vector_t& orig_points,
			     hb_vector_t<अचिन्हित> &scratch)
  {
    अचिन्हित point_गिनती = orig_points.लंबाई;
    अगर (point_गिनती != indices.लंबाई)
      वापस false;

    अचिन्हित ref_गिनती_2 = 0;

    hb_vector_t<अचिन्हित> &end_points = scratch.reset ();

    क्रम (अचिन्हित i = 0; i < point_गिनती; i++)
    {
      ref_गिनती_2 += indices.arrayZ[i];
      अगर (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    अगर (ref_गिनती_2 == point_गिनती)
      वापस true;
    अगर (unlikely (end_points.in_error ())) वापस false;

    hb_bit_set_t inferred_idxes;
    अचिन्हित start_point = 0;
    क्रम (अचिन्हित end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      अचिन्हित unref_गिनती = 0;
      क्रम (अचिन्हित i = start_point; i < end_point + 1; i++)
        unref_गिनती += indices.arrayZ[i];
      unref_गिनती = (end_point - start_point + 1) - unref_गिनती;

      अचिन्हित j = start_point;
      अगर (unref_गिनती == 0 || unref_गिनती > end_point - start_point)
        जाओ no_more_gaps;
      क्रम (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        अचिन्हित पूर्णांक prev, next, i;
        क्रम (;;)
        {
          i = j;
          j = अग्र_सूचकांक (i, start_point, end_point);
          अगर (indices.arrayZ[i] && !indices.arrayZ[j]) अवरोध;
        }
        prev = j = i;
        क्रम (;;)
        {
          i = j;
          j = अग्र_सूचकांक (i, start_point, end_point);
          अगर (!indices.arrayZ[i] && indices.arrayZ[j]) अवरोध;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        क्रम (;;)
        {
          i = अग्र_सूचकांक (i, start_point, end_point);
          अगर (i == next) अवरोध;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          अगर (--unref_गिनती == 0) जाओ no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    क्रम (अचिन्हित i = 0; i < point_गिनती; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      अगर ( !indices[i])
      {
        अगर (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = true;
      }
    }
    वापस true;
  }

  bool optimize (स्थिर contour_point_vector_t& contour_points,
                 bool is_composite,
		 optimize_scratch_t &scratch,
                 द्विगुन tolerance = 0.5 + 1e-10)
  {
    अचिन्हित गिनती = contour_points.लंबाई;
    अगर (deltas_x.लंबाई != गिनती ||
        deltas_y.लंबाई != गिनती)
      वापस false;

    hb_vector_t<bool> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<पूर्णांक> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<पूर्णांक> &rounded_y_deltas = scratch.rounded_y_deltas;

    अगर (unlikely (!rounded_x_deltas.resize_dirty  (गिनती) ||
                  !rounded_y_deltas.resize_dirty  (गिनती)))
      वापस false;

    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<पूर्णांक> (roundf ((द्विगुन) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<पूर्णांक> (roundf ((द्विगुन) deltas_y.arrayZ[i]));
    }

    अगर (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      वापस false;

    अचिन्हित ref_गिनती_2 = 0;
    क्रम (bool ref_flag : opt_indices)
       ref_गिनती_2 += ref_flag;

    अगर (ref_गिनती_2 == गिनती) वापस true;

    hb_vector_t<भग्न> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<भग्न> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    bool is_comp_glyph_wo_deltas = (is_composite && ref_गिनती_2 == 0);
    अगर (is_comp_glyph_wo_deltas)
    {
      अगर (unlikely (!opt_deltas_x.resize (गिनती) ||
                    !opt_deltas_y.resize (गिनती)))
        वापस false;

      opt_indices.arrayZ[0] = true;
      क्रम (अचिन्हित i = 1; i < गिनती; i++)
        opt_indices.arrayZ[i] = false;
    }

    hb_vector_t<अचिन्हित अक्षर> &opt_point_तथ्य = scratch.opt_point_तथ्य.reset ();
    अगर (!compile_point_set (opt_indices, opt_point_तथ्य))
      वापस false;
    hb_vector_t<अचिन्हित अक्षर> &opt_deltas_तथ्य = scratch.opt_deltas_तथ्य.reset ();
    अगर (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_तथ्य,
			 scratch.rounded_deltas))
      वापस false;

    hb_vector_t<अचिन्हित अक्षर> &point_तथ्य = scratch.point_तथ्य.reset ();
    अगर (!compile_point_set (indices, point_तथ्य))
      वापस false;
    hb_vector_t<अचिन्हित अक्षर> &deltas_तथ्य = scratch.deltas_तथ्य.reset ();
    अगर (!compile_deltas (indices, deltas_x, deltas_y, deltas_तथ्य, scratch.rounded_deltas))
      वापस false;

    अगर (opt_point_तथ्य.लंबाई + opt_deltas_तथ्य.लंबाई < point_तथ्य.लंबाई + deltas_तथ्य.लंबाई)
    {
      indices = std::move (opt_indices);

      अगर (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    वापस !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  अटल bool compile_point_set (स्थिर hb_vector_t<bool> &point_indices,
                                 hb_vector_t<अचिन्हित अक्षर>& compiled_points /* OUT */)
  {
    अचिन्हित अंक_points = 0;
    क्रम (bool i : point_indices)
      अगर (i) अंक_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    अगर (!अंक_points) वापस true;

    अचिन्हित indices_लंबाई = point_indices.लंबाई;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    अगर (अंक_points == indices_लंबाई)
      वापस compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    अचिन्हित अंक_bytes = 2 + 3 *अंक_points;
    अगर (unlikely (!compiled_points.resize_dirty  (अंक_bytes)))
      वापस false;

    अचिन्हित pos = 0;
    /* binary data starts with the total number of reference points */
    अगर (अंक_points < 0x80)
      compiled_points.arrayZ[pos++] = अंक_points;
    अन्यथा
    {
      compiled_points.arrayZ[pos++] = ((अंक_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = अंक_points & 0xFF;
    }

    स्थिर अचिन्हित max_run_लंबाई = 0x7F;
    अचिन्हित i = 0;
    अचिन्हित last_मूल्य = 0;
    अचिन्हित अंक_encoded = 0;
    जबतक (i < indices_लंबाई && अंक_encoded < अंक_points)
    {
      अचिन्हित run_लंबाई = 0;
      अचिन्हित header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      bool use_byte_encoding = false;
      bool new_run = true;
      जबतक (i < indices_लंबाई && अंक_encoded < अंक_points &&
             run_लंबाई <= max_run_लंबाई)
      {
        // find out next referenced point index
        जबतक (i < indices_लंबाई && !point_indices[i])
          i++;

        अगर (i >= indices_लंबाई) अवरोध;

        अचिन्हित cur_मूल्य = i;
        अचिन्हित delta = cur_मूल्य - last_मूल्य;

        अगर (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = false;
        }

        अगर (use_byte_encoding && delta > 0xFF)
          अवरोध;

        अगर (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        अन्यथा
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_मूल्य = cur_मूल्य;
        run_लंबाई++;
        अंक_encoded++;
      }

      अगर (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_लंबाई - 1;
      अन्यथा
        compiled_points.arrayZ[header_pos] = (run_लंबाई - 1) | 0x80;
    }
    वापस compiled_points.resize_dirty  (pos);
  }

  अटल भग्न infer_delta (भग्न target_val, भग्न पूर्व_val, भग्न अग्र_val, भग्न पूर्व_delta, भग्न अग्र_delta)
  {
    अगर (पूर्व_val == अग्र_val)
      वापस (पूर्व_delta == अग्र_delta) ? पूर्व_delta : 0.f;
    अन्यथा अगर (target_val <= hb_min (पूर्व_val, अग्र_val))
      वापस (पूर्व_val < अग्र_val) ? पूर्व_delta : अग्र_delta;
    अन्यथा अगर (target_val >= hb_max (पूर्व_val, अग्र_val))
      वापस (पूर्व_val > अग्र_val) ? पूर्व_delta : अग्र_delta;

    भग्न r = (target_val - पूर्व_val) / (अग्र_val - पूर्व_val);
    वापस पूर्व_delta + r * (अग्र_delta - पूर्व_delta);
  }

  अटल अचिन्हित पूर्णांक अग्र_सूचकांक (अचिन्हित पूर्णांक i, अचिन्हित पूर्णांक start, अचिन्हित पूर्णांक end)
  { वापस (i >= end) ? start : (i + 1); }
};

ढाँचा <typename OffType = HBUINT16>
काष्ठा Tuple_Variation_तथ्य
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (यह));
  }

  माप_प्रकार get_आकार (अचिन्हित axis_गिनती_times_2) स्थिर
  {
    माप_प्रकार योग_आकार = min_आकार;
    अचिन्हित गिनती = tuple_Var_गिनती.get_गिनती ();
    स्थिर TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      योग_आकार = hb_unsigned_add_saturate (योग_आकार,
					     tuple_var_header->get_आकार (axis_गिनती_times_2),
					     (माप_प्रकार) tuple_var_header->get_तथ्य_आकार ());
      tuple_var_header = &tuple_var_header->get_अग्र (axis_गिनती_times_2);
    }

    वापस योग_आकार;
  }

  स्थिर TupleVariationHeader &get_tuple_var_header (व्योम) स्थिर
  { वापस StructAfter<TupleVariationHeader> (तथ्य); }

  काष्ठा tuple_iterator_t;
  काष्ठा tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    निजी:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<स्थिर hb_vector_t<bool>*, hb_vector_t<अचिन्हित अक्षर>> point_तथ्य_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<स्थिर hb_vector_t<bool>*, अचिन्हित> point_set_गिनती_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<अचिन्हित अक्षर> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    अचिन्हित compiled_byte_आकार = 0;
    bool needs_padding = false;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    bool is_composite = false;

    खुला:
    tuple_variations_t () = शेष;
    tuple_variations_t (स्थिर tuple_variations_t&) = मिटाओ;
    tuple_variations_t& चालक=(स्थिर tuple_variations_t&) = मिटाओ;
    tuple_variations_t (tuple_variations_t&&) = शेष;
    tuple_variations_t& चालक=(tuple_variations_t&&) = शेष;
    ~tuple_variations_t () = शेष;

    explicit चालक bool () स्थिर { वापस bool (tuple_vars); }
    अचिन्हित get_var_गिनती () स्थिर
    {
      अचिन्हित गिनती = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      क्रम (स्वतः& tuple: tuple_vars)
        अगर (tuple.compiled_deltas) गिनती++;

      अगर (shared_points_bytes && shared_points_bytes->लंबाई)
        गिनती |= Tuple_Var_गिनती::SharedPointNumbers;
      वापस गिनती;
    }

    अचिन्हित get_compiled_byte_आकार () स्थिर
    { वापस compiled_byte_आकार; }

    bool create_from_tuple_var_तथ्य (tuple_iterator_t iterator,
                                     अचिन्हित tuple_var_गिनती,
                                     अचिन्हित point_गिनती,
                                     bool is_gvar,
                                     स्थिर hb_map_t *axes_old_सूचकांक_tag_map,
                                     स्थिर hb_vector_t<अचिन्हित> &shared_indices,
                                     स्थिर hb_array_t<स्थिर F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     bool is_composite_glyph = false)
    {
      hb_vector_t<अचिन्हित> private_indices;
      hb_vector_t<पूर्णांक> deltas_x;
      hb_vector_t<पूर्णांक> deltas_y;
      करो
      {
        स्थिर HBUINT8 *p = iterator.get_serialized_तथ्य ();
        अचिन्हित पूर्णांक लंबाई = iterator.current_tuple->get_तथ्य_आकार ();
        अगर (unlikely (!iterator.var_तथ्य_bytes.check_range (p, लंबाई)))
          वापस false;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        अगर (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_गिनती (), shared_tuples, axes_old_सूचकांक_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          वापस false;

	private_indices.reset ();
        bool has_private_points = iterator.current_tuple->has_private_points ();
        स्थिर HBUINT8 *end = p + लंबाई;
        अगर (has_private_points &&
            !Tuple_Variation_तथ्य::decompile_points (p, private_indices, end))
          वापस false;

        स्थिर hb_vector_t<अचिन्हित> &indices = has_private_points ? private_indices : shared_indices;
        bool apply_to_all = (indices.लंबाई == 0);
        अचिन्हित अंक_deltas = apply_to_all ? point_गिनती : indices.लंबाई;

        अगर (unlikely (!deltas_x.resize_dirty  (अंक_deltas) ||
                      !Tuple_Variation_तथ्य::decompile_deltas (p, deltas_x, end)))
          वापस false;

        अगर (is_gvar)
        {
          अगर (unlikely (!deltas_y.resize_dirty  (अंक_deltas) ||
                        !Tuple_Variation_तथ्य::decompile_deltas (p, deltas_y, end)))
            वापस false;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        अगर (unlikely (!var.indices.allocate_from_pool (pool, point_गिनती) ||
                      !var.deltas_x.allocate_from_pool (pool, point_गिनती, false)))
          वापस false;

        अगर (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_गिनती, false)))
          वापस false;

        क्रम (अचिन्हित i = 0; i < अंक_deltas; i++)
        {
          अचिन्हित idx = apply_to_all ? i : indices[i];
          अगर (idx >= point_गिनती) जारी;
          var.indices[idx] = true;
          var.deltas_x[idx] = deltas_x[i];
          अगर (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } जबतक (iterator.move_to_अग्र ());

      is_composite = is_composite_glyph;
      वापस true;
    }

    bool create_from_item_var_तथ्य (स्थिर Var_तथ्य &var_तथ्य,
                                    स्थिर hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    स्थिर hb_map_t& axes_old_सूचकांक_tag_map,
                                    अचिन्हित& item_गिनती_2,
                                    स्थिर hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      अगर (&var_तथ्य == &Null (Var_तथ्य))
        वापस true;

      अचिन्हित अंक_regions = var_तथ्य.get_region_सूचकांक_गिनती ();
      अगर (!tuple_vars.alloc (अंक_regions)) वापस false;

      item_गिनती_2 = inner_map ? inner_map->get_population () : var_तथ्य.get_item_गिनती ();
      अगर (!item_गिनती_2) वापस true;
      अचिन्हित row_आकार = var_तथ्य.get_row_आकार ();
      स्थिर HBUINT8 *delta_bytes = var_तथ्य.get_delta_bytes ();

      क्रम (अचिन्हित r = 0; r < अंक_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        अगर (!tuple.deltas_x.resize_dirty  (item_गिनती_2) ||
            !tuple.indices.resize_dirty  (item_गिनती_2))
          वापस false;

        क्रम (अचिन्हित i = 0; i < item_गिनती_2; i++)
        {
          tuple.indices.arrayZ[i] = true;
          tuple.deltas_x.arrayZ[i] = var_तथ्य.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_आकार);
        }

        अचिन्हित region_सूचकांक = var_तथ्य.get_region_सूचकांक (r);
        अगर (region_सूचकांक >= regions.लंबाई) वापस false;
        tuple.axis_tuples = regions.arrayZ[region_सूचकांक];

        tuple_vars.push (std::move (tuple));
      }
      वापस !tuple_vars.in_error ();
    }

    निजी:
    अटल पूर्णांक _cmp_axis_tag (स्थिर व्योम *pa, स्थिर व्योम *pb)
    {
      स्थिर hb_tag_t *a = (स्थिर hb_tag_t*) pa;
      स्थिर hb_tag_t *b = (स्थिर hb_tag_t*) pb;
      वापस (पूर्णांक)(*a) - (पूर्णांक)(*b);
    }

    bool change_tuple_variations_axis_limits (स्थिर hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              स्थिर hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      अगर (!axis_tags.alloc (normalized_axes_location.get_population ()))
        वापस false;
      क्रम (स्वतः t : normalized_axes_location.कुंजियाँ ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_परिणाम_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.क्विक (_cmp_axis_tag);
      क्रम (स्वतः axis_tag : axis_tags)
      {
        Triple *axis_limit;
        अगर (!normalized_axes_location.has (axis_tag, &axis_limit))
          वापस false;
        TripleDistances axis_triple_distances{1.f, 1.f};
        अगर (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.लो (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        क्रम (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          अगर (!out) जारी;

          अचिन्हित new_len = new_vars.लंबाई + out.लंबाई;

          अगर (unlikely (!new_vars.alloc (new_len, false)))
            वापस false;

          क्रम (अचिन्हित i = 0; i < out.लंबाई; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      वापस true;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    bool merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      अगर (unlikely (!new_vars.alloc (tuple_vars.लंबाई)))
	वापस false;
      hb_hashmap_t<स्थिर hb_hashmap_t<hb_tag_t, Triple>*, अचिन्हित> m;
      क्रम (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        अगर (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          अगर (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            वापस false;
          जारी;
        }

        अचिन्हित *idx;
        अगर (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        अन्यथा
        {
	  स्वतः *new_var = new_vars.push ();
	  अगर (unlikely (new_vars.in_error ()))
	    वापस false;
          hb_swap (*new_var, var);
          अगर (unlikely (!m.set (&(new_var->axis_tuples), new_vars.लंबाई - 1)))
            वापस false;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      वापस true;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    bool compile_all_point_sets ()
    {
      क्रम (स्थिर स्वतः& tuple: tuple_vars)
      {
        स्थिर hb_vector_t<bool>* points_set = &(tuple.indices);
        अगर (point_तथ्य_map.has (points_set))
        {
          अचिन्हित *गिनती;
          अगर (unlikely (!point_set_गिनती_map.has (points_set, &गिनती) ||
                        !point_set_गिनती_map.set (points_set, (*गिनती) + 1)))
            वापस false;
          जारी;
        }

        hb_vector_t<अचिन्हित अक्षर> compiled_point_तथ्य;
        अगर (!tuple_delta_t::compile_point_set (*points_set, compiled_point_तथ्य))
          वापस false;

        अगर (!point_तथ्य_map.set (points_set, std::move (compiled_point_तथ्य)) ||
            !point_set_गिनती_map.set (points_set, 1))
          वापस false;
      }
      वापस true;
    }

    /* find shared points set which saves most bytes */
    व्योम find_shared_points ()
    {
      अचिन्हित max_saved_bytes = 0;

      क्रम (स्थिर स्वतः& _ : point_तथ्य_map.iter_ref ())
      {
        स्थिर hb_vector_t<bool>* points_set = _.first;
        अचिन्हित तथ्य_लंबाई = _.second.लंबाई;
        अगर (!तथ्य_लंबाई) जारी;
        अचिन्हित *गिनती;
        अगर (unlikely (!point_set_गिनती_map.has (points_set, &गिनती) ||
                      *गिनती <= 1))
        {
          shared_points_bytes = nullptr;
          वापस;
        }

        अचिन्हित saved_bytes = तथ्य_लंबाई * ((*गिनती) -1);
        अगर (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    bool calc_inferred_deltas (स्थिर contour_point_vector_t& contour_points,
			       hb_vector_t<अचिन्हित> &scratch)
    {
      क्रम (tuple_delta_t& var : tuple_vars)
        अगर (!var.calc_inferred_deltas (contour_points, scratch))
          वापस false;

      वापस true;
    }

    bool iup_optimize (स्थिर contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      क्रम (tuple_delta_t& var : tuple_vars)
      {
        अगर (!var.optimize (contour_points, is_composite, scratch))
          वापस false;
      }
      वापस true;
    }

    खुला:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    व्योम cull_unreachable (स्थिर hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      अचिन्हित j = 0;
      क्रम (अचिन्हित i = 0; i < tuple_vars.लंबाई; i++)
      {
	अगर (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  जारी;
	अगर (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    bool instantiate (स्थिर hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      स्थिर hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      bool optimize = false)
    {
      अगर (!tuple_vars) वापस true;
      अगर (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        वापस false;
      /* compute inferred deltas only for gvar */
      अगर (contour_points)
      {
	hb_vector_t<अचिन्हित> scratch;
	अगर (!calc_inferred_deltas (*contour_points, scratch))
	  वापस false;
      }

      /* if iup delta opt is on, contour_points can't be null */
      अगर (optimize && !contour_points)
        वापस false;

      अगर (!merge_tuple_variations (optimize ? contour_points : nullptr))
        वापस false;

      अगर (optimize && !iup_optimize (*contour_points, scratch)) वापस false;
      वापस !tuple_vars.in_error ();
    }

    bool compile_bytes (स्थिर hb_map_t& axes_सूचकांक_map,
                        स्थिर hb_map_t& axes_old_सूचकांक_tag_map,
                        bool use_shared_points,
                        bool is_gvar = false,
                        स्थिर hb_hashmap_t<स्थिर hb_vector_t<F2DOT14>*, अचिन्हित>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      अगर (!tuple_vars)
        वापस true;

      // compile points set and store data in hashmap
      अगर (!compile_all_point_sets ())
        वापस false;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_आकार += 4;

      अगर (use_shared_points)
      {
        find_shared_points ();
        अगर (shared_points_bytes)
          compiled_byte_आकार += shared_points_bytes->लंबाई;
      }
      hb_vector_t<पूर्णांक> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      क्रम (स्वतः& tuple: tuple_vars)
      {
        स्थिर hb_vector_t<bool>* points_set = &(tuple.indices);
        hb_vector_t<अचिन्हित अक्षर> *points_तथ्य;
        अगर (unlikely (!point_तथ्य_map.has (points_set, &points_तथ्य)))
          वापस false;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        अगर (!points_तथ्य->लंबाई)
          जारी;
        अगर (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          वापस false;

        अचिन्हित points_तथ्य_लंबाई = (points_तथ्य != shared_points_bytes) ? points_तथ्य->लंबाई : 0;
        अगर (!tuple.compile_tuple_var_header (axes_सूचकांक_map, points_तथ्य_लंबाई, axes_old_सूचकांक_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          वापस false;
        compiled_byte_आकार += tuple.compiled_tuple_header.लंबाई + points_तथ्य_लंबाई + tuple.compiled_deltas.लंबाई;
      }

      अगर (is_gvar && (compiled_byte_आकार % 2))
      {
        needs_padding = true;
        compiled_byte_आकार += 1;
      }

      वापस true;
    }

    bool serialize_var_headers (hb_serialize_context_t *c, अचिन्हित& योग_header_len) स्थिर
    {
      TRACE_SERIALIZE (यह);
      क्रम (स्थिर स्वतः& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        अगर (c->in_error ()) return_trace (false);
        योग_header_len += tuple.compiled_tuple_header.लंबाई;
      }
      return_trace (true);
    }

    bool serialize_var_तथ्य (hb_serialize_context_t *c, bool is_gvar) स्थिर
    {
      TRACE_SERIALIZE (यह);
      अगर (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->लंबाई);
        s.copy (c);
      }

      क्रम (स्थिर स्वतः& tuple: tuple_vars)
      {
        स्थिर hb_vector_t<bool>* points_set = &(tuple.indices);
        hb_vector_t<अचिन्हित अक्षर> *point_तथ्य;
        अगर (!point_तथ्य_map.has (points_set, &point_तथ्य))
          return_trace (false);

        अगर (!is_gvar || point_तथ्य != shared_points_bytes)
        {
          hb_ubytes_t s (point_तथ्य->arrayZ, point_तथ्य->लंबाई);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        अगर (c->in_error ()) return_trace (false);
      }

      /* padding for gvar */
      अगर (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        अगर (!c->embed (pad)) return_trace (false);
      }
      return_trace (true);
    }
  };

  काष्ठा tuple_iterator_t
  {
    अचिन्हित get_axis_गिनती () स्थिर { वापस axis_गिनती; }

    व्योम init (hb_bytes_t var_तथ्य_bytes_2, अचिन्हित पूर्णांक axis_गिनती_3, स्थिर व्योम *table_base_)
    {
      var_तथ्य_bytes = var_तथ्य_bytes_2;
      var_तथ्य = var_तथ्य_bytes_2.as<Tuple_Variation_तथ्य> ();
      tuples_बायाँ = var_तथ्य->tuple_Var_गिनती.get_गिनती ();
      axis_गिनती = axis_गिनती_3;
      axis_गिनती_times_2 = axis_गिनती_3 * 2;
      current_tuple = &var_तथ्य->get_tuple_var_header ();
      तथ्य_offset = 0;
      table_base = table_base_;
    }

    bool get_shared_indices (hb_vector_t<अचिन्हित पूर्णांक> &shared_indices /* OUT */)
    {
      अगर (var_तथ्य->has_shared_point_numbers ())
      {
        स्थिर HBUINT8 *base = &(table_base+var_तथ्य->तथ्य);
        स्थिर HBUINT8 *p = base;
        अगर (!decompile_points (p, shared_indices, (स्थिर HBUINT8 *) (var_तथ्य_bytes.arrayZ + var_तथ्य_bytes.लंबाई))) वापस false;
        तथ्य_offset = p - base;
      }
      वापस true;
    }

    bool is_valid ()
    {
      अगर (unlikely (tuples_बायाँ <= 0))
	वापस false;

      current_tuple_आकार = TupleVariationHeader::min_आकार;
      अगर (unlikely (!var_तथ्य_bytes.check_end ((स्थिर अक्षर *) current_tuple + current_tuple_आकार)))
	वापस false;

      current_tuple_आकार = current_tuple->get_आकार (axis_गिनती_times_2);
      अगर (unlikely (!var_तथ्य_bytes.check_end ((स्थिर अक्षर *) current_tuple + current_tuple_आकार)))
	वापस false;

      वापस true;
    }

    HB_ALWAYS_INLINE
    bool move_to_अग्र ()
    {
      तथ्य_offset += current_tuple->get_तथ्य_आकार ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_आकार);
      tuples_बायाँ--;
      वापस is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    स्थिर HBUINT8 *get_serialized_तथ्य () स्थिर
    { वापस &(table_base+var_तथ्य->तथ्य) + तथ्य_offset; }

    निजी:
    चिन्हित tuples_बायाँ;
    स्थिर Tuple_Variation_तथ्य *var_तथ्य;
    अचिन्हित पूर्णांक axis_गिनती;
    अचिन्हित पूर्णांक axis_गिनती_times_2;
    अचिन्हित पूर्णांक तथ्य_offset;
    अचिन्हित पूर्णांक current_tuple_आकार;
    स्थिर व्योम *table_base;

    खुला:
    hb_bytes_t var_तथ्य_bytes;
    स्थिर TupleVariationHeader *current_tuple;
  };

  अटल bool get_tuple_iterator (hb_bytes_t var_तथ्य_bytes, अचिन्हित axis_गिनती,
                                  स्थिर व्योम *table_base,
                                  hb_vector_t<अचिन्हित पूर्णांक> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_तथ्य_bytes, axis_गिनती, table_base);
    अगर (!iterator->get_shared_indices (shared_indices))
      वापस false;
    वापस iterator->is_valid ();
  }

  bool has_shared_point_numbers () स्थिर { वापस tuple_Var_गिनती.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  bool cull_tuple_variations (hb_bytes_t var_तथ्य_bytes,
			      अचिन्हित axis_गिनती,
			      hb_array_t<स्थिर F2DOT14> shared_tuples,
			      स्थिर hb_map_t *axes_old_सूचकांक_tag_map,
			      स्थिर hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<अक्षर> &out /* OUT */,
			      bool *changed /* OUT */) स्थिर
  {
    *changed = false;

    hb_vector_t<अचिन्हित पूर्णांक> shared_indices;
    tuple_iterator_t iterator;
    अगर (!get_tuple_iterator (var_तथ्य_bytes, axis_गिनती, यह,
			     shared_indices, &iterator))
      वापस true; /* no tuples or malformed: keep original */

    स्थिर अक्षर *bytes_start = var_तथ्य_bytes.arrayZ;
    स्थिर अक्षर *bytes_end = bytes_start + var_तथ्य_bytes.लंबाई;
    स्थिर अक्षर *serialized_base = (स्थिर अक्षर *) &(यह+तथ्य);
    स्थिर अक्षर *shared_points_end = (स्थिर अक्षर *) iterator.get_serialized_तथ्य ();
    अगर (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      वापस true;

    काष्ठा kept_tuple_t
    {
      स्थिर अक्षर *header;
      अचिन्हित header_आकार;
      स्थिर अक्षर *तथ्य;
      अचिन्हित तथ्य_आकार;
    };
    hb_vector_t<kept_tuple_t> kept;
    अचिन्हित योग = 0;
    अचिन्हित kept_headers_आकार = 0, kept_तथ्य_आकार = 0;
    करो
    {
      स्थिर TupleVariationHeader *header = iterator.current_tuple;
      अचिन्हित header_आकार = header->get_आकार (axis_गिनती * 2);
      अचिन्हित तथ्य_आकार = header->get_तथ्य_आकार ();
      स्थिर अक्षर *tuple_तथ्य = (स्थिर अक्षर *) iterator.get_serialized_तथ्य ();
      अगर (unlikely (tuple_तथ्य < serialized_base ||
		    तथ्य_आकार > (अचिन्हित) (bytes_end - tuple_तथ्य)))
	वापस true; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      अगर (!header->unpack_axis_tuples (axis_गिनती, shared_tuples,
				       axes_old_सूचकांक_tag_map, axis_tuples))
	वापस true;

      योग++;
      अगर (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(स्थिर अक्षर *) header, header_आकार,
				 tuple_तथ्य, तथ्य_आकार});
	kept_headers_आकार += header_आकार;
	kept_तथ्य_आकार += तथ्य_आकार;
      }
    } जबतक (iterator.move_to_अग्र ());

    अगर (unlikely (kept.in_error ())) वापस false;
    अगर (kept.लंबाई == योग) वापस true; /* nothing to cull */

    *changed = true;
    अगर (!kept.लंबाई) वापस true; /* everything culled: no variation data */

    अचिन्हित shared_points_आकार = shared_points_end - serialized_base;
    अचिन्हित तथ्य_offset = min_आकार + kept_headers_आकार;
    अचिन्हित new_आकार = तथ्य_offset + shared_points_आकार + kept_तथ्य_आकार;
    अगर (unlikely (!out.resize (new_आकार)))
      वापस false;

    Tuple_Variation_तथ्य *out_तथ्य = (Tuple_Variation_तथ्य *) out.arrayZ;
    out_तथ्य->tuple_Var_गिनती = (uint16_t) (kept.लंबाई |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_तथ्य->तथ्य = तथ्य_offset;

    अक्षर *p = out.arrayZ + min_आकार;
    क्रम (स्थिर स्वतः &t : kept)
    {
      hb_memcpy (p, t.header, t.header_आकार);
      p += t.header_आकार;
    }
    hb_memcpy (p, serialized_base, shared_points_आकार);
    p += shared_points_आकार;
    क्रम (स्थिर स्वतः &t : kept)
    {
      hb_memcpy (p, t.तथ्य, t.तथ्य_आकार);
      p += t.तथ्य_आकार;
    }
    वापस true;
  }

  अटल bool decompile_points (स्थिर HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<अचिन्हित पूर्णांक> &points /* OUT */,
				स्थिर HBUINT8 *end)
  {
    क्रमागत packed_point_flag_t
    {
      POINTS_ARE_शब्दगण     = 0x80,
      POINT_RUN_गिनती_MASK = 0x7F
    };

    अगर (unlikely (p + 1 > end)) वापस false;

    अचिन्हित गिनती = *p++;
    अगर (गिनती & POINTS_ARE_शब्दगण)
    {
      अगर (unlikely (p + 1 > end)) वापस false;
      गिनती = ((गिनती & POINT_RUN_गिनती_MASK) << 8) | *p++;
    }
    अगर (unlikely (!points.resize_dirty  (गिनती))) वापस false;

    अचिन्हित n = 0;
    अचिन्हित i = 0;
    जबतक (i < गिनती)
    {
      अगर (unlikely (p + 1 > end)) वापस false;
      अचिन्हित control = *p++;
      अचिन्हित run_गिनती = (control & POINT_RUN_गिनती_MASK) + 1;
      अचिन्हित stop = i + run_गिनती;
      अगर (unlikely (stop > गिनती)) वापस false;
      अगर (control & POINTS_ARE_शब्दगण)
      {
        अगर (unlikely (p + run_गिनती * HBUINT16::static_आकार > end)) वापस false;
        क्रम (; i < stop; i++)
        {
          n += *(स्थिर HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_आकार;
        }
      }
      अन्यथा
      {
        अगर (unlikely (p + run_गिनती > end)) वापस false;
        क्रम (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    वापस true;
  }

  ढाँचा <typename T>
  HB_ALWAYS_INLINE
  अटल bool decompile_deltas (स्थिर HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				स्थिर HBUINT8 *end,
				bool consume_all = false,
				अचिन्हित start = 0)
  {
    वापस Tuple_मूल्यावली::decompile (p, deltas, end, consume_all, start);
  }

  bool has_तथ्य () स्थिर { वापस tuple_Var_गिनती; }

  bool decompile_tuple_variations (अचिन्हित point_गिनती,
                                   bool is_gvar,
                                   tuple_iterator_t iterator,
                                   स्थिर hb_map_t *axes_old_सूचकांक_tag_map,
                                   स्थिर hb_vector_t<अचिन्हित> &shared_indices,
                                   स्थिर hb_array_t<स्थिर F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   bool is_composite_glyph = false) स्थिर
  {
    वापस tuple_variations.create_from_tuple_var_तथ्य (iterator, tuple_Var_गिनती,
                                                        point_गिनती, is_gvar,
                                                        axes_old_सूचकांक_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  bool serialize (hb_serialize_context_t *c,
                  bool is_gvar,
                  स्थिर tuple_variations_t& tuple_variations) स्थिर
  {
    TRACE_SERIALIZE (यह);
    /* empty tuple variations, just return and skip serialization. */
    अगर (!tuple_variations) return_trace (true);

    स्वतः *out = c->start_embed (यह);
    अगर (unlikely (!c->extend_min (out))) return_trace (false);

    अगर (!c->check_assign (out->tuple_Var_गिनती, tuple_variations.get_var_गिनती (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    अचिन्हित योग_header_len = 0;

    अगर (!tuple_variations.serialize_var_headers (c, योग_header_len))
      return_trace (false);

    अचिन्हित तथ्य_offset = min_आकार + योग_header_len;
    अगर (!is_gvar) तथ्य_offset += 4;
    अगर (!c->check_assign (out->तथ्य, तथ्य_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);

    वापस tuple_variations.serialize_var_तथ्य (c, is_gvar);
  }

  रक्षित:
  काष्ठा Tuple_Var_गिनती : HBUINT16
  {
    मित्र काष्ठा tuple_variations_t;
    bool has_shared_point_numbers () स्थिर { वापस ((*यह) & SharedPointNumbers); }
    अचिन्हित पूर्णांक get_गिनती () स्थिर { वापस (*यह) & गिनती_Mask; }
    Tuple_Var_गिनती& चालक = (uint16_t i) { HBUINT16::चालक= (i); वापस *यह; }
    explicit चालक bool () स्थिर { वापस get_गिनती (); }

    रक्षित:
    क्रमागत Flags
    {
      SharedPointNumbers= 0x8000u,
      गिनती_Mask         = 0x0FFFu
    };
    खुला:
    DEFINE_आकार_STATIC (2);
  };

  Tuple_Var_गिनती tuple_Var_गिनती;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                तथ्य;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  खुला:
  DEFINE_आकार_MIN (2 + OffType::static_आकार);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = Tuple_Variation_तथ्य<HBUINT16>::tuple_variations_t;
काष्ठा item_variations_t
{
  using region_t = स्थिर hb_hashmap_t<hb_tag_t, Triple>*;
  निजी:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<अचिन्हित> var_तथ्य_अंक_rows;

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
  bool has_long = false;

  खुला:
  bool has_long_word () स्थिर
  { वापस has_long; }

  स्थिर hb_vector_t<region_t>& get_region_list () स्थिर
  { वापस region_list; }

  स्थिर hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () स्थिर
  { वापस encodings; }

  स्थिर hb_map_t& get_varidx_map () स्थिर
  { वापस varidx_map; }

  bool add_vardata_encoding_for_testing (hb_vector_t<स्थिर hb_vector_t<पूर्णांक>*> &&rows,
                                         अचिन्हित अंक_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), अंक_cols));
    वापस !encodings.in_error ();
  }

  bool compile_varidx_map_for_testing (स्थिर hb_hashmap_t<अचिन्हित, स्थिर hb_vector_t<पूर्णांक>*>& front_mapping)
  { वापस compile_varidx_map (front_mapping); }

  bool instantiate (स्थिर ItemVariationStore& varStore,
                    स्थिर hb_subset_plan_t *plan,
                    bool optimize=true,
                    bool use_no_variation_idx=true,
                    स्थिर hb_array_t <स्थिर hb_inc_bimap_t> inner_maps = hb_array_t<स्थिर hb_inc_bimap_t> (),
                    bool layout_conditions = false)
  {
    अगर (!create_from_item_varstore (varStore, plan->axes_old_सूचकांक_tag_map, inner_maps))
      वापस false;
    /* avar2 partial instancing: cull unreachable regions. */
    अगर (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      क्रम (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    अगर (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      वापस false;
    अगर (layout_conditions &&
        (!instantiate_condition_मूल्यावली (plan, varStore) || !build_region_list ()))
      वापस false;
    वापस as_item_varstore (optimize, use_no_variation_idx);
  }

  bool instantiate_condition_मूल्यावली (स्थिर hb_subset_plan_t *plan,
                                      स्थिर ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    स्वतः &condition_map = plan->layout_condition_idx_मूल्य_map;
    condition_map.सफा ();
    अगर (!plan->layout_condition_मूल्यावली) वापस true;
    using condition_मूल्य_t = hb_pair_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णांक>>;
    hb_vector_t<condition_मूल्य_t> conditions;
    क्रम (स्वतः condition : plan->layout_condition_मूल्यावली.iter ())
      conditions.push (condition);
    अगर (unlikely (conditions.in_error ())) वापस false;
    // Row order must not depend on the addresses of the source conditions.
    conditions.क्विक ([] (स्थिर condition_मूल्य_t &a, स्थिर condition_मूल्य_t &b) {
      अगर (a.second.first != b.second.first)
        वापस a.second.first < b.second.first ? -1 : 1;
      वापस a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    अचिन्हित new_outer = 0;
    क्रम (अचिन्हित i = 0; i < plan->gdef_varstore_inner_maps.लंबाई; i++)
      अगर (plan->gdef_varstore_inner_maps[i].get_population ())
        अगर (!outer_map.set (i, new_outer++)) वापस false;
    क्रम (स्वतः condition : conditions)
    {
      अचिन्हित old_idx = condition.second.first;
      द्विगुन value = condition.second.second +
                     varStore.get_delta<द्विगुन> (old_idx, plan->normalized_coords.as_array ());
      अचिन्हित old_outer = old_idx >> 16;
      अगर (!outer_map.has (old_outer))
        जारी;
      अचिन्हित outer = outer_map.लो (old_outer);
      अचिन्हित inner = plan->gdef_varstore_inner_maps[old_outer].लो (old_idx & 0xFFFF);
      अचिन्हित idx = (outer << 16) | inner;
      अगर (outer >= vars.लंबाई || inner >= var_तथ्य_अंक_rows[outer])
        जारी;

      द्विगुन maximum = भ_असल ((द्विगुन) value);
      bool variable = false;
      क्रम (स्थिर tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        अगर (unlikely (inner >= tuple.deltas_x.लंबाई)) वापस false;
        भग्न delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, भ_असल ((द्विगुन) delta));
      }
      अगर (!variable)
      {
        अगर (!condition_map.set (condition.first,
                                hb_pair (अचिन्हित (VarIdx::NO_VARIATION), पूर्णांक (value > 0))))
          वापस false;
        जारी;
      }

      द्विगुन scale = 1.;
      जबतक (maximum * scale > पूर्णांक_उच्च)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      जबतक (maximum * scale * 2 <= पूर्णांक_उच्च)
      {
        bool integral = (द्विगुन) value * scale == round ((द्विगुन) value * scale);
        क्रम (स्थिर tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (द्विगुन) tuple.deltas_x[inner] * scale ==
                      round ((द्विगुन) tuple.deltas_x[inner] * scale);
        अगर (integral) अवरोध;
        scale *= 2;
      }
      पूर्णांक default_मूल्य = (पूर्णांक) round ((द्विगुन) value * scale);
      अगर (scale != 1. || default_मूल्य < -32768 || default_मूल्य > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        क्रम (स्थिर tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          अगर (!tuple.deltas_x[inner]) जारी;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (true);
          copy.deltas_x.push ((भग्न) round ((द्विगुन) tuple.deltas_x[inner] * scale));
          अगर (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) वापस false;
          private_row.tuple_vars.push (std::move (copy));
        }
        अगर (unlikely (vars.लंबाई >= 0xFFFFu)) वापस false;
        अगर (unlikely (private_row.tuple_vars.in_error ())) वापस false;
        idx = vars.लंबाई << 16;
        vars.push (std::move (private_row));
        var_तथ्य_अंक_rows.push (1);
        अगर (unlikely (vars.in_error () || var_तथ्य_अंक_rows.in_error ())) वापस false;
        अगर (default_मूल्य < -32768 || default_मूल्य > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          पूर्णांक bias = default_मूल्य - hb_clamp (default_मूल्य, -32768, 32767);
          default_मूल्य -= bias;
          पूर्णांक ऊपरी = bias / 65536 * 65536;
          पूर्णांक parts[] = {ऊपरी, bias - ऊपरी};
          क्रम (पूर्णांक part : parts)
          {
            अगर (!part) जारी;
            अचिन्हित गिनती = vars[idx >> 16].tuple_vars.लंबाई;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            स्थिर स्वतः &tuples = vars[idx >> 16].tuple_vars;
            अगर (unlikely (tuples.in_error () || tuples.लंबाई != गिनती + 1 ||
                          tuples.पुच्छ ().indices.in_error () ||
                          tuples.पुच्छ ().deltas_x.in_error ())) वापस false;
          }
        }
      }
      अगर (!condition_map.set (condition.first, hb_pair (idx, default_मूल्य)))
        वापस false;
    }
    वापस !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  bool create_from_item_varstore (स्थिर ItemVariationStore& varStore,
                                  स्थिर hb_map_t& axes_old_सूचकांक_tag_map,
                                  स्थिर hb_array_t <स्थिर hb_inc_bimap_t> inner_maps = hb_array_t<स्थिर hb_inc_bimap_t> ())
  {
    स्थिर VarRegionList& regionList = varStore.get_region_list ();
    अगर (!regionList.get_var_regions (axes_old_सूचकांक_tag_map, orig_region_list))
      वापस false;

    अचिन्हित अंक_var_तथ्य = varStore.get_sub_table_गिनती ();
    अगर (inner_maps && inner_maps.लंबाई != अंक_var_तथ्य) वापस false;
    अगर (!vars.alloc (अंक_var_तथ्य) ||
        !var_तथ्य_अंक_rows.alloc (अंक_var_तथ्य)) वापस false;

    क्रम (अचिन्हित i = 0; i < अंक_var_तथ्य; i++)
    {
      अगर (inner_maps && !inner_maps.arrayZ[i].get_population ())
          जारी;
      tuple_variations_t var_तथ्य_tuples;
      अचिन्हित item_गिनती_2 = 0;
      अगर (!var_तथ्य_tuples.create_from_item_var_तथ्य (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_सूचकांक_tag_map,
                                                      item_गिनती_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        वापस false;

      var_तथ्य_अंक_rows.push (item_गिनती_2);
      vars.push (std::move (var_तथ्य_tuples));
    }
    वापस !vars.in_error () && !var_तथ्य_अंक_rows.in_error () && vars.लंबाई == var_तथ्य_अंक_rows.लंबाई;
  }

  bool instantiate_tuple_vars (स्थिर hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               स्थिर hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               bool build_regions = true)
  {
    optimize_scratch_t scratch;
    क्रम (tuple_variations_t& tuple_vars : vars)
      अगर (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        वापस false;

    वापस !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  अचिन्हित add_vardata (अचिन्हित item_गिनती_2)
  {
    vars.push (tuple_variations_t ());
    var_तथ्य_अंक_rows.push (item_गिनती_2);
    वापस vars.लंबाई - 1;
  }

  /* Get item count for a VarData subtable. */
  अचिन्हित get_item_गिनती (अचिन्हित outer) स्थिर
  { वापस outer < var_तथ्य_अंक_rows.लंबाई ? var_तथ्य_अंक_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  अचिन्हित duplicate_row (अचिन्हित outer, अचिन्हित inner)
  {
    अगर (unlikely (outer >= vars.लंबाई ||
                  inner >= var_तथ्य_अंक_rows[outer] ||
                  var_तथ्य_अंक_rows[outer] >= 0xFFFFu))
      वापस (अचिन्हित) -1;
    क्रम (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      अगर (unlikely (inner >= tuple.deltas_x.लंबाई ||
                    inner >= tuple.indices.लंबाई))
        वापस (अचिन्हित) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      अगर (inner < tuple.deltas_y.लंबाई)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      अगर (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        वापस (अचिन्हित) -1;
    }
    वापस var_तथ्य_अंक_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  व्योम add_tuple (अचिन्हित outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  अचिन्हित inner, पूर्णांक delta, अचिन्हित item_गिनती_2)
  {
    अगर (outer >= vars.लंबाई) वापस;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    अगर (!tuple.indices.resize (item_गिनती_2) ||
        !tuple.deltas_x.resize (item_गिनती_2)) वापस;
    क्रम (अचिन्हित i = 0; i < item_गिनती_2; i++)
    {
      tuple.indices.arrayZ[i] = true;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (भग्न) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  bool build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, अचिन्हित> all_regions;
    hb_hashmap_t<region_t, अचिन्हित> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    क्रम (स्थिर tuple_variations_t& sub_table : vars)
    {
      क्रम (स्थिर tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        अगर (!used_regions.has (r))
        {
          bool all_zeros = true;
          क्रम (भग्न d : tuple.deltas_x)
          {
            पूर्णांक delta = hb_clamp_to<पूर्णांक> (roundf ((द्विगुन) d));
            अगर (delta != 0)
            {
              all_zeros = false;
              अवरोध;
            }
          }
          अगर (!all_zeros)
          {
            अगर (!used_regions.set (r, 1))
              वापस false;
          }
        }
        अगर (all_regions.has (r))
          जारी;
        अगर (!all_regions.set (r, 1))
          वापस false;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    अगर (!all_regions || !all_unique_regions) वापस true;

    अगर (!region_list.alloc (all_regions.get_population ()))
      वापस false;

    अचिन्हित idx = 0;
    /* append the original regions that pre-existed */
    क्रम (स्थिर स्वतः& r : orig_region_list)
    {
      अगर (!all_regions.has (&r) || !used_regions.has (&r))
        जारी;

      region_list.push (&r);
      अगर (!region_map.set (&r, idx))
        वापस false;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    क्रम (स्थिर स्वतः& r: all_unique_regions)
    {
      अगर (!all_regions.has (r) || !used_regions.has (r))
        जारी;
      region_list.push (r);
      अगर (!region_map.set (r, idx))
        वापस false;
      all_regions.del (r);
      idx++;
    }
    वापस (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  काष्ठा combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = शेष;
    combined_gain_idx_tuple_t (अचिन्हित gain, अचिन्हित i, अचिन्हित j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      निश्चित (gain < 0xFFFFFF);
      निश्चित (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    bool चालक < (स्थिर combined_gain_idx_tuple_t& o)
    {
      वापस encoded < o.encoded;
    }

    bool चालक <= (स्थिर combined_gain_idx_tuple_t& o)
    {
      वापस encoded <= o.encoded;
    }

    अचिन्हित idx_1 () स्थिर { वापस (encoded >> 20) & 0xFFFFF; };
    अचिन्हित idx_2 () स्थिर { वापस encoded & 0xFFFFF; };
  };

  bool as_item_varstore (bool optimize=true, bool use_no_variation_idx=true)
  {
    /* return true if no variation data */
    अगर (!region_list) वापस true;
    अचिन्हित अंक_cols = region_list.लंबाई;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    अचिन्हित योग_rows = 0;
    क्रम (अचिन्हित major = 0; major < var_तथ्य_अंक_rows.लंबाई; major++)
      योग_rows += var_तथ्य_अंक_rows[major];

    अगर (!delta_rows.resize (योग_rows)) वापस false;
    /* init all rows to [0]*num_cols */
    क्रम (अचिन्हित i = 0; i < योग_rows; i++)
      अगर (!(delta_rows[i].resize (अंक_cols))) वापस false;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<अचिन्हित, स्थिर hb_vector_t<पूर्णांक>*> front_mapping;
    अचिन्हित start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<स्थिर hb_vector_t<पूर्णांक> *> major_rows;
    hb_hashmap_t<स्थिर hb_vector_t<पूर्णांक>*, अचिन्हित> delta_rows_map;
    क्रम (अचिन्हित major = 0; major < vars.लंबाई; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      स्थिर tuple_variations_t& tuples = vars[major];
      अचिन्हित अंक_rows = var_तथ्य_अंक_rows[major];

      अगर (!अंक_rows) जारी;

      क्रम (स्थिर tuple_delta_t& tuple: tuples.tuple_vars)
      {
        अगर (tuple.deltas_x.लंबाई != अंक_rows)
          वापस false;

        /* skip unused regions */
        अचिन्हित *col_idx;
        अगर (!region_map.has (&(tuple.axis_tuples), &col_idx))
          जारी;

        क्रम (अचिन्हित i = 0; i < अंक_rows; i++)
        {
          पूर्णांक rounded_delta = hb_clamp_to<पूर्णांक> (roundf ((द्विगुन) tuple.deltas_x[i]));
          पूर्णांक &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          अगर (unlikely (sum < पूर्णांक_न्यून || sum > पूर्णांक_उच्च)) वापस false;
          delta = (पूर्णांक) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      क्रम (अचिन्हित minor = 0; minor < अंक_rows; minor++)
      {
	स्थिर hb_vector_t<पूर्णांक>& row = delta_rows[start_row + minor];
	अगर (use_no_variation_idx)
	{
	  bool all_zeros = true;
	  क्रम (पूर्णांक delta : row)
	  {
	    अगर (delta != 0)
	    {
	      all_zeros = false;
	      अवरोध;
	    }
	  }
	  अगर (all_zeros)
	    जारी;
	}

	अगर (!front_mapping.set ((major<<16) + minor, &row))
	  वापस false;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	अगर (optimize)
	{
	  अगर (delta_rows_map.has (&row))
	    जारी;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      अगर (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        वापस false;
      अगर (major_rows)
      {
	स्वतः *encoding = encoding_objs.push (std::move (major_rows), अंक_cols);
	अगर (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  वापस false;
      }

      start_row += अंक_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    अगर (!optimize)
    {
      encodings = std::move (encoding_objs);
      वापस !encodings.in_error ();
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
    अचिन्हित अंक_todos = encoding_objs.लंबाई;
    क्रम (अचिन्हित i = 0; i < अंक_todos; i++)
    {
      क्रम (अचिन्हित j = i + 1; j < अंक_todos; j++)
      {
        पूर्णांक combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        अगर (combining_gain > 0)
          queue_वस्तुएँ.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_वस्तुएँ));

    hb_bit_set_t removed_todo_idxes;
    जबतक (queue)
    {
      स्वतः t = queue.निकालो_minimum ().first;
      अचिन्हित i = t.idx_1 ();
      अचिन्हित j = t.idx_2 ();

      अगर (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        जारी;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      अगर (unlikely (removed_todo_idxes.in_error ())) वापस false;

      encoding.merge (other_encoding);
      अगर (unlikely (encoding.in_error ())) वापस false;

      क्रम (अचिन्हित idx = 0; idx < encoding_objs.लंबाई; idx++)
      {
        अगर (removed_todo_idxes.has (idx)) जारी;

        स्थिर delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        अगर (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          क्रम (स्थिर स्वतः& row : obj.वस्तुएँ)
            encoding.add_row (row);
          अगर (unlikely (encoding.in_error ())) वापस false;

          removed_todo_idxes.add (idx);
          अगर (unlikely (removed_todo_idxes.in_error ())) वापस false;
          जारी;
        }

        पूर्णांक combined_gain = encoding.gain_from_merging (obj);
        अगर (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.लंबाई), 0);
      }

      स्वतः moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      अगर (unlikely (encoding_objs.in_error ())) वापस false;
    }

    पूर्णांक अंक_final_encodings = (पूर्णांक) encoding_objs.लंबाई - (पूर्णांक) removed_todo_idxes.get_population ();
    अगर (अंक_final_encodings <= 0) वापस false;

    अगर (!encodings.alloc (अंक_final_encodings)) वापस false;
    क्रम (अचिन्हित i = 0; i < encoding_objs.लंबाई; i++)
    {
      अगर (removed_todo_idxes.has (i)) जारी;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    वापस compile_varidx_map (front_mapping);
  }

  निजी:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  bool compile_varidx_map (स्थिर hb_hashmap_t<अचिन्हित, स्थिर hb_vector_t<पूर्णांक>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<स्थिर hb_vector_t<पूर्णांक>*, अचिन्हित> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    क्रम (अचिन्हित i = 0; i < encodings.लंबाई; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      अगर (encoding.is_empty () || encoding.in_error ())
        वापस false;

      अचिन्हित अंक_rows = encoding.वस्तुएँ.लंबाई;
      अचिन्हित अंक_cols = encoding.chars.लंबाई;

      /* sort rows, make result deterministic */
      encoding.वस्तुएँ.क्विक (_cmp_row);

      क्रम (अचिन्हित start = 0; start < अंक_rows; start += 0xFFFFu)
      {
        अचिन्हित chunk_len = hb_min (अंक_rows - start, 0xFFFFu);
        hb_vector_t<स्थिर hb_vector_t<पूर्णांक>*> rows;

        अगर (!rows.alloc (chunk_len))
          वापस false;

        अचिन्हित major = split_encodings.लंबाई;
        क्रम (अचिन्हित minor = 0; minor < chunk_len; minor++)
        {
          स्थिर hb_vector_t<पूर्णांक> *row = encoding.वस्तुएँ.arrayZ[start + minor];
          rows.push (row);
          अगर (!back_mapping.set (row, (major << 16) + minor))
            वापस false;
        }

        स्वतः *split = split_encodings.push (std::move (rows), अंक_cols);
        अगर (unlikely (split_encodings.in_error () || split->in_error ()))
          वापस false;
      }
    }

    encodings = std::move (split_encodings);
    अगर (encodings.in_error () || back_mapping.in_error ())
      वापस false;

    क्रम (स्वतः _ : front_mapping.iter ())
    {
      अचिन्हित old_varidx = _.first;
      अचिन्हित *new_varidx;
      अगर (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      अन्यथा
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक);
    }
    वापस !varidx_map.in_error ();
  }

  अटल पूर्णांक _cmp_row (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    स्थिर hb_vector_t<पूर्णांक>** a = (स्थिर hb_vector_t<पूर्णांक>**) pa;
    स्थिर hb_vector_t<पूर्णांक>** b = (स्थिर hb_vector_t<पूर्णांक>**) pb;

    क्रम (अचिन्हित i = 0; i < (*b)->लंबाई; i++)
    {
      पूर्णांक va = (*a)->arrayZ[i];
      पूर्णांक vb = (*b)->arrayZ[i];
      अगर (va != vb)
        वापस va < vb ? -1 : 1;
    }
    वापस 0;
  }
};


} /* namespace OT */


#पूर्ण_अगर /* HB_OT_VAR_COMMON_HH */

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

#અન્તર્ભાવ "hb-ot-layout-common.hh"
#અન્તર્ભાવ "hb-alloc-pool.hh"
#અન્તર્ભાવ "hb-priority-queue.hh"
#અન્તર્ભાવ "hb-subset-instancer-iup.hh"


નામાકાશ OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
સંરચના TupleVariationHeader
{
  friend સંરચના tuple_delta_t;
  size_t get_size (unsigned axis_ગણતરી_times_2) અચલ
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    જો (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned ગણતરી = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      પાછા hb_unsigned_mul_add_saturate (ગણતરી, axis_ગણતરી_times_2, min_size);
    }
    પાછા min_size;
  }

  unsigned get_data_size () અચલ { પાછા varDataSize; }

  અચલ TupleVariationHeader &get_next (unsigned axis_ગણતરી_times_2) અચલ
  { પાછા StructAtOffset<TupleVariationHeader> (આ, get_size (axis_ગણતરી_times_2)); }

  તાર્કિક unpack_axis_tuples (unsigned axis_ગણતરી,
                           અચલ hb_array_t<અચલ F2DOT14> shared_tuples,
                           અચલ hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) અચલ
  {
    અચલ F2DOT14 *peak_tuple = nullptr;
    જો (has_peak ())
      peak_tuple = get_peak_tuple (axis_ગણતરી);
    નહીંતર
    {
      unsigned પૂર્ણાંક index = get_index ();
      જો (unlikely ((index + 1) * axis_ગણતરી > shared_tuples.length))
        પાછા ખોટું;
      peak_tuple = shared_tuples.sub_array (axis_ગણતરી * index, axis_ગણતરી).arrayZ;
    }

    અચલ F2DOT14 *start_tuple = nullptr;
    અચલ F2DOT14 *end_tuple = nullptr;
    તાર્કિક has_interm = has_intermediate ();

    જો (has_interm)
    {
      start_tuple = get_start_tuple (axis_ગણતરી);
      end_tuple = get_end_tuple (axis_ગણતરી);
    }

    માટે (unsigned i = 0; i < axis_ગણતરી; i++)
    {
      દશાંશ peak = peak_tuple[i].to_float ();
      જો (peak == 0.f) ચાલુ;

      hb_tag_t *axis_tag;
      જો (!axes_old_index_tag_map->has (i, &axis_tag))
        પાછા ખોટું;

      દશાંશ start, end;
      જો (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      નહીંતર
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    પાછા સાચું;
  }

  HB_ALWAYS_INLINE
  દશાંશ calculate_scalar (hb_array_t<અચલ પૂર્ણાંક> coords, unsigned પૂર્ણાંક coord_ગણતરી,
			   અચલ hb_array_t<અચલ F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) અચલ
  {
    unsigned tuple_index = tupleIndex;
    unsigned પૂર્ણાંક index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    તાર્કિક plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    જો (likely (plain_shared_tuple))
    {
      દશાંશ scalar;
      જો (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	પાછા scalar;
    }

    અચલ F2DOT14 *peak_tuple;

    તાર્કિક has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    જો (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_ગણતરી);
      shared_tuple_scalar_cache = nullptr;
    }
    નહીંતર
    {
      દશાંશ scalar;
      જો (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        જો (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	નહીંતર
	  પાછા scalar;
      }

      જો (unlikely ((index + 1) * coord_ગણતરી > shared_tuples.length))
        પાછા 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_ગણતરી * index);

    }

    અચલ F2DOT14 *start_tuple = nullptr;
    અચલ F2DOT14 *end_tuple = nullptr;

    જો (has_interm)
    {
      start_tuple = get_start_tuple (coord_ગણતરી);
      end_tuple = get_end_tuple (coord_ગણતરી);
    }

    દશાંશ scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#જો HB_FAST_અંક_ACCESS
    તાર્કિક skip = coord_ગણતરી >= 16;
#endif
#endif
    માટે (unsigned પૂર્ણાંક i = 0; i < coord_ગણતરી; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#જો HB_FAST_અંક_ACCESS
      જો (skip)
      {
	જ્યારે (i + 4 <= coord_ગણતરી && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	જ્યારે (i < coord_ગણતરી && peak_tuple[i].to_int () == 0)
	  i += 1;
	જો (i >= coord_ગણતરી)
	  તોડો;
      }
#endif
#endif

      પૂર્ણાંક peak = peak_tuple[i].to_int ();
      જો (!peak) ચાલુ;

      પૂર્ણાંક v = coords[i];
      જો (!v) { scalar = 0.f; તોડો; }
      જો (v == peak) ચાલુ;

      જો (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        પૂર્ણાંક start = start_tuple[i].to_int ();
        પૂર્ણાંક end = end_tuple[i].to_int ();
        જો (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) ચાલુ;
        જો (v < start || v > end) { scalar = 0.f; તોડો; }
        જો (v < peak)
        { જો (peak != start) scalar *= (દશાંશ) (v - start) / (peak - start); }
        નહીંતર
        { જો (peak != end) scalar *= (દશાંશ) (end - v) / (end - peak); }
      }
      નહીંતર જો (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; તોડો; }
      નહીંતર
        scalar *= (દશાંશ) v / peak;
    }
    જો (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    પાછા scalar;
  }

  તાર્કિક           has_peak () અચલ { પાછા tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  તાર્કિક   has_intermediate () અચલ { પાછા tupleIndex & TupleIndex::IntermediateRegion; }
  તાર્કિક has_private_points () અચલ { પાછા tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () અચલ { પાછા tupleIndex & TupleIndex::TupleIndexMask; }
  સ્થિર constexpr unsigned max_shared_tuple_ગણતરી = 0x1000u;

  સુરક્ષિત:
  સંરચના TupleIndex : HBUINT16
  {
    ગણના Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); પાછા *આ; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<અચલ F2DOT14> get_all_tuples (unsigned axis_ગણતરી) અચલ
  { પાછા StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_ગણતરી); }
  અચલ F2DOT14* get_all_tuples_base (unsigned axis_ગણતરી) અચલ
  { પાછા StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  અચલ F2DOT14* get_peak_tuple (unsigned axis_ગણતરી) અચલ
  { પાછા get_all_tuples_base (axis_ગણતરી); }
  અચલ F2DOT14* get_start_tuple (unsigned axis_ગણતરી) અચલ
  { પાછા get_all_tuples_base (axis_ગણતરી) + has_peak () * axis_ગણતરી; }
  અચલ F2DOT14* get_end_tuple (unsigned axis_ગણતરી) અચલ
  { પાછા get_all_tuples_base (axis_ગણતરી) + has_peak () * axis_ગણતરી + axis_ગણતરી; }

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
  જાહેર:
  DEFINE_SIZE_MIN (4);
};

સંરચના optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<તાર્કિક> opt_indices;
  hb_vector_t<પૂર્ણાંક> rounded_x_deltas;
  hb_vector_t<પૂર્ણાંક> rounded_y_deltas;
  hb_vector_t<દશાંશ> opt_deltas_x;
  hb_vector_t<દશાંશ> opt_deltas_y;
  hb_vector_t<unsigned અક્ષર> opt_point_data;
  hb_vector_t<unsigned અક્ષર> opt_deltas_data;
  hb_vector_t<unsigned અક્ષર> point_data;
  hb_vector_t<unsigned અક્ષર> deltas_data;
  hb_vector_t<પૂર્ણાંક> rounded_deltas;
};

સંરચના tuple_delta_t
{
  સ્થિર constexpr તાર્કિક realloc_move = સાચું;  // Watch out when adding new members!

  જાહેર:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<તાર્કિક> indices;

  hb_vector_t<દશાંશ> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<દશાંશ> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned અક્ષર> compiled_tuple_header;
  hb_vector_t<unsigned અક્ષર> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (અચલ tuple_delta_t& o) = મૂળભૂત;
  tuple_delta_t& operator = (અચલ tuple_delta_t& o) = મૂળભૂત;

  friend શૂન્ય swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*આ, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*આ, o);
    પાછા *આ;
  }

  શૂન્ય copy_from (અચલ tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  શૂન્ય remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  તાર્કિક set_tent (hb_tag_t axis_tag, Triple tent)
  { પાછા axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (અચલ tuple_delta_t& o)
  {
    unsigned અંક = indices.length;
    માટે (unsigned i = 0; i < અંક; i++)
    {
      જો (indices.arrayZ[i])
      {
        જો (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          જો (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      નહીંતર
      {
        જો (!o.indices.arrayZ[i]) ચાલુ;
        indices.arrayZ[i] = સાચું;
        deltas_x[i] = o.deltas_x[i];
        જો (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    પાછા *આ;
  }

  tuple_delta_t& operator *= (દશાંશ scalar)
  {
    જો (scalar == 1.0f)
      પાછા *આ;

    unsigned અંક = indices.length;
    જો (deltas_y)
      માટે (unsigned i = 0; i < અંક; i++)
      {
	જો (!indices.arrayZ[i]) ચાલુ;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    નહીંતર
      માટે (unsigned i = 0; i < અંક; i++)
      {
	જો (!indices.arrayZ[i]) ચાલુ;
	deltas_x[i] *= scalar;
      }
    પાછા *આ;
  }

  શૂન્ય change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    જો (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*આ));
      પાછા;
    }

    જો ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->મધ્ય && tent->મધ્ય <= tent->maximum))
      પાછા;

    જો (tent->મધ્ય == 0.f)
    {
      out.push (std::move (*આ));
      પાછા;
    }

    જો (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.મધ્ય &&
          axis_limit.મધ્ય <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      પાછા;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    માટે (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      જો (i < solutions.length - 1)
	new_var.copy_from (*આ, pool);
      નહીંતર
	new_var = std::move (*આ);

      જો (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      નહીંતર
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  તાર્કિક compile_coords (અચલ hb_map_t& axes_index_map,
		       અચલ hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_ગણતરી = axes_index_map.get_population ();
    જો (pool)
    {
      જો (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_ગણતરી)))
	પાછા ખોટું;
    }
    નહીંતર જો (unlikely (!compiled_peak_coords.resize (cur_axis_ગણતરી)))
      પાછા ખોટું;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_ગણતરી = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    માટે (unsigned i = 0; i < orig_axis_ગણતરી; i++)
    {
      જો (!axes_index_map.has (i))
        ચાલુ;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      જો (axis_tuples.has (axis_tag, &coords))
      {
	દશાંશ min_val = coords->minimum;
	દશાંશ val = coords->મધ્ય;
	દશાંશ max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	જો (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  જો (!compiled_interm_coords)
	  {
	    જો (pool)
	    {
	      જો (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_ગણતરી)))
		પાછા ખોટું;
	    }
	    નહીંતર જો (unlikely (!compiled_interm_coords.resize (2 * cur_axis_ગણતરી)))
	      પાછા ખોટું;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_ગણતરી);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_ગણતરી);

	    માટે (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      જો (!peak) ચાલુ;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	જો (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    પાછા !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  તાર્કિક compile_tuple_var_header (અચલ hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 અચલ hb_map_t& axes_old_index_tag_map,
                                 અચલ hb_hashmap_t<અચલ hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    જો (!compiled_deltas) પાછા સાચું;

    unsigned cur_axis_ગણતરી = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_ગણતરી * (F2DOT14::static_size) + 4;
    જો (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, ખોટું))) પાછા ખોટું;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    જો (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_ગણતરી = 0;
    unsigned *shared_tuple_idx;
    જો (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    નહીંતર
    {
      peak_ગણતરી = encode_peak_coords(coords, flag);
      જો (!peak_ગણતરી) પાછા ખોટું;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_ગણતરી = encode_interm_coords (coords.sub_array (peak_ગણતરી), flag);

    /* pointdata length = 0 implies "use shared points" */
    જો (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_ગણતરી + interim_ગણતરી) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    પાછા સાચું;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) અચલ
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * કદ (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    પાછા compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) અચલ
  {
    જો (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * કદ (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    પાછા compiled_interm_coords.length;
  }

  તાર્કિક compile_deltas (hb_vector_t<પૂર્ણાંક> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { પાછા compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  સ્થિર તાર્કિક compile_deltas (hb_array_t<અચલ તાર્કિક> point_indices,
			      hb_array_t<અચલ દશાંશ> x_deltas,
			      hb_array_t<અચલ દશાંશ> y_deltas,
			      hb_vector_t<unsigned અક્ષર> &compiled_deltas, /* OUT */
			      hb_vector_t<પૂર્ણાંક> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    જો (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      પાછા ખોટું;

    unsigned j = 0;
    માટે (unsigned i = 0; i < point_indices.length; i++)
    {
      જો (!point_indices[i]) ચાલુ;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<પૂર્ણાંક> (roundf ((દ્વિગુણ) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    જો (!rounded_deltas) પાછા સાચું;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    જો (y_deltas)
      alloc_len *= 2;

    જો (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, ખોટું))) પાછા ખોટું;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    જો (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      માટે (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        જો (!point_indices[idx]) ચાલુ;
        પૂર્ણાંક rounded_delta = hb_clamp_to<પૂર્ણાંક> (roundf ((દ્વિગુણ) y_deltas.arrayZ[idx]));

        જો (j >= rounded_deltas.length) પાછા ખોટું;

        rounded_deltas[j++] = rounded_delta;
      }

      જો (j != rounded_deltas.length) પાછા ખોટું;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    પાછા સાચું;
  }

  સ્થિર unsigned compile_deltas (hb_array_t<unsigned અક્ષર> encoded_bytes,
				  hb_array_t<અચલ પૂર્ણાંક> deltas)
  {
    પાછા Tuple_કિંમતો::compile_unsafe (deltas, encoded_bytes);
  }

  તાર્કિક calc_inferred_deltas (અચલ contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_ગણતરી = orig_points.length;
    જો (point_ગણતરી != indices.length)
      પાછા ખોટું;

    unsigned ref_ગણતરી_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    માટે (unsigned i = 0; i < point_ગણતરી; i++)
    {
      ref_ગણતરી_2 += indices.arrayZ[i];
      જો (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    જો (ref_ગણતરી_2 == point_ગણતરી)
      પાછા સાચું;
    જો (unlikely (end_points.in_error ())) પાછા ખોટું;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    માટે (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_ગણતરી = 0;
      માટે (unsigned i = start_point; i < end_point + 1; i++)
        unref_ગણતરી += indices.arrayZ[i];
      unref_ગણતરી = (end_point - start_point + 1) - unref_ગણતરી;

      unsigned j = start_point;
      જો (unref_ગણતરી == 0 || unref_ગણતરી > end_point - start_point)
        જાઓ no_more_gaps;
      માટે (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned પૂર્ણાંક prev, next, i;
        માટે (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          જો (indices.arrayZ[i] && !indices.arrayZ[j]) તોડો;
        }
        prev = j = i;
        માટે (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          જો (!indices.arrayZ[i] && indices.arrayZ[j]) તોડો;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        માટે (;;)
        {
          i = next_index (i, start_point, end_point);
          જો (i == next) તોડો;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          જો (--unref_ગણતરી == 0) જાઓ no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    માટે (unsigned i = 0; i < point_ગણતરી; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      જો ( !indices[i])
      {
        જો (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = સાચું;
      }
    }
    પાછા સાચું;
  }

  તાર્કિક optimize (અચલ contour_point_vector_t& contour_points,
                 તાર્કિક is_composite,
		 optimize_scratch_t &scratch,
                 દ્વિગુણ tolerance = 0.5 + 1e-10)
  {
    unsigned ગણતરી = contour_points.length;
    જો (deltas_x.length != ગણતરી ||
        deltas_y.length != ગણતરી)
      પાછા ખોટું;

    hb_vector_t<તાર્કિક> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<પૂર્ણાંક> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<પૂર્ણાંક> &rounded_y_deltas = scratch.rounded_y_deltas;

    જો (unlikely (!rounded_x_deltas.resize_dirty  (ગણતરી) ||
                  !rounded_y_deltas.resize_dirty  (ગણતરી)))
      પાછા ખોટું;

    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<પૂર્ણાંક> (roundf ((દ્વિગુણ) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<પૂર્ણાંક> (roundf ((દ્વિગુણ) deltas_y.arrayZ[i]));
    }

    જો (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      પાછા ખોટું;

    unsigned ref_ગણતરી_2 = 0;
    માટે (તાર્કિક ref_flag : opt_indices)
       ref_ગણતરી_2 += ref_flag;

    જો (ref_ગણતરી_2 == ગણતરી) પાછા સાચું;

    hb_vector_t<દશાંશ> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<દશાંશ> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    તાર્કિક is_comp_glyph_wo_deltas = (is_composite && ref_ગણતરી_2 == 0);
    જો (is_comp_glyph_wo_deltas)
    {
      જો (unlikely (!opt_deltas_x.resize (ગણતરી) ||
                    !opt_deltas_y.resize (ગણતરી)))
        પાછા ખોટું;

      opt_indices.arrayZ[0] = સાચું;
      માટે (unsigned i = 1; i < ગણતરી; i++)
        opt_indices.arrayZ[i] = ખોટું;
    }

    hb_vector_t<unsigned અક્ષર> &opt_point_data = scratch.opt_point_data.reset ();
    જો (!compile_point_set (opt_indices, opt_point_data))
      પાછા ખોટું;
    hb_vector_t<unsigned અક્ષર> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    જો (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      પાછા ખોટું;

    hb_vector_t<unsigned અક્ષર> &point_data = scratch.point_data.reset ();
    જો (!compile_point_set (indices, point_data))
      પાછા ખોટું;
    hb_vector_t<unsigned અક્ષર> &deltas_data = scratch.deltas_data.reset ();
    જો (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      પાછા ખોટું;

    જો (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      જો (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    પાછા !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  સ્થિર તાર્કિક compile_point_set (અચલ hb_vector_t<તાર્કિક> &point_indices,
                                 hb_vector_t<unsigned અક્ષર>& compiled_points /* OUT */)
  {
    unsigned અંક_points = 0;
    માટે (તાર્કિક i : point_indices)
      જો (i) અંક_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    જો (!અંક_points) પાછા સાચું;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    જો (અંક_points == indices_length)
      પાછા compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned અંક_bytes = 2 + 3 *અંક_points;
    જો (unlikely (!compiled_points.resize_dirty  (અંક_bytes)))
      પાછા ખોટું;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    જો (અંક_points < 0x80)
      compiled_points.arrayZ[pos++] = અંક_points;
    નહીંતર
    {
      compiled_points.arrayZ[pos++] = ((અંક_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = અંક_points & 0xFF;
    }

    અચલ unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_કિંમત = 0;
    unsigned અંક_encoded = 0;
    જ્યારે (i < indices_length && અંક_encoded < અંક_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      તાર્કિક use_byte_encoding = ખોટું;
      તાર્કિક new_run = સાચું;
      જ્યારે (i < indices_length && અંક_encoded < અંક_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        જ્યારે (i < indices_length && !point_indices[i])
          i++;

        જો (i >= indices_length) તોડો;

        unsigned cur_કિંમત = i;
        unsigned delta = cur_કિંમત - last_કિંમત;

        જો (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = ખોટું;
        }

        જો (use_byte_encoding && delta > 0xFF)
          તોડો;

        જો (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        નહીંતર
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_કિંમત = cur_કિંમત;
        run_length++;
        અંક_encoded++;
      }

      જો (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      નહીંતર
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    પાછા compiled_points.resize_dirty  (pos);
  }

  સ્થિર દશાંશ infer_delta (દશાંશ target_val, દશાંશ prev_val, દશાંશ next_val, દશાંશ prev_delta, દશાંશ next_delta)
  {
    જો (prev_val == next_val)
      પાછા (prev_delta == next_delta) ? prev_delta : 0.f;
    નહીંતર જો (target_val <= hb_min (prev_val, next_val))
      પાછા (prev_val < next_val) ? prev_delta : next_delta;
    નહીંતર જો (target_val >= hb_max (prev_val, next_val))
      પાછા (prev_val > next_val) ? prev_delta : next_delta;

    દશાંશ r = (target_val - prev_val) / (next_val - prev_val);
    પાછા prev_delta + r * (next_delta - prev_delta);
  }

  સ્થિર unsigned પૂર્ણાંક next_index (unsigned પૂર્ણાંક i, unsigned પૂર્ણાંક start, unsigned પૂર્ણાંક end)
  { પાછા (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
સંરચના TupleVariationData
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (આ));
  }

  size_t get_size (unsigned axis_ગણતરી_times_2) અચલ
  {
    size_t total_size = min_size;
    unsigned ગણતરી = tuple_Var_ગણતરી.get_ગણતરી ();
    અચલ TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_ગણતરી_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_ગણતરી_times_2);
    }

    પાછા total_size;
  }

  અચલ TupleVariationHeader &get_tuple_var_header (શૂન્ય) અચલ
  { પાછા StructAfter<TupleVariationHeader> (data); }

  સંરચના tuple_iterator_t;
  સંરચના tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    ખાનગી:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<અચલ hb_vector_t<તાર્કિક>*, hb_vector_t<unsigned અક્ષર>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<અચલ hb_vector_t<તાર્કિક>*, unsigned> point_set_ગણતરી_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned અક્ષર> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    તાર્કિક needs_padding = ખોટું;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    તાર્કિક is_composite = ખોટું;

    જાહેર:
    tuple_variations_t () = મૂળભૂત;
    tuple_variations_t (અચલ tuple_variations_t&) = delete;
    tuple_variations_t& operator=(અચલ tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = મૂળભૂત;
    tuple_variations_t& operator=(tuple_variations_t&&) = મૂળભૂત;
    ~tuple_variations_t () = મૂળભૂત;

    explicit operator તાર્કિક () અચલ { પાછા તાર્કિક (tuple_vars); }
    unsigned get_var_ગણતરી () અચલ
    {
      unsigned ગણતરી = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      માટે (auto& tuple: tuple_vars)
        જો (tuple.compiled_deltas) ગણતરી++;

      જો (shared_points_bytes && shared_points_bytes->length)
        ગણતરી |= Tuple_Var_ગણતરી::SharedPointNumbers;
      પાછા ગણતરી;
    }

    unsigned get_compiled_byte_size () અચલ
    { પાછા compiled_byte_size; }

    તાર્કિક create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_ગણતરી,
                                     unsigned point_ગણતરી,
                                     તાર્કિક is_gvar,
                                     અચલ hb_map_t *axes_old_index_tag_map,
                                     અચલ hb_vector_t<unsigned> &shared_indices,
                                     અચલ hb_array_t<અચલ F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     તાર્કિક is_composite_glyph = ખોટું)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<પૂર્ણાંક> deltas_x;
      hb_vector_t<પૂર્ણાંક> deltas_y;
      કરો
      {
        અચલ HBUINT8 *p = iterator.get_serialized_data ();
        unsigned પૂર્ણાંક length = iterator.current_tuple->get_data_size ();
        જો (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          પાછા ખોટું;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        જો (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_ગણતરી (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          પાછા ખોટું;

	private_indices.reset ();
        તાર્કિક has_private_points = iterator.current_tuple->has_private_points ();
        અચલ HBUINT8 *end = p + length;
        જો (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          પાછા ખોટું;

        અચલ hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        તાર્કિક apply_to_all = (indices.length == 0);
        unsigned અંક_deltas = apply_to_all ? point_ગણતરી : indices.length;

        જો (unlikely (!deltas_x.resize_dirty  (અંક_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          પાછા ખોટું;

        જો (is_gvar)
        {
          જો (unlikely (!deltas_y.resize_dirty  (અંક_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            પાછા ખોટું;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        જો (unlikely (!var.indices.allocate_from_pool (pool, point_ગણતરી) ||
                      !var.deltas_x.allocate_from_pool (pool, point_ગણતરી, ખોટું)))
          પાછા ખોટું;

        જો (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_ગણતરી, ખોટું)))
          પાછા ખોટું;

        માટે (unsigned i = 0; i < અંક_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          જો (idx >= point_ગણતરી) ચાલુ;
          var.indices[idx] = સાચું;
          var.deltas_x[idx] = deltas_x[i];
          જો (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } જ્યારે (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      પાછા સાચું;
    }

    તાર્કિક create_from_item_var_data (અચલ VarData &var_data,
                                    અચલ hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    અચલ hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_ગણતરી_2,
                                    અચલ hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      જો (&var_data == &Null (VarData))
        પાછા સાચું;

      unsigned અંક_regions = var_data.get_region_index_ગણતરી ();
      જો (!tuple_vars.alloc (અંક_regions)) પાછા ખોટું;

      item_ગણતરી_2 = inner_map ? inner_map->get_population () : var_data.get_item_ગણતરી ();
      જો (!item_ગણતરી_2) પાછા સાચું;
      unsigned row_size = var_data.get_row_size ();
      અચલ HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      માટે (unsigned r = 0; r < અંક_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        જો (!tuple.deltas_x.resize_dirty  (item_ગણતરી_2) ||
            !tuple.indices.resize_dirty  (item_ગણતરી_2))
          પાછા ખોટું;

        માટે (unsigned i = 0; i < item_ગણતરી_2; i++)
        {
          tuple.indices.arrayZ[i] = સાચું;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        જો (region_index >= regions.length) પાછા ખોટું;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      પાછા !tuple_vars.in_error ();
    }

    ખાનગી:
    સ્થિર પૂર્ણાંક _cmp_axis_tag (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
    {
      અચલ hb_tag_t *a = (અચલ hb_tag_t*) pa;
      અચલ hb_tag_t *b = (અચલ hb_tag_t*) pb;
      પાછા (પૂર્ણાંક)(*a) - (પૂર્ણાંક)(*b);
    }

    તાર્કિક change_tuple_variations_axis_limits (અચલ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              અચલ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      જો (!axis_tags.alloc (normalized_axes_location.get_population ()))
        પાછા ખોટું;
      માટે (auto t : normalized_axes_location.ચાવીઓ ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      માટે (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        જો (!normalized_axes_location.has (axis_tag, &axis_limit))
          પાછા ખોટું;
        TripleDistances axis_triple_distances{1.f, 1.f};
        જો (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        માટે (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          જો (!out) ચાલુ;

          unsigned new_len = new_vars.length + out.length;

          જો (unlikely (!new_vars.alloc (new_len, ખોટું)))
            પાછા ખોટું;

          માટે (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      પાછા સાચું;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    તાર્કિક merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      જો (unlikely (!new_vars.alloc (tuple_vars.length)))
	પાછા ખોટું;
      hb_hashmap_t<અચલ hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      માટે (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        જો (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          જો (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            પાછા ખોટું;
          ચાલુ;
        }

        unsigned *idx;
        જો (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        નહીંતર
        {
	  auto *new_var = new_vars.push ();
	  જો (unlikely (new_vars.in_error ()))
	    પાછા ખોટું;
          hb_swap (*new_var, var);
          જો (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            પાછા ખોટું;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      પાછા સાચું;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    તાર્કિક compile_all_point_sets ()
    {
      માટે (અચલ auto& tuple: tuple_vars)
      {
        અચલ hb_vector_t<તાર્કિક>* points_set = &(tuple.indices);
        જો (point_data_map.has (points_set))
        {
          unsigned *ગણતરી;
          જો (unlikely (!point_set_ગણતરી_map.has (points_set, &ગણતરી) ||
                        !point_set_ગણતરી_map.set (points_set, (*ગણતરી) + 1)))
            પાછા ખોટું;
          ચાલુ;
        }

        hb_vector_t<unsigned અક્ષર> compiled_point_data;
        જો (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          પાછા ખોટું;

        જો (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_ગણતરી_map.set (points_set, 1))
          પાછા ખોટું;
      }
      પાછા સાચું;
    }

    /* find shared points set which saves most bytes */
    શૂન્ય find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      માટે (અચલ auto& _ : point_data_map.iter_ref ())
      {
        અચલ hb_vector_t<તાર્કિક>* points_set = _.first;
        unsigned data_length = _.second.length;
        જો (!data_length) ચાલુ;
        unsigned *ગણતરી;
        જો (unlikely (!point_set_ગણતરી_map.has (points_set, &ગણતરી) ||
                      *ગણતરી <= 1))
        {
          shared_points_bytes = nullptr;
          પાછા;
        }

        unsigned saved_bytes = data_length * ((*ગણતરી) -1);
        જો (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    તાર્કિક calc_inferred_deltas (અચલ contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      માટે (tuple_delta_t& var : tuple_vars)
        જો (!var.calc_inferred_deltas (contour_points, scratch))
          પાછા ખોટું;

      પાછા સાચું;
    }

    તાર્કિક iup_optimize (અચલ contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      માટે (tuple_delta_t& var : tuple_vars)
      {
        જો (!var.optimize (contour_points, is_composite, scratch))
          પાછા ખોટું;
      }
      પાછા સાચું;
    }

    જાહેર:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    શૂન્ય cull_unreachable (અચલ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      માટે (unsigned i = 0; i < tuple_vars.length; i++)
      {
	જો (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  ચાલુ;
	જો (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    તાર્કિક instantiate (અચલ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      અચલ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      તાર્કિક optimize = ખોટું)
    {
      જો (!tuple_vars) પાછા સાચું;
      જો (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        પાછા ખોટું;
      /* compute inferred deltas only for gvar */
      જો (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	જો (!calc_inferred_deltas (*contour_points, scratch))
	  પાછા ખોટું;
      }

      /* if iup delta opt is on, contour_points can't be null */
      જો (optimize && !contour_points)
        પાછા ખોટું;

      જો (!merge_tuple_variations (optimize ? contour_points : nullptr))
        પાછા ખોટું;

      જો (optimize && !iup_optimize (*contour_points, scratch)) પાછા ખોટું;
      પાછા !tuple_vars.in_error ();
    }

    તાર્કિક compile_bytes (અચલ hb_map_t& axes_index_map,
                        અચલ hb_map_t& axes_old_index_tag_map,
                        તાર્કિક use_shared_points,
                        તાર્કિક is_gvar = ખોટું,
                        અચલ hb_hashmap_t<અચલ hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      જો (!tuple_vars)
        પાછા સાચું;

      // compile points set and store data in hashmap
      જો (!compile_all_point_sets ())
        પાછા ખોટું;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      જો (use_shared_points)
      {
        find_shared_points ();
        જો (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<પૂર્ણાંક> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      માટે (auto& tuple: tuple_vars)
      {
        અચલ hb_vector_t<તાર્કિક>* points_set = &(tuple.indices);
        hb_vector_t<unsigned અક્ષર> *points_data;
        જો (unlikely (!point_data_map.has (points_set, &points_data)))
          પાછા ખોટું;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        જો (!points_data->length)
          ચાલુ;
        જો (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          પાછા ખોટું;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        જો (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          પાછા ખોટું;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      જો (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = સાચું;
        compiled_byte_size += 1;
      }

      પાછા સાચું;
    }

    તાર્કિક serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) અચલ
    {
      TRACE_SERIALIZE (આ);
      માટે (અચલ auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        જો (c->in_error ()) return_trace (ખોટું);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (સાચું);
    }

    તાર્કિક serialize_var_data (hb_serialize_context_t *c, તાર્કિક is_gvar) અચલ
    {
      TRACE_SERIALIZE (આ);
      જો (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      માટે (અચલ auto& tuple: tuple_vars)
      {
        અચલ hb_vector_t<તાર્કિક>* points_set = &(tuple.indices);
        hb_vector_t<unsigned અક્ષર> *point_data;
        જો (!point_data_map.has (points_set, &point_data))
          return_trace (ખોટું);

        જો (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        જો (c->in_error ()) return_trace (ખોટું);
      }

      /* padding for gvar */
      જો (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        જો (!c->embed (pad)) return_trace (ખોટું);
      }
      return_trace (સાચું);
    }
  };

  સંરચના tuple_iterator_t
  {
    unsigned get_axis_ગણતરી () અચલ { પાછા axis_ગણતરી; }

    શૂન્ય init (hb_bytes_t var_data_bytes_, unsigned પૂર્ણાંક axis_ગણતરી_3, અચલ શૂન્ય *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_ડાબું = var_data->tuple_Var_ગણતરી.get_ગણતરી ();
      axis_ગણતરી = axis_ગણતરી_3;
      axis_ગણતરી_times_2 = axis_ગણતરી_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    તાર્કિક get_shared_indices (hb_vector_t<unsigned પૂર્ણાંક> &shared_indices /* OUT */)
    {
      જો (var_data->has_shared_point_numbers ())
      {
        અચલ HBUINT8 *base = &(table_base+var_data->data);
        અચલ HBUINT8 *p = base;
        જો (!decompile_points (p, shared_indices, (અચલ HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) પાછા ખોટું;
        data_offset = p - base;
      }
      પાછા સાચું;
    }

    તાર્કિક is_valid ()
    {
      જો (unlikely (tuples_ડાબું <= 0))
	પાછા ખોટું;

      current_tuple_size = TupleVariationHeader::min_size;
      જો (unlikely (!var_data_bytes.check_end ((અચલ અક્ષર *) current_tuple + current_tuple_size)))
	પાછા ખોટું;

      current_tuple_size = current_tuple->get_size (axis_ગણતરી_times_2);
      જો (unlikely (!var_data_bytes.check_end ((અચલ અક્ષર *) current_tuple + current_tuple_size)))
	પાછા ખોટું;

      પાછા સાચું;
    }

    HB_ALWAYS_INLINE
    તાર્કિક move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_ડાબું--;
      પાછા is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    અચલ HBUINT8 *get_serialized_data () અચલ
    { પાછા &(table_base+var_data->data) + data_offset; }

    ખાનગી:
    signed tuples_ડાબું;
    અચલ TupleVariationData *var_data;
    unsigned પૂર્ણાંક axis_ગણતરી;
    unsigned પૂર્ણાંક axis_ગણતરી_times_2;
    unsigned પૂર્ણાંક data_offset;
    unsigned પૂર્ણાંક current_tuple_size;
    અચલ શૂન્ય *table_base;

    જાહેર:
    hb_bytes_t var_data_bytes;
    અચલ TupleVariationHeader *current_tuple;
  };

  સ્થિર તાર્કિક get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_ગણતરી,
                                  અચલ શૂન્ય *table_base,
                                  hb_vector_t<unsigned પૂર્ણાંક> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_ગણતરી, table_base);
    જો (!iterator->get_shared_indices (shared_indices))
      પાછા ખોટું;
    પાછા iterator->is_valid ();
  }

  તાર્કિક has_shared_point_numbers () અચલ { પાછા tuple_Var_ગણતરી.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  તાર્કિક cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_ગણતરી,
			      hb_array_t<અચલ F2DOT14> shared_tuples,
			      અચલ hb_map_t *axes_old_index_tag_map,
			      અચલ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<અક્ષર> &out /* OUT */,
			      તાર્કિક *changed /* OUT */) અચલ
  {
    *changed = ખોટું;

    hb_vector_t<unsigned પૂર્ણાંક> shared_indices;
    tuple_iterator_t iterator;
    જો (!get_tuple_iterator (var_data_bytes, axis_ગણતરી, આ,
			     shared_indices, &iterator))
      પાછા સાચું; /* no tuples or malformed: keep original */

    અચલ અક્ષર *bytes_start = var_data_bytes.arrayZ;
    અચલ અક્ષર *bytes_end = bytes_start + var_data_bytes.length;
    અચલ અક્ષર *serialized_base = (અચલ અક્ષર *) &(આ+data);
    અચલ અક્ષર *shared_points_end = (અચલ અક્ષર *) iterator.get_serialized_data ();
    જો (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      પાછા સાચું;

    સંરચના kept_tuple_t
    {
      અચલ અક્ષર *header;
      unsigned header_size;
      અચલ અક્ષર *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    કરો
    {
      અચલ TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_ગણતરી * 2);
      unsigned data_size = header->get_data_size ();
      અચલ અક્ષર *tuple_data = (અચલ અક્ષર *) iterator.get_serialized_data ();
      જો (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	પાછા સાચું; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      જો (!header->unpack_axis_tuples (axis_ગણતરી, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	પાછા સાચું;

      total++;
      જો (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(અચલ અક્ષર *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } જ્યારે (iterator.move_to_next ());

    જો (unlikely (kept.in_error ())) પાછા ખોટું;
    જો (kept.length == total) પાછા સાચું; /* nothing to cull */

    *changed = સાચું;
    જો (!kept.length) પાછા સાચું; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    જો (unlikely (!out.resize (new_size)))
      પાછા ખોટું;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_ગણતરી = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    અક્ષર *p = out.arrayZ + min_size;
    માટે (અચલ auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    માટે (અચલ auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    પાછા સાચું;
  }

  સ્થિર તાર્કિક decompile_points (અચલ HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned પૂર્ણાંક> &points /* OUT */,
				અચલ HBUINT8 *end)
  {
    ગણના packed_point_flag_t
    {
      POINTS_ARE_શબ્દો     = 0x80,
      POINT_RUN_ગણતરી_MASK = 0x7F
    };

    જો (unlikely (p + 1 > end)) પાછા ખોટું;

    unsigned ગણતરી = *p++;
    જો (ગણતરી & POINTS_ARE_શબ્દો)
    {
      જો (unlikely (p + 1 > end)) પાછા ખોટું;
      ગણતરી = ((ગણતરી & POINT_RUN_ગણતરી_MASK) << 8) | *p++;
    }
    જો (unlikely (!points.resize_dirty  (ગણતરી))) પાછા ખોટું;

    unsigned n = 0;
    unsigned i = 0;
    જ્યારે (i < ગણતરી)
    {
      જો (unlikely (p + 1 > end)) પાછા ખોટું;
      unsigned control = *p++;
      unsigned run_ગણતરી = (control & POINT_RUN_ગણતરી_MASK) + 1;
      unsigned stop = i + run_ગણતરી;
      જો (unlikely (stop > ગણતરી)) પાછા ખોટું;
      જો (control & POINTS_ARE_શબ્દો)
      {
        જો (unlikely (p + run_ગણતરી * HBUINT16::static_size > end)) પાછા ખોટું;
        માટે (; i < stop; i++)
        {
          n += *(અચલ HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      નહીંતર
      {
        જો (unlikely (p + run_ગણતરી > end)) પાછા ખોટું;
        માટે (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    પાછા સાચું;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  સ્થિર તાર્કિક decompile_deltas (અચલ HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				અચલ HBUINT8 *end,
				તાર્કિક consume_all = ખોટું,
				unsigned start = 0)
  {
    પાછા Tuple_કિંમતો::decompile (p, deltas, end, consume_all, start);
  }

  તાર્કિક has_data () અચલ { પાછા tuple_Var_ગણતરી; }

  તાર્કિક decompile_tuple_variations (unsigned point_ગણતરી,
                                   તાર્કિક is_gvar,
                                   tuple_iterator_t iterator,
                                   અચલ hb_map_t *axes_old_index_tag_map,
                                   અચલ hb_vector_t<unsigned> &shared_indices,
                                   અચલ hb_array_t<અચલ F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   તાર્કિક is_composite_glyph = ખોટું) અચલ
  {
    પાછા tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_ગણતરી,
                                                        point_ગણતરી, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
                  તાર્કિક is_gvar,
                  અચલ tuple_variations_t& tuple_variations) અચલ
  {
    TRACE_SERIALIZE (આ);
    /* empty tuple variations, just return and skip serialization. */
    જો (!tuple_variations) return_trace (સાચું);

    auto *out = c->start_embed (આ);
    જો (unlikely (!c->extend_min (out))) return_trace (ખોટું);

    જો (!c->check_assign (out->tuple_Var_ગણતરી, tuple_variations.get_var_ગણતરી (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ખોટું);

    unsigned total_header_len = 0;

    જો (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (ખોટું);

    unsigned data_offset = min_size + total_header_len;
    જો (!is_gvar) data_offset += 4;
    જો (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ખોટું);

    પાછા tuple_variations.serialize_var_data (c, is_gvar);
  }

  સુરક્ષિત:
  સંરચના Tuple_Var_ગણતરી : HBUINT16
  {
    friend સંરચના tuple_variations_t;
    તાર્કિક has_shared_point_numbers () અચલ { પાછા ((*આ) & SharedPointNumbers); }
    unsigned પૂર્ણાંક get_ગણતરી () અચલ { પાછા (*આ) & ગણતરી_Mask; }
    Tuple_Var_ગણતરી& operator = (uint16_t i) { HBUINT16::operator= (i); પાછા *આ; }
    explicit operator તાર્કિક () અચલ { પાછા get_ગણતરી (); }

    સુરક્ષિત:
    ગણના Flags
    {
      SharedPointNumbers= 0x8000u,
      ગણતરી_Mask         = 0x0FFFu
    };
    જાહેર:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_ગણતરી tuple_Var_ગણતરી;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  જાહેર:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
સંરચના item_variations_t
{
  using region_t = અચલ hb_hashmap_t<hb_tag_t, Triple>*;
  ખાનગી:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_data_અંક_rows;

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
  hb_vector_t<hb_vector_t<પૂર્ણાંક>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  તાર્કિક has_long = ખોટું;

  જાહેર:
  તાર્કિક has_long_word () અચલ
  { પાછા has_long; }

  અચલ hb_vector_t<region_t>& get_region_list () અચલ
  { પાછા region_list; }

  અચલ hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () અચલ
  { પાછા encodings; }

  અચલ hb_map_t& get_varidx_map () અચલ
  { પાછા varidx_map; }

  તાર્કિક add_vardata_encoding_for_testing (hb_vector_t<અચલ hb_vector_t<પૂર્ણાંક>*> &&rows,
                                         unsigned અંક_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), અંક_cols));
    પાછા !encodings.in_error ();
  }

  તાર્કિક compile_varidx_map_for_testing (અચલ hb_hashmap_t<unsigned, અચલ hb_vector_t<પૂર્ણાંક>*>& front_mapping)
  { પાછા compile_varidx_map (front_mapping); }

  તાર્કિક instantiate (અચલ ItemVariationStore& varStore,
                    અચલ hb_subset_plan_t *plan,
                    તાર્કિક optimize=સાચું,
                    તાર્કિક use_no_variation_idx=સાચું,
                    અચલ hb_array_t <અચલ hb_inc_bimap_t> inner_maps = hb_array_t<અચલ hb_inc_bimap_t> (),
                    તાર્કિક layout_conditions = ખોટું)
  {
    જો (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      પાછા ખોટું;
    /* avar2 partial instancing: cull unreachable regions. */
    જો (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      માટે (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    જો (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      પાછા ખોટું;
    જો (layout_conditions &&
        (!instantiate_condition_કિંમતો (plan, varStore) || !build_region_list ()))
      પાછા ખોટું;
    પાછા as_item_varstore (optimize, use_no_variation_idx);
  }

  તાર્કિક instantiate_condition_કિંમતો (અચલ hb_subset_plan_t *plan,
                                      અચલ ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_કિંમત_map;
    condition_map.clear ();
    જો (!plan->layout_condition_કિંમતો) પાછા સાચું;
    using condition_કિંમત_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, પૂર્ણાંક>>;
    hb_vector_t<condition_કિંમત_t> conditions;
    માટે (auto condition : plan->layout_condition_કિંમતો.iter ())
      conditions.push (condition);
    જો (unlikely (conditions.in_error ())) પાછા ખોટું;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (અચલ condition_કિંમત_t &a, અચલ condition_કિંમત_t &b) {
      જો (a.second.first != b.second.first)
        પાછા a.second.first < b.second.first ? -1 : 1;
      પાછા a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    માટે (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      જો (plan->gdef_varstore_inner_maps[i].get_population ())
        જો (!outer_map.set (i, new_outer++)) પાછા ખોટું;
    માટે (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      દ્વિગુણ value = condition.second.second +
                     varStore.get_delta<દ્વિગુણ> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      જો (!outer_map.has (old_outer))
        ચાલુ;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      જો (outer >= vars.length || inner >= var_data_અંક_rows[outer])
        ચાલુ;

      દ્વિગુણ maximum = fabs ((દ્વિગુણ) value);
      તાર્કિક variable = ખોટું;
      માટે (અચલ tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        જો (unlikely (inner >= tuple.deltas_x.length)) પાછા ખોટું;
        દશાંશ delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((દ્વિગુણ) delta));
      }
      જો (!variable)
      {
        જો (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), પૂર્ણાંક (value > 0))))
          પાછા ખોટું;
        ચાલુ;
      }

      દ્વિગુણ scale = 1.;
      જ્યારે (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      જ્યારે (maximum * scale * 2 <= INT_MAX)
      {
        તાર્કિક integral = (દ્વિગુણ) value * scale == round ((દ્વિગુણ) value * scale);
        માટે (અચલ tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (દ્વિગુણ) tuple.deltas_x[inner] * scale ==
                      round ((દ્વિગુણ) tuple.deltas_x[inner] * scale);
        જો (integral) તોડો;
        scale *= 2;
      }
      પૂર્ણાંક default_કિંમત = (પૂર્ણાંક) round ((દ્વિગુણ) value * scale);
      જો (scale != 1. || default_કિંમત < -32768 || default_કિંમત > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        માટે (અચલ tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          જો (!tuple.deltas_x[inner]) ચાલુ;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (સાચું);
          copy.deltas_x.push ((દશાંશ) round ((દ્વિગુણ) tuple.deltas_x[inner] * scale));
          જો (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) પાછા ખોટું;
          private_row.tuple_vars.push (std::move (copy));
        }
        જો (unlikely (vars.length >= 0xFFFFu)) પાછા ખોટું;
        જો (unlikely (private_row.tuple_vars.in_error ())) પાછા ખોટું;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_અંક_rows.push (1);
        જો (unlikely (vars.in_error () || var_data_અંક_rows.in_error ())) પાછા ખોટું;
        જો (default_કિંમત < -32768 || default_કિંમત > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          પૂર્ણાંક bias = default_કિંમત - hb_clamp (default_કિંમત, -32768, 32767);
          default_કિંમત -= bias;
          પૂર્ણાંક ઉપલું = bias / 65536 * 65536;
          પૂર્ણાંક parts[] = {ઉપલું, bias - ઉપલું};
          માટે (પૂર્ણાંક part : parts)
          {
            જો (!part) ચાલુ;
            unsigned ગણતરી = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            અચલ auto &tuples = vars[idx >> 16].tuple_vars;
            જો (unlikely (tuples.in_error () || tuples.length != ગણતરી + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) પાછા ખોટું;
          }
        }
      }
      જો (!condition_map.set (condition.first, hb_pair (idx, default_કિંમત)))
        પાછા ખોટું;
    }
    પાછા !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  તાર્કિક create_from_item_varstore (અચલ ItemVariationStore& varStore,
                                  અચલ hb_map_t& axes_old_index_tag_map,
                                  અચલ hb_array_t <અચલ hb_inc_bimap_t> inner_maps = hb_array_t<અચલ hb_inc_bimap_t> ())
  {
    અચલ VarRegionList& regionList = varStore.get_region_list ();
    જો (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      પાછા ખોટું;

    unsigned અંક_var_data = varStore.get_sub_table_ગણતરી ();
    જો (inner_maps && inner_maps.length != અંક_var_data) પાછા ખોટું;
    જો (!vars.alloc (અંક_var_data) ||
        !var_data_અંક_rows.alloc (અંક_var_data)) પાછા ખોટું;

    માટે (unsigned i = 0; i < અંક_var_data; i++)
    {
      જો (inner_maps && !inner_maps.arrayZ[i].get_population ())
          ચાલુ;
      tuple_variations_t var_data_tuples;
      unsigned item_ગણતરી_2 = 0;
      જો (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_ગણતરી_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        પાછા ખોટું;

      var_data_અંક_rows.push (item_ગણતરી_2);
      vars.push (std::move (var_data_tuples));
    }
    પાછા !vars.in_error () && !var_data_અંક_rows.in_error () && vars.length == var_data_અંક_rows.length;
  }

  તાર્કિક instantiate_tuple_vars (અચલ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               અચલ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               તાર્કિક build_regions = સાચું)
  {
    optimize_scratch_t scratch;
    માટે (tuple_variations_t& tuple_vars : vars)
      જો (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        પાછા ખોટું;

    પાછા !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_ગણતરી_2)
  {
    vars.push (tuple_variations_t ());
    var_data_અંક_rows.push (item_ગણતરી_2);
    પાછા vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_ગણતરી (unsigned outer) અચલ
  { પાછા outer < var_data_અંક_rows.length ? var_data_અંક_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    જો (unlikely (outer >= vars.length ||
                  inner >= var_data_અંક_rows[outer] ||
                  var_data_અંક_rows[outer] >= 0xFFFFu))
      પાછા (unsigned) -1;
    માટે (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      જો (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        પાછા (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      જો (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      જો (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        પાછા (unsigned) -1;
    }
    પાછા var_data_અંક_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  શૂન્ય add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, પૂર્ણાંક delta, unsigned item_ગણતરી_2)
  {
    જો (outer >= vars.length) પાછા;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    જો (!tuple.indices.resize (item_ગણતરી_2) ||
        !tuple.deltas_x.resize (item_ગણતરી_2)) પાછા;
    માટે (unsigned i = 0; i < item_ગણતરી_2; i++)
    {
      tuple.indices.arrayZ[i] = સાચું;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (દશાંશ) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  તાર્કિક build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    માટે (અચલ tuple_variations_t& sub_table : vars)
    {
      માટે (અચલ tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        જો (!used_regions.has (r))
        {
          તાર્કિક all_zeros = સાચું;
          માટે (દશાંશ d : tuple.deltas_x)
          {
            પૂર્ણાંક delta = hb_clamp_to<પૂર્ણાંક> (roundf ((દ્વિગુણ) d));
            જો (delta != 0)
            {
              all_zeros = ખોટું;
              તોડો;
            }
          }
          જો (!all_zeros)
          {
            જો (!used_regions.set (r, 1))
              પાછા ખોટું;
          }
        }
        જો (all_regions.has (r))
          ચાલુ;
        જો (!all_regions.set (r, 1))
          પાછા ખોટું;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    જો (!all_regions || !all_unique_regions) પાછા સાચું;

    જો (!region_list.alloc (all_regions.get_population ()))
      પાછા ખોટું;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    માટે (અચલ auto& r : orig_region_list)
    {
      જો (!all_regions.has (&r) || !used_regions.has (&r))
        ચાલુ;

      region_list.push (&r);
      જો (!region_map.set (&r, idx))
        પાછા ખોટું;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    માટે (અચલ auto& r: all_unique_regions)
    {
      જો (!all_regions.has (r) || !used_regions.has (r))
        ચાલુ;
      region_list.push (r);
      જો (!region_map.set (r, idx))
        પાછા ખોટું;
      all_regions.del (r);
      idx++;
    }
    પાછા (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  સંરચના combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = મૂળભૂત;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      પ્રતિજ્ઞા (gain < 0xFFFFFF);
      પ્રતિજ્ઞા (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    તાર્કિક operator < (અચલ combined_gain_idx_tuple_t& o)
    {
      પાછા encoded < o.encoded;
    }

    તાર્કિક operator <= (અચલ combined_gain_idx_tuple_t& o)
    {
      પાછા encoded <= o.encoded;
    }

    unsigned idx_1 () અચલ { પાછા (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () અચલ { પાછા encoded & 0xFFFFF; };
  };

  તાર્કિક as_item_varstore (તાર્કિક optimize=સાચું, તાર્કિક use_no_variation_idx=સાચું)
  {
    /* return true if no variation data */
    જો (!region_list) પાછા સાચું;
    unsigned અંક_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    માટે (unsigned major = 0; major < var_data_અંક_rows.length; major++)
      total_rows += var_data_અંક_rows[major];

    જો (!delta_rows.resize (total_rows)) પાછા ખોટું;
    /* init all rows to [0]*num_cols */
    માટે (unsigned i = 0; i < total_rows; i++)
      જો (!(delta_rows[i].resize (અંક_cols))) પાછા ખોટું;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, અચલ hb_vector_t<પૂર્ણાંક>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<અચલ hb_vector_t<પૂર્ણાંક> *> major_rows;
    hb_hashmap_t<અચલ hb_vector_t<પૂર્ણાંક>*, unsigned> delta_rows_map;
    માટે (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      અચલ tuple_variations_t& tuples = vars[major];
      unsigned અંક_rows = var_data_અંક_rows[major];

      જો (!અંક_rows) ચાલુ;

      માટે (અચલ tuple_delta_t& tuple: tuples.tuple_vars)
      {
        જો (tuple.deltas_x.length != અંક_rows)
          પાછા ખોટું;

        /* skip unused regions */
        unsigned *col_idx;
        જો (!region_map.has (&(tuple.axis_tuples), &col_idx))
          ચાલુ;

        માટે (unsigned i = 0; i < અંક_rows; i++)
        {
          પૂર્ણાંક rounded_delta = hb_clamp_to<પૂર્ણાંક> (roundf ((દ્વિગુણ) tuple.deltas_x[i]));
          પૂર્ણાંક &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          જો (unlikely (sum < INT_MIN || sum > INT_MAX)) પાછા ખોટું;
          delta = (પૂર્ણાંક) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      માટે (unsigned minor = 0; minor < અંક_rows; minor++)
      {
	અચલ hb_vector_t<પૂર્ણાંક>& row = delta_rows[start_row + minor];
	જો (use_no_variation_idx)
	{
	  તાર્કિક all_zeros = સાચું;
	  માટે (પૂર્ણાંક delta : row)
	  {
	    જો (delta != 0)
	    {
	      all_zeros = ખોટું;
	      તોડો;
	    }
	  }
	  જો (all_zeros)
	    ચાલુ;
	}

	જો (!front_mapping.set ((major<<16) + minor, &row))
	  પાછા ખોટું;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	જો (optimize)
	{
	  જો (delta_rows_map.has (&row))
	    ચાલુ;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      જો (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        પાછા ખોટું;
      જો (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), અંક_cols);
	જો (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  પાછા ખોટું;
      }

      start_row += અંક_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    જો (!optimize)
    {
      encodings = std::move (encoding_objs);
      પાછા !encodings.in_error ();
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
    hb_vector_t<item_t> queue_વસ્તુઓ;
    unsigned અંક_todos = encoding_objs.length;
    માટે (unsigned i = 0; i < અંક_todos; i++)
    {
      માટે (unsigned j = i + 1; j < અંક_todos; j++)
      {
        પૂર્ણાંક combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        જો (combining_gain > 0)
          queue_વસ્તુઓ.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_વસ્તુઓ));

    hb_bit_set_t removed_todo_idxes;
    જ્યારે (queue)
    {
      auto t = queue.કાઢો_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      જો (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        ચાલુ;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      જો (unlikely (removed_todo_idxes.in_error ())) પાછા ખોટું;

      encoding.merge (other_encoding);
      જો (unlikely (encoding.in_error ())) પાછા ખોટું;

      માટે (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        જો (removed_todo_idxes.has (idx)) ચાલુ;

        અચલ delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        જો (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          માટે (અચલ auto& row : obj.વસ્તુઓ)
            encoding.add_row (row);
          જો (unlikely (encoding.in_error ())) પાછા ખોટું;

          removed_todo_idxes.add (idx);
          જો (unlikely (removed_todo_idxes.in_error ())) પાછા ખોટું;
          ચાલુ;
        }

        પૂર્ણાંક combined_gain = encoding.gain_from_merging (obj);
        જો (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      જો (unlikely (encoding_objs.in_error ())) પાછા ખોટું;
    }

    પૂર્ણાંક અંક_final_encodings = (પૂર્ણાંક) encoding_objs.length - (પૂર્ણાંક) removed_todo_idxes.get_population ();
    જો (અંક_final_encodings <= 0) પાછા ખોટું;

    જો (!encodings.alloc (અંક_final_encodings)) પાછા ખોટું;
    માટે (unsigned i = 0; i < encoding_objs.length; i++)
    {
      જો (removed_todo_idxes.has (i)) ચાલુ;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    પાછા compile_varidx_map (front_mapping);
  }

  ખાનગી:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  તાર્કિક compile_varidx_map (અચલ hb_hashmap_t<unsigned, અચલ hb_vector_t<પૂર્ણાંક>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<અચલ hb_vector_t<પૂર્ણાંક>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    માટે (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      જો (encoding.is_empty () || encoding.in_error ())
        પાછા ખોટું;

      unsigned અંક_rows = encoding.વસ્તુઓ.length;
      unsigned અંક_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.વસ્તુઓ.qsort (_cmp_row);

      માટે (unsigned start = 0; start < અંક_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (અંક_rows - start, 0xFFFFu);
        hb_vector_t<અચલ hb_vector_t<પૂર્ણાંક>*> rows;

        જો (!rows.alloc (chunk_len))
          પાછા ખોટું;

        unsigned major = split_encodings.length;
        માટે (unsigned minor = 0; minor < chunk_len; minor++)
        {
          અચલ hb_vector_t<પૂર્ણાંક> *row = encoding.વસ્તુઓ.arrayZ[start + minor];
          rows.push (row);
          જો (!back_mapping.set (row, (major << 16) + minor))
            પાછા ખોટું;
        }

        auto *split = split_encodings.push (std::move (rows), અંક_cols);
        જો (unlikely (split_encodings.in_error () || split->in_error ()))
          પાછા ખોટું;
      }
    }

    encodings = std::move (split_encodings);
    જો (encodings.in_error () || back_mapping.in_error ())
      પાછા ખોટું;

    માટે (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      જો (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      નહીંતર
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    પાછા !varidx_map.in_error ();
  }

  સ્થિર પૂર્ણાંક _cmp_row (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    અચલ hb_vector_t<પૂર્ણાંક>** a = (અચલ hb_vector_t<પૂર્ણાંક>**) pa;
    અચલ hb_vector_t<પૂર્ણાંક>** b = (અચલ hb_vector_t<પૂર્ણાંક>**) pb;

    માટે (unsigned i = 0; i < (*b)->length; i++)
    {
      પૂર્ણાંક va = (*a)->arrayZ[i];
      પૂર્ણાંક vb = (*b)->arrayZ[i];
      જો (va != vb)
        પાછા va < vb ? -1 : 1;
    }
    પાછા 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

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

#അന്തര്ഭാവ "hb-ot-layout-common.hh"
#അന്തര്ഭാവ "hb-alloc-pool.hh"
#അന്തര്ഭാവ "hb-priority-queue.hh"
#അന്തര്ഭാവ "hb-subset-instancer-iup.hh"


നാമാകാശ OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
ഘടന TupleVariationHeader
{
  friend ഘടന tuple_delta_t;
  size_t get_size (unsigned axis_എണ്ണം_times_2) സ്ഥിരം
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    എങ്കിൽ (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned എണ്ണം = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      തിരികെ hb_unsigned_mul_add_saturate (എണ്ണം, axis_എണ്ണം_times_2, min_size);
    }
    തിരികെ min_size;
  }

  unsigned get_data_size () സ്ഥിരം { തിരികെ varDataSize; }

  സ്ഥിരം TupleVariationHeader &get_next (unsigned axis_എണ്ണം_times_2) സ്ഥിരം
  { തിരികെ StructAtOffset<TupleVariationHeader> (ഇത്, get_size (axis_എണ്ണം_times_2)); }

  തർക്കം unpack_axis_tuples (unsigned axis_എണ്ണം,
                           സ്ഥിരം hb_array_t<സ്ഥിരം F2DOT14> shared_tuples,
                           സ്ഥിരം hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) സ്ഥിരം
  {
    സ്ഥിരം F2DOT14 *peak_tuple = nullptr;
    എങ്കിൽ (has_peak ())
      peak_tuple = get_peak_tuple (axis_എണ്ണം);
    അല്ലെങ്കിൽ
    {
      unsigned പൂർണ്ണസംഖ്യ index = get_index ();
      എങ്കിൽ (unlikely ((index + 1) * axis_എണ്ണം > shared_tuples.length))
        തിരികെ അസത്യം;
      peak_tuple = shared_tuples.sub_array (axis_എണ്ണം * index, axis_എണ്ണം).arrayZ;
    }

    സ്ഥിരം F2DOT14 *start_tuple = nullptr;
    സ്ഥിരം F2DOT14 *end_tuple = nullptr;
    തർക്കം has_interm = has_intermediate ();

    എങ്കിൽ (has_interm)
    {
      start_tuple = get_start_tuple (axis_എണ്ണം);
      end_tuple = get_end_tuple (axis_എണ്ണം);
    }

    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം; i++)
    {
      ദശാംശം peak = peak_tuple[i].to_float ();
      എങ്കിൽ (peak == 0.f) തുടരുക;

      hb_tag_t *axis_tag;
      എങ്കിൽ (!axes_old_index_tag_map->has (i, &axis_tag))
        തിരികെ അസത്യം;

      ദശാംശം start, end;
      എങ്കിൽ (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      അല്ലെങ്കിൽ
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    തിരികെ സത്യം;
  }

  HB_ALWAYS_INLINE
  ദശാംശം calculate_scalar (hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
			   സ്ഥിരം hb_array_t<സ്ഥിരം F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) സ്ഥിരം
  {
    unsigned tuple_index = tupleIndex;
    unsigned പൂർണ്ണസംഖ്യ index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    തർക്കം plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    എങ്കിൽ (likely (plain_shared_tuple))
    {
      ദശാംശം scalar;
      എങ്കിൽ (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	തിരികെ scalar;
    }

    സ്ഥിരം F2DOT14 *peak_tuple;

    തർക്കം has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    എങ്കിൽ (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_എണ്ണം);
      shared_tuple_scalar_cache = nullptr;
    }
    അല്ലെങ്കിൽ
    {
      ദശാംശം scalar;
      എങ്കിൽ (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        എങ്കിൽ (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	അല്ലെങ്കിൽ
	  തിരികെ scalar;
      }

      എങ്കിൽ (unlikely ((index + 1) * coord_എണ്ണം > shared_tuples.length))
        തിരികെ 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_എണ്ണം * index);

    }

    സ്ഥിരം F2DOT14 *start_tuple = nullptr;
    സ്ഥിരം F2DOT14 *end_tuple = nullptr;

    എങ്കിൽ (has_interm)
    {
      start_tuple = get_start_tuple (coord_എണ്ണം);
      end_tuple = get_end_tuple (coord_എണ്ണം);
    }

    ദശാംശം scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
    തർക്കം skip = coord_എണ്ണം >= 16;
#endif
#endif
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < coord_എണ്ണം; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#എങ്കിൽ HB_FAST_അക്കം_ACCESS
      എങ്കിൽ (skip)
      {
	ആകുമ്പോൾ (i + 4 <= coord_എണ്ണം && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	ആകുമ്പോൾ (i < coord_എണ്ണം && peak_tuple[i].to_int () == 0)
	  i += 1;
	എങ്കിൽ (i >= coord_എണ്ണം)
	  നിർത്തുക;
      }
#endif
#endif

      പൂർണ്ണസംഖ്യ peak = peak_tuple[i].to_int ();
      എങ്കിൽ (!peak) തുടരുക;

      പൂർണ്ണസംഖ്യ v = coords[i];
      എങ്കിൽ (!v) { scalar = 0.f; നിർത്തുക; }
      എങ്കിൽ (v == peak) തുടരുക;

      എങ്കിൽ (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        പൂർണ്ണസംഖ്യ start = start_tuple[i].to_int ();
        പൂർണ്ണസംഖ്യ end = end_tuple[i].to_int ();
        എങ്കിൽ (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) തുടരുക;
        എങ്കിൽ (v < start || v > end) { scalar = 0.f; നിർത്തുക; }
        എങ്കിൽ (v < peak)
        { എങ്കിൽ (peak != start) scalar *= (ദശാംശം) (v - start) / (peak - start); }
        അല്ലെങ്കിൽ
        { എങ്കിൽ (peak != end) scalar *= (ദശാംശം) (end - v) / (end - peak); }
      }
      അല്ലെങ്കിൽ എങ്കിൽ (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; നിർത്തുക; }
      അല്ലെങ്കിൽ
        scalar *= (ദശാംശം) v / peak;
    }
    എങ്കിൽ (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    തിരികെ scalar;
  }

  തർക്കം           has_peak () സ്ഥിരം { തിരികെ tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  തർക്കം   has_intermediate () സ്ഥിരം { തിരികെ tupleIndex & TupleIndex::IntermediateRegion; }
  തർക്കം has_private_points () സ്ഥിരം { തിരികെ tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () സ്ഥിരം { തിരികെ tupleIndex & TupleIndex::TupleIndexMask; }
  സ്ഥിതം constexpr unsigned max_shared_tuple_എണ്ണം = 0x1000u;

  സംരക്ഷിത:
  ഘടന TupleIndex : HBUINT16
  {
    എണ്ണൽ Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); തിരികെ *ഇത്; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<സ്ഥിരം F2DOT14> get_all_tuples (unsigned axis_എണ്ണം) സ്ഥിരം
  { തിരികെ StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_എണ്ണം); }
  സ്ഥിരം F2DOT14* get_all_tuples_base (unsigned axis_എണ്ണം) സ്ഥിരം
  { തിരികെ StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  സ്ഥിരം F2DOT14* get_peak_tuple (unsigned axis_എണ്ണം) സ്ഥിരം
  { തിരികെ get_all_tuples_base (axis_എണ്ണം); }
  സ്ഥിരം F2DOT14* get_start_tuple (unsigned axis_എണ്ണം) സ്ഥിരം
  { തിരികെ get_all_tuples_base (axis_എണ്ണം) + has_peak () * axis_എണ്ണം; }
  സ്ഥിരം F2DOT14* get_end_tuple (unsigned axis_എണ്ണം) സ്ഥിരം
  { തിരികെ get_all_tuples_base (axis_എണ്ണം) + has_peak () * axis_എണ്ണം + axis_എണ്ണം; }

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
  പൊതു:
  DEFINE_SIZE_MIN (4);
};

ഘടന optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<തർക്കം> opt_indices;
  hb_vector_t<പൂർണ്ണസംഖ്യ> rounded_x_deltas;
  hb_vector_t<പൂർണ്ണസംഖ്യ> rounded_y_deltas;
  hb_vector_t<ദശാംശം> opt_deltas_x;
  hb_vector_t<ദശാംശം> opt_deltas_y;
  hb_vector_t<unsigned അക്ഷരം> opt_point_data;
  hb_vector_t<unsigned അക്ഷരം> opt_deltas_data;
  hb_vector_t<unsigned അക്ഷരം> point_data;
  hb_vector_t<unsigned അക്ഷരം> deltas_data;
  hb_vector_t<പൂർണ്ണസംഖ്യ> rounded_deltas;
};

ഘടന tuple_delta_t
{
  സ്ഥിതം constexpr തർക്കം realloc_move = സത്യം;  // Watch out when adding new members!

  പൊതു:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<തർക്കം> indices;

  hb_vector_t<ദശാംശം> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<ദശാംശം> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned അക്ഷരം> compiled_tuple_header;
  hb_vector_t<unsigned അക്ഷരം> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (സ്ഥിരം tuple_delta_t& o) = സ്ഥിരസ്ഥിതി;
  tuple_delta_t& operator = (സ്ഥിരം tuple_delta_t& o) = സ്ഥിരസ്ഥിതി;

  friend ശൂന്യം swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*ഇത്, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*ഇത്, o);
    തിരികെ *ഇത്;
  }

  ശൂന്യം copy_from (സ്ഥിരം tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  ശൂന്യം remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  തർക്കം set_tent (hb_tag_t axis_tag, Triple tent)
  { തിരികെ axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (സ്ഥിരം tuple_delta_t& o)
  {
    unsigned അക്കം = indices.length;
    വേണ്ടി (unsigned i = 0; i < അക്കം; i++)
    {
      എങ്കിൽ (indices.arrayZ[i])
      {
        എങ്കിൽ (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          എങ്കിൽ (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      അല്ലെങ്കിൽ
      {
        എങ്കിൽ (!o.indices.arrayZ[i]) തുടരുക;
        indices.arrayZ[i] = സത്യം;
        deltas_x[i] = o.deltas_x[i];
        എങ്കിൽ (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    തിരികെ *ഇത്;
  }

  tuple_delta_t& operator *= (ദശാംശം scalar)
  {
    എങ്കിൽ (scalar == 1.0f)
      തിരികെ *ഇത്;

    unsigned അക്കം = indices.length;
    എങ്കിൽ (deltas_y)
      വേണ്ടി (unsigned i = 0; i < അക്കം; i++)
      {
	എങ്കിൽ (!indices.arrayZ[i]) തുടരുക;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    അല്ലെങ്കിൽ
      വേണ്ടി (unsigned i = 0; i < അക്കം; i++)
      {
	എങ്കിൽ (!indices.arrayZ[i]) തുടരുക;
	deltas_x[i] *= scalar;
      }
    തിരികെ *ഇത്;
  }

  ശൂന്യം change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    എങ്കിൽ (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*ഇത്));
      തിരികെ;
    }

    എങ്കിൽ ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->നടുവ് && tent->നടുവ് <= tent->maximum))
      തിരികെ;

    എങ്കിൽ (tent->നടുവ് == 0.f)
    {
      out.push (std::move (*ഇത്));
      തിരികെ;
    }

    എങ്കിൽ (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.നടുവ് &&
          axis_limit.നടുവ് <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      തിരികെ;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    വേണ്ടി (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      എങ്കിൽ (i < solutions.length - 1)
	new_var.copy_from (*ഇത്, pool);
      അല്ലെങ്കിൽ
	new_var = std::move (*ഇത്);

      എങ്കിൽ (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      അല്ലെങ്കിൽ
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  തർക്കം compile_coords (സ്ഥിരം hb_map_t& axes_index_map,
		       സ്ഥിരം hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_എണ്ണം = axes_index_map.get_population ();
    എങ്കിൽ (pool)
    {
      എങ്കിൽ (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_എണ്ണം)))
	തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!compiled_peak_coords.resize (cur_axis_എണ്ണം)))
      തിരികെ അസത്യം;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_എണ്ണം = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    വേണ്ടി (unsigned i = 0; i < orig_axis_എണ്ണം; i++)
    {
      എങ്കിൽ (!axes_index_map.has (i))
        തുടരുക;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      എങ്കിൽ (axis_tuples.has (axis_tag, &coords))
      {
	ദശാംശം min_val = coords->minimum;
	ദശാംശം val = coords->നടുവ്;
	ദശാംശം max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	എങ്കിൽ (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  എങ്കിൽ (!compiled_interm_coords)
	  {
	    എങ്കിൽ (pool)
	    {
	      എങ്കിൽ (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_എണ്ണം)))
		തിരികെ അസത്യം;
	    }
	    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!compiled_interm_coords.resize (2 * cur_axis_എണ്ണം)))
	      തിരികെ അസത്യം;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_എണ്ണം);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_എണ്ണം);

	    വേണ്ടി (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      എങ്കിൽ (!peak) തുടരുക;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	എങ്കിൽ (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    തിരികെ !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  തർക്കം compile_tuple_var_header (സ്ഥിരം hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 സ്ഥിരം hb_map_t& axes_old_index_tag_map,
                                 സ്ഥിരം hb_hashmap_t<സ്ഥിരം hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    എങ്കിൽ (!compiled_deltas) തിരികെ സത്യം;

    unsigned cur_axis_എണ്ണം = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_എണ്ണം * (F2DOT14::static_size) + 4;
    എങ്കിൽ (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, അസത്യം))) തിരികെ അസത്യം;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    എങ്കിൽ (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_എണ്ണം = 0;
    unsigned *shared_tuple_idx;
    എങ്കിൽ (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    അല്ലെങ്കിൽ
    {
      peak_എണ്ണം = encode_peak_coords(coords, flag);
      എങ്കിൽ (!peak_എണ്ണം) തിരികെ അസത്യം;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_എണ്ണം = encode_interm_coords (coords.sub_array (peak_എണ്ണം), flag);

    /* pointdata length = 0 implies "use shared points" */
    എങ്കിൽ (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_എണ്ണം + interim_എണ്ണം) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    തിരികെ സത്യം;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) സ്ഥിരം
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * വലുപ്പം (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    തിരികെ compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) സ്ഥിരം
  {
    എങ്കിൽ (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * വലുപ്പം (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    തിരികെ compiled_interm_coords.length;
  }

  തർക്കം compile_deltas (hb_vector_t<പൂർണ്ണസംഖ്യ> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { തിരികെ compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  സ്ഥിതം തർക്കം compile_deltas (hb_array_t<സ്ഥിരം തർക്കം> point_indices,
			      hb_array_t<സ്ഥിരം ദശാംശം> x_deltas,
			      hb_array_t<സ്ഥിരം ദശാംശം> y_deltas,
			      hb_vector_t<unsigned അക്ഷരം> &compiled_deltas, /* OUT */
			      hb_vector_t<പൂർണ്ണസംഖ്യ> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    എങ്കിൽ (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      തിരികെ അസത്യം;

    unsigned j = 0;
    വേണ്ടി (unsigned i = 0; i < point_indices.length; i++)
    {
      എങ്കിൽ (!point_indices[i]) തുടരുക;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<പൂർണ്ണസംഖ്യ> (roundf ((ഇരട്ട) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    എങ്കിൽ (!rounded_deltas) തിരികെ സത്യം;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    എങ്കിൽ (y_deltas)
      alloc_len *= 2;

    എങ്കിൽ (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, അസത്യം))) തിരികെ അസത്യം;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    എങ്കിൽ (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      വേണ്ടി (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        എങ്കിൽ (!point_indices[idx]) തുടരുക;
        പൂർണ്ണസംഖ്യ rounded_delta = hb_clamp_to<പൂർണ്ണസംഖ്യ> (roundf ((ഇരട്ട) y_deltas.arrayZ[idx]));

        എങ്കിൽ (j >= rounded_deltas.length) തിരികെ അസത്യം;

        rounded_deltas[j++] = rounded_delta;
      }

      എങ്കിൽ (j != rounded_deltas.length) തിരികെ അസത്യം;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    തിരികെ സത്യം;
  }

  സ്ഥിതം unsigned compile_deltas (hb_array_t<unsigned അക്ഷരം> encoded_bytes,
				  hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> deltas)
  {
    തിരികെ Tuple_മൂല്യങ്ങൾ::compile_unsafe (deltas, encoded_bytes);
  }

  തർക്കം calc_inferred_deltas (സ്ഥിരം contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_എണ്ണം = orig_points.length;
    എങ്കിൽ (point_എണ്ണം != indices.length)
      തിരികെ അസത്യം;

    unsigned ref_എണ്ണം_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    വേണ്ടി (unsigned i = 0; i < point_എണ്ണം; i++)
    {
      ref_എണ്ണം_2 += indices.arrayZ[i];
      എങ്കിൽ (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    എങ്കിൽ (ref_എണ്ണം_2 == point_എണ്ണം)
      തിരികെ സത്യം;
    എങ്കിൽ (unlikely (end_points.in_error ())) തിരികെ അസത്യം;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    വേണ്ടി (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_എണ്ണം = 0;
      വേണ്ടി (unsigned i = start_point; i < end_point + 1; i++)
        unref_എണ്ണം += indices.arrayZ[i];
      unref_എണ്ണം = (end_point - start_point + 1) - unref_എണ്ണം;

      unsigned j = start_point;
      എങ്കിൽ (unref_എണ്ണം == 0 || unref_എണ്ണം > end_point - start_point)
        പോകുക no_more_gaps;
      വേണ്ടി (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned പൂർണ്ണസംഖ്യ prev, next, i;
        വേണ്ടി (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          എങ്കിൽ (indices.arrayZ[i] && !indices.arrayZ[j]) നിർത്തുക;
        }
        prev = j = i;
        വേണ്ടി (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          എങ്കിൽ (!indices.arrayZ[i] && indices.arrayZ[j]) നിർത്തുക;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        വേണ്ടി (;;)
        {
          i = next_index (i, start_point, end_point);
          എങ്കിൽ (i == next) നിർത്തുക;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          എങ്കിൽ (--unref_എണ്ണം == 0) പോകുക no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    വേണ്ടി (unsigned i = 0; i < point_എണ്ണം; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      എങ്കിൽ ( !indices[i])
      {
        എങ്കിൽ (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = സത്യം;
      }
    }
    തിരികെ സത്യം;
  }

  തർക്കം optimize (സ്ഥിരം contour_point_vector_t& contour_points,
                 തർക്കം is_composite,
		 optimize_scratch_t &scratch,
                 ഇരട്ട tolerance = 0.5 + 1e-10)
  {
    unsigned എണ്ണം = contour_points.length;
    എങ്കിൽ (deltas_x.length != എണ്ണം ||
        deltas_y.length != എണ്ണം)
      തിരികെ അസത്യം;

    hb_vector_t<തർക്കം> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<പൂർണ്ണസംഖ്യ> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<പൂർണ്ണസംഖ്യ> &rounded_y_deltas = scratch.rounded_y_deltas;

    എങ്കിൽ (unlikely (!rounded_x_deltas.resize_dirty  (എണ്ണം) ||
                  !rounded_y_deltas.resize_dirty  (എണ്ണം)))
      തിരികെ അസത്യം;

    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<പൂർണ്ണസംഖ്യ> (roundf ((ഇരട്ട) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<പൂർണ്ണസംഖ്യ> (roundf ((ഇരട്ട) deltas_y.arrayZ[i]));
    }

    എങ്കിൽ (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      തിരികെ അസത്യം;

    unsigned ref_എണ്ണം_2 = 0;
    വേണ്ടി (തർക്കം ref_flag : opt_indices)
       ref_എണ്ണം_2 += ref_flag;

    എങ്കിൽ (ref_എണ്ണം_2 == എണ്ണം) തിരികെ സത്യം;

    hb_vector_t<ദശാംശം> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<ദശാംശം> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    തർക്കം is_comp_glyph_wo_deltas = (is_composite && ref_എണ്ണം_2 == 0);
    എങ്കിൽ (is_comp_glyph_wo_deltas)
    {
      എങ്കിൽ (unlikely (!opt_deltas_x.resize (എണ്ണം) ||
                    !opt_deltas_y.resize (എണ്ണം)))
        തിരികെ അസത്യം;

      opt_indices.arrayZ[0] = സത്യം;
      വേണ്ടി (unsigned i = 1; i < എണ്ണം; i++)
        opt_indices.arrayZ[i] = അസത്യം;
    }

    hb_vector_t<unsigned അക്ഷരം> &opt_point_data = scratch.opt_point_data.reset ();
    എങ്കിൽ (!compile_point_set (opt_indices, opt_point_data))
      തിരികെ അസത്യം;
    hb_vector_t<unsigned അക്ഷരം> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    എങ്കിൽ (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      തിരികെ അസത്യം;

    hb_vector_t<unsigned അക്ഷരം> &point_data = scratch.point_data.reset ();
    എങ്കിൽ (!compile_point_set (indices, point_data))
      തിരികെ അസത്യം;
    hb_vector_t<unsigned അക്ഷരം> &deltas_data = scratch.deltas_data.reset ();
    എങ്കിൽ (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      തിരികെ അസത്യം;

    എങ്കിൽ (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      എങ്കിൽ (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    തിരികെ !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  സ്ഥിതം തർക്കം compile_point_set (സ്ഥിരം hb_vector_t<തർക്കം> &point_indices,
                                 hb_vector_t<unsigned അക്ഷരം>& compiled_points /* OUT */)
  {
    unsigned അക്കം_points = 0;
    വേണ്ടി (തർക്കം i : point_indices)
      എങ്കിൽ (i) അക്കം_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    എങ്കിൽ (!അക്കം_points) തിരികെ സത്യം;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    എങ്കിൽ (അക്കം_points == indices_length)
      തിരികെ compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned അക്കം_bytes = 2 + 3 *അക്കം_points;
    എങ്കിൽ (unlikely (!compiled_points.resize_dirty  (അക്കം_bytes)))
      തിരികെ അസത്യം;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    എങ്കിൽ (അക്കം_points < 0x80)
      compiled_points.arrayZ[pos++] = അക്കം_points;
    അല്ലെങ്കിൽ
    {
      compiled_points.arrayZ[pos++] = ((അക്കം_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = അക്കം_points & 0xFF;
    }

    സ്ഥിരം unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_മൂല്യം = 0;
    unsigned അക്കം_encoded = 0;
    ആകുമ്പോൾ (i < indices_length && അക്കം_encoded < അക്കം_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      തർക്കം use_byte_encoding = അസത്യം;
      തർക്കം new_run = സത്യം;
      ആകുമ്പോൾ (i < indices_length && അക്കം_encoded < അക്കം_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        ആകുമ്പോൾ (i < indices_length && !point_indices[i])
          i++;

        എങ്കിൽ (i >= indices_length) നിർത്തുക;

        unsigned cur_മൂല്യം = i;
        unsigned delta = cur_മൂല്യം - last_മൂല്യം;

        എങ്കിൽ (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = അസത്യം;
        }

        എങ്കിൽ (use_byte_encoding && delta > 0xFF)
          നിർത്തുക;

        എങ്കിൽ (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        അല്ലെങ്കിൽ
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_മൂല്യം = cur_മൂല്യം;
        run_length++;
        അക്കം_encoded++;
      }

      എങ്കിൽ (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      അല്ലെങ്കിൽ
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    തിരികെ compiled_points.resize_dirty  (pos);
  }

  സ്ഥിതം ദശാംശം infer_delta (ദശാംശം target_val, ദശാംശം prev_val, ദശാംശം next_val, ദശാംശം prev_delta, ദശാംശം next_delta)
  {
    എങ്കിൽ (prev_val == next_val)
      തിരികെ (prev_delta == next_delta) ? prev_delta : 0.f;
    അല്ലെങ്കിൽ എങ്കിൽ (target_val <= hb_min (prev_val, next_val))
      തിരികെ (prev_val < next_val) ? prev_delta : next_delta;
    അല്ലെങ്കിൽ എങ്കിൽ (target_val >= hb_max (prev_val, next_val))
      തിരികെ (prev_val > next_val) ? prev_delta : next_delta;

    ദശാംശം r = (target_val - prev_val) / (next_val - prev_val);
    തിരികെ prev_delta + r * (next_delta - prev_delta);
  }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ next_index (unsigned പൂർണ്ണസംഖ്യ i, unsigned പൂർണ്ണസംഖ്യ start, unsigned പൂർണ്ണസംഖ്യ end)
  { തിരികെ (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
ഘടന TupleVariationData
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (ഇത്));
  }

  size_t get_size (unsigned axis_എണ്ണം_times_2) സ്ഥിരം
  {
    size_t total_size = min_size;
    unsigned എണ്ണം = tuple_Var_എണ്ണം.get_എണ്ണം ();
    സ്ഥിരം TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_എണ്ണം_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_എണ്ണം_times_2);
    }

    തിരികെ total_size;
  }

  സ്ഥിരം TupleVariationHeader &get_tuple_var_header (ശൂന്യം) സ്ഥിരം
  { തിരികെ StructAfter<TupleVariationHeader> (data); }

  ഘടന tuple_iterator_t;
  ഘടന tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    സ്വകാര്യ:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<സ്ഥിരം hb_vector_t<തർക്കം>*, hb_vector_t<unsigned അക്ഷരം>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<സ്ഥിരം hb_vector_t<തർക്കം>*, unsigned> point_set_എണ്ണം_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned അക്ഷരം> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    തർക്കം needs_padding = അസത്യം;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    തർക്കം is_composite = അസത്യം;

    പൊതു:
    tuple_variations_t () = സ്ഥിരസ്ഥിതി;
    tuple_variations_t (സ്ഥിരം tuple_variations_t&) = delete;
    tuple_variations_t& operator=(സ്ഥിരം tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = സ്ഥിരസ്ഥിതി;
    tuple_variations_t& operator=(tuple_variations_t&&) = സ്ഥിരസ്ഥിതി;
    ~tuple_variations_t () = സ്ഥിരസ്ഥിതി;

    explicit operator തർക്കം () സ്ഥിരം { തിരികെ തർക്കം (tuple_vars); }
    unsigned get_var_എണ്ണം () സ്ഥിരം
    {
      unsigned എണ്ണം = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      വേണ്ടി (auto& tuple: tuple_vars)
        എങ്കിൽ (tuple.compiled_deltas) എണ്ണം++;

      എങ്കിൽ (shared_points_bytes && shared_points_bytes->length)
        എണ്ണം |= Tuple_Var_എണ്ണം::SharedPointNumbers;
      തിരികെ എണ്ണം;
    }

    unsigned get_compiled_byte_size () സ്ഥിരം
    { തിരികെ compiled_byte_size; }

    തർക്കം create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_എണ്ണം,
                                     unsigned point_എണ്ണം,
                                     തർക്കം is_gvar,
                                     സ്ഥിരം hb_map_t *axes_old_index_tag_map,
                                     സ്ഥിരം hb_vector_t<unsigned> &shared_indices,
                                     സ്ഥിരം hb_array_t<സ്ഥിരം F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     തർക്കം is_composite_glyph = അസത്യം)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<പൂർണ്ണസംഖ്യ> deltas_x;
      hb_vector_t<പൂർണ്ണസംഖ്യ> deltas_y;
      ചെയ്യുക
      {
        സ്ഥിരം HBUINT8 *p = iterator.get_serialized_data ();
        unsigned പൂർണ്ണസംഖ്യ length = iterator.current_tuple->get_data_size ();
        എങ്കിൽ (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          തിരികെ അസത്യം;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        എങ്കിൽ (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_എണ്ണം (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          തിരികെ അസത്യം;

	private_indices.reset ();
        തർക്കം has_private_points = iterator.current_tuple->has_private_points ();
        സ്ഥിരം HBUINT8 *end = p + length;
        എങ്കിൽ (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          തിരികെ അസത്യം;

        സ്ഥിരം hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        തർക്കം apply_to_all = (indices.length == 0);
        unsigned അക്കം_deltas = apply_to_all ? point_എണ്ണം : indices.length;

        എങ്കിൽ (unlikely (!deltas_x.resize_dirty  (അക്കം_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          തിരികെ അസത്യം;

        എങ്കിൽ (is_gvar)
        {
          എങ്കിൽ (unlikely (!deltas_y.resize_dirty  (അക്കം_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            തിരികെ അസത്യം;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        എങ്കിൽ (unlikely (!var.indices.allocate_from_pool (pool, point_എണ്ണം) ||
                      !var.deltas_x.allocate_from_pool (pool, point_എണ്ണം, അസത്യം)))
          തിരികെ അസത്യം;

        എങ്കിൽ (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_എണ്ണം, അസത്യം)))
          തിരികെ അസത്യം;

        വേണ്ടി (unsigned i = 0; i < അക്കം_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          എങ്കിൽ (idx >= point_എണ്ണം) തുടരുക;
          var.indices[idx] = സത്യം;
          var.deltas_x[idx] = deltas_x[i];
          എങ്കിൽ (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } ആകുമ്പോൾ (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      തിരികെ സത്യം;
    }

    തർക്കം create_from_item_var_data (സ്ഥിരം VarData &var_data,
                                    സ്ഥിരം hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    സ്ഥിരം hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_എണ്ണം_2,
                                    സ്ഥിരം hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      എങ്കിൽ (&var_data == &Null (VarData))
        തിരികെ സത്യം;

      unsigned അക്കം_regions = var_data.get_region_index_എണ്ണം ();
      എങ്കിൽ (!tuple_vars.alloc (അക്കം_regions)) തിരികെ അസത്യം;

      item_എണ്ണം_2 = inner_map ? inner_map->get_population () : var_data.get_item_എണ്ണം ();
      എങ്കിൽ (!item_എണ്ണം_2) തിരികെ സത്യം;
      unsigned row_size = var_data.get_row_size ();
      സ്ഥിരം HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      വേണ്ടി (unsigned r = 0; r < അക്കം_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        എങ്കിൽ (!tuple.deltas_x.resize_dirty  (item_എണ്ണം_2) ||
            !tuple.indices.resize_dirty  (item_എണ്ണം_2))
          തിരികെ അസത്യം;

        വേണ്ടി (unsigned i = 0; i < item_എണ്ണം_2; i++)
        {
          tuple.indices.arrayZ[i] = സത്യം;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        എങ്കിൽ (region_index >= regions.length) തിരികെ അസത്യം;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      തിരികെ !tuple_vars.in_error ();
    }

    സ്വകാര്യ:
    സ്ഥിതം പൂർണ്ണസംഖ്യ _cmp_axis_tag (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
    {
      സ്ഥിരം hb_tag_t *a = (സ്ഥിരം hb_tag_t*) pa;
      സ്ഥിരം hb_tag_t *b = (സ്ഥിരം hb_tag_t*) pb;
      തിരികെ (പൂർണ്ണസംഖ്യ)(*a) - (പൂർണ്ണസംഖ്യ)(*b);
    }

    തർക്കം change_tuple_variations_axis_limits (സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              സ്ഥിരം hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      എങ്കിൽ (!axis_tags.alloc (normalized_axes_location.get_population ()))
        തിരികെ അസത്യം;
      വേണ്ടി (auto t : normalized_axes_location.താക്കോലുകൾ ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      വേണ്ടി (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        എങ്കിൽ (!normalized_axes_location.has (axis_tag, &axis_limit))
          തിരികെ അസത്യം;
        TripleDistances axis_triple_distances{1.f, 1.f};
        എങ്കിൽ (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        വേണ്ടി (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          എങ്കിൽ (!out) തുടരുക;

          unsigned new_len = new_vars.length + out.length;

          എങ്കിൽ (unlikely (!new_vars.alloc (new_len, അസത്യം)))
            തിരികെ അസത്യം;

          വേണ്ടി (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      തിരികെ സത്യം;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    തർക്കം merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      എങ്കിൽ (unlikely (!new_vars.alloc (tuple_vars.length)))
	തിരികെ അസത്യം;
      hb_hashmap_t<സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      വേണ്ടി (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        എങ്കിൽ (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          എങ്കിൽ (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            തിരികെ അസത്യം;
          തുടരുക;
        }

        unsigned *idx;
        എങ്കിൽ (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        അല്ലെങ്കിൽ
        {
	  auto *new_var = new_vars.push ();
	  എങ്കിൽ (unlikely (new_vars.in_error ()))
	    തിരികെ അസത്യം;
          hb_swap (*new_var, var);
          എങ്കിൽ (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            തിരികെ അസത്യം;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      തിരികെ സത്യം;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    തർക്കം compile_all_point_sets ()
    {
      വേണ്ടി (സ്ഥിരം auto& tuple: tuple_vars)
      {
        സ്ഥിരം hb_vector_t<തർക്കം>* points_set = &(tuple.indices);
        എങ്കിൽ (point_data_map.has (points_set))
        {
          unsigned *എണ്ണം;
          എങ്കിൽ (unlikely (!point_set_എണ്ണം_map.has (points_set, &എണ്ണം) ||
                        !point_set_എണ്ണം_map.set (points_set, (*എണ്ണം) + 1)))
            തിരികെ അസത്യം;
          തുടരുക;
        }

        hb_vector_t<unsigned അക്ഷരം> compiled_point_data;
        എങ്കിൽ (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          തിരികെ അസത്യം;

        എങ്കിൽ (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_എണ്ണം_map.set (points_set, 1))
          തിരികെ അസത്യം;
      }
      തിരികെ സത്യം;
    }

    /* find shared points set which saves most bytes */
    ശൂന്യം find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      വേണ്ടി (സ്ഥിരം auto& _ : point_data_map.iter_ref ())
      {
        സ്ഥിരം hb_vector_t<തർക്കം>* points_set = _.first;
        unsigned data_length = _.second.length;
        എങ്കിൽ (!data_length) തുടരുക;
        unsigned *എണ്ണം;
        എങ്കിൽ (unlikely (!point_set_എണ്ണം_map.has (points_set, &എണ്ണം) ||
                      *എണ്ണം <= 1))
        {
          shared_points_bytes = nullptr;
          തിരികെ;
        }

        unsigned saved_bytes = data_length * ((*എണ്ണം) -1);
        എങ്കിൽ (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    തർക്കം calc_inferred_deltas (സ്ഥിരം contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      വേണ്ടി (tuple_delta_t& var : tuple_vars)
        എങ്കിൽ (!var.calc_inferred_deltas (contour_points, scratch))
          തിരികെ അസത്യം;

      തിരികെ സത്യം;
    }

    തർക്കം iup_optimize (സ്ഥിരം contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      വേണ്ടി (tuple_delta_t& var : tuple_vars)
      {
        എങ്കിൽ (!var.optimize (contour_points, is_composite, scratch))
          തിരികെ അസത്യം;
      }
      തിരികെ സത്യം;
    }

    പൊതു:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    ശൂന്യം cull_unreachable (സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      വേണ്ടി (unsigned i = 0; i < tuple_vars.length; i++)
      {
	എങ്കിൽ (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  തുടരുക;
	എങ്കിൽ (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    തർക്കം instantiate (സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      സ്ഥിരം hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      തർക്കം optimize = അസത്യം)
    {
      എങ്കിൽ (!tuple_vars) തിരികെ സത്യം;
      എങ്കിൽ (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        തിരികെ അസത്യം;
      /* compute inferred deltas only for gvar */
      എങ്കിൽ (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	എങ്കിൽ (!calc_inferred_deltas (*contour_points, scratch))
	  തിരികെ അസത്യം;
      }

      /* if iup delta opt is on, contour_points can't be null */
      എങ്കിൽ (optimize && !contour_points)
        തിരികെ അസത്യം;

      എങ്കിൽ (!merge_tuple_variations (optimize ? contour_points : nullptr))
        തിരികെ അസത്യം;

      എങ്കിൽ (optimize && !iup_optimize (*contour_points, scratch)) തിരികെ അസത്യം;
      തിരികെ !tuple_vars.in_error ();
    }

    തർക്കം compile_bytes (സ്ഥിരം hb_map_t& axes_index_map,
                        സ്ഥിരം hb_map_t& axes_old_index_tag_map,
                        തർക്കം use_shared_points,
                        തർക്കം is_gvar = അസത്യം,
                        സ്ഥിരം hb_hashmap_t<സ്ഥിരം hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      എങ്കിൽ (!tuple_vars)
        തിരികെ സത്യം;

      // compile points set and store data in hashmap
      എങ്കിൽ (!compile_all_point_sets ())
        തിരികെ അസത്യം;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      എങ്കിൽ (use_shared_points)
      {
        find_shared_points ();
        എങ്കിൽ (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<പൂർണ്ണസംഖ്യ> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      വേണ്ടി (auto& tuple: tuple_vars)
      {
        സ്ഥിരം hb_vector_t<തർക്കം>* points_set = &(tuple.indices);
        hb_vector_t<unsigned അക്ഷരം> *points_data;
        എങ്കിൽ (unlikely (!point_data_map.has (points_set, &points_data)))
          തിരികെ അസത്യം;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        എങ്കിൽ (!points_data->length)
          തുടരുക;
        എങ്കിൽ (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          തിരികെ അസത്യം;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        എങ്കിൽ (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          തിരികെ അസത്യം;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      എങ്കിൽ (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = സത്യം;
        compiled_byte_size += 1;
      }

      തിരികെ സത്യം;
    }

    തർക്കം serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) സ്ഥിരം
    {
      TRACE_SERIALIZE (ഇത്);
      വേണ്ടി (സ്ഥിരം auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        എങ്കിൽ (c->in_error ()) return_trace (അസത്യം);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (സത്യം);
    }

    തർക്കം serialize_var_data (hb_serialize_context_t *c, തർക്കം is_gvar) സ്ഥിരം
    {
      TRACE_SERIALIZE (ഇത്);
      എങ്കിൽ (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      വേണ്ടി (സ്ഥിരം auto& tuple: tuple_vars)
      {
        സ്ഥിരം hb_vector_t<തർക്കം>* points_set = &(tuple.indices);
        hb_vector_t<unsigned അക്ഷരം> *point_data;
        എങ്കിൽ (!point_data_map.has (points_set, &point_data))
          return_trace (അസത്യം);

        എങ്കിൽ (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        എങ്കിൽ (c->in_error ()) return_trace (അസത്യം);
      }

      /* padding for gvar */
      എങ്കിൽ (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        എങ്കിൽ (!c->embed (pad)) return_trace (അസത്യം);
      }
      return_trace (സത്യം);
    }
  };

  ഘടന tuple_iterator_t
  {
    unsigned get_axis_എണ്ണം () സ്ഥിരം { തിരികെ axis_എണ്ണം; }

    ശൂന്യം init (hb_bytes_t var_data_bytes_, unsigned പൂർണ്ണസംഖ്യ axis_എണ്ണം_3, സ്ഥിരം ശൂന്യം *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_ഇടത് = var_data->tuple_Var_എണ്ണം.get_എണ്ണം ();
      axis_എണ്ണം = axis_എണ്ണം_3;
      axis_എണ്ണം_times_2 = axis_എണ്ണം_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    തർക്കം get_shared_indices (hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> &shared_indices /* OUT */)
    {
      എങ്കിൽ (var_data->has_shared_point_numbers ())
      {
        സ്ഥിരം HBUINT8 *base = &(table_base+var_data->data);
        സ്ഥിരം HBUINT8 *p = base;
        എങ്കിൽ (!decompile_points (p, shared_indices, (സ്ഥിരം HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) തിരികെ അസത്യം;
        data_offset = p - base;
      }
      തിരികെ സത്യം;
    }

    തർക്കം is_valid ()
    {
      എങ്കിൽ (unlikely (tuples_ഇടത് <= 0))
	തിരികെ അസത്യം;

      current_tuple_size = TupleVariationHeader::min_size;
      എങ്കിൽ (unlikely (!var_data_bytes.check_end ((സ്ഥിരം അക്ഷരം *) current_tuple + current_tuple_size)))
	തിരികെ അസത്യം;

      current_tuple_size = current_tuple->get_size (axis_എണ്ണം_times_2);
      എങ്കിൽ (unlikely (!var_data_bytes.check_end ((സ്ഥിരം അക്ഷരം *) current_tuple + current_tuple_size)))
	തിരികെ അസത്യം;

      തിരികെ സത്യം;
    }

    HB_ALWAYS_INLINE
    തർക്കം move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_ഇടത്--;
      തിരികെ is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    സ്ഥിരം HBUINT8 *get_serialized_data () സ്ഥിരം
    { തിരികെ &(table_base+var_data->data) + data_offset; }

    സ്വകാര്യ:
    signed tuples_ഇടത്;
    സ്ഥിരം TupleVariationData *var_data;
    unsigned പൂർണ്ണസംഖ്യ axis_എണ്ണം;
    unsigned പൂർണ്ണസംഖ്യ axis_എണ്ണം_times_2;
    unsigned പൂർണ്ണസംഖ്യ data_offset;
    unsigned പൂർണ്ണസംഖ്യ current_tuple_size;
    സ്ഥിരം ശൂന്യം *table_base;

    പൊതു:
    hb_bytes_t var_data_bytes;
    സ്ഥിരം TupleVariationHeader *current_tuple;
  };

  സ്ഥിതം തർക്കം get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_എണ്ണം,
                                  സ്ഥിരം ശൂന്യം *table_base,
                                  hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_എണ്ണം, table_base);
    എങ്കിൽ (!iterator->get_shared_indices (shared_indices))
      തിരികെ അസത്യം;
    തിരികെ iterator->is_valid ();
  }

  തർക്കം has_shared_point_numbers () സ്ഥിരം { തിരികെ tuple_Var_എണ്ണം.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  തർക്കം cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_എണ്ണം,
			      hb_array_t<സ്ഥിരം F2DOT14> shared_tuples,
			      സ്ഥിരം hb_map_t *axes_old_index_tag_map,
			      സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<അക്ഷരം> &out /* OUT */,
			      തർക്കം *changed /* OUT */) സ്ഥിരം
  {
    *changed = അസത്യം;

    hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> shared_indices;
    tuple_iterator_t iterator;
    എങ്കിൽ (!get_tuple_iterator (var_data_bytes, axis_എണ്ണം, ഇത്,
			     shared_indices, &iterator))
      തിരികെ സത്യം; /* no tuples or malformed: keep original */

    സ്ഥിരം അക്ഷരം *bytes_start = var_data_bytes.arrayZ;
    സ്ഥിരം അക്ഷരം *bytes_end = bytes_start + var_data_bytes.length;
    സ്ഥിരം അക്ഷരം *serialized_base = (സ്ഥിരം അക്ഷരം *) &(ഇത്+data);
    സ്ഥിരം അക്ഷരം *shared_points_end = (സ്ഥിരം അക്ഷരം *) iterator.get_serialized_data ();
    എങ്കിൽ (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      തിരികെ സത്യം;

    ഘടന kept_tuple_t
    {
      സ്ഥിരം അക്ഷരം *header;
      unsigned header_size;
      സ്ഥിരം അക്ഷരം *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    ചെയ്യുക
    {
      സ്ഥിരം TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_എണ്ണം * 2);
      unsigned data_size = header->get_data_size ();
      സ്ഥിരം അക്ഷരം *tuple_data = (സ്ഥിരം അക്ഷരം *) iterator.get_serialized_data ();
      എങ്കിൽ (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	തിരികെ സത്യം; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      എങ്കിൽ (!header->unpack_axis_tuples (axis_എണ്ണം, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	തിരികെ സത്യം;

      total++;
      എങ്കിൽ (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(സ്ഥിരം അക്ഷരം *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } ആകുമ്പോൾ (iterator.move_to_next ());

    എങ്കിൽ (unlikely (kept.in_error ())) തിരികെ അസത്യം;
    എങ്കിൽ (kept.length == total) തിരികെ സത്യം; /* nothing to cull */

    *changed = സത്യം;
    എങ്കിൽ (!kept.length) തിരികെ സത്യം; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    എങ്കിൽ (unlikely (!out.resize (new_size)))
      തിരികെ അസത്യം;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_എണ്ണം = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    അക്ഷരം *p = out.arrayZ + min_size;
    വേണ്ടി (സ്ഥിരം auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    വേണ്ടി (സ്ഥിരം auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    തിരികെ സത്യം;
  }

  സ്ഥിതം തർക്കം decompile_points (സ്ഥിരം HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> &points /* OUT */,
				സ്ഥിരം HBUINT8 *end)
  {
    എണ്ണൽ packed_point_flag_t
    {
      POINTS_ARE_വാക്കുകൾ     = 0x80,
      POINT_RUN_എണ്ണം_MASK = 0x7F
    };

    എങ്കിൽ (unlikely (p + 1 > end)) തിരികെ അസത്യം;

    unsigned എണ്ണം = *p++;
    എങ്കിൽ (എണ്ണം & POINTS_ARE_വാക്കുകൾ)
    {
      എങ്കിൽ (unlikely (p + 1 > end)) തിരികെ അസത്യം;
      എണ്ണം = ((എണ്ണം & POINT_RUN_എണ്ണം_MASK) << 8) | *p++;
    }
    എങ്കിൽ (unlikely (!points.resize_dirty  (എണ്ണം))) തിരികെ അസത്യം;

    unsigned n = 0;
    unsigned i = 0;
    ആകുമ്പോൾ (i < എണ്ണം)
    {
      എങ്കിൽ (unlikely (p + 1 > end)) തിരികെ അസത്യം;
      unsigned control = *p++;
      unsigned run_എണ്ണം = (control & POINT_RUN_എണ്ണം_MASK) + 1;
      unsigned stop = i + run_എണ്ണം;
      എങ്കിൽ (unlikely (stop > എണ്ണം)) തിരികെ അസത്യം;
      എങ്കിൽ (control & POINTS_ARE_വാക്കുകൾ)
      {
        എങ്കിൽ (unlikely (p + run_എണ്ണം * HBUINT16::static_size > end)) തിരികെ അസത്യം;
        വേണ്ടി (; i < stop; i++)
        {
          n += *(സ്ഥിരം HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      അല്ലെങ്കിൽ
      {
        എങ്കിൽ (unlikely (p + run_എണ്ണം > end)) തിരികെ അസത്യം;
        വേണ്ടി (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    തിരികെ സത്യം;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  സ്ഥിതം തർക്കം decompile_deltas (സ്ഥിരം HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				സ്ഥിരം HBUINT8 *end,
				തർക്കം consume_all = അസത്യം,
				unsigned start = 0)
  {
    തിരികെ Tuple_മൂല്യങ്ങൾ::decompile (p, deltas, end, consume_all, start);
  }

  തർക്കം has_data () സ്ഥിരം { തിരികെ tuple_Var_എണ്ണം; }

  തർക്കം decompile_tuple_variations (unsigned point_എണ്ണം,
                                   തർക്കം is_gvar,
                                   tuple_iterator_t iterator,
                                   സ്ഥിരം hb_map_t *axes_old_index_tag_map,
                                   സ്ഥിരം hb_vector_t<unsigned> &shared_indices,
                                   സ്ഥിരം hb_array_t<സ്ഥിരം F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   തർക്കം is_composite_glyph = അസത്യം) സ്ഥിരം
  {
    തിരികെ tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_എണ്ണം,
                                                        point_എണ്ണം, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  തർക്കം serialize (hb_serialize_context_t *c,
                  തർക്കം is_gvar,
                  സ്ഥിരം tuple_variations_t& tuple_variations) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    /* empty tuple variations, just return and skip serialization. */
    എങ്കിൽ (!tuple_variations) return_trace (സത്യം);

    auto *out = c->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (out))) return_trace (അസത്യം);

    എങ്കിൽ (!c->check_assign (out->tuple_Var_എണ്ണം, tuple_variations.get_var_എണ്ണം (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (അസത്യം);

    unsigned total_header_len = 0;

    എങ്കിൽ (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (അസത്യം);

    unsigned data_offset = min_size + total_header_len;
    എങ്കിൽ (!is_gvar) data_offset += 4;
    എങ്കിൽ (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (അസത്യം);

    തിരികെ tuple_variations.serialize_var_data (c, is_gvar);
  }

  സംരക്ഷിത:
  ഘടന Tuple_Var_എണ്ണം : HBUINT16
  {
    friend ഘടന tuple_variations_t;
    തർക്കം has_shared_point_numbers () സ്ഥിരം { തിരികെ ((*ഇത്) & SharedPointNumbers); }
    unsigned പൂർണ്ണസംഖ്യ get_എണ്ണം () സ്ഥിരം { തിരികെ (*ഇത്) & എണ്ണം_Mask; }
    Tuple_Var_എണ്ണം& operator = (uint16_t i) { HBUINT16::operator= (i); തിരികെ *ഇത്; }
    explicit operator തർക്കം () സ്ഥിരം { തിരികെ get_എണ്ണം (); }

    സംരക്ഷിത:
    എണ്ണൽ Flags
    {
      SharedPointNumbers= 0x8000u,
      എണ്ണം_Mask         = 0x0FFFu
    };
    പൊതു:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_എണ്ണം tuple_Var_എണ്ണം;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  പൊതു:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
ഘടന item_variations_t
{
  using region_t = സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>*;
  സ്വകാര്യ:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_data_അക്കം_rows;

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
  hb_vector_t<hb_vector_t<പൂർണ്ണസംഖ്യ>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  തർക്കം has_long = അസത്യം;

  പൊതു:
  തർക്കം has_long_word () സ്ഥിരം
  { തിരികെ has_long; }

  സ്ഥിരം hb_vector_t<region_t>& get_region_list () സ്ഥിരം
  { തിരികെ region_list; }

  സ്ഥിരം hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () സ്ഥിരം
  { തിരികെ encodings; }

  സ്ഥിരം hb_map_t& get_varidx_map () സ്ഥിരം
  { തിരികെ varidx_map; }

  തർക്കം add_vardata_encoding_for_testing (hb_vector_t<സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*> &&rows,
                                         unsigned അക്കം_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), അക്കം_cols));
    തിരികെ !encodings.in_error ();
  }

  തർക്കം compile_varidx_map_for_testing (സ്ഥിരം hb_hashmap_t<unsigned, സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*>& front_mapping)
  { തിരികെ compile_varidx_map (front_mapping); }

  തർക്കം instantiate (സ്ഥിരം ItemVariationStore& varStore,
                    സ്ഥിരം hb_subset_plan_t *plan,
                    തർക്കം optimize=സത്യം,
                    തർക്കം use_no_variation_idx=സത്യം,
                    സ്ഥിരം hb_array_t <സ്ഥിരം hb_inc_bimap_t> inner_maps = hb_array_t<സ്ഥിരം hb_inc_bimap_t> (),
                    തർക്കം layout_conditions = അസത്യം)
  {
    എങ്കിൽ (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      തിരികെ അസത്യം;
    /* avar2 partial instancing: cull unreachable regions. */
    എങ്കിൽ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      വേണ്ടി (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    എങ്കിൽ (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      തിരികെ അസത്യം;
    എങ്കിൽ (layout_conditions &&
        (!instantiate_condition_മൂല്യങ്ങൾ (plan, varStore) || !build_region_list ()))
      തിരികെ അസത്യം;
    തിരികെ as_item_varstore (optimize, use_no_variation_idx);
  }

  തർക്കം instantiate_condition_മൂല്യങ്ങൾ (സ്ഥിരം hb_subset_plan_t *plan,
                                      സ്ഥിരം ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_മൂല്യം_map;
    condition_map.clear ();
    എങ്കിൽ (!plan->layout_condition_മൂല്യങ്ങൾ) തിരികെ സത്യം;
    using condition_മൂല്യം_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>>;
    hb_vector_t<condition_മൂല്യം_t> conditions;
    വേണ്ടി (auto condition : plan->layout_condition_മൂല്യങ്ങൾ.iter ())
      conditions.push (condition);
    എങ്കിൽ (unlikely (conditions.in_error ())) തിരികെ അസത്യം;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (സ്ഥിരം condition_മൂല്യം_t &a, സ്ഥിരം condition_മൂല്യം_t &b) {
      എങ്കിൽ (a.second.first != b.second.first)
        തിരികെ a.second.first < b.second.first ? -1 : 1;
      തിരികെ a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    വേണ്ടി (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      എങ്കിൽ (plan->gdef_varstore_inner_maps[i].get_population ())
        എങ്കിൽ (!outer_map.set (i, new_outer++)) തിരികെ അസത്യം;
    വേണ്ടി (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      ഇരട്ട value = condition.second.second +
                     varStore.get_delta<ഇരട്ട> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      എങ്കിൽ (!outer_map.has (old_outer))
        തുടരുക;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      എങ്കിൽ (outer >= vars.length || inner >= var_data_അക്കം_rows[outer])
        തുടരുക;

      ഇരട്ട maximum = fabs ((ഇരട്ട) value);
      തർക്കം variable = അസത്യം;
      വേണ്ടി (സ്ഥിരം tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        എങ്കിൽ (unlikely (inner >= tuple.deltas_x.length)) തിരികെ അസത്യം;
        ദശാംശം delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((ഇരട്ട) delta));
      }
      എങ്കിൽ (!variable)
      {
        എങ്കിൽ (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), പൂർണ്ണസംഖ്യ (value > 0))))
          തിരികെ അസത്യം;
        തുടരുക;
      }

      ഇരട്ട scale = 1.;
      ആകുമ്പോൾ (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      ആകുമ്പോൾ (maximum * scale * 2 <= INT_MAX)
      {
        തർക്കം integral = (ഇരട്ട) value * scale == round ((ഇരട്ട) value * scale);
        വേണ്ടി (സ്ഥിരം tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (ഇരട്ട) tuple.deltas_x[inner] * scale ==
                      round ((ഇരട്ട) tuple.deltas_x[inner] * scale);
        എങ്കിൽ (integral) നിർത്തുക;
        scale *= 2;
      }
      പൂർണ്ണസംഖ്യ default_മൂല്യം = (പൂർണ്ണസംഖ്യ) round ((ഇരട്ട) value * scale);
      എങ്കിൽ (scale != 1. || default_മൂല്യം < -32768 || default_മൂല്യം > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        വേണ്ടി (സ്ഥിരം tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          എങ്കിൽ (!tuple.deltas_x[inner]) തുടരുക;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (സത്യം);
          copy.deltas_x.push ((ദശാംശം) round ((ഇരട്ട) tuple.deltas_x[inner] * scale));
          എങ്കിൽ (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) തിരികെ അസത്യം;
          private_row.tuple_vars.push (std::move (copy));
        }
        എങ്കിൽ (unlikely (vars.length >= 0xFFFFu)) തിരികെ അസത്യം;
        എങ്കിൽ (unlikely (private_row.tuple_vars.in_error ())) തിരികെ അസത്യം;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_അക്കം_rows.push (1);
        എങ്കിൽ (unlikely (vars.in_error () || var_data_അക്കം_rows.in_error ())) തിരികെ അസത്യം;
        എങ്കിൽ (default_മൂല്യം < -32768 || default_മൂല്യം > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          പൂർണ്ണസംഖ്യ bias = default_മൂല്യം - hb_clamp (default_മൂല്യം, -32768, 32767);
          default_മൂല്യം -= bias;
          പൂർണ്ണസംഖ്യ മുകളിലെ = bias / 65536 * 65536;
          പൂർണ്ണസംഖ്യ parts[] = {മുകളിലെ, bias - മുകളിലെ};
          വേണ്ടി (പൂർണ്ണസംഖ്യ part : parts)
          {
            എങ്കിൽ (!part) തുടരുക;
            unsigned എണ്ണം = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            സ്ഥിരം auto &tuples = vars[idx >> 16].tuple_vars;
            എങ്കിൽ (unlikely (tuples.in_error () || tuples.length != എണ്ണം + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) തിരികെ അസത്യം;
          }
        }
      }
      എങ്കിൽ (!condition_map.set (condition.first, hb_pair (idx, default_മൂല്യം)))
        തിരികെ അസത്യം;
    }
    തിരികെ !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  തർക്കം create_from_item_varstore (സ്ഥിരം ItemVariationStore& varStore,
                                  സ്ഥിരം hb_map_t& axes_old_index_tag_map,
                                  സ്ഥിരം hb_array_t <സ്ഥിരം hb_inc_bimap_t> inner_maps = hb_array_t<സ്ഥിരം hb_inc_bimap_t> ())
  {
    സ്ഥിരം VarRegionList& regionList = varStore.get_region_list ();
    എങ്കിൽ (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      തിരികെ അസത്യം;

    unsigned അക്കം_var_data = varStore.get_sub_table_എണ്ണം ();
    എങ്കിൽ (inner_maps && inner_maps.length != അക്കം_var_data) തിരികെ അസത്യം;
    എങ്കിൽ (!vars.alloc (അക്കം_var_data) ||
        !var_data_അക്കം_rows.alloc (അക്കം_var_data)) തിരികെ അസത്യം;

    വേണ്ടി (unsigned i = 0; i < അക്കം_var_data; i++)
    {
      എങ്കിൽ (inner_maps && !inner_maps.arrayZ[i].get_population ())
          തുടരുക;
      tuple_variations_t var_data_tuples;
      unsigned item_എണ്ണം_2 = 0;
      എങ്കിൽ (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_എണ്ണം_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        തിരികെ അസത്യം;

      var_data_അക്കം_rows.push (item_എണ്ണം_2);
      vars.push (std::move (var_data_tuples));
    }
    തിരികെ !vars.in_error () && !var_data_അക്കം_rows.in_error () && vars.length == var_data_അക്കം_rows.length;
  }

  തർക്കം instantiate_tuple_vars (സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               സ്ഥിരം hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               തർക്കം build_regions = സത്യം)
  {
    optimize_scratch_t scratch;
    വേണ്ടി (tuple_variations_t& tuple_vars : vars)
      എങ്കിൽ (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        തിരികെ അസത്യം;

    തിരികെ !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_എണ്ണം_2)
  {
    vars.push (tuple_variations_t ());
    var_data_അക്കം_rows.push (item_എണ്ണം_2);
    തിരികെ vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_എണ്ണം (unsigned outer) സ്ഥിരം
  { തിരികെ outer < var_data_അക്കം_rows.length ? var_data_അക്കം_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    എങ്കിൽ (unlikely (outer >= vars.length ||
                  inner >= var_data_അക്കം_rows[outer] ||
                  var_data_അക്കം_rows[outer] >= 0xFFFFu))
      തിരികെ (unsigned) -1;
    വേണ്ടി (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      എങ്കിൽ (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        തിരികെ (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      എങ്കിൽ (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      എങ്കിൽ (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        തിരികെ (unsigned) -1;
    }
    തിരികെ var_data_അക്കം_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  ശൂന്യം add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, പൂർണ്ണസംഖ്യ delta, unsigned item_എണ്ണം_2)
  {
    എങ്കിൽ (outer >= vars.length) തിരികെ;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    എങ്കിൽ (!tuple.indices.resize (item_എണ്ണം_2) ||
        !tuple.deltas_x.resize (item_എണ്ണം_2)) തിരികെ;
    വേണ്ടി (unsigned i = 0; i < item_എണ്ണം_2; i++)
    {
      tuple.indices.arrayZ[i] = സത്യം;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (ദശാംശം) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  തർക്കം build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    വേണ്ടി (സ്ഥിരം tuple_variations_t& sub_table : vars)
    {
      വേണ്ടി (സ്ഥിരം tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        എങ്കിൽ (!used_regions.has (r))
        {
          തർക്കം all_zeros = സത്യം;
          വേണ്ടി (ദശാംശം d : tuple.deltas_x)
          {
            പൂർണ്ണസംഖ്യ delta = hb_clamp_to<പൂർണ്ണസംഖ്യ> (roundf ((ഇരട്ട) d));
            എങ്കിൽ (delta != 0)
            {
              all_zeros = അസത്യം;
              നിർത്തുക;
            }
          }
          എങ്കിൽ (!all_zeros)
          {
            എങ്കിൽ (!used_regions.set (r, 1))
              തിരികെ അസത്യം;
          }
        }
        എങ്കിൽ (all_regions.has (r))
          തുടരുക;
        എങ്കിൽ (!all_regions.set (r, 1))
          തിരികെ അസത്യം;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    എങ്കിൽ (!all_regions || !all_unique_regions) തിരികെ സത്യം;

    എങ്കിൽ (!region_list.alloc (all_regions.get_population ()))
      തിരികെ അസത്യം;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    വേണ്ടി (സ്ഥിരം auto& r : orig_region_list)
    {
      എങ്കിൽ (!all_regions.has (&r) || !used_regions.has (&r))
        തുടരുക;

      region_list.push (&r);
      എങ്കിൽ (!region_map.set (&r, idx))
        തിരികെ അസത്യം;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    വേണ്ടി (സ്ഥിരം auto& r: all_unique_regions)
    {
      എങ്കിൽ (!all_regions.has (r) || !used_regions.has (r))
        തുടരുക;
      region_list.push (r);
      എങ്കിൽ (!region_map.set (r, idx))
        തിരികെ അസത്യം;
      all_regions.del (r);
      idx++;
    }
    തിരികെ (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  ഘടന combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = സ്ഥിരസ്ഥിതി;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      പ്രതിജ്ഞാ (gain < 0xFFFFFF);
      പ്രതിജ്ഞാ (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    തർക്കം operator < (സ്ഥിരം combined_gain_idx_tuple_t& o)
    {
      തിരികെ encoded < o.encoded;
    }

    തർക്കം operator <= (സ്ഥിരം combined_gain_idx_tuple_t& o)
    {
      തിരികെ encoded <= o.encoded;
    }

    unsigned idx_1 () സ്ഥിരം { തിരികെ (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () സ്ഥിരം { തിരികെ encoded & 0xFFFFF; };
  };

  തർക്കം as_item_varstore (തർക്കം optimize=സത്യം, തർക്കം use_no_variation_idx=സത്യം)
  {
    /* return true if no variation data */
    എങ്കിൽ (!region_list) തിരികെ സത്യം;
    unsigned അക്കം_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    വേണ്ടി (unsigned major = 0; major < var_data_അക്കം_rows.length; major++)
      total_rows += var_data_അക്കം_rows[major];

    എങ്കിൽ (!delta_rows.resize (total_rows)) തിരികെ അസത്യം;
    /* init all rows to [0]*num_cols */
    വേണ്ടി (unsigned i = 0; i < total_rows; i++)
      എങ്കിൽ (!(delta_rows[i].resize (അക്കം_cols))) തിരികെ അസത്യം;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ> *> major_rows;
    hb_hashmap_t<സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*, unsigned> delta_rows_map;
    വേണ്ടി (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      സ്ഥിരം tuple_variations_t& tuples = vars[major];
      unsigned അക്കം_rows = var_data_അക്കം_rows[major];

      എങ്കിൽ (!അക്കം_rows) തുടരുക;

      വേണ്ടി (സ്ഥിരം tuple_delta_t& tuple: tuples.tuple_vars)
      {
        എങ്കിൽ (tuple.deltas_x.length != അക്കം_rows)
          തിരികെ അസത്യം;

        /* skip unused regions */
        unsigned *col_idx;
        എങ്കിൽ (!region_map.has (&(tuple.axis_tuples), &col_idx))
          തുടരുക;

        വേണ്ടി (unsigned i = 0; i < അക്കം_rows; i++)
        {
          പൂർണ്ണസംഖ്യ rounded_delta = hb_clamp_to<പൂർണ്ണസംഖ്യ> (roundf ((ഇരട്ട) tuple.deltas_x[i]));
          പൂർണ്ണസംഖ്യ &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          എങ്കിൽ (unlikely (sum < INT_MIN || sum > INT_MAX)) തിരികെ അസത്യം;
          delta = (പൂർണ്ണസംഖ്യ) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      വേണ്ടി (unsigned minor = 0; minor < അക്കം_rows; minor++)
      {
	സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>& row = delta_rows[start_row + minor];
	എങ്കിൽ (use_no_variation_idx)
	{
	  തർക്കം all_zeros = സത്യം;
	  വേണ്ടി (പൂർണ്ണസംഖ്യ delta : row)
	  {
	    എങ്കിൽ (delta != 0)
	    {
	      all_zeros = അസത്യം;
	      നിർത്തുക;
	    }
	  }
	  എങ്കിൽ (all_zeros)
	    തുടരുക;
	}

	എങ്കിൽ (!front_mapping.set ((major<<16) + minor, &row))
	  തിരികെ അസത്യം;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	എങ്കിൽ (optimize)
	{
	  എങ്കിൽ (delta_rows_map.has (&row))
	    തുടരുക;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      എങ്കിൽ (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        തിരികെ അസത്യം;
      എങ്കിൽ (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), അക്കം_cols);
	എങ്കിൽ (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  തിരികെ അസത്യം;
      }

      start_row += അക്കം_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    എങ്കിൽ (!optimize)
    {
      encodings = std::move (encoding_objs);
      തിരികെ !encodings.in_error ();
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
    hb_vector_t<item_t> queue_ഇനങ്ങൾ;
    unsigned അക്കം_todos = encoding_objs.length;
    വേണ്ടി (unsigned i = 0; i < അക്കം_todos; i++)
    {
      വേണ്ടി (unsigned j = i + 1; j < അക്കം_todos; j++)
      {
        പൂർണ്ണസംഖ്യ combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        എങ്കിൽ (combining_gain > 0)
          queue_ഇനങ്ങൾ.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_ഇനങ്ങൾ));

    hb_bit_set_t removed_todo_idxes;
    ആകുമ്പോൾ (queue)
    {
      auto t = queue.എടുക്കുക_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      എങ്കിൽ (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        തുടരുക;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      എങ്കിൽ (unlikely (removed_todo_idxes.in_error ())) തിരികെ അസത്യം;

      encoding.merge (other_encoding);
      എങ്കിൽ (unlikely (encoding.in_error ())) തിരികെ അസത്യം;

      വേണ്ടി (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        എങ്കിൽ (removed_todo_idxes.has (idx)) തുടരുക;

        സ്ഥിരം delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        എങ്കിൽ (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          വേണ്ടി (സ്ഥിരം auto& row : obj.ഇനങ്ങൾ)
            encoding.add_row (row);
          എങ്കിൽ (unlikely (encoding.in_error ())) തിരികെ അസത്യം;

          removed_todo_idxes.add (idx);
          എങ്കിൽ (unlikely (removed_todo_idxes.in_error ())) തിരികെ അസത്യം;
          തുടരുക;
        }

        പൂർണ്ണസംഖ്യ combined_gain = encoding.gain_from_merging (obj);
        എങ്കിൽ (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      എങ്കിൽ (unlikely (encoding_objs.in_error ())) തിരികെ അസത്യം;
    }

    പൂർണ്ണസംഖ്യ അക്കം_final_encodings = (പൂർണ്ണസംഖ്യ) encoding_objs.length - (പൂർണ്ണസംഖ്യ) removed_todo_idxes.get_population ();
    എങ്കിൽ (അക്കം_final_encodings <= 0) തിരികെ അസത്യം;

    എങ്കിൽ (!encodings.alloc (അക്കം_final_encodings)) തിരികെ അസത്യം;
    വേണ്ടി (unsigned i = 0; i < encoding_objs.length; i++)
    {
      എങ്കിൽ (removed_todo_idxes.has (i)) തുടരുക;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    തിരികെ compile_varidx_map (front_mapping);
  }

  സ്വകാര്യ:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  തർക്കം compile_varidx_map (സ്ഥിരം hb_hashmap_t<unsigned, സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    വേണ്ടി (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      എങ്കിൽ (encoding.is_empty () || encoding.in_error ())
        തിരികെ അസത്യം;

      unsigned അക്കം_rows = encoding.ഇനങ്ങൾ.length;
      unsigned അക്കം_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.ഇനങ്ങൾ.qsort (_cmp_row);

      വേണ്ടി (unsigned start = 0; start < അക്കം_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (അക്കം_rows - start, 0xFFFFu);
        hb_vector_t<സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*> rows;

        എങ്കിൽ (!rows.alloc (chunk_len))
          തിരികെ അസത്യം;

        unsigned major = split_encodings.length;
        വേണ്ടി (unsigned minor = 0; minor < chunk_len; minor++)
        {
          സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ> *row = encoding.ഇനങ്ങൾ.arrayZ[start + minor];
          rows.push (row);
          എങ്കിൽ (!back_mapping.set (row, (major << 16) + minor))
            തിരികെ അസത്യം;
        }

        auto *split = split_encodings.push (std::move (rows), അക്കം_cols);
        എങ്കിൽ (unlikely (split_encodings.in_error () || split->in_error ()))
          തിരികെ അസത്യം;
      }
    }

    encodings = std::move (split_encodings);
    എങ്കിൽ (encodings.in_error () || back_mapping.in_error ())
      തിരികെ അസത്യം;

    വേണ്ടി (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      എങ്കിൽ (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      അല്ലെങ്കിൽ
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    തിരികെ !varidx_map.in_error ();
  }

  സ്ഥിതം പൂർണ്ണസംഖ്യ _cmp_row (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>** a = (സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>**) pa;
    സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>** b = (സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>**) pb;

    വേണ്ടി (unsigned i = 0; i < (*b)->length; i++)
    {
      പൂർണ്ണസംഖ്യ va = (*a)->arrayZ[i];
      പൂർണ്ണസംഖ്യ vb = (*b)->arrayZ[i];
      എങ്കിൽ (va != vb)
        തിരികെ va < vb ? -1 : 1;
    }
    തിരികെ 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

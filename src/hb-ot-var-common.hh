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

#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-alloc-pool.hh"
#ଅନ୍ତର୍ଭାଵ "hb-priority-queue.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-instancer-iup.hh"


ନାମାକାଶ OT {

using rebase_tent_result_scratch_t = hb_pair_t<rebase_tent_result_t, rebase_tent_result_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
ସଂରଚନା TupleVariationHeader
{
  friend ସଂରଚନା tuple_delta_t;
  size_t get_size (unsigned axis_ଗଣନା_times_2) ଅଚଳ
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tupleIndex;
    ଯଦି (unlikely ((ti & (TupleIndex::EmbeddedPeakTuple | TupleIndex::IntermediateRegion))))
    {
      unsigned ଗଣନା_2 = ((ti & TupleIndex::EmbeddedPeakTuple) != 0) + ((ti & TupleIndex::IntermediateRegion) != 0) * 2;
      ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (ଗଣନା_2, axis_ଗଣନା_times_2, min_size);
    }
    ଫେରନ୍ତୁ min_size;
  }

  unsigned get_data_size () ଅଚଳ { ଫେରନ୍ତୁ varDataSize; }

  ଅଚଳ TupleVariationHeader &get_next (unsigned axis_ଗଣନା_times_2) ଅଚଳ
  { ଫେରନ୍ତୁ StructAtOffset<TupleVariationHeader> (ଏହି, get_size (axis_ଗଣନା_times_2)); }

  ତାର୍କିକ unpack_axis_tuples (unsigned axis_ଗଣନା,
                           ଅଚଳ hb_array_t<ଅଚଳ F2DOT14> shared_tuples,
                           ଅଚଳ hb_map_t *axes_old_index_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ଅଚଳ
  {
    ଅଚଳ F2DOT14 *peak_tuple = nullptr;
    ଯଦି (has_peak ())
      peak_tuple = get_peak_tuple (axis_ଗଣନା);
    ଅନ୍ୟଥା
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = get_index ();
      ଯଦି (unlikely ((index + 1) * axis_ଗଣନା > shared_tuples.length))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      peak_tuple = shared_tuples.sub_array (axis_ଗଣନା * index, axis_ଗଣନା).arrayZ;
    }

    ଅଚଳ F2DOT14 *start_tuple = nullptr;
    ଅଚଳ F2DOT14 *end_tuple = nullptr;
    ତାର୍କିକ has_interm = has_intermediate ();

    ଯଦି (has_interm)
    {
      start_tuple = get_start_tuple (axis_ଗଣନା);
      end_tuple = get_end_tuple (axis_ଗଣନା);
    }

    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା; i++)
    {
      ଦଶମିକ peak = peak_tuple[i].to_float ();
      ଯଦି (peak == 0.f) ଜାରି;

      hb_tag_t *axis_tag;
      ଯଦି (!axes_old_index_tag_map->has (i, &axis_tag))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଦଶମିକ start, end;
      ଯଦି (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      ଅନ୍ୟଥା
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  HB_ALWAYS_INLINE
  ଦଶମିକ calculate_scalar (hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
			   ଅଚଳ hb_array_t<ଅଚଳ F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ଅଚଳ
  {
    unsigned tuple_index = tupleIndex;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = tuple_index & TupleIndex::TupleIndexMask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    ତାର୍କିକ plain_shared_tuple = (tuple_index & ~TupleIndex::PrivatePointNumbers) < shared_tuple_scalar_cache->length;
    ଯଦି (likely (plain_shared_tuple))
    {
      ଦଶମିକ scalar;
      ଯଦି (likely (shared_tuple_scalar_cache->get (index, &scalar)))
	ଫେରନ୍ତୁ scalar;
    }

    ଅଚଳ F2DOT14 *peak_tuple;

    ତାର୍କିକ has_interm = tuple_index & TupleIndex::IntermediateRegion; // Inlined for performance

    ଯଦି (unlikely (tuple_index & TupleIndex::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_ଗଣନା);
      shared_tuple_scalar_cache = nullptr;
    }
    ଅନ୍ୟଥା
    {
      ଦଶମିକ scalar;
      ଯଦି (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->length &&
	  shared_tuple_scalar_cache->get (index, &scalar))
      {
        ଯଦି (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	ଅନ୍ୟଥା
	  ଫେରନ୍ତୁ scalar;
      }

      ଯଦି (unlikely ((index + 1) * coord_ଗଣନା > shared_tuples.length))
        ଫେରନ୍ତୁ 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_ଗଣନା * index);

    }

    ଅଚଳ F2DOT14 *start_tuple = nullptr;
    ଅଚଳ F2DOT14 *end_tuple = nullptr;

    ଯଦି (has_interm)
    {
      start_tuple = get_start_tuple (coord_ଗଣନା);
      end_tuple = get_end_tuple (coord_ଗଣନା);
    }

    ଦଶମିକ scalar = 1.f;
#ifndef HB_OPTIMIZE_SIZE
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
    ତାର୍କିକ skip = coord_ଗଣନା >= 16;
#endif
#endif
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < coord_ଗଣନା; i++)
    {
#ifndef HB_OPTIMIZE_SIZE
#ଯଦି HB_FAST_ଅଙ୍କ_ACCESS
      ଯଦି (skip)
      {
	ଯେତେବେଳେ (i + 4 <= coord_ଗଣନା && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	ଯେତେବେଳେ (i < coord_ଗଣନା && peak_tuple[i].to_int () == 0)
	  i += 1;
	ଯଦି (i >= coord_ଗଣନା)
	  ଭାଙ୍ଗ;
      }
#endif
#endif

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା peak = peak_tuple[i].to_int ();
      ଯଦି (!peak) ଜାରି;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = coords[i];
      ଯଦି (!v) { scalar = 0.f; ଭାଙ୍ଗ; }
      ଯଦି (v == peak) ଜାରି;

      ଯଦି (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = start_tuple[i].to_int ();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = end_tuple[i].to_int ();
        ଯଦି (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) ଜାରି;
        ଯଦି (v < start || v > end) { scalar = 0.f; ଭାଙ୍ଗ; }
        ଯଦି (v < peak)
        { ଯଦି (peak != start) scalar *= (ଦଶମିକ) (v - start) / (peak - start); }
        ଅନ୍ୟଥା
        { ଯଦି (peak != end) scalar *= (ଦଶମିକ) (end - v) / (end - peak); }
      }
      ଅନ୍ୟଥା ଯଦି (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; ଭାଙ୍ଗ; }
      ଅନ୍ୟଥା
        scalar *= (ଦଶମିକ) v / peak;
    }
    ଯଦି (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (index, scalar);
    ଫେରନ୍ତୁ scalar;
  }

  ତାର୍କିକ           has_peak () ଅଚଳ { ଫେରନ୍ତୁ tupleIndex & TupleIndex::EmbeddedPeakTuple; }
  ତାର୍କିକ   has_intermediate () ଅଚଳ { ଫେରନ୍ତୁ tupleIndex & TupleIndex::IntermediateRegion; }
  ତାର୍କିକ has_private_points () ଅଚଳ { ଫେରନ୍ତୁ tupleIndex & TupleIndex::PrivatePointNumbers; }
  unsigned      get_index () ଅଚଳ { ଫେରନ୍ତୁ tupleIndex & TupleIndex::TupleIndexMask; }
  ସ୍ଥିର constexpr unsigned max_shared_tuple_ଗଣନା = 0x1000u;

  ସୁରକ୍ଷିତ:
  ସଂରଚନା TupleIndex : HBUINT16
  {
    ଗଣନା Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      TupleIndexMask      = 0x0FFFu
    };

    TupleIndex& operator = (uint16_t i) { HBUINT16::operator= (i); ଫେରନ୍ତୁ *ଏହି; }
    DEFINE_SIZE_STATIC (2);
  };

  hb_array_t<ଅଚଳ F2DOT14> get_all_tuples (unsigned axis_ଗଣନା) ଅଚଳ
  { ଫେରନ୍ତୁ StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).as_array ((has_peak () + has_intermediate () * 2) * axis_ଗଣନା); }
  ଅଚଳ F2DOT14* get_all_tuples_base (unsigned axis_ଗଣନା) ଅଚଳ
  { ଫେରନ୍ତୁ StructAfter<UnsizedArrayOf<F2DOT14>> (tupleIndex).arrayZ; }
  ଅଚଳ F2DOT14* get_peak_tuple (unsigned axis_ଗଣନା) ଅଚଳ
  { ଫେରନ୍ତୁ get_all_tuples_base (axis_ଗଣନା); }
  ଅଚଳ F2DOT14* get_start_tuple (unsigned axis_ଗଣନା) ଅଚଳ
  { ଫେରନ୍ତୁ get_all_tuples_base (axis_ଗଣନା) + has_peak () * axis_ଗଣନା; }
  ଅଚଳ F2DOT14* get_end_tuple (unsigned axis_ଗଣନା) ଅଚଳ
  { ଫେରନ୍ତୁ get_all_tuples_base (axis_ଗଣନା) + has_peak () * axis_ଗଣନା + axis_ଗଣନା; }

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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (4);
};

ସଂରଚନା optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<ତାର୍କିକ> opt_indices;
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> rounded_x_deltas;
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> rounded_y_deltas;
  hb_vector_t<ଦଶମିକ> opt_deltas_x;
  hb_vector_t<ଦଶମିକ> opt_deltas_y;
  hb_vector_t<unsigned ଅକ୍ଷର> opt_point_data;
  hb_vector_t<unsigned ଅକ୍ଷର> opt_deltas_data;
  hb_vector_t<unsigned ଅକ୍ଷର> point_data;
  hb_vector_t<unsigned ଅକ୍ଷର> deltas_data;
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> rounded_deltas;
};

ସଂରଚନା tuple_delta_t
{
  ସ୍ଥିର constexpr ତାର୍କିକ realloc_move = ସତ୍ୟ;  // Watch out when adding new members!

  ସର୍ବସାଧାରଣ:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<ତାର୍କିକ> indices;

  hb_vector_t<ଦଶମିକ> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<ଦଶମିକ> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned ଅକ୍ଷର> compiled_tuple_header;
  hb_vector_t<unsigned ଅକ୍ଷର> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (ଅଚଳ tuple_delta_t& o) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  tuple_delta_t& operator = (ଅଚଳ tuple_delta_t& o) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  friend ଶୂନ୍ୟ swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*ଏହି, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*ଏହି, o);
    ଫେରନ୍ତୁ *ଏହି;
  }

  ଶୂନ୍ୟ copy_from (ଅଚଳ tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  ଶୂନ୍ୟ remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  ତାର୍କିକ set_tent (hb_tag_t axis_tag, Triple tent)
  { ଫେରନ୍ତୁ axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (ଅଚଳ tuple_delta_t& o)
  {
    unsigned ଅଙ୍କ = indices.length;
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ; i++)
    {
      ଯଦି (indices.arrayZ[i])
      {
        ଯଦି (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          ଯଦି (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      ଅନ୍ୟଥା
      {
        ଯଦି (!o.indices.arrayZ[i]) ଜାରି;
        indices.arrayZ[i] = ସତ୍ୟ;
        deltas_x[i] = o.deltas_x[i];
        ଯଦି (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    ଫେରନ୍ତୁ *ଏହି;
  }

  tuple_delta_t& operator *= (ଦଶମିକ scalar)
  {
    ଯଦି (scalar == 1.0f)
      ଫେରନ୍ତୁ *ଏହି;

    unsigned ଅଙ୍କ = indices.length;
    ଯଦି (deltas_y)
      ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ; i++)
      {
	ଯଦି (!indices.arrayZ[i]) ଜାରି;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    ଅନ୍ୟଥା
      ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ; i++)
      {
	ଯଦି (!indices.arrayZ[i]) ଜାରି;
	deltas_x[i] *= scalar;
      }
    ଫେରନ୍ତୁ *ଏହି;
  }

  ଶୂନ୍ୟ change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_result_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    ଯଦି (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*ଏହି));
      ଫେରନ୍ତୁ;
    }

    ଯଦି ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->ମଧ୍ୟ && tent->ମଧ୍ୟ <= tent->maximum))
      ଫେରନ୍ତୁ;

    ଯଦି (tent->ମଧ୍ୟ == 0.f)
    {
      out.push (std::move (*ଏହି));
      ଫେରନ୍ତୁ;
    }

    ଯଦି (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.ମଧ୍ୟ &&
          axis_limit.ମଧ୍ୟ <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      ଫେରନ୍ତୁ;

    rebase_tent_result_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    ପାଇଁ (unsigned i = 0; i < solutions.length; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      ଯଦି (i < solutions.length - 1)
	new_var.copy_from (*ଏହି, pool);
      ଅନ୍ୟଥା
	new_var = std::move (*ଏହି);

      ଯଦି (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      ଅନ୍ୟଥା
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  ତାର୍କିକ compile_coords (ଅଚଳ hb_map_t& axes_index_map,
		       ଅଚଳ hb_map_t& axes_old_index_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_ଗଣନା = axes_index_map.get_population ();
    ଯଦି (pool)
    {
      ଯଦି (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_ଗଣନା)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା ଯଦି (unlikely (!compiled_peak_coords.resize (cur_axis_ଗଣନା)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_ଗଣନା = axes_old_index_tag_map.get_population ();
    unsigned j = 0;
    ପାଇଁ (unsigned i = 0; i < orig_axis_ଗଣନା; i++)
    {
      ଯଦି (!axes_index_map.has (i))
        ଜାରି;

      hb_tag_t axis_tag = axes_old_index_tag_map.get (i);
      Triple *coords = nullptr;
      ଯଦି (axis_tuples.has (axis_tag, &coords))
      {
	ଦଶମିକ min_val = coords->minimum;
	ଦଶମିକ val = coords->ମଧ୍ୟ;
	ଦଶମିକ max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	ଯଦି (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  ଯଦି (!compiled_interm_coords)
	  {
	    ଯଦି (pool)
	    {
	      ଯଦି (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_ଗଣନା)))
		ଫେରନ୍ତୁ ମିଥ୍ୟା;
	    }
	    ଅନ୍ୟଥା ଯଦି (unlikely (!compiled_interm_coords.resize (2 * cur_axis_ଗଣନା)))
	      ଫେରନ୍ତୁ ମିଥ୍ୟା;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_ଗଣନା);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_ଗଣନା);

	    ପାଇଁ (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      ଯଦି (!peak) ଜାରି;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	ଯଦି (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    ଫେରନ୍ତୁ !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  ତାର୍କିକ compile_tuple_var_header (ଅଚଳ hb_map_t& axes_index_map,
                                 unsigned points_data_length,
                                 ଅଚଳ hb_map_t& axes_old_index_tag_map,
                                 ଅଚଳ hb_hashmap_t<ଅଚଳ hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    ଯଦି (!compiled_deltas) ଫେରନ୍ତୁ ସତ୍ୟ;

    unsigned cur_axis_ଗଣନା = axes_index_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_ଗଣନା * (F2DOT14::static_size) + 4;
    ଯଦି (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, ମିଥ୍ୟା))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    ଯଦି (!shared_tuples_idx_map)
      compile_coords (axes_index_map, axes_old_index_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_ଗଣନା = 0;
    unsigned *shared_tuple_idx;
    ଯଦି (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    ଅନ୍ୟଥା
    {
      peak_ଗଣନା = encode_peak_coords(coords, flag);
      ଯଦି (!peak_ଗଣନା) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_ଗଣନା = encode_interm_coords (coords.sub_array (peak_ଗଣନା), flag);

    /* pointdata length = 0 implies "use shared points" */
    ଯଦି (points_data_length)
      flag |= TupleVariationHeader::TupleIndex::PrivatePointNumbers;

    unsigned serialized_data_size = points_data_length + compiled_deltas.length;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->varDataSize = serialized_data_size;
    o->tupleIndex = flag;

    unsigned total_header_len = 4 + (peak_ଗଣନା + interim_ଗଣନା) * (F2DOT14::static_size);
    compiled_tuple_header.shrink_back_to_pool (pool, total_header_len);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) ଅଚଳ
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.length * ଆକାର (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::TupleIndex::EmbeddedPeakTuple;
    ଫେରନ୍ତୁ compiled_peak_coords.length;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) ଅଚଳ
  {
    ଯଦି (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.length * ଆକାର (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::TupleIndex::IntermediateRegion;
    }
    ଫେରନ୍ତୁ compiled_interm_coords.length;
  }

  ତାର୍କିକ compile_deltas (hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { ଫେରନ୍ତୁ compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ସ୍ଥିର ତାର୍କିକ compile_deltas (hb_array_t<ଅଚଳ ତାର୍କିକ> point_indices,
			      hb_array_t<ଅଚଳ ଦଶମିକ> x_deltas,
			      hb_array_t<ଅଚଳ ଦଶମିକ> y_deltas,
			      hb_vector_t<unsigned ଅକ୍ଷର> &compiled_deltas, /* OUT */
			      hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    ଯଦି (unlikely (!rounded_deltas.resize_dirty  (point_indices.length)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned j = 0;
    ପାଇଁ (unsigned i = 0; i < point_indices.length; i++)
    {
      ଯଦି (!point_indices[i]) ଜାରି;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf ((ଦ୍ୱିଗୁଣ) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    ଯଦି (!rounded_deltas) ଫେରନ୍ତୁ ସତ୍ୟ;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.length + (rounded_deltas.length + 63) / 64;
    ଯଦି (y_deltas)
      alloc_len *= 2;

    ଯଦି (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, ମିଥ୍ୟା))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    ଯଦି (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      ପାଇଁ (unsigned idx = 0; idx < point_indices.length; idx++)
      {
        ଯଦି (!point_indices[idx]) ଜାରି;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା rounded_delta = hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf ((ଦ୍ୱିଗୁଣ) y_deltas.arrayZ[idx]));

        ଯଦି (j >= rounded_deltas.length) ଫେରନ୍ତୁ ମିଥ୍ୟା;

        rounded_deltas[j++] = rounded_delta;
      }

      ଯଦି (j != rounded_deltas.length) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସ୍ଥିର unsigned compile_deltas (hb_array_t<unsigned ଅକ୍ଷର> encoded_bytes,
				  hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> deltas)
  {
    ଫେରନ୍ତୁ Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::compile_unsafe (deltas, encoded_bytes);
  }

  ତାର୍କିକ calc_inferred_deltas (ଅଚଳ contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_ଗଣନା = orig_points.length;
    ଯଦି (point_ଗଣନା != indices.length)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned ref_ଗଣନା_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    ପାଇଁ (unsigned i = 0; i < point_ଗଣନା; i++)
    {
      ref_ଗଣନା_2 += indices.arrayZ[i];
      ଯଦି (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    ଯଦି (ref_ଗଣନା_2 == point_ଗଣନା)
      ଫେରନ୍ତୁ ସତ୍ୟ;
    ଯଦି (unlikely (end_points.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    ପାଇଁ (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_ଗଣନା = 0;
      ପାଇଁ (unsigned i = start_point; i < end_point + 1; i++)
        unref_ଗଣନା += indices.arrayZ[i];
      unref_ଗଣନା = (end_point - start_point + 1) - unref_ଗଣନା;

      unsigned j = start_point;
      ଯଦି (unref_ଗଣନା == 0 || unref_ଗଣନା > end_point - start_point)
        ଯାଅ no_more_gaps;
      ପାଇଁ (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev, next, i;
        ପାଇଁ (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          ଯଦି (indices.arrayZ[i] && !indices.arrayZ[j]) ଭାଙ୍ଗ;
        }
        prev = j = i;
        ପାଇଁ (;;)
        {
          i = j;
          j = next_index (i, start_point, end_point);
          ଯଦି (!indices.arrayZ[i] && indices.arrayZ[j]) ଭାଙ୍ଗ;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        ପାଇଁ (;;)
        {
          i = next_index (i, start_point, end_point);
          ଯଦି (i == next) ଭାଙ୍ଗ;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          ଯଦି (--unref_ଗଣନା == 0) ଯାଅ no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    ପାଇଁ (unsigned i = 0; i < point_ଗଣନା; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      ଯଦି ( !indices[i])
      {
        ଯଦି (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = ସତ୍ୟ;
      }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ optimize (ଅଚଳ contour_point_vector_t& contour_points,
                 ତାର୍କିକ is_composite,
		 optimize_scratch_t &scratch,
                 ଦ୍ୱିଗୁଣ tolerance = 0.5 + 1e-10)
  {
    unsigned ଗଣନା_2 = contour_points.length;
    ଯଦି (deltas_x.length != ଗଣନା_2 ||
        deltas_y.length != ଗଣନା_2)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_vector_t<ତାର୍କିକ> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &rounded_y_deltas = scratch.rounded_y_deltas;

    ଯଦି (unlikely (!rounded_x_deltas.resize_dirty  (ଗଣନା_2) ||
                  !rounded_y_deltas.resize_dirty  (ଗଣନା_2)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf ((ଦ୍ୱିଗୁଣ) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf ((ଦ୍ୱିଗୁଣ) deltas_y.arrayZ[i]));
    }

    ଯଦି (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned ref_ଗଣନା_2 = 0;
    ପାଇଁ (ତାର୍କିକ ref_flag : opt_indices)
       ref_ଗଣନା_2 += ref_flag;

    ଯଦି (ref_ଗଣନା_2 == ଗଣନା_2) ଫେରନ୍ତୁ ସତ୍ୟ;

    hb_vector_t<ଦଶମିକ> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<ଦଶମିକ> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    ତାର୍କିକ is_comp_glyph_wo_deltas = (is_composite && ref_ଗଣନା_2 == 0);
    ଯଦି (is_comp_glyph_wo_deltas)
    {
      ଯଦି (unlikely (!opt_deltas_x.resize (ଗଣନା_2) ||
                    !opt_deltas_y.resize (ଗଣନା_2)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      opt_indices.arrayZ[0] = ସତ୍ୟ;
      ପାଇଁ (unsigned i = 1; i < ଗଣନା_2; i++)
        opt_indices.arrayZ[i] = ମିଥ୍ୟା;
    }

    hb_vector_t<unsigned ଅକ୍ଷର> &opt_point_data = scratch.opt_point_data.reset ();
    ଯଦି (!compile_point_set (opt_indices, opt_point_data))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_vector_t<unsigned ଅକ୍ଷର> &opt_deltas_data = scratch.opt_deltas_data.reset ();
    ଯଦି (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_data,
			 scratch.rounded_deltas))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_vector_t<unsigned ଅକ୍ଷର> &point_data = scratch.point_data.reset ();
    ଯଦି (!compile_point_set (indices, point_data))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_vector_t<unsigned ଅକ୍ଷର> &deltas_data = scratch.deltas_data.reset ();
    ଯଦି (!compile_deltas (indices, deltas_x, deltas_y, deltas_data, scratch.rounded_deltas))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (opt_point_data.length + opt_deltas_data.length < point_data.length + deltas_data.length)
    {
      indices = std::move (opt_indices);

      ଯଦି (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    ଫେରନ୍ତୁ !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  ସ୍ଥିର ତାର୍କିକ compile_point_set (ଅଚଳ hb_vector_t<ତାର୍କିକ> &point_indices,
                                 hb_vector_t<unsigned ଅକ୍ଷର>& compiled_points /* OUT */)
  {
    unsigned ଅଙ୍କ_points = 0;
    ପାଇଁ (ତାର୍କିକ i : point_indices)
      ଯଦି (i) ଅଙ୍କ_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    ଯଦି (!ଅଙ୍କ_points) ଫେରନ୍ତୁ ସତ୍ୟ;

    unsigned indices_length = point_indices.length;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    ଯଦି (ଅଙ୍କ_points == indices_length)
      ଫେରନ୍ତୁ compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned ଅଙ୍କ_bytes = 2 + 3 *ଅଙ୍କ_points;
    ଯଦି (unlikely (!compiled_points.resize_dirty  (ଅଙ୍କ_bytes)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    ଯଦି (ଅଙ୍କ_points < 0x80)
      compiled_points.arrayZ[pos++] = ଅଙ୍କ_points;
    ଅନ୍ୟଥା
    {
      compiled_points.arrayZ[pos++] = ((ଅଙ୍କ_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = ଅଙ୍କ_points & 0xFF;
    }

    ଅଚଳ unsigned max_run_length = 0x7F;
    unsigned i = 0;
    unsigned last_ମୂଲ୍ୟ = 0;
    unsigned ଅଙ୍କ_encoded = 0;
    ଯେତେବେଳେ (i < indices_length && ଅଙ୍କ_encoded < ଅଙ୍କ_points)
    {
      unsigned run_length = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      ତାର୍କିକ use_byte_encoding = ମିଥ୍ୟା;
      ତାର୍କିକ new_run = ସତ୍ୟ;
      ଯେତେବେଳେ (i < indices_length && ଅଙ୍କ_encoded < ଅଙ୍କ_points &&
             run_length <= max_run_length)
      {
        // find out next referenced point index
        ଯେତେବେଳେ (i < indices_length && !point_indices[i])
          i++;

        ଯଦି (i >= indices_length) ଭାଙ୍ଗ;

        unsigned cur_ମୂଲ୍ୟ = i;
        unsigned delta = cur_ମୂଲ୍ୟ - last_ମୂଲ୍ୟ;

        ଯଦି (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = ମିଥ୍ୟା;
        }

        ଯଦି (use_byte_encoding && delta > 0xFF)
          ଭାଙ୍ଗ;

        ଯଦି (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        ଅନ୍ୟଥା
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_ମୂଲ୍ୟ = cur_ମୂଲ୍ୟ;
        run_length++;
        ଅଙ୍କ_encoded++;
      }

      ଯଦି (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_length - 1;
      ଅନ୍ୟଥା
        compiled_points.arrayZ[header_pos] = (run_length - 1) | 0x80;
    }
    ଫେରନ୍ତୁ compiled_points.resize_dirty  (pos);
  }

  ସ୍ଥିର ଦଶମିକ infer_delta (ଦଶମିକ target_val, ଦଶମିକ prev_val, ଦଶମିକ next_val, ଦଶମିକ prev_delta, ଦଶମିକ next_delta)
  {
    ଯଦି (prev_val == next_val)
      ଫେରନ୍ତୁ (prev_delta == next_delta) ? prev_delta : 0.f;
    ଅନ୍ୟଥା ଯଦି (target_val <= hb_min (prev_val, next_val))
      ଫେରନ୍ତୁ (prev_val < next_val) ? prev_delta : next_delta;
    ଅନ୍ୟଥା ଯଦି (target_val >= hb_max (prev_val, next_val))
      ଫେରନ୍ତୁ (prev_val > next_val) ? prev_delta : next_delta;

    ଦଶମିକ r = (target_val - prev_val) / (next_val - prev_val);
    ଫେରନ୍ତୁ prev_delta + r * (next_delta - prev_delta);
  }

  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_index (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end)
  { ଫେରନ୍ତୁ (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
ସଂରଚନା TupleVariationData
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (ଏହି));
  }

  size_t get_size (unsigned axis_ଗଣନା_times_2) ଅଚଳ
  {
    size_t total_size = min_size;
    unsigned ଗଣନା_2 = tuple_Var_ଗଣନା.get_ଗଣନା ();
    ଅଚଳ TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      total_size = hb_unsigned_add_saturate (total_size,
					     tuple_var_header->get_size (axis_ଗଣନା_times_2),
					     (size_t) tuple_var_header->get_data_size ());
      tuple_var_header = &tuple_var_header->get_next (axis_ଗଣନା_times_2);
    }

    ଫେରନ୍ତୁ total_size;
  }

  ଅଚଳ TupleVariationHeader &get_tuple_var_header (ଶୂନ୍ୟ) ଅଚଳ
  { ଫେରନ୍ତୁ StructAfter<TupleVariationHeader> (data); }

  ସଂରଚନା tuple_iterator_t;
  ସଂରଚନା tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    ବ୍ୟକ୍ତିଗତ:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ଅଚଳ hb_vector_t<ତାର୍କିକ>*, hb_vector_t<unsigned ଅକ୍ଷର>> point_data_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ଅଚଳ hb_vector_t<ତାର୍କିକ>*, unsigned> point_set_ଗଣନା_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned ଅକ୍ଷର> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_size = 0;
    ତାର୍କିକ needs_padding = ମିଥ୍ୟା;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    ତାର୍କିକ is_composite = ମିଥ୍ୟା;

    ସର୍ବସାଧାରଣ:
    tuple_variations_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    tuple_variations_t (ଅଚଳ tuple_variations_t&) = delete;
    tuple_variations_t& operator=(ଅଚଳ tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    tuple_variations_t& operator=(tuple_variations_t&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    ~tuple_variations_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

    explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ ତାର୍କିକ (tuple_vars); }
    unsigned get_var_ଗଣନା () ଅଚଳ
    {
      unsigned ଗଣନା_2 = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      ପାଇଁ (auto& tuple: tuple_vars)
        ଯଦି (tuple.compiled_deltas) ଗଣନା_2++;

      ଯଦି (shared_points_bytes && shared_points_bytes->length)
        ଗଣନା_2 |= Tuple_Var_ଗଣନା::SharedPointNumbers;
      ଫେରନ୍ତୁ ଗଣନା_2;
    }

    unsigned get_compiled_byte_size () ଅଚଳ
    { ଫେରନ୍ତୁ compiled_byte_size; }

    ତାର୍କିକ create_from_tuple_var_data (tuple_iterator_t iterator,
                                     unsigned tuple_var_ଗଣନା,
                                     unsigned point_ଗଣନା,
                                     ତାର୍କିକ is_gvar,
                                     ଅଚଳ hb_map_t *axes_old_index_tag_map,
                                     ଅଚଳ hb_vector_t<unsigned> &shared_indices,
                                     ଅଚଳ hb_array_t<ଅଚଳ F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     ତାର୍କିକ is_composite_glyph = ମିଥ୍ୟା)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> deltas_x;
      hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> deltas_y;
      କର
      {
        ଅଚଳ HBUINT8 *p = iterator.get_serialized_data ();
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = iterator.current_tuple->get_data_size ();
        ଯଦି (unlikely (!iterator.var_data_bytes.check_range (p, length)))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        ଯଦି (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_ଗଣନା (), shared_tuples, axes_old_index_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

	private_indices.reset ();
        ତାର୍କିକ has_private_points = iterator.current_tuple->has_private_points ();
        ଅଚଳ HBUINT8 *end = p + length;
        ଯଦି (has_private_points &&
            !TupleVariationData::decompile_points (p, private_indices, end))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଅଚଳ hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        ତାର୍କିକ apply_to_all = (indices.length == 0);
        unsigned ଅଙ୍କ_deltas = apply_to_all ? point_ଗଣନା : indices.length;

        ଯଦି (unlikely (!deltas_x.resize_dirty  (ଅଙ୍କ_deltas) ||
                      !TupleVariationData::decompile_deltas (p, deltas_x, end)))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଯଦି (is_gvar)
        {
          ଯଦି (unlikely (!deltas_y.resize_dirty  (ଅଙ୍କ_deltas) ||
                        !TupleVariationData::decompile_deltas (p, deltas_y, end)))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        ଯଦି (unlikely (!var.indices.allocate_from_pool (pool, point_ଗଣନା) ||
                      !var.deltas_x.allocate_from_pool (pool, point_ଗଣନା, ମିଥ୍ୟା)))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଯଦି (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_ଗଣନା, ମିଥ୍ୟା)))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          ଯଦି (idx >= point_ଗଣନା) ଜାରି;
          var.indices[idx] = ସତ୍ୟ;
          var.deltas_x[idx] = deltas_x[i];
          ଯଦି (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } ଯେତେବେଳେ (iterator.move_to_next ());

      is_composite = is_composite_glyph;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ create_from_item_var_data (ଅଚଳ VarData &var_data,
                                    ଅଚଳ hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    ଅଚଳ hb_map_t& axes_old_index_tag_map,
                                    unsigned& item_ଗଣନା_2,
                                    ଅଚଳ hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      ଯଦି (&var_data == &Null (VarData))
        ଫେରନ୍ତୁ ସତ୍ୟ;

      unsigned ଅଙ୍କ_regions = var_data.get_region_index_ଗଣନା ();
      ଯଦି (!tuple_vars.alloc (ଅଙ୍କ_regions)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      item_ଗଣନା_2 = inner_map ? inner_map->get_population () : var_data.get_item_ଗଣନା ();
      ଯଦି (!item_ଗଣନା_2) ଫେରନ୍ତୁ ସତ୍ୟ;
      unsigned row_size = var_data.get_row_size ();
      ଅଚଳ HBUINT8 *delta_bytes = var_data.get_delta_bytes ();

      ପାଇଁ (unsigned r = 0; r < ଅଙ୍କ_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        ଯଦି (!tuple.deltas_x.resize_dirty  (item_ଗଣନା_2) ||
            !tuple.indices.resize_dirty  (item_ଗଣନା_2))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ପାଇଁ (unsigned i = 0; i < item_ଗଣନା_2; i++)
        {
          tuple.indices.arrayZ[i] = ସତ୍ୟ;
          tuple.deltas_x.arrayZ[i] = var_data.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_size);
        }

        unsigned region_index = var_data.get_region_index (r);
        ଯଦି (region_index >= regions.length) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        tuple.axis_tuples = regions.arrayZ[region_index];

        tuple_vars.push (std::move (tuple));
      }
      ଫେରନ୍ତୁ !tuple_vars.in_error ();
    }

    ବ୍ୟକ୍ତିଗତ:
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cmp_axis_tag (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
    {
      ଅଚଳ hb_tag_t *a = (ଅଚଳ hb_tag_t*) pa;
      ଅଚଳ hb_tag_t *b = (ଅଚଳ hb_tag_t*) pb;
      ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(*a) - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(*b);
    }

    ତାର୍କିକ change_tuple_variations_axis_limits (ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              ଅଚଳ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      ଯଦି (!axis_tags.alloc (normalized_axes_location.get_population ()))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ପାଇଁ (auto t : normalized_axes_location.ଚାବିଗୁଡ଼ିକ ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_result_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      ପାଇଁ (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        ଯଦି (!normalized_axes_location.has (axis_tag, &axis_limit))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        TripleDistances axis_triple_distances{1.f, 1.f};
        ଯଦି (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        ପାଇଁ (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          ଯଦି (!out) ଜାରି;

          unsigned new_len = new_vars.length + out.length;

          ଯଦି (unlikely (!new_vars.alloc (new_len, ମିଥ୍ୟା)))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

          ପାଇଁ (unsigned i = 0; i < out.length; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    ତାର୍କିକ merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      ଯଦି (unlikely (!new_vars.alloc (tuple_vars.length)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      hb_hashmap_t<ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      ପାଇଁ (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        ଯଦି (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          ଯଦି (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
          ଜାରି;
        }

        unsigned *idx;
        ଯଦି (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        ଅନ୍ୟଥା
        {
	  auto *new_var = new_vars.push ();
	  ଯଦି (unlikely (new_vars.in_error ()))
	    ଫେରନ୍ତୁ ମିଥ୍ୟା;
          hb_swap (*new_var, var);
          ଯଦି (unlikely (!m.set (&(new_var->axis_tuples), new_vars.length - 1)))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    ତାର୍କିକ compile_all_point_sets ()
    {
      ପାଇଁ (ଅଚଳ auto& tuple: tuple_vars)
      {
        ଅଚଳ hb_vector_t<ତାର୍କିକ>* points_set = &(tuple.indices);
        ଯଦି (point_data_map.has (points_set))
        {
          unsigned *ଗଣନା_2;
          ଯଦି (unlikely (!point_set_ଗଣନା_map.has (points_set, &ଗଣନା_2) ||
                        !point_set_ଗଣନା_map.set (points_set, (*ଗଣନା_2) + 1)))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
          ଜାରି;
        }

        hb_vector_t<unsigned ଅକ୍ଷର> compiled_point_data;
        ଯଦି (!tuple_delta_t::compile_point_set (*points_set, compiled_point_data))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଯଦି (!point_data_map.set (points_set, std::move (compiled_point_data)) ||
            !point_set_ଗଣନା_map.set (points_set, 1))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    /* find shared points set which saves most bytes */
    ଶୂନ୍ୟ find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      ପାଇଁ (ଅଚଳ auto& _ : point_data_map.iter_ref ())
      {
        ଅଚଳ hb_vector_t<ତାର୍କିକ>* points_set = _.first;
        unsigned data_length = _.second.length;
        ଯଦି (!data_length) ଜାରି;
        unsigned *ଗଣନା_2;
        ଯଦି (unlikely (!point_set_ଗଣନା_map.has (points_set, &ଗଣନା_2) ||
                      *ଗଣନା_2 <= 1))
        {
          shared_points_bytes = nullptr;
          ଫେରନ୍ତୁ;
        }

        unsigned saved_bytes = data_length * ((*ଗଣନା_2) -1);
        ଯଦି (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    ତାର୍କିକ calc_inferred_deltas (ଅଚଳ contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      ପାଇଁ (tuple_delta_t& var : tuple_vars)
        ଯଦି (!var.calc_inferred_deltas (contour_points, scratch))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ iup_optimize (ଅଚଳ contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      ପାଇଁ (tuple_delta_t& var : tuple_vars)
      {
        ଯଦି (!var.optimize (contour_points, is_composite, scratch))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ସର୍ବସାଧାରଣ:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    ଶୂନ୍ୟ cull_unreachable (ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      ପାଇଁ (unsigned i = 0; i < tuple_vars.length; i++)
      {
	ଯଦି (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  ଜାରି;
	ଯଦି (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    ତାର୍କିକ instantiate (ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      ଅଚଳ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      ତାର୍କିକ optimize = ମିଥ୍ୟା)
    {
      ଯଦି (!tuple_vars) ଫେରନ୍ତୁ ସତ୍ୟ;
      ଯଦି (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      /* compute inferred deltas only for gvar */
      ଯଦି (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	ଯଦି (!calc_inferred_deltas (*contour_points, scratch))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      /* if iup delta opt is on, contour_points can't be null */
      ଯଦି (optimize && !contour_points)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (!merge_tuple_variations (optimize ? contour_points : nullptr))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (optimize && !iup_optimize (*contour_points, scratch)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଫେରନ୍ତୁ !tuple_vars.in_error ();
    }

    ତାର୍କିକ compile_bytes (ଅଚଳ hb_map_t& axes_index_map,
                        ଅଚଳ hb_map_t& axes_old_index_tag_map,
                        ତାର୍କିକ use_shared_points,
                        ତାର୍କିକ is_gvar = ମିଥ୍ୟା,
                        ଅଚଳ hb_hashmap_t<ଅଚଳ hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      ଯଦି (!tuple_vars)
        ଫେରନ୍ତୁ ସତ୍ୟ;

      // compile points set and store data in hashmap
      ଯଦି (!compile_all_point_sets ())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_size += 4;

      ଯଦି (use_shared_points)
      {
        find_shared_points ();
        ଯଦି (shared_points_bytes)
          compiled_byte_size += shared_points_bytes->length;
      }
      hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      ପାଇଁ (auto& tuple: tuple_vars)
      {
        ଅଚଳ hb_vector_t<ତାର୍କିକ>* points_set = &(tuple.indices);
        hb_vector_t<unsigned ଅକ୍ଷର> *points_data;
        ଯଦି (unlikely (!point_data_map.has (points_set, &points_data)))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        ଯଦି (!points_data->length)
          ଜାରି;
        ଯଦି (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        unsigned points_data_length = (points_data != shared_points_bytes) ? points_data->length : 0;
        ଯଦି (!tuple.compile_tuple_var_header (axes_index_map, points_data_length, axes_old_index_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        compiled_byte_size += tuple.compiled_tuple_header.length + points_data_length + tuple.compiled_deltas.length;
      }

      ଯଦି (is_gvar && (compiled_byte_size % 2))
      {
        needs_padding = ସତ୍ୟ;
        compiled_byte_size += 1;
      }

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ serialize_var_headers (hb_serialize_context_t *c, unsigned& total_header_len) ଅଚଳ
    {
      TRACE_SERIALIZE (ଏହି);
      ପାଇଁ (ଅଚଳ auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        ଯଦି (c->in_error ()) return_trace (ମିଥ୍ୟା);
        total_header_len += tuple.compiled_tuple_header.length;
      }
      return_trace (ସତ୍ୟ);
    }

    ତାର୍କିକ serialize_var_data (hb_serialize_context_t *c, ତାର୍କିକ is_gvar) ଅଚଳ
    {
      TRACE_SERIALIZE (ଏହି);
      ଯଦି (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->length);
        s.copy (c);
      }

      ପାଇଁ (ଅଚଳ auto& tuple: tuple_vars)
      {
        ଅଚଳ hb_vector_t<ତାର୍କିକ>* points_set = &(tuple.indices);
        hb_vector_t<unsigned ଅକ୍ଷର> *point_data;
        ଯଦି (!point_data_map.has (points_set, &point_data))
          return_trace (ମିଥ୍ୟା);

        ଯଦି (!is_gvar || point_data != shared_points_bytes)
        {
          hb_ubytes_t s (point_data->arrayZ, point_data->length);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        ଯଦି (c->in_error ()) return_trace (ମିଥ୍ୟା);
      }

      /* padding for gvar */
      ଯଦି (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        ଯଦି (!c->embed (pad)) return_trace (ମିଥ୍ୟା);
      }
      return_trace (ସତ୍ୟ);
    }
  };

  ସଂରଚନା tuple_iterator_t
  {
    unsigned get_axis_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ axis_ଗଣନା; }

    ଶୂନ୍ୟ init (hb_bytes_t var_data_bytes_, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_ଗଣନା_3, ଅଚଳ ଶୂନ୍ୟ *table_base_)
    {
      var_data_bytes = var_data_bytes_;
      var_data = var_data_bytes_.as<TupleVariationData> ();
      tuples_ବାମ = var_data->tuple_Var_ଗଣନା.get_ଗଣନା ();
      axis_ଗଣନା = axis_ଗଣନା_3;
      axis_ଗଣନା_times_2 = axis_ଗଣନା_3 * 2;
      current_tuple = &var_data->get_tuple_var_header ();
      data_offset = 0;
      table_base = table_base_;
    }

    ତାର୍କିକ get_shared_indices (hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &shared_indices /* OUT */)
    {
      ଯଦି (var_data->has_shared_point_numbers ())
      {
        ଅଚଳ HBUINT8 *base = &(table_base+var_data->data);
        ଅଚଳ HBUINT8 *p = base;
        ଯଦି (!decompile_points (p, shared_indices, (ଅଚଳ HBUINT8 *) (var_data_bytes.arrayZ + var_data_bytes.length))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        data_offset = p - base;
      }
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ is_valid ()
    {
      ଯଦି (unlikely (tuples_ବାମ <= 0))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      current_tuple_size = TupleVariationHeader::min_size;
      ଯଦି (unlikely (!var_data_bytes.check_end ((ଅଚଳ ଅକ୍ଷର *) current_tuple + current_tuple_size)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      current_tuple_size = current_tuple->get_size (axis_ଗଣନା_times_2);
      ଯଦି (unlikely (!var_data_bytes.check_end ((ଅଚଳ ଅକ୍ଷର *) current_tuple + current_tuple_size)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    HB_ALWAYS_INLINE
    ତାର୍କିକ move_to_next ()
    {
      data_offset += current_tuple->get_data_size ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_size);
      tuples_ବାମ--;
      ଫେରନ୍ତୁ is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ଅଚଳ HBUINT8 *get_serialized_data () ଅଚଳ
    { ଫେରନ୍ତୁ &(table_base+var_data->data) + data_offset; }

    ବ୍ୟକ୍ତିଗତ:
    signed tuples_ବାମ;
    ଅଚଳ TupleVariationData *var_data;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_ଗଣନା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis_ଗଣନା_times_2;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା data_offset;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_tuple_size;
    ଅଚଳ ଶୂନ୍ୟ *table_base;

    ସର୍ବସାଧାରଣ:
    hb_bytes_t var_data_bytes;
    ଅଚଳ TupleVariationHeader *current_tuple;
  };

  ସ୍ଥିର ତାର୍କିକ get_tuple_iterator (hb_bytes_t var_data_bytes, unsigned axis_ଗଣନା,
                                  ଅଚଳ ଶୂନ୍ୟ *table_base,
                                  hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_data_bytes, axis_ଗଣନା, table_base);
    ଯଦି (!iterator->get_shared_indices (shared_indices))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ iterator->is_valid ();
  }

  ତାର୍କିକ has_shared_point_numbers () ଅଚଳ { ଫେରନ୍ତୁ tuple_Var_ଗଣନା.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  ତାର୍କିକ cull_tuple_variations (hb_bytes_t var_data_bytes,
			      unsigned axis_ଗଣନା,
			      hb_array_t<ଅଚଳ F2DOT14> shared_tuples,
			      ଅଚଳ hb_map_t *axes_old_index_tag_map,
			      ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<ଅକ୍ଷର> &out /* OUT */,
			      ତାର୍କିକ *changed /* OUT */) ଅଚଳ
  {
    *changed = ମିଥ୍ୟା;

    hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> shared_indices;
    tuple_iterator_t iterator;
    ଯଦି (!get_tuple_iterator (var_data_bytes, axis_ଗଣନା, ଏହି,
			     shared_indices, &iterator))
      ଫେରନ୍ତୁ ସତ୍ୟ; /* no tuples or malformed: keep original */

    ଅଚଳ ଅକ୍ଷର *bytes_start = var_data_bytes.arrayZ;
    ଅଚଳ ଅକ୍ଷର *bytes_end = bytes_start + var_data_bytes.length;
    ଅଚଳ ଅକ୍ଷର *serialized_base = (ଅଚଳ ଅକ୍ଷର *) &(ଏହି+data);
    ଅଚଳ ଅକ୍ଷର *shared_points_end = (ଅଚଳ ଅକ୍ଷର *) iterator.get_serialized_data ();
    ଯଦି (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      ଫେରନ୍ତୁ ସତ୍ୟ;

    ସଂରଚନା kept_tuple_t
    {
      ଅଚଳ ଅକ୍ଷର *header;
      unsigned header_size;
      ଅଚଳ ଅକ୍ଷର *data;
      unsigned data_size;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned total = 0;
    unsigned kept_headers_size = 0, kept_data_size = 0;
    କର
    {
      ଅଚଳ TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_size = header->get_size (axis_ଗଣନା * 2);
      unsigned data_size = header->get_data_size ();
      ଅଚଳ ଅକ୍ଷର *tuple_data = (ଅଚଳ ଅକ୍ଷର *) iterator.get_serialized_data ();
      ଯଦି (unlikely (tuple_data < serialized_base ||
		    data_size > (unsigned) (bytes_end - tuple_data)))
	ଫେରନ୍ତୁ ସତ୍ୟ; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      ଯଦି (!header->unpack_axis_tuples (axis_ଗଣନା, shared_tuples,
				       axes_old_index_tag_map, axis_tuples))
	ଫେରନ୍ତୁ ସତ୍ୟ;

      total++;
      ଯଦି (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ଅଚଳ ଅକ୍ଷର *) header, header_size,
				 tuple_data, data_size});
	kept_headers_size += header_size;
	kept_data_size += data_size;
      }
    } ଯେତେବେଳେ (iterator.move_to_next ());

    ଯଦି (unlikely (kept.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (kept.length == total) ଫେରନ୍ତୁ ସତ୍ୟ; /* nothing to cull */

    *changed = ସତ୍ୟ;
    ଯଦି (!kept.length) ଫେରନ୍ତୁ ସତ୍ୟ; /* everything culled: no variation data */

    unsigned shared_points_size = shared_points_end - serialized_base;
    unsigned data_offset = min_size + kept_headers_size;
    unsigned new_size = data_offset + shared_points_size + kept_data_size;
    ଯଦି (unlikely (!out.resize (new_size)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    TupleVariationData *out_data = (TupleVariationData *) out.arrayZ;
    out_data->tuple_Var_ଗଣନା = (uint16_t) (kept.length |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_data->data = data_offset;

    ଅକ୍ଷର *p = out.arrayZ + min_size;
    ପାଇଁ (ଅଚଳ auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_size);
      p += t.header_size;
    }
    hb_memcpy (p, serialized_base, shared_points_size);
    p += shared_points_size;
    ପାଇଁ (ଅଚଳ auto &t : kept)
    {
      hb_memcpy (p, t.data, t.data_size);
      p += t.data_size;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସ୍ଥିର ତାର୍କିକ decompile_points (ଅଚଳ HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &points /* OUT */,
				ଅଚଳ HBUINT8 *end)
  {
    ଗଣନା packed_point_flag_t
    {
      POINTS_ARE_ଶବ୍ଦଗୁଡ଼ିକ     = 0x80,
      POINT_RUN_ଗଣନା_MASK = 0x7F
    };

    ଯଦି (unlikely (p + 1 > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned ଗଣନା_2 = *p++;
    ଯଦି (ଗଣନା_2 & POINTS_ARE_ଶବ୍ଦଗୁଡ଼ିକ)
    {
      ଯଦି (unlikely (p + 1 > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଗଣନା_2 = ((ଗଣନା_2 & POINT_RUN_ଗଣନା_MASK) << 8) | *p++;
    }
    ଯଦି (unlikely (!points.resize_dirty  (ଗଣନା_2))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned n = 0;
    unsigned i = 0;
    ଯେତେବେଳେ (i < ଗଣନା_2)
    {
      ଯଦି (unlikely (p + 1 > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      unsigned control = *p++;
      unsigned run_ଗଣନା = (control & POINT_RUN_ଗଣନା_MASK) + 1;
      unsigned stop = i + run_ଗଣନା;
      ଯଦି (unlikely (stop > ଗଣନା_2)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (control & POINTS_ARE_ଶବ୍ଦଗୁଡ଼ିକ)
      {
        ଯଦି (unlikely (p + run_ଗଣନା * HBUINT16::static_size > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ପାଇଁ (; i < stop; i++)
        {
          n += *(ଅଚଳ HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_size;
        }
      }
      ଅନ୍ୟଥା
      {
        ଯଦି (unlikely (p + run_ଗଣନା > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ପାଇଁ (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  ସ୍ଥିର ତାର୍କିକ decompile_deltas (ଅଚଳ HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				ଅଚଳ HBUINT8 *end,
				ତାର୍କିକ consume_all = ମିଥ୍ୟା,
				unsigned start = 0)
  {
    ଫେରନ୍ତୁ Tuple_ମୂଲ୍ୟଗୁଡ଼ିକ::decompile (p, deltas, end, consume_all, start);
  }

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ tuple_Var_ଗଣନା; }

  ତାର୍କିକ decompile_tuple_variations (unsigned point_ଗଣନା,
                                   ତାର୍କିକ is_gvar,
                                   tuple_iterator_t iterator,
                                   ଅଚଳ hb_map_t *axes_old_index_tag_map,
                                   ଅଚଳ hb_vector_t<unsigned> &shared_indices,
                                   ଅଚଳ hb_array_t<ଅଚଳ F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   ତାର୍କିକ is_composite_glyph = ମିଥ୍ୟା) ଅଚଳ
  {
    ଫେରନ୍ତୁ tuple_variations.create_from_tuple_var_data (iterator, tuple_Var_ଗଣନା,
                                                        point_ଗଣନା, is_gvar,
                                                        axes_old_index_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  ତାର୍କିକ is_gvar,
                  ଅଚଳ tuple_variations_t& tuple_variations) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    /* empty tuple variations, just return and skip serialization. */
    ଯଦି (!tuple_variations) return_trace (ସତ୍ୟ);

    auto *out = c->start_embed (ଏହି);
    ଯଦି (unlikely (!c->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ଯଦି (!c->check_assign (out->tuple_Var_ଗଣନା, tuple_variations.get_var_ଗଣନା (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ମିଥ୍ୟା);

    unsigned total_header_len = 0;

    ଯଦି (!tuple_variations.serialize_var_headers (c, total_header_len))
      return_trace (ମିଥ୍ୟା);

    unsigned data_offset = min_size + total_header_len;
    ଯଦି (!is_gvar) data_offset += 4;
    ଯଦି (!c->check_assign (out->data, data_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (ମିଥ୍ୟା);

    ଫେରନ୍ତୁ tuple_variations.serialize_var_data (c, is_gvar);
  }

  ସୁରକ୍ଷିତ:
  ସଂରଚନା Tuple_Var_ଗଣନା : HBUINT16
  {
    friend ସଂରଚନା tuple_variations_t;
    ତାର୍କିକ has_shared_point_numbers () ଅଚଳ { ଫେରନ୍ତୁ ((*ଏହି) & SharedPointNumbers); }
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ (*ଏହି) & ଗଣନା_Mask; }
    Tuple_Var_ଗଣନା& operator = (uint16_t i) { HBUINT16::operator= (i); ଫେରନ୍ତୁ *ଏହି; }
    explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ get_ଗଣନା (); }

    ସୁରକ୍ଷିତ:
    ଗଣନା Flags
    {
      SharedPointNumbers= 0x8000u,
      ଗଣନା_Mask         = 0x0FFFu
    };
    ସର୍ବସାଧାରଣ:
    DEFINE_SIZE_STATIC (2);
  };

  Tuple_Var_ଗଣନା tuple_Var_ଗଣନା;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                data;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (2 + OffType::static_size);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = TupleVariationData<HBUINT16>::tuple_variations_t;
ସଂରଚନା item_variations_t
{
  using region_t = ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>*;
  ବ୍ୟକ୍ତିଗତ:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_data_ଅଙ୍କ_rows;

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
  hb_vector_t<hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  ତାର୍କିକ has_long = ମିଥ୍ୟା;

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ has_long_word () ଅଚଳ
  { ଫେରନ୍ତୁ has_long; }

  ଅଚଳ hb_vector_t<region_t>& get_region_list () ଅଚଳ
  { ଫେରନ୍ତୁ region_list; }

  ଅଚଳ hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () ଅଚଳ
  { ଫେରନ୍ତୁ encodings; }

  ଅଚଳ hb_map_t& get_varidx_map () ଅଚଳ
  { ଫେରନ୍ତୁ varidx_map; }

  ତାର୍କିକ add_vardata_encoding_for_testing (hb_vector_t<ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*> &&rows,
                                         unsigned ଅଙ୍କ_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), ଅଙ୍କ_cols));
    ଫେରନ୍ତୁ !encodings.in_error ();
  }

  ତାର୍କିକ compile_varidx_map_for_testing (ଅଚଳ hb_hashmap_t<unsigned, ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*>& front_mapping)
  { ଫେରନ୍ତୁ compile_varidx_map (front_mapping); }

  ତାର୍କିକ instantiate (ଅଚଳ ItemVariationStore& varStore,
                    ଅଚଳ hb_subset_plan_t *plan,
                    ତାର୍କିକ optimize=ସତ୍ୟ,
                    ତାର୍କିକ use_no_variation_idx=ସତ୍ୟ,
                    ଅଚଳ hb_array_t <ଅଚଳ hb_inc_bimap_t> inner_maps = hb_array_t<ଅଚଳ hb_inc_bimap_t> (),
                    ତାର୍କିକ layout_conditions = ମିଥ୍ୟା)
  {
    ଯଦି (!create_from_item_varstore (varStore, plan->axes_old_index_tag_map, inner_maps))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    /* avar2 partial instancing: cull unreachable regions. */
    ଯଦି (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      ପାଇଁ (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    ଯଦି (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (layout_conditions &&
        (!instantiate_condition_ମୂଲ୍ୟଗୁଡ଼ିକ (plan, varStore) || !build_region_list ()))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ as_item_varstore (optimize, use_no_variation_idx);
  }

  ତାର୍କିକ instantiate_condition_ମୂଲ୍ୟଗୁଡ଼ିକ (ଅଚଳ hb_subset_plan_t *plan,
                                      ଅଚଳ ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_ମୂଲ୍ୟ_map;
    condition_map.clear ();
    ଯଦି (!plan->layout_condition_ମୂଲ୍ୟଗୁଡ଼ିକ) ଫେରନ୍ତୁ ସତ୍ୟ;
    using condition_ମୂଲ୍ୟ_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>;
    hb_vector_t<condition_ମୂଲ୍ୟ_t> conditions;
    ପାଇଁ (auto condition : plan->layout_condition_ମୂଲ୍ୟଗୁଡ଼ିକ.iter ())
      conditions.push (condition);
    ଯଦି (unlikely (conditions.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ଅଚଳ condition_ମୂଲ୍ୟ_t &a, ଅଚଳ condition_ମୂଲ୍ୟ_t &b) {
      ଯଦି (a.second.first != b.second.first)
        ଫେରନ୍ତୁ a.second.first < b.second.first ? -1 : 1;
      ଫେରନ୍ତୁ a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    ପାଇଁ (unsigned i = 0; i < plan->gdef_varstore_inner_maps.length; i++)
      ଯଦି (plan->gdef_varstore_inner_maps[i].get_population ())
        ଯଦି (!outer_map.set (i, new_outer++)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      ଦ୍ୱିଗୁଣ value = condition.second.second +
                     varStore.get_delta<ଦ୍ୱିଗୁଣ> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      ଯଦି (!outer_map.has (old_outer))
        ଜାରି;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      ଯଦି (outer >= vars.length || inner >= var_data_ଅଙ୍କ_rows[outer])
        ଜାରି;

      ଦ୍ୱିଗୁଣ maximum = fabs ((ଦ୍ୱିଗୁଣ) value);
      ତାର୍କିକ variable = ମିଥ୍ୟା;
      ପାଇଁ (ଅଚଳ tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        ଯଦି (unlikely (inner >= tuple.deltas_x.length)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଦଶମିକ delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((ଦ୍ୱିଗୁଣ) delta));
      }
      ଯଦି (!variable)
      {
        ଯଦି (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), ପୂର୍ଣ୍ଣସଂଖ୍ୟା (value > 0))))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଜାରି;
      }

      ଦ୍ୱିଗୁଣ scale = 1.;
      ଯେତେବେଳେ (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      ଯେତେବେଳେ (maximum * scale * 2 <= INT_MAX)
      {
        ତାର୍କିକ integral = (ଦ୍ୱିଗୁଣ) value * scale == round ((ଦ୍ୱିଗୁଣ) value * scale);
        ପାଇଁ (ଅଚଳ tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (ଦ୍ୱିଗୁଣ) tuple.deltas_x[inner] * scale ==
                      round ((ଦ୍ୱିଗୁଣ) tuple.deltas_x[inner] * scale);
        ଯଦି (integral) ଭାଙ୍ଗ;
        scale *= 2;
      }
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_ମୂଲ୍ୟ = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) round ((ଦ୍ୱିଗୁଣ) value * scale);
      ଯଦି (scale != 1. || default_ମୂଲ୍ୟ < -32768 || default_ମୂଲ୍ୟ > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        ପାଇଁ (ଅଚଳ tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          ଯଦି (!tuple.deltas_x[inner]) ଜାରି;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (ସତ୍ୟ);
          copy.deltas_x.push ((ଦଶମିକ) round ((ଦ୍ୱିଗୁଣ) tuple.deltas_x[inner] * scale));
          ଯଦି (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
          private_row.tuple_vars.push (std::move (copy));
        }
        ଯଦି (unlikely (vars.length >= 0xFFFFu)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଯଦି (unlikely (private_row.tuple_vars.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        idx = vars.length << 16;
        vars.push (std::move (private_row));
        var_data_ଅଙ୍କ_rows.push (1);
        ଯଦି (unlikely (vars.in_error () || var_data_ଅଙ୍କ_rows.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଯଦି (default_ମୂଲ୍ୟ < -32768 || default_ମୂଲ୍ୟ > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା bias = default_ମୂଲ୍ୟ - hb_clamp (default_ମୂଲ୍ୟ, -32768, 32767);
          default_ମୂଲ୍ୟ -= bias;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଉପର = bias / 65536 * 65536;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା parts[] = {ଉପର, bias - ଉପର};
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା part : parts)
          {
            ଯଦି (!part) ଜାରି;
            unsigned ଗଣନା_2 = vars[idx >> 16].tuple_vars.length;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            ଅଚଳ auto &tuples = vars[idx >> 16].tuple_vars;
            ଯଦି (unlikely (tuples.in_error () || tuples.length != ଗଣନା_2 + 1 ||
                          tuples.tail ().indices.in_error () ||
                          tuples.tail ().deltas_x.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
          }
        }
      }
      ଯଦି (!condition_map.set (condition.first, hb_pair (idx, default_ମୂଲ୍ୟ)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  ତାର୍କିକ create_from_item_varstore (ଅଚଳ ItemVariationStore& varStore,
                                  ଅଚଳ hb_map_t& axes_old_index_tag_map,
                                  ଅଚଳ hb_array_t <ଅଚଳ hb_inc_bimap_t> inner_maps = hb_array_t<ଅଚଳ hb_inc_bimap_t> ())
  {
    ଅଚଳ VarRegionList& regionList = varStore.get_region_list ();
    ଯଦି (!regionList.get_var_regions (axes_old_index_tag_map, orig_region_list))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned ଅଙ୍କ_var_data = varStore.get_sub_table_ଗଣନା ();
    ଯଦି (inner_maps && inner_maps.length != ଅଙ୍କ_var_data) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!vars.alloc (ଅଙ୍କ_var_data) ||
        !var_data_ଅଙ୍କ_rows.alloc (ଅଙ୍କ_var_data)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_var_data; i++)
    {
      ଯଦି (inner_maps && !inner_maps.arrayZ[i].get_population ())
          ଜାରି;
      tuple_variations_t var_data_tuples;
      unsigned item_ଗଣନା_2 = 0;
      ଯଦି (!var_data_tuples.create_from_item_var_data (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_index_tag_map,
                                                      item_ଗଣନା_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      var_data_ଅଙ୍କ_rows.push (item_ଗଣନା_2);
      vars.push (std::move (var_data_tuples));
    }
    ଫେରନ୍ତୁ !vars.in_error () && !var_data_ଅଙ୍କ_rows.in_error () && vars.length == var_data_ଅଙ୍କ_rows.length;
  }

  ତାର୍କିକ instantiate_tuple_vars (ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               ଅଚଳ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               ତାର୍କିକ build_regions = ସତ୍ୟ)
  {
    optimize_scratch_t scratch;
    ପାଇଁ (tuple_variations_t& tuple_vars : vars)
      ଯଦି (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_ଗଣନା_2)
  {
    vars.push (tuple_variations_t ());
    var_data_ଅଙ୍କ_rows.push (item_ଗଣନା_2);
    ଫେରନ୍ତୁ vars.length - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_ଗଣନା (unsigned outer) ଅଚଳ
  { ଫେରନ୍ତୁ outer < var_data_ଅଙ୍କ_rows.length ? var_data_ଅଙ୍କ_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    ଯଦି (unlikely (outer >= vars.length ||
                  inner >= var_data_ଅଙ୍କ_rows[outer] ||
                  var_data_ଅଙ୍କ_rows[outer] >= 0xFFFFu))
      ଫେରନ୍ତୁ (unsigned) -1;
    ପାଇଁ (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      ଯଦି (unlikely (inner >= tuple.deltas_x.length ||
                    inner >= tuple.indices.length))
        ଫେରନ୍ତୁ (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      ଯଦି (inner < tuple.deltas_y.length)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      ଯଦି (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        ଫେରନ୍ତୁ (unsigned) -1;
    }
    ଫେରନ୍ତୁ var_data_ଅଙ୍କ_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  ଶୂନ୍ୟ add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta, unsigned item_ଗଣନା_2)
  {
    ଯଦି (outer >= vars.length) ଫେରନ୍ତୁ;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    ଯଦି (!tuple.indices.resize (item_ଗଣନା_2) ||
        !tuple.deltas_x.resize (item_ଗଣନା_2)) ଫେରନ୍ତୁ;
    ପାଇଁ (unsigned i = 0; i < item_ଗଣନା_2; i++)
    {
      tuple.indices.arrayZ[i] = ସତ୍ୟ;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (ଦଶମିକ) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  ତାର୍କିକ build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    ପାଇଁ (ଅଚଳ tuple_variations_t& sub_table : vars)
    {
      ପାଇଁ (ଅଚଳ tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        ଯଦି (!used_regions.has (r))
        {
          ତାର୍କିକ all_zeros = ସତ୍ୟ;
          ପାଇଁ (ଦଶମିକ d : tuple.deltas_x)
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf ((ଦ୍ୱିଗୁଣ) d));
            ଯଦି (delta != 0)
            {
              all_zeros = ମିଥ୍ୟା;
              ଭାଙ୍ଗ;
            }
          }
          ଯଦି (!all_zeros)
          {
            ଯଦି (!used_regions.set (r, 1))
              ଫେରନ୍ତୁ ମିଥ୍ୟା;
          }
        }
        ଯଦି (all_regions.has (r))
          ଜାରି;
        ଯଦି (!all_regions.set (r, 1))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    ଯଦି (!all_regions || !all_unique_regions) ଫେରନ୍ତୁ ସତ୍ୟ;

    ଯଦି (!region_list.alloc (all_regions.get_population ()))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    ପାଇଁ (ଅଚଳ auto& r : orig_region_list)
    {
      ଯଦି (!all_regions.has (&r) || !used_regions.has (&r))
        ଜାରି;

      region_list.push (&r);
      ଯଦି (!region_map.set (&r, idx))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    ପାଇଁ (ଅଚଳ auto& r: all_unique_regions)
    {
      ଯଦି (!all_regions.has (r) || !used_regions.has (r))
        ଜାରି;
      region_list.push (r);
      ଯଦି (!region_map.set (r, idx))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      all_regions.del (r);
      idx++;
    }
    ଫେରନ୍ତୁ (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  ସଂରଚନା combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      ପ୍ରତିଜ୍ଞା (gain < 0xFFFFFF);
      ପ୍ରତିଜ୍ଞା (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    ତାର୍କିକ operator < (ଅଚଳ combined_gain_idx_tuple_t& o)
    {
      ଫେରନ୍ତୁ encoded < o.encoded;
    }

    ତାର୍କିକ operator <= (ଅଚଳ combined_gain_idx_tuple_t& o)
    {
      ଫେରନ୍ତୁ encoded <= o.encoded;
    }

    unsigned idx_1 () ଅଚଳ { ଫେରନ୍ତୁ (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () ଅଚଳ { ଫେରନ୍ତୁ encoded & 0xFFFFF; };
  };

  ତାର୍କିକ as_item_varstore (ତାର୍କିକ optimize=ସତ୍ୟ, ତାର୍କିକ use_no_variation_idx=ସତ୍ୟ)
  {
    /* return true if no variation data */
    ଯଦି (!region_list) ଫେରନ୍ତୁ ସତ୍ୟ;
    unsigned ଅଙ୍କ_cols = region_list.length;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned total_rows = 0;
    ପାଇଁ (unsigned major = 0; major < var_data_ଅଙ୍କ_rows.length; major++)
      total_rows += var_data_ଅଙ୍କ_rows[major];

    ଯଦି (!delta_rows.resize (total_rows)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    /* init all rows to [0]*num_cols */
    ପାଇଁ (unsigned i = 0; i < total_rows; i++)
      ଯଦି (!(delta_rows[i].resize (ଅଙ୍କ_cols))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *> major_rows;
    hb_hashmap_t<ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*, unsigned> delta_rows_map;
    ପାଇଁ (unsigned major = 0; major < vars.length; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ଅଚଳ tuple_variations_t& tuples = vars[major];
      unsigned ଅଙ୍କ_rows = var_data_ଅଙ୍କ_rows[major];

      ଯଦି (!ଅଙ୍କ_rows) ଜାରି;

      ପାଇଁ (ଅଚଳ tuple_delta_t& tuple: tuples.tuple_vars)
      {
        ଯଦି (tuple.deltas_x.length != ଅଙ୍କ_rows)
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        /* skip unused regions */
        unsigned *col_idx;
        ଯଦି (!region_map.has (&(tuple.axis_tuples), &col_idx))
          ଜାରି;

        ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_rows; i++)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା rounded_delta = hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (roundf ((ଦ୍ୱିଗୁଣ) tuple.deltas_x[i]));
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          ଯଦି (unlikely (sum < INT_MIN || sum > INT_MAX)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
          delta = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      ପାଇଁ (unsigned minor = 0; minor < ଅଙ୍କ_rows; minor++)
      {
	ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& row = delta_rows[start_row + minor];
	ଯଦି (use_no_variation_idx)
	{
	  ତାର୍କିକ all_zeros = ସତ୍ୟ;
	  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta : row)
	  {
	    ଯଦି (delta != 0)
	    {
	      all_zeros = ମିଥ୍ୟା;
	      ଭାଙ୍ଗ;
	    }
	  }
	  ଯଦି (all_zeros)
	    ଜାରି;
	}

	ଯଦି (!front_mapping.set ((major<<16) + minor, &row))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	ଯଦି (optimize)
	{
	  ଯଦି (delta_rows_map.has (&row))
	    ଜାରି;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      ଯଦି (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), ଅଙ୍କ_cols);
	ଯଦି (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      start_row += ଅଙ୍କ_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    ଯଦି (!optimize)
    {
      encodings = std::move (encoding_objs);
      ଫେରନ୍ତୁ !encodings.in_error ();
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
    hb_vector_t<item_t> queue_ବସ୍ତୁଗୁଡ଼ିକ;
    unsigned ଅଙ୍କ_todos = encoding_objs.length;
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_todos; i++)
    {
      ପାଇଁ (unsigned j = i + 1; j < ଅଙ୍କ_todos; j++)
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        ଯଦି (combining_gain > 0)
          queue_ବସ୍ତୁଗୁଡ଼ିକ.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_ବସ୍ତୁଗୁଡ଼ିକ));

    hb_bit_set_t removed_todo_idxes;
    ଯେତେବେଳେ (queue)
    {
      auto t = queue.କାଢ଼_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      ଯଦି (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        ଜାରି;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      ଯଦି (unlikely (removed_todo_idxes.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      encoding.merge (other_encoding);
      ଯଦି (unlikely (encoding.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ପାଇଁ (unsigned idx = 0; idx < encoding_objs.length; idx++)
      {
        ଯଦି (removed_todo_idxes.has (idx)) ଜାରି;

        ଅଚଳ delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        ଯଦି (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          ପାଇଁ (ଅଚଳ auto& row : obj.ବସ୍ତୁଗୁଡ଼ିକ)
            encoding.add_row (row);
          ଯଦି (unlikely (encoding.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

          removed_todo_idxes.add (idx);
          ଯଦି (unlikely (removed_todo_idxes.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
          ଜାରି;
        }

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା combined_gain = encoding.gain_from_merging (obj);
        ଯଦି (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.length), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      ଯଦି (unlikely (encoding_objs.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_final_encodings = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) encoding_objs.length - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) removed_todo_idxes.get_population ();
    ଯଦି (ଅଙ୍କ_final_encodings <= 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (!encodings.alloc (ଅଙ୍କ_final_encodings)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned i = 0; i < encoding_objs.length; i++)
    {
      ଯଦି (removed_todo_idxes.has (i)) ଜାରି;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    ଫେରନ୍ତୁ compile_varidx_map (front_mapping);
  }

  ବ୍ୟକ୍ତିଗତ:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  ତାର୍କିକ compile_varidx_map (ଅଚଳ hb_hashmap_t<unsigned, ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    ପାଇଁ (unsigned i = 0; i < encodings.length; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      ଯଦି (encoding.is_empty () || encoding.in_error ())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      unsigned ଅଙ୍କ_rows = encoding.ବସ୍ତୁଗୁଡ଼ିକ.length;
      unsigned ଅଙ୍କ_cols = encoding.chars.length;

      /* sort rows, make result deterministic */
      encoding.ବସ୍ତୁଗୁଡ଼ିକ.qsort (_cmp_row);

      ପାଇଁ (unsigned start = 0; start < ଅଙ୍କ_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (ଅଙ୍କ_rows - start, 0xFFFFu);
        hb_vector_t<ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*> rows;

        ଯଦି (!rows.alloc (chunk_len))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        unsigned major = split_encodings.length;
        ପାଇଁ (unsigned minor = 0; minor < chunk_len; minor++)
        {
          ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *row = encoding.ବସ୍ତୁଗୁଡ଼ିକ.arrayZ[start + minor];
          rows.push (row);
          ଯଦି (!back_mapping.set (row, (major << 16) + minor))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }

        auto *split = split_encodings.push (std::move (rows), ଅଙ୍କ_cols);
        ଯଦି (unlikely (split_encodings.in_error () || split->in_error ()))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }

    encodings = std::move (split_encodings);
    ଯଦି (encodings.in_error () || back_mapping.in_error ())
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      ଯଦି (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      ଅନ୍ୟଥା
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_INDEX);
    }
    ଫେରନ୍ତୁ !varidx_map.in_error ();
  }

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cmp_row (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>** a = (ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>**) pa;
    ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>** b = (ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>**) pb;

    ପାଇଁ (unsigned i = 0; i < (*b)->length; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା va = (*a)->arrayZ[i];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା vb = (*b)->arrayZ[i];
      ଯଦି (va != vb)
        ଫେରନ୍ତୁ va < vb ? -1 : 1;
    }
    ଫେରନ୍ତୁ 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

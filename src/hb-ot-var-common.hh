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

#অন্তর্ভাব "hb-ot-layout-common.hh"
#অন্তর্ভাব "hb-alloc-pool.hh"
#অন্তর্ভাব "hb-priority-queue.hh"
#অন্তর্ভাব "hb-subset-instancer-iup.hh"


নামাকাশ OT {

using rebase_tent_ফলাফল_scratch_t = hb_pair_t<rebase_tent_ফলাফল_t, rebase_tent_ফলাফল_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
কাঠামো TupleVariationHeader
{
  friend কাঠামো tuple_delta_t;
  size_t get_আকার (unsigned axis_গুনতি_times_2) ধ্রুবক
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    unsigned ti = tuple_সূচক;
    যদি (unlikely ((ti & (Tuple_সূচক::EmbeddedPeakTuple | Tuple_সূচক::IntermediateRegion))))
    {
      unsigned গুনতি = ((ti & Tuple_সূচক::EmbeddedPeakTuple) != 0) + ((ti & Tuple_সূচক::IntermediateRegion) != 0) * 2;
      ফেরত hb_unsigned_mul_add_saturate (গুনতি, axis_গুনতি_times_2, min_আকার);
    }
    ফেরত min_আকার;
  }

  unsigned get_তথ্য_আকার () ধ্রুবক { ফেরত var_তথ্য_আকার; }

  ধ্রুবক TupleVariationHeader &get_পরবর্তী (unsigned axis_গুনতি_times_2) ধ্রুবক
  { ফেরত StructAtOffset<TupleVariationHeader> (এই, get_আকার (axis_গুনতি_times_2)); }

  যৌক্তিক unpack_axis_tuples (unsigned axis_গুনতি,
                           ধ্রুবক hb_array_t<ধ্রুবক F2DOT14> shared_tuples,
                           ধ্রুবক hb_map_t *axes_old_সূচক_tag_map,
                           hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ধ্রুবক
  {
    ধ্রুবক F2DOT14 *peak_tuple = nullptr;
    যদি (has_peak ())
      peak_tuple = get_peak_tuple (axis_গুনতি);
    অন্যথা
    {
      unsigned পূর্ণসংখ্যা সূচক_2 = get_সূচক ();
      যদি (unlikely ((সূচক_2 + 1) * axis_গুনতি > shared_tuples.দৈর্ঘ্য))
        ফেরত মিথ্যা;
      peak_tuple = shared_tuples.sub_array (axis_গুনতি * সূচক_2, axis_গুনতি).arrayZ;
    }

    ধ্রুবক F2DOT14 *start_tuple = nullptr;
    ধ্রুবক F2DOT14 *end_tuple = nullptr;
    যৌক্তিক has_interm = has_intermediate ();

    যদি (has_interm)
    {
      start_tuple = get_start_tuple (axis_গুনতি);
      end_tuple = get_end_tuple (axis_গুনতি);
    }

    জন্য (unsigned i = 0; i < axis_গুনতি; i++)
    {
      দশমিক peak = peak_tuple[i].to_float ();
      যদি (peak == 0.f) চালু;

      hb_tag_t *axis_tag;
      যদি (!axes_old_সূচক_tag_map->has (i, &axis_tag))
        ফেরত মিথ্যা;

      দশমিক start, end;
      যদি (has_interm)
      {
        start = start_tuple[i].to_float ();
        end = end_tuple[i].to_float ();
      }
      অন্যথা
      {
        start = hb_min (peak, 0.f);
        end = hb_max (peak, 0.f);
      }
      axis_tuples.set (*axis_tag, Triple (start, peak, end));
    }

    ফেরত সত্য;
  }

  HB_ALWAYS_INLINE
  দশমিক calculate_scalar (hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
			   ধ্রুবক hb_array_t<ধ্রুবক F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ধ্রুবক
  {
    unsigned tuple_সূচক_2 = tuple_সূচক;
    unsigned পূর্ণসংখ্যা সূচক_2 = tuple_সূচক_2 & Tuple_সূচক::Tuple_সূচক_Mask;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    যৌক্তিক plain_shared_tuple = (tuple_সূচক_2 & ~Tuple_সূচক::PrivatePointNumbers) < shared_tuple_scalar_cache->দৈর্ঘ্য;
    যদি (likely (plain_shared_tuple))
    {
      দশমিক scalar;
      যদি (likely (shared_tuple_scalar_cache->get (সূচক_2, &scalar)))
	ফেরত scalar;
    }

    ধ্রুবক F2DOT14 *peak_tuple;

    যৌক্তিক has_interm = tuple_সূচক_2 & Tuple_সূচক::IntermediateRegion; // Inlined for performance

    যদি (unlikely (tuple_সূচক_2 & Tuple_সূচক::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_গুনতি);
      shared_tuple_scalar_cache = nullptr;
    }
    অন্যথা
    {
      দশমিক scalar;
      যদি (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->দৈর্ঘ্য &&
	  shared_tuple_scalar_cache->get (সূচক_2, &scalar))
      {
        যদি (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = nullptr;
	অন্যথা
	  ফেরত scalar;
      }

      যদি (unlikely ((সূচক_2 + 1) * coord_গুনতি > shared_tuples.দৈর্ঘ্য))
        ফেরত 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_গুনতি * সূচক_2);

    }

    ধ্রুবক F2DOT14 *start_tuple = nullptr;
    ধ্রুবক F2DOT14 *end_tuple = nullptr;

    যদি (has_interm)
    {
      start_tuple = get_start_tuple (coord_গুনতি);
      end_tuple = get_end_tuple (coord_গুনতি);
    }

    দশমিক scalar = 1.f;
#ifndef HB_OPTIMIZE_আকার
#যদি HB_FAST_অঙ্ক_ACCESS
    যৌক্তিক skip = coord_গুনতি >= 16;
#endif
#endif
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < coord_গুনতি; i++)
    {
#ifndef HB_OPTIMIZE_আকার
#যদি HB_FAST_অঙ্ক_ACCESS
      যদি (skip)
      {
	যখন (i + 4 <= coord_গুনতি && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	যখন (i < coord_গুনতি && peak_tuple[i].to_int () == 0)
	  i += 1;
	যদি (i >= coord_গুনতি)
	  বিরতি;
      }
#endif
#endif

      পূর্ণসংখ্যা peak = peak_tuple[i].to_int ();
      যদি (!peak) চালু;

      পূর্ণসংখ্যা v = coords[i];
      যদি (!v) { scalar = 0.f; বিরতি; }
      যদি (v == peak) চালু;

      যদি (has_interm)
      {
	shared_tuple_scalar_cache = nullptr;
        পূর্ণসংখ্যা start = start_tuple[i].to_int ();
        পূর্ণসংখ্যা end = end_tuple[i].to_int ();
        যদি (unlikely (start > peak || peak > end ||
                      (start < 0 && end > 0 && peak))) চালু;
        যদি (v < start || v > end) { scalar = 0.f; বিরতি; }
        যদি (v < peak)
        { যদি (peak != start) scalar *= (দশমিক) (v - start) / (peak - start); }
        অন্যথা
        { যদি (peak != end) scalar *= (দশমিক) (end - v) / (end - peak); }
      }
      অন্যথা যদি (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; বিরতি; }
      অন্যথা
        scalar *= (দশমিক) v / peak;
    }
    যদি (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->set (সূচক_2, scalar);
    ফেরত scalar;
  }

  যৌক্তিক           has_peak () ধ্রুবক { ফেরত tuple_সূচক & Tuple_সূচক::EmbeddedPeakTuple; }
  যৌক্তিক   has_intermediate () ধ্রুবক { ফেরত tuple_সূচক & Tuple_সূচক::IntermediateRegion; }
  যৌক্তিক has_private_points () ধ্রুবক { ফেরত tuple_সূচক & Tuple_সূচক::PrivatePointNumbers; }
  unsigned      get_সূচক () ধ্রুবক { ফেরত tuple_সূচক & Tuple_সূচক::Tuple_সূচক_Mask; }
  স্থির constexpr unsigned max_shared_tuple_গুনতি = 0x1000u;

  সুরক্ষিত:
  কাঠামো Tuple_সূচক : HBUINT16
  {
    গণনা Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      PrivatePointNumbers = 0x2000u,
      Tuple_সূচক_Mask      = 0x0FFFu
    };

    Tuple_সূচক& operator = (uint16_t i) { HBUINT16::operator= (i); ফেরত *এই; }
    DEFINE_আকার_STATIC (2);
  };

  hb_array_t<ধ্রুবক F2DOT14> get_all_tuples (unsigned axis_গুনতি) ধ্রুবক
  { ফেরত StructAfter<UnsizedArrayOf<F2DOT14>> (tuple_সূচক).as_array ((has_peak () + has_intermediate () * 2) * axis_গুনতি); }
  ধ্রুবক F2DOT14* get_all_tuples_base (unsigned axis_গুনতি) ধ্রুবক
  { ফেরত StructAfter<UnsizedArrayOf<F2DOT14>> (tuple_সূচক).arrayZ; }
  ধ্রুবক F2DOT14* get_peak_tuple (unsigned axis_গুনতি) ধ্রুবক
  { ফেরত get_all_tuples_base (axis_গুনতি); }
  ধ্রুবক F2DOT14* get_start_tuple (unsigned axis_গুনতি) ধ্রুবক
  { ফেরত get_all_tuples_base (axis_গুনতি) + has_peak () * axis_গুনতি; }
  ধ্রুবক F2DOT14* get_end_tuple (unsigned axis_গুনতি) ধ্রুবক
  { ফেরত get_all_tuples_base (axis_গুনতি) + has_peak () * axis_গুনতি + axis_গুনতি; }

  HBUINT16      var_তথ্য_আকার;    /* The size in bytes of the serialized
                                 * data for this tuple variation table. */
  Tuple_সূচক    tuple_সূচক;     /* A packed field. The high 4 bits are flags (see below).
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
  সর্বজনীন:
  DEFINE_আকার_MIN (4);
};

কাঠামো optimize_scratch_t
{
  iup_scratch_t iup;
  hb_vector_t<যৌক্তিক> opt_indices;
  hb_vector_t<পূর্ণসংখ্যা> rounded_x_deltas;
  hb_vector_t<পূর্ণসংখ্যা> rounded_y_deltas;
  hb_vector_t<দশমিক> opt_deltas_x;
  hb_vector_t<দশমিক> opt_deltas_y;
  hb_vector_t<unsigned অক্ষর> opt_point_তথ্য;
  hb_vector_t<unsigned অক্ষর> opt_deltas_তথ্য;
  hb_vector_t<unsigned অক্ষর> point_তথ্য;
  hb_vector_t<unsigned অক্ষর> deltas_তথ্য;
  hb_vector_t<পূর্ণসংখ্যা> rounded_deltas;
};

কাঠামো tuple_delta_t
{
  স্থির constexpr যৌক্তিক realloc_move = সত্য;  // Watch out when adding new members!

  সর্বজনীন:
  hb_hashmap_t<hb_tag_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_vector_t<যৌক্তিক> indices;

  hb_vector_t<দশমিক> deltas_x;
  /* empty for cvar tuples */
  hb_vector_t<দশমিক> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_vector_t<unsigned অক্ষর> compiled_tuple_header;
  hb_vector_t<unsigned অক্ষর> compiled_deltas;

  hb_vector_t<F2DOT14> compiled_peak_coords;
  hb_vector_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = nullptr) {}
  tuple_delta_t (ধ্রুবক tuple_delta_t& o) = পূর্বনির্ধারিত;
  tuple_delta_t& operator = (ধ্রুবক tuple_delta_t& o) = পূর্বনির্ধারিত;

  friend শূন্য swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
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
  { hb_swap (*এই, o); }

  tuple_delta_t& operator = (tuple_delta_t&& o) noexcept
  {
    hb_swap (*এই, o);
    ফেরত *এই;
  }

  শূন্য copy_from (ধ্রুবক tuple_delta_t& o, hb_alloc_pool_t *pool = nullptr)
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

  শূন্য remove_axis (hb_tag_t axis_tag)
  { axis_tuples.del (axis_tag); }

  যৌক্তিক set_tent (hb_tag_t axis_tag, Triple tent)
  { ফেরত axis_tuples.set (axis_tag, tent); }

  tuple_delta_t& operator += (ধ্রুবক tuple_delta_t& o)
  {
    unsigned অঙ্ক = indices.দৈর্ঘ্য;
    জন্য (unsigned i = 0; i < অঙ্ক; i++)
    {
      যদি (indices.arrayZ[i])
      {
        যদি (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          যদি (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      অন্যথা
      {
        যদি (!o.indices.arrayZ[i]) চালু;
        indices.arrayZ[i] = সত্য;
        deltas_x[i] = o.deltas_x[i];
        যদি (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    ফেরত *এই;
  }

  tuple_delta_t& operator *= (দশমিক scalar)
  {
    যদি (scalar == 1.0f)
      ফেরত *এই;

    unsigned অঙ্ক = indices.দৈর্ঘ্য;
    যদি (deltas_y)
      জন্য (unsigned i = 0; i < অঙ্ক; i++)
      {
	যদি (!indices.arrayZ[i]) চালু;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    অন্যথা
      জন্য (unsigned i = 0; i < অঙ্ক; i++)
      {
	যদি (!indices.arrayZ[i]) চালু;
	deltas_x[i] *= scalar;
      }
    ফেরত *এই;
  }

  শূন্য change_tuple_var_axis_limit (hb_tag_t axis_tag, Triple axis_limit,
				    TripleDistances axis_triple_distances,
				    hb_vector_t<tuple_delta_t>& out,
				    rebase_tent_ফলাফল_scratch_t &scratch,
				    hb_alloc_pool_t *pool = nullptr)
  {
    // May move *this out.

    out.reset ();
    Triple *tent;
    যদি (!axis_tuples.has (axis_tag, &tent))
    {
      out.push (std::move (*এই));
      ফেরত;
    }

    যদি ((tent->minimum < 0.f && tent->maximum > 0.f) ||
        !(tent->minimum <= tent->মধ্য && tent->মধ্য <= tent->maximum))
      ফেরত;

    যদি (tent->মধ্য == 0.f)
    {
      out.push (std::move (*এই));
      ফেরত;
    }

    যদি (!axis_limit.is_point () &&
        !(-1.f <= axis_limit.minimum &&
          axis_limit.minimum <= axis_limit.মধ্য &&
          axis_limit.মধ্য <= axis_limit.maximum &&
          axis_limit.maximum <= +1.f))
      ফেরত;

    rebase_tent_ফলাফল_t &solutions = scratch.first;
    rebase_tent (*tent, axis_limit, axis_triple_distances, solutions, scratch.second);
    জন্য (unsigned i = 0; i < solutions.দৈর্ঘ্য; i++)
    {
      auto &t = solutions.arrayZ[i];

      tuple_delta_t new_var;
      যদি (i < solutions.দৈর্ঘ্য - 1)
	new_var.copy_from (*এই, pool);
      অন্যথা
	new_var = std::move (*এই);

      যদি (t.second == Triple ())
        new_var.remove_axis (axis_tag);
      অন্যথা
        new_var.set_tent (axis_tag, t.second);

      new_var *= t.first;
      out.push (std::move (new_var));
    }
  }

  যৌক্তিক compile_coords (ধ্রুবক hb_map_t& axes_সূচক_map,
		       ধ্রুবক hb_map_t& axes_old_সূচক_tag_map,
		       hb_alloc_pool_t *pool= nullptr)
  {
    unsigned cur_axis_গুনতি = axes_সূচক_map.get_population ();
    যদি (pool)
    {
      যদি (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_গুনতি)))
	ফেরত মিথ্যা;
    }
    অন্যথা যদি (unlikely (!compiled_peak_coords.resize (cur_axis_গুনতি)))
      ফেরত মিথ্যা;

    hb_array_t<F2DOT14> start_coords, end_coords;

    unsigned orig_axis_গুনতি = axes_old_সূচক_tag_map.get_population ();
    unsigned j = 0;
    জন্য (unsigned i = 0; i < orig_axis_গুনতি; i++)
    {
      যদি (!axes_সূচক_map.has (i))
        চালু;

      hb_tag_t axis_tag = axes_old_সূচক_tag_map.get (i);
      Triple *coords = nullptr;
      যদি (axis_tuples.has (axis_tag, &coords))
      {
	দশমিক min_val = coords->minimum;
	দশমিক val = coords->মধ্য;
	দশমিক max_val = coords->maximum;

	compiled_peak_coords.arrayZ[j].set_float (val);

	যদি (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  যদি (!compiled_interm_coords)
	  {
	    যদি (pool)
	    {
	      যদি (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_গুনতি)))
		ফেরত মিথ্যা;
	    }
	    অন্যথা যদি (unlikely (!compiled_interm_coords.resize (2 * cur_axis_গুনতি)))
	      ফেরত মিথ্যা;
	    start_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_গুনতি);
	    end_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_গুনতি);

	    জন্য (unsigned k = 0; k < j; k++)
	    {
	      signed peak = compiled_peak_coords.arrayZ[k].to_int ();
	      যদি (!peak) চালু;
	      start_coords.arrayZ[k].set_int (hb_min (peak, 0));
	      end_coords.arrayZ[k].set_int (hb_max (peak, 0));
	    }
	  }

	}

	যদি (compiled_interm_coords)
	{
	  start_coords.arrayZ[j].set_float (min_val);
	  end_coords.arrayZ[j].set_float (max_val);
	}
      }

      j++;
    }

    ফেরত !compiled_peak_coords.in_error () && !compiled_interm_coords.in_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  যৌক্তিক compile_tuple_var_header (ধ্রুবক hb_map_t& axes_সূচক_map,
                                 unsigned points_তথ্য_দৈর্ঘ্য,
                                 ধ্রুবক hb_map_t& axes_old_সূচক_tag_map,
                                 ধ্রুবক hb_hashmap_t<ধ্রুবক hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = nullptr)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    যদি (!compiled_deltas) ফেরত সত্য;

    unsigned cur_axis_গুনতি = axes_সূচক_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    unsigned alloc_len = 3 * cur_axis_গুনতি * (F2DOT14::static_আকার) + 4;
    যদি (unlikely (!compiled_tuple_header.allocate_from_pool (pool, alloc_len, মিথ্যা))) ফেরত মিথ্যা;

    unsigned flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.begin () + 4);
    F2DOT14* end = reinterpret_cast<F2DOT14 *> (compiled_tuple_header.end ());
    hb_array_t<F2DOT14> coords (p, end - p);

    যদি (!shared_tuples_idx_map)
      compile_coords (axes_সূচক_map, axes_old_সূচক_tag_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    unsigned peak_গুনতি = 0;
    unsigned *shared_tuple_idx;
    যদি (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    অন্যথা
    {
      peak_গুনতি = encode_peak_coords(coords, flag);
      যদি (!peak_গুনতি) ফেরত মিথ্যা;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    unsigned interim_গুনতি = encode_interm_coords (coords.sub_array (peak_গুনতি), flag);

    /* pointdata length = 0 implies "use shared points" */
    যদি (points_তথ্য_দৈর্ঘ্য)
      flag |= TupleVariationHeader::Tuple_সূচক::PrivatePointNumbers;

    unsigned serialized_তথ্য_আকার = points_তথ্য_দৈর্ঘ্য + compiled_deltas.দৈর্ঘ্য;
    TupleVariationHeader *o = reinterpret_cast<TupleVariationHeader *> (compiled_tuple_header.begin ());
    o->var_তথ্য_আকার = serialized_তথ্য_আকার;
    o->tuple_সূচক = flag;

    unsigned যোগ_header_len = 4 + (peak_গুনতি + interim_গুনতি) * (F2DOT14::static_আকার);
    compiled_tuple_header.shrink_back_to_pool (pool, যোগ_header_len);
    ফেরত সত্য;
  }

  unsigned encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               unsigned& flag) ধ্রুবক
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.দৈর্ঘ্য * আকার (compiled_peak_coords[0]));
    flag |= TupleVariationHeader::Tuple_সূচক::EmbeddedPeakTuple;
    ফেরত compiled_peak_coords.দৈর্ঘ্য;
  }

  /* if no need to encode intermediate coords, then just return p */
  unsigned encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 unsigned& flag) ধ্রুবক
  {
    যদি (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.দৈর্ঘ্য * আকার (compiled_interm_coords[0]));
      flag |= TupleVariationHeader::Tuple_সূচক::IntermediateRegion;
    }
    ফেরত compiled_interm_coords.দৈর্ঘ্য;
  }

  যৌক্তিক compile_deltas (hb_vector_t<পূর্ণসংখ্যা> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = nullptr)
  { ফেরত compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  স্থির যৌক্তিক compile_deltas (hb_array_t<ধ্রুবক যৌক্তিক> point_indices,
			      hb_array_t<ধ্রুবক দশমিক> x_deltas,
			      hb_array_t<ধ্রুবক দশমিক> y_deltas,
			      hb_vector_t<unsigned অক্ষর> &compiled_deltas, /* OUT */
			      hb_vector_t<পূর্ণসংখ্যা> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = nullptr)
  {
    যদি (unlikely (!rounded_deltas.resize_dirty  (point_indices.দৈর্ঘ্য)))
      ফেরত মিথ্যা;

    unsigned j = 0;
    জন্য (unsigned i = 0; i < point_indices.দৈর্ঘ্য; i++)
    {
      যদি (!point_indices[i]) চালু;
      rounded_deltas.arrayZ[j++] = hb_clamp_to<পূর্ণসংখ্যা> (roundf ((দ্বিগুণ) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    যদি (!rounded_deltas) ফেরত সত্য;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    unsigned alloc_len = 2 + 4 * rounded_deltas.দৈর্ঘ্য + (rounded_deltas.দৈর্ঘ্য + 63) / 64;
    যদি (y_deltas)
      alloc_len *= 2;

    যদি (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, মিথ্যা))) ফেরত মিথ্যা;

    unsigned encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    যদি (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      unsigned j = 0;
      জন্য (unsigned idx = 0; idx < point_indices.দৈর্ঘ্য; idx++)
      {
        যদি (!point_indices[idx]) চালু;
        পূর্ণসংখ্যা rounded_delta = hb_clamp_to<পূর্ণসংখ্যা> (roundf ((দ্বিগুণ) y_deltas.arrayZ[idx]));

        যদি (j >= rounded_deltas.দৈর্ঘ্য) ফেরত মিথ্যা;

        rounded_deltas[j++] = rounded_delta;
      }

      যদি (j != rounded_deltas.দৈর্ঘ্য) ফেরত মিথ্যা;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_back_to_pool (pool, encoded_len);
    ফেরত সত্য;
  }

  স্থির unsigned compile_deltas (hb_array_t<unsigned অক্ষর> encoded_bytes,
				  hb_array_t<ধ্রুবক পূর্ণসংখ্যা> deltas)
  {
    ফেরত Tuple_মানসমূহ::compile_unsafe (deltas, encoded_bytes);
  }

  যৌক্তিক calc_inferred_deltas (ধ্রুবক contour_point_vector_t& orig_points,
			     hb_vector_t<unsigned> &scratch)
  {
    unsigned point_গুনতি = orig_points.দৈর্ঘ্য;
    যদি (point_গুনতি != indices.দৈর্ঘ্য)
      ফেরত মিথ্যা;

    unsigned ref_গুনতি_2 = 0;

    hb_vector_t<unsigned> &end_points = scratch.reset ();

    জন্য (unsigned i = 0; i < point_গুনতি; i++)
    {
      ref_গুনতি_2 += indices.arrayZ[i];
      যদি (orig_points.arrayZ[i].is_end_point)
        end_points.push (i);
    }
    /* all points are referenced, nothing to do */
    যদি (ref_গুনতি_2 == point_গুনতি)
      ফেরত সত্য;
    যদি (unlikely (end_points.in_error ())) ফেরত মিথ্যা;

    hb_bit_set_t inferred_idxes;
    unsigned start_point = 0;
    জন্য (unsigned end_point : end_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      unsigned unref_গুনতি = 0;
      জন্য (unsigned i = start_point; i < end_point + 1; i++)
        unref_গুনতি += indices.arrayZ[i];
      unref_গুনতি = (end_point - start_point + 1) - unref_গুনতি;

      unsigned j = start_point;
      যদি (unref_গুনতি == 0 || unref_গুনতি > end_point - start_point)
        যাও no_more_gaps;
      জন্য (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        unsigned পূর্ণসংখ্যা prev, next, i;
        জন্য (;;)
        {
          i = j;
          j = পরবর্তী_সূচক (i, start_point, end_point);
          যদি (indices.arrayZ[i] && !indices.arrayZ[j]) বিরতি;
        }
        prev = j = i;
        জন্য (;;)
        {
          i = j;
          j = পরবর্তী_সূচক (i, start_point, end_point);
          যদি (!indices.arrayZ[i] && indices.arrayZ[j]) বিরতি;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        জন্য (;;)
        {
          i = পরবর্তী_সূচক (i, start_point, end_point);
          যদি (i == next) বিরতি;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.add (i);
          যদি (--unref_গুনতি == 0) যাও no_more_gaps;
        }
      }
    no_more_gaps:
      start_point = end_point + 1;
    }

    জন্য (unsigned i = 0; i < point_গুনতি; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      যদি ( !indices[i])
      {
        যদি (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = সত্য;
      }
    }
    ফেরত সত্য;
  }

  যৌক্তিক optimize (ধ্রুবক contour_point_vector_t& contour_points,
                 যৌক্তিক is_composite,
		 optimize_scratch_t &scratch,
                 দ্বিগুণ tolerance = 0.5 + 1e-10)
  {
    unsigned গুনতি = contour_points.দৈর্ঘ্য;
    যদি (deltas_x.দৈর্ঘ্য != গুনতি ||
        deltas_y.দৈর্ঘ্য != গুনতি)
      ফেরত মিথ্যা;

    hb_vector_t<যৌক্তিক> &opt_indices = scratch.opt_indices.reset ();
    hb_vector_t<পূর্ণসংখ্যা> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_vector_t<পূর্ণসংখ্যা> &rounded_y_deltas = scratch.rounded_y_deltas;

    যদি (unlikely (!rounded_x_deltas.resize_dirty  (গুনতি) ||
                  !rounded_y_deltas.resize_dirty  (গুনতি)))
      ফেরত মিথ্যা;

    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_to<পূর্ণসংখ্যা> (roundf ((দ্বিগুণ) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_to<পূর্ণসংখ্যা> (roundf ((দ্বিগুণ) deltas_y.arrayZ[i]));
    }

    যদি (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      ফেরত মিথ্যা;

    unsigned ref_গুনতি_2 = 0;
    জন্য (যৌক্তিক ref_flag : opt_indices)
       ref_গুনতি_2 += ref_flag;

    যদি (ref_গুনতি_2 == গুনতি) ফেরত সত্য;

    hb_vector_t<দশমিক> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_vector_t<দশমিক> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    যৌক্তিক is_comp_glyph_wo_deltas = (is_composite && ref_গুনতি_2 == 0);
    যদি (is_comp_glyph_wo_deltas)
    {
      যদি (unlikely (!opt_deltas_x.resize (গুনতি) ||
                    !opt_deltas_y.resize (গুনতি)))
        ফেরত মিথ্যা;

      opt_indices.arrayZ[0] = সত্য;
      জন্য (unsigned i = 1; i < গুনতি; i++)
        opt_indices.arrayZ[i] = মিথ্যা;
    }

    hb_vector_t<unsigned অক্ষর> &opt_point_তথ্য = scratch.opt_point_তথ্য.reset ();
    যদি (!compile_point_set (opt_indices, opt_point_তথ্য))
      ফেরত মিথ্যা;
    hb_vector_t<unsigned অক্ষর> &opt_deltas_তথ্য = scratch.opt_deltas_তথ্য.reset ();
    যদি (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_তথ্য,
			 scratch.rounded_deltas))
      ফেরত মিথ্যা;

    hb_vector_t<unsigned অক্ষর> &point_তথ্য = scratch.point_তথ্য.reset ();
    যদি (!compile_point_set (indices, point_তথ্য))
      ফেরত মিথ্যা;
    hb_vector_t<unsigned অক্ষর> &deltas_তথ্য = scratch.deltas_তথ্য.reset ();
    যদি (!compile_deltas (indices, deltas_x, deltas_y, deltas_তথ্য, scratch.rounded_deltas))
      ফেরত মিথ্যা;

    যদি (opt_point_তথ্য.দৈর্ঘ্য + opt_deltas_তথ্য.দৈর্ঘ্য < point_তথ্য.দৈর্ঘ্য + deltas_তথ্য.দৈর্ঘ্য)
    {
      indices = std::move (opt_indices);

      যদি (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    ফেরত !indices.in_error () && !deltas_x.in_error () && !deltas_y.in_error ();
  }

  স্থির যৌক্তিক compile_point_set (ধ্রুবক hb_vector_t<যৌক্তিক> &point_indices,
                                 hb_vector_t<unsigned অক্ষর>& compiled_points /* OUT */)
  {
    unsigned অঙ্ক_points = 0;
    জন্য (যৌক্তিক i : point_indices)
      যদি (i) অঙ্ক_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    যদি (!অঙ্ক_points) ফেরত সত্য;

    unsigned indices_দৈর্ঘ্য = point_indices.দৈর্ঘ্য;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    যদি (অঙ্ক_points == indices_দৈর্ঘ্য)
      ফেরত compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    unsigned অঙ্ক_bytes = 2 + 3 *অঙ্ক_points;
    যদি (unlikely (!compiled_points.resize_dirty  (অঙ্ক_bytes)))
      ফেরত মিথ্যা;

    unsigned pos = 0;
    /* binary data starts with the total number of reference points */
    যদি (অঙ্ক_points < 0x80)
      compiled_points.arrayZ[pos++] = অঙ্ক_points;
    অন্যথা
    {
      compiled_points.arrayZ[pos++] = ((অঙ্ক_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = অঙ্ক_points & 0xFF;
    }

    ধ্রুবক unsigned max_run_দৈর্ঘ্য = 0x7F;
    unsigned i = 0;
    unsigned last_মান = 0;
    unsigned অঙ্ক_encoded = 0;
    যখন (i < indices_দৈর্ঘ্য && অঙ্ক_encoded < অঙ্ক_points)
    {
      unsigned run_দৈর্ঘ্য = 0;
      unsigned header_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      যৌক্তিক use_byte_encoding = মিথ্যা;
      যৌক্তিক new_run = সত্য;
      যখন (i < indices_দৈর্ঘ্য && অঙ্ক_encoded < অঙ্ক_points &&
             run_দৈর্ঘ্য <= max_run_দৈর্ঘ্য)
      {
        // find out next referenced point index
        যখন (i < indices_দৈর্ঘ্য && !point_indices[i])
          i++;

        যদি (i >= indices_দৈর্ঘ্য) বিরতি;

        unsigned cur_মান = i;
        unsigned delta = cur_মান - last_মান;

        যদি (new_run)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_run = মিথ্যা;
        }

        যদি (use_byte_encoding && delta > 0xFF)
          বিরতি;

        যদি (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        অন্যথা
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        last_মান = cur_মান;
        run_দৈর্ঘ্য++;
        অঙ্ক_encoded++;
      }

      যদি (use_byte_encoding)
        compiled_points.arrayZ[header_pos] = run_দৈর্ঘ্য - 1;
      অন্যথা
        compiled_points.arrayZ[header_pos] = (run_দৈর্ঘ্য - 1) | 0x80;
    }
    ফেরত compiled_points.resize_dirty  (pos);
  }

  স্থির দশমিক infer_delta (দশমিক target_val, দশমিক পূর্ব_val, দশমিক পরবর্তী_val, দশমিক পূর্ব_delta, দশমিক পরবর্তী_delta)
  {
    যদি (পূর্ব_val == পরবর্তী_val)
      ফেরত (পূর্ব_delta == পরবর্তী_delta) ? পূর্ব_delta : 0.f;
    অন্যথা যদি (target_val <= hb_min (পূর্ব_val, পরবর্তী_val))
      ফেরত (পূর্ব_val < পরবর্তী_val) ? পূর্ব_delta : পরবর্তী_delta;
    অন্যথা যদি (target_val >= hb_max (পূর্ব_val, পরবর্তী_val))
      ফেরত (পূর্ব_val > পরবর্তী_val) ? পূর্ব_delta : পরবর্তী_delta;

    দশমিক r = (target_val - পূর্ব_val) / (পরবর্তী_val - পূর্ব_val);
    ফেরত পূর্ব_delta + r * (পরবর্তী_delta - পূর্ব_delta);
  }

  স্থির unsigned পূর্ণসংখ্যা পরবর্তী_সূচক (unsigned পূর্ণসংখ্যা i, unsigned পূর্ণসংখ্যা start, unsigned পূর্ণসংখ্যা end)
  { ফেরত (i >= end) ? start : (i + 1); }
};

template <typename OffType = HBUINT16>
কাঠামো Tuple_Variation_তথ্য
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (এই));
  }

  size_t get_আকার (unsigned axis_গুনতি_times_2) ধ্রুবক
  {
    size_t যোগ_আকার = min_আকার;
    unsigned গুনতি = tuple_Var_গুনতি.get_গুনতি ();
    ধ্রুবক TupleVariationHeader *tuple_var_header = &(get_tuple_var_header());
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      যোগ_আকার = hb_unsigned_add_saturate (যোগ_আকার,
					     tuple_var_header->get_আকার (axis_গুনতি_times_2),
					     (size_t) tuple_var_header->get_তথ্য_আকার ());
      tuple_var_header = &tuple_var_header->get_পরবর্তী (axis_গুনতি_times_2);
    }

    ফেরত যোগ_আকার;
  }

  ধ্রুবক TupleVariationHeader &get_tuple_var_header (শূন্য) ধ্রুবক
  { ফেরত StructAfter<TupleVariationHeader> (তথ্য); }

  কাঠামো tuple_iterator_t;
  কাঠামো tuple_variations_t
  {
    hb_vector_t<tuple_delta_t> tuple_vars;

    ব্যক্তিগত:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ধ্রুবক hb_vector_t<যৌক্তিক>*, hb_vector_t<unsigned অক্ষর>> point_তথ্য_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ধ্রুবক hb_vector_t<যৌক্তিক>*, unsigned> point_set_গুনতি_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_vector_t<unsigned অক্ষর> *shared_points_bytes = nullptr;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    unsigned compiled_byte_আকার = 0;
    যৌক্তিক needs_padding = মিথ্যা;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    যৌক্তিক is_composite = মিথ্যা;

    সর্বজনীন:
    tuple_variations_t () = পূর্বনির্ধারিত;
    tuple_variations_t (ধ্রুবক tuple_variations_t&) = delete;
    tuple_variations_t& operator=(ধ্রুবক tuple_variations_t&) = delete;
    tuple_variations_t (tuple_variations_t&&) = পূর্বনির্ধারিত;
    tuple_variations_t& operator=(tuple_variations_t&&) = পূর্বনির্ধারিত;
    ~tuple_variations_t () = পূর্বনির্ধারিত;

    explicit operator যৌক্তিক () ধ্রুবক { ফেরত যৌক্তিক (tuple_vars); }
    unsigned get_var_গুনতি () ধ্রুবক
    {
      unsigned গুনতি = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      জন্য (auto& tuple: tuple_vars)
        যদি (tuple.compiled_deltas) গুনতি++;

      যদি (shared_points_bytes && shared_points_bytes->দৈর্ঘ্য)
        গুনতি |= Tuple_Var_গুনতি::SharedPointNumbers;
      ফেরত গুনতি;
    }

    unsigned get_compiled_byte_আকার () ধ্রুবক
    { ফেরত compiled_byte_আকার; }

    যৌক্তিক create_from_tuple_var_তথ্য (tuple_iterator_t iterator,
                                     unsigned tuple_var_গুনতি,
                                     unsigned point_গুনতি,
                                     যৌক্তিক is_gvar,
                                     ধ্রুবক hb_map_t *axes_old_সূচক_tag_map,
                                     ধ্রুবক hb_vector_t<unsigned> &shared_indices,
                                     ধ্রুবক hb_array_t<ধ্রুবক F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = nullptr,
                                     যৌক্তিক is_composite_glyph = মিথ্যা)
    {
      hb_vector_t<unsigned> private_indices;
      hb_vector_t<পূর্ণসংখ্যা> deltas_x;
      hb_vector_t<পূর্ণসংখ্যা> deltas_y;
      করো
      {
        ধ্রুবক HBUINT8 *p = iterator.get_serialized_তথ্য ();
        unsigned পূর্ণসংখ্যা দৈর্ঘ্য = iterator.current_tuple->get_তথ্য_আকার ();
        যদি (unlikely (!iterator.var_তথ্য_bytes.check_range (p, দৈর্ঘ্য)))
          ফেরত মিথ্যা;

	hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
        যদি (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_গুনতি (), shared_tuples, axes_old_সূচক_tag_map, axis_tuples)
            || axis_tuples.is_empty ())
          ফেরত মিথ্যা;

	private_indices.reset ();
        যৌক্তিক has_private_points = iterator.current_tuple->has_private_points ();
        ধ্রুবক HBUINT8 *end = p + দৈর্ঘ্য;
        যদি (has_private_points &&
            !Tuple_Variation_তথ্য::decompile_points (p, private_indices, end))
          ফেরত মিথ্যা;

        ধ্রুবক hb_vector_t<unsigned> &indices = has_private_points ? private_indices : shared_indices;
        যৌক্তিক apply_to_all = (indices.দৈর্ঘ্য == 0);
        unsigned অঙ্ক_deltas = apply_to_all ? point_গুনতি : indices.দৈর্ঘ্য;

        যদি (unlikely (!deltas_x.resize_dirty  (অঙ্ক_deltas) ||
                      !Tuple_Variation_তথ্য::decompile_deltas (p, deltas_x, end)))
          ফেরত মিথ্যা;

        যদি (is_gvar)
        {
          যদি (unlikely (!deltas_y.resize_dirty  (অঙ্ক_deltas) ||
                        !Tuple_Variation_তথ্য::decompile_deltas (p, deltas_y, end)))
            ফেরত মিথ্যা;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        যদি (unlikely (!var.indices.allocate_from_pool (pool, point_গুনতি) ||
                      !var.deltas_x.allocate_from_pool (pool, point_গুনতি, মিথ্যা)))
          ফেরত মিথ্যা;

        যদি (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, point_গুনতি, মিথ্যা)))
          ফেরত মিথ্যা;

        জন্য (unsigned i = 0; i < অঙ্ক_deltas; i++)
        {
          unsigned idx = apply_to_all ? i : indices[i];
          যদি (idx >= point_গুনতি) চালু;
          var.indices[idx] = সত্য;
          var.deltas_x[idx] = deltas_x[i];
          যদি (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } যখন (iterator.move_to_পরবর্তী ());

      is_composite = is_composite_glyph;
      ফেরত সত্য;
    }

    যৌক্তিক create_from_item_var_তথ্য (ধ্রুবক Var_তথ্য &var_তথ্য,
                                    ধ্রুবক hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions,
                                    ধ্রুবক hb_map_t& axes_old_সূচক_tag_map,
                                    unsigned& item_গুনতি_2,
                                    ধ্রুবক hb_inc_bimap_t* inner_map = nullptr)
    {
      /* NULL offset, to keep original varidx valid, just return */
      যদি (&var_তথ্য == &Null (Var_তথ্য))
        ফেরত সত্য;

      unsigned অঙ্ক_regions = var_তথ্য.get_region_সূচক_গুনতি ();
      যদি (!tuple_vars.alloc (অঙ্ক_regions)) ফেরত মিথ্যা;

      item_গুনতি_2 = inner_map ? inner_map->get_population () : var_তথ্য.get_item_গুনতি ();
      যদি (!item_গুনতি_2) ফেরত সত্য;
      unsigned row_আকার = var_তথ্য.get_row_আকার ();
      ধ্রুবক HBUINT8 *delta_bytes = var_তথ্য.get_delta_bytes ();

      জন্য (unsigned r = 0; r < অঙ্ক_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        যদি (!tuple.deltas_x.resize_dirty  (item_গুনতি_2) ||
            !tuple.indices.resize_dirty  (item_গুনতি_2))
          ফেরত মিথ্যা;

        জন্য (unsigned i = 0; i < item_গুনতি_2; i++)
        {
          tuple.indices.arrayZ[i] = সত্য;
          tuple.deltas_x.arrayZ[i] = var_তথ্য.get_item_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_bytes, row_আকার);
        }

        unsigned region_সূচক = var_তথ্য.get_region_সূচক (r);
        যদি (region_সূচক >= regions.দৈর্ঘ্য) ফেরত মিথ্যা;
        tuple.axis_tuples = regions.arrayZ[region_সূচক];

        tuple_vars.push (std::move (tuple));
      }
      ফেরত !tuple_vars.in_error ();
    }

    ব্যক্তিগত:
    স্থির পূর্ণসংখ্যা _cmp_axis_tag (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
    {
      ধ্রুবক hb_tag_t *a = (ধ্রুবক hb_tag_t*) pa;
      ধ্রুবক hb_tag_t *b = (ধ্রুবক hb_tag_t*) pb;
      ফেরত (পূর্ণসংখ্যা)(*a) - (পূর্ণসংখ্যা)(*b);
    }

    যৌক্তিক change_tuple_variations_axis_limits (ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                                              ধ্রুবক hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = nullptr)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_vector_t<hb_tag_t> axis_tags;
      যদি (!axis_tags.alloc (normalized_axes_location.get_population ()))
        ফেরত মিথ্যা;
      জন্য (auto t : normalized_axes_location.চাবিগুলি ())
        axis_tags.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_ফলাফল_scratch_t scratch;
      hb_vector_t<tuple_delta_t> out;

      axis_tags.qsort (_cmp_axis_tag);
      জন্য (auto axis_tag : axis_tags)
      {
        Triple *axis_limit;
        যদি (!normalized_axes_location.has (axis_tag, &axis_limit))
          ফেরত মিথ্যা;
        TripleDistances axis_triple_distances{1.f, 1.f};
        যদি (axes_triple_distances.has (axis_tag))
          axis_triple_distances = axes_triple_distances.get (axis_tag);

        hb_vector_t<tuple_delta_t> new_vars;
        জন্য (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_limit (axis_tag, *axis_limit, axis_triple_distances, out, scratch, pool);
          যদি (!out) চালু;

          unsigned new_len = new_vars.দৈর্ঘ্য + out.দৈর্ঘ্য;

          যদি (unlikely (!new_vars.alloc (new_len, মিথ্যা)))
            ফেরত মিথ্যা;

          জন্য (unsigned i = 0; i < out.দৈর্ঘ্য; i++)
            new_vars.push (std::move (out[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      ফেরত সত্য;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    যৌক্তিক merge_tuple_variations (contour_point_vector_t* contour_points = nullptr)
    {
      hb_vector_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      যদি (unlikely (!new_vars.alloc (tuple_vars.দৈর্ঘ্য)))
	ফেরত মিথ্যা;
      hb_hashmap_t<ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>*, unsigned> m;
      জন্য (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        যদি (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          যদি (contour_points && !contour_points->add_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            ফেরত মিথ্যা;
          চালু;
        }

        unsigned *idx;
        যদি (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        অন্যথা
        {
	  auto *new_var = new_vars.push ();
	  যদি (unlikely (new_vars.in_error ()))
	    ফেরত মিথ্যা;
          hb_swap (*new_var, var);
          যদি (unlikely (!m.set (&(new_var->axis_tuples), new_vars.দৈর্ঘ্য - 1)))
            ফেরত মিথ্যা;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      ফেরত সত্য;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    যৌক্তিক compile_all_point_sets ()
    {
      জন্য (ধ্রুবক auto& tuple: tuple_vars)
      {
        ধ্রুবক hb_vector_t<যৌক্তিক>* points_set = &(tuple.indices);
        যদি (point_তথ্য_map.has (points_set))
        {
          unsigned *গুনতি;
          যদি (unlikely (!point_set_গুনতি_map.has (points_set, &গুনতি) ||
                        !point_set_গুনতি_map.set (points_set, (*গুনতি) + 1)))
            ফেরত মিথ্যা;
          চালু;
        }

        hb_vector_t<unsigned অক্ষর> compiled_point_তথ্য;
        যদি (!tuple_delta_t::compile_point_set (*points_set, compiled_point_তথ্য))
          ফেরত মিথ্যা;

        যদি (!point_তথ্য_map.set (points_set, std::move (compiled_point_তথ্য)) ||
            !point_set_গুনতি_map.set (points_set, 1))
          ফেরত মিথ্যা;
      }
      ফেরত সত্য;
    }

    /* find shared points set which saves most bytes */
    শূন্য find_shared_points ()
    {
      unsigned max_saved_bytes = 0;

      জন্য (ধ্রুবক auto& _ : point_তথ্য_map.iter_ref ())
      {
        ধ্রুবক hb_vector_t<যৌক্তিক>* points_set = _.first;
        unsigned তথ্য_দৈর্ঘ্য = _.second.দৈর্ঘ্য;
        যদি (!তথ্য_দৈর্ঘ্য) চালু;
        unsigned *গুনতি;
        যদি (unlikely (!point_set_গুনতি_map.has (points_set, &গুনতি) ||
                      *গুনতি <= 1))
        {
          shared_points_bytes = nullptr;
          ফেরত;
        }

        unsigned saved_bytes = তথ্য_দৈর্ঘ্য * ((*গুনতি) -1);
        যদি (saved_bytes > max_saved_bytes)
        {
          max_saved_bytes = saved_bytes;
          shared_points_bytes = &(_.second);
        }
      }
    }

    যৌক্তিক calc_inferred_deltas (ধ্রুবক contour_point_vector_t& contour_points,
			       hb_vector_t<unsigned> &scratch)
    {
      জন্য (tuple_delta_t& var : tuple_vars)
        যদি (!var.calc_inferred_deltas (contour_points, scratch))
          ফেরত মিথ্যা;

      ফেরত সত্য;
    }

    যৌক্তিক iup_optimize (ধ্রুবক contour_point_vector_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      জন্য (tuple_delta_t& var : tuple_vars)
      {
        যদি (!var.optimize (contour_points, is_composite, scratch))
          ফেরত মিথ্যা;
      }
      ফেরত সত্য;
    }

    সর্বজনীন:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    শূন্য cull_unreachable (ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
    {
      unsigned j = 0;
      জন্য (unsigned i = 0; i < tuple_vars.দৈর্ঘ্য; i++)
      {
	যদি (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  চালু;
	যদি (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    যৌক্তিক instantiate (ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                      ধ্রুবক hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = nullptr,
                      contour_point_vector_t* contour_points = nullptr,
                      যৌক্তিক optimize = মিথ্যা)
    {
      যদি (!tuple_vars) ফেরত সত্য;
      যদি (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        ফেরত মিথ্যা;
      /* compute inferred deltas only for gvar */
      যদি (contour_points)
      {
	hb_vector_t<unsigned> scratch;
	যদি (!calc_inferred_deltas (*contour_points, scratch))
	  ফেরত মিথ্যা;
      }

      /* if iup delta opt is on, contour_points can't be null */
      যদি (optimize && !contour_points)
        ফেরত মিথ্যা;

      যদি (!merge_tuple_variations (optimize ? contour_points : nullptr))
        ফেরত মিথ্যা;

      যদি (optimize && !iup_optimize (*contour_points, scratch)) ফেরত মিথ্যা;
      ফেরত !tuple_vars.in_error ();
    }

    যৌক্তিক compile_bytes (ধ্রুবক hb_map_t& axes_সূচক_map,
                        ধ্রুবক hb_map_t& axes_old_সূচক_tag_map,
                        যৌক্তিক use_shared_points,
                        যৌক্তিক is_gvar = মিথ্যা,
                        ধ্রুবক hb_hashmap_t<ধ্রুবক hb_vector_t<F2DOT14>*, unsigned>* shared_tuples_idx_map = nullptr,
			hb_alloc_pool_t *pool = nullptr)
    {
      // return true for empty glyph
      যদি (!tuple_vars)
        ফেরত সত্য;

      // compile points set and store data in hashmap
      যদি (!compile_all_point_sets ())
        ফেরত মিথ্যা;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_আকার += 4;

      যদি (use_shared_points)
      {
        find_shared_points ();
        যদি (shared_points_bytes)
          compiled_byte_আকার += shared_points_bytes->দৈর্ঘ্য;
      }
      hb_vector_t<পূর্ণসংখ্যা> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      জন্য (auto& tuple: tuple_vars)
      {
        ধ্রুবক hb_vector_t<যৌক্তিক>* points_set = &(tuple.indices);
        hb_vector_t<unsigned অক্ষর> *points_তথ্য;
        যদি (unlikely (!point_তথ্য_map.has (points_set, &points_তথ্য)))
          ফেরত মিথ্যা;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        যদি (!points_তথ্য->দৈর্ঘ্য)
          চালু;
        যদি (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          ফেরত মিথ্যা;

        unsigned points_তথ্য_দৈর্ঘ্য = (points_তথ্য != shared_points_bytes) ? points_তথ্য->দৈর্ঘ্য : 0;
        যদি (!tuple.compile_tuple_var_header (axes_সূচক_map, points_তথ্য_দৈর্ঘ্য, axes_old_সূচক_tag_map,
                                             shared_tuples_idx_map,
					     pool))
          ফেরত মিথ্যা;
        compiled_byte_আকার += tuple.compiled_tuple_header.দৈর্ঘ্য + points_তথ্য_দৈর্ঘ্য + tuple.compiled_deltas.দৈর্ঘ্য;
      }

      যদি (is_gvar && (compiled_byte_আকার % 2))
      {
        needs_padding = সত্য;
        compiled_byte_আকার += 1;
      }

      ফেরত সত্য;
    }

    যৌক্তিক serialize_var_headers (hb_serialize_context_t *c, unsigned& যোগ_header_len) ধ্রুবক
    {
      TRACE_SERIALIZE (এই);
      জন্য (ধ্রুবক auto& tuple: tuple_vars)
      {
        tuple.compiled_tuple_header.as_array ().copy (c);
        যদি (c->in_error ()) return_trace (মিথ্যা);
        যোগ_header_len += tuple.compiled_tuple_header.দৈর্ঘ্য;
      }
      return_trace (সত্য);
    }

    যৌক্তিক serialize_var_তথ্য (hb_serialize_context_t *c, যৌক্তিক is_gvar) ধ্রুবক
    {
      TRACE_SERIALIZE (এই);
      যদি (is_gvar && shared_points_bytes)
      {
        hb_ubytes_t s (shared_points_bytes->arrayZ, shared_points_bytes->দৈর্ঘ্য);
        s.copy (c);
      }

      জন্য (ধ্রুবক auto& tuple: tuple_vars)
      {
        ধ্রুবক hb_vector_t<যৌক্তিক>* points_set = &(tuple.indices);
        hb_vector_t<unsigned অক্ষর> *point_তথ্য;
        যদি (!point_তথ্য_map.has (points_set, &point_তথ্য))
          return_trace (মিথ্যা);

        যদি (!is_gvar || point_তথ্য != shared_points_bytes)
        {
          hb_ubytes_t s (point_তথ্য->arrayZ, point_তথ্য->দৈর্ঘ্য);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        যদি (c->in_error ()) return_trace (মিথ্যা);
      }

      /* padding for gvar */
      যদি (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        যদি (!c->embed (pad)) return_trace (মিথ্যা);
      }
      return_trace (সত্য);
    }
  };

  কাঠামো tuple_iterator_t
  {
    unsigned get_axis_গুনতি () ধ্রুবক { ফেরত axis_গুনতি; }

    শূন্য init (hb_bytes_t var_তথ্য_bytes_2, unsigned পূর্ণসংখ্যা axis_গুনতি_3, ধ্রুবক শূন্য *table_base_)
    {
      var_তথ্য_bytes = var_তথ্য_bytes_2;
      var_তথ্য = var_তথ্য_bytes_2.as<Tuple_Variation_তথ্য> ();
      tuples_বাম = var_তথ্য->tuple_Var_গুনতি.get_গুনতি ();
      axis_গুনতি = axis_গুনতি_3;
      axis_গুনতি_times_2 = axis_গুনতি_3 * 2;
      current_tuple = &var_তথ্য->get_tuple_var_header ();
      তথ্য_offset = 0;
      table_base = table_base_;
    }

    যৌক্তিক get_shared_indices (hb_vector_t<unsigned পূর্ণসংখ্যা> &shared_indices /* OUT */)
    {
      যদি (var_তথ্য->has_shared_point_numbers ())
      {
        ধ্রুবক HBUINT8 *base = &(table_base+var_তথ্য->তথ্য);
        ধ্রুবক HBUINT8 *p = base;
        যদি (!decompile_points (p, shared_indices, (ধ্রুবক HBUINT8 *) (var_তথ্য_bytes.arrayZ + var_তথ্য_bytes.দৈর্ঘ্য))) ফেরত মিথ্যা;
        তথ্য_offset = p - base;
      }
      ফেরত সত্য;
    }

    যৌক্তিক is_valid ()
    {
      যদি (unlikely (tuples_বাম <= 0))
	ফেরত মিথ্যা;

      current_tuple_আকার = TupleVariationHeader::min_আকার;
      যদি (unlikely (!var_তথ্য_bytes.check_end ((ধ্রুবক অক্ষর *) current_tuple + current_tuple_আকার)))
	ফেরত মিথ্যা;

      current_tuple_আকার = current_tuple->get_আকার (axis_গুনতি_times_2);
      যদি (unlikely (!var_তথ্য_bytes.check_end ((ধ্রুবক অক্ষর *) current_tuple + current_tuple_আকার)))
	ফেরত মিথ্যা;

      ফেরত সত্য;
    }

    HB_ALWAYS_INLINE
    যৌক্তিক move_to_পরবর্তী ()
    {
      তথ্য_offset += current_tuple->get_তথ্য_আকার ();
      current_tuple = &StructAtOffset<TupleVariationHeader> (current_tuple, current_tuple_আকার);
      tuples_বাম--;
      ফেরত is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ধ্রুবক HBUINT8 *get_serialized_তথ্য () ধ্রুবক
    { ফেরত &(table_base+var_তথ্য->তথ্য) + তথ্য_offset; }

    ব্যক্তিগত:
    signed tuples_বাম;
    ধ্রুবক Tuple_Variation_তথ্য *var_তথ্য;
    unsigned পূর্ণসংখ্যা axis_গুনতি;
    unsigned পূর্ণসংখ্যা axis_গুনতি_times_2;
    unsigned পূর্ণসংখ্যা তথ্য_offset;
    unsigned পূর্ণসংখ্যা current_tuple_আকার;
    ধ্রুবক শূন্য *table_base;

    সর্বজনীন:
    hb_bytes_t var_তথ্য_bytes;
    ধ্রুবক TupleVariationHeader *current_tuple;
  };

  স্থির যৌক্তিক get_tuple_iterator (hb_bytes_t var_তথ্য_bytes, unsigned axis_গুনতি,
                                  ধ্রুবক শূন্য *table_base,
                                  hb_vector_t<unsigned পূর্ণসংখ্যা> &shared_indices /* OUT */,
                                  tuple_iterator_t *iterator /* OUT */)
  {
    iterator->init (var_তথ্য_bytes, axis_গুনতি, table_base);
    যদি (!iterator->get_shared_indices (shared_indices))
      ফেরত মিথ্যা;
    ফেরত iterator->is_valid ();
  }

  যৌক্তিক has_shared_point_numbers () ধ্রুবক { ফেরত tuple_Var_গুনতি.has_shared_point_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  যৌক্তিক cull_tuple_variations (hb_bytes_t var_তথ্য_bytes,
			      unsigned axis_গুনতি,
			      hb_array_t<ধ্রুবক F2DOT14> shared_tuples,
			      ধ্রুবক hb_map_t *axes_old_সূচক_tag_map,
			      ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			      hb_vector_t<অক্ষর> &out /* OUT */,
			      যৌক্তিক *changed /* OUT */) ধ্রুবক
  {
    *changed = মিথ্যা;

    hb_vector_t<unsigned পূর্ণসংখ্যা> shared_indices;
    tuple_iterator_t iterator;
    যদি (!get_tuple_iterator (var_তথ্য_bytes, axis_গুনতি, এই,
			     shared_indices, &iterator))
      ফেরত সত্য; /* no tuples or malformed: keep original */

    ধ্রুবক অক্ষর *bytes_start = var_তথ্য_bytes.arrayZ;
    ধ্রুবক অক্ষর *bytes_end = bytes_start + var_তথ্য_bytes.দৈর্ঘ্য;
    ধ্রুবক অক্ষর *serialized_base = (ধ্রুবক অক্ষর *) &(এই+তথ্য);
    ধ্রুবক অক্ষর *shared_points_end = (ধ্রুবক অক্ষর *) iterator.get_serialized_তথ্য ();
    যদি (unlikely (serialized_base < bytes_start || serialized_base > bytes_end ||
		  shared_points_end < serialized_base || shared_points_end > bytes_end))
      ফেরত সত্য;

    কাঠামো kept_tuple_t
    {
      ধ্রুবক অক্ষর *header;
      unsigned header_আকার;
      ধ্রুবক অক্ষর *তথ্য;
      unsigned তথ্য_আকার;
    };
    hb_vector_t<kept_tuple_t> kept;
    unsigned যোগ = 0;
    unsigned kept_headers_আকার = 0, kept_তথ্য_আকার = 0;
    করো
    {
      ধ্রুবক TupleVariationHeader *header = iterator.current_tuple;
      unsigned header_আকার = header->get_আকার (axis_গুনতি * 2);
      unsigned তথ্য_আকার = header->get_তথ্য_আকার ();
      ধ্রুবক অক্ষর *tuple_তথ্য = (ধ্রুবক অক্ষর *) iterator.get_serialized_তথ্য ();
      যদি (unlikely (tuple_তথ্য < serialized_base ||
		    তথ্য_আকার > (unsigned) (bytes_end - tuple_তথ্য)))
	ফেরত সত্য; /* malformed: keep original */

      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      যদি (!header->unpack_axis_tuples (axis_গুনতি, shared_tuples,
				       axes_old_সূচক_tag_map, axis_tuples))
	ফেরত সত্য;

      যোগ++;
      যদি (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ধ্রুবক অক্ষর *) header, header_আকার,
				 tuple_তথ্য, তথ্য_আকার});
	kept_headers_আকার += header_আকার;
	kept_তথ্য_আকার += তথ্য_আকার;
      }
    } যখন (iterator.move_to_পরবর্তী ());

    যদি (unlikely (kept.in_error ())) ফেরত মিথ্যা;
    যদি (kept.দৈর্ঘ্য == যোগ) ফেরত সত্য; /* nothing to cull */

    *changed = সত্য;
    যদি (!kept.দৈর্ঘ্য) ফেরত সত্য; /* everything culled: no variation data */

    unsigned shared_points_আকার = shared_points_end - serialized_base;
    unsigned তথ্য_offset = min_আকার + kept_headers_আকার;
    unsigned new_আকার = তথ্য_offset + shared_points_আকার + kept_তথ্য_আকার;
    যদি (unlikely (!out.resize (new_আকার)))
      ফেরত মিথ্যা;

    Tuple_Variation_তথ্য *out_তথ্য = (Tuple_Variation_তথ্য *) out.arrayZ;
    out_তথ্য->tuple_Var_গুনতি = (uint16_t) (kept.দৈর্ঘ্য |
					  (has_shared_point_numbers () ? 0x8000u : 0u));
    out_তথ্য->তথ্য = তথ্য_offset;

    অক্ষর *p = out.arrayZ + min_আকার;
    জন্য (ধ্রুবক auto &t : kept)
    {
      hb_memcpy (p, t.header, t.header_আকার);
      p += t.header_আকার;
    }
    hb_memcpy (p, serialized_base, shared_points_আকার);
    p += shared_points_আকার;
    জন্য (ধ্রুবক auto &t : kept)
    {
      hb_memcpy (p, t.তথ্য, t.তথ্য_আকার);
      p += t.তথ্য_আকার;
    }
    ফেরত সত্য;
  }

  স্থির যৌক্তিক decompile_points (ধ্রুবক HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<unsigned পূর্ণসংখ্যা> &points /* OUT */,
				ধ্রুবক HBUINT8 *end)
  {
    গণনা packed_point_flag_t
    {
      POINTS_ARE_শব্দগুলি     = 0x80,
      POINT_RUN_গুনতি_MASK = 0x7F
    };

    যদি (unlikely (p + 1 > end)) ফেরত মিথ্যা;

    unsigned গুনতি = *p++;
    যদি (গুনতি & POINTS_ARE_শব্দগুলি)
    {
      যদি (unlikely (p + 1 > end)) ফেরত মিথ্যা;
      গুনতি = ((গুনতি & POINT_RUN_গুনতি_MASK) << 8) | *p++;
    }
    যদি (unlikely (!points.resize_dirty  (গুনতি))) ফেরত মিথ্যা;

    unsigned n = 0;
    unsigned i = 0;
    যখন (i < গুনতি)
    {
      যদি (unlikely (p + 1 > end)) ফেরত মিথ্যা;
      unsigned control = *p++;
      unsigned run_গুনতি = (control & POINT_RUN_গুনতি_MASK) + 1;
      unsigned stop = i + run_গুনতি;
      যদি (unlikely (stop > গুনতি)) ফেরত মিথ্যা;
      যদি (control & POINTS_ARE_শব্দগুলি)
      {
        যদি (unlikely (p + run_গুনতি * HBUINT16::static_আকার > end)) ফেরত মিথ্যা;
        জন্য (; i < stop; i++)
        {
          n += *(ধ্রুবক HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_আকার;
        }
      }
      অন্যথা
      {
        যদি (unlikely (p + run_গুনতি > end)) ফেরত মিথ্যা;
        জন্য (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    ফেরত সত্য;
  }

  template <typename T>
  HB_ALWAYS_INLINE
  স্থির যৌক্তিক decompile_deltas (ধ্রুবক HBUINT8 *&p /* IN/OUT */,
				hb_vector_t<T> &deltas /* IN/OUT */,
				ধ্রুবক HBUINT8 *end,
				যৌক্তিক consume_all = মিথ্যা,
				unsigned start = 0)
  {
    ফেরত Tuple_মানসমূহ::decompile (p, deltas, end, consume_all, start);
  }

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত tuple_Var_গুনতি; }

  যৌক্তিক decompile_tuple_variations (unsigned point_গুনতি,
                                   যৌক্তিক is_gvar,
                                   tuple_iterator_t iterator,
                                   ধ্রুবক hb_map_t *axes_old_সূচক_tag_map,
                                   ধ্রুবক hb_vector_t<unsigned> &shared_indices,
                                   ধ্রুবক hb_array_t<ধ্রুবক F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /* OUT */
				   hb_alloc_pool_t *pool = nullptr,
                                   যৌক্তিক is_composite_glyph = মিথ্যা) ধ্রুবক
  {
    ফেরত tuple_variations.create_from_tuple_var_তথ্য (iterator, tuple_Var_গুনতি,
                                                        point_গুনতি, is_gvar,
                                                        axes_old_সূচক_tag_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_composite_glyph);
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
                  যৌক্তিক is_gvar,
                  ধ্রুবক tuple_variations_t& tuple_variations) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    /* empty tuple variations, just return and skip serialization. */
    যদি (!tuple_variations) return_trace (সত্য);

    auto *out = c->start_embed (এই);
    যদি (unlikely (!c->extend_min (out))) return_trace (মিথ্যা);

    যদি (!c->check_assign (out->tuple_Var_গুনতি, tuple_variations.get_var_গুনতি (),
                          HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (মিথ্যা);

    unsigned যোগ_header_len = 0;

    যদি (!tuple_variations.serialize_var_headers (c, যোগ_header_len))
      return_trace (মিথ্যা);

    unsigned তথ্য_offset = min_আকার + যোগ_header_len;
    যদি (!is_gvar) তথ্য_offset += 4;
    যদি (!c->check_assign (out->তথ্য, তথ্য_offset, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (মিথ্যা);

    ফেরত tuple_variations.serialize_var_তথ্য (c, is_gvar);
  }

  সুরক্ষিত:
  কাঠামো Tuple_Var_গুনতি : HBUINT16
  {
    friend কাঠামো tuple_variations_t;
    যৌক্তিক has_shared_point_numbers () ধ্রুবক { ফেরত ((*এই) & SharedPointNumbers); }
    unsigned পূর্ণসংখ্যা get_গুনতি () ধ্রুবক { ফেরত (*এই) & গুনতি_Mask; }
    Tuple_Var_গুনতি& operator = (uint16_t i) { HBUINT16::operator= (i); ফেরত *এই; }
    explicit operator যৌক্তিক () ধ্রুবক { ফেরত get_গুনতি (); }

    সুরক্ষিত:
    গণনা Flags
    {
      SharedPointNumbers= 0x8000u,
      গুনতি_Mask         = 0x0FFFu
    };
    সর্বজনীন:
    DEFINE_আকার_STATIC (2);
  };

  Tuple_Var_গুনতি tuple_Var_গুনতি;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  OffsetTo<HBUINT8, OffType>
                তথ্য;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  সর্বজনীন:
  DEFINE_আকার_MIN (2 + OffType::static_আকার);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
using tuple_variations_t = Tuple_Variation_তথ্য<HBUINT16>::tuple_variations_t;
কাঠামো item_variations_t
{
  using region_t = ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>*;
  ব্যক্তিগত:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_vector_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_vector_t<unsigned> var_তথ্য_অঙ্ক_rows;

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
  hb_vector_t<hb_vector_t<পূর্ণসংখ্যা>> delta_rows;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_vector_t<delta_row_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  যৌক্তিক has_long = মিথ্যা;

  সর্বজনীন:
  যৌক্তিক has_long_word () ধ্রুবক
  { ফেরত has_long; }

  ধ্রুবক hb_vector_t<region_t>& get_region_list () ধ্রুবক
  { ফেরত region_list; }

  ধ্রুবক hb_vector_t<delta_row_encoding_t>& get_vardata_encodings () ধ্রুবক
  { ফেরত encodings; }

  ধ্রুবক hb_map_t& get_varidx_map () ধ্রুবক
  { ফেরত varidx_map; }

  যৌক্তিক add_vardata_encoding_for_testing (hb_vector_t<ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*> &&rows,
                                         unsigned অঙ্ক_cols)
  {
    encodings.push (delta_row_encoding_t (std::move (rows), অঙ্ক_cols));
    ফেরত !encodings.in_error ();
  }

  যৌক্তিক compile_varidx_map_for_testing (ধ্রুবক hb_hashmap_t<unsigned, ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*>& front_mapping)
  { ফেরত compile_varidx_map (front_mapping); }

  যৌক্তিক instantiate (ধ্রুবক ItemVariationStore& varStore,
                    ধ্রুবক hb_subset_plan_t *plan,
                    যৌক্তিক optimize=সত্য,
                    যৌক্তিক use_no_variation_idx=সত্য,
                    ধ্রুবক hb_array_t <ধ্রুবক hb_inc_bimap_t> inner_maps = hb_array_t<ধ্রুবক hb_inc_bimap_t> (),
                    যৌক্তিক layout_conditions = মিথ্যা)
  {
    যদি (!create_from_item_varstore (varStore, plan->axes_old_সূচক_tag_map, inner_maps))
      ফেরত মিথ্যা;
    /* avar2 partial instancing: cull unreachable regions. */
    যদি (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      জন্য (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    যদি (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      ফেরত মিথ্যা;
    যদি (layout_conditions &&
        (!instantiate_condition_মানসমূহ (plan, varStore) || !build_region_list ()))
      ফেরত মিথ্যা;
    ফেরত as_item_varstore (optimize, use_no_variation_idx);
  }

  যৌক্তিক instantiate_condition_মানসমূহ (ধ্রুবক hb_subset_plan_t *plan,
                                      ধ্রুবক ItemVariationStore &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    auto &condition_map = plan->layout_condition_idx_মান_map;
    condition_map.clear ();
    যদি (!plan->layout_condition_মানসমূহ) ফেরত সত্য;
    using condition_মান_t = hb_pair_t<uintptr_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>>;
    hb_vector_t<condition_মান_t> conditions;
    জন্য (auto condition : plan->layout_condition_মানসমূহ.iter ())
      conditions.push (condition);
    যদি (unlikely (conditions.in_error ())) ফেরত মিথ্যা;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ধ্রুবক condition_মান_t &a, ধ্রুবক condition_মান_t &b) {
      যদি (a.second.first != b.second.first)
        ফেরত a.second.first < b.second.first ? -1 : 1;
      ফেরত a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    unsigned new_outer = 0;
    জন্য (unsigned i = 0; i < plan->gdef_varstore_inner_maps.দৈর্ঘ্য; i++)
      যদি (plan->gdef_varstore_inner_maps[i].get_population ())
        যদি (!outer_map.set (i, new_outer++)) ফেরত মিথ্যা;
    জন্য (auto condition : conditions)
    {
      unsigned old_idx = condition.second.first;
      দ্বিগুণ value = condition.second.second +
                     varStore.get_delta<দ্বিগুণ> (old_idx, plan->normalized_coords.as_array ());
      unsigned old_outer = old_idx >> 16;
      যদি (!outer_map.has (old_outer))
        চালু;
      unsigned outer = outer_map.get (old_outer);
      unsigned inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      unsigned idx = (outer << 16) | inner;
      যদি (outer >= vars.দৈর্ঘ্য || inner >= var_তথ্য_অঙ্ক_rows[outer])
        চালু;

      দ্বিগুণ maximum = fabs ((দ্বিগুণ) value);
      যৌক্তিক variable = মিথ্যা;
      জন্য (ধ্রুবক tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        যদি (unlikely (inner >= tuple.deltas_x.দৈর্ঘ্য)) ফেরত মিথ্যা;
        দশমিক delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        maximum = hb_max (maximum, fabs ((দ্বিগুণ) delta));
      }
      যদি (!variable)
      {
        যদি (!condition_map.set (condition.first,
                                hb_pair (unsigned (VarIdx::NO_VARIATION), পূর্ণসংখ্যা (value > 0))))
          ফেরত মিথ্যা;
        চালু;
      }

      দ্বিগুণ scale = 1.;
      যখন (maximum * scale > INT_MAX)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      যখন (maximum * scale * 2 <= INT_MAX)
      {
        যৌক্তিক integral = (দ্বিগুণ) value * scale == round ((দ্বিগুণ) value * scale);
        জন্য (ধ্রুবক tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (দ্বিগুণ) tuple.deltas_x[inner] * scale ==
                      round ((দ্বিগুণ) tuple.deltas_x[inner] * scale);
        যদি (integral) বিরতি;
        scale *= 2;
      }
      পূর্ণসংখ্যা default_মান = (পূর্ণসংখ্যা) round ((দ্বিগুণ) value * scale);
      যদি (scale != 1. || default_মান < -32768 || default_মান > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t private_row;
        জন্য (ধ্রুবক tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          যদি (!tuple.deltas_x[inner]) চালু;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (সত্য);
          copy.deltas_x.push ((দশমিক) round ((দ্বিগুণ) tuple.deltas_x[inner] * scale));
          যদি (unlikely (copy.axis_tuples.in_error () || copy.indices.in_error () ||
                        copy.deltas_x.in_error ())) ফেরত মিথ্যা;
          private_row.tuple_vars.push (std::move (copy));
        }
        যদি (unlikely (vars.দৈর্ঘ্য >= 0xFFFFu)) ফেরত মিথ্যা;
        যদি (unlikely (private_row.tuple_vars.in_error ())) ফেরত মিথ্যা;
        idx = vars.দৈর্ঘ্য << 16;
        vars.push (std::move (private_row));
        var_তথ্য_অঙ্ক_rows.push (1);
        যদি (unlikely (vars.in_error () || var_তথ্য_অঙ্ক_rows.in_error ())) ফেরত মিথ্যা;
        যদি (default_মান < -32768 || default_মান > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          পূর্ণসংখ্যা bias = default_মান - hb_clamp (default_মান, -32768, 32767);
          default_মান -= bias;
          পূর্ণসংখ্যা উপরের = bias / 65536 * 65536;
          পূর্ণসংখ্যা parts[] = {উপরের, bias - উপরের};
          জন্য (পূর্ণসংখ্যা part : parts)
          {
            যদি (!part) চালু;
            unsigned গুনতি = vars[idx >> 16].tuple_vars.দৈর্ঘ্য;
            add_tuple (idx >> 16, hb_hashmap_t<hb_tag_t, Triple> (),
                       0, part, 1);
            ধ্রুবক auto &tuples = vars[idx >> 16].tuple_vars;
            যদি (unlikely (tuples.in_error () || tuples.দৈর্ঘ্য != গুনতি + 1 ||
                          tuples.লেজ ().indices.in_error () ||
                          tuples.লেজ ().deltas_x.in_error ())) ফেরত মিথ্যা;
          }
        }
      }
      যদি (!condition_map.set (condition.first, hb_pair (idx, default_মান)))
        ফেরত মিথ্যা;
    }
    ফেরত !condition_map.in_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  যৌক্তিক create_from_item_varstore (ধ্রুবক ItemVariationStore& varStore,
                                  ধ্রুবক hb_map_t& axes_old_সূচক_tag_map,
                                  ধ্রুবক hb_array_t <ধ্রুবক hb_inc_bimap_t> inner_maps = hb_array_t<ধ্রুবক hb_inc_bimap_t> ())
  {
    ধ্রুবক VarRegionList& regionList = varStore.get_region_list ();
    যদি (!regionList.get_var_regions (axes_old_সূচক_tag_map, orig_region_list))
      ফেরত মিথ্যা;

    unsigned অঙ্ক_var_তথ্য = varStore.get_sub_table_গুনতি ();
    যদি (inner_maps && inner_maps.দৈর্ঘ্য != অঙ্ক_var_তথ্য) ফেরত মিথ্যা;
    যদি (!vars.alloc (অঙ্ক_var_তথ্য) ||
        !var_তথ্য_অঙ্ক_rows.alloc (অঙ্ক_var_তথ্য)) ফেরত মিথ্যা;

    জন্য (unsigned i = 0; i < অঙ্ক_var_তথ্য; i++)
    {
      যদি (inner_maps && !inner_maps.arrayZ[i].get_population ())
          চালু;
      tuple_variations_t var_তথ্য_tuples;
      unsigned item_গুনতি_2 = 0;
      যদি (!var_তথ্য_tuples.create_from_item_var_তথ্য (varStore.get_sub_table (i),
                                                      orig_region_list,
                                                      axes_old_সূচক_tag_map,
                                                      item_গুনতি_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : nullptr))
        ফেরত মিথ্যা;

      var_তথ্য_অঙ্ক_rows.push (item_গুনতি_2);
      vars.push (std::move (var_তথ্য_tuples));
    }
    ফেরত !vars.in_error () && !var_তথ্য_অঙ্ক_rows.in_error () && vars.দৈর্ঘ্য == var_তথ্য_অঙ্ক_rows.দৈর্ঘ্য;
  }

  যৌক্তিক instantiate_tuple_vars (ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location,
                               ধ্রুবক hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances,
                               যৌক্তিক build_regions = সত্য)
  {
    optimize_scratch_t scratch;
    জন্য (tuple_variations_t& tuple_vars : vars)
      যদি (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        ফেরত মিথ্যা;

    ফেরত !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  unsigned add_vardata (unsigned item_গুনতি_2)
  {
    vars.push (tuple_variations_t ());
    var_তথ্য_অঙ্ক_rows.push (item_গুনতি_2);
    ফেরত vars.দৈর্ঘ্য - 1;
  }

  /* Get item count for a VarData subtable. */
  unsigned get_item_গুনতি (unsigned outer) ধ্রুবক
  { ফেরত outer < var_তথ্য_অঙ্ক_rows.দৈর্ঘ্য ? var_তথ্য_অঙ্ক_rows[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  unsigned duplicate_row (unsigned outer, unsigned inner)
  {
    যদি (unlikely (outer >= vars.দৈর্ঘ্য ||
                  inner >= var_তথ্য_অঙ্ক_rows[outer] ||
                  var_তথ্য_অঙ্ক_rows[outer] >= 0xFFFFu))
      ফেরত (unsigned) -1;
    জন্য (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      যদি (unlikely (inner >= tuple.deltas_x.দৈর্ঘ্য ||
                    inner >= tuple.indices.দৈর্ঘ্য))
        ফেরত (unsigned) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      যদি (inner < tuple.deltas_y.দৈর্ঘ্য)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      যদি (unlikely (tuple.indices.in_error () ||
                    tuple.deltas_x.in_error () ||
                    tuple.deltas_y.in_error ()))
        ফেরত (unsigned) -1;
    }
    ফেরত var_তথ্য_অঙ্ক_rows[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  শূন্য add_tuple (unsigned outer,
                  hb_hashmap_t<hb_tag_t, Triple>&& axis_tuples,
                  unsigned inner, পূর্ণসংখ্যা delta, unsigned item_গুনতি_2)
  {
    যদি (outer >= vars.দৈর্ঘ্য) ফেরত;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    যদি (!tuple.indices.resize (item_গুনতি_2) ||
        !tuple.deltas_x.resize (item_গুনতি_2)) ফেরত;
    জন্য (unsigned i = 0; i < item_গুনতি_2; i++)
    {
      tuple.indices.arrayZ[i] = সত্য;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (দশমিক) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  যৌক্তিক build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, unsigned> all_regions;
    hb_hashmap_t<region_t, unsigned> used_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_vector_t<region_t> all_unique_regions;
    জন্য (ধ্রুবক tuple_variations_t& sub_table : vars)
    {
      জন্য (ধ্রুবক tuple_delta_t& tuple : sub_table.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        যদি (!used_regions.has (r))
        {
          যৌক্তিক all_zeros = সত্য;
          জন্য (দশমিক d : tuple.deltas_x)
          {
            পূর্ণসংখ্যা delta = hb_clamp_to<পূর্ণসংখ্যা> (roundf ((দ্বিগুণ) d));
            যদি (delta != 0)
            {
              all_zeros = মিথ্যা;
              বিরতি;
            }
          }
          যদি (!all_zeros)
          {
            যদি (!used_regions.set (r, 1))
              ফেরত মিথ্যা;
          }
        }
        যদি (all_regions.has (r))
          চালু;
        যদি (!all_regions.set (r, 1))
          ফেরত মিথ্যা;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    যদি (!all_regions || !all_unique_regions) ফেরত সত্য;

    যদি (!region_list.alloc (all_regions.get_population ()))
      ফেরত মিথ্যা;

    unsigned idx = 0;
    /* append the original regions that pre-existed */
    জন্য (ধ্রুবক auto& r : orig_region_list)
    {
      যদি (!all_regions.has (&r) || !used_regions.has (&r))
        চালু;

      region_list.push (&r);
      যদি (!region_map.set (&r, idx))
        ফেরত মিথ্যা;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    জন্য (ধ্রুবক auto& r: all_unique_regions)
    {
      যদি (!all_regions.has (r) || !used_regions.has (r))
        চালু;
      region_list.push (r);
      যদি (!region_map.set (r, idx))
        ফেরত মিথ্যা;
      all_regions.del (r);
      idx++;
    }
    ফেরত (!region_list.in_error ()) && (!region_map.in_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  কাঠামো combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = পূর্বনির্ধারিত;
    combined_gain_idx_tuple_t (unsigned gain, unsigned i, unsigned j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      প্রতিজ্ঞা (gain < 0xFFFFFF);
      প্রতিজ্ঞা (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    যৌক্তিক operator < (ধ্রুবক combined_gain_idx_tuple_t& o)
    {
      ফেরত encoded < o.encoded;
    }

    যৌক্তিক operator <= (ধ্রুবক combined_gain_idx_tuple_t& o)
    {
      ফেরত encoded <= o.encoded;
    }

    unsigned idx_1 () ধ্রুবক { ফেরত (encoded >> 20) & 0xFFFFF; };
    unsigned idx_2 () ধ্রুবক { ফেরত encoded & 0xFFFFF; };
  };

  যৌক্তিক as_item_varstore (যৌক্তিক optimize=সত্য, যৌক্তিক use_no_variation_idx=সত্য)
  {
    /* return true if no variation data */
    যদি (!region_list) ফেরত সত্য;
    unsigned অঙ্ক_cols = region_list.দৈর্ঘ্য;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    unsigned যোগ_rows = 0;
    জন্য (unsigned major = 0; major < var_তথ্য_অঙ্ক_rows.দৈর্ঘ্য; major++)
      যোগ_rows += var_তথ্য_অঙ্ক_rows[major];

    যদি (!delta_rows.resize (যোগ_rows)) ফেরত মিথ্যা;
    /* init all rows to [0]*num_cols */
    জন্য (unsigned i = 0; i < যোগ_rows; i++)
      যদি (!(delta_rows[i].resize (অঙ্ক_cols))) ফেরত মিথ্যা;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<unsigned, ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*> front_mapping;
    unsigned start_row = 0;
    hb_vector_t<delta_row_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_vector_t<ধ্রুবক hb_vector_t<পূর্ণসংখ্যা> *> major_rows;
    hb_hashmap_t<ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*, unsigned> delta_rows_map;
    জন্য (unsigned major = 0; major < vars.দৈর্ঘ্য; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ধ্রুবক tuple_variations_t& tuples = vars[major];
      unsigned অঙ্ক_rows = var_তথ্য_অঙ্ক_rows[major];

      যদি (!অঙ্ক_rows) চালু;

      জন্য (ধ্রুবক tuple_delta_t& tuple: tuples.tuple_vars)
      {
        যদি (tuple.deltas_x.দৈর্ঘ্য != অঙ্ক_rows)
          ফেরত মিথ্যা;

        /* skip unused regions */
        unsigned *col_idx;
        যদি (!region_map.has (&(tuple.axis_tuples), &col_idx))
          চালু;

        জন্য (unsigned i = 0; i < অঙ্ক_rows; i++)
        {
          পূর্ণসংখ্যা rounded_delta = hb_clamp_to<পূর্ণসংখ্যা> (roundf ((দ্বিগুণ) tuple.deltas_x[i]));
          পূর্ণসংখ্যা &delta = delta_rows[start_row + i][*col_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          যদি (unlikely (sum < INT_MIN || sum > INT_MAX)) ফেরত মিথ্যা;
          delta = (পূর্ণসংখ্যা) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_rows.reset ();
      জন্য (unsigned minor = 0; minor < অঙ্ক_rows; minor++)
      {
	ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>& row = delta_rows[start_row + minor];
	যদি (use_no_variation_idx)
	{
	  যৌক্তিক all_zeros = সত্য;
	  জন্য (পূর্ণসংখ্যা delta : row)
	  {
	    যদি (delta != 0)
	    {
	      all_zeros = মিথ্যা;
	      বিরতি;
	    }
	  }
	  যদি (all_zeros)
	    চালু;
	}

	যদি (!front_mapping.set ((major<<16) + minor, &row))
	  ফেরত মিথ্যা;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	যদি (optimize)
	{
	  যদি (delta_rows_map.has (&row))
	    চালু;

	  delta_rows_map.set (&row, 1);
	}

	major_rows.push (&row);
      }

      যদি (unlikely (major_rows.in_error () || delta_rows_map.in_error ()))
        ফেরত মিথ্যা;
      যদি (major_rows)
      {
	auto *encoding = encoding_objs.push (std::move (major_rows), অঙ্ক_cols);
	যদি (unlikely (encoding_objs.in_error () || encoding->in_error ()))
	  ফেরত মিথ্যা;
      }

      start_row += অঙ্ক_rows;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    যদি (!optimize)
    {
      encodings = std::move (encoding_objs);
      ফেরত !encodings.in_error ();
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
    hb_vector_t<item_t> queue_বস্তুসমূহ;
    unsigned অঙ্ক_todos = encoding_objs.দৈর্ঘ্য;
    জন্য (unsigned i = 0; i < অঙ্ক_todos; i++)
    {
      জন্য (unsigned j = i + 1; j < অঙ্ক_todos; j++)
      {
        পূর্ণসংখ্যা combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        যদি (combining_gain > 0)
          queue_বস্তুসমূহ.push (item_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_priority_queue_t<combined_gain_idx_tuple_t> queue (std::move (queue_বস্তুসমূহ));

    hb_bit_set_t removed_todo_idxes;
    যখন (queue)
    {
      auto t = queue.বের_করো_minimum ().first;
      unsigned i = t.idx_1 ();
      unsigned j = t.idx_2 ();

      যদি (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        চালু;

      delta_row_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_row_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.add (i);
      removed_todo_idxes.add (j);
      যদি (unlikely (removed_todo_idxes.in_error ())) ফেরত মিথ্যা;

      encoding.merge (other_encoding);
      যদি (unlikely (encoding.in_error ())) ফেরত মিথ্যা;

      জন্য (unsigned idx = 0; idx < encoding_objs.দৈর্ঘ্য; idx++)
      {
        যদি (removed_todo_idxes.has (idx)) চালু;

        ধ্রুবক delta_row_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        যদি (obj.width == encoding.width && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          জন্য (ধ্রুবক auto& row : obj.বস্তুসমূহ)
            encoding.add_row (row);
          যদি (unlikely (encoding.in_error ())) ফেরত মিথ্যা;

          removed_todo_idxes.add (idx);
          যদি (unlikely (removed_todo_idxes.in_error ())) ফেরত মিথ্যা;
          চালু;
        }

        পূর্ণসংখ্যা combined_gain = encoding.gain_from_merging (obj);
        যদি (combined_gain > 0)
          queue.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.দৈর্ঘ্য), 0);
      }

      auto moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      যদি (unlikely (encoding_objs.in_error ())) ফেরত মিথ্যা;
    }

    পূর্ণসংখ্যা অঙ্ক_final_encodings = (পূর্ণসংখ্যা) encoding_objs.দৈর্ঘ্য - (পূর্ণসংখ্যা) removed_todo_idxes.get_population ();
    যদি (অঙ্ক_final_encodings <= 0) ফেরত মিথ্যা;

    যদি (!encodings.alloc (অঙ্ক_final_encodings)) ফেরত মিথ্যা;
    জন্য (unsigned i = 0; i < encoding_objs.দৈর্ঘ্য; i++)
    {
      যদি (removed_todo_idxes.has (i)) চালু;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    ফেরত compile_varidx_map (front_mapping);
  }

  ব্যক্তিগত:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  যৌক্তিক compile_varidx_map (ধ্রুবক hb_hashmap_t<unsigned, ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*, unsigned> back_mapping;
    hb_vector_t<delta_row_encoding_t> split_encodings;

    জন্য (unsigned i = 0; i < encodings.দৈর্ঘ্য; i++)
    {
      delta_row_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      যদি (encoding.is_empty () || encoding.in_error ())
        ফেরত মিথ্যা;

      unsigned অঙ্ক_rows = encoding.বস্তুসমূহ.দৈর্ঘ্য;
      unsigned অঙ্ক_cols = encoding.chars.দৈর্ঘ্য;

      /* sort rows, make result deterministic */
      encoding.বস্তুসমূহ.qsort (_cmp_row);

      জন্য (unsigned start = 0; start < অঙ্ক_rows; start += 0xFFFFu)
      {
        unsigned chunk_len = hb_min (অঙ্ক_rows - start, 0xFFFFu);
        hb_vector_t<ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*> rows;

        যদি (!rows.alloc (chunk_len))
          ফেরত মিথ্যা;

        unsigned major = split_encodings.দৈর্ঘ্য;
        জন্য (unsigned minor = 0; minor < chunk_len; minor++)
        {
          ধ্রুবক hb_vector_t<পূর্ণসংখ্যা> *row = encoding.বস্তুসমূহ.arrayZ[start + minor];
          rows.push (row);
          যদি (!back_mapping.set (row, (major << 16) + minor))
            ফেরত মিথ্যা;
        }

        auto *split = split_encodings.push (std::move (rows), অঙ্ক_cols);
        যদি (unlikely (split_encodings.in_error () || split->in_error ()))
          ফেরত মিথ্যা;
      }
    }

    encodings = std::move (split_encodings);
    যদি (encodings.in_error () || back_mapping.in_error ())
      ফেরত মিথ্যা;

    জন্য (auto _ : front_mapping.iter ())
    {
      unsigned old_varidx = _.first;
      unsigned *new_varidx;
      যদি (back_mapping.has (_.second, &new_varidx))
        varidx_map.set (old_varidx, *new_varidx);
      অন্যথা
        varidx_map.set (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_সূচক);
    }
    ফেরত !varidx_map.in_error ();
  }

  স্থির পূর্ণসংখ্যা _cmp_row (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>** a = (ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>**) pa;
    ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>** b = (ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>**) pb;

    জন্য (unsigned i = 0; i < (*b)->দৈর্ঘ্য; i++)
    {
      পূর্ণসংখ্যা va = (*a)->arrayZ[i];
      পূর্ণসংখ্যা vb = (*b)->arrayZ[i];
      যদি (va != vb)
        ফেরত va < vb ? -1 : 1;
    }
    ফেরত 0;
  }
};


} /* namespace OT */


#endif /* HB_OT_VAR_COMMON_HH */

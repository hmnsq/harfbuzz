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

#که_نه_تعریف_شوی HB_OT_VAR_COMMON_HH
#تعریف_کړه HB_OT_VAR_COMMON_HH

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-alloc-pool.hh"
#ضمن "hb-priority-queue.hh"
#ضمن "hb-subset-instancer-iup.hh"


نطاق OT {

کارول rebase_tent_پایله_scratch_t = hb_pair_t<rebase_tent_پایله_t, rebase_tent_پایله_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
بنية Tuple_Variation_سرلیک
{
  ملګری بنية tuple_delta_t;
  size_t get_اندازه (بینښه axis_شمېر_times_2) ثابت
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    بینښه ti = tuple_شاخص;
    إذا (unlikely ((ti & (Tuple_شاخص::EmbeddedPeakTuple | Tuple_شاخص::IntermediateRegion))))
    {
      بینښه شمېر = ((ti & Tuple_شاخص::EmbeddedPeakTuple) != 0) + ((ti & Tuple_شاخص::IntermediateRegion) != 0) * 2;
      أرجع hb_unsigned_mul_جمع_کړه_saturate (شمېر, axis_شمېر_times_2, min_اندازه);
    }
    أرجع min_اندازه;
  }

  بینښه get_معلومات_اندازه () ثابت { أرجع var_معلومات_اندازه; }

  ثابت Tuple_Variation_سرلیک &get_بل (بینښه axis_شمېر_times_2) ثابت
  { أرجع Struct_At_دننه_واټن<Tuple_Variation_سرلیک> (هذا, get_اندازه (axis_شمېر_times_2)); }

  منطقي unpack_axis_tuples (بینښه axis_شمېر,
                           ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                           ثابت hb_map_t *axes_old_شاخص_نښان_map,
                           hb_hashmap_t<hb_نښان_t, Triple>& axis_tuples /*بهر*/) ثابت
  {
    ثابت F2DOT14 *peak_tuple = تش_اشاره;
    إذا (has_peak ())
      peak_tuple = get_peak_tuple (axis_شمېر);
    وإلا
    {
      بینښه صحيح شاخص_2 = get_شاخص ();
      إذا (unlikely ((شاخص_2 + 1) * axis_شمېر > shared_tuples.اوږدتیا))
        أرجع خطأ;
      peak_tuple = shared_tuples.sub_array (axis_شمېر * شاخص_2, axis_شمېر).arrayZ;
    }

    ثابت F2DOT14 *پیل_tuple = تش_اشاره;
    ثابت F2DOT14 *پای_ټکی_tuple = تش_اشاره;
    منطقي has_interm = has_intermediate ();

    إذا (has_interm)
    {
      پیل_tuple = get_پیل_tuple (axis_شمېر);
      پای_ټکی_tuple = get_پای_ټکی_tuple (axis_شمېر);
    }

    لكل (بینښه i = 0; i < axis_شمېر; i++)
    {
      عائم peak = peak_tuple[i].منزل_float ();
      إذا (peak == 0.f) استمر;

      hb_نښان_t *axis_نښان_2;
      إذا (!axes_old_شاخص_نښان_map->has (i, &axis_نښان_2))
        أرجع خطأ;

      عائم پیل, پای_ټکی;
      إذا (has_interm)
      {
        پیل = پیل_tuple[i].منزل_float ();
        پای_ټکی = پای_ټکی_tuple[i].منزل_float ();
      }
      وإلا
      {
        پیل = hb_min (peak, 0.f);
        پای_ټکی = hb_max (peak, 0.f);
      }
      axis_tuples.سېټ (*axis_نښان_2, Triple (پیل, peak, پای_ټکی));
    }

    أرجع ریښتیا;
  }

  HB_ALWAYS_INLINE
  عائم calculate_scalar (hb_array_t<ثابت صحيح> coords, بینښه صحيح coord_شمېر,
			   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ثابت
  {
    بینښه tuple_شاخص_2 = tuple_شاخص;
    بینښه صحيح شاخص_2 = tuple_شاخص_2 & Tuple_شاخص::Tuple_شاخص_ماسک;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    منطقي plain_shared_tuple = (tuple_شاخص_2 & ~Tuple_شاخص::شخصي_ټکی_Numbers) < shared_tuple_scalar_cache->اوږدتیا;
    إذا (likely (plain_shared_tuple))
    {
      عائم scalar;
      إذا (likely (shared_tuple_scalar_cache->get (شاخص_2, &scalar)))
	أرجع scalar;
    }

    ثابت F2DOT14 *peak_tuple;

    منطقي has_interm = tuple_شاخص_2 & Tuple_شاخص::IntermediateRegion; // Inlined for performance

    إذا (unlikely (tuple_شاخص_2 & Tuple_شاخص::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_شمېر);
      shared_tuple_scalar_cache = تش_اشاره;
    }
    وإلا
    {
      عائم scalar;
      إذا (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->اوږدتیا &&
	  shared_tuple_scalar_cache->get (شاخص_2, &scalar))
      {
        إذا (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = تش_اشاره;
	وإلا
	  أرجع scalar;
      }

      إذا (unlikely ((شاخص_2 + 1) * coord_شمېر > shared_tuples.اوږدتیا))
        أرجع 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_شمېر * شاخص_2);

    }

    ثابت F2DOT14 *پیل_tuple = تش_اشاره;
    ثابت F2DOT14 *پای_ټکی_tuple = تش_اشاره;

    إذا (has_interm)
    {
      پیل_tuple = get_پیل_tuple (coord_شمېر);
      پای_ټکی_tuple = get_پای_ټکی_tuple (coord_شمېر);
    }

    عائم scalar = 1.f;
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
#إذا HB_FAST_عدد_ACCESS
    منطقي skip = coord_شمېر >= 16;
#د_که_پای
#د_که_پای
    لكل (بینښه صحيح i = 0; i < coord_شمېر; i++)
    {
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
#إذا HB_FAST_عدد_ACCESS
      إذا (skip)
      {
	طالما (i + 4 <= coord_شمېر && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	طالما (i < coord_شمېر && peak_tuple[i].منزل_int () == 0)
	  i += 1;
	إذا (i >= coord_شمېر)
	  اكسر;
      }
#د_که_پای
#د_که_پای

      صحيح peak = peak_tuple[i].منزل_int ();
      إذا (!peak) استمر;

      صحيح v = coords[i];
      إذا (!v) { scalar = 0.f; اكسر; }
      إذا (v == peak) استمر;

      إذا (has_interm)
      {
	shared_tuple_scalar_cache = تش_اشاره;
        صحيح پیل = پیل_tuple[i].منزل_int ();
        صحيح پای_ټکی = پای_ټکی_tuple[i].منزل_int ();
        إذا (unlikely (پیل > peak || peak > پای_ټکی ||
                      (پیل < 0 && پای_ټکی > 0 && peak))) استمر;
        إذا (v < پیل || v > پای_ټکی) { scalar = 0.f; اكسر; }
        إذا (v < peak)
        { إذا (peak != پیل) scalar *= (عائم) (v - پیل) / (peak - پیل); }
        وإلا
        { إذا (peak != پای_ټکی) scalar *= (عائم) (پای_ټکی - v) / (پای_ټکی - peak); }
      }
      وإلا إذا (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; اكسر; }
      وإلا
        scalar *= (عائم) v / peak;
    }
    إذا (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->سېټ (شاخص_2, scalar);
    أرجع scalar;
  }

  منطقي           has_peak () ثابت { أرجع tuple_شاخص & Tuple_شاخص::EmbeddedPeakTuple; }
  منطقي   has_intermediate () ثابت { أرجع tuple_شاخص & Tuple_شاخص::IntermediateRegion; }
  منطقي has_شخصي_points () ثابت { أرجع tuple_شاخص & Tuple_شاخص::شخصي_ټکی_Numbers; }
  بینښه      get_شاخص () ثابت { أرجع tuple_شاخص & Tuple_شاخص::Tuple_شاخص_ماسک; }
  ساكن constexpr بینښه max_shared_tuple_شمېر = 0x1000u;

  محمي:
  بنية Tuple_شاخص : HBUINT16
  {
    تعداد Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      شخصي_ټکی_Numbers = 0x2000u,
      Tuple_شاخص_ماسک      = 0x0FFFu
    };

    Tuple_شاخص& چلوونکی = (uint16_t i) { HBUINT16::چلوونکی= (i); أرجع *هذا; }
    DEFINE_اندازه_STATIC (2);
  };

  hb_array_t<ثابت F2DOT14> get_all_tuples (بینښه axis_شمېر) ثابت
  { أرجع StructAfter<Unsized_Array_د<F2DOT14>> (tuple_شاخص).as_array ((has_peak () + has_intermediate () * 2) * axis_شمېر); }
  ثابت F2DOT14* get_all_tuples_بنسټ (بینښه axis_شمېر) ثابت
  { أرجع StructAfter<Unsized_Array_د<F2DOT14>> (tuple_شاخص).arrayZ; }
  ثابت F2DOT14* get_peak_tuple (بینښه axis_شمېر) ثابت
  { أرجع get_all_tuples_بنسټ (axis_شمېر); }
  ثابت F2DOT14* get_پیل_tuple (بینښه axis_شمېر) ثابت
  { أرجع get_all_tuples_بنسټ (axis_شمېر) + has_peak () * axis_شمېر; }
  ثابت F2DOT14* get_پای_ټکی_tuple (بینښه axis_شمېر) ثابت
  { أرجع get_all_tuples_بنسټ (axis_شمېر) + has_peak () * axis_شمېر + axis_شمېر; }

  HBUINT16      var_معلومات_اندازه;    /* The size in bytes of the serialized
                                 * data for this tuple variation table. */
  Tuple_شاخص    tuple_شاخص;     /* A packed field. The high 4 bits are flags (see below).
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
  DEFINE_اندازه_MIN (4);
};

بنية optimize_scratch_t
{
  iup_scratch_t iup;
  hb_ویکټور_t<منطقي> opt_indices;
  hb_ویکټور_t<صحيح> rounded_x_deltas;
  hb_ویکټور_t<صحيح> rounded_y_deltas;
  hb_ویکټور_t<عائم> opt_deltas_x;
  hb_ویکټور_t<عائم> opt_deltas_y;
  hb_ویکټور_t<بینښه حرف> opt_ټکی_معلومات;
  hb_ویکټور_t<بینښه حرف> opt_deltas_معلومات;
  hb_ویکټور_t<بینښه حرف> ټکی_معلومات;
  hb_ویکټور_t<بینښه حرف> deltas_معلومات;
  hb_ویکټور_t<صحيح> rounded_deltas;
};

بنية tuple_delta_t
{
  ساكن constexpr منطقي realloc_move = ریښتیا;  // Watch out when adding new members!

  عام:
  hb_hashmap_t<hb_نښان_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_ویکټور_t<منطقي> indices;

  hb_ویکټور_t<عائم> deltas_x;
  /* empty for cvar tuples */
  hb_ویکټور_t<عائم> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_ویکټور_t<بینښه حرف> compiled_tuple_سرلیک;
  hb_ویکټور_t<بینښه حرف> compiled_deltas;

  hb_ویکټور_t<F2DOT14> compiled_peak_coords;
  hb_ویکټور_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = تش_اشاره) {}
  tuple_delta_t (ثابت tuple_delta_t& o) = افتراضي;
  tuple_delta_t& چلوونکی = (ثابت tuple_delta_t& o) = افتراضي;

  ملګری فراغ swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
  {
    hb_بدلون (a.axis_tuples, b.axis_tuples);
    hb_بدلون (a.indices, b.indices);
    hb_بدلون (a.deltas_x, b.deltas_x);
    hb_بدلون (a.deltas_y, b.deltas_y);
    hb_بدلون (a.compiled_tuple_سرلیک, b.compiled_tuple_سرلیک);
    hb_بدلون (a.compiled_deltas, b.compiled_deltas);
    hb_بدلون (a.compiled_peak_coords, b.compiled_peak_coords);
  }

  tuple_delta_t (tuple_delta_t&& o)  noexcept : tuple_delta_t ()
  { hb_بدلون (*هذا, o); }

  tuple_delta_t& چلوونکی = (tuple_delta_t&& o) noexcept
  {
    hb_بدلون (*هذا, o);
    أرجع *هذا;
  }

  فراغ copy_from (ثابت tuple_delta_t& o, hb_alloc_pool_t *pool = تش_اشاره)
  {
    axis_tuples = o.axis_tuples;
    indices.duplicate_ویکټور_from_pool (pool, o.indices);
    deltas_x.duplicate_ویکټور_from_pool (pool, o.deltas_x);
    deltas_y.duplicate_ویکټور_from_pool (pool, o.deltas_y);
    compiled_tuple_سرلیک.duplicate_ویکټور_from_pool (pool, o.compiled_tuple_سرلیک);
    compiled_deltas.duplicate_ویکټور_from_pool (pool, o.compiled_deltas);
    compiled_peak_coords.duplicate_ویکټور_from_pool (pool, o.compiled_peak_coords);
    compiled_interm_coords.duplicate_ویکټور_from_pool (pool, o.compiled_interm_coords);
  }

  فراغ remove_axis (hb_نښان_t axis_نښان_2)
  { axis_tuples.del (axis_نښان_2); }

  منطقي سېټ_tent (hb_نښان_t axis_نښان_2, Triple tent)
  { أرجع axis_tuples.سېټ (axis_نښان_2, tent); }

  tuple_delta_t& چلوونکی += (ثابت tuple_delta_t& o)
  {
    بینښه عدد = indices.اوږدتیا;
    لكل (بینښه i = 0; i < عدد; i++)
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
        indices.arrayZ[i] = ریښتیا;
        deltas_x[i] = o.deltas_x[i];
        إذا (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    أرجع *هذا;
  }

  tuple_delta_t& چلوونکی *= (عائم scalar)
  {
    إذا (scalar == 1.0f)
      أرجع *هذا;

    بینښه عدد = indices.اوږدتیا;
    إذا (deltas_y)
      لكل (بینښه i = 0; i < عدد; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    وإلا
      لكل (بینښه i = 0; i < عدد; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
      }
    أرجع *هذا;
  }

  فراغ change_tuple_var_axis_حد (hb_نښان_t axis_نښان_2, Triple axis_حد,
				    TripleDistances axis_triple_distances,
				    hb_ویکټور_t<tuple_delta_t>& بهر,
				    rebase_tent_پایله_scratch_t &scratch,
				    hb_alloc_pool_t *pool = تش_اشاره)
  {
    // May move *this out.

    بهر.reset ();
    Triple *tent;
    إذا (!axis_tuples.has (axis_نښان_2, &tent))
    {
      بهر.push (std::move (*هذا));
      أرجع;
    }

    إذا ((tent->minimum < 0.f && tent->اعظمي > 0.f) ||
        !(tent->minimum <= tent->منځ && tent->منځ <= tent->اعظمي))
      أرجع;

    إذا (tent->منځ == 0.f)
    {
      بهر.push (std::move (*هذا));
      أرجع;
    }

    إذا (!axis_حد.is_ټکی () &&
        !(-1.f <= axis_حد.minimum &&
          axis_حد.minimum <= axis_حد.منځ &&
          axis_حد.منځ <= axis_حد.اعظمي &&
          axis_حد.اعظمي <= +1.f))
      أرجع;

    rebase_tent_پایله_t &حلونه = scratch.لومړی;
    rebase_tent (*tent, axis_حد, axis_triple_distances, حلونه, scratch.second);
    لكل (بینښه i = 0; i < حلونه.اوږدتیا; i++)
    {
      خپلکاری &t = حلونه.arrayZ[i];

      tuple_delta_t new_var;
      إذا (i < حلونه.اوږدتیا - 1)
	new_var.copy_from (*هذا, pool);
      وإلا
	new_var = std::move (*هذا);

      إذا (t.second == Triple ())
        new_var.remove_axis (axis_نښان_2);
      وإلا
        new_var.سېټ_tent (axis_نښان_2, t.second);

      new_var *= t.لومړی;
      بهر.push (std::move (new_var));
    }
  }

  منطقي compile_coords (ثابت hb_map_t& axes_شاخص_map,
		       ثابت hb_map_t& axes_old_شاخص_نښان_map,
		       hb_alloc_pool_t *pool= تش_اشاره)
  {
    بینښه cur_axis_شمېر = axes_شاخص_map.get_population ();
    إذا (pool)
    {
      إذا (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_شمېر)))
	أرجع خطأ;
    }
    وإلا إذا (unlikely (!compiled_peak_coords.resize (cur_axis_شمېر)))
      أرجع خطأ;

    hb_array_t<F2DOT14> پیل_coords, پای_ټکی_coords;

    بینښه orig_axis_شمېر = axes_old_شاخص_نښان_map.get_population ();
    بینښه j = 0;
    لكل (بینښه i = 0; i < orig_axis_شمېر; i++)
    {
      إذا (!axes_شاخص_map.has (i))
        استمر;

      hb_نښان_t axis_نښان_2 = axes_old_شاخص_نښان_map.get (i);
      Triple *coords = تش_اشاره;
      إذا (axis_tuples.has (axis_نښان_2, &coords))
      {
	عائم min_val = coords->minimum;
	عائم val = coords->منځ;
	عائم max_val = coords->اعظمي;

	compiled_peak_coords.arrayZ[j].سېټ_float (val);

	إذا (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  إذا (!compiled_interm_coords)
	  {
	    إذا (pool)
	    {
	      إذا (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_شمېر)))
		أرجع خطأ;
	    }
	    وإلا إذا (unlikely (!compiled_interm_coords.resize (2 * cur_axis_شمېر)))
	      أرجع خطأ;
	    پیل_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_شمېر);
	    پای_ټکی_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_شمېر);

	    لكل (بینښه k = 0; k < j; k++)
	    {
	      نښهدار peak = compiled_peak_coords.arrayZ[k].منزل_int ();
	      إذا (!peak) استمر;
	      پیل_coords.arrayZ[k].سېټ_int (hb_min (peak, 0));
	      پای_ټکی_coords.arrayZ[k].سېټ_int (hb_max (peak, 0));
	    }
	  }

	}

	إذا (compiled_interm_coords)
	{
	  پیل_coords.arrayZ[j].سېټ_float (min_val);
	  پای_ټکی_coords.arrayZ[j].سېټ_float (max_val);
	}
      }

      j++;
    }

    أرجع !compiled_peak_coords.دننه_error () && !compiled_interm_coords.دننه_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  منطقي compile_tuple_var_سرلیک (ثابت hb_map_t& axes_شاخص_map,
                                 بینښه points_معلومات_اوږدتیا,
                                 ثابت hb_map_t& axes_old_شاخص_نښان_map,
                                 ثابت hb_hashmap_t<ثابت hb_ویکټور_t<F2DOT14>*, بینښه>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = تش_اشاره)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    إذا (!compiled_deltas) أرجع ریښتیا;

    بینښه cur_axis_شمېر = axes_شاخص_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    بینښه alloc_len = 3 * cur_axis_شمېر * (F2DOT14::static_اندازه) + 4;
    إذا (unlikely (!compiled_tuple_سرلیک.allocate_from_pool (pool, alloc_len, خطأ))) أرجع خطأ;

    بینښه flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرلیک.begin () + 4);
    F2DOT14* پای_ټکی = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرلیک.پای_ټکی ());
    hb_array_t<F2DOT14> coords (p, پای_ټکی - p);

    إذا (!shared_tuples_idx_map)
      compile_coords (axes_شاخص_map, axes_old_شاخص_نښان_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    بینښه peak_شمېر = 0;
    بینښه *shared_tuple_idx;
    إذا (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    وإلا
    {
      peak_شمېر = encode_peak_coords(coords, flag);
      إذا (!peak_شمېر) أرجع خطأ;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    بینښه interim_شمېر = encode_interm_coords (coords.sub_array (peak_شمېر), flag);

    /* pointdata length = 0 implies "use shared points" */
    إذا (points_معلومات_اوږدتیا)
      flag |= Tuple_Variation_سرلیک::Tuple_شاخص::شخصي_ټکی_Numbers;

    بینښه serialized_معلومات_اندازه = points_معلومات_اوږدتیا + compiled_deltas.اوږدتیا;
    Tuple_Variation_سرلیک *o = reinterpret_cast<Tuple_Variation_سرلیک *> (compiled_tuple_سرلیک.begin ());
    o->var_معلومات_اندازه = serialized_معلومات_اندازه;
    o->tuple_شاخص = flag;

    بینښه ټول_سرلیک_len = 4 + (peak_شمېر + interim_شمېر) * (F2DOT14::static_اندازه);
    compiled_tuple_سرلیک.shrink_شا_منزل_pool (pool, ټول_سرلیک_len);
    أرجع ریښتیا;
  }

  بینښه encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               بینښه& flag) ثابت
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.اوږدتیا * حجم (compiled_peak_coords[0]));
    flag |= Tuple_Variation_سرلیک::Tuple_شاخص::EmbeddedPeakTuple;
    أرجع compiled_peak_coords.اوږدتیا;
  }

  /* if no need to encode intermediate coords, then just return p */
  بینښه encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 بینښه& flag) ثابت
  {
    إذا (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.اوږدتیا * حجم (compiled_interm_coords[0]));
      flag |= Tuple_Variation_سرلیک::Tuple_شاخص::IntermediateRegion;
    }
    أرجع compiled_interm_coords.اوږدتیا;
  }

  منطقي compile_deltas (hb_ویکټور_t<صحيح> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = تش_اشاره)
  { أرجع compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ساكن منطقي compile_deltas (hb_array_t<ثابت منطقي> ټکی_indices,
			      hb_array_t<ثابت عائم> x_deltas,
			      hb_array_t<ثابت عائم> y_deltas,
			      hb_ویکټور_t<بینښه حرف> &compiled_deltas, /*بهر*/
			      hb_ویکټور_t<صحيح> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = تش_اشاره)
  {
    إذا (unlikely (!rounded_deltas.resize_dirty  (ټکی_indices.اوږدتیا)))
      أرجع خطأ;

    بینښه j = 0;
    لكل (بینښه i = 0; i < ټکی_indices.اوږدتیا; i++)
    {
      إذا (!ټکی_indices[i]) استمر;
      rounded_deltas.arrayZ[j++] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    إذا (!rounded_deltas) أرجع ریښتیا;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    بینښه alloc_len = 2 + 4 * rounded_deltas.اوږدتیا + (rounded_deltas.اوږدتیا + 63) / 64;
    إذا (y_deltas)
      alloc_len *= 2;

    إذا (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, خطأ))) أرجع خطأ;

    بینښه encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    إذا (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      بینښه j = 0;
      لكل (بینښه idx = 0; idx < ټکی_indices.اوږدتیا; idx++)
      {
        إذا (!ټکی_indices[idx]) استمر;
        صحيح rounded_delta = hb_clamp_منزل<صحيح> (roundf ((مزدوج) y_deltas.arrayZ[idx]));

        إذا (j >= rounded_deltas.اوږدتیا) أرجع خطأ;

        rounded_deltas[j++] = rounded_delta;
      }

      إذا (j != rounded_deltas.اوږدتیا) أرجع خطأ;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_شا_منزل_pool (pool, encoded_len);
    أرجع ریښتیا;
  }

  ساكن بینښه compile_deltas (hb_array_t<بینښه حرف> encoded_بایټونه,
				  hb_array_t<ثابت صحيح> deltas)
  {
    أرجع Tuple_ارزښتونه::compile_unsafe (deltas, encoded_بایټونه);
  }

  منطقي calc_inferred_deltas (ثابت contour_ټکی_ویکټور_t& orig_points,
			     hb_ویکټور_t<بینښه> &scratch)
  {
    بینښه ټکی_شمېر = orig_points.اوږدتیا;
    إذا (ټکی_شمېر != indices.اوږدتیا)
      أرجع خطأ;

    بینښه ref_شمېر_2 = 0;

    hb_ویکټور_t<بینښه> &پای_ټکی_points = scratch.reset ();

    لكل (بینښه i = 0; i < ټکی_شمېر; i++)
    {
      ref_شمېر_2 += indices.arrayZ[i];
      إذا (orig_points.arrayZ[i].is_پای_ټکی_ټکی)
        پای_ټکی_points.push (i);
    }
    /* all points are referenced, nothing to do */
    إذا (ref_شمېر_2 == ټکی_شمېر)
      أرجع ریښتیا;
    إذا (unlikely (پای_ټکی_points.دننه_error ())) أرجع خطأ;

    hb_بټ_سېټ_t inferred_idxes;
    بینښه پیل_ټکی_2 = 0;
    لكل (بینښه پای_ټکی_ټکی : پای_ټکی_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      بینښه unref_شمېر = 0;
      لكل (بینښه i = پیل_ټکی_2; i < پای_ټکی_ټکی + 1; i++)
        unref_شمېر += indices.arrayZ[i];
      unref_شمېر = (پای_ټکی_ټکی - پیل_ټکی_2 + 1) - unref_شمېر;

      بینښه j = پیل_ټکی_2;
      إذا (unref_شمېر == 0 || unref_شمېر > پای_ټکی_ټکی - پیل_ټکی_2)
        اذهب no_نور_gaps;
      لكل (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        بینښه صحيح prev, next, i;
        لكل (;;)
        {
          i = j;
          j = بل_شاخص (i, پیل_ټکی_2, پای_ټکی_ټکی);
          إذا (indices.arrayZ[i] && !indices.arrayZ[j]) اكسر;
        }
        prev = j = i;
        لكل (;;)
        {
          i = j;
          j = بل_شاخص (i, پیل_ټکی_2, پای_ټکی_ټکی);
          إذا (!indices.arrayZ[i] && indices.arrayZ[j]) اكسر;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        لكل (;;)
        {
          i = بل_شاخص (i, پیل_ټکی_2, پای_ټکی_ټکی);
          إذا (i == next) اكسر;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.جمع_کړه (i);
          إذا (--unref_شمېر == 0) اذهب no_نور_gaps;
        }
      }
    no_نور_gaps:
      پیل_ټکی_2 = پای_ټکی_ټکی + 1;
    }

    لكل (بینښه i = 0; i < ټکی_شمېر; i++)
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
        indices[i] = ریښتیا;
      }
    }
    أرجع ریښتیا;
  }

  منطقي optimize (ثابت contour_ټکی_ویکټور_t& contour_points,
                 منطقي is_مرکب,
		 optimize_scratch_t &scratch,
                 مزدوج tolerance = 0.5 + 1e-10)
  {
    بینښه شمېر = contour_points.اوږدتیا;
    إذا (deltas_x.اوږدتیا != شمېر ||
        deltas_y.اوږدتیا != شمېر)
      أرجع خطأ;

    hb_ویکټور_t<منطقي> &opt_indices = scratch.opt_indices.reset ();
    hb_ویکټور_t<صحيح> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_ویکټور_t<صحيح> &rounded_y_deltas = scratch.rounded_y_deltas;

    إذا (unlikely (!rounded_x_deltas.resize_dirty  (شمېر) ||
                  !rounded_y_deltas.resize_dirty  (شمېر)))
      أرجع خطأ;

    لكل (بینښه i = 0; i < شمېر; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) deltas_y.arrayZ[i]));
    }

    إذا (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      أرجع خطأ;

    بینښه ref_شمېر_2 = 0;
    لكل (منطقي ref_flag : opt_indices)
       ref_شمېر_2 += ref_flag;

    إذا (ref_شمېر_2 == شمېر) أرجع ریښتیا;

    hb_ویکټور_t<عائم> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_ویکټور_t<عائم> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    منطقي is_comp_glyph_wo_deltas = (is_مرکب && ref_شمېر_2 == 0);
    إذا (is_comp_glyph_wo_deltas)
    {
      إذا (unlikely (!opt_deltas_x.resize (شمېر) ||
                    !opt_deltas_y.resize (شمېر)))
        أرجع خطأ;

      opt_indices.arrayZ[0] = ریښتیا;
      لكل (بینښه i = 1; i < شمېر; i++)
        opt_indices.arrayZ[i] = خطأ;
    }

    hb_ویکټور_t<بینښه حرف> &opt_ټکی_معلومات = scratch.opt_ټکی_معلومات.reset ();
    إذا (!compile_ټکی_سېټ (opt_indices, opt_ټکی_معلومات))
      أرجع خطأ;
    hb_ویکټور_t<بینښه حرف> &opt_deltas_معلومات = scratch.opt_deltas_معلومات.reset ();
    إذا (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_معلومات,
			 scratch.rounded_deltas))
      أرجع خطأ;

    hb_ویکټور_t<بینښه حرف> &ټکی_معلومات = scratch.ټکی_معلومات.reset ();
    إذا (!compile_ټکی_سېټ (indices, ټکی_معلومات))
      أرجع خطأ;
    hb_ویکټور_t<بینښه حرف> &deltas_معلومات = scratch.deltas_معلومات.reset ();
    إذا (!compile_deltas (indices, deltas_x, deltas_y, deltas_معلومات, scratch.rounded_deltas))
      أرجع خطأ;

    إذا (opt_ټکی_معلومات.اوږدتیا + opt_deltas_معلومات.اوږدتیا < ټکی_معلومات.اوږدتیا + deltas_معلومات.اوږدتیا)
    {
      indices = std::move (opt_indices);

      إذا (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    أرجع !indices.دننه_error () && !deltas_x.دننه_error () && !deltas_y.دننه_error ();
  }

  ساكن منطقي compile_ټکی_سېټ (ثابت hb_ویکټور_t<منطقي> &ټکی_indices,
                                 hb_ویکټور_t<بینښه حرف>& compiled_points /*بهر*/)
  {
    بینښه عدد_points = 0;
    لكل (منطقي i : ټکی_indices)
      إذا (i) عدد_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    إذا (!عدد_points) أرجع ریښتیا;

    بینښه indices_اوږدتیا = ټکی_indices.اوږدتیا;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    إذا (عدد_points == indices_اوږدتیا)
      أرجع compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    بینښه عدد_بایټونه = 2 + 3 *عدد_points;
    إذا (unlikely (!compiled_points.resize_dirty  (عدد_بایټونه)))
      أرجع خطأ;

    بینښه pos = 0;
    /* binary data starts with the total number of reference points */
    إذا (عدد_points < 0x80)
      compiled_points.arrayZ[pos++] = عدد_points;
    وإلا
    {
      compiled_points.arrayZ[pos++] = ((عدد_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = عدد_points & 0xFF;
    }

    ثابت بینښه max_پرلپسې_اوږدتیا = 0x7F;
    بینښه i = 0;
    بینښه وروستی_ارزښت = 0;
    بینښه عدد_encoded = 0;
    طالما (i < indices_اوږدتیا && عدد_encoded < عدد_points)
    {
      بینښه پرلپسې_اوږدتیا = 0;
      بینښه سرلیک_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      منطقي use_byte_encoding = خطأ;
      منطقي new_پرلپسې_2 = ریښتیا;
      طالما (i < indices_اوږدتیا && عدد_encoded < عدد_points &&
             پرلپسې_اوږدتیا <= max_پرلپسې_اوږدتیا)
      {
        // find out next referenced point index
        طالما (i < indices_اوږدتیا && !ټکی_indices[i])
          i++;

        إذا (i >= indices_اوږدتیا) اكسر;

        بینښه cur_ارزښت = i;
        بینښه delta = cur_ارزښت - وروستی_ارزښت;

        إذا (new_پرلپسې_2)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_پرلپسې_2 = خطأ;
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
        وروستی_ارزښت = cur_ارزښت;
        پرلپسې_اوږدتیا++;
        عدد_encoded++;
      }

      إذا (use_byte_encoding)
        compiled_points.arrayZ[سرلیک_pos] = پرلپسې_اوږدتیا - 1;
      وإلا
        compiled_points.arrayZ[سرلیک_pos] = (پرلپسې_اوږدتیا - 1) | 0x80;
    }
    أرجع compiled_points.resize_dirty  (pos);
  }

  ساكن عائم infer_delta (عائم موخه_val, عائم prev_val, عائم بل_val, عائم prev_delta, عائم بل_delta)
  {
    إذا (prev_val == بل_val)
      أرجع (prev_delta == بل_delta) ? prev_delta : 0.f;
    وإلا إذا (موخه_val <= hb_min (prev_val, بل_val))
      أرجع (prev_val < بل_val) ? prev_delta : بل_delta;
    وإلا إذا (موخه_val >= hb_max (prev_val, بل_val))
      أرجع (prev_val > بل_val) ? prev_delta : بل_delta;

    عائم r = (موخه_val - prev_val) / (بل_val - prev_val);
    أرجع prev_delta + r * (بل_delta - prev_delta);
  }

  ساكن بینښه صحيح بل_شاخص (بینښه صحيح i, بینښه صحيح پیل, بینښه صحيح پای_ټکی)
  { أرجع (i >= پای_ټکی) ? پیل : (i + 1); }
};

کينډۍ <ډول_نوم OffType = HBUINT16>
بنية Tuple_Variation_معلومات
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (هذا));
  }

  size_t get_اندازه (بینښه axis_شمېر_times_2) ثابت
  {
    size_t ټول_اندازه = min_اندازه;
    بینښه شمېر = tuple_Var_شمېر.get_شمېر ();
    ثابت Tuple_Variation_سرلیک *tuple_var_سرلیک = &(get_tuple_var_سرلیک());
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      ټول_اندازه = hb_unsigned_جمع_کړه_saturate (ټول_اندازه,
					     tuple_var_سرلیک->get_اندازه (axis_شمېر_times_2),
					     (size_t) tuple_var_سرلیک->get_معلومات_اندازه ());
      tuple_var_سرلیک = &tuple_var_سرلیک->get_بل (axis_شمېر_times_2);
    }

    أرجع ټول_اندازه;
  }

  ثابت Tuple_Variation_سرلیک &get_tuple_var_سرلیک (فراغ) ثابت
  { أرجع StructAfter<Tuple_Variation_سرلیک> (معلومات); }

  بنية tuple_iterator_t;
  بنية tuple_variations_t
  {
    hb_ویکټور_t<tuple_delta_t> tuple_vars;

    خاص:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ثابت hb_ویکټور_t<منطقي>*, hb_ویکټور_t<بینښه حرف>> ټکی_معلومات_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ثابت hb_ویکټور_t<منطقي>*, بینښه> ټکی_سېټ_شمېر_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_ویکټور_t<بینښه حرف> *shared_points_بایټونه = تش_اشاره;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    بینښه compiled_byte_اندازه = 0;
    منطقي needs_padding = خطأ;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    منطقي is_مرکب = خطأ;

    عام:
    tuple_variations_t () = افتراضي;
    tuple_variations_t (ثابت tuple_variations_t&) = ړنګول;
    tuple_variations_t& چلوونکی=(ثابت tuple_variations_t&) = ړنګول;
    tuple_variations_t (tuple_variations_t&&) = افتراضي;
    tuple_variations_t& چلوونکی=(tuple_variations_t&&) = افتراضي;
    ~tuple_variations_t () = افتراضي;

    څرګند چلوونکی منطقي () ثابت { أرجع منطقي (tuple_vars); }
    بینښه get_var_شمېر () ثابت
    {
      بینښه شمېر = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      لكل (خپلکاری& tuple: tuple_vars)
        إذا (tuple.compiled_deltas) شمېر++;

      إذا (shared_points_بایټونه && shared_points_بایټونه->اوږدتیا)
        شمېر |= Tuple_Var_شمېر::Shared_ټکی_Numbers;
      أرجع شمېر;
    }

    بینښه get_compiled_byte_اندازه () ثابت
    { أرجع compiled_byte_اندازه; }

    منطقي create_from_tuple_var_معلومات (tuple_iterator_t iterator,
                                     بینښه tuple_var_شمېر,
                                     بینښه ټکی_شمېر,
                                     منطقي is_gvar,
                                     ثابت hb_map_t *axes_old_شاخص_نښان_map,
                                     ثابت hb_ویکټور_t<بینښه> &shared_indices,
                                     ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = تش_اشاره,
                                     منطقي is_مرکب_glyph = خطأ)
    {
      hb_ویکټور_t<بینښه> شخصي_indices;
      hb_ویکټور_t<صحيح> deltas_x;
      hb_ویکټور_t<صحيح> deltas_y;
      افعل
      {
        ثابت HBUINT8 *p = iterator.get_serialized_معلومات ();
        بینښه صحيح اوږدتیا = iterator.current_tuple->get_معلومات_اندازه ();
        إذا (unlikely (!iterator.var_معلومات_بایټونه.check_range (p, اوږدتیا)))
          أرجع خطأ;

	hb_hashmap_t<hb_نښان_t, Triple> axis_tuples;
        إذا (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_شمېر (), shared_tuples, axes_old_شاخص_نښان_map, axis_tuples)
            || axis_tuples.is_empty ())
          أرجع خطأ;

	شخصي_indices.reset ();
        منطقي has_شخصي_points = iterator.current_tuple->has_شخصي_points ();
        ثابت HBUINT8 *پای_ټکی = p + اوږدتیا;
        إذا (has_شخصي_points &&
            !Tuple_Variation_معلومات::decompile_points (p, شخصي_indices, پای_ټکی))
          أرجع خطأ;

        ثابت hb_ویکټور_t<بینښه> &indices = has_شخصي_points ? شخصي_indices : shared_indices;
        منطقي apply_منزل_all = (indices.اوږدتیا == 0);
        بینښه عدد_deltas = apply_منزل_all ? ټکی_شمېر : indices.اوږدتیا;

        إذا (unlikely (!deltas_x.resize_dirty  (عدد_deltas) ||
                      !Tuple_Variation_معلومات::decompile_deltas (p, deltas_x, پای_ټکی)))
          أرجع خطأ;

        إذا (is_gvar)
        {
          إذا (unlikely (!deltas_y.resize_dirty  (عدد_deltas) ||
                        !Tuple_Variation_معلومات::decompile_deltas (p, deltas_y, پای_ټکی)))
            أرجع خطأ;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        إذا (unlikely (!var.indices.allocate_from_pool (pool, ټکی_شمېر) ||
                      !var.deltas_x.allocate_from_pool (pool, ټکی_شمېر, خطأ)))
          أرجع خطأ;

        إذا (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, ټکی_شمېر, خطأ)))
          أرجع خطأ;

        لكل (بینښه i = 0; i < عدد_deltas; i++)
        {
          بینښه idx = apply_منزل_all ? i : indices[i];
          إذا (idx >= ټکی_شمېر) استمر;
          var.indices[idx] = ریښتیا;
          var.deltas_x[idx] = deltas_x[i];
          إذا (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } طالما (iterator.move_منزل_بل ());

      is_مرکب = is_مرکب_glyph;
      أرجع ریښتیا;
    }

    منطقي create_from_توکی_var_معلومات (ثابت Var_معلومات &var_معلومات,
                                    ثابت hb_ویکټور_t<hb_hashmap_t<hb_نښان_t, Triple>>& regions,
                                    ثابت hb_map_t& axes_old_شاخص_نښان_map,
                                    بینښه& توکی_شمېر_2,
                                    ثابت hb_inc_bimap_t* inner_map = تش_اشاره)
    {
      /* NULL offset, to keep original varidx valid, just return */
      إذا (&var_معلومات == &Null (Var_معلومات))
        أرجع ریښتیا;

      بینښه عدد_regions = var_معلومات.get_region_شاخص_شمېر ();
      إذا (!tuple_vars.alloc (عدد_regions)) أرجع خطأ;

      توکی_شمېر_2 = inner_map ? inner_map->get_population () : var_معلومات.get_توکی_شمېر ();
      إذا (!توکی_شمېر_2) أرجع ریښتیا;
      بینښه لیکه_اندازه = var_معلومات.get_لیکه_اندازه ();
      ثابت HBUINT8 *delta_بایټونه = var_معلومات.get_delta_بایټونه ();

      لكل (بینښه r = 0; r < عدد_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        إذا (!tuple.deltas_x.resize_dirty  (توکی_شمېر_2) ||
            !tuple.indices.resize_dirty  (توکی_شمېر_2))
          أرجع خطأ;

        لكل (بینښه i = 0; i < توکی_شمېر_2; i++)
        {
          tuple.indices.arrayZ[i] = ریښتیا;
          tuple.deltas_x.arrayZ[i] = var_معلومات.get_توکی_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_بایټونه, لیکه_اندازه);
        }

        بینښه region_شاخص = var_معلومات.get_region_شاخص (r);
        إذا (region_شاخص >= regions.اوږدتیا) أرجع خطأ;
        tuple.axis_tuples = regions.arrayZ[region_شاخص];

        tuple_vars.push (std::move (tuple));
      }
      أرجع !tuple_vars.دننه_error ();
    }

    خاص:
    ساكن صحيح cmp_axis_نښان (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت hb_نښان_t *a = (ثابت hb_نښان_t*) pa;
      ثابت hb_نښان_t *b = (ثابت hb_نښان_t*) pb;
      أرجع (صحيح)(*a) - (صحيح)(*b);
    }

    منطقي change_tuple_variations_axis_limits (ثابت hb_hashmap_t<hb_نښان_t, Triple>& normalized_axes_location,
                                              ثابت hb_hashmap_t<hb_نښان_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = تش_اشاره)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_ویکټور_t<hb_نښان_t> axis_نښانونه;
      إذا (!axis_نښانونه.alloc (normalized_axes_location.get_population ()))
        أرجع خطأ;
      لكل (خپلکاری t : normalized_axes_location.کلیګانې ())
        axis_نښانونه.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_پایله_scratch_t scratch;
      hb_ویکټور_t<tuple_delta_t> بهر;

      axis_نښانونه.qsort (cmp_axis_نښان);
      لكل (خپلکاری axis_نښان_2 : axis_نښانونه)
      {
        Triple *axis_حد;
        إذا (!normalized_axes_location.has (axis_نښان_2, &axis_حد))
          أرجع خطأ;
        TripleDistances axis_triple_distances{1.f, 1.f};
        إذا (axes_triple_distances.has (axis_نښان_2))
          axis_triple_distances = axes_triple_distances.get (axis_نښان_2);

        hb_ویکټور_t<tuple_delta_t> new_vars;
        لكل (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_حد (axis_نښان_2, *axis_حد, axis_triple_distances, بهر, scratch, pool);
          إذا (!بهر) استمر;

          بینښه new_len = new_vars.اوږدتیا + بهر.اوږدتیا;

          إذا (unlikely (!new_vars.alloc (new_len, خطأ)))
            أرجع خطأ;

          لكل (بینښه i = 0; i < بهر.اوږدتیا; i++)
            new_vars.push (std::move (بهر[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      أرجع ریښتیا;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    منطقي یوځای_tuple_variations (contour_ټکی_ویکټور_t* contour_points = تش_اشاره)
    {
      hb_ویکټور_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      إذا (unlikely (!new_vars.alloc (tuple_vars.اوږدتیا)))
	أرجع خطأ;
      hb_hashmap_t<ثابت hb_hashmap_t<hb_نښان_t, Triple>*, بینښه> m;
      لكل (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        إذا (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          إذا (contour_points && !contour_points->جمع_کړه_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            أرجع خطأ;
          استمر;
        }

        بینښه *idx;
        إذا (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        وإلا
        {
	  خپلکاری *new_var = new_vars.push ();
	  إذا (unlikely (new_vars.دننه_error ()))
	    أرجع خطأ;
          hb_بدلون (*new_var, var);
          إذا (unlikely (!m.سېټ (&(new_var->axis_tuples), new_vars.اوږدتیا - 1)))
            أرجع خطأ;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      أرجع ریښتیا;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    منطقي compile_all_ټکی_sets ()
    {
      لكل (ثابت خپلکاری& tuple: tuple_vars)
      {
        ثابت hb_ویکټور_t<منطقي>* points_سېټ = &(tuple.indices);
        إذا (ټکی_معلومات_map.has (points_سېټ))
        {
          بینښه *شمېر;
          إذا (unlikely (!ټکی_سېټ_شمېر_map.has (points_سېټ, &شمېر) ||
                        !ټکی_سېټ_شمېر_map.سېټ (points_سېټ, (*شمېر) + 1)))
            أرجع خطأ;
          استمر;
        }

        hb_ویکټور_t<بینښه حرف> compiled_ټکی_معلومات;
        إذا (!tuple_delta_t::compile_ټکی_سېټ (*points_سېټ, compiled_ټکی_معلومات))
          أرجع خطأ;

        إذا (!ټکی_معلومات_map.سېټ (points_سېټ, std::move (compiled_ټکی_معلومات)) ||
            !ټکی_سېټ_شمېر_map.سېټ (points_سېټ, 1))
          أرجع خطأ;
      }
      أرجع ریښتیا;
    }

    /* find shared points set which saves most bytes */
    فراغ پیدا_کړه_shared_points ()
    {
      بینښه max_saved_بایټونه = 0;

      لكل (ثابت خپلکاری& _ : ټکی_معلومات_map.iter_ref ())
      {
        ثابت hb_ویکټور_t<منطقي>* points_سېټ = _.لومړی;
        بینښه معلومات_اوږدتیا = _.second.اوږدتیا;
        إذا (!معلومات_اوږدتیا) استمر;
        بینښه *شمېر;
        إذا (unlikely (!ټکی_سېټ_شمېر_map.has (points_سېټ, &شمېر) ||
                      *شمېر <= 1))
        {
          shared_points_بایټونه = تش_اشاره;
          أرجع;
        }

        بینښه saved_بایټونه = معلومات_اوږدتیا * ((*شمېر) -1);
        إذا (saved_بایټونه > max_saved_بایټونه)
        {
          max_saved_بایټونه = saved_بایټونه;
          shared_points_بایټونه = &(_.second);
        }
      }
    }

    منطقي calc_inferred_deltas (ثابت contour_ټکی_ویکټور_t& contour_points,
			       hb_ویکټور_t<بینښه> &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
        إذا (!var.calc_inferred_deltas (contour_points, scratch))
          أرجع خطأ;

      أرجع ریښتیا;
    }

    منطقي iup_optimize (ثابت contour_ټکی_ویکټور_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
      {
        إذا (!var.optimize (contour_points, is_مرکب, scratch))
          أرجع خطأ;
      }
      أرجع ریښتیا;
    }

    عام:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    فراغ cull_unreachable (ثابت hb_hashmap_t<hb_نښان_t, Triple> &reachable_ranges)
    {
      بینښه j = 0;
      لكل (بینښه i = 0; i < tuple_vars.اوږدتیا; i++)
      {
	إذا (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  استمر;
	إذا (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    منطقي instantiate (ثابت hb_hashmap_t<hb_نښان_t, Triple>& normalized_axes_location,
                      ثابت hb_hashmap_t<hb_نښان_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = تش_اشاره,
                      contour_ټکی_ویکټور_t* contour_points = تش_اشاره,
                      منطقي optimize = خطأ)
    {
      إذا (!tuple_vars) أرجع ریښتیا;
      إذا (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        أرجع خطأ;
      /* compute inferred deltas only for gvar */
      إذا (contour_points)
      {
	hb_ویکټور_t<بینښه> scratch;
	إذا (!calc_inferred_deltas (*contour_points, scratch))
	  أرجع خطأ;
      }

      /* if iup delta opt is on, contour_points can't be null */
      إذا (optimize && !contour_points)
        أرجع خطأ;

      إذا (!یوځای_tuple_variations (optimize ? contour_points : تش_اشاره))
        أرجع خطأ;

      إذا (optimize && !iup_optimize (*contour_points, scratch)) أرجع خطأ;
      أرجع !tuple_vars.دننه_error ();
    }

    منطقي compile_بایټونه (ثابت hb_map_t& axes_شاخص_map,
                        ثابت hb_map_t& axes_old_شاخص_نښان_map,
                        منطقي use_shared_points,
                        منطقي is_gvar = خطأ,
                        ثابت hb_hashmap_t<ثابت hb_ویکټور_t<F2DOT14>*, بینښه>* shared_tuples_idx_map = تش_اشاره,
			hb_alloc_pool_t *pool = تش_اشاره)
    {
      // return true for empty glyph
      إذا (!tuple_vars)
        أرجع ریښتیا;

      // compile points set and store data in hashmap
      إذا (!compile_all_ټکی_sets ())
        أرجع خطأ;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_اندازه += 4;

      إذا (use_shared_points)
      {
        پیدا_کړه_shared_points ();
        إذا (shared_points_بایټونه)
          compiled_byte_اندازه += shared_points_بایټونه->اوږدتیا;
      }
      hb_ویکټور_t<صحيح> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      لكل (خپلکاری& tuple: tuple_vars)
      {
        ثابت hb_ویکټور_t<منطقي>* points_سېټ = &(tuple.indices);
        hb_ویکټور_t<بینښه حرف> *points_معلومات;
        إذا (unlikely (!ټکی_معلومات_map.has (points_سېټ, &points_معلومات)))
          أرجع خطأ;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        إذا (!points_معلومات->اوږدتیا)
          استمر;
        إذا (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          أرجع خطأ;

        بینښه points_معلومات_اوږدتیا = (points_معلومات != shared_points_بایټونه) ? points_معلومات->اوږدتیا : 0;
        إذا (!tuple.compile_tuple_var_سرلیک (axes_شاخص_map, points_معلومات_اوږدتیا, axes_old_شاخص_نښان_map,
                                             shared_tuples_idx_map,
					     pool))
          أرجع خطأ;
        compiled_byte_اندازه += tuple.compiled_tuple_سرلیک.اوږدتیا + points_معلومات_اوږدتیا + tuple.compiled_deltas.اوږدتیا;
      }

      إذا (is_gvar && (compiled_byte_اندازه % 2))
      {
        needs_padding = ریښتیا;
        compiled_byte_اندازه += 1;
      }

      أرجع ریښتیا;
    }

    منطقي serialize_var_سرلیکونه (hb_serialize_context_t *c, بینښه& ټول_سرلیک_len) ثابت
    {
      TRACE_SERIALIZE (هذا);
      لكل (ثابت خپلکاری& tuple: tuple_vars)
      {
        tuple.compiled_tuple_سرلیک.as_array ().copy (c);
        إذا (c->دننه_error ()) return_trace (خطأ);
        ټول_سرلیک_len += tuple.compiled_tuple_سرلیک.اوږدتیا;
      }
      return_trace (ریښتیا);
    }

    منطقي serialize_var_معلومات (hb_serialize_context_t *c, منطقي is_gvar) ثابت
    {
      TRACE_SERIALIZE (هذا);
      إذا (is_gvar && shared_points_بایټونه)
      {
        hb_ubytes_t s (shared_points_بایټونه->arrayZ, shared_points_بایټونه->اوږدتیا);
        s.copy (c);
      }

      لكل (ثابت خپلکاری& tuple: tuple_vars)
      {
        ثابت hb_ویکټور_t<منطقي>* points_سېټ = &(tuple.indices);
        hb_ویکټور_t<بینښه حرف> *ټکی_معلومات;
        إذا (!ټکی_معلومات_map.has (points_سېټ, &ټکی_معلومات))
          return_trace (خطأ);

        إذا (!is_gvar || ټکی_معلومات != shared_points_بایټونه)
        {
          hb_ubytes_t s (ټکی_معلومات->arrayZ, ټکی_معلومات->اوږدتیا);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        إذا (c->دننه_error ()) return_trace (خطأ);
      }

      /* padding for gvar */
      إذا (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        إذا (!c->embed (pad)) return_trace (خطأ);
      }
      return_trace (ریښتیا);
    }
  };

  بنية tuple_iterator_t
  {
    بینښه get_axis_شمېر () ثابت { أرجع axis_شمېر; }

    فراغ init (hb_بایټونه_t var_معلومات_بایټونه_2, بینښه صحيح axis_شمېر_3, ثابت فراغ *جدول_بنسټ)
    {
      var_معلومات_بایټونه = var_معلومات_بایټونه_2;
      var_معلومات = var_معلومات_بایټونه_2.as<Tuple_Variation_معلومات> ();
      tuples_کیڼ = var_معلومات->tuple_Var_شمېر.get_شمېر ();
      axis_شمېر = axis_شمېر_3;
      axis_شمېر_times_2 = axis_شمېر_3 * 2;
      current_tuple = &var_معلومات->get_tuple_var_سرلیک ();
      معلومات_دننه_واټن_2 = 0;
      جدول_بنسټ_2 = جدول_بنسټ;
    }

    منطقي get_shared_indices (hb_ویکټور_t<بینښه صحيح> &shared_indices /*بهر*/)
    {
      إذا (var_معلومات->has_shared_ټکی_numbers ())
      {
        ثابت HBUINT8 *بنسټ = &(جدول_بنسټ_2+var_معلومات->معلومات);
        ثابت HBUINT8 *p = بنسټ;
        إذا (!decompile_points (p, shared_indices, (ثابت HBUINT8 *) (var_معلومات_بایټونه.arrayZ + var_معلومات_بایټونه.اوږدتیا))) أرجع خطأ;
        معلومات_دننه_واټن_2 = p - بنسټ;
      }
      أرجع ریښتیا;
    }

    منطقي is_valid ()
    {
      إذا (unlikely (tuples_کیڼ <= 0))
	أرجع خطأ;

      current_tuple_اندازه = Tuple_Variation_سرلیک::min_اندازه;
      إذا (unlikely (!var_معلومات_بایټونه.check_پای_ټکی ((ثابت حرف *) current_tuple + current_tuple_اندازه)))
	أرجع خطأ;

      current_tuple_اندازه = current_tuple->get_اندازه (axis_شمېر_times_2);
      إذا (unlikely (!var_معلومات_بایټونه.check_پای_ټکی ((ثابت حرف *) current_tuple + current_tuple_اندازه)))
	أرجع خطأ;

      أرجع ریښتیا;
    }

    HB_ALWAYS_INLINE
    منطقي move_منزل_بل ()
    {
      معلومات_دننه_واټن_2 += current_tuple->get_معلومات_اندازه ();
      current_tuple = &Struct_At_دننه_واټن<Tuple_Variation_سرلیک> (current_tuple, current_tuple_اندازه);
      tuples_کیڼ--;
      أرجع is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ثابت HBUINT8 *get_serialized_معلومات () ثابت
    { أرجع &(جدول_بنسټ_2+var_معلومات->معلومات) + معلومات_دننه_واټن_2; }

    خاص:
    نښهدار tuples_کیڼ;
    ثابت Tuple_Variation_معلومات *var_معلومات;
    بینښه صحيح axis_شمېر;
    بینښه صحيح axis_شمېر_times_2;
    بینښه صحيح معلومات_دننه_واټن_2;
    بینښه صحيح current_tuple_اندازه;
    ثابت فراغ *جدول_بنسټ_2;

    عام:
    hb_بایټونه_t var_معلومات_بایټونه;
    ثابت Tuple_Variation_سرلیک *current_tuple;
  };

  ساكن منطقي get_tuple_iterator (hb_بایټونه_t var_معلومات_بایټونه, بینښه axis_شمېر,
                                  ثابت فراغ *جدول_بنسټ_2,
                                  hb_ویکټور_t<بینښه صحيح> &shared_indices /*بهر*/,
                                  tuple_iterator_t *iterator /*بهر*/)
  {
    iterator->init (var_معلومات_بایټونه, axis_شمېر, جدول_بنسټ_2);
    إذا (!iterator->get_shared_indices (shared_indices))
      أرجع خطأ;
    أرجع iterator->is_valid ();
  }

  منطقي has_shared_ټکی_numbers () ثابت { أرجع tuple_Var_شمېر.has_shared_ټکی_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  منطقي cull_tuple_variations (hb_بایټونه_t var_معلومات_بایټونه,
			      بینښه axis_شمېر,
			      hb_array_t<ثابت F2DOT14> shared_tuples,
			      ثابت hb_map_t *axes_old_شاخص_نښان_map,
			      ثابت hb_hashmap_t<hb_نښان_t, Triple> &reachable_ranges,
			      hb_ویکټور_t<حرف> &بهر /*بهر*/,
			      منطقي *بدل_شوی /*بهر*/) ثابت
  {
    *بدل_شوی = خطأ;

    hb_ویکټور_t<بینښه صحيح> shared_indices;
    tuple_iterator_t iterator;
    إذا (!get_tuple_iterator (var_معلومات_بایټونه, axis_شمېر, هذا,
			     shared_indices, &iterator))
      أرجع ریښتیا; /* no tuples or malformed: keep original */

    ثابت حرف *بایټونه_پیل = var_معلومات_بایټونه.arrayZ;
    ثابت حرف *بایټونه_پای_ټکی = بایټونه_پیل + var_معلومات_بایټونه.اوږدتیا;
    ثابت حرف *serialized_بنسټ = (ثابت حرف *) &(هذا+معلومات);
    ثابت حرف *shared_points_پای_ټکی = (ثابت حرف *) iterator.get_serialized_معلومات ();
    إذا (unlikely (serialized_بنسټ < بایټونه_پیل || serialized_بنسټ > بایټونه_پای_ټکی ||
		  shared_points_پای_ټکی < serialized_بنسټ || shared_points_پای_ټکی > بایټونه_پای_ټکی))
      أرجع ریښتیا;

    بنية kept_tuple_t
    {
      ثابت حرف *سرلیک;
      بینښه سرلیک_اندازه;
      ثابت حرف *معلومات;
      بینښه معلومات_اندازه;
    };
    hb_ویکټور_t<kept_tuple_t> kept;
    بینښه ټول = 0;
    بینښه kept_سرلیکونه_اندازه = 0, kept_معلومات_اندازه = 0;
    افعل
    {
      ثابت Tuple_Variation_سرلیک *سرلیک = iterator.current_tuple;
      بینښه سرلیک_اندازه = سرلیک->get_اندازه (axis_شمېر * 2);
      بینښه معلومات_اندازه = سرلیک->get_معلومات_اندازه ();
      ثابت حرف *tuple_معلومات = (ثابت حرف *) iterator.get_serialized_معلومات ();
      إذا (unlikely (tuple_معلومات < serialized_بنسټ ||
		    معلومات_اندازه > (بینښه) (بایټونه_پای_ټکی - tuple_معلومات)))
	أرجع ریښتیا; /* malformed: keep original */

      hb_hashmap_t<hb_نښان_t, Triple> axis_tuples;
      إذا (!سرلیک->unpack_axis_tuples (axis_شمېر, shared_tuples,
				       axes_old_شاخص_نښان_map, axis_tuples))
	أرجع ریښتیا;

      ټول++;
      إذا (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ثابت حرف *) سرلیک, سرلیک_اندازه,
				 tuple_معلومات, معلومات_اندازه});
	kept_سرلیکونه_اندازه += سرلیک_اندازه;
	kept_معلومات_اندازه += معلومات_اندازه;
      }
    } طالما (iterator.move_منزل_بل ());

    إذا (unlikely (kept.دننه_error ())) أرجع خطأ;
    إذا (kept.اوږدتیا == ټول) أرجع ریښتیا; /* nothing to cull */

    *بدل_شوی = ریښتیا;
    إذا (!kept.اوږدتیا) أرجع ریښتیا; /* everything culled: no variation data */

    بینښه shared_points_اندازه = shared_points_پای_ټکی - serialized_بنسټ;
    بینښه معلومات_دننه_واټن_2 = min_اندازه + kept_سرلیکونه_اندازه;
    بینښه new_اندازه = معلومات_دننه_واټن_2 + shared_points_اندازه + kept_معلومات_اندازه;
    إذا (unlikely (!بهر.resize (new_اندازه)))
      أرجع خطأ;

    Tuple_Variation_معلومات *بهر_معلومات = (Tuple_Variation_معلومات *) بهر.arrayZ;
    بهر_معلومات->tuple_Var_شمېر = (uint16_t) (kept.اوږدتیا |
					  (has_shared_ټکی_numbers () ? 0x8000u : 0u));
    بهر_معلومات->معلومات = معلومات_دننه_واټن_2;

    حرف *p = بهر.arrayZ + min_اندازه;
    لكل (ثابت خپلکاری &t : kept)
    {
      hb_memcpy (p, t.سرلیک, t.سرلیک_اندازه);
      p += t.سرلیک_اندازه;
    }
    hb_memcpy (p, serialized_بنسټ, shared_points_اندازه);
    p += shared_points_اندازه;
    لكل (ثابت خپلکاری &t : kept)
    {
      hb_memcpy (p, t.معلومات, t.معلومات_اندازه);
      p += t.معلومات_اندازه;
    }
    أرجع ریښتیا;
  }

  ساكن منطقي decompile_points (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_ویکټور_t<بینښه صحيح> &points /*بهر*/,
				ثابت HBUINT8 *پای_ټکی)
  {
    تعداد packed_ټکی_flag_t
    {
      POINTS_ARE_ټکي     = 0x80,
      ټکی_پرلپسې_شمېر_ماسک = 0x7F
    };

    إذا (unlikely (p + 1 > پای_ټکی)) أرجع خطأ;

    بینښه شمېر = *p++;
    إذا (شمېر & POINTS_ARE_ټکي)
    {
      إذا (unlikely (p + 1 > پای_ټکی)) أرجع خطأ;
      شمېر = ((شمېر & ټکی_پرلپسې_شمېر_ماسک) << 8) | *p++;
    }
    إذا (unlikely (!points.resize_dirty  (شمېر))) أرجع خطأ;

    بینښه n = 0;
    بینښه i = 0;
    طالما (i < شمېر)
    {
      إذا (unlikely (p + 1 > پای_ټکی)) أرجع خطأ;
      بینښه control = *p++;
      بینښه پرلپسې_شمېر = (control & ټکی_پرلپسې_شمېر_ماسک) + 1;
      بینښه stop = i + پرلپسې_شمېر;
      إذا (unlikely (stop > شمېر)) أرجع خطأ;
      إذا (control & POINTS_ARE_ټکي)
      {
        إذا (unlikely (p + پرلپسې_شمېر * HBUINT16::static_اندازه > پای_ټکی)) أرجع خطأ;
        لكل (; i < stop; i++)
        {
          n += *(ثابت HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_اندازه;
        }
      }
      وإلا
      {
        إذا (unlikely (p + پرلپسې_شمېر > پای_ټکی)) أرجع خطأ;
        لكل (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم T>
  HB_ALWAYS_INLINE
  ساكن منطقي decompile_deltas (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_ویکټور_t<T> &deltas /* IN/OUT */,
				ثابت HBUINT8 *پای_ټکی,
				منطقي consume_all = خطأ,
				بینښه پیل = 0)
  {
    أرجع Tuple_ارزښتونه::decompile (p, deltas, پای_ټکی, consume_all, پیل);
  }

  منطقي has_معلومات () ثابت { أرجع tuple_Var_شمېر; }

  منطقي decompile_tuple_variations (بینښه ټکی_شمېر,
                                   منطقي is_gvar,
                                   tuple_iterator_t iterator,
                                   ثابت hb_map_t *axes_old_شاخص_نښان_map,
                                   ثابت hb_ویکټور_t<بینښه> &shared_indices,
                                   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /*بهر*/
				   hb_alloc_pool_t *pool = تش_اشاره,
                                   منطقي is_مرکب_glyph = خطأ) ثابت
  {
    أرجع tuple_variations.create_from_tuple_var_معلومات (iterator, tuple_Var_شمېر,
                                                        ټکی_شمېر, is_gvar,
                                                        axes_old_شاخص_نښان_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_مرکب_glyph);
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي is_gvar,
                  ثابت tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (هذا);
    /* empty tuple variations, just return and skip serialization. */
    إذا (!tuple_variations) return_trace (ریښتیا);

    خپلکاری *بهر = c->پیل_embed (هذا);
    إذا (unlikely (!c->extend_min (بهر))) return_trace (خطأ);

    إذا (!c->check_ټاکل (بهر->tuple_Var_شمېر, tuple_variations.get_var_شمېر (),
                          HB_SERIALIZE_ERROR_INT_ډکېدنه)) return_trace (خطأ);

    بینښه ټول_سرلیک_len = 0;

    إذا (!tuple_variations.serialize_var_سرلیکونه (c, ټول_سرلیک_len))
      return_trace (خطأ);

    بینښه معلومات_دننه_واټن_2 = min_اندازه + ټول_سرلیک_len;
    إذا (!is_gvar) معلومات_دننه_واټن_2 += 4;
    إذا (!c->check_ټاکل (بهر->معلومات, معلومات_دننه_واټن_2, HB_SERIALIZE_ERROR_INT_ډکېدنه)) return_trace (خطأ);

    أرجع tuple_variations.serialize_var_معلومات (c, is_gvar);
  }

  محمي:
  بنية Tuple_Var_شمېر : HBUINT16
  {
    ملګری بنية tuple_variations_t;
    منطقي has_shared_ټکی_numbers () ثابت { أرجع ((*هذا) & Shared_ټکی_Numbers); }
    بینښه صحيح get_شمېر () ثابت { أرجع (*هذا) & شمېر_ماسک; }
    Tuple_Var_شمېر& چلوونکی = (uint16_t i) { HBUINT16::چلوونکی= (i); أرجع *هذا; }
    څرګند چلوونکی منطقي () ثابت { أرجع get_شمېر (); }

    محمي:
    تعداد Flags
    {
      Shared_ټکی_Numbers= 0x8000u,
      شمېر_ماسک         = 0x0FFFu
    };
    عام:
    DEFINE_اندازه_STATIC (2);
  };

  Tuple_Var_شمېر tuple_Var_شمېر;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  دننه_واټن_منزل<HBUINT8, OffType>
                معلومات;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  عام:
  DEFINE_اندازه_MIN (2 + OffType::static_اندازه);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
کارول tuple_variations_t = Tuple_Variation_معلومات<HBUINT16>::tuple_variations_t;
بنية توکی_variations_t
{
  کارول region_t = ثابت hb_hashmap_t<hb_نښان_t, Triple>*;
  خاص:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_ویکټور_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_ویکټور_t<بینښه> var_معلومات_عدد_لیکې;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_ویکټور_t<hb_hashmap_t<hb_نښان_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_ویکټور_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, بینښه> region_map;

  /* all delta rows after instantiation */
  hb_ویکټور_t<hb_ویکټور_t<صحيح>> delta_لیکې;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_ویکټور_t<delta_لیکه_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  منطقي has_long = خطأ;

  عام:
  منطقي has_long_کلمه () ثابت
  { أرجع has_long; }

  ثابت hb_ویکټور_t<region_t>& get_region_list () ثابت
  { أرجع region_list; }

  ثابت hb_ویکټور_t<delta_لیکه_encoding_t>& get_vardata_encodings () ثابت
  { أرجع encodings; }

  ثابت hb_map_t& get_varidx_map () ثابت
  { أرجع varidx_map; }

  منطقي جمع_کړه_vardata_encoding_for_testing (hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح>*> &&لیکې,
                                         بینښه عدد_ستون_شمېر)
  {
    encodings.push (delta_لیکه_encoding_t (std::move (لیکې), عدد_ستون_شمېر));
    أرجع !encodings.دننه_error ();
  }

  منطقي compile_varidx_map_for_testing (ثابت hb_hashmap_t<بینښه, ثابت hb_ویکټور_t<صحيح>*>& front_mapping)
  { أرجع compile_varidx_map (front_mapping); }

  منطقي instantiate (ثابت توکی_Variation_Store& varStore,
                    ثابت hb_subset_plan_t *plan,
                    منطقي optimize=ریښتیا,
                    منطقي use_no_variation_idx=ریښتیا,
                    ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> (),
                    منطقي layout_conditions = خطأ)
  {
    إذا (!create_from_توکی_varstore (varStore, plan->axes_old_شاخص_نښان_map, inner_maps))
      أرجع خطأ;
    /* avar2 partial instancing: cull unreachable regions. */
    إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      لكل (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    إذا (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      أرجع خطأ;
    إذا (layout_conditions &&
        (!instantiate_condition_ارزښتونه (plan, varStore) || !build_region_list ()))
      أرجع خطأ;
    أرجع as_توکی_varstore (optimize, use_no_variation_idx);
  }

  منطقي instantiate_condition_ارزښتونه (ثابت hb_subset_plan_t *plan,
                                      ثابت توکی_Variation_Store &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    خپلکاری &condition_map = plan->layout_condition_idx_ارزښت_map;
    condition_map.clear ();
    إذا (!plan->layout_condition_ارزښتونه) أرجع ریښتیا;
    کارول condition_ارزښت_t = hb_pair_t<uintptr_t, hb_pair_t<بینښه, صحيح>>;
    hb_ویکټور_t<condition_ارزښت_t> conditions;
    لكل (خپلکاری condition : plan->layout_condition_ارزښتونه.iter ())
      conditions.push (condition);
    إذا (unlikely (conditions.دننه_error ())) أرجع خطأ;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ثابت condition_ارزښت_t &a, ثابت condition_ارزښت_t &b) {
      إذا (a.second.لومړی != b.second.لومړی)
        أرجع a.second.لومړی < b.second.لومړی ? -1 : 1;
      أرجع a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    بینښه new_outer = 0;
    لكل (بینښه i = 0; i < plan->gdef_varstore_inner_maps.اوږدتیا; i++)
      إذا (plan->gdef_varstore_inner_maps[i].get_population ())
        إذا (!outer_map.سېټ (i, new_outer++)) أرجع خطأ;
    لكل (خپلکاری condition : conditions)
    {
      بینښه old_idx = condition.second.لومړی;
      مزدوج value = condition.second.second +
                     varStore.get_delta<مزدوج> (old_idx, plan->normalized_coords.as_array ());
      بینښه old_outer = old_idx >> 16;
      إذا (!outer_map.has (old_outer))
        استمر;
      بینښه outer = outer_map.get (old_outer);
      بینښه inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      بینښه idx = (outer << 16) | inner;
      إذا (outer >= vars.اوږدتیا || inner >= var_معلومات_عدد_لیکې[outer])
        استمر;

      مزدوج اعظمي = fabs ((مزدوج) value);
      منطقي variable = خطأ;
      لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        إذا (unlikely (inner >= tuple.deltas_x.اوږدتیا)) أرجع خطأ;
        عائم delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        اعظمي = hb_max (اعظمي, fabs ((مزدوج) delta));
      }
      إذا (!variable)
      {
        إذا (!condition_map.سېټ (condition.لومړی,
                                hb_pair (بینښه (VarIdx::NO_VARIATION), صحيح (value > 0))))
          أرجع خطأ;
        استمر;
      }

      مزدوج scale = 1.;
      طالما (اعظمي * scale > لوی_عدد)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      طالما (اعظمي * scale * 2 <= لوی_عدد)
      {
        منطقي integral = (مزدوج) value * scale == round ((مزدوج) value * scale);
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (مزدوج) tuple.deltas_x[inner] * scale ==
                      round ((مزدوج) tuple.deltas_x[inner] * scale);
        إذا (integral) اكسر;
        scale *= 2;
      }
      صحيح default_ارزښت = (صحيح) round ((مزدوج) value * scale);
      إذا (scale != 1. || default_ارزښت < -32768 || default_ارزښت > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t شخصي_لیکه;
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          إذا (!tuple.deltas_x[inner]) استمر;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (ریښتیا);
          copy.deltas_x.push ((عائم) round ((مزدوج) tuple.deltas_x[inner] * scale));
          إذا (unlikely (copy.axis_tuples.دننه_error () || copy.indices.دننه_error () ||
                        copy.deltas_x.دننه_error ())) أرجع خطأ;
          شخصي_لیکه.tuple_vars.push (std::move (copy));
        }
        إذا (unlikely (vars.اوږدتیا >= 0xFFFFu)) أرجع خطأ;
        إذا (unlikely (شخصي_لیکه.tuple_vars.دننه_error ())) أرجع خطأ;
        idx = vars.اوږدتیا << 16;
        vars.push (std::move (شخصي_لیکه));
        var_معلومات_عدد_لیکې.push (1);
        إذا (unlikely (vars.دننه_error () || var_معلومات_عدد_لیکې.دننه_error ())) أرجع خطأ;
        إذا (default_ارزښت < -32768 || default_ارزښت > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          صحيح bias = default_ارزښت - hb_clamp (default_ارزښت, -32768, 32767);
          default_ارزښت -= bias;
          صحيح لوړ = bias / 65536 * 65536;
          صحيح parts[] = {لوړ, bias - لوړ};
          لكل (صحيح part : parts)
          {
            إذا (!part) استمر;
            بینښه شمېر = vars[idx >> 16].tuple_vars.اوږدتیا;
            جمع_کړه_tuple (idx >> 16, hb_hashmap_t<hb_نښان_t, Triple> (),
                       0, part, 1);
            ثابت خپلکاری &tuples = vars[idx >> 16].tuple_vars;
            إذا (unlikely (tuples.دننه_error () || tuples.اوږدتیا != شمېر + 1 ||
                          tuples.لکۍ ().indices.دننه_error () ||
                          tuples.لکۍ ().deltas_x.دننه_error ())) أرجع خطأ;
          }
        }
      }
      إذا (!condition_map.سېټ (condition.لومړی, hb_pair (idx, default_ارزښت)))
        أرجع خطأ;
    }
    أرجع !condition_map.دننه_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  منطقي create_from_توکی_varstore (ثابت توکی_Variation_Store& varStore,
                                  ثابت hb_map_t& axes_old_شاخص_نښان_map,
                                  ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> ())
  {
    ثابت VarRegionList& regionList = varStore.get_region_list ();
    إذا (!regionList.get_var_regions (axes_old_شاخص_نښان_map, orig_region_list))
      أرجع خطأ;

    بینښه عدد_var_معلومات = varStore.get_sub_جدول_شمېر ();
    إذا (inner_maps && inner_maps.اوږدتیا != عدد_var_معلومات) أرجع خطأ;
    إذا (!vars.alloc (عدد_var_معلومات) ||
        !var_معلومات_عدد_لیکې.alloc (عدد_var_معلومات)) أرجع خطأ;

    لكل (بینښه i = 0; i < عدد_var_معلومات; i++)
    {
      إذا (inner_maps && !inner_maps.arrayZ[i].get_population ())
          استمر;
      tuple_variations_t var_معلومات_tuples;
      بینښه توکی_شمېر_2 = 0;
      إذا (!var_معلومات_tuples.create_from_توکی_var_معلومات (varStore.get_sub_جدول (i),
                                                      orig_region_list,
                                                      axes_old_شاخص_نښان_map,
                                                      توکی_شمېر_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : تش_اشاره))
        أرجع خطأ;

      var_معلومات_عدد_لیکې.push (توکی_شمېر_2);
      vars.push (std::move (var_معلومات_tuples));
    }
    أرجع !vars.دننه_error () && !var_معلومات_عدد_لیکې.دننه_error () && vars.اوږدتیا == var_معلومات_عدد_لیکې.اوږدتیا;
  }

  منطقي instantiate_tuple_vars (ثابت hb_hashmap_t<hb_نښان_t, Triple>& normalized_axes_location,
                               ثابت hb_hashmap_t<hb_نښان_t, TripleDistances>& axes_triple_distances,
                               منطقي build_regions = ریښتیا)
  {
    optimize_scratch_t scratch;
    لكل (tuple_variations_t& tuple_vars : vars)
      إذا (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        أرجع خطأ;

    أرجع !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  بینښه جمع_کړه_vardata (بینښه توکی_شمېر_2)
  {
    vars.push (tuple_variations_t ());
    var_معلومات_عدد_لیکې.push (توکی_شمېر_2);
    أرجع vars.اوږدتیا - 1;
  }

  /* Get item count for a VarData subtable. */
  بینښه get_توکی_شمېر (بینښه outer) ثابت
  { أرجع outer < var_معلومات_عدد_لیکې.اوږدتیا ? var_معلومات_عدد_لیکې[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  بینښه duplicate_لیکه (بینښه outer, بینښه inner)
  {
    إذا (unlikely (outer >= vars.اوږدتیا ||
                  inner >= var_معلومات_عدد_لیکې[outer] ||
                  var_معلومات_عدد_لیکې[outer] >= 0xFFFFu))
      أرجع (بینښه) -1;
    لكل (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      إذا (unlikely (inner >= tuple.deltas_x.اوږدتیا ||
                    inner >= tuple.indices.اوږدتیا))
        أرجع (بینښه) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      إذا (inner < tuple.deltas_y.اوږدتیا)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      إذا (unlikely (tuple.indices.دننه_error () ||
                    tuple.deltas_x.دننه_error () ||
                    tuple.deltas_y.دننه_error ()))
        أرجع (بینښه) -1;
    }
    أرجع var_معلومات_عدد_لیکې[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  فراغ جمع_کړه_tuple (بینښه outer,
                  hb_hashmap_t<hb_نښان_t, Triple>&& axis_tuples,
                  بینښه inner, صحيح delta, بینښه توکی_شمېر_2)
  {
    إذا (outer >= vars.اوږدتیا) أرجع;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    إذا (!tuple.indices.resize (توکی_شمېر_2) ||
        !tuple.deltas_x.resize (توکی_شمېر_2)) أرجع;
    لكل (بینښه i = 0; i < توکی_شمېر_2; i++)
    {
      tuple.indices.arrayZ[i] = ریښتیا;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (عائم) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  منطقي build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, بینښه> all_regions;
    hb_hashmap_t<region_t, بینښه> کارول_شوی_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_ویکټور_t<region_t> all_unique_regions;
    لكل (ثابت tuple_variations_t& sub_جدول_2 : vars)
    {
      لكل (ثابت tuple_delta_t& tuple : sub_جدول_2.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        إذا (!کارول_شوی_regions.has (r))
        {
          منطقي all_zeros = ریښتیا;
          لكل (عائم d : tuple.deltas_x)
          {
            صحيح delta = hb_clamp_منزل<صحيح> (roundf ((مزدوج) d));
            إذا (delta != 0)
            {
              all_zeros = خطأ;
              اكسر;
            }
          }
          إذا (!all_zeros)
          {
            إذا (!کارول_شوی_regions.سېټ (r, 1))
              أرجع خطأ;
          }
        }
        إذا (all_regions.has (r))
          استمر;
        إذا (!all_regions.سېټ (r, 1))
          أرجع خطأ;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    إذا (!all_regions || !all_unique_regions) أرجع ریښتیا;

    إذا (!region_list.alloc (all_regions.get_population ()))
      أرجع خطأ;

    بینښه idx = 0;
    /* append the original regions that pre-existed */
    لكل (ثابت خپلکاری& r : orig_region_list)
    {
      إذا (!all_regions.has (&r) || !کارول_شوی_regions.has (&r))
        استمر;

      region_list.push (&r);
      إذا (!region_map.سېټ (&r, idx))
        أرجع خطأ;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    لكل (ثابت خپلکاری& r: all_unique_regions)
    {
      إذا (!all_regions.has (r) || !کارول_شوی_regions.has (r))
        استمر;
      region_list.push (r);
      إذا (!region_map.سېټ (r, idx))
        أرجع خطأ;
      all_regions.del (r);
      idx++;
    }
    أرجع (!region_list.دننه_error ()) && (!region_map.دننه_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  بنية combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = افتراضي;
    combined_gain_idx_tuple_t (بینښه gain, بینښه i, بینښه j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      أكد (gain < 0xFFFFFF);
      أكد (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    منطقي چلوونکی < (ثابت combined_gain_idx_tuple_t& o)
    {
      أرجع encoded < o.encoded;
    }

    منطقي چلوونکی <= (ثابت combined_gain_idx_tuple_t& o)
    {
      أرجع encoded <= o.encoded;
    }

    بینښه idx_1 () ثابت { أرجع (encoded >> 20) & 0xFFFFF; };
    بینښه idx_2 () ثابت { أرجع encoded & 0xFFFFF; };
  };

  منطقي as_توکی_varstore (منطقي optimize=ریښتیا, منطقي use_no_variation_idx=ریښتیا)
  {
    /* return true if no variation data */
    إذا (!region_list) أرجع ریښتیا;
    بینښه عدد_ستون_شمېر = region_list.اوږدتیا;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    بینښه ټول_لیکې = 0;
    لكل (بینښه major = 0; major < var_معلومات_عدد_لیکې.اوږدتیا; major++)
      ټول_لیکې += var_معلومات_عدد_لیکې[major];

    إذا (!delta_لیکې.resize (ټول_لیکې)) أرجع خطأ;
    /* init all rows to [0]*num_cols */
    لكل (بینښه i = 0; i < ټول_لیکې; i++)
      إذا (!(delta_لیکې[i].resize (عدد_ستون_شمېر))) أرجع خطأ;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<بینښه, ثابت hb_ویکټور_t<صحيح>*> front_mapping;
    بینښه پیل_لیکه = 0;
    hb_ویکټور_t<delta_لیکه_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح> *> major_لیکې;
    hb_hashmap_t<ثابت hb_ویکټور_t<صحيح>*, بینښه> delta_لیکې_map;
    لكل (بینښه major = 0; major < vars.اوږدتیا; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ثابت tuple_variations_t& tuples = vars[major];
      بینښه عدد_لیکې = var_معلومات_عدد_لیکې[major];

      إذا (!عدد_لیکې) استمر;

      لكل (ثابت tuple_delta_t& tuple: tuples.tuple_vars)
      {
        إذا (tuple.deltas_x.اوږدتیا != عدد_لیکې)
          أرجع خطأ;

        /* skip unused regions */
        بینښه *ستون_idx;
        إذا (!region_map.has (&(tuple.axis_tuples), &ستون_idx))
          استمر;

        لكل (بینښه i = 0; i < عدد_لیکې; i++)
        {
          صحيح rounded_delta = hb_clamp_منزل<صحيح> (roundf ((مزدوج) tuple.deltas_x[i]));
          صحيح &delta = delta_لیکې[پیل_لیکه + i][*ستون_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          إذا (unlikely (sum < INT_MIN || sum > لوی_عدد)) أرجع خطأ;
          delta = (صحيح) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_لیکې.reset ();
      لكل (بینښه minor = 0; minor < عدد_لیکې; minor++)
      {
	ثابت hb_ویکټور_t<صحيح>& لیکه = delta_لیکې[پیل_لیکه + minor];
	إذا (use_no_variation_idx)
	{
	  منطقي all_zeros = ریښتیا;
	  لكل (صحيح delta : لیکه)
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

	إذا (!front_mapping.سېټ ((major<<16) + minor, &لیکه))
	  أرجع خطأ;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	إذا (optimize)
	{
	  إذا (delta_لیکې_map.has (&لیکه))
	    استمر;

	  delta_لیکې_map.سېټ (&لیکه, 1);
	}

	major_لیکې.push (&لیکه);
      }

      إذا (unlikely (major_لیکې.دننه_error () || delta_لیکې_map.دننه_error ()))
        أرجع خطأ;
      إذا (major_لیکې)
      {
	خپلکاری *encoding = encoding_objs.push (std::move (major_لیکې), عدد_ستون_شمېر);
	إذا (unlikely (encoding_objs.دننه_error () || encoding->دننه_error ()))
	  أرجع خطأ;
      }

      پیل_لیکه += عدد_لیکې;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    إذا (!optimize)
    {
      encodings = std::move (encoding_objs);
      أرجع !encodings.دننه_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.qsort ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    کارول توکی_t = hb_لومړیتوب_کتار_t<combined_gain_idx_tuple_t>::توکی_t;
    hb_ویکټور_t<توکی_t> کتار_توکي;
    بینښه عدد_todos = encoding_objs.اوږدتیا;
    لكل (بینښه i = 0; i < عدد_todos; i++)
    {
      لكل (بینښه j = i + 1; j < عدد_todos; j++)
      {
        صحيح combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        إذا (combining_gain > 0)
          کتار_توکي.push (توکی_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_لومړیتوب_کتار_t<combined_gain_idx_tuple_t> کتار (std::move (کتار_توکي));

    hb_بټ_سېټ_t removed_todo_idxes;
    طالما (کتار)
    {
      خپلکاری t = کتار.راوباسه_minimum ().لومړی;
      بینښه i = t.idx_1 ();
      بینښه j = t.idx_2 ();

      إذا (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        استمر;

      delta_لیکه_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_لیکه_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.جمع_کړه (i);
      removed_todo_idxes.جمع_کړه (j);
      إذا (unlikely (removed_todo_idxes.دننه_error ())) أرجع خطأ;

      encoding.یوځای (other_encoding);
      إذا (unlikely (encoding.دننه_error ())) أرجع خطأ;

      لكل (بینښه idx = 0; idx < encoding_objs.اوږدتیا; idx++)
      {
        إذا (removed_todo_idxes.has (idx)) استمر;

        ثابت delta_لیکه_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        إذا (obj.پلنوالی == encoding.پلنوالی && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          لكل (ثابت خپلکاری& لیکه : obj.توکي)
            encoding.جمع_کړه_لیکه (لیکه);
          إذا (unlikely (encoding.دننه_error ())) أرجع خطأ;

          removed_todo_idxes.جمع_کړه (idx);
          إذا (unlikely (removed_todo_idxes.دننه_error ())) أرجع خطأ;
          استمر;
        }

        صحيح combined_gain = encoding.gain_from_merging (obj);
        إذا (combined_gain > 0)
          کتار.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.اوږدتیا), 0);
      }

      خپلکاری moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      إذا (unlikely (encoding_objs.دننه_error ())) أرجع خطأ;
    }

    صحيح عدد_final_encodings = (صحيح) encoding_objs.اوږدتیا - (صحيح) removed_todo_idxes.get_population ();
    إذا (عدد_final_encodings <= 0) أرجع خطأ;

    إذا (!encodings.alloc (عدد_final_encodings)) أرجع خطأ;
    لكل (بینښه i = 0; i < encoding_objs.اوږدتیا; i++)
    {
      إذا (removed_todo_idxes.has (i)) استمر;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    أرجع compile_varidx_map (front_mapping);
  }

  خاص:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  منطقي compile_varidx_map (ثابت hb_hashmap_t<بینښه, ثابت hb_ویکټور_t<صحيح>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ثابت hb_ویکټور_t<صحيح>*, بینښه> شا_mapping;
    hb_ویکټور_t<delta_لیکه_encoding_t> وېشل_encodings;

    لكل (بینښه i = 0; i < encodings.اوږدتیا; i++)
    {
      delta_لیکه_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      إذا (encoding.is_empty () || encoding.دننه_error ())
        أرجع خطأ;

      بینښه عدد_لیکې = encoding.توکي.اوږدتیا;
      بینښه عدد_ستون_شمېر = encoding.chars.اوږدتیا;

      /* sort rows, make result deterministic */
      encoding.توکي.qsort (cmp_لیکه);

      لكل (بینښه پیل = 0; پیل < عدد_لیکې; پیل += 0xFFFFu)
      {
        بینښه chunk_len = hb_min (عدد_لیکې - پیل, 0xFFFFu);
        hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح>*> لیکې;

        إذا (!لیکې.alloc (chunk_len))
          أرجع خطأ;

        بینښه major = وېشل_encodings.اوږدتیا;
        لكل (بینښه minor = 0; minor < chunk_len; minor++)
        {
          ثابت hb_ویکټور_t<صحيح> *لیکه = encoding.توکي.arrayZ[پیل + minor];
          لیکې.push (لیکه);
          إذا (!شا_mapping.سېټ (لیکه, (major << 16) + minor))
            أرجع خطأ;
        }

        خپلکاری *وېشل = وېشل_encodings.push (std::move (لیکې), عدد_ستون_شمېر);
        إذا (unlikely (وېشل_encodings.دننه_error () || وېشل->دننه_error ()))
          أرجع خطأ;
      }
    }

    encodings = std::move (وېشل_encodings);
    إذا (encodings.دننه_error () || شا_mapping.دننه_error ())
      أرجع خطأ;

    لكل (خپلکاری _ : front_mapping.iter ())
    {
      بینښه old_varidx = _.لومړی;
      بینښه *new_varidx;
      إذا (شا_mapping.has (_.second, &new_varidx))
        varidx_map.سېټ (old_varidx, *new_varidx);
      وإلا
        varidx_map.سېټ (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_شاخص);
    }
    أرجع !varidx_map.دننه_error ();
  }

  ساكن صحيح cmp_لیکه (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ثابت hb_ویکټور_t<صحيح>** a = (ثابت hb_ویکټور_t<صحيح>**) pa;
    ثابت hb_ویکټور_t<صحيح>** b = (ثابت hb_ویکټور_t<صحيح>**) pb;

    لكل (بینښه i = 0; i < (*b)->اوږدتیا; i++)
    {
      صحيح va = (*a)->arrayZ[i];
      صحيح vb = (*b)->arrayZ[i];
      إذا (va != vb)
        أرجع va < vb ? -1 : 1;
    }
    أرجع 0;
  }
};


} /* namespace OT */


#د_که_پای /* HB_OT_VAR_COMMON_HH */

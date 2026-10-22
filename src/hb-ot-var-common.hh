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

#اگر_تعریف_نشده HB_OT_VAR_COMMON_HH
#تعریف_کن HB_OT_VAR_COMMON_HH

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-alloc-pool.hh"
#ضمن "hb-priority-queue.hh"
#ضمن "hb-subset-instancer-iup.hh"


نطاق OT {

استفاده rebase_tent_نتیجه_scratch_t = hb_pair_t<rebase_tent_نتیجه_t, rebase_tent_نتیجه_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
بنية Tuple_Variation_سرآیند
{
  دوست بنية tuple_delta_t;
  size_t get_اندازه (بیعلامت axis_شمار_times_2) ثابت
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    بیعلامت ti = tuple_شاخص;
    إذا (unlikely ((ti & (Tuple_شاخص::EmbeddedPeakTuple | Tuple_شاخص::IntermediateRegion))))
    {
      بیعلامت شمار = ((ti & Tuple_شاخص::EmbeddedPeakTuple) != 0) + ((ti & Tuple_شاخص::IntermediateRegion) != 0) * 2;
      أرجع hb_unsigned_mul_جمع_کن_saturate (شمار, axis_شمار_times_2, min_اندازه);
    }
    أرجع min_اندازه;
  }

  بیعلامت get_داده_اندازه () ثابت { أرجع var_داده_اندازه; }

  ثابت Tuple_Variation_سرآیند &get_بعدی (بیعلامت axis_شمار_times_2) ثابت
  { أرجع Struct_At_جابه_جایی<Tuple_Variation_سرآیند> (هذا, get_اندازه (axis_شمار_times_2)); }

  منطقي unpack_axis_tuples (بیعلامت axis_شمار,
                           ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                           ثابت hb_map_t *axes_old_شاخص_برچسب_map,
                           hb_hashmap_t<hb_برچسب_t, Triple>& axis_tuples /*بیرون*/) ثابت
  {
    ثابت F2DOT14 *peak_tuple = اشاره_تهی;
    إذا (has_peak ())
      peak_tuple = get_peak_tuple (axis_شمار);
    وإلا
    {
      بیعلامت صحيح شاخص_2 = get_شاخص ();
      إذا (unlikely ((شاخص_2 + 1) * axis_شمار > shared_tuples.درازی))
        أرجع خطأ;
      peak_tuple = shared_tuples.sub_array (axis_شمار * شاخص_2, axis_شمار).arrayZ;
    }

    ثابت F2DOT14 *شروع_tuple = اشاره_تهی;
    ثابت F2DOT14 *انتها_tuple = اشاره_تهی;
    منطقي has_interm = has_intermediate ();

    إذا (has_interm)
    {
      شروع_tuple = get_شروع_tuple (axis_شمار);
      انتها_tuple = get_انتها_tuple (axis_شمار);
    }

    لكل (بیعلامت i = 0; i < axis_شمار; i++)
    {
      عائم peak = peak_tuple[i].مقصد_float ();
      إذا (peak == 0.f) استمر;

      hb_برچسب_t *axis_برچسب_2;
      إذا (!axes_old_شاخص_برچسب_map->has (i, &axis_برچسب_2))
        أرجع خطأ;

      عائم شروع, انتها;
      إذا (has_interm)
      {
        شروع = شروع_tuple[i].مقصد_float ();
        انتها = انتها_tuple[i].مقصد_float ();
      }
      وإلا
      {
        شروع = hb_min (peak, 0.f);
        انتها = hb_max (peak, 0.f);
      }
      axis_tuples.مجموعه (*axis_برچسب_2, Triple (شروع, peak, انتها));
    }

    أرجع درست;
  }

  HB_ALWAYS_INLINE
  عائم calculate_scalar (hb_array_t<ثابت صحيح> coords, بیعلامت صحيح coord_شمار,
			   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ثابت
  {
    بیعلامت tuple_شاخص_2 = tuple_شاخص;
    بیعلامت صحيح شاخص_2 = tuple_شاخص_2 & Tuple_شاخص::Tuple_شاخص_ماسک;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    منطقي plain_shared_tuple = (tuple_شاخص_2 & ~Tuple_شاخص::خصوصی_نقطه_Numbers) < shared_tuple_scalar_cache->درازی;
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
      peak_tuple = get_peak_tuple (coord_شمار);
      shared_tuple_scalar_cache = اشاره_تهی;
    }
    وإلا
    {
      عائم scalar;
      إذا (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->درازی &&
	  shared_tuple_scalar_cache->get (شاخص_2, &scalar))
      {
        إذا (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = اشاره_تهی;
	وإلا
	  أرجع scalar;
      }

      إذا (unlikely ((شاخص_2 + 1) * coord_شمار > shared_tuples.درازی))
        أرجع 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_شمار * شاخص_2);

    }

    ثابت F2DOT14 *شروع_tuple = اشاره_تهی;
    ثابت F2DOT14 *انتها_tuple = اشاره_تهی;

    إذا (has_interm)
    {
      شروع_tuple = get_شروع_tuple (coord_شمار);
      انتها_tuple = get_انتها_tuple (coord_شمار);
    }

    عائم scalar = 1.f;
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
#إذا HB_FAST_رقم_ACCESS
    منطقي skip = coord_شمار >= 16;
#پایان_اگر
#پایان_اگر
    لكل (بیعلامت صحيح i = 0; i < coord_شمار; i++)
    {
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
#إذا HB_FAST_رقم_ACCESS
      إذا (skip)
      {
	طالما (i + 4 <= coord_شمار && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	طالما (i < coord_شمار && peak_tuple[i].مقصد_int () == 0)
	  i += 1;
	إذا (i >= coord_شمار)
	  اكسر;
      }
#پایان_اگر
#پایان_اگر

      صحيح peak = peak_tuple[i].مقصد_int ();
      إذا (!peak) استمر;

      صحيح v = coords[i];
      إذا (!v) { scalar = 0.f; اكسر; }
      إذا (v == peak) استمر;

      إذا (has_interm)
      {
	shared_tuple_scalar_cache = اشاره_تهی;
        صحيح شروع = شروع_tuple[i].مقصد_int ();
        صحيح انتها = انتها_tuple[i].مقصد_int ();
        إذا (unlikely (شروع > peak || peak > انتها ||
                      (شروع < 0 && انتها > 0 && peak))) استمر;
        إذا (v < شروع || v > انتها) { scalar = 0.f; اكسر; }
        إذا (v < peak)
        { إذا (peak != شروع) scalar *= (عائم) (v - شروع) / (peak - شروع); }
        وإلا
        { إذا (peak != انتها) scalar *= (عائم) (انتها - v) / (انتها - peak); }
      }
      وإلا إذا (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; اكسر; }
      وإلا
        scalar *= (عائم) v / peak;
    }
    إذا (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->مجموعه (شاخص_2, scalar);
    أرجع scalar;
  }

  منطقي           has_peak () ثابت { أرجع tuple_شاخص & Tuple_شاخص::EmbeddedPeakTuple; }
  منطقي   has_intermediate () ثابت { أرجع tuple_شاخص & Tuple_شاخص::IntermediateRegion; }
  منطقي has_خصوصی_points () ثابت { أرجع tuple_شاخص & Tuple_شاخص::خصوصی_نقطه_Numbers; }
  بیعلامت      get_شاخص () ثابت { أرجع tuple_شاخص & Tuple_شاخص::Tuple_شاخص_ماسک; }
  ساكن constexpr بیعلامت max_shared_tuple_شمار = 0x1000u;

  محمي:
  بنية Tuple_شاخص : HBUINT16
  {
    تعداد Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      خصوصی_نقطه_Numbers = 0x2000u,
      Tuple_شاخص_ماسک      = 0x0FFFu
    };

    Tuple_شاخص& عملگر = (uint16_t i) { HBUINT16::عملگر= (i); أرجع *هذا; }
    DEFINE_اندازه_STATIC (2);
  };

  hb_array_t<ثابت F2DOT14> get_all_tuples (بیعلامت axis_شمار) ثابت
  { أرجع StructAfter<Unsized_Array_از<F2DOT14>> (tuple_شاخص).as_array ((has_peak () + has_intermediate () * 2) * axis_شمار); }
  ثابت F2DOT14* get_all_tuples_پایه (بیعلامت axis_شمار) ثابت
  { أرجع StructAfter<Unsized_Array_از<F2DOT14>> (tuple_شاخص).arrayZ; }
  ثابت F2DOT14* get_peak_tuple (بیعلامت axis_شمار) ثابت
  { أرجع get_all_tuples_پایه (axis_شمار); }
  ثابت F2DOT14* get_شروع_tuple (بیعلامت axis_شمار) ثابت
  { أرجع get_all_tuples_پایه (axis_شمار) + has_peak () * axis_شمار; }
  ثابت F2DOT14* get_انتها_tuple (بیعلامت axis_شمار) ثابت
  { أرجع get_all_tuples_پایه (axis_شمار) + has_peak () * axis_شمار + axis_شمار; }

  HBUINT16      var_داده_اندازه;    /* The size in bytes of the serialized
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
  hb_وکتور_t<منطقي> opt_indices;
  hb_وکتور_t<صحيح> rounded_x_deltas;
  hb_وکتور_t<صحيح> rounded_y_deltas;
  hb_وکتور_t<عائم> opt_deltas_x;
  hb_وکتور_t<عائم> opt_deltas_y;
  hb_وکتور_t<بیعلامت حرف> opt_نقطه_داده;
  hb_وکتور_t<بیعلامت حرف> opt_deltas_داده;
  hb_وکتور_t<بیعلامت حرف> نقطه_داده;
  hb_وکتور_t<بیعلامت حرف> deltas_داده;
  hb_وکتور_t<صحيح> rounded_deltas;
};

بنية tuple_delta_t
{
  ساكن constexpr منطقي realloc_move = درست;  // Watch out when adding new members!

  عام:
  hb_hashmap_t<hb_برچسب_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_وکتور_t<منطقي> indices;

  hb_وکتور_t<عائم> deltas_x;
  /* empty for cvar tuples */
  hb_وکتور_t<عائم> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_وکتور_t<بیعلامت حرف> compiled_tuple_سرآیند;
  hb_وکتور_t<بیعلامت حرف> compiled_deltas;

  hb_وکتور_t<F2DOT14> compiled_peak_coords;
  hb_وکتور_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = اشاره_تهی) {}
  tuple_delta_t (ثابت tuple_delta_t& o) = افتراضي;
  tuple_delta_t& عملگر = (ثابت tuple_delta_t& o) = افتراضي;

  دوست فراغ swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
  {
    hb_جابجا (a.axis_tuples, b.axis_tuples);
    hb_جابجا (a.indices, b.indices);
    hb_جابجا (a.deltas_x, b.deltas_x);
    hb_جابجا (a.deltas_y, b.deltas_y);
    hb_جابجا (a.compiled_tuple_سرآیند, b.compiled_tuple_سرآیند);
    hb_جابجا (a.compiled_deltas, b.compiled_deltas);
    hb_جابجا (a.compiled_peak_coords, b.compiled_peak_coords);
  }

  tuple_delta_t (tuple_delta_t&& o)  noexcept : tuple_delta_t ()
  { hb_جابجا (*هذا, o); }

  tuple_delta_t& عملگر = (tuple_delta_t&& o) noexcept
  {
    hb_جابجا (*هذا, o);
    أرجع *هذا;
  }

  فراغ copy_from (ثابت tuple_delta_t& o, hb_alloc_pool_t *pool = اشاره_تهی)
  {
    axis_tuples = o.axis_tuples;
    indices.duplicate_وکتور_from_pool (pool, o.indices);
    deltas_x.duplicate_وکتور_from_pool (pool, o.deltas_x);
    deltas_y.duplicate_وکتور_from_pool (pool, o.deltas_y);
    compiled_tuple_سرآیند.duplicate_وکتور_from_pool (pool, o.compiled_tuple_سرآیند);
    compiled_deltas.duplicate_وکتور_from_pool (pool, o.compiled_deltas);
    compiled_peak_coords.duplicate_وکتور_from_pool (pool, o.compiled_peak_coords);
    compiled_interm_coords.duplicate_وکتور_from_pool (pool, o.compiled_interm_coords);
  }

  فراغ remove_axis (hb_برچسب_t axis_برچسب_2)
  { axis_tuples.del (axis_برچسب_2); }

  منطقي مجموعه_tent (hb_برچسب_t axis_برچسب_2, Triple tent)
  { أرجع axis_tuples.مجموعه (axis_برچسب_2, tent); }

  tuple_delta_t& عملگر += (ثابت tuple_delta_t& o)
  {
    بیعلامت رقم = indices.درازی;
    لكل (بیعلامت i = 0; i < رقم; i++)
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
        indices.arrayZ[i] = درست;
        deltas_x[i] = o.deltas_x[i];
        إذا (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    أرجع *هذا;
  }

  tuple_delta_t& عملگر *= (عائم scalar)
  {
    إذا (scalar == 1.0f)
      أرجع *هذا;

    بیعلامت رقم = indices.درازی;
    إذا (deltas_y)
      لكل (بیعلامت i = 0; i < رقم; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    وإلا
      لكل (بیعلامت i = 0; i < رقم; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
      }
    أرجع *هذا;
  }

  فراغ change_tuple_var_axis_حد (hb_برچسب_t axis_برچسب_2, Triple axis_حد,
				    TripleDistances axis_triple_distances,
				    hb_وکتور_t<tuple_delta_t>& بیرون,
				    rebase_tent_نتیجه_scratch_t &scratch,
				    hb_alloc_pool_t *pool = اشاره_تهی)
  {
    // May move *this out.

    بیرون.reset ();
    Triple *tent;
    إذا (!axis_tuples.has (axis_برچسب_2, &tent))
    {
      بیرون.push (std::move (*هذا));
      أرجع;
    }

    إذا ((tent->minimum < 0.f && tent->بیشینه > 0.f) ||
        !(tent->minimum <= tent->میانه && tent->میانه <= tent->بیشینه))
      أرجع;

    إذا (tent->میانه == 0.f)
    {
      بیرون.push (std::move (*هذا));
      أرجع;
    }

    إذا (!axis_حد.is_نقطه () &&
        !(-1.f <= axis_حد.minimum &&
          axis_حد.minimum <= axis_حد.میانه &&
          axis_حد.میانه <= axis_حد.بیشینه &&
          axis_حد.بیشینه <= +1.f))
      أرجع;

    rebase_tent_نتیجه_t &راهحلها = scratch.اولی;
    rebase_tent (*tent, axis_حد, axis_triple_distances, راهحلها, scratch.second);
    لكل (بیعلامت i = 0; i < راهحلها.درازی; i++)
    {
      خودکار &t = راهحلها.arrayZ[i];

      tuple_delta_t new_var;
      إذا (i < راهحلها.درازی - 1)
	new_var.copy_from (*هذا, pool);
      وإلا
	new_var = std::move (*هذا);

      إذا (t.second == Triple ())
        new_var.remove_axis (axis_برچسب_2);
      وإلا
        new_var.مجموعه_tent (axis_برچسب_2, t.second);

      new_var *= t.اولی;
      بیرون.push (std::move (new_var));
    }
  }

  منطقي compile_coords (ثابت hb_map_t& axes_شاخص_map,
		       ثابت hb_map_t& axes_old_شاخص_برچسب_map,
		       hb_alloc_pool_t *pool= اشاره_تهی)
  {
    بیعلامت cur_axis_شمار = axes_شاخص_map.get_population ();
    إذا (pool)
    {
      إذا (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_شمار)))
	أرجع خطأ;
    }
    وإلا إذا (unlikely (!compiled_peak_coords.resize (cur_axis_شمار)))
      أرجع خطأ;

    hb_array_t<F2DOT14> شروع_coords, انتها_coords;

    بیعلامت orig_axis_شمار = axes_old_شاخص_برچسب_map.get_population ();
    بیعلامت j = 0;
    لكل (بیعلامت i = 0; i < orig_axis_شمار; i++)
    {
      إذا (!axes_شاخص_map.has (i))
        استمر;

      hb_برچسب_t axis_برچسب_2 = axes_old_شاخص_برچسب_map.get (i);
      Triple *coords = اشاره_تهی;
      إذا (axis_tuples.has (axis_برچسب_2, &coords))
      {
	عائم min_val = coords->minimum;
	عائم val = coords->میانه;
	عائم max_val = coords->بیشینه;

	compiled_peak_coords.arrayZ[j].مجموعه_float (val);

	إذا (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  إذا (!compiled_interm_coords)
	  {
	    إذا (pool)
	    {
	      إذا (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_شمار)))
		أرجع خطأ;
	    }
	    وإلا إذا (unlikely (!compiled_interm_coords.resize (2 * cur_axis_شمار)))
	      أرجع خطأ;
	    شروع_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_شمار);
	    انتها_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_شمار);

	    لكل (بیعلامت k = 0; k < j; k++)
	    {
	      علامتدار peak = compiled_peak_coords.arrayZ[k].مقصد_int ();
	      إذا (!peak) استمر;
	      شروع_coords.arrayZ[k].مجموعه_int (hb_min (peak, 0));
	      انتها_coords.arrayZ[k].مجموعه_int (hb_max (peak, 0));
	    }
	  }

	}

	إذا (compiled_interm_coords)
	{
	  شروع_coords.arrayZ[j].مجموعه_float (min_val);
	  انتها_coords.arrayZ[j].مجموعه_float (max_val);
	}
      }

      j++;
    }

    أرجع !compiled_peak_coords.درون_error () && !compiled_interm_coords.درون_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  منطقي compile_tuple_var_سرآیند (ثابت hb_map_t& axes_شاخص_map,
                                 بیعلامت points_داده_درازی,
                                 ثابت hb_map_t& axes_old_شاخص_برچسب_map,
                                 ثابت hb_hashmap_t<ثابت hb_وکتور_t<F2DOT14>*, بیعلامت>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = اشاره_تهی)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    إذا (!compiled_deltas) أرجع درست;

    بیعلامت cur_axis_شمار = axes_شاخص_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    بیعلامت alloc_len = 3 * cur_axis_شمار * (F2DOT14::static_اندازه) + 4;
    إذا (unlikely (!compiled_tuple_سرآیند.allocate_from_pool (pool, alloc_len, خطأ))) أرجع خطأ;

    بیعلامت flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرآیند.begin () + 4);
    F2DOT14* انتها = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرآیند.انتها ());
    hb_array_t<F2DOT14> coords (p, انتها - p);

    إذا (!shared_tuples_idx_map)
      compile_coords (axes_شاخص_map, axes_old_شاخص_برچسب_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    بیعلامت peak_شمار = 0;
    بیعلامت *shared_tuple_idx;
    إذا (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    وإلا
    {
      peak_شمار = encode_peak_coords(coords, flag);
      إذا (!peak_شمار) أرجع خطأ;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    بیعلامت interim_شمار = encode_interm_coords (coords.sub_array (peak_شمار), flag);

    /* pointdata length = 0 implies "use shared points" */
    إذا (points_داده_درازی)
      flag |= Tuple_Variation_سرآیند::Tuple_شاخص::خصوصی_نقطه_Numbers;

    بیعلامت serialized_داده_اندازه = points_داده_درازی + compiled_deltas.درازی;
    Tuple_Variation_سرآیند *o = reinterpret_cast<Tuple_Variation_سرآیند *> (compiled_tuple_سرآیند.begin ());
    o->var_داده_اندازه = serialized_داده_اندازه;
    o->tuple_شاخص = flag;

    بیعلامت مجموع_سرآیند_len = 4 + (peak_شمار + interim_شمار) * (F2DOT14::static_اندازه);
    compiled_tuple_سرآیند.shrink_پشت_مقصد_pool (pool, مجموع_سرآیند_len);
    أرجع درست;
  }

  بیعلامت encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               بیعلامت& flag) ثابت
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.درازی * حجم (compiled_peak_coords[0]));
    flag |= Tuple_Variation_سرآیند::Tuple_شاخص::EmbeddedPeakTuple;
    أرجع compiled_peak_coords.درازی;
  }

  /* if no need to encode intermediate coords, then just return p */
  بیعلامت encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 بیعلامت& flag) ثابت
  {
    إذا (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.درازی * حجم (compiled_interm_coords[0]));
      flag |= Tuple_Variation_سرآیند::Tuple_شاخص::IntermediateRegion;
    }
    أرجع compiled_interm_coords.درازی;
  }

  منطقي compile_deltas (hb_وکتور_t<صحيح> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = اشاره_تهی)
  { أرجع compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ساكن منطقي compile_deltas (hb_array_t<ثابت منطقي> نقطه_indices,
			      hb_array_t<ثابت عائم> x_deltas,
			      hb_array_t<ثابت عائم> y_deltas,
			      hb_وکتور_t<بیعلامت حرف> &compiled_deltas, /*بیرون*/
			      hb_وکتور_t<صحيح> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = اشاره_تهی)
  {
    إذا (unlikely (!rounded_deltas.resize_dirty  (نقطه_indices.درازی)))
      أرجع خطأ;

    بیعلامت j = 0;
    لكل (بیعلامت i = 0; i < نقطه_indices.درازی; i++)
    {
      إذا (!نقطه_indices[i]) استمر;
      rounded_deltas.arrayZ[j++] = hb_clamp_مقصد<صحيح> (roundf ((مزدوج) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    إذا (!rounded_deltas) أرجع درست;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    بیعلامت alloc_len = 2 + 4 * rounded_deltas.درازی + (rounded_deltas.درازی + 63) / 64;
    إذا (y_deltas)
      alloc_len *= 2;

    إذا (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, خطأ))) أرجع خطأ;

    بیعلامت encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    إذا (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      بیعلامت j = 0;
      لكل (بیعلامت idx = 0; idx < نقطه_indices.درازی; idx++)
      {
        إذا (!نقطه_indices[idx]) استمر;
        صحيح rounded_delta = hb_clamp_مقصد<صحيح> (roundf ((مزدوج) y_deltas.arrayZ[idx]));

        إذا (j >= rounded_deltas.درازی) أرجع خطأ;

        rounded_deltas[j++] = rounded_delta;
      }

      إذا (j != rounded_deltas.درازی) أرجع خطأ;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_پشت_مقصد_pool (pool, encoded_len);
    أرجع درست;
  }

  ساكن بیعلامت compile_deltas (hb_array_t<بیعلامت حرف> encoded_بایتها,
				  hb_array_t<ثابت صحيح> deltas)
  {
    أرجع Tuple_مقادیر::compile_unsafe (deltas, encoded_بایتها);
  }

  منطقي calc_inferred_deltas (ثابت contour_نقطه_وکتور_t& orig_points,
			     hb_وکتور_t<بیعلامت> &scratch)
  {
    بیعلامت نقطه_شمار = orig_points.درازی;
    إذا (نقطه_شمار != indices.درازی)
      أرجع خطأ;

    بیعلامت ref_شمار_2 = 0;

    hb_وکتور_t<بیعلامت> &انتها_points = scratch.reset ();

    لكل (بیعلامت i = 0; i < نقطه_شمار; i++)
    {
      ref_شمار_2 += indices.arrayZ[i];
      إذا (orig_points.arrayZ[i].is_انتها_نقطه)
        انتها_points.push (i);
    }
    /* all points are referenced, nothing to do */
    إذا (ref_شمار_2 == نقطه_شمار)
      أرجع درست;
    إذا (unlikely (انتها_points.درون_error ())) أرجع خطأ;

    hb_بیت_مجموعه_t inferred_idxes;
    بیعلامت شروع_نقطه_2 = 0;
    لكل (بیعلامت انتها_نقطه : انتها_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      بیعلامت unref_شمار = 0;
      لكل (بیعلامت i = شروع_نقطه_2; i < انتها_نقطه + 1; i++)
        unref_شمار += indices.arrayZ[i];
      unref_شمار = (انتها_نقطه - شروع_نقطه_2 + 1) - unref_شمار;

      بیعلامت j = شروع_نقطه_2;
      إذا (unref_شمار == 0 || unref_شمار > انتها_نقطه - شروع_نقطه_2)
        اذهب no_بیشتر_gaps;
      لكل (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        بیعلامت صحيح prev, next, i;
        لكل (;;)
        {
          i = j;
          j = بعدی_شاخص (i, شروع_نقطه_2, انتها_نقطه);
          إذا (indices.arrayZ[i] && !indices.arrayZ[j]) اكسر;
        }
        prev = j = i;
        لكل (;;)
        {
          i = j;
          j = بعدی_شاخص (i, شروع_نقطه_2, انتها_نقطه);
          إذا (!indices.arrayZ[i] && indices.arrayZ[j]) اكسر;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        لكل (;;)
        {
          i = بعدی_شاخص (i, شروع_نقطه_2, انتها_نقطه);
          إذا (i == next) اكسر;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.جمع_کن (i);
          إذا (--unref_شمار == 0) اذهب no_بیشتر_gaps;
        }
      }
    no_بیشتر_gaps:
      شروع_نقطه_2 = انتها_نقطه + 1;
    }

    لكل (بیعلامت i = 0; i < نقطه_شمار; i++)
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
        indices[i] = درست;
      }
    }
    أرجع درست;
  }

  منطقي optimize (ثابت contour_نقطه_وکتور_t& contour_points,
                 منطقي is_مرکب,
		 optimize_scratch_t &scratch,
                 مزدوج tolerance = 0.5 + 1e-10)
  {
    بیعلامت شمار = contour_points.درازی;
    إذا (deltas_x.درازی != شمار ||
        deltas_y.درازی != شمار)
      أرجع خطأ;

    hb_وکتور_t<منطقي> &opt_indices = scratch.opt_indices.reset ();
    hb_وکتور_t<صحيح> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_وکتور_t<صحيح> &rounded_y_deltas = scratch.rounded_y_deltas;

    إذا (unlikely (!rounded_x_deltas.resize_dirty  (شمار) ||
                  !rounded_y_deltas.resize_dirty  (شمار)))
      أرجع خطأ;

    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_مقصد<صحيح> (roundf ((مزدوج) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_مقصد<صحيح> (roundf ((مزدوج) deltas_y.arrayZ[i]));
    }

    إذا (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      أرجع خطأ;

    بیعلامت ref_شمار_2 = 0;
    لكل (منطقي ref_flag : opt_indices)
       ref_شمار_2 += ref_flag;

    إذا (ref_شمار_2 == شمار) أرجع درست;

    hb_وکتور_t<عائم> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_وکتور_t<عائم> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    منطقي is_comp_glyph_wo_deltas = (is_مرکب && ref_شمار_2 == 0);
    إذا (is_comp_glyph_wo_deltas)
    {
      إذا (unlikely (!opt_deltas_x.resize (شمار) ||
                    !opt_deltas_y.resize (شمار)))
        أرجع خطأ;

      opt_indices.arrayZ[0] = درست;
      لكل (بیعلامت i = 1; i < شمار; i++)
        opt_indices.arrayZ[i] = خطأ;
    }

    hb_وکتور_t<بیعلامت حرف> &opt_نقطه_داده = scratch.opt_نقطه_داده.reset ();
    إذا (!compile_نقطه_مجموعه (opt_indices, opt_نقطه_داده))
      أرجع خطأ;
    hb_وکتور_t<بیعلامت حرف> &opt_deltas_داده = scratch.opt_deltas_داده.reset ();
    إذا (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_داده,
			 scratch.rounded_deltas))
      أرجع خطأ;

    hb_وکتور_t<بیعلامت حرف> &نقطه_داده = scratch.نقطه_داده.reset ();
    إذا (!compile_نقطه_مجموعه (indices, نقطه_داده))
      أرجع خطأ;
    hb_وکتور_t<بیعلامت حرف> &deltas_داده = scratch.deltas_داده.reset ();
    إذا (!compile_deltas (indices, deltas_x, deltas_y, deltas_داده, scratch.rounded_deltas))
      أرجع خطأ;

    إذا (opt_نقطه_داده.درازی + opt_deltas_داده.درازی < نقطه_داده.درازی + deltas_داده.درازی)
    {
      indices = std::move (opt_indices);

      إذا (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    أرجع !indices.درون_error () && !deltas_x.درون_error () && !deltas_y.درون_error ();
  }

  ساكن منطقي compile_نقطه_مجموعه (ثابت hb_وکتور_t<منطقي> &نقطه_indices,
                                 hb_وکتور_t<بیعلامت حرف>& compiled_points /*بیرون*/)
  {
    بیعلامت رقم_points = 0;
    لكل (منطقي i : نقطه_indices)
      إذا (i) رقم_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    إذا (!رقم_points) أرجع درست;

    بیعلامت indices_درازی = نقطه_indices.درازی;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    إذا (رقم_points == indices_درازی)
      أرجع compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    بیعلامت رقم_بایتها = 2 + 3 *رقم_points;
    إذا (unlikely (!compiled_points.resize_dirty  (رقم_بایتها)))
      أرجع خطأ;

    بیعلامت pos = 0;
    /* binary data starts with the total number of reference points */
    إذا (رقم_points < 0x80)
      compiled_points.arrayZ[pos++] = رقم_points;
    وإلا
    {
      compiled_points.arrayZ[pos++] = ((رقم_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = رقم_points & 0xFF;
    }

    ثابت بیعلامت max_پیاپی_درازی = 0x7F;
    بیعلامت i = 0;
    بیعلامت آخرین_مقدار = 0;
    بیعلامت رقم_encoded = 0;
    طالما (i < indices_درازی && رقم_encoded < رقم_points)
    {
      بیعلامت پیاپی_درازی = 0;
      بیعلامت سرآیند_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      منطقي use_byte_encoding = خطأ;
      منطقي new_پیاپی_2 = درست;
      طالما (i < indices_درازی && رقم_encoded < رقم_points &&
             پیاپی_درازی <= max_پیاپی_درازی)
      {
        // find out next referenced point index
        طالما (i < indices_درازی && !نقطه_indices[i])
          i++;

        إذا (i >= indices_درازی) اكسر;

        بیعلامت cur_مقدار = i;
        بیعلامت delta = cur_مقدار - آخرین_مقدار;

        إذا (new_پیاپی_2)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_پیاپی_2 = خطأ;
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
        آخرین_مقدار = cur_مقدار;
        پیاپی_درازی++;
        رقم_encoded++;
      }

      إذا (use_byte_encoding)
        compiled_points.arrayZ[سرآیند_pos] = پیاپی_درازی - 1;
      وإلا
        compiled_points.arrayZ[سرآیند_pos] = (پیاپی_درازی - 1) | 0x80;
    }
    أرجع compiled_points.resize_dirty  (pos);
  }

  ساكن عائم infer_delta (عائم هدف_val, عائم prev_val, عائم بعدی_val, عائم prev_delta, عائم بعدی_delta)
  {
    إذا (prev_val == بعدی_val)
      أرجع (prev_delta == بعدی_delta) ? prev_delta : 0.f;
    وإلا إذا (هدف_val <= hb_min (prev_val, بعدی_val))
      أرجع (prev_val < بعدی_val) ? prev_delta : بعدی_delta;
    وإلا إذا (هدف_val >= hb_max (prev_val, بعدی_val))
      أرجع (prev_val > بعدی_val) ? prev_delta : بعدی_delta;

    عائم r = (هدف_val - prev_val) / (بعدی_val - prev_val);
    أرجع prev_delta + r * (بعدی_delta - prev_delta);
  }

  ساكن بیعلامت صحيح بعدی_شاخص (بیعلامت صحيح i, بیعلامت صحيح شروع, بیعلامت صحيح انتها)
  { أرجع (i >= انتها) ? شروع : (i + 1); }
};

قالب <نام_نوع OffType = HBUINT16>
بنية Tuple_Variation_داده
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (هذا));
  }

  size_t get_اندازه (بیعلامت axis_شمار_times_2) ثابت
  {
    size_t مجموع_اندازه = min_اندازه;
    بیعلامت شمار = tuple_Var_شمار.get_شمار ();
    ثابت Tuple_Variation_سرآیند *tuple_var_سرآیند = &(get_tuple_var_سرآیند());
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      مجموع_اندازه = hb_unsigned_جمع_کن_saturate (مجموع_اندازه,
					     tuple_var_سرآیند->get_اندازه (axis_شمار_times_2),
					     (size_t) tuple_var_سرآیند->get_داده_اندازه ());
      tuple_var_سرآیند = &tuple_var_سرآیند->get_بعدی (axis_شمار_times_2);
    }

    أرجع مجموع_اندازه;
  }

  ثابت Tuple_Variation_سرآیند &get_tuple_var_سرآیند (فراغ) ثابت
  { أرجع StructAfter<Tuple_Variation_سرآیند> (داده); }

  بنية tuple_iterator_t;
  بنية tuple_variations_t
  {
    hb_وکتور_t<tuple_delta_t> tuple_vars;

    خاص:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ثابت hb_وکتور_t<منطقي>*, hb_وکتور_t<بیعلامت حرف>> نقطه_داده_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ثابت hb_وکتور_t<منطقي>*, بیعلامت> نقطه_مجموعه_شمار_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_وکتور_t<بیعلامت حرف> *shared_points_بایتها = اشاره_تهی;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    بیعلامت compiled_byte_اندازه = 0;
    منطقي needs_padding = خطأ;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    منطقي is_مرکب = خطأ;

    عام:
    tuple_variations_t () = افتراضي;
    tuple_variations_t (ثابت tuple_variations_t&) = حذف;
    tuple_variations_t& عملگر=(ثابت tuple_variations_t&) = حذف;
    tuple_variations_t (tuple_variations_t&&) = افتراضي;
    tuple_variations_t& عملگر=(tuple_variations_t&&) = افتراضي;
    ~tuple_variations_t () = افتراضي;

    صریح عملگر منطقي () ثابت { أرجع منطقي (tuple_vars); }
    بیعلامت get_var_شمار () ثابت
    {
      بیعلامت شمار = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      لكل (خودکار& tuple: tuple_vars)
        إذا (tuple.compiled_deltas) شمار++;

      إذا (shared_points_بایتها && shared_points_بایتها->درازی)
        شمار |= Tuple_Var_شمار::Shared_نقطه_Numbers;
      أرجع شمار;
    }

    بیعلامت get_compiled_byte_اندازه () ثابت
    { أرجع compiled_byte_اندازه; }

    منطقي create_from_tuple_var_داده (tuple_iterator_t iterator,
                                     بیعلامت tuple_var_شمار,
                                     بیعلامت نقطه_شمار,
                                     منطقي is_gvar,
                                     ثابت hb_map_t *axes_old_شاخص_برچسب_map,
                                     ثابت hb_وکتور_t<بیعلامت> &shared_indices,
                                     ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = اشاره_تهی,
                                     منطقي is_مرکب_glyph = خطأ)
    {
      hb_وکتور_t<بیعلامت> خصوصی_indices;
      hb_وکتور_t<صحيح> deltas_x;
      hb_وکتور_t<صحيح> deltas_y;
      افعل
      {
        ثابت HBUINT8 *p = iterator.get_serialized_داده ();
        بیعلامت صحيح درازی = iterator.current_tuple->get_داده_اندازه ();
        إذا (unlikely (!iterator.var_داده_بایتها.check_range (p, درازی)))
          أرجع خطأ;

	hb_hashmap_t<hb_برچسب_t, Triple> axis_tuples;
        إذا (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_شمار (), shared_tuples, axes_old_شاخص_برچسب_map, axis_tuples)
            || axis_tuples.is_empty ())
          أرجع خطأ;

	خصوصی_indices.reset ();
        منطقي has_خصوصی_points = iterator.current_tuple->has_خصوصی_points ();
        ثابت HBUINT8 *انتها = p + درازی;
        إذا (has_خصوصی_points &&
            !Tuple_Variation_داده::decompile_points (p, خصوصی_indices, انتها))
          أرجع خطأ;

        ثابت hb_وکتور_t<بیعلامت> &indices = has_خصوصی_points ? خصوصی_indices : shared_indices;
        منطقي apply_مقصد_all = (indices.درازی == 0);
        بیعلامت رقم_deltas = apply_مقصد_all ? نقطه_شمار : indices.درازی;

        إذا (unlikely (!deltas_x.resize_dirty  (رقم_deltas) ||
                      !Tuple_Variation_داده::decompile_deltas (p, deltas_x, انتها)))
          أرجع خطأ;

        إذا (is_gvar)
        {
          إذا (unlikely (!deltas_y.resize_dirty  (رقم_deltas) ||
                        !Tuple_Variation_داده::decompile_deltas (p, deltas_y, انتها)))
            أرجع خطأ;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        إذا (unlikely (!var.indices.allocate_from_pool (pool, نقطه_شمار) ||
                      !var.deltas_x.allocate_from_pool (pool, نقطه_شمار, خطأ)))
          أرجع خطأ;

        إذا (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, نقطه_شمار, خطأ)))
          أرجع خطأ;

        لكل (بیعلامت i = 0; i < رقم_deltas; i++)
        {
          بیعلامت idx = apply_مقصد_all ? i : indices[i];
          إذا (idx >= نقطه_شمار) استمر;
          var.indices[idx] = درست;
          var.deltas_x[idx] = deltas_x[i];
          إذا (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } طالما (iterator.move_مقصد_بعدی ());

      is_مرکب = is_مرکب_glyph;
      أرجع درست;
    }

    منطقي create_from_قلم_var_داده (ثابت Var_داده &var_داده,
                                    ثابت hb_وکتور_t<hb_hashmap_t<hb_برچسب_t, Triple>>& regions,
                                    ثابت hb_map_t& axes_old_شاخص_برچسب_map,
                                    بیعلامت& قلم_شمار_2,
                                    ثابت hb_inc_bimap_t* inner_map = اشاره_تهی)
    {
      /* NULL offset, to keep original varidx valid, just return */
      إذا (&var_داده == &Null (Var_داده))
        أرجع درست;

      بیعلامت رقم_regions = var_داده.get_region_شاخص_شمار ();
      إذا (!tuple_vars.alloc (رقم_regions)) أرجع خطأ;

      قلم_شمار_2 = inner_map ? inner_map->get_population () : var_داده.get_قلم_شمار ();
      إذا (!قلم_شمار_2) أرجع درست;
      بیعلامت ردیف_اندازه = var_داده.get_ردیف_اندازه ();
      ثابت HBUINT8 *delta_بایتها = var_داده.get_delta_بایتها ();

      لكل (بیعلامت r = 0; r < رقم_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        إذا (!tuple.deltas_x.resize_dirty  (قلم_شمار_2) ||
            !tuple.indices.resize_dirty  (قلم_شمار_2))
          أرجع خطأ;

        لكل (بیعلامت i = 0; i < قلم_شمار_2; i++)
        {
          tuple.indices.arrayZ[i] = درست;
          tuple.deltas_x.arrayZ[i] = var_داده.get_قلم_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_بایتها, ردیف_اندازه);
        }

        بیعلامت region_شاخص = var_داده.get_region_شاخص (r);
        إذا (region_شاخص >= regions.درازی) أرجع خطأ;
        tuple.axis_tuples = regions.arrayZ[region_شاخص];

        tuple_vars.push (std::move (tuple));
      }
      أرجع !tuple_vars.درون_error ();
    }

    خاص:
    ساكن صحيح cmp_axis_برچسب (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت hb_برچسب_t *a = (ثابت hb_برچسب_t*) pa;
      ثابت hb_برچسب_t *b = (ثابت hb_برچسب_t*) pb;
      أرجع (صحيح)(*a) - (صحيح)(*b);
    }

    منطقي change_tuple_variations_axis_limits (ثابت hb_hashmap_t<hb_برچسب_t, Triple>& normalized_axes_location,
                                              ثابت hb_hashmap_t<hb_برچسب_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = اشاره_تهی)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_وکتور_t<hb_برچسب_t> axis_برچسبها;
      إذا (!axis_برچسبها.alloc (normalized_axes_location.get_population ()))
        أرجع خطأ;
      لكل (خودکار t : normalized_axes_location.کلیدها ())
        axis_برچسبها.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_نتیجه_scratch_t scratch;
      hb_وکتور_t<tuple_delta_t> بیرون;

      axis_برچسبها.qsort (cmp_axis_برچسب);
      لكل (خودکار axis_برچسب_2 : axis_برچسبها)
      {
        Triple *axis_حد;
        إذا (!normalized_axes_location.has (axis_برچسب_2, &axis_حد))
          أرجع خطأ;
        TripleDistances axis_triple_distances{1.f, 1.f};
        إذا (axes_triple_distances.has (axis_برچسب_2))
          axis_triple_distances = axes_triple_distances.get (axis_برچسب_2);

        hb_وکتور_t<tuple_delta_t> new_vars;
        لكل (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_حد (axis_برچسب_2, *axis_حد, axis_triple_distances, بیرون, scratch, pool);
          إذا (!بیرون) استمر;

          بیعلامت new_len = new_vars.درازی + بیرون.درازی;

          إذا (unlikely (!new_vars.alloc (new_len, خطأ)))
            أرجع خطأ;

          لكل (بیعلامت i = 0; i < بیرون.درازی; i++)
            new_vars.push (std::move (بیرون[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      أرجع درست;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    منطقي ادغام_tuple_variations (contour_نقطه_وکتور_t* contour_points = اشاره_تهی)
    {
      hb_وکتور_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      إذا (unlikely (!new_vars.alloc (tuple_vars.درازی)))
	أرجع خطأ;
      hb_hashmap_t<ثابت hb_hashmap_t<hb_برچسب_t, Triple>*, بیعلامت> m;
      لكل (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        إذا (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          إذا (contour_points && !contour_points->جمع_کن_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            أرجع خطأ;
          استمر;
        }

        بیعلامت *idx;
        إذا (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        وإلا
        {
	  خودکار *new_var = new_vars.push ();
	  إذا (unlikely (new_vars.درون_error ()))
	    أرجع خطأ;
          hb_جابجا (*new_var, var);
          إذا (unlikely (!m.مجموعه (&(new_var->axis_tuples), new_vars.درازی - 1)))
            أرجع خطأ;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      أرجع درست;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    منطقي compile_all_نقطه_sets ()
    {
      لكل (ثابت خودکار& tuple: tuple_vars)
      {
        ثابت hb_وکتور_t<منطقي>* points_مجموعه = &(tuple.indices);
        إذا (نقطه_داده_map.has (points_مجموعه))
        {
          بیعلامت *شمار;
          إذا (unlikely (!نقطه_مجموعه_شمار_map.has (points_مجموعه, &شمار) ||
                        !نقطه_مجموعه_شمار_map.مجموعه (points_مجموعه, (*شمار) + 1)))
            أرجع خطأ;
          استمر;
        }

        hb_وکتور_t<بیعلامت حرف> compiled_نقطه_داده;
        إذا (!tuple_delta_t::compile_نقطه_مجموعه (*points_مجموعه, compiled_نقطه_داده))
          أرجع خطأ;

        إذا (!نقطه_داده_map.مجموعه (points_مجموعه, std::move (compiled_نقطه_داده)) ||
            !نقطه_مجموعه_شمار_map.مجموعه (points_مجموعه, 1))
          أرجع خطأ;
      }
      أرجع درست;
    }

    /* find shared points set which saves most bytes */
    فراغ پیدا_کن_shared_points ()
    {
      بیعلامت max_saved_بایتها = 0;

      لكل (ثابت خودکار& _ : نقطه_داده_map.iter_ref ())
      {
        ثابت hb_وکتور_t<منطقي>* points_مجموعه = _.اولی;
        بیعلامت داده_درازی = _.second.درازی;
        إذا (!داده_درازی) استمر;
        بیعلامت *شمار;
        إذا (unlikely (!نقطه_مجموعه_شمار_map.has (points_مجموعه, &شمار) ||
                      *شمار <= 1))
        {
          shared_points_بایتها = اشاره_تهی;
          أرجع;
        }

        بیعلامت saved_بایتها = داده_درازی * ((*شمار) -1);
        إذا (saved_بایتها > max_saved_بایتها)
        {
          max_saved_بایتها = saved_بایتها;
          shared_points_بایتها = &(_.second);
        }
      }
    }

    منطقي calc_inferred_deltas (ثابت contour_نقطه_وکتور_t& contour_points,
			       hb_وکتور_t<بیعلامت> &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
        إذا (!var.calc_inferred_deltas (contour_points, scratch))
          أرجع خطأ;

      أرجع درست;
    }

    منطقي iup_optimize (ثابت contour_نقطه_وکتور_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
      {
        إذا (!var.optimize (contour_points, is_مرکب, scratch))
          أرجع خطأ;
      }
      أرجع درست;
    }

    عام:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    فراغ cull_unreachable (ثابت hb_hashmap_t<hb_برچسب_t, Triple> &reachable_ranges)
    {
      بیعلامت j = 0;
      لكل (بیعلامت i = 0; i < tuple_vars.درازی; i++)
      {
	إذا (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  استمر;
	إذا (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    منطقي instantiate (ثابت hb_hashmap_t<hb_برچسب_t, Triple>& normalized_axes_location,
                      ثابت hb_hashmap_t<hb_برچسب_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = اشاره_تهی,
                      contour_نقطه_وکتور_t* contour_points = اشاره_تهی,
                      منطقي optimize = خطأ)
    {
      إذا (!tuple_vars) أرجع درست;
      إذا (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        أرجع خطأ;
      /* compute inferred deltas only for gvar */
      إذا (contour_points)
      {
	hb_وکتور_t<بیعلامت> scratch;
	إذا (!calc_inferred_deltas (*contour_points, scratch))
	  أرجع خطأ;
      }

      /* if iup delta opt is on, contour_points can't be null */
      إذا (optimize && !contour_points)
        أرجع خطأ;

      إذا (!ادغام_tuple_variations (optimize ? contour_points : اشاره_تهی))
        أرجع خطأ;

      إذا (optimize && !iup_optimize (*contour_points, scratch)) أرجع خطأ;
      أرجع !tuple_vars.درون_error ();
    }

    منطقي compile_بایتها (ثابت hb_map_t& axes_شاخص_map,
                        ثابت hb_map_t& axes_old_شاخص_برچسب_map,
                        منطقي use_shared_points,
                        منطقي is_gvar = خطأ,
                        ثابت hb_hashmap_t<ثابت hb_وکتور_t<F2DOT14>*, بیعلامت>* shared_tuples_idx_map = اشاره_تهی,
			hb_alloc_pool_t *pool = اشاره_تهی)
    {
      // return true for empty glyph
      إذا (!tuple_vars)
        أرجع درست;

      // compile points set and store data in hashmap
      إذا (!compile_all_نقطه_sets ())
        أرجع خطأ;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_اندازه += 4;

      إذا (use_shared_points)
      {
        پیدا_کن_shared_points ();
        إذا (shared_points_بایتها)
          compiled_byte_اندازه += shared_points_بایتها->درازی;
      }
      hb_وکتور_t<صحيح> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      لكل (خودکار& tuple: tuple_vars)
      {
        ثابت hb_وکتور_t<منطقي>* points_مجموعه = &(tuple.indices);
        hb_وکتور_t<بیعلامت حرف> *points_داده;
        إذا (unlikely (!نقطه_داده_map.has (points_مجموعه, &points_داده)))
          أرجع خطأ;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        إذا (!points_داده->درازی)
          استمر;
        إذا (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          أرجع خطأ;

        بیعلامت points_داده_درازی = (points_داده != shared_points_بایتها) ? points_داده->درازی : 0;
        إذا (!tuple.compile_tuple_var_سرآیند (axes_شاخص_map, points_داده_درازی, axes_old_شاخص_برچسب_map,
                                             shared_tuples_idx_map,
					     pool))
          أرجع خطأ;
        compiled_byte_اندازه += tuple.compiled_tuple_سرآیند.درازی + points_داده_درازی + tuple.compiled_deltas.درازی;
      }

      إذا (is_gvar && (compiled_byte_اندازه % 2))
      {
        needs_padding = درست;
        compiled_byte_اندازه += 1;
      }

      أرجع درست;
    }

    منطقي serialize_var_سرآیندها (hb_serialize_context_t *c, بیعلامت& مجموع_سرآیند_len) ثابت
    {
      TRACE_SERIALIZE (هذا);
      لكل (ثابت خودکار& tuple: tuple_vars)
      {
        tuple.compiled_tuple_سرآیند.as_array ().copy (c);
        إذا (c->درون_error ()) return_trace (خطأ);
        مجموع_سرآیند_len += tuple.compiled_tuple_سرآیند.درازی;
      }
      return_trace (درست);
    }

    منطقي serialize_var_داده (hb_serialize_context_t *c, منطقي is_gvar) ثابت
    {
      TRACE_SERIALIZE (هذا);
      إذا (is_gvar && shared_points_بایتها)
      {
        hb_ubytes_t s (shared_points_بایتها->arrayZ, shared_points_بایتها->درازی);
        s.copy (c);
      }

      لكل (ثابت خودکار& tuple: tuple_vars)
      {
        ثابت hb_وکتور_t<منطقي>* points_مجموعه = &(tuple.indices);
        hb_وکتور_t<بیعلامت حرف> *نقطه_داده;
        إذا (!نقطه_داده_map.has (points_مجموعه, &نقطه_داده))
          return_trace (خطأ);

        إذا (!is_gvar || نقطه_داده != shared_points_بایتها)
        {
          hb_ubytes_t s (نقطه_داده->arrayZ, نقطه_داده->درازی);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        إذا (c->درون_error ()) return_trace (خطأ);
      }

      /* padding for gvar */
      إذا (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        إذا (!c->embed (pad)) return_trace (خطأ);
      }
      return_trace (درست);
    }
  };

  بنية tuple_iterator_t
  {
    بیعلامت get_axis_شمار () ثابت { أرجع axis_شمار; }

    فراغ init (hb_بایتها_t var_داده_بایتها_2, بیعلامت صحيح axis_شمار_3, ثابت فراغ *جدول_پایه)
    {
      var_داده_بایتها = var_داده_بایتها_2;
      var_داده = var_داده_بایتها_2.as<Tuple_Variation_داده> ();
      tuples_چپ = var_داده->tuple_Var_شمار.get_شمار ();
      axis_شمار = axis_شمار_3;
      axis_شمار_times_2 = axis_شمار_3 * 2;
      current_tuple = &var_داده->get_tuple_var_سرآیند ();
      داده_جابه_جایی_2 = 0;
      جدول_پایه_2 = جدول_پایه;
    }

    منطقي get_shared_indices (hb_وکتور_t<بیعلامت صحيح> &shared_indices /*بیرون*/)
    {
      إذا (var_داده->has_shared_نقطه_numbers ())
      {
        ثابت HBUINT8 *پایه = &(جدول_پایه_2+var_داده->داده);
        ثابت HBUINT8 *p = پایه;
        إذا (!decompile_points (p, shared_indices, (ثابت HBUINT8 *) (var_داده_بایتها.arrayZ + var_داده_بایتها.درازی))) أرجع خطأ;
        داده_جابه_جایی_2 = p - پایه;
      }
      أرجع درست;
    }

    منطقي is_valid ()
    {
      إذا (unlikely (tuples_چپ <= 0))
	أرجع خطأ;

      current_tuple_اندازه = Tuple_Variation_سرآیند::min_اندازه;
      إذا (unlikely (!var_داده_بایتها.check_انتها ((ثابت حرف *) current_tuple + current_tuple_اندازه)))
	أرجع خطأ;

      current_tuple_اندازه = current_tuple->get_اندازه (axis_شمار_times_2);
      إذا (unlikely (!var_داده_بایتها.check_انتها ((ثابت حرف *) current_tuple + current_tuple_اندازه)))
	أرجع خطأ;

      أرجع درست;
    }

    HB_ALWAYS_INLINE
    منطقي move_مقصد_بعدی ()
    {
      داده_جابه_جایی_2 += current_tuple->get_داده_اندازه ();
      current_tuple = &Struct_At_جابه_جایی<Tuple_Variation_سرآیند> (current_tuple, current_tuple_اندازه);
      tuples_چپ--;
      أرجع is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ثابت HBUINT8 *get_serialized_داده () ثابت
    { أرجع &(جدول_پایه_2+var_داده->داده) + داده_جابه_جایی_2; }

    خاص:
    علامتدار tuples_چپ;
    ثابت Tuple_Variation_داده *var_داده;
    بیعلامت صحيح axis_شمار;
    بیعلامت صحيح axis_شمار_times_2;
    بیعلامت صحيح داده_جابه_جایی_2;
    بیعلامت صحيح current_tuple_اندازه;
    ثابت فراغ *جدول_پایه_2;

    عام:
    hb_بایتها_t var_داده_بایتها;
    ثابت Tuple_Variation_سرآیند *current_tuple;
  };

  ساكن منطقي get_tuple_iterator (hb_بایتها_t var_داده_بایتها, بیعلامت axis_شمار,
                                  ثابت فراغ *جدول_پایه_2,
                                  hb_وکتور_t<بیعلامت صحيح> &shared_indices /*بیرون*/,
                                  tuple_iterator_t *iterator /*بیرون*/)
  {
    iterator->init (var_داده_بایتها, axis_شمار, جدول_پایه_2);
    إذا (!iterator->get_shared_indices (shared_indices))
      أرجع خطأ;
    أرجع iterator->is_valid ();
  }

  منطقي has_shared_نقطه_numbers () ثابت { أرجع tuple_Var_شمار.has_shared_نقطه_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  منطقي cull_tuple_variations (hb_بایتها_t var_داده_بایتها,
			      بیعلامت axis_شمار,
			      hb_array_t<ثابت F2DOT14> shared_tuples,
			      ثابت hb_map_t *axes_old_شاخص_برچسب_map,
			      ثابت hb_hashmap_t<hb_برچسب_t, Triple> &reachable_ranges,
			      hb_وکتور_t<حرف> &بیرون /*بیرون*/,
			      منطقي *تغییریافته /*بیرون*/) ثابت
  {
    *تغییریافته = خطأ;

    hb_وکتور_t<بیعلامت صحيح> shared_indices;
    tuple_iterator_t iterator;
    إذا (!get_tuple_iterator (var_داده_بایتها, axis_شمار, هذا,
			     shared_indices, &iterator))
      أرجع درست; /* no tuples or malformed: keep original */

    ثابت حرف *بایتها_شروع = var_داده_بایتها.arrayZ;
    ثابت حرف *بایتها_انتها = بایتها_شروع + var_داده_بایتها.درازی;
    ثابت حرف *serialized_پایه = (ثابت حرف *) &(هذا+داده);
    ثابت حرف *shared_points_انتها = (ثابت حرف *) iterator.get_serialized_داده ();
    إذا (unlikely (serialized_پایه < بایتها_شروع || serialized_پایه > بایتها_انتها ||
		  shared_points_انتها < serialized_پایه || shared_points_انتها > بایتها_انتها))
      أرجع درست;

    بنية kept_tuple_t
    {
      ثابت حرف *سرآیند;
      بیعلامت سرآیند_اندازه;
      ثابت حرف *داده;
      بیعلامت داده_اندازه;
    };
    hb_وکتور_t<kept_tuple_t> kept;
    بیعلامت مجموع = 0;
    بیعلامت kept_سرآیندها_اندازه = 0, kept_داده_اندازه = 0;
    افعل
    {
      ثابت Tuple_Variation_سرآیند *سرآیند = iterator.current_tuple;
      بیعلامت سرآیند_اندازه = سرآیند->get_اندازه (axis_شمار * 2);
      بیعلامت داده_اندازه = سرآیند->get_داده_اندازه ();
      ثابت حرف *tuple_داده = (ثابت حرف *) iterator.get_serialized_داده ();
      إذا (unlikely (tuple_داده < serialized_پایه ||
		    داده_اندازه > (بیعلامت) (بایتها_انتها - tuple_داده)))
	أرجع درست; /* malformed: keep original */

      hb_hashmap_t<hb_برچسب_t, Triple> axis_tuples;
      إذا (!سرآیند->unpack_axis_tuples (axis_شمار, shared_tuples,
				       axes_old_شاخص_برچسب_map, axis_tuples))
	أرجع درست;

      مجموع++;
      إذا (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ثابت حرف *) سرآیند, سرآیند_اندازه,
				 tuple_داده, داده_اندازه});
	kept_سرآیندها_اندازه += سرآیند_اندازه;
	kept_داده_اندازه += داده_اندازه;
      }
    } طالما (iterator.move_مقصد_بعدی ());

    إذا (unlikely (kept.درون_error ())) أرجع خطأ;
    إذا (kept.درازی == مجموع) أرجع درست; /* nothing to cull */

    *تغییریافته = درست;
    إذا (!kept.درازی) أرجع درست; /* everything culled: no variation data */

    بیعلامت shared_points_اندازه = shared_points_انتها - serialized_پایه;
    بیعلامت داده_جابه_جایی_2 = min_اندازه + kept_سرآیندها_اندازه;
    بیعلامت new_اندازه = داده_جابه_جایی_2 + shared_points_اندازه + kept_داده_اندازه;
    إذا (unlikely (!بیرون.resize (new_اندازه)))
      أرجع خطأ;

    Tuple_Variation_داده *بیرون_داده = (Tuple_Variation_داده *) بیرون.arrayZ;
    بیرون_داده->tuple_Var_شمار = (uint16_t) (kept.درازی |
					  (has_shared_نقطه_numbers () ? 0x8000u : 0u));
    بیرون_داده->داده = داده_جابه_جایی_2;

    حرف *p = بیرون.arrayZ + min_اندازه;
    لكل (ثابت خودکار &t : kept)
    {
      hb_memcpy (p, t.سرآیند, t.سرآیند_اندازه);
      p += t.سرآیند_اندازه;
    }
    hb_memcpy (p, serialized_پایه, shared_points_اندازه);
    p += shared_points_اندازه;
    لكل (ثابت خودکار &t : kept)
    {
      hb_memcpy (p, t.داده, t.داده_اندازه);
      p += t.داده_اندازه;
    }
    أرجع درست;
  }

  ساكن منطقي decompile_points (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_وکتور_t<بیعلامت صحيح> &points /*بیرون*/,
				ثابت HBUINT8 *انتها)
  {
    تعداد packed_نقطه_flag_t
    {
      POINTS_ARE_واژگان     = 0x80,
      نقطه_پیاپی_شمار_ماسک = 0x7F
    };

    إذا (unlikely (p + 1 > انتها)) أرجع خطأ;

    بیعلامت شمار = *p++;
    إذا (شمار & POINTS_ARE_واژگان)
    {
      إذا (unlikely (p + 1 > انتها)) أرجع خطأ;
      شمار = ((شمار & نقطه_پیاپی_شمار_ماسک) << 8) | *p++;
    }
    إذا (unlikely (!points.resize_dirty  (شمار))) أرجع خطأ;

    بیعلامت n = 0;
    بیعلامت i = 0;
    طالما (i < شمار)
    {
      إذا (unlikely (p + 1 > انتها)) أرجع خطأ;
      بیعلامت control = *p++;
      بیعلامت پیاپی_شمار = (control & نقطه_پیاپی_شمار_ماسک) + 1;
      بیعلامت stop = i + پیاپی_شمار;
      إذا (unlikely (stop > شمار)) أرجع خطأ;
      إذا (control & POINTS_ARE_واژگان)
      {
        إذا (unlikely (p + پیاپی_شمار * HBUINT16::static_اندازه > انتها)) أرجع خطأ;
        لكل (; i < stop; i++)
        {
          n += *(ثابت HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_اندازه;
        }
      }
      وإلا
      {
        إذا (unlikely (p + پیاپی_شمار > انتها)) أرجع خطأ;
        لكل (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    أرجع درست;
  }

  قالب <نام_نوع T>
  HB_ALWAYS_INLINE
  ساكن منطقي decompile_deltas (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_وکتور_t<T> &deltas /* IN/OUT */,
				ثابت HBUINT8 *انتها,
				منطقي consume_all = خطأ,
				بیعلامت شروع = 0)
  {
    أرجع Tuple_مقادیر::decompile (p, deltas, انتها, consume_all, شروع);
  }

  منطقي has_داده () ثابت { أرجع tuple_Var_شمار; }

  منطقي decompile_tuple_variations (بیعلامت نقطه_شمار,
                                   منطقي is_gvar,
                                   tuple_iterator_t iterator,
                                   ثابت hb_map_t *axes_old_شاخص_برچسب_map,
                                   ثابت hb_وکتور_t<بیعلامت> &shared_indices,
                                   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /*بیرون*/
				   hb_alloc_pool_t *pool = اشاره_تهی,
                                   منطقي is_مرکب_glyph = خطأ) ثابت
  {
    أرجع tuple_variations.create_from_tuple_var_داده (iterator, tuple_Var_شمار,
                                                        نقطه_شمار, is_gvar,
                                                        axes_old_شاخص_برچسب_map,
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
    إذا (!tuple_variations) return_trace (درست);

    خودکار *بیرون = c->شروع_embed (هذا);
    إذا (unlikely (!c->extend_min (بیرون))) return_trace (خطأ);

    إذا (!c->check_تخصیص_بده (بیرون->tuple_Var_شمار, tuple_variations.get_var_شمار (),
                          HB_SERIALIZE_ERROR_INT_سرریز)) return_trace (خطأ);

    بیعلامت مجموع_سرآیند_len = 0;

    إذا (!tuple_variations.serialize_var_سرآیندها (c, مجموع_سرآیند_len))
      return_trace (خطأ);

    بیعلامت داده_جابه_جایی_2 = min_اندازه + مجموع_سرآیند_len;
    إذا (!is_gvar) داده_جابه_جایی_2 += 4;
    إذا (!c->check_تخصیص_بده (بیرون->داده, داده_جابه_جایی_2, HB_SERIALIZE_ERROR_INT_سرریز)) return_trace (خطأ);

    أرجع tuple_variations.serialize_var_داده (c, is_gvar);
  }

  محمي:
  بنية Tuple_Var_شمار : HBUINT16
  {
    دوست بنية tuple_variations_t;
    منطقي has_shared_نقطه_numbers () ثابت { أرجع ((*هذا) & Shared_نقطه_Numbers); }
    بیعلامت صحيح get_شمار () ثابت { أرجع (*هذا) & شمار_ماسک; }
    Tuple_Var_شمار& عملگر = (uint16_t i) { HBUINT16::عملگر= (i); أرجع *هذا; }
    صریح عملگر منطقي () ثابت { أرجع get_شمار (); }

    محمي:
    تعداد Flags
    {
      Shared_نقطه_Numbers= 0x8000u,
      شمار_ماسک         = 0x0FFFu
    };
    عام:
    DEFINE_اندازه_STATIC (2);
  };

  Tuple_Var_شمار tuple_Var_شمار;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  جابه_جایی_مقصد<HBUINT8, OffType>
                داده;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  عام:
  DEFINE_اندازه_MIN (2 + OffType::static_اندازه);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
استفاده tuple_variations_t = Tuple_Variation_داده<HBUINT16>::tuple_variations_t;
بنية قلم_variations_t
{
  استفاده region_t = ثابت hb_hashmap_t<hb_برچسب_t, Triple>*;
  خاص:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_وکتور_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_وکتور_t<بیعلامت> var_داده_رقم_ردیفها;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_وکتور_t<hb_hashmap_t<hb_برچسب_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_وکتور_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, بیعلامت> region_map;

  /* all delta rows after instantiation */
  hb_وکتور_t<hb_وکتور_t<صحيح>> delta_ردیفها;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_وکتور_t<delta_ردیف_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  منطقي has_long = خطأ;

  عام:
  منطقي has_long_کلمه () ثابت
  { أرجع has_long; }

  ثابت hb_وکتور_t<region_t>& get_region_list () ثابت
  { أرجع region_list; }

  ثابت hb_وکتور_t<delta_ردیف_encoding_t>& get_vardata_encodings () ثابت
  { أرجع encodings; }

  ثابت hb_map_t& get_varidx_map () ثابت
  { أرجع varidx_map; }

  منطقي جمع_کن_vardata_encoding_for_testing (hb_وکتور_t<ثابت hb_وکتور_t<صحيح>*> &&ردیفها,
                                         بیعلامت رقم_تعداد_ستون)
  {
    encodings.push (delta_ردیف_encoding_t (std::move (ردیفها), رقم_تعداد_ستون));
    أرجع !encodings.درون_error ();
  }

  منطقي compile_varidx_map_for_testing (ثابت hb_hashmap_t<بیعلامت, ثابت hb_وکتور_t<صحيح>*>& front_mapping)
  { أرجع compile_varidx_map (front_mapping); }

  منطقي instantiate (ثابت قلم_Variation_Store& varStore,
                    ثابت hb_subset_plan_t *plan,
                    منطقي optimize=درست,
                    منطقي use_no_variation_idx=درست,
                    ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> (),
                    منطقي layout_conditions = خطأ)
  {
    إذا (!create_from_قلم_varstore (varStore, plan->axes_old_شاخص_برچسب_map, inner_maps))
      أرجع خطأ;
    /* avar2 partial instancing: cull unreachable regions. */
    إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      لكل (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    إذا (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      أرجع خطأ;
    إذا (layout_conditions &&
        (!instantiate_condition_مقادیر (plan, varStore) || !build_region_list ()))
      أرجع خطأ;
    أرجع as_قلم_varstore (optimize, use_no_variation_idx);
  }

  منطقي instantiate_condition_مقادیر (ثابت hb_subset_plan_t *plan,
                                      ثابت قلم_Variation_Store &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    خودکار &condition_map = plan->layout_condition_idx_مقدار_map;
    condition_map.clear ();
    إذا (!plan->layout_condition_مقادیر) أرجع درست;
    استفاده condition_مقدار_t = hb_pair_t<uintptr_t, hb_pair_t<بیعلامت, صحيح>>;
    hb_وکتور_t<condition_مقدار_t> conditions;
    لكل (خودکار condition : plan->layout_condition_مقادیر.iter ())
      conditions.push (condition);
    إذا (unlikely (conditions.درون_error ())) أرجع خطأ;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ثابت condition_مقدار_t &a, ثابت condition_مقدار_t &b) {
      إذا (a.second.اولی != b.second.اولی)
        أرجع a.second.اولی < b.second.اولی ? -1 : 1;
      أرجع a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    بیعلامت new_outer = 0;
    لكل (بیعلامت i = 0; i < plan->gdef_varstore_inner_maps.درازی; i++)
      إذا (plan->gdef_varstore_inner_maps[i].get_population ())
        إذا (!outer_map.مجموعه (i, new_outer++)) أرجع خطأ;
    لكل (خودکار condition : conditions)
    {
      بیعلامت old_idx = condition.second.اولی;
      مزدوج value = condition.second.second +
                     varStore.get_delta<مزدوج> (old_idx, plan->normalized_coords.as_array ());
      بیعلامت old_outer = old_idx >> 16;
      إذا (!outer_map.has (old_outer))
        استمر;
      بیعلامت outer = outer_map.get (old_outer);
      بیعلامت inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      بیعلامت idx = (outer << 16) | inner;
      إذا (outer >= vars.درازی || inner >= var_داده_رقم_ردیفها[outer])
        استمر;

      مزدوج بیشینه = fabs ((مزدوج) value);
      منطقي variable = خطأ;
      لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        إذا (unlikely (inner >= tuple.deltas_x.درازی)) أرجع خطأ;
        عائم delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        بیشینه = hb_max (بیشینه, fabs ((مزدوج) delta));
      }
      إذا (!variable)
      {
        إذا (!condition_map.مجموعه (condition.اولی,
                                hb_pair (بیعلامت (VarIdx::NO_VARIATION), صحيح (value > 0))))
          أرجع خطأ;
        استمر;
      }

      مزدوج scale = 1.;
      طالما (بیشینه * scale > بیشترین_صحیح)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      طالما (بیشینه * scale * 2 <= بیشترین_صحیح)
      {
        منطقي integral = (مزدوج) value * scale == round ((مزدوج) value * scale);
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (مزدوج) tuple.deltas_x[inner] * scale ==
                      round ((مزدوج) tuple.deltas_x[inner] * scale);
        إذا (integral) اكسر;
        scale *= 2;
      }
      صحيح default_مقدار = (صحيح) round ((مزدوج) value * scale);
      إذا (scale != 1. || default_مقدار < -32768 || default_مقدار > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t خصوصی_ردیف;
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          إذا (!tuple.deltas_x[inner]) استمر;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (درست);
          copy.deltas_x.push ((عائم) round ((مزدوج) tuple.deltas_x[inner] * scale));
          إذا (unlikely (copy.axis_tuples.درون_error () || copy.indices.درون_error () ||
                        copy.deltas_x.درون_error ())) أرجع خطأ;
          خصوصی_ردیف.tuple_vars.push (std::move (copy));
        }
        إذا (unlikely (vars.درازی >= 0xFFFFu)) أرجع خطأ;
        إذا (unlikely (خصوصی_ردیف.tuple_vars.درون_error ())) أرجع خطأ;
        idx = vars.درازی << 16;
        vars.push (std::move (خصوصی_ردیف));
        var_داده_رقم_ردیفها.push (1);
        إذا (unlikely (vars.درون_error () || var_داده_رقم_ردیفها.درون_error ())) أرجع خطأ;
        إذا (default_مقدار < -32768 || default_مقدار > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          صحيح bias = default_مقدار - hb_clamp (default_مقدار, -32768, 32767);
          default_مقدار -= bias;
          صحيح بالا = bias / 65536 * 65536;
          صحيح parts[] = {بالا, bias - بالا};
          لكل (صحيح part : parts)
          {
            إذا (!part) استمر;
            بیعلامت شمار = vars[idx >> 16].tuple_vars.درازی;
            جمع_کن_tuple (idx >> 16, hb_hashmap_t<hb_برچسب_t, Triple> (),
                       0, part, 1);
            ثابت خودکار &tuples = vars[idx >> 16].tuple_vars;
            إذا (unlikely (tuples.درون_error () || tuples.درازی != شمار + 1 ||
                          tuples.دم ().indices.درون_error () ||
                          tuples.دم ().deltas_x.درون_error ())) أرجع خطأ;
          }
        }
      }
      إذا (!condition_map.مجموعه (condition.اولی, hb_pair (idx, default_مقدار)))
        أرجع خطأ;
    }
    أرجع !condition_map.درون_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  منطقي create_from_قلم_varstore (ثابت قلم_Variation_Store& varStore,
                                  ثابت hb_map_t& axes_old_شاخص_برچسب_map,
                                  ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> ())
  {
    ثابت VarRegionList& regionList = varStore.get_region_list ();
    إذا (!regionList.get_var_regions (axes_old_شاخص_برچسب_map, orig_region_list))
      أرجع خطأ;

    بیعلامت رقم_var_داده = varStore.get_sub_جدول_شمار ();
    إذا (inner_maps && inner_maps.درازی != رقم_var_داده) أرجع خطأ;
    إذا (!vars.alloc (رقم_var_داده) ||
        !var_داده_رقم_ردیفها.alloc (رقم_var_داده)) أرجع خطأ;

    لكل (بیعلامت i = 0; i < رقم_var_داده; i++)
    {
      إذا (inner_maps && !inner_maps.arrayZ[i].get_population ())
          استمر;
      tuple_variations_t var_داده_tuples;
      بیعلامت قلم_شمار_2 = 0;
      إذا (!var_داده_tuples.create_from_قلم_var_داده (varStore.get_sub_جدول (i),
                                                      orig_region_list,
                                                      axes_old_شاخص_برچسب_map,
                                                      قلم_شمار_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : اشاره_تهی))
        أرجع خطأ;

      var_داده_رقم_ردیفها.push (قلم_شمار_2);
      vars.push (std::move (var_داده_tuples));
    }
    أرجع !vars.درون_error () && !var_داده_رقم_ردیفها.درون_error () && vars.درازی == var_داده_رقم_ردیفها.درازی;
  }

  منطقي instantiate_tuple_vars (ثابت hb_hashmap_t<hb_برچسب_t, Triple>& normalized_axes_location,
                               ثابت hb_hashmap_t<hb_برچسب_t, TripleDistances>& axes_triple_distances,
                               منطقي build_regions = درست)
  {
    optimize_scratch_t scratch;
    لكل (tuple_variations_t& tuple_vars : vars)
      إذا (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        أرجع خطأ;

    أرجع !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  بیعلامت جمع_کن_vardata (بیعلامت قلم_شمار_2)
  {
    vars.push (tuple_variations_t ());
    var_داده_رقم_ردیفها.push (قلم_شمار_2);
    أرجع vars.درازی - 1;
  }

  /* Get item count for a VarData subtable. */
  بیعلامت get_قلم_شمار (بیعلامت outer) ثابت
  { أرجع outer < var_داده_رقم_ردیفها.درازی ? var_داده_رقم_ردیفها[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  بیعلامت duplicate_ردیف (بیعلامت outer, بیعلامت inner)
  {
    إذا (unlikely (outer >= vars.درازی ||
                  inner >= var_داده_رقم_ردیفها[outer] ||
                  var_داده_رقم_ردیفها[outer] >= 0xFFFFu))
      أرجع (بیعلامت) -1;
    لكل (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      إذا (unlikely (inner >= tuple.deltas_x.درازی ||
                    inner >= tuple.indices.درازی))
        أرجع (بیعلامت) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      إذا (inner < tuple.deltas_y.درازی)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      إذا (unlikely (tuple.indices.درون_error () ||
                    tuple.deltas_x.درون_error () ||
                    tuple.deltas_y.درون_error ()))
        أرجع (بیعلامت) -1;
    }
    أرجع var_داده_رقم_ردیفها[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  فراغ جمع_کن_tuple (بیعلامت outer,
                  hb_hashmap_t<hb_برچسب_t, Triple>&& axis_tuples,
                  بیعلامت inner, صحيح delta, بیعلامت قلم_شمار_2)
  {
    إذا (outer >= vars.درازی) أرجع;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    إذا (!tuple.indices.resize (قلم_شمار_2) ||
        !tuple.deltas_x.resize (قلم_شمار_2)) أرجع;
    لكل (بیعلامت i = 0; i < قلم_شمار_2; i++)
    {
      tuple.indices.arrayZ[i] = درست;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (عائم) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  منطقي build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, بیعلامت> all_regions;
    hb_hashmap_t<region_t, بیعلامت> استفاده_شده_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_وکتور_t<region_t> all_unique_regions;
    لكل (ثابت tuple_variations_t& sub_جدول_2 : vars)
    {
      لكل (ثابت tuple_delta_t& tuple : sub_جدول_2.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        إذا (!استفاده_شده_regions.has (r))
        {
          منطقي all_zeros = درست;
          لكل (عائم d : tuple.deltas_x)
          {
            صحيح delta = hb_clamp_مقصد<صحيح> (roundf ((مزدوج) d));
            إذا (delta != 0)
            {
              all_zeros = خطأ;
              اكسر;
            }
          }
          إذا (!all_zeros)
          {
            إذا (!استفاده_شده_regions.مجموعه (r, 1))
              أرجع خطأ;
          }
        }
        إذا (all_regions.has (r))
          استمر;
        إذا (!all_regions.مجموعه (r, 1))
          أرجع خطأ;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    إذا (!all_regions || !all_unique_regions) أرجع درست;

    إذا (!region_list.alloc (all_regions.get_population ()))
      أرجع خطأ;

    بیعلامت idx = 0;
    /* append the original regions that pre-existed */
    لكل (ثابت خودکار& r : orig_region_list)
    {
      إذا (!all_regions.has (&r) || !استفاده_شده_regions.has (&r))
        استمر;

      region_list.push (&r);
      إذا (!region_map.مجموعه (&r, idx))
        أرجع خطأ;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    لكل (ثابت خودکار& r: all_unique_regions)
    {
      إذا (!all_regions.has (r) || !استفاده_شده_regions.has (r))
        استمر;
      region_list.push (r);
      إذا (!region_map.مجموعه (r, idx))
        أرجع خطأ;
      all_regions.del (r);
      idx++;
    }
    أرجع (!region_list.درون_error ()) && (!region_map.درون_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  بنية combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = افتراضي;
    combined_gain_idx_tuple_t (بیعلامت gain, بیعلامت i, بیعلامت j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      أكد (gain < 0xFFFFFF);
      أكد (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    منطقي عملگر < (ثابت combined_gain_idx_tuple_t& o)
    {
      أرجع encoded < o.encoded;
    }

    منطقي عملگر <= (ثابت combined_gain_idx_tuple_t& o)
    {
      أرجع encoded <= o.encoded;
    }

    بیعلامت idx_1 () ثابت { أرجع (encoded >> 20) & 0xFFFFF; };
    بیعلامت idx_2 () ثابت { أرجع encoded & 0xFFFFF; };
  };

  منطقي as_قلم_varstore (منطقي optimize=درست, منطقي use_no_variation_idx=درست)
  {
    /* return true if no variation data */
    إذا (!region_list) أرجع درست;
    بیعلامت رقم_تعداد_ستون = region_list.درازی;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    بیعلامت مجموع_ردیفها = 0;
    لكل (بیعلامت major = 0; major < var_داده_رقم_ردیفها.درازی; major++)
      مجموع_ردیفها += var_داده_رقم_ردیفها[major];

    إذا (!delta_ردیفها.resize (مجموع_ردیفها)) أرجع خطأ;
    /* init all rows to [0]*num_cols */
    لكل (بیعلامت i = 0; i < مجموع_ردیفها; i++)
      إذا (!(delta_ردیفها[i].resize (رقم_تعداد_ستون))) أرجع خطأ;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<بیعلامت, ثابت hb_وکتور_t<صحيح>*> front_mapping;
    بیعلامت شروع_ردیف = 0;
    hb_وکتور_t<delta_ردیف_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_وکتور_t<ثابت hb_وکتور_t<صحيح> *> major_ردیفها;
    hb_hashmap_t<ثابت hb_وکتور_t<صحيح>*, بیعلامت> delta_ردیفها_map;
    لكل (بیعلامت major = 0; major < vars.درازی; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ثابت tuple_variations_t& tuples = vars[major];
      بیعلامت رقم_ردیفها = var_داده_رقم_ردیفها[major];

      إذا (!رقم_ردیفها) استمر;

      لكل (ثابت tuple_delta_t& tuple: tuples.tuple_vars)
      {
        إذا (tuple.deltas_x.درازی != رقم_ردیفها)
          أرجع خطأ;

        /* skip unused regions */
        بیعلامت *ستون_idx;
        إذا (!region_map.has (&(tuple.axis_tuples), &ستون_idx))
          استمر;

        لكل (بیعلامت i = 0; i < رقم_ردیفها; i++)
        {
          صحيح rounded_delta = hb_clamp_مقصد<صحيح> (roundf ((مزدوج) tuple.deltas_x[i]));
          صحيح &delta = delta_ردیفها[شروع_ردیف + i][*ستون_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          إذا (unlikely (sum < INT_MIN || sum > بیشترین_صحیح)) أرجع خطأ;
          delta = (صحيح) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_ردیفها.reset ();
      لكل (بیعلامت minor = 0; minor < رقم_ردیفها; minor++)
      {
	ثابت hb_وکتور_t<صحيح>& ردیف = delta_ردیفها[شروع_ردیف + minor];
	إذا (use_no_variation_idx)
	{
	  منطقي all_zeros = درست;
	  لكل (صحيح delta : ردیف)
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

	إذا (!front_mapping.مجموعه ((major<<16) + minor, &ردیف))
	  أرجع خطأ;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	إذا (optimize)
	{
	  إذا (delta_ردیفها_map.has (&ردیف))
	    استمر;

	  delta_ردیفها_map.مجموعه (&ردیف, 1);
	}

	major_ردیفها.push (&ردیف);
      }

      إذا (unlikely (major_ردیفها.درون_error () || delta_ردیفها_map.درون_error ()))
        أرجع خطأ;
      إذا (major_ردیفها)
      {
	خودکار *encoding = encoding_objs.push (std::move (major_ردیفها), رقم_تعداد_ستون);
	إذا (unlikely (encoding_objs.درون_error () || encoding->درون_error ()))
	  أرجع خطأ;
      }

      شروع_ردیف += رقم_ردیفها;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    إذا (!optimize)
    {
      encodings = std::move (encoding_objs);
      أرجع !encodings.درون_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.qsort ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    استفاده قلم_t = hb_اولویت_صف_t<combined_gain_idx_tuple_t>::قلم_t;
    hb_وکتور_t<قلم_t> صف_اقلام;
    بیعلامت رقم_todos = encoding_objs.درازی;
    لكل (بیعلامت i = 0; i < رقم_todos; i++)
    {
      لكل (بیعلامت j = i + 1; j < رقم_todos; j++)
      {
        صحيح combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        إذا (combining_gain > 0)
          صف_اقلام.push (قلم_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_اولویت_صف_t<combined_gain_idx_tuple_t> صف (std::move (صف_اقلام));

    hb_بیت_مجموعه_t removed_todo_idxes;
    طالما (صف)
    {
      خودکار t = صف.برداشت_minimum ().اولی;
      بیعلامت i = t.idx_1 ();
      بیعلامت j = t.idx_2 ();

      إذا (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        استمر;

      delta_ردیف_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_ردیف_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.جمع_کن (i);
      removed_todo_idxes.جمع_کن (j);
      إذا (unlikely (removed_todo_idxes.درون_error ())) أرجع خطأ;

      encoding.ادغام (other_encoding);
      إذا (unlikely (encoding.درون_error ())) أرجع خطأ;

      لكل (بیعلامت idx = 0; idx < encoding_objs.درازی; idx++)
      {
        إذا (removed_todo_idxes.has (idx)) استمر;

        ثابت delta_ردیف_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        إذا (obj.عرض == encoding.عرض && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          لكل (ثابت خودکار& ردیف : obj.اقلام)
            encoding.جمع_کن_ردیف (ردیف);
          إذا (unlikely (encoding.درون_error ())) أرجع خطأ;

          removed_todo_idxes.جمع_کن (idx);
          إذا (unlikely (removed_todo_idxes.درون_error ())) أرجع خطأ;
          استمر;
        }

        صحيح combined_gain = encoding.gain_from_merging (obj);
        إذا (combined_gain > 0)
          صف.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.درازی), 0);
      }

      خودکار moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      إذا (unlikely (encoding_objs.درون_error ())) أرجع خطأ;
    }

    صحيح رقم_final_encodings = (صحيح) encoding_objs.درازی - (صحيح) removed_todo_idxes.get_population ();
    إذا (رقم_final_encodings <= 0) أرجع خطأ;

    إذا (!encodings.alloc (رقم_final_encodings)) أرجع خطأ;
    لكل (بیعلامت i = 0; i < encoding_objs.درازی; i++)
    {
      إذا (removed_todo_idxes.has (i)) استمر;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    أرجع compile_varidx_map (front_mapping);
  }

  خاص:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  منطقي compile_varidx_map (ثابت hb_hashmap_t<بیعلامت, ثابت hb_وکتور_t<صحيح>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ثابت hb_وکتور_t<صحيح>*, بیعلامت> پشت_mapping;
    hb_وکتور_t<delta_ردیف_encoding_t> تقسیم_encodings;

    لكل (بیعلامت i = 0; i < encodings.درازی; i++)
    {
      delta_ردیف_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      إذا (encoding.is_empty () || encoding.درون_error ())
        أرجع خطأ;

      بیعلامت رقم_ردیفها = encoding.اقلام.درازی;
      بیعلامت رقم_تعداد_ستون = encoding.chars.درازی;

      /* sort rows, make result deterministic */
      encoding.اقلام.qsort (cmp_ردیف);

      لكل (بیعلامت شروع = 0; شروع < رقم_ردیفها; شروع += 0xFFFFu)
      {
        بیعلامت chunk_len = hb_min (رقم_ردیفها - شروع, 0xFFFFu);
        hb_وکتور_t<ثابت hb_وکتور_t<صحيح>*> ردیفها;

        إذا (!ردیفها.alloc (chunk_len))
          أرجع خطأ;

        بیعلامت major = تقسیم_encodings.درازی;
        لكل (بیعلامت minor = 0; minor < chunk_len; minor++)
        {
          ثابت hb_وکتور_t<صحيح> *ردیف = encoding.اقلام.arrayZ[شروع + minor];
          ردیفها.push (ردیف);
          إذا (!پشت_mapping.مجموعه (ردیف, (major << 16) + minor))
            أرجع خطأ;
        }

        خودکار *تقسیم = تقسیم_encodings.push (std::move (ردیفها), رقم_تعداد_ستون);
        إذا (unlikely (تقسیم_encodings.درون_error () || تقسیم->درون_error ()))
          أرجع خطأ;
      }
    }

    encodings = std::move (تقسیم_encodings);
    إذا (encodings.درون_error () || پشت_mapping.درون_error ())
      أرجع خطأ;

    لكل (خودکار _ : front_mapping.iter ())
    {
      بیعلامت old_varidx = _.اولی;
      بیعلامت *new_varidx;
      إذا (پشت_mapping.has (_.second, &new_varidx))
        varidx_map.مجموعه (old_varidx, *new_varidx);
      وإلا
        varidx_map.مجموعه (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_شاخص);
    }
    أرجع !varidx_map.درون_error ();
  }

  ساكن صحيح cmp_ردیف (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ثابت hb_وکتور_t<صحيح>** a = (ثابت hb_وکتور_t<صحيح>**) pa;
    ثابت hb_وکتور_t<صحيح>** b = (ثابت hb_وکتور_t<صحيح>**) pb;

    لكل (بیعلامت i = 0; i < (*b)->درازی; i++)
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


#پایان_اگر /* HB_OT_VAR_COMMON_HH */

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

#اگر_غیر_تعریف_شدہ HB_OT_VAR_COMMON_HH
#تعریف_کرو HB_OT_VAR_COMMON_HH

#شامل "hb-ot-layout-common.hh"
#شامل "hb-alloc-pool.hh"
#شامل "hb-priority-queue.hh"
#شامل "hb-subset-instancer-iup.hh"


نامخلا OT {

استعمال rebase_tent_نتیجہ_scratch_t = hb_pair_t<rebase_tent_نتیجہ_t, rebase_tent_نتیجہ_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
ساخت Tuple_Variation_سرخی
{
  دوست ساخت tuple_delta_t;
  size_t get_سائز (غیرعلامتی axis_گنتی_times_2) ثابت
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    غیرعلامتی ti = tuple_اشاریہ;
    اگر (unlikely ((ti & (Tuple_اشاریہ::EmbeddedPeakTuple | Tuple_اشاریہ::IntermediateRegion))))
    {
      غیرعلامتی گنتی = ((ti & Tuple_اشاریہ::EmbeddedPeakTuple) != 0) + ((ti & Tuple_اشاریہ::IntermediateRegion) != 0) * 2;
      واپس hb_unsigned_mul_جوڑو_saturate (گنتی, axis_گنتی_times_2, min_سائز);
    }
    واپس min_سائز;
  }

  غیرعلامتی get_ڈیٹا_سائز () ثابت { واپس var_ڈیٹا_سائز; }

  ثابت Tuple_Variation_سرخی &get_اگلا (غیرعلامتی axis_گنتی_times_2) ثابت
  { واپس Struct_At_انحراف<Tuple_Variation_سرخی> (یہ, get_سائز (axis_گنتی_times_2)); }

  منطقی unpack_axis_tuples (غیرعلامتی axis_گنتی,
                           ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                           ثابت hb_map_t *axes_old_اشاریہ_لیبل_map,
                           hb_hashmap_t<hb_لیبل_t, Triple>& axis_tuples /*برآمد*/) ثابت
  {
    ثابت F2DOT14 *peak_tuple = خالی_اشاریہ;
    اگر (has_peak ())
      peak_tuple = get_peak_tuple (axis_گنتی);
    ورنہ
    {
      غیرعلامتی صحیح اشاریہ_2 = get_اشاریہ ();
      اگر (unlikely ((اشاریہ_2 + 1) * axis_گنتی > shared_tuples.لمبائی))
        واپس جھوٹ;
      peak_tuple = shared_tuples.sub_array (axis_گنتی * اشاریہ_2, axis_گنتی).arrayZ;
    }

    ثابت F2DOT14 *آغاز_tuple = خالی_اشاریہ;
    ثابت F2DOT14 *انتہا_tuple = خالی_اشاریہ;
    منطقی has_interm = has_intermediate ();

    اگر (has_interm)
    {
      آغاز_tuple = get_آغاز_tuple (axis_گنتی);
      انتہا_tuple = get_انتہا_tuple (axis_گنتی);
    }

    برائے (غیرعلامتی i = 0; i < axis_گنتی; i++)
    {
      اعشاری peak = peak_tuple[i].مقصد_float ();
      اگر (peak == 0.f) جاری;

      hb_لیبل_t *axis_لیبل_2;
      اگر (!axes_old_اشاریہ_لیبل_map->has (i, &axis_لیبل_2))
        واپس جھوٹ;

      اعشاری آغاز, انتہا;
      اگر (has_interm)
      {
        آغاز = آغاز_tuple[i].مقصد_float ();
        انتہا = انتہا_tuple[i].مقصد_float ();
      }
      ورنہ
      {
        آغاز = hb_min (peak, 0.f);
        انتہا = hb_max (peak, 0.f);
      }
      axis_tuples.سیٹ (*axis_لیبل_2, Triple (آغاز, peak, انتہا));
    }

    واپس سچ;
  }

  HB_ALWAYS_INLINE
  اعشاری calculate_scalar (hb_array_t<ثابت صحیح> coords, غیرعلامتی صحیح coord_گنتی,
			   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ثابت
  {
    غیرعلامتی tuple_اشاریہ_2 = tuple_اشاریہ;
    غیرعلامتی صحیح اشاریہ_2 = tuple_اشاریہ_2 & Tuple_اشاریہ::Tuple_اشاریہ_نقاب;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    منطقی plain_shared_tuple = (tuple_اشاریہ_2 & ~Tuple_اشاریہ::نجی_نقطہ_Numbers) < shared_tuple_scalar_cache->لمبائی;
    اگر (likely (plain_shared_tuple))
    {
      اعشاری scalar;
      اگر (likely (shared_tuple_scalar_cache->get (اشاریہ_2, &scalar)))
	واپس scalar;
    }

    ثابت F2DOT14 *peak_tuple;

    منطقی has_interm = tuple_اشاریہ_2 & Tuple_اشاریہ::IntermediateRegion; // Inlined for performance

    اگر (unlikely (tuple_اشاریہ_2 & Tuple_اشاریہ::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_گنتی);
      shared_tuple_scalar_cache = خالی_اشاریہ;
    }
    ورنہ
    {
      اعشاری scalar;
      اگر (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->لمبائی &&
	  shared_tuple_scalar_cache->get (اشاریہ_2, &scalar))
      {
        اگر (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = خالی_اشاریہ;
	ورنہ
	  واپس scalar;
      }

      اگر (unlikely ((اشاریہ_2 + 1) * coord_گنتی > shared_tuples.لمبائی))
        واپس 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_گنتی * اشاریہ_2);

    }

    ثابت F2DOT14 *آغاز_tuple = خالی_اشاریہ;
    ثابت F2DOT14 *انتہا_tuple = خالی_اشاریہ;

    اگر (has_interm)
    {
      آغاز_tuple = get_آغاز_tuple (coord_گنتی);
      انتہا_tuple = get_انتہا_tuple (coord_گنتی);
    }

    اعشاری scalar = 1.f;
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
#اگر HB_FAST_عدد_ACCESS
    منطقی skip = coord_گنتی >= 16;
#اگر_ختم
#اگر_ختم
    برائے (غیرعلامتی صحیح i = 0; i < coord_گنتی; i++)
    {
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
#اگر HB_FAST_عدد_ACCESS
      اگر (skip)
      {
	جب (i + 4 <= coord_گنتی && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	جب (i < coord_گنتی && peak_tuple[i].مقصد_int () == 0)
	  i += 1;
	اگر (i >= coord_گنتی)
	  توڑو;
      }
#اگر_ختم
#اگر_ختم

      صحیح peak = peak_tuple[i].مقصد_int ();
      اگر (!peak) جاری;

      صحیح v = coords[i];
      اگر (!v) { scalar = 0.f; توڑو; }
      اگر (v == peak) جاری;

      اگر (has_interm)
      {
	shared_tuple_scalar_cache = خالی_اشاریہ;
        صحیح آغاز = آغاز_tuple[i].مقصد_int ();
        صحیح انتہا = انتہا_tuple[i].مقصد_int ();
        اگر (unlikely (آغاز > peak || peak > انتہا ||
                      (آغاز < 0 && انتہا > 0 && peak))) جاری;
        اگر (v < آغاز || v > انتہا) { scalar = 0.f; توڑو; }
        اگر (v < peak)
        { اگر (peak != آغاز) scalar *= (اعشاری) (v - آغاز) / (peak - آغاز); }
        ورنہ
        { اگر (peak != انتہا) scalar *= (اعشاری) (انتہا - v) / (انتہا - peak); }
      }
      ورنہ اگر (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; توڑو; }
      ورنہ
        scalar *= (اعشاری) v / peak;
    }
    اگر (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->سیٹ (اشاریہ_2, scalar);
    واپس scalar;
  }

  منطقی           has_peak () ثابت { واپس tuple_اشاریہ & Tuple_اشاریہ::EmbeddedPeakTuple; }
  منطقی   has_intermediate () ثابت { واپس tuple_اشاریہ & Tuple_اشاریہ::IntermediateRegion; }
  منطقی has_نجی_points () ثابت { واپس tuple_اشاریہ & Tuple_اشاریہ::نجی_نقطہ_Numbers; }
  غیرعلامتی      get_اشاریہ () ثابت { واپس tuple_اشاریہ & Tuple_اشاریہ::Tuple_اشاریہ_نقاب; }
  ساکن constexpr غیرعلامتی max_shared_tuple_گنتی = 0x1000u;

  محفوظ:
  ساخت Tuple_اشاریہ : HBUINT16
  {
    شمار Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      نجی_نقطہ_Numbers = 0x2000u,
      Tuple_اشاریہ_نقاب      = 0x0FFFu
    };

    Tuple_اشاریہ& عامل = (uint16_t i) { HBUINT16::عامل= (i); واپس *یہ; }
    DEFINE_سائز_STATIC (2);
  };

  hb_array_t<ثابت F2DOT14> get_all_tuples (غیرعلامتی axis_گنتی) ثابت
  { واپس StructAfter<Unsized_Array_کا<F2DOT14>> (tuple_اشاریہ).as_array ((has_peak () + has_intermediate () * 2) * axis_گنتی); }
  ثابت F2DOT14* get_all_tuples_بنیاد (غیرعلامتی axis_گنتی) ثابت
  { واپس StructAfter<Unsized_Array_کا<F2DOT14>> (tuple_اشاریہ).arrayZ; }
  ثابت F2DOT14* get_peak_tuple (غیرعلامتی axis_گنتی) ثابت
  { واپس get_all_tuples_بنیاد (axis_گنتی); }
  ثابت F2DOT14* get_آغاز_tuple (غیرعلامتی axis_گنتی) ثابت
  { واپس get_all_tuples_بنیاد (axis_گنتی) + has_peak () * axis_گنتی; }
  ثابت F2DOT14* get_انتہا_tuple (غیرعلامتی axis_گنتی) ثابت
  { واپس get_all_tuples_بنیاد (axis_گنتی) + has_peak () * axis_گنتی + axis_گنتی; }

  HBUINT16      var_ڈیٹا_سائز;    /* The size in bytes of the serialized
                                 * data for this tuple variation table. */
  Tuple_اشاریہ    tuple_اشاریہ;     /* A packed field. The high 4 bits are flags (see below).
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
  عوامی:
  DEFINE_سائز_MIN (4);
};

ساخت optimize_scratch_t
{
  iup_scratch_t iup;
  hb_سمتیہ_t<منطقی> opt_indices;
  hb_سمتیہ_t<صحیح> rounded_x_deltas;
  hb_سمتیہ_t<صحیح> rounded_y_deltas;
  hb_سمتیہ_t<اعشاری> opt_deltas_x;
  hb_سمتیہ_t<اعشاری> opt_deltas_y;
  hb_سمتیہ_t<غیرعلامتی حرف> opt_نقطہ_ڈیٹا;
  hb_سمتیہ_t<غیرعلامتی حرف> opt_deltas_ڈیٹا;
  hb_سمتیہ_t<غیرعلامتی حرف> نقطہ_ڈیٹا;
  hb_سمتیہ_t<غیرعلامتی حرف> deltas_ڈیٹا;
  hb_سمتیہ_t<صحیح> rounded_deltas;
};

ساخت tuple_delta_t
{
  ساکن constexpr منطقی realloc_move = سچ;  // Watch out when adding new members!

  عوامی:
  hb_hashmap_t<hb_لیبل_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_سمتیہ_t<منطقی> indices;

  hb_سمتیہ_t<اعشاری> deltas_x;
  /* empty for cvar tuples */
  hb_سمتیہ_t<اعشاری> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_سمتیہ_t<غیرعلامتی حرف> compiled_tuple_سرخی;
  hb_سمتیہ_t<غیرعلامتی حرف> compiled_deltas;

  hb_سمتیہ_t<F2DOT14> compiled_peak_coords;
  hb_سمتیہ_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = خالی_اشاریہ) {}
  tuple_delta_t (ثابت tuple_delta_t& o) = طےشدہ;
  tuple_delta_t& عامل = (ثابت tuple_delta_t& o) = طےشدہ;

  دوست خالی swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
  {
    hb_تبادلہ (a.axis_tuples, b.axis_tuples);
    hb_تبادلہ (a.indices, b.indices);
    hb_تبادلہ (a.deltas_x, b.deltas_x);
    hb_تبادلہ (a.deltas_y, b.deltas_y);
    hb_تبادلہ (a.compiled_tuple_سرخی, b.compiled_tuple_سرخی);
    hb_تبادلہ (a.compiled_deltas, b.compiled_deltas);
    hb_تبادلہ (a.compiled_peak_coords, b.compiled_peak_coords);
  }

  tuple_delta_t (tuple_delta_t&& o)  noexcept : tuple_delta_t ()
  { hb_تبادلہ (*یہ, o); }

  tuple_delta_t& عامل = (tuple_delta_t&& o) noexcept
  {
    hb_تبادلہ (*یہ, o);
    واپس *یہ;
  }

  خالی copy_from (ثابت tuple_delta_t& o, hb_alloc_pool_t *pool = خالی_اشاریہ)
  {
    axis_tuples = o.axis_tuples;
    indices.duplicate_سمتیہ_from_pool (pool, o.indices);
    deltas_x.duplicate_سمتیہ_from_pool (pool, o.deltas_x);
    deltas_y.duplicate_سمتیہ_from_pool (pool, o.deltas_y);
    compiled_tuple_سرخی.duplicate_سمتیہ_from_pool (pool, o.compiled_tuple_سرخی);
    compiled_deltas.duplicate_سمتیہ_from_pool (pool, o.compiled_deltas);
    compiled_peak_coords.duplicate_سمتیہ_from_pool (pool, o.compiled_peak_coords);
    compiled_interm_coords.duplicate_سمتیہ_from_pool (pool, o.compiled_interm_coords);
  }

  خالی remove_axis (hb_لیبل_t axis_لیبل_2)
  { axis_tuples.del (axis_لیبل_2); }

  منطقی سیٹ_tent (hb_لیبل_t axis_لیبل_2, Triple tent)
  { واپس axis_tuples.سیٹ (axis_لیبل_2, tent); }

  tuple_delta_t& عامل += (ثابت tuple_delta_t& o)
  {
    غیرعلامتی عدد = indices.لمبائی;
    برائے (غیرعلامتی i = 0; i < عدد; i++)
    {
      اگر (indices.arrayZ[i])
      {
        اگر (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          اگر (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      ورنہ
      {
        اگر (!o.indices.arrayZ[i]) جاری;
        indices.arrayZ[i] = سچ;
        deltas_x[i] = o.deltas_x[i];
        اگر (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    واپس *یہ;
  }

  tuple_delta_t& عامل *= (اعشاری scalar)
  {
    اگر (scalar == 1.0f)
      واپس *یہ;

    غیرعلامتی عدد = indices.لمبائی;
    اگر (deltas_y)
      برائے (غیرعلامتی i = 0; i < عدد; i++)
      {
	اگر (!indices.arrayZ[i]) جاری;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    ورنہ
      برائے (غیرعلامتی i = 0; i < عدد; i++)
      {
	اگر (!indices.arrayZ[i]) جاری;
	deltas_x[i] *= scalar;
      }
    واپس *یہ;
  }

  خالی change_tuple_var_axis_حد_بندی (hb_لیبل_t axis_لیبل_2, Triple axis_حد_بندی,
				    TripleDistances axis_triple_distances,
				    hb_سمتیہ_t<tuple_delta_t>& برآمد,
				    rebase_tent_نتیجہ_scratch_t &scratch,
				    hb_alloc_pool_t *pool = خالی_اشاریہ)
  {
    // May move *this out.

    برآمد.reset ();
    Triple *tent;
    اگر (!axis_tuples.has (axis_لیبل_2, &tent))
    {
      برآمد.push (std::move (*یہ));
      واپس;
    }

    اگر ((tent->minimum < 0.f && tent->زیادہ_سے_زیادہ > 0.f) ||
        !(tent->minimum <= tent->درمیان && tent->درمیان <= tent->زیادہ_سے_زیادہ))
      واپس;

    اگر (tent->درمیان == 0.f)
    {
      برآمد.push (std::move (*یہ));
      واپس;
    }

    اگر (!axis_حد_بندی.is_نقطہ () &&
        !(-1.f <= axis_حد_بندی.minimum &&
          axis_حد_بندی.minimum <= axis_حد_بندی.درمیان &&
          axis_حد_بندی.درمیان <= axis_حد_بندی.زیادہ_سے_زیادہ &&
          axis_حد_بندی.زیادہ_سے_زیادہ <= +1.f))
      واپس;

    rebase_tent_نتیجہ_t &حل = scratch.پہلا;
    rebase_tent (*tent, axis_حد_بندی, axis_triple_distances, حل, scratch.second);
    برائے (غیرعلامتی i = 0; i < حل.لمبائی; i++)
    {
      خودکار &t = حل.arrayZ[i];

      tuple_delta_t new_var;
      اگر (i < حل.لمبائی - 1)
	new_var.copy_from (*یہ, pool);
      ورنہ
	new_var = std::move (*یہ);

      اگر (t.second == Triple ())
        new_var.remove_axis (axis_لیبل_2);
      ورنہ
        new_var.سیٹ_tent (axis_لیبل_2, t.second);

      new_var *= t.پہلا;
      برآمد.push (std::move (new_var));
    }
  }

  منطقی compile_coords (ثابت hb_map_t& axes_اشاریہ_map,
		       ثابت hb_map_t& axes_old_اشاریہ_لیبل_map,
		       hb_alloc_pool_t *pool= خالی_اشاریہ)
  {
    غیرعلامتی cur_axis_گنتی = axes_اشاریہ_map.get_population ();
    اگر (pool)
    {
      اگر (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_گنتی)))
	واپس جھوٹ;
    }
    ورنہ اگر (unlikely (!compiled_peak_coords.resize (cur_axis_گنتی)))
      واپس جھوٹ;

    hb_array_t<F2DOT14> آغاز_coords, انتہا_coords;

    غیرعلامتی orig_axis_گنتی = axes_old_اشاریہ_لیبل_map.get_population ();
    غیرعلامتی j = 0;
    برائے (غیرعلامتی i = 0; i < orig_axis_گنتی; i++)
    {
      اگر (!axes_اشاریہ_map.has (i))
        جاری;

      hb_لیبل_t axis_لیبل_2 = axes_old_اشاریہ_لیبل_map.get (i);
      Triple *coords = خالی_اشاریہ;
      اگر (axis_tuples.has (axis_لیبل_2, &coords))
      {
	اعشاری min_val = coords->minimum;
	اعشاری val = coords->درمیان;
	اعشاری max_val = coords->زیادہ_سے_زیادہ;

	compiled_peak_coords.arrayZ[j].سیٹ_float (val);

	اگر (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  اگر (!compiled_interm_coords)
	  {
	    اگر (pool)
	    {
	      اگر (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_گنتی)))
		واپس جھوٹ;
	    }
	    ورنہ اگر (unlikely (!compiled_interm_coords.resize (2 * cur_axis_گنتی)))
	      واپس جھوٹ;
	    آغاز_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_گنتی);
	    انتہا_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_گنتی);

	    برائے (غیرعلامتی k = 0; k < j; k++)
	    {
	      علامتی peak = compiled_peak_coords.arrayZ[k].مقصد_int ();
	      اگر (!peak) جاری;
	      آغاز_coords.arrayZ[k].سیٹ_int (hb_min (peak, 0));
	      انتہا_coords.arrayZ[k].سیٹ_int (hb_max (peak, 0));
	    }
	  }

	}

	اگر (compiled_interm_coords)
	{
	  آغاز_coords.arrayZ[j].سیٹ_float (min_val);
	  انتہا_coords.arrayZ[j].سیٹ_float (max_val);
	}
      }

      j++;
    }

    واپس !compiled_peak_coords.اندر_error () && !compiled_interm_coords.اندر_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  منطقی compile_tuple_var_سرخی (ثابت hb_map_t& axes_اشاریہ_map,
                                 غیرعلامتی points_ڈیٹا_لمبائی,
                                 ثابت hb_map_t& axes_old_اشاریہ_لیبل_map,
                                 ثابت hb_hashmap_t<ثابت hb_سمتیہ_t<F2DOT14>*, غیرعلامتی>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = خالی_اشاریہ)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    اگر (!compiled_deltas) واپس سچ;

    غیرعلامتی cur_axis_گنتی = axes_اشاریہ_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    غیرعلامتی alloc_len = 3 * cur_axis_گنتی * (F2DOT14::static_سائز) + 4;
    اگر (unlikely (!compiled_tuple_سرخی.allocate_from_pool (pool, alloc_len, جھوٹ))) واپس جھوٹ;

    غیرعلامتی flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرخی.begin () + 4);
    F2DOT14* انتہا = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرخی.انتہا ());
    hb_array_t<F2DOT14> coords (p, انتہا - p);

    اگر (!shared_tuples_idx_map)
      compile_coords (axes_اشاریہ_map, axes_old_اشاریہ_لیبل_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    غیرعلامتی peak_گنتی = 0;
    غیرعلامتی *shared_tuple_idx;
    اگر (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    ورنہ
    {
      peak_گنتی = encode_peak_coords(coords, flag);
      اگر (!peak_گنتی) واپس جھوٹ;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    غیرعلامتی interim_گنتی = encode_interm_coords (coords.sub_array (peak_گنتی), flag);

    /* pointdata length = 0 implies "use shared points" */
    اگر (points_ڈیٹا_لمبائی)
      flag |= Tuple_Variation_سرخی::Tuple_اشاریہ::نجی_نقطہ_Numbers;

    غیرعلامتی serialized_ڈیٹا_سائز = points_ڈیٹا_لمبائی + compiled_deltas.لمبائی;
    Tuple_Variation_سرخی *o = reinterpret_cast<Tuple_Variation_سرخی *> (compiled_tuple_سرخی.begin ());
    o->var_ڈیٹا_سائز = serialized_ڈیٹا_سائز;
    o->tuple_اشاریہ = flag;

    غیرعلامتی کل_سرخی_len = 4 + (peak_گنتی + interim_گنتی) * (F2DOT14::static_سائز);
    compiled_tuple_سرخی.shrink_عقب_مقصد_pool (pool, کل_سرخی_len);
    واپس سچ;
  }

  غیرعلامتی encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               غیرعلامتی& flag) ثابت
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.لمبائی * حجم (compiled_peak_coords[0]));
    flag |= Tuple_Variation_سرخی::Tuple_اشاریہ::EmbeddedPeakTuple;
    واپس compiled_peak_coords.لمبائی;
  }

  /* if no need to encode intermediate coords, then just return p */
  غیرعلامتی encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 غیرعلامتی& flag) ثابت
  {
    اگر (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.لمبائی * حجم (compiled_interm_coords[0]));
      flag |= Tuple_Variation_سرخی::Tuple_اشاریہ::IntermediateRegion;
    }
    واپس compiled_interm_coords.لمبائی;
  }

  منطقی compile_deltas (hb_سمتیہ_t<صحیح> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = خالی_اشاریہ)
  { واپس compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ساکن منطقی compile_deltas (hb_array_t<ثابت منطقی> نقطہ_indices,
			      hb_array_t<ثابت اعشاری> x_deltas,
			      hb_array_t<ثابت اعشاری> y_deltas,
			      hb_سمتیہ_t<غیرعلامتی حرف> &compiled_deltas, /*برآمد*/
			      hb_سمتیہ_t<صحیح> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = خالی_اشاریہ)
  {
    اگر (unlikely (!rounded_deltas.resize_dirty  (نقطہ_indices.لمبائی)))
      واپس جھوٹ;

    غیرعلامتی j = 0;
    برائے (غیرعلامتی i = 0; i < نقطہ_indices.لمبائی; i++)
    {
      اگر (!نقطہ_indices[i]) جاری;
      rounded_deltas.arrayZ[j++] = hb_clamp_مقصد<صحیح> (roundf ((دوہرا) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    اگر (!rounded_deltas) واپس سچ;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    غیرعلامتی alloc_len = 2 + 4 * rounded_deltas.لمبائی + (rounded_deltas.لمبائی + 63) / 64;
    اگر (y_deltas)
      alloc_len *= 2;

    اگر (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, جھوٹ))) واپس جھوٹ;

    غیرعلامتی encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    اگر (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      غیرعلامتی j = 0;
      برائے (غیرعلامتی idx = 0; idx < نقطہ_indices.لمبائی; idx++)
      {
        اگر (!نقطہ_indices[idx]) جاری;
        صحیح rounded_delta = hb_clamp_مقصد<صحیح> (roundf ((دوہرا) y_deltas.arrayZ[idx]));

        اگر (j >= rounded_deltas.لمبائی) واپس جھوٹ;

        rounded_deltas[j++] = rounded_delta;
      }

      اگر (j != rounded_deltas.لمبائی) واپس جھوٹ;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_عقب_مقصد_pool (pool, encoded_len);
    واپس سچ;
  }

  ساکن غیرعلامتی compile_deltas (hb_array_t<غیرعلامتی حرف> encoded_بائٹیں,
				  hb_array_t<ثابت صحیح> deltas)
  {
    واپس Tuple_اقدار::compile_unsafe (deltas, encoded_بائٹیں);
  }

  منطقی calc_inferred_deltas (ثابت contour_نقطہ_سمتیہ_t& orig_points,
			     hb_سمتیہ_t<غیرعلامتی> &scratch)
  {
    غیرعلامتی نقطہ_گنتی = orig_points.لمبائی;
    اگر (نقطہ_گنتی != indices.لمبائی)
      واپس جھوٹ;

    غیرعلامتی ref_گنتی_2 = 0;

    hb_سمتیہ_t<غیرعلامتی> &انتہا_points = scratch.reset ();

    برائے (غیرعلامتی i = 0; i < نقطہ_گنتی; i++)
    {
      ref_گنتی_2 += indices.arrayZ[i];
      اگر (orig_points.arrayZ[i].is_انتہا_نقطہ)
        انتہا_points.push (i);
    }
    /* all points are referenced, nothing to do */
    اگر (ref_گنتی_2 == نقطہ_گنتی)
      واپس سچ;
    اگر (unlikely (انتہا_points.اندر_error ())) واپس جھوٹ;

    hb_ثنائی_ہندسہ_سیٹ_t inferred_idxes;
    غیرعلامتی آغاز_نقطہ_2 = 0;
    برائے (غیرعلامتی انتہا_نقطہ : انتہا_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      غیرعلامتی unref_گنتی = 0;
      برائے (غیرعلامتی i = آغاز_نقطہ_2; i < انتہا_نقطہ + 1; i++)
        unref_گنتی += indices.arrayZ[i];
      unref_گنتی = (انتہا_نقطہ - آغاز_نقطہ_2 + 1) - unref_گنتی;

      غیرعلامتی j = آغاز_نقطہ_2;
      اگر (unref_گنتی == 0 || unref_گنتی > انتہا_نقطہ - آغاز_نقطہ_2)
        جاؤ no_مزید_gaps;
      برائے (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        غیرعلامتی صحیح prev, next, i;
        برائے (;;)
        {
          i = j;
          j = اگلا_اشاریہ (i, آغاز_نقطہ_2, انتہا_نقطہ);
          اگر (indices.arrayZ[i] && !indices.arrayZ[j]) توڑو;
        }
        prev = j = i;
        برائے (;;)
        {
          i = j;
          j = اگلا_اشاریہ (i, آغاز_نقطہ_2, انتہا_نقطہ);
          اگر (!indices.arrayZ[i] && indices.arrayZ[j]) توڑو;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        برائے (;;)
        {
          i = اگلا_اشاریہ (i, آغاز_نقطہ_2, انتہا_نقطہ);
          اگر (i == next) توڑو;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.جوڑو (i);
          اگر (--unref_گنتی == 0) جاؤ no_مزید_gaps;
        }
      }
    no_مزید_gaps:
      آغاز_نقطہ_2 = انتہا_نقطہ + 1;
    }

    برائے (غیرعلامتی i = 0; i < نقطہ_گنتی; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      اگر ( !indices[i])
      {
        اگر (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = سچ;
      }
    }
    واپس سچ;
  }

  منطقی optimize (ثابت contour_نقطہ_سمتیہ_t& contour_points,
                 منطقی is_مرکب,
		 optimize_scratch_t &scratch,
                 دوہرا tolerance = 0.5 + 1e-10)
  {
    غیرعلامتی گنتی = contour_points.لمبائی;
    اگر (deltas_x.لمبائی != گنتی ||
        deltas_y.لمبائی != گنتی)
      واپس جھوٹ;

    hb_سمتیہ_t<منطقی> &opt_indices = scratch.opt_indices.reset ();
    hb_سمتیہ_t<صحیح> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_سمتیہ_t<صحیح> &rounded_y_deltas = scratch.rounded_y_deltas;

    اگر (unlikely (!rounded_x_deltas.resize_dirty  (گنتی) ||
                  !rounded_y_deltas.resize_dirty  (گنتی)))
      واپس جھوٹ;

    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_مقصد<صحیح> (roundf ((دوہرا) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_مقصد<صحیح> (roundf ((دوہرا) deltas_y.arrayZ[i]));
    }

    اگر (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      واپس جھوٹ;

    غیرعلامتی ref_گنتی_2 = 0;
    برائے (منطقی ref_flag : opt_indices)
       ref_گنتی_2 += ref_flag;

    اگر (ref_گنتی_2 == گنتی) واپس سچ;

    hb_سمتیہ_t<اعشاری> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_سمتیہ_t<اعشاری> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    منطقی is_comp_glyph_wo_deltas = (is_مرکب && ref_گنتی_2 == 0);
    اگر (is_comp_glyph_wo_deltas)
    {
      اگر (unlikely (!opt_deltas_x.resize (گنتی) ||
                    !opt_deltas_y.resize (گنتی)))
        واپس جھوٹ;

      opt_indices.arrayZ[0] = سچ;
      برائے (غیرعلامتی i = 1; i < گنتی; i++)
        opt_indices.arrayZ[i] = جھوٹ;
    }

    hb_سمتیہ_t<غیرعلامتی حرف> &opt_نقطہ_ڈیٹا = scratch.opt_نقطہ_ڈیٹا.reset ();
    اگر (!compile_نقطہ_سیٹ (opt_indices, opt_نقطہ_ڈیٹا))
      واپس جھوٹ;
    hb_سمتیہ_t<غیرعلامتی حرف> &opt_deltas_ڈیٹا = scratch.opt_deltas_ڈیٹا.reset ();
    اگر (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_ڈیٹا,
			 scratch.rounded_deltas))
      واپس جھوٹ;

    hb_سمتیہ_t<غیرعلامتی حرف> &نقطہ_ڈیٹا = scratch.نقطہ_ڈیٹا.reset ();
    اگر (!compile_نقطہ_سیٹ (indices, نقطہ_ڈیٹا))
      واپس جھوٹ;
    hb_سمتیہ_t<غیرعلامتی حرف> &deltas_ڈیٹا = scratch.deltas_ڈیٹا.reset ();
    اگر (!compile_deltas (indices, deltas_x, deltas_y, deltas_ڈیٹا, scratch.rounded_deltas))
      واپس جھوٹ;

    اگر (opt_نقطہ_ڈیٹا.لمبائی + opt_deltas_ڈیٹا.لمبائی < نقطہ_ڈیٹا.لمبائی + deltas_ڈیٹا.لمبائی)
    {
      indices = std::move (opt_indices);

      اگر (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    واپس !indices.اندر_error () && !deltas_x.اندر_error () && !deltas_y.اندر_error ();
  }

  ساکن منطقی compile_نقطہ_سیٹ (ثابت hb_سمتیہ_t<منطقی> &نقطہ_indices,
                                 hb_سمتیہ_t<غیرعلامتی حرف>& compiled_points /*برآمد*/)
  {
    غیرعلامتی عدد_points = 0;
    برائے (منطقی i : نقطہ_indices)
      اگر (i) عدد_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    اگر (!عدد_points) واپس سچ;

    غیرعلامتی indices_لمبائی = نقطہ_indices.لمبائی;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    اگر (عدد_points == indices_لمبائی)
      واپس compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    غیرعلامتی عدد_بائٹیں = 2 + 3 *عدد_points;
    اگر (unlikely (!compiled_points.resize_dirty  (عدد_بائٹیں)))
      واپس جھوٹ;

    غیرعلامتی pos = 0;
    /* binary data starts with the total number of reference points */
    اگر (عدد_points < 0x80)
      compiled_points.arrayZ[pos++] = عدد_points;
    ورنہ
    {
      compiled_points.arrayZ[pos++] = ((عدد_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = عدد_points & 0xFF;
    }

    ثابت غیرعلامتی max_تسلسل_لمبائی = 0x7F;
    غیرعلامتی i = 0;
    غیرعلامتی آخری_قدر = 0;
    غیرعلامتی عدد_encoded = 0;
    جب (i < indices_لمبائی && عدد_encoded < عدد_points)
    {
      غیرعلامتی تسلسل_لمبائی = 0;
      غیرعلامتی سرخی_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      منطقی use_byte_encoding = جھوٹ;
      منطقی new_تسلسل_2 = سچ;
      جب (i < indices_لمبائی && عدد_encoded < عدد_points &&
             تسلسل_لمبائی <= max_تسلسل_لمبائی)
      {
        // find out next referenced point index
        جب (i < indices_لمبائی && !نقطہ_indices[i])
          i++;

        اگر (i >= indices_لمبائی) توڑو;

        غیرعلامتی cur_قدر = i;
        غیرعلامتی delta = cur_قدر - آخری_قدر;

        اگر (new_تسلسل_2)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_تسلسل_2 = جھوٹ;
        }

        اگر (use_byte_encoding && delta > 0xFF)
          توڑو;

        اگر (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        ورنہ
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        آخری_قدر = cur_قدر;
        تسلسل_لمبائی++;
        عدد_encoded++;
      }

      اگر (use_byte_encoding)
        compiled_points.arrayZ[سرخی_pos] = تسلسل_لمبائی - 1;
      ورنہ
        compiled_points.arrayZ[سرخی_pos] = (تسلسل_لمبائی - 1) | 0x80;
    }
    واپس compiled_points.resize_dirty  (pos);
  }

  ساکن اعشاری infer_delta (اعشاری ہدف_val, اعشاری پچھلا_val, اعشاری اگلا_val, اعشاری پچھلا_delta, اعشاری اگلا_delta)
  {
    اگر (پچھلا_val == اگلا_val)
      واپس (پچھلا_delta == اگلا_delta) ? پچھلا_delta : 0.f;
    ورنہ اگر (ہدف_val <= hb_min (پچھلا_val, اگلا_val))
      واپس (پچھلا_val < اگلا_val) ? پچھلا_delta : اگلا_delta;
    ورنہ اگر (ہدف_val >= hb_max (پچھلا_val, اگلا_val))
      واپس (پچھلا_val > اگلا_val) ? پچھلا_delta : اگلا_delta;

    اعشاری r = (ہدف_val - پچھلا_val) / (اگلا_val - پچھلا_val);
    واپس پچھلا_delta + r * (اگلا_delta - پچھلا_delta);
  }

  ساکن غیرعلامتی صحیح اگلا_اشاریہ (غیرعلامتی صحیح i, غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا)
  { واپس (i >= انتہا) ? آغاز : (i + 1); }
};

سانچہ <قسم_نام OffType = HBUINT16>
ساخت Tuple_Variation_ڈیٹا
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (یہ));
  }

  size_t get_سائز (غیرعلامتی axis_گنتی_times_2) ثابت
  {
    size_t کل_سائز = min_سائز;
    غیرعلامتی گنتی = tuple_Var_گنتی.get_گنتی ();
    ثابت Tuple_Variation_سرخی *tuple_var_سرخی = &(get_tuple_var_سرخی());
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      کل_سائز = hb_unsigned_جوڑو_saturate (کل_سائز,
					     tuple_var_سرخی->get_سائز (axis_گنتی_times_2),
					     (size_t) tuple_var_سرخی->get_ڈیٹا_سائز ());
      tuple_var_سرخی = &tuple_var_سرخی->get_اگلا (axis_گنتی_times_2);
    }

    واپس کل_سائز;
  }

  ثابت Tuple_Variation_سرخی &get_tuple_var_سرخی (خالی) ثابت
  { واپس StructAfter<Tuple_Variation_سرخی> (ڈیٹا); }

  ساخت tuple_iterator_t;
  ساخت tuple_variations_t
  {
    hb_سمتیہ_t<tuple_delta_t> tuple_vars;

    نجی:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ثابت hb_سمتیہ_t<منطقی>*, hb_سمتیہ_t<غیرعلامتی حرف>> نقطہ_ڈیٹا_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ثابت hb_سمتیہ_t<منطقی>*, غیرعلامتی> نقطہ_سیٹ_گنتی_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_سمتیہ_t<غیرعلامتی حرف> *shared_points_بائٹیں = خالی_اشاریہ;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    غیرعلامتی compiled_byte_سائز = 0;
    منطقی needs_padding = جھوٹ;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    منطقی is_مرکب = جھوٹ;

    عوامی:
    tuple_variations_t () = طےشدہ;
    tuple_variations_t (ثابت tuple_variations_t&) = حذف;
    tuple_variations_t& عامل=(ثابت tuple_variations_t&) = حذف;
    tuple_variations_t (tuple_variations_t&&) = طےشدہ;
    tuple_variations_t& عامل=(tuple_variations_t&&) = طےشدہ;
    ~tuple_variations_t () = طےشدہ;

    صریح عامل منطقی () ثابت { واپس منطقی (tuple_vars); }
    غیرعلامتی get_var_گنتی () ثابت
    {
      غیرعلامتی گنتی = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      برائے (خودکار& tuple: tuple_vars)
        اگر (tuple.compiled_deltas) گنتی++;

      اگر (shared_points_بائٹیں && shared_points_بائٹیں->لمبائی)
        گنتی |= Tuple_Var_گنتی::Shared_نقطہ_Numbers;
      واپس گنتی;
    }

    غیرعلامتی get_compiled_byte_سائز () ثابت
    { واپس compiled_byte_سائز; }

    منطقی create_from_tuple_var_ڈیٹا (tuple_iterator_t iterator,
                                     غیرعلامتی tuple_var_گنتی,
                                     غیرعلامتی نقطہ_گنتی,
                                     منطقی is_gvar,
                                     ثابت hb_map_t *axes_old_اشاریہ_لیبل_map,
                                     ثابت hb_سمتیہ_t<غیرعلامتی> &shared_indices,
                                     ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = خالی_اشاریہ,
                                     منطقی is_مرکب_glyph = جھوٹ)
    {
      hb_سمتیہ_t<غیرعلامتی> نجی_indices;
      hb_سمتیہ_t<صحیح> deltas_x;
      hb_سمتیہ_t<صحیح> deltas_y;
      کرو
      {
        ثابت HBUINT8 *p = iterator.get_serialized_ڈیٹا ();
        غیرعلامتی صحیح لمبائی = iterator.current_tuple->get_ڈیٹا_سائز ();
        اگر (unlikely (!iterator.var_ڈیٹا_بائٹیں.check_range (p, لمبائی)))
          واپس جھوٹ;

	hb_hashmap_t<hb_لیبل_t, Triple> axis_tuples;
        اگر (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_گنتی (), shared_tuples, axes_old_اشاریہ_لیبل_map, axis_tuples)
            || axis_tuples.is_empty ())
          واپس جھوٹ;

	نجی_indices.reset ();
        منطقی has_نجی_points = iterator.current_tuple->has_نجی_points ();
        ثابت HBUINT8 *انتہا = p + لمبائی;
        اگر (has_نجی_points &&
            !Tuple_Variation_ڈیٹا::decompile_points (p, نجی_indices, انتہا))
          واپس جھوٹ;

        ثابت hb_سمتیہ_t<غیرعلامتی> &indices = has_نجی_points ? نجی_indices : shared_indices;
        منطقی apply_مقصد_all = (indices.لمبائی == 0);
        غیرعلامتی عدد_deltas = apply_مقصد_all ? نقطہ_گنتی : indices.لمبائی;

        اگر (unlikely (!deltas_x.resize_dirty  (عدد_deltas) ||
                      !Tuple_Variation_ڈیٹا::decompile_deltas (p, deltas_x, انتہا)))
          واپس جھوٹ;

        اگر (is_gvar)
        {
          اگر (unlikely (!deltas_y.resize_dirty  (عدد_deltas) ||
                        !Tuple_Variation_ڈیٹا::decompile_deltas (p, deltas_y, انتہا)))
            واپس جھوٹ;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        اگر (unlikely (!var.indices.allocate_from_pool (pool, نقطہ_گنتی) ||
                      !var.deltas_x.allocate_from_pool (pool, نقطہ_گنتی, جھوٹ)))
          واپس جھوٹ;

        اگر (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, نقطہ_گنتی, جھوٹ)))
          واپس جھوٹ;

        برائے (غیرعلامتی i = 0; i < عدد_deltas; i++)
        {
          غیرعلامتی idx = apply_مقصد_all ? i : indices[i];
          اگر (idx >= نقطہ_گنتی) جاری;
          var.indices[idx] = سچ;
          var.deltas_x[idx] = deltas_x[i];
          اگر (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } جب (iterator.move_مقصد_اگلا ());

      is_مرکب = is_مرکب_glyph;
      واپس سچ;
    }

    منطقی create_from_شے_var_ڈیٹا (ثابت Var_ڈیٹا &var_ڈیٹا,
                                    ثابت hb_سمتیہ_t<hb_hashmap_t<hb_لیبل_t, Triple>>& regions,
                                    ثابت hb_map_t& axes_old_اشاریہ_لیبل_map,
                                    غیرعلامتی& شے_گنتی_2,
                                    ثابت hb_inc_bimap_t* inner_map = خالی_اشاریہ)
    {
      /* NULL offset, to keep original varidx valid, just return */
      اگر (&var_ڈیٹا == &Null (Var_ڈیٹا))
        واپس سچ;

      غیرعلامتی عدد_regions = var_ڈیٹا.get_region_اشاریہ_گنتی ();
      اگر (!tuple_vars.alloc (عدد_regions)) واپس جھوٹ;

      شے_گنتی_2 = inner_map ? inner_map->get_population () : var_ڈیٹا.get_شے_گنتی ();
      اگر (!شے_گنتی_2) واپس سچ;
      غیرعلامتی صف_سائز = var_ڈیٹا.get_صف_سائز ();
      ثابت HBUINT8 *delta_بائٹیں = var_ڈیٹا.get_delta_بائٹیں ();

      برائے (غیرعلامتی r = 0; r < عدد_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        اگر (!tuple.deltas_x.resize_dirty  (شے_گنتی_2) ||
            !tuple.indices.resize_dirty  (شے_گنتی_2))
          واپس جھوٹ;

        برائے (غیرعلامتی i = 0; i < شے_گنتی_2; i++)
        {
          tuple.indices.arrayZ[i] = سچ;
          tuple.deltas_x.arrayZ[i] = var_ڈیٹا.get_شے_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_بائٹیں, صف_سائز);
        }

        غیرعلامتی region_اشاریہ = var_ڈیٹا.get_region_اشاریہ (r);
        اگر (region_اشاریہ >= regions.لمبائی) واپس جھوٹ;
        tuple.axis_tuples = regions.arrayZ[region_اشاریہ];

        tuple_vars.push (std::move (tuple));
      }
      واپس !tuple_vars.اندر_error ();
    }

    نجی:
    ساکن صحیح cmp_axis_لیبل (ثابت خالی *pa, ثابت خالی *pb)
    {
      ثابت hb_لیبل_t *a = (ثابت hb_لیبل_t*) pa;
      ثابت hb_لیبل_t *b = (ثابت hb_لیبل_t*) pb;
      واپس (صحیح)(*a) - (صحیح)(*b);
    }

    منطقی change_tuple_variations_axis_limits (ثابت hb_hashmap_t<hb_لیبل_t, Triple>& normalized_axes_location,
                                              ثابت hb_hashmap_t<hb_لیبل_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = خالی_اشاریہ)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_سمتیہ_t<hb_لیبل_t> axis_لیبل_فہرست;
      اگر (!axis_لیبل_فہرست.alloc (normalized_axes_location.get_population ()))
        واپس جھوٹ;
      برائے (خودکار t : normalized_axes_location.کلیدیں ())
        axis_لیبل_فہرست.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_نتیجہ_scratch_t scratch;
      hb_سمتیہ_t<tuple_delta_t> برآمد;

      axis_لیبل_فہرست.qsort (cmp_axis_لیبل);
      برائے (خودکار axis_لیبل_2 : axis_لیبل_فہرست)
      {
        Triple *axis_حد_بندی;
        اگر (!normalized_axes_location.has (axis_لیبل_2, &axis_حد_بندی))
          واپس جھوٹ;
        TripleDistances axis_triple_distances{1.f, 1.f};
        اگر (axes_triple_distances.has (axis_لیبل_2))
          axis_triple_distances = axes_triple_distances.get (axis_لیبل_2);

        hb_سمتیہ_t<tuple_delta_t> new_vars;
        برائے (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_حد_بندی (axis_لیبل_2, *axis_حد_بندی, axis_triple_distances, برآمد, scratch, pool);
          اگر (!برآمد) جاری;

          غیرعلامتی new_len = new_vars.لمبائی + برآمد.لمبائی;

          اگر (unlikely (!new_vars.alloc (new_len, جھوٹ)))
            واپس جھوٹ;

          برائے (غیرعلامتی i = 0; i < برآمد.لمبائی; i++)
            new_vars.push (std::move (برآمد[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      واپس سچ;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    منطقی ملاؤ_tuple_variations (contour_نقطہ_سمتیہ_t* contour_points = خالی_اشاریہ)
    {
      hb_سمتیہ_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      اگر (unlikely (!new_vars.alloc (tuple_vars.لمبائی)))
	واپس جھوٹ;
      hb_hashmap_t<ثابت hb_hashmap_t<hb_لیبل_t, Triple>*, غیرعلامتی> m;
      برائے (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        اگر (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          اگر (contour_points && !contour_points->جوڑو_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            واپس جھوٹ;
          جاری;
        }

        غیرعلامتی *idx;
        اگر (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        ورنہ
        {
	  خودکار *new_var = new_vars.push ();
	  اگر (unlikely (new_vars.اندر_error ()))
	    واپس جھوٹ;
          hb_تبادلہ (*new_var, var);
          اگر (unlikely (!m.سیٹ (&(new_var->axis_tuples), new_vars.لمبائی - 1)))
            واپس جھوٹ;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      واپس سچ;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    منطقی compile_all_نقطہ_sets ()
    {
      برائے (ثابت خودکار& tuple: tuple_vars)
      {
        ثابت hb_سمتیہ_t<منطقی>* points_سیٹ = &(tuple.indices);
        اگر (نقطہ_ڈیٹا_map.has (points_سیٹ))
        {
          غیرعلامتی *گنتی;
          اگر (unlikely (!نقطہ_سیٹ_گنتی_map.has (points_سیٹ, &گنتی) ||
                        !نقطہ_سیٹ_گنتی_map.سیٹ (points_سیٹ, (*گنتی) + 1)))
            واپس جھوٹ;
          جاری;
        }

        hb_سمتیہ_t<غیرعلامتی حرف> compiled_نقطہ_ڈیٹا;
        اگر (!tuple_delta_t::compile_نقطہ_سیٹ (*points_سیٹ, compiled_نقطہ_ڈیٹا))
          واپس جھوٹ;

        اگر (!نقطہ_ڈیٹا_map.سیٹ (points_سیٹ, std::move (compiled_نقطہ_ڈیٹا)) ||
            !نقطہ_سیٹ_گنتی_map.سیٹ (points_سیٹ, 1))
          واپس جھوٹ;
      }
      واپس سچ;
    }

    /* find shared points set which saves most bytes */
    خالی ڈھونڈو_shared_points ()
    {
      غیرعلامتی max_saved_بائٹیں = 0;

      برائے (ثابت خودکار& _ : نقطہ_ڈیٹا_map.iter_ref ())
      {
        ثابت hb_سمتیہ_t<منطقی>* points_سیٹ = _.پہلا;
        غیرعلامتی ڈیٹا_لمبائی = _.second.لمبائی;
        اگر (!ڈیٹا_لمبائی) جاری;
        غیرعلامتی *گنتی;
        اگر (unlikely (!نقطہ_سیٹ_گنتی_map.has (points_سیٹ, &گنتی) ||
                      *گنتی <= 1))
        {
          shared_points_بائٹیں = خالی_اشاریہ;
          واپس;
        }

        غیرعلامتی saved_بائٹیں = ڈیٹا_لمبائی * ((*گنتی) -1);
        اگر (saved_بائٹیں > max_saved_بائٹیں)
        {
          max_saved_بائٹیں = saved_بائٹیں;
          shared_points_بائٹیں = &(_.second);
        }
      }
    }

    منطقی calc_inferred_deltas (ثابت contour_نقطہ_سمتیہ_t& contour_points,
			       hb_سمتیہ_t<غیرعلامتی> &scratch)
    {
      برائے (tuple_delta_t& var : tuple_vars)
        اگر (!var.calc_inferred_deltas (contour_points, scratch))
          واپس جھوٹ;

      واپس سچ;
    }

    منطقی iup_optimize (ثابت contour_نقطہ_سمتیہ_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      برائے (tuple_delta_t& var : tuple_vars)
      {
        اگر (!var.optimize (contour_points, is_مرکب, scratch))
          واپس جھوٹ;
      }
      واپس سچ;
    }

    عوامی:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    خالی cull_unreachable (ثابت hb_hashmap_t<hb_لیبل_t, Triple> &reachable_ranges)
    {
      غیرعلامتی j = 0;
      برائے (غیرعلامتی i = 0; i < tuple_vars.لمبائی; i++)
      {
	اگر (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  جاری;
	اگر (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    منطقی instantiate (ثابت hb_hashmap_t<hb_لیبل_t, Triple>& normalized_axes_location,
                      ثابت hb_hashmap_t<hb_لیبل_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = خالی_اشاریہ,
                      contour_نقطہ_سمتیہ_t* contour_points = خالی_اشاریہ,
                      منطقی optimize = جھوٹ)
    {
      اگر (!tuple_vars) واپس سچ;
      اگر (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        واپس جھوٹ;
      /* compute inferred deltas only for gvar */
      اگر (contour_points)
      {
	hb_سمتیہ_t<غیرعلامتی> scratch;
	اگر (!calc_inferred_deltas (*contour_points, scratch))
	  واپس جھوٹ;
      }

      /* if iup delta opt is on, contour_points can't be null */
      اگر (optimize && !contour_points)
        واپس جھوٹ;

      اگر (!ملاؤ_tuple_variations (optimize ? contour_points : خالی_اشاریہ))
        واپس جھوٹ;

      اگر (optimize && !iup_optimize (*contour_points, scratch)) واپس جھوٹ;
      واپس !tuple_vars.اندر_error ();
    }

    منطقی compile_بائٹیں (ثابت hb_map_t& axes_اشاریہ_map,
                        ثابت hb_map_t& axes_old_اشاریہ_لیبل_map,
                        منطقی use_shared_points,
                        منطقی is_gvar = جھوٹ,
                        ثابت hb_hashmap_t<ثابت hb_سمتیہ_t<F2DOT14>*, غیرعلامتی>* shared_tuples_idx_map = خالی_اشاریہ,
			hb_alloc_pool_t *pool = خالی_اشاریہ)
    {
      // return true for empty glyph
      اگر (!tuple_vars)
        واپس سچ;

      // compile points set and store data in hashmap
      اگر (!compile_all_نقطہ_sets ())
        واپس جھوٹ;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_سائز += 4;

      اگر (use_shared_points)
      {
        ڈھونڈو_shared_points ();
        اگر (shared_points_بائٹیں)
          compiled_byte_سائز += shared_points_بائٹیں->لمبائی;
      }
      hb_سمتیہ_t<صحیح> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      برائے (خودکار& tuple: tuple_vars)
      {
        ثابت hb_سمتیہ_t<منطقی>* points_سیٹ = &(tuple.indices);
        hb_سمتیہ_t<غیرعلامتی حرف> *points_ڈیٹا;
        اگر (unlikely (!نقطہ_ڈیٹا_map.has (points_سیٹ, &points_ڈیٹا)))
          واپس جھوٹ;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        اگر (!points_ڈیٹا->لمبائی)
          جاری;
        اگر (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          واپس جھوٹ;

        غیرعلامتی points_ڈیٹا_لمبائی = (points_ڈیٹا != shared_points_بائٹیں) ? points_ڈیٹا->لمبائی : 0;
        اگر (!tuple.compile_tuple_var_سرخی (axes_اشاریہ_map, points_ڈیٹا_لمبائی, axes_old_اشاریہ_لیبل_map,
                                             shared_tuples_idx_map,
					     pool))
          واپس جھوٹ;
        compiled_byte_سائز += tuple.compiled_tuple_سرخی.لمبائی + points_ڈیٹا_لمبائی + tuple.compiled_deltas.لمبائی;
      }

      اگر (is_gvar && (compiled_byte_سائز % 2))
      {
        needs_padding = سچ;
        compiled_byte_سائز += 1;
      }

      واپس سچ;
    }

    منطقی serialize_var_سرخیاں (hb_serialize_context_t *c, غیرعلامتی& کل_سرخی_len) ثابت
    {
      TRACE_SERIALIZE (یہ);
      برائے (ثابت خودکار& tuple: tuple_vars)
      {
        tuple.compiled_tuple_سرخی.as_array ().copy (c);
        اگر (c->اندر_error ()) return_trace (جھوٹ);
        کل_سرخی_len += tuple.compiled_tuple_سرخی.لمبائی;
      }
      return_trace (سچ);
    }

    منطقی serialize_var_ڈیٹا (hb_serialize_context_t *c, منطقی is_gvar) ثابت
    {
      TRACE_SERIALIZE (یہ);
      اگر (is_gvar && shared_points_بائٹیں)
      {
        hb_ubytes_t s (shared_points_بائٹیں->arrayZ, shared_points_بائٹیں->لمبائی);
        s.copy (c);
      }

      برائے (ثابت خودکار& tuple: tuple_vars)
      {
        ثابت hb_سمتیہ_t<منطقی>* points_سیٹ = &(tuple.indices);
        hb_سمتیہ_t<غیرعلامتی حرف> *نقطہ_ڈیٹا;
        اگر (!نقطہ_ڈیٹا_map.has (points_سیٹ, &نقطہ_ڈیٹا))
          return_trace (جھوٹ);

        اگر (!is_gvar || نقطہ_ڈیٹا != shared_points_بائٹیں)
        {
          hb_ubytes_t s (نقطہ_ڈیٹا->arrayZ, نقطہ_ڈیٹا->لمبائی);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        اگر (c->اندر_error ()) return_trace (جھوٹ);
      }

      /* padding for gvar */
      اگر (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        اگر (!c->embed (pad)) return_trace (جھوٹ);
      }
      return_trace (سچ);
    }
  };

  ساخت tuple_iterator_t
  {
    غیرعلامتی get_axis_گنتی () ثابت { واپس axis_گنتی; }

    خالی init (hb_بائٹیں_t var_ڈیٹا_بائٹیں_2, غیرعلامتی صحیح axis_گنتی_3, ثابت خالی *جدول_بنیاد)
    {
      var_ڈیٹا_بائٹیں = var_ڈیٹا_بائٹیں_2;
      var_ڈیٹا = var_ڈیٹا_بائٹیں_2.as<Tuple_Variation_ڈیٹا> ();
      tuples_بایاں = var_ڈیٹا->tuple_Var_گنتی.get_گنتی ();
      axis_گنتی = axis_گنتی_3;
      axis_گنتی_times_2 = axis_گنتی_3 * 2;
      current_tuple = &var_ڈیٹا->get_tuple_var_سرخی ();
      ڈیٹا_انحراف_2 = 0;
      جدول_بنیاد_2 = جدول_بنیاد;
    }

    منطقی get_shared_indices (hb_سمتیہ_t<غیرعلامتی صحیح> &shared_indices /*برآمد*/)
    {
      اگر (var_ڈیٹا->has_shared_نقطہ_numbers ())
      {
        ثابت HBUINT8 *بنیاد = &(جدول_بنیاد_2+var_ڈیٹا->ڈیٹا);
        ثابت HBUINT8 *p = بنیاد;
        اگر (!decompile_points (p, shared_indices, (ثابت HBUINT8 *) (var_ڈیٹا_بائٹیں.arrayZ + var_ڈیٹا_بائٹیں.لمبائی))) واپس جھوٹ;
        ڈیٹا_انحراف_2 = p - بنیاد;
      }
      واپس سچ;
    }

    منطقی is_valid ()
    {
      اگر (unlikely (tuples_بایاں <= 0))
	واپس جھوٹ;

      current_tuple_سائز = Tuple_Variation_سرخی::min_سائز;
      اگر (unlikely (!var_ڈیٹا_بائٹیں.check_انتہا ((ثابت حرف *) current_tuple + current_tuple_سائز)))
	واپس جھوٹ;

      current_tuple_سائز = current_tuple->get_سائز (axis_گنتی_times_2);
      اگر (unlikely (!var_ڈیٹا_بائٹیں.check_انتہا ((ثابت حرف *) current_tuple + current_tuple_سائز)))
	واپس جھوٹ;

      واپس سچ;
    }

    HB_ALWAYS_INLINE
    منطقی move_مقصد_اگلا ()
    {
      ڈیٹا_انحراف_2 += current_tuple->get_ڈیٹا_سائز ();
      current_tuple = &Struct_At_انحراف<Tuple_Variation_سرخی> (current_tuple, current_tuple_سائز);
      tuples_بایاں--;
      واپس is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ثابت HBUINT8 *get_serialized_ڈیٹا () ثابت
    { واپس &(جدول_بنیاد_2+var_ڈیٹا->ڈیٹا) + ڈیٹا_انحراف_2; }

    نجی:
    علامتی tuples_بایاں;
    ثابت Tuple_Variation_ڈیٹا *var_ڈیٹا;
    غیرعلامتی صحیح axis_گنتی;
    غیرعلامتی صحیح axis_گنتی_times_2;
    غیرعلامتی صحیح ڈیٹا_انحراف_2;
    غیرعلامتی صحیح current_tuple_سائز;
    ثابت خالی *جدول_بنیاد_2;

    عوامی:
    hb_بائٹیں_t var_ڈیٹا_بائٹیں;
    ثابت Tuple_Variation_سرخی *current_tuple;
  };

  ساکن منطقی get_tuple_iterator (hb_بائٹیں_t var_ڈیٹا_بائٹیں, غیرعلامتی axis_گنتی,
                                  ثابت خالی *جدول_بنیاد_2,
                                  hb_سمتیہ_t<غیرعلامتی صحیح> &shared_indices /*برآمد*/,
                                  tuple_iterator_t *iterator /*برآمد*/)
  {
    iterator->init (var_ڈیٹا_بائٹیں, axis_گنتی, جدول_بنیاد_2);
    اگر (!iterator->get_shared_indices (shared_indices))
      واپس جھوٹ;
    واپس iterator->is_valid ();
  }

  منطقی has_shared_نقطہ_numbers () ثابت { واپس tuple_Var_گنتی.has_shared_نقطہ_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  منطقی cull_tuple_variations (hb_بائٹیں_t var_ڈیٹا_بائٹیں,
			      غیرعلامتی axis_گنتی,
			      hb_array_t<ثابت F2DOT14> shared_tuples,
			      ثابت hb_map_t *axes_old_اشاریہ_لیبل_map,
			      ثابت hb_hashmap_t<hb_لیبل_t, Triple> &reachable_ranges,
			      hb_سمتیہ_t<حرف> &برآمد /*برآمد*/,
			      منطقی *بدلا /*برآمد*/) ثابت
  {
    *بدلا = جھوٹ;

    hb_سمتیہ_t<غیرعلامتی صحیح> shared_indices;
    tuple_iterator_t iterator;
    اگر (!get_tuple_iterator (var_ڈیٹا_بائٹیں, axis_گنتی, یہ,
			     shared_indices, &iterator))
      واپس سچ; /* no tuples or malformed: keep original */

    ثابت حرف *بائٹیں_آغاز = var_ڈیٹا_بائٹیں.arrayZ;
    ثابت حرف *بائٹیں_انتہا = بائٹیں_آغاز + var_ڈیٹا_بائٹیں.لمبائی;
    ثابت حرف *serialized_بنیاد = (ثابت حرف *) &(یہ+ڈیٹا);
    ثابت حرف *shared_points_انتہا = (ثابت حرف *) iterator.get_serialized_ڈیٹا ();
    اگر (unlikely (serialized_بنیاد < بائٹیں_آغاز || serialized_بنیاد > بائٹیں_انتہا ||
		  shared_points_انتہا < serialized_بنیاد || shared_points_انتہا > بائٹیں_انتہا))
      واپس سچ;

    ساخت kept_tuple_t
    {
      ثابت حرف *سرخی;
      غیرعلامتی سرخی_سائز;
      ثابت حرف *ڈیٹا;
      غیرعلامتی ڈیٹا_سائز;
    };
    hb_سمتیہ_t<kept_tuple_t> kept;
    غیرعلامتی کل = 0;
    غیرعلامتی kept_سرخیاں_سائز = 0, kept_ڈیٹا_سائز = 0;
    کرو
    {
      ثابت Tuple_Variation_سرخی *سرخی = iterator.current_tuple;
      غیرعلامتی سرخی_سائز = سرخی->get_سائز (axis_گنتی * 2);
      غیرعلامتی ڈیٹا_سائز = سرخی->get_ڈیٹا_سائز ();
      ثابت حرف *tuple_ڈیٹا = (ثابت حرف *) iterator.get_serialized_ڈیٹا ();
      اگر (unlikely (tuple_ڈیٹا < serialized_بنیاد ||
		    ڈیٹا_سائز > (غیرعلامتی) (بائٹیں_انتہا - tuple_ڈیٹا)))
	واپس سچ; /* malformed: keep original */

      hb_hashmap_t<hb_لیبل_t, Triple> axis_tuples;
      اگر (!سرخی->unpack_axis_tuples (axis_گنتی, shared_tuples,
				       axes_old_اشاریہ_لیبل_map, axis_tuples))
	واپس سچ;

      کل++;
      اگر (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ثابت حرف *) سرخی, سرخی_سائز,
				 tuple_ڈیٹا, ڈیٹا_سائز});
	kept_سرخیاں_سائز += سرخی_سائز;
	kept_ڈیٹا_سائز += ڈیٹا_سائز;
      }
    } جب (iterator.move_مقصد_اگلا ());

    اگر (unlikely (kept.اندر_error ())) واپس جھوٹ;
    اگر (kept.لمبائی == کل) واپس سچ; /* nothing to cull */

    *بدلا = سچ;
    اگر (!kept.لمبائی) واپس سچ; /* everything culled: no variation data */

    غیرعلامتی shared_points_سائز = shared_points_انتہا - serialized_بنیاد;
    غیرعلامتی ڈیٹا_انحراف_2 = min_سائز + kept_سرخیاں_سائز;
    غیرعلامتی new_سائز = ڈیٹا_انحراف_2 + shared_points_سائز + kept_ڈیٹا_سائز;
    اگر (unlikely (!برآمد.resize (new_سائز)))
      واپس جھوٹ;

    Tuple_Variation_ڈیٹا *برآمد_ڈیٹا = (Tuple_Variation_ڈیٹا *) برآمد.arrayZ;
    برآمد_ڈیٹا->tuple_Var_گنتی = (uint16_t) (kept.لمبائی |
					  (has_shared_نقطہ_numbers () ? 0x8000u : 0u));
    برآمد_ڈیٹا->ڈیٹا = ڈیٹا_انحراف_2;

    حرف *p = برآمد.arrayZ + min_سائز;
    برائے (ثابت خودکار &t : kept)
    {
      hb_memcpy (p, t.سرخی, t.سرخی_سائز);
      p += t.سرخی_سائز;
    }
    hb_memcpy (p, serialized_بنیاد, shared_points_سائز);
    p += shared_points_سائز;
    برائے (ثابت خودکار &t : kept)
    {
      hb_memcpy (p, t.ڈیٹا, t.ڈیٹا_سائز);
      p += t.ڈیٹا_سائز;
    }
    واپس سچ;
  }

  ساکن منطقی decompile_points (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_سمتیہ_t<غیرعلامتی صحیح> &points /*برآمد*/,
				ثابت HBUINT8 *انتہا)
  {
    شمار packed_نقطہ_flag_t
    {
      POINTS_ARE_الفاظ     = 0x80,
      نقطہ_تسلسل_گنتی_نقاب = 0x7F
    };

    اگر (unlikely (p + 1 > انتہا)) واپس جھوٹ;

    غیرعلامتی گنتی = *p++;
    اگر (گنتی & POINTS_ARE_الفاظ)
    {
      اگر (unlikely (p + 1 > انتہا)) واپس جھوٹ;
      گنتی = ((گنتی & نقطہ_تسلسل_گنتی_نقاب) << 8) | *p++;
    }
    اگر (unlikely (!points.resize_dirty  (گنتی))) واپس جھوٹ;

    غیرعلامتی n = 0;
    غیرعلامتی i = 0;
    جب (i < گنتی)
    {
      اگر (unlikely (p + 1 > انتہا)) واپس جھوٹ;
      غیرعلامتی control = *p++;
      غیرعلامتی تسلسل_گنتی = (control & نقطہ_تسلسل_گنتی_نقاب) + 1;
      غیرعلامتی stop = i + تسلسل_گنتی;
      اگر (unlikely (stop > گنتی)) واپس جھوٹ;
      اگر (control & POINTS_ARE_الفاظ)
      {
        اگر (unlikely (p + تسلسل_گنتی * HBUINT16::static_سائز > انتہا)) واپس جھوٹ;
        برائے (; i < stop; i++)
        {
          n += *(ثابت HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_سائز;
        }
      }
      ورنہ
      {
        اگر (unlikely (p + تسلسل_گنتی > انتہا)) واپس جھوٹ;
        برائے (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    واپس سچ;
  }

  سانچہ <قسم_نام T>
  HB_ALWAYS_INLINE
  ساکن منطقی decompile_deltas (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_سمتیہ_t<T> &deltas /* IN/OUT */,
				ثابت HBUINT8 *انتہا,
				منطقی consume_all = جھوٹ,
				غیرعلامتی آغاز = 0)
  {
    واپس Tuple_اقدار::decompile (p, deltas, انتہا, consume_all, آغاز);
  }

  منطقی has_ڈیٹا () ثابت { واپس tuple_Var_گنتی; }

  منطقی decompile_tuple_variations (غیرعلامتی نقطہ_گنتی,
                                   منطقی is_gvar,
                                   tuple_iterator_t iterator,
                                   ثابت hb_map_t *axes_old_اشاریہ_لیبل_map,
                                   ثابت hb_سمتیہ_t<غیرعلامتی> &shared_indices,
                                   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /*برآمد*/
				   hb_alloc_pool_t *pool = خالی_اشاریہ,
                                   منطقی is_مرکب_glyph = جھوٹ) ثابت
  {
    واپس tuple_variations.create_from_tuple_var_ڈیٹا (iterator, tuple_Var_گنتی,
                                                        نقطہ_گنتی, is_gvar,
                                                        axes_old_اشاریہ_لیبل_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_مرکب_glyph);
  }

  منطقی serialize (hb_serialize_context_t *c,
                  منطقی is_gvar,
                  ثابت tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (یہ);
    /* empty tuple variations, just return and skip serialization. */
    اگر (!tuple_variations) return_trace (سچ);

    خودکار *برآمد = c->آغاز_embed (یہ);
    اگر (unlikely (!c->extend_min (برآمد))) return_trace (جھوٹ);

    اگر (!c->check_تفویض (برآمد->tuple_Var_گنتی, tuple_variations.get_var_گنتی (),
                          HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)) return_trace (جھوٹ);

    غیرعلامتی کل_سرخی_len = 0;

    اگر (!tuple_variations.serialize_var_سرخیاں (c, کل_سرخی_len))
      return_trace (جھوٹ);

    غیرعلامتی ڈیٹا_انحراف_2 = min_سائز + کل_سرخی_len;
    اگر (!is_gvar) ڈیٹا_انحراف_2 += 4;
    اگر (!c->check_تفویض (برآمد->ڈیٹا, ڈیٹا_انحراف_2, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)) return_trace (جھوٹ);

    واپس tuple_variations.serialize_var_ڈیٹا (c, is_gvar);
  }

  محفوظ:
  ساخت Tuple_Var_گنتی : HBUINT16
  {
    دوست ساخت tuple_variations_t;
    منطقی has_shared_نقطہ_numbers () ثابت { واپس ((*یہ) & Shared_نقطہ_Numbers); }
    غیرعلامتی صحیح get_گنتی () ثابت { واپس (*یہ) & گنتی_نقاب; }
    Tuple_Var_گنتی& عامل = (uint16_t i) { HBUINT16::عامل= (i); واپس *یہ; }
    صریح عامل منطقی () ثابت { واپس get_گنتی (); }

    محفوظ:
    شمار Flags
    {
      Shared_نقطہ_Numbers= 0x8000u,
      گنتی_نقاب         = 0x0FFFu
    };
    عوامی:
    DEFINE_سائز_STATIC (2);
  };

  Tuple_Var_گنتی tuple_Var_گنتی;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  انحراف_مقصد<HBUINT8, OffType>
                ڈیٹا;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  عوامی:
  DEFINE_سائز_MIN (2 + OffType::static_سائز);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
استعمال tuple_variations_t = Tuple_Variation_ڈیٹا<HBUINT16>::tuple_variations_t;
ساخت شے_variations_t
{
  استعمال region_t = ثابت hb_hashmap_t<hb_لیبل_t, Triple>*;
  نجی:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_سمتیہ_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_سمتیہ_t<غیرعلامتی> var_ڈیٹا_عدد_صفیں;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_سمتیہ_t<hb_hashmap_t<hb_لیبل_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_سمتیہ_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, غیرعلامتی> region_map;

  /* all delta rows after instantiation */
  hb_سمتیہ_t<hb_سمتیہ_t<صحیح>> delta_صفیں;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_سمتیہ_t<delta_صف_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  منطقی has_long = جھوٹ;

  عوامی:
  منطقی has_long_لفظ () ثابت
  { واپس has_long; }

  ثابت hb_سمتیہ_t<region_t>& get_region_list () ثابت
  { واپس region_list; }

  ثابت hb_سمتیہ_t<delta_صف_encoding_t>& get_vardata_encodings () ثابت
  { واپس encodings; }

  ثابت hb_map_t& get_varidx_map () ثابت
  { واپس varidx_map; }

  منطقی جوڑو_vardata_encoding_for_testing (hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح>*> &&صفیں,
                                         غیرعلامتی عدد_ستون_تعداد)
  {
    encodings.push (delta_صف_encoding_t (std::move (صفیں), عدد_ستون_تعداد));
    واپس !encodings.اندر_error ();
  }

  منطقی compile_varidx_map_for_testing (ثابت hb_hashmap_t<غیرعلامتی, ثابت hb_سمتیہ_t<صحیح>*>& front_mapping)
  { واپس compile_varidx_map (front_mapping); }

  منطقی instantiate (ثابت شے_Variation_Store& varStore,
                    ثابت hb_subset_plan_t *plan,
                    منطقی optimize=سچ,
                    منطقی use_no_variation_idx=سچ,
                    ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> (),
                    منطقی layout_conditions = جھوٹ)
  {
    اگر (!create_from_شے_varstore (varStore, plan->axes_old_اشاریہ_لیبل_map, inner_maps))
      واپس جھوٹ;
    /* avar2 partial instancing: cull unreachable regions. */
    اگر (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      برائے (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    اگر (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      واپس جھوٹ;
    اگر (layout_conditions &&
        (!instantiate_condition_اقدار (plan, varStore) || !build_region_list ()))
      واپس جھوٹ;
    واپس as_شے_varstore (optimize, use_no_variation_idx);
  }

  منطقی instantiate_condition_اقدار (ثابت hb_subset_plan_t *plan,
                                      ثابت شے_Variation_Store &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    خودکار &condition_map = plan->layout_condition_idx_قدر_map;
    condition_map.clear ();
    اگر (!plan->layout_condition_اقدار) واپس سچ;
    استعمال condition_قدر_t = hb_pair_t<uintptr_t, hb_pair_t<غیرعلامتی, صحیح>>;
    hb_سمتیہ_t<condition_قدر_t> conditions;
    برائے (خودکار condition : plan->layout_condition_اقدار.iter ())
      conditions.push (condition);
    اگر (unlikely (conditions.اندر_error ())) واپس جھوٹ;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ثابت condition_قدر_t &a, ثابت condition_قدر_t &b) {
      اگر (a.second.پہلا != b.second.پہلا)
        واپس a.second.پہلا < b.second.پہلا ? -1 : 1;
      واپس a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    غیرعلامتی new_outer = 0;
    برائے (غیرعلامتی i = 0; i < plan->gdef_varstore_inner_maps.لمبائی; i++)
      اگر (plan->gdef_varstore_inner_maps[i].get_population ())
        اگر (!outer_map.سیٹ (i, new_outer++)) واپس جھوٹ;
    برائے (خودکار condition : conditions)
    {
      غیرعلامتی old_idx = condition.second.پہلا;
      دوہرا value = condition.second.second +
                     varStore.get_delta<دوہرا> (old_idx, plan->normalized_coords.as_array ());
      غیرعلامتی old_outer = old_idx >> 16;
      اگر (!outer_map.has (old_outer))
        جاری;
      غیرعلامتی outer = outer_map.get (old_outer);
      غیرعلامتی inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      غیرعلامتی idx = (outer << 16) | inner;
      اگر (outer >= vars.لمبائی || inner >= var_ڈیٹا_عدد_صفیں[outer])
        جاری;

      دوہرا زیادہ_سے_زیادہ = fabs ((دوہرا) value);
      منطقی variable = جھوٹ;
      برائے (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        اگر (unlikely (inner >= tuple.deltas_x.لمبائی)) واپس جھوٹ;
        اعشاری delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        زیادہ_سے_زیادہ = hb_max (زیادہ_سے_زیادہ, fabs ((دوہرا) delta));
      }
      اگر (!variable)
      {
        اگر (!condition_map.سیٹ (condition.پہلا,
                                hb_pair (غیرعلامتی (VarIdx::NO_VARIATION), صحیح (value > 0))))
          واپس جھوٹ;
        جاری;
      }

      دوہرا scale = 1.;
      جب (زیادہ_سے_زیادہ * scale > زیادہ_سے_زیادہ_عدد)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      جب (زیادہ_سے_زیادہ * scale * 2 <= زیادہ_سے_زیادہ_عدد)
      {
        منطقی integral = (دوہرا) value * scale == round ((دوہرا) value * scale);
        برائے (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (دوہرا) tuple.deltas_x[inner] * scale ==
                      round ((دوہرا) tuple.deltas_x[inner] * scale);
        اگر (integral) توڑو;
        scale *= 2;
      }
      صحیح default_قدر = (صحیح) round ((دوہرا) value * scale);
      اگر (scale != 1. || default_قدر < -32768 || default_قدر > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t نجی_صف;
        برائے (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          اگر (!tuple.deltas_x[inner]) جاری;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (سچ);
          copy.deltas_x.push ((اعشاری) round ((دوہرا) tuple.deltas_x[inner] * scale));
          اگر (unlikely (copy.axis_tuples.اندر_error () || copy.indices.اندر_error () ||
                        copy.deltas_x.اندر_error ())) واپس جھوٹ;
          نجی_صف.tuple_vars.push (std::move (copy));
        }
        اگر (unlikely (vars.لمبائی >= 0xFFFFu)) واپس جھوٹ;
        اگر (unlikely (نجی_صف.tuple_vars.اندر_error ())) واپس جھوٹ;
        idx = vars.لمبائی << 16;
        vars.push (std::move (نجی_صف));
        var_ڈیٹا_عدد_صفیں.push (1);
        اگر (unlikely (vars.اندر_error () || var_ڈیٹا_عدد_صفیں.اندر_error ())) واپس جھوٹ;
        اگر (default_قدر < -32768 || default_قدر > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          صحیح bias = default_قدر - hb_clamp (default_قدر, -32768, 32767);
          default_قدر -= bias;
          صحیح بلند = bias / 65536 * 65536;
          صحیح parts[] = {بلند, bias - بلند};
          برائے (صحیح part : parts)
          {
            اگر (!part) جاری;
            غیرعلامتی گنتی = vars[idx >> 16].tuple_vars.لمبائی;
            جوڑو_tuple (idx >> 16, hb_hashmap_t<hb_لیبل_t, Triple> (),
                       0, part, 1);
            ثابت خودکار &tuples = vars[idx >> 16].tuple_vars;
            اگر (unlikely (tuples.اندر_error () || tuples.لمبائی != گنتی + 1 ||
                          tuples.دم ().indices.اندر_error () ||
                          tuples.دم ().deltas_x.اندر_error ())) واپس جھوٹ;
          }
        }
      }
      اگر (!condition_map.سیٹ (condition.پہلا, hb_pair (idx, default_قدر)))
        واپس جھوٹ;
    }
    واپس !condition_map.اندر_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  منطقی create_from_شے_varstore (ثابت شے_Variation_Store& varStore,
                                  ثابت hb_map_t& axes_old_اشاریہ_لیبل_map,
                                  ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> ())
  {
    ثابت VarRegionList& regionList = varStore.get_region_list ();
    اگر (!regionList.get_var_regions (axes_old_اشاریہ_لیبل_map, orig_region_list))
      واپس جھوٹ;

    غیرعلامتی عدد_var_ڈیٹا = varStore.get_sub_جدول_گنتی ();
    اگر (inner_maps && inner_maps.لمبائی != عدد_var_ڈیٹا) واپس جھوٹ;
    اگر (!vars.alloc (عدد_var_ڈیٹا) ||
        !var_ڈیٹا_عدد_صفیں.alloc (عدد_var_ڈیٹا)) واپس جھوٹ;

    برائے (غیرعلامتی i = 0; i < عدد_var_ڈیٹا; i++)
    {
      اگر (inner_maps && !inner_maps.arrayZ[i].get_population ())
          جاری;
      tuple_variations_t var_ڈیٹا_tuples;
      غیرعلامتی شے_گنتی_2 = 0;
      اگر (!var_ڈیٹا_tuples.create_from_شے_var_ڈیٹا (varStore.get_sub_جدول (i),
                                                      orig_region_list,
                                                      axes_old_اشاریہ_لیبل_map,
                                                      شے_گنتی_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : خالی_اشاریہ))
        واپس جھوٹ;

      var_ڈیٹا_عدد_صفیں.push (شے_گنتی_2);
      vars.push (std::move (var_ڈیٹا_tuples));
    }
    واپس !vars.اندر_error () && !var_ڈیٹا_عدد_صفیں.اندر_error () && vars.لمبائی == var_ڈیٹا_عدد_صفیں.لمبائی;
  }

  منطقی instantiate_tuple_vars (ثابت hb_hashmap_t<hb_لیبل_t, Triple>& normalized_axes_location,
                               ثابت hb_hashmap_t<hb_لیبل_t, TripleDistances>& axes_triple_distances,
                               منطقی build_regions = سچ)
  {
    optimize_scratch_t scratch;
    برائے (tuple_variations_t& tuple_vars : vars)
      اگر (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        واپس جھوٹ;

    واپس !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  غیرعلامتی جوڑو_vardata (غیرعلامتی شے_گنتی_2)
  {
    vars.push (tuple_variations_t ());
    var_ڈیٹا_عدد_صفیں.push (شے_گنتی_2);
    واپس vars.لمبائی - 1;
  }

  /* Get item count for a VarData subtable. */
  غیرعلامتی get_شے_گنتی (غیرعلامتی outer) ثابت
  { واپس outer < var_ڈیٹا_عدد_صفیں.لمبائی ? var_ڈیٹا_عدد_صفیں[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  غیرعلامتی duplicate_صف (غیرعلامتی outer, غیرعلامتی inner)
  {
    اگر (unlikely (outer >= vars.لمبائی ||
                  inner >= var_ڈیٹا_عدد_صفیں[outer] ||
                  var_ڈیٹا_عدد_صفیں[outer] >= 0xFFFFu))
      واپس (غیرعلامتی) -1;
    برائے (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      اگر (unlikely (inner >= tuple.deltas_x.لمبائی ||
                    inner >= tuple.indices.لمبائی))
        واپس (غیرعلامتی) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      اگر (inner < tuple.deltas_y.لمبائی)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      اگر (unlikely (tuple.indices.اندر_error () ||
                    tuple.deltas_x.اندر_error () ||
                    tuple.deltas_y.اندر_error ()))
        واپس (غیرعلامتی) -1;
    }
    واپس var_ڈیٹا_عدد_صفیں[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  خالی جوڑو_tuple (غیرعلامتی outer,
                  hb_hashmap_t<hb_لیبل_t, Triple>&& axis_tuples,
                  غیرعلامتی inner, صحیح delta, غیرعلامتی شے_گنتی_2)
  {
    اگر (outer >= vars.لمبائی) واپس;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    اگر (!tuple.indices.resize (شے_گنتی_2) ||
        !tuple.deltas_x.resize (شے_گنتی_2)) واپس;
    برائے (غیرعلامتی i = 0; i < شے_گنتی_2; i++)
    {
      tuple.indices.arrayZ[i] = سچ;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (اعشاری) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  منطقی build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, غیرعلامتی> all_regions;
    hb_hashmap_t<region_t, غیرعلامتی> مستعمل_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_سمتیہ_t<region_t> all_unique_regions;
    برائے (ثابت tuple_variations_t& sub_جدول_2 : vars)
    {
      برائے (ثابت tuple_delta_t& tuple : sub_جدول_2.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        اگر (!مستعمل_regions.has (r))
        {
          منطقی all_zeros = سچ;
          برائے (اعشاری d : tuple.deltas_x)
          {
            صحیح delta = hb_clamp_مقصد<صحیح> (roundf ((دوہرا) d));
            اگر (delta != 0)
            {
              all_zeros = جھوٹ;
              توڑو;
            }
          }
          اگر (!all_zeros)
          {
            اگر (!مستعمل_regions.سیٹ (r, 1))
              واپس جھوٹ;
          }
        }
        اگر (all_regions.has (r))
          جاری;
        اگر (!all_regions.سیٹ (r, 1))
          واپس جھوٹ;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    اگر (!all_regions || !all_unique_regions) واپس سچ;

    اگر (!region_list.alloc (all_regions.get_population ()))
      واپس جھوٹ;

    غیرعلامتی idx = 0;
    /* append the original regions that pre-existed */
    برائے (ثابت خودکار& r : orig_region_list)
    {
      اگر (!all_regions.has (&r) || !مستعمل_regions.has (&r))
        جاری;

      region_list.push (&r);
      اگر (!region_map.سیٹ (&r, idx))
        واپس جھوٹ;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    برائے (ثابت خودکار& r: all_unique_regions)
    {
      اگر (!all_regions.has (r) || !مستعمل_regions.has (r))
        جاری;
      region_list.push (r);
      اگر (!region_map.سیٹ (r, idx))
        واپس جھوٹ;
      all_regions.del (r);
      idx++;
    }
    واپس (!region_list.اندر_error ()) && (!region_map.اندر_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  ساخت combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = طےشدہ;
    combined_gain_idx_tuple_t (غیرعلامتی gain, غیرعلامتی i, غیرعلامتی j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      أكد (gain < 0xFFFFFF);
      أكد (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    منطقی عامل < (ثابت combined_gain_idx_tuple_t& o)
    {
      واپس encoded < o.encoded;
    }

    منطقی عامل <= (ثابت combined_gain_idx_tuple_t& o)
    {
      واپس encoded <= o.encoded;
    }

    غیرعلامتی idx_1 () ثابت { واپس (encoded >> 20) & 0xFFFFF; };
    غیرعلامتی idx_2 () ثابت { واپس encoded & 0xFFFFF; };
  };

  منطقی as_شے_varstore (منطقی optimize=سچ, منطقی use_no_variation_idx=سچ)
  {
    /* return true if no variation data */
    اگر (!region_list) واپس سچ;
    غیرعلامتی عدد_ستون_تعداد = region_list.لمبائی;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    غیرعلامتی کل_صفیں = 0;
    برائے (غیرعلامتی major = 0; major < var_ڈیٹا_عدد_صفیں.لمبائی; major++)
      کل_صفیں += var_ڈیٹا_عدد_صفیں[major];

    اگر (!delta_صفیں.resize (کل_صفیں)) واپس جھوٹ;
    /* init all rows to [0]*num_cols */
    برائے (غیرعلامتی i = 0; i < کل_صفیں; i++)
      اگر (!(delta_صفیں[i].resize (عدد_ستون_تعداد))) واپس جھوٹ;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<غیرعلامتی, ثابت hb_سمتیہ_t<صحیح>*> front_mapping;
    غیرعلامتی آغاز_صف = 0;
    hb_سمتیہ_t<delta_صف_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح> *> major_صفیں;
    hb_hashmap_t<ثابت hb_سمتیہ_t<صحیح>*, غیرعلامتی> delta_صفیں_map;
    برائے (غیرعلامتی major = 0; major < vars.لمبائی; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ثابت tuple_variations_t& tuples = vars[major];
      غیرعلامتی عدد_صفیں = var_ڈیٹا_عدد_صفیں[major];

      اگر (!عدد_صفیں) جاری;

      برائے (ثابت tuple_delta_t& tuple: tuples.tuple_vars)
      {
        اگر (tuple.deltas_x.لمبائی != عدد_صفیں)
          واپس جھوٹ;

        /* skip unused regions */
        غیرعلامتی *ستون_idx;
        اگر (!region_map.has (&(tuple.axis_tuples), &ستون_idx))
          جاری;

        برائے (غیرعلامتی i = 0; i < عدد_صفیں; i++)
        {
          صحیح rounded_delta = hb_clamp_مقصد<صحیح> (roundf ((دوہرا) tuple.deltas_x[i]));
          صحیح &delta = delta_صفیں[آغاز_صف + i][*ستون_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          اگر (unlikely (sum < INT_MIN || sum > زیادہ_سے_زیادہ_عدد)) واپس جھوٹ;
          delta = (صحیح) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_صفیں.reset ();
      برائے (غیرعلامتی minor = 0; minor < عدد_صفیں; minor++)
      {
	ثابت hb_سمتیہ_t<صحیح>& صف = delta_صفیں[آغاز_صف + minor];
	اگر (use_no_variation_idx)
	{
	  منطقی all_zeros = سچ;
	  برائے (صحیح delta : صف)
	  {
	    اگر (delta != 0)
	    {
	      all_zeros = جھوٹ;
	      توڑو;
	    }
	  }
	  اگر (all_zeros)
	    جاری;
	}

	اگر (!front_mapping.سیٹ ((major<<16) + minor, &صف))
	  واپس جھوٹ;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	اگر (optimize)
	{
	  اگر (delta_صفیں_map.has (&صف))
	    جاری;

	  delta_صفیں_map.سیٹ (&صف, 1);
	}

	major_صفیں.push (&صف);
      }

      اگر (unlikely (major_صفیں.اندر_error () || delta_صفیں_map.اندر_error ()))
        واپس جھوٹ;
      اگر (major_صفیں)
      {
	خودکار *encoding = encoding_objs.push (std::move (major_صفیں), عدد_ستون_تعداد);
	اگر (unlikely (encoding_objs.اندر_error () || encoding->اندر_error ()))
	  واپس جھوٹ;
      }

      آغاز_صف += عدد_صفیں;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    اگر (!optimize)
    {
      encodings = std::move (encoding_objs);
      واپس !encodings.اندر_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.qsort ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    استعمال شے_t = hb_ترجیح_قطار_t<combined_gain_idx_tuple_t>::شے_t;
    hb_سمتیہ_t<شے_t> قطار_اشیاء;
    غیرعلامتی عدد_todos = encoding_objs.لمبائی;
    برائے (غیرعلامتی i = 0; i < عدد_todos; i++)
    {
      برائے (غیرعلامتی j = i + 1; j < عدد_todos; j++)
      {
        صحیح combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        اگر (combining_gain > 0)
          قطار_اشیاء.push (شے_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_ترجیح_قطار_t<combined_gain_idx_tuple_t> قطار (std::move (قطار_اشیاء));

    hb_ثنائی_ہندسہ_سیٹ_t removed_todo_idxes;
    جب (قطار)
    {
      خودکار t = قطار.نکالو_minimum ().پہلا;
      غیرعلامتی i = t.idx_1 ();
      غیرعلامتی j = t.idx_2 ();

      اگر (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        جاری;

      delta_صف_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_صف_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.جوڑو (i);
      removed_todo_idxes.جوڑو (j);
      اگر (unlikely (removed_todo_idxes.اندر_error ())) واپس جھوٹ;

      encoding.ملاؤ (other_encoding);
      اگر (unlikely (encoding.اندر_error ())) واپس جھوٹ;

      برائے (غیرعلامتی idx = 0; idx < encoding_objs.لمبائی; idx++)
      {
        اگر (removed_todo_idxes.has (idx)) جاری;

        ثابت delta_صف_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        اگر (obj.چوڑائی == encoding.چوڑائی && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          برائے (ثابت خودکار& صف : obj.اشیاء)
            encoding.جوڑو_صف (صف);
          اگر (unlikely (encoding.اندر_error ())) واپس جھوٹ;

          removed_todo_idxes.جوڑو (idx);
          اگر (unlikely (removed_todo_idxes.اندر_error ())) واپس جھوٹ;
          جاری;
        }

        صحیح combined_gain = encoding.gain_from_merging (obj);
        اگر (combined_gain > 0)
          قطار.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.لمبائی), 0);
      }

      خودکار moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      اگر (unlikely (encoding_objs.اندر_error ())) واپس جھوٹ;
    }

    صحیح عدد_final_encodings = (صحیح) encoding_objs.لمبائی - (صحیح) removed_todo_idxes.get_population ();
    اگر (عدد_final_encodings <= 0) واپس جھوٹ;

    اگر (!encodings.alloc (عدد_final_encodings)) واپس جھوٹ;
    برائے (غیرعلامتی i = 0; i < encoding_objs.لمبائی; i++)
    {
      اگر (removed_todo_idxes.has (i)) جاری;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    واپس compile_varidx_map (front_mapping);
  }

  نجی:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  منطقی compile_varidx_map (ثابت hb_hashmap_t<غیرعلامتی, ثابت hb_سمتیہ_t<صحیح>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ثابت hb_سمتیہ_t<صحیح>*, غیرعلامتی> عقب_mapping;
    hb_سمتیہ_t<delta_صف_encoding_t> بانٹو_encodings;

    برائے (غیرعلامتی i = 0; i < encodings.لمبائی; i++)
    {
      delta_صف_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      اگر (encoding.is_empty () || encoding.اندر_error ())
        واپس جھوٹ;

      غیرعلامتی عدد_صفیں = encoding.اشیاء.لمبائی;
      غیرعلامتی عدد_ستون_تعداد = encoding.chars.لمبائی;

      /* sort rows, make result deterministic */
      encoding.اشیاء.qsort (cmp_صف);

      برائے (غیرعلامتی آغاز = 0; آغاز < عدد_صفیں; آغاز += 0xFFFFu)
      {
        غیرعلامتی chunk_len = hb_min (عدد_صفیں - آغاز, 0xFFFFu);
        hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح>*> صفیں;

        اگر (!صفیں.alloc (chunk_len))
          واپس جھوٹ;

        غیرعلامتی major = بانٹو_encodings.لمبائی;
        برائے (غیرعلامتی minor = 0; minor < chunk_len; minor++)
        {
          ثابت hb_سمتیہ_t<صحیح> *صف = encoding.اشیاء.arrayZ[آغاز + minor];
          صفیں.push (صف);
          اگر (!عقب_mapping.سیٹ (صف, (major << 16) + minor))
            واپس جھوٹ;
        }

        خودکار *بانٹو = بانٹو_encodings.push (std::move (صفیں), عدد_ستون_تعداد);
        اگر (unlikely (بانٹو_encodings.اندر_error () || بانٹو->اندر_error ()))
          واپس جھوٹ;
      }
    }

    encodings = std::move (بانٹو_encodings);
    اگر (encodings.اندر_error () || عقب_mapping.اندر_error ())
      واپس جھوٹ;

    برائے (خودکار _ : front_mapping.iter ())
    {
      غیرعلامتی old_varidx = _.پہلا;
      غیرعلامتی *new_varidx;
      اگر (عقب_mapping.has (_.second, &new_varidx))
        varidx_map.سیٹ (old_varidx, *new_varidx);
      ورنہ
        varidx_map.سیٹ (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ);
    }
    واپس !varidx_map.اندر_error ();
  }

  ساکن صحیح cmp_صف (ثابت خالی *pa, ثابت خالی *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ثابت hb_سمتیہ_t<صحیح>** a = (ثابت hb_سمتیہ_t<صحیح>**) pa;
    ثابت hb_سمتیہ_t<صحیح>** b = (ثابت hb_سمتیہ_t<صحیح>**) pb;

    برائے (غیرعلامتی i = 0; i < (*b)->لمبائی; i++)
    {
      صحیح va = (*a)->arrayZ[i];
      صحیح vb = (*b)->arrayZ[i];
      اگر (va != vb)
        واپس va < vb ? -1 : 1;
    }
    واپس 0;
  }
};


} /* namespace OT */


#اگر_ختم /* HB_OT_VAR_COMMON_HH */

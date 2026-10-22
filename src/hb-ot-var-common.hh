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

#جے_بنا_تعریف HB_OT_VAR_COMMON_HH
#تعریف_کرو HB_OT_VAR_COMMON_HH

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-alloc-pool.hh"
#ضمن "hb-priority-queue.hh"
#ضمن "hb-subset-instancer-iup.hh"


نطاق OT {

ورتوں rebase_tent_نتیجہ_scratch_t = hb_pair_t<rebase_tent_نتیجہ_t, rebase_tent_نتیجہ_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
بنية Tuple_Variation_سرخی
{
  یار بنية tuple_delta_t;
  size_t get_ناپ (بنانشانی axis_گنتی_times_2) ثابت
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    بنانشانی ti = tuple_اشاریہ;
    إذا (unlikely ((ti & (Tuple_اشاریہ::EmbeddedPeakTuple | Tuple_اشاریہ::IntermediateRegion))))
    {
      بنانشانی گنتی = ((ti & Tuple_اشاریہ::EmbeddedPeakTuple) != 0) + ((ti & Tuple_اشاریہ::IntermediateRegion) != 0) * 2;
      أرجع hb_unsigned_mul_جوڑو_saturate (گنتی, axis_گنتی_times_2, min_ناپ);
    }
    أرجع min_ناپ;
  }

  بنانشانی get_انکڑے_ناپ () ثابت { أرجع var_انکڑے_ناپ; }

  ثابت Tuple_Variation_سرخی &get_اگلا (بنانشانی axis_گنتی_times_2) ثابت
  { أرجع Struct_At_وتھ<Tuple_Variation_سرخی> (هذا, get_ناپ (axis_گنتی_times_2)); }

  منطقي unpack_axis_tuples (بنانشانی axis_گنتی,
                           ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                           ثابت hb_map_t *axes_old_اشاریہ_ٹیگ_map,
                           hb_hashmap_t<hb_ٹیگ_t, Triple>& axis_tuples /*باہرلا*/) ثابت
  {
    ثابت F2DOT14 *peak_tuple = خالی_سوچک;
    إذا (has_peak ())
      peak_tuple = get_peak_tuple (axis_گنتی);
    وإلا
    {
      بنانشانی صحيح اشاریہ_2 = get_اشاریہ ();
      إذا (unlikely ((اشاریہ_2 + 1) * axis_گنتی > shared_tuples.لمائی))
        أرجع خطأ;
      peak_tuple = shared_tuples.sub_array (axis_گنتی * اشاریہ_2, axis_گنتی).arrayZ;
    }

    ثابت F2DOT14 *شروع_tuple = خالی_سوچک;
    ثابت F2DOT14 *سرا_tuple = خالی_سوچک;
    منطقي has_interm = has_intermediate ();

    إذا (has_interm)
    {
      شروع_tuple = get_شروع_tuple (axis_گنتی);
      سرا_tuple = get_سرا_tuple (axis_گنتی);
    }

    لكل (بنانشانی i = 0; i < axis_گنتی; i++)
    {
      عائم peak = peak_tuple[i].منزل_float ();
      إذا (peak == 0.f) استمر;

      hb_ٹیگ_t *axis_ٹیگ_2;
      إذا (!axes_old_اشاریہ_ٹیگ_map->has (i, &axis_ٹیگ_2))
        أرجع خطأ;

      عائم شروع, سرا;
      إذا (has_interm)
      {
        شروع = شروع_tuple[i].منزل_float ();
        سرا = سرا_tuple[i].منزل_float ();
      }
      وإلا
      {
        شروع = hb_min (peak, 0.f);
        سرا = hb_max (peak, 0.f);
      }
      axis_tuples.سیٹ (*axis_ٹیگ_2, Triple (شروع, peak, سرا));
    }

    أرجع سچ;
  }

  HB_ALWAYS_INLINE
  عائم calculate_scalar (hb_array_t<ثابت صحيح> coords, بنانشانی صحيح coord_گنتی,
			   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ثابت
  {
    بنانشانی tuple_اشاریہ_2 = tuple_اشاریہ;
    بنانشانی صحيح اشاریہ_2 = tuple_اشاریہ_2 & Tuple_اشاریہ::Tuple_اشاریہ_نقاب;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    منطقي plain_shared_tuple = (tuple_اشاریہ_2 & ~Tuple_اشاریہ::نجی_نقطہ_Numbers) < shared_tuple_scalar_cache->لمائی;
    إذا (likely (plain_shared_tuple))
    {
      عائم scalar;
      إذا (likely (shared_tuple_scalar_cache->get (اشاریہ_2, &scalar)))
	أرجع scalar;
    }

    ثابت F2DOT14 *peak_tuple;

    منطقي has_interm = tuple_اشاریہ_2 & Tuple_اشاریہ::IntermediateRegion; // Inlined for performance

    إذا (unlikely (tuple_اشاریہ_2 & Tuple_اشاریہ::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_گنتی);
      shared_tuple_scalar_cache = خالی_سوچک;
    }
    وإلا
    {
      عائم scalar;
      إذا (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->لمائی &&
	  shared_tuple_scalar_cache->get (اشاریہ_2, &scalar))
      {
        إذا (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = خالی_سوچک;
	وإلا
	  أرجع scalar;
      }

      إذا (unlikely ((اشاریہ_2 + 1) * coord_گنتی > shared_tuples.لمائی))
        أرجع 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_گنتی * اشاریہ_2);

    }

    ثابت F2DOT14 *شروع_tuple = خالی_سوچک;
    ثابت F2DOT14 *سرا_tuple = خالی_سوچک;

    إذا (has_interm)
    {
      شروع_tuple = get_شروع_tuple (coord_گنتی);
      سرا_tuple = get_سرا_tuple (coord_گنتی);
    }

    عائم scalar = 1.f;
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
#إذا HB_FAST_عدد_ACCESS
    منطقي skip = coord_گنتی >= 16;
#جے_انت
#جے_انت
    لكل (بنانشانی صحيح i = 0; i < coord_گنتی; i++)
    {
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
#إذا HB_FAST_عدد_ACCESS
      إذا (skip)
      {
	طالما (i + 4 <= coord_گنتی && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	طالما (i < coord_گنتی && peak_tuple[i].منزل_int () == 0)
	  i += 1;
	إذا (i >= coord_گنتی)
	  اكسر;
      }
#جے_انت
#جے_انت

      صحيح peak = peak_tuple[i].منزل_int ();
      إذا (!peak) استمر;

      صحيح v = coords[i];
      إذا (!v) { scalar = 0.f; اكسر; }
      إذا (v == peak) استمر;

      إذا (has_interm)
      {
	shared_tuple_scalar_cache = خالی_سوچک;
        صحيح شروع = شروع_tuple[i].منزل_int ();
        صحيح سرا = سرا_tuple[i].منزل_int ();
        إذا (unlikely (شروع > peak || peak > سرا ||
                      (شروع < 0 && سرا > 0 && peak))) استمر;
        إذا (v < شروع || v > سرا) { scalar = 0.f; اكسر; }
        إذا (v < peak)
        { إذا (peak != شروع) scalar *= (عائم) (v - شروع) / (peak - شروع); }
        وإلا
        { إذا (peak != سرا) scalar *= (عائم) (سرا - v) / (سرا - peak); }
      }
      وإلا إذا (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; اكسر; }
      وإلا
        scalar *= (عائم) v / peak;
    }
    إذا (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->سیٹ (اشاریہ_2, scalar);
    أرجع scalar;
  }

  منطقي           has_peak () ثابت { أرجع tuple_اشاریہ & Tuple_اشاریہ::EmbeddedPeakTuple; }
  منطقي   has_intermediate () ثابت { أرجع tuple_اشاریہ & Tuple_اشاریہ::IntermediateRegion; }
  منطقي has_نجی_points () ثابت { أرجع tuple_اشاریہ & Tuple_اشاریہ::نجی_نقطہ_Numbers; }
  بنانشانی      get_اشاریہ () ثابت { أرجع tuple_اشاریہ & Tuple_اشاریہ::Tuple_اشاریہ_نقاب; }
  ساكن constexpr بنانشانی max_shared_tuple_گنتی = 0x1000u;

  محمي:
  بنية Tuple_اشاریہ : HBUINT16
  {
    تعداد Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      نجی_نقطہ_Numbers = 0x2000u,
      Tuple_اشاریہ_نقاب      = 0x0FFFu
    };

    Tuple_اشاریہ& عامل = (uint16_t i) { HBUINT16::عامل= (i); أرجع *هذا; }
    DEFINE_ناپ_STATIC (2);
  };

  hb_array_t<ثابت F2DOT14> get_all_tuples (بنانشانی axis_گنتی) ثابت
  { أرجع StructAfter<Unsized_Array_دا<F2DOT14>> (tuple_اشاریہ).as_array ((has_peak () + has_intermediate () * 2) * axis_گنتی); }
  ثابت F2DOT14* get_all_tuples_ادھار (بنانشانی axis_گنتی) ثابت
  { أرجع StructAfter<Unsized_Array_دا<F2DOT14>> (tuple_اشاریہ).arrayZ; }
  ثابت F2DOT14* get_peak_tuple (بنانشانی axis_گنتی) ثابت
  { أرجع get_all_tuples_ادھار (axis_گنتی); }
  ثابت F2DOT14* get_شروع_tuple (بنانشانی axis_گنتی) ثابت
  { أرجع get_all_tuples_ادھار (axis_گنتی) + has_peak () * axis_گنتی; }
  ثابت F2DOT14* get_سرا_tuple (بنانشانی axis_گنتی) ثابت
  { أرجع get_all_tuples_ادھار (axis_گنتی) + has_peak () * axis_گنتی + axis_گنتی; }

  HBUINT16      var_انکڑے_ناپ;    /* The size in bytes of the serialized
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
  عام:
  DEFINE_ناپ_MIN (4);
};

بنية optimize_scratch_t
{
  iup_scratch_t iup;
  hb_سمتیہ_t<منطقي> opt_indices;
  hb_سمتیہ_t<صحيح> rounded_x_deltas;
  hb_سمتیہ_t<صحيح> rounded_y_deltas;
  hb_سمتیہ_t<عائم> opt_deltas_x;
  hb_سمتیہ_t<عائم> opt_deltas_y;
  hb_سمتیہ_t<بنانشانی حرف> opt_نقطہ_انکڑے;
  hb_سمتیہ_t<بنانشانی حرف> opt_deltas_انکڑے;
  hb_سمتیہ_t<بنانشانی حرف> نقطہ_انکڑے;
  hb_سمتیہ_t<بنانشانی حرف> deltas_انکڑے;
  hb_سمتیہ_t<صحيح> rounded_deltas;
};

بنية tuple_delta_t
{
  ساكن constexpr منطقي realloc_move = سچ;  // Watch out when adding new members!

  عام:
  hb_hashmap_t<hb_ٹیگ_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_سمتیہ_t<منطقي> indices;

  hb_سمتیہ_t<عائم> deltas_x;
  /* empty for cvar tuples */
  hb_سمتیہ_t<عائم> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_سمتیہ_t<بنانشانی حرف> compiled_tuple_سرخی;
  hb_سمتیہ_t<بنانشانی حرف> compiled_deltas;

  hb_سمتیہ_t<F2DOT14> compiled_peak_coords;
  hb_سمتیہ_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = خالی_سوچک) {}
  tuple_delta_t (ثابت tuple_delta_t& o) = افتراضي;
  tuple_delta_t& عامل = (ثابت tuple_delta_t& o) = افتراضي;

  یار فراغ swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
  {
    hb_وٹاندرا (a.axis_tuples, b.axis_tuples);
    hb_وٹاندرا (a.indices, b.indices);
    hb_وٹاندرا (a.deltas_x, b.deltas_x);
    hb_وٹاندرا (a.deltas_y, b.deltas_y);
    hb_وٹاندرا (a.compiled_tuple_سرخی, b.compiled_tuple_سرخی);
    hb_وٹاندرا (a.compiled_deltas, b.compiled_deltas);
    hb_وٹاندرا (a.compiled_peak_coords, b.compiled_peak_coords);
  }

  tuple_delta_t (tuple_delta_t&& o)  noexcept : tuple_delta_t ()
  { hb_وٹاندرا (*هذا, o); }

  tuple_delta_t& عامل = (tuple_delta_t&& o) noexcept
  {
    hb_وٹاندرا (*هذا, o);
    أرجع *هذا;
  }

  فراغ copy_from (ثابت tuple_delta_t& o, hb_alloc_pool_t *pool = خالی_سوچک)
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

  فراغ remove_axis (hb_ٹیگ_t axis_ٹیگ_2)
  { axis_tuples.del (axis_ٹیگ_2); }

  منطقي سیٹ_tent (hb_ٹیگ_t axis_ٹیگ_2, Triple tent)
  { أرجع axis_tuples.سیٹ (axis_ٹیگ_2, tent); }

  tuple_delta_t& عامل += (ثابت tuple_delta_t& o)
  {
    بنانشانی عدد = indices.لمائی;
    لكل (بنانشانی i = 0; i < عدد; i++)
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
        indices.arrayZ[i] = سچ;
        deltas_x[i] = o.deltas_x[i];
        إذا (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    أرجع *هذا;
  }

  tuple_delta_t& عامل *= (عائم scalar)
  {
    إذا (scalar == 1.0f)
      أرجع *هذا;

    بنانشانی عدد = indices.لمائی;
    إذا (deltas_y)
      لكل (بنانشانی i = 0; i < عدد; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    وإلا
      لكل (بنانشانی i = 0; i < عدد; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
      }
    أرجع *هذا;
  }

  فراغ change_tuple_var_axis_حد_بندی (hb_ٹیگ_t axis_ٹیگ_2, Triple axis_حد_بندی,
				    TripleDistances axis_triple_distances,
				    hb_سمتیہ_t<tuple_delta_t>& باہرلا,
				    rebase_tent_نتیجہ_scratch_t &scratch,
				    hb_alloc_pool_t *pool = خالی_سوچک)
  {
    // May move *this out.

    باہرلا.reset ();
    Triple *tent;
    إذا (!axis_tuples.has (axis_ٹیگ_2, &tent))
    {
      باہرلا.push (std::move (*هذا));
      أرجع;
    }

    إذا ((tent->minimum < 0.f && tent->ودھ_توں_ودھ > 0.f) ||
        !(tent->minimum <= tent->درمیان && tent->درمیان <= tent->ودھ_توں_ودھ))
      أرجع;

    إذا (tent->درمیان == 0.f)
    {
      باہرلا.push (std::move (*هذا));
      أرجع;
    }

    إذا (!axis_حد_بندی.is_نقطہ () &&
        !(-1.f <= axis_حد_بندی.minimum &&
          axis_حد_بندی.minimum <= axis_حد_بندی.درمیان &&
          axis_حد_بندی.درمیان <= axis_حد_بندی.ودھ_توں_ودھ &&
          axis_حد_بندی.ودھ_توں_ودھ <= +1.f))
      أرجع;

    rebase_tent_نتیجہ_t &حل = scratch.پہلا;
    rebase_tent (*tent, axis_حد_بندی, axis_triple_distances, حل, scratch.second);
    لكل (بنانشانی i = 0; i < حل.لمائی; i++)
    {
      خودکار &t = حل.arrayZ[i];

      tuple_delta_t new_var;
      إذا (i < حل.لمائی - 1)
	new_var.copy_from (*هذا, pool);
      وإلا
	new_var = std::move (*هذا);

      إذا (t.second == Triple ())
        new_var.remove_axis (axis_ٹیگ_2);
      وإلا
        new_var.سیٹ_tent (axis_ٹیگ_2, t.second);

      new_var *= t.پہلا;
      باہرلا.push (std::move (new_var));
    }
  }

  منطقي compile_coords (ثابت hb_map_t& axes_اشاریہ_map,
		       ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map,
		       hb_alloc_pool_t *pool= خالی_سوچک)
  {
    بنانشانی cur_axis_گنتی = axes_اشاریہ_map.get_population ();
    إذا (pool)
    {
      إذا (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_گنتی)))
	أرجع خطأ;
    }
    وإلا إذا (unlikely (!compiled_peak_coords.resize (cur_axis_گنتی)))
      أرجع خطأ;

    hb_array_t<F2DOT14> شروع_coords, سرا_coords;

    بنانشانی orig_axis_گنتی = axes_old_اشاریہ_ٹیگ_map.get_population ();
    بنانشانی j = 0;
    لكل (بنانشانی i = 0; i < orig_axis_گنتی; i++)
    {
      إذا (!axes_اشاریہ_map.has (i))
        استمر;

      hb_ٹیگ_t axis_ٹیگ_2 = axes_old_اشاریہ_ٹیگ_map.get (i);
      Triple *coords = خالی_سوچک;
      إذا (axis_tuples.has (axis_ٹیگ_2, &coords))
      {
	عائم min_val = coords->minimum;
	عائم val = coords->درمیان;
	عائم max_val = coords->ودھ_توں_ودھ;

	compiled_peak_coords.arrayZ[j].سیٹ_float (val);

	إذا (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  إذا (!compiled_interm_coords)
	  {
	    إذا (pool)
	    {
	      إذا (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_گنتی)))
		أرجع خطأ;
	    }
	    وإلا إذا (unlikely (!compiled_interm_coords.resize (2 * cur_axis_گنتی)))
	      أرجع خطأ;
	    شروع_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_گنتی);
	    سرا_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_گنتی);

	    لكل (بنانشانی k = 0; k < j; k++)
	    {
	      نشانیوالا peak = compiled_peak_coords.arrayZ[k].منزل_int ();
	      إذا (!peak) استمر;
	      شروع_coords.arrayZ[k].سیٹ_int (hb_min (peak, 0));
	      سرا_coords.arrayZ[k].سیٹ_int (hb_max (peak, 0));
	    }
	  }

	}

	إذا (compiled_interm_coords)
	{
	  شروع_coords.arrayZ[j].سیٹ_float (min_val);
	  سرا_coords.arrayZ[j].سیٹ_float (max_val);
	}
      }

      j++;
    }

    أرجع !compiled_peak_coords.وچ_error () && !compiled_interm_coords.وچ_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  منطقي compile_tuple_var_سرخی (ثابت hb_map_t& axes_اشاریہ_map,
                                 بنانشانی points_انکڑے_لمائی,
                                 ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map,
                                 ثابت hb_hashmap_t<ثابت hb_سمتیہ_t<F2DOT14>*, بنانشانی>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = خالی_سوچک)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    إذا (!compiled_deltas) أرجع سچ;

    بنانشانی cur_axis_گنتی = axes_اشاریہ_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    بنانشانی alloc_len = 3 * cur_axis_گنتی * (F2DOT14::static_ناپ) + 4;
    إذا (unlikely (!compiled_tuple_سرخی.allocate_from_pool (pool, alloc_len, خطأ))) أرجع خطأ;

    بنانشانی flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرخی.begin () + 4);
    F2DOT14* سرا = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرخی.سرا ());
    hb_array_t<F2DOT14> coords (p, سرا - p);

    إذا (!shared_tuples_idx_map)
      compile_coords (axes_اشاریہ_map, axes_old_اشاریہ_ٹیگ_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    بنانشانی peak_گنتی = 0;
    بنانشانی *shared_tuple_idx;
    إذا (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    وإلا
    {
      peak_گنتی = encode_peak_coords(coords, flag);
      إذا (!peak_گنتی) أرجع خطأ;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    بنانشانی interim_گنتی = encode_interm_coords (coords.sub_array (peak_گنتی), flag);

    /* pointdata length = 0 implies "use shared points" */
    إذا (points_انکڑے_لمائی)
      flag |= Tuple_Variation_سرخی::Tuple_اشاریہ::نجی_نقطہ_Numbers;

    بنانشانی serialized_انکڑے_ناپ = points_انکڑے_لمائی + compiled_deltas.لمائی;
    Tuple_Variation_سرخی *o = reinterpret_cast<Tuple_Variation_سرخی *> (compiled_tuple_سرخی.begin ());
    o->var_انکڑے_ناپ = serialized_انکڑے_ناپ;
    o->tuple_اشاریہ = flag;

    بنانشانی کل_سرخی_len = 4 + (peak_گنتی + interim_گنتی) * (F2DOT14::static_ناپ);
    compiled_tuple_سرخی.shrink_پٹھ_منزل_pool (pool, کل_سرخی_len);
    أرجع سچ;
  }

  بنانشانی encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               بنانشانی& flag) ثابت
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.لمائی * حجم (compiled_peak_coords[0]));
    flag |= Tuple_Variation_سرخی::Tuple_اشاریہ::EmbeddedPeakTuple;
    أرجع compiled_peak_coords.لمائی;
  }

  /* if no need to encode intermediate coords, then just return p */
  بنانشانی encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 بنانشانی& flag) ثابت
  {
    إذا (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.لمائی * حجم (compiled_interm_coords[0]));
      flag |= Tuple_Variation_سرخی::Tuple_اشاریہ::IntermediateRegion;
    }
    أرجع compiled_interm_coords.لمائی;
  }

  منطقي compile_deltas (hb_سمتیہ_t<صحيح> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = خالی_سوچک)
  { أرجع compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ساكن منطقي compile_deltas (hb_array_t<ثابت منطقي> نقطہ_indices,
			      hb_array_t<ثابت عائم> x_deltas,
			      hb_array_t<ثابت عائم> y_deltas,
			      hb_سمتیہ_t<بنانشانی حرف> &compiled_deltas, /*باہرلا*/
			      hb_سمتیہ_t<صحيح> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = خالی_سوچک)
  {
    إذا (unlikely (!rounded_deltas.resize_dirty  (نقطہ_indices.لمائی)))
      أرجع خطأ;

    بنانشانی j = 0;
    لكل (بنانشانی i = 0; i < نقطہ_indices.لمائی; i++)
    {
      إذا (!نقطہ_indices[i]) استمر;
      rounded_deltas.arrayZ[j++] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    إذا (!rounded_deltas) أرجع سچ;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    بنانشانی alloc_len = 2 + 4 * rounded_deltas.لمائی + (rounded_deltas.لمائی + 63) / 64;
    إذا (y_deltas)
      alloc_len *= 2;

    إذا (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, خطأ))) أرجع خطأ;

    بنانشانی encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    إذا (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      بنانشانی j = 0;
      لكل (بنانشانی idx = 0; idx < نقطہ_indices.لمائی; idx++)
      {
        إذا (!نقطہ_indices[idx]) استمر;
        صحيح rounded_delta = hb_clamp_منزل<صحيح> (roundf ((مزدوج) y_deltas.arrayZ[idx]));

        إذا (j >= rounded_deltas.لمائی) أرجع خطأ;

        rounded_deltas[j++] = rounded_delta;
      }

      إذا (j != rounded_deltas.لمائی) أرجع خطأ;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_پٹھ_منزل_pool (pool, encoded_len);
    أرجع سچ;
  }

  ساكن بنانشانی compile_deltas (hb_array_t<بنانشانی حرف> encoded_بائٹاں,
				  hb_array_t<ثابت صحيح> deltas)
  {
    أرجع Tuple_اقدار::compile_unsafe (deltas, encoded_بائٹاں);
  }

  منطقي calc_inferred_deltas (ثابت contour_نقطہ_سمتیہ_t& orig_points,
			     hb_سمتیہ_t<بنانشانی> &scratch)
  {
    بنانشانی نقطہ_گنتی = orig_points.لمائی;
    إذا (نقطہ_گنتی != indices.لمائی)
      أرجع خطأ;

    بنانشانی ref_گنتی_2 = 0;

    hb_سمتیہ_t<بنانشانی> &سرا_points = scratch.reset ();

    لكل (بنانشانی i = 0; i < نقطہ_گنتی; i++)
    {
      ref_گنتی_2 += indices.arrayZ[i];
      إذا (orig_points.arrayZ[i].is_سرا_نقطہ)
        سرا_points.push (i);
    }
    /* all points are referenced, nothing to do */
    إذا (ref_گنتی_2 == نقطہ_گنتی)
      أرجع سچ;
    إذا (unlikely (سرا_points.وچ_error ())) أرجع خطأ;

    hb_ثنائی_ہندسہ_سیٹ_t inferred_idxes;
    بنانشانی شروع_نقطہ_2 = 0;
    لكل (بنانشانی سرا_نقطہ : سرا_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      بنانشانی unref_گنتی = 0;
      لكل (بنانشانی i = شروع_نقطہ_2; i < سرا_نقطہ + 1; i++)
        unref_گنتی += indices.arrayZ[i];
      unref_گنتی = (سرا_نقطہ - شروع_نقطہ_2 + 1) - unref_گنتی;

      بنانشانی j = شروع_نقطہ_2;
      إذا (unref_گنتی == 0 || unref_گنتی > سرا_نقطہ - شروع_نقطہ_2)
        اذهب no_ہور_gaps;
      لكل (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        بنانشانی صحيح prev, next, i;
        لكل (;;)
        {
          i = j;
          j = اگلا_اشاریہ (i, شروع_نقطہ_2, سرا_نقطہ);
          إذا (indices.arrayZ[i] && !indices.arrayZ[j]) اكسر;
        }
        prev = j = i;
        لكل (;;)
        {
          i = j;
          j = اگلا_اشاریہ (i, شروع_نقطہ_2, سرا_نقطہ);
          إذا (!indices.arrayZ[i] && indices.arrayZ[j]) اكسر;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        لكل (;;)
        {
          i = اگلا_اشاریہ (i, شروع_نقطہ_2, سرا_نقطہ);
          إذا (i == next) اكسر;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.جوڑو (i);
          إذا (--unref_گنتی == 0) اذهب no_ہور_gaps;
        }
      }
    no_ہور_gaps:
      شروع_نقطہ_2 = سرا_نقطہ + 1;
    }

    لكل (بنانشانی i = 0; i < نقطہ_گنتی; i++)
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
        indices[i] = سچ;
      }
    }
    أرجع سچ;
  }

  منطقي optimize (ثابت contour_نقطہ_سمتیہ_t& contour_points,
                 منطقي is_مرکب,
		 optimize_scratch_t &scratch,
                 مزدوج tolerance = 0.5 + 1e-10)
  {
    بنانشانی گنتی = contour_points.لمائی;
    إذا (deltas_x.لمائی != گنتی ||
        deltas_y.لمائی != گنتی)
      أرجع خطأ;

    hb_سمتیہ_t<منطقي> &opt_indices = scratch.opt_indices.reset ();
    hb_سمتیہ_t<صحيح> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_سمتیہ_t<صحيح> &rounded_y_deltas = scratch.rounded_y_deltas;

    إذا (unlikely (!rounded_x_deltas.resize_dirty  (گنتی) ||
                  !rounded_y_deltas.resize_dirty  (گنتی)))
      أرجع خطأ;

    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) deltas_y.arrayZ[i]));
    }

    إذا (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      أرجع خطأ;

    بنانشانی ref_گنتی_2 = 0;
    لكل (منطقي ref_flag : opt_indices)
       ref_گنتی_2 += ref_flag;

    إذا (ref_گنتی_2 == گنتی) أرجع سچ;

    hb_سمتیہ_t<عائم> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_سمتیہ_t<عائم> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    منطقي is_comp_glyph_wo_deltas = (is_مرکب && ref_گنتی_2 == 0);
    إذا (is_comp_glyph_wo_deltas)
    {
      إذا (unlikely (!opt_deltas_x.resize (گنتی) ||
                    !opt_deltas_y.resize (گنتی)))
        أرجع خطأ;

      opt_indices.arrayZ[0] = سچ;
      لكل (بنانشانی i = 1; i < گنتی; i++)
        opt_indices.arrayZ[i] = خطأ;
    }

    hb_سمتیہ_t<بنانشانی حرف> &opt_نقطہ_انکڑے = scratch.opt_نقطہ_انکڑے.reset ();
    إذا (!compile_نقطہ_سیٹ (opt_indices, opt_نقطہ_انکڑے))
      أرجع خطأ;
    hb_سمتیہ_t<بنانشانی حرف> &opt_deltas_انکڑے = scratch.opt_deltas_انکڑے.reset ();
    إذا (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_انکڑے,
			 scratch.rounded_deltas))
      أرجع خطأ;

    hb_سمتیہ_t<بنانشانی حرف> &نقطہ_انکڑے = scratch.نقطہ_انکڑے.reset ();
    إذا (!compile_نقطہ_سیٹ (indices, نقطہ_انکڑے))
      أرجع خطأ;
    hb_سمتیہ_t<بنانشانی حرف> &deltas_انکڑے = scratch.deltas_انکڑے.reset ();
    إذا (!compile_deltas (indices, deltas_x, deltas_y, deltas_انکڑے, scratch.rounded_deltas))
      أرجع خطأ;

    إذا (opt_نقطہ_انکڑے.لمائی + opt_deltas_انکڑے.لمائی < نقطہ_انکڑے.لمائی + deltas_انکڑے.لمائی)
    {
      indices = std::move (opt_indices);

      إذا (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    أرجع !indices.وچ_error () && !deltas_x.وچ_error () && !deltas_y.وچ_error ();
  }

  ساكن منطقي compile_نقطہ_سیٹ (ثابت hb_سمتیہ_t<منطقي> &نقطہ_indices,
                                 hb_سمتیہ_t<بنانشانی حرف>& compiled_points /*باہرلا*/)
  {
    بنانشانی عدد_points = 0;
    لكل (منطقي i : نقطہ_indices)
      إذا (i) عدد_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    إذا (!عدد_points) أرجع سچ;

    بنانشانی indices_لمائی = نقطہ_indices.لمائی;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    إذا (عدد_points == indices_لمائی)
      أرجع compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    بنانشانی عدد_بائٹاں = 2 + 3 *عدد_points;
    إذا (unlikely (!compiled_points.resize_dirty  (عدد_بائٹاں)))
      أرجع خطأ;

    بنانشانی pos = 0;
    /* binary data starts with the total number of reference points */
    إذا (عدد_points < 0x80)
      compiled_points.arrayZ[pos++] = عدد_points;
    وإلا
    {
      compiled_points.arrayZ[pos++] = ((عدد_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = عدد_points & 0xFF;
    }

    ثابت بنانشانی max_لگاتار_لمائی = 0x7F;
    بنانشانی i = 0;
    بنانشانی آخری_قدر = 0;
    بنانشانی عدد_encoded = 0;
    طالما (i < indices_لمائی && عدد_encoded < عدد_points)
    {
      بنانشانی لگاتار_لمائی = 0;
      بنانشانی سرخی_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      منطقي use_byte_encoding = خطأ;
      منطقي new_لگاتار_2 = سچ;
      طالما (i < indices_لمائی && عدد_encoded < عدد_points &&
             لگاتار_لمائی <= max_لگاتار_لمائی)
      {
        // find out next referenced point index
        طالما (i < indices_لمائی && !نقطہ_indices[i])
          i++;

        إذا (i >= indices_لمائی) اكسر;

        بنانشانی cur_قدر = i;
        بنانشانی delta = cur_قدر - آخری_قدر;

        إذا (new_لگاتار_2)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_لگاتار_2 = خطأ;
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
        آخری_قدر = cur_قدر;
        لگاتار_لمائی++;
        عدد_encoded++;
      }

      إذا (use_byte_encoding)
        compiled_points.arrayZ[سرخی_pos] = لگاتار_لمائی - 1;
      وإلا
        compiled_points.arrayZ[سرخی_pos] = (لگاتار_لمائی - 1) | 0x80;
    }
    أرجع compiled_points.resize_dirty  (pos);
  }

  ساكن عائم infer_delta (عائم نشانہ_val, عائم prev_val, عائم اگلا_val, عائم prev_delta, عائم اگلا_delta)
  {
    إذا (prev_val == اگلا_val)
      أرجع (prev_delta == اگلا_delta) ? prev_delta : 0.f;
    وإلا إذا (نشانہ_val <= hb_min (prev_val, اگلا_val))
      أرجع (prev_val < اگلا_val) ? prev_delta : اگلا_delta;
    وإلا إذا (نشانہ_val >= hb_max (prev_val, اگلا_val))
      أرجع (prev_val > اگلا_val) ? prev_delta : اگلا_delta;

    عائم r = (نشانہ_val - prev_val) / (اگلا_val - prev_val);
    أرجع prev_delta + r * (اگلا_delta - prev_delta);
  }

  ساكن بنانشانی صحيح اگلا_اشاریہ (بنانشانی صحيح i, بنانشانی صحيح شروع, بنانشانی صحيح سرا)
  { أرجع (i >= سرا) ? شروع : (i + 1); }
};

سانچا <قسم_ناں OffType = HBUINT16>
بنية Tuple_Variation_انکڑے
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (هذا));
  }

  size_t get_ناپ (بنانشانی axis_گنتی_times_2) ثابت
  {
    size_t کل_ناپ = min_ناپ;
    بنانشانی گنتی = tuple_Var_گنتی.get_گنتی ();
    ثابت Tuple_Variation_سرخی *tuple_var_سرخی = &(get_tuple_var_سرخی());
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      کل_ناپ = hb_unsigned_جوڑو_saturate (کل_ناپ,
					     tuple_var_سرخی->get_ناپ (axis_گنتی_times_2),
					     (size_t) tuple_var_سرخی->get_انکڑے_ناپ ());
      tuple_var_سرخی = &tuple_var_سرخی->get_اگلا (axis_گنتی_times_2);
    }

    أرجع کل_ناپ;
  }

  ثابت Tuple_Variation_سرخی &get_tuple_var_سرخی (فراغ) ثابت
  { أرجع StructAfter<Tuple_Variation_سرخی> (انکڑے); }

  بنية tuple_iterator_t;
  بنية tuple_variations_t
  {
    hb_سمتیہ_t<tuple_delta_t> tuple_vars;

    خاص:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ثابت hb_سمتیہ_t<منطقي>*, hb_سمتیہ_t<بنانشانی حرف>> نقطہ_انکڑے_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ثابت hb_سمتیہ_t<منطقي>*, بنانشانی> نقطہ_سیٹ_گنتی_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_سمتیہ_t<بنانشانی حرف> *shared_points_بائٹاں = خالی_سوچک;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    بنانشانی compiled_byte_ناپ = 0;
    منطقي needs_padding = خطأ;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    منطقي is_مرکب = خطأ;

    عام:
    tuple_variations_t () = افتراضي;
    tuple_variations_t (ثابت tuple_variations_t&) = مٹاؤ;
    tuple_variations_t& عامل=(ثابت tuple_variations_t&) = مٹاؤ;
    tuple_variations_t (tuple_variations_t&&) = افتراضي;
    tuple_variations_t& عامل=(tuple_variations_t&&) = افتراضي;
    ~tuple_variations_t () = افتراضي;

    واضح عامل منطقي () ثابت { أرجع منطقي (tuple_vars); }
    بنانشانی get_var_گنتی () ثابت
    {
      بنانشانی گنتی = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      لكل (خودکار& tuple: tuple_vars)
        إذا (tuple.compiled_deltas) گنتی++;

      إذا (shared_points_بائٹاں && shared_points_بائٹاں->لمائی)
        گنتی |= Tuple_Var_گنتی::Shared_نقطہ_Numbers;
      أرجع گنتی;
    }

    بنانشانی get_compiled_byte_ناپ () ثابت
    { أرجع compiled_byte_ناپ; }

    منطقي create_from_tuple_var_انکڑے (tuple_iterator_t iterator,
                                     بنانشانی tuple_var_گنتی,
                                     بنانشانی نقطہ_گنتی,
                                     منطقي is_gvar,
                                     ثابت hb_map_t *axes_old_اشاریہ_ٹیگ_map,
                                     ثابت hb_سمتیہ_t<بنانشانی> &shared_indices,
                                     ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = خالی_سوچک,
                                     منطقي is_مرکب_glyph = خطأ)
    {
      hb_سمتیہ_t<بنانشانی> نجی_indices;
      hb_سمتیہ_t<صحيح> deltas_x;
      hb_سمتیہ_t<صحيح> deltas_y;
      افعل
      {
        ثابت HBUINT8 *p = iterator.get_serialized_انکڑے ();
        بنانشانی صحيح لمائی = iterator.current_tuple->get_انکڑے_ناپ ();
        إذا (unlikely (!iterator.var_انکڑے_بائٹاں.check_range (p, لمائی)))
          أرجع خطأ;

	hb_hashmap_t<hb_ٹیگ_t, Triple> axis_tuples;
        إذا (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_گنتی (), shared_tuples, axes_old_اشاریہ_ٹیگ_map, axis_tuples)
            || axis_tuples.is_empty ())
          أرجع خطأ;

	نجی_indices.reset ();
        منطقي has_نجی_points = iterator.current_tuple->has_نجی_points ();
        ثابت HBUINT8 *سرا = p + لمائی;
        إذا (has_نجی_points &&
            !Tuple_Variation_انکڑے::decompile_points (p, نجی_indices, سرا))
          أرجع خطأ;

        ثابت hb_سمتیہ_t<بنانشانی> &indices = has_نجی_points ? نجی_indices : shared_indices;
        منطقي apply_منزل_all = (indices.لمائی == 0);
        بنانشانی عدد_deltas = apply_منزل_all ? نقطہ_گنتی : indices.لمائی;

        إذا (unlikely (!deltas_x.resize_dirty  (عدد_deltas) ||
                      !Tuple_Variation_انکڑے::decompile_deltas (p, deltas_x, سرا)))
          أرجع خطأ;

        إذا (is_gvar)
        {
          إذا (unlikely (!deltas_y.resize_dirty  (عدد_deltas) ||
                        !Tuple_Variation_انکڑے::decompile_deltas (p, deltas_y, سرا)))
            أرجع خطأ;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        إذا (unlikely (!var.indices.allocate_from_pool (pool, نقطہ_گنتی) ||
                      !var.deltas_x.allocate_from_pool (pool, نقطہ_گنتی, خطأ)))
          أرجع خطأ;

        إذا (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, نقطہ_گنتی, خطأ)))
          أرجع خطأ;

        لكل (بنانشانی i = 0; i < عدد_deltas; i++)
        {
          بنانشانی idx = apply_منزل_all ? i : indices[i];
          إذا (idx >= نقطہ_گنتی) استمر;
          var.indices[idx] = سچ;
          var.deltas_x[idx] = deltas_x[i];
          إذا (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } طالما (iterator.move_منزل_اگلا ());

      is_مرکب = is_مرکب_glyph;
      أرجع سچ;
    }

    منطقي create_from_شے_var_انکڑے (ثابت Var_انکڑے &var_انکڑے,
                                    ثابت hb_سمتیہ_t<hb_hashmap_t<hb_ٹیگ_t, Triple>>& regions,
                                    ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map,
                                    بنانشانی& شے_گنتی_2,
                                    ثابت hb_inc_bimap_t* inner_map = خالی_سوچک)
    {
      /* NULL offset, to keep original varidx valid, just return */
      إذا (&var_انکڑے == &Null (Var_انکڑے))
        أرجع سچ;

      بنانشانی عدد_regions = var_انکڑے.get_region_اشاریہ_گنتی ();
      إذا (!tuple_vars.alloc (عدد_regions)) أرجع خطأ;

      شے_گنتی_2 = inner_map ? inner_map->get_population () : var_انکڑے.get_شے_گنتی ();
      إذا (!شے_گنتی_2) أرجع سچ;
      بنانشانی صف_ناپ = var_انکڑے.get_صف_ناپ ();
      ثابت HBUINT8 *delta_بائٹاں = var_انکڑے.get_delta_بائٹاں ();

      لكل (بنانشانی r = 0; r < عدد_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        إذا (!tuple.deltas_x.resize_dirty  (شے_گنتی_2) ||
            !tuple.indices.resize_dirty  (شے_گنتی_2))
          أرجع خطأ;

        لكل (بنانشانی i = 0; i < شے_گنتی_2; i++)
        {
          tuple.indices.arrayZ[i] = سچ;
          tuple.deltas_x.arrayZ[i] = var_انکڑے.get_شے_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_بائٹاں, صف_ناپ);
        }

        بنانشانی region_اشاریہ = var_انکڑے.get_region_اشاریہ (r);
        إذا (region_اشاریہ >= regions.لمائی) أرجع خطأ;
        tuple.axis_tuples = regions.arrayZ[region_اشاریہ];

        tuple_vars.push (std::move (tuple));
      }
      أرجع !tuple_vars.وچ_error ();
    }

    خاص:
    ساكن صحيح cmp_axis_ٹیگ (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت hb_ٹیگ_t *a = (ثابت hb_ٹیگ_t*) pa;
      ثابت hb_ٹیگ_t *b = (ثابت hb_ٹیگ_t*) pb;
      أرجع (صحيح)(*a) - (صحيح)(*b);
    }

    منطقي change_tuple_variations_axis_limits (ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>& normalized_axes_location,
                                              ثابت hb_hashmap_t<hb_ٹیگ_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = خالی_سوچک)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_سمتیہ_t<hb_ٹیگ_t> axis_ٹیگ_سوچی;
      إذا (!axis_ٹیگ_سوچی.alloc (normalized_axes_location.get_population ()))
        أرجع خطأ;
      لكل (خودکار t : normalized_axes_location.کلیدیں ())
        axis_ٹیگ_سوچی.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_نتیجہ_scratch_t scratch;
      hb_سمتیہ_t<tuple_delta_t> باہرلا;

      axis_ٹیگ_سوچی.qsort (cmp_axis_ٹیگ);
      لكل (خودکار axis_ٹیگ_2 : axis_ٹیگ_سوچی)
      {
        Triple *axis_حد_بندی;
        إذا (!normalized_axes_location.has (axis_ٹیگ_2, &axis_حد_بندی))
          أرجع خطأ;
        TripleDistances axis_triple_distances{1.f, 1.f};
        إذا (axes_triple_distances.has (axis_ٹیگ_2))
          axis_triple_distances = axes_triple_distances.get (axis_ٹیگ_2);

        hb_سمتیہ_t<tuple_delta_t> new_vars;
        لكل (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_حد_بندی (axis_ٹیگ_2, *axis_حد_بندی, axis_triple_distances, باہرلا, scratch, pool);
          إذا (!باہرلا) استمر;

          بنانشانی new_len = new_vars.لمائی + باہرلا.لمائی;

          إذا (unlikely (!new_vars.alloc (new_len, خطأ)))
            أرجع خطأ;

          لكل (بنانشانی i = 0; i < باہرلا.لمائی; i++)
            new_vars.push (std::move (باہرلا[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      أرجع سچ;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    منطقي ملاؤ_tuple_variations (contour_نقطہ_سمتیہ_t* contour_points = خالی_سوچک)
    {
      hb_سمتیہ_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      إذا (unlikely (!new_vars.alloc (tuple_vars.لمائی)))
	أرجع خطأ;
      hb_hashmap_t<ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>*, بنانشانی> m;
      لكل (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        إذا (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          إذا (contour_points && !contour_points->جوڑو_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            أرجع خطأ;
          استمر;
        }

        بنانشانی *idx;
        إذا (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        وإلا
        {
	  خودکار *new_var = new_vars.push ();
	  إذا (unlikely (new_vars.وچ_error ()))
	    أرجع خطأ;
          hb_وٹاندرا (*new_var, var);
          إذا (unlikely (!m.سیٹ (&(new_var->axis_tuples), new_vars.لمائی - 1)))
            أرجع خطأ;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      أرجع سچ;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    منطقي compile_all_نقطہ_sets ()
    {
      لكل (ثابت خودکار& tuple: tuple_vars)
      {
        ثابت hb_سمتیہ_t<منطقي>* points_سیٹ = &(tuple.indices);
        إذا (نقطہ_انکڑے_map.has (points_سیٹ))
        {
          بنانشانی *گنتی;
          إذا (unlikely (!نقطہ_سیٹ_گنتی_map.has (points_سیٹ, &گنتی) ||
                        !نقطہ_سیٹ_گنتی_map.سیٹ (points_سیٹ, (*گنتی) + 1)))
            أرجع خطأ;
          استمر;
        }

        hb_سمتیہ_t<بنانشانی حرف> compiled_نقطہ_انکڑے;
        إذا (!tuple_delta_t::compile_نقطہ_سیٹ (*points_سیٹ, compiled_نقطہ_انکڑے))
          أرجع خطأ;

        إذا (!نقطہ_انکڑے_map.سیٹ (points_سیٹ, std::move (compiled_نقطہ_انکڑے)) ||
            !نقطہ_سیٹ_گنتی_map.سیٹ (points_سیٹ, 1))
          أرجع خطأ;
      }
      أرجع سچ;
    }

    /* find shared points set which saves most bytes */
    فراغ لبھو_shared_points ()
    {
      بنانشانی max_saved_بائٹاں = 0;

      لكل (ثابت خودکار& _ : نقطہ_انکڑے_map.iter_ref ())
      {
        ثابت hb_سمتیہ_t<منطقي>* points_سیٹ = _.پہلا;
        بنانشانی انکڑے_لمائی = _.second.لمائی;
        إذا (!انکڑے_لمائی) استمر;
        بنانشانی *گنتی;
        إذا (unlikely (!نقطہ_سیٹ_گنتی_map.has (points_سیٹ, &گنتی) ||
                      *گنتی <= 1))
        {
          shared_points_بائٹاں = خالی_سوچک;
          أرجع;
        }

        بنانشانی saved_بائٹاں = انکڑے_لمائی * ((*گنتی) -1);
        إذا (saved_بائٹاں > max_saved_بائٹاں)
        {
          max_saved_بائٹاں = saved_بائٹاں;
          shared_points_بائٹاں = &(_.second);
        }
      }
    }

    منطقي calc_inferred_deltas (ثابت contour_نقطہ_سمتیہ_t& contour_points,
			       hb_سمتیہ_t<بنانشانی> &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
        إذا (!var.calc_inferred_deltas (contour_points, scratch))
          أرجع خطأ;

      أرجع سچ;
    }

    منطقي iup_optimize (ثابت contour_نقطہ_سمتیہ_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
      {
        إذا (!var.optimize (contour_points, is_مرکب, scratch))
          أرجع خطأ;
      }
      أرجع سچ;
    }

    عام:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    فراغ cull_unreachable (ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> &reachable_ranges)
    {
      بنانشانی j = 0;
      لكل (بنانشانی i = 0; i < tuple_vars.لمائی; i++)
      {
	إذا (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  استمر;
	إذا (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    منطقي instantiate (ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>& normalized_axes_location,
                      ثابت hb_hashmap_t<hb_ٹیگ_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = خالی_سوچک,
                      contour_نقطہ_سمتیہ_t* contour_points = خالی_سوچک,
                      منطقي optimize = خطأ)
    {
      إذا (!tuple_vars) أرجع سچ;
      إذا (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        أرجع خطأ;
      /* compute inferred deltas only for gvar */
      إذا (contour_points)
      {
	hb_سمتیہ_t<بنانشانی> scratch;
	إذا (!calc_inferred_deltas (*contour_points, scratch))
	  أرجع خطأ;
      }

      /* if iup delta opt is on, contour_points can't be null */
      إذا (optimize && !contour_points)
        أرجع خطأ;

      إذا (!ملاؤ_tuple_variations (optimize ? contour_points : خالی_سوچک))
        أرجع خطأ;

      إذا (optimize && !iup_optimize (*contour_points, scratch)) أرجع خطأ;
      أرجع !tuple_vars.وچ_error ();
    }

    منطقي compile_بائٹاں (ثابت hb_map_t& axes_اشاریہ_map,
                        ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map,
                        منطقي use_shared_points,
                        منطقي is_gvar = خطأ,
                        ثابت hb_hashmap_t<ثابت hb_سمتیہ_t<F2DOT14>*, بنانشانی>* shared_tuples_idx_map = خالی_سوچک,
			hb_alloc_pool_t *pool = خالی_سوچک)
    {
      // return true for empty glyph
      إذا (!tuple_vars)
        أرجع سچ;

      // compile points set and store data in hashmap
      إذا (!compile_all_نقطہ_sets ())
        أرجع خطأ;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_ناپ += 4;

      إذا (use_shared_points)
      {
        لبھو_shared_points ();
        إذا (shared_points_بائٹاں)
          compiled_byte_ناپ += shared_points_بائٹاں->لمائی;
      }
      hb_سمتیہ_t<صحيح> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      لكل (خودکار& tuple: tuple_vars)
      {
        ثابت hb_سمتیہ_t<منطقي>* points_سیٹ = &(tuple.indices);
        hb_سمتیہ_t<بنانشانی حرف> *points_انکڑے;
        إذا (unlikely (!نقطہ_انکڑے_map.has (points_سیٹ, &points_انکڑے)))
          أرجع خطأ;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        إذا (!points_انکڑے->لمائی)
          استمر;
        إذا (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          أرجع خطأ;

        بنانشانی points_انکڑے_لمائی = (points_انکڑے != shared_points_بائٹاں) ? points_انکڑے->لمائی : 0;
        إذا (!tuple.compile_tuple_var_سرخی (axes_اشاریہ_map, points_انکڑے_لمائی, axes_old_اشاریہ_ٹیگ_map,
                                             shared_tuples_idx_map,
					     pool))
          أرجع خطأ;
        compiled_byte_ناپ += tuple.compiled_tuple_سرخی.لمائی + points_انکڑے_لمائی + tuple.compiled_deltas.لمائی;
      }

      إذا (is_gvar && (compiled_byte_ناپ % 2))
      {
        needs_padding = سچ;
        compiled_byte_ناپ += 1;
      }

      أرجع سچ;
    }

    منطقي serialize_var_سرخیاں (hb_serialize_context_t *c, بنانشانی& کل_سرخی_len) ثابت
    {
      TRACE_SERIALIZE (هذا);
      لكل (ثابت خودکار& tuple: tuple_vars)
      {
        tuple.compiled_tuple_سرخی.as_array ().copy (c);
        إذا (c->وچ_error ()) return_trace (خطأ);
        کل_سرخی_len += tuple.compiled_tuple_سرخی.لمائی;
      }
      return_trace (سچ);
    }

    منطقي serialize_var_انکڑے (hb_serialize_context_t *c, منطقي is_gvar) ثابت
    {
      TRACE_SERIALIZE (هذا);
      إذا (is_gvar && shared_points_بائٹاں)
      {
        hb_ubytes_t s (shared_points_بائٹاں->arrayZ, shared_points_بائٹاں->لمائی);
        s.copy (c);
      }

      لكل (ثابت خودکار& tuple: tuple_vars)
      {
        ثابت hb_سمتیہ_t<منطقي>* points_سیٹ = &(tuple.indices);
        hb_سمتیہ_t<بنانشانی حرف> *نقطہ_انکڑے;
        إذا (!نقطہ_انکڑے_map.has (points_سیٹ, &نقطہ_انکڑے))
          return_trace (خطأ);

        إذا (!is_gvar || نقطہ_انکڑے != shared_points_بائٹاں)
        {
          hb_ubytes_t s (نقطہ_انکڑے->arrayZ, نقطہ_انکڑے->لمائی);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        إذا (c->وچ_error ()) return_trace (خطأ);
      }

      /* padding for gvar */
      إذا (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        إذا (!c->embed (pad)) return_trace (خطأ);
      }
      return_trace (سچ);
    }
  };

  بنية tuple_iterator_t
  {
    بنانشانی get_axis_گنتی () ثابت { أرجع axis_گنتی; }

    فراغ init (hb_بائٹاں_t var_انکڑے_بائٹاں_2, بنانشانی صحيح axis_گنتی_3, ثابت فراغ *جدول_ادھار)
    {
      var_انکڑے_بائٹاں = var_انکڑے_بائٹاں_2;
      var_انکڑے = var_انکڑے_بائٹاں_2.as<Tuple_Variation_انکڑے> ();
      tuples_بایاں = var_انکڑے->tuple_Var_گنتی.get_گنتی ();
      axis_گنتی = axis_گنتی_3;
      axis_گنتی_times_2 = axis_گنتی_3 * 2;
      current_tuple = &var_انکڑے->get_tuple_var_سرخی ();
      انکڑے_وتھ_2 = 0;
      جدول_ادھار_2 = جدول_ادھار;
    }

    منطقي get_shared_indices (hb_سمتیہ_t<بنانشانی صحيح> &shared_indices /*باہرلا*/)
    {
      إذا (var_انکڑے->has_shared_نقطہ_numbers ())
      {
        ثابت HBUINT8 *ادھار = &(جدول_ادھار_2+var_انکڑے->انکڑے);
        ثابت HBUINT8 *p = ادھار;
        إذا (!decompile_points (p, shared_indices, (ثابت HBUINT8 *) (var_انکڑے_بائٹاں.arrayZ + var_انکڑے_بائٹاں.لمائی))) أرجع خطأ;
        انکڑے_وتھ_2 = p - ادھار;
      }
      أرجع سچ;
    }

    منطقي is_valid ()
    {
      إذا (unlikely (tuples_بایاں <= 0))
	أرجع خطأ;

      current_tuple_ناپ = Tuple_Variation_سرخی::min_ناپ;
      إذا (unlikely (!var_انکڑے_بائٹاں.check_سرا ((ثابت حرف *) current_tuple + current_tuple_ناپ)))
	أرجع خطأ;

      current_tuple_ناپ = current_tuple->get_ناپ (axis_گنتی_times_2);
      إذا (unlikely (!var_انکڑے_بائٹاں.check_سرا ((ثابت حرف *) current_tuple + current_tuple_ناپ)))
	أرجع خطأ;

      أرجع سچ;
    }

    HB_ALWAYS_INLINE
    منطقي move_منزل_اگلا ()
    {
      انکڑے_وتھ_2 += current_tuple->get_انکڑے_ناپ ();
      current_tuple = &Struct_At_وتھ<Tuple_Variation_سرخی> (current_tuple, current_tuple_ناپ);
      tuples_بایاں--;
      أرجع is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ثابت HBUINT8 *get_serialized_انکڑے () ثابت
    { أرجع &(جدول_ادھار_2+var_انکڑے->انکڑے) + انکڑے_وتھ_2; }

    خاص:
    نشانیوالا tuples_بایاں;
    ثابت Tuple_Variation_انکڑے *var_انکڑے;
    بنانشانی صحيح axis_گنتی;
    بنانشانی صحيح axis_گنتی_times_2;
    بنانشانی صحيح انکڑے_وتھ_2;
    بنانشانی صحيح current_tuple_ناپ;
    ثابت فراغ *جدول_ادھار_2;

    عام:
    hb_بائٹاں_t var_انکڑے_بائٹاں;
    ثابت Tuple_Variation_سرخی *current_tuple;
  };

  ساكن منطقي get_tuple_iterator (hb_بائٹاں_t var_انکڑے_بائٹاں, بنانشانی axis_گنتی,
                                  ثابت فراغ *جدول_ادھار_2,
                                  hb_سمتیہ_t<بنانشانی صحيح> &shared_indices /*باہرلا*/,
                                  tuple_iterator_t *iterator /*باہرلا*/)
  {
    iterator->init (var_انکڑے_بائٹاں, axis_گنتی, جدول_ادھار_2);
    إذا (!iterator->get_shared_indices (shared_indices))
      أرجع خطأ;
    أرجع iterator->is_valid ();
  }

  منطقي has_shared_نقطہ_numbers () ثابت { أرجع tuple_Var_گنتی.has_shared_نقطہ_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  منطقي cull_tuple_variations (hb_بائٹاں_t var_انکڑے_بائٹاں,
			      بنانشانی axis_گنتی,
			      hb_array_t<ثابت F2DOT14> shared_tuples,
			      ثابت hb_map_t *axes_old_اشاریہ_ٹیگ_map,
			      ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> &reachable_ranges,
			      hb_سمتیہ_t<حرف> &باہرلا /*باہرلا*/,
			      منطقي *بدلیا /*باہرلا*/) ثابت
  {
    *بدلیا = خطأ;

    hb_سمتیہ_t<بنانشانی صحيح> shared_indices;
    tuple_iterator_t iterator;
    إذا (!get_tuple_iterator (var_انکڑے_بائٹاں, axis_گنتی, هذا,
			     shared_indices, &iterator))
      أرجع سچ; /* no tuples or malformed: keep original */

    ثابت حرف *بائٹاں_شروع = var_انکڑے_بائٹاں.arrayZ;
    ثابت حرف *بائٹاں_سرا = بائٹاں_شروع + var_انکڑے_بائٹاں.لمائی;
    ثابت حرف *serialized_ادھار = (ثابت حرف *) &(هذا+انکڑے);
    ثابت حرف *shared_points_سرا = (ثابت حرف *) iterator.get_serialized_انکڑے ();
    إذا (unlikely (serialized_ادھار < بائٹاں_شروع || serialized_ادھار > بائٹاں_سرا ||
		  shared_points_سرا < serialized_ادھار || shared_points_سرا > بائٹاں_سرا))
      أرجع سچ;

    بنية kept_tuple_t
    {
      ثابت حرف *سرخی;
      بنانشانی سرخی_ناپ;
      ثابت حرف *انکڑے;
      بنانشانی انکڑے_ناپ;
    };
    hb_سمتیہ_t<kept_tuple_t> kept;
    بنانشانی کل = 0;
    بنانشانی kept_سرخیاں_ناپ = 0, kept_انکڑے_ناپ = 0;
    افعل
    {
      ثابت Tuple_Variation_سرخی *سرخی = iterator.current_tuple;
      بنانشانی سرخی_ناپ = سرخی->get_ناپ (axis_گنتی * 2);
      بنانشانی انکڑے_ناپ = سرخی->get_انکڑے_ناپ ();
      ثابت حرف *tuple_انکڑے = (ثابت حرف *) iterator.get_serialized_انکڑے ();
      إذا (unlikely (tuple_انکڑے < serialized_ادھار ||
		    انکڑے_ناپ > (بنانشانی) (بائٹاں_سرا - tuple_انکڑے)))
	أرجع سچ; /* malformed: keep original */

      hb_hashmap_t<hb_ٹیگ_t, Triple> axis_tuples;
      إذا (!سرخی->unpack_axis_tuples (axis_گنتی, shared_tuples,
				       axes_old_اشاریہ_ٹیگ_map, axis_tuples))
	أرجع سچ;

      کل++;
      إذا (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ثابت حرف *) سرخی, سرخی_ناپ,
				 tuple_انکڑے, انکڑے_ناپ});
	kept_سرخیاں_ناپ += سرخی_ناپ;
	kept_انکڑے_ناپ += انکڑے_ناپ;
      }
    } طالما (iterator.move_منزل_اگلا ());

    إذا (unlikely (kept.وچ_error ())) أرجع خطأ;
    إذا (kept.لمائی == کل) أرجع سچ; /* nothing to cull */

    *بدلیا = سچ;
    إذا (!kept.لمائی) أرجع سچ; /* everything culled: no variation data */

    بنانشانی shared_points_ناپ = shared_points_سرا - serialized_ادھار;
    بنانشانی انکڑے_وتھ_2 = min_ناپ + kept_سرخیاں_ناپ;
    بنانشانی new_ناپ = انکڑے_وتھ_2 + shared_points_ناپ + kept_انکڑے_ناپ;
    إذا (unlikely (!باہرلا.resize (new_ناپ)))
      أرجع خطأ;

    Tuple_Variation_انکڑے *باہرلا_انکڑے = (Tuple_Variation_انکڑے *) باہرلا.arrayZ;
    باہرلا_انکڑے->tuple_Var_گنتی = (uint16_t) (kept.لمائی |
					  (has_shared_نقطہ_numbers () ? 0x8000u : 0u));
    باہرلا_انکڑے->انکڑے = انکڑے_وتھ_2;

    حرف *p = باہرلا.arrayZ + min_ناپ;
    لكل (ثابت خودکار &t : kept)
    {
      hb_memcpy (p, t.سرخی, t.سرخی_ناپ);
      p += t.سرخی_ناپ;
    }
    hb_memcpy (p, serialized_ادھار, shared_points_ناپ);
    p += shared_points_ناپ;
    لكل (ثابت خودکار &t : kept)
    {
      hb_memcpy (p, t.انکڑے, t.انکڑے_ناپ);
      p += t.انکڑے_ناپ;
    }
    أرجع سچ;
  }

  ساكن منطقي decompile_points (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_سمتیہ_t<بنانشانی صحيح> &points /*باہرلا*/,
				ثابت HBUINT8 *سرا)
  {
    تعداد packed_نقطہ_flag_t
    {
      POINTS_ARE_الفاظ     = 0x80,
      نقطہ_لگاتار_گنتی_نقاب = 0x7F
    };

    إذا (unlikely (p + 1 > سرا)) أرجع خطأ;

    بنانشانی گنتی = *p++;
    إذا (گنتی & POINTS_ARE_الفاظ)
    {
      إذا (unlikely (p + 1 > سرا)) أرجع خطأ;
      گنتی = ((گنتی & نقطہ_لگاتار_گنتی_نقاب) << 8) | *p++;
    }
    إذا (unlikely (!points.resize_dirty  (گنتی))) أرجع خطأ;

    بنانشانی n = 0;
    بنانشانی i = 0;
    طالما (i < گنتی)
    {
      إذا (unlikely (p + 1 > سرا)) أرجع خطأ;
      بنانشانی control = *p++;
      بنانشانی لگاتار_گنتی = (control & نقطہ_لگاتار_گنتی_نقاب) + 1;
      بنانشانی stop = i + لگاتار_گنتی;
      إذا (unlikely (stop > گنتی)) أرجع خطأ;
      إذا (control & POINTS_ARE_الفاظ)
      {
        إذا (unlikely (p + لگاتار_گنتی * HBUINT16::static_ناپ > سرا)) أرجع خطأ;
        لكل (; i < stop; i++)
        {
          n += *(ثابت HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_ناپ;
        }
      }
      وإلا
      {
        إذا (unlikely (p + لگاتار_گنتی > سرا)) أرجع خطأ;
        لكل (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    أرجع سچ;
  }

  سانچا <قسم_ناں T>
  HB_ALWAYS_INLINE
  ساكن منطقي decompile_deltas (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_سمتیہ_t<T> &deltas /* IN/OUT */,
				ثابت HBUINT8 *سرا,
				منطقي consume_all = خطأ,
				بنانشانی شروع = 0)
  {
    أرجع Tuple_اقدار::decompile (p, deltas, سرا, consume_all, شروع);
  }

  منطقي has_انکڑے () ثابت { أرجع tuple_Var_گنتی; }

  منطقي decompile_tuple_variations (بنانشانی نقطہ_گنتی,
                                   منطقي is_gvar,
                                   tuple_iterator_t iterator,
                                   ثابت hb_map_t *axes_old_اشاریہ_ٹیگ_map,
                                   ثابت hb_سمتیہ_t<بنانشانی> &shared_indices,
                                   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /*باہرلا*/
				   hb_alloc_pool_t *pool = خالی_سوچک,
                                   منطقي is_مرکب_glyph = خطأ) ثابت
  {
    أرجع tuple_variations.create_from_tuple_var_انکڑے (iterator, tuple_Var_گنتی,
                                                        نقطہ_گنتی, is_gvar,
                                                        axes_old_اشاریہ_ٹیگ_map,
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
    إذا (!tuple_variations) return_trace (سچ);

    خودکار *باہرلا = c->شروع_embed (هذا);
    إذا (unlikely (!c->extend_min (باہرلا))) return_trace (خطأ);

    إذا (!c->check_سونپو (باہرلا->tuple_Var_گنتی, tuple_variations.get_var_گنتی (),
                          HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)) return_trace (خطأ);

    بنانشانی کل_سرخی_len = 0;

    إذا (!tuple_variations.serialize_var_سرخیاں (c, کل_سرخی_len))
      return_trace (خطأ);

    بنانشانی انکڑے_وتھ_2 = min_ناپ + کل_سرخی_len;
    إذا (!is_gvar) انکڑے_وتھ_2 += 4;
    إذا (!c->check_سونپو (باہرلا->انکڑے, انکڑے_وتھ_2, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)) return_trace (خطأ);

    أرجع tuple_variations.serialize_var_انکڑے (c, is_gvar);
  }

  محمي:
  بنية Tuple_Var_گنتی : HBUINT16
  {
    یار بنية tuple_variations_t;
    منطقي has_shared_نقطہ_numbers () ثابت { أرجع ((*هذا) & Shared_نقطہ_Numbers); }
    بنانشانی صحيح get_گنتی () ثابت { أرجع (*هذا) & گنتی_نقاب; }
    Tuple_Var_گنتی& عامل = (uint16_t i) { HBUINT16::عامل= (i); أرجع *هذا; }
    واضح عامل منطقي () ثابت { أرجع get_گنتی (); }

    محمي:
    تعداد Flags
    {
      Shared_نقطہ_Numbers= 0x8000u,
      گنتی_نقاب         = 0x0FFFu
    };
    عام:
    DEFINE_ناپ_STATIC (2);
  };

  Tuple_Var_گنتی tuple_Var_گنتی;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  وتھ_منزل<HBUINT8, OffType>
                انکڑے;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  عام:
  DEFINE_ناپ_MIN (2 + OffType::static_ناپ);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
ورتوں tuple_variations_t = Tuple_Variation_انکڑے<HBUINT16>::tuple_variations_t;
بنية شے_variations_t
{
  ورتوں region_t = ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>*;
  خاص:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_سمتیہ_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_سمتیہ_t<بنانشانی> var_انکڑے_عدد_صفاں;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_سمتیہ_t<hb_hashmap_t<hb_ٹیگ_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_سمتیہ_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, بنانشانی> region_map;

  /* all delta rows after instantiation */
  hb_سمتیہ_t<hb_سمتیہ_t<صحيح>> delta_صفاں;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_سمتیہ_t<delta_صف_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  منطقي has_long = خطأ;

  عام:
  منطقي has_long_لفظ () ثابت
  { أرجع has_long; }

  ثابت hb_سمتیہ_t<region_t>& get_region_list () ثابت
  { أرجع region_list; }

  ثابت hb_سمتیہ_t<delta_صف_encoding_t>& get_vardata_encodings () ثابت
  { أرجع encodings; }

  ثابت hb_map_t& get_varidx_map () ثابت
  { أرجع varidx_map; }

  منطقي جوڑو_vardata_encoding_for_testing (hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح>*> &&صفاں,
                                         بنانشانی عدد_خانے_گنتی)
  {
    encodings.push (delta_صف_encoding_t (std::move (صفاں), عدد_خانے_گنتی));
    أرجع !encodings.وچ_error ();
  }

  منطقي compile_varidx_map_for_testing (ثابت hb_hashmap_t<بنانشانی, ثابت hb_سمتیہ_t<صحيح>*>& front_mapping)
  { أرجع compile_varidx_map (front_mapping); }

  منطقي instantiate (ثابت شے_Variation_Store& varStore,
                    ثابت hb_subset_plan_t *plan,
                    منطقي optimize=سچ,
                    منطقي use_no_variation_idx=سچ,
                    ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> (),
                    منطقي layout_conditions = خطأ)
  {
    إذا (!create_from_شے_varstore (varStore, plan->axes_old_اشاریہ_ٹیگ_map, inner_maps))
      أرجع خطأ;
    /* avar2 partial instancing: cull unreachable regions. */
    إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      لكل (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    إذا (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      أرجع خطأ;
    إذا (layout_conditions &&
        (!instantiate_condition_اقدار (plan, varStore) || !build_region_list ()))
      أرجع خطأ;
    أرجع as_شے_varstore (optimize, use_no_variation_idx);
  }

  منطقي instantiate_condition_اقدار (ثابت hb_subset_plan_t *plan,
                                      ثابت شے_Variation_Store &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    خودکار &condition_map = plan->layout_condition_idx_قدر_map;
    condition_map.clear ();
    إذا (!plan->layout_condition_اقدار) أرجع سچ;
    ورتوں condition_قدر_t = hb_pair_t<uintptr_t, hb_pair_t<بنانشانی, صحيح>>;
    hb_سمتیہ_t<condition_قدر_t> conditions;
    لكل (خودکار condition : plan->layout_condition_اقدار.iter ())
      conditions.push (condition);
    إذا (unlikely (conditions.وچ_error ())) أرجع خطأ;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ثابت condition_قدر_t &a, ثابت condition_قدر_t &b) {
      إذا (a.second.پہلا != b.second.پہلا)
        أرجع a.second.پہلا < b.second.پہلا ? -1 : 1;
      أرجع a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    بنانشانی new_outer = 0;
    لكل (بنانشانی i = 0; i < plan->gdef_varstore_inner_maps.لمائی; i++)
      إذا (plan->gdef_varstore_inner_maps[i].get_population ())
        إذا (!outer_map.سیٹ (i, new_outer++)) أرجع خطأ;
    لكل (خودکار condition : conditions)
    {
      بنانشانی old_idx = condition.second.پہلا;
      مزدوج value = condition.second.second +
                     varStore.get_delta<مزدوج> (old_idx, plan->normalized_coords.as_array ());
      بنانشانی old_outer = old_idx >> 16;
      إذا (!outer_map.has (old_outer))
        استمر;
      بنانشانی outer = outer_map.get (old_outer);
      بنانشانی inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      بنانشانی idx = (outer << 16) | inner;
      إذا (outer >= vars.لمائی || inner >= var_انکڑے_عدد_صفاں[outer])
        استمر;

      مزدوج ودھ_توں_ودھ = fabs ((مزدوج) value);
      منطقي variable = خطأ;
      لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        إذا (unlikely (inner >= tuple.deltas_x.لمائی)) أرجع خطأ;
        عائم delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        ودھ_توں_ودھ = hb_max (ودھ_توں_ودھ, fabs ((مزدوج) delta));
      }
      إذا (!variable)
      {
        إذا (!condition_map.سیٹ (condition.پہلا,
                                hb_pair (بنانشانی (VarIdx::NO_VARIATION), صحيح (value > 0))))
          أرجع خطأ;
        استمر;
      }

      مزدوج scale = 1.;
      طالما (ودھ_توں_ودھ * scale > ودھ_توں_ودھ_عدد)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      طالما (ودھ_توں_ودھ * scale * 2 <= ودھ_توں_ودھ_عدد)
      {
        منطقي integral = (مزدوج) value * scale == round ((مزدوج) value * scale);
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (مزدوج) tuple.deltas_x[inner] * scale ==
                      round ((مزدوج) tuple.deltas_x[inner] * scale);
        إذا (integral) اكسر;
        scale *= 2;
      }
      صحيح default_قدر = (صحيح) round ((مزدوج) value * scale);
      إذا (scale != 1. || default_قدر < -32768 || default_قدر > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t نجی_صف;
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          إذا (!tuple.deltas_x[inner]) استمر;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (سچ);
          copy.deltas_x.push ((عائم) round ((مزدوج) tuple.deltas_x[inner] * scale));
          إذا (unlikely (copy.axis_tuples.وچ_error () || copy.indices.وچ_error () ||
                        copy.deltas_x.وچ_error ())) أرجع خطأ;
          نجی_صف.tuple_vars.push (std::move (copy));
        }
        إذا (unlikely (vars.لمائی >= 0xFFFFu)) أرجع خطأ;
        إذا (unlikely (نجی_صف.tuple_vars.وچ_error ())) أرجع خطأ;
        idx = vars.لمائی << 16;
        vars.push (std::move (نجی_صف));
        var_انکڑے_عدد_صفاں.push (1);
        إذا (unlikely (vars.وچ_error () || var_انکڑے_عدد_صفاں.وچ_error ())) أرجع خطأ;
        إذا (default_قدر < -32768 || default_قدر > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          صحيح bias = default_قدر - hb_clamp (default_قدر, -32768, 32767);
          default_قدر -= bias;
          صحيح بلند = bias / 65536 * 65536;
          صحيح parts[] = {بلند, bias - بلند};
          لكل (صحيح part : parts)
          {
            إذا (!part) استمر;
            بنانشانی گنتی = vars[idx >> 16].tuple_vars.لمائی;
            جوڑو_tuple (idx >> 16, hb_hashmap_t<hb_ٹیگ_t, Triple> (),
                       0, part, 1);
            ثابت خودکار &tuples = vars[idx >> 16].tuple_vars;
            إذا (unlikely (tuples.وچ_error () || tuples.لمائی != گنتی + 1 ||
                          tuples.پوچھ ().indices.وچ_error () ||
                          tuples.پوچھ ().deltas_x.وچ_error ())) أرجع خطأ;
          }
        }
      }
      إذا (!condition_map.سیٹ (condition.پہلا, hb_pair (idx, default_قدر)))
        أرجع خطأ;
    }
    أرجع !condition_map.وچ_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  منطقي create_from_شے_varstore (ثابت شے_Variation_Store& varStore,
                                  ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map,
                                  ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> ())
  {
    ثابت VarRegionList& regionList = varStore.get_region_list ();
    إذا (!regionList.get_var_regions (axes_old_اشاریہ_ٹیگ_map, orig_region_list))
      أرجع خطأ;

    بنانشانی عدد_var_انکڑے = varStore.get_sub_جدول_گنتی ();
    إذا (inner_maps && inner_maps.لمائی != عدد_var_انکڑے) أرجع خطأ;
    إذا (!vars.alloc (عدد_var_انکڑے) ||
        !var_انکڑے_عدد_صفاں.alloc (عدد_var_انکڑے)) أرجع خطأ;

    لكل (بنانشانی i = 0; i < عدد_var_انکڑے; i++)
    {
      إذا (inner_maps && !inner_maps.arrayZ[i].get_population ())
          استمر;
      tuple_variations_t var_انکڑے_tuples;
      بنانشانی شے_گنتی_2 = 0;
      إذا (!var_انکڑے_tuples.create_from_شے_var_انکڑے (varStore.get_sub_جدول (i),
                                                      orig_region_list,
                                                      axes_old_اشاریہ_ٹیگ_map,
                                                      شے_گنتی_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : خالی_سوچک))
        أرجع خطأ;

      var_انکڑے_عدد_صفاں.push (شے_گنتی_2);
      vars.push (std::move (var_انکڑے_tuples));
    }
    أرجع !vars.وچ_error () && !var_انکڑے_عدد_صفاں.وچ_error () && vars.لمائی == var_انکڑے_عدد_صفاں.لمائی;
  }

  منطقي instantiate_tuple_vars (ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>& normalized_axes_location,
                               ثابت hb_hashmap_t<hb_ٹیگ_t, TripleDistances>& axes_triple_distances,
                               منطقي build_regions = سچ)
  {
    optimize_scratch_t scratch;
    لكل (tuple_variations_t& tuple_vars : vars)
      إذا (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        أرجع خطأ;

    أرجع !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  بنانشانی جوڑو_vardata (بنانشانی شے_گنتی_2)
  {
    vars.push (tuple_variations_t ());
    var_انکڑے_عدد_صفاں.push (شے_گنتی_2);
    أرجع vars.لمائی - 1;
  }

  /* Get item count for a VarData subtable. */
  بنانشانی get_شے_گنتی (بنانشانی outer) ثابت
  { أرجع outer < var_انکڑے_عدد_صفاں.لمائی ? var_انکڑے_عدد_صفاں[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  بنانشانی duplicate_صف (بنانشانی outer, بنانشانی inner)
  {
    إذا (unlikely (outer >= vars.لمائی ||
                  inner >= var_انکڑے_عدد_صفاں[outer] ||
                  var_انکڑے_عدد_صفاں[outer] >= 0xFFFFu))
      أرجع (بنانشانی) -1;
    لكل (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      إذا (unlikely (inner >= tuple.deltas_x.لمائی ||
                    inner >= tuple.indices.لمائی))
        أرجع (بنانشانی) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      إذا (inner < tuple.deltas_y.لمائی)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      إذا (unlikely (tuple.indices.وچ_error () ||
                    tuple.deltas_x.وچ_error () ||
                    tuple.deltas_y.وچ_error ()))
        أرجع (بنانشانی) -1;
    }
    أرجع var_انکڑے_عدد_صفاں[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  فراغ جوڑو_tuple (بنانشانی outer,
                  hb_hashmap_t<hb_ٹیگ_t, Triple>&& axis_tuples,
                  بنانشانی inner, صحيح delta, بنانشانی شے_گنتی_2)
  {
    إذا (outer >= vars.لمائی) أرجع;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    إذا (!tuple.indices.resize (شے_گنتی_2) ||
        !tuple.deltas_x.resize (شے_گنتی_2)) أرجع;
    لكل (بنانشانی i = 0; i < شے_گنتی_2; i++)
    {
      tuple.indices.arrayZ[i] = سچ;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (عائم) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  منطقي build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, بنانشانی> all_regions;
    hb_hashmap_t<region_t, بنانشانی> ورتیا_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_سمتیہ_t<region_t> all_unique_regions;
    لكل (ثابت tuple_variations_t& sub_جدول_2 : vars)
    {
      لكل (ثابت tuple_delta_t& tuple : sub_جدول_2.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        إذا (!ورتیا_regions.has (r))
        {
          منطقي all_zeros = سچ;
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
            إذا (!ورتیا_regions.سیٹ (r, 1))
              أرجع خطأ;
          }
        }
        إذا (all_regions.has (r))
          استمر;
        إذا (!all_regions.سیٹ (r, 1))
          أرجع خطأ;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    إذا (!all_regions || !all_unique_regions) أرجع سچ;

    إذا (!region_list.alloc (all_regions.get_population ()))
      أرجع خطأ;

    بنانشانی idx = 0;
    /* append the original regions that pre-existed */
    لكل (ثابت خودکار& r : orig_region_list)
    {
      إذا (!all_regions.has (&r) || !ورتیا_regions.has (&r))
        استمر;

      region_list.push (&r);
      إذا (!region_map.سیٹ (&r, idx))
        أرجع خطأ;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    لكل (ثابت خودکار& r: all_unique_regions)
    {
      إذا (!all_regions.has (r) || !ورتیا_regions.has (r))
        استمر;
      region_list.push (r);
      إذا (!region_map.سیٹ (r, idx))
        أرجع خطأ;
      all_regions.del (r);
      idx++;
    }
    أرجع (!region_list.وچ_error ()) && (!region_map.وچ_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  بنية combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = افتراضي;
    combined_gain_idx_tuple_t (بنانشانی gain, بنانشانی i, بنانشانی j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      أكد (gain < 0xFFFFFF);
      أكد (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    منطقي عامل < (ثابت combined_gain_idx_tuple_t& o)
    {
      أرجع encoded < o.encoded;
    }

    منطقي عامل <= (ثابت combined_gain_idx_tuple_t& o)
    {
      أرجع encoded <= o.encoded;
    }

    بنانشانی idx_1 () ثابت { أرجع (encoded >> 20) & 0xFFFFF; };
    بنانشانی idx_2 () ثابت { أرجع encoded & 0xFFFFF; };
  };

  منطقي as_شے_varstore (منطقي optimize=سچ, منطقي use_no_variation_idx=سچ)
  {
    /* return true if no variation data */
    إذا (!region_list) أرجع سچ;
    بنانشانی عدد_خانے_گنتی = region_list.لمائی;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    بنانشانی کل_صفاں = 0;
    لكل (بنانشانی major = 0; major < var_انکڑے_عدد_صفاں.لمائی; major++)
      کل_صفاں += var_انکڑے_عدد_صفاں[major];

    إذا (!delta_صفاں.resize (کل_صفاں)) أرجع خطأ;
    /* init all rows to [0]*num_cols */
    لكل (بنانشانی i = 0; i < کل_صفاں; i++)
      إذا (!(delta_صفاں[i].resize (عدد_خانے_گنتی))) أرجع خطأ;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<بنانشانی, ثابت hb_سمتیہ_t<صحيح>*> front_mapping;
    بنانشانی شروع_صف = 0;
    hb_سمتیہ_t<delta_صف_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح> *> major_صفاں;
    hb_hashmap_t<ثابت hb_سمتیہ_t<صحيح>*, بنانشانی> delta_صفاں_map;
    لكل (بنانشانی major = 0; major < vars.لمائی; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ثابت tuple_variations_t& tuples = vars[major];
      بنانشانی عدد_صفاں = var_انکڑے_عدد_صفاں[major];

      إذا (!عدد_صفاں) استمر;

      لكل (ثابت tuple_delta_t& tuple: tuples.tuple_vars)
      {
        إذا (tuple.deltas_x.لمائی != عدد_صفاں)
          أرجع خطأ;

        /* skip unused regions */
        بنانشانی *خانہ_idx;
        إذا (!region_map.has (&(tuple.axis_tuples), &خانہ_idx))
          استمر;

        لكل (بنانشانی i = 0; i < عدد_صفاں; i++)
        {
          صحيح rounded_delta = hb_clamp_منزل<صحيح> (roundf ((مزدوج) tuple.deltas_x[i]));
          صحيح &delta = delta_صفاں[شروع_صف + i][*خانہ_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          إذا (unlikely (sum < INT_MIN || sum > ودھ_توں_ودھ_عدد)) أرجع خطأ;
          delta = (صحيح) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_صفاں.reset ();
      لكل (بنانشانی minor = 0; minor < عدد_صفاں; minor++)
      {
	ثابت hb_سمتیہ_t<صحيح>& صف = delta_صفاں[شروع_صف + minor];
	إذا (use_no_variation_idx)
	{
	  منطقي all_zeros = سچ;
	  لكل (صحيح delta : صف)
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

	إذا (!front_mapping.سیٹ ((major<<16) + minor, &صف))
	  أرجع خطأ;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	إذا (optimize)
	{
	  إذا (delta_صفاں_map.has (&صف))
	    استمر;

	  delta_صفاں_map.سیٹ (&صف, 1);
	}

	major_صفاں.push (&صف);
      }

      إذا (unlikely (major_صفاں.وچ_error () || delta_صفاں_map.وچ_error ()))
        أرجع خطأ;
      إذا (major_صفاں)
      {
	خودکار *encoding = encoding_objs.push (std::move (major_صفاں), عدد_خانے_گنتی);
	إذا (unlikely (encoding_objs.وچ_error () || encoding->وچ_error ()))
	  أرجع خطأ;
      }

      شروع_صف += عدد_صفاں;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    إذا (!optimize)
    {
      encodings = std::move (encoding_objs);
      أرجع !encodings.وچ_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.qsort ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    ورتوں شے_t = hb_ترجیح_قطار_t<combined_gain_idx_tuple_t>::شے_t;
    hb_سمتیہ_t<شے_t> قطار_اشیاء;
    بنانشانی عدد_todos = encoding_objs.لمائی;
    لكل (بنانشانی i = 0; i < عدد_todos; i++)
    {
      لكل (بنانشانی j = i + 1; j < عدد_todos; j++)
      {
        صحيح combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        إذا (combining_gain > 0)
          قطار_اشیاء.push (شے_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_ترجیح_قطار_t<combined_gain_idx_tuple_t> قطار (std::move (قطار_اشیاء));

    hb_ثنائی_ہندسہ_سیٹ_t removed_todo_idxes;
    طالما (قطار)
    {
      خودکار t = قطار.نکالو_minimum ().پہلا;
      بنانشانی i = t.idx_1 ();
      بنانشانی j = t.idx_2 ();

      إذا (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        استمر;

      delta_صف_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_صف_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.جوڑو (i);
      removed_todo_idxes.جوڑو (j);
      إذا (unlikely (removed_todo_idxes.وچ_error ())) أرجع خطأ;

      encoding.ملاؤ (other_encoding);
      إذا (unlikely (encoding.وچ_error ())) أرجع خطأ;

      لكل (بنانشانی idx = 0; idx < encoding_objs.لمائی; idx++)
      {
        إذا (removed_todo_idxes.has (idx)) استمر;

        ثابت delta_صف_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        إذا (obj.چوڑائی == encoding.چوڑائی && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          لكل (ثابت خودکار& صف : obj.اشیاء)
            encoding.جوڑو_صف (صف);
          إذا (unlikely (encoding.وچ_error ())) أرجع خطأ;

          removed_todo_idxes.جوڑو (idx);
          إذا (unlikely (removed_todo_idxes.وچ_error ())) أرجع خطأ;
          استمر;
        }

        صحيح combined_gain = encoding.gain_from_merging (obj);
        إذا (combined_gain > 0)
          قطار.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.لمائی), 0);
      }

      خودکار moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      إذا (unlikely (encoding_objs.وچ_error ())) أرجع خطأ;
    }

    صحيح عدد_final_encodings = (صحيح) encoding_objs.لمائی - (صحيح) removed_todo_idxes.get_population ();
    إذا (عدد_final_encodings <= 0) أرجع خطأ;

    إذا (!encodings.alloc (عدد_final_encodings)) أرجع خطأ;
    لكل (بنانشانی i = 0; i < encoding_objs.لمائی; i++)
    {
      إذا (removed_todo_idxes.has (i)) استمر;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    أرجع compile_varidx_map (front_mapping);
  }

  خاص:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  منطقي compile_varidx_map (ثابت hb_hashmap_t<بنانشانی, ثابت hb_سمتیہ_t<صحيح>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ثابت hb_سمتیہ_t<صحيح>*, بنانشانی> پٹھ_mapping;
    hb_سمتیہ_t<delta_صف_encoding_t> ونڈو_encodings;

    لكل (بنانشانی i = 0; i < encodings.لمائی; i++)
    {
      delta_صف_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      إذا (encoding.is_empty () || encoding.وچ_error ())
        أرجع خطأ;

      بنانشانی عدد_صفاں = encoding.اشیاء.لمائی;
      بنانشانی عدد_خانے_گنتی = encoding.chars.لمائی;

      /* sort rows, make result deterministic */
      encoding.اشیاء.qsort (cmp_صف);

      لكل (بنانشانی شروع = 0; شروع < عدد_صفاں; شروع += 0xFFFFu)
      {
        بنانشانی chunk_len = hb_min (عدد_صفاں - شروع, 0xFFFFu);
        hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح>*> صفاں;

        إذا (!صفاں.alloc (chunk_len))
          أرجع خطأ;

        بنانشانی major = ونڈو_encodings.لمائی;
        لكل (بنانشانی minor = 0; minor < chunk_len; minor++)
        {
          ثابت hb_سمتیہ_t<صحيح> *صف = encoding.اشیاء.arrayZ[شروع + minor];
          صفاں.push (صف);
          إذا (!پٹھ_mapping.سیٹ (صف, (major << 16) + minor))
            أرجع خطأ;
        }

        خودکار *ونڈو = ونڈو_encodings.push (std::move (صفاں), عدد_خانے_گنتی);
        إذا (unlikely (ونڈو_encodings.وچ_error () || ونڈو->وچ_error ()))
          أرجع خطأ;
      }
    }

    encodings = std::move (ونڈو_encodings);
    إذا (encodings.وچ_error () || پٹھ_mapping.وچ_error ())
      أرجع خطأ;

    لكل (خودکار _ : front_mapping.iter ())
    {
      بنانشانی old_varidx = _.پہلا;
      بنانشانی *new_varidx;
      إذا (پٹھ_mapping.has (_.second, &new_varidx))
        varidx_map.سیٹ (old_varidx, *new_varidx);
      وإلا
        varidx_map.سیٹ (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ);
    }
    أرجع !varidx_map.وچ_error ();
  }

  ساكن صحيح cmp_صف (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ثابت hb_سمتیہ_t<صحيح>** a = (ثابت hb_سمتیہ_t<صحيح>**) pa;
    ثابت hb_سمتیہ_t<صحيح>** b = (ثابت hb_سمتیہ_t<صحيح>**) pb;

    لكل (بنانشانی i = 0; i < (*b)->لمائی; i++)
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


#جے_انت /* HB_OT_VAR_COMMON_HH */

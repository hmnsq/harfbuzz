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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_VAR_COMMON_HH
#وصف_ڪريو HB_OT_VAR_COMMON_HH

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-alloc-pool.hh"
#ضمن "hb-priority-queue.hh"
#ضمن "hb-subset-instancer-iup.hh"


نطاق OT {

استعمال rebase_tent_نتيجو_scratch_t = hb_pair_t<rebase_tent_نتيجو_t, rebase_tent_نتيجو_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
بنية Tuple_Variation_سرو
{
  دوست بنية tuple_delta_t;
  size_t get_سائيز (بنانشان axis_ڳڻپ_times_2) ثابت
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    بنانشان ti = tuple_اشاريو;
    إذا (unlikely ((ti & (Tuple_اشاريو::EmbeddedPeakTuple | Tuple_اشاريو::IntermediateRegion))))
    {
      بنانشان ڳڻپ = ((ti & Tuple_اشاريو::EmbeddedPeakTuple) != 0) + ((ti & Tuple_اشاريو::IntermediateRegion) != 0) * 2;
      أرجع hb_unsigned_mul_جوڙيو_saturate (ڳڻپ, axis_ڳڻپ_times_2, min_سائيز);
    }
    أرجع min_سائيز;
  }

  بنانشان get_معلومات_سائيز () ثابت { أرجع var_معلومات_سائيز; }

  ثابت Tuple_Variation_سرو &get_ايندڙ (بنانشان axis_ڳڻپ_times_2) ثابت
  { أرجع Struct_At_وٿي<Tuple_Variation_سرو> (هذا, get_سائيز (axis_ڳڻپ_times_2)); }

  منطقي unpack_axis_tuples (بنانشان axis_ڳڻپ,
                           ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                           ثابت hb_map_t *axes_old_اشاريو_ٽيگ_map,
                           hb_hashmap_t<hb_ٽيگ_t, Triple>& axis_tuples /*ٻاهريون*/) ثابت
  {
    ثابت F2DOT14 *peak_tuple = خالي_اشارو;
    إذا (has_peak ())
      peak_tuple = get_peak_tuple (axis_ڳڻپ);
    وإلا
    {
      بنانشان صحيح اشاريو_2 = get_اشاريو ();
      إذا (unlikely ((اشاريو_2 + 1) * axis_ڳڻپ > shared_tuples.ڊيگهائي))
        أرجع خطأ;
      peak_tuple = shared_tuples.sub_array (axis_ڳڻپ * اشاريو_2, axis_ڳڻپ).arrayZ;
    }

    ثابت F2DOT14 *شروعات_tuple = خالي_اشارو;
    ثابت F2DOT14 *پڇاڙي_tuple = خالي_اشارو;
    منطقي has_interm = has_intermediate ();

    إذا (has_interm)
    {
      شروعات_tuple = get_شروعات_tuple (axis_ڳڻپ);
      پڇاڙي_tuple = get_پڇاڙي_tuple (axis_ڳڻپ);
    }

    لكل (بنانشان i = 0; i < axis_ڳڻپ; i++)
    {
      عائم peak = peak_tuple[i].منزل_float ();
      إذا (peak == 0.f) استمر;

      hb_ٽيگ_t *axis_ٽيگ_2;
      إذا (!axes_old_اشاريو_ٽيگ_map->has (i, &axis_ٽيگ_2))
        أرجع خطأ;

      عائم شروعات, پڇاڙي;
      إذا (has_interm)
      {
        شروعات = شروعات_tuple[i].منزل_float ();
        پڇاڙي = پڇاڙي_tuple[i].منزل_float ();
      }
      وإلا
      {
        شروعات = hb_min (peak, 0.f);
        پڇاڙي = hb_max (peak, 0.f);
      }
      axis_tuples.سيٽ (*axis_ٽيگ_2, Triple (شروعات, peak, پڇاڙي));
    }

    أرجع سچ;
  }

  HB_ALWAYS_INLINE
  عائم calculate_scalar (hb_array_t<ثابت صحيح> coords, بنانشان صحيح coord_ڳڻپ,
			   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) ثابت
  {
    بنانشان tuple_اشاريو_2 = tuple_اشاريو;
    بنانشان صحيح اشاريو_2 = tuple_اشاريو_2 & Tuple_اشاريو::Tuple_اشاريو_نقاب;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    منطقي plain_shared_tuple = (tuple_اشاريو_2 & ~Tuple_اشاريو::ذاتي_نقطو_Numbers) < shared_tuple_scalar_cache->ڊيگهائي;
    إذا (likely (plain_shared_tuple))
    {
      عائم scalar;
      إذا (likely (shared_tuple_scalar_cache->get (اشاريو_2, &scalar)))
	أرجع scalar;
    }

    ثابت F2DOT14 *peak_tuple;

    منطقي has_interm = tuple_اشاريو_2 & Tuple_اشاريو::IntermediateRegion; // Inlined for performance

    إذا (unlikely (tuple_اشاريو_2 & Tuple_اشاريو::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_ڳڻپ);
      shared_tuple_scalar_cache = خالي_اشارو;
    }
    وإلا
    {
      عائم scalar;
      إذا (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->ڊيگهائي &&
	  shared_tuple_scalar_cache->get (اشاريو_2, &scalar))
      {
        إذا (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = خالي_اشارو;
	وإلا
	  أرجع scalar;
      }

      إذا (unlikely ((اشاريو_2 + 1) * coord_ڳڻپ > shared_tuples.ڊيگهائي))
        أرجع 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_ڳڻپ * اشاريو_2);

    }

    ثابت F2DOT14 *شروعات_tuple = خالي_اشارو;
    ثابت F2DOT14 *پڇاڙي_tuple = خالي_اشارو;

    إذا (has_interm)
    {
      شروعات_tuple = get_شروعات_tuple (coord_ڳڻپ);
      پڇاڙي_tuple = get_پڇاڙي_tuple (coord_ڳڻپ);
    }

    عائم scalar = 1.f;
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
#إذا HB_FAST_انگ_ACCESS
    منطقي skip = coord_ڳڻپ >= 16;
#جيڪڏهن_پورو
#جيڪڏهن_پورو
    لكل (بنانشان صحيح i = 0; i < coord_ڳڻپ; i++)
    {
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
#إذا HB_FAST_انگ_ACCESS
      إذا (skip)
      {
	طالما (i + 4 <= coord_ڳڻپ && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	طالما (i < coord_ڳڻپ && peak_tuple[i].منزل_int () == 0)
	  i += 1;
	إذا (i >= coord_ڳڻپ)
	  اكسر;
      }
#جيڪڏهن_پورو
#جيڪڏهن_پورو

      صحيح peak = peak_tuple[i].منزل_int ();
      إذا (!peak) استمر;

      صحيح v = coords[i];
      إذا (!v) { scalar = 0.f; اكسر; }
      إذا (v == peak) استمر;

      إذا (has_interm)
      {
	shared_tuple_scalar_cache = خالي_اشارو;
        صحيح شروعات = شروعات_tuple[i].منزل_int ();
        صحيح پڇاڙي = پڇاڙي_tuple[i].منزل_int ();
        إذا (unlikely (شروعات > peak || peak > پڇاڙي ||
                      (شروعات < 0 && پڇاڙي > 0 && peak))) استمر;
        إذا (v < شروعات || v > پڇاڙي) { scalar = 0.f; اكسر; }
        إذا (v < peak)
        { إذا (peak != شروعات) scalar *= (عائم) (v - شروعات) / (peak - شروعات); }
        وإلا
        { إذا (peak != پڇاڙي) scalar *= (عائم) (پڇاڙي - v) / (پڇاڙي - peak); }
      }
      وإلا إذا (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; اكسر; }
      وإلا
        scalar *= (عائم) v / peak;
    }
    إذا (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->سيٽ (اشاريو_2, scalar);
    أرجع scalar;
  }

  منطقي           has_peak () ثابت { أرجع tuple_اشاريو & Tuple_اشاريو::EmbeddedPeakTuple; }
  منطقي   has_intermediate () ثابت { أرجع tuple_اشاريو & Tuple_اشاريو::IntermediateRegion; }
  منطقي has_ذاتي_points () ثابت { أرجع tuple_اشاريو & Tuple_اشاريو::ذاتي_نقطو_Numbers; }
  بنانشان      get_اشاريو () ثابت { أرجع tuple_اشاريو & Tuple_اشاريو::Tuple_اشاريو_نقاب; }
  ساكن constexpr بنانشان max_shared_tuple_ڳڻپ = 0x1000u;

  محمي:
  بنية Tuple_اشاريو : HBUINT16
  {
    تعداد Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      ذاتي_نقطو_Numbers = 0x2000u,
      Tuple_اشاريو_نقاب      = 0x0FFFu
    };

    Tuple_اشاريو& عامل = (uint16_t i) { HBUINT16::عامل= (i); أرجع *هذا; }
    DEFINE_سائيز_STATIC (2);
  };

  hb_array_t<ثابت F2DOT14> get_all_tuples (بنانشان axis_ڳڻپ) ثابت
  { أرجع StructAfter<Unsized_Array_جو<F2DOT14>> (tuple_اشاريو).as_array ((has_peak () + has_intermediate () * 2) * axis_ڳڻپ); }
  ثابت F2DOT14* get_all_tuples_بنياد (بنانشان axis_ڳڻپ) ثابت
  { أرجع StructAfter<Unsized_Array_جو<F2DOT14>> (tuple_اشاريو).arrayZ; }
  ثابت F2DOT14* get_peak_tuple (بنانشان axis_ڳڻپ) ثابت
  { أرجع get_all_tuples_بنياد (axis_ڳڻپ); }
  ثابت F2DOT14* get_شروعات_tuple (بنانشان axis_ڳڻپ) ثابت
  { أرجع get_all_tuples_بنياد (axis_ڳڻپ) + has_peak () * axis_ڳڻپ; }
  ثابت F2DOT14* get_پڇاڙي_tuple (بنانشان axis_ڳڻپ) ثابت
  { أرجع get_all_tuples_بنياد (axis_ڳڻپ) + has_peak () * axis_ڳڻپ + axis_ڳڻپ; }

  HBUINT16      var_معلومات_سائيز;    /* The size in bytes of the serialized
                                 * data for this tuple variation table. */
  Tuple_اشاريو    tuple_اشاريو;     /* A packed field. The high 4 bits are flags (see below).
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
  DEFINE_سائيز_MIN (4);
};

بنية optimize_scratch_t
{
  iup_scratch_t iup;
  hb_ويڪٽر_t<منطقي> opt_indices;
  hb_ويڪٽر_t<صحيح> rounded_x_deltas;
  hb_ويڪٽر_t<صحيح> rounded_y_deltas;
  hb_ويڪٽر_t<عائم> opt_deltas_x;
  hb_ويڪٽر_t<عائم> opt_deltas_y;
  hb_ويڪٽر_t<بنانشان حرف> opt_نقطو_معلومات;
  hb_ويڪٽر_t<بنانشان حرف> opt_deltas_معلومات;
  hb_ويڪٽر_t<بنانشان حرف> نقطو_معلومات;
  hb_ويڪٽر_t<بنانشان حرف> deltas_معلومات;
  hb_ويڪٽر_t<صحيح> rounded_deltas;
};

بنية tuple_delta_t
{
  ساكن constexpr منطقي realloc_move = سچ;  // Watch out when adding new members!

  عام:
  hb_hashmap_t<hb_ٽيگ_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_ويڪٽر_t<منطقي> indices;

  hb_ويڪٽر_t<عائم> deltas_x;
  /* empty for cvar tuples */
  hb_ويڪٽر_t<عائم> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_ويڪٽر_t<بنانشان حرف> compiled_tuple_سرو;
  hb_ويڪٽر_t<بنانشان حرف> compiled_deltas;

  hb_ويڪٽر_t<F2DOT14> compiled_peak_coords;
  hb_ويڪٽر_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = خالي_اشارو) {}
  tuple_delta_t (ثابت tuple_delta_t& o) = افتراضي;
  tuple_delta_t& عامل = (ثابت tuple_delta_t& o) = افتراضي;

  دوست فراغ swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
  {
    hb_مٽا_سٽا (a.axis_tuples, b.axis_tuples);
    hb_مٽا_سٽا (a.indices, b.indices);
    hb_مٽا_سٽا (a.deltas_x, b.deltas_x);
    hb_مٽا_سٽا (a.deltas_y, b.deltas_y);
    hb_مٽا_سٽا (a.compiled_tuple_سرو, b.compiled_tuple_سرو);
    hb_مٽا_سٽا (a.compiled_deltas, b.compiled_deltas);
    hb_مٽا_سٽا (a.compiled_peak_coords, b.compiled_peak_coords);
  }

  tuple_delta_t (tuple_delta_t&& o)  noexcept : tuple_delta_t ()
  { hb_مٽا_سٽا (*هذا, o); }

  tuple_delta_t& عامل = (tuple_delta_t&& o) noexcept
  {
    hb_مٽا_سٽا (*هذا, o);
    أرجع *هذا;
  }

  فراغ copy_from (ثابت tuple_delta_t& o, hb_alloc_pool_t *pool = خالي_اشارو)
  {
    axis_tuples = o.axis_tuples;
    indices.duplicate_ويڪٽر_from_pool (pool, o.indices);
    deltas_x.duplicate_ويڪٽر_from_pool (pool, o.deltas_x);
    deltas_y.duplicate_ويڪٽر_from_pool (pool, o.deltas_y);
    compiled_tuple_سرو.duplicate_ويڪٽر_from_pool (pool, o.compiled_tuple_سرو);
    compiled_deltas.duplicate_ويڪٽر_from_pool (pool, o.compiled_deltas);
    compiled_peak_coords.duplicate_ويڪٽر_from_pool (pool, o.compiled_peak_coords);
    compiled_interm_coords.duplicate_ويڪٽر_from_pool (pool, o.compiled_interm_coords);
  }

  فراغ remove_axis (hb_ٽيگ_t axis_ٽيگ_2)
  { axis_tuples.del (axis_ٽيگ_2); }

  منطقي سيٽ_tent (hb_ٽيگ_t axis_ٽيگ_2, Triple tent)
  { أرجع axis_tuples.سيٽ (axis_ٽيگ_2, tent); }

  tuple_delta_t& عامل += (ثابت tuple_delta_t& o)
  {
    بنانشان انگ = indices.ڊيگهائي;
    لكل (بنانشان i = 0; i < انگ; i++)
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

    بنانشان انگ = indices.ڊيگهائي;
    إذا (deltas_y)
      لكل (بنانشان i = 0; i < انگ; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    وإلا
      لكل (بنانشان i = 0; i < انگ; i++)
      {
	إذا (!indices.arrayZ[i]) استمر;
	deltas_x[i] *= scalar;
      }
    أرجع *هذا;
  }

  فراغ change_tuple_var_axis_حد_بندي (hb_ٽيگ_t axis_ٽيگ_2, Triple axis_حد_بندي,
				    TripleDistances axis_triple_distances,
				    hb_ويڪٽر_t<tuple_delta_t>& ٻاهريون,
				    rebase_tent_نتيجو_scratch_t &scratch,
				    hb_alloc_pool_t *pool = خالي_اشارو)
  {
    // May move *this out.

    ٻاهريون.reset ();
    Triple *tent;
    إذا (!axis_tuples.has (axis_ٽيگ_2, &tent))
    {
      ٻاهريون.push (std::move (*هذا));
      أرجع;
    }

    إذا ((tent->minimum < 0.f && tent->وڌ_کان_وڌ > 0.f) ||
        !(tent->minimum <= tent->وچ && tent->وچ <= tent->وڌ_کان_وڌ))
      أرجع;

    إذا (tent->وچ == 0.f)
    {
      ٻاهريون.push (std::move (*هذا));
      أرجع;
    }

    إذا (!axis_حد_بندي.is_نقطو () &&
        !(-1.f <= axis_حد_بندي.minimum &&
          axis_حد_بندي.minimum <= axis_حد_بندي.وچ &&
          axis_حد_بندي.وچ <= axis_حد_بندي.وڌ_کان_وڌ &&
          axis_حد_بندي.وڌ_کان_وڌ <= +1.f))
      أرجع;

    rebase_tent_نتيجو_t &حل = scratch.پهريون;
    rebase_tent (*tent, axis_حد_بندي, axis_triple_distances, حل, scratch.second);
    لكل (بنانشان i = 0; i < حل.ڊيگهائي; i++)
    {
      خودڪار &t = حل.arrayZ[i];

      tuple_delta_t new_var;
      إذا (i < حل.ڊيگهائي - 1)
	new_var.copy_from (*هذا, pool);
      وإلا
	new_var = std::move (*هذا);

      إذا (t.second == Triple ())
        new_var.remove_axis (axis_ٽيگ_2);
      وإلا
        new_var.سيٽ_tent (axis_ٽيگ_2, t.second);

      new_var *= t.پهريون;
      ٻاهريون.push (std::move (new_var));
    }
  }

  منطقي compile_coords (ثابت hb_map_t& axes_اشاريو_map,
		       ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map,
		       hb_alloc_pool_t *pool= خالي_اشارو)
  {
    بنانشان cur_axis_ڳڻپ = axes_اشاريو_map.get_population ();
    إذا (pool)
    {
      إذا (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_ڳڻپ)))
	أرجع خطأ;
    }
    وإلا إذا (unlikely (!compiled_peak_coords.resize (cur_axis_ڳڻپ)))
      أرجع خطأ;

    hb_array_t<F2DOT14> شروعات_coords, پڇاڙي_coords;

    بنانشان orig_axis_ڳڻپ = axes_old_اشاريو_ٽيگ_map.get_population ();
    بنانشان j = 0;
    لكل (بنانشان i = 0; i < orig_axis_ڳڻپ; i++)
    {
      إذا (!axes_اشاريو_map.has (i))
        استمر;

      hb_ٽيگ_t axis_ٽيگ_2 = axes_old_اشاريو_ٽيگ_map.get (i);
      Triple *coords = خالي_اشارو;
      إذا (axis_tuples.has (axis_ٽيگ_2, &coords))
      {
	عائم min_val = coords->minimum;
	عائم val = coords->وچ;
	عائم max_val = coords->وڌ_کان_وڌ;

	compiled_peak_coords.arrayZ[j].سيٽ_float (val);

	إذا (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  إذا (!compiled_interm_coords)
	  {
	    إذا (pool)
	    {
	      إذا (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_ڳڻپ)))
		أرجع خطأ;
	    }
	    وإلا إذا (unlikely (!compiled_interm_coords.resize (2 * cur_axis_ڳڻپ)))
	      أرجع خطأ;
	    شروعات_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_ڳڻپ);
	    پڇاڙي_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_ڳڻپ);

	    لكل (بنانشان k = 0; k < j; k++)
	    {
	      نشانوارو peak = compiled_peak_coords.arrayZ[k].منزل_int ();
	      إذا (!peak) استمر;
	      شروعات_coords.arrayZ[k].سيٽ_int (hb_min (peak, 0));
	      پڇاڙي_coords.arrayZ[k].سيٽ_int (hb_max (peak, 0));
	    }
	  }

	}

	إذا (compiled_interm_coords)
	{
	  شروعات_coords.arrayZ[j].سيٽ_float (min_val);
	  پڇاڙي_coords.arrayZ[j].سيٽ_float (max_val);
	}
      }

      j++;
    }

    أرجع !compiled_peak_coords.اندريون_error () && !compiled_interm_coords.اندريون_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  منطقي compile_tuple_var_سرو (ثابت hb_map_t& axes_اشاريو_map,
                                 بنانشان points_معلومات_ڊيگهائي,
                                 ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map,
                                 ثابت hb_hashmap_t<ثابت hb_ويڪٽر_t<F2DOT14>*, بنانشان>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = خالي_اشارو)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    إذا (!compiled_deltas) أرجع سچ;

    بنانشان cur_axis_ڳڻپ = axes_اشاريو_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    بنانشان alloc_len = 3 * cur_axis_ڳڻپ * (F2DOT14::static_سائيز) + 4;
    إذا (unlikely (!compiled_tuple_سرو.allocate_from_pool (pool, alloc_len, خطأ))) أرجع خطأ;

    بنانشان flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرو.begin () + 4);
    F2DOT14* پڇاڙي = reinterpret_cast<F2DOT14 *> (compiled_tuple_سرو.پڇاڙي ());
    hb_array_t<F2DOT14> coords (p, پڇاڙي - p);

    إذا (!shared_tuples_idx_map)
      compile_coords (axes_اشاريو_map, axes_old_اشاريو_ٽيگ_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    بنانشان peak_ڳڻپ = 0;
    بنانشان *shared_tuple_idx;
    إذا (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    وإلا
    {
      peak_ڳڻپ = encode_peak_coords(coords, flag);
      إذا (!peak_ڳڻپ) أرجع خطأ;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    بنانشان interim_ڳڻپ = encode_interm_coords (coords.sub_array (peak_ڳڻپ), flag);

    /* pointdata length = 0 implies "use shared points" */
    إذا (points_معلومات_ڊيگهائي)
      flag |= Tuple_Variation_سرو::Tuple_اشاريو::ذاتي_نقطو_Numbers;

    بنانشان serialized_معلومات_سائيز = points_معلومات_ڊيگهائي + compiled_deltas.ڊيگهائي;
    Tuple_Variation_سرو *o = reinterpret_cast<Tuple_Variation_سرو *> (compiled_tuple_سرو.begin ());
    o->var_معلومات_سائيز = serialized_معلومات_سائيز;
    o->tuple_اشاريو = flag;

    بنانشان ڪل_سرو_len = 4 + (peak_ڳڻپ + interim_ڳڻپ) * (F2DOT14::static_سائيز);
    compiled_tuple_سرو.shrink_پٺ_منزل_pool (pool, ڪل_سرو_len);
    أرجع سچ;
  }

  بنانشان encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               بنانشان& flag) ثابت
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.ڊيگهائي * حجم (compiled_peak_coords[0]));
    flag |= Tuple_Variation_سرو::Tuple_اشاريو::EmbeddedPeakTuple;
    أرجع compiled_peak_coords.ڊيگهائي;
  }

  /* if no need to encode intermediate coords, then just return p */
  بنانشان encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 بنانشان& flag) ثابت
  {
    إذا (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.ڊيگهائي * حجم (compiled_interm_coords[0]));
      flag |= Tuple_Variation_سرو::Tuple_اشاريو::IntermediateRegion;
    }
    أرجع compiled_interm_coords.ڊيگهائي;
  }

  منطقي compile_deltas (hb_ويڪٽر_t<صحيح> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = خالي_اشارو)
  { أرجع compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  ساكن منطقي compile_deltas (hb_array_t<ثابت منطقي> نقطو_indices,
			      hb_array_t<ثابت عائم> x_deltas,
			      hb_array_t<ثابت عائم> y_deltas,
			      hb_ويڪٽر_t<بنانشان حرف> &compiled_deltas, /*ٻاهريون*/
			      hb_ويڪٽر_t<صحيح> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = خالي_اشارو)
  {
    إذا (unlikely (!rounded_deltas.resize_dirty  (نقطو_indices.ڊيگهائي)))
      أرجع خطأ;

    بنانشان j = 0;
    لكل (بنانشان i = 0; i < نقطو_indices.ڊيگهائي; i++)
    {
      إذا (!نقطو_indices[i]) استمر;
      rounded_deltas.arrayZ[j++] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    إذا (!rounded_deltas) أرجع سچ;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    بنانشان alloc_len = 2 + 4 * rounded_deltas.ڊيگهائي + (rounded_deltas.ڊيگهائي + 63) / 64;
    إذا (y_deltas)
      alloc_len *= 2;

    إذا (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, خطأ))) أرجع خطأ;

    بنانشان encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    إذا (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      بنانشان j = 0;
      لكل (بنانشان idx = 0; idx < نقطو_indices.ڊيگهائي; idx++)
      {
        إذا (!نقطو_indices[idx]) استمر;
        صحيح rounded_delta = hb_clamp_منزل<صحيح> (roundf ((مزدوج) y_deltas.arrayZ[idx]));

        إذا (j >= rounded_deltas.ڊيگهائي) أرجع خطأ;

        rounded_deltas[j++] = rounded_delta;
      }

      إذا (j != rounded_deltas.ڊيگهائي) أرجع خطأ;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_پٺ_منزل_pool (pool, encoded_len);
    أرجع سچ;
  }

  ساكن بنانشان compile_deltas (hb_array_t<بنانشان حرف> encoded_بائيٽون,
				  hb_array_t<ثابت صحيح> deltas)
  {
    أرجع Tuple_قدرون::compile_unsafe (deltas, encoded_بائيٽون);
  }

  منطقي calc_inferred_deltas (ثابت contour_نقطو_ويڪٽر_t& orig_points,
			     hb_ويڪٽر_t<بنانشان> &scratch)
  {
    بنانشان نقطو_ڳڻپ = orig_points.ڊيگهائي;
    إذا (نقطو_ڳڻپ != indices.ڊيگهائي)
      أرجع خطأ;

    بنانشان ref_ڳڻپ_2 = 0;

    hb_ويڪٽر_t<بنانشان> &پڇاڙي_points = scratch.reset ();

    لكل (بنانشان i = 0; i < نقطو_ڳڻپ; i++)
    {
      ref_ڳڻپ_2 += indices.arrayZ[i];
      إذا (orig_points.arrayZ[i].is_پڇاڙي_نقطو)
        پڇاڙي_points.push (i);
    }
    /* all points are referenced, nothing to do */
    إذا (ref_ڳڻپ_2 == نقطو_ڳڻپ)
      أرجع سچ;
    إذا (unlikely (پڇاڙي_points.اندريون_error ())) أرجع خطأ;

    hb_ٻٽو_انگ_سيٽ_t inferred_idxes;
    بنانشان شروعات_نقطو_2 = 0;
    لكل (بنانشان پڇاڙي_نقطو : پڇاڙي_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      بنانشان unref_ڳڻپ = 0;
      لكل (بنانشان i = شروعات_نقطو_2; i < پڇاڙي_نقطو + 1; i++)
        unref_ڳڻپ += indices.arrayZ[i];
      unref_ڳڻپ = (پڇاڙي_نقطو - شروعات_نقطو_2 + 1) - unref_ڳڻپ;

      بنانشان j = شروعات_نقطو_2;
      إذا (unref_ڳڻپ == 0 || unref_ڳڻپ > پڇاڙي_نقطو - شروعات_نقطو_2)
        اذهب no_وڌيڪ_gaps;
      لكل (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        بنانشان صحيح prev, next, i;
        لكل (;;)
        {
          i = j;
          j = ايندڙ_اشاريو (i, شروعات_نقطو_2, پڇاڙي_نقطو);
          إذا (indices.arrayZ[i] && !indices.arrayZ[j]) اكسر;
        }
        prev = j = i;
        لكل (;;)
        {
          i = j;
          j = ايندڙ_اشاريو (i, شروعات_نقطو_2, پڇاڙي_نقطو);
          إذا (!indices.arrayZ[i] && indices.arrayZ[j]) اكسر;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        لكل (;;)
        {
          i = ايندڙ_اشاريو (i, شروعات_نقطو_2, پڇاڙي_نقطو);
          إذا (i == next) اكسر;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.جوڙيو (i);
          إذا (--unref_ڳڻپ == 0) اذهب no_وڌيڪ_gaps;
        }
      }
    no_وڌيڪ_gaps:
      شروعات_نقطو_2 = پڇاڙي_نقطو + 1;
    }

    لكل (بنانشان i = 0; i < نقطو_ڳڻپ; i++)
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

  منطقي optimize (ثابت contour_نقطو_ويڪٽر_t& contour_points,
                 منطقي is_مرکب,
		 optimize_scratch_t &scratch,
                 مزدوج tolerance = 0.5 + 1e-10)
  {
    بنانشان ڳڻپ = contour_points.ڊيگهائي;
    إذا (deltas_x.ڊيگهائي != ڳڻپ ||
        deltas_y.ڊيگهائي != ڳڻپ)
      أرجع خطأ;

    hb_ويڪٽر_t<منطقي> &opt_indices = scratch.opt_indices.reset ();
    hb_ويڪٽر_t<صحيح> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_ويڪٽر_t<صحيح> &rounded_y_deltas = scratch.rounded_y_deltas;

    إذا (unlikely (!rounded_x_deltas.resize_dirty  (ڳڻپ) ||
                  !rounded_y_deltas.resize_dirty  (ڳڻپ)))
      أرجع خطأ;

    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_منزل<صحيح> (roundf ((مزدوج) deltas_y.arrayZ[i]));
    }

    إذا (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      أرجع خطأ;

    بنانشان ref_ڳڻپ_2 = 0;
    لكل (منطقي ref_flag : opt_indices)
       ref_ڳڻپ_2 += ref_flag;

    إذا (ref_ڳڻپ_2 == ڳڻپ) أرجع سچ;

    hb_ويڪٽر_t<عائم> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_ويڪٽر_t<عائم> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    منطقي is_comp_glyph_wo_deltas = (is_مرکب && ref_ڳڻپ_2 == 0);
    إذا (is_comp_glyph_wo_deltas)
    {
      إذا (unlikely (!opt_deltas_x.resize (ڳڻپ) ||
                    !opt_deltas_y.resize (ڳڻپ)))
        أرجع خطأ;

      opt_indices.arrayZ[0] = سچ;
      لكل (بنانشان i = 1; i < ڳڻپ; i++)
        opt_indices.arrayZ[i] = خطأ;
    }

    hb_ويڪٽر_t<بنانشان حرف> &opt_نقطو_معلومات = scratch.opt_نقطو_معلومات.reset ();
    إذا (!compile_نقطو_سيٽ (opt_indices, opt_نقطو_معلومات))
      أرجع خطأ;
    hb_ويڪٽر_t<بنانشان حرف> &opt_deltas_معلومات = scratch.opt_deltas_معلومات.reset ();
    إذا (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_معلومات,
			 scratch.rounded_deltas))
      أرجع خطأ;

    hb_ويڪٽر_t<بنانشان حرف> &نقطو_معلومات = scratch.نقطو_معلومات.reset ();
    إذا (!compile_نقطو_سيٽ (indices, نقطو_معلومات))
      أرجع خطأ;
    hb_ويڪٽر_t<بنانشان حرف> &deltas_معلومات = scratch.deltas_معلومات.reset ();
    إذا (!compile_deltas (indices, deltas_x, deltas_y, deltas_معلومات, scratch.rounded_deltas))
      أرجع خطأ;

    إذا (opt_نقطو_معلومات.ڊيگهائي + opt_deltas_معلومات.ڊيگهائي < نقطو_معلومات.ڊيگهائي + deltas_معلومات.ڊيگهائي)
    {
      indices = std::move (opt_indices);

      إذا (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    أرجع !indices.اندريون_error () && !deltas_x.اندريون_error () && !deltas_y.اندريون_error ();
  }

  ساكن منطقي compile_نقطو_سيٽ (ثابت hb_ويڪٽر_t<منطقي> &نقطو_indices,
                                 hb_ويڪٽر_t<بنانشان حرف>& compiled_points /*ٻاهريون*/)
  {
    بنانشان انگ_points = 0;
    لكل (منطقي i : نقطو_indices)
      إذا (i) انگ_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    إذا (!انگ_points) أرجع سچ;

    بنانشان indices_ڊيگهائي = نقطو_indices.ڊيگهائي;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    إذا (انگ_points == indices_ڊيگهائي)
      أرجع compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    بنانشان انگ_بائيٽون = 2 + 3 *انگ_points;
    إذا (unlikely (!compiled_points.resize_dirty  (انگ_بائيٽون)))
      أرجع خطأ;

    بنانشان pos = 0;
    /* binary data starts with the total number of reference points */
    إذا (انگ_points < 0x80)
      compiled_points.arrayZ[pos++] = انگ_points;
    وإلا
    {
      compiled_points.arrayZ[pos++] = ((انگ_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = انگ_points & 0xFF;
    }

    ثابت بنانشان max_لاڳيتو_ڊيگهائي = 0x7F;
    بنانشان i = 0;
    بنانشان آخري_قدر = 0;
    بنانشان انگ_encoded = 0;
    طالما (i < indices_ڊيگهائي && انگ_encoded < انگ_points)
    {
      بنانشان لاڳيتو_ڊيگهائي = 0;
      بنانشان سرو_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      منطقي use_byte_encoding = خطأ;
      منطقي new_لاڳيتو_2 = سچ;
      طالما (i < indices_ڊيگهائي && انگ_encoded < انگ_points &&
             لاڳيتو_ڊيگهائي <= max_لاڳيتو_ڊيگهائي)
      {
        // find out next referenced point index
        طالما (i < indices_ڊيگهائي && !نقطو_indices[i])
          i++;

        إذا (i >= indices_ڊيگهائي) اكسر;

        بنانشان cur_قدر = i;
        بنانشان delta = cur_قدر - آخري_قدر;

        إذا (new_لاڳيتو_2)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_لاڳيتو_2 = خطأ;
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
        آخري_قدر = cur_قدر;
        لاڳيتو_ڊيگهائي++;
        انگ_encoded++;
      }

      إذا (use_byte_encoding)
        compiled_points.arrayZ[سرو_pos] = لاڳيتو_ڊيگهائي - 1;
      وإلا
        compiled_points.arrayZ[سرو_pos] = (لاڳيتو_ڊيگهائي - 1) | 0x80;
    }
    أرجع compiled_points.resize_dirty  (pos);
  }

  ساكن عائم infer_delta (عائم هدف_val, عائم prev_val, عائم ايندڙ_val, عائم prev_delta, عائم ايندڙ_delta)
  {
    إذا (prev_val == ايندڙ_val)
      أرجع (prev_delta == ايندڙ_delta) ? prev_delta : 0.f;
    وإلا إذا (هدف_val <= hb_min (prev_val, ايندڙ_val))
      أرجع (prev_val < ايندڙ_val) ? prev_delta : ايندڙ_delta;
    وإلا إذا (هدف_val >= hb_max (prev_val, ايندڙ_val))
      أرجع (prev_val > ايندڙ_val) ? prev_delta : ايندڙ_delta;

    عائم r = (هدف_val - prev_val) / (ايندڙ_val - prev_val);
    أرجع prev_delta + r * (ايندڙ_delta - prev_delta);
  }

  ساكن بنانشان صحيح ايندڙ_اشاريو (بنانشان صحيح i, بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
  { أرجع (i >= پڇاڙي) ? شروعات : (i + 1); }
};

سانچو <قسم_نالو OffType = HBUINT16>
بنية Tuple_Variation_معلومات
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (هذا));
  }

  size_t get_سائيز (بنانشان axis_ڳڻپ_times_2) ثابت
  {
    size_t ڪل_سائيز = min_سائيز;
    بنانشان ڳڻپ = tuple_Var_ڳڻپ.get_ڳڻپ ();
    ثابت Tuple_Variation_سرو *tuple_var_سرو = &(get_tuple_var_سرو());
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      ڪل_سائيز = hb_unsigned_جوڙيو_saturate (ڪل_سائيز,
					     tuple_var_سرو->get_سائيز (axis_ڳڻپ_times_2),
					     (size_t) tuple_var_سرو->get_معلومات_سائيز ());
      tuple_var_سرو = &tuple_var_سرو->get_ايندڙ (axis_ڳڻپ_times_2);
    }

    أرجع ڪل_سائيز;
  }

  ثابت Tuple_Variation_سرو &get_tuple_var_سرو (فراغ) ثابت
  { أرجع StructAfter<Tuple_Variation_سرو> (معلومات); }

  بنية tuple_iterator_t;
  بنية tuple_variations_t
  {
    hb_ويڪٽر_t<tuple_delta_t> tuple_vars;

    خاص:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<ثابت hb_ويڪٽر_t<منطقي>*, hb_ويڪٽر_t<بنانشان حرف>> نقطو_معلومات_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<ثابت hb_ويڪٽر_t<منطقي>*, بنانشان> نقطو_سيٽ_ڳڻپ_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_ويڪٽر_t<بنانشان حرف> *shared_points_بائيٽون = خالي_اشارو;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    بنانشان compiled_byte_سائيز = 0;
    منطقي needs_padding = خطأ;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    منطقي is_مرکب = خطأ;

    عام:
    tuple_variations_t () = افتراضي;
    tuple_variations_t (ثابت tuple_variations_t&) = ميٽيو;
    tuple_variations_t& عامل=(ثابت tuple_variations_t&) = ميٽيو;
    tuple_variations_t (tuple_variations_t&&) = افتراضي;
    tuple_variations_t& عامل=(tuple_variations_t&&) = افتراضي;
    ~tuple_variations_t () = افتراضي;

    واضح عامل منطقي () ثابت { أرجع منطقي (tuple_vars); }
    بنانشان get_var_ڳڻپ () ثابت
    {
      بنانشان ڳڻپ = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      لكل (خودڪار& tuple: tuple_vars)
        إذا (tuple.compiled_deltas) ڳڻپ++;

      إذا (shared_points_بائيٽون && shared_points_بائيٽون->ڊيگهائي)
        ڳڻپ |= Tuple_Var_ڳڻپ::Shared_نقطو_Numbers;
      أرجع ڳڻپ;
    }

    بنانشان get_compiled_byte_سائيز () ثابت
    { أرجع compiled_byte_سائيز; }

    منطقي create_from_tuple_var_معلومات (tuple_iterator_t iterator,
                                     بنانشان tuple_var_ڳڻپ,
                                     بنانشان نقطو_ڳڻپ,
                                     منطقي is_gvar,
                                     ثابت hb_map_t *axes_old_اشاريو_ٽيگ_map,
                                     ثابت hb_ويڪٽر_t<بنانشان> &shared_indices,
                                     ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = خالي_اشارو,
                                     منطقي is_مرکب_glyph = خطأ)
    {
      hb_ويڪٽر_t<بنانشان> ذاتي_indices;
      hb_ويڪٽر_t<صحيح> deltas_x;
      hb_ويڪٽر_t<صحيح> deltas_y;
      افعل
      {
        ثابت HBUINT8 *p = iterator.get_serialized_معلومات ();
        بنانشان صحيح ڊيگهائي = iterator.current_tuple->get_معلومات_سائيز ();
        إذا (unlikely (!iterator.var_معلومات_بائيٽون.check_range (p, ڊيگهائي)))
          أرجع خطأ;

	hb_hashmap_t<hb_ٽيگ_t, Triple> axis_tuples;
        إذا (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_ڳڻپ (), shared_tuples, axes_old_اشاريو_ٽيگ_map, axis_tuples)
            || axis_tuples.is_empty ())
          أرجع خطأ;

	ذاتي_indices.reset ();
        منطقي has_ذاتي_points = iterator.current_tuple->has_ذاتي_points ();
        ثابت HBUINT8 *پڇاڙي = p + ڊيگهائي;
        إذا (has_ذاتي_points &&
            !Tuple_Variation_معلومات::decompile_points (p, ذاتي_indices, پڇاڙي))
          أرجع خطأ;

        ثابت hb_ويڪٽر_t<بنانشان> &indices = has_ذاتي_points ? ذاتي_indices : shared_indices;
        منطقي apply_منزل_all = (indices.ڊيگهائي == 0);
        بنانشان انگ_deltas = apply_منزل_all ? نقطو_ڳڻپ : indices.ڊيگهائي;

        إذا (unlikely (!deltas_x.resize_dirty  (انگ_deltas) ||
                      !Tuple_Variation_معلومات::decompile_deltas (p, deltas_x, پڇاڙي)))
          أرجع خطأ;

        إذا (is_gvar)
        {
          إذا (unlikely (!deltas_y.resize_dirty  (انگ_deltas) ||
                        !Tuple_Variation_معلومات::decompile_deltas (p, deltas_y, پڇاڙي)))
            أرجع خطأ;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        إذا (unlikely (!var.indices.allocate_from_pool (pool, نقطو_ڳڻپ) ||
                      !var.deltas_x.allocate_from_pool (pool, نقطو_ڳڻپ, خطأ)))
          أرجع خطأ;

        إذا (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, نقطو_ڳڻپ, خطأ)))
          أرجع خطأ;

        لكل (بنانشان i = 0; i < انگ_deltas; i++)
        {
          بنانشان idx = apply_منزل_all ? i : indices[i];
          إذا (idx >= نقطو_ڳڻپ) استمر;
          var.indices[idx] = سچ;
          var.deltas_x[idx] = deltas_x[i];
          إذا (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } طالما (iterator.move_منزل_ايندڙ ());

      is_مرکب = is_مرکب_glyph;
      أرجع سچ;
    }

    منطقي create_from_شي_var_معلومات (ثابت Var_معلومات &var_معلومات,
                                    ثابت hb_ويڪٽر_t<hb_hashmap_t<hb_ٽيگ_t, Triple>>& regions,
                                    ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map,
                                    بنانشان& شي_ڳڻپ_2,
                                    ثابت hb_inc_bimap_t* inner_map = خالي_اشارو)
    {
      /* NULL offset, to keep original varidx valid, just return */
      إذا (&var_معلومات == &Null (Var_معلومات))
        أرجع سچ;

      بنانشان انگ_regions = var_معلومات.get_region_اشاريو_ڳڻپ ();
      إذا (!tuple_vars.alloc (انگ_regions)) أرجع خطأ;

      شي_ڳڻپ_2 = inner_map ? inner_map->get_population () : var_معلومات.get_شي_ڳڻپ ();
      إذا (!شي_ڳڻپ_2) أرجع سچ;
      بنانشان صف_سائيز = var_معلومات.get_صف_سائيز ();
      ثابت HBUINT8 *delta_بائيٽون = var_معلومات.get_delta_بائيٽون ();

      لكل (بنانشان r = 0; r < انگ_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        إذا (!tuple.deltas_x.resize_dirty  (شي_ڳڻپ_2) ||
            !tuple.indices.resize_dirty  (شي_ڳڻپ_2))
          أرجع خطأ;

        لكل (بنانشان i = 0; i < شي_ڳڻپ_2; i++)
        {
          tuple.indices.arrayZ[i] = سچ;
          tuple.deltas_x.arrayZ[i] = var_معلومات.get_شي_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_بائيٽون, صف_سائيز);
        }

        بنانشان region_اشاريو = var_معلومات.get_region_اشاريو (r);
        إذا (region_اشاريو >= regions.ڊيگهائي) أرجع خطأ;
        tuple.axis_tuples = regions.arrayZ[region_اشاريو];

        tuple_vars.push (std::move (tuple));
      }
      أرجع !tuple_vars.اندريون_error ();
    }

    خاص:
    ساكن صحيح cmp_axis_ٽيگ (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت hb_ٽيگ_t *a = (ثابت hb_ٽيگ_t*) pa;
      ثابت hb_ٽيگ_t *b = (ثابت hb_ٽيگ_t*) pb;
      أرجع (صحيح)(*a) - (صحيح)(*b);
    }

    منطقي change_tuple_variations_axis_limits (ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>& normalized_axes_location,
                                              ثابت hb_hashmap_t<hb_ٽيگ_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = خالي_اشارو)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_ويڪٽر_t<hb_ٽيگ_t> axis_ٽيگ_فهرست;
      إذا (!axis_ٽيگ_فهرست.alloc (normalized_axes_location.get_population ()))
        أرجع خطأ;
      لكل (خودڪار t : normalized_axes_location.ڪنجيون ())
        axis_ٽيگ_فهرست.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_نتيجو_scratch_t scratch;
      hb_ويڪٽر_t<tuple_delta_t> ٻاهريون;

      axis_ٽيگ_فهرست.qsort (cmp_axis_ٽيگ);
      لكل (خودڪار axis_ٽيگ_2 : axis_ٽيگ_فهرست)
      {
        Triple *axis_حد_بندي;
        إذا (!normalized_axes_location.has (axis_ٽيگ_2, &axis_حد_بندي))
          أرجع خطأ;
        TripleDistances axis_triple_distances{1.f, 1.f};
        إذا (axes_triple_distances.has (axis_ٽيگ_2))
          axis_triple_distances = axes_triple_distances.get (axis_ٽيگ_2);

        hb_ويڪٽر_t<tuple_delta_t> new_vars;
        لكل (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_حد_بندي (axis_ٽيگ_2, *axis_حد_بندي, axis_triple_distances, ٻاهريون, scratch, pool);
          إذا (!ٻاهريون) استمر;

          بنانشان new_len = new_vars.ڊيگهائي + ٻاهريون.ڊيگهائي;

          إذا (unlikely (!new_vars.alloc (new_len, خطأ)))
            أرجع خطأ;

          لكل (بنانشان i = 0; i < ٻاهريون.ڊيگهائي; i++)
            new_vars.push (std::move (ٻاهريون[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      أرجع سچ;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    منطقي ملايو_tuple_variations (contour_نقطو_ويڪٽر_t* contour_points = خالي_اشارو)
    {
      hb_ويڪٽر_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      إذا (unlikely (!new_vars.alloc (tuple_vars.ڊيگهائي)))
	أرجع خطأ;
      hb_hashmap_t<ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>*, بنانشان> m;
      لكل (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        إذا (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          إذا (contour_points && !contour_points->جوڙيو_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            أرجع خطأ;
          استمر;
        }

        بنانشان *idx;
        إذا (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        وإلا
        {
	  خودڪار *new_var = new_vars.push ();
	  إذا (unlikely (new_vars.اندريون_error ()))
	    أرجع خطأ;
          hb_مٽا_سٽا (*new_var, var);
          إذا (unlikely (!m.سيٽ (&(new_var->axis_tuples), new_vars.ڊيگهائي - 1)))
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
    منطقي compile_all_نقطو_sets ()
    {
      لكل (ثابت خودڪار& tuple: tuple_vars)
      {
        ثابت hb_ويڪٽر_t<منطقي>* points_سيٽ = &(tuple.indices);
        إذا (نقطو_معلومات_map.has (points_سيٽ))
        {
          بنانشان *ڳڻپ;
          إذا (unlikely (!نقطو_سيٽ_ڳڻپ_map.has (points_سيٽ, &ڳڻپ) ||
                        !نقطو_سيٽ_ڳڻپ_map.سيٽ (points_سيٽ, (*ڳڻپ) + 1)))
            أرجع خطأ;
          استمر;
        }

        hb_ويڪٽر_t<بنانشان حرف> compiled_نقطو_معلومات;
        إذا (!tuple_delta_t::compile_نقطو_سيٽ (*points_سيٽ, compiled_نقطو_معلومات))
          أرجع خطأ;

        إذا (!نقطو_معلومات_map.سيٽ (points_سيٽ, std::move (compiled_نقطو_معلومات)) ||
            !نقطو_سيٽ_ڳڻپ_map.سيٽ (points_سيٽ, 1))
          أرجع خطأ;
      }
      أرجع سچ;
    }

    /* find shared points set which saves most bytes */
    فراغ ڳوليو_shared_points ()
    {
      بنانشان max_saved_بائيٽون = 0;

      لكل (ثابت خودڪار& _ : نقطو_معلومات_map.iter_ref ())
      {
        ثابت hb_ويڪٽر_t<منطقي>* points_سيٽ = _.پهريون;
        بنانشان معلومات_ڊيگهائي = _.second.ڊيگهائي;
        إذا (!معلومات_ڊيگهائي) استمر;
        بنانشان *ڳڻپ;
        إذا (unlikely (!نقطو_سيٽ_ڳڻپ_map.has (points_سيٽ, &ڳڻپ) ||
                      *ڳڻپ <= 1))
        {
          shared_points_بائيٽون = خالي_اشارو;
          أرجع;
        }

        بنانشان saved_بائيٽون = معلومات_ڊيگهائي * ((*ڳڻپ) -1);
        إذا (saved_بائيٽون > max_saved_بائيٽون)
        {
          max_saved_بائيٽون = saved_بائيٽون;
          shared_points_بائيٽون = &(_.second);
        }
      }
    }

    منطقي calc_inferred_deltas (ثابت contour_نقطو_ويڪٽر_t& contour_points,
			       hb_ويڪٽر_t<بنانشان> &scratch)
    {
      لكل (tuple_delta_t& var : tuple_vars)
        إذا (!var.calc_inferred_deltas (contour_points, scratch))
          أرجع خطأ;

      أرجع سچ;
    }

    منطقي iup_optimize (ثابت contour_نقطو_ويڪٽر_t& contour_points,
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
    فراغ cull_unreachable (ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> &reachable_ranges)
    {
      بنانشان j = 0;
      لكل (بنانشان i = 0; i < tuple_vars.ڊيگهائي; i++)
      {
	إذا (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  استمر;
	إذا (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    منطقي instantiate (ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>& normalized_axes_location,
                      ثابت hb_hashmap_t<hb_ٽيگ_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = خالي_اشارو,
                      contour_نقطو_ويڪٽر_t* contour_points = خالي_اشارو,
                      منطقي optimize = خطأ)
    {
      إذا (!tuple_vars) أرجع سچ;
      إذا (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        أرجع خطأ;
      /* compute inferred deltas only for gvar */
      إذا (contour_points)
      {
	hb_ويڪٽر_t<بنانشان> scratch;
	إذا (!calc_inferred_deltas (*contour_points, scratch))
	  أرجع خطأ;
      }

      /* if iup delta opt is on, contour_points can't be null */
      إذا (optimize && !contour_points)
        أرجع خطأ;

      إذا (!ملايو_tuple_variations (optimize ? contour_points : خالي_اشارو))
        أرجع خطأ;

      إذا (optimize && !iup_optimize (*contour_points, scratch)) أرجع خطأ;
      أرجع !tuple_vars.اندريون_error ();
    }

    منطقي compile_بائيٽون (ثابت hb_map_t& axes_اشاريو_map,
                        ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map,
                        منطقي use_shared_points,
                        منطقي is_gvar = خطأ,
                        ثابت hb_hashmap_t<ثابت hb_ويڪٽر_t<F2DOT14>*, بنانشان>* shared_tuples_idx_map = خالي_اشارو,
			hb_alloc_pool_t *pool = خالي_اشارو)
    {
      // return true for empty glyph
      إذا (!tuple_vars)
        أرجع سچ;

      // compile points set and store data in hashmap
      إذا (!compile_all_نقطو_sets ())
        أرجع خطأ;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_سائيز += 4;

      إذا (use_shared_points)
      {
        ڳوليو_shared_points ();
        إذا (shared_points_بائيٽون)
          compiled_byte_سائيز += shared_points_بائيٽون->ڊيگهائي;
      }
      hb_ويڪٽر_t<صحيح> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      لكل (خودڪار& tuple: tuple_vars)
      {
        ثابت hb_ويڪٽر_t<منطقي>* points_سيٽ = &(tuple.indices);
        hb_ويڪٽر_t<بنانشان حرف> *points_معلومات;
        إذا (unlikely (!نقطو_معلومات_map.has (points_سيٽ, &points_معلومات)))
          أرجع خطأ;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        إذا (!points_معلومات->ڊيگهائي)
          استمر;
        إذا (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          أرجع خطأ;

        بنانشان points_معلومات_ڊيگهائي = (points_معلومات != shared_points_بائيٽون) ? points_معلومات->ڊيگهائي : 0;
        إذا (!tuple.compile_tuple_var_سرو (axes_اشاريو_map, points_معلومات_ڊيگهائي, axes_old_اشاريو_ٽيگ_map,
                                             shared_tuples_idx_map,
					     pool))
          أرجع خطأ;
        compiled_byte_سائيز += tuple.compiled_tuple_سرو.ڊيگهائي + points_معلومات_ڊيگهائي + tuple.compiled_deltas.ڊيگهائي;
      }

      إذا (is_gvar && (compiled_byte_سائيز % 2))
      {
        needs_padding = سچ;
        compiled_byte_سائيز += 1;
      }

      أرجع سچ;
    }

    منطقي serialize_var_سرا (hb_serialize_context_t *c, بنانشان& ڪل_سرو_len) ثابت
    {
      TRACE_SERIALIZE (هذا);
      لكل (ثابت خودڪار& tuple: tuple_vars)
      {
        tuple.compiled_tuple_سرو.as_array ().copy (c);
        إذا (c->اندريون_error ()) return_trace (خطأ);
        ڪل_سرو_len += tuple.compiled_tuple_سرو.ڊيگهائي;
      }
      return_trace (سچ);
    }

    منطقي serialize_var_معلومات (hb_serialize_context_t *c, منطقي is_gvar) ثابت
    {
      TRACE_SERIALIZE (هذا);
      إذا (is_gvar && shared_points_بائيٽون)
      {
        hb_ubytes_t s (shared_points_بائيٽون->arrayZ, shared_points_بائيٽون->ڊيگهائي);
        s.copy (c);
      }

      لكل (ثابت خودڪار& tuple: tuple_vars)
      {
        ثابت hb_ويڪٽر_t<منطقي>* points_سيٽ = &(tuple.indices);
        hb_ويڪٽر_t<بنانشان حرف> *نقطو_معلومات;
        إذا (!نقطو_معلومات_map.has (points_سيٽ, &نقطو_معلومات))
          return_trace (خطأ);

        إذا (!is_gvar || نقطو_معلومات != shared_points_بائيٽون)
        {
          hb_ubytes_t s (نقطو_معلومات->arrayZ, نقطو_معلومات->ڊيگهائي);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        إذا (c->اندريون_error ()) return_trace (خطأ);
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
    بنانشان get_axis_ڳڻپ () ثابت { أرجع axis_ڳڻپ; }

    فراغ init (hb_بائيٽون_t var_معلومات_بائيٽون_2, بنانشان صحيح axis_ڳڻپ_3, ثابت فراغ *جدول_بنياد)
    {
      var_معلومات_بائيٽون = var_معلومات_بائيٽون_2;
      var_معلومات = var_معلومات_بائيٽون_2.as<Tuple_Variation_معلومات> ();
      tuples_کاٻو = var_معلومات->tuple_Var_ڳڻپ.get_ڳڻپ ();
      axis_ڳڻپ = axis_ڳڻپ_3;
      axis_ڳڻپ_times_2 = axis_ڳڻپ_3 * 2;
      current_tuple = &var_معلومات->get_tuple_var_سرو ();
      معلومات_وٿي_2 = 0;
      جدول_بنياد_2 = جدول_بنياد;
    }

    منطقي get_shared_indices (hb_ويڪٽر_t<بنانشان صحيح> &shared_indices /*ٻاهريون*/)
    {
      إذا (var_معلومات->has_shared_نقطو_numbers ())
      {
        ثابت HBUINT8 *بنياد = &(جدول_بنياد_2+var_معلومات->معلومات);
        ثابت HBUINT8 *p = بنياد;
        إذا (!decompile_points (p, shared_indices, (ثابت HBUINT8 *) (var_معلومات_بائيٽون.arrayZ + var_معلومات_بائيٽون.ڊيگهائي))) أرجع خطأ;
        معلومات_وٿي_2 = p - بنياد;
      }
      أرجع سچ;
    }

    منطقي is_valid ()
    {
      إذا (unlikely (tuples_کاٻو <= 0))
	أرجع خطأ;

      current_tuple_سائيز = Tuple_Variation_سرو::min_سائيز;
      إذا (unlikely (!var_معلومات_بائيٽون.check_پڇاڙي ((ثابت حرف *) current_tuple + current_tuple_سائيز)))
	أرجع خطأ;

      current_tuple_سائيز = current_tuple->get_سائيز (axis_ڳڻپ_times_2);
      إذا (unlikely (!var_معلومات_بائيٽون.check_پڇاڙي ((ثابت حرف *) current_tuple + current_tuple_سائيز)))
	أرجع خطأ;

      أرجع سچ;
    }

    HB_ALWAYS_INLINE
    منطقي move_منزل_ايندڙ ()
    {
      معلومات_وٿي_2 += current_tuple->get_معلومات_سائيز ();
      current_tuple = &Struct_At_وٿي<Tuple_Variation_سرو> (current_tuple, current_tuple_سائيز);
      tuples_کاٻو--;
      أرجع is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    ثابت HBUINT8 *get_serialized_معلومات () ثابت
    { أرجع &(جدول_بنياد_2+var_معلومات->معلومات) + معلومات_وٿي_2; }

    خاص:
    نشانوارو tuples_کاٻو;
    ثابت Tuple_Variation_معلومات *var_معلومات;
    بنانشان صحيح axis_ڳڻپ;
    بنانشان صحيح axis_ڳڻپ_times_2;
    بنانشان صحيح معلومات_وٿي_2;
    بنانشان صحيح current_tuple_سائيز;
    ثابت فراغ *جدول_بنياد_2;

    عام:
    hb_بائيٽون_t var_معلومات_بائيٽون;
    ثابت Tuple_Variation_سرو *current_tuple;
  };

  ساكن منطقي get_tuple_iterator (hb_بائيٽون_t var_معلومات_بائيٽون, بنانشان axis_ڳڻپ,
                                  ثابت فراغ *جدول_بنياد_2,
                                  hb_ويڪٽر_t<بنانشان صحيح> &shared_indices /*ٻاهريون*/,
                                  tuple_iterator_t *iterator /*ٻاهريون*/)
  {
    iterator->init (var_معلومات_بائيٽون, axis_ڳڻپ, جدول_بنياد_2);
    إذا (!iterator->get_shared_indices (shared_indices))
      أرجع خطأ;
    أرجع iterator->is_valid ();
  }

  منطقي has_shared_نقطو_numbers () ثابت { أرجع tuple_Var_ڳڻپ.has_shared_نقطو_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  منطقي cull_tuple_variations (hb_بائيٽون_t var_معلومات_بائيٽون,
			      بنانشان axis_ڳڻپ,
			      hb_array_t<ثابت F2DOT14> shared_tuples,
			      ثابت hb_map_t *axes_old_اشاريو_ٽيگ_map,
			      ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> &reachable_ranges,
			      hb_ويڪٽر_t<حرف> &ٻاهريون /*ٻاهريون*/,
			      منطقي *بدليو /*ٻاهريون*/) ثابت
  {
    *بدليو = خطأ;

    hb_ويڪٽر_t<بنانشان صحيح> shared_indices;
    tuple_iterator_t iterator;
    إذا (!get_tuple_iterator (var_معلومات_بائيٽون, axis_ڳڻپ, هذا,
			     shared_indices, &iterator))
      أرجع سچ; /* no tuples or malformed: keep original */

    ثابت حرف *بائيٽون_شروعات = var_معلومات_بائيٽون.arrayZ;
    ثابت حرف *بائيٽون_پڇاڙي = بائيٽون_شروعات + var_معلومات_بائيٽون.ڊيگهائي;
    ثابت حرف *serialized_بنياد = (ثابت حرف *) &(هذا+معلومات);
    ثابت حرف *shared_points_پڇاڙي = (ثابت حرف *) iterator.get_serialized_معلومات ();
    إذا (unlikely (serialized_بنياد < بائيٽون_شروعات || serialized_بنياد > بائيٽون_پڇاڙي ||
		  shared_points_پڇاڙي < serialized_بنياد || shared_points_پڇاڙي > بائيٽون_پڇاڙي))
      أرجع سچ;

    بنية kept_tuple_t
    {
      ثابت حرف *سرو;
      بنانشان سرو_سائيز;
      ثابت حرف *معلومات;
      بنانشان معلومات_سائيز;
    };
    hb_ويڪٽر_t<kept_tuple_t> kept;
    بنانشان ڪل = 0;
    بنانشان kept_سرا_سائيز = 0, kept_معلومات_سائيز = 0;
    افعل
    {
      ثابت Tuple_Variation_سرو *سرو = iterator.current_tuple;
      بنانشان سرو_سائيز = سرو->get_سائيز (axis_ڳڻپ * 2);
      بنانشان معلومات_سائيز = سرو->get_معلومات_سائيز ();
      ثابت حرف *tuple_معلومات = (ثابت حرف *) iterator.get_serialized_معلومات ();
      إذا (unlikely (tuple_معلومات < serialized_بنياد ||
		    معلومات_سائيز > (بنانشان) (بائيٽون_پڇاڙي - tuple_معلومات)))
	أرجع سچ; /* malformed: keep original */

      hb_hashmap_t<hb_ٽيگ_t, Triple> axis_tuples;
      إذا (!سرو->unpack_axis_tuples (axis_ڳڻپ, shared_tuples,
				       axes_old_اشاريو_ٽيگ_map, axis_tuples))
	أرجع سچ;

      ڪل++;
      إذا (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(ثابت حرف *) سرو, سرو_سائيز,
				 tuple_معلومات, معلومات_سائيز});
	kept_سرا_سائيز += سرو_سائيز;
	kept_معلومات_سائيز += معلومات_سائيز;
      }
    } طالما (iterator.move_منزل_ايندڙ ());

    إذا (unlikely (kept.اندريون_error ())) أرجع خطأ;
    إذا (kept.ڊيگهائي == ڪل) أرجع سچ; /* nothing to cull */

    *بدليو = سچ;
    إذا (!kept.ڊيگهائي) أرجع سچ; /* everything culled: no variation data */

    بنانشان shared_points_سائيز = shared_points_پڇاڙي - serialized_بنياد;
    بنانشان معلومات_وٿي_2 = min_سائيز + kept_سرا_سائيز;
    بنانشان new_سائيز = معلومات_وٿي_2 + shared_points_سائيز + kept_معلومات_سائيز;
    إذا (unlikely (!ٻاهريون.resize (new_سائيز)))
      أرجع خطأ;

    Tuple_Variation_معلومات *ٻاهريون_معلومات = (Tuple_Variation_معلومات *) ٻاهريون.arrayZ;
    ٻاهريون_معلومات->tuple_Var_ڳڻپ = (uint16_t) (kept.ڊيگهائي |
					  (has_shared_نقطو_numbers () ? 0x8000u : 0u));
    ٻاهريون_معلومات->معلومات = معلومات_وٿي_2;

    حرف *p = ٻاهريون.arrayZ + min_سائيز;
    لكل (ثابت خودڪار &t : kept)
    {
      hb_memcpy (p, t.سرو, t.سرو_سائيز);
      p += t.سرو_سائيز;
    }
    hb_memcpy (p, serialized_بنياد, shared_points_سائيز);
    p += shared_points_سائيز;
    لكل (ثابت خودڪار &t : kept)
    {
      hb_memcpy (p, t.معلومات, t.معلومات_سائيز);
      p += t.معلومات_سائيز;
    }
    أرجع سچ;
  }

  ساكن منطقي decompile_points (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_ويڪٽر_t<بنانشان صحيح> &points /*ٻاهريون*/,
				ثابت HBUINT8 *پڇاڙي)
  {
    تعداد packed_نقطو_flag_t
    {
      POINTS_ARE_لفظ     = 0x80,
      نقطو_لاڳيتو_ڳڻپ_نقاب = 0x7F
    };

    إذا (unlikely (p + 1 > پڇاڙي)) أرجع خطأ;

    بنانشان ڳڻپ = *p++;
    إذا (ڳڻپ & POINTS_ARE_لفظ)
    {
      إذا (unlikely (p + 1 > پڇاڙي)) أرجع خطأ;
      ڳڻپ = ((ڳڻپ & نقطو_لاڳيتو_ڳڻپ_نقاب) << 8) | *p++;
    }
    إذا (unlikely (!points.resize_dirty  (ڳڻپ))) أرجع خطأ;

    بنانشان n = 0;
    بنانشان i = 0;
    طالما (i < ڳڻپ)
    {
      إذا (unlikely (p + 1 > پڇاڙي)) أرجع خطأ;
      بنانشان control = *p++;
      بنانشان لاڳيتو_ڳڻپ = (control & نقطو_لاڳيتو_ڳڻپ_نقاب) + 1;
      بنانشان stop = i + لاڳيتو_ڳڻپ;
      إذا (unlikely (stop > ڳڻپ)) أرجع خطأ;
      إذا (control & POINTS_ARE_لفظ)
      {
        إذا (unlikely (p + لاڳيتو_ڳڻپ * HBUINT16::static_سائيز > پڇاڙي)) أرجع خطأ;
        لكل (; i < stop; i++)
        {
          n += *(ثابت HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_سائيز;
        }
      }
      وإلا
      {
        إذا (unlikely (p + لاڳيتو_ڳڻپ > پڇاڙي)) أرجع خطأ;
        لكل (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    أرجع سچ;
  }

  سانچو <قسم_نالو T>
  HB_ALWAYS_INLINE
  ساكن منطقي decompile_deltas (ثابت HBUINT8 *&p /* IN/OUT */,
				hb_ويڪٽر_t<T> &deltas /* IN/OUT */,
				ثابت HBUINT8 *پڇاڙي,
				منطقي consume_all = خطأ,
				بنانشان شروعات = 0)
  {
    أرجع Tuple_قدرون::decompile (p, deltas, پڇاڙي, consume_all, شروعات);
  }

  منطقي has_معلومات () ثابت { أرجع tuple_Var_ڳڻپ; }

  منطقي decompile_tuple_variations (بنانشان نقطو_ڳڻپ,
                                   منطقي is_gvar,
                                   tuple_iterator_t iterator,
                                   ثابت hb_map_t *axes_old_اشاريو_ٽيگ_map,
                                   ثابت hb_ويڪٽر_t<بنانشان> &shared_indices,
                                   ثابت hb_array_t<ثابت F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /*ٻاهريون*/
				   hb_alloc_pool_t *pool = خالي_اشارو,
                                   منطقي is_مرکب_glyph = خطأ) ثابت
  {
    أرجع tuple_variations.create_from_tuple_var_معلومات (iterator, tuple_Var_ڳڻپ,
                                                        نقطو_ڳڻپ, is_gvar,
                                                        axes_old_اشاريو_ٽيگ_map,
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

    خودڪار *ٻاهريون = c->شروعات_embed (هذا);
    إذا (unlikely (!c->extend_min (ٻاهريون))) return_trace (خطأ);

    إذا (!c->check_سونپيو (ٻاهريون->tuple_Var_ڳڻپ, tuple_variations.get_var_ڳڻپ (),
                          HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)) return_trace (خطأ);

    بنانشان ڪل_سرو_len = 0;

    إذا (!tuple_variations.serialize_var_سرا (c, ڪل_سرو_len))
      return_trace (خطأ);

    بنانشان معلومات_وٿي_2 = min_سائيز + ڪل_سرو_len;
    إذا (!is_gvar) معلومات_وٿي_2 += 4;
    إذا (!c->check_سونپيو (ٻاهريون->معلومات, معلومات_وٿي_2, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)) return_trace (خطأ);

    أرجع tuple_variations.serialize_var_معلومات (c, is_gvar);
  }

  محمي:
  بنية Tuple_Var_ڳڻپ : HBUINT16
  {
    دوست بنية tuple_variations_t;
    منطقي has_shared_نقطو_numbers () ثابت { أرجع ((*هذا) & Shared_نقطو_Numbers); }
    بنانشان صحيح get_ڳڻپ () ثابت { أرجع (*هذا) & ڳڻپ_نقاب; }
    Tuple_Var_ڳڻپ& عامل = (uint16_t i) { HBUINT16::عامل= (i); أرجع *هذا; }
    واضح عامل منطقي () ثابت { أرجع get_ڳڻپ (); }

    محمي:
    تعداد Flags
    {
      Shared_نقطو_Numbers= 0x8000u,
      ڳڻپ_نقاب         = 0x0FFFu
    };
    عام:
    DEFINE_سائيز_STATIC (2);
  };

  Tuple_Var_ڳڻپ tuple_Var_ڳڻپ;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  وٿي_منزل<HBUINT8, OffType>
                معلومات;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  عام:
  DEFINE_سائيز_MIN (2 + OffType::static_سائيز);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
استعمال tuple_variations_t = Tuple_Variation_معلومات<HBUINT16>::tuple_variations_t;
بنية شي_variations_t
{
  استعمال region_t = ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>*;
  خاص:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_ويڪٽر_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_ويڪٽر_t<بنانشان> var_معلومات_انگ_صفون;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_ويڪٽر_t<hb_hashmap_t<hb_ٽيگ_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_ويڪٽر_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, بنانشان> region_map;

  /* all delta rows after instantiation */
  hb_ويڪٽر_t<hb_ويڪٽر_t<صحيح>> delta_صفون;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_ويڪٽر_t<delta_صف_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  منطقي has_long = خطأ;

  عام:
  منطقي has_long_لفظ () ثابت
  { أرجع has_long; }

  ثابت hb_ويڪٽر_t<region_t>& get_region_list () ثابت
  { أرجع region_list; }

  ثابت hb_ويڪٽر_t<delta_صف_encoding_t>& get_vardata_encodings () ثابت
  { أرجع encodings; }

  ثابت hb_map_t& get_varidx_map () ثابت
  { أرجع varidx_map; }

  منطقي جوڙيو_vardata_encoding_for_testing (hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح>*> &&صفون,
                                         بنانشان انگ_کانن_ڳڻپ)
  {
    encodings.push (delta_صف_encoding_t (std::move (صفون), انگ_کانن_ڳڻپ));
    أرجع !encodings.اندريون_error ();
  }

  منطقي compile_varidx_map_for_testing (ثابت hb_hashmap_t<بنانشان, ثابت hb_ويڪٽر_t<صحيح>*>& front_mapping)
  { أرجع compile_varidx_map (front_mapping); }

  منطقي instantiate (ثابت شي_Variation_Store& varStore,
                    ثابت hb_subset_plan_t *plan,
                    منطقي optimize=سچ,
                    منطقي use_no_variation_idx=سچ,
                    ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> (),
                    منطقي layout_conditions = خطأ)
  {
    إذا (!create_from_شي_varstore (varStore, plan->axes_old_اشاريو_ٽيگ_map, inner_maps))
      أرجع خطأ;
    /* avar2 partial instancing: cull unreachable regions. */
    إذا (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      لكل (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    إذا (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      أرجع خطأ;
    إذا (layout_conditions &&
        (!instantiate_condition_قدرون (plan, varStore) || !build_region_list ()))
      أرجع خطأ;
    أرجع as_شي_varstore (optimize, use_no_variation_idx);
  }

  منطقي instantiate_condition_قدرون (ثابت hb_subset_plan_t *plan,
                                      ثابت شي_Variation_Store &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    خودڪار &condition_map = plan->layout_condition_idx_قدر_map;
    condition_map.clear ();
    إذا (!plan->layout_condition_قدرون) أرجع سچ;
    استعمال condition_قدر_t = hb_pair_t<uintptr_t, hb_pair_t<بنانشان, صحيح>>;
    hb_ويڪٽر_t<condition_قدر_t> conditions;
    لكل (خودڪار condition : plan->layout_condition_قدرون.iter ())
      conditions.push (condition);
    إذا (unlikely (conditions.اندريون_error ())) أرجع خطأ;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (ثابت condition_قدر_t &a, ثابت condition_قدر_t &b) {
      إذا (a.second.پهريون != b.second.پهريون)
        أرجع a.second.پهريون < b.second.پهريون ? -1 : 1;
      أرجع a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    بنانشان new_outer = 0;
    لكل (بنانشان i = 0; i < plan->gdef_varstore_inner_maps.ڊيگهائي; i++)
      إذا (plan->gdef_varstore_inner_maps[i].get_population ())
        إذا (!outer_map.سيٽ (i, new_outer++)) أرجع خطأ;
    لكل (خودڪار condition : conditions)
    {
      بنانشان old_idx = condition.second.پهريون;
      مزدوج value = condition.second.second +
                     varStore.get_delta<مزدوج> (old_idx, plan->normalized_coords.as_array ());
      بنانشان old_outer = old_idx >> 16;
      إذا (!outer_map.has (old_outer))
        استمر;
      بنانشان outer = outer_map.get (old_outer);
      بنانشان inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      بنانشان idx = (outer << 16) | inner;
      إذا (outer >= vars.ڊيگهائي || inner >= var_معلومات_انگ_صفون[outer])
        استمر;

      مزدوج وڌ_کان_وڌ = fabs ((مزدوج) value);
      منطقي variable = خطأ;
      لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        إذا (unlikely (inner >= tuple.deltas_x.ڊيگهائي)) أرجع خطأ;
        عائم delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        وڌ_کان_وڌ = hb_max (وڌ_کان_وڌ, fabs ((مزدوج) delta));
      }
      إذا (!variable)
      {
        إذا (!condition_map.سيٽ (condition.پهريون,
                                hb_pair (بنانشان (VarIdx::NO_VARIATION), صحيح (value > 0))))
          أرجع خطأ;
        استمر;
      }

      مزدوج scale = 1.;
      طالما (وڌ_کان_وڌ * scale > وڌ_کان_وڌ_عدد)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      طالما (وڌ_کان_وڌ * scale * 2 <= وڌ_کان_وڌ_عدد)
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
        tuple_variations_t ذاتي_صف;
        لكل (ثابت tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          إذا (!tuple.deltas_x[inner]) استمر;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (سچ);
          copy.deltas_x.push ((عائم) round ((مزدوج) tuple.deltas_x[inner] * scale));
          إذا (unlikely (copy.axis_tuples.اندريون_error () || copy.indices.اندريون_error () ||
                        copy.deltas_x.اندريون_error ())) أرجع خطأ;
          ذاتي_صف.tuple_vars.push (std::move (copy));
        }
        إذا (unlikely (vars.ڊيگهائي >= 0xFFFFu)) أرجع خطأ;
        إذا (unlikely (ذاتي_صف.tuple_vars.اندريون_error ())) أرجع خطأ;
        idx = vars.ڊيگهائي << 16;
        vars.push (std::move (ذاتي_صف));
        var_معلومات_انگ_صفون.push (1);
        إذا (unlikely (vars.اندريون_error () || var_معلومات_انگ_صفون.اندريون_error ())) أرجع خطأ;
        إذا (default_قدر < -32768 || default_قدر > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          صحيح bias = default_قدر - hb_clamp (default_قدر, -32768, 32767);
          default_قدر -= bias;
          صحيح مٿاهون = bias / 65536 * 65536;
          صحيح parts[] = {مٿاهون, bias - مٿاهون};
          لكل (صحيح part : parts)
          {
            إذا (!part) استمر;
            بنانشان ڳڻپ = vars[idx >> 16].tuple_vars.ڊيگهائي;
            جوڙيو_tuple (idx >> 16, hb_hashmap_t<hb_ٽيگ_t, Triple> (),
                       0, part, 1);
            ثابت خودڪار &tuples = vars[idx >> 16].tuple_vars;
            إذا (unlikely (tuples.اندريون_error () || tuples.ڊيگهائي != ڳڻپ + 1 ||
                          tuples.پڇ ().indices.اندريون_error () ||
                          tuples.پڇ ().deltas_x.اندريون_error ())) أرجع خطأ;
          }
        }
      }
      إذا (!condition_map.سيٽ (condition.پهريون, hb_pair (idx, default_قدر)))
        أرجع خطأ;
    }
    أرجع !condition_map.اندريون_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  منطقي create_from_شي_varstore (ثابت شي_Variation_Store& varStore,
                                  ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map,
                                  ثابت hb_array_t <ثابت hb_inc_bimap_t> inner_maps = hb_array_t<ثابت hb_inc_bimap_t> ())
  {
    ثابت VarRegionList& regionList = varStore.get_region_list ();
    إذا (!regionList.get_var_regions (axes_old_اشاريو_ٽيگ_map, orig_region_list))
      أرجع خطأ;

    بنانشان انگ_var_معلومات = varStore.get_sub_جدول_ڳڻپ ();
    إذا (inner_maps && inner_maps.ڊيگهائي != انگ_var_معلومات) أرجع خطأ;
    إذا (!vars.alloc (انگ_var_معلومات) ||
        !var_معلومات_انگ_صفون.alloc (انگ_var_معلومات)) أرجع خطأ;

    لكل (بنانشان i = 0; i < انگ_var_معلومات; i++)
    {
      إذا (inner_maps && !inner_maps.arrayZ[i].get_population ())
          استمر;
      tuple_variations_t var_معلومات_tuples;
      بنانشان شي_ڳڻپ_2 = 0;
      إذا (!var_معلومات_tuples.create_from_شي_var_معلومات (varStore.get_sub_جدول (i),
                                                      orig_region_list,
                                                      axes_old_اشاريو_ٽيگ_map,
                                                      شي_ڳڻپ_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : خالي_اشارو))
        أرجع خطأ;

      var_معلومات_انگ_صفون.push (شي_ڳڻپ_2);
      vars.push (std::move (var_معلومات_tuples));
    }
    أرجع !vars.اندريون_error () && !var_معلومات_انگ_صفون.اندريون_error () && vars.ڊيگهائي == var_معلومات_انگ_صفون.ڊيگهائي;
  }

  منطقي instantiate_tuple_vars (ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>& normalized_axes_location,
                               ثابت hb_hashmap_t<hb_ٽيگ_t, TripleDistances>& axes_triple_distances,
                               منطقي build_regions = سچ)
  {
    optimize_scratch_t scratch;
    لكل (tuple_variations_t& tuple_vars : vars)
      إذا (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        أرجع خطأ;

    أرجع !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  بنانشان جوڙيو_vardata (بنانشان شي_ڳڻپ_2)
  {
    vars.push (tuple_variations_t ());
    var_معلومات_انگ_صفون.push (شي_ڳڻپ_2);
    أرجع vars.ڊيگهائي - 1;
  }

  /* Get item count for a VarData subtable. */
  بنانشان get_شي_ڳڻپ (بنانشان outer) ثابت
  { أرجع outer < var_معلومات_انگ_صفون.ڊيگهائي ? var_معلومات_انگ_صفون[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  بنانشان duplicate_صف (بنانشان outer, بنانشان inner)
  {
    إذا (unlikely (outer >= vars.ڊيگهائي ||
                  inner >= var_معلومات_انگ_صفون[outer] ||
                  var_معلومات_انگ_صفون[outer] >= 0xFFFFu))
      أرجع (بنانشان) -1;
    لكل (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      إذا (unlikely (inner >= tuple.deltas_x.ڊيگهائي ||
                    inner >= tuple.indices.ڊيگهائي))
        أرجع (بنانشان) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      إذا (inner < tuple.deltas_y.ڊيگهائي)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      إذا (unlikely (tuple.indices.اندريون_error () ||
                    tuple.deltas_x.اندريون_error () ||
                    tuple.deltas_y.اندريون_error ()))
        أرجع (بنانشان) -1;
    }
    أرجع var_معلومات_انگ_صفون[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  فراغ جوڙيو_tuple (بنانشان outer,
                  hb_hashmap_t<hb_ٽيگ_t, Triple>&& axis_tuples,
                  بنانشان inner, صحيح delta, بنانشان شي_ڳڻپ_2)
  {
    إذا (outer >= vars.ڊيگهائي) أرجع;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    إذا (!tuple.indices.resize (شي_ڳڻپ_2) ||
        !tuple.deltas_x.resize (شي_ڳڻپ_2)) أرجع;
    لكل (بنانشان i = 0; i < شي_ڳڻپ_2; i++)
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
    hb_hashmap_t<region_t, بنانشان> all_regions;
    hb_hashmap_t<region_t, بنانشان> استعمال_ٿيل_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_ويڪٽر_t<region_t> all_unique_regions;
    لكل (ثابت tuple_variations_t& sub_جدول_2 : vars)
    {
      لكل (ثابت tuple_delta_t& tuple : sub_جدول_2.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        إذا (!استعمال_ٿيل_regions.has (r))
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
            إذا (!استعمال_ٿيل_regions.سيٽ (r, 1))
              أرجع خطأ;
          }
        }
        إذا (all_regions.has (r))
          استمر;
        إذا (!all_regions.سيٽ (r, 1))
          أرجع خطأ;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    إذا (!all_regions || !all_unique_regions) أرجع سچ;

    إذا (!region_list.alloc (all_regions.get_population ()))
      أرجع خطأ;

    بنانشان idx = 0;
    /* append the original regions that pre-existed */
    لكل (ثابت خودڪار& r : orig_region_list)
    {
      إذا (!all_regions.has (&r) || !استعمال_ٿيل_regions.has (&r))
        استمر;

      region_list.push (&r);
      إذا (!region_map.سيٽ (&r, idx))
        أرجع خطأ;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    لكل (ثابت خودڪار& r: all_unique_regions)
    {
      إذا (!all_regions.has (r) || !استعمال_ٿيل_regions.has (r))
        استمر;
      region_list.push (r);
      إذا (!region_map.سيٽ (r, idx))
        أرجع خطأ;
      all_regions.del (r);
      idx++;
    }
    أرجع (!region_list.اندريون_error ()) && (!region_map.اندريون_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  بنية combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = افتراضي;
    combined_gain_idx_tuple_t (بنانشان gain, بنانشان i, بنانشان j)
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

    بنانشان idx_1 () ثابت { أرجع (encoded >> 20) & 0xFFFFF; };
    بنانشان idx_2 () ثابت { أرجع encoded & 0xFFFFF; };
  };

  منطقي as_شي_varstore (منطقي optimize=سچ, منطقي use_no_variation_idx=سچ)
  {
    /* return true if no variation data */
    إذا (!region_list) أرجع سچ;
    بنانشان انگ_کانن_ڳڻپ = region_list.ڊيگهائي;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    بنانشان ڪل_صفون = 0;
    لكل (بنانشان major = 0; major < var_معلومات_انگ_صفون.ڊيگهائي; major++)
      ڪل_صفون += var_معلومات_انگ_صفون[major];

    إذا (!delta_صفون.resize (ڪل_صفون)) أرجع خطأ;
    /* init all rows to [0]*num_cols */
    لكل (بنانشان i = 0; i < ڪل_صفون; i++)
      إذا (!(delta_صفون[i].resize (انگ_کانن_ڳڻپ))) أرجع خطأ;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<بنانشان, ثابت hb_ويڪٽر_t<صحيح>*> front_mapping;
    بنانشان شروعات_صف = 0;
    hb_ويڪٽر_t<delta_صف_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح> *> major_صفون;
    hb_hashmap_t<ثابت hb_ويڪٽر_t<صحيح>*, بنانشان> delta_صفون_map;
    لكل (بنانشان major = 0; major < vars.ڊيگهائي; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      ثابت tuple_variations_t& tuples = vars[major];
      بنانشان انگ_صفون = var_معلومات_انگ_صفون[major];

      إذا (!انگ_صفون) استمر;

      لكل (ثابت tuple_delta_t& tuple: tuples.tuple_vars)
      {
        إذا (tuple.deltas_x.ڊيگهائي != انگ_صفون)
          أرجع خطأ;

        /* skip unused regions */
        بنانشان *کانو_idx;
        إذا (!region_map.has (&(tuple.axis_tuples), &کانو_idx))
          استمر;

        لكل (بنانشان i = 0; i < انگ_صفون; i++)
        {
          صحيح rounded_delta = hb_clamp_منزل<صحيح> (roundf ((مزدوج) tuple.deltas_x[i]));
          صحيح &delta = delta_صفون[شروعات_صف + i][*کانو_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          إذا (unlikely (sum < INT_MIN || sum > وڌ_کان_وڌ_عدد)) أرجع خطأ;
          delta = (صحيح) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_صفون.reset ();
      لكل (بنانشان minor = 0; minor < انگ_صفون; minor++)
      {
	ثابت hb_ويڪٽر_t<صحيح>& صف = delta_صفون[شروعات_صف + minor];
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

	إذا (!front_mapping.سيٽ ((major<<16) + minor, &صف))
	  أرجع خطأ;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	إذا (optimize)
	{
	  إذا (delta_صفون_map.has (&صف))
	    استمر;

	  delta_صفون_map.سيٽ (&صف, 1);
	}

	major_صفون.push (&صف);
      }

      إذا (unlikely (major_صفون.اندريون_error () || delta_صفون_map.اندريون_error ()))
        أرجع خطأ;
      إذا (major_صفون)
      {
	خودڪار *encoding = encoding_objs.push (std::move (major_صفون), انگ_کانن_ڳڻپ);
	إذا (unlikely (encoding_objs.اندريون_error () || encoding->اندريون_error ()))
	  أرجع خطأ;
      }

      شروعات_صف += انگ_صفون;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    إذا (!optimize)
    {
      encodings = std::move (encoding_objs);
      أرجع !encodings.اندريون_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.qsort ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    استعمال شي_t = hb_ترجيح_قطار_t<combined_gain_idx_tuple_t>::شي_t;
    hb_ويڪٽر_t<شي_t> قطار_شيون;
    بنانشان انگ_todos = encoding_objs.ڊيگهائي;
    لكل (بنانشان i = 0; i < انگ_todos; i++)
    {
      لكل (بنانشان j = i + 1; j < انگ_todos; j++)
      {
        صحيح combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        إذا (combining_gain > 0)
          قطار_شيون.push (شي_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_ترجيح_قطار_t<combined_gain_idx_tuple_t> قطار (std::move (قطار_شيون));

    hb_ٻٽو_انگ_سيٽ_t removed_todo_idxes;
    طالما (قطار)
    {
      خودڪار t = قطار.ڪڍو_minimum ().پهريون;
      بنانشان i = t.idx_1 ();
      بنانشان j = t.idx_2 ();

      إذا (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        استمر;

      delta_صف_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_صف_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.جوڙيو (i);
      removed_todo_idxes.جوڙيو (j);
      إذا (unlikely (removed_todo_idxes.اندريون_error ())) أرجع خطأ;

      encoding.ملايو (other_encoding);
      إذا (unlikely (encoding.اندريون_error ())) أرجع خطأ;

      لكل (بنانشان idx = 0; idx < encoding_objs.ڊيگهائي; idx++)
      {
        إذا (removed_todo_idxes.has (idx)) استمر;

        ثابت delta_صف_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        إذا (obj.ويڪر == encoding.ويڪر && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          لكل (ثابت خودڪار& صف : obj.شيون)
            encoding.جوڙيو_صف (صف);
          إذا (unlikely (encoding.اندريون_error ())) أرجع خطأ;

          removed_todo_idxes.جوڙيو (idx);
          إذا (unlikely (removed_todo_idxes.اندريون_error ())) أرجع خطأ;
          استمر;
        }

        صحيح combined_gain = encoding.gain_from_merging (obj);
        إذا (combined_gain > 0)
          قطار.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.ڊيگهائي), 0);
      }

      خودڪار moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      إذا (unlikely (encoding_objs.اندريون_error ())) أرجع خطأ;
    }

    صحيح انگ_final_encodings = (صحيح) encoding_objs.ڊيگهائي - (صحيح) removed_todo_idxes.get_population ();
    إذا (انگ_final_encodings <= 0) أرجع خطأ;

    إذا (!encodings.alloc (انگ_final_encodings)) أرجع خطأ;
    لكل (بنانشان i = 0; i < encoding_objs.ڊيگهائي; i++)
    {
      إذا (removed_todo_idxes.has (i)) استمر;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    أرجع compile_varidx_map (front_mapping);
  }

  خاص:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  منطقي compile_varidx_map (ثابت hb_hashmap_t<بنانشان, ثابت hb_ويڪٽر_t<صحيح>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<ثابت hb_ويڪٽر_t<صحيح>*, بنانشان> پٺ_mapping;
    hb_ويڪٽر_t<delta_صف_encoding_t> ورهايو_encodings;

    لكل (بنانشان i = 0; i < encodings.ڊيگهائي; i++)
    {
      delta_صف_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      إذا (encoding.is_empty () || encoding.اندريون_error ())
        أرجع خطأ;

      بنانشان انگ_صفون = encoding.شيون.ڊيگهائي;
      بنانشان انگ_کانن_ڳڻپ = encoding.chars.ڊيگهائي;

      /* sort rows, make result deterministic */
      encoding.شيون.qsort (cmp_صف);

      لكل (بنانشان شروعات = 0; شروعات < انگ_صفون; شروعات += 0xFFFFu)
      {
        بنانشان chunk_len = hb_min (انگ_صفون - شروعات, 0xFFFFu);
        hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح>*> صفون;

        إذا (!صفون.alloc (chunk_len))
          أرجع خطأ;

        بنانشان major = ورهايو_encodings.ڊيگهائي;
        لكل (بنانشان minor = 0; minor < chunk_len; minor++)
        {
          ثابت hb_ويڪٽر_t<صحيح> *صف = encoding.شيون.arrayZ[شروعات + minor];
          صفون.push (صف);
          إذا (!پٺ_mapping.سيٽ (صف, (major << 16) + minor))
            أرجع خطأ;
        }

        خودڪار *ورهايو = ورهايو_encodings.push (std::move (صفون), انگ_کانن_ڳڻپ);
        إذا (unlikely (ورهايو_encodings.اندريون_error () || ورهايو->اندريون_error ()))
          أرجع خطأ;
      }
    }

    encodings = std::move (ورهايو_encodings);
    إذا (encodings.اندريون_error () || پٺ_mapping.اندريون_error ())
      أرجع خطأ;

    لكل (خودڪار _ : front_mapping.iter ())
    {
      بنانشان old_varidx = _.پهريون;
      بنانشان *new_varidx;
      إذا (پٺ_mapping.has (_.second, &new_varidx))
        varidx_map.سيٽ (old_varidx, *new_varidx);
      وإلا
        varidx_map.سيٽ (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_اشاريو);
    }
    أرجع !varidx_map.اندريون_error ();
  }

  ساكن صحيح cmp_صف (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    ثابت hb_ويڪٽر_t<صحيح>** a = (ثابت hb_ويڪٽر_t<صحيح>**) pa;
    ثابت hb_ويڪٽر_t<صحيح>** b = (ثابت hb_ويڪٽر_t<صحيح>**) pb;

    لكل (بنانشان i = 0; i < (*b)->ڊيگهائي; i++)
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


#جيڪڏهن_پورو /* HB_OT_VAR_COMMON_HH */

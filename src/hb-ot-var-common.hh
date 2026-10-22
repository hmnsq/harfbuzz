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

#నిర్వచితం_కాకపోతే HB_OT_VAR_COMMON_HH
#నిర్వచించు HB_OT_VAR_COMMON_HH

#అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-alloc-pool.hh"
#అన్తర్భావ "hb-priority-queue.hh"
#అన్తర్భావ "hb-subset-instancer-iup.hh"


నామాకాశ OT {

ఉపయోగించి rebase_tent_ఫలితం_scratch_t = hb_pair_t<rebase_tent_ఫలితం_t, rebase_tent_ఫలితం_t>;

/* https://docs.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats#tuplevariationheader */
నిర్మాణం Tuple_Variation_శీర్షిక
{
  మిత్ర నిర్మాణం tuple_delta_t;
  size_t get_కొలత (గుర్తులేని axis_లెక్క_times_2) స్థిరరాశి
  {
    // This function is super hot in mega-var-fonts with hundreds of masters.
    గుర్తులేని ti = tuple_సూచీ;
    ఒకవేళ (unlikely ((ti & (Tuple_సూచీ::EmbeddedPeakTuple | Tuple_సూచీ::IntermediateRegion))))
    {
      గుర్తులేని లెక్క = ((ti & Tuple_సూచీ::EmbeddedPeakTuple) != 0) + ((ti & Tuple_సూచీ::IntermediateRegion) != 0) * 2;
      తిరిగి hb_unsigned_mul_కూడు_saturate (లెక్క, axis_లెక్క_times_2, min_కొలత);
    }
    తిరిగి min_కొలత;
  }

  గుర్తులేని get_దత్తాంశం_కొలత () స్థిరరాశి { తిరిగి var_దత్తాంశం_కొలత; }

  స్థిరరాశి Tuple_Variation_శీర్షిక &get_తదుపరి (గుర్తులేని axis_లెక్క_times_2) స్థిరరాశి
  { తిరిగి Struct_At_స్థానభ్రంశం<Tuple_Variation_శీర్షిక> (ఇది, get_కొలత (axis_లెక్క_times_2)); }

  తార్కిక unpack_axis_tuples (గుర్తులేని axis_లెక్క,
                           స్థిరరాశి hb_array_t<స్థిరరాశి F2DOT14> shared_tuples,
                           స్థిరరాశి hb_map_t *axes_old_సూచీ_ట్యాగ్_map,
                           hb_hashmap_t<hb_ట్యాగ్_t, Triple>& axis_tuples /*బహిర్గతం*/) స్థిరరాశి
  {
    స్థిరరాశి F2DOT14 *peak_tuple = శూన్యసూచిక;
    ఒకవేళ (has_peak ())
      peak_tuple = get_peak_tuple (axis_లెక్క);
    లేకపోతే
    {
      గుర్తులేని పూర్ణాంకం సూచీ_2 = get_సూచీ ();
      ఒకవేళ (unlikely ((సూచీ_2 + 1) * axis_లెక్క > shared_tuples.నిడివి))
        తిరిగి అబద్ధం;
      peak_tuple = shared_tuples.sub_array (axis_లెక్క * సూచీ_2, axis_లెక్క).arrayZ;
    }

    స్థిరరాశి F2DOT14 *ప్రారంభం_tuple = శూన్యసూచిక;
    స్థిరరాశి F2DOT14 *చివర_tuple = శూన్యసూచిక;
    తార్కిక has_interm = has_intermediate ();

    ఒకవేళ (has_interm)
    {
      ప్రారంభం_tuple = get_ప్రారంభం_tuple (axis_లెక్క);
      చివర_tuple = get_చివర_tuple (axis_లెక్క);
    }

    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క; i++)
    {
      దశాంశం peak = peak_tuple[i].గమ్యం_float ();
      ఒకవేళ (peak == 0.f) కొనసాగు;

      hb_ట్యాగ్_t *axis_ట్యాగ్_2;
      ఒకవేళ (!axes_old_సూచీ_ట్యాగ్_map->has (i, &axis_ట్యాగ్_2))
        తిరిగి అబద్ధం;

      దశాంశం ప్రారంభం, చివర;
      ఒకవేళ (has_interm)
      {
        ప్రారంభం = ప్రారంభం_tuple[i].గమ్యం_float ();
        చివర = చివర_tuple[i].గమ్యం_float ();
      }
      లేకపోతే
      {
        ప్రారంభం = hb_min (peak, 0.f);
        చివర = hb_max (peak, 0.f);
      }
      axis_tuples.సమితి (*axis_ట్యాగ్_2, Triple (ప్రారంభం, peak, చివర));
    }

    తిరిగి నిజం;
  }

  HB_ALWAYS_INLINE
  దశాంశం calculate_scalar (hb_array_t<స్థిరరాశి పూర్ణాంకం> coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
			   స్థిరరాశి hb_array_t<స్థిరరాశి F2DOT14> shared_tuples,
			   hb_scalar_cache_t *shared_tuple_scalar_cache = (hb_scalar_cache_t *) &Null(hb_scalar_cache_t)) స్థిరరాశి
  {
    గుర్తులేని tuple_సూచీ_2 = tuple_సూచీ;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = tuple_సూచీ_2 & Tuple_సూచీ::Tuple_సూచీ_ముసుగు;

    /* The gvar shared-tuple cache is clamped to the 12-bit tuple-index range.
     * Only PrivatePointNumbers may accompany a cached shared-tuple index; any
     * other flag sets a bit at or above 0x1000, making this range test fail.
     */
    తార్కిక plain_shared_tuple = (tuple_సూచీ_2 & ~Tuple_సూచీ::వ్యక్తిగత_బిందువు_Numbers) < shared_tuple_scalar_cache->నిడివి;
    ఒకవేళ (likely (plain_shared_tuple))
    {
      దశాంశం scalar;
      ఒకవేళ (likely (shared_tuple_scalar_cache->get (సూచీ_2, &scalar)))
	తిరిగి scalar;
    }

    స్థిరరాశి F2DOT14 *peak_tuple;

    తార్కిక has_interm = tuple_సూచీ_2 & Tuple_సూచీ::IntermediateRegion; // Inlined for performance

    ఒకవేళ (unlikely (tuple_సూచీ_2 & Tuple_సూచీ::EmbeddedPeakTuple)) // Inlined for performance
    {
      peak_tuple = get_peak_tuple (coord_లెక్క);
      shared_tuple_scalar_cache = శూన్యసూచిక;
    }
    లేకపోతే
    {
      దశాంశం scalar;
      ఒకవేళ (!plain_shared_tuple &&
	  shared_tuple_scalar_cache->నిడివి &&
	  shared_tuple_scalar_cache->get (సూచీ_2, &scalar))
      {
        ఒకవేళ (has_interm && (scalar != 0 && scalar != 1.f))
	  shared_tuple_scalar_cache = శూన్యసూచిక;
	లేకపోతే
	  తిరిగి scalar;
      }

      ఒకవేళ (unlikely ((సూచీ_2 + 1) * coord_లెక్క > shared_tuples.నిడివి))
        తిరిగి 0.f;
      peak_tuple = shared_tuples.arrayZ + (coord_లెక్క * సూచీ_2);

    }

    స్థిరరాశి F2DOT14 *ప్రారంభం_tuple = శూన్యసూచిక;
    స్థిరరాశి F2DOT14 *చివర_tuple = శూన్యసూచిక;

    ఒకవేళ (has_interm)
    {
      ప్రారంభం_tuple = get_ప్రారంభం_tuple (coord_లెక్క);
      చివర_tuple = get_చివర_tuple (coord_లెక్క);
    }

    దశాంశం scalar = 1.f;
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
#ఒకవేళ HB_FAST_అంకె_ACCESS
    తార్కిక skip = coord_లెక్క >= 16;
#అయితే_ముగింపు
#అయితే_ముగింపు
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < coord_లెక్క; i++)
    {
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
#ఒకవేళ HB_FAST_అంకె_ACCESS
      ఒకవేళ (skip)
      {
	అయితే (i + 4 <= coord_లెక్క && * (HBUINT64LE *) &peak_tuple[i] == 0)
	  i += 4;
	అయితే (i < coord_లెక్క && peak_tuple[i].గమ్యం_int () == 0)
	  i += 1;
	ఒకవేళ (i >= coord_లెక్క)
	  ఆపు;
      }
#అయితే_ముగింపు
#అయితే_ముగింపు

      పూర్ణాంకం peak = peak_tuple[i].గమ్యం_int ();
      ఒకవేళ (!peak) కొనసాగు;

      పూర్ణాంకం v = coords[i];
      ఒకవేళ (!v) { scalar = 0.f; ఆపు; }
      ఒకవేళ (v == peak) కొనసాగు;

      ఒకవేళ (has_interm)
      {
	shared_tuple_scalar_cache = శూన్యసూచిక;
        పూర్ణాంకం ప్రారంభం = ప్రారంభం_tuple[i].గమ్యం_int ();
        పూర్ణాంకం చివర = చివర_tuple[i].గమ్యం_int ();
        ఒకవేళ (unlikely (ప్రారంభం > peak || peak > చివర ||
                      (ప్రారంభం < 0 && చివర > 0 && peak))) కొనసాగు;
        ఒకవేళ (v < ప్రారంభం || v > చివర) { scalar = 0.f; ఆపు; }
        ఒకవేళ (v < peak)
        { ఒకవేళ (peak != ప్రారంభం) scalar *= (దశాంశం) (v - ప్రారంభం) / (peak - ప్రారంభం); }
        లేకపోతే
        { ఒకవేళ (peak != చివర) scalar *= (దశాంశం) (చివర - v) / (చివర - peak); }
      }
      లేకపోతే ఒకవేళ (v < hb_min (0, peak) || v > hb_max (0, peak)) { scalar = 0.f; ఆపు; }
      లేకపోతే
        scalar *= (దశాంశం) v / peak;
    }
    ఒకవేళ (shared_tuple_scalar_cache)
      shared_tuple_scalar_cache->సమితి (సూచీ_2, scalar);
    తిరిగి scalar;
  }

  తార్కిక           has_peak () స్థిరరాశి { తిరిగి tuple_సూచీ & Tuple_సూచీ::EmbeddedPeakTuple; }
  తార్కిక   has_intermediate () స్థిరరాశి { తిరిగి tuple_సూచీ & Tuple_సూచీ::IntermediateRegion; }
  తార్కిక has_వ్యక్తిగత_points () స్థిరరాశి { తిరిగి tuple_సూచీ & Tuple_సూచీ::వ్యక్తిగత_బిందువు_Numbers; }
  గుర్తులేని      get_సూచీ () స్థిరరాశి { తిరిగి tuple_సూచీ & Tuple_సూచీ::Tuple_సూచీ_ముసుగు; }
  స్థిర constexpr గుర్తులేని max_shared_tuple_లెక్క = 0x1000u;

  రక్షిత:
  నిర్మాణం Tuple_సూచీ : HBUINT16
  {
    గణన Flags {
      EmbeddedPeakTuple   = 0x8000u,
      IntermediateRegion  = 0x4000u,
      వ్యక్తిగత_బిందువు_Numbers = 0x2000u,
      Tuple_సూచీ_ముసుగు      = 0x0FFFu
    };

    Tuple_సూచీ& క్రియాకారకం = (uint16_t i) { HBUINT16::క్రియాకారకం= (i); తిరిగి *ఇది; }
    DEFINE_కొలత_STATIC (2);
  };

  hb_array_t<స్థిరరాశి F2DOT14> get_all_tuples (గుర్తులేని axis_లెక్క) స్థిరరాశి
  { తిరిగి StructAfter<Unsized_Array_యొక్క<F2DOT14>> (tuple_సూచీ).as_array ((has_peak () + has_intermediate () * 2) * axis_లెక్క); }
  స్థిరరాశి F2DOT14* get_all_tuples_పునాది (గుర్తులేని axis_లెక్క) స్థిరరాశి
  { తిరిగి StructAfter<Unsized_Array_యొక్క<F2DOT14>> (tuple_సూచీ).arrayZ; }
  స్థిరరాశి F2DOT14* get_peak_tuple (గుర్తులేని axis_లెక్క) స్థిరరాశి
  { తిరిగి get_all_tuples_పునాది (axis_లెక్క); }
  స్థిరరాశి F2DOT14* get_ప్రారంభం_tuple (గుర్తులేని axis_లెక్క) స్థిరరాశి
  { తిరిగి get_all_tuples_పునాది (axis_లెక్క) + has_peak () * axis_లెక్క; }
  స్థిరరాశి F2DOT14* get_చివర_tuple (గుర్తులేని axis_లెక్క) స్థిరరాశి
  { తిరిగి get_all_tuples_పునాది (axis_లెక్క) + has_peak () * axis_లెక్క + axis_లెక్క; }

  HBUINT16      var_దత్తాంశం_కొలత;    /* The size in bytes of the serialized
                                 * data for this tuple variation table. */
  Tuple_సూచీ    tuple_సూచీ;     /* A packed field. The high 4 bits are flags (see below).
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
  బహిరంగ:
  DEFINE_కొలత_MIN (4);
};

నిర్మాణం optimize_scratch_t
{
  iup_scratch_t iup;
  hb_సదిశ_t<తార్కిక> opt_indices;
  hb_సదిశ_t<పూర్ణాంకం> rounded_x_deltas;
  hb_సదిశ_t<పూర్ణాంకం> rounded_y_deltas;
  hb_సదిశ_t<దశాంశం> opt_deltas_x;
  hb_సదిశ_t<దశాంశం> opt_deltas_y;
  hb_సదిశ_t<గుర్తులేని అక్షరం> opt_బిందువు_దత్తాంశం;
  hb_సదిశ_t<గుర్తులేని అక్షరం> opt_deltas_దత్తాంశం;
  hb_సదిశ_t<గుర్తులేని అక్షరం> బిందువు_దత్తాంశం;
  hb_సదిశ_t<గుర్తులేని అక్షరం> deltas_దత్తాంశం;
  hb_సదిశ_t<పూర్ణాంకం> rounded_deltas;
};

నిర్మాణం tuple_delta_t
{
  స్థిర constexpr తార్కిక realloc_move = నిజం;  // Watch out when adding new members!

  బహిరంగ:
  hb_hashmap_t<hb_ట్యాగ్_t, Triple> axis_tuples;

  /* indices_length = point_count, indice[i] = 1 means point i is referenced */
  hb_సదిశ_t<తార్కిక> indices;

  hb_సదిశ_t<దశాంశం> deltas_x;
  /* empty for cvar tuples */
  hb_సదిశ_t<దశాంశం> deltas_y;

  /* compiled data: header and deltas
   * compiled point data is saved in a hashmap within tuple_variations_t cause
   * some point sets might be reused by different tuple variations */
  hb_సదిశ_t<గుర్తులేని అక్షరం> compiled_tuple_శీర్షిక;
  hb_సదిశ_t<గుర్తులేని అక్షరం> compiled_deltas;

  hb_సదిశ_t<F2DOT14> compiled_peak_coords;
  hb_సదిశ_t<F2DOT14> compiled_interm_coords;

  tuple_delta_t (hb_alloc_pool_t *pool = శూన్యసూచిక) {}
  tuple_delta_t (స్థిరరాశి tuple_delta_t& o) = అప్రమేయ;
  tuple_delta_t& క్రియాకారకం = (స్థిరరాశి tuple_delta_t& o) = అప్రమేయ;

  మిత్ర శూన్య swap (tuple_delta_t& a, tuple_delta_t& b) noexcept
  {
    hb_మార్పిడి (a.axis_tuples, b.axis_tuples);
    hb_మార్పిడి (a.indices, b.indices);
    hb_మార్పిడి (a.deltas_x, b.deltas_x);
    hb_మార్పిడి (a.deltas_y, b.deltas_y);
    hb_మార్పిడి (a.compiled_tuple_శీర్షిక, b.compiled_tuple_శీర్షిక);
    hb_మార్పిడి (a.compiled_deltas, b.compiled_deltas);
    hb_మార్పిడి (a.compiled_peak_coords, b.compiled_peak_coords);
  }

  tuple_delta_t (tuple_delta_t&& o)  noexcept : tuple_delta_t ()
  { hb_మార్పిడి (*ఇది, o); }

  tuple_delta_t& క్రియాకారకం = (tuple_delta_t&& o) noexcept
  {
    hb_మార్పిడి (*ఇది, o);
    తిరిగి *ఇది;
  }

  శూన్య copy_from (స్థిరరాశి tuple_delta_t& o, hb_alloc_pool_t *pool = శూన్యసూచిక)
  {
    axis_tuples = o.axis_tuples;
    indices.duplicate_సదిశ_from_pool (pool, o.indices);
    deltas_x.duplicate_సదిశ_from_pool (pool, o.deltas_x);
    deltas_y.duplicate_సదిశ_from_pool (pool, o.deltas_y);
    compiled_tuple_శీర్షిక.duplicate_సదిశ_from_pool (pool, o.compiled_tuple_శీర్షిక);
    compiled_deltas.duplicate_సదిశ_from_pool (pool, o.compiled_deltas);
    compiled_peak_coords.duplicate_సదిశ_from_pool (pool, o.compiled_peak_coords);
    compiled_interm_coords.duplicate_సదిశ_from_pool (pool, o.compiled_interm_coords);
  }

  శూన్య remove_axis (hb_ట్యాగ్_t axis_ట్యాగ్_2)
  { axis_tuples.del (axis_ట్యాగ్_2); }

  తార్కిక సమితి_tent (hb_ట్యాగ్_t axis_ట్యాగ్_2, Triple tent)
  { తిరిగి axis_tuples.సమితి (axis_ట్యాగ్_2, tent); }

  tuple_delta_t& క్రియాకారకం += (స్థిరరాశి tuple_delta_t& o)
  {
    గుర్తులేని అంకె = indices.నిడివి;
    కొరకు (గుర్తులేని i = 0; i < అంకె; i++)
    {
      ఒకవేళ (indices.arrayZ[i])
      {
        ఒకవేళ (o.indices.arrayZ[i])
        {
          deltas_x[i] += o.deltas_x[i];
          ఒకవేళ (deltas_y && o.deltas_y)
            deltas_y[i] += o.deltas_y[i];
        }
      }
      లేకపోతే
      {
        ఒకవేళ (!o.indices.arrayZ[i]) కొనసాగు;
        indices.arrayZ[i] = నిజం;
        deltas_x[i] = o.deltas_x[i];
        ఒకవేళ (deltas_y && o.deltas_y)
          deltas_y[i] = o.deltas_y[i];
      }
    }
    తిరిగి *ఇది;
  }

  tuple_delta_t& క్రియాకారకం *= (దశాంశం scalar)
  {
    ఒకవేళ (scalar == 1.0f)
      తిరిగి *ఇది;

    గుర్తులేని అంకె = indices.నిడివి;
    ఒకవేళ (deltas_y)
      కొరకు (గుర్తులేని i = 0; i < అంకె; i++)
      {
	ఒకవేళ (!indices.arrayZ[i]) కొనసాగు;
	deltas_x[i] *= scalar;
	deltas_y[i] *= scalar;
      }
    లేకపోతే
      కొరకు (గుర్తులేని i = 0; i < అంకె; i++)
      {
	ఒకవేళ (!indices.arrayZ[i]) కొనసాగు;
	deltas_x[i] *= scalar;
      }
    తిరిగి *ఇది;
  }

  శూన్య change_tuple_var_axis_హద్దు (hb_ట్యాగ్_t axis_ట్యాగ్_2, Triple axis_హద్దు,
				    TripleDistances axis_triple_distances,
				    hb_సదిశ_t<tuple_delta_t>& బహిర్గతం,
				    rebase_tent_ఫలితం_scratch_t &scratch,
				    hb_alloc_pool_t *pool = శూన్యసూచిక)
  {
    // May move *this out.

    బహిర్గతం.reset ();
    Triple *tent;
    ఒకవేళ (!axis_tuples.has (axis_ట్యాగ్_2, &tent))
    {
      బహిర్గతం.push (std::move (*ఇది));
      తిరిగి;
    }

    ఒకవేళ ((tent->minimum < 0.f && tent->గరిష్ఠ > 0.f) ||
        !(tent->minimum <= tent->మధ్య && tent->మధ్య <= tent->గరిష్ఠ))
      తిరిగి;

    ఒకవేళ (tent->మధ్య == 0.f)
    {
      బహిర్గతం.push (std::move (*ఇది));
      తిరిగి;
    }

    ఒకవేళ (!axis_హద్దు.is_బిందువు () &&
        !(-1.f <= axis_హద్దు.minimum &&
          axis_హద్దు.minimum <= axis_హద్దు.మధ్య &&
          axis_హద్దు.మధ్య <= axis_హద్దు.గరిష్ఠ &&
          axis_హద్దు.గరిష్ఠ <= +1.f))
      తిరిగి;

    rebase_tent_ఫలితం_t &పరిష్కారాలు = scratch.మొదటి;
    rebase_tent (*tent, axis_హద్దు, axis_triple_distances, పరిష్కారాలు, scratch.second);
    కొరకు (గుర్తులేని i = 0; i < పరిష్కారాలు.నిడివి; i++)
    {
      స్వయంచాలక &t = పరిష్కారాలు.arrayZ[i];

      tuple_delta_t new_var;
      ఒకవేళ (i < పరిష్కారాలు.నిడివి - 1)
	new_var.copy_from (*ఇది, pool);
      లేకపోతే
	new_var = std::move (*ఇది);

      ఒకవేళ (t.second == Triple ())
        new_var.remove_axis (axis_ట్యాగ్_2);
      లేకపోతే
        new_var.సమితి_tent (axis_ట్యాగ్_2, t.second);

      new_var *= t.మొదటి;
      బహిర్గతం.push (std::move (new_var));
    }
  }

  తార్కిక compile_coords (స్థిరరాశి hb_map_t& axes_సూచీ_map,
		       స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map,
		       hb_alloc_pool_t *pool= శూన్యసూచిక)
  {
    గుర్తులేని cur_axis_లెక్క = axes_సూచీ_map.get_population ();
    ఒకవేళ (pool)
    {
      ఒకవేళ (unlikely (!compiled_peak_coords.allocate_from_pool (pool, cur_axis_లెక్క)))
	తిరిగి అబద్ధం;
    }
    లేకపోతే ఒకవేళ (unlikely (!compiled_peak_coords.resize (cur_axis_లెక్క)))
      తిరిగి అబద్ధం;

    hb_array_t<F2DOT14> ప్రారంభం_coords, చివర_coords;

    గుర్తులేని orig_axis_లెక్క = axes_old_సూచీ_ట్యాగ్_map.get_population ();
    గుర్తులేని j = 0;
    కొరకు (గుర్తులేని i = 0; i < orig_axis_లెక్క; i++)
    {
      ఒకవేళ (!axes_సూచీ_map.has (i))
        కొనసాగు;

      hb_ట్యాగ్_t axis_ట్యాగ్_2 = axes_old_సూచీ_ట్యాగ్_map.get (i);
      Triple *coords = శూన్యసూచిక;
      ఒకవేళ (axis_tuples.has (axis_ట్యాగ్_2, &coords))
      {
	దశాంశం min_val = coords->minimum;
	దశాంశం val = coords->మధ్య;
	దశాంశం max_val = coords->గరిష్ఠ;

	compiled_peak_coords.arrayZ[j].సమితి_float (val);

	ఒకవేళ (min_val != hb_min (val, 0.f) || max_val != hb_max (val, 0.f))
	{
	  ఒకవేళ (!compiled_interm_coords)
	  {
	    ఒకవేళ (pool)
	    {
	      ఒకవేళ (unlikely (!compiled_interm_coords.allocate_from_pool (pool, 2 * cur_axis_లెక్క)))
		తిరిగి అబద్ధం;
	    }
	    లేకపోతే ఒకవేళ (unlikely (!compiled_interm_coords.resize (2 * cur_axis_లెక్క)))
	      తిరిగి అబద్ధం;
	    ప్రారంభం_coords = compiled_interm_coords.as_array ().sub_array (0, cur_axis_లెక్క);
	    చివర_coords = compiled_interm_coords.as_array ().sub_array (cur_axis_లెక్క);

	    కొరకు (గుర్తులేని k = 0; k < j; k++)
	    {
	      గుర్తుగల peak = compiled_peak_coords.arrayZ[k].గమ్యం_int ();
	      ఒకవేళ (!peak) కొనసాగు;
	      ప్రారంభం_coords.arrayZ[k].సమితి_int (hb_min (peak, 0));
	      చివర_coords.arrayZ[k].సమితి_int (hb_max (peak, 0));
	    }
	  }

	}

	ఒకవేళ (compiled_interm_coords)
	{
	  ప్రారంభం_coords.arrayZ[j].సమితి_float (min_val);
	  చివర_coords.arrayZ[j].సమితి_float (max_val);
	}
      }

      j++;
    }

    తిరిగి !compiled_peak_coords.లోపల_error () && !compiled_interm_coords.లోపల_error ();
  }

  /* deltas should be compiled already before we compile tuple
   * variation header cause we need to fill in the size of the
   * serialized data for this tuple variation */
  తార్కిక compile_tuple_var_శీర్షిక (స్థిరరాశి hb_map_t& axes_సూచీ_map,
                                 గుర్తులేని points_దత్తాంశం_నిడివి,
                                 స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map,
                                 స్థిరరాశి hb_hashmap_t<స్థిరరాశి hb_సదిశ_t<F2DOT14>*, గుర్తులేని>* shared_tuples_idx_map,
				 hb_alloc_pool_t *pool = శూన్యసూచిక)
  {
    /* compiled_deltas could be empty after iup delta optimization, we can skip
     * compiling this tuple and return true */
    ఒకవేళ (!compiled_deltas) తిరిగి నిజం;

    గుర్తులేని cur_axis_లెక్క = axes_సూచీ_map.get_population ();
    /* allocate enough memory: 1 peak + 2 intermediate coords + fixed header size */
    గుర్తులేని alloc_len = 3 * cur_axis_లెక్క * (F2DOT14::static_కొలత) + 4;
    ఒకవేళ (unlikely (!compiled_tuple_శీర్షిక.allocate_from_pool (pool, alloc_len, అబద్ధం))) తిరిగి అబద్ధం;

    గుర్తులేని flag = 0;
    /* skip the first 4 header bytes: variationDataSize+tupleIndex */
    F2DOT14* p = reinterpret_cast<F2DOT14 *> (compiled_tuple_శీర్షిక.begin () + 4);
    F2DOT14* చివర = reinterpret_cast<F2DOT14 *> (compiled_tuple_శీర్షిక.చివర ());
    hb_array_t<F2DOT14> coords (p, చివర - p);

    ఒకవేళ (!shared_tuples_idx_map)
      compile_coords (axes_సూచీ_map, axes_old_సూచీ_ట్యాగ్_map); // non-gvar tuples do not have compiled coords yet

    /* encode peak coords */
    గుర్తులేని peak_లెక్క = 0;
    గుర్తులేని *shared_tuple_idx;
    ఒకవేళ (shared_tuples_idx_map &&
        shared_tuples_idx_map->has (&compiled_peak_coords, &shared_tuple_idx))
    {
      flag = *shared_tuple_idx;
    }
    లేకపోతే
    {
      peak_లెక్క = encode_peak_coords(coords, flag);
      ఒకవేళ (!peak_లెక్క) తిరిగి అబద్ధం;
    }

    /* encode interim coords, it's optional so returned num could be 0 */
    గుర్తులేని interim_లెక్క = encode_interm_coords (coords.sub_array (peak_లెక్క), flag);

    /* pointdata length = 0 implies "use shared points" */
    ఒకవేళ (points_దత్తాంశం_నిడివి)
      flag |= Tuple_Variation_శీర్షిక::Tuple_సూచీ::వ్యక్తిగత_బిందువు_Numbers;

    గుర్తులేని serialized_దత్తాంశం_కొలత = points_దత్తాంశం_నిడివి + compiled_deltas.నిడివి;
    Tuple_Variation_శీర్షిక *o = reinterpret_cast<Tuple_Variation_శీర్షిక *> (compiled_tuple_శీర్షిక.begin ());
    o->var_దత్తాంశం_కొలత = serialized_దత్తాంశం_కొలత;
    o->tuple_సూచీ = flag;

    గుర్తులేని మొత్తం_శీర్షిక_len = 4 + (peak_లెక్క + interim_లెక్క) * (F2DOT14::static_కొలత);
    compiled_tuple_శీర్షిక.shrink_వెనుక_గమ్యం_pool (pool, మొత్తం_శీర్షిక_len);
    తిరిగి నిజం;
  }

  గుర్తులేని encode_peak_coords (hb_array_t<F2DOT14> peak_coords,
                               గుర్తులేని& flag) స్థిరరాశి
  {
    hb_memcpy (&peak_coords[0], &compiled_peak_coords[0], compiled_peak_coords.నిడివి * పరిమాణం (compiled_peak_coords[0]));
    flag |= Tuple_Variation_శీర్షిక::Tuple_సూచీ::EmbeddedPeakTuple;
    తిరిగి compiled_peak_coords.నిడివి;
  }

  /* if no need to encode intermediate coords, then just return p */
  గుర్తులేని encode_interm_coords (hb_array_t<F2DOT14> coords,
                                 గుర్తులేని& flag) స్థిరరాశి
  {
    ఒకవేళ (compiled_interm_coords)
    {
      hb_memcpy (&coords[0], &compiled_interm_coords[0], compiled_interm_coords.నిడివి * పరిమాణం (compiled_interm_coords[0]));
      flag |= Tuple_Variation_శీర్షిక::Tuple_సూచీ::IntermediateRegion;
    }
    తిరిగి compiled_interm_coords.నిడివి;
  }

  తార్కిక compile_deltas (hb_సదిశ_t<పూర్ణాంకం> &rounded_deltas_scratch,
		       hb_alloc_pool_t *pool = శూన్యసూచిక)
  { తిరిగి compile_deltas (indices, deltas_x, deltas_y, compiled_deltas, rounded_deltas_scratch, pool); }

  స్థిర తార్కిక compile_deltas (hb_array_t<స్థిరరాశి తార్కిక> బిందువు_indices,
			      hb_array_t<స్థిరరాశి దశాంశం> x_deltas,
			      hb_array_t<స్థిరరాశి దశాంశం> y_deltas,
			      hb_సదిశ_t<గుర్తులేని అక్షరం> &compiled_deltas, /*బహిర్గతం*/
			      hb_సదిశ_t<పూర్ణాంకం> &rounded_deltas, /* scratch */
			      hb_alloc_pool_t *pool = శూన్యసూచిక)
  {
    ఒకవేళ (unlikely (!rounded_deltas.resize_dirty  (బిందువు_indices.నిడివి)))
      తిరిగి అబద్ధం;

    గుర్తులేని j = 0;
    కొరకు (గుర్తులేని i = 0; i < బిందువు_indices.నిడివి; i++)
    {
      ఒకవేళ (!బిందువు_indices[i]) కొనసాగు;
      rounded_deltas.arrayZ[j++] = hb_clamp_గమ్యం<పూర్ణాంకం> (roundf ((ద్విగుణ) x_deltas.arrayZ[i]));
    }
    rounded_deltas.resize (j);

    ఒకవేళ (!rounded_deltas) తిరిగి నిజం;
    /* Allocate enough memory: this is the correct bound:
     * Worst case scenario is that each delta has to be encoded in 4 bytes, and there
     * are runs of 64 items each. Any delta encoded in less than 4 bytes (2, 1, or 0)
     * is still smaller than the 4-byte encoding even with their control byte.
     * The initial 2 is to handle length==0, for both x and y deltas. */
    గుర్తులేని alloc_len = 2 + 4 * rounded_deltas.నిడివి + (rounded_deltas.నిడివి + 63) / 64;
    ఒకవేళ (y_deltas)
      alloc_len *= 2;

    ఒకవేళ (unlikely (!compiled_deltas.allocate_from_pool (pool, alloc_len, అబద్ధం))) తిరిగి అబద్ధం;

    గుర్తులేని encoded_len = compile_deltas (compiled_deltas, rounded_deltas);

    ఒకవేళ (y_deltas)
    {
      /* reuse the rounded_deltas vector, check that y_deltas have the same num of deltas as x_deltas */
      గుర్తులేని j = 0;
      కొరకు (గుర్తులేని idx = 0; idx < బిందువు_indices.నిడివి; idx++)
      {
        ఒకవేళ (!బిందువు_indices[idx]) కొనసాగు;
        పూర్ణాంకం rounded_delta = hb_clamp_గమ్యం<పూర్ణాంకం> (roundf ((ద్విగుణ) y_deltas.arrayZ[idx]));

        ఒకవేళ (j >= rounded_deltas.నిడివి) తిరిగి అబద్ధం;

        rounded_deltas[j++] = rounded_delta;
      }

      ఒకవేళ (j != rounded_deltas.నిడివి) తిరిగి అబద్ధం;
      encoded_len += compile_deltas (compiled_deltas.as_array ().sub_array (encoded_len), rounded_deltas);
    }
    compiled_deltas.shrink_వెనుక_గమ్యం_pool (pool, encoded_len);
    తిరిగి నిజం;
  }

  స్థిర గుర్తులేని compile_deltas (hb_array_t<గుర్తులేని అక్షరం> encoded_బైట్లు,
				  hb_array_t<స్థిరరాశి పూర్ణాంకం> deltas)
  {
    తిరిగి Tuple_విలువలు::compile_unsafe (deltas, encoded_బైట్లు);
  }

  తార్కిక calc_inferred_deltas (స్థిరరాశి contour_బిందువు_సదిశ_t& orig_points,
			     hb_సదిశ_t<గుర్తులేని> &scratch)
  {
    గుర్తులేని బిందువు_లెక్క = orig_points.నిడివి;
    ఒకవేళ (బిందువు_లెక్క != indices.నిడివి)
      తిరిగి అబద్ధం;

    గుర్తులేని ref_లెక్క_2 = 0;

    hb_సదిశ_t<గుర్తులేని> &చివర_points = scratch.reset ();

    కొరకు (గుర్తులేని i = 0; i < బిందువు_లెక్క; i++)
    {
      ref_లెక్క_2 += indices.arrayZ[i];
      ఒకవేళ (orig_points.arrayZ[i].is_చివర_బిందువు)
        చివర_points.push (i);
    }
    /* all points are referenced, nothing to do */
    ఒకవేళ (ref_లెక్క_2 == బిందువు_లెక్క)
      తిరిగి నిజం;
    ఒకవేళ (unlikely (చివర_points.లోపల_error ())) తిరిగి అబద్ధం;

    hb_బిట్టు_సమితి_t inferred_idxes;
    గుర్తులేని ప్రారంభం_బిందువు_2 = 0;
    కొరకు (గుర్తులేని చివర_బిందువు : చివర_points)
    {
      /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
      గుర్తులేని unref_లెక్క = 0;
      కొరకు (గుర్తులేని i = ప్రారంభం_బిందువు_2; i < చివర_బిందువు + 1; i++)
        unref_లెక్క += indices.arrayZ[i];
      unref_లెక్క = (చివర_బిందువు - ప్రారంభం_బిందువు_2 + 1) - unref_లెక్క;

      గుర్తులేని j = ప్రారంభం_బిందువు_2;
      ఒకవేళ (unref_లెక్క == 0 || unref_లెక్క > చివర_బిందువు - ప్రారంభం_బిందువు_2)
        వెళ్ళు no_మరిన్ని_gaps;
      కొరకు (;;)
      {
        /* Locate the next gap of unreferenced points between two referenced points prev and next.
         * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
         */
        గుర్తులేని పూర్ణాంకం prev, next, i;
        కొరకు (;;)
        {
          i = j;
          j = తదుపరి_సూచీ (i, ప్రారంభం_బిందువు_2, చివర_బిందువు);
          ఒకవేళ (indices.arrayZ[i] && !indices.arrayZ[j]) ఆపు;
        }
        prev = j = i;
        కొరకు (;;)
        {
          i = j;
          j = తదుపరి_సూచీ (i, ప్రారంభం_బిందువు_2, చివర_బిందువు);
          ఒకవేళ (!indices.arrayZ[i] && indices.arrayZ[j]) ఆపు;
        }
        next = j;
       /* Infer deltas for all unref points in the gap between prev and next */
        i = prev;
        కొరకు (;;)
        {
          i = తదుపరి_సూచీ (i, ప్రారంభం_బిందువు_2, చివర_బిందువు);
          ఒకవేళ (i == next) ఆపు;
          deltas_x.arrayZ[i] = infer_delta (orig_points.arrayZ[i].x,
                                            orig_points.arrayZ[prev].x,
                                            orig_points.arrayZ[next].x,
                                            deltas_x.arrayZ[prev], deltas_x.arrayZ[next]);
          deltas_y.arrayZ[i] = infer_delta (orig_points.arrayZ[i].y,
                                            orig_points.arrayZ[prev].y,
                                            orig_points.arrayZ[next].y,
                                            deltas_y.arrayZ[prev], deltas_y.arrayZ[next]);
          inferred_idxes.కూడు (i);
          ఒకవేళ (--unref_లెక్క == 0) వెళ్ళు no_మరిన్ని_gaps;
        }
      }
    no_మరిన్ని_gaps:
      ప్రారంభం_బిందువు_2 = చివర_బిందువు + 1;
    }

    కొరకు (గుర్తులేని i = 0; i < బిందువు_లెక్క; i++)
    {
      /* if points are not referenced and deltas are not inferred, set to 0.
       * reference all points for gvar */
      ఒకవేళ ( !indices[i])
      {
        ఒకవేళ (!inferred_idxes.has (i))
        {
          deltas_x.arrayZ[i] = 0.f;
          deltas_y.arrayZ[i] = 0.f;
        }
        indices[i] = నిజం;
      }
    }
    తిరిగి నిజం;
  }

  తార్కిక optimize (స్థిరరాశి contour_బిందువు_సదిశ_t& contour_points,
                 తార్కిక is_సంయుక్త,
		 optimize_scratch_t &scratch,
                 ద్విగుణ tolerance = 0.5 + 1e-10)
  {
    గుర్తులేని లెక్క = contour_points.నిడివి;
    ఒకవేళ (deltas_x.నిడివి != లెక్క ||
        deltas_y.నిడివి != లెక్క)
      తిరిగి అబద్ధం;

    hb_సదిశ_t<తార్కిక> &opt_indices = scratch.opt_indices.reset ();
    hb_సదిశ_t<పూర్ణాంకం> &rounded_x_deltas = scratch.rounded_x_deltas;
    hb_సదిశ_t<పూర్ణాంకం> &rounded_y_deltas = scratch.rounded_y_deltas;

    ఒకవేళ (unlikely (!rounded_x_deltas.resize_dirty  (లెక్క) ||
                  !rounded_y_deltas.resize_dirty  (లెక్క)))
      తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      rounded_x_deltas.arrayZ[i] = hb_clamp_గమ్యం<పూర్ణాంకం> (roundf ((ద్విగుణ) deltas_x.arrayZ[i]));
      rounded_y_deltas.arrayZ[i] = hb_clamp_గమ్యం<పూర్ణాంకం> (roundf ((ద్విగుణ) deltas_y.arrayZ[i]));
    }

    ఒకవేళ (!iup_delta_optimize (contour_points, rounded_x_deltas, rounded_y_deltas, opt_indices, scratch.iup, tolerance))
      తిరిగి అబద్ధం;

    గుర్తులేని ref_లెక్క_2 = 0;
    కొరకు (తార్కిక ref_flag : opt_indices)
       ref_లెక్క_2 += ref_flag;

    ఒకవేళ (ref_లెక్క_2 == లెక్క) తిరిగి నిజం;

    hb_సదిశ_t<దశాంశం> &opt_deltas_x = scratch.opt_deltas_x.reset ();
    hb_సదిశ_t<దశాంశం> &opt_deltas_y = scratch.opt_deltas_y.reset ();
    తార్కిక is_comp_glyph_wo_deltas = (is_సంయుక్త && ref_లెక్క_2 == 0);
    ఒకవేళ (is_comp_glyph_wo_deltas)
    {
      ఒకవేళ (unlikely (!opt_deltas_x.resize (లెక్క) ||
                    !opt_deltas_y.resize (లెక్క)))
        తిరిగి అబద్ధం;

      opt_indices.arrayZ[0] = నిజం;
      కొరకు (గుర్తులేని i = 1; i < లెక్క; i++)
        opt_indices.arrayZ[i] = అబద్ధం;
    }

    hb_సదిశ_t<గుర్తులేని అక్షరం> &opt_బిందువు_దత్తాంశం = scratch.opt_బిందువు_దత్తాంశం.reset ();
    ఒకవేళ (!compile_బిందువు_సమితి (opt_indices, opt_బిందువు_దత్తాంశం))
      తిరిగి అబద్ధం;
    hb_సదిశ_t<గుర్తులేని అక్షరం> &opt_deltas_దత్తాంశం = scratch.opt_deltas_దత్తాంశం.reset ();
    ఒకవేళ (!compile_deltas (opt_indices,
                         is_comp_glyph_wo_deltas ? opt_deltas_x : deltas_x,
                         is_comp_glyph_wo_deltas ? opt_deltas_y : deltas_y,
                         opt_deltas_దత్తాంశం,
			 scratch.rounded_deltas))
      తిరిగి అబద్ధం;

    hb_సదిశ_t<గుర్తులేని అక్షరం> &బిందువు_దత్తాంశం = scratch.బిందువు_దత్తాంశం.reset ();
    ఒకవేళ (!compile_బిందువు_సమితి (indices, బిందువు_దత్తాంశం))
      తిరిగి అబద్ధం;
    hb_సదిశ_t<గుర్తులేని అక్షరం> &deltas_దత్తాంశం = scratch.deltas_దత్తాంశం.reset ();
    ఒకవేళ (!compile_deltas (indices, deltas_x, deltas_y, deltas_దత్తాంశం, scratch.rounded_deltas))
      తిరిగి అబద్ధం;

    ఒకవేళ (opt_బిందువు_దత్తాంశం.నిడివి + opt_deltas_దత్తాంశం.నిడివి < బిందువు_దత్తాంశం.నిడివి + deltas_దత్తాంశం.నిడివి)
    {
      indices = std::move (opt_indices);

      ఒకవేళ (is_comp_glyph_wo_deltas)
      {
        deltas_x = std::move (opt_deltas_x);
        deltas_y = std::move (opt_deltas_y);
      }
    }
    తిరిగి !indices.లోపల_error () && !deltas_x.లోపల_error () && !deltas_y.లోపల_error ();
  }

  స్థిర తార్కిక compile_బిందువు_సమితి (స్థిరరాశి hb_సదిశ_t<తార్కిక> &బిందువు_indices,
                                 hb_సదిశ_t<గుర్తులేని అక్షరం>& compiled_points /*బహిర్గతం*/)
  {
    గుర్తులేని అంకె_points = 0;
    కొరకు (తార్కిక i : బిందువు_indices)
      ఒకవేళ (i) అంకె_points++;

    /* when iup optimization is enabled, num of referenced points could be 0 */
    ఒకవేళ (!అంకె_points) తిరిగి నిజం;

    గుర్తులేని indices_నిడివి = బిందువు_indices.నిడివి;
    /* If the points set consists of all points in the glyph, it's encoded with a
     * single zero byte */
    ఒకవేళ (అంకె_points == indices_నిడివి)
      తిరిగి compiled_points.resize (1);

    /* allocate enough memories: 2 bytes for count + 3 bytes for each point */
    గుర్తులేని అంకె_బైట్లు = 2 + 3 *అంకె_points;
    ఒకవేళ (unlikely (!compiled_points.resize_dirty  (అంకె_బైట్లు)))
      తిరిగి అబద్ధం;

    గుర్తులేని pos = 0;
    /* binary data starts with the total number of reference points */
    ఒకవేళ (అంకె_points < 0x80)
      compiled_points.arrayZ[pos++] = అంకె_points;
    లేకపోతే
    {
      compiled_points.arrayZ[pos++] = ((అంకె_points >> 8) | 0x80);
      compiled_points.arrayZ[pos++] = అంకె_points & 0xFF;
    }

    స్థిరరాశి గుర్తులేని max_నిరంతరం_నిడివి = 0x7F;
    గుర్తులేని i = 0;
    గుర్తులేని చివరి_విలువ = 0;
    గుర్తులేని అంకె_encoded = 0;
    అయితే (i < indices_నిడివి && అంకె_encoded < అంకె_points)
    {
      గుర్తులేని నిరంతరం_నిడివి = 0;
      గుర్తులేని శీర్షిక_pos = pos;
      compiled_points.arrayZ[pos++] = 0;

      తార్కిక use_byte_encoding = అబద్ధం;
      తార్కిక new_నిరంతరం_2 = నిజం;
      అయితే (i < indices_నిడివి && అంకె_encoded < అంకె_points &&
             నిరంతరం_నిడివి <= max_నిరంతరం_నిడివి)
      {
        // find out next referenced point index
        అయితే (i < indices_నిడివి && !బిందువు_indices[i])
          i++;

        ఒకవేళ (i >= indices_నిడివి) ఆపు;

        గుర్తులేని cur_విలువ = i;
        గుర్తులేని delta = cur_విలువ - చివరి_విలువ;

        ఒకవేళ (new_నిరంతరం_2)
        {
          use_byte_encoding = (delta <= 0xFF);
          new_నిరంతరం_2 = అబద్ధం;
        }

        ఒకవేళ (use_byte_encoding && delta > 0xFF)
          ఆపు;

        ఒకవేళ (use_byte_encoding)
          compiled_points.arrayZ[pos++] = delta;
        లేకపోతే
        {
          compiled_points.arrayZ[pos++] = delta >> 8;
          compiled_points.arrayZ[pos++] = delta & 0xFF;
        }
        i++;
        చివరి_విలువ = cur_విలువ;
        నిరంతరం_నిడివి++;
        అంకె_encoded++;
      }

      ఒకవేళ (use_byte_encoding)
        compiled_points.arrayZ[శీర్షిక_pos] = నిరంతరం_నిడివి - 1;
      లేకపోతే
        compiled_points.arrayZ[శీర్షిక_pos] = (నిరంతరం_నిడివి - 1) | 0x80;
    }
    తిరిగి compiled_points.resize_dirty  (pos);
  }

  స్థిర దశాంశం infer_delta (దశాంశం లక్ష్యం_val, దశాంశం prev_val, దశాంశం తదుపరి_val, దశాంశం prev_delta, దశాంశం తదుపరి_delta)
  {
    ఒకవేళ (prev_val == తదుపరి_val)
      తిరిగి (prev_delta == తదుపరి_delta) ? prev_delta : 0.f;
    లేకపోతే ఒకవేళ (లక్ష్యం_val <= hb_min (prev_val, తదుపరి_val))
      తిరిగి (prev_val < తదుపరి_val) ? prev_delta : తదుపరి_delta;
    లేకపోతే ఒకవేళ (లక్ష్యం_val >= hb_max (prev_val, తదుపరి_val))
      తిరిగి (prev_val > తదుపరి_val) ? prev_delta : తదుపరి_delta;

    దశాంశం r = (లక్ష్యం_val - prev_val) / (తదుపరి_val - prev_val);
    తిరిగి prev_delta + r * (తదుపరి_delta - prev_delta);
  }

  స్థిర గుర్తులేని పూర్ణాంకం తదుపరి_సూచీ (గుర్తులేని పూర్ణాంకం i, గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
  { తిరిగి (i >= చివర) ? ప్రారంభం : (i + 1); }
};

మూస <రకం_పేరు OffType = HBUINT16>
నిర్మాణం Tuple_Variation_దత్తాంశం
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    // here check on min_size only, TupleVariationHeader and var data will be
    // checked while accessing through iterator.
    return_trace (c->check_struct (ఇది));
  }

  size_t get_కొలత (గుర్తులేని axis_లెక్క_times_2) స్థిరరాశి
  {
    size_t మొత్తం_కొలత = min_కొలత;
    గుర్తులేని లెక్క = tuple_Var_లెక్క.get_లెక్క ();
    స్థిరరాశి Tuple_Variation_శీర్షిక *tuple_var_శీర్షిక = &(get_tuple_var_శీర్షిక());
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      మొత్తం_కొలత = hb_unsigned_కూడు_saturate (మొత్తం_కొలత,
					     tuple_var_శీర్షిక->get_కొలత (axis_లెక్క_times_2),
					     (size_t) tuple_var_శీర్షిక->get_దత్తాంశం_కొలత ());
      tuple_var_శీర్షిక = &tuple_var_శీర్షిక->get_తదుపరి (axis_లెక్క_times_2);
    }

    తిరిగి మొత్తం_కొలత;
  }

  స్థిరరాశి Tuple_Variation_శీర్షిక &get_tuple_var_శీర్షిక (శూన్య) స్థిరరాశి
  { తిరిగి StructAfter<Tuple_Variation_శీర్షిక> (దత్తాంశం); }

  నిర్మాణం tuple_iterator_t;
  నిర్మాణం tuple_variations_t
  {
    hb_సదిశ_t<tuple_delta_t> tuple_vars;

    అంతరంగ:
    /* referenced point set->compiled point data map */
    hb_hashmap_t<స్థిరరాశి hb_సదిశ_t<తార్కిక>*, hb_సదిశ_t<గుర్తులేని అక్షరం>> బిందువు_దత్తాంశం_map;
    /* referenced point set-> count map, used in finding shared points */
    hb_hashmap_t<స్థిరరాశి hb_సదిశ_t<తార్కిక>*, గుర్తులేని> బిందువు_సమితి_లెక్క_map;

    /* empty for non-gvar tuples.
     * shared_points_bytes is a pointer to some value in the point_data_map,
     * which will be freed during map destruction. Save it for serialization, so
     * no need to do find_shared_points () again */
    hb_సదిశ_t<గుర్తులేని అక్షరం> *shared_points_బైట్లు = శూన్యసూచిక;

    /* total compiled byte size as TupleVariationData format, initialized to 0 */
    గుర్తులేని compiled_byte_కొలత = 0;
    తార్కిక needs_padding = అబద్ధం;

    /* for gvar iup delta optimization: whether this is a composite glyph */
    తార్కిక is_సంయుక్త = అబద్ధం;

    బహిరంగ:
    tuple_variations_t () = అప్రమేయ;
    tuple_variations_t (స్థిరరాశి tuple_variations_t&) = తొలగించు;
    tuple_variations_t& క్రియాకారకం=(స్థిరరాశి tuple_variations_t&) = తొలగించు;
    tuple_variations_t (tuple_variations_t&&) = అప్రమేయ;
    tuple_variations_t& క్రియాకారకం=(tuple_variations_t&&) = అప్రమేయ;
    ~tuple_variations_t () = అప్రమేయ;

    స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి తార్కిక (tuple_vars); }
    గుర్తులేని get_var_లెక్క () స్థిరరాశి
    {
      గుర్తులేని లెక్క = 0;
      /* when iup delta opt is enabled, compiled_deltas could be empty and we
       * should skip this tuple */
      కొరకు (స్వయంచాలక& tuple: tuple_vars)
        ఒకవేళ (tuple.compiled_deltas) లెక్క++;

      ఒకవేళ (shared_points_బైట్లు && shared_points_బైట్లు->నిడివి)
        లెక్క |= Tuple_Var_లెక్క::Shared_బిందువు_Numbers;
      తిరిగి లెక్క;
    }

    గుర్తులేని get_compiled_byte_కొలత () స్థిరరాశి
    { తిరిగి compiled_byte_కొలత; }

    తార్కిక create_from_tuple_var_దత్తాంశం (tuple_iterator_t iterator,
                                     గుర్తులేని tuple_var_లెక్క,
                                     గుర్తులేని బిందువు_లెక్క,
                                     తార్కిక is_gvar,
                                     స్థిరరాశి hb_map_t *axes_old_సూచీ_ట్యాగ్_map,
                                     స్థిరరాశి hb_సదిశ_t<గుర్తులేని> &shared_indices,
                                     స్థిరరాశి hb_array_t<స్థిరరాశి F2DOT14> shared_tuples,
				     hb_alloc_pool_t *pool = శూన్యసూచిక,
                                     తార్కిక is_సంయుక్త_glyph = అబద్ధం)
    {
      hb_సదిశ_t<గుర్తులేని> వ్యక్తిగత_indices;
      hb_సదిశ_t<పూర్ణాంకం> deltas_x;
      hb_సదిశ_t<పూర్ణాంకం> deltas_y;
      చేయి
      {
        స్థిరరాశి HBUINT8 *p = iterator.get_serialized_దత్తాంశం ();
        గుర్తులేని పూర్ణాంకం నిడివి = iterator.current_tuple->get_దత్తాంశం_కొలత ();
        ఒకవేళ (unlikely (!iterator.var_దత్తాంశం_బైట్లు.check_range (p, నిడివి)))
          తిరిగి అబద్ధం;

	hb_hashmap_t<hb_ట్యాగ్_t, Triple> axis_tuples;
        ఒకవేళ (!iterator.current_tuple->unpack_axis_tuples (iterator.get_axis_లెక్క (), shared_tuples, axes_old_సూచీ_ట్యాగ్_map, axis_tuples)
            || axis_tuples.is_empty ())
          తిరిగి అబద్ధం;

	వ్యక్తిగత_indices.reset ();
        తార్కిక has_వ్యక్తిగత_points = iterator.current_tuple->has_వ్యక్తిగత_points ();
        స్థిరరాశి HBUINT8 *చివర = p + నిడివి;
        ఒకవేళ (has_వ్యక్తిగత_points &&
            !Tuple_Variation_దత్తాంశం::decompile_points (p, వ్యక్తిగత_indices, చివర))
          తిరిగి అబద్ధం;

        స్థిరరాశి hb_సదిశ_t<గుర్తులేని> &indices = has_వ్యక్తిగత_points ? వ్యక్తిగత_indices : shared_indices;
        తార్కిక apply_గమ్యం_all = (indices.నిడివి == 0);
        గుర్తులేని అంకె_deltas = apply_గమ్యం_all ? బిందువు_లెక్క : indices.నిడివి;

        ఒకవేళ (unlikely (!deltas_x.resize_dirty  (అంకె_deltas) ||
                      !Tuple_Variation_దత్తాంశం::decompile_deltas (p, deltas_x, చివర)))
          తిరిగి అబద్ధం;

        ఒకవేళ (is_gvar)
        {
          ఒకవేళ (unlikely (!deltas_y.resize_dirty  (అంకె_deltas) ||
                        !Tuple_Variation_దత్తాంశం::decompile_deltas (p, deltas_y, చివర)))
            తిరిగి అబద్ధం;
        }

        tuple_delta_t var;
        var.axis_tuples = std::move (axis_tuples);
        ఒకవేళ (unlikely (!var.indices.allocate_from_pool (pool, బిందువు_లెక్క) ||
                      !var.deltas_x.allocate_from_pool (pool, బిందువు_లెక్క, అబద్ధం)))
          తిరిగి అబద్ధం;

        ఒకవేళ (is_gvar && unlikely (!var.deltas_y.allocate_from_pool (pool, బిందువు_లెక్క, అబద్ధం)))
          తిరిగి అబద్ధం;

        కొరకు (గుర్తులేని i = 0; i < అంకె_deltas; i++)
        {
          గుర్తులేని idx = apply_గమ్యం_all ? i : indices[i];
          ఒకవేళ (idx >= బిందువు_లెక్క) కొనసాగు;
          var.indices[idx] = నిజం;
          var.deltas_x[idx] = deltas_x[i];
          ఒకవేళ (is_gvar)
            var.deltas_y[idx] = deltas_y[i];
        }
        tuple_vars.push (std::move (var));
      } అయితే (iterator.move_గమ్యం_తదుపరి ());

      is_సంయుక్త = is_సంయుక్త_glyph;
      తిరిగి నిజం;
    }

    తార్కిక create_from_అంశం_var_దత్తాంశం (స్థిరరాశి Var_దత్తాంశం &var_దత్తాంశం,
                                    స్థిరరాశి hb_సదిశ_t<hb_hashmap_t<hb_ట్యాగ్_t, Triple>>& regions,
                                    స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map,
                                    గుర్తులేని& అంశం_లెక్క_2,
                                    స్థిరరాశి hb_inc_bimap_t* inner_map = శూన్యసూచిక)
    {
      /* NULL offset, to keep original varidx valid, just return */
      ఒకవేళ (&var_దత్తాంశం == &Null (Var_దత్తాంశం))
        తిరిగి నిజం;

      గుర్తులేని అంకె_regions = var_దత్తాంశం.get_region_సూచీ_లెక్క ();
      ఒకవేళ (!tuple_vars.alloc (అంకె_regions)) తిరిగి అబద్ధం;

      అంశం_లెక్క_2 = inner_map ? inner_map->get_population () : var_దత్తాంశం.get_అంశం_లెక్క ();
      ఒకవేళ (!అంశం_లెక్క_2) తిరిగి నిజం;
      గుర్తులేని అడ్డువరుస_కొలత = var_దత్తాంశం.get_అడ్డువరుస_కొలత ();
      స్థిరరాశి HBUINT8 *delta_బైట్లు = var_దత్తాంశం.get_delta_బైట్లు ();

      కొరకు (గుర్తులేని r = 0; r < అంకె_regions; r++)
      {
        /* In VarData, deltas are organized in rows, convert them into
         * column(region) based tuples, resize deltas_x first */
        tuple_delta_t tuple;
        ఒకవేళ (!tuple.deltas_x.resize_dirty  (అంశం_లెక్క_2) ||
            !tuple.indices.resize_dirty  (అంశం_లెక్క_2))
          తిరిగి అబద్ధం;

        కొరకు (గుర్తులేని i = 0; i < అంశం_లెక్క_2; i++)
        {
          tuple.indices.arrayZ[i] = నిజం;
          tuple.deltas_x.arrayZ[i] = var_దత్తాంశం.get_అంశం_delta_fast (inner_map ? inner_map->backward (i) : i,
                                                                   r, delta_బైట్లు, అడ్డువరుస_కొలత);
        }

        గుర్తులేని region_సూచీ = var_దత్తాంశం.get_region_సూచీ (r);
        ఒకవేళ (region_సూచీ >= regions.నిడివి) తిరిగి అబద్ధం;
        tuple.axis_tuples = regions.arrayZ[region_సూచీ];

        tuple_vars.push (std::move (tuple));
      }
      తిరిగి !tuple_vars.లోపల_error ();
    }

    అంతరంగ:
    స్థిర పూర్ణాంకం cmp_axis_ట్యాగ్ (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
    {
      స్థిరరాశి hb_ట్యాగ్_t *a = (స్థిరరాశి hb_ట్యాగ్_t*) pa;
      స్థిరరాశి hb_ట్యాగ్_t *b = (స్థిరరాశి hb_ట్యాగ్_t*) pb;
      తిరిగి (పూర్ణాంకం)(*a) - (పూర్ణాంకం)(*b);
    }

    తార్కిక change_tuple_variations_axis_limits (స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>& normalized_axes_location,
                                              స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, TripleDistances>& axes_triple_distances,
					      hb_alloc_pool_t *pool = శూన్యసూచిక)
    {
      /* sort axis_tag/axis_limits, make result deterministic */
      hb_సదిశ_t<hb_ట్యాగ్_t> axis_ట్యాగులు;
      ఒకవేళ (!axis_ట్యాగులు.alloc (normalized_axes_location.get_population ()))
        తిరిగి అబద్ధం;
      కొరకు (స్వయంచాలక t : normalized_axes_location.కీలకాలు ())
        axis_ట్యాగులు.push (t);

      // Reused vectors for reduced malloc pressure.
      rebase_tent_ఫలితం_scratch_t scratch;
      hb_సదిశ_t<tuple_delta_t> బహిర్గతం;

      axis_ట్యాగులు.qsort (cmp_axis_ట్యాగ్);
      కొరకు (స్వయంచాలక axis_ట్యాగ్_2 : axis_ట్యాగులు)
      {
        Triple *axis_హద్దు;
        ఒకవేళ (!normalized_axes_location.has (axis_ట్యాగ్_2, &axis_హద్దు))
          తిరిగి అబద్ధం;
        TripleDistances axis_triple_distances{1.f, 1.f};
        ఒకవేళ (axes_triple_distances.has (axis_ట్యాగ్_2))
          axis_triple_distances = axes_triple_distances.get (axis_ట్యాగ్_2);

        hb_సదిశ_t<tuple_delta_t> new_vars;
        కొరకు (tuple_delta_t& var : tuple_vars)
        {
	  // This may move var out.
	  var.change_tuple_var_axis_హద్దు (axis_ట్యాగ్_2, *axis_హద్దు, axis_triple_distances, బహిర్గతం, scratch, pool);
          ఒకవేళ (!బహిర్గతం) కొనసాగు;

          గుర్తులేని new_len = new_vars.నిడివి + బహిర్గతం.నిడివి;

          ఒకవేళ (unlikely (!new_vars.alloc (new_len, అబద్ధం)))
            తిరిగి అబద్ధం;

          కొరకు (గుర్తులేని i = 0; i < బహిర్గతం.నిడివి; i++)
            new_vars.push (std::move (బహిర్గతం[i]));
        }
        tuple_vars = std::move (new_vars);
      }
      తిరిగి నిజం;
    }

    /* merge tuple variations with overlapping tents, if iup delta optimization
     * is enabled, add default deltas to contour_points */
    తార్కిక విలీనం_tuple_variations (contour_బిందువు_సదిశ_t* contour_points = శూన్యసూచిక)
    {
      hb_సదిశ_t<tuple_delta_t> new_vars;
      // The pre-allocation is essential for address stability of pointers
      // we store in the hashmap.
      ఒకవేళ (unlikely (!new_vars.alloc (tuple_vars.నిడివి)))
	తిరిగి అబద్ధం;
      hb_hashmap_t<స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>*, గుర్తులేని> m;
      కొరకు (tuple_delta_t& var : tuple_vars)
      {
        /* if all axes are pinned, drop the tuple variation */
        ఒకవేళ (var.axis_tuples.is_empty ())
        {
          /* if iup_delta_optimize is enabled, add deltas to contour coords */
          ఒకవేళ (contour_points && !contour_points->కూడు_deltas (var.deltas_x,
                                                             var.deltas_y,
                                                             var.indices))
            తిరిగి అబద్ధం;
          కొనసాగు;
        }

        గుర్తులేని *idx;
        ఒకవేళ (m.has (&(var.axis_tuples), &idx))
        {
          new_vars[*idx] += var;
        }
        లేకపోతే
        {
	  స్వయంచాలక *new_var = new_vars.push ();
	  ఒకవేళ (unlikely (new_vars.లోపల_error ()))
	    తిరిగి అబద్ధం;
          hb_మార్పిడి (*new_var, var);
          ఒకవేళ (unlikely (!m.సమితి (&(new_var->axis_tuples), new_vars.నిడివి - 1)))
            తిరిగి అబద్ధం;
        }
      }
      m.fini (); // Just in case, since it points into new_vars data.
		 // Shouldn't be necessary though, since we only move new_vars, not its
		 // contents.
      tuple_vars = std::move (new_vars);
      తిరిగి నిజం;
    }

    /* compile all point set and store byte data in a point_set->hb_bytes_t hashmap,
     * also update point_set->count map, which will be used in finding shared
     * point set*/
    తార్కిక compile_all_బిందువు_sets ()
    {
      కొరకు (స్థిరరాశి స్వయంచాలక& tuple: tuple_vars)
      {
        స్థిరరాశి hb_సదిశ_t<తార్కిక>* points_సమితి = &(tuple.indices);
        ఒకవేళ (బిందువు_దత్తాంశం_map.has (points_సమితి))
        {
          గుర్తులేని *లెక్క;
          ఒకవేళ (unlikely (!బిందువు_సమితి_లెక్క_map.has (points_సమితి, &లెక్క) ||
                        !బిందువు_సమితి_లెక్క_map.సమితి (points_సమితి, (*లెక్క) + 1)))
            తిరిగి అబద్ధం;
          కొనసాగు;
        }

        hb_సదిశ_t<గుర్తులేని అక్షరం> compiled_బిందువు_దత్తాంశం;
        ఒకవేళ (!tuple_delta_t::compile_బిందువు_సమితి (*points_సమితి, compiled_బిందువు_దత్తాంశం))
          తిరిగి అబద్ధం;

        ఒకవేళ (!బిందువు_దత్తాంశం_map.సమితి (points_సమితి, std::move (compiled_బిందువు_దత్తాంశం)) ||
            !బిందువు_సమితి_లెక్క_map.సమితి (points_సమితి, 1))
          తిరిగి అబద్ధం;
      }
      తిరిగి నిజం;
    }

    /* find shared points set which saves most bytes */
    శూన్య కనుగొను_shared_points ()
    {
      గుర్తులేని max_saved_బైట్లు = 0;

      కొరకు (స్థిరరాశి స్వయంచాలక& _ : బిందువు_దత్తాంశం_map.iter_ref ())
      {
        స్థిరరాశి hb_సదిశ_t<తార్కిక>* points_సమితి = _.మొదటి;
        గుర్తులేని దత్తాంశం_నిడివి = _.second.నిడివి;
        ఒకవేళ (!దత్తాంశం_నిడివి) కొనసాగు;
        గుర్తులేని *లెక్క;
        ఒకవేళ (unlikely (!బిందువు_సమితి_లెక్క_map.has (points_సమితి, &లెక్క) ||
                      *లెక్క <= 1))
        {
          shared_points_బైట్లు = శూన్యసూచిక;
          తిరిగి;
        }

        గుర్తులేని saved_బైట్లు = దత్తాంశం_నిడివి * ((*లెక్క) -1);
        ఒకవేళ (saved_బైట్లు > max_saved_బైట్లు)
        {
          max_saved_బైట్లు = saved_బైట్లు;
          shared_points_బైట్లు = &(_.second);
        }
      }
    }

    తార్కిక calc_inferred_deltas (స్థిరరాశి contour_బిందువు_సదిశ_t& contour_points,
			       hb_సదిశ_t<గుర్తులేని> &scratch)
    {
      కొరకు (tuple_delta_t& var : tuple_vars)
        ఒకవేళ (!var.calc_inferred_deltas (contour_points, scratch))
          తిరిగి అబద్ధం;

      తిరిగి నిజం;
    }

    తార్కిక iup_optimize (స్థిరరాశి contour_బిందువు_సదిశ_t& contour_points,
		       optimize_scratch_t &scratch)
    {
      కొరకు (tuple_delta_t& var : tuple_vars)
      {
        ఒకవేళ (!var.optimize (contour_points, is_సంయుక్త, scratch))
          తిరిగి అబద్ధం;
      }
      తిరిగి నిజం;
    }

    బహిరంగ:
    /* avar2 partial-instancing culling: drop tuples whose region a partial
     * instance can never reach (hb_subset_plan_t::avar2_reachable_ranges). */
    శూన్య cull_unreachable (స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> &reachable_ranges)
    {
      గుర్తులేని j = 0;
      కొరకు (గుర్తులేని i = 0; i < tuple_vars.నిడివి; i++)
      {
	ఒకవేళ (_hb_avar2_region_is_dead (tuple_vars.arrayZ[i].axis_tuples, reachable_ranges))
	  కొనసాగు;
	ఒకవేళ (i != j)
	  tuple_vars.arrayZ[j] = std::move (tuple_vars.arrayZ[i]);
	j++;
      }
      tuple_vars.shrink (j);
    }

    తార్కిక instantiate (స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>& normalized_axes_location,
                      స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, TripleDistances>& axes_triple_distances,
		      optimize_scratch_t &scratch,
		      hb_alloc_pool_t *pool = శూన్యసూచిక,
                      contour_బిందువు_సదిశ_t* contour_points = శూన్యసూచిక,
                      తార్కిక optimize = అబద్ధం)
    {
      ఒకవేళ (!tuple_vars) తిరిగి నిజం;
      ఒకవేళ (!change_tuple_variations_axis_limits (normalized_axes_location, axes_triple_distances, pool))
        తిరిగి అబద్ధం;
      /* compute inferred deltas only for gvar */
      ఒకవేళ (contour_points)
      {
	hb_సదిశ_t<గుర్తులేని> scratch;
	ఒకవేళ (!calc_inferred_deltas (*contour_points, scratch))
	  తిరిగి అబద్ధం;
      }

      /* if iup delta opt is on, contour_points can't be null */
      ఒకవేళ (optimize && !contour_points)
        తిరిగి అబద్ధం;

      ఒకవేళ (!విలీనం_tuple_variations (optimize ? contour_points : శూన్యసూచిక))
        తిరిగి అబద్ధం;

      ఒకవేళ (optimize && !iup_optimize (*contour_points, scratch)) తిరిగి అబద్ధం;
      తిరిగి !tuple_vars.లోపల_error ();
    }

    తార్కిక compile_బైట్లు (స్థిరరాశి hb_map_t& axes_సూచీ_map,
                        స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map,
                        తార్కిక use_shared_points,
                        తార్కిక is_gvar = అబద్ధం,
                        స్థిరరాశి hb_hashmap_t<స్థిరరాశి hb_సదిశ_t<F2DOT14>*, గుర్తులేని>* shared_tuples_idx_map = శూన్యసూచిక,
			hb_alloc_pool_t *pool = శూన్యసూచిక)
    {
      // return true for empty glyph
      ఒకవేళ (!tuple_vars)
        తిరిగి నిజం;

      // compile points set and store data in hashmap
      ఒకవేళ (!compile_all_బిందువు_sets ())
        తిరిగి అబద్ధం;

      /* total compiled byte size as TupleVariationData format, initialized to its
       * min_size: 4 */
      compiled_byte_కొలత += 4;

      ఒకవేళ (use_shared_points)
      {
        కనుగొను_shared_points ();
        ఒకవేళ (shared_points_బైట్లు)
          compiled_byte_కొలత += shared_points_బైట్లు->నిడివి;
      }
      hb_సదిశ_t<పూర్ణాంకం> rounded_deltas_scratch;
      // compile delta and tuple var header for each tuple variation
      కొరకు (స్వయంచాలక& tuple: tuple_vars)
      {
        స్థిరరాశి hb_సదిశ_t<తార్కిక>* points_సమితి = &(tuple.indices);
        hb_సదిశ_t<గుర్తులేని అక్షరం> *points_దత్తాంశం;
        ఒకవేళ (unlikely (!బిందువు_దత్తాంశం_map.has (points_సమితి, &points_దత్తాంశం)))
          తిరిగి అబద్ధం;

        /* when iup optimization is enabled, num of referenced points could be 0
         * and thus the compiled points bytes is empty, we should skip compiling
         * this tuple */
        ఒకవేళ (!points_దత్తాంశం->నిడివి)
          కొనసాగు;
        ఒకవేళ (!tuple.compile_deltas (rounded_deltas_scratch, pool))
          తిరిగి అబద్ధం;

        గుర్తులేని points_దత్తాంశం_నిడివి = (points_దత్తాంశం != shared_points_బైట్లు) ? points_దత్తాంశం->నిడివి : 0;
        ఒకవేళ (!tuple.compile_tuple_var_శీర్షిక (axes_సూచీ_map, points_దత్తాంశం_నిడివి, axes_old_సూచీ_ట్యాగ్_map,
                                             shared_tuples_idx_map,
					     pool))
          తిరిగి అబద్ధం;
        compiled_byte_కొలత += tuple.compiled_tuple_శీర్షిక.నిడివి + points_దత్తాంశం_నిడివి + tuple.compiled_deltas.నిడివి;
      }

      ఒకవేళ (is_gvar && (compiled_byte_కొలత % 2))
      {
        needs_padding = నిజం;
        compiled_byte_కొలత += 1;
      }

      తిరిగి నిజం;
    }

    తార్కిక serialize_var_శీర్షికలు (hb_serialize_context_t *c, గుర్తులేని& మొత్తం_శీర్షిక_len) స్థిరరాశి
    {
      TRACE_SERIALIZE (ఇది);
      కొరకు (స్థిరరాశి స్వయంచాలక& tuple: tuple_vars)
      {
        tuple.compiled_tuple_శీర్షిక.as_array ().copy (c);
        ఒకవేళ (c->లోపల_error ()) return_trace (అబద్ధం);
        మొత్తం_శీర్షిక_len += tuple.compiled_tuple_శీర్షిక.నిడివి;
      }
      return_trace (నిజం);
    }

    తార్కిక serialize_var_దత్తాంశం (hb_serialize_context_t *c, తార్కిక is_gvar) స్థిరరాశి
    {
      TRACE_SERIALIZE (ఇది);
      ఒకవేళ (is_gvar && shared_points_బైట్లు)
      {
        hb_ubytes_t s (shared_points_బైట్లు->arrayZ, shared_points_బైట్లు->నిడివి);
        s.copy (c);
      }

      కొరకు (స్థిరరాశి స్వయంచాలక& tuple: tuple_vars)
      {
        స్థిరరాశి hb_సదిశ_t<తార్కిక>* points_సమితి = &(tuple.indices);
        hb_సదిశ_t<గుర్తులేని అక్షరం> *బిందువు_దత్తాంశం;
        ఒకవేళ (!బిందువు_దత్తాంశం_map.has (points_సమితి, &బిందువు_దత్తాంశం))
          return_trace (అబద్ధం);

        ఒకవేళ (!is_gvar || బిందువు_దత్తాంశం != shared_points_బైట్లు)
        {
          hb_ubytes_t s (బిందువు_దత్తాంశం->arrayZ, బిందువు_దత్తాంశం->నిడివి);
          s.copy (c);
        }

        tuple.compiled_deltas.as_array ().copy (c);
        ఒకవేళ (c->లోపల_error ()) return_trace (అబద్ధం);
      }

      /* padding for gvar */
      ఒకవేళ (is_gvar && needs_padding)
      {
        HBUINT8 pad;
        pad = 0;
        ఒకవేళ (!c->embed (pad)) return_trace (అబద్ధం);
      }
      return_trace (నిజం);
    }
  };

  నిర్మాణం tuple_iterator_t
  {
    గుర్తులేని get_axis_లెక్క () స్థిరరాశి { తిరిగి axis_లెక్క; }

    శూన్య init (hb_బైట్లు_t var_దత్తాంశం_బైట్లు_2, గుర్తులేని పూర్ణాంకం axis_లెక్క_3, స్థిరరాశి శూన్య *పట్టిక_పునాది)
    {
      var_దత్తాంశం_బైట్లు = var_దత్తాంశం_బైట్లు_2;
      var_దత్తాంశం = var_దత్తాంశం_బైట్లు_2.as<Tuple_Variation_దత్తాంశం> ();
      tuples_ఎడమ = var_దత్తాంశం->tuple_Var_లెక్క.get_లెక్క ();
      axis_లెక్క = axis_లెక్క_3;
      axis_లెక్క_times_2 = axis_లెక్క_3 * 2;
      current_tuple = &var_దత్తాంశం->get_tuple_var_శీర్షిక ();
      దత్తాంశం_స్థానభ్రంశం_2 = 0;
      పట్టిక_పునాది_2 = పట్టిక_పునాది;
    }

    తార్కిక get_shared_indices (hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> &shared_indices /*బహిర్గతం*/)
    {
      ఒకవేళ (var_దత్తాంశం->has_shared_బిందువు_numbers ())
      {
        స్థిరరాశి HBUINT8 *పునాది = &(పట్టిక_పునాది_2+var_దత్తాంశం->దత్తాంశం);
        స్థిరరాశి HBUINT8 *p = పునాది;
        ఒకవేళ (!decompile_points (p, shared_indices, (స్థిరరాశి HBUINT8 *) (var_దత్తాంశం_బైట్లు.arrayZ + var_దత్తాంశం_బైట్లు.నిడివి))) తిరిగి అబద్ధం;
        దత్తాంశం_స్థానభ్రంశం_2 = p - పునాది;
      }
      తిరిగి నిజం;
    }

    తార్కిక is_valid ()
    {
      ఒకవేళ (unlikely (tuples_ఎడమ <= 0))
	తిరిగి అబద్ధం;

      current_tuple_కొలత = Tuple_Variation_శీర్షిక::min_కొలత;
      ఒకవేళ (unlikely (!var_దత్తాంశం_బైట్లు.check_చివర ((స్థిరరాశి అక్షరం *) current_tuple + current_tuple_కొలత)))
	తిరిగి అబద్ధం;

      current_tuple_కొలత = current_tuple->get_కొలత (axis_లెక్క_times_2);
      ఒకవేళ (unlikely (!var_దత్తాంశం_బైట్లు.check_చివర ((స్థిరరాశి అక్షరం *) current_tuple + current_tuple_కొలత)))
	తిరిగి అబద్ధం;

      తిరిగి నిజం;
    }

    HB_ALWAYS_INLINE
    తార్కిక move_గమ్యం_తదుపరి ()
    {
      దత్తాంశం_స్థానభ్రంశం_2 += current_tuple->get_దత్తాంశం_కొలత ();
      current_tuple = &Struct_At_స్థానభ్రంశం<Tuple_Variation_శీర్షిక> (current_tuple, current_tuple_కొలత);
      tuples_ఎడమ--;
      తిరిగి is_valid ();
    }

    // TODO: Make it return (sanitized) hb_bytes_t
    స్థిరరాశి HBUINT8 *get_serialized_దత్తాంశం () స్థిరరాశి
    { తిరిగి &(పట్టిక_పునాది_2+var_దత్తాంశం->దత్తాంశం) + దత్తాంశం_స్థానభ్రంశం_2; }

    అంతరంగ:
    గుర్తుగల tuples_ఎడమ;
    స్థిరరాశి Tuple_Variation_దత్తాంశం *var_దత్తాంశం;
    గుర్తులేని పూర్ణాంకం axis_లెక్క;
    గుర్తులేని పూర్ణాంకం axis_లెక్క_times_2;
    గుర్తులేని పూర్ణాంకం దత్తాంశం_స్థానభ్రంశం_2;
    గుర్తులేని పూర్ణాంకం current_tuple_కొలత;
    స్థిరరాశి శూన్య *పట్టిక_పునాది_2;

    బహిరంగ:
    hb_బైట్లు_t var_దత్తాంశం_బైట్లు;
    స్థిరరాశి Tuple_Variation_శీర్షిక *current_tuple;
  };

  స్థిర తార్కిక get_tuple_iterator (hb_బైట్లు_t var_దత్తాంశం_బైట్లు, గుర్తులేని axis_లెక్క,
                                  స్థిరరాశి శూన్య *పట్టిక_పునాది_2,
                                  hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> &shared_indices /*బహిర్గతం*/,
                                  tuple_iterator_t *iterator /*బహిర్గతం*/)
  {
    iterator->init (var_దత్తాంశం_బైట్లు, axis_లెక్క, పట్టిక_పునాది_2);
    ఒకవేళ (!iterator->get_shared_indices (shared_indices))
      తిరిగి అబద్ధం;
    తిరిగి iterator->is_valid ();
  }

  తార్కిక has_shared_బిందువు_numbers () స్థిరరాశి { తిరిగి tuple_Var_లెక్క.has_shared_బిందువు_numbers (); }

  /* avar2 partial-instancing culling: rewrite this tuple variation data,
   * dropping TupleVariations whose region a partial instance can never
   * reach (see hb_subset_plan_t::avar2_reachable_ranges). On success with
   * *changed set, out contains the rewritten data — possibly empty, when
   * every tuple died. Keeps the original data (*changed stays false) when
   * nothing is culled or the data is malformed. Returns false only on
   * allocation failure. */
  తార్కిక cull_tuple_variations (hb_బైట్లు_t var_దత్తాంశం_బైట్లు,
			      గుర్తులేని axis_లెక్క,
			      hb_array_t<స్థిరరాశి F2DOT14> shared_tuples,
			      స్థిరరాశి hb_map_t *axes_old_సూచీ_ట్యాగ్_map,
			      స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> &reachable_ranges,
			      hb_సదిశ_t<అక్షరం> &బహిర్గతం /*బహిర్గతం*/,
			      తార్కిక *మారింది /*బహిర్గతం*/) స్థిరరాశి
  {
    *మారింది = అబద్ధం;

    hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> shared_indices;
    tuple_iterator_t iterator;
    ఒకవేళ (!get_tuple_iterator (var_దత్తాంశం_బైట్లు, axis_లెక్క, ఇది,
			     shared_indices, &iterator))
      తిరిగి నిజం; /* no tuples or malformed: keep original */

    స్థిరరాశి అక్షరం *బైట్లు_ప్రారంభం = var_దత్తాంశం_బైట్లు.arrayZ;
    స్థిరరాశి అక్షరం *బైట్లు_చివర = బైట్లు_ప్రారంభం + var_దత్తాంశం_బైట్లు.నిడివి;
    స్థిరరాశి అక్షరం *serialized_పునాది = (స్థిరరాశి అక్షరం *) &(ఇది+దత్తాంశం);
    స్థిరరాశి అక్షరం *shared_points_చివర = (స్థిరరాశి అక్షరం *) iterator.get_serialized_దత్తాంశం ();
    ఒకవేళ (unlikely (serialized_పునాది < బైట్లు_ప్రారంభం || serialized_పునాది > బైట్లు_చివర ||
		  shared_points_చివర < serialized_పునాది || shared_points_చివర > బైట్లు_చివర))
      తిరిగి నిజం;

    నిర్మాణం kept_tuple_t
    {
      స్థిరరాశి అక్షరం *శీర్షిక;
      గుర్తులేని శీర్షిక_కొలత;
      స్థిరరాశి అక్షరం *దత్తాంశం;
      గుర్తులేని దత్తాంశం_కొలత;
    };
    hb_సదిశ_t<kept_tuple_t> kept;
    గుర్తులేని మొత్తం = 0;
    గుర్తులేని kept_శీర్షికలు_కొలత = 0, kept_దత్తాంశం_కొలత = 0;
    చేయి
    {
      స్థిరరాశి Tuple_Variation_శీర్షిక *శీర్షిక = iterator.current_tuple;
      గుర్తులేని శీర్షిక_కొలత = శీర్షిక->get_కొలత (axis_లెక్క * 2);
      గుర్తులేని దత్తాంశం_కొలత = శీర్షిక->get_దత్తాంశం_కొలత ();
      స్థిరరాశి అక్షరం *tuple_దత్తాంశం = (స్థిరరాశి అక్షరం *) iterator.get_serialized_దత్తాంశం ();
      ఒకవేళ (unlikely (tuple_దత్తాంశం < serialized_పునాది ||
		    దత్తాంశం_కొలత > (గుర్తులేని) (బైట్లు_చివర - tuple_దత్తాంశం)))
	తిరిగి నిజం; /* malformed: keep original */

      hb_hashmap_t<hb_ట్యాగ్_t, Triple> axis_tuples;
      ఒకవేళ (!శీర్షిక->unpack_axis_tuples (axis_లెక్క, shared_tuples,
				       axes_old_సూచీ_ట్యాగ్_map, axis_tuples))
	తిరిగి నిజం;

      మొత్తం++;
      ఒకవేళ (!_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
      {
	kept.push (kept_tuple_t {(స్థిరరాశి అక్షరం *) శీర్షిక, శీర్షిక_కొలత,
				 tuple_దత్తాంశం, దత్తాంశం_కొలత});
	kept_శీర్షికలు_కొలత += శీర్షిక_కొలత;
	kept_దత్తాంశం_కొలత += దత్తాంశం_కొలత;
      }
    } అయితే (iterator.move_గమ్యం_తదుపరి ());

    ఒకవేళ (unlikely (kept.లోపల_error ())) తిరిగి అబద్ధం;
    ఒకవేళ (kept.నిడివి == మొత్తం) తిరిగి నిజం; /* nothing to cull */

    *మారింది = నిజం;
    ఒకవేళ (!kept.నిడివి) తిరిగి నిజం; /* everything culled: no variation data */

    గుర్తులేని shared_points_కొలత = shared_points_చివర - serialized_పునాది;
    గుర్తులేని దత్తాంశం_స్థానభ్రంశం_2 = min_కొలత + kept_శీర్షికలు_కొలత;
    గుర్తులేని new_కొలత = దత్తాంశం_స్థానభ్రంశం_2 + shared_points_కొలత + kept_దత్తాంశం_కొలత;
    ఒకవేళ (unlikely (!బహిర్గతం.resize (new_కొలత)))
      తిరిగి అబద్ధం;

    Tuple_Variation_దత్తాంశం *బహిర్గతం_దత్తాంశం = (Tuple_Variation_దత్తాంశం *) బహిర్గతం.arrayZ;
    బహిర్గతం_దత్తాంశం->tuple_Var_లెక్క = (uint16_t) (kept.నిడివి |
					  (has_shared_బిందువు_numbers () ? 0x8000u : 0u));
    బహిర్గతం_దత్తాంశం->దత్తాంశం = దత్తాంశం_స్థానభ్రంశం_2;

    అక్షరం *p = బహిర్గతం.arrayZ + min_కొలత;
    కొరకు (స్థిరరాశి స్వయంచాలక &t : kept)
    {
      hb_memcpy (p, t.శీర్షిక, t.శీర్షిక_కొలత);
      p += t.శీర్షిక_కొలత;
    }
    hb_memcpy (p, serialized_పునాది, shared_points_కొలత);
    p += shared_points_కొలత;
    కొరకు (స్థిరరాశి స్వయంచాలక &t : kept)
    {
      hb_memcpy (p, t.దత్తాంశం, t.దత్తాంశం_కొలత);
      p += t.దత్తాంశం_కొలత;
    }
    తిరిగి నిజం;
  }

  స్థిర తార్కిక decompile_points (స్థిరరాశి HBUINT8 *&p /* IN/OUT */,
				hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> &points /*బహిర్గతం*/,
				స్థిరరాశి HBUINT8 *చివర)
  {
    గణన packed_బిందువు_flag_t
    {
      POINTS_ARE_పదాలు     = 0x80,
      బిందువు_నిరంతరం_లెక్క_ముసుగు = 0x7F
    };

    ఒకవేళ (unlikely (p + 1 > చివర)) తిరిగి అబద్ధం;

    గుర్తులేని లెక్క = *p++;
    ఒకవేళ (లెక్క & POINTS_ARE_పదాలు)
    {
      ఒకవేళ (unlikely (p + 1 > చివర)) తిరిగి అబద్ధం;
      లెక్క = ((లెక్క & బిందువు_నిరంతరం_లెక్క_ముసుగు) << 8) | *p++;
    }
    ఒకవేళ (unlikely (!points.resize_dirty  (లెక్క))) తిరిగి అబద్ధం;

    గుర్తులేని n = 0;
    గుర్తులేని i = 0;
    అయితే (i < లెక్క)
    {
      ఒకవేళ (unlikely (p + 1 > చివర)) తిరిగి అబద్ధం;
      గుర్తులేని control = *p++;
      గుర్తులేని నిరంతరం_లెక్క = (control & బిందువు_నిరంతరం_లెక్క_ముసుగు) + 1;
      గుర్తులేని stop = i + నిరంతరం_లెక్క;
      ఒకవేళ (unlikely (stop > లెక్క)) తిరిగి అబద్ధం;
      ఒకవేళ (control & POINTS_ARE_పదాలు)
      {
        ఒకవేళ (unlikely (p + నిరంతరం_లెక్క * HBUINT16::static_కొలత > చివర)) తిరిగి అబద్ధం;
        కొరకు (; i < stop; i++)
        {
          n += *(స్థిరరాశి HBUINT16 *)p;
          points.arrayZ[i] = n;
          p += HBUINT16::static_కొలత;
        }
      }
      లేకపోతే
      {
        ఒకవేళ (unlikely (p + నిరంతరం_లెక్క > చివర)) తిరిగి అబద్ధం;
        కొరకు (; i < stop; i++)
        {
          n += *p++;
          points.arrayZ[i] = n;
        }
      }
    }
    తిరిగి నిజం;
  }

  మూస <రకం_పేరు T>
  HB_ALWAYS_INLINE
  స్థిర తార్కిక decompile_deltas (స్థిరరాశి HBUINT8 *&p /* IN/OUT */,
				hb_సదిశ_t<T> &deltas /* IN/OUT */,
				స్థిరరాశి HBUINT8 *చివర,
				తార్కిక consume_all = అబద్ధం,
				గుర్తులేని ప్రారంభం = 0)
  {
    తిరిగి Tuple_విలువలు::decompile (p, deltas, చివర, consume_all, ప్రారంభం);
  }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి tuple_Var_లెక్క; }

  తార్కిక decompile_tuple_variations (గుర్తులేని బిందువు_లెక్క,
                                   తార్కిక is_gvar,
                                   tuple_iterator_t iterator,
                                   స్థిరరాశి hb_map_t *axes_old_సూచీ_ట్యాగ్_map,
                                   స్థిరరాశి hb_సదిశ_t<గుర్తులేని> &shared_indices,
                                   స్థిరరాశి hb_array_t<స్థిరరాశి F2DOT14> shared_tuples,
                                   tuple_variations_t& tuple_variations, /*బహిర్గతం*/
				   hb_alloc_pool_t *pool = శూన్యసూచిక,
                                   తార్కిక is_సంయుక్త_glyph = అబద్ధం) స్థిరరాశి
  {
    తిరిగి tuple_variations.create_from_tuple_var_దత్తాంశం (iterator, tuple_Var_లెక్క,
                                                        బిందువు_లెక్క, is_gvar,
                                                        axes_old_సూచీ_ట్యాగ్_map,
                                                        shared_indices,
                                                        shared_tuples,
							pool,
                                                        is_సంయుక్త_glyph);
  }

  తార్కిక serialize (hb_serialize_context_t *c,
                  తార్కిక is_gvar,
                  స్థిరరాశి tuple_variations_t& tuple_variations) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    /* empty tuple variations, just return and skip serialization. */
    ఒకవేళ (!tuple_variations) return_trace (నిజం);

    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    ఒకవేళ (!c->check_కేటాయించు (బహిర్గతం->tuple_Var_లెక్క, tuple_variations.get_var_లెక్క (),
                          HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)) return_trace (అబద్ధం);

    గుర్తులేని మొత్తం_శీర్షిక_len = 0;

    ఒకవేళ (!tuple_variations.serialize_var_శీర్షికలు (c, మొత్తం_శీర్షిక_len))
      return_trace (అబద్ధం);

    గుర్తులేని దత్తాంశం_స్థానభ్రంశం_2 = min_కొలత + మొత్తం_శీర్షిక_len;
    ఒకవేళ (!is_gvar) దత్తాంశం_స్థానభ్రంశం_2 += 4;
    ఒకవేళ (!c->check_కేటాయించు (బహిర్గతం->దత్తాంశం, దత్తాంశం_స్థానభ్రంశం_2, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)) return_trace (అబద్ధం);

    తిరిగి tuple_variations.serialize_var_దత్తాంశం (c, is_gvar);
  }

  రక్షిత:
  నిర్మాణం Tuple_Var_లెక్క : HBUINT16
  {
    మిత్ర నిర్మాణం tuple_variations_t;
    తార్కిక has_shared_బిందువు_numbers () స్థిరరాశి { తిరిగి ((*ఇది) & Shared_బిందువు_Numbers); }
    గుర్తులేని పూర్ణాంకం get_లెక్క () స్థిరరాశి { తిరిగి (*ఇది) & లెక్క_ముసుగు; }
    Tuple_Var_లెక్క& క్రియాకారకం = (uint16_t i) { HBUINT16::క్రియాకారకం= (i); తిరిగి *ఇది; }
    స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి get_లెక్క (); }

    రక్షిత:
    గణన Flags
    {
      Shared_బిందువు_Numbers= 0x8000u,
      లెక్క_ముసుగు         = 0x0FFFu
    };
    బహిరంగ:
    DEFINE_కొలత_STATIC (2);
  };

  Tuple_Var_లెక్క tuple_Var_లెక్క;  /* A packed field. The high 4 bits are flags, and the
                                 * low 12 bits are the number of tuple variation tables
                                 * for this glyph. The number of tuple variation tables
                                 * can be any number between 1 and 4095. */
  స్థానభ్రంశం_గమ్యం<HBUINT8, OffType>
                దత్తాంశం;           /* Offset from the start of the base table
                                 * to the serialized data. */
  /* TupleVariationHeader tupleVariationHeaders[] *//* Array of tuple variation headers. */
  బహిరంగ:
  DEFINE_కొలత_MIN (2 + OffType::static_కొలత);
};

// TODO: Move tuple_variations_t to outside of TupleVariationData
ఉపయోగించి tuple_variations_t = Tuple_Variation_దత్తాంశం<HBUINT16>::tuple_variations_t;
నిర్మాణం అంశం_variations_t
{
  ఉపయోగించి region_t = స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>*;
  అంతరంగ:
  /* each subtable is decompiled into a tuple_variations_t, in which all tuples
   * have the same num of deltas (rows) */
  hb_సదిశ_t<tuple_variations_t> vars;

  /* num of retained rows for each subtable, there're 2 cases when var_data is empty:
   * 1. retained item_count is zero
   * 2. regions is empty and item_count is non-zero.
   * when converting to tuples, both will be dropped because the tuple is empty,
   * however, we need to retain 2. as all-zero rows to keep original varidx
   * valid, so we need a way to remember the num of rows for each subtable */
  hb_సదిశ_t<గుర్తులేని> var_దత్తాంశం_అంకె_అడ్డువరుసలు;

  /* original region list, decompiled from item varstore, used when rebuilding
   * region list after instantiation */
  hb_సదిశ_t<hb_hashmap_t<hb_ట్యాగ్_t, Triple>> orig_region_list;

  /* region list: vector of Regions, maintain the original order for the regions
   * that existed before instantiate (), append the new regions at the end.
   * Regions are stored in each tuple already, save pointers only.
   * When converting back to item varstore, unused regions will be pruned */
  hb_సదిశ_t<region_t> region_list;

  /* region -> idx map after instantiation and pruning unused regions */
  hb_hashmap_t<region_t, గుర్తులేని> region_map;

  /* all delta rows after instantiation */
  hb_సదిశ_t<hb_సదిశ_t<పూర్ణాంకం>> delta_అడ్డువరుసలు;
  /* final optimized vector of encoding objects used to assemble the varstore */
  hb_సదిశ_t<delta_అడ్డువరుస_encoding_t> encodings;

  /* old varidxes -> new var_idxes map */
  hb_map_t varidx_map;

  /* has long words */
  తార్కిక has_long = అబద్ధం;

  బహిరంగ:
  తార్కిక has_long_మాట () స్థిరరాశి
  { తిరిగి has_long; }

  స్థిరరాశి hb_సదిశ_t<region_t>& get_region_list () స్థిరరాశి
  { తిరిగి region_list; }

  స్థిరరాశి hb_సదిశ_t<delta_అడ్డువరుస_encoding_t>& get_vardata_encodings () స్థిరరాశి
  { తిరిగి encodings; }

  స్థిరరాశి hb_map_t& get_varidx_map () స్థిరరాశి
  { తిరిగి varidx_map; }

  తార్కిక కూడు_vardata_encoding_for_testing (hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*> &&అడ్డువరుసలు,
                                         గుర్తులేని అంకె_నిలువుల_సంఖ్య)
  {
    encodings.push (delta_అడ్డువరుస_encoding_t (std::move (అడ్డువరుసలు), అంకె_నిలువుల_సంఖ్య));
    తిరిగి !encodings.లోపల_error ();
  }

  తార్కిక compile_varidx_map_for_testing (స్థిరరాశి hb_hashmap_t<గుర్తులేని, స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*>& front_mapping)
  { తిరిగి compile_varidx_map (front_mapping); }

  తార్కిక instantiate (స్థిరరాశి అంశం_Variation_Store& varStore,
                    స్థిరరాశి hb_subset_plan_t *plan,
                    తార్కిక optimize=నిజం,
                    తార్కిక use_no_variation_idx=నిజం,
                    స్థిరరాశి hb_array_t <స్థిరరాశి hb_inc_bimap_t> inner_maps = hb_array_t<స్థిరరాశి hb_inc_bimap_t> (),
                    తార్కిక layout_conditions = అబద్ధం)
  {
    ఒకవేళ (!create_from_అంశం_varstore (varStore, plan->axes_old_సూచీ_ట్యాగ్_map, inner_maps))
      తిరిగి అబద్ధం;
    /* avar2 partial instancing: cull unreachable regions. */
    ఒకవేళ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
      కొరకు (tuple_variations_t& tuple_vars : vars)
	tuple_vars.cull_unreachable (plan->avar2_reachable_ranges);
    ఒకవేళ (!instantiate_tuple_vars (plan->axes_location, plan->axes_triple_distances,
                                 !layout_conditions))
      తిరిగి అబద్ధం;
    ఒకవేళ (layout_conditions &&
        (!instantiate_condition_విలువలు (plan, varStore) || !build_region_list ()))
      తిరిగి అబద్ధం;
    తిరిగి as_అంశం_varstore (optimize, use_no_variation_idx);
  }

  తార్కిక instantiate_condition_విలువలు (స్థిరరాశి hb_subset_plan_t *plan,
                                      స్థిరరాశి అంశం_Variation_Store &varStore)
  {
    // GDEF serialization can be retried; always start from the source values.
    స్వయంచాలక &condition_map = plan->layout_condition_idx_విలువ_map;
    condition_map.clear ();
    ఒకవేళ (!plan->layout_condition_విలువలు) తిరిగి నిజం;
    ఉపయోగించి condition_విలువ_t = hb_pair_t<uintptr_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>>;
    hb_సదిశ_t<condition_విలువ_t> conditions;
    కొరకు (స్వయంచాలక condition : plan->layout_condition_విలువలు.iter ())
      conditions.push (condition);
    ఒకవేళ (unlikely (conditions.లోపల_error ())) తిరిగి అబద్ధం;
    // Row order must not depend on the addresses of the source conditions.
    conditions.qsort ([] (స్థిరరాశి condition_విలువ_t &a, స్థిరరాశి condition_విలువ_t &b) {
      ఒకవేళ (a.second.మొదటి != b.second.మొదటి)
        తిరిగి a.second.మొదటి < b.second.మొదటి ? -1 : 1;
      తిరిగి a.second.second < b.second.second ? -1 :
             a.second.second > b.second.second ? 1 : 0;
    });
    hb_map_t outer_map;
    గుర్తులేని new_outer = 0;
    కొరకు (గుర్తులేని i = 0; i < plan->gdef_varstore_inner_maps.నిడివి; i++)
      ఒకవేళ (plan->gdef_varstore_inner_maps[i].get_population ())
        ఒకవేళ (!outer_map.సమితి (i, new_outer++)) తిరిగి అబద్ధం;
    కొరకు (స్వయంచాలక condition : conditions)
    {
      గుర్తులేని old_idx = condition.second.మొదటి;
      ద్విగుణ value = condition.second.second +
                     varStore.get_delta<ద్విగుణ> (old_idx, plan->normalized_coords.as_array ());
      గుర్తులేని old_outer = old_idx >> 16;
      ఒకవేళ (!outer_map.has (old_outer))
        కొనసాగు;
      గుర్తులేని outer = outer_map.get (old_outer);
      గుర్తులేని inner = plan->gdef_varstore_inner_maps[old_outer].get (old_idx & 0xFFFF);
      గుర్తులేని idx = (outer << 16) | inner;
      ఒకవేళ (outer >= vars.నిడివి || inner >= var_దత్తాంశం_అంకె_అడ్డువరుసలు[outer])
        కొనసాగు;

      ద్విగుణ గరిష్ఠ = fabs ((ద్విగుణ) value);
      తార్కిక variable = అబద్ధం;
      కొరకు (స్థిరరాశి tuple_delta_t &tuple : vars[outer].tuple_vars)
      {
        ఒకవేళ (unlikely (inner >= tuple.deltas_x.నిడివి)) తిరిగి అబద్ధం;
        దశాంశం delta = tuple.deltas_x[inner];
        variable |= delta != 0.f;
        గరిష్ఠ = hb_max (గరిష్ఠ, fabs ((ద్విగుణ) delta));
      }
      ఒకవేళ (!variable)
      {
        ఒకవేళ (!condition_map.సమితి (condition.మొదటి,
                                hb_pair (గుర్తులేని (VarIdx::NO_VARIATION), పూర్ణాంకం (value > 0))))
          తిరిగి అబద్ధం;
        కొనసాగు;
      }

      ద్విగుణ scale = 1.;
      అయితే (గరిష్ఠ * scale > గరిష్ఠ_పూర్ణాంకం)
        scale *= .5;
      // Positive rescaling preserves the Boolean boundary. Prefer exact
      // integers, otherwise use the available 32-bit precision.
      అయితే (గరిష్ఠ * scale * 2 <= గరిష్ఠ_పూర్ణాంకం)
      {
        తార్కిక integral = (ద్విగుణ) value * scale == round ((ద్విగుణ) value * scale);
        కొరకు (స్థిరరాశి tuple_delta_t &tuple : vars[outer].tuple_vars)
          integral &= (ద్విగుణ) tuple.deltas_x[inner] * scale ==
                      round ((ద్విగుణ) tuple.deltas_x[inner] * scale);
        ఒకవేళ (integral) ఆపు;
        scale *= 2;
      }
      పూర్ణాంకం default_విలువ = (పూర్ణాంకం) round ((ద్విగుణ) value * scale);
      ఒకవేళ (scale != 1. || default_విలువ < -32768 || default_విలువ > 32767)
      {
        // A condition may share its row with a positioning value. Give it a
        // private copy before integer rounding discards fractional deltas.
        tuple_variations_t వ్యక్తిగత_అడ్డువరుస;
        కొరకు (స్థిరరాశి tuple_delta_t &tuple : vars[outer].tuple_vars)
        {
          ఒకవేళ (!tuple.deltas_x[inner]) కొనసాగు;
          tuple_delta_t copy;
          copy.axis_tuples = tuple.axis_tuples;
          copy.indices.push (నిజం);
          copy.deltas_x.push ((దశాంశం) round ((ద్విగుణ) tuple.deltas_x[inner] * scale));
          ఒకవేళ (unlikely (copy.axis_tuples.లోపల_error () || copy.indices.లోపల_error () ||
                        copy.deltas_x.లోపల_error ())) తిరిగి అబద్ధం;
          వ్యక్తిగత_అడ్డువరుస.tuple_vars.push (std::move (copy));
        }
        ఒకవేళ (unlikely (vars.నిడివి >= 0xFFFFu)) తిరిగి అబద్ధం;
        ఒకవేళ (unlikely (వ్యక్తిగత_అడ్డువరుస.tuple_vars.లోపల_error ())) తిరిగి అబద్ధం;
        idx = vars.నిడివి << 16;
        vars.push (std::move (వ్యక్తిగత_అడ్డువరుస));
        var_దత్తాంశం_అంకె_అడ్డువరుసలు.push (1);
        ఒకవేళ (unlikely (vars.లోపల_error () || var_దత్తాంశం_అంకె_అడ్డువరుసలు.లోపల_error ())) తిరిగి అబద్ధం;
        ఒకవేళ (default_విలువ < -32768 || default_విలువ > 32767)
        {
          // Keep the Boolean result in the int16 default: no deltas apply
          // at the default instance. A neutral region carries the excess.
          // Each part is exact in float; integer row assembly adds them
          // before serialization, preserving all 32 bits of the bias.
          పూర్ణాంకం bias = default_విలువ - hb_clamp (default_విలువ, -32768, 32767);
          default_విలువ -= bias;
          పూర్ణాంకం ఎగువ = bias / 65536 * 65536;
          పూర్ణాంకం parts[] = {ఎగువ, bias - ఎగువ};
          కొరకు (పూర్ణాంకం part : parts)
          {
            ఒకవేళ (!part) కొనసాగు;
            గుర్తులేని లెక్క = vars[idx >> 16].tuple_vars.నిడివి;
            కూడు_tuple (idx >> 16, hb_hashmap_t<hb_ట్యాగ్_t, Triple> (),
                       0, part, 1);
            స్థిరరాశి స్వయంచాలక &tuples = vars[idx >> 16].tuple_vars;
            ఒకవేళ (unlikely (tuples.లోపల_error () || tuples.నిడివి != లెక్క + 1 ||
                          tuples.తోక ().indices.లోపల_error () ||
                          tuples.తోక ().deltas_x.లోపల_error ())) తిరిగి అబద్ధం;
          }
        }
      }
      ఒకవేళ (!condition_map.సమితి (condition.మొదటి, hb_pair (idx, default_విలువ)))
        తిరిగి అబద్ధం;
    }
    తిరిగి !condition_map.లోపల_error ();
  }

  /* keep below APIs public only for unit test: test-item-varstore */
  తార్కిక create_from_అంశం_varstore (స్థిరరాశి అంశం_Variation_Store& varStore,
                                  స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map,
                                  స్థిరరాశి hb_array_t <స్థిరరాశి hb_inc_bimap_t> inner_maps = hb_array_t<స్థిరరాశి hb_inc_bimap_t> ())
  {
    స్థిరరాశి VarRegionList& regionList = varStore.get_region_list ();
    ఒకవేళ (!regionList.get_var_regions (axes_old_సూచీ_ట్యాగ్_map, orig_region_list))
      తిరిగి అబద్ధం;

    గుర్తులేని అంకె_var_దత్తాంశం = varStore.get_sub_పట్టిక_లెక్క ();
    ఒకవేళ (inner_maps && inner_maps.నిడివి != అంకె_var_దత్తాంశం) తిరిగి అబద్ధం;
    ఒకవేళ (!vars.alloc (అంకె_var_దత్తాంశం) ||
        !var_దత్తాంశం_అంకె_అడ్డువరుసలు.alloc (అంకె_var_దత్తాంశం)) తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని i = 0; i < అంకె_var_దత్తాంశం; i++)
    {
      ఒకవేళ (inner_maps && !inner_maps.arrayZ[i].get_population ())
          కొనసాగు;
      tuple_variations_t var_దత్తాంశం_tuples;
      గుర్తులేని అంశం_లెక్క_2 = 0;
      ఒకవేళ (!var_దత్తాంశం_tuples.create_from_అంశం_var_దత్తాంశం (varStore.get_sub_పట్టిక (i),
                                                      orig_region_list,
                                                      axes_old_సూచీ_ట్యాగ్_map,
                                                      అంశం_లెక్క_2,
                                                      inner_maps ? &(inner_maps.arrayZ[i]) : శూన్యసూచిక))
        తిరిగి అబద్ధం;

      var_దత్తాంశం_అంకె_అడ్డువరుసలు.push (అంశం_లెక్క_2);
      vars.push (std::move (var_దత్తాంశం_tuples));
    }
    తిరిగి !vars.లోపల_error () && !var_దత్తాంశం_అంకె_అడ్డువరుసలు.లోపల_error () && vars.నిడివి == var_దత్తాంశం_అంకె_అడ్డువరుసలు.నిడివి;
  }

  తార్కిక instantiate_tuple_vars (స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>& normalized_axes_location,
                               స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, TripleDistances>& axes_triple_distances,
                               తార్కిక build_regions = నిజం)
  {
    optimize_scratch_t scratch;
    కొరకు (tuple_variations_t& tuple_vars : vars)
      ఒకవేళ (!tuple_vars.instantiate (normalized_axes_location, axes_triple_distances, scratch))
        తిరిగి అబద్ధం;

    తిరిగి !build_regions || build_region_list ();
  }

  /* Add a new VarData subtable. Returns outer index. */
  గుర్తులేని కూడు_vardata (గుర్తులేని అంశం_లెక్క_2)
  {
    vars.push (tuple_variations_t ());
    var_దత్తాంశం_అంకె_అడ్డువరుసలు.push (అంశం_లెక్క_2);
    తిరిగి vars.నిడివి - 1;
  }

  /* Get item count for a VarData subtable. */
  గుర్తులేని get_అంశం_లెక్క (గుర్తులేని outer) స్థిరరాశి
  { తిరిగి outer < var_దత్తాంశం_అంకె_అడ్డువరుసలు.నిడివి ? var_దత్తాంశం_అంకె_అడ్డువరుసలు[outer] : 0; }

  /* Duplicate the delta row at position inner within VarData subtable outer,
   * appending the copy as a new item. Used to give an axis a private copy of
   * a delta row that avar2's VarIdxMap shares between several axes, before
   * per-axis offset-compensation deltas are written into it. Returns the new
   * inner index, or (unsigned) -1 on failure. */
  గుర్తులేని duplicate_అడ్డువరుస (గుర్తులేని outer, గుర్తులేని inner)
  {
    ఒకవేళ (unlikely (outer >= vars.నిడివి ||
                  inner >= var_దత్తాంశం_అంకె_అడ్డువరుసలు[outer] ||
                  var_దత్తాంశం_అంకె_అడ్డువరుసలు[outer] >= 0xFFFFu))
      తిరిగి (గుర్తులేని) -1;
    కొరకు (tuple_delta_t& tuple : vars[outer].tuple_vars)
    {
      ఒకవేళ (unlikely (inner >= tuple.deltas_x.నిడివి ||
                    inner >= tuple.indices.నిడివి))
        తిరిగి (గుర్తులేని) -1;
      tuple.indices.push (tuple.indices.arrayZ[inner]);
      tuple.deltas_x.push (tuple.deltas_x.arrayZ[inner]);
      ఒకవేళ (inner < tuple.deltas_y.నిడివి)
        tuple.deltas_y.push (tuple.deltas_y.arrayZ[inner]);
      ఒకవేళ (unlikely (tuple.indices.లోపల_error () ||
                    tuple.deltas_x.లోపల_error () ||
                    tuple.deltas_y.లోపల_error ()))
        తిరిగి (గుర్తులేని) -1;
    }
    తిరిగి var_దత్తాంశం_అంకె_అడ్డువరుసలు[outer]++;
  }

  /* Add a tuple with a single non-zero delta at position inner.
   * axis_tuples defines the region (empty map = constant/bias tuple). */
  శూన్య కూడు_tuple (గుర్తులేని outer,
                  hb_hashmap_t<hb_ట్యాగ్_t, Triple>&& axis_tuples,
                  గుర్తులేని inner, పూర్ణాంకం delta, గుర్తులేని అంశం_లెక్క_2)
  {
    ఒకవేళ (outer >= vars.నిడివి) తిరిగి;
    tuple_delta_t tuple;
    tuple.axis_tuples = std::move (axis_tuples);
    ఒకవేళ (!tuple.indices.resize (అంశం_లెక్క_2) ||
        !tuple.deltas_x.resize (అంశం_లెక్క_2)) తిరిగి;
    కొరకు (గుర్తులేని i = 0; i < అంశం_లెక్క_2; i++)
    {
      tuple.indices.arrayZ[i] = నిజం;
      tuple.deltas_x.arrayZ[i] = 0.f;
    }
    tuple.deltas_x[inner] = (దశాంశం) delta;
    vars[outer].tuple_vars.push (std::move (tuple));
  }

  తార్కిక build_region_list ()
  {
    /* scan all tuples and collect all unique regions, prune unused regions */
    hb_hashmap_t<region_t, గుర్తులేని> all_regions;
    hb_hashmap_t<region_t, గుర్తులేని> వాడిన_regions;

    /* use a vector when inserting new regions, make result deterministic */
    hb_సదిశ_t<region_t> all_unique_regions;
    కొరకు (స్థిరరాశి tuple_variations_t& sub_పట్టిక_2 : vars)
    {
      కొరకు (స్థిరరాశి tuple_delta_t& tuple : sub_పట్టిక_2.tuple_vars)
      {
        region_t r = &(tuple.axis_tuples);
        ఒకవేళ (!వాడిన_regions.has (r))
        {
          తార్కిక all_zeros = నిజం;
          కొరకు (దశాంశం d : tuple.deltas_x)
          {
            పూర్ణాంకం delta = hb_clamp_గమ్యం<పూర్ణాంకం> (roundf ((ద్విగుణ) d));
            ఒకవేళ (delta != 0)
            {
              all_zeros = అబద్ధం;
              ఆపు;
            }
          }
          ఒకవేళ (!all_zeros)
          {
            ఒకవేళ (!వాడిన_regions.సమితి (r, 1))
              తిరిగి అబద్ధం;
          }
        }
        ఒకవేళ (all_regions.has (r))
          కొనసాగు;
        ఒకవేళ (!all_regions.సమితి (r, 1))
          తిరిగి అబద్ధం;
        all_unique_regions.push (r);
      }
    }

    /* regions are empty means no variation data, return true */
    ఒకవేళ (!all_regions || !all_unique_regions) తిరిగి నిజం;

    ఒకవేళ (!region_list.alloc (all_regions.get_population ()))
      తిరిగి అబద్ధం;

    గుర్తులేని idx = 0;
    /* append the original regions that pre-existed */
    కొరకు (స్థిరరాశి స్వయంచాలక& r : orig_region_list)
    {
      ఒకవేళ (!all_regions.has (&r) || !వాడిన_regions.has (&r))
        కొనసాగు;

      region_list.push (&r);
      ఒకవేళ (!region_map.సమితి (&r, idx))
        తిరిగి అబద్ధం;
      all_regions.del (&r);
      idx++;
    }

    /* append the new regions at the end */
    కొరకు (స్థిరరాశి స్వయంచాలక& r: all_unique_regions)
    {
      ఒకవేళ (!all_regions.has (r) || !వాడిన_regions.has (r))
        కొనసాగు;
      region_list.push (r);
      ఒకవేళ (!region_map.సమితి (r, idx))
        తిరిగి అబద్ధం;
      all_regions.del (r);
      idx++;
    }
    తిరిగి (!region_list.లోపల_error ()) && (!region_map.లోపల_error ());
  }

  /* main algorithm ported from fonttools VarStore_optimize() method, optimize
   * varstore by default */

  నిర్మాణం combined_gain_idx_tuple_t
  {
    uint64_t encoded;

    combined_gain_idx_tuple_t () = అప్రమేయ;
    combined_gain_idx_tuple_t (గుర్తులేని gain, గుర్తులేని i, గుర్తులేని j)
        : encoded ((uint64_t (0xFFFFFF - gain) << 40) | (uint64_t (i) << 20) | uint64_t (j))
    {
      ప్రతిజ్ఞా (gain < 0xFFFFFF);
      ప్రతిజ్ఞా (i < 0xFFFFFFF && j < 0xFFFFFFF);
    }

    తార్కిక క్రియాకారకం < (స్థిరరాశి combined_gain_idx_tuple_t& o)
    {
      తిరిగి encoded < o.encoded;
    }

    తార్కిక క్రియాకారకం <= (స్థిరరాశి combined_gain_idx_tuple_t& o)
    {
      తిరిగి encoded <= o.encoded;
    }

    గుర్తులేని idx_1 () స్థిరరాశి { తిరిగి (encoded >> 20) & 0xFFFFF; };
    గుర్తులేని idx_2 () స్థిరరాశి { తిరిగి encoded & 0xFFFFF; };
  };

  తార్కిక as_అంశం_varstore (తార్కిక optimize=నిజం, తార్కిక use_no_variation_idx=నిజం)
  {
    /* return true if no variation data */
    ఒకవేళ (!region_list) తిరిగి నిజం;
    గుర్తులేని అంకె_నిలువుల_సంఖ్య = region_list.నిడివి;
    /* pre-alloc a 2D vector for all sub_table's VarData rows */
    గుర్తులేని మొత్తం_అడ్డువరుసలు = 0;
    కొరకు (గుర్తులేని major = 0; major < var_దత్తాంశం_అంకె_అడ్డువరుసలు.నిడివి; major++)
      మొత్తం_అడ్డువరుసలు += var_దత్తాంశం_అంకె_అడ్డువరుసలు[major];

    ఒకవేళ (!delta_అడ్డువరుసలు.resize (మొత్తం_అడ్డువరుసలు)) తిరిగి అబద్ధం;
    /* init all rows to [0]*num_cols */
    కొరకు (గుర్తులేని i = 0; i < మొత్తం_అడ్డువరుసలు; i++)
      ఒకవేళ (!(delta_అడ్డువరుసలు[i].resize (అంకె_నిలువుల_సంఖ్య))) తిరిగి అబద్ధం;

    /* old VarIdxes -> full encoding_row mapping */
    hb_hashmap_t<గుర్తులేని, స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*> front_mapping;
    గుర్తులేని ప్రారంభం_అడ్డువరుస = 0;
    hb_సదిశ_t<delta_అడ్డువరుస_encoding_t> encoding_objs;

    /* delta_rows map, used for filtering out duplicate rows */
    hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం> *> major_అడ్డువరుసలు;
    hb_hashmap_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*, గుర్తులేని> delta_అడ్డువరుసలు_map;
    కొరకు (గుర్తులేని major = 0; major < vars.నిడివి; major++)
    {
      /* deltas are stored in tuples(column based), convert them back into items
       * (row based) delta */
      స్థిరరాశి tuple_variations_t& tuples = vars[major];
      గుర్తులేని అంకె_అడ్డువరుసలు = var_దత్తాంశం_అంకె_అడ్డువరుసలు[major];

      ఒకవేళ (!అంకె_అడ్డువరుసలు) కొనసాగు;

      కొరకు (స్థిరరాశి tuple_delta_t& tuple: tuples.tuple_vars)
      {
        ఒకవేళ (tuple.deltas_x.నిడివి != అంకె_అడ్డువరుసలు)
          తిరిగి అబద్ధం;

        /* skip unused regions */
        గుర్తులేని *నిలువు_idx;
        ఒకవేళ (!region_map.has (&(tuple.axis_tuples), &నిలువు_idx))
          కొనసాగు;

        కొరకు (గుర్తులేని i = 0; i < అంకె_అడ్డువరుసలు; i++)
        {
          పూర్ణాంకం rounded_delta = hb_clamp_గమ్యం<పూర్ణాంకం> (roundf ((ద్విగుణ) tuple.deltas_x[i]));
          పూర్ణాంకం &delta = delta_అడ్డువరుసలు[ప్రారంభం_అడ్డువరుస + i][*నిలువు_idx];
          int64_t sum = (int64_t) delta + rounded_delta;
          ఒకవేళ (unlikely (sum < INT_MIN || sum > గరిష్ఠ_పూర్ణాంకం)) తిరిగి అబద్ధం;
          delta = (పూర్ణాంకం) sum;
          has_long |= delta < -32768 || delta > 32767;
        }
      }

      major_అడ్డువరుసలు.reset ();
      కొరకు (గుర్తులేని minor = 0; minor < అంకె_అడ్డువరుసలు; minor++)
      {
	స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& అడ్డువరుస = delta_అడ్డువరుసలు[ప్రారంభం_అడ్డువరుస + minor];
	ఒకవేళ (use_no_variation_idx)
	{
	  తార్కిక all_zeros = నిజం;
	  కొరకు (పూర్ణాంకం delta : అడ్డువరుస)
	  {
	    ఒకవేళ (delta != 0)
	    {
	      all_zeros = అబద్ధం;
	      ఆపు;
	    }
	  }
	  ఒకవేళ (all_zeros)
	    కొనసాగు;
	}

	ఒకవేళ (!front_mapping.సమితి ((major<<16) + minor, &అడ్డువరుస))
	  తిరిగి అబద్ధం;

	/* Duplicate rows may only be dropped when optimizing: without a
	 * varidx_map the caller relies on original VariationIndex values
	 * staying valid (e.g. HVAR/VVAR with an implicit advance mapping). */
	ఒకవేళ (optimize)
	{
	  ఒకవేళ (delta_అడ్డువరుసలు_map.has (&అడ్డువరుస))
	    కొనసాగు;

	  delta_అడ్డువరుసలు_map.సమితి (&అడ్డువరుస, 1);
	}

	major_అడ్డువరుసలు.push (&అడ్డువరుస);
      }

      ఒకవేళ (unlikely (major_అడ్డువరుసలు.లోపల_error () || delta_అడ్డువరుసలు_map.లోపల_error ()))
        తిరిగి అబద్ధం;
      ఒకవేళ (major_అడ్డువరుసలు)
      {
	స్వయంచాలక *encoding = encoding_objs.push (std::move (major_అడ్డువరుసలు), అంకె_నిలువుల_సంఖ్య);
	ఒకవేళ (unlikely (encoding_objs.లోపల_error () || encoding->లోపల_error ()))
	  తిరిగి అబద్ధం;
      }

      ప్రారంభం_అడ్డువరుస += అంకె_అడ్డువరుసలు;
    }

    /* return directly if no optimization, maintain original VariationIndex so
     * varidx_map would be empty */
    ఒకవేళ (!optimize)
    {
      encodings = std::move (encoding_objs);
      తిరిగి !encodings.లోపల_error ();
    }

    /* NOTE: Fonttools instancer always optimizes VarStore from scratch. This
     * is too costly for large fonts. So, instead, we retain the encodings of
     * the original VarStore, and just try to combine them if possible. This
     * is a compromise between optimization and performance and practically
     * works very well. */

    // This produces slightly smaller results in some cases.
    encoding_objs.qsort ();

    /* main algorithm: repeatedly pick 2 best encodings to combine, and combine them */
    ఉపయోగించి అంశం_t = hb_ప్రాధాన్యత_వరుస_t<combined_gain_idx_tuple_t>::అంశం_t;
    hb_సదిశ_t<అంశం_t> వరుస_వస్తువులు;
    గుర్తులేని అంకె_todos = encoding_objs.నిడివి;
    కొరకు (గుర్తులేని i = 0; i < అంకె_todos; i++)
    {
      కొరకు (గుర్తులేని j = i + 1; j < అంకె_todos; j++)
      {
        పూర్ణాంకం combining_gain = encoding_objs.arrayZ[i].gain_from_merging (encoding_objs.arrayZ[j]);
        ఒకవేళ (combining_gain > 0)
          వరుస_వస్తువులు.push (అంశం_t (combined_gain_idx_tuple_t (combining_gain, i, j), 0));
      }
    }

    hb_ప్రాధాన్యత_వరుస_t<combined_gain_idx_tuple_t> వరుస (std::move (వరుస_వస్తువులు));

    hb_బిట్టు_సమితి_t removed_todo_idxes;
    అయితే (వరుస)
    {
      స్వయంచాలక t = వరుస.తీయి_minimum ().మొదటి;
      గుర్తులేని i = t.idx_1 ();
      గుర్తులేని j = t.idx_2 ();

      ఒకవేళ (removed_todo_idxes.has (i) || removed_todo_idxes.has (j))
        కొనసాగు;

      delta_అడ్డువరుస_encoding_t& encoding = encoding_objs.arrayZ[i];
      delta_అడ్డువరుస_encoding_t& other_encoding = encoding_objs.arrayZ[j];

      removed_todo_idxes.కూడు (i);
      removed_todo_idxes.కూడు (j);
      ఒకవేళ (unlikely (removed_todo_idxes.లోపల_error ())) తిరిగి అబద్ధం;

      encoding.విలీనం (other_encoding);
      ఒకవేళ (unlikely (encoding.లోపల_error ())) తిరిగి అబద్ధం;

      కొరకు (గుర్తులేని idx = 0; idx < encoding_objs.నిడివి; idx++)
      {
        ఒకవేళ (removed_todo_idxes.has (idx)) కొనసాగు;

        స్థిరరాశి delta_అడ్డువరుస_encoding_t& obj = encoding_objs.arrayZ[idx];
	// In the unlikely event that the same encoding exists already, combine it.
        ఒకవేళ (obj.వెడల్పు == encoding.వెడల్పు && obj.chars == encoding.chars)
        {
	  // This is straight port from fonttools algorithm. I added this branch there
	  // because I thought it can happen. But looks like we never get in here in
	  // practice. I'm not confident enough to remove it though; in theory it can
	  // happen. I think it's just that our tests are not extensive enough to hit
	  // this path.

          కొరకు (స్థిరరాశి స్వయంచాలక& అడ్డువరుస : obj.వస్తువులు)
            encoding.కూడు_అడ్డువరుస (అడ్డువరుస);
          ఒకవేళ (unlikely (encoding.లోపల_error ())) తిరిగి అబద్ధం;

          removed_todo_idxes.కూడు (idx);
          ఒకవేళ (unlikely (removed_todo_idxes.లోపల_error ())) తిరిగి అబద్ధం;
          కొనసాగు;
        }

        పూర్ణాంకం combined_gain = encoding.gain_from_merging (obj);
        ఒకవేళ (combined_gain > 0)
          వరుస.insert (combined_gain_idx_tuple_t (combined_gain, idx, encoding_objs.నిడివి), 0);
      }

      స్వయంచాలక moved_encoding = std::move (encoding);
      encoding_objs.push (std::move (moved_encoding));
      ఒకవేళ (unlikely (encoding_objs.లోపల_error ())) తిరిగి అబద్ధం;
    }

    పూర్ణాంకం అంకె_final_encodings = (పూర్ణాంకం) encoding_objs.నిడివి - (పూర్ణాంకం) removed_todo_idxes.get_population ();
    ఒకవేళ (అంకె_final_encodings <= 0) తిరిగి అబద్ధం;

    ఒకవేళ (!encodings.alloc (అంకె_final_encodings)) తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని i = 0; i < encoding_objs.నిడివి; i++)
    {
      ఒకవేళ (removed_todo_idxes.has (i)) కొనసాగు;
      encodings.push (std::move (encoding_objs.arrayZ[i]));
    }

    తిరిగి compile_varidx_map (front_mapping);
  }

  అంతరంగ:
  /* compile varidx_map for one VarData subtable (index specified by major) */
  తార్కిక compile_varidx_map (స్థిరరాశి hb_hashmap_t<గుర్తులేని, స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*>& front_mapping)
  {
    /* full encoding_row -> new VarIdxes mapping */
    hb_hashmap_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*, గుర్తులేని> వెనుక_mapping;
    hb_సదిశ_t<delta_అడ్డువరుస_encoding_t> విభజించు_encodings;

    కొరకు (గుర్తులేని i = 0; i < encodings.నిడివి; i++)
    {
      delta_అడ్డువరుస_encoding_t& encoding = encodings[i];
      /* just sanity check, this shouldn't happen */
      ఒకవేళ (encoding.is_empty () || encoding.లోపల_error ())
        తిరిగి అబద్ధం;

      గుర్తులేని అంకె_అడ్డువరుసలు = encoding.వస్తువులు.నిడివి;
      గుర్తులేని అంకె_నిలువుల_సంఖ్య = encoding.chars.నిడివి;

      /* sort rows, make result deterministic */
      encoding.వస్తువులు.qsort (cmp_అడ్డువరుస);

      కొరకు (గుర్తులేని ప్రారంభం = 0; ప్రారంభం < అంకె_అడ్డువరుసలు; ప్రారంభం += 0xFFFFu)
      {
        గుర్తులేని chunk_len = hb_min (అంకె_అడ్డువరుసలు - ప్రారంభం, 0xFFFFu);
        hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*> అడ్డువరుసలు;

        ఒకవేళ (!అడ్డువరుసలు.alloc (chunk_len))
          తిరిగి అబద్ధం;

        గుర్తులేని major = విభజించు_encodings.నిడివి;
        కొరకు (గుర్తులేని minor = 0; minor < chunk_len; minor++)
        {
          స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం> *అడ్డువరుస = encoding.వస్తువులు.arrayZ[ప్రారంభం + minor];
          అడ్డువరుసలు.push (అడ్డువరుస);
          ఒకవేళ (!వెనుక_mapping.సమితి (అడ్డువరుస, (major << 16) + minor))
            తిరిగి అబద్ధం;
        }

        స్వయంచాలక *విభజించు = విభజించు_encodings.push (std::move (అడ్డువరుసలు), అంకె_నిలువుల_సంఖ్య);
        ఒకవేళ (unlikely (విభజించు_encodings.లోపల_error () || విభజించు->లోపల_error ()))
          తిరిగి అబద్ధం;
      }
    }

    encodings = std::move (విభజించు_encodings);
    ఒకవేళ (encodings.లోపల_error () || వెనుక_mapping.లోపల_error ())
      తిరిగి అబద్ధం;

    కొరకు (స్వయంచాలక _ : front_mapping.iter ())
    {
      గుర్తులేని old_varidx = _.మొదటి;
      గుర్తులేని *new_varidx;
      ఒకవేళ (వెనుక_mapping.has (_.second, &new_varidx))
        varidx_map.సమితి (old_varidx, *new_varidx);
      లేకపోతే
        varidx_map.సమితి (old_varidx, HB_OT_LAYOUT_NO_VARIATIONS_సూచీ);
    }
    తిరిగి !varidx_map.లోపల_error ();
  }

  స్థిర పూర్ణాంకం cmp_అడ్డువరుస (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    /* compare pointers of vectors(const hb_vector_t<int>*) that represent a row */
    స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>** a = (స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>**) pa;
    స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>** b = (స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>**) pb;

    కొరకు (గుర్తులేని i = 0; i < (*b)->నిడివి; i++)
    {
      పూర్ణాంకం va = (*a)->arrayZ[i];
      పూర్ణాంకం vb = (*b)->arrayZ[i];
      ఒకవేళ (va != vb)
        తిరిగి va < vb ? -1 : 1;
    }
    తిరిగి 0;
  }
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_VAR_COMMON_HH */

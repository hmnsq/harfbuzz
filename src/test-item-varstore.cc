/*
 * Copyright © 2020  Google, Inc.
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
#అన్తర్భావ "hb-ot-var-common.hh"
#అన్తర్భావ "hb-ot-var-hvar-table.hh"
// HVAR table data from SourceSerif4Variable-Roman_subset.otf
స్థిరరాశి అక్షరం hvar_దత్తాంశం[] = "\x0\x1\x0\x0\x0\x0\x0\x14\x0\x0\x0\xc4\x0\x0\x0\x0\x0\x0\x0\x0\x0\x1\x0\x0\x0\x10\x0\x2\x0\x0\x0\x74\x0\x0\x0\x7a\x0\x2\x0\x8\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\x0\x1\x0\x0\x0\x0\x0\x4\x0\x0\x0\x8\x0\x0\x0\x1\x0\x2\x0\x3\x0\x4\x0\x5\x0\x6\x0\x7\xf9\xf\x2f\xbf\xfb\xfb\x35\xf9\x4\x4\xf3\xb4\xf2\xfb\x2e\xf3\x4\x4\xe\xad\xfa\x1\x1a\x1\x15\x22\x59\xd6\xe3\xf6\x6\xf5\x0\x1\x0\x5\x0\x4\x7\x5\x6";

స్థిర శూన్య
test_scalar_cache_precision ()
{
  స్థిరరాశి దశాంశం scalars[] = {
    0.f,
    1.f,
    1.f / 3.f,
    /* Three variation axes can produce a scalar below the old cache's
     * fixed-point precision. */
    1.f / (16384.f * 16384.f * 16384.f),
  };
  స్థిరరాశి గుర్తులేని counts[] = {4, 129};
  OT::hb_scalar_cache_t scratch_cache;
  కొరకు (గుర్తులేని లెక్క : counts)
  {
    స్వయంచాలక *cache = OT::hb_scalar_cache_t::create (లెక్క, &scratch_cache);
    hb_always_assert (cache->నిడివి == లెక్క);
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      దశాంశం value;
      దశాంశం scalar = scalars[i % ARRAY_నిడివి (scalars)];
      hb_always_assert (!cache->get (i, &value));
      cache->సమితి (i, scalar);
      hb_always_assert (cache->get (i, &value));
      hb_always_assert (value == scalar);
    }
    cache->clear ();
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      దశాంశం value;
      hb_always_assert (!cache->get (i, &value));
    }
    OT::hb_scalar_cache_t::destroy (cache, &scratch_cache);
  }
}

స్థిర శూన్య
test_అంశం_variations ()
{
  స్థిరరాశి OT::HVAR* hvar_పట్టిక = reinterpret_cast<స్థిరరాశి OT::HVAR*> (hvar_దత్తాంశం);

  hb_ట్యాగ్_t axis_ట్యాగ్_2 = HB_ట్యాగ్ ('w', 'g', 'h', 't');
  hb_map_t axis_idx_ట్యాగ్_map;
  axis_idx_ట్యాగ్_map.సమితి (0, axis_ట్యాగ్_2);

  axis_ట్యాగ్_2 = HB_ట్యాగ్ ('o', 'p', 's', 'z');
  axis_idx_ట్యాగ్_map.సమితి (1, axis_ట్యాగ్_2);

  OT::అంశం_variations_t అంశం_vars;
  స్థిరరాశి OT::అంశం_Variation_Store& src_var_store = hvar_పట్టిక+(hvar_పట్టిక->varStore);
  తార్కిక ఫలితం = అంశం_vars.create_from_అంశం_varstore (src_var_store, axis_idx_ట్యాగ్_map);
      
  hb_always_assert (ఫలితం);

  /* partial instancing wght=300:800 */
  hb_hashmap_t<hb_ట్యాగ్_t, Triple> normalized_axes_location;
  normalized_axes_location.సమితి (axis_ట్యాగ్_2, Triple (-0.512817, 0.0, 0.7000120));

  hb_hashmap_t<hb_ట్యాగ్_t, TripleDistances> axes_triple_distances;
  axes_triple_distances.సమితి (axis_ట్యాగ్_2, TripleDistances (200.0, 500.0));

  ఫలితం = అంశం_vars.instantiate_tuple_vars (normalized_axes_location, axes_triple_distances);
  hb_always_assert (ఫలితం);
  ఫలితం = అంశం_vars.as_అంశం_varstore (అబద్ధం);
  hb_always_assert (ఫలితం);
  hb_always_assert (అంశం_vars.get_region_list().నిడివి == 8);
}

స్థిర శూన్య
test_implicit_advance_mapping ()
{
  అక్షరం దత్తాంశం[పరిమాణం (hvar_దత్తాంశం)];
  hb_memcpy (దత్తాంశం, hvar_దత్తాంశం, పరిమాణం (దత్తాంశం));
  స్వయంచాలక *hvar = reinterpret_cast<OT::HVAR *> (దత్తాంశం);
  స్థిరరాశి స్వయంచాలక &store = hvar + hvar->varStore;
  స్థిరరాశి పూర్ణాంకం locations[] = {-16384, 0, 16384};
  తార్కిక exercised_outer_one = అబద్ధం;

  కొరకు (పూర్ణాంకం బరువు : locations)
    కొరకు (పూర్ణాంకం కొలత : locations)
    {
      పూర్ణాంకం coords[] = {బరువు, కొలత};
      గుర్తులేని mapped = (hvar + hvar->advMap).map (0x10000u);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) ==
                        store.get_delta (mapped, coords, 2));
    }

  hvar->advMap = 0;
  కొరకు (పూర్ణాంకం బరువు : locations)
    కొరకు (పూర్ణాంకం కొలత : locations)
    {
      పూర్ణాంకం coords[] = {బరువు, కొలత};
      exercised_outer_one |= store.get_delta (0x10000u, coords, 2) != 0;
      hb_always_assert (hvar->get_advance_delta_unscaled (0, coords, 2) ==
                        store.get_delta (0, coords, 2));
      hb_always_assert (hvar->get_advance_delta_unscaled (0xFFFFu, coords, 2) == 0);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) == 0);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10001u, coords, 2) == 0);
    }
  hb_always_assert (exercised_outer_one);
}

స్థిర శూన్య
test_అంశం_variations_అధికప్రవాహం ()
{
  hb_సదిశ_t<hb_సదిశ_t<పూర్ణాంకం>> అడ్డువరుస_storage;
  hb_hashmap_t<గుర్తులేని, స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*> front_mapping;
  OT::అంశం_variations_t అంశం_vars;
  గుర్తులేని old_varidx = 0;
  స్థిరరాశి గుర్తులేని మొత్తం_లెక్క = 0xFFFFu * 2u;

  స్థిరరాశి గుర్తులేని counts[] = {
    128u,                 /* 0..127: 1-byte deltas */
    32768u - 128u,        /* 128..32767: 2-byte deltas */
    0xFFFFu * 2u - 32768u /* 32768..131069: 4-byte deltas */
  };
  స్థిరరాశి పూర్ణాంకం starts[] = {
    0,
    128,
    32768
  };

  hb_always_assert (అడ్డువరుస_storage.alloc (మొత్తం_లెక్క));

  కొరకు (గుర్తులేని పాత్ర = 0; పాత్ర < 3; పాత్ర++)
  {
    hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*> అడ్డువరుసలు;
    hb_always_assert (అడ్డువరుసలు.alloc (counts[పాత్ర]));

    కొరకు (గుర్తులేని i = 0; i < counts[పాత్ర]; i++)
    {
      hb_సదిశ_t<పూర్ణాంకం> అడ్డువరుస;
      hb_always_assert (అడ్డువరుస.resize (1));
      అడ్డువరుస[0] = starts[పాత్ర] + i;
      అడ్డువరుస_storage.push (std::move (అడ్డువరుస));
    }

    hb_always_assert (!అడ్డువరుస_storage.లోపల_error ());

    గుర్తులేని పునాది = అడ్డువరుస_storage.నిడివి - counts[పాత్ర];
    కొరకు (గుర్తులేని i = 0; i < counts[పాత్ర]; i++)
    {
      స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం> *అడ్డువరుస = &అడ్డువరుస_storage.arrayZ[పునాది + i];
      అడ్డువరుసలు.push (అడ్డువరుస);
      front_mapping.సమితి (old_varidx++, అడ్డువరుస);
    }

    hb_always_assert (!అడ్డువరుసలు.లోపల_error ());
    hb_always_assert (అంశం_vars.కూడు_vardata_encoding_for_testing (std::move (అడ్డువరుసలు), 1));
  }

  hb_always_assert (!front_mapping.లోపల_error ());
  hb_always_assert (అంశం_vars.compile_varidx_map_for_testing (front_mapping));

  స్థిరరాశి స్వయంచాలక& encodings = అంశం_vars.get_vardata_encodings ();
  hb_always_assert (encodings.నిడివి == 4);
  hb_always_assert (encodings[0].వస్తువులు.నిడివి == 128u);
  hb_always_assert (encodings[1].వస్తువులు.నిడివి == 32768u - 128u);
  hb_always_assert (encodings[2].వస్తువులు.నిడివి == 0xFFFFu);
  hb_always_assert (encodings[3].వస్తువులు.నిడివి == 32767u);

  స్థిరరాశి hb_map_t& varidx_map = అంశం_vars.get_varidx_map ();
  hb_always_assert (varidx_map.get (0) == 0);
  hb_always_assert (varidx_map.get (127) == 127);
  hb_always_assert (varidx_map.get (128) == 0x00010000u);
  hb_always_assert (varidx_map.get (32767) == 0x00017F7Fu);
  hb_always_assert (varidx_map.get (32768) == 0x00020000u);
  hb_always_assert (varidx_map.get (98302) == 0x0002FFFEu);
  hb_always_assert (varidx_map.get (98303) == 0x00030000u);
  hb_always_assert (varidx_map.get (131069) == 0x00037FFEu);
}

స్థిర శూన్య
test_signed_delta_widths ()
{
  స్థిరరాశి పూర్ణాంకం deltas[] = {32767, 32768, 65535, 65536,
                       -32768, -32769, -65536, -65537};
  hb_ట్యాగ్_t axis = HB_ట్యాగ్ ('T', 'E', 'S', 'T');
  hb_సదిశ_t<hb_ట్యాగ్_t> axes;
  axes.push (axis);
  hb_map_t axis_map;
  axis_map.సమితి (0, axis);
  hb_hashmap_t<hb_ట్యాగ్_t, Triple> region;
  region.సమితి (axis, Triple (0, 1, 1));
  hb_సదిశ_t<స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *> regions;
  regions.push (&region);

  కొరకు (పూర్ణాంకం delta : deltas)
  {
    hb_సదిశ_t<పూర్ణాంకం> అడ్డువరుస;
    అడ్డువరుస.push (delta);
    hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం> *> అడ్డువరుసలు;
    అడ్డువరుసలు.push (&అడ్డువరుస);
    hb_సదిశ_t<OT::delta_అడ్డువరుస_encoding_t> encodings;
    encodings.push (OT::delta_అడ్డువరుస_encoding_t (std::move (అడ్డువరుసలు), 1));

    అక్షరం buf[128];
    hb_serialize_context_t s (buf, పరిమాణం (buf));
    స్వయంచాలక *బహిర్గతం = s.ప్రారంభం_serialize<OT::అంశం_Variation_Store> ();
    hb_always_assert (బహిర్గతం->serialize (&s, నిజం, axes, regions, encodings));
    s.చివర_serialize ();
    hb_బైట్లు_t బైట్లు = s.copy_బైట్లు ();
    స్థిరరాశి స్వయంచాలక &store = *reinterpret_cast<స్థిరరాశి OT::అంశం_Variation_Store *> (బైట్లు.arrayZ);
    స్థిరరాశి స్వయంచాలక &దత్తాంశం = store.get_sub_పట్టిక (0);
    hb_always_assert (దత్తాంశం.get_అంశం_delta (0, 0) == delta);

    hb_inc_bimap_t inner_map, region_map;
    inner_map.కూడు (0);
    region_map.కూడు (0);
    అక్షరం subset_buf[32];
    hb_serialize_context_t subset_s (subset_buf, పరిమాణం (subset_buf));
    స్వయంచాలక *subset = subset_s.ప్రారంభం_serialize<OT::Var_దత్తాంశం> ();
    hb_always_assert (subset->serialize (&subset_s, &దత్తాంశం, inner_map, region_map));
    subset_s.చివర_serialize ();
    hb_బైట్లు_t subset_బైట్లు = subset_s.copy_బైట్లు ();
    స్థిరరాశి స్వయంచాలక &subset_దత్తాంశం = *reinterpret_cast<స్థిరరాశి OT::Var_దత్తాంశం *> (subset_బైట్లు.arrayZ);
    hb_always_assert (subset_దత్తాంశం.get_అంశం_delta (0, 0) == delta);

    OT::అంశం_variations_t అంశం_vars;
    hb_always_assert (అంశం_vars.create_from_అంశం_varstore (store, axis_map));
    hb_hashmap_t<hb_ట్యాగ్_t, Triple> locations;
    hb_hashmap_t<hb_ట్యాగ్_t, TripleDistances> distances;
    hb_always_assert (అంశం_vars.instantiate_tuple_vars (locations, distances));
    hb_always_assert (అంశం_vars.as_అంశం_varstore ());
    hb_always_assert (అంశం_vars.has_long_మాట () ==
                      (delta < -32768 || delta > 32767));
    subset_బైట్లు.fini ();
    బైట్లు.fini ();
  }
}

స్థిర శూన్య
test_accumulated_delta_widths ()
{
  స్థిరరాశి పూర్ణాంకం deltas[] = {20000, -20000, 1073741824};
  కొరకు (పూర్ణాంకం delta : deltas)
  {
    OT::అంశం_variations_t అంశం_vars;
    గుర్తులేని outer = అంశం_vars.కూడు_vardata (1);
    అంశం_vars.కూడు_tuple (outer, hb_hashmap_t<hb_ట్యాగ్_t, Triple> (), 0, delta, 1);
    అంశం_vars.కూడు_tuple (outer, hb_hashmap_t<hb_ట్యాగ్_t, Triple> (), 0, delta, 1);
    hb_always_assert (అంశం_vars.build_region_list ());
    ఒకవేళ (delta == 1073741824)
      hb_always_assert (!అంశం_vars.as_అంశం_varstore ());
    లేకపోతే
    {
      hb_always_assert (అంశం_vars.as_అంశం_varstore ());
      hb_always_assert (అంశం_vars.has_long_మాట ());
      hb_always_assert ((*అంశం_vars.get_vardata_encodings ()[0].వస్తువులు[0])[0] == 2 * delta);
    }
  }
}

స్థిర శూన్య
test_condition_delta_precision ()
{
  hb_సదిశ_t<hb_ట్యాగ్_t> axes;
  axes.push (HB_ట్యాగ్ ('T','E','S','T'));
  hb_hashmap_t<hb_ట్యాగ్_t, Triple> neutral, region;
  region.సమితి (axes[0], Triple (0, 1, 1));
  hb_సదిశ_t<స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *> regions;
  regions.push (&neutral);
  regions.push (&region);
  hb_సదిశ_t<పూర్ణాంకం> అడ్డువరుస;
  అడ్డువరుస.push (16777217);
  అడ్డువరుస.push (-16777216);
  hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం> *> అడ్డువరుసలు;
  అడ్డువరుసలు.push (&అడ్డువరుస);
  hb_సదిశ_t<OT::delta_అడ్డువరుస_encoding_t> encodings;
  encodings.push (OT::delta_అడ్డువరుస_encoding_t (std::move (అడ్డువరుసలు), 2));
  అక్షరం buf[128];
  hb_serialize_context_t s (buf, పరిమాణం (buf));
  స్వయంచాలక *బహిర్గతం = s.ప్రారంభం_serialize<OT::అంశం_Variation_Store> ();
  hb_always_assert (బహిర్గతం->serialize (&s, నిజం, axes, regions, encodings));
  s.చివర_serialize ();
  hb_బైట్లు_t బైట్లు = s.copy_బైట్లు ();
  స్థిరరాశి స్వయంచాలక &store = *reinterpret_cast<స్థిరరాశి OT::అంశం_Variation_Store *> (బైట్లు.arrayZ);
  పూర్ణాంకం coords[] = {16384};
  OT::అంశం_Var_Store_Instancer instancer (&store, శూన్యసూచిక, hb_array (coords));
  // Positioning keeps float arithmetic, but conditions must retain the unit
  // left after cancellation of two deltas exceeding float's integer range.
  hb_always_assert (instancer[0] == 0.f);
  hb_always_assert (instancer.get_condition_delta (0) == 1.);
  స్థిరరాశి అక్షరం condition_దత్తాంశం[] = "\x00\x02\x00\x00\x00\x00\x00\x00";
  స్థిరరాశి స్వయంచాలక &condition = *reinterpret_cast<స్థిరరాశి OT::Condition *> (condition_దత్తాంశం);
  hb_always_assert (condition.evaluate (coords, 1, &instancer));
  బైట్లు.fini ();
}

స్థిర శూన్య
test_failed_encoding ()
{
  hb_సదిశ_t<పూర్ణాంకం> అడ్డువరుస;
  అడ్డువరుస.push (1);
  hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం> *> అడ్డువరుసలు;
  అడ్డువరుసలు.push (&అడ్డువరుస);
  OT::అంశం_variations_t అంశం_vars;
  hb_always_assert (అంశం_vars.కూడు_vardata_encoding_for_testing (std::move (అడ్డువరుసలు), 1));
  స్వయంచాలక &encoding = const_cast<OT::delta_అడ్డువరుస_encoding_t &> (అంశం_vars.get_vardata_encodings ()[0]);
  /* Simulate a failed allocation of the encoding's column metadata. */
  encoding.chars.clear ();
  encoding.chars.సమితి_error ();
  hb_always_assert (encoding.లోపల_error ());
  hb_hashmap_t<గుర్తులేని, స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం> *> front_mapping;
  front_mapping.సమితి (0, &అడ్డువరుస);
  hb_always_assert (!అంశం_vars.compile_varidx_map_for_testing (front_mapping));
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_scalar_cache_precision ();
  test_అంశం_variations ();
  test_implicit_advance_mapping ();
  test_అంశం_variations_అధికప్రవాహం ();
  test_signed_delta_widths ();
  test_failed_encoding ();
  test_accumulated_delta_widths ();
  test_condition_delta_precision ();
}

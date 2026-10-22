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
#ضمن "hb-ot-var-common.hh"
#ضمن "hb-ot-var-hvar-table.hh"
// HVAR table data from SourceSerif4Variable-Roman_subset.otf
ثابت حرف hvar_معلومات[] = "\x0\x1\x0\x0\x0\x0\x0\x14\x0\x0\x0\xc4\x0\x0\x0\x0\x0\x0\x0\x0\x0\x1\x0\x0\x0\x10\x0\x2\x0\x0\x0\x74\x0\x0\x0\x7a\x0\x2\x0\x8\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\x0\x1\x0\x0\x0\x0\x0\x4\x0\x0\x0\x8\x0\x0\x0\x1\x0\x2\x0\x3\x0\x4\x0\x5\x0\x6\x0\x7\xf9\xf\x2f\xbf\xfb\xfb\x35\xf9\x4\x4\xf3\xb4\xf2\xfb\x2e\xf3\x4\x4\xe\xad\xfa\x1\x1a\x1\x15\x22\x59\xd6\xe3\xf6\x6\xf5\x0\x1\x0\x5\x0\x4\x7\x5\x6";

ساكن فراغ
test_scalar_cache_precision ()
{
  ثابت عائم scalars[] = {
    0.f,
    1.f,
    1.f / 3.f,
    /* Three variation axes can produce a scalar below the old cache's
     * fixed-point precision. */
    1.f / (16384.f * 16384.f * 16384.f),
  };
  ثابت بنانشان counts[] = {4, 129};
  OT::hb_scalar_cache_t scratch_cache;
  لكل (بنانشان ڳڻپ : counts)
  {
    خودڪار *cache = OT::hb_scalar_cache_t::create (ڳڻپ, &scratch_cache);
    hb_always_assert (cache->ڊيگهائي == ڳڻپ);
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      عائم value;
      عائم scalar = scalars[i % ARRAY_ڊيگهائي (scalars)];
      hb_always_assert (!cache->get (i, &value));
      cache->سيٽ (i, scalar);
      hb_always_assert (cache->get (i, &value));
      hb_always_assert (value == scalar);
    }
    cache->clear ();
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      عائم value;
      hb_always_assert (!cache->get (i, &value));
    }
    OT::hb_scalar_cache_t::destroy (cache, &scratch_cache);
  }
}

ساكن فراغ
test_شي_variations ()
{
  ثابت OT::HVAR* hvar_جدول = reinterpret_cast<ثابت OT::HVAR*> (hvar_معلومات);

  hb_ٽيگ_t axis_ٽيگ_2 = HB_ٽيگ ('w', 'g', 'h', 't');
  hb_map_t axis_idx_ٽيگ_map;
  axis_idx_ٽيگ_map.سيٽ (0, axis_ٽيگ_2);

  axis_ٽيگ_2 = HB_ٽيگ ('o', 'p', 's', 'z');
  axis_idx_ٽيگ_map.سيٽ (1, axis_ٽيگ_2);

  OT::شي_variations_t شي_vars;
  ثابت OT::شي_Variation_Store& src_var_store = hvar_جدول+(hvar_جدول->varStore);
  منطقي نتيجو = شي_vars.create_from_شي_varstore (src_var_store, axis_idx_ٽيگ_map);
      
  hb_always_assert (نتيجو);

  /* partial instancing wght=300:800 */
  hb_hashmap_t<hb_ٽيگ_t, Triple> normalized_axes_location;
  normalized_axes_location.سيٽ (axis_ٽيگ_2, Triple (-0.512817, 0.0, 0.7000120));

  hb_hashmap_t<hb_ٽيگ_t, TripleDistances> axes_triple_distances;
  axes_triple_distances.سيٽ (axis_ٽيگ_2, TripleDistances (200.0, 500.0));

  نتيجو = شي_vars.instantiate_tuple_vars (normalized_axes_location, axes_triple_distances);
  hb_always_assert (نتيجو);
  نتيجو = شي_vars.as_شي_varstore (خطأ);
  hb_always_assert (نتيجو);
  hb_always_assert (شي_vars.get_region_list().ڊيگهائي == 8);
}

ساكن فراغ
test_implicit_advance_mapping ()
{
  حرف معلومات[حجم (hvar_معلومات)];
  hb_memcpy (معلومات, hvar_معلومات, حجم (معلومات));
  خودڪار *hvar = reinterpret_cast<OT::HVAR *> (معلومات);
  ثابت خودڪار &store = hvar + hvar->varStore;
  ثابت صحيح locations[] = {-16384, 0, 16384};
  منطقي exercised_outer_one = خطأ;

  لكل (صحيح وزن : locations)
    لكل (صحيح سائيز : locations)
    {
      صحيح coords[] = {وزن, سائيز};
      بنانشان mapped = (hvar + hvar->advMap).map (0x10000u);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) ==
                        store.get_delta (mapped, coords, 2));
    }

  hvar->advMap = 0;
  لكل (صحيح وزن : locations)
    لكل (صحيح سائيز : locations)
    {
      صحيح coords[] = {وزن, سائيز};
      exercised_outer_one |= store.get_delta (0x10000u, coords, 2) != 0;
      hb_always_assert (hvar->get_advance_delta_unscaled (0, coords, 2) ==
                        store.get_delta (0, coords, 2));
      hb_always_assert (hvar->get_advance_delta_unscaled (0xFFFFu, coords, 2) == 0);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) == 0);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10001u, coords, 2) == 0);
    }
  hb_always_assert (exercised_outer_one);
}

ساكن فراغ
test_شي_variations_حد_کان_مٿي ()
{
  hb_ويڪٽر_t<hb_ويڪٽر_t<صحيح>> صف_storage;
  hb_hashmap_t<بنانشان, ثابت hb_ويڪٽر_t<صحيح>*> front_mapping;
  OT::شي_variations_t شي_vars;
  بنانشان old_varidx = 0;
  ثابت بنانشان ڪل_ڳڻپ = 0xFFFFu * 2u;

  ثابت بنانشان counts[] = {
    128u,                 /* 0..127: 1-byte deltas */
    32768u - 128u,        /* 128..32767: 2-byte deltas */
    0xFFFFu * 2u - 32768u /* 32768..131069: 4-byte deltas */
  };
  ثابت صحيح starts[] = {
    0,
    128,
    32768
  };

  hb_always_assert (صف_storage.alloc (ڪل_ڳڻپ));

  لكل (بنانشان ٻالٽي = 0; ٻالٽي < 3; ٻالٽي++)
  {
    hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح>*> صفون;
    hb_always_assert (صفون.alloc (counts[ٻالٽي]));

    لكل (بنانشان i = 0; i < counts[ٻالٽي]; i++)
    {
      hb_ويڪٽر_t<صحيح> صف;
      hb_always_assert (صف.resize (1));
      صف[0] = starts[ٻالٽي] + i;
      صف_storage.push (std::move (صف));
    }

    hb_always_assert (!صف_storage.اندريون_error ());

    بنانشان بنياد = صف_storage.ڊيگهائي - counts[ٻالٽي];
    لكل (بنانشان i = 0; i < counts[ٻالٽي]; i++)
    {
      ثابت hb_ويڪٽر_t<صحيح> *صف = &صف_storage.arrayZ[بنياد + i];
      صفون.push (صف);
      front_mapping.سيٽ (old_varidx++, صف);
    }

    hb_always_assert (!صفون.اندريون_error ());
    hb_always_assert (شي_vars.جوڙيو_vardata_encoding_for_testing (std::move (صفون), 1));
  }

  hb_always_assert (!front_mapping.اندريون_error ());
  hb_always_assert (شي_vars.compile_varidx_map_for_testing (front_mapping));

  ثابت خودڪار& encodings = شي_vars.get_vardata_encodings ();
  hb_always_assert (encodings.ڊيگهائي == 4);
  hb_always_assert (encodings[0].شيون.ڊيگهائي == 128u);
  hb_always_assert (encodings[1].شيون.ڊيگهائي == 32768u - 128u);
  hb_always_assert (encodings[2].شيون.ڊيگهائي == 0xFFFFu);
  hb_always_assert (encodings[3].شيون.ڊيگهائي == 32767u);

  ثابت hb_map_t& varidx_map = شي_vars.get_varidx_map ();
  hb_always_assert (varidx_map.get (0) == 0);
  hb_always_assert (varidx_map.get (127) == 127);
  hb_always_assert (varidx_map.get (128) == 0x00010000u);
  hb_always_assert (varidx_map.get (32767) == 0x00017F7Fu);
  hb_always_assert (varidx_map.get (32768) == 0x00020000u);
  hb_always_assert (varidx_map.get (98302) == 0x0002FFFEu);
  hb_always_assert (varidx_map.get (98303) == 0x00030000u);
  hb_always_assert (varidx_map.get (131069) == 0x00037FFEu);
}

ساكن فراغ
test_signed_delta_widths ()
{
  ثابت صحيح deltas[] = {32767, 32768, 65535, 65536,
                       -32768, -32769, -65536, -65537};
  hb_ٽيگ_t axis = HB_ٽيگ ('T', 'E', 'S', 'T');
  hb_ويڪٽر_t<hb_ٽيگ_t> axes;
  axes.push (axis);
  hb_map_t axis_map;
  axis_map.سيٽ (0, axis);
  hb_hashmap_t<hb_ٽيگ_t, Triple> region;
  region.سيٽ (axis, Triple (0, 1, 1));
  hb_ويڪٽر_t<ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *> regions;
  regions.push (&region);

  لكل (صحيح delta : deltas)
  {
    hb_ويڪٽر_t<صحيح> صف;
    صف.push (delta);
    hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح> *> صفون;
    صفون.push (&صف);
    hb_ويڪٽر_t<OT::delta_صف_encoding_t> encodings;
    encodings.push (OT::delta_صف_encoding_t (std::move (صفون), 1));

    حرف buf[128];
    hb_serialize_context_t s (buf, حجم (buf));
    خودڪار *ٻاهريون = s.شروعات_serialize<OT::شي_Variation_Store> ();
    hb_always_assert (ٻاهريون->serialize (&s, سچ, axes, regions, encodings));
    s.پڇاڙي_serialize ();
    hb_بائيٽون_t بائيٽون = s.copy_بائيٽون ();
    ثابت خودڪار &store = *reinterpret_cast<ثابت OT::شي_Variation_Store *> (بائيٽون.arrayZ);
    ثابت خودڪار &معلومات = store.get_sub_جدول (0);
    hb_always_assert (معلومات.get_شي_delta (0, 0) == delta);

    hb_inc_bimap_t inner_map, region_map;
    inner_map.جوڙيو (0);
    region_map.جوڙيو (0);
    حرف subset_buf[32];
    hb_serialize_context_t subset_s (subset_buf, حجم (subset_buf));
    خودڪار *subset = subset_s.شروعات_serialize<OT::Var_معلومات> ();
    hb_always_assert (subset->serialize (&subset_s, &معلومات, inner_map, region_map));
    subset_s.پڇاڙي_serialize ();
    hb_بائيٽون_t subset_بائيٽون = subset_s.copy_بائيٽون ();
    ثابت خودڪار &subset_معلومات = *reinterpret_cast<ثابت OT::Var_معلومات *> (subset_بائيٽون.arrayZ);
    hb_always_assert (subset_معلومات.get_شي_delta (0, 0) == delta);

    OT::شي_variations_t شي_vars;
    hb_always_assert (شي_vars.create_from_شي_varstore (store, axis_map));
    hb_hashmap_t<hb_ٽيگ_t, Triple> locations;
    hb_hashmap_t<hb_ٽيگ_t, TripleDistances> distances;
    hb_always_assert (شي_vars.instantiate_tuple_vars (locations, distances));
    hb_always_assert (شي_vars.as_شي_varstore ());
    hb_always_assert (شي_vars.has_long_لفظ () ==
                      (delta < -32768 || delta > 32767));
    subset_بائيٽون.fini ();
    بائيٽون.fini ();
  }
}

ساكن فراغ
test_accumulated_delta_widths ()
{
  ثابت صحيح deltas[] = {20000, -20000, 1073741824};
  لكل (صحيح delta : deltas)
  {
    OT::شي_variations_t شي_vars;
    بنانشان outer = شي_vars.جوڙيو_vardata (1);
    شي_vars.جوڙيو_tuple (outer, hb_hashmap_t<hb_ٽيگ_t, Triple> (), 0, delta, 1);
    شي_vars.جوڙيو_tuple (outer, hb_hashmap_t<hb_ٽيگ_t, Triple> (), 0, delta, 1);
    hb_always_assert (شي_vars.build_region_list ());
    إذا (delta == 1073741824)
      hb_always_assert (!شي_vars.as_شي_varstore ());
    وإلا
    {
      hb_always_assert (شي_vars.as_شي_varstore ());
      hb_always_assert (شي_vars.has_long_لفظ ());
      hb_always_assert ((*شي_vars.get_vardata_encodings ()[0].شيون[0])[0] == 2 * delta);
    }
  }
}

ساكن فراغ
test_condition_delta_precision ()
{
  hb_ويڪٽر_t<hb_ٽيگ_t> axes;
  axes.push (HB_ٽيگ ('T','E','S','T'));
  hb_hashmap_t<hb_ٽيگ_t, Triple> neutral, region;
  region.سيٽ (axes[0], Triple (0, 1, 1));
  hb_ويڪٽر_t<ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *> regions;
  regions.push (&neutral);
  regions.push (&region);
  hb_ويڪٽر_t<صحيح> صف;
  صف.push (16777217);
  صف.push (-16777216);
  hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح> *> صفون;
  صفون.push (&صف);
  hb_ويڪٽر_t<OT::delta_صف_encoding_t> encodings;
  encodings.push (OT::delta_صف_encoding_t (std::move (صفون), 2));
  حرف buf[128];
  hb_serialize_context_t s (buf, حجم (buf));
  خودڪار *ٻاهريون = s.شروعات_serialize<OT::شي_Variation_Store> ();
  hb_always_assert (ٻاهريون->serialize (&s, سچ, axes, regions, encodings));
  s.پڇاڙي_serialize ();
  hb_بائيٽون_t بائيٽون = s.copy_بائيٽون ();
  ثابت خودڪار &store = *reinterpret_cast<ثابت OT::شي_Variation_Store *> (بائيٽون.arrayZ);
  صحيح coords[] = {16384};
  OT::شي_Var_Store_Instancer instancer (&store, خالي_اشارو, hb_array (coords));
  // Positioning keeps float arithmetic, but conditions must retain the unit
  // left after cancellation of two deltas exceeding float's integer range.
  hb_always_assert (instancer[0] == 0.f);
  hb_always_assert (instancer.get_condition_delta (0) == 1.);
  ثابت حرف condition_معلومات[] = "\x00\x02\x00\x00\x00\x00\x00\x00";
  ثابت خودڪار &condition = *reinterpret_cast<ثابت OT::Condition *> (condition_معلومات);
  hb_always_assert (condition.evaluate (coords, 1, &instancer));
  بائيٽون.fini ();
}

ساكن فراغ
test_failed_encoding ()
{
  hb_ويڪٽر_t<صحيح> صف;
  صف.push (1);
  hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح> *> صفون;
  صفون.push (&صف);
  OT::شي_variations_t شي_vars;
  hb_always_assert (شي_vars.جوڙيو_vardata_encoding_for_testing (std::move (صفون), 1));
  خودڪار &encoding = const_cast<OT::delta_صف_encoding_t &> (شي_vars.get_vardata_encodings ()[0]);
  /* Simulate a failed allocation of the encoding's column metadata. */
  encoding.chars.clear ();
  encoding.chars.سيٽ_error ();
  hb_always_assert (encoding.اندريون_error ());
  hb_hashmap_t<بنانشان, ثابت hb_ويڪٽر_t<صحيح> *> front_mapping;
  front_mapping.سيٽ (0, &صف);
  hb_always_assert (!شي_vars.compile_varidx_map_for_testing (front_mapping));
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  test_scalar_cache_precision ();
  test_شي_variations ();
  test_implicit_advance_mapping ();
  test_شي_variations_حد_کان_مٿي ();
  test_signed_delta_widths ();
  test_failed_encoding ();
  test_accumulated_delta_widths ();
  test_condition_delta_precision ();
}

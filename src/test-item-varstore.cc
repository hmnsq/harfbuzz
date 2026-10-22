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
ثابت حرف hvar_انکڑے[] = "\x0\x1\x0\x0\x0\x0\x0\x14\x0\x0\x0\xc4\x0\x0\x0\x0\x0\x0\x0\x0\x0\x1\x0\x0\x0\x10\x0\x2\x0\x0\x0\x74\x0\x0\x0\x7a\x0\x2\x0\x8\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\x0\x1\x0\x0\x0\x0\x0\x4\x0\x0\x0\x8\x0\x0\x0\x1\x0\x2\x0\x3\x0\x4\x0\x5\x0\x6\x0\x7\xf9\xf\x2f\xbf\xfb\xfb\x35\xf9\x4\x4\xf3\xb4\xf2\xfb\x2e\xf3\x4\x4\xe\xad\xfa\x1\x1a\x1\x15\x22\x59\xd6\xe3\xf6\x6\xf5\x0\x1\x0\x5\x0\x4\x7\x5\x6";

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
  ثابت بنانشانی counts[] = {4, 129};
  OT::hb_scalar_cache_t scratch_cache;
  لكل (بنانشانی گنتی : counts)
  {
    خودکار *cache = OT::hb_scalar_cache_t::create (گنتی, &scratch_cache);
    hb_always_assert (cache->لمائی == گنتی);
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      عائم value;
      عائم scalar = scalars[i % ARRAY_لمائی (scalars)];
      hb_always_assert (!cache->get (i, &value));
      cache->سیٹ (i, scalar);
      hb_always_assert (cache->get (i, &value));
      hb_always_assert (value == scalar);
    }
    cache->clear ();
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      عائم value;
      hb_always_assert (!cache->get (i, &value));
    }
    OT::hb_scalar_cache_t::destroy (cache, &scratch_cache);
  }
}

ساكن فراغ
test_شے_variations ()
{
  ثابت OT::HVAR* hvar_جدول = reinterpret_cast<ثابت OT::HVAR*> (hvar_انکڑے);

  hb_ٹیگ_t axis_ٹیگ_2 = HB_ٹیگ ('w', 'g', 'h', 't');
  hb_map_t axis_idx_ٹیگ_map;
  axis_idx_ٹیگ_map.سیٹ (0, axis_ٹیگ_2);

  axis_ٹیگ_2 = HB_ٹیگ ('o', 'p', 's', 'z');
  axis_idx_ٹیگ_map.سیٹ (1, axis_ٹیگ_2);

  OT::شے_variations_t شے_vars;
  ثابت OT::شے_Variation_Store& src_var_store = hvar_جدول+(hvar_جدول->varStore);
  منطقي نتیجہ = شے_vars.create_from_شے_varstore (src_var_store, axis_idx_ٹیگ_map);
      
  hb_always_assert (نتیجہ);

  /* partial instancing wght=300:800 */
  hb_hashmap_t<hb_ٹیگ_t, Triple> normalized_axes_location;
  normalized_axes_location.سیٹ (axis_ٹیگ_2, Triple (-0.512817, 0.0, 0.7000120));

  hb_hashmap_t<hb_ٹیگ_t, TripleDistances> axes_triple_distances;
  axes_triple_distances.سیٹ (axis_ٹیگ_2, TripleDistances (200.0, 500.0));

  نتیجہ = شے_vars.instantiate_tuple_vars (normalized_axes_location, axes_triple_distances);
  hb_always_assert (نتیجہ);
  نتیجہ = شے_vars.as_شے_varstore (خطأ);
  hb_always_assert (نتیجہ);
  hb_always_assert (شے_vars.get_region_list().لمائی == 8);
}

ساكن فراغ
test_implicit_advance_mapping ()
{
  حرف انکڑے[حجم (hvar_انکڑے)];
  hb_memcpy (انکڑے, hvar_انکڑے, حجم (انکڑے));
  خودکار *hvar = reinterpret_cast<OT::HVAR *> (انکڑے);
  ثابت خودکار &store = hvar + hvar->varStore;
  ثابت صحيح locations[] = {-16384, 0, 16384};
  منطقي exercised_outer_one = خطأ;

  لكل (صحيح بھار : locations)
    لكل (صحيح ناپ : locations)
    {
      صحيح coords[] = {بھار, ناپ};
      بنانشانی mapped = (hvar + hvar->advMap).map (0x10000u);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) ==
                        store.get_delta (mapped, coords, 2));
    }

  hvar->advMap = 0;
  لكل (صحيح بھار : locations)
    لكل (صحيح ناپ : locations)
    {
      صحيح coords[] = {بھار, ناپ};
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
test_شے_variations_حد_توں_ودھ ()
{
  hb_سمتیہ_t<hb_سمتیہ_t<صحيح>> صف_storage;
  hb_hashmap_t<بنانشانی, ثابت hb_سمتیہ_t<صحيح>*> front_mapping;
  OT::شے_variations_t شے_vars;
  بنانشانی old_varidx = 0;
  ثابت بنانشانی کل_گنتی = 0xFFFFu * 2u;

  ثابت بنانشانی counts[] = {
    128u,                 /* 0..127: 1-byte deltas */
    32768u - 128u,        /* 128..32767: 2-byte deltas */
    0xFFFFu * 2u - 32768u /* 32768..131069: 4-byte deltas */
  };
  ثابت صحيح starts[] = {
    0,
    128,
    32768
  };

  hb_always_assert (صف_storage.alloc (کل_گنتی));

  لكل (بنانشانی بالٹی = 0; بالٹی < 3; بالٹی++)
  {
    hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح>*> صفاں;
    hb_always_assert (صفاں.alloc (counts[بالٹی]));

    لكل (بنانشانی i = 0; i < counts[بالٹی]; i++)
    {
      hb_سمتیہ_t<صحيح> صف;
      hb_always_assert (صف.resize (1));
      صف[0] = starts[بالٹی] + i;
      صف_storage.push (std::move (صف));
    }

    hb_always_assert (!صف_storage.وچ_error ());

    بنانشانی ادھار = صف_storage.لمائی - counts[بالٹی];
    لكل (بنانشانی i = 0; i < counts[بالٹی]; i++)
    {
      ثابت hb_سمتیہ_t<صحيح> *صف = &صف_storage.arrayZ[ادھار + i];
      صفاں.push (صف);
      front_mapping.سیٹ (old_varidx++, صف);
    }

    hb_always_assert (!صفاں.وچ_error ());
    hb_always_assert (شے_vars.جوڑو_vardata_encoding_for_testing (std::move (صفاں), 1));
  }

  hb_always_assert (!front_mapping.وچ_error ());
  hb_always_assert (شے_vars.compile_varidx_map_for_testing (front_mapping));

  ثابت خودکار& encodings = شے_vars.get_vardata_encodings ();
  hb_always_assert (encodings.لمائی == 4);
  hb_always_assert (encodings[0].اشیاء.لمائی == 128u);
  hb_always_assert (encodings[1].اشیاء.لمائی == 32768u - 128u);
  hb_always_assert (encodings[2].اشیاء.لمائی == 0xFFFFu);
  hb_always_assert (encodings[3].اشیاء.لمائی == 32767u);

  ثابت hb_map_t& varidx_map = شے_vars.get_varidx_map ();
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
  hb_ٹیگ_t axis = HB_ٹیگ ('T', 'E', 'S', 'T');
  hb_سمتیہ_t<hb_ٹیگ_t> axes;
  axes.push (axis);
  hb_map_t axis_map;
  axis_map.سیٹ (0, axis);
  hb_hashmap_t<hb_ٹیگ_t, Triple> region;
  region.سیٹ (axis, Triple (0, 1, 1));
  hb_سمتیہ_t<ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *> regions;
  regions.push (&region);

  لكل (صحيح delta : deltas)
  {
    hb_سمتیہ_t<صحيح> صف;
    صف.push (delta);
    hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح> *> صفاں;
    صفاں.push (&صف);
    hb_سمتیہ_t<OT::delta_صف_encoding_t> encodings;
    encodings.push (OT::delta_صف_encoding_t (std::move (صفاں), 1));

    حرف buf[128];
    hb_serialize_context_t s (buf, حجم (buf));
    خودکار *باہرلا = s.شروع_serialize<OT::شے_Variation_Store> ();
    hb_always_assert (باہرلا->serialize (&s, سچ, axes, regions, encodings));
    s.سرا_serialize ();
    hb_بائٹاں_t بائٹاں = s.copy_بائٹاں ();
    ثابت خودکار &store = *reinterpret_cast<ثابت OT::شے_Variation_Store *> (بائٹاں.arrayZ);
    ثابت خودکار &انکڑے = store.get_sub_جدول (0);
    hb_always_assert (انکڑے.get_شے_delta (0, 0) == delta);

    hb_inc_bimap_t inner_map, region_map;
    inner_map.جوڑو (0);
    region_map.جوڑو (0);
    حرف subset_buf[32];
    hb_serialize_context_t subset_s (subset_buf, حجم (subset_buf));
    خودکار *subset = subset_s.شروع_serialize<OT::Var_انکڑے> ();
    hb_always_assert (subset->serialize (&subset_s, &انکڑے, inner_map, region_map));
    subset_s.سرا_serialize ();
    hb_بائٹاں_t subset_بائٹاں = subset_s.copy_بائٹاں ();
    ثابت خودکار &subset_انکڑے = *reinterpret_cast<ثابت OT::Var_انکڑے *> (subset_بائٹاں.arrayZ);
    hb_always_assert (subset_انکڑے.get_شے_delta (0, 0) == delta);

    OT::شے_variations_t شے_vars;
    hb_always_assert (شے_vars.create_from_شے_varstore (store, axis_map));
    hb_hashmap_t<hb_ٹیگ_t, Triple> locations;
    hb_hashmap_t<hb_ٹیگ_t, TripleDistances> distances;
    hb_always_assert (شے_vars.instantiate_tuple_vars (locations, distances));
    hb_always_assert (شے_vars.as_شے_varstore ());
    hb_always_assert (شے_vars.has_long_لفظ () ==
                      (delta < -32768 || delta > 32767));
    subset_بائٹاں.fini ();
    بائٹاں.fini ();
  }
}

ساكن فراغ
test_accumulated_delta_widths ()
{
  ثابت صحيح deltas[] = {20000, -20000, 1073741824};
  لكل (صحيح delta : deltas)
  {
    OT::شے_variations_t شے_vars;
    بنانشانی outer = شے_vars.جوڑو_vardata (1);
    شے_vars.جوڑو_tuple (outer, hb_hashmap_t<hb_ٹیگ_t, Triple> (), 0, delta, 1);
    شے_vars.جوڑو_tuple (outer, hb_hashmap_t<hb_ٹیگ_t, Triple> (), 0, delta, 1);
    hb_always_assert (شے_vars.build_region_list ());
    إذا (delta == 1073741824)
      hb_always_assert (!شے_vars.as_شے_varstore ());
    وإلا
    {
      hb_always_assert (شے_vars.as_شے_varstore ());
      hb_always_assert (شے_vars.has_long_لفظ ());
      hb_always_assert ((*شے_vars.get_vardata_encodings ()[0].اشیاء[0])[0] == 2 * delta);
    }
  }
}

ساكن فراغ
test_condition_delta_precision ()
{
  hb_سمتیہ_t<hb_ٹیگ_t> axes;
  axes.push (HB_ٹیگ ('T','E','S','T'));
  hb_hashmap_t<hb_ٹیگ_t, Triple> neutral, region;
  region.سیٹ (axes[0], Triple (0, 1, 1));
  hb_سمتیہ_t<ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *> regions;
  regions.push (&neutral);
  regions.push (&region);
  hb_سمتیہ_t<صحيح> صف;
  صف.push (16777217);
  صف.push (-16777216);
  hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح> *> صفاں;
  صفاں.push (&صف);
  hb_سمتیہ_t<OT::delta_صف_encoding_t> encodings;
  encodings.push (OT::delta_صف_encoding_t (std::move (صفاں), 2));
  حرف buf[128];
  hb_serialize_context_t s (buf, حجم (buf));
  خودکار *باہرلا = s.شروع_serialize<OT::شے_Variation_Store> ();
  hb_always_assert (باہرلا->serialize (&s, سچ, axes, regions, encodings));
  s.سرا_serialize ();
  hb_بائٹاں_t بائٹاں = s.copy_بائٹاں ();
  ثابت خودکار &store = *reinterpret_cast<ثابت OT::شے_Variation_Store *> (بائٹاں.arrayZ);
  صحيح coords[] = {16384};
  OT::شے_Var_Store_Instancer instancer (&store, خالی_سوچک, hb_array (coords));
  // Positioning keeps float arithmetic, but conditions must retain the unit
  // left after cancellation of two deltas exceeding float's integer range.
  hb_always_assert (instancer[0] == 0.f);
  hb_always_assert (instancer.get_condition_delta (0) == 1.);
  ثابت حرف condition_انکڑے[] = "\x00\x02\x00\x00\x00\x00\x00\x00";
  ثابت خودکار &condition = *reinterpret_cast<ثابت OT::Condition *> (condition_انکڑے);
  hb_always_assert (condition.evaluate (coords, 1, &instancer));
  بائٹاں.fini ();
}

ساكن فراغ
test_failed_encoding ()
{
  hb_سمتیہ_t<صحيح> صف;
  صف.push (1);
  hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح> *> صفاں;
  صفاں.push (&صف);
  OT::شے_variations_t شے_vars;
  hb_always_assert (شے_vars.جوڑو_vardata_encoding_for_testing (std::move (صفاں), 1));
  خودکار &encoding = const_cast<OT::delta_صف_encoding_t &> (شے_vars.get_vardata_encodings ()[0]);
  /* Simulate a failed allocation of the encoding's column metadata. */
  encoding.chars.clear ();
  encoding.chars.سیٹ_error ();
  hb_always_assert (encoding.وچ_error ());
  hb_hashmap_t<بنانشانی, ثابت hb_سمتیہ_t<صحيح> *> front_mapping;
  front_mapping.سیٹ (0, &صف);
  hb_always_assert (!شے_vars.compile_varidx_map_for_testing (front_mapping));
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  test_scalar_cache_precision ();
  test_شے_variations ();
  test_implicit_advance_mapping ();
  test_شے_variations_حد_توں_ودھ ();
  test_signed_delta_widths ();
  test_failed_encoding ();
  test_accumulated_delta_widths ();
  test_condition_delta_precision ();
}

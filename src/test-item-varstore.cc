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
  ثابت بینښه counts[] = {4, 129};
  OT::hb_scalar_cache_t scratch_cache;
  لكل (بینښه شمېر : counts)
  {
    خپلکاری *cache = OT::hb_scalar_cache_t::create (شمېر, &scratch_cache);
    hb_always_assert (cache->اوږدتیا == شمېر);
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      عائم value;
      عائم scalar = scalars[i % ARRAY_اوږدتیا (scalars)];
      hb_always_assert (!cache->get (i, &value));
      cache->سېټ (i, scalar);
      hb_always_assert (cache->get (i, &value));
      hb_always_assert (value == scalar);
    }
    cache->clear ();
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      عائم value;
      hb_always_assert (!cache->get (i, &value));
    }
    OT::hb_scalar_cache_t::destroy (cache, &scratch_cache);
  }
}

ساكن فراغ
test_توکی_variations ()
{
  ثابت OT::HVAR* hvar_جدول = reinterpret_cast<ثابت OT::HVAR*> (hvar_معلومات);

  hb_نښان_t axis_نښان_2 = HB_نښان ('w', 'g', 'h', 't');
  hb_map_t axis_idx_نښان_map;
  axis_idx_نښان_map.سېټ (0, axis_نښان_2);

  axis_نښان_2 = HB_نښان ('o', 'p', 's', 'z');
  axis_idx_نښان_map.سېټ (1, axis_نښان_2);

  OT::توکی_variations_t توکی_vars;
  ثابت OT::توکی_Variation_Store& src_var_store = hvar_جدول+(hvar_جدول->varStore);
  منطقي پایله = توکی_vars.create_from_توکی_varstore (src_var_store, axis_idx_نښان_map);
      
  hb_always_assert (پایله);

  /* partial instancing wght=300:800 */
  hb_hashmap_t<hb_نښان_t, Triple> normalized_axes_location;
  normalized_axes_location.سېټ (axis_نښان_2, Triple (-0.512817, 0.0, 0.7000120));

  hb_hashmap_t<hb_نښان_t, TripleDistances> axes_triple_distances;
  axes_triple_distances.سېټ (axis_نښان_2, TripleDistances (200.0, 500.0));

  پایله = توکی_vars.instantiate_tuple_vars (normalized_axes_location, axes_triple_distances);
  hb_always_assert (پایله);
  پایله = توکی_vars.as_توکی_varstore (خطأ);
  hb_always_assert (پایله);
  hb_always_assert (توکی_vars.get_region_list().اوږدتیا == 8);
}

ساكن فراغ
test_implicit_advance_mapping ()
{
  حرف معلومات[حجم (hvar_معلومات)];
  hb_memcpy (معلومات, hvar_معلومات, حجم (معلومات));
  خپلکاری *hvar = reinterpret_cast<OT::HVAR *> (معلومات);
  ثابت خپلکاری &store = hvar + hvar->varStore;
  ثابت صحيح locations[] = {-16384, 0, 16384};
  منطقي exercised_outer_one = خطأ;

  لكل (صحيح وزن : locations)
    لكل (صحيح اندازه : locations)
    {
      صحيح coords[] = {وزن, اندازه};
      بینښه mapped = (hvar + hvar->advMap).map (0x10000u);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) ==
                        store.get_delta (mapped, coords, 2));
    }

  hvar->advMap = 0;
  لكل (صحيح وزن : locations)
    لكل (صحيح اندازه : locations)
    {
      صحيح coords[] = {وزن, اندازه};
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
test_توکی_variations_ډکېدنه ()
{
  hb_ویکټور_t<hb_ویکټور_t<صحيح>> لیکه_storage;
  hb_hashmap_t<بینښه, ثابت hb_ویکټور_t<صحيح>*> front_mapping;
  OT::توکی_variations_t توکی_vars;
  بینښه old_varidx = 0;
  ثابت بینښه ټول_شمېر = 0xFFFFu * 2u;

  ثابت بینښه counts[] = {
    128u,                 /* 0..127: 1-byte deltas */
    32768u - 128u,        /* 128..32767: 2-byte deltas */
    0xFFFFu * 2u - 32768u /* 32768..131069: 4-byte deltas */
  };
  ثابت صحيح starts[] = {
    0,
    128,
    32768
  };

  hb_always_assert (لیکه_storage.alloc (ټول_شمېر));

  لكل (بینښه لوښی = 0; لوښی < 3; لوښی++)
  {
    hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح>*> لیکې;
    hb_always_assert (لیکې.alloc (counts[لوښی]));

    لكل (بینښه i = 0; i < counts[لوښی]; i++)
    {
      hb_ویکټور_t<صحيح> لیکه;
      hb_always_assert (لیکه.resize (1));
      لیکه[0] = starts[لوښی] + i;
      لیکه_storage.push (std::move (لیکه));
    }

    hb_always_assert (!لیکه_storage.دننه_error ());

    بینښه بنسټ = لیکه_storage.اوږدتیا - counts[لوښی];
    لكل (بینښه i = 0; i < counts[لوښی]; i++)
    {
      ثابت hb_ویکټور_t<صحيح> *لیکه = &لیکه_storage.arrayZ[بنسټ + i];
      لیکې.push (لیکه);
      front_mapping.سېټ (old_varidx++, لیکه);
    }

    hb_always_assert (!لیکې.دننه_error ());
    hb_always_assert (توکی_vars.جمع_کړه_vardata_encoding_for_testing (std::move (لیکې), 1));
  }

  hb_always_assert (!front_mapping.دننه_error ());
  hb_always_assert (توکی_vars.compile_varidx_map_for_testing (front_mapping));

  ثابت خپلکاری& encodings = توکی_vars.get_vardata_encodings ();
  hb_always_assert (encodings.اوږدتیا == 4);
  hb_always_assert (encodings[0].توکي.اوږدتیا == 128u);
  hb_always_assert (encodings[1].توکي.اوږدتیا == 32768u - 128u);
  hb_always_assert (encodings[2].توکي.اوږدتیا == 0xFFFFu);
  hb_always_assert (encodings[3].توکي.اوږدتیا == 32767u);

  ثابت hb_map_t& varidx_map = توکی_vars.get_varidx_map ();
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
  hb_نښان_t axis = HB_نښان ('T', 'E', 'S', 'T');
  hb_ویکټور_t<hb_نښان_t> axes;
  axes.push (axis);
  hb_map_t axis_map;
  axis_map.سېټ (0, axis);
  hb_hashmap_t<hb_نښان_t, Triple> region;
  region.سېټ (axis, Triple (0, 1, 1));
  hb_ویکټور_t<ثابت hb_hashmap_t<hb_نښان_t, Triple> *> regions;
  regions.push (&region);

  لكل (صحيح delta : deltas)
  {
    hb_ویکټور_t<صحيح> لیکه;
    لیکه.push (delta);
    hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح> *> لیکې;
    لیکې.push (&لیکه);
    hb_ویکټور_t<OT::delta_لیکه_encoding_t> encodings;
    encodings.push (OT::delta_لیکه_encoding_t (std::move (لیکې), 1));

    حرف buf[128];
    hb_serialize_context_t s (buf, حجم (buf));
    خپلکاری *بهر = s.پیل_serialize<OT::توکی_Variation_Store> ();
    hb_always_assert (بهر->serialize (&s, ریښتیا, axes, regions, encodings));
    s.پای_ټکی_serialize ();
    hb_بایټونه_t بایټونه = s.copy_بایټونه ();
    ثابت خپلکاری &store = *reinterpret_cast<ثابت OT::توکی_Variation_Store *> (بایټونه.arrayZ);
    ثابت خپلکاری &معلومات = store.get_sub_جدول (0);
    hb_always_assert (معلومات.get_توکی_delta (0, 0) == delta);

    hb_inc_bimap_t inner_map, region_map;
    inner_map.جمع_کړه (0);
    region_map.جمع_کړه (0);
    حرف subset_buf[32];
    hb_serialize_context_t subset_s (subset_buf, حجم (subset_buf));
    خپلکاری *subset = subset_s.پیل_serialize<OT::Var_معلومات> ();
    hb_always_assert (subset->serialize (&subset_s, &معلومات, inner_map, region_map));
    subset_s.پای_ټکی_serialize ();
    hb_بایټونه_t subset_بایټونه = subset_s.copy_بایټونه ();
    ثابت خپلکاری &subset_معلومات = *reinterpret_cast<ثابت OT::Var_معلومات *> (subset_بایټونه.arrayZ);
    hb_always_assert (subset_معلومات.get_توکی_delta (0, 0) == delta);

    OT::توکی_variations_t توکی_vars;
    hb_always_assert (توکی_vars.create_from_توکی_varstore (store, axis_map));
    hb_hashmap_t<hb_نښان_t, Triple> locations;
    hb_hashmap_t<hb_نښان_t, TripleDistances> distances;
    hb_always_assert (توکی_vars.instantiate_tuple_vars (locations, distances));
    hb_always_assert (توکی_vars.as_توکی_varstore ());
    hb_always_assert (توکی_vars.has_long_کلمه () ==
                      (delta < -32768 || delta > 32767));
    subset_بایټونه.fini ();
    بایټونه.fini ();
  }
}

ساكن فراغ
test_accumulated_delta_widths ()
{
  ثابت صحيح deltas[] = {20000, -20000, 1073741824};
  لكل (صحيح delta : deltas)
  {
    OT::توکی_variations_t توکی_vars;
    بینښه outer = توکی_vars.جمع_کړه_vardata (1);
    توکی_vars.جمع_کړه_tuple (outer, hb_hashmap_t<hb_نښان_t, Triple> (), 0, delta, 1);
    توکی_vars.جمع_کړه_tuple (outer, hb_hashmap_t<hb_نښان_t, Triple> (), 0, delta, 1);
    hb_always_assert (توکی_vars.build_region_list ());
    إذا (delta == 1073741824)
      hb_always_assert (!توکی_vars.as_توکی_varstore ());
    وإلا
    {
      hb_always_assert (توکی_vars.as_توکی_varstore ());
      hb_always_assert (توکی_vars.has_long_کلمه ());
      hb_always_assert ((*توکی_vars.get_vardata_encodings ()[0].توکي[0])[0] == 2 * delta);
    }
  }
}

ساكن فراغ
test_condition_delta_precision ()
{
  hb_ویکټور_t<hb_نښان_t> axes;
  axes.push (HB_نښان ('T','E','S','T'));
  hb_hashmap_t<hb_نښان_t, Triple> neutral, region;
  region.سېټ (axes[0], Triple (0, 1, 1));
  hb_ویکټور_t<ثابت hb_hashmap_t<hb_نښان_t, Triple> *> regions;
  regions.push (&neutral);
  regions.push (&region);
  hb_ویکټور_t<صحيح> لیکه;
  لیکه.push (16777217);
  لیکه.push (-16777216);
  hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح> *> لیکې;
  لیکې.push (&لیکه);
  hb_ویکټور_t<OT::delta_لیکه_encoding_t> encodings;
  encodings.push (OT::delta_لیکه_encoding_t (std::move (لیکې), 2));
  حرف buf[128];
  hb_serialize_context_t s (buf, حجم (buf));
  خپلکاری *بهر = s.پیل_serialize<OT::توکی_Variation_Store> ();
  hb_always_assert (بهر->serialize (&s, ریښتیا, axes, regions, encodings));
  s.پای_ټکی_serialize ();
  hb_بایټونه_t بایټونه = s.copy_بایټونه ();
  ثابت خپلکاری &store = *reinterpret_cast<ثابت OT::توکی_Variation_Store *> (بایټونه.arrayZ);
  صحيح coords[] = {16384};
  OT::توکی_Var_Store_Instancer instancer (&store, تش_اشاره, hb_array (coords));
  // Positioning keeps float arithmetic, but conditions must retain the unit
  // left after cancellation of two deltas exceeding float's integer range.
  hb_always_assert (instancer[0] == 0.f);
  hb_always_assert (instancer.get_condition_delta (0) == 1.);
  ثابت حرف condition_معلومات[] = "\x00\x02\x00\x00\x00\x00\x00\x00";
  ثابت خپلکاری &condition = *reinterpret_cast<ثابت OT::Condition *> (condition_معلومات);
  hb_always_assert (condition.evaluate (coords, 1, &instancer));
  بایټونه.fini ();
}

ساكن فراغ
test_failed_encoding ()
{
  hb_ویکټور_t<صحيح> لیکه;
  لیکه.push (1);
  hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح> *> لیکې;
  لیکې.push (&لیکه);
  OT::توکی_variations_t توکی_vars;
  hb_always_assert (توکی_vars.جمع_کړه_vardata_encoding_for_testing (std::move (لیکې), 1));
  خپلکاری &encoding = const_cast<OT::delta_لیکه_encoding_t &> (توکی_vars.get_vardata_encodings ()[0]);
  /* Simulate a failed allocation of the encoding's column metadata. */
  encoding.chars.clear ();
  encoding.chars.سېټ_error ();
  hb_always_assert (encoding.دننه_error ());
  hb_hashmap_t<بینښه, ثابت hb_ویکټور_t<صحيح> *> front_mapping;
  front_mapping.سېټ (0, &لیکه);
  hb_always_assert (!توکی_vars.compile_varidx_map_for_testing (front_mapping));
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  test_scalar_cache_precision ();
  test_توکی_variations ();
  test_implicit_advance_mapping ();
  test_توکی_variations_ډکېدنه ();
  test_signed_delta_widths ();
  test_failed_encoding ();
  test_accumulated_delta_widths ();
  test_condition_delta_precision ();
}

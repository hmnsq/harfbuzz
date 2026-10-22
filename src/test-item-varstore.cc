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
ثابت حرف hvar_داده[] = "\x0\x1\x0\x0\x0\x0\x0\x14\x0\x0\x0\xc4\x0\x0\x0\x0\x0\x0\x0\x0\x0\x1\x0\x0\x0\x10\x0\x2\x0\x0\x0\x74\x0\x0\x0\x7a\x0\x2\x0\x8\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\x0\x1\x0\x0\x0\x0\x0\x4\x0\x0\x0\x8\x0\x0\x0\x1\x0\x2\x0\x3\x0\x4\x0\x5\x0\x6\x0\x7\xf9\xf\x2f\xbf\xfb\xfb\x35\xf9\x4\x4\xf3\xb4\xf2\xfb\x2e\xf3\x4\x4\xe\xad\xfa\x1\x1a\x1\x15\x22\x59\xd6\xe3\xf6\x6\xf5\x0\x1\x0\x5\x0\x4\x7\x5\x6";

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
  ثابت بیعلامت counts[] = {4, 129};
  OT::hb_scalar_cache_t scratch_cache;
  لكل (بیعلامت شمار : counts)
  {
    خودکار *cache = OT::hb_scalar_cache_t::create (شمار, &scratch_cache);
    hb_always_assert (cache->درازی == شمار);
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      عائم value;
      عائم scalar = scalars[i % ARRAY_درازی (scalars)];
      hb_always_assert (!cache->get (i, &value));
      cache->مجموعه (i, scalar);
      hb_always_assert (cache->get (i, &value));
      hb_always_assert (value == scalar);
    }
    cache->clear ();
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      عائم value;
      hb_always_assert (!cache->get (i, &value));
    }
    OT::hb_scalar_cache_t::destroy (cache, &scratch_cache);
  }
}

ساكن فراغ
test_قلم_variations ()
{
  ثابت OT::HVAR* hvar_جدول = reinterpret_cast<ثابت OT::HVAR*> (hvar_داده);

  hb_برچسب_t axis_برچسب_2 = HB_برچسب ('w', 'g', 'h', 't');
  hb_map_t axis_idx_برچسب_map;
  axis_idx_برچسب_map.مجموعه (0, axis_برچسب_2);

  axis_برچسب_2 = HB_برچسب ('o', 'p', 's', 'z');
  axis_idx_برچسب_map.مجموعه (1, axis_برچسب_2);

  OT::قلم_variations_t قلم_vars;
  ثابت OT::قلم_Variation_Store& src_var_store = hvar_جدول+(hvar_جدول->varStore);
  منطقي نتیجه = قلم_vars.create_from_قلم_varstore (src_var_store, axis_idx_برچسب_map);
      
  hb_always_assert (نتیجه);

  /* partial instancing wght=300:800 */
  hb_hashmap_t<hb_برچسب_t, Triple> normalized_axes_location;
  normalized_axes_location.مجموعه (axis_برچسب_2, Triple (-0.512817, 0.0, 0.7000120));

  hb_hashmap_t<hb_برچسب_t, TripleDistances> axes_triple_distances;
  axes_triple_distances.مجموعه (axis_برچسب_2, TripleDistances (200.0, 500.0));

  نتیجه = قلم_vars.instantiate_tuple_vars (normalized_axes_location, axes_triple_distances);
  hb_always_assert (نتیجه);
  نتیجه = قلم_vars.as_قلم_varstore (خطأ);
  hb_always_assert (نتیجه);
  hb_always_assert (قلم_vars.get_region_list().درازی == 8);
}

ساكن فراغ
test_implicit_advance_mapping ()
{
  حرف داده[حجم (hvar_داده)];
  hb_memcpy (داده, hvar_داده, حجم (داده));
  خودکار *hvar = reinterpret_cast<OT::HVAR *> (داده);
  ثابت خودکار &store = hvar + hvar->varStore;
  ثابت صحيح locations[] = {-16384, 0, 16384};
  منطقي exercised_outer_one = خطأ;

  لكل (صحيح وزن : locations)
    لكل (صحيح اندازه : locations)
    {
      صحيح coords[] = {وزن, اندازه};
      بیعلامت mapped = (hvar + hvar->advMap).map (0x10000u);
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
test_قلم_variations_سرریز ()
{
  hb_وکتور_t<hb_وکتور_t<صحيح>> ردیف_storage;
  hb_hashmap_t<بیعلامت, ثابت hb_وکتور_t<صحيح>*> front_mapping;
  OT::قلم_variations_t قلم_vars;
  بیعلامت old_varidx = 0;
  ثابت بیعلامت مجموع_شمار = 0xFFFFu * 2u;

  ثابت بیعلامت counts[] = {
    128u,                 /* 0..127: 1-byte deltas */
    32768u - 128u,        /* 128..32767: 2-byte deltas */
    0xFFFFu * 2u - 32768u /* 32768..131069: 4-byte deltas */
  };
  ثابت صحيح starts[] = {
    0,
    128,
    32768
  };

  hb_always_assert (ردیف_storage.alloc (مجموع_شمار));

  لكل (بیعلامت سطل = 0; سطل < 3; سطل++)
  {
    hb_وکتور_t<ثابت hb_وکتور_t<صحيح>*> ردیفها;
    hb_always_assert (ردیفها.alloc (counts[سطل]));

    لكل (بیعلامت i = 0; i < counts[سطل]; i++)
    {
      hb_وکتور_t<صحيح> ردیف;
      hb_always_assert (ردیف.resize (1));
      ردیف[0] = starts[سطل] + i;
      ردیف_storage.push (std::move (ردیف));
    }

    hb_always_assert (!ردیف_storage.درون_error ());

    بیعلامت پایه = ردیف_storage.درازی - counts[سطل];
    لكل (بیعلامت i = 0; i < counts[سطل]; i++)
    {
      ثابت hb_وکتور_t<صحيح> *ردیف = &ردیف_storage.arrayZ[پایه + i];
      ردیفها.push (ردیف);
      front_mapping.مجموعه (old_varidx++, ردیف);
    }

    hb_always_assert (!ردیفها.درون_error ());
    hb_always_assert (قلم_vars.جمع_کن_vardata_encoding_for_testing (std::move (ردیفها), 1));
  }

  hb_always_assert (!front_mapping.درون_error ());
  hb_always_assert (قلم_vars.compile_varidx_map_for_testing (front_mapping));

  ثابت خودکار& encodings = قلم_vars.get_vardata_encodings ();
  hb_always_assert (encodings.درازی == 4);
  hb_always_assert (encodings[0].اقلام.درازی == 128u);
  hb_always_assert (encodings[1].اقلام.درازی == 32768u - 128u);
  hb_always_assert (encodings[2].اقلام.درازی == 0xFFFFu);
  hb_always_assert (encodings[3].اقلام.درازی == 32767u);

  ثابت hb_map_t& varidx_map = قلم_vars.get_varidx_map ();
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
  hb_برچسب_t axis = HB_برچسب ('T', 'E', 'S', 'T');
  hb_وکتور_t<hb_برچسب_t> axes;
  axes.push (axis);
  hb_map_t axis_map;
  axis_map.مجموعه (0, axis);
  hb_hashmap_t<hb_برچسب_t, Triple> region;
  region.مجموعه (axis, Triple (0, 1, 1));
  hb_وکتور_t<ثابت hb_hashmap_t<hb_برچسب_t, Triple> *> regions;
  regions.push (&region);

  لكل (صحيح delta : deltas)
  {
    hb_وکتور_t<صحيح> ردیف;
    ردیف.push (delta);
    hb_وکتور_t<ثابت hb_وکتور_t<صحيح> *> ردیفها;
    ردیفها.push (&ردیف);
    hb_وکتور_t<OT::delta_ردیف_encoding_t> encodings;
    encodings.push (OT::delta_ردیف_encoding_t (std::move (ردیفها), 1));

    حرف buf[128];
    hb_serialize_context_t s (buf, حجم (buf));
    خودکار *بیرون = s.شروع_serialize<OT::قلم_Variation_Store> ();
    hb_always_assert (بیرون->serialize (&s, درست, axes, regions, encodings));
    s.انتها_serialize ();
    hb_بایتها_t بایتها = s.copy_بایتها ();
    ثابت خودکار &store = *reinterpret_cast<ثابت OT::قلم_Variation_Store *> (بایتها.arrayZ);
    ثابت خودکار &داده = store.get_sub_جدول (0);
    hb_always_assert (داده.get_قلم_delta (0, 0) == delta);

    hb_inc_bimap_t inner_map, region_map;
    inner_map.جمع_کن (0);
    region_map.جمع_کن (0);
    حرف subset_buf[32];
    hb_serialize_context_t subset_s (subset_buf, حجم (subset_buf));
    خودکار *subset = subset_s.شروع_serialize<OT::Var_داده> ();
    hb_always_assert (subset->serialize (&subset_s, &داده, inner_map, region_map));
    subset_s.انتها_serialize ();
    hb_بایتها_t subset_بایتها = subset_s.copy_بایتها ();
    ثابت خودکار &subset_داده = *reinterpret_cast<ثابت OT::Var_داده *> (subset_بایتها.arrayZ);
    hb_always_assert (subset_داده.get_قلم_delta (0, 0) == delta);

    OT::قلم_variations_t قلم_vars;
    hb_always_assert (قلم_vars.create_from_قلم_varstore (store, axis_map));
    hb_hashmap_t<hb_برچسب_t, Triple> locations;
    hb_hashmap_t<hb_برچسب_t, TripleDistances> distances;
    hb_always_assert (قلم_vars.instantiate_tuple_vars (locations, distances));
    hb_always_assert (قلم_vars.as_قلم_varstore ());
    hb_always_assert (قلم_vars.has_long_کلمه () ==
                      (delta < -32768 || delta > 32767));
    subset_بایتها.fini ();
    بایتها.fini ();
  }
}

ساكن فراغ
test_accumulated_delta_widths ()
{
  ثابت صحيح deltas[] = {20000, -20000, 1073741824};
  لكل (صحيح delta : deltas)
  {
    OT::قلم_variations_t قلم_vars;
    بیعلامت outer = قلم_vars.جمع_کن_vardata (1);
    قلم_vars.جمع_کن_tuple (outer, hb_hashmap_t<hb_برچسب_t, Triple> (), 0, delta, 1);
    قلم_vars.جمع_کن_tuple (outer, hb_hashmap_t<hb_برچسب_t, Triple> (), 0, delta, 1);
    hb_always_assert (قلم_vars.build_region_list ());
    إذا (delta == 1073741824)
      hb_always_assert (!قلم_vars.as_قلم_varstore ());
    وإلا
    {
      hb_always_assert (قلم_vars.as_قلم_varstore ());
      hb_always_assert (قلم_vars.has_long_کلمه ());
      hb_always_assert ((*قلم_vars.get_vardata_encodings ()[0].اقلام[0])[0] == 2 * delta);
    }
  }
}

ساكن فراغ
test_condition_delta_precision ()
{
  hb_وکتور_t<hb_برچسب_t> axes;
  axes.push (HB_برچسب ('T','E','S','T'));
  hb_hashmap_t<hb_برچسب_t, Triple> neutral, region;
  region.مجموعه (axes[0], Triple (0, 1, 1));
  hb_وکتور_t<ثابت hb_hashmap_t<hb_برچسب_t, Triple> *> regions;
  regions.push (&neutral);
  regions.push (&region);
  hb_وکتور_t<صحيح> ردیف;
  ردیف.push (16777217);
  ردیف.push (-16777216);
  hb_وکتور_t<ثابت hb_وکتور_t<صحيح> *> ردیفها;
  ردیفها.push (&ردیف);
  hb_وکتور_t<OT::delta_ردیف_encoding_t> encodings;
  encodings.push (OT::delta_ردیف_encoding_t (std::move (ردیفها), 2));
  حرف buf[128];
  hb_serialize_context_t s (buf, حجم (buf));
  خودکار *بیرون = s.شروع_serialize<OT::قلم_Variation_Store> ();
  hb_always_assert (بیرون->serialize (&s, درست, axes, regions, encodings));
  s.انتها_serialize ();
  hb_بایتها_t بایتها = s.copy_بایتها ();
  ثابت خودکار &store = *reinterpret_cast<ثابت OT::قلم_Variation_Store *> (بایتها.arrayZ);
  صحيح coords[] = {16384};
  OT::قلم_Var_Store_Instancer instancer (&store, اشاره_تهی, hb_array (coords));
  // Positioning keeps float arithmetic, but conditions must retain the unit
  // left after cancellation of two deltas exceeding float's integer range.
  hb_always_assert (instancer[0] == 0.f);
  hb_always_assert (instancer.get_condition_delta (0) == 1.);
  ثابت حرف condition_داده[] = "\x00\x02\x00\x00\x00\x00\x00\x00";
  ثابت خودکار &condition = *reinterpret_cast<ثابت OT::Condition *> (condition_داده);
  hb_always_assert (condition.evaluate (coords, 1, &instancer));
  بایتها.fini ();
}

ساكن فراغ
test_failed_encoding ()
{
  hb_وکتور_t<صحيح> ردیف;
  ردیف.push (1);
  hb_وکتور_t<ثابت hb_وکتور_t<صحيح> *> ردیفها;
  ردیفها.push (&ردیف);
  OT::قلم_variations_t قلم_vars;
  hb_always_assert (قلم_vars.جمع_کن_vardata_encoding_for_testing (std::move (ردیفها), 1));
  خودکار &encoding = const_cast<OT::delta_ردیف_encoding_t &> (قلم_vars.get_vardata_encodings ()[0]);
  /* Simulate a failed allocation of the encoding's column metadata. */
  encoding.chars.clear ();
  encoding.chars.مجموعه_error ();
  hb_always_assert (encoding.درون_error ());
  hb_hashmap_t<بیعلامت, ثابت hb_وکتور_t<صحيح> *> front_mapping;
  front_mapping.مجموعه (0, &ردیف);
  hb_always_assert (!قلم_vars.compile_varidx_map_for_testing (front_mapping));
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  test_scalar_cache_precision ();
  test_قلم_variations ();
  test_implicit_advance_mapping ();
  test_قلم_variations_سرریز ();
  test_signed_delta_widths ();
  test_failed_encoding ();
  test_accumulated_delta_widths ();
  test_condition_delta_precision ();
}

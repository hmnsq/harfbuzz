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
#شامل "hb-ot-var-common.hh"
#شامل "hb-ot-var-hvar-table.hh"
// HVAR table data from SourceSerif4Variable-Roman_subset.otf
ثابت حرف hvar_ڈیٹا[] = "\x0\x1\x0\x0\x0\x0\x0\x14\x0\x0\x0\xc4\x0\x0\x0\x0\x0\x0\x0\x0\x0\x1\x0\x0\x0\x10\x0\x2\x0\x0\x0\x74\x0\x0\x0\x7a\x0\x2\x0\x8\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\x0\x1\x0\x0\x0\x0\x0\x4\x0\x0\x0\x8\x0\x0\x0\x1\x0\x2\x0\x3\x0\x4\x0\x5\x0\x6\x0\x7\xf9\xf\x2f\xbf\xfb\xfb\x35\xf9\x4\x4\xf3\xb4\xf2\xfb\x2e\xf3\x4\x4\xe\xad\xfa\x1\x1a\x1\x15\x22\x59\xd6\xe3\xf6\x6\xf5\x0\x1\x0\x5\x0\x4\x7\x5\x6";

ساکن خالی
test_scalar_cache_precision ()
{
  ثابت اعشاری scalars[] = {
    0.f,
    1.f,
    1.f / 3.f,
    /* Three variation axes can produce a scalar below the old cache's
     * fixed-point precision. */
    1.f / (16384.f * 16384.f * 16384.f),
  };
  ثابت غیرعلامتی counts[] = {4, 129};
  OT::hb_scalar_cache_t scratch_cache;
  برائے (غیرعلامتی گنتی : counts)
  {
    خودکار *cache = OT::hb_scalar_cache_t::create (گنتی, &scratch_cache);
    hb_always_assert (cache->لمبائی == گنتی);
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      اعشاری value;
      اعشاری scalar = scalars[i % ARRAY_لمبائی (scalars)];
      hb_always_assert (!cache->get (i, &value));
      cache->سیٹ (i, scalar);
      hb_always_assert (cache->get (i, &value));
      hb_always_assert (value == scalar);
    }
    cache->clear ();
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      اعشاری value;
      hb_always_assert (!cache->get (i, &value));
    }
    OT::hb_scalar_cache_t::destroy (cache, &scratch_cache);
  }
}

ساکن خالی
test_شے_variations ()
{
  ثابت OT::HVAR* hvar_جدول = reinterpret_cast<ثابت OT::HVAR*> (hvar_ڈیٹا);

  hb_لیبل_t axis_لیبل_2 = HB_لیبل ('w', 'g', 'h', 't');
  hb_map_t axis_idx_لیبل_map;
  axis_idx_لیبل_map.سیٹ (0, axis_لیبل_2);

  axis_لیبل_2 = HB_لیبل ('o', 'p', 's', 'z');
  axis_idx_لیبل_map.سیٹ (1, axis_لیبل_2);

  OT::شے_variations_t شے_vars;
  ثابت OT::شے_Variation_Store& src_var_store = hvar_جدول+(hvar_جدول->varStore);
  منطقی نتیجہ = شے_vars.create_from_شے_varstore (src_var_store, axis_idx_لیبل_map);
      
  hb_always_assert (نتیجہ);

  /* partial instancing wght=300:800 */
  hb_hashmap_t<hb_لیبل_t, Triple> normalized_axes_location;
  normalized_axes_location.سیٹ (axis_لیبل_2, Triple (-0.512817, 0.0, 0.7000120));

  hb_hashmap_t<hb_لیبل_t, TripleDistances> axes_triple_distances;
  axes_triple_distances.سیٹ (axis_لیبل_2, TripleDistances (200.0, 500.0));

  نتیجہ = شے_vars.instantiate_tuple_vars (normalized_axes_location, axes_triple_distances);
  hb_always_assert (نتیجہ);
  نتیجہ = شے_vars.as_شے_varstore (جھوٹ);
  hb_always_assert (نتیجہ);
  hb_always_assert (شے_vars.get_region_list().لمبائی == 8);
}

ساکن خالی
test_implicit_advance_mapping ()
{
  حرف ڈیٹا[حجم (hvar_ڈیٹا)];
  hb_memcpy (ڈیٹا, hvar_ڈیٹا, حجم (ڈیٹا));
  خودکار *hvar = reinterpret_cast<OT::HVAR *> (ڈیٹا);
  ثابت خودکار &store = hvar + hvar->varStore;
  ثابت صحیح locations[] = {-16384, 0, 16384};
  منطقی exercised_outer_one = جھوٹ;

  برائے (صحیح وزن : locations)
    برائے (صحیح سائز : locations)
    {
      صحیح coords[] = {وزن, سائز};
      غیرعلامتی mapped = (hvar + hvar->advMap).map (0x10000u);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) ==
                        store.get_delta (mapped, coords, 2));
    }

  hvar->advMap = 0;
  برائے (صحیح وزن : locations)
    برائے (صحیح سائز : locations)
    {
      صحیح coords[] = {وزن, سائز};
      exercised_outer_one |= store.get_delta (0x10000u, coords, 2) != 0;
      hb_always_assert (hvar->get_advance_delta_unscaled (0, coords, 2) ==
                        store.get_delta (0, coords, 2));
      hb_always_assert (hvar->get_advance_delta_unscaled (0xFFFFu, coords, 2) == 0);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) == 0);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10001u, coords, 2) == 0);
    }
  hb_always_assert (exercised_outer_one);
}

ساکن خالی
test_شے_variations_حد_سے_تجاوز ()
{
  hb_سمتیہ_t<hb_سمتیہ_t<صحیح>> صف_storage;
  hb_hashmap_t<غیرعلامتی, ثابت hb_سمتیہ_t<صحیح>*> front_mapping;
  OT::شے_variations_t شے_vars;
  غیرعلامتی old_varidx = 0;
  ثابت غیرعلامتی کل_گنتی = 0xFFFFu * 2u;

  ثابت غیرعلامتی counts[] = {
    128u,                 /* 0..127: 1-byte deltas */
    32768u - 128u,        /* 128..32767: 2-byte deltas */
    0xFFFFu * 2u - 32768u /* 32768..131069: 4-byte deltas */
  };
  ثابت صحیح starts[] = {
    0,
    128,
    32768
  };

  hb_always_assert (صف_storage.alloc (کل_گنتی));

  برائے (غیرعلامتی بالٹی = 0; بالٹی < 3; بالٹی++)
  {
    hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح>*> صفیں;
    hb_always_assert (صفیں.alloc (counts[بالٹی]));

    برائے (غیرعلامتی i = 0; i < counts[بالٹی]; i++)
    {
      hb_سمتیہ_t<صحیح> صف;
      hb_always_assert (صف.resize (1));
      صف[0] = starts[بالٹی] + i;
      صف_storage.push (std::move (صف));
    }

    hb_always_assert (!صف_storage.اندر_error ());

    غیرعلامتی بنیاد = صف_storage.لمبائی - counts[بالٹی];
    برائے (غیرعلامتی i = 0; i < counts[بالٹی]; i++)
    {
      ثابت hb_سمتیہ_t<صحیح> *صف = &صف_storage.arrayZ[بنیاد + i];
      صفیں.push (صف);
      front_mapping.سیٹ (old_varidx++, صف);
    }

    hb_always_assert (!صفیں.اندر_error ());
    hb_always_assert (شے_vars.جوڑو_vardata_encoding_for_testing (std::move (صفیں), 1));
  }

  hb_always_assert (!front_mapping.اندر_error ());
  hb_always_assert (شے_vars.compile_varidx_map_for_testing (front_mapping));

  ثابت خودکار& encodings = شے_vars.get_vardata_encodings ();
  hb_always_assert (encodings.لمبائی == 4);
  hb_always_assert (encodings[0].اشیاء.لمبائی == 128u);
  hb_always_assert (encodings[1].اشیاء.لمبائی == 32768u - 128u);
  hb_always_assert (encodings[2].اشیاء.لمبائی == 0xFFFFu);
  hb_always_assert (encodings[3].اشیاء.لمبائی == 32767u);

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

ساکن خالی
test_signed_delta_widths ()
{
  ثابت صحیح deltas[] = {32767, 32768, 65535, 65536,
                       -32768, -32769, -65536, -65537};
  hb_لیبل_t axis = HB_لیبل ('T', 'E', 'S', 'T');
  hb_سمتیہ_t<hb_لیبل_t> axes;
  axes.push (axis);
  hb_map_t axis_map;
  axis_map.سیٹ (0, axis);
  hb_hashmap_t<hb_لیبل_t, Triple> region;
  region.سیٹ (axis, Triple (0, 1, 1));
  hb_سمتیہ_t<ثابت hb_hashmap_t<hb_لیبل_t, Triple> *> regions;
  regions.push (&region);

  برائے (صحیح delta : deltas)
  {
    hb_سمتیہ_t<صحیح> صف;
    صف.push (delta);
    hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح> *> صفیں;
    صفیں.push (&صف);
    hb_سمتیہ_t<OT::delta_صف_encoding_t> encodings;
    encodings.push (OT::delta_صف_encoding_t (std::move (صفیں), 1));

    حرف buf[128];
    hb_serialize_context_t s (buf, حجم (buf));
    خودکار *برآمد = s.آغاز_serialize<OT::شے_Variation_Store> ();
    hb_always_assert (برآمد->serialize (&s, سچ, axes, regions, encodings));
    s.انتہا_serialize ();
    hb_بائٹیں_t بائٹیں = s.copy_بائٹیں ();
    ثابت خودکار &store = *reinterpret_cast<ثابت OT::شے_Variation_Store *> (بائٹیں.arrayZ);
    ثابت خودکار &ڈیٹا = store.get_sub_جدول (0);
    hb_always_assert (ڈیٹا.get_شے_delta (0, 0) == delta);

    hb_inc_bimap_t inner_map, region_map;
    inner_map.جوڑو (0);
    region_map.جوڑو (0);
    حرف subset_buf[32];
    hb_serialize_context_t subset_s (subset_buf, حجم (subset_buf));
    خودکار *subset = subset_s.آغاز_serialize<OT::Var_ڈیٹا> ();
    hb_always_assert (subset->serialize (&subset_s, &ڈیٹا, inner_map, region_map));
    subset_s.انتہا_serialize ();
    hb_بائٹیں_t subset_بائٹیں = subset_s.copy_بائٹیں ();
    ثابت خودکار &subset_ڈیٹا = *reinterpret_cast<ثابت OT::Var_ڈیٹا *> (subset_بائٹیں.arrayZ);
    hb_always_assert (subset_ڈیٹا.get_شے_delta (0, 0) == delta);

    OT::شے_variations_t شے_vars;
    hb_always_assert (شے_vars.create_from_شے_varstore (store, axis_map));
    hb_hashmap_t<hb_لیبل_t, Triple> locations;
    hb_hashmap_t<hb_لیبل_t, TripleDistances> distances;
    hb_always_assert (شے_vars.instantiate_tuple_vars (locations, distances));
    hb_always_assert (شے_vars.as_شے_varstore ());
    hb_always_assert (شے_vars.has_long_لفظ () ==
                      (delta < -32768 || delta > 32767));
    subset_بائٹیں.fini ();
    بائٹیں.fini ();
  }
}

ساکن خالی
test_accumulated_delta_widths ()
{
  ثابت صحیح deltas[] = {20000, -20000, 1073741824};
  برائے (صحیح delta : deltas)
  {
    OT::شے_variations_t شے_vars;
    غیرعلامتی outer = شے_vars.جوڑو_vardata (1);
    شے_vars.جوڑو_tuple (outer, hb_hashmap_t<hb_لیبل_t, Triple> (), 0, delta, 1);
    شے_vars.جوڑو_tuple (outer, hb_hashmap_t<hb_لیبل_t, Triple> (), 0, delta, 1);
    hb_always_assert (شے_vars.build_region_list ());
    اگر (delta == 1073741824)
      hb_always_assert (!شے_vars.as_شے_varstore ());
    ورنہ
    {
      hb_always_assert (شے_vars.as_شے_varstore ());
      hb_always_assert (شے_vars.has_long_لفظ ());
      hb_always_assert ((*شے_vars.get_vardata_encodings ()[0].اشیاء[0])[0] == 2 * delta);
    }
  }
}

ساکن خالی
test_condition_delta_precision ()
{
  hb_سمتیہ_t<hb_لیبل_t> axes;
  axes.push (HB_لیبل ('T','E','S','T'));
  hb_hashmap_t<hb_لیبل_t, Triple> neutral, region;
  region.سیٹ (axes[0], Triple (0, 1, 1));
  hb_سمتیہ_t<ثابت hb_hashmap_t<hb_لیبل_t, Triple> *> regions;
  regions.push (&neutral);
  regions.push (&region);
  hb_سمتیہ_t<صحیح> صف;
  صف.push (16777217);
  صف.push (-16777216);
  hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح> *> صفیں;
  صفیں.push (&صف);
  hb_سمتیہ_t<OT::delta_صف_encoding_t> encodings;
  encodings.push (OT::delta_صف_encoding_t (std::move (صفیں), 2));
  حرف buf[128];
  hb_serialize_context_t s (buf, حجم (buf));
  خودکار *برآمد = s.آغاز_serialize<OT::شے_Variation_Store> ();
  hb_always_assert (برآمد->serialize (&s, سچ, axes, regions, encodings));
  s.انتہا_serialize ();
  hb_بائٹیں_t بائٹیں = s.copy_بائٹیں ();
  ثابت خودکار &store = *reinterpret_cast<ثابت OT::شے_Variation_Store *> (بائٹیں.arrayZ);
  صحیح coords[] = {16384};
  OT::شے_Var_Store_Instancer instancer (&store, خالی_اشاریہ, hb_array (coords));
  // Positioning keeps float arithmetic, but conditions must retain the unit
  // left after cancellation of two deltas exceeding float's integer range.
  hb_always_assert (instancer[0] == 0.f);
  hb_always_assert (instancer.get_condition_delta (0) == 1.);
  ثابت حرف condition_ڈیٹا[] = "\x00\x02\x00\x00\x00\x00\x00\x00";
  ثابت خودکار &condition = *reinterpret_cast<ثابت OT::Condition *> (condition_ڈیٹا);
  hb_always_assert (condition.evaluate (coords, 1, &instancer));
  بائٹیں.fini ();
}

ساکن خالی
test_failed_encoding ()
{
  hb_سمتیہ_t<صحیح> صف;
  صف.push (1);
  hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح> *> صفیں;
  صفیں.push (&صف);
  OT::شے_variations_t شے_vars;
  hb_always_assert (شے_vars.جوڑو_vardata_encoding_for_testing (std::move (صفیں), 1));
  خودکار &encoding = const_cast<OT::delta_صف_encoding_t &> (شے_vars.get_vardata_encodings ()[0]);
  /* Simulate a failed allocation of the encoding's column metadata. */
  encoding.chars.clear ();
  encoding.chars.سیٹ_error ();
  hb_always_assert (encoding.اندر_error ());
  hb_hashmap_t<غیرعلامتی, ثابت hb_سمتیہ_t<صحیح> *> front_mapping;
  front_mapping.سیٹ (0, &صف);
  hb_always_assert (!شے_vars.compile_varidx_map_for_testing (front_mapping));
}

صحیح
مین (صحیح argc, حرف **argv)
{
  test_scalar_cache_precision ();
  test_شے_variations ();
  test_implicit_advance_mapping ();
  test_شے_variations_حد_سے_تجاوز ();
  test_signed_delta_widths ();
  test_failed_encoding ();
  test_accumulated_delta_widths ();
  test_condition_delta_precision ();
}

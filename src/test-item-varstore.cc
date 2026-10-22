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
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-hvar-table.hh"
// HVAR table data from SourceSerif4Variable-Roman_subset.otf
ਅਚਲ ਅੱਖਰ hvar_ਅੰਕੜੇ[] = "\x0\x1\x0\x0\x0\x0\x0\x14\x0\x0\x0\xc4\x0\x0\x0\x0\x0\x0\x0\x0\x0\x1\x0\x0\x0\x10\x0\x2\x0\x0\x0\x74\x0\x0\x0\x7a\x0\x2\x0\x8\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\xc0\x0\xc0\x0\x0\x0\x0\x0\x40\x0\x40\x0\x0\x0\x40\x0\x40\x0\x0\x1\x0\x0\x0\x0\x0\x4\x0\x0\x0\x8\x0\x0\x0\x1\x0\x2\x0\x3\x0\x4\x0\x5\x0\x6\x0\x7\xf9\xf\x2f\xbf\xfb\xfb\x35\xf9\x4\x4\xf3\xb4\xf2\xfb\x2e\xf3\x4\x4\xe\xad\xfa\x1\x1a\x1\x15\x22\x59\xd6\xe3\xf6\x6\xf5\x0\x1\x0\x5\x0\x4\x7\x5\x6";

ਸਥਿਰ ਖਾਲੀ
test_scalar_cache_precision ()
{
  ਅਚਲ ਦਸ਼ਮਲਵ scalars[] = {
    0.f,
    1.f,
    1.f / 3.f,
    /* Three variation axes can produce a scalar below the old cache's
     * fixed-point precision. */
    1.f / (16384.f * 16384.f * 16384.f),
  };
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ counts[] = {4, 129};
  OT::hb_scalar_cache_t scratch_cache;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 : counts)
  {
    ਸਵੈਚਾਲਿਤ *cache = OT::hb_scalar_cache_t::create (ਗਿਣਤੀ_2, &scratch_cache);
    hb_always_assert (cache->ਲੰਮਾਈ == ਗਿਣਤੀ_2);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਦਸ਼ਮਲਵ value;
      ਦਸ਼ਮਲਵ scalar = scalars[i % ARRAY_ਲੰਮਾਈ (scalars)];
      hb_always_assert (!cache->get (i, &value));
      cache->ਸੈੱਟ (i, scalar);
      hb_always_assert (cache->get (i, &value));
      hb_always_assert (value == scalar);
    }
    cache->clear ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਦਸ਼ਮਲਵ value;
      hb_always_assert (!cache->get (i, &value));
    }
    OT::hb_scalar_cache_t::destroy (cache, &scratch_cache);
  }
}

ਸਥਿਰ ਖਾਲੀ
test_ਵਸਤੂ_variations ()
{
  ਅਚਲ OT::HVAR* hvar_ਸਾਰਣੀ = reinterpret_cast<ਅਚਲ OT::HVAR*> (hvar_ਅੰਕੜੇ);

  hb_ਟੈਗ_t axis_ਟੈਗ_2 = HB_ਟੈਗ ('w', 'g', 'h', 't');
  hb_map_t axis_idx_ਟੈਗ_map;
  axis_idx_ਟੈਗ_map.ਸੈੱਟ (0, axis_ਟੈਗ_2);

  axis_ਟੈਗ_2 = HB_ਟੈਗ ('o', 'p', 's', 'z');
  axis_idx_ਟੈਗ_map.ਸੈੱਟ (1, axis_ਟੈਗ_2);

  OT::ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
  ਅਚਲ OT::ਵਸਤੂ_Variation_Store& src_var_store = hvar_ਸਾਰਣੀ+(hvar_ਸਾਰਣੀ->varStore);
  ਤਾਰਕਿਕ ਨਤੀਜਾ = ਵਸਤੂ_vars.create_from_ਵਸਤੂ_varstore (src_var_store, axis_idx_ਟੈਗ_map);
      
  hb_always_assert (ਨਤੀਜਾ);

  /* partial instancing wght=300:800 */
  hb_hashmap_t<hb_ਟੈਗ_t, Triple> normalized_axes_location;
  normalized_axes_location.ਸੈੱਟ (axis_ਟੈਗ_2, Triple (-0.512817, 0.0, 0.7000120));

  hb_hashmap_t<hb_ਟੈਗ_t, TripleDistances> axes_triple_distances;
  axes_triple_distances.ਸੈੱਟ (axis_ਟੈਗ_2, TripleDistances (200.0, 500.0));

  ਨਤੀਜਾ = ਵਸਤੂ_vars.instantiate_tuple_vars (normalized_axes_location, axes_triple_distances);
  hb_always_assert (ਨਤੀਜਾ);
  ਨਤੀਜਾ = ਵਸਤੂ_vars.as_ਵਸਤੂ_varstore (ਝੂਠ);
  hb_always_assert (ਨਤੀਜਾ);
  hb_always_assert (ਵਸਤੂ_vars.get_region_list().ਲੰਮਾਈ == 8);
}

ਸਥਿਰ ਖਾਲੀ
test_implicit_advance_mapping ()
{
  ਅੱਖਰ ਅੰਕੜੇ[ਆਕਾਰ (hvar_ਅੰਕੜੇ)];
  hb_memcpy (ਅੰਕੜੇ, hvar_ਅੰਕੜੇ, ਆਕਾਰ (ਅੰਕੜੇ));
  ਸਵੈਚਾਲਿਤ *hvar = reinterpret_cast<OT::HVAR *> (ਅੰਕੜੇ);
  ਅਚਲ ਸਵੈਚਾਲਿਤ &store = hvar + hvar->varStore;
  ਅਚਲ ਪੂਰਨ locations[] = {-16384, 0, 16384};
  ਤਾਰਕਿਕ exercised_outer_one = ਝੂਠ;

  ਲਈ (ਪੂਰਨ ਭਾਰ : locations)
    ਲਈ (ਪੂਰਨ ਮਾਪ : locations)
    {
      ਪੂਰਨ coords[] = {ਭਾਰ, ਮਾਪ};
      ਬਿਨਾਨਿਸ਼ਾਨੀ mapped = (hvar + hvar->advMap).map (0x10000u);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) ==
                        store.get_delta (mapped, coords, 2));
    }

  hvar->advMap = 0;
  ਲਈ (ਪੂਰਨ ਭਾਰ : locations)
    ਲਈ (ਪੂਰਨ ਮਾਪ : locations)
    {
      ਪੂਰਨ coords[] = {ਭਾਰ, ਮਾਪ};
      exercised_outer_one |= store.get_delta (0x10000u, coords, 2) != 0;
      hb_always_assert (hvar->get_advance_delta_unscaled (0, coords, 2) ==
                        store.get_delta (0, coords, 2));
      hb_always_assert (hvar->get_advance_delta_unscaled (0xFFFFu, coords, 2) == 0);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10000u, coords, 2) == 0);
      hb_always_assert (hvar->get_advance_delta_unscaled (0x10001u, coords, 2) == 0);
    }
  hb_always_assert (exercised_outer_one);
}

ਸਥਿਰ ਖਾਲੀ
test_ਵਸਤੂ_variations_ਉਛਾਲ ()
{
  hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਪੂਰਨ>> ਪੰਕਤੀ_storage;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*> front_mapping;
  OT::ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
  ਬਿਨਾਨਿਸ਼ਾਨੀ old_varidx = 0;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਗਿਣਤੀ = 0xFFFFu * 2u;

  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ counts[] = {
    128u,                 /* 0..127: 1-byte deltas */
    32768u - 128u,        /* 128..32767: 2-byte deltas */
    0xFFFFu * 2u - 32768u /* 32768..131069: 4-byte deltas */
  };
  ਅਚਲ ਪੂਰਨ starts[] = {
    0,
    128,
    32768
  };

  hb_always_assert (ਪੰਕਤੀ_storage.alloc (ਕੁੱਲ_ਗਿਣਤੀ));

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਲਟੀ = 0; ਬਾਲਟੀ < 3; ਬਾਲਟੀ++)
  {
    hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*> ਪੰਕਤੀਆਂ;
    hb_always_assert (ਪੰਕਤੀਆਂ.alloc (counts[ਬਾਲਟੀ]));

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < counts[ਬਾਲਟੀ]; i++)
    {
      hb_ਸਦਿਸ਼_t<ਪੂਰਨ> ਪੰਕਤੀ;
      hb_always_assert (ਪੰਕਤੀ.resize (1));
      ਪੰਕਤੀ[0] = starts[ਬਾਲਟੀ] + i;
      ਪੰਕਤੀ_storage.push (std::move (ਪੰਕਤੀ));
    }

    hb_always_assert (!ਪੰਕਤੀ_storage.ਵਿੱਚ_error ());

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ = ਪੰਕਤੀ_storage.ਲੰਮਾਈ - counts[ਬਾਲਟੀ];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < counts[ਬਾਲਟੀ]; i++)
    {
      ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ> *ਪੰਕਤੀ = &ਪੰਕਤੀ_storage.arrayZ[ਅਧਾਰ + i];
      ਪੰਕਤੀਆਂ.push (ਪੰਕਤੀ);
      front_mapping.ਸੈੱਟ (old_varidx++, ਪੰਕਤੀ);
    }

    hb_always_assert (!ਪੰਕਤੀਆਂ.ਵਿੱਚ_error ());
    hb_always_assert (ਵਸਤੂ_vars.ਜੋੜੋ_vardata_encoding_for_testing (std::move (ਪੰਕਤੀਆਂ), 1));
  }

  hb_always_assert (!front_mapping.ਵਿੱਚ_error ());
  hb_always_assert (ਵਸਤੂ_vars.compile_varidx_map_for_testing (front_mapping));

  ਅਚਲ ਸਵੈਚਾਲਿਤ& encodings = ਵਸਤੂ_vars.get_vardata_encodings ();
  hb_always_assert (encodings.ਲੰਮਾਈ == 4);
  hb_always_assert (encodings[0].ਚੀਜ਼ਾਂ.ਲੰਮਾਈ == 128u);
  hb_always_assert (encodings[1].ਚੀਜ਼ਾਂ.ਲੰਮਾਈ == 32768u - 128u);
  hb_always_assert (encodings[2].ਚੀਜ਼ਾਂ.ਲੰਮਾਈ == 0xFFFFu);
  hb_always_assert (encodings[3].ਚੀਜ਼ਾਂ.ਲੰਮਾਈ == 32767u);

  ਅਚਲ hb_map_t& varidx_map = ਵਸਤੂ_vars.get_varidx_map ();
  hb_always_assert (varidx_map.get (0) == 0);
  hb_always_assert (varidx_map.get (127) == 127);
  hb_always_assert (varidx_map.get (128) == 0x00010000u);
  hb_always_assert (varidx_map.get (32767) == 0x00017F7Fu);
  hb_always_assert (varidx_map.get (32768) == 0x00020000u);
  hb_always_assert (varidx_map.get (98302) == 0x0002FFFEu);
  hb_always_assert (varidx_map.get (98303) == 0x00030000u);
  hb_always_assert (varidx_map.get (131069) == 0x00037FFEu);
}

ਸਥਿਰ ਖਾਲੀ
test_signed_delta_widths ()
{
  ਅਚਲ ਪੂਰਨ deltas[] = {32767, 32768, 65535, 65536,
                       -32768, -32769, -65536, -65537};
  hb_ਟੈਗ_t axis = HB_ਟੈਗ ('T', 'E', 'S', 'T');
  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> axes;
  axes.push (axis);
  hb_map_t axis_map;
  axis_map.ਸੈੱਟ (0, axis);
  hb_hashmap_t<hb_ਟੈਗ_t, Triple> region;
  region.ਸੈੱਟ (axis, Triple (0, 1, 1));
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *> regions;
  regions.push (&region);

  ਲਈ (ਪੂਰਨ delta : deltas)
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> ਪੰਕਤੀ;
    ਪੰਕਤੀ.push (delta);
    hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ> *> ਪੰਕਤੀਆਂ;
    ਪੰਕਤੀਆਂ.push (&ਪੰਕਤੀ);
    hb_ਸਦਿਸ਼_t<OT::delta_ਪੰਕਤੀ_encoding_t> encodings;
    encodings.push (OT::delta_ਪੰਕਤੀ_encoding_t (std::move (ਪੰਕਤੀਆਂ), 1));

    ਅੱਖਰ buf[128];
    hb_serialize_context_t s (buf, ਆਕਾਰ (buf));
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = s.ਸ਼ੁਰੂ_serialize<OT::ਵਸਤੂ_Variation_Store> ();
    hb_always_assert (ਬਾਹਰਲਾ->serialize (&s, ਸੱਚ, axes, regions, encodings));
    s.ਸਿਰਾ_serialize ();
    hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ = s.copy_ਬਾਈਟਾਂ ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &store = *reinterpret_cast<ਅਚਲ OT::ਵਸਤੂ_Variation_Store *> (ਬਾਈਟਾਂ.arrayZ);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਅੰਕੜੇ = store.get_sub_ਸਾਰਣੀ (0);
    hb_always_assert (ਅੰਕੜੇ.get_ਵਸਤੂ_delta (0, 0) == delta);

    hb_inc_bimap_t inner_map, region_map;
    inner_map.ਜੋੜੋ (0);
    region_map.ਜੋੜੋ (0);
    ਅੱਖਰ subset_buf[32];
    hb_serialize_context_t subset_s (subset_buf, ਆਕਾਰ (subset_buf));
    ਸਵੈਚਾਲਿਤ *subset = subset_s.ਸ਼ੁਰੂ_serialize<OT::Var_ਅੰਕੜੇ> ();
    hb_always_assert (subset->serialize (&subset_s, &ਅੰਕੜੇ, inner_map, region_map));
    subset_s.ਸਿਰਾ_serialize ();
    hb_ਬਾਈਟਾਂ_t subset_ਬਾਈਟਾਂ = subset_s.copy_ਬਾਈਟਾਂ ();
    ਅਚਲ ਸਵੈਚਾਲਿਤ &subset_ਅੰਕੜੇ = *reinterpret_cast<ਅਚਲ OT::Var_ਅੰਕੜੇ *> (subset_ਬਾਈਟਾਂ.arrayZ);
    hb_always_assert (subset_ਅੰਕੜੇ.get_ਵਸਤੂ_delta (0, 0) == delta);

    OT::ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
    hb_always_assert (ਵਸਤੂ_vars.create_from_ਵਸਤੂ_varstore (store, axis_map));
    hb_hashmap_t<hb_ਟੈਗ_t, Triple> locations;
    hb_hashmap_t<hb_ਟੈਗ_t, TripleDistances> distances;
    hb_always_assert (ਵਸਤੂ_vars.instantiate_tuple_vars (locations, distances));
    hb_always_assert (ਵਸਤੂ_vars.as_ਵਸਤੂ_varstore ());
    hb_always_assert (ਵਸਤੂ_vars.has_long_ਸ਼ਬਦ () ==
                      (delta < -32768 || delta > 32767));
    subset_ਬਾਈਟਾਂ.fini ();
    ਬਾਈਟਾਂ.fini ();
  }
}

ਸਥਿਰ ਖਾਲੀ
test_accumulated_delta_widths ()
{
  ਅਚਲ ਪੂਰਨ deltas[] = {20000, -20000, 1073741824};
  ਲਈ (ਪੂਰਨ delta : deltas)
  {
    OT::ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
    ਬਿਨਾਨਿਸ਼ਾਨੀ outer = ਵਸਤੂ_vars.ਜੋੜੋ_vardata (1);
    ਵਸਤੂ_vars.ਜੋੜੋ_tuple (outer, hb_hashmap_t<hb_ਟੈਗ_t, Triple> (), 0, delta, 1);
    ਵਸਤੂ_vars.ਜੋੜੋ_tuple (outer, hb_hashmap_t<hb_ਟੈਗ_t, Triple> (), 0, delta, 1);
    hb_always_assert (ਵਸਤੂ_vars.build_region_list ());
    ਜੇ (delta == 1073741824)
      hb_always_assert (!ਵਸਤੂ_vars.as_ਵਸਤੂ_varstore ());
    ਨਹੀਂਤਾਂ
    {
      hb_always_assert (ਵਸਤੂ_vars.as_ਵਸਤੂ_varstore ());
      hb_always_assert (ਵਸਤੂ_vars.has_long_ਸ਼ਬਦ ());
      hb_always_assert ((*ਵਸਤੂ_vars.get_vardata_encodings ()[0].ਚੀਜ਼ਾਂ[0])[0] == 2 * delta);
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
test_condition_delta_precision ()
{
  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> axes;
  axes.push (HB_ਟੈਗ ('T','E','S','T'));
  hb_hashmap_t<hb_ਟੈਗ_t, Triple> neutral, region;
  region.ਸੈੱਟ (axes[0], Triple (0, 1, 1));
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *> regions;
  regions.push (&neutral);
  regions.push (&region);
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> ਪੰਕਤੀ;
  ਪੰਕਤੀ.push (16777217);
  ਪੰਕਤੀ.push (-16777216);
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ> *> ਪੰਕਤੀਆਂ;
  ਪੰਕਤੀਆਂ.push (&ਪੰਕਤੀ);
  hb_ਸਦਿਸ਼_t<OT::delta_ਪੰਕਤੀ_encoding_t> encodings;
  encodings.push (OT::delta_ਪੰਕਤੀ_encoding_t (std::move (ਪੰਕਤੀਆਂ), 2));
  ਅੱਖਰ buf[128];
  hb_serialize_context_t s (buf, ਆਕਾਰ (buf));
  ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = s.ਸ਼ੁਰੂ_serialize<OT::ਵਸਤੂ_Variation_Store> ();
  hb_always_assert (ਬਾਹਰਲਾ->serialize (&s, ਸੱਚ, axes, regions, encodings));
  s.ਸਿਰਾ_serialize ();
  hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ = s.copy_ਬਾਈਟਾਂ ();
  ਅਚਲ ਸਵੈਚਾਲਿਤ &store = *reinterpret_cast<ਅਚਲ OT::ਵਸਤੂ_Variation_Store *> (ਬਾਈਟਾਂ.arrayZ);
  ਪੂਰਨ coords[] = {16384};
  OT::ਵਸਤੂ_Var_Store_Instancer instancer (&store, ਖਾਲੀ_ਸੂਚਕ, hb_array (coords));
  // Positioning keeps float arithmetic, but conditions must retain the unit
  // left after cancellation of two deltas exceeding float's integer range.
  hb_always_assert (instancer[0] == 0.f);
  hb_always_assert (instancer.get_condition_delta (0) == 1.);
  ਅਚਲ ਅੱਖਰ condition_ਅੰਕੜੇ[] = "\x00\x02\x00\x00\x00\x00\x00\x00";
  ਅਚਲ ਸਵੈਚਾਲਿਤ &condition = *reinterpret_cast<ਅਚਲ OT::Condition *> (condition_ਅੰਕੜੇ);
  hb_always_assert (condition.evaluate (coords, 1, &instancer));
  ਬਾਈਟਾਂ.fini ();
}

ਸਥਿਰ ਖਾਲੀ
test_failed_encoding ()
{
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> ਪੰਕਤੀ;
  ਪੰਕਤੀ.push (1);
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ> *> ਪੰਕਤੀਆਂ;
  ਪੰਕਤੀਆਂ.push (&ਪੰਕਤੀ);
  OT::ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
  hb_always_assert (ਵਸਤੂ_vars.ਜੋੜੋ_vardata_encoding_for_testing (std::move (ਪੰਕਤੀਆਂ), 1));
  ਸਵੈਚਾਲਿਤ &encoding = const_cast<OT::delta_ਪੰਕਤੀ_encoding_t &> (ਵਸਤੂ_vars.get_vardata_encodings ()[0]);
  /* Simulate a failed allocation of the encoding's column metadata. */
  encoding.chars.clear ();
  encoding.chars.ਸੈੱਟ_error ();
  hb_always_assert (encoding.ਵਿੱਚ_error ());
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ> *> front_mapping;
  front_mapping.ਸੈੱਟ (0, &ਪੰਕਤੀ);
  hb_always_assert (!ਵਸਤੂ_vars.compile_varidx_map_for_testing (front_mapping));
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  test_scalar_cache_precision ();
  test_ਵਸਤੂ_variations ();
  test_implicit_advance_mapping ();
  test_ਵਸਤੂ_variations_ਉਛਾਲ ();
  test_signed_delta_widths ();
  test_failed_encoding ();
  test_accumulated_delta_widths ();
  test_condition_delta_precision ();
}

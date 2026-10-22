/*
 * Copyright © 2022  Behdad Esfahbod
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

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-serialize.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-ot-layout-gsubgpos.hh"
#అన్తర్భావ "hb-ot-layout-gsub-table.hh"
#అన్తర్భావ "OT/Layout/GPOS/ValueFormat.hh"

ఉపయోగించి OT::Layout::Common::Coverage;

స్థిర శూన్య
test_null_condition ()
{
  స్థిర స్థిరరాశి అక్షరం and_condition[పరిమాణం (OT::Condition)] = {
    0, 3, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  స్థిర స్థిరరాశి అక్షరం or_condition[పరిమాణం (OT::Condition)] = {
    0, 4, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  స్థిర స్థిరరాశి అక్షరం negate_condition[పరిమాణం (OT::Condition)] = {
    0, 5,                        /* format */
    0, 0, 0,                     /* conditionOffset */
  };

  స్థిరరాశి స్వయంచాలక &condition = Null (OT::Condition);
  స్థిరరాశి స్వయంచాలక &condition_and =
      *reinterpret_cast<స్థిరరాశి OT::Condition *> (and_condition);
  స్థిరరాశి స్వయంచాలక &condition_or =
      *reinterpret_cast<స్థిరరాశి OT::Condition *> (or_condition);
  స్థిరరాశి స్వయంచాలక &condition_negate =
      *reinterpret_cast<స్థిరరాశి OT::Condition *> (negate_condition);
  OT::అంశం_Var_Store_Instancer *instancer = శూన్యసూచిక;
  hb_సమితి_t var_indices;

  hb_always_assert (condition.evaluate (శూన్యసూచిక, 0, instancer));
  hb_always_assert (condition_and.evaluate (శూన్యసూచిక, 0, instancer));
  hb_always_assert (condition_or.evaluate (శూన్యసూచిక, 0, instancer));
  hb_always_assert (!condition_negate.evaluate (శూన్యసూచిక, 0, instancer));

  hb_always_assert (condition.collect_var_indices (&var_indices));
  hb_always_assert (condition_and.collect_var_indices (&var_indices));
  hb_always_assert (condition_or.collect_var_indices (&var_indices));
  hb_always_assert (condition_negate.collect_var_indices (&var_indices));
  hb_always_assert (var_indices.is_empty ());

  అక్షరం buf[8];
  hb_serialize_context_t s (buf, పరిమాణం (buf));
  hb_map_t varidx_map;
  స్థిర స్థిరరాశి గుర్తులేని అక్షరం expected[] = {
    0, 2,                        /* format */
    0, 1,                        /* defaultValue */
    0xFF, 0xFF, 0xFF, 0xFF,     /* varIdx */
  };

  స్వయంచాలక *copy = s.ప్రారంభం_serialize<OT::Condition> ();
  hb_always_assert (copy->serialize (&s, &condition, varidx_map));
  s.చివర_serialize ();

  hb_బైట్లు_t బైట్లు = s.copy_బైట్లు ();
  hb_always_assert (బైట్లు.నిడివి == పరిమాణం (expected));
  hb_always_assert (!hb_memcmp (బైట్లు.arrayZ, expected, పరిమాణం (expected)));
  బైట్లు.fini ();
}

స్థిర శూన్య
test_lookup_condition_sharing ()
{
  స్థిరరాశి గుర్తులేని feature_లెక్క_2 = 10922, lookup_లెక్క = 402;
  స్థిరరాశి గుర్తులేని పట్టిక_స్థానభ్రంశం = 12 + 6 * feature_లెక్క_2;
  స్థిరరాశి గుర్తులేని condition_స్థానభ్రంశం = పట్టిక_స్థానభ్రంశం + 20 + 2 * lookup_లెక్క;
  hb_సదిశ_t<అక్షరం> దత్తాంశం;
  hb_always_assert (దత్తాంశం.resize (condition_స్థానభ్రంశం + 8));
  hb_memset (దత్తాంశం.arrayZ, 0, దత్తాంశం.నిడివి);
  స్వయంచాలక put16 = [&దత్తాంశం] (గుర్తులేని స్థానభ్రంశం, గుర్తులేని value)
  {
    దత్తాంశం[స్థానభ్రంశం] = value >> 8;
    దత్తాంశం[స్థానభ్రంశం + 1] = value;
  };
  స్వయంచాలక put32 = [&put16] (గుర్తులేని స్థానభ్రంశం, గుర్తులేని value)
  {
    put16 (స్థానభ్రంశం, value >> 16);
    put16 (స్థానభ్రంశం + 2, value);
  };
  put32 (0, 0x00010001);
  put32 (8, feature_లెక్క_2);
  కొరకు (గుర్తులేని i = 0; i < feature_లెక్క_2; i++)
  {
    put16 (12 + 6 * i, i);
    put32 (14 + 6 * i, పట్టిక_స్థానభ్రంశం);
  }
  put32 (పట్టిక_స్థానభ్రంశం, 0x00010000);
  put32 (పట్టిక_స్థానభ్రంశం + 6, 1); /* LookupConditionRecord count */
  put32 (పట్టిక_స్థానభ్రంశం + 10, condition_స్థానభ్రంశం - పట్టిక_స్థానభ్రంశం);
  put32 (పట్టిక_స్థానభ్రంశం + 14, 18); /* LookupIndexList offset */
  put16 (పట్టిక_స్థానభ్రంశం + 18, lookup_లెక్క);
  కొరకు (గుర్తులేని i = 0; i < lookup_లెక్క; i++)
    put16 (పట్టిక_స్థానభ్రంశం + 20 + 2 * i, i);
  put16 (condition_స్థానభ్రంశం, 2); /* Constant true ConditionValue */
  put16 (condition_స్థానభ్రంశం + 2, 1);
  put32 (condition_స్థానభ్రంశం + 4, OT::VarIdx::NO_VARIATION);

  స్థిరరాశి స్వయంచాలక &variations = *reinterpret_cast<స్థిరరాశి OT::FeatureVariations *> (దత్తాంశం.arrayZ);
  OT::అంశం_Var_Store_Instancer instancer (శూన్యసూచిక, శూన్యసూచిక, hb_array_t<స్థిరరాశి పూర్ణాంకం> ());
  hb_సదిశ_t<గుర్తులేని> state;
  hb_always_assert (variations.collect_lookup_conditions (శూన్యసూచిక, 0, &instancer, &state));
  hb_always_assert (state.నిడివి == 1 && state[0] == condition_స్థానభ్రంశం);

  కొరకు (గుర్తులేని feature_సూచీ : {0u, feature_లెక్క_2 - 1})
  {
    hb_సమితి_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        feature_సూచీ, Null (OT::Feature), state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == lookup_లెక్క);
    hb_always_assert (lookups.get_min () == 0 && lookups.get_max () == lookup_లెక్క - 1);
  }

  /* Null conditions need no state, even when thousands of records use them. */
  put32 (పట్టిక_స్థానభ్రంశం + 10, 0);
  state.reset ();
  hb_always_assert (variations.collect_lookup_conditions (శూన్యసూచిక, 0, &instancer, &state));
  hb_always_assert (!state.నిడివి);
  hb_సమితి_t lookups;
  hb_always_assert (variations.get_lookup_variations (
      0, Null (OT::Feature), state.as_sorted_array (), &lookups));
  hb_always_assert (lookups.get_population () == lookup_లెక్క);

  put32 (పట్టిక_స్థానభ్రంశం + 10, condition_స్థానభ్రంశం - పట్టిక_స్థానభ్రంశం);
  state.సమితి_error (); /* Simulate failure allocating the condition key. */
  hb_always_assert (!variations.collect_lookup_conditions (శూన్యసూచిక, 0, &instancer, &state));
}

స్థిర శూన్య
test_lookup_condition_resolution ()
{
  స్థిర స్థిరరాశి అక్షరం దత్తాంశం[] = {
    0, 1, 0, 1, 0, 0, 0, 0,     /* FeatureVariations 1.1, no 1.0 records */
    0, 0, 0, 2,                 /* LookupVariationRecord count */
    0, 0, 0, 0, 0, 24,          /* Feature 0 */
    0, 1, 0, 0, 0, 50,          /* Feature 1 */
    0, 1, 0, 0, 0, 1, 0, 0, 0, 2, /* ADD_DEFAULT_LOOKUPS, two conditions */
    0, 0, 0, 0, 0, 0, 0, 52,    /* Null condition, list A */
    0, 0, 0, 76, 0, 0, 0, 60,   /* Axis 0 condition, list B */
    0, 1, 0, 0, 0, 0, 0, 0, 0, 2, /* Two conditions, no default lookups */
    0, 0, 0, 42, 0, 0, 0, 34,   /* Axis 1 condition, list B */
    0, 0, 0, 50, 0, 0, 0, 26,   /* Shared axis 0 condition, list A */
    0, 3, 0, 1, 0, 2, 0, 3,     /* List A: 1, 2, 3 */
    0, 3, 0, 3, 0, 4, 0, 5,     /* List B: 3, 4, 5 */
    0, 1, 0, 1, 0x20, 0, 0x40, 0, /* Axis 1: [0.5, 1] */
    0, 1, 0, 0, 0x20, 0, 0x40, 0, /* Axis 0: [0.5, 1] */
  };
  స్థిర స్థిరరాశి అక్షరం feature_దత్తాంశం[] = {0, 0, 0, 2, 0, 0, 0, 6};
  స్థిరరాశి స్వయంచాలక &variations = *reinterpret_cast<స్థిరరాశి OT::FeatureVariations *> (దత్తాంశం);
  స్థిరరాశి స్వయంచాలక &feature = *reinterpret_cast<స్థిరరాశి OT::Feature *> (feature_దత్తాంశం);

  కొరకు (గుర్తులేని ముసుగు = 0; ముసుగు < 4; ముసుగు++)
  {
    పూర్ణాంకం coords[] = {ముసుగు & 1 ? 16384 : 0, ముసుగు & 2 ? 16384 : 0};
    OT::అంశం_Var_Store_Instancer instancer (శూన్యసూచిక, శూన్యసూచిక, hb_array (coords));
    hb_సదిశ_t<గుర్తులేని> state;
    hb_always_assert (variations.collect_lookup_conditions (coords, 2, &instancer, &state));
    గుర్తులేని condition_లెక్క = !!(ముసుగు & 1) + !!(ముసుగు & 2);
    hb_always_assert (state.నిడివి == condition_లెక్క);
    ఒకవేళ (ముసుగు == 3)
      hb_always_assert (state[0] == 92 && state[1] == 100);

    hb_సమితి_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        0, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (ముసుగు & 1 ? 7 : 5));
    hb_always_assert (lookups.has (0) && lookups.has (6));
    hb_always_assert (lookups.has (4) == !!(ముసుగు & 1));

    lookups.clear ();
    hb_always_assert (variations.get_lookup_variations (
        1, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (ముసుగు == 3 ? 5 : ముసుగు ? 3 : 0));
    hb_always_assert (!lookups.has (0) && !lookups.has (6));
    hb_always_assert (lookups.has (1) == !!(ముసుగు & 1));
    hb_always_assert (lookups.has (5) == !!(ముసుగు & 2));
    hb_always_assert (!variations.get_lookup_variations (
        2, feature, state.as_sorted_array (), &lookups));
  }
}

స్థిర శూన్య
test_feature_substitution_params_sanitize ()
{
  స్థిర స్థిరరాశి అక్షరం దత్తాంశం[] = {
    0, 1, 0, 1,                 /* GSUB 1.1 */
    0, 14, 0, 16, 0, 28,        /* ScriptList, FeatureList, LookupList */
    0, 0, 0, 30,                /* FeatureVariations */
    0, 0,                       /* ScriptList: no scripts */
    0, 1, 'c', 'v', '0', '1', 0, 8, /* FeatureList: cv01 */
    0, 0, 0, 0,                 /* cv01: no params, no lookups */
    0, 0,                       /* LookupList: no lookups */
    0, 1, 0, 0, 0, 0, 0, 1,     /* FeatureVariations 1.0, one record */
    0, 0, 0, 0, 0, 0, 0, 16,    /* Null ConditionSet, substitution */
    0, 1, 0, 0, 0, 1,           /* FeatureTableSubstitution 1.0, one record */
    0, 0, 0, 0, 0, 12,          /* Feature 0 */
    0, 4, 0, 0,                 /* Substitute: params, no lookups */
    0, 0, 0, 0, 0, 0, 0, 0,     /* cvXX: format, label, tooltip, sample */
    0, 0, 0, 0, 0, 1, 0, 0, 0x41, /* No named parameters, one character */
  };

  hb_blob_t *blob = hb_blob_create (దత్తాంశం, పరిమాణం (దత్తాంశం),
				    HB_జ్ఞాపకం_బహుళకం_READONLY, శూన్యసూచిక, శూన్యసూచిక);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (hb_blob_get_నిడివి (blob) == పరిమాణం (దత్తాంశం));
  hb_blob_destroy (blob);

#ఒకవేళ !defined(HB_NO_VAR) && !defined(HB_NO_LAYOUT_FEATURE_PARAMS)
  blob = hb_blob_create (దత్తాంశం, పరిమాణం (దత్తాంశం) - 1,
			 HB_జ్ఞాపకం_బహుళకం_READONLY, శూన్యసూచిక, శూన్యసూచిక);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (!hb_blob_get_నిడివి (blob));
  hb_blob_destroy (blob);
#అయితే_ముగింపు
}

స్థిర శూన్య
test_medium_నియమం ()
{
  స్థిర స్థిరరాశి అక్షరం దత్తాంశం[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    0, 0, 5,                    /* input glyph */
    0, 1, 0, 2,                 /* LookupRecord */
  };
  స్థిర స్థిరరాశి అక్షరం expected[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    1, 0, 1,                    /* input glyph */
    0, 1, 0, 3,                 /* LookupRecord */
  };
  స్థిరరాశి స్వయంచాలక &నియమం_2 = *reinterpret_cast<స్థిరరాశి OT::నియమం_3<OT::Layout::MediumTypes> *> (దత్తాంశం);

  hb_map_t glyph_map;
  glyph_map.సమితి (5, 0x10001);
  hb_map_t lookup_map;
  lookup_map.సమితి (2, 3);

  అక్షరం buf[32];
  hb_serialize_context_t s (buf, పరిమాణం (buf));
  s.ప్రారంభం_serialize ();
  hb_always_assert (నియమం_2.serialize (&s, &glyph_map, &lookup_map));
  s.చివర_serialize ();

  hb_బైట్లు_t బైట్లు = s.copy_బైట్లు ();
  hb_always_assert (బైట్లు.నిడివి == పరిమాణం (expected));
  hb_always_assert (!hb_memcmp (బైట్లు.arrayZ, expected, పరిమాణం (expected)));
  బైట్లు.fini ();
}

స్థిర శూన్య
test_medium_chain_నియమం ()
{
  స్థిర స్థిరరాశి అక్షరం దత్తాంశం[] = {
    0, 1, 0, 0, 5,             /* backtrack */
    0, 2, 0, 0, 6,             /*ఆదానం*/
    0, 1, 0, 0, 7,             /* lookahead */
    0, 1, 0, 0, 0, 2,          /* LookupRecord */
  };
  స్థిర స్థిరరాశి అక్షరం expected[] = {
    0, 1, 1, 0, 1,             /* backtrack */
    0, 2, 1, 0, 2,             /*ఆదానం*/
    0, 1, 1, 0, 3,             /* lookahead */
    0, 1, 0, 0, 0, 3,          /* LookupRecord */
  };
  స్థిరరాశి స్వయంచాలక &నియమం_2 = *reinterpret_cast<స్థిరరాశి OT::Chain_నియమం<OT::Layout::MediumTypes> *> (దత్తాంశం);

  hb_map_t glyph_map;
  glyph_map.సమితి (5, 0x10001);
  glyph_map.సమితి (6, 0x10002);
  glyph_map.సమితి (7, 0x10003);
  hb_map_t lookup_map;
  lookup_map.సమితి (2, 3);

  అక్షరం buf[32];
  hb_serialize_context_t s (buf, పరిమాణం (buf));
  s.ప్రారంభం_serialize ();
  hb_always_assert (నియమం_2.serialize (&s, &lookup_map, &glyph_map));
  s.చివర_serialize ();

  hb_బైట్లు_t బైట్లు = s.copy_బైట్లు ();
  hb_always_assert (బైట్లు.నిడివి == పరిమాణం (expected));
  hb_always_assert (!hb_memcmp (బైట్లు.arrayZ, expected, పరిమాణం (expected)));
  బైట్లు.fini ();
}

స్థిర శూన్య
test_device_only_విలువ ()
{
  ఉపయోగించి నామాకాశ OT::Layout::GPOS_impl;
  స్థిర స్థిరరాశి అక్షరం దత్తాంశం[] = {
    0, 2,                      /* Device offset */
    0, 0, 0, 0, (అక్షరం) 0x80, 0, /* VariationIndex 0 */
  };
  స్థిరరాశి స్వయంచాలక *పునాది = reinterpret_cast<స్థిరరాశి విలువ_పునాది *> (దత్తాంశం);
  స్థిరరాశి స్వయంచాలక *values = reinterpret_cast<స్థిరరాశి విలువ *> (దత్తాంశం);
  కొరకు (గుర్తులేని device = విలువ_Format::xPlaDevice;
       device <= విలువ_Format::yAdvDevice; device <<= 1)
    కొరకు (తార్కిక partial : {అబద్ధం, నిజం})
    {
      విలువ_Format format;
      format = device;
      hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> mapping;
      mapping.సమితి (0, hb_pair (partial ? 1u : OT::VarIdx::NO_VARIATION, 25));
      గుర్తులేని new_format = format.get_effective_format (values, అబద్ధం, అబద్ధం, పునాది, &mapping);
      hb_always_assert (new_format == (device >> 4 | (partial ? device : 0)));

      అక్షరం buf[32];
      hb_serialize_context_t s (buf, పరిమాణం (buf));
      s.ప్రారంభం_serialize ();
      format.copy_విలువలు (&s, new_format, పునాది, values, &mapping);
      s.చివర_serialize ();
      hb_బైట్లు_t బైట్లు = s.copy_బైట్లు ();
      hb_always_assert (!s.లోపల_error ());
      hb_always_assert (బైట్లు.నిడివి >= OT::HBINT16::static_కొలత);
      hb_always_assert (*reinterpret_cast<స్థిరరాశి OT::HBINT16 *> (బైట్లు.arrayZ) == 25);
      బైట్లు.fini ();
    }
}

స్థిర శూన్య
test_failed_class_mapping ()
{
  hb_సమితి_t klasses;
  klasses.కూడు (1);
  hb_sorted_సదిశ_t<hb_codepoint_pair_t> glyph_and_klass;
  glyph_and_klass.push (hb_pair (hb_codepoint_t (42), hb_codepoint_t (1)));
  hb_map_t klass_map;
  klass_map.successful = అబద్ధం; /* Simulate failure allocating the class map. */
  అక్షరం buf[32];
  hb_serialize_context_t s (buf, పరిమాణం (buf));
  s.ప్రారంభం_serialize<OT::ClassDef> ();
  hb_always_assert (!OT::ClassDef_remap_and_serialize (
      &s, klasses, నిజం, glyph_and_klass, &klass_map));
  hb_always_assert (s.లోపల_error () && !s.only_అధికప్రవాహం ());
  hb_always_assert (glyph_and_klass[0].second == 1);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_null_condition ();
  test_lookup_condition_sharing ();
  test_lookup_condition_resolution ();
  test_feature_substitution_params_sanitize ();
  test_medium_నియమం ();
  test_medium_chain_నియమం ();
  test_device_only_విలువ ();
  test_failed_class_mapping ();

  అక్షరం buf[16384];

  hb_serialize_context_t s (buf, పరిమాణం (buf));

  hb_sorted_సదిశ_t<hb_codepoint_t> v{1, 2, 5};

  స్వయంచాలక c = s.ప్రారంభం_serialize<Coverage> ();

  c->serialize (&s, hb_iter (v));

  s.చివర_serialize ();

  hb_బైట్లు_t బైట్లు = s.copy_బైట్లు ();
  hb_always_assert (బైట్లు.నిడివి == 10);
  బైట్లు.fini ();

  తిరిగి 0;
}

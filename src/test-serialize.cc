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

#ضمن "hb.hh"
#ضمن "hb-serialize.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-layout-gsubgpos.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "OT/Layout/GPOS/ValueFormat.hh"

استعمال OT::Layout::Common::Coverage;

ساكن فراغ
test_null_condition ()
{
  ساكن ثابت حرف and_condition[حجم (OT::Condition)] = {
    0, 3, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  ساكن ثابت حرف or_condition[حجم (OT::Condition)] = {
    0, 4, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  ساكن ثابت حرف negate_condition[حجم (OT::Condition)] = {
    0, 5,                        /* format */
    0, 0, 0,                     /* conditionOffset */
  };

  ثابت خودڪار &condition = Null (OT::Condition);
  ثابت خودڪار &condition_and =
      *reinterpret_cast<ثابت OT::Condition *> (and_condition);
  ثابت خودڪار &condition_or =
      *reinterpret_cast<ثابت OT::Condition *> (or_condition);
  ثابت خودڪار &condition_negate =
      *reinterpret_cast<ثابت OT::Condition *> (negate_condition);
  OT::شي_Var_Store_Instancer *instancer = خالي_اشارو;
  hb_سيٽ_t var_indices;

  hb_always_assert (condition.evaluate (خالي_اشارو, 0, instancer));
  hb_always_assert (condition_and.evaluate (خالي_اشارو, 0, instancer));
  hb_always_assert (condition_or.evaluate (خالي_اشارو, 0, instancer));
  hb_always_assert (!condition_negate.evaluate (خالي_اشارو, 0, instancer));

  hb_always_assert (condition.collect_var_indices (&var_indices));
  hb_always_assert (condition_and.collect_var_indices (&var_indices));
  hb_always_assert (condition_or.collect_var_indices (&var_indices));
  hb_always_assert (condition_negate.collect_var_indices (&var_indices));
  hb_always_assert (var_indices.is_empty ());

  حرف buf[8];
  hb_serialize_context_t s (buf, حجم (buf));
  hb_map_t varidx_map;
  ساكن ثابت بنانشان حرف expected[] = {
    0, 2,                        /* format */
    0, 1,                        /* defaultValue */
    0xFF, 0xFF, 0xFF, 0xFF,     /* varIdx */
  };

  خودڪار *copy = s.شروعات_serialize<OT::Condition> ();
  hb_always_assert (copy->serialize (&s, &condition, varidx_map));
  s.پڇاڙي_serialize ();

  hb_بائيٽون_t بائيٽون = s.copy_بائيٽون ();
  hb_always_assert (بائيٽون.ڊيگهائي == حجم (expected));
  hb_always_assert (!hb_memcmp (بائيٽون.arrayZ, expected, حجم (expected)));
  بائيٽون.fini ();
}

ساكن فراغ
test_lookup_condition_sharing ()
{
  ثابت بنانشان feature_ڳڻپ_2 = 10922, lookup_ڳڻپ = 402;
  ثابت بنانشان جدول_وٿي = 12 + 6 * feature_ڳڻپ_2;
  ثابت بنانشان condition_وٿي = جدول_وٿي + 20 + 2 * lookup_ڳڻپ;
  hb_ويڪٽر_t<حرف> معلومات;
  hb_always_assert (معلومات.resize (condition_وٿي + 8));
  hb_memset (معلومات.arrayZ, 0, معلومات.ڊيگهائي);
  خودڪار put16 = [&معلومات] (بنانشان وٿي, بنانشان value)
  {
    معلومات[وٿي] = value >> 8;
    معلومات[وٿي + 1] = value;
  };
  خودڪار put32 = [&put16] (بنانشان وٿي, بنانشان value)
  {
    put16 (وٿي, value >> 16);
    put16 (وٿي + 2, value);
  };
  put32 (0, 0x00010001);
  put32 (8, feature_ڳڻپ_2);
  لكل (بنانشان i = 0; i < feature_ڳڻپ_2; i++)
  {
    put16 (12 + 6 * i, i);
    put32 (14 + 6 * i, جدول_وٿي);
  }
  put32 (جدول_وٿي, 0x00010000);
  put32 (جدول_وٿي + 6, 1); /* LookupConditionRecord count */
  put32 (جدول_وٿي + 10, condition_وٿي - جدول_وٿي);
  put32 (جدول_وٿي + 14, 18); /* LookupIndexList offset */
  put16 (جدول_وٿي + 18, lookup_ڳڻپ);
  لكل (بنانشان i = 0; i < lookup_ڳڻپ; i++)
    put16 (جدول_وٿي + 20 + 2 * i, i);
  put16 (condition_وٿي, 2); /* Constant true ConditionValue */
  put16 (condition_وٿي + 2, 1);
  put32 (condition_وٿي + 4, OT::VarIdx::NO_VARIATION);

  ثابت خودڪار &variations = *reinterpret_cast<ثابت OT::FeatureVariations *> (معلومات.arrayZ);
  OT::شي_Var_Store_Instancer instancer (خالي_اشارو, خالي_اشارو, hb_array_t<ثابت صحيح> ());
  hb_ويڪٽر_t<بنانشان> state;
  hb_always_assert (variations.collect_lookup_conditions (خالي_اشارو, 0, &instancer, &state));
  hb_always_assert (state.ڊيگهائي == 1 && state[0] == condition_وٿي);

  لكل (بنانشان feature_اشاريو : {0u, feature_ڳڻپ_2 - 1})
  {
    hb_سيٽ_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        feature_اشاريو, Null (OT::Feature), state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == lookup_ڳڻپ);
    hb_always_assert (lookups.get_min () == 0 && lookups.get_max () == lookup_ڳڻپ - 1);
  }

  /* Null conditions need no state, even when thousands of records use them. */
  put32 (جدول_وٿي + 10, 0);
  state.reset ();
  hb_always_assert (variations.collect_lookup_conditions (خالي_اشارو, 0, &instancer, &state));
  hb_always_assert (!state.ڊيگهائي);
  hb_سيٽ_t lookups;
  hb_always_assert (variations.get_lookup_variations (
      0, Null (OT::Feature), state.as_sorted_array (), &lookups));
  hb_always_assert (lookups.get_population () == lookup_ڳڻپ);

  put32 (جدول_وٿي + 10, condition_وٿي - جدول_وٿي);
  state.سيٽ_error (); /* Simulate failure allocating the condition key. */
  hb_always_assert (!variations.collect_lookup_conditions (خالي_اشارو, 0, &instancer, &state));
}

ساكن فراغ
test_lookup_condition_resolution ()
{
  ساكن ثابت حرف معلومات[] = {
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
  ساكن ثابت حرف feature_معلومات[] = {0, 0, 0, 2, 0, 0, 0, 6};
  ثابت خودڪار &variations = *reinterpret_cast<ثابت OT::FeatureVariations *> (معلومات);
  ثابت خودڪار &feature = *reinterpret_cast<ثابت OT::Feature *> (feature_معلومات);

  لكل (بنانشان نقاب = 0; نقاب < 4; نقاب++)
  {
    صحيح coords[] = {نقاب & 1 ? 16384 : 0, نقاب & 2 ? 16384 : 0};
    OT::شي_Var_Store_Instancer instancer (خالي_اشارو, خالي_اشارو, hb_array (coords));
    hb_ويڪٽر_t<بنانشان> state;
    hb_always_assert (variations.collect_lookup_conditions (coords, 2, &instancer, &state));
    بنانشان condition_ڳڻپ = !!(نقاب & 1) + !!(نقاب & 2);
    hb_always_assert (state.ڊيگهائي == condition_ڳڻپ);
    إذا (نقاب == 3)
      hb_always_assert (state[0] == 92 && state[1] == 100);

    hb_سيٽ_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        0, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (نقاب & 1 ? 7 : 5));
    hb_always_assert (lookups.has (0) && lookups.has (6));
    hb_always_assert (lookups.has (4) == !!(نقاب & 1));

    lookups.clear ();
    hb_always_assert (variations.get_lookup_variations (
        1, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (نقاب == 3 ? 5 : نقاب ? 3 : 0));
    hb_always_assert (!lookups.has (0) && !lookups.has (6));
    hb_always_assert (lookups.has (1) == !!(نقاب & 1));
    hb_always_assert (lookups.has (5) == !!(نقاب & 2));
    hb_always_assert (!variations.get_lookup_variations (
        2, feature, state.as_sorted_array (), &lookups));
  }
}

ساكن فراغ
test_feature_substitution_params_sanitize ()
{
  ساكن ثابت حرف معلومات[] = {
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

  hb_blob_t *blob = hb_blob_create (معلومات, حجم (معلومات),
				    HB_يادگيري_گهڻائي_READONLY, خالي_اشارو, خالي_اشارو);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (hb_blob_get_ڊيگهائي (blob) == حجم (معلومات));
  hb_blob_destroy (blob);

#إذا !defined(HB_NO_VAR) && !defined(HB_NO_LAYOUT_FEATURE_PARAMS)
  blob = hb_blob_create (معلومات, حجم (معلومات) - 1,
			 HB_يادگيري_گهڻائي_READONLY, خالي_اشارو, خالي_اشارو);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (!hb_blob_get_ڊيگهائي (blob));
  hb_blob_destroy (blob);
#جيڪڏهن_پورو
}

ساكن فراغ
test_medium_قاعدو ()
{
  ساكن ثابت حرف معلومات[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    0, 0, 5,                    /* input glyph */
    0, 1, 0, 2,                 /* LookupRecord */
  };
  ساكن ثابت حرف expected[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    1, 0, 1,                    /* input glyph */
    0, 1, 0, 3,                 /* LookupRecord */
  };
  ثابت خودڪار &قاعدو_2 = *reinterpret_cast<ثابت OT::قاعدو_3<OT::Layout::MediumTypes> *> (معلومات);

  hb_map_t glyph_map;
  glyph_map.سيٽ (5, 0x10001);
  hb_map_t lookup_map;
  lookup_map.سيٽ (2, 3);

  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.شروعات_serialize ();
  hb_always_assert (قاعدو_2.serialize (&s, &glyph_map, &lookup_map));
  s.پڇاڙي_serialize ();

  hb_بائيٽون_t بائيٽون = s.copy_بائيٽون ();
  hb_always_assert (بائيٽون.ڊيگهائي == حجم (expected));
  hb_always_assert (!hb_memcmp (بائيٽون.arrayZ, expected, حجم (expected)));
  بائيٽون.fini ();
}

ساكن فراغ
test_medium_chain_قاعدو ()
{
  ساكن ثابت حرف معلومات[] = {
    0, 1, 0, 0, 5,             /* backtrack */
    0, 2, 0, 0, 6,             /*داخلا*/
    0, 1, 0, 0, 7,             /* lookahead */
    0, 1, 0, 0, 0, 2,          /* LookupRecord */
  };
  ساكن ثابت حرف expected[] = {
    0, 1, 1, 0, 1,             /* backtrack */
    0, 2, 1, 0, 2,             /*داخلا*/
    0, 1, 1, 0, 3,             /* lookahead */
    0, 1, 0, 0, 0, 3,          /* LookupRecord */
  };
  ثابت خودڪار &قاعدو_2 = *reinterpret_cast<ثابت OT::Chain_قاعدو<OT::Layout::MediumTypes> *> (معلومات);

  hb_map_t glyph_map;
  glyph_map.سيٽ (5, 0x10001);
  glyph_map.سيٽ (6, 0x10002);
  glyph_map.سيٽ (7, 0x10003);
  hb_map_t lookup_map;
  lookup_map.سيٽ (2, 3);

  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.شروعات_serialize ();
  hb_always_assert (قاعدو_2.serialize (&s, &lookup_map, &glyph_map));
  s.پڇاڙي_serialize ();

  hb_بائيٽون_t بائيٽون = s.copy_بائيٽون ();
  hb_always_assert (بائيٽون.ڊيگهائي == حجم (expected));
  hb_always_assert (!hb_memcmp (بائيٽون.arrayZ, expected, حجم (expected)));
  بائيٽون.fini ();
}

ساكن فراغ
test_device_only_قدر ()
{
  استعمال نطاق OT::Layout::GPOS_impl;
  ساكن ثابت حرف معلومات[] = {
    0, 2,                      /* Device offset */
    0, 0, 0, 0, (حرف) 0x80, 0, /* VariationIndex 0 */
  };
  ثابت خودڪار *بنياد = reinterpret_cast<ثابت قدر_بنياد *> (معلومات);
  ثابت خودڪار *values = reinterpret_cast<ثابت قدر *> (معلومات);
  لكل (بنانشان device = قدر_Format::xPlaDevice;
       device <= قدر_Format::yAdvDevice; device <<= 1)
    لكل (منطقي partial : {خطأ, سچ})
    {
      قدر_Format format;
      format = device;
      hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> mapping;
      mapping.سيٽ (0, hb_pair (partial ? 1u : OT::VarIdx::NO_VARIATION, 25));
      بنانشان new_format = format.get_effective_format (values, خطأ, خطأ, بنياد, &mapping);
      hb_always_assert (new_format == (device >> 4 | (partial ? device : 0)));

      حرف buf[32];
      hb_serialize_context_t s (buf, حجم (buf));
      s.شروعات_serialize ();
      format.copy_قدرون (&s, new_format, بنياد, values, &mapping);
      s.پڇاڙي_serialize ();
      hb_بائيٽون_t بائيٽون = s.copy_بائيٽون ();
      hb_always_assert (!s.اندريون_error ());
      hb_always_assert (بائيٽون.ڊيگهائي >= OT::HBINT16::static_سائيز);
      hb_always_assert (*reinterpret_cast<ثابت OT::HBINT16 *> (بائيٽون.arrayZ) == 25);
      بائيٽون.fini ();
    }
}

ساكن فراغ
test_failed_class_mapping ()
{
  hb_سيٽ_t klasses;
  klasses.جوڙيو (1);
  hb_sorted_ويڪٽر_t<hb_codepoint_pair_t> glyph_and_klass;
  glyph_and_klass.push (hb_pair (hb_codepoint_t (42), hb_codepoint_t (1)));
  hb_map_t klass_map;
  klass_map.successful = خطأ; /* Simulate failure allocating the class map. */
  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.شروعات_serialize<OT::ClassDef> ();
  hb_always_assert (!OT::ClassDef_remap_and_serialize (
      &s, klasses, سچ, glyph_and_klass, &klass_map));
  hb_always_assert (s.اندريون_error () && !s.only_حد_کان_مٿي ());
  hb_always_assert (glyph_and_klass[0].second == 1);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  test_null_condition ();
  test_lookup_condition_sharing ();
  test_lookup_condition_resolution ();
  test_feature_substitution_params_sanitize ();
  test_medium_قاعدو ();
  test_medium_chain_قاعدو ();
  test_device_only_قدر ();
  test_failed_class_mapping ();

  حرف buf[16384];

  hb_serialize_context_t s (buf, حجم (buf));

  hb_sorted_ويڪٽر_t<hb_codepoint_t> v{1, 2, 5};

  خودڪار c = s.شروعات_serialize<Coverage> ();

  c->serialize (&s, hb_iter (v));

  s.پڇاڙي_serialize ();

  hb_بائيٽون_t بائيٽون = s.copy_بائيٽون ();
  hb_always_assert (بائيٽون.ڊيگهائي == 10);
  بائيٽون.fini ();

  أرجع 0;
}

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

کارول OT::Layout::Common::Coverage;

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

  ثابت خپلکاری &condition = Null (OT::Condition);
  ثابت خپلکاری &condition_and =
      *reinterpret_cast<ثابت OT::Condition *> (and_condition);
  ثابت خپلکاری &condition_or =
      *reinterpret_cast<ثابت OT::Condition *> (or_condition);
  ثابت خپلکاری &condition_negate =
      *reinterpret_cast<ثابت OT::Condition *> (negate_condition);
  OT::توکی_Var_Store_Instancer *instancer = تش_اشاره;
  hb_سېټ_t var_indices;

  hb_always_assert (condition.evaluate (تش_اشاره, 0, instancer));
  hb_always_assert (condition_and.evaluate (تش_اشاره, 0, instancer));
  hb_always_assert (condition_or.evaluate (تش_اشاره, 0, instancer));
  hb_always_assert (!condition_negate.evaluate (تش_اشاره, 0, instancer));

  hb_always_assert (condition.collect_var_indices (&var_indices));
  hb_always_assert (condition_and.collect_var_indices (&var_indices));
  hb_always_assert (condition_or.collect_var_indices (&var_indices));
  hb_always_assert (condition_negate.collect_var_indices (&var_indices));
  hb_always_assert (var_indices.is_empty ());

  حرف buf[8];
  hb_serialize_context_t s (buf, حجم (buf));
  hb_map_t varidx_map;
  ساكن ثابت بینښه حرف expected[] = {
    0, 2,                        /* format */
    0, 1,                        /* defaultValue */
    0xFF, 0xFF, 0xFF, 0xFF,     /* varIdx */
  };

  خپلکاری *copy = s.پیل_serialize<OT::Condition> ();
  hb_always_assert (copy->serialize (&s, &condition, varidx_map));
  s.پای_ټکی_serialize ();

  hb_بایټونه_t بایټونه = s.copy_بایټونه ();
  hb_always_assert (بایټونه.اوږدتیا == حجم (expected));
  hb_always_assert (!hb_memcmp (بایټونه.arrayZ, expected, حجم (expected)));
  بایټونه.fini ();
}

ساكن فراغ
test_lookup_condition_sharing ()
{
  ثابت بینښه feature_شمېر_2 = 10922, lookup_شمېر = 402;
  ثابت بینښه جدول_دننه_واټن = 12 + 6 * feature_شمېر_2;
  ثابت بینښه condition_دننه_واټن = جدول_دننه_واټن + 20 + 2 * lookup_شمېر;
  hb_ویکټور_t<حرف> معلومات;
  hb_always_assert (معلومات.resize (condition_دننه_واټن + 8));
  hb_memset (معلومات.arrayZ, 0, معلومات.اوږدتیا);
  خپلکاری put16 = [&معلومات] (بینښه دننه_واټن, بینښه value)
  {
    معلومات[دننه_واټن] = value >> 8;
    معلومات[دننه_واټن + 1] = value;
  };
  خپلکاری put32 = [&put16] (بینښه دننه_واټن, بینښه value)
  {
    put16 (دننه_واټن, value >> 16);
    put16 (دننه_واټن + 2, value);
  };
  put32 (0, 0x00010001);
  put32 (8, feature_شمېر_2);
  لكل (بینښه i = 0; i < feature_شمېر_2; i++)
  {
    put16 (12 + 6 * i, i);
    put32 (14 + 6 * i, جدول_دننه_واټن);
  }
  put32 (جدول_دننه_واټن, 0x00010000);
  put32 (جدول_دننه_واټن + 6, 1); /* LookupConditionRecord count */
  put32 (جدول_دننه_واټن + 10, condition_دننه_واټن - جدول_دننه_واټن);
  put32 (جدول_دننه_واټن + 14, 18); /* LookupIndexList offset */
  put16 (جدول_دننه_واټن + 18, lookup_شمېر);
  لكل (بینښه i = 0; i < lookup_شمېر; i++)
    put16 (جدول_دننه_واټن + 20 + 2 * i, i);
  put16 (condition_دننه_واټن, 2); /* Constant true ConditionValue */
  put16 (condition_دننه_واټن + 2, 1);
  put32 (condition_دننه_واټن + 4, OT::VarIdx::NO_VARIATION);

  ثابت خپلکاری &variations = *reinterpret_cast<ثابت OT::FeatureVariations *> (معلومات.arrayZ);
  OT::توکی_Var_Store_Instancer instancer (تش_اشاره, تش_اشاره, hb_array_t<ثابت صحيح> ());
  hb_ویکټور_t<بینښه> state;
  hb_always_assert (variations.collect_lookup_conditions (تش_اشاره, 0, &instancer, &state));
  hb_always_assert (state.اوږدتیا == 1 && state[0] == condition_دننه_واټن);

  لكل (بینښه feature_شاخص : {0u, feature_شمېر_2 - 1})
  {
    hb_سېټ_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        feature_شاخص, Null (OT::Feature), state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == lookup_شمېر);
    hb_always_assert (lookups.get_min () == 0 && lookups.get_max () == lookup_شمېر - 1);
  }

  /* Null conditions need no state, even when thousands of records use them. */
  put32 (جدول_دننه_واټن + 10, 0);
  state.reset ();
  hb_always_assert (variations.collect_lookup_conditions (تش_اشاره, 0, &instancer, &state));
  hb_always_assert (!state.اوږدتیا);
  hb_سېټ_t lookups;
  hb_always_assert (variations.get_lookup_variations (
      0, Null (OT::Feature), state.as_sorted_array (), &lookups));
  hb_always_assert (lookups.get_population () == lookup_شمېر);

  put32 (جدول_دننه_واټن + 10, condition_دننه_واټن - جدول_دننه_واټن);
  state.سېټ_error (); /* Simulate failure allocating the condition key. */
  hb_always_assert (!variations.collect_lookup_conditions (تش_اشاره, 0, &instancer, &state));
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
  ثابت خپلکاری &variations = *reinterpret_cast<ثابت OT::FeatureVariations *> (معلومات);
  ثابت خپلکاری &feature = *reinterpret_cast<ثابت OT::Feature *> (feature_معلومات);

  لكل (بینښه ماسک = 0; ماسک < 4; ماسک++)
  {
    صحيح coords[] = {ماسک & 1 ? 16384 : 0, ماسک & 2 ? 16384 : 0};
    OT::توکی_Var_Store_Instancer instancer (تش_اشاره, تش_اشاره, hb_array (coords));
    hb_ویکټور_t<بینښه> state;
    hb_always_assert (variations.collect_lookup_conditions (coords, 2, &instancer, &state));
    بینښه condition_شمېر = !!(ماسک & 1) + !!(ماسک & 2);
    hb_always_assert (state.اوږدتیا == condition_شمېر);
    إذا (ماسک == 3)
      hb_always_assert (state[0] == 92 && state[1] == 100);

    hb_سېټ_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        0, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (ماسک & 1 ? 7 : 5));
    hb_always_assert (lookups.has (0) && lookups.has (6));
    hb_always_assert (lookups.has (4) == !!(ماسک & 1));

    lookups.clear ();
    hb_always_assert (variations.get_lookup_variations (
        1, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (ماسک == 3 ? 5 : ماسک ? 3 : 0));
    hb_always_assert (!lookups.has (0) && !lookups.has (6));
    hb_always_assert (lookups.has (1) == !!(ماسک & 1));
    hb_always_assert (lookups.has (5) == !!(ماسک & 2));
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
				    HB_حافظه_ډېرپېښ_READONLY, تش_اشاره, تش_اشاره);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (hb_blob_get_اوږدتیا (blob) == حجم (معلومات));
  hb_blob_destroy (blob);

#إذا !defined(HB_NO_VAR) && !defined(HB_NO_LAYOUT_FEATURE_PARAMS)
  blob = hb_blob_create (معلومات, حجم (معلومات) - 1,
			 HB_حافظه_ډېرپېښ_READONLY, تش_اشاره, تش_اشاره);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (!hb_blob_get_اوږدتیا (blob));
  hb_blob_destroy (blob);
#د_که_پای
}

ساكن فراغ
test_medium_قاعده ()
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
  ثابت خپلکاری &قاعده_2 = *reinterpret_cast<ثابت OT::قاعده_3<OT::Layout::MediumTypes> *> (معلومات);

  hb_map_t glyph_map;
  glyph_map.سېټ (5, 0x10001);
  hb_map_t lookup_map;
  lookup_map.سېټ (2, 3);

  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.پیل_serialize ();
  hb_always_assert (قاعده_2.serialize (&s, &glyph_map, &lookup_map));
  s.پای_ټکی_serialize ();

  hb_بایټونه_t بایټونه = s.copy_بایټونه ();
  hb_always_assert (بایټونه.اوږدتیا == حجم (expected));
  hb_always_assert (!hb_memcmp (بایټونه.arrayZ, expected, حجم (expected)));
  بایټونه.fini ();
}

ساكن فراغ
test_medium_chain_قاعده ()
{
  ساكن ثابت حرف معلومات[] = {
    0, 1, 0, 0, 5,             /* backtrack */
    0, 2, 0, 0, 6,             /*ننوت*/
    0, 1, 0, 0, 7,             /* lookahead */
    0, 1, 0, 0, 0, 2,          /* LookupRecord */
  };
  ساكن ثابت حرف expected[] = {
    0, 1, 1, 0, 1,             /* backtrack */
    0, 2, 1, 0, 2,             /*ننوت*/
    0, 1, 1, 0, 3,             /* lookahead */
    0, 1, 0, 0, 0, 3,          /* LookupRecord */
  };
  ثابت خپلکاری &قاعده_2 = *reinterpret_cast<ثابت OT::Chain_قاعده<OT::Layout::MediumTypes> *> (معلومات);

  hb_map_t glyph_map;
  glyph_map.سېټ (5, 0x10001);
  glyph_map.سېټ (6, 0x10002);
  glyph_map.سېټ (7, 0x10003);
  hb_map_t lookup_map;
  lookup_map.سېټ (2, 3);

  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.پیل_serialize ();
  hb_always_assert (قاعده_2.serialize (&s, &lookup_map, &glyph_map));
  s.پای_ټکی_serialize ();

  hb_بایټونه_t بایټونه = s.copy_بایټونه ();
  hb_always_assert (بایټونه.اوږدتیا == حجم (expected));
  hb_always_assert (!hb_memcmp (بایټونه.arrayZ, expected, حجم (expected)));
  بایټونه.fini ();
}

ساكن فراغ
test_device_only_ارزښت ()
{
  کارول نطاق OT::Layout::GPOS_impl;
  ساكن ثابت حرف معلومات[] = {
    0, 2,                      /* Device offset */
    0, 0, 0, 0, (حرف) 0x80, 0, /* VariationIndex 0 */
  };
  ثابت خپلکاری *بنسټ = reinterpret_cast<ثابت ارزښت_بنسټ *> (معلومات);
  ثابت خپلکاری *values = reinterpret_cast<ثابت ارزښت *> (معلومات);
  لكل (بینښه device = ارزښت_Format::xPlaDevice;
       device <= ارزښت_Format::yAdvDevice; device <<= 1)
    لكل (منطقي partial : {خطأ, ریښتیا})
    {
      ارزښت_Format format;
      format = device;
      hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> mapping;
      mapping.سېټ (0, hb_pair (partial ? 1u : OT::VarIdx::NO_VARIATION, 25));
      بینښه new_format = format.get_effective_format (values, خطأ, خطأ, بنسټ, &mapping);
      hb_always_assert (new_format == (device >> 4 | (partial ? device : 0)));

      حرف buf[32];
      hb_serialize_context_t s (buf, حجم (buf));
      s.پیل_serialize ();
      format.copy_ارزښتونه (&s, new_format, بنسټ, values, &mapping);
      s.پای_ټکی_serialize ();
      hb_بایټونه_t بایټونه = s.copy_بایټونه ();
      hb_always_assert (!s.دننه_error ());
      hb_always_assert (بایټونه.اوږدتیا >= OT::HBINT16::static_اندازه);
      hb_always_assert (*reinterpret_cast<ثابت OT::HBINT16 *> (بایټونه.arrayZ) == 25);
      بایټونه.fini ();
    }
}

ساكن فراغ
test_failed_class_mapping ()
{
  hb_سېټ_t klasses;
  klasses.جمع_کړه (1);
  hb_sorted_ویکټور_t<hb_codepoint_pair_t> glyph_and_klass;
  glyph_and_klass.push (hb_pair (hb_codepoint_t (42), hb_codepoint_t (1)));
  hb_map_t klass_map;
  klass_map.successful = خطأ; /* Simulate failure allocating the class map. */
  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.پیل_serialize<OT::ClassDef> ();
  hb_always_assert (!OT::ClassDef_remap_and_serialize (
      &s, klasses, ریښتیا, glyph_and_klass, &klass_map));
  hb_always_assert (s.دننه_error () && !s.only_ډکېدنه ());
  hb_always_assert (glyph_and_klass[0].second == 1);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  test_null_condition ();
  test_lookup_condition_sharing ();
  test_lookup_condition_resolution ();
  test_feature_substitution_params_sanitize ();
  test_medium_قاعده ();
  test_medium_chain_قاعده ();
  test_device_only_ارزښت ();
  test_failed_class_mapping ();

  حرف buf[16384];

  hb_serialize_context_t s (buf, حجم (buf));

  hb_sorted_ویکټور_t<hb_codepoint_t> v{1, 2, 5};

  خپلکاری c = s.پیل_serialize<Coverage> ();

  c->serialize (&s, hb_iter (v));

  s.پای_ټکی_serialize ();

  hb_بایټونه_t بایټونه = s.copy_بایټونه ();
  hb_always_assert (بایټونه.اوږدتیا == 10);
  بایټونه.fini ();

  أرجع 0;
}

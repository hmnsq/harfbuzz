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

#شامل "hb.hh"
#شامل "hb-serialize.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-layout-gsubgpos.hh"
#شامل "hb-ot-layout-gsub-table.hh"
#شامل "OT/Layout/GPOS/ValueFormat.hh"

استعمال OT::Layout::Common::Coverage;

ساکن خالی
test_null_condition ()
{
  ساکن ثابت حرف and_condition[حجم (OT::Condition)] = {
    0, 3, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  ساکن ثابت حرف or_condition[حجم (OT::Condition)] = {
    0, 4, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  ساکن ثابت حرف negate_condition[حجم (OT::Condition)] = {
    0, 5,                        /* format */
    0, 0, 0,                     /* conditionOffset */
  };

  ثابت خودکار &condition = Null (OT::Condition);
  ثابت خودکار &condition_and =
      *reinterpret_cast<ثابت OT::Condition *> (and_condition);
  ثابت خودکار &condition_or =
      *reinterpret_cast<ثابت OT::Condition *> (or_condition);
  ثابت خودکار &condition_negate =
      *reinterpret_cast<ثابت OT::Condition *> (negate_condition);
  OT::شے_Var_Store_Instancer *instancer = خالی_اشاریہ;
  hb_سیٹ_t var_indices;

  hb_always_assert (condition.evaluate (خالی_اشاریہ, 0, instancer));
  hb_always_assert (condition_and.evaluate (خالی_اشاریہ, 0, instancer));
  hb_always_assert (condition_or.evaluate (خالی_اشاریہ, 0, instancer));
  hb_always_assert (!condition_negate.evaluate (خالی_اشاریہ, 0, instancer));

  hb_always_assert (condition.collect_var_indices (&var_indices));
  hb_always_assert (condition_and.collect_var_indices (&var_indices));
  hb_always_assert (condition_or.collect_var_indices (&var_indices));
  hb_always_assert (condition_negate.collect_var_indices (&var_indices));
  hb_always_assert (var_indices.is_empty ());

  حرف buf[8];
  hb_serialize_context_t s (buf, حجم (buf));
  hb_map_t varidx_map;
  ساکن ثابت غیرعلامتی حرف expected[] = {
    0, 2,                        /* format */
    0, 1,                        /* defaultValue */
    0xFF, 0xFF, 0xFF, 0xFF,     /* varIdx */
  };

  خودکار *copy = s.آغاز_serialize<OT::Condition> ();
  hb_always_assert (copy->serialize (&s, &condition, varidx_map));
  s.انتہا_serialize ();

  hb_بائٹیں_t بائٹیں = s.copy_بائٹیں ();
  hb_always_assert (بائٹیں.لمبائی == حجم (expected));
  hb_always_assert (!hb_memcmp (بائٹیں.arrayZ, expected, حجم (expected)));
  بائٹیں.fini ();
}

ساکن خالی
test_lookup_condition_sharing ()
{
  ثابت غیرعلامتی feature_گنتی_2 = 10922, lookup_گنتی = 402;
  ثابت غیرعلامتی جدول_انحراف = 12 + 6 * feature_گنتی_2;
  ثابت غیرعلامتی condition_انحراف = جدول_انحراف + 20 + 2 * lookup_گنتی;
  hb_سمتیہ_t<حرف> ڈیٹا;
  hb_always_assert (ڈیٹا.resize (condition_انحراف + 8));
  hb_memset (ڈیٹا.arrayZ, 0, ڈیٹا.لمبائی);
  خودکار put16 = [&ڈیٹا] (غیرعلامتی انحراف, غیرعلامتی value)
  {
    ڈیٹا[انحراف] = value >> 8;
    ڈیٹا[انحراف + 1] = value;
  };
  خودکار put32 = [&put16] (غیرعلامتی انحراف, غیرعلامتی value)
  {
    put16 (انحراف, value >> 16);
    put16 (انحراف + 2, value);
  };
  put32 (0, 0x00010001);
  put32 (8, feature_گنتی_2);
  برائے (غیرعلامتی i = 0; i < feature_گنتی_2; i++)
  {
    put16 (12 + 6 * i, i);
    put32 (14 + 6 * i, جدول_انحراف);
  }
  put32 (جدول_انحراف, 0x00010000);
  put32 (جدول_انحراف + 6, 1); /* LookupConditionRecord count */
  put32 (جدول_انحراف + 10, condition_انحراف - جدول_انحراف);
  put32 (جدول_انحراف + 14, 18); /* LookupIndexList offset */
  put16 (جدول_انحراف + 18, lookup_گنتی);
  برائے (غیرعلامتی i = 0; i < lookup_گنتی; i++)
    put16 (جدول_انحراف + 20 + 2 * i, i);
  put16 (condition_انحراف, 2); /* Constant true ConditionValue */
  put16 (condition_انحراف + 2, 1);
  put32 (condition_انحراف + 4, OT::VarIdx::NO_VARIATION);

  ثابت خودکار &variations = *reinterpret_cast<ثابت OT::FeatureVariations *> (ڈیٹا.arrayZ);
  OT::شے_Var_Store_Instancer instancer (خالی_اشاریہ, خالی_اشاریہ, hb_array_t<ثابت صحیح> ());
  hb_سمتیہ_t<غیرعلامتی> state;
  hb_always_assert (variations.collect_lookup_conditions (خالی_اشاریہ, 0, &instancer, &state));
  hb_always_assert (state.لمبائی == 1 && state[0] == condition_انحراف);

  برائے (غیرعلامتی feature_اشاریہ : {0u, feature_گنتی_2 - 1})
  {
    hb_سیٹ_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        feature_اشاریہ, Null (OT::Feature), state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == lookup_گنتی);
    hb_always_assert (lookups.get_min () == 0 && lookups.get_max () == lookup_گنتی - 1);
  }

  /* Null conditions need no state, even when thousands of records use them. */
  put32 (جدول_انحراف + 10, 0);
  state.reset ();
  hb_always_assert (variations.collect_lookup_conditions (خالی_اشاریہ, 0, &instancer, &state));
  hb_always_assert (!state.لمبائی);
  hb_سیٹ_t lookups;
  hb_always_assert (variations.get_lookup_variations (
      0, Null (OT::Feature), state.as_sorted_array (), &lookups));
  hb_always_assert (lookups.get_population () == lookup_گنتی);

  put32 (جدول_انحراف + 10, condition_انحراف - جدول_انحراف);
  state.سیٹ_error (); /* Simulate failure allocating the condition key. */
  hb_always_assert (!variations.collect_lookup_conditions (خالی_اشاریہ, 0, &instancer, &state));
}

ساکن خالی
test_lookup_condition_resolution ()
{
  ساکن ثابت حرف ڈیٹا[] = {
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
  ساکن ثابت حرف feature_ڈیٹا[] = {0, 0, 0, 2, 0, 0, 0, 6};
  ثابت خودکار &variations = *reinterpret_cast<ثابت OT::FeatureVariations *> (ڈیٹا);
  ثابت خودکار &feature = *reinterpret_cast<ثابت OT::Feature *> (feature_ڈیٹا);

  برائے (غیرعلامتی نقاب = 0; نقاب < 4; نقاب++)
  {
    صحیح coords[] = {نقاب & 1 ? 16384 : 0, نقاب & 2 ? 16384 : 0};
    OT::شے_Var_Store_Instancer instancer (خالی_اشاریہ, خالی_اشاریہ, hb_array (coords));
    hb_سمتیہ_t<غیرعلامتی> state;
    hb_always_assert (variations.collect_lookup_conditions (coords, 2, &instancer, &state));
    غیرعلامتی condition_گنتی = !!(نقاب & 1) + !!(نقاب & 2);
    hb_always_assert (state.لمبائی == condition_گنتی);
    اگر (نقاب == 3)
      hb_always_assert (state[0] == 92 && state[1] == 100);

    hb_سیٹ_t lookups;
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

ساکن خالی
test_feature_substitution_params_sanitize ()
{
  ساکن ثابت حرف ڈیٹا[] = {
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

  hb_blob_t *blob = hb_blob_create (ڈیٹا, حجم (ڈیٹا),
				    HB_حافظہ_منوال_READONLY, خالی_اشاریہ, خالی_اشاریہ);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (hb_blob_get_لمبائی (blob) == حجم (ڈیٹا));
  hb_blob_destroy (blob);

#اگر !defined(HB_NO_VAR) && !defined(HB_NO_LAYOUT_FEATURE_PARAMS)
  blob = hb_blob_create (ڈیٹا, حجم (ڈیٹا) - 1,
			 HB_حافظہ_منوال_READONLY, خالی_اشاریہ, خالی_اشاریہ);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (!hb_blob_get_لمبائی (blob));
  hb_blob_destroy (blob);
#اگر_ختم
}

ساکن خالی
test_medium_قاعدہ ()
{
  ساکن ثابت حرف ڈیٹا[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    0, 0, 5,                    /* input glyph */
    0, 1, 0, 2,                 /* LookupRecord */
  };
  ساکن ثابت حرف expected[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    1, 0, 1,                    /* input glyph */
    0, 1, 0, 3,                 /* LookupRecord */
  };
  ثابت خودکار &قاعدہ_2 = *reinterpret_cast<ثابت OT::قاعدہ_3<OT::Layout::MediumTypes> *> (ڈیٹا);

  hb_map_t glyph_map;
  glyph_map.سیٹ (5, 0x10001);
  hb_map_t lookup_map;
  lookup_map.سیٹ (2, 3);

  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.آغاز_serialize ();
  hb_always_assert (قاعدہ_2.serialize (&s, &glyph_map, &lookup_map));
  s.انتہا_serialize ();

  hb_بائٹیں_t بائٹیں = s.copy_بائٹیں ();
  hb_always_assert (بائٹیں.لمبائی == حجم (expected));
  hb_always_assert (!hb_memcmp (بائٹیں.arrayZ, expected, حجم (expected)));
  بائٹیں.fini ();
}

ساکن خالی
test_medium_chain_قاعدہ ()
{
  ساکن ثابت حرف ڈیٹا[] = {
    0, 1, 0, 0, 5,             /* backtrack */
    0, 2, 0, 0, 6,             /*داخلہ*/
    0, 1, 0, 0, 7,             /* lookahead */
    0, 1, 0, 0, 0, 2,          /* LookupRecord */
  };
  ساکن ثابت حرف expected[] = {
    0, 1, 1, 0, 1,             /* backtrack */
    0, 2, 1, 0, 2,             /*داخلہ*/
    0, 1, 1, 0, 3,             /* lookahead */
    0, 1, 0, 0, 0, 3,          /* LookupRecord */
  };
  ثابت خودکار &قاعدہ_2 = *reinterpret_cast<ثابت OT::Chain_قاعدہ<OT::Layout::MediumTypes> *> (ڈیٹا);

  hb_map_t glyph_map;
  glyph_map.سیٹ (5, 0x10001);
  glyph_map.سیٹ (6, 0x10002);
  glyph_map.سیٹ (7, 0x10003);
  hb_map_t lookup_map;
  lookup_map.سیٹ (2, 3);

  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.آغاز_serialize ();
  hb_always_assert (قاعدہ_2.serialize (&s, &lookup_map, &glyph_map));
  s.انتہا_serialize ();

  hb_بائٹیں_t بائٹیں = s.copy_بائٹیں ();
  hb_always_assert (بائٹیں.لمبائی == حجم (expected));
  hb_always_assert (!hb_memcmp (بائٹیں.arrayZ, expected, حجم (expected)));
  بائٹیں.fini ();
}

ساکن خالی
test_device_only_قدر ()
{
  استعمال نامخلا OT::Layout::GPOS_impl;
  ساکن ثابت حرف ڈیٹا[] = {
    0, 2,                      /* Device offset */
    0, 0, 0, 0, (حرف) 0x80, 0, /* VariationIndex 0 */
  };
  ثابت خودکار *بنیاد = reinterpret_cast<ثابت قدر_بنیاد *> (ڈیٹا);
  ثابت خودکار *values = reinterpret_cast<ثابت قدر *> (ڈیٹا);
  برائے (غیرعلامتی device = قدر_Format::xPlaDevice;
       device <= قدر_Format::yAdvDevice; device <<= 1)
    برائے (منطقی partial : {جھوٹ, سچ})
    {
      قدر_Format format;
      format = device;
      hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> mapping;
      mapping.سیٹ (0, hb_pair (partial ? 1u : OT::VarIdx::NO_VARIATION, 25));
      غیرعلامتی new_format = format.get_effective_format (values, جھوٹ, جھوٹ, بنیاد, &mapping);
      hb_always_assert (new_format == (device >> 4 | (partial ? device : 0)));

      حرف buf[32];
      hb_serialize_context_t s (buf, حجم (buf));
      s.آغاز_serialize ();
      format.copy_اقدار (&s, new_format, بنیاد, values, &mapping);
      s.انتہا_serialize ();
      hb_بائٹیں_t بائٹیں = s.copy_بائٹیں ();
      hb_always_assert (!s.اندر_error ());
      hb_always_assert (بائٹیں.لمبائی >= OT::HBINT16::static_سائز);
      hb_always_assert (*reinterpret_cast<ثابت OT::HBINT16 *> (بائٹیں.arrayZ) == 25);
      بائٹیں.fini ();
    }
}

ساکن خالی
test_failed_class_mapping ()
{
  hb_سیٹ_t klasses;
  klasses.جوڑو (1);
  hb_sorted_سمتیہ_t<hb_codepoint_pair_t> glyph_and_klass;
  glyph_and_klass.push (hb_pair (hb_codepoint_t (42), hb_codepoint_t (1)));
  hb_map_t klass_map;
  klass_map.successful = جھوٹ; /* Simulate failure allocating the class map. */
  حرف buf[32];
  hb_serialize_context_t s (buf, حجم (buf));
  s.آغاز_serialize<OT::ClassDef> ();
  hb_always_assert (!OT::ClassDef_remap_and_serialize (
      &s, klasses, سچ, glyph_and_klass, &klass_map));
  hb_always_assert (s.اندر_error () && !s.only_حد_سے_تجاوز ());
  hb_always_assert (glyph_and_klass[0].second == 1);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  test_null_condition ();
  test_lookup_condition_sharing ();
  test_lookup_condition_resolution ();
  test_feature_substitution_params_sanitize ();
  test_medium_قاعدہ ();
  test_medium_chain_قاعدہ ();
  test_device_only_قدر ();
  test_failed_class_mapping ();

  حرف buf[16384];

  hb_serialize_context_t s (buf, حجم (buf));

  hb_sorted_سمتیہ_t<hb_codepoint_t> v{1, 2, 5};

  خودکار c = s.آغاز_serialize<Coverage> ();

  c->serialize (&s, hb_iter (v));

  s.انتہا_serialize ();

  hb_بائٹیں_t بائٹیں = s.copy_بائٹیں ();
  hb_always_assert (بائٹیں.لمبائی == 10);
  بائٹیں.fini ();

  واپس 0;
}

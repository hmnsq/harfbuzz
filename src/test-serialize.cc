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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-serialize.hh"
#অন্তর্ভাব "hb-ot-layout-common.hh"
#অন্তর্ভাব "hb-ot-layout-gsubgpos.hh"
#অন্তর্ভাব "hb-ot-layout-gsub-table.hh"
#অন্তর্ভাব "OT/Layout/GPOS/ValueFormat.hh"

using OT::Layout::Common::Coverage;

স্থির শূন্য
test_null_condition ()
{
  স্থির ধ্রুবক অক্ষর and_condition[আকার (OT::Condition)] = {
    0, 3, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  স্থির ধ্রুবক অক্ষর or_condition[আকার (OT::Condition)] = {
    0, 4, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  স্থির ধ্রুবক অক্ষর negate_condition[আকার (OT::Condition)] = {
    0, 5,                        /* format */
    0, 0, 0,                     /* conditionOffset */
  };

  ধ্রুবক auto &condition = Null (OT::Condition);
  ধ্রুবক auto &condition_and =
      *reinterpret_cast<ধ্রুবক OT::Condition *> (and_condition);
  ধ্রুবক auto &condition_or =
      *reinterpret_cast<ধ্রুবক OT::Condition *> (or_condition);
  ধ্রুবক auto &condition_negate =
      *reinterpret_cast<ধ্রুবক OT::Condition *> (negate_condition);
  OT::ItemVarStoreInstancer *instancer = nullptr;
  hb_set_t var_indices;

  hb_always_assert (condition.evaluate (nullptr, 0, instancer));
  hb_always_assert (condition_and.evaluate (nullptr, 0, instancer));
  hb_always_assert (condition_or.evaluate (nullptr, 0, instancer));
  hb_always_assert (!condition_negate.evaluate (nullptr, 0, instancer));

  hb_always_assert (condition.collect_var_indices (&var_indices));
  hb_always_assert (condition_and.collect_var_indices (&var_indices));
  hb_always_assert (condition_or.collect_var_indices (&var_indices));
  hb_always_assert (condition_negate.collect_var_indices (&var_indices));
  hb_always_assert (var_indices.is_empty ());

  অক্ষর buf[8];
  hb_serialize_context_t s (buf, আকার (buf));
  hb_map_t varidx_map;
  স্থির ধ্রুবক unsigned অক্ষর expected[] = {
    0, 2,                        /* format */
    0, 1,                        /* defaultValue */
    0xFF, 0xFF, 0xFF, 0xFF,     /* varIdx */
  };

  auto *copy = s.start_serialize<OT::Condition> ();
  hb_always_assert (copy->serialize (&s, &condition, varidx_map));
  s.end_serialize ();

  hb_bytes_t bytes = s.copy_bytes ();
  hb_always_assert (bytes.দৈর্ঘ্য == আকার (expected));
  hb_always_assert (!hb_memcmp (bytes.arrayZ, expected, আকার (expected)));
  bytes.fini ();
}

স্থির শূন্য
test_lookup_condition_sharing ()
{
  ধ্রুবক unsigned feature_গুনতি_2 = 10922, lookup_গুনতি = 402;
  ধ্রুবক unsigned table_offset = 12 + 6 * feature_গুনতি_2;
  ধ্রুবক unsigned condition_offset = table_offset + 20 + 2 * lookup_গুনতি;
  hb_vector_t<অক্ষর> তথ্য;
  hb_always_assert (তথ্য.resize (condition_offset + 8));
  hb_memset (তথ্য.arrayZ, 0, তথ্য.দৈর্ঘ্য);
  auto put16 = [&তথ্য] (unsigned offset, unsigned value)
  {
    তথ্য[offset] = value >> 8;
    তথ্য[offset + 1] = value;
  };
  auto put32 = [&put16] (unsigned offset, unsigned value)
  {
    put16 (offset, value >> 16);
    put16 (offset + 2, value);
  };
  put32 (0, 0x00010001);
  put32 (8, feature_গুনতি_2);
  জন্য (unsigned i = 0; i < feature_গুনতি_2; i++)
  {
    put16 (12 + 6 * i, i);
    put32 (14 + 6 * i, table_offset);
  }
  put32 (table_offset, 0x00010000);
  put32 (table_offset + 6, 1); /* LookupConditionRecord count */
  put32 (table_offset + 10, condition_offset - table_offset);
  put32 (table_offset + 14, 18); /* LookupIndexList offset */
  put16 (table_offset + 18, lookup_গুনতি);
  জন্য (unsigned i = 0; i < lookup_গুনতি; i++)
    put16 (table_offset + 20 + 2 * i, i);
  put16 (condition_offset, 2); /* Constant true ConditionValue */
  put16 (condition_offset + 2, 1);
  put32 (condition_offset + 4, OT::VarIdx::NO_VARIATION);

  ধ্রুবক auto &variations = *reinterpret_cast<ধ্রুবক OT::FeatureVariations *> (তথ্য.arrayZ);
  OT::ItemVarStoreInstancer instancer (nullptr, nullptr, hb_array_t<ধ্রুবক পূর্ণসংখ্যা> ());
  hb_vector_t<unsigned> state;
  hb_always_assert (variations.collect_lookup_conditions (nullptr, 0, &instancer, &state));
  hb_always_assert (state.দৈর্ঘ্য == 1 && state[0] == condition_offset);

  জন্য (unsigned feature_সূচক : {0u, feature_গুনতি_2 - 1})
  {
    hb_set_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        feature_সূচক, Null (OT::Feature), state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == lookup_গুনতি);
    hb_always_assert (lookups.get_min () == 0 && lookups.get_max () == lookup_গুনতি - 1);
  }

  /* Null conditions need no state, even when thousands of records use them. */
  put32 (table_offset + 10, 0);
  state.reset ();
  hb_always_assert (variations.collect_lookup_conditions (nullptr, 0, &instancer, &state));
  hb_always_assert (!state.দৈর্ঘ্য);
  hb_set_t lookups;
  hb_always_assert (variations.get_lookup_variations (
      0, Null (OT::Feature), state.as_sorted_array (), &lookups));
  hb_always_assert (lookups.get_population () == lookup_গুনতি);

  put32 (table_offset + 10, condition_offset - table_offset);
  state.set_error (); /* Simulate failure allocating the condition key. */
  hb_always_assert (!variations.collect_lookup_conditions (nullptr, 0, &instancer, &state));
}

স্থির শূন্য
test_lookup_condition_resolution ()
{
  স্থির ধ্রুবক অক্ষর তথ্য[] = {
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
  স্থির ধ্রুবক অক্ষর feature_তথ্য[] = {0, 0, 0, 2, 0, 0, 0, 6};
  ধ্রুবক auto &variations = *reinterpret_cast<ধ্রুবক OT::FeatureVariations *> (তথ্য);
  ধ্রুবক auto &feature = *reinterpret_cast<ধ্রুবক OT::Feature *> (feature_তথ্য);

  জন্য (unsigned mask = 0; mask < 4; mask++)
  {
    পূর্ণসংখ্যা coords[] = {mask & 1 ? 16384 : 0, mask & 2 ? 16384 : 0};
    OT::ItemVarStoreInstancer instancer (nullptr, nullptr, hb_array (coords));
    hb_vector_t<unsigned> state;
    hb_always_assert (variations.collect_lookup_conditions (coords, 2, &instancer, &state));
    unsigned condition_গুনতি = !!(mask & 1) + !!(mask & 2);
    hb_always_assert (state.দৈর্ঘ্য == condition_গুনতি);
    যদি (mask == 3)
      hb_always_assert (state[0] == 92 && state[1] == 100);

    hb_set_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        0, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (mask & 1 ? 7 : 5));
    hb_always_assert (lookups.has (0) && lookups.has (6));
    hb_always_assert (lookups.has (4) == !!(mask & 1));

    lookups.clear ();
    hb_always_assert (variations.get_lookup_variations (
        1, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (mask == 3 ? 5 : mask ? 3 : 0));
    hb_always_assert (!lookups.has (0) && !lookups.has (6));
    hb_always_assert (lookups.has (1) == !!(mask & 1));
    hb_always_assert (lookups.has (5) == !!(mask & 2));
    hb_always_assert (!variations.get_lookup_variations (
        2, feature, state.as_sorted_array (), &lookups));
  }
}

স্থির শূন্য
test_feature_substitution_params_sanitize ()
{
  স্থির ধ্রুবক অক্ষর তথ্য[] = {
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

  hb_blob_t *blob = hb_blob_create (তথ্য, আকার (তথ্য),
				    HB_MEMORY_MODE_READONLY, nullptr, nullptr);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (hb_blob_get_দৈর্ঘ্য (blob) == আকার (তথ্য));
  hb_blob_destroy (blob);

#যদি !defined(HB_NO_VAR) && !defined(HB_NO_LAYOUT_FEATURE_PARAMS)
  blob = hb_blob_create (তথ্য, আকার (তথ্য) - 1,
			 HB_MEMORY_MODE_READONLY, nullptr, nullptr);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (!hb_blob_get_দৈর্ঘ্য (blob));
  hb_blob_destroy (blob);
#endif
}

স্থির শূন্য
test_medium_rule ()
{
  স্থির ধ্রুবক অক্ষর তথ্য[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    0, 0, 5,                    /* input glyph */
    0, 1, 0, 2,                 /* LookupRecord */
  };
  স্থির ধ্রুবক অক্ষর expected[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    1, 0, 1,                    /* input glyph */
    0, 1, 0, 3,                 /* LookupRecord */
  };
  ধ্রুবক auto &rule = *reinterpret_cast<ধ্রুবক OT::Rule<OT::Layout::MediumTypes> *> (তথ্য);

  hb_map_t glyph_map;
  glyph_map.set (5, 0x10001);
  hb_map_t lookup_map;
  lookup_map.set (2, 3);

  অক্ষর buf[32];
  hb_serialize_context_t s (buf, আকার (buf));
  s.start_serialize ();
  hb_always_assert (rule.serialize (&s, &glyph_map, &lookup_map));
  s.end_serialize ();

  hb_bytes_t bytes = s.copy_bytes ();
  hb_always_assert (bytes.দৈর্ঘ্য == আকার (expected));
  hb_always_assert (!hb_memcmp (bytes.arrayZ, expected, আকার (expected)));
  bytes.fini ();
}

স্থির শূন্য
test_medium_chain_rule ()
{
  স্থির ধ্রুবক অক্ষর তথ্য[] = {
    0, 1, 0, 0, 5,             /* backtrack */
    0, 2, 0, 0, 6,             /*প্রবেশ*/
    0, 1, 0, 0, 7,             /* lookahead */
    0, 1, 0, 0, 0, 2,          /* LookupRecord */
  };
  স্থির ধ্রুবক অক্ষর expected[] = {
    0, 1, 1, 0, 1,             /* backtrack */
    0, 2, 1, 0, 2,             /*প্রবেশ*/
    0, 1, 1, 0, 3,             /* lookahead */
    0, 1, 0, 0, 0, 3,          /* LookupRecord */
  };
  ধ্রুবক auto &rule = *reinterpret_cast<ধ্রুবক OT::ChainRule<OT::Layout::MediumTypes> *> (তথ্য);

  hb_map_t glyph_map;
  glyph_map.set (5, 0x10001);
  glyph_map.set (6, 0x10002);
  glyph_map.set (7, 0x10003);
  hb_map_t lookup_map;
  lookup_map.set (2, 3);

  অক্ষর buf[32];
  hb_serialize_context_t s (buf, আকার (buf));
  s.start_serialize ();
  hb_always_assert (rule.serialize (&s, &lookup_map, &glyph_map));
  s.end_serialize ();

  hb_bytes_t bytes = s.copy_bytes ();
  hb_always_assert (bytes.দৈর্ঘ্য == আকার (expected));
  hb_always_assert (!hb_memcmp (bytes.arrayZ, expected, আকার (expected)));
  bytes.fini ();
}

স্থির শূন্য
test_device_only_মান ()
{
  using নামাকাশ OT::Layout::GPOS_impl;
  স্থির ধ্রুবক অক্ষর তথ্য[] = {
    0, 2,                      /* Device offset */
    0, 0, 0, 0, (অক্ষর) 0x80, 0, /* VariationIndex 0 */
  };
  ধ্রুবক auto *base = reinterpret_cast<ধ্রুবক মান_Base *> (তথ্য);
  ধ্রুবক auto *values = reinterpret_cast<ধ্রুবক মান *> (তথ্য);
  জন্য (unsigned device = মান_Format::xPlaDevice;
       device <= মান_Format::yAdvDevice; device <<= 1)
    জন্য (যৌক্তিক partial : {মিথ্যা, সত্য})
    {
      মান_Format format;
      format = device;
      hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> mapping;
      mapping.set (0, hb_pair (partial ? 1u : OT::VarIdx::NO_VARIATION, 25));
      unsigned new_format = format.get_effective_format (values, মিথ্যা, মিথ্যা, base, &mapping);
      hb_always_assert (new_format == (device >> 4 | (partial ? device : 0)));

      অক্ষর buf[32];
      hb_serialize_context_t s (buf, আকার (buf));
      s.start_serialize ();
      format.copy_মানসমূহ (&s, new_format, base, values, &mapping);
      s.end_serialize ();
      hb_bytes_t bytes = s.copy_bytes ();
      hb_always_assert (!s.in_error ());
      hb_always_assert (bytes.দৈর্ঘ্য >= OT::HBINT16::static_আকার);
      hb_always_assert (*reinterpret_cast<ধ্রুবক OT::HBINT16 *> (bytes.arrayZ) == 25);
      bytes.fini ();
    }
}

স্থির শূন্য
test_failed_class_mapping ()
{
  hb_set_t klasses;
  klasses.add (1);
  hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
  glyph_and_klass.push (hb_pair (hb_codepoint_t (42), hb_codepoint_t (1)));
  hb_map_t klass_map;
  klass_map.successful = মিথ্যা; /* Simulate failure allocating the class map. */
  অক্ষর buf[32];
  hb_serialize_context_t s (buf, আকার (buf));
  s.start_serialize<OT::ClassDef> ();
  hb_always_assert (!OT::ClassDef_remap_and_serialize (
      &s, klasses, সত্য, glyph_and_klass, &klass_map));
  hb_always_assert (s.in_error () && !s.only_overflow ());
  hb_always_assert (glyph_and_klass[0].second == 1);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  test_null_condition ();
  test_lookup_condition_sharing ();
  test_lookup_condition_resolution ();
  test_feature_substitution_params_sanitize ();
  test_medium_rule ();
  test_medium_chain_rule ();
  test_device_only_মান ();
  test_failed_class_mapping ();

  অক্ষর buf[16384];

  hb_serialize_context_t s (buf, আকার (buf));

  hb_sorted_vector_t<hb_codepoint_t> v{1, 2, 5};

  auto c = s.start_serialize<Coverage> ();

  c->serialize (&s, hb_iter (v));

  s.end_serialize ();

  hb_bytes_t bytes = s.copy_bytes ();
  hb_always_assert (bytes.দৈর্ঘ্য == 10);
  bytes.fini ();

  ফেরত 0;
}

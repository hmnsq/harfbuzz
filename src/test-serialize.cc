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

#समावेश "hb.hh"
#समावेश "hb-serialize.hh"
#समावेश "hb-ot-layout-common.hh"
#समावेश "hb-ot-layout-gsubgpos.hh"
#समावेश "hb-ot-layout-gsub-table.hh"
#समावेश "OT/Layout/GPOS/ValueFormat.hh"

using OT::Layout::Common::Coverage;

अटल व्योम
test_null_condition ()
{
  अटल स्थिर अक्षर and_condition[माप (OT::Condition)] = {
    0, 3, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  अटल स्थिर अक्षर or_condition[माप (OT::Condition)] = {
    0, 4, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  अटल स्थिर अक्षर negate_condition[माप (OT::Condition)] = {
    0, 5,                        /* format */
    0, 0, 0,                     /* conditionOffset */
  };

  स्थिर स्वतः &condition = Null (OT::Condition);
  स्थिर स्वतः &condition_and =
      *reinterpret_cast<स्थिर OT::Condition *> (and_condition);
  स्थिर स्वतः &condition_or =
      *reinterpret_cast<स्थिर OT::Condition *> (or_condition);
  स्थिर स्वतः &condition_negate =
      *reinterpret_cast<स्थिर OT::Condition *> (negate_condition);
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

  अक्षर buf[8];
  hb_serialize_context_t s (buf, माप (buf));
  hb_map_t varidx_map;
  अटल स्थिर अचिन्हित अक्षर expected[] = {
    0, 2,                        /* format */
    0, 1,                        /* defaultValue */
    0xFF, 0xFF, 0xFF, 0xFF,     /* varIdx */
  };

  स्वतः *copy = s.start_serialize<OT::Condition> ();
  hb_always_assert (copy->serialize (&s, &condition, varidx_map));
  s.end_serialize ();

  hb_bytes_t bytes = s.copy_bytes ();
  hb_always_assert (bytes.लंबाई == माप (expected));
  hb_always_assert (!hb_memcmp (bytes.arrayZ, expected, माप (expected)));
  bytes.fini ();
}

अटल व्योम
test_lookup_condition_sharing ()
{
  स्थिर अचिन्हित feature_गिनती_2 = 10922, lookup_गिनती = 402;
  स्थिर अचिन्हित table_offset = 12 + 6 * feature_गिनती_2;
  स्थिर अचिन्हित condition_offset = table_offset + 20 + 2 * lookup_गिनती;
  hb_vector_t<अक्षर> तथ्य;
  hb_always_assert (तथ्य.resize (condition_offset + 8));
  hb_memset (तथ्य.arrayZ, 0, तथ्य.लंबाई);
  स्वतः put16 = [&तथ्य] (अचिन्हित offset, अचिन्हित value)
  {
    तथ्य[offset] = value >> 8;
    तथ्य[offset + 1] = value;
  };
  स्वतः put32 = [&put16] (अचिन्हित offset, अचिन्हित value)
  {
    put16 (offset, value >> 16);
    put16 (offset + 2, value);
  };
  put32 (0, 0x00010001);
  put32 (8, feature_गिनती_2);
  क्रम (अचिन्हित i = 0; i < feature_गिनती_2; i++)
  {
    put16 (12 + 6 * i, i);
    put32 (14 + 6 * i, table_offset);
  }
  put32 (table_offset, 0x00010000);
  put32 (table_offset + 6, 1); /* LookupConditionRecord count */
  put32 (table_offset + 10, condition_offset - table_offset);
  put32 (table_offset + 14, 18); /* LookupIndexList offset */
  put16 (table_offset + 18, lookup_गिनती);
  क्रम (अचिन्हित i = 0; i < lookup_गिनती; i++)
    put16 (table_offset + 20 + 2 * i, i);
  put16 (condition_offset, 2); /* Constant true ConditionValue */
  put16 (condition_offset + 2, 1);
  put32 (condition_offset + 4, OT::VarIdx::NO_VARIATION);

  स्थिर स्वतः &variations = *reinterpret_cast<स्थिर OT::FeatureVariations *> (तथ्य.arrayZ);
  OT::ItemVarStoreInstancer instancer (nullptr, nullptr, hb_array_t<स्थिर पूर्णांक> ());
  hb_vector_t<अचिन्हित> state;
  hb_always_assert (variations.collect_lookup_conditions (nullptr, 0, &instancer, &state));
  hb_always_assert (state.लंबाई == 1 && state[0] == condition_offset);

  क्रम (अचिन्हित feature_सूचकांक : {0u, feature_गिनती_2 - 1})
  {
    hb_set_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        feature_सूचकांक, Null (OT::Feature), state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == lookup_गिनती);
    hb_always_assert (lookups.get_min () == 0 && lookups.get_max () == lookup_गिनती - 1);
  }

  /* Null conditions need no state, even when thousands of records use them. */
  put32 (table_offset + 10, 0);
  state.reset ();
  hb_always_assert (variations.collect_lookup_conditions (nullptr, 0, &instancer, &state));
  hb_always_assert (!state.लंबाई);
  hb_set_t lookups;
  hb_always_assert (variations.get_lookup_variations (
      0, Null (OT::Feature), state.as_sorted_array (), &lookups));
  hb_always_assert (lookups.get_population () == lookup_गिनती);

  put32 (table_offset + 10, condition_offset - table_offset);
  state.set_error (); /* Simulate failure allocating the condition key. */
  hb_always_assert (!variations.collect_lookup_conditions (nullptr, 0, &instancer, &state));
}

अटल व्योम
test_lookup_condition_resolution ()
{
  अटल स्थिर अक्षर तथ्य[] = {
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
  अटल स्थिर अक्षर feature_तथ्य[] = {0, 0, 0, 2, 0, 0, 0, 6};
  स्थिर स्वतः &variations = *reinterpret_cast<स्थिर OT::FeatureVariations *> (तथ्य);
  स्थिर स्वतः &feature = *reinterpret_cast<स्थिर OT::Feature *> (feature_तथ्य);

  क्रम (अचिन्हित mask = 0; mask < 4; mask++)
  {
    पूर्णांक coords[] = {mask & 1 ? 16384 : 0, mask & 2 ? 16384 : 0};
    OT::ItemVarStoreInstancer instancer (nullptr, nullptr, hb_array (coords));
    hb_vector_t<अचिन्हित> state;
    hb_always_assert (variations.collect_lookup_conditions (coords, 2, &instancer, &state));
    अचिन्हित condition_गिनती = !!(mask & 1) + !!(mask & 2);
    hb_always_assert (state.लंबाई == condition_गिनती);
    अगर (mask == 3)
      hb_always_assert (state[0] == 92 && state[1] == 100);

    hb_set_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        0, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (mask & 1 ? 7 : 5));
    hb_always_assert (lookups.has (0) && lookups.has (6));
    hb_always_assert (lookups.has (4) == !!(mask & 1));

    lookups.सफा ();
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

अटल व्योम
test_feature_substitution_params_sanitize ()
{
  अटल स्थिर अक्षर तथ्य[] = {
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

  hb_blob_t *blob = hb_blob_create (तथ्य, माप (तथ्य),
				    HB_MEMORY_MODE_READONLY, nullptr, nullptr);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (hb_blob_get_लंबाई (blob) == माप (तथ्य));
  hb_blob_destroy (blob);

#अगर !defined(HB_NO_VAR) && !defined(HB_NO_LAYOUT_FEATURE_PARAMS)
  blob = hb_blob_create (तथ्य, माप (तथ्य) - 1,
			 HB_MEMORY_MODE_READONLY, nullptr, nullptr);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (!hb_blob_get_लंबाई (blob));
  hb_blob_destroy (blob);
#पूर्ण_अगर
}

अटल व्योम
test_medium_rule ()
{
  अटल स्थिर अक्षर तथ्य[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    0, 0, 5,                    /* input glyph */
    0, 1, 0, 2,                 /* LookupRecord */
  };
  अटल स्थिर अक्षर expected[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    1, 0, 1,                    /* input glyph */
    0, 1, 0, 3,                 /* LookupRecord */
  };
  स्थिर स्वतः &rule = *reinterpret_cast<स्थिर OT::Rule<OT::Layout::MediumTypes> *> (तथ्य);

  hb_map_t glyph_map;
  glyph_map.set (5, 0x10001);
  hb_map_t lookup_map;
  lookup_map.set (2, 3);

  अक्षर buf[32];
  hb_serialize_context_t s (buf, माप (buf));
  s.start_serialize ();
  hb_always_assert (rule.serialize (&s, &glyph_map, &lookup_map));
  s.end_serialize ();

  hb_bytes_t bytes = s.copy_bytes ();
  hb_always_assert (bytes.लंबाई == माप (expected));
  hb_always_assert (!hb_memcmp (bytes.arrayZ, expected, माप (expected)));
  bytes.fini ();
}

अटल व्योम
test_medium_chain_rule ()
{
  अटल स्थिर अक्षर तथ्य[] = {
    0, 1, 0, 0, 5,             /* backtrack */
    0, 2, 0, 0, 6,             /*निवेश*/
    0, 1, 0, 0, 7,             /* lookahead */
    0, 1, 0, 0, 0, 2,          /* LookupRecord */
  };
  अटल स्थिर अक्षर expected[] = {
    0, 1, 1, 0, 1,             /* backtrack */
    0, 2, 1, 0, 2,             /*निवेश*/
    0, 1, 1, 0, 3,             /* lookahead */
    0, 1, 0, 0, 0, 3,          /* LookupRecord */
  };
  स्थिर स्वतः &rule = *reinterpret_cast<स्थिर OT::ChainRule<OT::Layout::MediumTypes> *> (तथ्य);

  hb_map_t glyph_map;
  glyph_map.set (5, 0x10001);
  glyph_map.set (6, 0x10002);
  glyph_map.set (7, 0x10003);
  hb_map_t lookup_map;
  lookup_map.set (2, 3);

  अक्षर buf[32];
  hb_serialize_context_t s (buf, माप (buf));
  s.start_serialize ();
  hb_always_assert (rule.serialize (&s, &lookup_map, &glyph_map));
  s.end_serialize ();

  hb_bytes_t bytes = s.copy_bytes ();
  hb_always_assert (bytes.लंबाई == माप (expected));
  hb_always_assert (!hb_memcmp (bytes.arrayZ, expected, माप (expected)));
  bytes.fini ();
}

अटल व्योम
test_device_only_मूल्य ()
{
  using namespace OT::Layout::GPOS_impl;
  अटल स्थिर अक्षर तथ्य[] = {
    0, 2,                      /* Device offset */
    0, 0, 0, 0, (अक्षर) 0x80, 0, /* VariationIndex 0 */
  };
  स्थिर स्वतः *base = reinterpret_cast<स्थिर मूल्य_Base *> (तथ्य);
  स्थिर स्वतः *values = reinterpret_cast<स्थिर मूल्य *> (तथ्य);
  क्रम (अचिन्हित device = मूल्य_Format::xPlaDevice;
       device <= मूल्य_Format::yAdvDevice; device <<= 1)
    क्रम (bool partial : {false, true})
    {
      मूल्य_Format format;
      format = device;
      hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> mapping;
      mapping.set (0, hb_pair (partial ? 1u : OT::VarIdx::NO_VARIATION, 25));
      अचिन्हित new_format = format.get_effective_format (values, false, false, base, &mapping);
      hb_always_assert (new_format == (device >> 4 | (partial ? device : 0)));

      अक्षर buf[32];
      hb_serialize_context_t s (buf, माप (buf));
      s.start_serialize ();
      format.copy_मूल्यावली (&s, new_format, base, values, &mapping);
      s.end_serialize ();
      hb_bytes_t bytes = s.copy_bytes ();
      hb_always_assert (!s.in_error ());
      hb_always_assert (bytes.लंबाई >= OT::HBINT16::static_आकार);
      hb_always_assert (*reinterpret_cast<स्थिर OT::HBINT16 *> (bytes.arrayZ) == 25);
      bytes.fini ();
    }
}

अटल व्योम
test_failed_class_mapping ()
{
  hb_set_t klasses;
  klasses.add (1);
  hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
  glyph_and_klass.push (hb_pair (hb_codepoint_t (42), hb_codepoint_t (1)));
  hb_map_t klass_map;
  klass_map.successful = false; /* Simulate failure allocating the class map. */
  अक्षर buf[32];
  hb_serialize_context_t s (buf, माप (buf));
  s.start_serialize<OT::ClassDef> ();
  hb_always_assert (!OT::ClassDef_remap_and_serialize (
      &s, klasses, true, glyph_and_klass, &klass_map));
  hb_always_assert (s.in_error () && !s.only_overflow ());
  hb_always_assert (glyph_and_klass[0].second == 1);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  test_null_condition ();
  test_lookup_condition_sharing ();
  test_lookup_condition_resolution ();
  test_feature_substitution_params_sanitize ();
  test_medium_rule ();
  test_medium_chain_rule ();
  test_device_only_मूल्य ();
  test_failed_class_mapping ();

  अक्षर buf[16384];

  hb_serialize_context_t s (buf, माप (buf));

  hb_sorted_vector_t<hb_codepoint_t> v{1, 2, 5};

  स्वतः c = s.start_serialize<Coverage> ();

  c->serialize (&s, hb_iter (v));

  s.end_serialize ();

  hb_bytes_t bytes = s.copy_bytes ();
  hb_always_assert (bytes.लंबाई == 10);
  bytes.fini ();

  वापस 0;
}

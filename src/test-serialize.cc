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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-serialize.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsubgpos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsub-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Layout/GPOS/ValueFormat.hh"

ਵਰਤੋਂ OT::Layout::Common::Coverage;

ਸਥਿਰ ਖਾਲੀ
test_null_condition ()
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ and_condition[ਆਕਾਰ (OT::Condition)] = {
    0, 3, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ or_condition[ਆਕਾਰ (OT::Condition)] = {
    0, 4, 1,                     /* format, conditionCount */
    0, 0, 0,                     /* conditionOffset */
  };
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ negate_condition[ਆਕਾਰ (OT::Condition)] = {
    0, 5,                        /* format */
    0, 0, 0,                     /* conditionOffset */
  };

  ਅਚਲ ਸਵੈਚਾਲਿਤ &condition = Null (OT::Condition);
  ਅਚਲ ਸਵੈਚਾਲਿਤ &condition_and =
      *reinterpret_cast<ਅਚਲ OT::Condition *> (and_condition);
  ਅਚਲ ਸਵੈਚਾਲਿਤ &condition_or =
      *reinterpret_cast<ਅਚਲ OT::Condition *> (or_condition);
  ਅਚਲ ਸਵੈਚਾਲਿਤ &condition_negate =
      *reinterpret_cast<ਅਚਲ OT::Condition *> (negate_condition);
  OT::ਵਸਤੂ_Var_Store_Instancer *instancer = ਖਾਲੀ_ਸੂਚਕ;
  hb_ਸੈੱਟ_t var_indices;

  hb_always_assert (condition.evaluate (ਖਾਲੀ_ਸੂਚਕ, 0, instancer));
  hb_always_assert (condition_and.evaluate (ਖਾਲੀ_ਸੂਚਕ, 0, instancer));
  hb_always_assert (condition_or.evaluate (ਖਾਲੀ_ਸੂਚਕ, 0, instancer));
  hb_always_assert (!condition_negate.evaluate (ਖਾਲੀ_ਸੂਚਕ, 0, instancer));

  hb_always_assert (condition.collect_var_indices (&var_indices));
  hb_always_assert (condition_and.collect_var_indices (&var_indices));
  hb_always_assert (condition_or.collect_var_indices (&var_indices));
  hb_always_assert (condition_negate.collect_var_indices (&var_indices));
  hb_always_assert (var_indices.is_empty ());

  ਅੱਖਰ buf[8];
  hb_serialize_context_t s (buf, ਆਕਾਰ (buf));
  hb_map_t varidx_map;
  ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ expected[] = {
    0, 2,                        /* format */
    0, 1,                        /* defaultValue */
    0xFF, 0xFF, 0xFF, 0xFF,     /* varIdx */
  };

  ਸਵੈਚਾਲਿਤ *copy = s.ਸ਼ੁਰੂ_serialize<OT::Condition> ();
  hb_always_assert (copy->serialize (&s, &condition, varidx_map));
  s.ਸਿਰਾ_serialize ();

  hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ = s.copy_ਬਾਈਟਾਂ ();
  hb_always_assert (ਬਾਈਟਾਂ.ਲੰਮਾਈ == ਆਕਾਰ (expected));
  hb_always_assert (!hb_memcmp (ਬਾਈਟਾਂ.arrayZ, expected, ਆਕਾਰ (expected)));
  ਬਾਈਟਾਂ.fini ();
}

ਸਥਿਰ ਖਾਲੀ
test_lookup_condition_sharing ()
{
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਗਿਣਤੀ_2 = 10922, lookup_ਗਿਣਤੀ = 402;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_ਵਿੱਥ = 12 + 6 * feature_ਗਿਣਤੀ_2;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ condition_ਵਿੱਥ = ਸਾਰਣੀ_ਵਿੱਥ + 20 + 2 * lookup_ਗਿਣਤੀ;
  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> ਅੰਕੜੇ;
  hb_always_assert (ਅੰਕੜੇ.resize (condition_ਵਿੱਥ + 8));
  hb_memset (ਅੰਕੜੇ.arrayZ, 0, ਅੰਕੜੇ.ਲੰਮਾਈ);
  ਸਵੈਚਾਲਿਤ put16 = [&ਅੰਕੜੇ] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ value)
  {
    ਅੰਕੜੇ[ਵਿੱਥ] = value >> 8;
    ਅੰਕੜੇ[ਵਿੱਥ + 1] = value;
  };
  ਸਵੈਚਾਲਿਤ put32 = [&put16] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ value)
  {
    put16 (ਵਿੱਥ, value >> 16);
    put16 (ਵਿੱਥ + 2, value);
  };
  put32 (0, 0x00010001);
  put32 (8, feature_ਗਿਣਤੀ_2);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < feature_ਗਿਣਤੀ_2; i++)
  {
    put16 (12 + 6 * i, i);
    put32 (14 + 6 * i, ਸਾਰਣੀ_ਵਿੱਥ);
  }
  put32 (ਸਾਰਣੀ_ਵਿੱਥ, 0x00010000);
  put32 (ਸਾਰਣੀ_ਵਿੱਥ + 6, 1); /* LookupConditionRecord count */
  put32 (ਸਾਰਣੀ_ਵਿੱਥ + 10, condition_ਵਿੱਥ - ਸਾਰਣੀ_ਵਿੱਥ);
  put32 (ਸਾਰਣੀ_ਵਿੱਥ + 14, 18); /* LookupIndexList offset */
  put16 (ਸਾਰਣੀ_ਵਿੱਥ + 18, lookup_ਗਿਣਤੀ);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < lookup_ਗਿਣਤੀ; i++)
    put16 (ਸਾਰਣੀ_ਵਿੱਥ + 20 + 2 * i, i);
  put16 (condition_ਵਿੱਥ, 2); /* Constant true ConditionValue */
  put16 (condition_ਵਿੱਥ + 2, 1);
  put32 (condition_ਵਿੱਥ + 4, OT::VarIdx::NO_VARIATION);

  ਅਚਲ ਸਵੈਚਾਲਿਤ &variations = *reinterpret_cast<ਅਚਲ OT::FeatureVariations *> (ਅੰਕੜੇ.arrayZ);
  OT::ਵਸਤੂ_Var_Store_Instancer instancer (ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, hb_array_t<ਅਚਲ ਪੂਰਨ> ());
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> state;
  hb_always_assert (variations.collect_lookup_conditions (ਖਾਲੀ_ਸੂਚਕ, 0, &instancer, &state));
  hb_always_assert (state.ਲੰਮਾਈ == 1 && state[0] == condition_ਵਿੱਥ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ : {0u, feature_ਗਿਣਤੀ_2 - 1})
  {
    hb_ਸੈੱਟ_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        feature_ਸੂਚਕਾਂਕ, Null (OT::Feature), state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == lookup_ਗਿਣਤੀ);
    hb_always_assert (lookups.get_min () == 0 && lookups.get_max () == lookup_ਗਿਣਤੀ - 1);
  }

  /* Null conditions need no state, even when thousands of records use them. */
  put32 (ਸਾਰਣੀ_ਵਿੱਥ + 10, 0);
  state.reset ();
  hb_always_assert (variations.collect_lookup_conditions (ਖਾਲੀ_ਸੂਚਕ, 0, &instancer, &state));
  hb_always_assert (!state.ਲੰਮਾਈ);
  hb_ਸੈੱਟ_t lookups;
  hb_always_assert (variations.get_lookup_variations (
      0, Null (OT::Feature), state.as_sorted_array (), &lookups));
  hb_always_assert (lookups.get_population () == lookup_ਗਿਣਤੀ);

  put32 (ਸਾਰਣੀ_ਵਿੱਥ + 10, condition_ਵਿੱਥ - ਸਾਰਣੀ_ਵਿੱਥ);
  state.ਸੈੱਟ_error (); /* Simulate failure allocating the condition key. */
  hb_always_assert (!variations.collect_lookup_conditions (ਖਾਲੀ_ਸੂਚਕ, 0, &instancer, &state));
}

ਸਥਿਰ ਖਾਲੀ
test_lookup_condition_resolution ()
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਅੰਕੜੇ[] = {
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
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ feature_ਅੰਕੜੇ[] = {0, 0, 0, 2, 0, 0, 0, 6};
  ਅਚਲ ਸਵੈਚਾਲਿਤ &variations = *reinterpret_cast<ਅਚਲ OT::FeatureVariations *> (ਅੰਕੜੇ);
  ਅਚਲ ਸਵੈਚਾਲਿਤ &feature = *reinterpret_cast<ਅਚਲ OT::Feature *> (feature_ਅੰਕੜੇ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ = 0; ਨਕਾਬ < 4; ਨਕਾਬ++)
  {
    ਪੂਰਨ coords[] = {ਨਕਾਬ & 1 ? 16384 : 0, ਨਕਾਬ & 2 ? 16384 : 0};
    OT::ਵਸਤੂ_Var_Store_Instancer instancer (ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, hb_array (coords));
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> state;
    hb_always_assert (variations.collect_lookup_conditions (coords, 2, &instancer, &state));
    ਬਿਨਾਨਿਸ਼ਾਨੀ condition_ਗਿਣਤੀ = !!(ਨਕਾਬ & 1) + !!(ਨਕਾਬ & 2);
    hb_always_assert (state.ਲੰਮਾਈ == condition_ਗਿਣਤੀ);
    ਜੇ (ਨਕਾਬ == 3)
      hb_always_assert (state[0] == 92 && state[1] == 100);

    hb_ਸੈੱਟ_t lookups;
    hb_always_assert (variations.get_lookup_variations (
        0, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (ਨਕਾਬ & 1 ? 7 : 5));
    hb_always_assert (lookups.has (0) && lookups.has (6));
    hb_always_assert (lookups.has (4) == !!(ਨਕਾਬ & 1));

    lookups.clear ();
    hb_always_assert (variations.get_lookup_variations (
        1, feature, state.as_sorted_array (), &lookups));
    hb_always_assert (lookups.get_population () == (ਨਕਾਬ == 3 ? 5 : ਨਕਾਬ ? 3 : 0));
    hb_always_assert (!lookups.has (0) && !lookups.has (6));
    hb_always_assert (lookups.has (1) == !!(ਨਕਾਬ & 1));
    hb_always_assert (lookups.has (5) == !!(ਨਕਾਬ & 2));
    hb_always_assert (!variations.get_lookup_variations (
        2, feature, state.as_sorted_array (), &lookups));
  }
}

ਸਥਿਰ ਖਾਲੀ
test_feature_substitution_params_sanitize ()
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਅੰਕੜੇ[] = {
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

  hb_blob_t *blob = hb_blob_create (ਅੰਕੜੇ, ਆਕਾਰ (ਅੰਕੜੇ),
				    HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (hb_blob_get_ਲੰਮਾਈ (blob) == ਆਕਾਰ (ਅੰਕੜੇ));
  hb_blob_destroy (blob);

#ਜੇ !defined(HB_NO_VAR) && !defined(HB_NO_LAYOUT_FEATURE_PARAMS)
  blob = hb_blob_create (ਅੰਕੜੇ, ਆਕਾਰ (ਅੰਕੜੇ) - 1,
			 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  blob = hb_sanitize_context_t ().sanitize_blob<OT::Layout::GSUB> (blob);
  hb_always_assert (!hb_blob_get_ਲੰਮਾਈ (blob));
  hb_blob_destroy (blob);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ
test_medium_ਨਿਯਮ ()
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਅੰਕੜੇ[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    0, 0, 5,                    /* input glyph */
    0, 1, 0, 2,                 /* LookupRecord */
  };
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ expected[] = {
    0, 2, 0, 1,                 /* inputCount, lookupCount */
    1, 0, 1,                    /* input glyph */
    0, 1, 0, 3,                 /* LookupRecord */
  };
  ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਯਮ_2 = *reinterpret_cast<ਅਚਲ OT::ਨਿਯਮ_3<OT::Layout::MediumTypes> *> (ਅੰਕੜੇ);

  hb_map_t glyph_map;
  glyph_map.ਸੈੱਟ (5, 0x10001);
  hb_map_t lookup_map;
  lookup_map.ਸੈੱਟ (2, 3);

  ਅੱਖਰ buf[32];
  hb_serialize_context_t s (buf, ਆਕਾਰ (buf));
  s.ਸ਼ੁਰੂ_serialize ();
  hb_always_assert (ਨਿਯਮ_2.serialize (&s, &glyph_map, &lookup_map));
  s.ਸਿਰਾ_serialize ();

  hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ = s.copy_ਬਾਈਟਾਂ ();
  hb_always_assert (ਬਾਈਟਾਂ.ਲੰਮਾਈ == ਆਕਾਰ (expected));
  hb_always_assert (!hb_memcmp (ਬਾਈਟਾਂ.arrayZ, expected, ਆਕਾਰ (expected)));
  ਬਾਈਟਾਂ.fini ();
}

ਸਥਿਰ ਖਾਲੀ
test_medium_chain_ਨਿਯਮ ()
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਅੰਕੜੇ[] = {
    0, 1, 0, 0, 5,             /* backtrack */
    0, 2, 0, 0, 6,             /*ਨਿਵੇਸ਼*/
    0, 1, 0, 0, 7,             /* lookahead */
    0, 1, 0, 0, 0, 2,          /* LookupRecord */
  };
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ expected[] = {
    0, 1, 1, 0, 1,             /* backtrack */
    0, 2, 1, 0, 2,             /*ਨਿਵੇਸ਼*/
    0, 1, 1, 0, 3,             /* lookahead */
    0, 1, 0, 0, 0, 3,          /* LookupRecord */
  };
  ਅਚਲ ਸਵੈਚਾਲਿਤ &ਨਿਯਮ_2 = *reinterpret_cast<ਅਚਲ OT::Chain_ਨਿਯਮ<OT::Layout::MediumTypes> *> (ਅੰਕੜੇ);

  hb_map_t glyph_map;
  glyph_map.ਸੈੱਟ (5, 0x10001);
  glyph_map.ਸੈੱਟ (6, 0x10002);
  glyph_map.ਸੈੱਟ (7, 0x10003);
  hb_map_t lookup_map;
  lookup_map.ਸੈੱਟ (2, 3);

  ਅੱਖਰ buf[32];
  hb_serialize_context_t s (buf, ਆਕਾਰ (buf));
  s.ਸ਼ੁਰੂ_serialize ();
  hb_always_assert (ਨਿਯਮ_2.serialize (&s, &lookup_map, &glyph_map));
  s.ਸਿਰਾ_serialize ();

  hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ = s.copy_ਬਾਈਟਾਂ ();
  hb_always_assert (ਬਾਈਟਾਂ.ਲੰਮਾਈ == ਆਕਾਰ (expected));
  hb_always_assert (!hb_memcmp (ਬਾਈਟਾਂ.arrayZ, expected, ਆਕਾਰ (expected)));
  ਬਾਈਟਾਂ.fini ();
}

ਸਥਿਰ ਖਾਲੀ
test_device_only_ਮੁੱਲ ()
{
  ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT::Layout::GPOS_impl;
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਅੰਕੜੇ[] = {
    0, 2,                      /* Device offset */
    0, 0, 0, 0, (ਅੱਖਰ) 0x80, 0, /* VariationIndex 0 */
  };
  ਅਚਲ ਸਵੈਚਾਲਿਤ *ਅਧਾਰ = reinterpret_cast<ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *> (ਅੰਕੜੇ);
  ਅਚਲ ਸਵੈਚਾਲਿਤ *values = reinterpret_cast<ਅਚਲ ਮੁੱਲ *> (ਅੰਕੜੇ);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ device = ਮੁੱਲ_Format::xPlaDevice;
       device <= ਮੁੱਲ_Format::yAdvDevice; device <<= 1)
    ਲਈ (ਤਾਰਕਿਕ partial : {ਝੂਠ, ਸੱਚ})
    {
      ਮੁੱਲ_Format format;
      format = device;
      hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> mapping;
      mapping.ਸੈੱਟ (0, hb_pair (partial ? 1u : OT::VarIdx::NO_VARIATION, 25));
      ਬਿਨਾਨਿਸ਼ਾਨੀ new_format = format.get_effective_format (values, ਝੂਠ, ਝੂਠ, ਅਧਾਰ, &mapping);
      hb_always_assert (new_format == (device >> 4 | (partial ? device : 0)));

      ਅੱਖਰ buf[32];
      hb_serialize_context_t s (buf, ਆਕਾਰ (buf));
      s.ਸ਼ੁਰੂ_serialize ();
      format.copy_ਮੁੱਲਾਂ (&s, new_format, ਅਧਾਰ, values, &mapping);
      s.ਸਿਰਾ_serialize ();
      hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ = s.copy_ਬਾਈਟਾਂ ();
      hb_always_assert (!s.ਵਿੱਚ_error ());
      hb_always_assert (ਬਾਈਟਾਂ.ਲੰਮਾਈ >= OT::HBINT16::static_ਮਾਪ);
      hb_always_assert (*reinterpret_cast<ਅਚਲ OT::HBINT16 *> (ਬਾਈਟਾਂ.arrayZ) == 25);
      ਬਾਈਟਾਂ.fini ();
    }
}

ਸਥਿਰ ਖਾਲੀ
test_failed_class_mapping ()
{
  hb_ਸੈੱਟ_t klasses;
  klasses.ਜੋੜੋ (1);
  hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_pair_t> glyph_and_klass;
  glyph_and_klass.push (hb_pair (hb_codepoint_t (42), hb_codepoint_t (1)));
  hb_map_t klass_map;
  klass_map.successful = ਝੂਠ; /* Simulate failure allocating the class map. */
  ਅੱਖਰ buf[32];
  hb_serialize_context_t s (buf, ਆਕਾਰ (buf));
  s.ਸ਼ੁਰੂ_serialize<OT::ClassDef> ();
  hb_always_assert (!OT::ClassDef_remap_and_serialize (
      &s, klasses, ਸੱਚ, glyph_and_klass, &klass_map));
  hb_always_assert (s.ਵਿੱਚ_error () && !s.only_ਉਛਾਲ ());
  hb_always_assert (glyph_and_klass[0].second == 1);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  test_null_condition ();
  test_lookup_condition_sharing ();
  test_lookup_condition_resolution ();
  test_feature_substitution_params_sanitize ();
  test_medium_ਨਿਯਮ ();
  test_medium_chain_ਨਿਯਮ ();
  test_device_only_ਮੁੱਲ ();
  test_failed_class_mapping ();

  ਅੱਖਰ buf[16384];

  hb_serialize_context_t s (buf, ਆਕਾਰ (buf));

  hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> v{1, 2, 5};

  ਸਵੈਚਾਲਿਤ c = s.ਸ਼ੁਰੂ_serialize<Coverage> ();

  c->serialize (&s, hb_iter (v));

  s.ਸਿਰਾ_serialize ();

  hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ = s.copy_ਬਾਈਟਾਂ ();
  hb_always_assert (ਬਾਈਟਾਂ.ਲੰਮਾਈ == 10);
  ਬਾਈਟਾਂ.fini ();

  ਵਾਪਸ 0;
}

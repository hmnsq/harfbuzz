/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_FEAT_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_FEAT_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_feat HB_ਟੈਗ('f','e','a','t')


ਨਾਮਾਕਾਸ਼ AAT {


ਬਣਤਰ SettingName
{
  ਮਿੱਤਰ ਬਣਤਰ FeatureName;

  ਪੂਰਨ cmp (hb_aat_layout_feature_selector_t key) ਅਚਲ
  { ਵਾਪਸ (ਪੂਰਨ) key - (ਪੂਰਨ) setting; }

  hb_aat_layout_feature_selector_t get_selector () ਅਚਲ
  { ਵਾਪਸ (hb_aat_layout_feature_selector_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) ਅਚਲ
  {
    ਵਾਪਸ {
      name_ਸੂਚਕਾਂਕ,
      (hb_aat_layout_feature_selector_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	setting;	/* The setting. */
  NameID	name_ਸੂਚਕਾਂਕ;	/* The name table index for the setting's name. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};
DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ (AAT, SettingName);

ਬਣਤਰ feat;

ਬਣਤਰ FeatureName
{
  ਪੂਰਨ cmp (hb_aat_layout_feature_type_t key) ਅਚਲ
  { ਵਾਪਸ (ਪੂਰਨ) key - (ਪੂਰਨ) feature; }

  ਗਿਣਤੀ {
    Exclusive	= 0x8000,	/* If set, the feature settings are mutually exclusive. */
    NotDefault	= 0x4000,	/* If clear, then the setting with an index of 0 in
				 * the setting name array for this feature should
				 * be taken as the default for the feature
				 * (if one is required). If set, then bits 0-15 of this
				 * featureFlags field contain the index of the setting
				 * which is to be taken as the default. */
    ਸੂਚਕਾਂਕ_ਨਕਾਬ	= 0x00FF	/* If bits 30 and 31 are set, then these sixteen bits
				 * indicate the index of the setting in the setting name
				 * array for this feature which should be taken
				 * as the default. */
  };

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_selector_infos (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                           ਸ਼ੁਰੂ_ਵਿੱਥ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                          *selectors_ਗਿਣਤੀ, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                          *pdefault_ਸੂਚਕਾਂਕ,  /* OUT.     May be NULL. */
				   ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    hb_array_t< ਅਚਲ SettingName> settings_ਸਾਰਣੀ = (ਅਧਾਰ+setting_ਸਾਰਣੀ_Z).as_array (nSettings);

    static_assert (ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ == HB_AAT_LAYOUT_NO_SELECTOR_ਸੂਚਕਾਂਕ, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ default_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ;
    ਜੇ (featureFlags & Exclusive)
    {
      default_ਸੂਚਕਾਂਕ = (featureFlags & NotDefault) ? featureFlags & ਸੂਚਕਾਂਕ_ਨਕਾਬ : 0;
      default_selector = settings_ਸਾਰਣੀ[default_ਸੂਚਕਾਂਕ].get_selector ();
    }
    ਜੇ (pdefault_ਸੂਚਕਾਂਕ)
      *pdefault_ਸੂਚਕਾਂਕ = default_ਸੂਚਕਾਂਕ;

    ਜੇ (selectors_ਗਿਣਤੀ && selectors)
    {
      + settings_ਸਾਰਣੀ.sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, selectors_ਗਿਣਤੀ)
      | hb_map ([=] (ਅਚਲ SettingName& setting) { ਵਾਪਸ setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_ਗਿਣਤੀ))
      ;
    }
    ਵਾਪਸ settings_ਸਾਰਣੀ.ਲੰਮਾਈ;
  }

  hb_aat_layout_feature_type_t get_feature_type () ਅਚਲ
  { ਵਾਪਸ (hb_aat_layout_feature_type_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) feature; }

  hb_ot_name_id_t get_feature_name_id () ਅਚਲ { ਵਾਪਸ name_ਸੂਚਕਾਂਕ; }

  ਤਾਰਕਿਕ is_exclusive () ਅਚਲ { ਵਾਪਸ featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ nSettings; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  (ਅਧਾਰ+setting_ਸਾਰਣੀ_Z).sanitize (c, nSettings)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<SettingName>>
		setting_ਸਾਰਣੀ_Z;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	name_ਸੂਚਕਾਂਕ;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

ਬਣਤਰ feat
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_AAT_ਟੈਗ_feat;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_feature_types (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                  ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                 *ਗਿਣਤੀ_2,
				  hb_aat_layout_feature_type_t *features) ਅਚਲ
  {
    ਜੇ (ਗਿਣਤੀ_2 && features)
    {
      + ਨਾਂ_Z.as_array (feature_Name_ਗਿਣਤੀ).sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਗਿਣਤੀ_2)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *ਗਿਣਤੀ_2))
      ;
    }
    ਵਾਪਸ feature_Name_ਗਿਣਤੀ;
  }

  ਤਾਰਕਿਕ exposes_feature (hb_aat_layout_feature_type_t feature_type) ਅਚਲ
  { ਵਾਪਸ get_feature (feature_type).has_ਅੰਕੜੇ (); }

  ਅਚਲ FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) ਅਚਲ
  { ਵਾਪਸ ਨਾਂ_Z.bsearch (feature_Name_ਗਿਣਤੀ, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) ਅਚਲ
  { ਵਾਪਸ get_feature (feature).get_feature_name_id (); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                           ਸ਼ੁਰੂ_ਵਿੱਥ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                          *selectors_ਗਿਣਤੀ, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                          *default_ਸੂਚਕਾਂਕ    /* OUT.     May be NULL. */) ਅਚਲ
  {
    ਵਾਪਸ get_feature (feature_type).get_selector_infos (ਸ਼ੁਰੂ_ਵਿੱਥ, selectors_ਗਿਣਤੀ, selectors,
							  default_ਸੂਚਕਾਂਕ, ਇਹ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਸੰਸਕਰਣ.major == 1 &&
			  ਨਾਂ_Z.sanitize (c, feature_Name_ਗਿਣਤੀ, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_ਗਿਣਤੀ;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  Sorted_Unsized_Array_ਦਾ<FeatureName>
		ਨਾਂ_Z;		/* The feature name array. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (12, ਨਾਂ_Z);
};

} /* namespace AAT */

#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

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

#ifndef HB_AAT_LAYOUT_FEAT_TABLE_HH
#define HB_AAT_LAYOUT_FEAT_TABLE_HH

#অন্তর্ভাব "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#define HB_AAT_TAG_feat HB_TAG('f','e','a','t')


নামাকাশ AAT {


কাঠামো SettingName
{
  friend কাঠামো FeatureName;

  পূর্ণসংখ্যা cmp (hb_aat_layout_feature_selector_t key) ধ্রুবক
  { ফেরত (পূর্ণসংখ্যা) key - (পূর্ণসংখ্যা) setting; }

  hb_aat_layout_feature_selector_t get_selector () ধ্রুবক
  { ফেরত (hb_aat_layout_feature_selector_t) (unsigned) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) ধ্রুবক
  {
    ফেরত {
      name_সূচক,
      (hb_aat_layout_feature_selector_t) (unsigned পূর্ণসংখ্যা) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	setting;	/* The setting. */
  NameID	name_সূচক;	/* The name table index for the setting's name. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};
DECLARE_NULL_NAMESPACE_BYTES (AAT, SettingName);

কাঠামো feat;

কাঠামো FeatureName
{
  পূর্ণসংখ্যা cmp (hb_aat_layout_feature_type_t key) ধ্রুবক
  { ফেরত (পূর্ণসংখ্যা) key - (পূর্ণসংখ্যা) feature; }

  গণনা {
    Exclusive	= 0x8000,	/* If set, the feature settings are mutually exclusive. */
    NotDefault	= 0x4000,	/* If clear, then the setting with an index of 0 in
				 * the setting name array for this feature should
				 * be taken as the default for the feature
				 * (if one is required). If set, then bits 0-15 of this
				 * featureFlags field contain the index of the setting
				 * which is to be taken as the default. */
    সূচক_Mask	= 0x00FF	/* If bits 30 and 31 are set, then these sixteen bits
				 * indicate the index of the setting in the setting name
				 * array for this feature which should be taken
				 * as the default. */
  };

  unsigned পূর্ণসংখ্যা get_selector_infos (unsigned পূর্ণসংখ্যা                           start_offset,
				   unsigned পূর্ণসংখ্যা                          *selectors_গুনতি, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned পূর্ণসংখ্যা                          *pdefault_সূচক,  /* OUT.     May be NULL. */
				   ধ্রুবক শূন্য *base) ধ্রুবক
  {
    hb_array_t< ধ্রুবক SettingName> settings_table = (base+settingTableZ).as_array (nSettings);

    static_assert (সূচক::NOT_FOUND_সূচক == HB_AAT_LAYOUT_NO_SELECTOR_সূচক, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    unsigned পূর্ণসংখ্যা default_সূচক = সূচক::NOT_FOUND_সূচক;
    যদি (featureFlags & Exclusive)
    {
      default_সূচক = (featureFlags & NotDefault) ? featureFlags & সূচক_Mask : 0;
      default_selector = settings_table[default_সূচক].get_selector ();
    }
    যদি (pdefault_সূচক)
      *pdefault_সূচক = default_সূচক;

    যদি (selectors_গুনতি && selectors)
    {
      + settings_table.sub_array (start_offset, selectors_গুনতি)
      | hb_map ([=] (ধ্রুবক SettingName& setting) { ফেরত setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_গুনতি))
      ;
    }
    ফেরত settings_table.দৈর্ঘ্য;
  }

  hb_aat_layout_feature_type_t get_feature_type () ধ্রুবক
  { ফেরত (hb_aat_layout_feature_type_t) (unsigned পূর্ণসংখ্যা) feature; }

  hb_ot_name_id_t get_feature_name_id () ধ্রুবক { ফেরত name_সূচক; }

  যৌক্তিক is_exclusive () ধ্রুবক { ফেরত featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত nSettings; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  (base+settingTableZ).sanitize (c, nSettings)));
  }

  সুরক্ষিত:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32To<UnsizedArrayOf<SettingName>>
		settingTableZ;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	name_সূচক;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

কাঠামো feat
{
  স্থির constexpr hb_tag_t tableTag = HB_AAT_TAG_feat;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }

  unsigned পূর্ণসংখ্যা get_feature_types (unsigned পূর্ণসংখ্যা                  start_offset,
				  unsigned পূর্ণসংখ্যা                 *গুনতি,
				  hb_aat_layout_feature_type_t *features) ধ্রুবক
  {
    যদি (গুনতি && features)
    {
      + namesZ.as_array (feature_Name_গুনতি).sub_array (start_offset, গুনতি)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *গুনতি))
      ;
    }
    ফেরত feature_Name_গুনতি;
  }

  যৌক্তিক exposes_feature (hb_aat_layout_feature_type_t feature_type) ধ্রুবক
  { ফেরত get_feature (feature_type).has_তথ্য (); }

  ধ্রুবক FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) ধ্রুবক
  { ফেরত namesZ.bsearch (feature_Name_গুনতি, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) ধ্রুবক
  { ফেরত get_feature (feature).get_feature_name_id (); }

  unsigned পূর্ণসংখ্যা get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   unsigned পূর্ণসংখ্যা                           start_offset,
				   unsigned পূর্ণসংখ্যা                          *selectors_গুনতি, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned পূর্ণসংখ্যা                          *default_সূচক    /* OUT.     May be NULL. */) ধ্রুবক
  {
    ফেরত get_feature (feature_type).get_selector_infos (start_offset, selectors_গুনতি, selectors,
							  default_সূচক, এই);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  namesZ.sanitize (c, feature_Name_গুনতি, এই)));
  }

  সুরক্ষিত:
  FixedVersion<>version;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_গুনতি;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  SortedUnsizedArrayOf<FeatureName>
		namesZ;		/* The feature name array. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (12, namesZ);
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

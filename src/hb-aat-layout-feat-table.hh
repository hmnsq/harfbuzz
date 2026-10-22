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

#جيڪڏهن_وصف_نه_ٿيل HB_AAT_LAYOUT_FEAT_جدول_HH
#وصف_ڪريو HB_AAT_LAYOUT_FEAT_جدول_HH

#ضمن "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#وصف_ڪريو HB_AAT_ٽيگ_feat HB_ٽيگ('f','e','a','t')


نطاق AAT {


بنية SettingName
{
  دوست بنية FeatureName;

  صحيح cmp (hb_aat_layout_feature_selector_t key) ثابت
  { أرجع (صحيح) key - (صحيح) setting; }

  hb_aat_layout_feature_selector_t get_selector () ثابت
  { أرجع (hb_aat_layout_feature_selector_t) (بنانشان) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) ثابت
  {
    أرجع {
      name_اشاريو,
      (hb_aat_layout_feature_selector_t) (بنانشان صحيح) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	setting;	/* The setting. */
  NameID	name_اشاريو;	/* The name table index for the setting's name. */
  عام:
  DEFINE_سائيز_STATIC (4);
};
DECLARE_NULL_NAMESPACE_بائيٽون (AAT, SettingName);

بنية feat;

بنية FeatureName
{
  صحيح cmp (hb_aat_layout_feature_type_t key) ثابت
  { أرجع (صحيح) key - (صحيح) feature; }

  تعداد {
    Exclusive	= 0x8000,	/* If set, the feature settings are mutually exclusive. */
    NotDefault	= 0x4000,	/* If clear, then the setting with an index of 0 in
				 * the setting name array for this feature should
				 * be taken as the default for the feature
				 * (if one is required). If set, then bits 0-15 of this
				 * featureFlags field contain the index of the setting
				 * which is to be taken as the default. */
    اشاريو_نقاب	= 0x00FF	/* If bits 30 and 31 are set, then these sixteen bits
				 * indicate the index of the setting in the setting name
				 * array for this feature which should be taken
				 * as the default. */
  };

  بنانشان صحيح get_selector_infos (بنانشان صحيح                           شروعات_وٿي,
				   بنانشان صحيح                          *selectors_ڳڻپ, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   بنانشان صحيح                          *pdefault_اشاريو,  /* OUT.     May be NULL. */
				   ثابت فراغ *بنياد) ثابت
  {
    hb_array_t< ثابت SettingName> settings_جدول = (بنياد+setting_جدول_Z).as_array (nSettings);

    static_assert (اشاريو::NOT_مليو_اشاريو == HB_AAT_LAYOUT_NO_SELECTOR_اشاريو, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    بنانشان صحيح default_اشاريو = اشاريو::NOT_مليو_اشاريو;
    إذا (featureFlags & Exclusive)
    {
      default_اشاريو = (featureFlags & NotDefault) ? featureFlags & اشاريو_نقاب : 0;
      default_selector = settings_جدول[default_اشاريو].get_selector ();
    }
    إذا (pdefault_اشاريو)
      *pdefault_اشاريو = default_اشاريو;

    إذا (selectors_ڳڻپ && selectors)
    {
      + settings_جدول.sub_array (شروعات_وٿي, selectors_ڳڻپ)
      | hb_map ([=] (ثابت SettingName& setting) { أرجع setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_ڳڻپ))
      ;
    }
    أرجع settings_جدول.ڊيگهائي;
  }

  hb_aat_layout_feature_type_t get_feature_type () ثابت
  { أرجع (hb_aat_layout_feature_type_t) (بنانشان صحيح) feature; }

  hb_ot_name_id_t get_feature_name_id () ثابت { أرجع name_اشاريو; }

  منطقي is_exclusive () ثابت { أرجع featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  منطقي has_معلومات () ثابت { أرجع nSettings; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (بنياد+setting_جدول_Z).sanitize (c, nSettings)));
  }

  محمي:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32_منزل<Unsized_Array_جو<SettingName>>
		setting_جدول_Z;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	name_اشاريو;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  عام:
  DEFINE_سائيز_STATIC (12);
};

بنية feat
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_AAT_ٽيگ_feat;

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }

  بنانشان صحيح get_feature_types (بنانشان صحيح                  شروعات_وٿي,
				  بنانشان صحيح                 *ڳڻپ,
				  hb_aat_layout_feature_type_t *features) ثابت
  {
    إذا (ڳڻپ && features)
    {
      + نالا_Z.as_array (feature_Name_ڳڻپ).sub_array (شروعات_وٿي, ڳڻپ)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *ڳڻپ))
      ;
    }
    أرجع feature_Name_ڳڻپ;
  }

  منطقي exposes_feature (hb_aat_layout_feature_type_t feature_type) ثابت
  { أرجع get_feature (feature_type).has_معلومات (); }

  ثابت FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) ثابت
  { أرجع نالا_Z.bsearch (feature_Name_ڳڻپ, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) ثابت
  { أرجع get_feature (feature).get_feature_name_id (); }

  بنانشان صحيح get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   بنانشان صحيح                           شروعات_وٿي,
				   بنانشان صحيح                          *selectors_ڳڻپ, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   بنانشان صحيح                          *default_اشاريو    /* OUT.     May be NULL. */) ثابت
  {
    أرجع get_feature (feature_type).get_selector_infos (شروعات_وٿي, selectors_ڳڻپ, selectors,
							  default_اشاريو, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخو.major == 1 &&
			  نالا_Z.sanitize (c, feature_Name_ڳڻپ, هذا)));
  }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_ڳڻپ;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  Sorted_Unsized_Array_جو<FeatureName>
		نالا_Z;		/* The feature name array. */
  عام:
  DEFINE_سائيز_ARRAY (12, نالا_Z);
};

} /* namespace AAT */

#جيڪڏهن_پورو /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

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

#جے_بنا_تعریف HB_AAT_LAYOUT_FEAT_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_FEAT_جدول_HH

#ضمن "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#تعریف_کرو HB_AAT_ٹیگ_feat HB_ٹیگ('f','e','a','t')


نطاق AAT {


بنية SettingName
{
  یار بنية FeatureName;

  صحيح cmp (hb_aat_layout_feature_selector_t key) ثابت
  { أرجع (صحيح) key - (صحيح) setting; }

  hb_aat_layout_feature_selector_t get_selector () ثابت
  { أرجع (hb_aat_layout_feature_selector_t) (بنانشانی) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) ثابت
  {
    أرجع {
      name_اشاریہ,
      (hb_aat_layout_feature_selector_t) (بنانشانی صحيح) setting,
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
  NameID	name_اشاریہ;	/* The name table index for the setting's name. */
  عام:
  DEFINE_ناپ_STATIC (4);
};
DECLARE_NULL_NAMESPACE_بائٹاں (AAT, SettingName);

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
    اشاریہ_نقاب	= 0x00FF	/* If bits 30 and 31 are set, then these sixteen bits
				 * indicate the index of the setting in the setting name
				 * array for this feature which should be taken
				 * as the default. */
  };

  بنانشانی صحيح get_selector_infos (بنانشانی صحيح                           شروع_وتھ,
				   بنانشانی صحيح                          *selectors_گنتی, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   بنانشانی صحيح                          *pdefault_اشاریہ,  /* OUT.     May be NULL. */
				   ثابت فراغ *ادھار) ثابت
  {
    hb_array_t< ثابت SettingName> settings_جدول = (ادھار+setting_جدول_Z).as_array (nSettings);

    static_assert (اشاریہ::NOT_لبھیا_اشاریہ == HB_AAT_LAYOUT_NO_SELECTOR_اشاریہ, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    بنانشانی صحيح default_اشاریہ = اشاریہ::NOT_لبھیا_اشاریہ;
    إذا (featureFlags & Exclusive)
    {
      default_اشاریہ = (featureFlags & NotDefault) ? featureFlags & اشاریہ_نقاب : 0;
      default_selector = settings_جدول[default_اشاریہ].get_selector ();
    }
    إذا (pdefault_اشاریہ)
      *pdefault_اشاریہ = default_اشاریہ;

    إذا (selectors_گنتی && selectors)
    {
      + settings_جدول.sub_array (شروع_وتھ, selectors_گنتی)
      | hb_map ([=] (ثابت SettingName& setting) { أرجع setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_گنتی))
      ;
    }
    أرجع settings_جدول.لمائی;
  }

  hb_aat_layout_feature_type_t get_feature_type () ثابت
  { أرجع (hb_aat_layout_feature_type_t) (بنانشانی صحيح) feature; }

  hb_ot_name_id_t get_feature_name_id () ثابت { أرجع name_اشاریہ; }

  منطقي is_exclusive () ثابت { أرجع featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  منطقي has_انکڑے () ثابت { أرجع nSettings; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (ادھار+setting_جدول_Z).sanitize (c, nSettings)));
  }

  محمي:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32_منزل<Unsized_Array_دا<SettingName>>
		setting_جدول_Z;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	name_اشاریہ;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  عام:
  DEFINE_ناپ_STATIC (12);
};

بنية feat
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_AAT_ٹیگ_feat;

  منطقي has_انکڑے () ثابت { أرجع نسخہ.منزل_int (); }

  بنانشانی صحيح get_feature_types (بنانشانی صحيح                  شروع_وتھ,
				  بنانشانی صحيح                 *گنتی,
				  hb_aat_layout_feature_type_t *features) ثابت
  {
    إذا (گنتی && features)
    {
      + ناں_Z.as_array (feature_Name_گنتی).sub_array (شروع_وتھ, گنتی)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *گنتی))
      ;
    }
    أرجع feature_Name_گنتی;
  }

  منطقي exposes_feature (hb_aat_layout_feature_type_t feature_type) ثابت
  { أرجع get_feature (feature_type).has_انکڑے (); }

  ثابت FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) ثابت
  { أرجع ناں_Z.bsearch (feature_Name_گنتی, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) ثابت
  { أرجع get_feature (feature).get_feature_name_id (); }

  بنانشانی صحيح get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   بنانشانی صحيح                           شروع_وتھ,
				   بنانشانی صحيح                          *selectors_گنتی, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   بنانشانی صحيح                          *default_اشاریہ    /* OUT.     May be NULL. */) ثابت
  {
    أرجع get_feature (feature_type).get_selector_infos (شروع_وتھ, selectors_گنتی, selectors,
							  default_اشاریہ, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخہ.major == 1 &&
			  ناں_Z.sanitize (c, feature_Name_گنتی, هذا)));
  }

  محمي:
  Fixed_نسخہ<>نسخہ;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_گنتی;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  Sorted_Unsized_Array_دا<FeatureName>
		ناں_Z;		/* The feature name array. */
  عام:
  DEFINE_ناپ_ARRAY (12, ناں_Z);
};

} /* namespace AAT */

#جے_انت /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

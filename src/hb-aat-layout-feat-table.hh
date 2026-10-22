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

#اگر_تعریف_نشده HB_AAT_LAYOUT_FEAT_جدول_HH
#تعریف_کن HB_AAT_LAYOUT_FEAT_جدول_HH

#ضمن "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#تعریف_کن HB_AAT_برچسب_feat HB_برچسب('f','e','a','t')


نطاق AAT {


بنية SettingName
{
  دوست بنية FeatureName;

  صحيح cmp (hb_aat_layout_feature_selector_t key) ثابت
  { أرجع (صحيح) key - (صحيح) setting; }

  hb_aat_layout_feature_selector_t get_selector () ثابت
  { أرجع (hb_aat_layout_feature_selector_t) (بیعلامت) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) ثابت
  {
    أرجع {
      name_شاخص,
      (hb_aat_layout_feature_selector_t) (بیعلامت صحيح) setting,
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
  NameID	name_شاخص;	/* The name table index for the setting's name. */
  عام:
  DEFINE_اندازه_STATIC (4);
};
DECLARE_NULL_NAMESPACE_بایتها (AAT, SettingName);

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
    شاخص_ماسک	= 0x00FF	/* If bits 30 and 31 are set, then these sixteen bits
				 * indicate the index of the setting in the setting name
				 * array for this feature which should be taken
				 * as the default. */
  };

  بیعلامت صحيح get_selector_infos (بیعلامت صحيح                           شروع_جابه_جایی,
				   بیعلامت صحيح                          *selectors_شمار, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   بیعلامت صحيح                          *pdefault_شاخص,  /* OUT.     May be NULL. */
				   ثابت فراغ *پایه) ثابت
  {
    hb_array_t< ثابت SettingName> settings_جدول = (پایه+setting_جدول_Z).as_array (nSettings);

    static_assert (شاخص::NOT_یافت_شاخص == HB_AAT_LAYOUT_NO_SELECTOR_شاخص, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    بیعلامت صحيح default_شاخص = شاخص::NOT_یافت_شاخص;
    إذا (featureFlags & Exclusive)
    {
      default_شاخص = (featureFlags & NotDefault) ? featureFlags & شاخص_ماسک : 0;
      default_selector = settings_جدول[default_شاخص].get_selector ();
    }
    إذا (pdefault_شاخص)
      *pdefault_شاخص = default_شاخص;

    إذا (selectors_شمار && selectors)
    {
      + settings_جدول.sub_array (شروع_جابه_جایی, selectors_شمار)
      | hb_map ([=] (ثابت SettingName& setting) { أرجع setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_شمار))
      ;
    }
    أرجع settings_جدول.درازی;
  }

  hb_aat_layout_feature_type_t get_feature_type () ثابت
  { أرجع (hb_aat_layout_feature_type_t) (بیعلامت صحيح) feature; }

  hb_ot_name_id_t get_feature_name_id () ثابت { أرجع name_شاخص; }

  منطقي is_exclusive () ثابت { أرجع featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  منطقي has_داده () ثابت { أرجع nSettings; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (پایه+setting_جدول_Z).sanitize (c, nSettings)));
  }

  محمي:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32_مقصد<Unsized_Array_از<SettingName>>
		setting_جدول_Z;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	name_شاخص;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية feat
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_AAT_برچسب_feat;

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }

  بیعلامت صحيح get_feature_types (بیعلامت صحيح                  شروع_جابه_جایی,
				  بیعلامت صحيح                 *شمار,
				  hb_aat_layout_feature_type_t *features) ثابت
  {
    إذا (شمار && features)
    {
      + نامها_Z.as_array (feature_Name_شمار).sub_array (شروع_جابه_جایی, شمار)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *شمار))
      ;
    }
    أرجع feature_Name_شمار;
  }

  منطقي exposes_feature (hb_aat_layout_feature_type_t feature_type) ثابت
  { أرجع get_feature (feature_type).has_داده (); }

  ثابت FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) ثابت
  { أرجع نامها_Z.bsearch (feature_Name_شمار, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) ثابت
  { أرجع get_feature (feature).get_feature_name_id (); }

  بیعلامت صحيح get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   بیعلامت صحيح                           شروع_جابه_جایی,
				   بیعلامت صحيح                          *selectors_شمار, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   بیعلامت صحيح                          *default_شاخص    /* OUT.     May be NULL. */) ثابت
  {
    أرجع get_feature (feature_type).get_selector_infos (شروع_جابه_جایی, selectors_شمار, selectors,
							  default_شاخص, هذا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه.major == 1 &&
			  نامها_Z.sanitize (c, feature_Name_شمار, هذا)));
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_شمار;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  Sorted_Unsized_Array_از<FeatureName>
		نامها_Z;		/* The feature name array. */
  عام:
  DEFINE_اندازه_ARRAY (12, نامها_Z);
};

} /* namespace AAT */

#پایان_اگر /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

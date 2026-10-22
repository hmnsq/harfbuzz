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

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_FEAT_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_FEAT_جدول_HH

#شامل "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#تعریف_کرو HB_AAT_لیبل_feat HB_لیبل('f','e','a','t')


نامخلا AAT {


ساخت SettingName
{
  دوست ساخت FeatureName;

  صحیح cmp (hb_aat_layout_feature_selector_t key) ثابت
  { واپس (صحیح) key - (صحیح) setting; }

  hb_aat_layout_feature_selector_t get_selector () ثابت
  { واپس (hb_aat_layout_feature_selector_t) (غیرعلامتی) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) ثابت
  {
    واپس {
      name_اشاریہ,
      (hb_aat_layout_feature_selector_t) (غیرعلامتی صحیح) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	setting;	/* The setting. */
  NameID	name_اشاریہ;	/* The name table index for the setting's name. */
  عوامی:
  DEFINE_سائز_STATIC (4);
};
DECLARE_NULL_NAMESPACE_بائٹیں (AAT, SettingName);

ساخت feat;

ساخت FeatureName
{
  صحیح cmp (hb_aat_layout_feature_type_t key) ثابت
  { واپس (صحیح) key - (صحیح) feature; }

  شمار {
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

  غیرعلامتی صحیح get_selector_infos (غیرعلامتی صحیح                           آغاز_انحراف,
				   غیرعلامتی صحیح                          *selectors_گنتی, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   غیرعلامتی صحیح                          *pdefault_اشاریہ,  /* OUT.     May be NULL. */
				   ثابت خالی *بنیاد) ثابت
  {
    hb_array_t< ثابت SettingName> settings_جدول = (بنیاد+setting_جدول_Z).as_array (nSettings);

    static_assert (اشاریہ::NOT_ملا_اشاریہ == HB_AAT_LAYOUT_NO_SELECTOR_اشاریہ, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    غیرعلامتی صحیح default_اشاریہ = اشاریہ::NOT_ملا_اشاریہ;
    اگر (featureFlags & Exclusive)
    {
      default_اشاریہ = (featureFlags & NotDefault) ? featureFlags & اشاریہ_نقاب : 0;
      default_selector = settings_جدول[default_اشاریہ].get_selector ();
    }
    اگر (pdefault_اشاریہ)
      *pdefault_اشاریہ = default_اشاریہ;

    اگر (selectors_گنتی && selectors)
    {
      + settings_جدول.sub_array (آغاز_انحراف, selectors_گنتی)
      | hb_map ([=] (ثابت SettingName& setting) { واپس setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_گنتی))
      ;
    }
    واپس settings_جدول.لمبائی;
  }

  hb_aat_layout_feature_type_t get_feature_type () ثابت
  { واپس (hb_aat_layout_feature_type_t) (غیرعلامتی صحیح) feature; }

  hb_ot_name_id_t get_feature_name_id () ثابت { واپس name_اشاریہ; }

  منطقی is_exclusive () ثابت { واپس featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  منطقی has_ڈیٹا () ثابت { واپس nSettings; }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  (بنیاد+setting_جدول_Z).sanitize (c, nSettings)));
  }

  محفوظ:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32_مقصد<Unsized_Array_کا<SettingName>>
		setting_جدول_Z;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	name_اشاریہ;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  عوامی:
  DEFINE_سائز_STATIC (12);
};

ساخت feat
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_AAT_لیبل_feat;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }

  غیرعلامتی صحیح get_feature_types (غیرعلامتی صحیح                  آغاز_انحراف,
				  غیرعلامتی صحیح                 *گنتی,
				  hb_aat_layout_feature_type_t *features) ثابت
  {
    اگر (گنتی && features)
    {
      + نام_Z.as_array (feature_Name_گنتی).sub_array (آغاز_انحراف, گنتی)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *گنتی))
      ;
    }
    واپس feature_Name_گنتی;
  }

  منطقی exposes_feature (hb_aat_layout_feature_type_t feature_type) ثابت
  { واپس get_feature (feature_type).has_ڈیٹا (); }

  ثابت FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) ثابت
  { واپس نام_Z.bsearch (feature_Name_گنتی, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) ثابت
  { واپس get_feature (feature).get_feature_name_id (); }

  غیرعلامتی صحیح get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   غیرعلامتی صحیح                           آغاز_انحراف,
				   غیرعلامتی صحیح                          *selectors_گنتی, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   غیرعلامتی صحیح                          *default_اشاریہ    /* OUT.     May be NULL. */) ثابت
  {
    واپس get_feature (feature_type).get_selector_infos (آغاز_انحراف, selectors_گنتی, selectors,
							  default_اشاریہ, یہ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  نسخہ.major == 1 &&
			  نام_Z.sanitize (c, feature_Name_گنتی, یہ)));
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_گنتی;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  Sorted_Unsized_Array_کا<FeatureName>
		نام_Z;		/* The feature name array. */
  عوامی:
  DEFINE_سائز_ARRAY (12, نام_Z);
};

} /* namespace AAT */

#اگر_ختم /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

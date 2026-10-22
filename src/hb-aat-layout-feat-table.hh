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

#ಅನ್ತರ್ಭಾವ "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#define HB_AAT_TAG_feat HB_TAG('f','e','a','t')


ನಾಮಾಕಾಶ AAT {


ಸಂರಚನಾ SettingName
{
  friend ಸಂರಚನಾ FeatureName;

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_aat_layout_feature_selector_t key) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) key - (ಪೂರ್ಣಾಙ್ಕ) setting; }

  hb_aat_layout_feature_selector_t get_selector () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (hb_aat_layout_feature_selector_t) (unsigned) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ {
      nameIndex,
      (hb_aat_layout_feature_selector_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	setting;	/* The setting. */
  NameID	nameIndex;	/* The name table index for the setting's name. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};
DECLARE_NULL_NAMESPACE_BYTES (AAT, SettingName);

ಸಂರಚನಾ feat;

ಸಂರಚನಾ FeatureName
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_aat_layout_feature_type_t key) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) key - (ಪೂರ್ಣಾಙ್ಕ) feature; }

  ಗಣನಾ {
    Exclusive	= 0x8000,	/* If set, the feature settings are mutually exclusive. */
    NotDefault	= 0x4000,	/* If clear, then the setting with an index of 0 in
				 * the setting name array for this feature should
				 * be taken as the default for the feature
				 * (if one is required). If set, then bits 0-15 of this
				 * featureFlags field contain the index of the setting
				 * which is to be taken as the default. */
    IndexMask	= 0x00FF	/* If bits 30 and 31 are set, then these sixteen bits
				 * indicate the index of the setting in the setting name
				 * array for this feature which should be taken
				 * as the default. */
  };

  unsigned ಪೂರ್ಣಾಙ್ಕ get_selector_infos (unsigned ಪೂರ್ಣಾಙ್ಕ                           start_offset,
				   unsigned ಪೂರ್ಣಾಙ್ಕ                          *selectors_ಎಣಿಕೆ, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned ಪೂರ್ಣಾಙ್ಕ                          *pdefault_index,  /* OUT.     May be NULL. */
				   ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    hb_array_t< ಧ್ರುವ SettingName> settings_table = (base+settingTableZ).as_array (nSettings);

    static_assert (Index::NOT_FOUND_INDEX == HB_AAT_LAYOUT_NO_SELECTOR_INDEX, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    unsigned ಪೂರ್ಣಾಙ್ಕ default_index = Index::NOT_FOUND_INDEX;
    ಯದಿ (featureFlags & Exclusive)
    {
      default_index = (featureFlags & NotDefault) ? featureFlags & IndexMask : 0;
      default_selector = settings_table[default_index].get_selector ();
    }
    ಯದಿ (pdefault_index)
      *pdefault_index = default_index;

    ಯದಿ (selectors_ಎಣಿಕೆ && selectors)
    {
      + settings_table.sub_array (start_offset, selectors_ಎಣಿಕೆ)
      | hb_map ([=] (ಧ್ರುವ SettingName& setting) { ಪ್ರತ್ಯಾಗಮ setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_ಎಣಿಕೆ))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ settings_table.length;
  }

  hb_aat_layout_feature_type_t get_feature_type () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (hb_aat_layout_feature_type_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) feature; }

  hb_ot_name_id_t get_feature_name_id () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ nameIndex; }

  ತಾರ್ಕಿಕ is_exclusive () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ nSettings; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  (base+settingTableZ).sanitize (c, nSettings)));
  }

  ರಕ್ಷಿತ:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32To<UnsizedArrayOf<SettingName>>
		settingTableZ;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	nameIndex;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

ಸಂರಚನಾ feat
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_AAT_TAG_feat;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_feature_types (unsigned ಪೂರ್ಣಾಙ್ಕ                  start_offset,
				  unsigned ಪೂರ್ಣಾಙ್ಕ                 *ಎಣಿಕೆ,
				  hb_aat_layout_feature_type_t *features) ಧ್ರುವ
  {
    ಯದಿ (ಎಣಿಕೆ && features)
    {
      + namesZ.as_array (feature_Name_ಎಣಿಕೆ).sub_array (start_offset, ಎಣಿಕೆ)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *ಎಣಿಕೆ))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ feature_Name_ಎಣಿಕೆ;
  }

  ತಾರ್ಕಿಕ exposes_feature (hb_aat_layout_feature_type_t feature_type) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_feature (feature_type).has_data (); }

  ಧ್ರುವ FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ namesZ.bsearch (feature_Name_ಎಣಿಕೆ, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_feature (feature).get_feature_name_id (); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   unsigned ಪೂರ್ಣಾಙ್ಕ                           start_offset,
				   unsigned ಪೂರ್ಣಾಙ್ಕ                          *selectors_ಎಣಿಕೆ, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned ಪೂರ್ಣಾಙ್ಕ                          *default_index    /* OUT.     May be NULL. */) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ get_feature (feature_type).get_selector_infos (start_offset, selectors_ಎಣಿಕೆ, selectors,
							  default_index, ಏತತ್);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  namesZ.sanitize (c, feature_Name_ಎಣಿಕೆ, ಏತತ್)));
  }

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_ಎಣಿಕೆ;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  SortedUnsizedArrayOf<FeatureName>
		namesZ;		/* The feature name array. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (12, namesZ);
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

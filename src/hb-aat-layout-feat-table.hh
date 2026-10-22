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

#அந்தர்பாவ "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#define HB_AAT_TAG_feat HB_TAG('f','e','a','t')


நாமாகாஶ AAT {


கட்டமைப்பு SettingName
{
  friend கட்டமைப்பு FeatureName;

  முழு cmp (hb_aat_layout_feature_selector_t key) மாறிலி
  { திரும்பு (முழு) key - (முழு) setting; }

  hb_aat_layout_feature_selector_t get_selector () மாறிலி
  { திரும்பு (hb_aat_layout_feature_selector_t) (unsigned) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) மாறிலி
  {
    திரும்பு {
      nameIndex,
      (hb_aat_layout_feature_selector_t) (unsigned முழு) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	setting;	/* The setting. */
  NameID	nameIndex;	/* The name table index for the setting's name. */
  பொது:
  DEFINE_SIZE_STATIC (4);
};
DECLARE_NULL_NAMESPACE_BYTES (AAT, SettingName);

கட்டமைப்பு feat;

கட்டமைப்பு FeatureName
{
  முழு cmp (hb_aat_layout_feature_type_t key) மாறிலி
  { திரும்பு (முழு) key - (முழு) feature; }

  எண்ணிக்கை {
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

  unsigned முழு get_selector_infos (unsigned முழு                           start_offset,
				   unsigned முழு                          *selectors_மொத்தம், /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned முழு                          *pdefault_index,  /* OUT.     May be NULL. */
				   மாறிலி வெறுமை *base) மாறிலி
  {
    hb_array_t< மாறிலி SettingName> settings_table = (base+settingTableZ).as_array (nSettings);

    static_assert (Index::NOT_FOUND_INDEX == HB_AAT_LAYOUT_NO_SELECTOR_INDEX, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    unsigned முழு default_index = Index::NOT_FOUND_INDEX;
    எனில் (featureFlags & Exclusive)
    {
      default_index = (featureFlags & NotDefault) ? featureFlags & IndexMask : 0;
      default_selector = settings_table[default_index].get_selector ();
    }
    எனில் (pdefault_index)
      *pdefault_index = default_index;

    எனில் (selectors_மொத்தம் && selectors)
    {
      + settings_table.sub_array (start_offset, selectors_மொத்தம்)
      | hb_map ([=] (மாறிலி SettingName& setting) { திரும்பு setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_மொத்தம்))
      ;
    }
    திரும்பு settings_table.length;
  }

  hb_aat_layout_feature_type_t get_feature_type () மாறிலி
  { திரும்பு (hb_aat_layout_feature_type_t) (unsigned முழு) feature; }

  hb_ot_name_id_t get_feature_name_id () மாறிலி { திரும்பு nameIndex; }

  தருக்க is_exclusive () மாறிலி { திரும்பு featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  தருக்க has_data () மாறிலி { திரும்பு nSettings; }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  (base+settingTableZ).sanitize (c, nSettings)));
  }

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_STATIC (12);
};

கட்டமைப்பு feat
{
  நிலையான constexpr hb_tag_t tableTag = HB_AAT_TAG_feat;

  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }

  unsigned முழு get_feature_types (unsigned முழு                  start_offset,
				  unsigned முழு                 *மொத்தம்,
				  hb_aat_layout_feature_type_t *features) மாறிலி
  {
    எனில் (மொத்தம் && features)
    {
      + namesZ.as_array (feature_Name_மொத்தம்).sub_array (start_offset, மொத்தம்)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *மொத்தம்))
      ;
    }
    திரும்பு feature_Name_மொத்தம்;
  }

  தருக்க exposes_feature (hb_aat_layout_feature_type_t feature_type) மாறிலி
  { திரும்பு get_feature (feature_type).has_data (); }

  மாறிலி FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) மாறிலி
  { திரும்பு namesZ.bsearch (feature_Name_மொத்தம், feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) மாறிலி
  { திரும்பு get_feature (feature).get_feature_name_id (); }

  unsigned முழு get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   unsigned முழு                           start_offset,
				   unsigned முழு                          *selectors_மொத்தம், /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned முழு                          *default_index    /* OUT.     May be NULL. */) மாறிலி
  {
    திரும்பு get_feature (feature_type).get_selector_infos (start_offset, selectors_மொத்தம், selectors,
							  default_index, இது);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  namesZ.sanitize (c, feature_Name_மொத்தம், இது)));
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_மொத்தம்;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  SortedUnsizedArrayOf<FeatureName>
		namesZ;		/* The feature name array. */
  பொது:
  DEFINE_SIZE_ARRAY (12, namesZ);
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

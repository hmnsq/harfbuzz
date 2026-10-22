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

#include "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#define HB_AAT_TAG_feat HB_TAG('f','e','a','t')


namespace AAT {


sa_nracanaa SettingName
{
  friend sa_nracanaa FeatureName;

  puur_naa_nka cmp (hb_aat_layout_feature_selector_t key) const
  { ᬯᬮᬶ (puur_naa_nka) key - (puur_naa_nka) setting; }

  hb_aat_layout_feature_selector_t get_selector () const
  { ᬯᬮᬶ (hb_aat_layout_feature_selector_t) (unsigned) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) const
  {
    ᬯᬮᬶ {
      nameIndex,
      (hb_aat_layout_feature_selector_t) (unsigned puur_naa_nka) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  rak_sita:
  HBUINT16	setting;	/* The setting. */
  NameID	nameIndex;	/* The name table index for the setting's name. */
  public:
  DEFINE_SIZE_STATIC (4);
};
DECLARE_NULL_NAMESPACE_BYTES (AAT, SettingName);

sa_nracanaa feat;

sa_nracanaa FeatureName
{
  puur_naa_nka cmp (hb_aat_layout_feature_type_t key) const
  { ᬯᬮᬶ (puur_naa_nka) key - (puur_naa_nka) feature; }

  ga_nanaa {
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

  unsigned puur_naa_nka get_selector_infos (unsigned puur_naa_nka                           start_offset,
				   unsigned puur_naa_nka                          *selectors_count, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned puur_naa_nka                          *pdefault_index,  /* OUT.     May be NULL. */
				   const void *base) const
  {
    hb_array_t< const SettingName> settings_table = (base+settingTableZ).as_array (nSettings);

    static_assert (Index::NOT_FOUND_INDEX == HB_AAT_LAYOUT_NO_SELECTOR_INDEX, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    unsigned puur_naa_nka default_index = Index::NOT_FOUND_INDEX;
    ᬬᬾᬦ᭄ (featureFlags & Exclusive)
    {
      default_index = (featureFlags & NotDefault) ? featureFlags & IndexMask : 0;
      default_selector = settings_table[default_index].get_selector ();
    }
    ᬬᬾᬦ᭄ (pdefault_index)
      *pdefault_index = default_index;

    ᬬᬾᬦ᭄ (selectors_count && selectors)
    {
      + settings_table.sub_array (start_offset, selectors_count)
      | hb_map ([=] (const SettingName& setting) { ᬯᬮᬶ setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_count))
      ;
    }
    ᬯᬮᬶ settings_table.length;
  }

  hb_aat_layout_feature_type_t get_feature_type () const
  { ᬯᬮᬶ (hb_aat_layout_feature_type_t) (unsigned puur_naa_nka) feature; }

  hb_ot_name_id_t get_feature_name_id () const { ᬯᬮᬶ nameIndex; }

  bool is_exclusive () const { ᬯᬮᬶ featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  bool has_data () const { ᬯᬮᬶ nSettings; }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  hb_barrier () &&
			  (base+settingTableZ).sanitize (c, nSettings)));
  }

  rak_sita:
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
  public:
  DEFINE_SIZE_STATIC (12);
};

sa_nracanaa feat
{
  static constexpr hb_tag_t tableTag = HB_AAT_TAG_feat;

  bool has_data () const { ᬯᬮᬶ version.to_int (); }

  unsigned puur_naa_nka get_feature_types (unsigned puur_naa_nka                  start_offset,
				  unsigned puur_naa_nka                 *count,
				  hb_aat_layout_feature_type_t *features) const
  {
    ᬬᬾᬦ᭄ (count && features)
    {
      + namesZ.as_array (featureNameCount).sub_array (start_offset, count)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *count))
      ;
    }
    ᬯᬮᬶ featureNameCount;
  }

  bool exposes_feature (hb_aat_layout_feature_type_t feature_type) const
  { ᬯᬮᬶ get_feature (feature_type).has_data (); }

  const FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) const
  { ᬯᬮᬶ namesZ.bsearch (featureNameCount, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) const
  { ᬯᬮᬶ get_feature (feature).get_feature_name_id (); }

  unsigned puur_naa_nka get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   unsigned puur_naa_nka                           start_offset,
				   unsigned puur_naa_nka                          *selectors_count, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned puur_naa_nka                          *default_index    /* OUT.     May be NULL. */) const
  {
    ᬯᬮᬶ get_feature (feature_type).get_selector_infos (start_offset, selectors_count, selectors,
							  default_index, this);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  namesZ.sanitize (c, featureNameCount, this)));
  }

  rak_sita:
  FixedVersion<>version;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	featureNameCount;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  SortedUnsizedArrayOf<FeatureName>
		namesZ;		/* The feature name array. */
  public:
  DEFINE_SIZE_ARRAY (12, namesZ);
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

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

#അന്തര്ഭാവ "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#define HB_AAT_TAG_feat HB_TAG('f','e','a','t')


നാമാകാശ AAT {


ഘടന SettingName
{
  friend ഘടന FeatureName;

  പൂർണ്ണസംഖ്യ cmp (hb_aat_layout_feature_selector_t key) സ്ഥിരം
  { തിരികെ (പൂർണ്ണസംഖ്യ) key - (പൂർണ്ണസംഖ്യ) setting; }

  hb_aat_layout_feature_selector_t get_selector () സ്ഥിരം
  { തിരികെ (hb_aat_layout_feature_selector_t) (unsigned) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) സ്ഥിരം
  {
    തിരികെ {
      nameIndex,
      (hb_aat_layout_feature_selector_t) (unsigned പൂർണ്ണസംഖ്യ) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	setting;	/* The setting. */
  NameID	nameIndex;	/* The name table index for the setting's name. */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};
DECLARE_NULL_NAMESPACE_BYTES (AAT, SettingName);

ഘടന feat;

ഘടന FeatureName
{
  പൂർണ്ണസംഖ്യ cmp (hb_aat_layout_feature_type_t key) സ്ഥിരം
  { തിരികെ (പൂർണ്ണസംഖ്യ) key - (പൂർണ്ണസംഖ്യ) feature; }

  എണ്ണൽ {
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

  unsigned പൂർണ്ണസംഖ്യ get_selector_infos (unsigned പൂർണ്ണസംഖ്യ                           start_offset,
				   unsigned പൂർണ്ണസംഖ്യ                          *selectors_എണ്ണം, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned പൂർണ്ണസംഖ്യ                          *pdefault_index,  /* OUT.     May be NULL. */
				   സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    hb_array_t< സ്ഥിരം SettingName> settings_table = (base+settingTableZ).as_array (nSettings);

    static_assert (Index::NOT_FOUND_INDEX == HB_AAT_LAYOUT_NO_SELECTOR_INDEX, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    unsigned പൂർണ്ണസംഖ്യ default_index = Index::NOT_FOUND_INDEX;
    എങ്കിൽ (featureFlags & Exclusive)
    {
      default_index = (featureFlags & NotDefault) ? featureFlags & IndexMask : 0;
      default_selector = settings_table[default_index].get_selector ();
    }
    എങ്കിൽ (pdefault_index)
      *pdefault_index = default_index;

    എങ്കിൽ (selectors_എണ്ണം && selectors)
    {
      + settings_table.sub_array (start_offset, selectors_എണ്ണം)
      | hb_map ([=] (സ്ഥിരം SettingName& setting) { തിരികെ setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_എണ്ണം))
      ;
    }
    തിരികെ settings_table.length;
  }

  hb_aat_layout_feature_type_t get_feature_type () സ്ഥിരം
  { തിരികെ (hb_aat_layout_feature_type_t) (unsigned പൂർണ്ണസംഖ്യ) feature; }

  hb_ot_name_id_t get_feature_name_id () സ്ഥിരം { തിരികെ nameIndex; }

  തർക്കം is_exclusive () സ്ഥിരം { തിരികെ featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  തർക്കം has_data () സ്ഥിരം { തിരികെ nSettings; }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  (base+settingTableZ).sanitize (c, nSettings)));
  }

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

ഘടന feat
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_AAT_TAG_feat;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }

  unsigned പൂർണ്ണസംഖ്യ get_feature_types (unsigned പൂർണ്ണസംഖ്യ                  start_offset,
				  unsigned പൂർണ്ണസംഖ്യ                 *എണ്ണം,
				  hb_aat_layout_feature_type_t *features) സ്ഥിരം
  {
    എങ്കിൽ (എണ്ണം && features)
    {
      + namesZ.as_array (feature_Name_എണ്ണം).sub_array (start_offset, എണ്ണം)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *എണ്ണം))
      ;
    }
    തിരികെ feature_Name_എണ്ണം;
  }

  തർക്കം exposes_feature (hb_aat_layout_feature_type_t feature_type) സ്ഥിരം
  { തിരികെ get_feature (feature_type).has_data (); }

  സ്ഥിരം FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) സ്ഥിരം
  { തിരികെ namesZ.bsearch (feature_Name_എണ്ണം, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) സ്ഥിരം
  { തിരികെ get_feature (feature).get_feature_name_id (); }

  unsigned പൂർണ്ണസംഖ്യ get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   unsigned പൂർണ്ണസംഖ്യ                           start_offset,
				   unsigned പൂർണ്ണസംഖ്യ                          *selectors_എണ്ണം, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned പൂർണ്ണസംഖ്യ                          *default_index    /* OUT.     May be NULL. */) സ്ഥിരം
  {
    തിരികെ get_feature (feature_type).get_selector_infos (start_offset, selectors_എണ്ണം, selectors,
							  default_index, ഇത്);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  namesZ.sanitize (c, feature_Name_എണ്ണം, ഇത്)));
  }

  സംരക്ഷിത:
  FixedVersion<>version;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_എണ്ണം;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  SortedUnsizedArrayOf<FeatureName>
		namesZ;		/* The feature name array. */
  പൊതു:
  DEFINE_SIZE_ARRAY (12, namesZ);
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

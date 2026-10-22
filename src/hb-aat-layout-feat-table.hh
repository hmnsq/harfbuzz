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

#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#define HB_AAT_TAG_feat HB_TAG('f','e','a','t')


ନାମାକାଶ AAT {


ସଂରଚନା SettingName
{
  friend ସଂରଚନା FeatureName;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_aat_layout_feature_selector_t key) ଅଚଳ
  { ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) key - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) setting; }

  hb_aat_layout_feature_selector_t get_selector () ଅଚଳ
  { ଫେରନ୍ତୁ (hb_aat_layout_feature_selector_t) (unsigned) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) ଅଚଳ
  {
    ଫେରନ୍ତୁ {
      nameIndex,
      (hb_aat_layout_feature_selector_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	setting;	/* The setting. */
  NameID	nameIndex;	/* The name table index for the setting's name. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};
DECLARE_NULL_NAMESPACE_BYTES (AAT, SettingName);

ସଂରଚନା feat;

ସଂରଚନା FeatureName
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_aat_layout_feature_type_t key) ଅଚଳ
  { ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) key - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) feature; }

  ଗଣନା {
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

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_selector_infos (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                           start_offset,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                          *selectors_ଗଣନା, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                          *pdefault_index,  /* OUT.     May be NULL. */
				   ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    hb_array_t< ଅଚଳ SettingName> settings_table = (base+settingTableZ).as_array (nSettings);

    static_assert (Index::NOT_FOUND_INDEX == HB_AAT_LAYOUT_NO_SELECTOR_INDEX, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_index = Index::NOT_FOUND_INDEX;
    ଯଦି (featureFlags & Exclusive)
    {
      default_index = (featureFlags & NotDefault) ? featureFlags & IndexMask : 0;
      default_selector = settings_table[default_index].get_selector ();
    }
    ଯଦି (pdefault_index)
      *pdefault_index = default_index;

    ଯଦି (selectors_ଗଣନା && selectors)
    {
      + settings_table.sub_array (start_offset, selectors_ଗଣନା)
      | hb_map ([=] (ଅଚଳ SettingName& setting) { ଫେରନ୍ତୁ setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_ଗଣନା))
      ;
    }
    ଫେରନ୍ତୁ settings_table.length;
  }

  hb_aat_layout_feature_type_t get_feature_type () ଅଚଳ
  { ଫେରନ୍ତୁ (hb_aat_layout_feature_type_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) feature; }

  hb_ot_name_id_t get_feature_name_id () ଅଚଳ { ଫେରନ୍ତୁ nameIndex; }

  ତାର୍କିକ is_exclusive () ଅଚଳ { ଫେରନ୍ତୁ featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ nSettings; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  (base+settingTableZ).sanitize (c, nSettings)));
  }

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

ସଂରଚନା feat
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_AAT_TAG_feat;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_feature_types (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                  start_offset,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                 *ଗଣନା_2,
				  hb_aat_layout_feature_type_t *features) ଅଚଳ
  {
    ଯଦି (ଗଣନା_2 && features)
    {
      + namesZ.as_array (feature_Name_ଗଣନା).sub_array (start_offset, ଗଣନା_2)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *ଗଣନା_2))
      ;
    }
    ଫେରନ୍ତୁ feature_Name_ଗଣନା;
  }

  ତାର୍କିକ exposes_feature (hb_aat_layout_feature_type_t feature_type) ଅଚଳ
  { ଫେରନ୍ତୁ get_feature (feature_type).has_data (); }

  ଅଚଳ FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) ଅଚଳ
  { ଫେରନ୍ତୁ namesZ.bsearch (feature_Name_ଗଣନା, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) ଅଚଳ
  { ଫେରନ୍ତୁ get_feature (feature).get_feature_name_id (); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                           start_offset,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                          *selectors_ଗଣନା, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                          *default_index    /* OUT.     May be NULL. */) ଅଚଳ
  {
    ଫେରନ୍ତୁ get_feature (feature_type).get_selector_infos (start_offset, selectors_ଗଣନା, selectors,
							  default_index, ଏହି);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  namesZ.sanitize (c, feature_Name_ଗଣନା, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_ଗଣନା;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  SortedUnsizedArrayOf<FeatureName>
		namesZ;		/* The feature name array. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (12, namesZ);
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

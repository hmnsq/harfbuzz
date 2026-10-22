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

#अगर_अघोषित HB_AAT_LAYOUT_FEAT_TABLE_HH
#घोषणा HB_AAT_LAYOUT_FEAT_TABLE_HH

#समावेश "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#घोषणा HB_AAT_TAG_feat HB_TAG('f','e','a','t')


namespace AAT {


काष्ठा SettingName
{
  मित्र काष्ठा FeatureName;

  पूर्णांक cmp (hb_aat_layout_feature_selector_t key) स्थिर
  { वापस (पूर्णांक) key - (पूर्णांक) setting; }

  hb_aat_layout_feature_selector_t get_selector () स्थिर
  { वापस (hb_aat_layout_feature_selector_t) (अचिन्हित) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) स्थिर
  {
    वापस {
      name_सूचकांक,
      (hb_aat_layout_feature_selector_t) (अचिन्हित पूर्णांक) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	setting;	/* The setting. */
  NameID	name_सूचकांक;	/* The name table index for the setting's name. */
  खुला:
  DEFINE_आकार_STATIC (4);
};
DECLARE_NULL_NAMESPACE_BYTES (AAT, SettingName);

काष्ठा feat;

काष्ठा FeatureName
{
  पूर्णांक cmp (hb_aat_layout_feature_type_t key) स्थिर
  { वापस (पूर्णांक) key - (पूर्णांक) feature; }

  क्रमागत {
    Exclusive	= 0x8000,	/* If set, the feature settings are mutually exclusive. */
    NotDefault	= 0x4000,	/* If clear, then the setting with an index of 0 in
				 * the setting name array for this feature should
				 * be taken as the default for the feature
				 * (if one is required). If set, then bits 0-15 of this
				 * featureFlags field contain the index of the setting
				 * which is to be taken as the default. */
    सूचकांक_Mask	= 0x00FF	/* If bits 30 and 31 are set, then these sixteen bits
				 * indicate the index of the setting in the setting name
				 * array for this feature which should be taken
				 * as the default. */
  };

  अचिन्हित पूर्णांक get_selector_infos (अचिन्हित पूर्णांक                           start_offset,
				   अचिन्हित पूर्णांक                          *selectors_गिनती, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   अचिन्हित पूर्णांक                          *pdefault_सूचकांक,  /* OUT.     May be NULL. */
				   स्थिर व्योम *base) स्थिर
  {
    hb_array_t< स्थिर SettingName> settings_table = (base+settingTableZ).as_array (nSettings);

    static_assert (सूचकांक::NOT_FOUND_सूचकांक == HB_AAT_LAYOUT_NO_SELECTOR_सूचकांक, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    अचिन्हित पूर्णांक default_सूचकांक = सूचकांक::NOT_FOUND_सूचकांक;
    अगर (featureFlags & Exclusive)
    {
      default_सूचकांक = (featureFlags & NotDefault) ? featureFlags & सूचकांक_Mask : 0;
      default_selector = settings_table[default_सूचकांक].get_selector ();
    }
    अगर (pdefault_सूचकांक)
      *pdefault_सूचकांक = default_सूचकांक;

    अगर (selectors_गिनती && selectors)
    {
      + settings_table.sub_array (start_offset, selectors_गिनती)
      | hb_map ([=] (स्थिर SettingName& setting) { वापस setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_गिनती))
      ;
    }
    वापस settings_table.लंबाई;
  }

  hb_aat_layout_feature_type_t get_feature_type () स्थिर
  { वापस (hb_aat_layout_feature_type_t) (अचिन्हित पूर्णांक) feature; }

  hb_ot_name_id_t get_feature_name_id () स्थिर { वापस name_सूचकांक; }

  bool is_exclusive () स्थिर { वापस featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  bool has_तथ्य () स्थिर { वापस nSettings; }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  (base+settingTableZ).sanitize (c, nSettings)));
  }

  रक्षित:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32To<UnsizedArrayOf<SettingName>>
		settingTableZ;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	name_सूचकांक;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  खुला:
  DEFINE_आकार_STATIC (12);
};

काष्ठा feat
{
  अटल constexpr hb_tag_t tableTag = HB_AAT_TAG_feat;

  bool has_तथ्य () स्थिर { वापस version.to_int (); }

  अचिन्हित पूर्णांक get_feature_types (अचिन्हित पूर्णांक                  start_offset,
				  अचिन्हित पूर्णांक                 *गिनती,
				  hb_aat_layout_feature_type_t *features) स्थिर
  {
    अगर (गिनती && features)
    {
      + namesZ.as_array (feature_Name_गिनती).sub_array (start_offset, गिनती)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *गिनती))
      ;
    }
    वापस feature_Name_गिनती;
  }

  bool exposes_feature (hb_aat_layout_feature_type_t feature_type) स्थिर
  { वापस get_feature (feature_type).has_तथ्य (); }

  स्थिर FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) स्थिर
  { वापस namesZ.द्वा_खोज (feature_Name_गिनती, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) स्थिर
  { वापस get_feature (feature).get_feature_name_id (); }

  अचिन्हित पूर्णांक get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   अचिन्हित पूर्णांक                           start_offset,
				   अचिन्हित पूर्णांक                          *selectors_गिनती, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   अचिन्हित पूर्णांक                          *default_सूचकांक    /* OUT.     May be NULL. */) स्थिर
  {
    वापस get_feature (feature_type).get_selector_infos (start_offset, selectors_गिनती, selectors,
							  default_सूचकांक, यह);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  namesZ.sanitize (c, feature_Name_गिनती, यह)));
  }

  रक्षित:
  FixedVersion<>version;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_गिनती;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  SortedUnsizedArrayOf<FeatureName>
		namesZ;		/* The feature name array. */
  खुला:
  DEFINE_आकार_ARRAY (12, namesZ);
};

} /* namespace AAT */

#पूर्ण_अगर /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

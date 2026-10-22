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

#अन्तर्भाव "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#define HB_AAT_TAG_feat HB_TAG('f','e','a','t')


नामाकाश AAT {


संरचना SettingName
{
  मित्र संरचना FeatureName;

  पूर्णांक cmp (hb_aat_layout_feature_selector_t key) स्थिरांक
  { परत (पूर्णांक) key - (पूर्णांक) setting; }

  hb_aat_layout_feature_selector_t get_selector () स्थिरांक
  { परत (hb_aat_layout_feature_selector_t) (अचिन्हित) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) स्थिरांक
  {
    परत {
      nameIndex,
      (hb_aat_layout_feature_selector_t) (अचिन्हित पूर्णांक) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	setting;	/* The setting. */
  NameID	nameIndex;	/* The name table index for the setting's name. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};
DECLARE_NULL_NAMESPACE_BYTES (AAT, SettingName);

संरचना feat;

संरचना FeatureName
{
  पूर्णांक cmp (hb_aat_layout_feature_type_t key) स्थिरांक
  { परत (पूर्णांक) key - (पूर्णांक) feature; }

  गणना {
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

  अचिन्हित पूर्णांक get_selector_infos (अचिन्हित पूर्णांक                           start_offset,
				   अचिन्हित पूर्णांक                          *selectors_मोजणी, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   अचिन्हित पूर्णांक                          *pdefault_index,  /* OUT.     May be NULL. */
				   स्थिरांक शून्य *base) स्थिरांक
  {
    hb_array_t< स्थिरांक SettingName> settings_table = (base+settingTableZ).as_array (nSettings);

    static_assert (Index::NOT_FOUND_INDEX == HB_AAT_LAYOUT_NO_SELECTOR_INDEX, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    अचिन्हित पूर्णांक default_index = Index::NOT_FOUND_INDEX;
    जर (featureFlags & Exclusive)
    {
      default_index = (featureFlags & NotDefault) ? featureFlags & IndexMask : 0;
      default_selector = settings_table[default_index].get_selector ();
    }
    जर (pdefault_index)
      *pdefault_index = default_index;

    जर (selectors_मोजणी && selectors)
    {
      + settings_table.sub_array (start_offset, selectors_मोजणी)
      | hb_map ([=] (स्थिरांक SettingName& setting) { परत setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_मोजणी))
      ;
    }
    परत settings_table.length;
  }

  hb_aat_layout_feature_type_t get_feature_type () स्थिरांक
  { परत (hb_aat_layout_feature_type_t) (अचिन्हित पूर्णांक) feature; }

  hb_ot_name_id_t get_feature_name_id () स्थिरांक { परत nameIndex; }

  तार्किक is_exclusive () स्थिरांक { परत featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  तार्किक has_data () स्थिरांक { परत nSettings; }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  (base+settingTableZ).sanitize (c, nSettings)));
  }

  संरक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

संरचना feat
{
  स्थिर constexpr hb_tag_t tableTag = HB_AAT_TAG_feat;

  तार्किक has_data () स्थिरांक { परत version.to_int (); }

  अचिन्हित पूर्णांक get_feature_types (अचिन्हित पूर्णांक                  start_offset,
				  अचिन्हित पूर्णांक                 *मोजणी,
				  hb_aat_layout_feature_type_t *features) स्थिरांक
  {
    जर (मोजणी && features)
    {
      + namesZ.as_array (feature_Name_मोजणी).sub_array (start_offset, मोजणी)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *मोजणी))
      ;
    }
    परत feature_Name_मोजणी;
  }

  तार्किक exposes_feature (hb_aat_layout_feature_type_t feature_type) स्थिरांक
  { परत get_feature (feature_type).has_data (); }

  स्थिरांक FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) स्थिरांक
  { परत namesZ.द्वा_खोज (feature_Name_मोजणी, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) स्थिरांक
  { परत get_feature (feature).get_feature_name_id (); }

  अचिन्हित पूर्णांक get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   अचिन्हित पूर्णांक                           start_offset,
				   अचिन्हित पूर्णांक                          *selectors_मोजणी, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   अचिन्हित पूर्णांक                          *default_index    /* OUT.     May be NULL. */) स्थिरांक
  {
    परत get_feature (feature_type).get_selector_infos (start_offset, selectors_मोजणी, selectors,
							  default_index, हे);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  namesZ.sanitize (c, feature_Name_मोजणी, हे)));
  }

  संरक्षित:
  FixedVersion<>version;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_मोजणी;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  SortedUnsizedArrayOf<FeatureName>
		namesZ;		/* The feature name array. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (12, namesZ);
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

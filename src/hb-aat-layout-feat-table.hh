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

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_FEAT_పట్టిక_HH
#నిర్వచించు HB_AAT_LAYOUT_FEAT_పట్టిక_HH

#అన్తర్భావ "hb-aat-layout-common.hh"

/*
 * feat -- Feature Name
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6feat.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_feat HB_ట్యాగ్('f','e','a','t')


నామాకాశ AAT {


నిర్మాణం SettingName
{
  మిత్ర నిర్మాణం FeatureName;

  పూర్ణాంకం cmp (hb_aat_layout_feature_selector_t key) స్థిరరాశి
  { తిరిగి (పూర్ణాంకం) key - (పూర్ణాంకం) setting; }

  hb_aat_layout_feature_selector_t get_selector () స్థిరరాశి
  { తిరిగి (hb_aat_layout_feature_selector_t) (గుర్తులేని) setting; }

  hb_aat_layout_feature_selector_info_t get_info (hb_aat_layout_feature_selector_t default_selector) స్థిరరాశి
  {
    తిరిగి {
      name_సూచీ,
      (hb_aat_layout_feature_selector_t) (గుర్తులేని పూర్ణాంకం) setting,
      default_selector == HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID
	? (hb_aat_layout_feature_selector_t) (setting + 1)
	: default_selector,
      0
    };
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	setting;	/* The setting. */
  NameID	name_సూచీ;	/* The name table index for the setting's name. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};
DECLARE_NULL_NAMESPACE_బైట్లు (AAT, SettingName);

నిర్మాణం feat;

నిర్మాణం FeatureName
{
  పూర్ణాంకం cmp (hb_aat_layout_feature_type_t key) స్థిరరాశి
  { తిరిగి (పూర్ణాంకం) key - (పూర్ణాంకం) feature; }

  గణన {
    Exclusive	= 0x8000,	/* If set, the feature settings are mutually exclusive. */
    NotDefault	= 0x4000,	/* If clear, then the setting with an index of 0 in
				 * the setting name array for this feature should
				 * be taken as the default for the feature
				 * (if one is required). If set, then bits 0-15 of this
				 * featureFlags field contain the index of the setting
				 * which is to be taken as the default. */
    సూచీ_ముసుగు	= 0x00FF	/* If bits 30 and 31 are set, then these sixteen bits
				 * indicate the index of the setting in the setting name
				 * array for this feature which should be taken
				 * as the default. */
  };

  గుర్తులేని పూర్ణాంకం get_selector_infos (గుర్తులేని పూర్ణాంకం                           ప్రారంభం_స్థానభ్రంశం,
				   గుర్తులేని పూర్ణాంకం                          *selectors_లెక్క, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   గుర్తులేని పూర్ణాంకం                          *pdefault_సూచీ,  /* OUT.     May be NULL. */
				   స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    hb_array_t< స్థిరరాశి SettingName> settings_పట్టిక = (పునాది+setting_పట్టిక_Z).as_array (nSettings);

    static_assert (సూచీ::NOT_దొరికింది_సూచీ == HB_AAT_LAYOUT_NO_SELECTOR_సూచీ, "");

    hb_aat_layout_feature_selector_t default_selector = HB_AAT_LAYOUT_FEATURE_SELECTOR_INVALID;
    గుర్తులేని పూర్ణాంకం default_సూచీ = సూచీ::NOT_దొరికింది_సూచీ;
    ఒకవేళ (featureFlags & Exclusive)
    {
      default_సూచీ = (featureFlags & NotDefault) ? featureFlags & సూచీ_ముసుగు : 0;
      default_selector = settings_పట్టిక[default_సూచీ].get_selector ();
    }
    ఒకవేళ (pdefault_సూచీ)
      *pdefault_సూచీ = default_సూచీ;

    ఒకవేళ (selectors_లెక్క && selectors)
    {
      + settings_పట్టిక.sub_array (ప్రారంభం_స్థానభ్రంశం, selectors_లెక్క)
      | hb_map ([=] (స్థిరరాశి SettingName& setting) { తిరిగి setting.get_info (default_selector); })
      | hb_sink (hb_array (selectors, *selectors_లెక్క))
      ;
    }
    తిరిగి settings_పట్టిక.నిడివి;
  }

  hb_aat_layout_feature_type_t get_feature_type () స్థిరరాశి
  { తిరిగి (hb_aat_layout_feature_type_t) (గుర్తులేని పూర్ణాంకం) feature; }

  hb_ot_name_id_t get_feature_name_id () స్థిరరాశి { తిరిగి name_సూచీ; }

  తార్కిక is_exclusive () స్థిరరాశి { తిరిగి featureFlags & Exclusive; }

  /* A FeatureName with no settings is meaningless */
  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి nSettings; }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  (పునాది+setting_పట్టిక_Z).sanitize (c, nSettings)));
  }

  రక్షిత:
  HBUINT16	feature;	/* Feature type. */
  HBUINT16	nSettings;	/* The number of records in the setting name array. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<SettingName>>
		setting_పట్టిక_Z;	/* Offset in bytes from the beginning of this table to
				 * this feature's setting name array. The actual type of
				 * record this offset refers to will depend on the
				 * exclusivity value, as described below. */
  HBUINT16	featureFlags;	/* Single-bit flags associated with the feature type. */
  HBINT16	name_సూచీ;	/* The name table index for the feature's name.
				 * This index has values greater than 255 and
				 * less than 32768. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

నిర్మాణం feat
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_AAT_ట్యాగ్_feat;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }

  గుర్తులేని పూర్ణాంకం get_feature_types (గుర్తులేని పూర్ణాంకం                  ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం                 *లెక్క,
				  hb_aat_layout_feature_type_t *features) స్థిరరాశి
  {
    ఒకవేళ (లెక్క && features)
    {
      + పేర్లు_Z.as_array (feature_Name_లెక్క).sub_array (ప్రారంభం_స్థానభ్రంశం, లెక్క)
      | hb_map (&FeatureName::get_feature_type)
      | hb_sink (hb_array (features, *లెక్క))
      ;
    }
    తిరిగి feature_Name_లెక్క;
  }

  తార్కిక exposes_feature (hb_aat_layout_feature_type_t feature_type) స్థిరరాశి
  { తిరిగి get_feature (feature_type).has_దత్తాంశం (); }

  స్థిరరాశి FeatureName& get_feature (hb_aat_layout_feature_type_t feature_type) స్థిరరాశి
  { తిరిగి పేర్లు_Z.bsearch (feature_Name_లెక్క, feature_type); }

  hb_ot_name_id_t get_feature_name_id (hb_aat_layout_feature_type_t feature) స్థిరరాశి
  { తిరిగి get_feature (feature).get_feature_name_id (); }

  గుర్తులేని పూర్ణాంకం get_selector_infos (hb_aat_layout_feature_type_t           feature_type,
				   గుర్తులేని పూర్ణాంకం                           ప్రారంభం_స్థానభ్రంశం,
				   గుర్తులేని పూర్ణాంకం                          *selectors_లెక్క, /* IN/OUT.  May be NULL. */
				   hb_aat_layout_feature_selector_info_t *selectors,       /* OUT.     May be NULL. */
				   గుర్తులేని పూర్ణాంకం                          *default_సూచీ    /* OUT.     May be NULL. */) స్థిరరాశి
  {
    తిరిగి get_feature (feature_type).get_selector_infos (ప్రారంభం_స్థానభ్రంశం, selectors_లెక్క, selectors,
							  default_సూచీ, ఇది);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  సంస్కరణ.major == 1 &&
			  పేర్లు_Z.sanitize (c, feature_Name_లెక్క, ఇది)));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version number of the feature name table
				 * (0x00010000 for the current version). */
  HBUINT16	feature_Name_లెక్క;
				/* The number of entries in the feature name array. */
  HBUINT16	reserved1;	/* Reserved (set to zero). */
  HBUINT32	reserved2;	/* Reserved (set to zero). */
  Sorted_Unsized_Array_యొక్క<FeatureName>
		పేర్లు_Z;		/* The feature name array. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (12, పేర్లు_Z);
};

} /* namespace AAT */

#అయితే_ముగింపు /* HB_AAT_LAYOUT_FEAT_TABLE_HH */

/*
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2010,2011,2012,2013  Google, Inc.
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
 *
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_MAP_HH
#నిర్వచించు HB_OT_MAP_HH

#అన్తర్భావ "hb-buffer.hh"


#నిర్వచించు HB_OT_MAP_MAX_బిట్లు 8u
#నిర్వచించు HB_OT_MAP_MAX_విలువ ((1u << HB_OT_MAP_MAX_బిట్లు) - 1u)

నిర్మాణం hb_ot_shape_plan_t;

స్థిర స్థిరరాశి hb_ట్యాగ్_t పట్టిక_ట్యాగులు[2] = {HB_OT_ట్యాగ్_GSUB, HB_OT_ట్యాగ్_GPOS};

నిర్మాణం hb_ot_map_t
{
  మిత్ర నిర్మాణం hb_ot_map_builder_t;

  బహిరంగ:

  నిర్మాణం feature_map_t {
    hb_ట్యాగ్_t ట్యాగ్_2; /* should be first for our bsearch to work */
    గుర్తులేని పూర్ణాంకం సూచీ_2[2]; /* GSUB/GPOS */
    గుర్తులేని పూర్ణాంకం stage[2]; /* GSUB/GPOS */
    గుర్తులేని పూర్ణాంకం జరుపు;
    hb_ముసుగు_t ముసుగు;
    hb_ముసుగు_t _1_mask; /* mask for value=1, for quick access */
    గుర్తులేని పూర్ణాంకం needs_fallback : 1;
    గుర్తులేని పూర్ణాంకం auto_zwnj : 1;
    గుర్తులేని పూర్ణాంకం auto_zwj : 1;
    గుర్తులేని పూర్ణాంకం random : 1;
    గుర్తులేని పూర్ణాంకం చొప్పున_syllable : 1;

    పూర్ణాంకం cmp (స్థిరరాశి hb_ట్యాగ్_t ట్యాగ్_4) స్థిరరాశి
    { తిరిగి ట్యాగ్_4 < ట్యాగ్_2 ? -1 : ట్యాగ్_4 > ట్యాగ్_2 ? 1 : 0; }

    HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
    {
      స్థిరరాశి feature_map_t *a = (స్థిరరాశి feature_map_t *) pa;
      స్థిరరాశి feature_map_t *b = (స్థిరరాశి feature_map_t *) pb;
      తిరిగి a->ట్యాగ్_2 < b->ట్యాగ్_2 ? -1 : a->ట్యాగ్_2 > b->ట్యాగ్_2 ? 1 : 0;
    }
  };

  నిర్మాణం lookup_map_t {
    గుర్తులేని పొట్టి సూచీ_2;
    గుర్తులేని పొట్టి auto_zwnj : 1;
    గుర్తులేని పొట్టి auto_zwj : 1;
    గుర్తులేని పొట్టి random : 1;
    గుర్తులేని పొట్టి చొప్పున_syllable : 1;
    hb_ముసుగు_t ముసుగు;
    hb_ట్యాగ్_t feature_ట్యాగ్;

    HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
    {
      స్థిరరాశి lookup_map_t *a = (స్థిరరాశి lookup_map_t *) pa;
      స్థిరరాశి lookup_map_t *b = (స్థిరరాశి lookup_map_t *) pb;
      తిరిగి a->సూచీ_2 < b->సూచీ_2 ? -1 : a->సూచీ_2 > b->సూచీ_2 ? 1 : 0;
    }
  };

  /* Pause functions return true if new glyph indices might have been
   * added to the buffer.  This is used to update buffer digest. */
  రకనిర్వచనం తార్కిక (*pause_func_t) (స్థిరరాశి నిర్మాణం hb_ot_shape_plan_t *plan, hb_font_t *font, hb_బఫరు_t *బఫరు);

  నిర్మాణం stage_map_t {
    గుర్తులేని పూర్ణాంకం చివరి_lookup; /* Cumulative */
    pause_func_t pause_func;
  };

  శూన్య init ()
  {
    hb_memset (ఇది, 0, పరిమాణం (*ఇది));

    features.init0 ();
    కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
    {
      lookups[పట్టిక_సూచీ].init0 ();
      stages[పట్టిక_సూచీ].init0 ();
    }
  }
  శూన్య fini ()
  {
    features.fini ();
    కొరకు (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ = 0; పట్టిక_సూచీ < 2; పట్టిక_సూచీ++)
    {
      lookups[పట్టిక_సూచీ].fini ();
      stages[పట్టిక_సూచీ].fini ();
    }
  }

  hb_ముసుగు_t get_global_ముసుగు () స్థిరరాశి { తిరిగి global_ముసుగు; }

  hb_ముసుగు_t get_ముసుగు (hb_ట్యాగ్_t feature_ట్యాగ్, గుర్తులేని పూర్ణాంకం *జరుపు = శూన్యసూచిక) స్థిరరాశి
  {
    స్థిరరాశి feature_map_t *map = features.bsearch (feature_ట్యాగ్);
    ఒకవేళ (జరుపు) *జరుపు = map ? map->జరుపు : 0;
    తిరిగి map ? map->ముసుగు : 0;
  }

  తార్కిక needs_fallback (hb_ట్యాగ్_t feature_ట్యాగ్) స్థిరరాశి
  {
    స్థిరరాశి feature_map_t *map = features.bsearch (feature_ట్యాగ్);
    తిరిగి map ? map->needs_fallback : అబద్ధం;
  }

  hb_ముసుగు_t get_1_ముసుగు (hb_ట్యాగ్_t feature_ట్యాగ్) స్థిరరాశి
  {
    స్థిరరాశి feature_map_t *map = features.bsearch (feature_ట్యాగ్);
    తిరిగి map ? map->_1_mask : 0;
  }

  గుర్తులేని పూర్ణాంకం get_feature_సూచీ (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ, hb_ట్యాగ్_t feature_ట్యాగ్) స్థిరరాశి
  {
    స్థిరరాశి feature_map_t *map = features.bsearch (feature_ట్యాగ్);
    తిరిగి map ? map->సూచీ_2[పట్టిక_సూచీ] : HB_OT_LAYOUT_NO_FEATURE_సూచీ;
  }

  గుర్తులేని పూర్ణాంకం get_feature_stage (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ, hb_ట్యాగ్_t feature_ట్యాగ్) స్థిరరాశి
  {
    స్థిరరాశి feature_map_t *map = features.bsearch (feature_ట్యాగ్);
    తిరిగి map ? map->stage[పట్టిక_సూచీ] : UINT_MAX;
  }

  hb_array_t<స్థిరరాశి hb_ot_map_t::lookup_map_t>
  get_stage_lookups (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ, గుర్తులేని పూర్ణాంకం stage) స్థిరరాశి
  {
    ఒకవేళ (unlikely (stage > stages[పట్టిక_సూచీ].నిడివి))
      తిరిగి hb_array<స్థిరరాశి hb_ot_map_t::lookup_map_t> (శూన్యసూచిక, 0);

    గుర్తులేని పూర్ణాంకం ప్రారంభం = stage ? stages[పట్టిక_సూచీ][stage - 1].చివరి_lookup : 0;
    గుర్తులేని పూర్ణాంకం చివర   = stage < stages[పట్టిక_సూచీ].నిడివి ? stages[పట్టిక_సూచీ][stage].చివరి_lookup : lookups[పట్టిక_సూచీ].నిడివి;
    తిరిగి lookups[పట్టిక_సూచీ].as_array ().sub_array (ప్రారంభం, చివర - ప్రారంభం);
  }

  HB_అంతర్గత శూన్య collect_lookups (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ, hb_సమితి_t *lookups) స్థిరరాశి;
  మూస <రకం_పేరు Proxy>
  HB_అంతర్గత శూన్య apply (స్థిరరాశి Proxy &proxy,
			  స్థిరరాశి నిర్మాణం hb_ot_shape_plan_t *plan, hb_font_t *font, hb_బఫరు_t *బఫరు) స్థిరరాశి;
  HB_అంతర్గత శూన్య substitute (స్థిరరాశి నిర్మాణం hb_ot_shape_plan_t *plan, hb_font_t *font, hb_బఫరు_t *బఫరు) స్థిరరాశి;
  HB_అంతర్గత శూన్య position (స్థిరరాశి నిర్మాణం hb_ot_shape_plan_t *plan, hb_font_t *font, hb_బఫరు_t *బఫరు) స్థిరరాశి;
  HB_అంతర్గత గుర్తులేని పూర్ణాంకం get_feature_ట్యాగులు (గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
					     గుర్తులేని పూర్ణాంకం *ట్యాగ్_లెక్క, /* IN/OUT */
					     hb_ట్యాగ్_t     *ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి;

  బహిరంగ:
  hb_ట్యాగ్_t chosen_script[2];
  తార్కిక దొరికింది_script[2];

  అంతరంగ:

  hb_ముసుగు_t global_ముసుగు = 0;

  hb_sorted_సదిశ_t<feature_map_t> features;
  hb_సదిశ_t<lookup_map_t> lookups[2]; /* GSUB/GPOS */
  hb_సదిశ_t<stage_map_t> stages[2]; /* GSUB/GPOS */
};

గణన hb_ot_map_feature_flags_t
{
  F_NONE		= 0x0000u,
  F_GLOBAL		= 0x0001u, /* Feature applies to all characters; results in no mask allocated for it. */
  F_HAS_FALLBACK	= 0x0002u, /* Has fallback implementation, so include mask bit even if feature not found. */
  F_MANUAL_ZWNJ		= 0x0004u, /* Don't skip over ZWNJ when matching **context**. */
  F_MANUAL_ZWJ		= 0x0008u, /* Don't skip over ZWJ when matching **input**. */
  F_MANUAL_JOINERS	= F_MANUAL_ZWNJ | F_MANUAL_ZWJ,
  F_GLOBAL_MANUAL_JOINERS= F_GLOBAL | F_MANUAL_JOINERS,
  F_GLOBAL_HAS_FALLBACK = F_GLOBAL | F_HAS_FALLBACK,
  F_GLOBAL_శోధన	= 0x0010u, /* If feature not found in LangSys, look for it in global feature list and pick one. */
  F_RANDOM		= 0x0020u, /* Randomly select a glyph from an AlternateSubstFormat1 subtable. */
  F_చొప్పున_SYLLABLE	= 0x0040u  /* Contain lookup application to within syllable. */
};
HB_MARK_AS_FLAG_T (hb_ot_map_feature_flags_t);


నిర్మాణం hb_ot_map_feature_t
{
  hb_ట్యాగ్_t ట్యాగ్_2;
  hb_ot_map_feature_flags_t flags;
};

నిర్మాణం hb_ot_shape_plan_కీలకం_t;

నిర్మాణం hb_ot_map_builder_t
{
  బహిరంగ:

  HB_అంతర్గత hb_ot_map_builder_t (hb_face_t *face_,
				   స్థిరరాశి hb_segment_properties_t &props_);

  HB_అంతర్గత ~hb_ot_map_builder_t ();

  HB_అంతర్గత శూన్య కూడు_feature (hb_ట్యాగ్_t ట్యాగ్_2,
				hb_ot_map_feature_flags_t flags=F_NONE,
				గుర్తులేని పూర్ణాంకం value=1);

  HB_అంతర్గత తార్కిక has_feature (hb_ట్యాగ్_t ట్యాగ్_2);

  శూన్య కూడు_feature (స్థిరరాశి hb_ot_map_feature_t &feat)
  { కూడు_feature (feat.ట్యాగ్_2, feat.flags); }

  శూన్య enable_feature (hb_ట్యాగ్_t ట్యాగ్_2,
		       hb_ot_map_feature_flags_t flags=F_NONE,
		       గుర్తులేని పూర్ణాంకం value=1)
  { కూడు_feature (ట్యాగ్_2, F_GLOBAL | flags, value); }

  శూన్య disable_feature (hb_ట్యాగ్_t ట్యాగ్_2)
  { కూడు_feature (ట్యాగ్_2, F_GLOBAL, 0); }

  శూన్య కూడు_gsub_pause (hb_ot_map_t::pause_func_t pause_func)
  { కూడు_pause (0, pause_func); }
  శూన్య కూడు_gpos_pause (hb_ot_map_t::pause_func_t pause_func)
  { కూడు_pause (1, pause_func); }

  HB_అంతర్గత శూన్య compile (hb_ot_map_t                  &m,
			    స్థిరరాశి hb_ot_shape_plan_కీలకం_t &key);

  అంతరంగ:

  HB_అంతర్గత శూన్య కూడు_lookups (hb_ot_map_t  &m,
				గుర్తులేని పూర్ణాంకం  పట్టిక_సూచీ,
				గుర్తులేని పూర్ణాంకం  feature_సూచీ,
				స్థిరరాశి hb_ot_shape_plan_కీలకం_t &key,
				hb_ముసుగు_t     ముసుగు,
				తార్కిక          auto_zwnj = నిజం,
				తార్కిక          auto_zwj = నిజం,
				తార్కిక          random = అబద్ధం,
				తార్కిక          చొప్పున_syllable = అబద్ధం,
				hb_ట్యాగ్_t      feature_ట్యాగ్ = HB_ట్యాగ్(' ',' ',' ',' '));

  నిర్మాణం feature_info_t {
    hb_ట్యాగ్_t ట్యాగ్_2;
    గుర్తులేని పూర్ణాంకం seq; /* sequence#, used for stable sorting only */
    గుర్తులేని పూర్ణాంకం max_విలువ;
    hb_ot_map_feature_flags_t flags;
    గుర్తులేని పూర్ణాంకం default_విలువ; /* for non-global features, what should the unset glyphs take */
    గుర్తులేని పూర్ణాంకం stage[2]; /* GSUB/GPOS */

    HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
    {
      స్థిరరాశి feature_info_t *a = (స్థిరరాశి feature_info_t *) pa;
      స్థిరరాశి feature_info_t *b = (స్థిరరాశి feature_info_t *) pb;
      తిరిగి (a->ట్యాగ్_2 != b->ట్యాగ్_2) ?  (a->ట్యాగ్_2 < b->ట్యాగ్_2 ? -1 : 1) :
	     (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }
  };

  నిర్మాణం stage_info_t {
    గుర్తులేని పూర్ణాంకం సూచీ_2;
    hb_ot_map_t::pause_func_t pause_func;
  };

  HB_అంతర్గత శూన్య కూడు_pause (గుర్తులేని పూర్ణాంకం పట్టిక_సూచీ, hb_ot_map_t::pause_func_t pause_func);

  బహిరంగ:

  hb_face_t *face;
  hb_segment_properties_t props;
  తార్కిక is_simple;

  hb_ట్యాగ్_t chosen_script[2];
  తార్కిక దొరికింది_script[2];
  గుర్తులేని పూర్ణాంకం script_సూచీ[2], language_సూచీ[2];

  అంతరంగ:

  గుర్తులేని పూర్ణాంకం current_stage[2]; /* GSUB/GPOS */
  hb_సదిశ_t<feature_info_t> feature_infos;
  hb_సదిశ_t<stage_info_t> stages[2]; /* GSUB/GPOS */
};



#అయితే_ముగింపు /* HB_OT_MAP_HH */

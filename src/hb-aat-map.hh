/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_AAT_MAP_HH
#నిర్వచించు HB_AAT_MAP_HH

#అన్తర్భావ "hb.hh"


నిర్మాణం hb_aat_map_t
{
  మిత్ర నిర్మాణం hb_aat_map_builder_t;

  బహిరంగ:
  నిర్మాణం range_flags_t
  {
    hb_ముసుగు_t flags;
    గుర్తులేని cluster_మొదటి;
    గుర్తులేని cluster_చివరి; // end - 1
  };

  బహిరంగ:
  hb_సదిశ_t<hb_sorted_సదిశ_t<range_flags_t>> chain_flags;
};

నిర్మాణం hb_aat_map_builder_t
{
  బహిరంగ:

  HB_అంతర్గత hb_aat_map_builder_t (hb_face_t *face_,
				    స్థిరరాశి hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_అంతర్గత శూన్య కూడు_feature (స్థిరరాశి hb_feature_t &feature);

  HB_అంతర్గత శూన్య compile (hb_aat_map_t  &m);

  బహిరంగ:
  నిర్మాణం feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    తార్కిక is_exclusive;
    గుర్తులేని  seq; /* For stable sorting only. */

    HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
    {
      స్థిరరాశి feature_info_t *a = (స్థిరరాశి feature_info_t *) pa;
      స్థిరరాశి feature_info_t *b = (స్థిరరాశి feature_info_t *) pb;
      ఒకవేళ (a->type != b->type) తిరిగి (a->type < b->type ? -1 : 1);
      ఒకవేళ (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) తిరిగి (a->setting < b->setting ? -1 : 1);
	    తిరిగి (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    పూర్ణాంకం cmp (స్థిరరాశి feature_info_t& f) స్థిరరాశి
    {
      తిరిగి (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  నిర్మాణం feature_range_t
  {
    feature_info_t info;
    గుర్తులేని ప్రారంభం;
    గుర్తులేని చివర;
  };

  అంతరంగ:
  నిర్మాణం feature_event_t
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2;
    తార్కిక ప్రారంభం;
    feature_info_t feature;

    HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb) {
      స్థిరరాశి feature_event_t *a = (స్థిరరాశి feature_event_t *) pa;
      స్థిరరాశి feature_event_t *b = (స్థిరరాశి feature_event_t *) pb;
      తిరిగి a->సూచీ_2 < b->సూచీ_2 ? -1 : a->సూచీ_2 > b->సూచీ_2 ? 1 :
	     a->ప్రారంభం < b->ప్రారంభం ? -1 : a->ప్రారంభం > b->ప్రారంభం ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  బహిరంగ:
  hb_face_t *face;
  hb_segment_properties_t props;

  బహిరంగ:
  hb_sorted_సదిశ_t<feature_range_t> features;
  hb_sorted_సదిశ_t<feature_info_t> current_features;
  గుర్తులేని range_మొదటి = HB_FEATURE_GLOBAL_ప్రారంభం;
  గుర్తులేని range_చివరి = HB_FEATURE_GLOBAL_చివర;
};


#అయితే_ముగింపు /* HB_AAT_MAP_HH */

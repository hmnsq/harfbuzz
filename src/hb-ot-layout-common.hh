/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_OT_LAYOUT_COMMON_HH
#నిర్వచించు HB_OT_LAYOUT_COMMON_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-ot-layout.hh"
#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-bimap.hh"
#అన్తర్భావ "hb-cache.hh"
#అన్తర్భావ "hb-ot-dual.hh"

ఉపయోగించి hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (పరిమాణం (hb_ot_layout_mapping_cache_t) == 512, "");

ఉపయోగించి hb_ot_layout_ద్విమాన_cache_t = hb_cache_t<14, 1, 8>;
static_assert (పరిమాణం (hb_ot_layout_ద్విమాన_cache_t) == 256, "");

#అన్తర్భావ "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
ఉపయోగించి OT::Layout::Common::Coverage;
ఉపయోగించి OT::Layout::Common::Range_రికార్డు;
ఉపయోగించి OT::Layout::SmallTypes;
ఉపయోగించి OT::Layout::MediumTypes;


నామాకాశ OT {

మూస<రకం_పేరు Iterator>
స్థిర అంతఃవరుస తార్కిక ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

స్థిర తార్కిక ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    స్థిరరాశి hb_సమితి_t &klasses,
    తార్కిక use_class_zero,
    hb_sorted_సదిశ_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

నిర్మాణం అంశం_Var_Store_Instancer;

నిర్మాణం hb_collect_feature_substitutes_with_var_context_t
{
  స్థిరరాశి hb_map_t *axes_సూచీ_ట్యాగ్_map;
  స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *axes_location;
  hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *రికార్డు_cond_idx_map;
  hb_hashmap_t<గుర్తులేని, స్థిరరాశి Feature*> *feature_substitutes_map;
  hb_సమితి_t& catch_all_రికార్డు_feature_idxes;

  // not stored in subset_plan
  hb_సమితి_t *feature_indices;
  తార్కిక apply;
  తార్కిక variation_applied;
  తార్కిక universal;
  గుర్తులేని cur_రికార్డు_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, గుర్తులేని> *conditionset_map;
  అంశం_Var_Store_Instancer *instancer;
  తార్కిక all_axes_pinned;
};

నిర్మాణం hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (స్థిరరాశి శూన్య         *పట్టిక_2,
                              hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> *script_langsys_map_,
                              స్థిరరాశి hb_map_t     *duplicate_feature_map_,
                              hb_సమితి_t           *new_collected_feature_indexes_)
      :పట్టిక (పట్టిక_2),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_లెక్క (0),langsys_feature_లెక్క (0) {}

  తార్కిక సందర్శించు_Script ()
  { తిరిగి script_లెక్క++ < HB_MAX_SCRIPTS; }

  తార్కిక సందర్శించు_Langsys (గుర్తులేని feature_లెక్క_2)
  {
    langsys_feature_లెక్క += feature_లెక్క_2;
    తిరిగి langsys_feature_లెక్క < HB_MAX_LANGSYS_FEATURE_లెక్క;
  }

  బహిరంగ:
  స్థిరరాశి శూన్య *పట్టిక;
  hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> *script_langsys_map;
  స్థిరరాశి hb_map_t     *duplicate_feature_map;
  hb_సమితి_t           *new_feature_indexes;

  అంతరంగ:
  గుర్తులేని script_లెక్క;
  గుర్తులేని langsys_feature_లెక్క;
};

నిర్మాణం hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_ఖాళీలు_t, HB_DEBUG_SUBSET>
{
  స్థిరరాశి అక్షరం *get_name () { తిరిగి "SUBSET_LAYOUT"; }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }

  తార్కిక సందర్శించు_Script ()
  {
    తిరిగి script_లెక్క++ < HB_MAX_SCRIPTS;
  }

  తార్కిక సందర్శించు_Lang_Sys ()
  {
    తిరిగి langsys_లెక్క++ < HB_MAX_LANGSYS;
  }

  తార్కిక సందర్శించు_Feature_సూచీ (పూర్ణాంకం లెక్క)
  {
    feature_సూచీ_లెక్క += లెక్క;
    తిరిగి feature_సూచీ_లెక్క < HB_MAX_FEATURE_INDICES;
  }

  తార్కిక సందర్శించు_Lookup_సూచీ()
  {
    lookup_సూచీ_లెక్క++;
    తిరిగి lookup_సూచీ_లెక్క < HB_MAX_LOOKUP_సందర్శించు_లెక్క;
  }

  hb_subset_context_t *subset_context;
  స్థిరరాశి hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2;
  స్థిరరాశి hb_map_t *lookup_సూచీ_map;
  స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb::unique_ptr<hb_సమితి_t>> *script_langsys_map;
  స్థిరరాశి hb_map_t *feature_సూచీ_map;
  స్థిరరాశి hb_map_t *feature_map_w_duplicates;
  స్థిరరాశి hb_hashmap_t<గుర్తులేని, స్థిరరాశి Feature*> *feature_substitutes_map;
  స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *lookup_variations;
  hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map;
  స్థిరరాశి hb_సమితి_t *catch_all_రికార్డు_feature_idxes;
  స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*>> *feature_idx_ట్యాగ్_map;

  గుర్తులేని cur_script_సూచీ;
  గుర్తులేని cur_feature_సూచీ;
  గుర్తులేని cur_feature_var_రికార్డు_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_ట్యాగ్_t ట్యాగ్_4) :
				subset_context (c_),
				పట్టిక_ట్యాగ్_2 (ట్యాగ్_4),
				cur_script_సూచీ (0xFFFFu),
				cur_feature_సూచీ (సూచీ::NOT_దొరికింది_సూచీ),
				cur_feature_var_రికార్డు_idx (0u),
				script_లెక్క (0),
				langsys_లెక్క (0),
				feature_సూచీ_లెక్క (0),
				lookup_సూచీ_లెక్క (0)
  {
    ఒకవేళ (ట్యాగ్_4 == HB_OT_ట్యాగ్_GSUB)
    {
      lookup_సూచీ_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_సూచీ_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_రికార్డు_cond_idx_map = c_->plan->user_axes_location.is_empty () ? శూన్యసూచిక : &c_->plan->gsub_feature_రికార్డు_cond_idx_map;
      catch_all_రికార్డు_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_ట్యాగ్_map = &c_->plan->gsub_old_feature_idx_ట్యాగ్_map;
    }
    లేకపోతే
    {
      lookup_సూచీ_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_సూచీ_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_రికార్డు_cond_idx_map = c_->plan->user_axes_location.is_empty () ? శూన్యసూచిక : &c_->plan->gpos_feature_రికార్డు_cond_idx_map;
      catch_all_రికార్డు_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_ట్యాగ్_map = &c_->plan->gpos_old_feature_idx_ట్యాగ్_map;
    }
  }

  అంతరంగ:
  గుర్తులేని script_లెక్క;
  గుర్తులేని langsys_లెక్క;
  గుర్తులేని feature_సూచీ_లెక్క;
  గుర్తులేని lookup_సూచీ_లెక్క;
};

నిర్మాణం అంశం_Variation_Store;
నిర్మాణం hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj) { obj.collect_variation_indices (ఇది); తిరిగి hb_ఖాళీలు_t (); }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }

  hb_సమితి_t *layout_variation_indices;
  స్థిరరాశి hb_సమితి_t *glyph_సమితి;
  స్థిరరాశి hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *condition_విలువలు = శూన్యసూచిక;

  hb_collect_variation_indices_context_t (hb_సమితి_t *layout_variation_indices_,
					  స్థిరరాశి hb_సమితి_t *glyph_సమితి_2,
					  స్థిరరాశి hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_సమితి (glyph_సమితి_2),
					gpos_lookups (gpos_lookups_) {}
};

మూస<రకం_పేరు OutputArray>
నిర్మాణం subset_స్థానభ్రంశం_array_t
{
  subset_స్థానభ్రంశం_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& బహిర్గతం_4,
			 స్థిరరాశి శూన్య *పునాది_2) : subset_context (subset_context_),
					      బహిర్గతం (బహిర్గతం_4), పునాది (పునాది_2) {}

  మూస <రకం_పేరు T>
  తార్కిక క్రియాకారకం () (T&& స్థానభ్రంశం)
  {
    స్వయంచాలక snap = subset_context->serializer->snapshot ();
    స్వయంచాలక *o = బహిర్గతం.serialize_append (subset_context->serializer);
    ఒకవేళ (unlikely (!o)) తిరిగి అబద్ధం;
    తార్కిక ret = o->serialize_subset (subset_context, స్థానభ్రంశం, పునాది);
    ఒకవేళ (!ret)
    {
      బహిర్గతం.తీయి ();
      subset_context->serializer->revert (snap);
    }
    తిరిగి ret;
  }

  అంతరంగ:
  hb_subset_context_t *subset_context;
  OutputArray &బహిర్గతం;
  స్థిరరాశి శూన్య *పునాది;
};


మూస<రకం_పేరు OutputArray, రకం_పేరు Arg>
నిర్మాణం subset_స్థానభ్రంశం_array_arg_t
{
  subset_స్థానభ్రంశం_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& బహిర్గతం_4,
			     స్థిరరాశి శూన్య *పునాది_2,
			     Arg &&arg_) : subset_context (subset_context_), బహిర్గతం (బహిర్గతం_4),
					  పునాది (పునాది_2), arg (arg_) {}

  మూస <రకం_పేరు T>
  తార్కిక క్రియాకారకం () (T&& స్థానభ్రంశం)
  {
    స్వయంచాలక snap = subset_context->serializer->snapshot ();
    స్వయంచాలక *o = బహిర్గతం.serialize_append (subset_context->serializer);
    ఒకవేళ (unlikely (!o)) తిరిగి అబద్ధం;
    తార్కిక ret = o->serialize_subset (subset_context, స్థానభ్రంశం, పునాది, arg);
    ఒకవేళ (!ret)
    {
      బహిర్గతం.తీయి ();
      subset_context->serializer->revert (snap);
    }
    తిరిగి ret;
  }

  అంతరంగ:
  hb_subset_context_t *subset_context;
  OutputArray &బహిర్గతం;
  స్థిరరాశి శూన్య *పునాది;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
నిర్మాణం
{
  మూస<రకం_పేరు OutputArray>
  subset_స్థానభ్రంశం_array_t<OutputArray>
  క్రియాకారకం () (hb_subset_context_t *subset_context, OutputArray& బహిర్గతం,
	       స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  { తిరిగి subset_స్థానభ్రంశం_array_t<OutputArray> (subset_context, బహిర్గతం, పునాది); }

  /* Variant with one extra argument passed to serialize_subset */
  మూస<రకం_పేరు OutputArray, రకం_పేరు Arg>
  subset_స్థానభ్రంశం_array_arg_t<OutputArray, Arg>
  క్రియాకారకం () (hb_subset_context_t *subset_context, OutputArray& బహిర్గతం,
	       స్థిరరాశి శూన్య *పునాది, Arg &&arg) స్థిరరాశి
  { తిరిగి subset_స్థానభ్రంశం_array_arg_t<OutputArray, Arg> (subset_context, బహిర్గతం, పునాది, arg); }
}
HB_FUNCOBJ (subset_స్థానభ్రంశం_array);

మూస<రకం_పేరు OutputArray>
నిర్మాణం subset_రికార్డు_array_t
{
  subset_రికార్డు_array_t (hb_subset_layout_context_t *c_, OutputArray* బహిర్గతం_4,
			 స్థిరరాశి శూన్య *పునాది_2) : subset_layout_context (c_),
					      బహిర్గతం (బహిర్గతం_4), పునాది (పునాది_2) {}

  మూస <రకం_పేరు T>
  శూన్య
  క్రియాకారకం () (T&& రికార్డు)
  {
    స్వయంచాలక snap = subset_layout_context->subset_context->serializer->snapshot ();
    తార్కిక ret = రికార్డు.subset (subset_layout_context, పునాది);
    ఒకవేళ (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    లేకపోతే బహిర్గతం->len++;
  }

  అంతరంగ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *బహిర్గతం;
  స్థిరరాశి శూన్య *పునాది;
};

మూస<రకం_పేరు OutputArray, రకం_పేరు Arg>
నిర్మాణం subset_రికార్డు_array_arg_t
{
  subset_రికార్డు_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* బహిర్గతం_4,
			     స్థిరరాశి శూన్య *పునాది_2,
			     Arg &&arg_) : subset_layout_context (c_),
					   బహిర్గతం (బహిర్గతం_4), పునాది (పునాది_2), arg (arg_) {}

  మూస <రకం_పేరు T>
  శూన్య
  క్రియాకారకం () (T&& రికార్డు)
  {
    స్వయంచాలక snap = subset_layout_context->subset_context->serializer->snapshot ();
    తార్కిక ret = రికార్డు.subset (subset_layout_context, పునాది, arg);
    ఒకవేళ (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    లేకపోతే బహిర్గతం->len++;
  }

  అంతరంగ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *బహిర్గతం;
  స్థిరరాశి శూన్య *పునాది;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
నిర్మాణం
{
  మూస<రకం_పేరు OutputArray>
  subset_రికార్డు_array_t<OutputArray>
  క్రియాకారకం () (hb_subset_layout_context_t *c, OutputArray* బహిర్గతం,
	       స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  { తిరిగి subset_రికార్డు_array_t<OutputArray> (c, బహిర్గతం, పునాది); }

  /* Variant with one extra argument passed to subset */
  మూస<రకం_పేరు OutputArray, రకం_పేరు Arg>
  subset_రికార్డు_array_arg_t<OutputArray, Arg>
  క్రియాకారకం () (hb_subset_layout_context_t *c, OutputArray* బహిర్గతం,
               స్థిరరాశి శూన్య *పునాది, Arg &&arg) స్థిరరాశి
  { తిరిగి subset_రికార్డు_array_arg_t<OutputArray, Arg> (c, బహిర్గతం, పునాది, arg); }
}
HB_FUNCOBJ (subset_రికార్డు_array);


మూస<రకం_పేరు OutputArray>
నిర్మాణం serialize_math_రికార్డు_array_t
{
  serialize_math_రికార్డు_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& బహిర్గతం_4,
                         స్థిరరాశి శూన్య *పునాది_2,
                         స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              బహిర్గతం (బహిర్గతం_4), పునాది (పునాది_2), variation_map (variation_map_) {}

  మూస <రకం_పేరు T>
  తార్కిక క్రియాకారకం () (T&& రికార్డు)
  {
    ఒకవేళ (!serialize_context->copy (రికార్డు, పునాది, variation_map)) తిరిగి అబద్ధం;
    బహిర్గతం.len++;
    తిరిగి నిజం;
  }

  అంతరంగ:
  hb_serialize_context_t *serialize_context;
  OutputArray &బహిర్గతం;
  స్థిరరాశి శూన్య *పునాది;
  స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
నిర్మాణం
{
  మూస<రకం_పేరు OutputArray>
  serialize_math_రికార్డు_array_t<OutputArray>
  క్రియాకారకం () (hb_serialize_context_t *serialize_context, OutputArray& బహిర్గతం,
               స్థిరరాశి శూన్య *పునాది,
               స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *variation_map) స్థిరరాశి
  { తిరిగి serialize_math_రికార్డు_array_t<OutputArray> (serialize_context, బహిర్గతం, పునాది, variation_map); }

}
HB_FUNCOBJ (serialize_math_రికార్డు_array);

/*
 *
 * OpenType Layout Common Table Formats
 *
 */


/*
 * Script, ScriptList, LangSys, Feature, FeatureList, Lookup, LookupList
 */

నిర్మాణం సూచీ_Array : Array16_యొక్క<సూచీ>
{
  తార్కిక intersects (స్థిరరాశి hb_map_t *indexes) స్థిరరాశి
  { తిరిగి hb_any (*ఇది, indexes); }

  మూస <రకం_పేరు Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    ఒకవేళ (!it) తిరిగి;
    ఒకవేళ (unlikely (!c->extend_min ((*ఇది)))) తిరిగి;

    కొరకు (స్థిరరాశి స్వయంచాలక _ : it)
    {
      ఒకవేళ (!l->సందర్శించు_Lookup_సూచీ()) ఆపు;

      సూచీ i;
      i = _;
      c->copy (i);
      ఇది->len++;
    }
  }

  గుర్తులేని పూర్ణాంకం get_indexes (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			    గుర్తులేని పూర్ణాంకం *లెక్క_2 /* IN/OUT */,
			    గుర్తులేని పూర్ణాంకం *_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (లెక్క_2 && _indexes)
    {
      + ఇది->as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, లెక్క_2)
      | hb_sink (hb_array (_indexes, *లెక్క_2))
      ;
    }
    తిరిగి ఇది->len;
  }

  శూన్య కూడు_indexes_గమ్యం (hb_సమితి_t* output /*బహిర్గతం*/) స్థిరరాశి
  {
    output->కూడు_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
నిర్మాణం Feature_Params_కొలత
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది))) return_trace (అబద్ధం);
    hb_barrier ();

    /* This subtable has some "history", if you will.  Some earlier versions of
     * Adobe tools calculated the offset of the FeatureParams subtable from the
     * beginning of the FeatureList table!  Now, that is dealt with in the
     * Feature implementation.  But we still need to be able to tell junk from
     * real data.  Note: We don't check that the nameID actually exists.
     *
     * Read Roberts wrote on 9/15/06 on opentype-list@indx.co.uk :
     *
     * Yes, it is correct that a new version of the AFDKO (version 2.0) will be
     * coming out soon, and that the makeotf program will build a font with a
     * 'size' feature that is correct by the specification.
     *
     * The specification for this feature tag is in the "OpenType Layout Tag
     * Registry". You can see a copy of this at:
     * https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#tag-size
     *
     * Here is one set of rules to determine if the 'size' feature is built
     * correctly, or as by the older versions of MakeOTF. You may be able to do
     * better.
     *
     * Assume that the offset to the size feature is according to specification,
     * and make the following value checks. If it fails, assume the size
     * feature is calculated as versions of MakeOTF before the AFDKO 2.0 built it.
     * If this fails, reject the 'size' feature. The older makeOTF's calculated the
     * offset from the beginning of the FeatureList table, rather than from the
     * beginning of the 'size' Feature table.
     *
     * If "design size" == 0:
     *     fails check
     *
     * Else if ("subfamily identifier" == 0 and
     *     "range start" == 0 and
     *     "range end" == 0 and
     *     "range start" == 0 and
     *     "menu name ID" == 0)
     *     passes check: this is the format used when there is a design size
     * specified, but there is no recommended size range.
     *
     * Else if ("design size" <  "range start" or
     *     "design size" >   "range end" or
     *     "range end" <= "range start" or
     *     "menu name ID"  < 256 or
     *     "menu name ID"  > 32767 or
     *     menu name ID is not a name ID which is actually in the name table)
     *     fails test
     * Else
     *     passes test.
     */

    ఒకవేళ (!design_కొలత)
      return_trace (అబద్ధం);
    లేకపోతే ఒకవేళ (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     range_ప్రారంభం == 0 &&
	     range_చివర == 0)
      return_trace (నిజం);
    లేకపోతే ఒకవేళ (design_కొలత < range_ప్రారంభం ||
	     design_కొలత > range_చివర ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (అబద్ధం);
    లేకపోతే
      return_trace (నిజం);
  }

  శూన్య collect_name_ids (hb_సమితి_t *nameids_గమ్యం_retain /*బహిర్గతం*/) స్థిరరాశి
  { nameids_గమ్యం_retain->కూడు (subfamilyNameID); }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    return_trace ((తార్కిక) c->serializer->embed (*ఇది));
  }

  HBUINT16	design_కొలత;	/* Represents the design size in 720/inch
				 * units (decipoints).  The design size entry
				 * must be non-zero.  When there is a design
				 * size but no recommended size range, the
				 * rest of the array will consist of zeros. */
  HBUINT16	subfamilyID;	/* Has no independent meaning, but serves
				 * as an identifier that associates fonts
				 * in a subfamily. All fonts which share a
				 * Preferred or Font Family name and which
				 * differ only by size range shall have the
				 * same subfamily value, and no fonts which
				 * differ in weight or style shall have the
				 * same subfamily value. If this value is
				 * zero, the remaining fields in the array
				 * will be ignored. */
  NameID	subfamilyNameID;/* If the preceding value is non-zero, this
				 * value must be set in the range 256 - 32767
				 * (inclusive). It records the value of a
				 * field in the name table, which must
				 * contain English-language strings encoded
				 * in Windows Unicode and Macintosh Roman,
				 * and may contain additional strings
				 * localized to other scripts and languages.
				 * Each of these strings is the name an
				 * application should use, in combination
				 * with the family name, to represent the
				 * subfamily in a menu.  Applications will
				 * choose the appropriate version based on
				 * their selection criteria. */
  HBUINT16	range_ప్రారంభం;	/* Large end of the recommended usage range
				 * (inclusive), stored in 720/inch units
				 * (decipoints). */
  HBUINT16	range_చివర;	/* Small end of the recommended usage range
				   (exclusive), stored in 720/inch units
				 * (decipoints). */
  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
నిర్మాణం Feature_Params_Stylistic_సమితి
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (ఇది));
  }

  శూన్య collect_name_ids (hb_సమితి_t *nameids_గమ్యం_retain /*బహిర్గతం*/) స్థిరరాశి
  { nameids_గమ్యం_retain->కూడు (uiNameID); }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    return_trace ((తార్కిక) c->serializer->embed (*ఇది));
  }

  HBUINT16	సంస్కరణ;	/* (set to 0): This corresponds to a “minor”
				 * version number. Additional data may be
				 * added to the end of this Feature Parameters
				 * table in the future. */

  NameID	uiNameID;	/* The 'name' table name ID that specifies a
				 * string (or strings, for multiple languages)
				 * for a user-interface label for this
				 * feature.  The values of uiLabelNameId and
				 * sampleTextNameId are expected to be in the
				 * font-specific name ID range (256-32767),
				 * though that is not a requirement in this
				 * Feature Parameters specification. The
				 * user-interface label for the feature can
				 * be provided in multiple languages. An
				 * English string should be included as a
				 * fallback. The string should be kept to a
				 * minimal length to fit comfortably with
				 * different application interfaces. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
నిర్మాణం FeatureParamsCharacterVariants
{
  గుర్తులేని
  get_characters (గుర్తులేని ప్రారంభం_స్థానభ్రంశం, గుర్తులేని *char_లెక్క, hb_codepoint_t *chars) స్థిరరాశి
  {
    ఒకవేళ (char_లెక్క && chars)
    {
      + characters.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, char_లెక్క)
      | hb_sink (hb_array (chars, *char_లెక్క))
      ;
    }
    తిరిగి characters.len;
  }

  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_mul_కూడు_saturate (characters.len, HBUINT24::static_కొలత, min_కొలత); }

  శూన్య collect_name_ids (hb_సమితి_t *nameids_గమ్యం_retain /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (featUILableNameID) nameids_గమ్యం_retain->కూడు (featUILableNameID);
    ఒకవేళ (feat_UITooltip_పాఠ్యం_Name_ID) nameids_గమ్యం_retain->కూడు (feat_UITooltip_పాఠ్యం_Name_ID);
    ఒకవేళ (sample_పాఠ్యం_Name_ID) nameids_గమ్యం_retain->కూడు (sample_పాఠ్యం_Name_ID);

    ఒకవేళ (!మొదటి_Param_UILabel_Name_ID || !అంకె_Named_Parameters || అంకె_Named_Parameters >= 0x7FFF)
      తిరిగి;

    గుర్తులేని చివరి_name_id = (గుర్తులేని) మొదటి_Param_UILabel_Name_ID + (గుర్తులేని) అంకె_Named_Parameters - 1;
    nameids_గమ్యం_retain->కూడు_range (మొదటి_Param_UILabel_Name_ID, చివరి_name_id);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    return_trace ((తార్కిక) c->serializer->embed (*ఇది));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  characters.sanitize (c));
  }

  HBUINT16	format;			/* Format number is set to 0. */
  NameID	featUILableNameID;	/* The ‘name’ table name ID that
					 * specifies a string (or strings,
					 * for multiple languages) for a
					 * user-interface label for this
					 * feature. (May be NULL.) */
  NameID	feat_UITooltip_పాఠ్యం_Name_ID;/* The ‘name’ table name ID that
					 * specifies a string (or strings,
					 * for multiple languages) that an
					 * application can use for tooltip
					 * text for this feature. (May be
					 * nullptr.) */
  NameID	sample_పాఠ్యం_Name_ID;	/* The ‘name’ table name ID that
					 * specifies sample text that
					 * illustrates the effect of this
					 * feature. (May be NULL.) */
  HBUINT16	అంకె_Named_Parameters;	/* Number of named parameters. (May
					 * be zero.) */
  NameID	మొదటి_Param_UILabel_Name_ID;/* The first ‘name’ table name ID
					 * used to specify strings for
					 * user-interface labels for the
					 * feature parameters. (Must be zero
					 * if numParameters is zero.) */
  Array16_యొక్క<HBUINT24>
		characters;		/* Array of the Unicode Scalar Value
					 * of the characters for which this
					 * feature provides glyph variants.
					 * (May be zero.) */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (14, characters);
};

నిర్మాణం FeatureParams
{
  తార్కిక sanitize (hb_sanitize_context_t *c, hb_ట్యాగ్_t ట్యాగ్_2) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_LAYOUT_FEATURE_PARAMS
    తిరిగి నిజం;
#అయితే_ముగింపు
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (ట్యాగ్_2 == HB_ట్యాగ్ ('s','i','z','e') && hb_barrier ())
      return_trace (u.కొలత.sanitize (c));
    ఒకవేళ ((ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_సమితి.sanitize (c));
    ఒకవేళ ((ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (నిజం);
  }

  శూన్య collect_name_ids (hb_ట్యాగ్_t ట్యాగ్_2, hb_సమితి_t *nameids_గమ్యం_retain /*బహిర్గతం*/) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_LAYOUT_FEATURE_PARAMS
    తిరిగి;
#అయితే_ముగింపు
    ఒకవేళ (ట్యాగ్_2 == HB_ట్యాగ్ ('s','i','z','e') && hb_barrier ())
      తిరిగి (u.కొలత.collect_name_ids (nameids_గమ్యం_retain));
    ఒకవేళ ((ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('s','s','\0','\0')) /* ssXX */
      తిరిగి (u.stylistic_సమితి.collect_name_ids (nameids_గమ్యం_retain));
    ఒకవేళ ((ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('c','v','\0','\0')) /* cvXX */
      తిరిగి (u.characterVariants.collect_name_ids (nameids_గమ్యం_retain));
  }

  తార్కిక subset (hb_subset_context_t *c, స్థిరరాశి ట్యాగ్* ట్యాగ్_2) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_LAYOUT_FEATURE_PARAMS
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
    TRACE_SUBSET (ఇది);
    ఒకవేళ (!ట్యాగ్_2) return_trace (అబద్ధం);
    ఒకవేళ (*ట్యాగ్_2 == HB_ట్యాగ్ ('s','i','z','e') && hb_barrier ())
      return_trace (u.కొలత.subset (c));
    ఒకవేళ ((*ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_సమితి.subset (c));
    ఒకవేళ ((*ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (అబద్ధం);
  }

#నిర్వచితం_కాకపోతే HB_NO_LAYOUT_FEATURE_PARAMS
  స్థిరరాశి Feature_Params_కొలత& get_కొలత_params (hb_ట్యాగ్_t ట్యాగ్_2) స్థిరరాశి
  {
    ఒకవేళ (ట్యాగ్_2 == HB_ట్యాగ్ ('s','i','z','e') && hb_barrier ())
      తిరిగి u.కొలత;
    తిరిగి Null (Feature_Params_కొలత);
  }
  స్థిరరాశి Feature_Params_Stylistic_సమితి& get_stylistic_సమితి_params (hb_ట్యాగ్_t ట్యాగ్_2) స్థిరరాశి
  {
    ఒకవేళ ((ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('s','s','\0','\0')) /* ssXX */
      తిరిగి u.stylistic_సమితి;
    తిరిగి Null (Feature_Params_Stylistic_సమితి);
  }
  స్థిరరాశి FeatureParamsCharacterVariants& get_character_variants_params (hb_ట్యాగ్_t ట్యాగ్_2) స్థిరరాశి
  {
    ఒకవేళ ((ట్యాగ్_2 & 0xFFFF0000u) == HB_ట్యాగ్ ('c','v','\0','\0')) /* cvXX */
      తిరిగి u.characterVariants;
    తిరిగి Null (FeatureParamsCharacterVariants);
  }
#అయితే_ముగింపు

  అంతరంగ:
  సంఘం {
  Feature_Params_కొలత			కొలత;
  Feature_Params_Stylistic_సమితి		stylistic_సమితి;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (0);
};

నిర్మాణం రికార్డు_sanitize_సంవృతం_t {
  hb_ట్యాగ్_t ట్యాగ్_2;
  స్థిరరాశి శూన్య *list_పునాది;
};

నిర్మాణం Feature
{
  గుర్తులేని పూర్ణాంకం get_lookup_లెక్క () స్థిరరాశి
  { తిరిగి lookup_సూచీ_2.len; }
  hb_ట్యాగ్_t get_lookup_సూచీ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి lookup_సూచీ_2[i]; }
  గుర్తులేని పూర్ణాంకం get_lookup_indexes (గుర్తులేని పూర్ణాంకం ప్రారంభం_సూచీ,
				   గుర్తులేని పూర్ణాంకం *lookup_లెక్క /* IN/OUT */,
				   గుర్తులేని పూర్ణాంకం *lookup_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి lookup_సూచీ_2.get_indexes (ప్రారంభం_సూచీ, lookup_లెక్క, lookup_ట్యాగులు); }
  శూన్య కూడు_lookup_indexes_గమ్యం (hb_సమితి_t *lookup_indexes) స్థిరరాశి
  { lookup_సూచీ_2.కూడు_indexes_గమ్యం (lookup_indexes); }

  స్థిరరాశి FeatureParams &get_feature_params () స్థిరరాశి
  { తిరిగి ఇది+featureParams; }

  తార్కిక intersects_lookup_indexes (స్థిరరాశి hb_map_t *lookup_indexes) స్థిరరాశి
  { తిరిగి lookup_సూచీ_2.intersects (lookup_indexes); }

  శూన్య collect_name_ids (hb_ట్యాగ్_t ట్యాగ్_2, hb_సమితి_t *nameids_గమ్యం_retain /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (featureParams)
      get_feature_params ().collect_name_ids (ట్యాగ్_2, nameids_గమ్యం_retain);
  }

  తార్కిక subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       స్థిరరాశి ట్యాగ్                   *ట్యాగ్_2 = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->featureParams.serialize_subset (c, featureParams, ఇది, ట్యాగ్_2);

    hb::shared_ptr<hb_సమితి_t> *lookup_variations;
    ఒకవేళ (l->cur_feature_సూచీ != సూచీ::NOT_దొరికింది_సూచీ &&
	l->lookup_variations->has (l->cur_feature_సూచీ, &lookup_variations))
    {
      స్వయంచాలక it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_సూచీ_map)
      | hb_map (l->lookup_సూచీ_map)
      ;
      బహిర్గతం->lookup_సూచీ_2.serialize (c->serializer, l, it);
    }
    లేకపోతే
    {
      స్వయంచాలక it =
      + hb_iter (lookup_సూచీ_2)
      | hb_filter (l->lookup_సూచీ_map)
      | hb_map (l->lookup_సూచీ_map)
      ;
      బహిర్గతం->lookup_సూచీ_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి రికార్డు_sanitize_సంవృతం_t *సంవృతం = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  featureParams.sanitize (c, ఇది, సంవృతం ? సంవృతం->ట్యాగ్_2 : HB_ట్యాగ్_NONE) &&
		  lookup_సూచీ_2.sanitize (c));
  }

  Offset16_గమ్యం<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  సూచీ_Array	 lookup_సూచీ_2;	/* Array of LookupList indices */
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (4, lookup_సూచీ_2);
};

మూస <రకం_పేరు Type>
నిర్మాణం రికార్డు_2
{
  పూర్ణాంకం cmp (hb_ట్యాగ్_t a) స్థిరరాశి { తిరిగి ట్యాగ్_2.cmp (a); }

  తార్కిక subset (hb_subset_layout_context_t *c, స్థిరరాశి శూన్య *పునాది, స్థిరరాశి శూన్య *f_sub = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->subset_context->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (!f_sub)
      return_trace (బహిర్గతం->స్థానభ్రంశం.serialize_subset (c->subset_context, స్థానభ్రంశం, పునాది, c, &ట్యాగ్_2));

    స్థిరరాశి Feature& f = *reinterpret_cast<స్థిరరాశి Feature *> (f_sub);
    స్వయంచాలక *s = c->subset_context->serializer;
    s->push ();

    బహిర్గతం->స్థానభ్రంశం = 0;
    తార్కిక ret = f.subset (c->subset_context, c, &ట్యాగ్_2);
    ఒకవేళ (ret)
      s->కూడు_link (బహిర్గతం->స్థానభ్రంశం, s->తీయి_pack ());
    లేకపోతే
      s->తీయి_discard ();

    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    స్థిరరాశి రికార్డు_sanitize_సంవృతం_t సంవృతం = {ట్యాగ్_2, పునాది};
    return_trace (c->check_struct (ఇది) &&
		  స్థానభ్రంశం.sanitize (c, పునాది, &సంవృతం));
  }

  ట్యాగ్           ట్యాగ్_2;            /* 4-byte Tag identifier */
  Offset16_గమ్యం<Type>
                స్థానభ్రంశం;         /* Offset from beginning of object holding
                                 * the Record */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

మూస <రకం_పేరు Type>
నిర్మాణం రికార్డు_Array_యొక్క : Sorted_Array16_యొక్క<రికార్డు_2<Type>>
{
  స్థిరరాశి Offset16_గమ్యం<Type>& get_స్థానభ్రంశం (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి (*ఇది)[i].స్థానభ్రంశం; }
  Offset16_గమ్యం<Type>& get_స్థానభ్రంశం (గుర్తులేని పూర్ణాంకం i)
  { తిరిగి (*ఇది)[i].స్థానభ్రంశం; }
  స్థిరరాశి ట్యాగ్& get_ట్యాగ్ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి (*ఇది)[i].ట్యాగ్_2; }
  గుర్తులేని పూర్ణాంకం get_ట్యాగులు (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
                         గుర్తులేని పూర్ణాంకం *రికార్డు_లెక్క /* IN/OUT */,
                         hb_ట్యాగ్_t     *రికార్డు_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (రికార్డు_లెక్క && రికార్డు_ట్యాగులు)
    {
      + ఇది->as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, రికార్డు_లెక్క)
      | hb_map (&రికార్డు_2<Type>::ట్యాగ్_2)
      | hb_sink (hb_array (రికార్డు_ట్యాగులు, *రికార్డు_లెక్క))
      ;
    }
    తిరిగి ఇది->len;
  }
  తార్కిక కనుగొను_సూచీ (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని పూర్ణాంకం *సూచీ_2) స్థిరరాశి
  {
    తిరిగి ఇది->bfind (ట్యాగ్_2, సూచీ_2, HB_NOT_దొరికింది_STORE, సూచీ::NOT_దొరికింది_సూచీ);
  }
};

మూస <రకం_పేరు Type>
నిర్మాణం రికార్డు_List_యొక్క : రికార్డు_Array_యొక్క<Type>
{
  స్థిరరాశి Type& క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి ఇది+ఇది->get_స్థానభ్రంశం (i); }

  తార్కిక subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    + ఇది->iter ()
    | hb_apply (subset_రికార్డు_array (l, బహిర్గతం, ఇది))
    ;
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (రికార్డు_Array_యొక్క<Type>::sanitize (c, ఇది));
  }
};

నిర్మాణం రికార్డు_List_యొక్క_Feature : రికార్డు_List_యొక్క<Feature>
{
  తార్కిక subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    + hb_enumerate (*ఇది)
    | hb_filter (l->feature_సూచీ_map, hb_మొదటి)
    | hb_apply ([l, బహిర్గతం, ఇది] (స్థిరరాశి hb_pair_t<గుర్తులేని, స్థిరరాశి రికార్డు_2<Feature>&>& _)
                {
                  స్థిరరాశి Feature *f_sub = శూన్యసూచిక;
                  స్థిరరాశి Feature **f = శూన్యసూచిక;
                  ఒకవేళ (l->feature_substitutes_map->has (_.మొదటి, &f))
                    f_sub = *f;

                  l->cur_feature_సూచీ = _.మొదటి;
                  subset_రికార్డు_array (l, బహిర్గతం, ఇది, f_sub) (_.second);
                  l->cur_feature_సూచీ = సూచీ::NOT_దొరికింది_సూచీ;
                })
    ;

    return_trace (నిజం);
  }
};

రకనిర్వచనం రికార్డు_List_యొక్క<Feature> FeatureList;


నిర్మాణం LangSys
{
  గుర్తులేని పూర్ణాంకం get_feature_లెక్క () స్థిరరాశి
  { తిరిగి feature_సూచీ_2.len; }
  hb_ట్యాగ్_t get_feature_సూచీ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి feature_సూచీ_2[i]; }
  గుర్తులేని పూర్ణాంకం get_feature_indexes (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				    గుర్తులేని పూర్ణాంకం *feature_లెక్క_2 /* IN/OUT */,
				    గుర్తులేని పూర్ణాంకం *feature_indexes /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి feature_సూచీ_2.get_indexes (ప్రారంభం_స్థానభ్రంశం, feature_లెక్క_2, feature_indexes); }
  శూన్య కూడు_feature_indexes_గమ్యం (hb_సమితి_t *feature_indexes) స్థిరరాశి
  { feature_సూచీ_2.కూడు_indexes_గమ్యం (feature_indexes); }

  తార్కిక has_required_feature () స్థిరరాశి { తిరిగి req_Feature_సూచీ != 0xFFFFu; }
  గుర్తులేని పూర్ణాంకం get_required_feature_సూచీ () స్థిరరాశి
  {
    ఒకవేళ (req_Feature_సూచీ == 0xFFFFu)
      తిరిగి సూచీ::NOT_దొరికింది_సూచీ;
   తిరిగి req_Feature_సూచీ;
  }

  LangSys* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed (*ఇది));
  }

  తార్కిక compare (స్థిరరాశి LangSys& o, స్థిరరాశి hb_map_t *feature_సూచీ_map) స్థిరరాశి
  {
    ఒకవేళ (req_Feature_సూచీ != o.req_Feature_సూచీ)
      తిరిగి అబద్ధం;

    స్వయంచాలక iter =
    + hb_iter (feature_సూచీ_2)
    | hb_filter (feature_సూచీ_map)
    | hb_map (feature_సూచీ_map)
    ;

    స్వయంచాలక o_iter =
    + hb_iter (o.feature_సూచీ_2)
    | hb_filter (feature_సూచీ_map)
    | hb_map (feature_సూచీ_map)
    ;

    కొరకు (; iter && o_iter; iter++, o_iter++)
    {
      గుర్తులేని a = *iter;
      గుర్తులేని b = *o_iter;
      ఒకవేళ (a != b) తిరిగి అబద్ధం;
    }

    ఒకవేళ (iter || o_iter) తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  శూన్య collect_features (hb_prune_langsys_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!has_required_feature () && !get_feature_లెక్క ()) తిరిగి;
    ఒకవేళ (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_సూచీ))
      c->new_feature_indexes->కూడు (get_required_feature_సూచీ ());

    + hb_iter (feature_సూచీ_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  తార్కిక subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       స్థిరరాశి ట్యాగ్                  *ట్యాగ్_2 = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    స్థిరరాశి uint32_t *v;
    బహిర్గతం->req_Feature_సూచీ = l->feature_map_w_duplicates->has (req_Feature_సూచీ, &v) ? *v : 0xFFFFu;

    ఒకవేళ (!l->సందర్శించు_Feature_సూచీ (feature_సూచీ_2.len))
      return_trace (అబద్ధం);

    స్వయంచాలక it =
    + hb_iter (feature_సూచీ_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    తార్కిక ret = తార్కిక (it);
    బహిర్గతం->feature_సూచీ_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి రికార్డు_sanitize_సంవృతం_t * = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && feature_సూచీ_2.sanitize (c));
  }

  Offset16	lookup_క్రమం_Z;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	req_Feature_సూచీ;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  సూచీ_Array	feature_సూచీ_2;	/* Array of indices into the FeatureList */
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (6, feature_సూచీ_2);
};
DECLARE_NULL_NAMESPACE_బైట్లు (OT, LangSys);

నిర్మాణం Script
{
  గుర్తులేని పూర్ణాంకం get_lang_sys_లెక్క () స్థిరరాశి
  { తిరిగి langSys.len; }
  స్థిరరాశి ట్యాగ్& get_lang_sys_ట్యాగ్ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి langSys.get_ట్యాగ్ (i); }
  గుర్తులేని పూర్ణాంకం get_lang_sys_ట్యాగులు (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం *lang_sys_లెక్క /* IN/OUT */,
				  hb_ట్యాగ్_t     *lang_sys_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి langSys.get_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, lang_sys_లెక్క, lang_sys_ట్యాగులు); }
  స్థిరరాశి LangSys& get_lang_sys (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
    ఒకవేళ (i == సూచీ::NOT_దొరికింది_సూచీ) తిరిగి get_default_lang_sys ();
    తిరిగి ఇది+langSys[i].స్థానభ్రంశం;
  }
  తార్కిక కనుగొను_lang_sys_సూచీ (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని పూర్ణాంకం *సూచీ_2) స్థిరరాశి
  { తిరిగి langSys.కనుగొను_సూచీ (ట్యాగ్_2, సూచీ_2); }

  తార్కిక has_default_lang_sys () స్థిరరాశి           { తిరిగి defaultLangSys != 0; }
  స్థిరరాశి LangSys& get_default_lang_sys () స్థిరరాశి { తిరిగి ఇది+defaultLangSys; }

  శూన్య prune_langsys (hb_prune_langsys_context_t *c,
                      గుర్తులేని script_సూచీ) స్థిరరాశి
  {
    ఒకవేళ (!has_default_lang_sys () && !get_lang_sys_లెక్క ()) తిరిగి;
    ఒకవేళ (!c->సందర్శించు_Script ()) తిరిగి;

    ఒకవేళ (!c->script_langsys_map->has (script_సూచీ))
    {
      ఒకవేళ (unlikely (!c->script_langsys_map->సమితి (script_సూచీ, hb::unique_ptr<hb_సమితి_t> {hb_సమితి_create ()})))
	తిరిగి;
    }

    ఒకవేళ (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      స్థిరరాశి LangSys& d = get_default_lang_sys ();
      ఒకవేళ (c->సందర్శించు_Langsys (d.get_feature_లెక్క ())) {
        d.collect_features (c);
      }

      కొరకు (స్వయంచాలక _ : + hb_enumerate (langSys))
      {
        స్థిరరాశి LangSys& l = ఇది+_.second.స్థానభ్రంశం;
        ఒకవేళ (!c->సందర్శించు_Langsys (l.get_feature_లెక్క ())) కొనసాగు;
        ఒకవేళ (l.compare (d, c->duplicate_feature_map)) కొనసాగు;

        l.collect_features (c);
        c->script_langsys_map->get (script_సూచీ)->కూడు (_.మొదటి);
      }
    }
    లేకపోతే
    {
      కొరకు (స్వయంచాలక _ : + hb_enumerate (langSys))
      {
        స్థిరరాశి LangSys& l = ఇది+_.second.స్థానభ్రంశం;
        ఒకవేళ (!c->సందర్శించు_Langsys (l.get_feature_లెక్క ())) కొనసాగు;
        l.collect_features (c);
        c->script_langsys_map->get (script_సూచీ)->కూడు (_.మొదటి);
      }
    }
  }

  తార్కిక subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       స్థిరరాశి ట్యాగ్                   *ట్యాగ్_2) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (!l->సందర్శించు_Script ()) return_trace (అబద్ధం);
    ఒకవేళ (ట్యాగ్_2 && !c->plan->layout_scripts.has (*ట్యాగ్_2))
      తిరిగి అబద్ధం;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    తార్కిక defaultLang = అబద్ధం;
    ఒకవేళ (has_default_lang_sys ())
    {
      c->serializer->push ();
      స్థిరరాశి LangSys& ls = ఇది+defaultLangSys;
      తార్కిక ret = ls.subset (c, l);
      ఒకవేళ (!ret && ట్యాగ్_2 && *ట్యాగ్_2 != HB_ట్యాగ్ ('D', 'F', 'L', 'T'))
      {
	c->serializer->తీయి_discard ();
	బహిర్గతం->defaultLangSys = 0;
      }
      లేకపోతే
      {
	c->serializer->కూడు_link (బహిర్గతం->defaultLangSys, c->serializer->తీయి_pack ());
	defaultLang = నిజం;
      }
    }

    స్థిరరాశి hb_సమితి_t *చురుకైన_langsys = l->script_langsys_map->get (l->cur_script_సూచీ);
    ఒకవేళ (చురుకైన_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (చురుకైన_langsys, hb_మొదటి)
      | hb_map (hb_రెండవ)
      | hb_filter ([=] (స్థిరరాశి రికార్డు_2<LangSys>& రికార్డు) {తిరిగి l->సందర్శించు_Lang_Sys (); })
      | hb_apply (subset_రికార్డు_array (l, &(బహిర్గతం->langSys), ఇది))
      ;
    }

    return_trace (తార్కిక (బహిర్గతం->langSys.len) || defaultLang || l->పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి రికార్డు_sanitize_సంవృతం_t * = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (defaultLangSys.sanitize (c, ఇది) && langSys.sanitize (c, ఇది));
  }

  రక్షిత:
  Offset16_గమ్యం<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  రికార్డు_Array_యొక్క<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (4, langSys);
};

నిర్మాణం రికార్డు_List_యొక్క_Script : రికార్డు_List_యొక్క<Script>
{
  తార్కిక subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    కొరకు (స్వయంచాలక _ : + hb_enumerate (*ఇది))
    {
      స్వయంచాలక snap = c->serializer->snapshot ();
      l->cur_script_సూచీ = _.మొదటి;
      తార్కిక ret = _.second.subset (l, ఇది);
      ఒకవేళ (!ret) c->serializer->revert (snap);
      లేకపోతే బహిర్గతం->len++;
    }

    return_trace (నిజం);
  }
};

రకనిర్వచనం రికార్డు_List_యొక్క_Script ScriptList;



నిర్మాణం LookupFlag : HBUINT16
{
  గణన Flags {
    కుడి_గమ్యం_ఎడమ		= 0x0001u,
    Ignore_పునాది_Glyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    Use_Mark_Filtering_సమితి	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  బహిరంగ:
  DEFINE_కొలత_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
నామాకాశ OT {

నిర్మాణం Lookup
{
  గుర్తులేని పూర్ణాంకం get_subtable_లెక్క () స్థిరరాశి { తిరిగి sub_పట్టిక.len; }

  మూస <రకం_పేరు TSub_పట్టిక>
  స్థిరరాశి Array16_యొక్క_Offset16_గమ్యం<TSub_పట్టిక>& get_subtables () స్థిరరాశి
  { తిరిగి reinterpret_cast<స్థిరరాశి Array16_యొక్క_Offset16_గమ్యం<TSub_పట్టిక> &> (sub_పట్టిక); }
  మూస <రకం_పేరు TSub_పట్టిక>
  Array16_యొక్క_Offset16_గమ్యం<TSub_పట్టిక>& get_subtables ()
  { తిరిగి reinterpret_cast<Array16_యొక్క_Offset16_గమ్యం<TSub_పట్టిక> &> (sub_పట్టిక); }

  మూస <రకం_పేరు TSub_పట్టిక>
  స్థిరరాశి TSub_పట్టిక& get_subtable (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి ఇది+get_subtables<TSub_పట్టిక> ()[i]; }
  మూస <రకం_పేరు TSub_పట్టిక>
  TSub_పట్టిక& get_subtable (గుర్తులేని పూర్ణాంకం i)
  { తిరిగి ఇది+get_subtables<TSub_పట్టిక> ()[i]; }

  size_t get_కొలత () స్థిరరాశి
  {
    స్థిరరాశి HBUINT16 &mark_Filtering_సమితి = StructAfter<స్థిరరాశి HBUINT16> (sub_పట్టిక);
    ఒకవేళ (lookupFlag & LookupFlag::Use_Mark_Filtering_సమితి)
      తిరిగి (స్థిరరాశి అక్షరం *) &StructAfter<స్థిరరాశి అక్షరం> (mark_Filtering_సమితి) - (స్థిరరాశి అక్షరం *) ఇది;
    తిరిగి (స్థిరరాశి అక్షరం *) &mark_Filtering_సమితి - (స్థిరరాశి అక్షరం *) ఇది;
  }

  గుర్తులేని పూర్ణాంకం get_type () స్థిరరాశి { తిరిగి lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం flag = lookupFlag;
    ఒకవేళ (unlikely (flag & LookupFlag::Use_Mark_Filtering_సమితి))
    {
      స్థిరరాశి HBUINT16 &mark_Filtering_సమితి = StructAfter<HBUINT16> (sub_పట్టిక);
      flag += (mark_Filtering_సమితి << 16);
    }
    తిరిగి flag;
  }

  మూస <రకం_పేరు TSub_పట్టిక, రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం lookup_type = get_type ();
    TRACE_DISPATCH (ఇది, lookup_type);
    గుర్తులేని పూర్ణాంకం లెక్క = get_subtable_లెక్క ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++) {
      రకం_పేరు context_t::return_t r = get_subtable<TSub_పట్టిక> (i).dispatch (c, lookup_type, ds...);
      ఒకవేళ (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_విలువ ());
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  గుర్తులేని పూర్ణాంకం lookup_type,
		  uint32_t lookup_props,
		  గుర్తులేని పూర్ణాంకం అంకె_subtables)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    ఒకవేళ (unlikely (!sub_పట్టిక.serialize (c, అంకె_subtables))) return_trace (అబద్ధం);
    ఒకవేళ (lookupFlag & LookupFlag::Use_Mark_Filtering_సమితి)
    {
      ఒకవేళ (unlikely (!c->extend (ఇది))) return_trace (అబద్ధం);
      HBUINT16 &mark_Filtering_సమితి = StructAfter<HBUINT16> (sub_పట్టిక);
      mark_Filtering_సమితి = lookup_props >> 16;
    }
    return_trace (నిజం);
  }

  మూస <రకం_పేరు TSub_పట్టిక>
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->lookupType = lookupType;
    బహిర్గతం->lookupFlag = lookupFlag;

    స్థిరరాశి hb_సమితి_t *glyphset = c->plan->glyphset_gsub ();
    గుర్తులేని పూర్ణాంకం lookup_type = get_type ();
    + hb_iter (get_subtables <TSub_పట్టిక> ())
    | hb_filter ([ఇది, glyphset, lookup_type] (స్థిరరాశి Offset16_గమ్యం<TSub_పట్టిక> &_) { తిరిగి (ఇది+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_స్థానభ్రంశం_array (c, బహిర్గతం->get_subtables<TSub_పట్టిక> (), ఇది, lookup_type))
    ;

    ఒకవేళ (lookupFlag & LookupFlag::Use_Mark_Filtering_సమితి)
    {
      స్థిరరాశి HBUINT16 &mark_Filtering_సమితి = StructAfter<HBUINT16> (sub_పట్టిక);
      hb_codepoint_t *idx;
      ఒకవేళ (!c->plan->వాడిన_mark_sets_map.has (mark_Filtering_సమితి, &idx))
      {
        గుర్తులేని new_flag = lookupFlag;
        new_flag &= ~LookupFlag::Use_Mark_Filtering_సమితి;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        బహిర్గతం->lookupFlag = new_flag;
      }
      లేకపోతే
      {
        ఒకవేళ (unlikely (!c->serializer->extend (బహిర్గతం))) return_trace (అబద్ధం);
        HBUINT16 &బహిర్గతం_Mark_Filtering_సమితి = StructAfter<HBUINT16> (బహిర్గతం->sub_పట్టిక);
        బహిర్గతం_Mark_Filtering_సమితి = *idx;
      }
    }

    // Always keep the lookup even if it's empty. The rest of layout subsetting depends on lookup
    // indices being consistent with those computed during planning. So if an empty lookup is
    // discarded during the subset phase it will invalidate all subsequent lookup indices.
    // Generally we shouldn't end up with an empty lookup as we pre-prune them during the planning
    // phase, but it can happen in rare cases such as when during closure subtable is considered
    // degenerate (see: https://github.com/harfbuzz/harfbuzz/issues/3853)
    return_trace (నిజం);
  }

  మూస <రకం_పేరు TSub_పట్టిక>
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(c->check_struct (ఇది) && sub_పట్టిక.sanitize (c))) return_trace (అబద్ధం);
    hb_barrier ();

    గుర్తులేని subtables = get_subtable_లెక్క ();
    ఒకవేళ (unlikely (!c->సందర్శించు_subtables (subtables))) return_trace (అబద్ధం);

    ఒకవేళ (lookupFlag & LookupFlag::Use_Mark_Filtering_సమితి)
    {
      స్థిరరాశి HBUINT16 &mark_Filtering_సమితి = StructAfter<HBUINT16> (sub_పట్టిక);
      ఒకవేళ (!mark_Filtering_సమితి.sanitize (c)) return_trace (అబద్ధం);
    }

    ఒకవేళ (unlikely (!get_subtables<TSub_పట్టిక> ().sanitize (c, ఇది, get_type ())))
      return_trace (అబద్ధం);

    ఒకవేళ (unlikely (get_type () == TSub_పట్టిక::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      గుర్తులేని పూర్ణాంకం type = get_subtable<TSub_పట్టిక> (0).u.extension.get_type ();
      కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < subtables; i++)
	ఒకవేళ (get_subtable<TSub_పట్టిక> (i).u.extension.get_type () != type)
	  return_trace (అబద్ధం);
    }
    return_trace (నిజం);
  }

  రక్షిత:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16_యొక్క<Offset16>
		sub_పట్టిక;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, sub_పట్టిక);
};

మూస <రకం_పేరు Types>
ఉపయోగించి LookupList = List16_యొక్క_స్థానభ్రంశం_గమ్యం<Lookup, రకం_పేరు Types::HBLUINT>;

మూస <రకం_పేరు TLookup, రకం_పేరు స్థానభ్రంశం_Type>
నిర్మాణం Lookup_స్థానభ్రంశం_List : List16_యొక్క_స్థానభ్రంశం_గమ్యం<TLookup, స్థానభ్రంశం_Type>
{
  తార్కిక subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    + hb_enumerate (*ఇది)
    | hb_filter (l->lookup_సూచీ_map, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_apply (subset_స్థానభ్రంశం_array (c, *బహిర్గతం, ఇది))
    ;
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace ((List16_యొక్క_స్థానభ్రంశం_గమ్యం<TLookup, స్థానభ్రంశం_Type>::sanitize (c, ఇది)));
  }
};


/*
 * Coverage Table
 */


స్థిర తార్కిక ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  స్థిరరాశి hb_సమితి_t &klasses,
                                          తార్కిక use_class_zero,
                                          hb_sorted_సదిశ_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  ఒకవేళ (!klass_map)
    తిరిగి ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  ఒకవేళ (!use_class_zero)
    klass_map->సమితి (0, 0);

  గుర్తులేని idx = klass_map->has (0) ? 1 : 0;
  కొరకు (స్థిరరాశి గుర్తులేని k: klasses)
  {
    ఒకవేళ (klass_map->has (k)) కొనసాగు;
    klass_map->సమితి (k, idx);
    idx++;
  }


  ఒకవేళ (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    తిరిగి అబద్ధం;

  కొరకు (గుర్తులేని i = 0; i < glyph_and_klass.నిడివి; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  తిరిగి ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

మూస <రకం_పేరు Types>
నిర్మాణం ClassDefFormat1_3
{
  మిత్ర నిర్మాణం ClassDef;

  అంతరంగ:
  గుర్తులేని పూర్ణాంకం get_class (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    తిరిగి class_విలువ[(గుర్తులేని పూర్ణాంకం) (glyph_id - ప్రారంభం_Glyph)];
  }

  గుర్తులేని get_population () స్థిరరాశి
  {
    తిరిగి class_విలువ.len;
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!it))
    {
      classFormat = Types::కొలత == 2 ? 1 : 3;
      ప్రారంభం_Glyph = 0;
      class_విలువ.len = 0;
      return_trace (నిజం);
    }

    hb_codepoint_t glyph_min = (*it).మొదటి;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_మొదటి)
			       | hb_reduce (hb_max, 0u);
    గుర్తులేని glyph_లెక్క = glyph_max - glyph_min + 1;

    ప్రారంభం_Glyph = glyph_min;
    ఒకవేళ (unlikely (!class_విలువ.serialize (c, glyph_లెక్క))) return_trace (అబద్ధం);
    కొరకు (స్థిరరాశి hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      గుర్తులేని idx = gid_klass_pair.మొదటి - glyph_min;
      class_విలువ[idx] = gid_klass_pair.second;
    }
    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = శూన్యసూచిక /*OUT*/,
               తార్కిక keep_ఖాళీలు_పట్టిక = నిజం,
               తార్కిక use_class_zero = నిజం,
               స్థిరరాశి Coverage* glyph_filter = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    hb_sorted_సదిశ_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_సమితి_t orig_klasses;

    hb_codepoint_t ప్రారంభం = ప్రారంభం_Glyph;
    hb_codepoint_t చివర   = ప్రారంభం + class_విలువ.len;

    కొరకు (స్థిరరాశి hb_codepoint_t gid : + hb_range (ప్రారంభం, చివర))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      ఒకవేళ (new_gid == HB_MAP_విలువ_INVALID) కొనసాగు;
      ఒకవేళ (glyph_filter && !glyph_filter->has(gid)) కొనసాగు;

      గుర్తులేని klass = class_విలువ[gid - ప్రారంభం];
      ఒకవేళ (!klass) కొనసాగు;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.కూడు (klass);
    }

    ఒకవేళ (use_class_zero)
    {
      స్థిరరాశి hb_సమితి_t& glyphset = *c->plan->glyphset_gsub ();
      గుర్తులేని glyph_లెక్క = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_లెక్క <= glyph_and_klass.నిడివి;
    }
    ఒకవేళ (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (అబద్ధం);
    return_trace (keep_ఖాళీలు_పట్టిక || (తార్కిక) glyph_and_klass);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && class_విలువ.sanitize (c));
  }

  గుర్తులేని ఖర్చు () స్థిరరాశి { తిరిగి 1; }

  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం ప్రారంభం = 0;
    గుర్తులేని పూర్ణాంకం లెక్క = class_విలువ.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (class_విలువ[i])
	కొనసాగు;

      ఒకవేళ (ప్రారంభం != i)
	ఒకవేళ (unlikely (!glyphs->కూడు_range (ప్రారంభం_Glyph + ప్రారంభం, ప్రారంభం_Glyph + i)))
	  తిరిగి అబద్ధం;

      ప్రారంభం = i + 1;
    }
    ఒకవేళ (ప్రారంభం != లెక్క)
      ఒకవేళ (unlikely (!glyphs->కూడు_range (ప్రారంభం_Glyph + ప్రారంభం, ప్రారంభం_Glyph + లెక్క)))
	తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_class (సమితి_t *glyphs, గుర్తులేని klass) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = class_విలువ.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (class_విలువ[i] == klass) glyphs->కూడు (ప్రారంభం_Glyph + i);
    తిరిగి నిజం;
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    hb_codepoint_t ప్రారంభం = ప్రారంభం_Glyph;
    hb_codepoint_t చివర = ప్రారంభం_Glyph + class_విలువ.len;
    కొరకు (hb_codepoint_t iter = ప్రారంభం_Glyph - 1;
	 glyphs->next (&iter) && iter < చివర;)
      ఒకవేళ (class_విలువ[iter - ప్రారంభం]) తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }
  తార్కిక intersects_class (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని klass) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = class_విలువ.len;
    ఒకవేళ (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_సమితి_విలువ_INVALID;
      ఒకవేళ (!glyphs->next (&g)) తిరిగి అబద్ధం;
      ఒకవేళ (g < ప్రారంభం_Glyph) తిరిగి నిజం;
      g = ప్రారంభం_Glyph + లెక్క - 1;
      ఒకవేళ (glyphs->next (&g)) తిరిగి నిజం;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    స్థిరరాశి రకం_పేరు Types::HBUINT *arr = class_విలువ.arrayZ;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (arr[i] == klass && glyphs->has (ప్రారంభం_Glyph + i))
	తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }

  శూన్య intersected_class_glyphs (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని klass, hb_సమితి_t *intersect_glyphs) స్థిరరాశి
  {
    గుర్తులేని లెక్క = class_విలువ.len;
    ఒకవేళ (klass == 0)
    {
      గుర్తులేని ప్రారంభం_glyph = ప్రారంభం_Glyph;
      కొరకు (uint32_t g = HB_సమితి_విలువ_INVALID;
	   glyphs->next (&g) && g < ప్రారంభం_glyph;)
	intersect_glyphs->కూడు (g);

      కొరకు (uint32_t g = ప్రారంభం_Glyph + లెక్క - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->కూడు (g);

      తిరిగి;
    }

    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      ఒకవేళ (class_విలువ[i] == klass && glyphs->has (ప్రారంభం_Glyph + i))
	intersect_glyphs->కూడు (ప్రారంభం_Glyph + i);

#ఒకవేళ 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    గుర్తులేని ప్రారంభం_glyph = ప్రారంభం_Glyph;
    గుర్తులేని చివర_glyph = ప్రారంభం_glyph + లెక్క;
    కొరకు (గుర్తులేని g = ప్రారంభం_Glyph - 1;
	 glyphs->next (&g) && g < చివర_glyph;)
      ఒకవేళ (class_విలువ.arrayZ[g - ప్రారంభం_glyph] == klass)
        intersect_glyphs->కూడు (g);
#అయితే_ముగింపు
  }

  శూన్య intersected_classes (స్థిరరాశి hb_సమితి_t *glyphs, hb_సమితి_t *intersect_classes) స్థిరరాశి
  {
    ఒకవేళ (glyphs->is_empty ()) తిరిగి;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    ఒకవేళ (!class_విలువ.len)
    {
      intersect_classes->కూడు (0);
      తిరిగి;
    }

    hb_codepoint_t చివర_glyph = ప్రారంభం_Glyph + class_విలువ.len - 1;
    ఒకవేళ (glyphs->get_min () < ప్రారంభం_Glyph ||
        glyphs->get_max () > చివర_glyph)
      intersect_classes->కూడు (0);

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : + hb_enumerate (class_విలువ))
    {
      hb_codepoint_t g = ప్రారంభం_Glyph + _.మొదటి;
      ఒకవేళ (glyphs->has (g))
        intersect_classes->కూడు (_.second);
    }
  }

  రక్షిత:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  రకం_పేరు Types::HBGlyphID
		 ప్రారంభం_Glyph;	/* First GlyphID of the classValueArray */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::HBUINT>
		class_విలువ;	/* Array of Class Values--one per GlyphID */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + 2 * Types::కొలత, class_విలువ);
};

మూస <రకం_పేరు Types>
నిర్మాణం ClassDefFormat2_4
{
  మిత్ర నిర్మాణం ClassDef;

  అంతరంగ:
  గుర్తులేని పూర్ణాంకం get_class (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    తిరిగి range_రికార్డు.bsearch (glyph_id).value;
  }

  గుర్తులేని get_population () స్థిరరాశి
  {
    రకం_పేరు Types::large_int ret = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక &r : range_రికార్డు)
      ret += r.get_population ();
    తిరిగి ret > UINT_MAX ? UINT_MAX : (గుర్తులేని) ret;
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!it))
    {
      classFormat = Types::కొలత == 2 ? 2 : 4;
      range_రికార్డు.len = 0;
      return_trace (నిజం);
    }

    గుర్తులేని unsorted = అబద్ధం;
    గుర్తులేని అంకె_ranges = 1;
    hb_codepoint_t prev_gid = (*it).మొదటి;
    గుర్తులేని prev_klass = (*it).second;

    Range_రికార్డు<Types> range_rec;
    range_rec.మొదటి = prev_gid;
    range_rec.చివరి = prev_gid;
    range_rec.value = prev_klass;

    స్వయంచాలక *రికార్డు = c->copy (range_rec);
    ఒకవేళ (unlikely (!రికార్డు)) return_trace (అబద్ధం);

    కొరకు (స్థిరరాశి స్వయంచాలక gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.మొదటి;
      గుర్తులేని cur_klass = gid_klass_pair.second;

      ఒకవేళ (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	ఒకవేళ (unlikely (cur_gid < prev_gid))
	  unsorted = నిజం;

	ఒకవేళ (unlikely (!రికార్డు)) ఆపు;
	రికార్డు->చివరి = prev_gid;
	అంకె_ranges++;

	range_rec.మొదటి = cur_gid;
	range_rec.చివరి = cur_gid;
	range_rec.value = cur_klass;

	రికార్డు = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    ఒకవేళ (unlikely (c->లోపల_error ())) return_trace (అబద్ధం);

    ఒకవేళ (likely (రికార్డు)) రికార్డు->చివరి = prev_gid;
    range_రికార్డు.len = అంకె_ranges;

    ఒకవేళ (unlikely (unsorted))
      range_రికార్డు.as_array ().qsort (Range_రికార్డు<Types>::cmp_range);

    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = శూన్యసూచిక /*OUT*/,
               తార్కిక keep_ఖాళీలు_పట్టిక = నిజం,
               తార్కిక use_class_zero = నిజం,
               స్థిరరాశి Coverage* glyph_filter = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyph_సమితి = *c->plan->glyphset_gsub ();

    hb_sorted_సదిశ_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_సమితి_t orig_klasses;

    ఒకవేళ (glyph_సమితి.get_population () * hb_బిట్టు_storage ((గుర్తులేని) range_రికార్డు.len)
	< get_population ())
    {
      కొరకు (hb_codepoint_t g : glyph_సమితి)
      {
	గుర్తులేని klass = get_class (g);
	ఒకవేళ (!klass) కొనసాగు;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	ఒకవేళ (new_gid == HB_MAP_విలువ_INVALID) కొనసాగు;
	ఒకవేళ (glyph_filter && !glyph_filter->has (g)) కొనసాగు;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.కూడు (klass);
      }
    }
    లేకపోతే
    {
      గుర్తులేని అంకె_ఆధారం_glyphs = c->plan->ఆధారం->get_అంకె_glyphs ();
      కొరకు (స్వయంచాలక &range : range_రికార్డు)
      {
	గుర్తులేని klass = range.value;
	ఒకవేళ (!klass) కొనసాగు;
	hb_codepoint_t ప్రారంభం = range.మొదటి;
	hb_codepoint_t చివర   = hb_min (range.చివరి + 1, అంకె_ఆధారం_glyphs);
	కొరకు (hb_codepoint_t g = ప్రారంభం; g < చివర; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  ఒకవేళ (new_gid == HB_MAP_విలువ_INVALID) కొనసాగు;
	  ఒకవేళ (glyph_filter && !glyph_filter->has (g)) కొనసాగు;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.కూడు (klass);
	}
      }
    }

    స్థిరరాశి hb_సమితి_t& glyphset = *c->plan->glyphset_gsub ();
    గుర్తులేని glyph_లెక్క = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_లెక్క <= glyph_and_klass.నిడివి;
    ఒకవేళ (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (అబద్ధం);
    return_trace (keep_ఖాళీలు_పట్టిక || (తార్కిక) glyph_and_klass);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (range_రికార్డు.sanitize (c));
  }

  గుర్తులేని ఖర్చు () స్థిరరాశి { తిరిగి hb_బిట్టు_storage ((గుర్తులేని) range_రికార్డు.len); /* bsearch cost */ }

  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  {
    కొరకు (స్వయంచాలక &range : range_రికార్డు)
      ఒకవేళ (range.value)
	ఒకవేళ (unlikely (!range.collect_coverage (glyphs)))
	  తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_class (సమితి_t *glyphs, గుర్తులేని పూర్ణాంకం klass) స్థిరరాశి
  {
    కొరకు (స్వయంచాలక &range : range_రికార్డు)
    {
      ఒకవేళ (range.value == klass)
	ఒకవేళ (unlikely (!range.collect_coverage (glyphs)))
	  తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఒకవేళ (range_రికార్డు.len > glyphs->get_population () * hb_బిట్టు_storage ((గుర్తులేని) range_రికార్డు.len))
    {
      కొరకు (స్వయంచాలక g : *glyphs)
        ఒకవేళ (get_class (g))
	  తిరిగి నిజం;
      తిరిగి అబద్ధం;
    }

    తిరిగి hb_any (+ hb_iter (range_రికార్డు)
                   | hb_map ([glyphs] (స్థిరరాశి Range_రికార్డు<Types> &range) { తిరిగి range.intersects (*glyphs) && range.value; }));
  }
  తార్కిక intersects_class (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని klass) స్థిరరాశి
  {
    ఒకవేళ (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      ఒకవేళ (!range_రికార్డు.len)
        తిరిగి !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_సమితి_విలువ_INVALID;
      hb_codepoint_t చివరి = HB_సమితి_విలువ_INVALID;
      స్వయంచాలక it = hb_iter (range_రికార్డు);
      కొరకు (స్వయంచాలక &range : it)
      {
        ఒకవేళ (it->మొదటి == చివరి + 1)
	{
	  it++;
	  కొనసాగు;
	}

	ఒకవేళ (!glyphs->next (&g))
	  ఆపు;
	ఒకవేళ (g < range.మొదటి)
	  తిరిగి నిజం;
	g = range.చివరి;
	చివరి = g;
      }
      ఒకవేళ (g != HB_సమితి_విలువ_INVALID && glyphs->next (&g))
	తిరిగి నిజం;
      /* Fall through. */
    }
    కొరకు (స్థిరరాశి స్వయంచాలక &range : range_రికార్డు)
      ఒకవేళ (range.value == klass && range.intersects (*glyphs))
	తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }

  శూన్య intersected_class_glyphs (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని klass, hb_సమితి_t *intersect_glyphs) స్థిరరాశి
  {
    ఒకవేళ (klass == 0)
    {
      hb_codepoint_t g = HB_సమితి_విలువ_INVALID;
      కొరకు (స్వయంచాలక &range : range_రికార్డు)
      {
	ఒకవేళ (!glyphs->next (&g))
	  వెళ్ళు పూర్తి;
	అయితే (g < range.మొదటి)
	{
	  intersect_glyphs->కూడు (g);
	  ఒకవేళ (!glyphs->next (&g))
	    వెళ్ళు పూర్తి;
        }
        g = range.చివరి;
      }
      అయితే (glyphs->next (&g))
	intersect_glyphs->కూడు (g);
      పూర్తి:

      తిరిగి;
    }

    గుర్తులేని లెక్క = range_రికార్డు.len;
    ఒకవేళ (లెక్క > glyphs->get_population () * hb_బిట్టు_storage (లెక్క))
    {
      కొరకు (స్వయంచాలక g : *glyphs)
      {
        గుర్తులేని i;
        ఒకవేళ (range_రికార్డు.as_array ().bfind (g, &i) &&
	    range_రికార్డు.arrayZ[i].value == klass)
	  intersect_glyphs->కూడు (g);
      }
      తిరిగి;
    }

    కొరకు (స్వయంచాలక &range : range_రికార్డు)
    {
      ఒకవేళ (range.value != klass) కొనసాగు;

      గుర్తులేని చివర = range.చివరి + 1;
      కొరకు (hb_codepoint_t g = range.మొదటి - 1;
	   glyphs->next (&g) && g < చివర;)
	intersect_glyphs->కూడు (g);
    }
  }

  శూన్య intersected_classes (స్థిరరాశి hb_సమితి_t *glyphs, hb_సమితి_t *intersect_classes) స్థిరరాశి
  {
    ఒకవేళ (glyphs->is_empty ()) తిరిగి;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    ఒకవేళ (!range_రికార్డు.len)
    {
      intersect_classes->కూడు (0);
      తిరిగి;
    }

    hb_codepoint_t g = HB_సమితి_విలువ_INVALID;
    కొరకు (స్వయంచాలక &range : range_రికార్డు)
    {
      ఒకవేళ (!glyphs->next (&g))
        ఆపు;
      ఒకవేళ (g < range.మొదటి)
      {
        intersect_classes->కూడు (0);
        ఆపు;
      }
      g = range.చివరి;
    }
    ఒకవేళ (g != HB_సమితి_విలువ_INVALID && glyphs->next (&g))
      intersect_classes->కూడు (0);

    కొరకు (స్థిరరాశి స్వయంచాలక& range : range_రికార్డు)
      ఒకవేళ (range.intersects (*glyphs))
        intersect_classes->కూడు (range.value);
  }

  రక్షిత:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  రకం_పేరు Types::మూస Sorted_Array_యొక్క<Range_రికార్డు<Types>>
		range_రికార్డు;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::కొలత, range_రికార్డు);
};

నిర్మాణం ClassDef
{
  /* Has interface. */
  గుర్తులేని క్రియాకారకం [] (hb_codepoint_t k) స్థిరరాశి { తిరిగి get (k); }
  తార్కిక has (hb_codepoint_t k) స్థిరరాశి { తిరిగి (*ఇది)[k]; }
  /* Projection. */
  hb_codepoint_t క్రియాకారకం () (hb_codepoint_t k) స్థిరరాశి { తిరిగి get (k); }

  గుర్తులేని పూర్ణాంకం get (hb_codepoint_t k) స్థిరరాశి { తిరిగి get_class (k); }
  గుర్తులేని పూర్ణాంకం get_class (hb_codepoint_t glyph_id) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_class (glyph_id);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_class (glyph_id);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_class (glyph_id);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_class (glyph_id);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి 0;
    }
  }
  గుర్తులేని పూర్ణాంకం get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) స్థిరరాశి
  {
    గుర్తులేని klass;
    ఒకవేళ (cache && cache->get (glyph_id, &klass)) తిరిగి klass;
    klass = get_class (glyph_id);
    ఒకవేళ (cache) cache->సమితి (glyph_id, klass);
    తిరిగి klass;
  }

  గుర్తులేని get_population () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_population ();
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_population ();
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_population ();
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_population ();
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి NOT_COVERED;
    }
  }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    స్వయంచాలక glyphs = + it_with_class_zero | hb_filter (hb_రెండవ);

    గుర్తులేని format = 2;
    hb_codepoint_t glyph_max = 0;
    గుర్తులేని class_max = 0;
    ఒకవేళ (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).మొదటి;
      glyph_max = glyph_min;

      గుర్తులేని అంకె_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      గుర్తులేని prev_klass = (*glyphs).second;

      కొరకు (స్థిరరాశి స్వయంచాలక gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.మొదటి;
	గుర్తులేని cur_klass = gid_klass_pair.second;
	ఒకవేళ (cur_klass > class_max) class_max = cur_klass;
	ఒకవేళ (cur_gid == glyph_min) కొనసాగు;
        ఒకవేళ (cur_gid > glyph_max) glyph_max = cur_gid;
	ఒకవేళ (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  అంకె_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t ఉత్తమ_కొలత = UINT64_MAX;
      గుర్తులేని glyph_పరిధి = glyph_max - glyph_min + 1;
      స్వయంచాలక consider = [&] (గుర్తులేని candidate, uint64_t కొలత)
      {
	ఒకవేళ (కొలత >= ఉత్తమ_కొలత) తిరిగి;
	ఉత్తమ_కొలత = కొలత;
	format = candidate;
      };

      ఒకవేళ (glyph_max <= 0xFFFFu && glyph_పరిధి <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_కొలత * glyph_పరిధి);
      ఒకవేళ (glyph_max <= 0xFFFFu && అంకె_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) అంకె_ranges *
		     (2 * HBGlyphID16::static_కొలత + HBUINT16::static_కొలత));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      ఒకవేళ (glyph_max <= 0xFFFFFFu && glyph_పరిధి <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_కొలత * glyph_పరిధి);
      ఒకవేళ (glyph_max <= 0xFFFFFFu && అంకె_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) అంకె_ranges *
		     (2 * HBGlyphID24::static_కొలత + HBUINT16::static_కొలత));
#అయితే_ముగింపు
      ఒకవేళ (unlikely (ఉత్తమ_కొలత == UINT64_MAX))
      {
	c->check_success (అబద్ధం, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);
	return_trace (అబద్ధం);
      }
    }

    u.format.v = format;

    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_రెండవ)));
    స్థితి 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_రెండవ)));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_రెండవ)));
    స్థితి 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_రెండవ)));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  తార్కిక subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = శూన్యసూచిక /*OUT*/,
               తార్కిక keep_ఖాళీలు_పట్టిక = నిజం,
               తార్కిక use_class_zero = నిజం,
               స్థిరరాశి Coverage* glyph_filter = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_ఖాళీలు_పట్టిక, use_class_zero, glyph_filter));
    స్థితి 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_ఖాళీలు_పట్టిక, use_class_zero, glyph_filter));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_ఖాళీలు_పట్టిక, use_class_zero, glyph_filter));
    స్థితి 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_ఖాళీలు_పట్టిక, use_class_zero, glyph_filter));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    స్థితి 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (నిజం);
    }
  }

  గుర్తులేని ఖర్చు () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.ఖర్చు ();
    స్థితి 2: hb_barrier (); తిరిగి u.format2.ఖర్చు ();
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.ఖర్చు ();
    స్థితి 4: hb_barrier (); తిరిగి u.format4.ఖర్చు ();
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.collect_coverage (glyphs);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.collect_coverage (glyphs);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.collect_coverage (glyphs);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.collect_coverage (glyphs);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_class (సమితి_t *glyphs, గుర్తులేని పూర్ణాంకం klass) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.collect_class (glyphs, klass);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.collect_class (glyphs, klass);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.collect_class (glyphs, klass);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.collect_class (glyphs, klass);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.intersects (glyphs);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.intersects (glyphs);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.intersects (glyphs);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.intersects (glyphs);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }
  తార్కిక intersects_class (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని పూర్ణాంకం klass) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.intersects_class (glyphs, klass);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.intersects_class (glyphs, klass);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.intersects_class (glyphs, klass);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.intersects_class (glyphs, klass);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  శూన్య intersected_class_glyphs (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని klass, hb_సమితి_t *intersect_glyphs) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి;
    }
  }

  శూన్య intersected_classes (స్థిరరాశి hb_సమితి_t *glyphs, hb_సమితి_t *intersect_classes) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.intersected_classes (glyphs, intersect_classes);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.intersected_classes (glyphs, intersect_classes);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); తిరిగి u.format3.intersected_classes (glyphs, intersect_classes);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.intersected_classes (glyphs, intersect_classes);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి;
    }
  }


  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#అయితే_ముగింపు
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);
};

మూస<రకం_పేరు Iterator>
స్థిర అంతఃవరుస తార్కిక ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ తిరిగి (c->ప్రారంభం_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
నిర్మాణం delta_అడ్డువరుస_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  నిర్మాణం chars_t : hb_సదిశ_t<uint8_t>
  {
    పూర్ణాంకం cmp (స్థిరరాశి chars_t& other) స్థిరరాశి
    {
      తిరిగి as_array ().cmp (other.as_array ());
    }

    hb_pair_t<గుర్తులేని, గుర్తులేని> get_వెడల్పు ()
    {
      గుర్తులేని వెడల్పు = 0;
      గుర్తులేని columns = 0;
      కొరకు (గుర్తులేని i = 0; i < నిడివి; i++)
      {
	గుర్తులేని v = arrayZ[i];
	వెడల్పు += v;
	columns += (v != 0);
      }
      తిరిగి hb_pair (వెడల్పు, columns);
    }

    HB_HOT
    hb_pair_t<గుర్తులేని, గుర్తులేని> combine_వెడల్పు (స్థిరరాశి chars_t& other) స్థిరరాశి
    {
      గుర్తులేని combined_వెడల్పు = 0;
      గుర్తులేని combined_columns = 0;
      కొరకు (గుర్తులేని i = 0; i < నిడివి; i++)
      {
        గుర్తులేని v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_వెడల్పు += v;
	combined_columns += (v != 0);
      }
      తిరిగి hb_pair (combined_వెడల్పు, combined_columns);
    }
  };

  hb_pair_t<గుర్తులేని, గుర్తులేని> combine_వెడల్పు (స్థిరరాశి delta_అడ్డువరుస_encoding_t& other_encoding) స్థిరరాశి { తిరిగి chars.combine_వెడల్పు (other_encoding.chars); }

  // Actual data

  chars_t chars;
  గుర్తులేని వెడల్పు = 0;
  గుర్తులేని overhead = 0;
  hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*> వస్తువులు;

  delta_అడ్డువరుస_encoding_t () = అప్రమేయ;
  delta_అడ్డువరుస_encoding_t (hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*> &&అడ్డువరుసలు, గుర్తులేని అంకె_నిలువుల_సంఖ్య)
  {
    ప్రతిజ్ఞా (అడ్డువరుసలు);

    వస్తువులు = std::move (అడ్డువరుసలు);

    ఒకవేళ (unlikely (!chars.resize (అంకె_నిలువుల_సంఖ్య)))
      తిరిగి;

    calculate_chars ();
  }

  శూన్య విలీనం (స్థిరరాశి delta_అడ్డువరుస_encoding_t& other)
  {
    వస్తువులు.alloc (వస్తువులు.నిడివి + other.వస్తువులు.నిడివి);
    కొరకు (స్వయంచాలక &అడ్డువరుస : other.వస్తువులు)
      కూడు_అడ్డువరుస (అడ్డువరుస);

    // Merge chars
    ప్రతిజ్ఞా (chars.నిడివి == other.chars.నిడివి);
    కొరకు (గుర్తులేని i = 0; i < chars.నిడివి; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_మారింది ();
  }

  శూన్య chars_మారింది ()
  {
    స్వయంచాలక _ = chars.get_వెడల్పు ();
    వెడల్పు = _.మొదటి;
    overhead = get_chars_overhead (_.second);
  }

  శూన్య calculate_chars ()
  {
    ప్రతిజ్ఞా (వస్తువులు);

    తార్కిక long_పదాలు = అబద్ధం;

    కొరకు (స్వయంచాలక &అడ్డువరుస : వస్తువులు)
    {
      ప్రతిజ్ఞా (అడ్డువరుస->నిడివి == chars.నిడివి);

      /* 0/1/2 byte encoding */
      కొరకు (గుర్తులేని i = 0; i < అడ్డువరుస->నిడివి; i++)
      {
	పూర్ణాంకం v =  అడ్డువరుస->arrayZ[i];
	ఒకవేళ (v == 0)
	  కొనసాగు;
	లేకపోతే ఒకవేళ (v > 32767 || v < -32768)
	{
	  long_పదాలు = నిజం;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	లేకపోతే ఒకవేళ (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	లేకపోతే
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    ఒకవేళ (long_పదాలు)
    {
      // Convert 1s to 2s
      కొరకు (స్వయంచాలక &v : chars)
	ఒకవేళ (v == 1)
	  v = 2;
    }

    chars_మారింది ();
  }

  తార్కిక is_empty () స్థిరరాశి
  { తిరిగి !వస్తువులు; }

  తార్కిక లోపల_error () స్థిరరాశి
  { తిరిగి chars.లోపల_error () || వస్తువులు.లోపల_error (); }

  స్థిర అంతఃవరుస గుర్తులేని get_chars_overhead (గుర్తులేని అంకె_columns)
  {
    గుర్తులేని c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    తిరిగి c + అంకె_columns * 2;
  }

  గుర్తులేని get_gain (గుర్తులేని additional_బైట్లు_చొప్పున_అడ్డువరుసలు = 1) స్థిరరాశి
  {
    పూర్ణాంకం లెక్క = వస్తువులు.నిడివి;
    తిరిగి hb_max (0, (పూర్ణాంకం) overhead - లెక్క * (పూర్ణాంకం) additional_బైట్లు_చొప్పున_అడ్డువరుసలు);
  }

  పూర్ణాంకం gain_from_merging (స్థిరరాశి delta_అడ్డువరుస_encoding_t& other_encoding) స్థిరరాశి
  {
    // Back of the envelope calculations to reject early.
    గుర్తుగల additional_బైట్లు_చొప్పున_అడ్డువరుసలు = other_encoding.వెడల్పు - వెడల్పు;
    ఒకవేళ (additional_బైట్లు_చొప్పున_అడ్డువరుసలు > 0)
    {
      ఒకవేళ (get_gain (additional_బైట్లు_చొప్పున_అడ్డువరుసలు) == 0)
        తిరిగి 0;
    }
    లేకపోతే
    {
      ఒకవేళ (other_encoding.get_gain (-additional_బైట్లు_చొప్పున_అడ్డువరుసలు) == 0)
	తిరిగి 0;
    }

    స్వయంచాలక pair = combine_వెడల్పు (other_encoding);
    గుర్తులేని combined_వెడల్పు = pair.మొదటి;
    గుర్తులేని combined_columns = pair.second;

    పూర్ణాంకం combined_gain = (పూర్ణాంకం) overhead + (పూర్ణాంకం) other_encoding.overhead;
    combined_gain -= (combined_వెడల్పు - (పూర్ణాంకం) వెడల్పు) * వస్తువులు.నిడివి;
    combined_gain -= (combined_వెడల్పు - (పూర్ణాంకం) other_encoding.వెడల్పు) * other_encoding.వస్తువులు.నిడివి;
    combined_gain -= get_chars_overhead (combined_columns);

    తిరిగి combined_gain;
  }

  తార్కిక కూడు_అడ్డువరుస (స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>* అడ్డువరుస)
  { తిరిగి వస్తువులు.push (అడ్డువరుస); }

  స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    స్థిరరాశి delta_అడ్డువరుస_encoding_t *a = (స్థిరరాశి delta_అడ్డువరుస_encoding_t *)pa;
    స్థిరరాశి delta_అడ్డువరుస_encoding_t *b = (స్థిరరాశి delta_అడ్డువరుస_encoding_t *)pb;

    ఒకవేళ (a->వెడల్పు != b->వెడల్పు)
      తిరిగి (పూర్ణాంకం) a->వెడల్పు - (పూర్ణాంకం) b->వెడల్పు;

    తిరిగి b->chars.cmp (a->chars);
  }
};

నిర్మాణం VarRegionAxis
{
  దశాంశం evaluate (పూర్ణాంకం coord) స్థిరరాశి
  {
    పూర్ణాంకం peak = peakCoord.గమ్యం_int ();
    ఒకవేళ (peak == 0 || coord == peak)
      తిరిగి 1.f;
    లేకపోతే ఒకవేళ (coord == 0) // Faster
      తిరిగి 0.f;

    పూర్ణాంకం ప్రారంభం = ప్రారంభం_Coord.గమ్యం_int (), చివర = చివర_Coord.గమ్యం_int ();

    /* TODO Move these to sanitize(). */
    ఒకవేళ (unlikely (ప్రారంభం > peak || peak > చివర))
      తిరిగి 1.f;
    ఒకవేళ (unlikely (ప్రారంభం < 0 && చివర > 0))
      తిరిగి 1.f;

    ఒకవేళ (coord <= ప్రారంభం || చివర <= coord)
      తిరిగి 0.f;

    /* Interpolate */
    ఒకవేళ (coord < peak)
      తిరిగి దశాంశం (coord - ప్రారంభం) / (peak - ప్రారంభం);
    లేకపోతే
      తిరిగి దశాంశం (చివర - coord) / (చివర - peak);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  తార్కిక serialize (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed (ఇది));
  }

  బహిరంగ:
  F2DOT14	ప్రారంభం_Coord;
  F2DOT14	peakCoord;
  F2DOT14	చివర_Coord;
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};
నిర్మాణం SparseVarRegionAxis
{
  దశాంశం evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len) స్థిరరాశి
  {
    గుర్తులేని i = axis_సూచీ_2;
    పూర్ణాంకం coord = i < coord_len ? coords[i] : 0;
    తిరిగి axis.evaluate (coord);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి SparseVarRegionAxis *src,
		  స్థిరరాశి hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->embed (src);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    ఒకవేళ (axis_map)
    {
      ఒకవేళ (unlikely (!axis_map->has (బహిర్గతం->axis_సూచీ_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (అబద్ధం);
      }
      ఒకవేళ (unlikely (!c->check_కేటాయించు (బహిర్గతం->axis_సూచీ_2, axis_map->get (బహిర్గతం->axis_సూచీ_2),
				    HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
	return_trace (అబద్ధం);
    }
    return_trace (నిజం);
  }

  బహిరంగ:
  HBUINT16 axis_సూచీ_2;
  VarRegionAxis axis;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం hb_scalar_cache_t
{
  అంతరంగ:
  స్థిర constexpr గుర్తులేని STATIC_నిడివి = 128;
  స్థిర constexpr దశాంశం INVALID = NAN;

  బహిరంగ:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (స్థిరరాశి hb_scalar_cache_t&) = తొలగించు;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = తొలగించు;
  hb_scalar_cache_t& క్రియాకారకం= (స్థిరరాశి hb_scalar_cache_t&) = తొలగించు;
  hb_scalar_cache_t& క్రియాకారకం= (hb_scalar_cache_t&&) = తొలగించు;

  స్థిర hb_scalar_cache_t *create (గుర్తులేని పూర్ణాంకం లెక్క,
				    hb_scalar_cache_t *scratch_cache = శూన్యసూచిక)
  {
    ఒకవేళ (!లెక్క) తిరిగి (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    ఒకవేళ (scratch_cache && లెక్క <= STATIC_నిడివి)
    {
      scratch_cache->నిడివి = లెక్క;
      scratch_cache->clear ();
      తిరిగి scratch_cache;
    }

    స్వయంచాలక *cache = (hb_scalar_cache_t *) hb_malloc (పరిమాణం (hb_scalar_cache_t) - పరిమాణం (static_విలువలు) + పరిమాణం (static_విలువలు[0]) * లెక్క);
    ఒకవేళ (unlikely (!cache)) తిరిగి (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->నిడివి = లెక్క;
    cache->clear ();

    తిరిగి cache;
  }

  స్థిర శూన్య destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = శూన్యసూచిక)
  {
    ఒకవేళ (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  శూన్య clear ()
  {
    స్వయంచాలక *values = &static_విలువలు[0];
    గుర్తులేని i = 0;
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
    కొరకు (; i + 3 < నిడివి; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#అయితే_ముగింపు
    కొరకు (; i < నిడివి; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  తార్కిక get (గుర్తులేని i, దశాంశం *value) స్థిరరాశి
  {
    ఒకవేళ (unlikely (i >= నిడివి))
    {
      *value = 0.f;
      తిరిగి నిజం;
    }
    స్వయంచాలక *values = &static_విలువలు[0];
    స్వయంచాలక *cached_విలువ = &values[i];
    దశాంశం v = *cached_విలువ;
    ఒకవేళ (std::isnan (v))
      తిరిగి అబద్ధం;
    *value = v;
    తిరిగి నిజం;
  }

  HB_ALWAYS_INLINE
  శూన్య సమితి (గుర్తులేని i, దశాంశం value)
  {
    ఒకవేళ (unlikely (i >= నిడివి)) తిరిగి;
    స్వయంచాలక *values = &static_విలువలు[0];
    స్వయంచాలక *cached_విలువ = &values[i];
    *cached_విలువ = value;
  }

  బహిరంగ:
  గుర్తులేని నిడివి;
  అంతరంగ:
  మార్చదగిన hb_atomic_t<దశాంశం> static_విలువలు[STATIC_నిడివి];
};

నిర్మాణం VarRegionList
{
  అంతరంగ:
  దశాంశం evaluate_impl (గుర్తులేని పూర్ణాంకం region_సూచీ,
		       స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len) స్థిరరాశి
  {
    స్థిరరాశి VarRegionAxis *axes = axesZ.arrayZ + (region_సూచీ * axis_లెక్క_2);
    దశాంశం v = 1.f;

    గుర్తులేని పూర్ణాంకం లెక్క = axis_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      పూర్ణాంకం coord = i < coord_len ? coords[i] : 0;
      దశాంశం గుణకం = axes[i].evaluate (coord);
      ఒకవేళ (గుణకం == 0.f)
      {
	v = 0.f;
	ఆపు;
      }
      v *= గుణకం;
    }

    తిరిగి v;
  }

  బహిరంగ:
  HB_ALWAYS_INLINE
  దశాంశం evaluate (గుర్తులేని పూర్ణాంకం region_సూచీ,
		  స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		  hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (unlikely (region_సూచీ >= region_లెక్క))
      తిరిగి 0.;

    దశాంశం v;
    ఒకవేళ (cache && cache->get (region_సూచీ, &v))
      తిరిగి v;

    v = evaluate_impl (region_సూచీ, coords, coord_len);

    ఒకవేళ (cache)
      cache->సమితి (region_సూచీ, v);
    తిరిగి v;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_లెక్క_2 * region_లెక్క));
  }

  తార్కిక serialize (hb_serialize_context_t *c,
                  స్థిరరాశి hb_సదిశ_t<hb_ట్యాగ్_t>& axis_ట్యాగులు,
                  స్థిరరాశి hb_సదిశ_t<స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని axis_లెక్క = axis_ట్యాగులు.నిడివి;
    గుర్తులేని region_లెక్క_2 = regions.నిడివి;
    ఒకవేళ (!axis_లెక్క || !region_లెక్క_2) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (axis_లెక్క * region_లెక్క_2,
                                             VarRegionAxis::static_కొలత))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    axis_లెక్క_2 = axis_లెక్క;
    region_లెక్క = region_లెక్క_2;

    కొరకు (గుర్తులేని r = 0; r < region_లెక్క_2; r++)
    {
      స్థిరరాశి స్వయంచాలక& region = regions[r];
      కొరకు (గుర్తులేని i = 0; i < axis_లెక్క; i++)
      {
        hb_ట్యాగ్_t ట్యాగ్_2 = axis_ట్యాగులు.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        ఒకవేళ (region->has (ట్యాగ్_2, &coords))
        {
          var_region_rec.ప్రారంభం_Coord.సమితి_float (coords->minimum);
          var_region_rec.peakCoord.సమితి_float (coords->మధ్య);
          var_region_rec.చివర_Coord.సమితి_float (coords->గరిష్ఠ);
        }
        లేకపోతే
        {
          var_region_rec.ప్రారంభం_Coord.సమితి_int (0);
          var_region_rec.peakCoord.సమితి_int (0);
          var_region_rec.చివర_Coord.సమితి_int (0);
        }
        ఒకవేళ (!var_region_rec.serialize (c))
          return_trace (అబద్ధం);
      }
    }
    return_trace (నిజం);
  }

  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి VarRegionList *src, స్థిరరాశి hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    axis_లెక్క_2 = src->axis_లెక్క_2;
    region_లెక్క = region_map.get_population ();
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (axis_లెక్క_2 * region_లెక్క,
					     VarRegionAxis::static_కొలత))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->extend (ఇది))) return_trace (అబద్ధం);
    గుర్తులేని పూర్ణాంకం region_లెక్క_2 = src->region_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం r = 0; r < region_లెక్క; r++)
    {
      గుర్తులేని పూర్ణాంకం backward = region_map.backward (r);
      ఒకవేళ (backward >= region_లెక్క_2) return_trace (అబద్ధం);
      hb_memcpy (&axesZ[axis_లెక్క_2 * r], &src->axesZ[axis_లెక్క_2 * backward], VarRegionAxis::static_కొలత * axis_లెక్క_2);
    }

    return_trace (నిజం);
  }

  తార్కిక get_var_region (గుర్తులేని region_సూచీ,
                       స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map,
                       hb_hashmap_t<hb_ట్యాగ్_t, Triple>& axis_tuples /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (region_సూచీ >= region_లెక్క) తిరిగి అబద్ధం;
    స్థిరరాశి VarRegionAxis* axis_region = axesZ.arrayZ + (region_సూచీ * axis_లెక్క_2);
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      hb_ట్యాగ్_t *axis_ట్యాగ్_2;
      ఒకవేళ (!axes_old_సూచీ_ట్యాగ్_map.has (i, &axis_ట్యాగ్_2))
        తిరిగి అబద్ధం;

      దశాంశం min_val = axis_region->ప్రారంభం_Coord.గమ్యం_float ();
      దశాంశం def_val = axis_region->peakCoord.గమ్యం_float ();
      దశాంశం max_val = axis_region->చివర_Coord.గమ్యం_float ();

      ఒకవేళ (def_val != 0.f)
        axis_tuples.సమితి (*axis_ట్యాగ్_2, Triple ((ద్విగుణ) min_val, (ద్విగుణ) def_val, (ద్విగుణ) max_val));
      axis_region++;
    }
    తిరిగి !axis_tuples.లోపల_error ();
  }

  తార్కిక get_var_regions (స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map,
                        hb_సదిశ_t<hb_hashmap_t<hb_ట్యాగ్_t, Triple>>& regions /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (!regions.alloc (region_లెక్క))
      తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని i = 0; i < region_లెక్క; i++)
    {
      hb_hashmap_t<hb_ట్యాగ్_t, Triple> axis_tuples;
      ఒకవేళ (!get_var_region (i, axes_old_సూచీ_ట్యాగ్_map, axis_tuples))
        తిరిగి అబద్ధం;
      regions.push (std::move (axis_tuples));
    }
    తిరిగి !regions.లోపల_error ();
  }

  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_కూడు_saturate (min_కొలత,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_కొలత,
							       axis_లెక్క_2, region_లెక్క)); }

  బహిరంగ:
  HBUINT16	axis_లెక్క_2;
  HBUINT15	region_లెక్క;
  రక్షిత:
  Unsized_Array_యొక్క<VarRegionAxis>
		axesZ;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
స్థిర అంతఃవరుస తార్కిక
_hb_avar2_region_is_dead (స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> &axis_tuples,
			  స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> &reachable_ranges)
{
  కొరకు (స్వయంచాలక _ : axis_tuples)
  {
    Triple *range;
    ఒకవేళ (!reachable_ranges.has (_.మొదటి, &range)) కొనసాగు;
    స్థిరరాశి Triple &tent = _.second;
    పూర్ణాంకం ప్రారంభం = (పూర్ణాంకం) roundf ((దశాంశం) tent.minimum * 16384.f);
    పూర్ణాంకం peak  = (పూర్ణాంకం) roundf ((దశాంశం) tent.మధ్య * 16384.f);
    పూర్ణాంకం చివర   = (పూర్ణాంకం) roundf ((దశాంశం) tent.గరిష్ఠ * 16384.f);
    ఒకవేళ (!peak) కొనసాగు;
    ఒకవేళ (ప్రారంభం > peak || peak > చివర) కొనసాగు;
    ఒకవేళ (ప్రారంభం < 0 && చివర > 0) కొనసాగు;
    పూర్ణాంకం lo = (పూర్ణాంకం) roundf ((దశాంశం) range->minimum * 16384.f);
    పూర్ణాంకం hi = (పూర్ణాంకం) roundf ((దశాంశం) range->గరిష్ఠ * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    ఒకవేళ (hi < ప్రారంభం || lo > చివర)
      తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}

నిర్మాణం SparseVariationRegion
{
  దశాంశం evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len) స్థిరరాశి
  {
    దశాంశం v = 1.f;
    గుర్తులేని పూర్ణాంకం లెక్క = axes.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      దశాంశం గుణకం = (ఇది+axes[i]).evaluate (coords, coord_len);
      ఒకవేళ (గుణకం == 0.f)
	తిరిగి 0.;
      v *= గుణకం;
    }
    తిరిగి v;
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి SparseVariationRegion *src,
		  స్థిరరాశి hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!axes.serialize (c, (గుర్తులేని) src->axes.len))) return_trace (అబద్ధం);
    కొరకు (గుర్తులేని i = 0; i < axes.len; i++)
      ఒకవేళ (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (axes.sanitize (c, ఇది));
  }

  రక్షిత:
  Array16_యొక్క<Offset32_గమ్యం<SparseVarRegionAxis>> axes;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, axes);
};

నిర్మాణం SparseVarRegionList
{
  HB_ALWAYS_INLINE
  దశాంశం evaluate (గుర్తులేని పూర్ణాంకం region_సూచీ,
		  స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		  hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (unlikely (region_సూచీ >= regions.len))
      తిరిగి 0.;

    దశాంశం v;
    ఒకవేళ (cache && cache->get (region_సూచీ, &v))
      తిరిగి v;

    స్థిరరాశి SparseVariationRegion &region = ఇది+regions[region_సూచీ];

    v = region.evaluate (coords, coord_len);
    ఒకవేళ (cache)
      cache->సమితి (region_సూచీ, v);

    తిరిగి v;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (regions.sanitize (c, ఇది));
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి SparseVarRegionList *src,
		  స్థిరరాశి hb_inc_bimap_t &region_map,
		  స్థిరరాశి hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    regions.len = region_map.get_population ();
    ఒకవేళ (unlikely (!c->extend (regions))) return_trace (అబద్ధం);

    కొరకు (గుర్తులేని i = 0; i < regions.len; i++)
    {
      గుర్తులేని old_సూచీ = region_map.backward (i);
      ఒకవేళ (unlikely (old_సూచీ >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_సూచీ]),
					     axis_map)))
	return_trace (అబద్ధం);
    }
    return_trace (నిజం);
  }

  బహిరంగ:
  Array32_యొక్క<Offset32_గమ్యం<SparseVariationRegion>>
		regions;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, regions);
};


నిర్మాణం Var_దత్తాంశం
{
  గుర్తులేని పూర్ణాంకం get_అంశం_లెక్క () స్థిరరాశి
  { తిరిగి అంశం_లెక్క; }

  గుర్తులేని పూర్ణాంకం get_region_సూచీ_లెక్క () స్థిరరాశి
  { తిరిగి regionIndices.len; }

  గుర్తులేని get_region_సూచీ (గుర్తులేని i) స్థిరరాశి
  { తిరిగి i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_అడ్డువరుస_కొలత () స్థిరరాశి
  { తిరిగి (మాట_లెక్క () + regionIndices.len) * (long_పదాలు_2 () ? 2 : 1); }

  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_కూడు_saturate (min_కొలత - regionIndices.min_కొలత,
				     regionIndices.get_కొలత (),
				     hb_unsigned_mul_saturate (అంశం_లెక్క, get_అడ్డువరుస_కొలత ())); }

  మూస <రకం_పేరు Float = దశాంశం>
  Float _get_delta (గుర్తులేని పూర్ణాంకం inner,
		    స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
		    స్థిరరాశి VarRegionList &regions,
		    hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (unlikely (inner >= అంశం_లెక్క))
      తిరిగి 0.;
    తార్కిక is_long = long_పదాలు_2 ();
    గుర్తులేని పూర్ణాంకం లెక్క = regionIndices.len;
    గుర్తులేని మాట_లెక్క_2 = మాట_లెక్క ();
    గుర్తులేని పూర్ణాంకం scount = is_long ? లెక్క : మాట_లెక్క_2;
    గుర్తులేని పూర్ణాంకం lcount = is_long ? మాట_లెక్క_2 : 0;

    స్థిరరాశి HBUINT8 *బైట్లు = get_delta_బైట్లు ();
    స్థిరరాశి HBUINT8 *అడ్డువరుస = బైట్లు + inner * get_అడ్డువరుస_కొలత ();

    Float delta = 0;
    గుర్తులేని పూర్ణాంకం i = 0;

    స్థిరరాశి HBINT32 *lcursor = reinterpret_cast<స్థిరరాశి HBINT32 *> (అడ్డువరుస);
    కొరకు (; i < lcount; i++)
    {
      దశాంశం scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_లెక్క, cache);
      ఒకవేళ (scalar)
        delta += (Float) scalar * (పూర్ణాంకం) *lcursor;
      lcursor++;
    }
    స్థిరరాశి HBINT16 *scursor = reinterpret_cast<స్థిరరాశి HBINT16 *> (lcursor);
    కొరకు (; i < scount; i++)
    {
      దశాంశం scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_లెక్క, cache);
      ఒకవేళ (scalar)
       delta += (Float) scalar * (పూర్ణాంకం) *scursor;
      scursor++;
    }
    స్థిరరాశి HBINT8 *bcursor = reinterpret_cast<స్థిరరాశి HBINT8 *> (scursor);
    కొరకు (; i < లెక్క; i++)
    {
      దశాంశం scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_లెక్క, cache);
      ఒకవేళ (scalar)
        delta += (Float) scalar * (పూర్ణాంకం) *bcursor;
      bcursor++;
    }

    తిరిగి delta;
  }

  మూస <రకం_పేరు Float = దశాంశం>
  HB_ALWAYS_INLINE
  Float get_delta (గుర్తులేని పూర్ణాంకం inner,
		   స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
		   స్థిరరాశి VarRegionList &regions,
		   hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = regionIndices.len;
    ఒకవేళ (!లెక్క) తిరిగి Float (0); // This is quite common, so optimize it.
    తిరిగి _get_delta<Float> (inner, coords, coord_లెక్క, regions, cache);
  }

  శూన్య get_region_scalars (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
			   స్థిరరాశి VarRegionList &regions,
			   దశాంశం *scalars /*OUT */,
			   గుర్తులేని పూర్ణాంకం అంకె_scalars) స్థిరరాశి
  {
    గుర్తులేని లెక్క = hb_min (అంకె_scalars, regionIndices.len);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_లెక్క);
    కొరకు (గుర్తులేని పూర్ణాంకం i = లెక్క; i < అంకె_scalars; i++)
      scalars[i] = 0.f;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  మాట_లెక్క () <= regionIndices.len &&
		  c->check_range (get_delta_బైట్లు (),
				  అంశం_లెక్క,
				  get_అడ్డువరుస_కొలత ()));
  }

  తార్కిక serialize (hb_serialize_context_t *c,
                  తార్కిక has_long,
                  స్థిరరాశి hb_సదిశ_t<స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>*>& అడ్డువరుసలు)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని అడ్డువరుస_లెక్క_3 = అడ్డువరుసలు.నిడివి;
    ఒకవేళ (!అడ్డువరుస_లెక్క_3) {
      // Nothing to serialize, will be empty.
      తిరిగి అబద్ధం;
    }

    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->check_కేటాయించు (అంశం_లెక్క, అడ్డువరుస_లెక్క_3,
                                    HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
      return_trace (అబద్ధం);

    పూర్ణాంకం min_పరిమితి = has_long ? -32768 : -128;
    పూర్ణాంకం max_పరిమితి = has_long ? +32767 : +127;
    గణన delta_కొలత_t { kZero=0, k_Non_మాట, k_మాట };
    hb_సదిశ_t<delta_కొలత_t> delta_sz;
    గుర్తులేని అంకె_regions = అడ్డువరుసలు[0]->నిడివి;
    ఒకవేళ (!delta_sz.resize (అంకె_regions))
      return_trace (అబద్ధం);

    గుర్తులేని మాట_లెక్క_2 = 0;
    కొరకు (గుర్తులేని r = 0; r < అంకె_regions; r++)
    {
      కొరకు (గుర్తులేని i = 0; i < అడ్డువరుస_లెక్క_3; i++)
      {
        పూర్ణాంకం delta = అడ్డువరుసలు[i]->arrayZ[r];
        ఒకవేళ (delta < min_పరిమితి || delta > max_పరిమితి)
        {
          delta_sz[r] = k_మాట;
          మాట_లెక్క_2++;
          ఆపు;
        }
        లేకపోతే ఒకవేళ (delta != 0)
        {
          delta_sz[r] = k_Non_మాట;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    గుర్తులేని మాట_సూచీ = 0;
    గుర్తులేని non_మాట_సూచీ = మాట_లెక్క_2;
    hb_map_t ri_map;
    కొరకు (గుర్తులేని r = 0; r < అంకె_regions; r++)
    {
      ఒకవేళ (!delta_sz[r]) కొనసాగు;
      గుర్తులేని new_r = (delta_sz[r] == k_మాట)? మాట_సూచీ++ : non_మాట_సూచీ++;
      ఒకవేళ (!ri_map.సమితి (new_r, r))
        return_trace (అబద్ధం);
    }

    మాట_కొలత_లెక్క = మాట_లెక్క_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    గుర్తులేని ri_లెక్క = ri_map.get_population ();
    regionIndices.len = ri_లెక్క;
    ఒకవేళ (unlikely (!c->extend (ఇది))) return_trace (అబద్ధం);

    కొరకు (గుర్తులేని r = 0; r < ri_లెక్క; r++)
    {
      hb_codepoint_t *idx;
      ఒకవేళ (!ri_map.has (r, &idx))
        return_trace (అబద్ధం);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_బైట్లు = get_delta_బైట్లు ();
    గుర్తులేని అడ్డువరుస_కొలత = get_అడ్డువరుస_కొలత ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అడ్డువరుస_లెక్క_3; i++)
    {
      కొరకు (గుర్తులేని పూర్ణాంకం r = 0; r < ri_లెక్క; r++)
      {
        పూర్ణాంకం delta = అడ్డువరుసలు[i]->arrayZ[ri_map[r]];
        సమితి_అంశం_delta_fast (i, r, delta, delta_బైట్లు, అడ్డువరుస_కొలత);
      }
    }
    return_trace (నిజం);
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి Var_దత్తాంశం *src,
		  స్థిరరాశి hb_inc_bimap_t &inner_map,
		  స్థిరరాశి hb_inc_bimap_t &region_map,
		  స్థిరరాశి hb_సమితి_t *culled_regions = శూన్యసూచిక)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    అంశం_లెక్క = inner_map.get_తదుపరి_విలువ ();

    /* Optimize word count */
    గుర్తులేని ri_లెక్క = src->regionIndices.len;
    గణన delta_కొలత_t { kZero=0, k_Non_మాట, k_మాట };
    hb_సదిశ_t<delta_కొలత_t> delta_sz;
    hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_లెక్క);
    ri_map.resize (ri_లెక్క);
    గుర్తులేని పూర్ణాంకం new_మాట_లెక్క = 0;
    గుర్తులేని పూర్ణాంకం r;

    స్థిరరాశి HBUINT8 *src_delta_బైట్లు = src->get_delta_బైట్లు ();
    గుర్తులేని src_అడ్డువరుస_కొలత = src->get_అడ్డువరుస_కొలత ();
    గుర్తులేని src_మాట_లెక్క = src->మాట_లెక్క ();
    తార్కిక     src_long_పదాలు = src->long_పదాలు_2 ();

    తార్కిక has_long = అబద్ధం;
    ఒకవేళ (src_long_పదాలు)
    {
      కొరకు (r = 0; r < src_మాట_లెక్క; r++)
      {
        కొరకు (గుర్తులేని old_gid : inner_map.కీలకాలు())
	{
	  int32_t delta = src->get_అంశం_delta_fast (old_gid, r, src_delta_బైట్లు, src_అడ్డువరుస_కొలత);
	  ఒకవేళ (delta < -32768 || 32767 < delta)
	  {
	    has_long = నిజం;
	    ఆపు;
	  }
        }
      }
    }

    గుర్తుగల min_పరిమితి = has_long ? -32768 : -128;
    గుర్తుగల max_పరిమితి = has_long ? +32767 : +127;
    కొరకు (r = 0; r < ri_లెక్క; r++)
    {
      తార్కిక short_circuit = src_long_పదాలు == has_long && src_మాట_లెక్క <= r;

      delta_sz[r] = kZero;
      ఒకవేళ (culled_regions && culled_regions->has (src->regionIndices[r]))
	కొనసాగు; /* unreachable region: drop its column */
      కొరకు (గుర్తులేని old_gid : inner_map.కీలకాలు())
      {
	int32_t delta = src->get_అంశం_delta_fast (old_gid, r, src_delta_బైట్లు, src_అడ్డువరుస_కొలత);
	ఒకవేళ (delta < min_పరిమితి || max_పరిమితి < delta)
	{
	  delta_sz[r] = k_మాట;
	  new_మాట_లెక్క++;
	  ఆపు;
	}
	లేకపోతే ఒకవేళ (delta != 0)
	{
	  delta_sz[r] = k_Non_మాట;
	  ఒకవేళ (short_circuit)
	    ఆపు;
	}
      }
    }

    గుర్తులేని పూర్ణాంకం మాట_సూచీ = 0;
    గుర్తులేని పూర్ణాంకం non_మాట_సూచీ = new_మాట_లెక్క;
    గుర్తులేని పూర్ణాంకం new_ri_లెక్క = 0;
    కొరకు (r = 0; r < ri_లెక్క; r++)
      ఒకవేళ (delta_sz[r])
      {
	గుర్తులేని new_r = (delta_sz[r] == k_మాట)? మాట_సూచీ++ : non_మాట_సూచీ++;
	ri_map[new_r] = r;
	new_ri_లెక్క++;
      }

    మాట_కొలత_లెక్క = new_మాట_లెక్క | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_లెక్క;

    ఒకవేళ (unlikely (!c->extend (ఇది))) return_trace (అబద్ధం);

    కొరకు (r = 0; r < new_ri_లెక్క; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_బైట్లు = get_delta_బైట్లు ();
    గుర్తులేని అడ్డువరుస_కొలత = get_అడ్డువరుస_కొలత ();
    గుర్తులేని లెక్క = అంశం_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      గుర్తులేని పూర్ణాంకం old = inner_map.backward (i);
      కొరకు (గుర్తులేని పూర్ణాంకం r = 0; r < new_ri_లెక్క; r++)
	సమితి_అంశం_delta_fast (i, r,
			     src->get_అంశం_delta_fast (old, ri_map[r],
						       src_delta_బైట్లు, src_అడ్డువరుస_కొలత),
			     delta_బైట్లు, అడ్డువరుస_కొలత);
    }

    return_trace (నిజం);
  }

  శూన్య collect_region_refs (hb_సమితి_t &region_indices, స్థిరరాశి hb_inc_bimap_t &inner_map) స్థిరరాశి
  {
    స్థిరరాశి HBUINT8 *delta_బైట్లు = get_delta_బైట్లు ();
    గుర్తులేని అడ్డువరుస_కొలత = get_అడ్డువరుస_కొలత ();

    కొరకు (గుర్తులేని పూర్ణాంకం r = 0; r < regionIndices.len; r++)
    {
      గుర్తులేని పూర్ణాంకం region = regionIndices.arrayZ[r];
      ఒకవేళ (region_indices.has (region)) కొనసాగు;
      కొరకు (hb_codepoint_t old_gid : inner_map.కీలకాలు())
	ఒకవేళ (get_అంశం_delta_fast (old_gid, r, delta_బైట్లు, అడ్డువరుస_కొలత) != 0)
	{
	  region_indices.కూడు (region);
	  ఆపు;
	}
    }
  }

  బహిరంగ:
  స్థిరరాశి HBUINT8 *get_delta_బైట్లు () స్థిరరాశి
  { తిరిగి &StructAfter<HBUINT8> (regionIndices); }

  రక్షిత:
  HBUINT8 *get_delta_బైట్లు ()
  { తిరిగి &StructAfter<HBUINT8> (regionIndices); }

  బహిరంగ:
  int32_t get_అంశం_delta_fast (గుర్తులేని పూర్ణాంకం అంశం, గుర్తులేని పూర్ణాంకం region,
			       స్థిరరాశి HBUINT8 *delta_బైట్లు, గుర్తులేని అడ్డువరుస_కొలత) స్థిరరాశి
  {
    ఒకవేళ (unlikely (అంశం >= అంశం_లెక్క || region >= regionIndices.len)) తిరిగి 0;

    స్థిరరాశి HBINT8 *p = (స్థిరరాశి HBINT8 *) delta_బైట్లు + అంశం * అడ్డువరుస_కొలత;
    గుర్తులేని మాట_లెక్క_2 = మాట_లెక్క ();
    తార్కిక is_long = long_పదాలు_2 ();
    ఒకవేళ (is_long)
    {
      ఒకవేళ (region < మాట_లెక్క_2)
	తిరిగి ((స్థిరరాశి HBINT32 *) p)[region];
      లేకపోతే
	తిరిగి ((స్థిరరాశి HBINT16 *)(p + HBINT32::static_కొలత * మాట_లెక్క_2))[region - మాట_లెక్క_2];
    }
    లేకపోతే
    {
      ఒకవేళ (region < మాట_లెక్క_2)
	తిరిగి ((స్థిరరాశి HBINT16 *) p)[region];
      లేకపోతే
	తిరిగి (p + HBINT16::static_కొలత * మాట_లెక్క_2)[region - మాట_లెక్క_2];
    }
  }
  int32_t get_అంశం_delta (గుర్తులేని పూర్ణాంకం అంశం, గుర్తులేని పూర్ణాంకం region) స్థిరరాశి
  {
     తిరిగి get_అంశం_delta_fast (అంశం, region,
				 get_delta_బైట్లు (),
				 get_అడ్డువరుస_కొలత ());
  }

  రక్షిత:
  శూన్య సమితి_అంశం_delta_fast (గుర్తులేని పూర్ణాంకం అంశం, గుర్తులేని పూర్ణాంకం region, int32_t delta,
			    HBUINT8 *delta_బైట్లు, గుర్తులేని అడ్డువరుస_కొలత)
  {
    HBINT8 *p = (HBINT8 *) delta_బైట్లు + అంశం * అడ్డువరుస_కొలత;
    గుర్తులేని మాట_లెక్క_2 = మాట_లెక్క ();
    తార్కిక is_long = long_పదాలు_2 ();
    ఒకవేళ (is_long)
    {
      ఒకవేళ (region < మాట_లెక్క_2)
	((HBINT32 *) p)[region] = delta;
      లేకపోతే
	((HBINT16 *)(p + HBINT32::static_కొలత * మాట_లెక్క_2))[region - మాట_లెక్క_2] = delta;
    }
    లేకపోతే
    {
      ఒకవేళ (region < మాట_లెక్క_2)
	((HBINT16 *) p)[region] = delta;
      లేకపోతే
	(p + HBINT16::static_కొలత * మాట_లెక్క_2)[region - మాట_లెక్క_2] = delta;
    }
  }
  శూన్య సమితి_అంశం_delta (గుర్తులేని పూర్ణాంకం అంశం, గుర్తులేని పూర్ణాంకం region, int32_t delta)
  {
    సమితి_అంశం_delta_fast (అంశం, region, delta,
			 get_delta_బైట్లు (),
			 get_అడ్డువరుస_కొలత ());
  }

  తార్కిక long_పదాలు_2 () స్థిరరాశి { తిరిగి మాట_కొలత_లెక్క & 0x8000u /* LONG_WORDS */; }
  గుర్తులేని మాట_లెక్క () స్థిరరాశి { తిరిగి మాట_కొలత_లెక్క & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  రక్షిత:
  HBUINT16		అంశం_లెక్క;
  HBUINT16		మాట_కొలత_లెక్క;
  Array16_యొక్క<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, regionIndices);
};

నిర్మాణం Multi_Var_దత్తాంశం
{
  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_కూడు_saturate (min_కొలత - regionIndices.min_కొలత,
				     regionIndices.get_కొలత ()); }

  స్థిరరాశి TupleList &get_delta_sets () స్థిరరాశి
  { తిరిగి ఇది+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  శూన్య get_delta (గుర్తులేని పూర్ణాంకం inner,
		  స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
		  స్థిరరాశి SparseVarRegionList &regions,
		  hb_array_t<ద్విగుణ> బహిర్గతం,
		  hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    స్వయంచాలక &deltaSets = get_delta_sets ();

    స్వయంచాలక విలువలు_iter = deltaSets.fetcher (inner);
    గుర్తులేని region_లెక్క = regionIndices.len;
    గుర్తులేని skip = 0;
    కొరకు (గుర్తులేని region_సూచీ_2 = 0; region_సూచీ_2 < region_లెక్క; region_సూచీ_2++)
    {
      దశాంశం scalar = regions.evaluate (regionIndices.arrayZ[region_సూచీ_2],
				       coords, coord_లెక్క,
				       cache);
      // We skip lazily. Helps with the tail end.
      ఒకవేళ (scalar == 0.0f)
        skip += బహిర్గతం.నిడివి;
      లేకపోతే
      {
        ఒకవేళ (skip)
	{
	  విలువలు_iter.skip (skip);
	  skip = 0;
	}
	విలువలు_iter.కూడు_గమ్యం_2 (బహిర్గతం, scalar);
      }
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, ఇది));
  }

  గుర్తులేని get_అంశం_లెక్క () స్థిరరాశి
  { తిరిగి get_delta_sets ().లెక్క; }

  శూన్య collect_region_refs (hb_సమితి_t &region_indices) స్థిరరాశి
  {
    కొరకు (గుర్తులేని region : regionIndices)
      region_indices.కూడు (region);
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి Multi_Var_దత్తాంశం *src,
		  స్థిరరాశి hb_inc_bimap_t &inner_map,
		  స్థిరరాశి hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని region_లెక్క_2 = src->regionIndices.len;
    గుర్తులేని శీర్షిక_కొలత = HBUINT8::static_కొలత +
			   Array16_యొక్క<HBUINT16>::min_కొలత +
			   region_లెక్క_2 * HBUINT16::static_కొలత +
			   HBUINT32::static_కొలత;
    ఒకవేళ (unlikely (!c->extend_కొలత (ఇది, శీర్షిక_కొలత, అబద్ధం)))
      return_trace (అబద్ధం);
    format = 1;
    regionIndices.len = region_లెక్క_2;
    కొరకు (గుర్తులేని i = 0; i < regionIndices.len; i++)
    {
      గుర్తులేని old_region = src->regionIndices[i];
      ఒకవేళ (unlikely (!region_map.has (old_region)))
	return_trace (అబద్ధం);
      regionIndices[i] = region_map.get (old_region);
    }

    స్థిరరాశి స్వయంచాలక &src_delta_sets = src->get_delta_sets ();
    hb_సదిశ_t<hb_ubytes_t> delta_sets;
    గుర్తులేని దత్తాంశం_కొలత = 0;
    ఒకవేళ (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (అబద్ధం);
    కొరకు (గుర్తులేని i = 0; i < inner_map.get_population (); i++)
    {
      గుర్తులేని old_inner = inner_map.backward (i);
      ఒకవేళ (unlikely (old_inner >= src_delta_sets.లెక్క)) return_trace (అబద్ధం);
      hb_ubytes_t బైట్లు = static_cast<స్థిరరాశి CFF2_సూచీ &> (src_delta_sets)[old_inner];
      దత్తాంశం_కొలత = hb_unsigned_కూడు_saturate (దత్తాంశం_కొలత, బైట్లు.నిడివి);
      delta_sets.push (బైట్లు);
    }
    ఒకవేళ (unlikely (దత్తాంశం_కొలత == UINT_MAX || delta_sets.లోపల_error ()))
      return_trace (అబద్ధం);

    స్వయంచాలక &బహిర్గతం_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (బహిర్గతం_delta_sets.serialize_serialize (c, delta_sets.iter (), &దత్తాంశం_కొలత));
  }

  రక్షిత:
  HBUINT8	      format; // 1
  Array16_యొక్క<HBUINT16> regionIndices;
  Offset32_గమ్యం<TupleList> deltaSetsX;
  బహిరంగ:
  DEFINE_కొలత_MIN (7);
};

నిర్మాణం అంశం_Variation_Store
{
  మిత్ర నిర్మాణం అంశం_variations_t;

  hb_scalar_cache_t *create_cache () స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి hb_scalar_cache_t::create (0);
#అయితే_ముగింపు
    తిరిగి hb_scalar_cache_t::create ((ఇది+regions).region_లెక్క);
  }

  స్థిర శూన్య destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  అంతరంగ:
  మూస <రకం_పేరు Float = దశాంశం>
  Float get_delta (గుర్తులేని పూర్ణాంకం outer, గుర్తులేని పూర్ణాంకం inner,
		   స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
		   hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి Float (0);
#అయితే_ముగింపు

    ఒకవేళ (unlikely (outer >= దత్తాంశం_Sets.len))
      తిరిగి Float (0);

    తిరిగి (ఇది+దత్తాంశం_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_లెక్క,
					     ఇది+regions,
					     cache);
  }

  బహిరంగ:
  తార్కిక has_delta_సమితి (గుర్తులేని పూర్ణాంకం సూచీ_2) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి అబద్ధం;
#అయితే_ముగింపు

    గుర్తులేని పూర్ణాంకం outer = సూచీ_2 >> 16;
    గుర్తులేని పూర్ణాంకం inner = సూచీ_2 & 0xFFFF;
    తిరిగి outer < దత్తాంశం_Sets.len &&
	   inner < (ఇది+దత్తాంశం_Sets[outer]).get_అంశం_లెక్క ();
  }

  మూస <రకం_పేరు Float = దశాంశం>
  Float get_delta (గుర్తులేని పూర్ణాంకం సూచీ_2,
		   స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
		   hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం outer = సూచీ_2 >> 16;
    గుర్తులేని పూర్ణాంకం inner = సూచీ_2 & 0xFFFF;
    తిరిగి get_delta<Float> (outer, inner, coords, coord_లెక్క, cache);
  }
  మూస <రకం_పేరు Float = దశాంశం>
  Float get_delta (గుర్తులేని పూర్ణాంకం సూచీ_2,
		   hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
		   hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    తిరిగి get_delta<Float> (సూచీ_2,
		      coords.arrayZ, coords.నిడివి,
		      cache);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి నిజం;
#అయితే_ముగింపు

    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ఇది) &&
		  దత్తాంశం_Sets.sanitize (c, ఇది));
  }

  తార్కిక serialize (hb_serialize_context_t *c,
                  తార్కిక has_long,
                  స్థిరరాశి hb_సదిశ_t<hb_ట్యాగ్_t>& axis_ట్యాగులు,
                  స్థిరరాశి hb_సదిశ_t<స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>*>& region_list,
                  స్థిరరాశి hb_సదిశ_t<delta_అడ్డువరుస_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (ఇది);
#నిర్వచితమైతే HB_NO_VAR
    return_trace (అబద్ధం);
#అయితే_ముగింపు
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    format = 1;
    ఒకవేళ (!regions.serialize_serialize (c, axis_ట్యాగులు, region_list))
      return_trace (అబద్ధం);

    గుర్తులేని అంకె_var_దత్తాంశం = vardata_encodings.నిడివి;
    ఒకవేళ (!అంకె_var_దత్తాంశం) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->check_కేటాయించు (దత్తాంశం_Sets.len, అంకె_var_దత్తాంశం,
                                    HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
      return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!c->extend (దత్తాంశం_Sets))) return_trace (అబద్ధం);
    కొరకు (గుర్తులేని i = 0; i < అంకె_var_దత్తాంశం; i++)
      ఒకవేళ (!దత్తాంశం_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].వస్తువులు))
        return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి అంశం_Variation_Store *src,
		  స్థిరరాశి hb_array_t <స్థిరరాశి hb_inc_bimap_t> &inner_maps,
		  స్థిరరాశి hb_సమితి_t *culled_regions = శూన్యసూచిక)
  {
    TRACE_SERIALIZE (ఇది);
#నిర్వచితమైతే HB_NO_VAR
    return_trace (అబద్ధం);
#అయితే_ముగింపు

    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    గుర్తులేని పూర్ణాంకం సమితి_లెక్క = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < inner_maps.నిడివి; i++)
      ఒకవేళ (inner_maps[i].get_population ())
	సమితి_లెక్క++;

    format = 1;

    స్థిరరాశి స్వయంచాలక &src_regions = src+src->regions;

    hb_సమితి_t region_indices;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < inner_maps.నిడివి; i++)
      (src+src->దత్తాంశం_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    ఒకవేళ (region_indices.లోపల_error ())
      return_trace (అబద్ధం);

    region_indices.del_range ((src_regions).region_లెక్క, hb_సమితి_t::INVALID);

    ఒకవేళ (culled_regions)
      region_indices.వ్యవకలనం (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (గుర్తులేని _) { region_map.కూడు(_); })
    ;
    ఒకవేళ (region_map.లోపల_error())
      return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (అబద్ధం);

    దత్తాంశం_Sets.len = సమితి_లెక్క;
    ఒకవేళ (unlikely (!c->extend (దత్తాంశం_Sets))) return_trace (అబద్ధం);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    గుర్తులేని పూర్ణాంకం సమితి_సూచీ = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < inner_maps.నిడివి; i++)
    {
      ఒకవేళ (!inner_maps[i].get_population ()) కొనసాగు;
      ఒకవేళ (unlikely (!దత్తాంశం_Sets[సమితి_సూచీ++]
		     .serialize_serialize (c, &(src+src->దత్తాంశం_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  అంశం_Variation_Store *copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (శూన్యసూచిక);

    hb_సదిశ_t <hb_inc_bimap_t> inner_maps;
    గుర్తులేని లెక్క = దత్తాంశం_Sets.len;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      ఒకవేళ (unlikely (!c->propagate_error(inner_maps)))
        return_trace(శూన్యసూచిక);
      స్వయంచాలక &దత్తాంశం = ఇది+దత్తాంశం_Sets[i];

      గుర్తులేని అంశం_లెక్క = దత్తాంశం.get_అంశం_లెక్క ();
      కొరకు (గుర్తులేని j = 0; j < అంశం_లెక్క; j++)
	map->కూడు (j);
    }

    ఒకవేళ (unlikely (!బహిర్గతం->serialize (c, ఇది, inner_maps))) return_trace (శూన్యసూచిక);

    return_trace (బహిర్గతం);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  తార్కిక collect_dead_regions (స్థిరరాశి hb_map_t &axes_old_సూచీ_ట్యాగ్_map,
			     స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> &reachable_ranges,
			     hb_సమితి_t &dead_regions /*బహిర్గతం*/) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (!reachable_ranges.get_population ()) తిరిగి నిజం;
    స్థిరరాశి VarRegionList &region_list = ఇది+regions;
    గుర్తులేని లెక్క = region_list.region_లెక్క;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      hb_hashmap_t<hb_ట్యాగ్_t, Triple> axis_tuples;
      ఒకవేళ (!region_list.get_var_region (i, axes_old_సూచీ_ట్యాగ్_map, axis_tuples))
	తిరిగి అబద్ధం;
      ఒకవేళ (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.కూడు (i);
    }
    తిరిగి !dead_regions.లోపల_error ();
#లేకపోతే
    తిరిగి నిజం;
#అయితే_ముగింపు
  }

  తార్కిక subset (hb_subset_context_t *c, స్థిరరాశి hb_array_t<స్థిరరాశి hb_inc_bimap_t> &inner_maps) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
#నిర్వచితమైతే HB_NO_VAR
    return_trace (అబద్ధం);
#అయితే_ముగింపు

    అంశం_Variation_Store *varstore_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed<అంశం_Variation_Store> ();
    ఒకవేళ (unlikely (!varstore_ప్రధానసంఖ్య)) return_trace (అబద్ధం);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_సమితి_t dead_regions;
    ఒకవేళ (c->plan->has_avar2)
      ఒకవేళ (!collect_dead_regions (c->plan->axes_old_సూచీ_ట్యాగ్_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (అబద్ధం);

    varstore_ప్రధానసంఖ్య->serialize (c->serializer, ఇది, inner_maps,
			       dead_regions.get_population () ? &dead_regions : శూన్యసూచిక);

    return_trace (
        !c->serializer->లోపల_error()
        && varstore_ప్రధానసంఖ్య->దత్తాంశం_Sets);
  }

  గుర్తులేని పూర్ణాంకం get_region_సూచీ_లెక్క (గుర్తులేని పూర్ణాంకం major) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి 0;
#అయితే_ముగింపు
    తిరిగి (ఇది+దత్తాంశం_Sets[major]).get_region_సూచీ_లెక్క ();
  }

  శూన్య get_region_scalars (గుర్తులేని పూర్ణాంకం major,
			   స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
			   దశాంశం *scalars /*OUT*/,
			   గుర్తులేని పూర్ణాంకం అంకె_scalars) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    కొరకు (గుర్తులేని i = 0; i < అంకె_scalars; i++)
      scalars[i] = 0.f;
    తిరిగి;
#అయితే_ముగింపు

    (ఇది+దత్తాంశం_Sets[major]).get_region_scalars (coords, coord_లెక్క,
					       ఇది+regions,
					       &scalars[0], అంకె_scalars);
  }

  గుర్తులేని పూర్ణాంకం get_sub_పట్టిక_లెక్క () స్థిరరాశి
   {
#నిర్వచితమైతే HB_NO_VAR
     తిరిగి 0;
#అయితే_ముగింపు
     తిరిగి దత్తాంశం_Sets.len;
   }

  స్థిరరాశి Var_దత్తాంశం& get_sub_పట్టిక (గుర్తులేని i) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
     తిరిగి Null (Var_దత్తాంశం);
#అయితే_ముగింపు
     తిరిగి ఇది+దత్తాంశం_Sets[i];
  }

  స్థిరరాశి VarRegionList& get_region_list () స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
     తిరిగి Null (VarRegionList);
#అయితే_ముగింపు
     తిరిగి ఇది+regions;
  }

  రక్షిత:
  HBUINT16				format;
  Offset32_గమ్యం<VarRegionList>		regions;
  Array16_యొక్క_Offset32_గమ్యం<Var_దత్తాంశం>		దత్తాంశం_Sets;
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (8, దత్తాంశం_Sets);
};

నిర్మాణం Multi_అంశం_Variation_Store
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = శూన్యసూచిక) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి hb_scalar_cache_t::create (0);
#అయితే_ముగింపు
    స్వయంచాలక &r = ఇది+regions;
    గుర్తులేని లెక్క = r.regions.len;

    తిరిగి hb_scalar_cache_t::create (లెక్క, static_cache);
  }

  స్థిర శూన్య destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = శూన్యసూచిక)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  అంతరంగ:
  శూన్య get_delta (గుర్తులేని పూర్ణాంకం outer, గుర్తులేని పూర్ణాంకం inner,
		  స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
		  hb_array_t<ద్విగుణ> బహిర్గతం,
		  hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి;
#అయితే_ముగింపు

    ఒకవేళ (unlikely (outer >= దత్తాంశం_Sets.len))
      తిరిగి;

    తిరిగి (ఇది+దత్తాంశం_Sets[outer]).get_delta (inner,
					     coords, coord_లెక్క,
					     ఇది+regions,
					     బహిర్గతం,
					     cache);
  }

  బహిరంగ:
  శూన్య get_delta (గుర్తులేని పూర్ణాంకం సూచీ_2,
		  స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
		  hb_array_t<ద్విగుణ> బహిర్గతం,
		  hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం outer = సూచీ_2 >> 16;
    గుర్తులేని పూర్ణాంకం inner = సూచీ_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_లెక్క, బహిర్గతం, cache);
  }
  శూన్య get_delta (గుర్తులేని పూర్ణాంకం సూచీ_2,
		  hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
		  hb_array_t<ద్విగుణ> బహిర్గతం,
		  hb_scalar_cache_t *cache = శూన్యసూచిక) స్థిరరాశి
  {
    తిరిగి get_delta (సూచీ_2,
		      coords.arrayZ, coords.నిడివి,
		      బహిర్గతం,
		      cache);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
#నిర్వచితమైతే HB_NO_VAR
    తిరిగి నిజం;
#అయితే_ముగింపు

    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ఇది) &&
		  దత్తాంశం_Sets.sanitize (c, ఇది));
  }

  తార్కిక create_subset_plan (స్థిరరాశి hb_సమితి_t &var_indices,
			   hb_సదిశ_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!inner_maps->resize (దత్తాంశం_Sets.len))) తిరిగి అబద్ధం;
    కొరకు (hb_codepoint_t var_idx : var_indices)
    {
      గుర్తులేని outer = var_idx >> 16;
      గుర్తులేని inner = var_idx & 0xFFFFu;
      ఒకవేళ (unlikely (outer >= దత్తాంశం_Sets.len ||
		    inner >= (ఇది+దత్తాంశం_Sets[outer]).get_అంశం_లెక్క ()))
	తిరిగి అబద్ధం;
      (*inner_maps)[outer].కూడు (inner);
    }

    గుర్తులేని new_outer = 0;
    కొరకు (గుర్తులేని outer = 0; outer < inner_maps->నిడివి; outer++)
    {
      స్థిరరాశి hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      ఒకవేళ (!inner_map.get_population ()) కొనసాగు;
      ఒకవేళ (unlikely (inner_map.లోపల_error ())) తిరిగి అబద్ధం;
      కొరకు (గుర్తులేని new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	గుర్తులేని old_inner = inner_map.backward (new_inner);
	varidx_map->సమితి ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    తిరిగి !inner_maps->లోపల_error () && !varidx_map->లోపల_error ();
  }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి Multi_అంశం_Variation_Store *src,
		  స్థిరరాశి hb_array_t<స్థిరరాశి hb_inc_bimap_t> &inner_maps,
		  స్థిరరాశి hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది) ||
		  inner_maps.నిడివి < src->దత్తాంశం_Sets.len))
      return_trace (అబద్ధం);
    format = 1;

    స్థిరరాశి స్వయంచాలక &src_regions = src+src->regions;
    hb_సమితి_t region_indices;
    గుర్తులేని సమితి_లెక్క = 0;
    కొరకు (గుర్తులేని i = 0; i < src->దత్తాంశం_Sets.len; i++)
    {
      ఒకవేళ (!inner_maps[i].get_population ()) కొనసాగు;
      సమితి_లెక్క++;
      (src+src->దత్తాంశం_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_సమితి_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.కూడు_సమితి (&region_indices);
    ఒకవేళ (unlikely (region_indices.లోపల_error () || region_map.లోపల_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (అబద్ధం);

    దత్తాంశం_Sets.len = సమితి_లెక్క;
    ఒకవేళ (unlikely (!c->extend (దత్తాంశం_Sets))) return_trace (అబద్ధం);
    గుర్తులేని సమితి_సూచీ = 0;
    కొరకు (గుర్తులేని i = 0; i < src->దత్తాంశం_Sets.len; i++)
    {
      ఒకవేళ (!inner_maps[i].get_population ()) కొనసాగు;
      ఒకవేళ (unlikely (!దత్తాంశం_Sets[సమితి_సూచీ++].serialize_serialize (
			    c, &(src+src->దత్తాంశం_Sets[i]), inner_maps[i], region_map)))
	return_trace (అబద్ధం);
    }
    return_trace (నిజం);
  }

  రక్షిత:
  HBUINT16				format; // 1
  Offset32_గమ్యం<SparseVarRegionList>	regions;
  Array16_యొక్క_Offset32_గమ్యం<Multi_Var_దత్తాంశం>	దత్తాంశం_Sets;
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (8, దత్తాంశం_Sets);
};

మూస <రకం_పేరు Map_లెక్క_T>
నిర్మాణం Delta_సమితి_సూచీ_Map_Format01
{
  మిత్ర నిర్మాణం Delta_సమితి_సూచీ_Map;

  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_mul_కూడు_saturate (map_లెక్క_2, get_వెడల్పు (), min_కొలత); }

  అంతరంగ:
  Delta_సమితి_సూచీ_Map_Format01* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed (ఇది));
  }

  మూస <రకం_పేరు T>
  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి T &plan)
  {
    గుర్తులేని పూర్ణాంకం వెడల్పు = plan.get_వెడల్పు ();
    గుర్తులేని పూర్ణాంకం inner_బిట్టు_లెక్క = plan.get_inner_బిట్టు_లెక్క ();
    స్థిరరాశి hb_array_t<స్థిరరాశి uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (output_map.నిడివి && ((((inner_బిట్టు_లెక్క-1)&~0xF)!=0) || (((వెడల్పు-1)&~0x3)!=0))))
      return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    నమోదు_Format = ((వెడల్పు-1)<<4)|(inner_బిట్టు_లెక్క-1);
    map_లెక్క_2 = output_map.నిడివి;
    HBUINT8 *p = c->allocate_కొలత<HBUINT8> (వెడల్పు * output_map.నిడివి);
    ఒకవేళ (unlikely (!p)) return_trace (అబద్ధం);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < output_map.నిడివి; i++)
    {
      గుర్తులేని పూర్ణాంకం v = output_map.arrayZ[i];
      ఒకవేళ (v)
      {
	గుర్తులేని పూర్ణాంకం outer = v >> 16;
	గుర్తులేని పూర్ణాంకం inner = v & 0xFFFF;
	గుర్తులేని పూర్ణాంకం u = (outer << inner_బిట్టు_లెక్క) | inner;
	కొరకు (గుర్తులేని పూర్ణాంకం w = వెడల్పు; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += వెడల్పు;
    }
    return_trace (నిజం);
  }

  HB_ALWAYS_INLINE
  uint32_t map (గుర్తులేని పూర్ణాంకం v) స్థిరరాశి /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    ఒకవేళ (!map_లెక్క_2)
      తిరిగి v;
    తిరిగి _map (v);
  }

  HB_HOT
  uint32_t _map (గుర్తులేని పూర్ణాంకం v) స్థిరరాశి /* Returns 16.16 outer.inner. */
  {
    ఒకవేళ (v >= map_లెక్క_2)
      v = map_లెక్క_2 - 1;

    గుర్తులేని పూర్ణాంకం u = 0;
    { /* Fetch it. */
      గుర్తులేని పూర్ణాంకం w = get_వెడల్పు ();
      స్థిరరాశి HBUINT8 *p = map_దత్తాంశం_Z.arrayZ + w * v;
      కొరకు (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      గుర్తులేని పూర్ణాంకం n = get_inner_బిట్టు_లెక్క ();
      గుర్తులేని పూర్ణాంకం outer = u >> n;
      గుర్తులేని పూర్ణాంకం inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    తిరిగి u;
  }

  గుర్తులేని get_map_లెక్క () స్థిరరాశి       { తిరిగి map_లెక్క_2; }
  గుర్తులేని get_వెడల్పు () స్థిరరాశి           { తిరిగి ((నమోదు_Format >> 4) & 3) + 1; }
  గుర్తులేని get_inner_బిట్టు_లెక్క () స్థిరరాశి { తిరిగి (నమోదు_Format & 0xF) + 1; }


  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
                  c->check_range (map_దత్తాంశం_Z.arrayZ,
                                  map_లెక్క_2,
                                  get_వెడల్పు ()));
  }

  రక్షిత:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       నమోదు_Format;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_లెక్క_T     map_లెక్క_2;       /* The number of mapping entries. */
  Unsized_Array_యొక్క<HBUINT8>
                map_దత్తాంశం_Z;       /* The delta-set index mapping data. */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (2+Map_లెక్క_T::static_కొలత, map_దత్తాంశం_Z);
};

నిర్మాణం Delta_సమితి_సూచీ_Map
{
  మూస <రకం_పేరు T>
  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి T &plan)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని నిడివి = plan.get_output_map ().నిడివి;
    u.format.v = నిడివి <= 0xFFFF ? 0 : 1;
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    స్థితి 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  uint32_t map (గుర్తులేని v) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); తిరిగి (u.format0.map (v));
    స్థితి 1: hb_barrier (); తిరిగి (u.format1.map (v));
    అప్రమేయ:తిరిగి v;
    }
  }

  గుర్తులేని get_map_లెక్క () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_map_లెక్క ();
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_map_లెక్క ();
    అప్రమేయ:తిరిగి 0;
    }
  }

  గుర్తులేని get_వెడల్పు () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_వెడల్పు ();
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_వెడల్పు ();
    అప్రమేయ:తిరిగి 0;
    }
  }

  గుర్తులేని get_inner_బిట్టు_లెక్క () స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_inner_బిట్టు_లెక్క ();
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_inner_బిట్టు_లెక్క ();
    అప్రమేయ:తిరిగి 0;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  Delta_సమితి_సూచీ_Map* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    ఎంపిక (u.format.v) {
    స్థితి 0: hb_barrier (); return_trace (reinterpret_cast<Delta_సమితి_సూచీ_Map *> (u.format0.copy (c)));
    స్థితి 1: hb_barrier (); return_trace (reinterpret_cast<Delta_సమితి_సూచీ_Map *> (u.format1.copy (c)));
    అప్రమేయ:return_trace (శూన్యసూచిక);
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT8 v; }             format;         /* Format identifier */
  Delta_సమితి_సూచీ_Map_Format01<HBUINT16> format0;
  Delta_సమితి_సూచీ_Map_Format01<HBUINT32> format1;
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (1, format.v);
};


నిర్మాణం అంశం_Var_Store_Instancer
{
  అంశం_Var_Store_Instancer (స్థిరరాశి అంశం_Variation_Store *varStore_,
			 స్థిరరాశి Delta_సమితి_సూచీ_Map *varIdxMap,
			 hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
			 hb_scalar_cache_t *cache = శూన్యసూచిక) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    ఒకవేళ (!varStore)
      varStore = &Null(అంశం_Variation_Store);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    ఒకవేళ (!hb_any (coords))
      ఇది->coords = {};
  }

  క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి varStore && తార్కిక (coords); }

  దశాంశం క్రియాకారకం[] (uint32_t varIdx) స్థిరరాశి
  { తిరిగి (*ఇది) (varIdx); }

  దశాంశం క్రియాకారకం() (uint32_t varIdx, గుర్తులేని పొట్టి స్థానభ్రంశం = 0) స్థిరరాశి
  { తిరిగి get_delta (varIdx, స్థానభ్రంశం); }

  ద్విగుణ get_condition_delta (uint32_t varIdx) స్థిరరాశి
  { తిరిగి get_delta<ద్విగుణ> (varIdx); }

  మూస <రకం_పేరు Float = దశాంశం>
  Float get_delta (uint32_t varIdx, గుర్తులేని పొట్టి స్థానభ్రంశం = 0) స్థిరరాశి
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   ఒకవేళ (!coords || varIdx == VarIdx::NO_VARIATION)
     తిరిగి Float (0);

    varIdx += స్థానభ్రంశం;
    ఒకవేళ (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    తిరిగి varStore->get_delta<Float> (varIdx, coords, cache);
  }

  స్థిరరాశి అంశం_Variation_Store *varStore;
  స్థిరరాశి Delta_సమితి_సూచీ_Map *varIdxMap;
  hb_array_t<స్థిరరాశి పూర్ణాంకం> coords;
  hb_scalar_cache_t *cache;
};

నిర్మాణం Multi_అంశం_Var_Store_Instancer
{
  Multi_అంశం_Var_Store_Instancer (స్థిరరాశి Multi_అంశం_Variation_Store *varStore,
			      స్థిరరాశి Delta_సమితి_సూచీ_Map *varIdxMap,
			      hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
			      hb_scalar_cache_t *cache = శూన్యసూచిక) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    ఒకవేళ (!varStore)
      varStore = &Null(Multi_అంశం_Variation_Store);
  }

  క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి varStore; }

  ద్విగుణ get_condition_delta (uint32_t varIdx) స్థిరరాశి
  { తిరిగి (ద్విగుణ) (*ఇది)[varIdx]; }

  ద్విగుణ క్రియాకారకం[] (uint32_t varIdx) స్థిరరాశి
  {
    ద్విగుణ v = 0;
    (*ఇది) (hb_array (&v, 1), varIdx);
    తిరిగి v;
  }

  శూన్య క్రియాకారకం() (hb_array_t<ద్విగుణ> బహిర్గతం, uint32_t varIdx, గుర్తులేని పొట్టి స్థానభ్రంశం = 0) స్థిరరాశి
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    ఒకవేళ (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += స్థానభ్రంశం;
      ఒకవేళ (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, బహిర్గతం, cache);
    }
    లేకపోతే
      కొరకు (గుర్తులేని i = 0; i < బహిర్గతం.నిడివి; i++)
        బహిర్గతం.arrayZ[i] = 0.;
  }

  స్థిరరాశి Multi_అంశం_Variation_Store *varStore;
  స్థిరరాశి Delta_సమితి_సూచీ_Map *varIdxMap;
  hb_array_t<స్థిరరాశి పూర్ణాంకం> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
గణన Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_రికార్డు_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_రికార్డు_WITH_VAR = 3,
};

గణన lookup_condition_subset_ఫలితం_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

నిర్మాణం Condition;

మూస <రకం_పేరు Instancer>
స్థిర తార్కిక
_hb_recurse_condition_evaluate (స్థిరరాశి నిర్మాణం Condition &condition,
				స్థిరరాశి పూర్ణాంకం *coords,
				గుర్తులేని పూర్ణాంకం coord_len,
				Instancer *instancer);

నిర్మాణం ConditionAxisRange
{
  మిత్ర నిర్మాణం Condition;

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    స్థిరరాశి hb_map_t *సూచీ_map = &c->plan->axes_సూచీ_map;
    ఒకవేళ (సూచీ_map->is_empty ()) return_trace (నిజం);

    స్థిరరాశి hb_map_t& axes_old_సూచీ_ట్యాగ్_map = c->plan->axes_old_సూచీ_ట్యాగ్_map;
    hb_codepoint_t *axis_ట్యాగ్_2;
    ఒకవేళ (!axes_old_సూచీ_ట్యాగ్_map.has (axis_సూచీ_2, &axis_ట్యాగ్_2) ||
        !సూచీ_map->has (axis_సూచీ_2))
      return_trace (అబద్ధం);

    స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_హద్దు{-1.0, 0.0, 1.0};
    Triple *normalized_హద్దు;
    ఒకవేళ (normalized_axes_location.has (*axis_ట్యాగ్_2, &normalized_హద్దు))
      axis_హద్దు = *normalized_హద్దు;

    స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    ఒకవేళ (axes_triple_distances.has (*axis_ట్యాగ్_2, &triple_dists))
      axis_triple_distances = *triple_dists;

    దశాంశం normalized_min = renormalize_విలువ ((ద్విగుణ) filter_Range_Min_విలువ.గమ్యం_float (), axis_హద్దు, axis_triple_distances, అబద్ధం);
    దశాంశం normalized_max = renormalize_విలువ ((ద్విగుణ) filter_Range_Max_విలువ.గమ్యం_float (), axis_హద్దు, axis_triple_distances, అబద్ధం);
    బహిర్గతం->filter_Range_Min_విలువ.సమితి_float (normalized_min);
    బహిర్గతం->filter_Range_Max_విలువ.సమితి_float (normalized_max);

    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->axis_సూచీ_2, సూచీ_map->get (axis_సూచీ_2),
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  అంతరంగ:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*బహిర్గతం*/) స్థిరరాశి
  {
    //invalid axis index, drop the entire record
    ఒకవేళ (!c->axes_సూచీ_ట్యాగ్_map->has (axis_సూచీ_2))
      తిరిగి DROP_రికార్డు_WITH_VAR;

    hb_ట్యాగ్_t axis_ట్యాగ్_2 = c->axes_సూచీ_ట్యాగ్_map->get (axis_సూచీ_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_హద్దు;
    తార్కిక axis_సమితి_by_user = అబద్ధం;
    ఒకవేళ (c->axes_location->has (axis_ట్యాగ్_2, &axis_హద్దు))
    {
      axis_range = *axis_హద్దు;
      axis_సమితి_by_user = నిజం;
    }

    దశాంశం axis_min_val = axis_range.minimum;
    దశాంశం axis_default_val = axis_range.మధ్య;
    దశాంశం axis_max_val = axis_range.గరిష్ఠ;

    దశాంశం filter_min_val = filter_Range_Min_విలువ.గమ్యం_float ();
    దశాంశం filter_max_val = filter_Range_Max_విలువ.గమ్యం_float ();

    ఒకవేళ (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = అబద్ధం;

    //condition not met, drop the entire record
    ఒకవేళ (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      తిరిగి DROP_రికార్డు_WITH_VAR;

    //condition met and axis pinned, drop the condition
    ఒకవేళ (axis_సమితి_by_user && axis_range.is_బిందువు ())
      తిరిగి DROP_COND_WITH_VAR;

    ఒకవేళ (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_విలువ.గమ్యం_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_విలువ.గమ్యం_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->సమితి (axis_సూచీ_2, val);
      తిరిగి KEEP_COND_WITH_VAR;
    }
    తిరిగి KEEP_రికార్డు_WITH_VAR;
  }

  మూస <రకం_పేరు Instancer>
  తార్కిక evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		 Instancer *instancer HB_వాడని) స్థిరరాశి
  {
    పూర్ణాంకం coord = axis_సూచీ_2 < coord_len ? coords[axis_సూచీ_2] : 0;
    తిరిగి filter_Range_Min_విలువ.గమ్యం_int () <= coord && coord <= filter_Range_Max_విలువ.గమ్యం_int ();
  }

  బహిరంగ:
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axis_సూచీ_2;
  F2DOT14	filter_Range_Min_విలువ;
  F2DOT14	filter_Range_Max_విలువ;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం Condition_విలువ
{
  మిత్ర నిర్మాణం Condition;

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (varIdx == VarIdx::NO_VARIATION)
      return_trace (నిజం);

    hb_pair_t<గుర్తులేని, పూర్ణాంకం> *condition_విలువ;
    ఒకవేళ (c->plan->layout_condition_idx_విలువ_map.has ((uintptr_t) ఇది, &condition_విలువ))
    {
      బహిర్గతం->default_విలువ_2 = condition_విలువ->second;
      బహిర్గతం->varIdx = condition_విలువ->మొదటి;
      return_trace (నిజం);
    }

    hb_pair_t<గుర్తులేని, పూర్ణాంకం> *new_varidx_delta;
    ఒకవేళ (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (
		బహిర్గతం->default_విలువ_2,
		default_విలువ_2 + hb_రెండవ (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
      return_trace (అబద్ధం);
    బహిర్గతం->varIdx = hb_మొదటి (*new_varidx_delta);
    return_trace (నిజం);
  }

  అంతరంగ:
  మూస <రకం_పేరు Instancer>
  తార్కిక evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		 Instancer *instancer) స్థిరరాశి
  {
    ద్విగుణ value = default_విలువ_2;
    value += instancer->get_condition_delta (varIdx);
    తిరిగి value > 0;
  }

  బహిరంగ:
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_విలువ_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం ConditionAnd
{
  మిత్ర నిర్మాణం Condition;

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం)))
      return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : conditions)
    {
      ఒకవేళ (స్థానభ్రంశం.is_null ())
      {
	స్వయంచాలక *బహిర్గతం_స్థానభ్రంశం = బహిర్గతం->conditions.serialize_append (c->serializer);
	ఒకవేళ (unlikely (!బహిర్గతం_స్థానభ్రంశం)) return_trace (అబద్ధం);
	*బహిర్గతం_స్థానభ్రంశం = 0;
      }
      లేకపోతే ఒకవేళ (unlikely (!subset_స్థానభ్రంశం_array (c, బహిర్గతం->conditions, ఇది) (స్థానభ్రంశం)))
	return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  అంతరంగ:
  మూస <రకం_పేరు Instancer>
  తార్కిక evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		 Instancer *instancer) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = conditions.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!_hb_recurse_condition_evaluate (ఇది+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  బహిరంగ:
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (conditions.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8_యొక్క_Offset24_గమ్యం<నిర్మాణం Condition>	conditions;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (3, conditions);
};

నిర్మాణం ConditionOr
{
  మిత్ర నిర్మాణం Condition;

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం)))
      return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : conditions)
    {
      ఒకవేళ (స్థానభ్రంశం.is_null ())
      {
	స్వయంచాలక *బహిర్గతం_స్థానభ్రంశం = బహిర్గతం->conditions.serialize_append (c->serializer);
	ఒకవేళ (unlikely (!బహిర్గతం_స్థానభ్రంశం)) return_trace (అబద్ధం);
	*బహిర్గతం_స్థానభ్రంశం = 0;
      }
      లేకపోతే ఒకవేళ (unlikely (!subset_స్థానభ్రంశం_array (c, బహిర్గతం->conditions, ఇది) (స్థానభ్రంశం)))
	return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  అంతరంగ:
  మూస <రకం_పేరు Instancer>
  తార్కిక evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		 Instancer *instancer) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = conditions.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (_hb_recurse_condition_evaluate (ఇది+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }

  బహిరంగ:
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (conditions.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8_యొక్క_Offset24_గమ్యం<నిర్మాణం Condition>	conditions;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (3, conditions);
};

నిర్మాణం ConditionNegate
{
  మిత్ర నిర్మాణం Condition;

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    ఒకవేళ (condition.is_null ())
    {
      బహిర్గతం->condition = 0;
      return_trace (నిజం);
    }
    return_trace (బహిర్గతం->condition.serialize_subset (c, condition, ఇది));
  }

  అంతరంగ:
  మూస <రకం_పేరు Instancer>
  తార్కిక evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		 Instancer *instancer) స్థిరరాశి
  {
    తిరిగి !_hb_recurse_condition_evaluate (ఇది+condition,
					    coords, coord_len,
					    instancer);
  }

  బహిరంగ:
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (condition.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24_గమ్యం<నిర్మాణం Condition>	condition;
  బహిరంగ:
  DEFINE_కొలత_STATIC (5);
};

నిర్మాణం Condition
{
  మిత్ర నిర్మాణం Condition_సమితి;

  స్థిర తార్కిక serialize_constant (hb_serialize_context_t *c, తార్కిక value)
  {
    స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed<Condition_విలువ> ();
    ఒకవేళ (unlikely (!బహిర్గతం || !c->extend_min (బహిర్గతం))) తిరిగి అబద్ధం;
    బహిర్గతం->format = 2;
    బహిర్గతం->default_విలువ_2 = value ? 1 : 0;
    బహిర్గతం->varIdx = VarIdx::NO_VARIATION;
    తిరిగి నిజం;
  }

  lookup_condition_subset_ఫలితం_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      గుర్తులేని లోతు = HB_MAX_NESTING_LEVEL,
				      తార్కిక materialize_true = అబద్ధం) స్థిరరాశి;

  మూస <రకం_పేరు Instancer>
  తార్కిక evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		 Instancer *instancer) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    స్థితి 0: తిరిగి నిజం;
    స్థితి 1: hb_barrier (); తిరిగి u.format1.evaluate (coords, coord_len, instancer);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.evaluate (coords, coord_len, instancer);
    స్థితి 3: hb_barrier (); తిరిగి u.format3.evaluate (coords, coord_len, instancer);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.evaluate (coords, coord_len, instancer);
    స్థితి 5: hb_barrier (); తిరిగి u.format5.evaluate (coords, coord_len, instancer);
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*బహిర్గతం*/,
                                             గుర్తులేని లోతు = HB_MAX_NESTING_LEVEL) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!లోతు))
    {
      c->apply = అబద్ధం;
      తిరిగి DROP_రికార్డు_WITH_VAR;
    }

    ఎంపిక (u.format.v) {
    స్థితి 0: తిరిగి KEEP_రికార్డు_WITH_VAR;
    స్థితి 1: hb_barrier (); తిరిగి u.format1.keep_with_variations (c, condition_map);
    స్థితి 2: స్థితి 3: స్థితి 4: స్థితి 5: ఆపు;
    అప్రమేయ: c->apply = అబద్ధం; తిరిగి DROP_రికార్డు_WITH_VAR;
    }

    hb_barrier ();
    తార్కిక applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.నిడివి, c->instancer);
    c->apply &= applies;
    ఒకవేళ (c->all_axes_pinned)
      తిరిగి applies ? DROP_COND_WITH_VAR : DROP_రికార్డు_WITH_VAR;

    ఒకవేళ (u.format.v == 2)
      తిరిగి u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_సమితి (u.format2.varIdx) ?
             (applies ? KEEP_రికార్డు_WITH_VAR : DROP_రికార్డు_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    స్వయంచాలక classify_బిడ్డ = [c, condition_map, లోతు] (స్థిరరాశి Condition &బిడ్డ)
    {
      స్వయంచాలక బిడ్డ_context = *c;
      బిడ్డ_context.apply = నిజం;
      తిరిగి బిడ్డ.keep_with_variations (&బిడ్డ_context, condition_map, లోతు - 1);
    };

    ఒకవేళ (u.format.v == 5)
    {
      స్వయంచాలక ఫలితం = classify_బిడ్డ (&u.format5 + u.format5.condition);
      తిరిగి ఫలితం == DROP_రికార్డు_WITH_VAR ? KEEP_రికార్డు_WITH_VAR :
             ఫలితం == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_రికార్డు_WITH_VAR;
    }

    తార్కిక is_and = u.format.v == 3;
    తార్కిక variable = అబద్ధం;
    స్థిరరాశి స్వయంచాలక &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    స్థిరరాశి శూన్య *పునాది = is_and ? (స్థిరరాశి శూన్య *) &u.format3 : (స్థిరరాశి శూన్య *) &u.format4;
    కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : conditions)
    {
      స్వయంచాలక ఫలితం = classify_బిడ్డ (పునాది + స్థానభ్రంశం);
      ఒకవేళ (is_and && ఫలితం == DROP_రికార్డు_WITH_VAR)
        తిరిగి DROP_రికార్డు_WITH_VAR;
      ఒకవేళ (!is_and && (ఫలితం == DROP_COND_WITH_VAR || ఫలితం == KEEP_రికార్డు_WITH_VAR))
        తిరిగి KEEP_రికార్డు_WITH_VAR;
      variable |= ఫలితం == KEEP_COND_WITH_VAR;
    }
    తిరిగి variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_రికార్డు_WITH_VAR : DROP_రికార్డు_WITH_VAR;
  }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, &u.format.v))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, u.format.v);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    స్థితి 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    స్థితి 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    స్థితి 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    స్థితి 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    ఒకవేళ (unlikely (!c->check_ప్రారంభం_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_విలువ ());

    return_trace (c->చివర_recursion (ఇది->dispatch (c)));
  }

  తార్కిక collect_var_indices (hb_సమితి_t *var_indices,
			    గుర్తులేని లోతు = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *condition_విలువలు = శూన్యసూచిక) స్థిరరాశి;

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి Condition *src,
		  స్థిరరాశి hb_map_t &varidx_map,
		  స్థిరరాశి hb_map_t *axis_map = శూన్యసూచిక);

  అంతరంగ:
  lookup_condition_subset_ఫలితం_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      గుర్తులేని లోతు) స్థిరరాశి;

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_విలువ	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);
};

మూస <రకం_పేరు Instancer>
తార్కిక
_hb_recurse_condition_evaluate (స్థిరరాశి నిర్మాణం Condition &condition,
				స్థిరరాశి పూర్ణాంకం *coords,
				గుర్తులేని పూర్ణాంకం coord_len,
				Instancer *instancer)
{
  తిరిగి condition.evaluate (coords, coord_len, instancer);
}

నిర్మాణం ConditionList
{
  స్థిరరాశి Condition& క్రియాకారకం[] (గుర్తులేని i) స్థిరరాశి
  { తిరిగి ఇది+conditions[i]; }

  గుర్తులేని get_లెక్క () స్థిరరాశి { తిరిగి conditions.len; }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి ConditionList *src,
		  స్థిరరాశి hb_inc_bimap_t &condition_map,
		  స్థిరరాశి hb_map_t &varidx_map,
		  స్థిరరాశి hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    conditions.len = condition_map.get_population ();
    ఒకవేళ (unlikely (!c->extend (conditions))) return_trace (అబద్ధం);
    కొరకు (గుర్తులేని i = 0; i < conditions.len; i++)
    {
      గుర్తులేని old_సూచీ = condition_map.backward (i);
      ఒకవేళ (unlikely (old_సూచీ >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_సూచీ]), varidx_map, axis_map)))
	return_trace (అబద్ధం);
    }
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (conditions.sanitize (c, ఇది));
  }

  రక్షిత:
  Array32_యొక్క_Offset32_గమ్యం<Condition> conditions;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, conditions);
};

అంతఃవరుస తార్కిక
Condition::collect_var_indices (hb_సమితి_t *var_indices, గుర్తులేని లోతు,
                               hb_hashmap_t<uintptr_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *condition_విలువలు) స్థిరరాశి
{
  ఒకవేళ (unlikely (!లోతు)) తిరిగి అబద్ధం;
  ఎంపిక (u.format.v)
  {
    స్థితి 0:
    స్థితి 1:
      తిరిగి నిజం;
    స్థితి 2:
      ఒకవేళ (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->కూడు (u.format2.varIdx);
        ఒకవేళ (condition_విలువలు &&
            !condition_విలువలు->సమితి ((uintptr_t) &u.format2,
                                    hb_pair (గుర్తులేని (u.format2.varIdx), పూర్ణాంకం (u.format2.default_విలువ_2))))
          తిరిగి అబద్ధం;
      }
      తిరిగి !var_indices->లోపల_error ();
    స్థితి 3:
      కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : u.format3.conditions)
	ఒకవేళ (unlikely (!(&u.format3+స్థానభ్రంశం).collect_var_indices (var_indices,
							     లోతు - 1, condition_విలువలు)))
	  తిరిగి అబద్ధం;
      తిరిగి నిజం;
    స్థితి 4:
      కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : u.format4.conditions)
	ఒకవేళ (unlikely (!(&u.format4+స్థానభ్రంశం).collect_var_indices (var_indices,
							     లోతు - 1, condition_విలువలు)))
	  తిరిగి అబద్ధం;
      తిరిగి నిజం;
    స్థితి 5:
      తిరిగి (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  లోతు - 1, condition_విలువలు);
    అప్రమేయ:
      తిరిగి అబద్ధం;
  }
}

అంతఃవరుస తార్కిక
Condition::serialize (hb_serialize_context_t *c,
		      స్థిరరాశి Condition *src,
		      స్థిరరాశి hb_map_t &varidx_map,
		      స్థిరరాశి hb_map_t *axis_map)
{
  TRACE_SERIALIZE (ఇది);
  ఎంపిక (src->u.format.v)
  {
    స్థితి 0:
    {
      స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed<Condition_విలువ> ();
      ఒకవేళ (unlikely (!బహిర్గతం || !c->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
      బహిర్గతం->format = 2;
      బహిర్గతం->default_విలువ_2 = 1;
      బహిర్గతం->varIdx = VarIdx::NO_VARIATION;
      return_trace (నిజం);
    }
    స్థితి 1:
    {
      స్వయంచాలక *బహిర్గతం = c->embed (&src->u.format1);
      ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
      ఒకవేళ (axis_map)
      {
	ఒకవేళ (unlikely (!axis_map->has (బహిర్గతం->axis_సూచీ_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (అబద్ధం);
	}
	ఒకవేళ (unlikely (!c->check_కేటాయించు (బహిర్గతం->axis_సూచీ_2, axis_map->get (బహిర్గతం->axis_సూచీ_2),
				      HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
	  return_trace (అబద్ధం);
      }
      return_trace (నిజం);
    }
    స్థితి 2:
    {
      స్వయంచాలక *బహిర్గతం = c->embed (&src->u.format2);
      ఒకవేళ (unlikely (!బహిర్గతం ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (అబద్ధం);
      బహిర్గతం->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (నిజం);
    }
    స్థితి 3:
    {
      స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed<ConditionAnd> ();
      ఒకవేళ (unlikely (!c->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
      బహిర్గతం->format = 3;
      బహిర్గతం->conditions.len = src->u.format3.conditions.len;
      ఒకవేళ (unlikely (!c->extend (బహిర్గతం->conditions))) return_trace (అబద్ధం);
      కొరకు (గుర్తులేని i = 0; i < బహిర్గతం->conditions.len; i++)
	ఒకవేళ (unlikely (!బహిర్గతం->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (అబద్ధం);
      return_trace (నిజం);
    }
    స్థితి 4:
    {
      స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed<ConditionOr> ();
      ఒకవేళ (unlikely (!c->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
      బహిర్గతం->format = 4;
      బహిర్గతం->conditions.len = src->u.format4.conditions.len;
      ఒకవేళ (unlikely (!c->extend (బహిర్గతం->conditions))) return_trace (అబద్ధం);
      కొరకు (గుర్తులేని i = 0; i < బహిర్గతం->conditions.len; i++)
	ఒకవేళ (unlikely (!బహిర్గతం->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (అబద్ధం);
      return_trace (నిజం);
    }
    స్థితి 5:
    {
      స్వయంచాలక *బహిర్గతం = c->embed (&src->u.format5);
      ఒకవేళ (unlikely (!బహిర్గతం ||
		    !బహిర్గతం->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (అబద్ధం);
      return_trace (నిజం);
    }
    అప్రమేయ:
      return_trace (అబద్ధం);
  }
}

అంతఃవరుస lookup_condition_subset_ఫలితం_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    గుర్తులేని లోతు,
				    తార్కిక materialize_true) స్థిరరాశి
{
  స్వయంచాలక snap = c->serializer->snapshot ();
  lookup_condition_subset_ఫలితం_t ఫలితం =
      subset_lookup_condition_impl (c, లోతు);
  ఒకవేళ (ఫలితం == LOOKUP_CONDITION_SUBSET_KEEP)
    తిరిగి ఫలితం;

  c->serializer->revert (snap);
  ఒకవేళ (ఫలితం != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    తిరిగి ఫలితం;

  తిరిగి serialize_constant (c->serializer, నిజం) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

అంతఃవరుస lookup_condition_subset_ఫలితం_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 గుర్తులేని లోతు) స్థిరరాశి
{
  ఒకవేళ (unlikely (!లోతు))
    తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;

  స్వయంచాలక subset_స్థానభ్రంశం = [c, లోతు] (Array8_యొక్క_Offset24_గమ్యం<Condition> &బహిర్గతం,
				   స్థిరరాశి Condition &condition)
      -> lookup_condition_subset_ఫలితం_t
  {
    స్వయంచాలక snap = c->serializer->snapshot ();
    స్వయంచాలక *స్థానభ్రంశం = బహిర్గతం.serialize_append (c->serializer);
    ఒకవేళ (unlikely (!స్థానభ్రంశం))
      తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_ఫలితం_t ఫలితం =
        condition.subset_lookup_condition (c, లోతు - 1);
    ఒకవేళ (ఫలితం == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->కూడు_link (*స్థానభ్రంశం, c->serializer->తీయి_pack ());
    లేకపోతే
    {
      c->serializer->తీయి_discard ();
      బహిర్గతం.తీయి ();
      c->serializer->revert (snap);
    }
    తిరిగి ఫలితం;
  };

  ఎంపిక (u.format.v)
  {
    స్థితి 0:
      తిరిగి LOOKUP_CONDITION_SUBSET_TRUE;

    స్థితి 1:
    {
      స్థిరరాశి ConditionAxisRange &src = u.format1;
      ఒకవేళ (c->plan->user_axes_location.is_empty ())
        తిరిగి c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_ట్యాగ్_2;
      ఒకవేళ (!c->plan->axes_old_సూచీ_ట్యాగ్_map.has (src.axis_సూచీ_2, &axis_ట్యాగ్_2))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_హద్దు {-1.f, 0.f, 1.f};
      Triple *normalized_హద్దు;
      ఒకవేళ (c->plan->axes_location.has (*axis_ట్యాగ్_2, &normalized_హద్దు))
        axis_హద్దు = *normalized_హద్దు;

      దశాంశం filter_min = src.filter_Range_Min_విలువ.గమ్యం_float ();
      దశాంశం filter_max = src.filter_Range_Max_విలువ.గమ్యం_float ();
      ఒకవేళ (filter_min > filter_max)
        తిరిగి LOOKUP_CONDITION_SUBSET_FALSE;
      ఒకవేళ (axis_హద్దు.is_బిందువు ())
        తిరిగి filter_min <= axis_హద్దు.మధ్య &&
	       axis_హద్దు.మధ్య <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      ఒకవేళ (axis_హద్దు.గరిష్ఠ < filter_min ||
	  filter_max < axis_హద్దు.minimum)
        తిరిగి LOOKUP_CONDITION_SUBSET_FALSE;
      ఒకవేళ (filter_min <= axis_హద్దు.minimum &&
	  axis_హద్దు.గరిష్ఠ <= filter_max)
        తిరిగి LOOKUP_CONDITION_SUBSET_TRUE;
      ఒకవేళ (!c->plan->axes_సూచీ_map.has (src.axis_సూచీ_2))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;

      స్వయంచాలక *బహిర్గతం = c->serializer->embed (&src);
      ఒకవేళ (unlikely (!బహిర్గతం))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      ఒకవేళ (c->plan->axes_triple_distances.has (*axis_ట్యాగ్_2, &distances))
        axis_distances = *distances;

      బహిర్గతం->filter_Range_Min_విలువ.సమితి_float (
	  renormalize_విలువ (filter_min, axis_హద్దు, axis_distances, అబద్ధం));
      బహిర్గతం->filter_Range_Max_విలువ.సమితి_float (
	  renormalize_విలువ (filter_max, axis_హద్దు, axis_distances, అబద్ధం));
      ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (
		బహిర్గతం->axis_సూచీ_2,
		c->plan->axes_సూచీ_map.get (src.axis_సూచీ_2),
		HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;
      తిరిగి LOOKUP_CONDITION_SUBSET_KEEP;
    }

    స్థితి 2:
    {
      స్థిరరాశి Condition_విలువ &src = u.format2;
      స్వయంచాలక *బహిర్గతం = c->serializer->embed (&src);
      ఒకవేళ (unlikely (!బహిర్గతం))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;

      ఒకవేళ (src.varIdx == VarIdx::NO_VARIATION)
        తిరిగి c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((పూర్ణాంకం) src.default_విలువ_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<గుర్తులేని, పూర్ణాంకం> *condition_విలువ;
      ఒకవేళ (c->plan->layout_condition_idx_విలువ_map.has ((uintptr_t) &src, &condition_విలువ))
      {
        బహిర్గతం->default_విలువ_2 = condition_విలువ->second;
        బహిర్గతం->varIdx = condition_విలువ->మొదటి;
        ఒకవేళ (బహిర్గతం->varIdx == VarIdx::NO_VARIATION)
          తిరిగి బహిర్గతం->default_విలువ_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        తిరిగి LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<గుర్తులేని, పూర్ణాంకం> *new_varidx_delta;
      ఒకవేళ (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        తిరిగి c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((పూర్ణాంకం) src.default_విలువ_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      ఒకవేళ (!c->plan->user_axes_location.is_empty () &&
          hb_మొదటి (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        ద్విగుణ value = (పూర్ణాంకం) src.default_విలువ_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        తిరిగి value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      పూర్ణాంకం default_విలువ = (పూర్ణాంకం) src.default_విలువ_2 +
			  hb_రెండవ (*new_varidx_delta);
      ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (
		బహిర్గతం->default_విలువ_2,
		default_విలువ,
		HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;
      బహిర్గతం->varIdx = hb_మొదటి (*new_varidx_delta);

      తిరిగి LOOKUP_CONDITION_SUBSET_KEEP;
    }

    స్థితి 3:
    {
      స్థిరరాశి ConditionAnd &src = u.format3;
      స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (&src);
      ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం)))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;
      బహిర్గతం->format = 3;

      కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : src.conditions)
      {
        lookup_condition_subset_ఫలితం_t ఫలితం =
            subset_స్థానభ్రంశం (బహిర్గతం->conditions, &src + స్థానభ్రంశం);
        ఒకవేళ (ఫలితం == LOOKUP_CONDITION_SUBSET_ERROR ||
	    ఫలితం == LOOKUP_CONDITION_SUBSET_FALSE)
          తిరిగి ఫలితం;
      }
      తిరిగి బహిర్గతం->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    స్థితి 4:
    {
      స్థిరరాశి ConditionOr &src = u.format4;
      స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (&src);
      ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం)))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;
      బహిర్గతం->format = 4;

      కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : src.conditions)
      {
        lookup_condition_subset_ఫలితం_t ఫలితం =
            subset_స్థానభ్రంశం (బహిర్గతం->conditions, &src + స్థానభ్రంశం);
        ఒకవేళ (ఫలితం == LOOKUP_CONDITION_SUBSET_ERROR ||
	    ఫలితం == LOOKUP_CONDITION_SUBSET_TRUE)
          తిరిగి ఫలితం;
      }
      తిరిగి బహిర్గతం->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    స్థితి 5:
    {
      స్థిరరాశి ConditionNegate &src = u.format5;
      స్వయంచాలక *బహిర్గతం = c->serializer->embed (&src);
      ఒకవేళ (unlikely (!బహిర్గతం))
        తిరిగి LOOKUP_CONDITION_SUBSET_ERROR;
      బహిర్గతం->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_ఫలితం_t ఫలితం =
          (&src + src.condition).subset_lookup_condition (c, లోతు - 1);
      ఒకవేళ (ఫలితం == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->కూడు_link (బహిర్గతం->condition,
				 c->serializer->తీయి_pack ());
        తిరిగి ఫలితం;
      }

      c->serializer->తీయి_discard ();
      ఒకవేళ (ఫలితం == LOOKUP_CONDITION_SUBSET_TRUE)
        తిరిగి LOOKUP_CONDITION_SUBSET_FALSE;
      ఒకవేళ (ఫలితం == LOOKUP_CONDITION_SUBSET_FALSE)
        తిరిగి LOOKUP_CONDITION_SUBSET_TRUE;
      తిరిగి ఫలితం;
    }

    అప్రమేయ:
      తిరిగి LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

నిర్మాణం Condition_సమితి
{
  తార్కిక evaluate (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		 అంశం_Var_Store_Instancer *instancer) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = conditions.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!(ఇది+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  శూన్య keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) స్థిరరాశి
  {
    hb_map_t *condition_map = hb_map_create ();
    ఒకవేళ (unlikely (!condition_map)) తిరిగి;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_సమితి_t *cond_సమితి = hb_సమితి_create ();
    ఒకవేళ (unlikely (!cond_సమితి)) తిరిగి;
    hb::shared_ptr<hb_సమితి_t> s {cond_సమితి};

    c->apply = నిజం;
    తార్కిక should_keep = అబద్ధం;
    /* The range map cannot identify value or boolean expressions uniquely. */
    తార్కిక can_deduplicate = నిజం;
    గుర్తులేని అంకె_kept_cond = 0, cond_idx = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& స్థానభ్రంశం : conditions)
    {
      స్థిరరాశి Condition &condition = ఇది+స్థానభ్రంశం;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      ఒకవేళ (ret == DROP_రికార్డు_WITH_VAR)
      {
        c->apply = అబద్ధం;
        తిరిగి;
      }

      ఒకవేళ (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = నిజం;
        cond_సమితి->కూడు (cond_idx);
        అంకె_kept_cond++;
      }

      ఒకవేళ (ret == KEEP_రికార్డు_WITH_VAR)
        should_keep = నిజం;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = అంకె_kept_cond == 0;
    ఒకవేళ (!should_keep && c->all_axes_pinned) తిరిగి;

    //check if condition_set is unique with variations
    ఒకవేళ (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      తిరిగి;

    ఒకవేళ (can_deduplicate)
      c->conditionset_map->సమితి (p, 1);
    c->రికార్డు_cond_idx_map->సమితి (c->cur_రికార్డు_idx, s);
  }

  తార్కిక subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               తార్కిక చొప్పించు_catch_all) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    ఒకవేళ (చొప్పించు_catch_all) return_trace (నిజం);

    hb_సమితి_t *retained_cond_సమితి = శూన్యసూచిక;
    ఒకవేళ (l->feature_రికార్డు_cond_idx_map != శూన్యసూచిక)
      retained_cond_సమితి = l->feature_రికార్డు_cond_idx_map->get (l->cur_feature_var_రికార్డు_idx);

    గుర్తులేని పూర్ణాంకం లెక్క = conditions.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (retained_cond_సమితి != శూన్యసూచిక && !retained_cond_సమితి->has (i))
        కొనసాగు;
      స్థిరరాశి Condition &condition = ఇది+conditions[i];
      ఒకవేళ (condition.u.format.v == 1)
      {
        subset_స్థానభ్రంశం_array (c, బహిర్గతం->conditions, ఇది) (conditions[i]);
        కొనసాగు;
      }

      స్వయంచాలక *స్థానభ్రంశం = బహిర్గతం->conditions.serialize_append (c->serializer);
      ఒకవేళ (unlikely (!స్థానభ్రంశం)) return_trace (అబద్ధం);
      c->serializer->push ();
      స్వయంచాలక ఫలితం = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, నిజం);
      ఒకవేళ (ఫలితం == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        ఫలితం = Condition::serialize_constant (c->serializer, అబద్ధం) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      ఒకవేళ (unlikely (ఫలితం == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->తీయి_discard ();
        c->serializer->check_success (అబద్ధం);
        return_trace (అబద్ధం);
      }
      c->serializer->కూడు_link (*స్థానభ్రంశం, c->serializer->తీయి_pack ());
    }

    return_trace (తార్కిక (బహిర్గతం->conditions));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (conditions.sanitize (c, ఇది));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : conditions)
      ఒకవేళ (unlikely (!(ఇది+స్థానభ్రంశం).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_విలువలు)))
	c->layout_variation_indices->err ();
  }

  రక్షిత:
  Array16_యొక్క_Offset32_గమ్యం<Condition>	conditions;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, conditions);
};

నిర్మాణం Feature_పట్టిక_Substitution_రికార్డు
{
  మిత్ర నిర్మాణం Feature_పట్టిక_Substitution;

  శూన్య collect_lookups (స్థిరరాశి శూన్య *పునాది, hb_సమితి_t *lookup_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    తిరిగి (పునాది+feature).కూడు_lookup_indexes_గమ్యం (lookup_indexes);
  }

  శూన్య సంవృతం_features (స్థిరరాశి శూన్య *పునాది,
			 స్థిరరాశి hb_map_t *lookup_indexes,
			 hb_సమితి_t       *feature_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ ((పునాది+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->కూడు (feature_సూచీ_2);
  }

  శూన్య collect_feature_substitutes_with_variations (hb_hashmap_t<గుర్తులేని, స్థిరరాశి Feature*> *feature_substitutes_map,
                                                    hb_సమితి_t& catch_all_రికార్డు_feature_idxes,
                                                    స్థిరరాశి hb_సమితి_t *feature_indices,
                                                    స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    ఒకవేళ (feature_indices->has (feature_సూచీ_2))
    {
      feature_substitutes_map->సమితి (feature_సూచీ_2, &(పునాది+feature));
      catch_all_రికార్డు_feature_idxes.కూడు (feature_సూచీ_2);
    }
  }

  తార్కిక serialize (hb_subset_layout_context_t *c,
                  గుర్తులేని feature_సూచీ,
                  స్థిరరాశి Feature *f, స్థిరరాశి ట్యాగ్ *ట్యాగ్_2)
  {
    TRACE_SERIALIZE (ఇది);
    hb_serialize_context_t *s = c->subset_context->serializer;
    ఒకవేళ (unlikely (!s->extend_min (ఇది))) return_trace (అబద్ధం);

    uint32_t *new_feature_idx;
    ఒకవేళ (!c->feature_map_w_duplicates->has (feature_సూచీ, &new_feature_idx))
      return_trace (అబద్ధం);

    ఒకవేళ (!s->check_కేటాయించు (feature_సూచీ_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    s->push ();
    తార్కిక ret = f->subset (c->subset_context, c, ట్యాగ్_2);
    ఒకవేళ (ret) s->కూడు_link (feature, s->తీయి_pack ());
    లేకపోతే s->తీయి_discard ();

    return_trace (ret);
  }

  తార్కిక subset (hb_subset_layout_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    uint32_t *new_feature_సూచీ;
    ఒకవేళ (!c->feature_map_w_duplicates->has (feature_సూచీ_2, &new_feature_సూచీ))
      return_trace (అబద్ధం);

    స్వయంచాలక *బహిర్గతం = c->subset_context->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->feature_సూచీ_2 = *new_feature_సూచీ;
    hb_pair_t<స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*> *original;
    ఒకవేళ (!c->feature_idx_ట్యాగ్_map->has (feature_సూచీ_2, &original))
      return_trace (అబద్ధం);
    స్థిరరాశి ట్యాగ్ *ట్యాగ్_2 = reinterpret_cast<స్థిరరాశి ట్యాగ్*> (original->second);
    return_trace (బహిర్గతం->feature.serialize_subset (c->subset_context, feature, పునాది, c, ట్యాగ్_2));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది,
		 స్థిరరాశి FeatureList *feature_list) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది))) return_trace (అబద్ధం);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    స్థిరరాశి రికార్డు_sanitize_సంవృతం_t సంవృతం = {feature_list->get_ట్యాగ్ (feature_సూచీ_2), పునాది};
    return_trace (feature.sanitize (c, పునాది, &సంవృతం));
  }

  రక్షిత:
  HBUINT16		feature_సూచీ_2;
  Offset32_గమ్యం<Feature>	feature;
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం Feature_పట్టిక_Substitution
{
  శూన్య collect_feature_indices (hb_సమితి_t *feature_indices) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Feature_పట్టిక_Substitution_రికార్డు &రికార్డు : substitutions)
      feature_indices->కూడు (రికార్డు.feature_సూచీ_2);
  }

  స్థిరరాశి Feature *కనుగొను_substitute (గుర్తులేని పూర్ణాంకం feature_సూచీ) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = substitutions.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      స్థిరరాశి Feature_పట్టిక_Substitution_రికార్డు &రికార్డు = substitutions.arrayZ[i];
      ఒకవేళ (రికార్డు.feature_సూచీ_2 == feature_సూచీ)
	తిరిగి &(ఇది+రికార్డు.feature);
    }
    తిరిగి శూన్యసూచిక;
  }

  శూన్య collect_lookups (స్థిరరాశి hb_సమితి_t *feature_indexes,
			hb_సమితి_t       *lookup_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &Feature_పట్టిక_Substitution_రికార్డు::feature_సూచీ_2)
    | hb_apply ([ఇది, lookup_indexes] (స్థిరరాశి Feature_పట్టిక_Substitution_రికార్డు& r)
		{ r.collect_lookups (ఇది, lookup_indexes); })
    ;
  }

  మూస <రకం_పేరు ట్యాగ్_Getter>
  శూన్య collect_name_ids (స్థిరరాశి hb_map_t *feature_సూచీ_map,
                         ట్యాగ్_Getter get_ట్యాగ్,
                         hb_సమితి_t *nameids_గమ్యం_retain) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Feature_పట్టిక_Substitution_రికార్డు &రికార్డు : substitutions)
      ఒకవేళ (feature_సూచీ_map->has (రికార్డు.feature_సూచీ_2))
        (ఇది+రికార్డు.feature).collect_name_ids (get_ట్యాగ్ (రికార్డు.feature_సూచీ_2), nameids_గమ్యం_retain);
  }

  శూన్య సంవృతం_features (స్థిరరాశి hb_map_t *lookup_indexes,
			 hb_సమితి_t       *feature_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Feature_పట్టిక_Substitution_రికార్డు& రికార్డు : substitutions)
      రికార్డు.సంవృతం_features (ఇది, lookup_indexes, feature_indexes);
  }

  తార్కిక intersects_features (స్థిరరాశి hb_map_t *feature_సూచీ_map) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Feature_పట్టిక_Substitution_రికార్డు& రికార్డు : substitutions)
    {
      ఒకవేళ (feature_సూచీ_map->has (రికార్డు.feature_సూచీ_2)) తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
  }

  శూన్య collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Feature_పట్టిక_Substitution_రికార్డు& రికార్డు : substitutions)
      రికార్డు.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_రికార్డు_feature_idxes,
                                                          c->feature_indices, ఇది);
  }

  తార్కిక subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               తార్కిక చొప్పించు_catch_all) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ.major = సంస్కరణ.major;
    బహిర్గతం->సంస్కరణ.minor = సంస్కరణ.minor;

    ఒకవేళ (చొప్పించు_catch_all)
    {
      కొరకు (గుర్తులేని feature_సూచీ : *(l->catch_all_రికార్డు_feature_idxes))
      {
        hb_pair_t<స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*> *p;
        ఒకవేళ (!l->feature_idx_ట్యాగ్_map->has (feature_సూచీ, &p))
          return_trace (అబద్ధం);
        స్వయంచాలక *o = బహిర్గతం->substitutions.serialize_append (c->serializer);
        ఒకవేళ (!o->serialize (l, feature_సూచీ,
                           reinterpret_cast<స్థిరరాశి Feature*> (p->మొదటి),
                           reinterpret_cast<స్థిరరాశి ట్యాగ్*> (p->second)))
          return_trace (అబద్ధం);
      }
      return_trace (నిజం);
    }

    + substitutions.iter ()
    | hb_apply (subset_రికార్డు_array (l, &(బహిర్గతం->substitutions), ఇది))
    ;

    return_trace (తార్కిక (బహిర్గతం->substitutions));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి FeatureList *feature_list) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సంస్కరణ.sanitize (c) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1) &&
		  substitutions.sanitize (c, ఇది, feature_list));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>	సంస్కరణ;	/* Version--0x00010000u */
  Array16_యొక్క<Feature_పట్టిక_Substitution_రికార్డు>
			substitutions;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, substitutions);
};

నిర్మాణం Feature_Variation_రికార్డు
{
  మిత్ర నిర్మాణం FeatureVariations;

  శూన్య collect_lookups (స్థిరరాశి శూన్య     *పునాది,
			స్థిరరాశి hb_సమితి_t *feature_indexes,
			hb_సమితి_t       *lookup_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    తిరిగి (పునాది+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  మూస <రకం_పేరు ట్యాగ్_Getter>
  శూన్య collect_name_ids (స్థిరరాశి శూన్య *పునాది,
                         స్థిరరాశి hb_map_t *feature_సూచీ_map,
                         ట్యాగ్_Getter get_ట్యాగ్,
                         hb_సమితి_t *nameids_గమ్యం_retain) స్థిరరాశి
  {
    (పునాది+substitutions).collect_name_ids (feature_సూచీ_map, get_ట్యాగ్, nameids_గమ్యం_retain);
  }

  శూన్య సంవృతం_features (స్థిరరాశి శూన్య     *పునాది,
			 స్థిరరాశి hb_map_t *lookup_indexes,
			 hb_సమితి_t       *feature_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    (పునాది+substitutions).సంవృతం_features (lookup_indexes, feature_indexes);
  }

  తార్కిక intersects_features (స్థిరరాశి శూన్య *పునాది, స్థిరరాశి hb_map_t *feature_సూచీ_map) స్థిరరాశి
  {
    తిరిగి (పునాది+substitutions).intersects_features (feature_సూచీ_map);
  }

  శూన్య collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    (పునాది+conditions).keep_with_variations (c);
    ఒకవేళ (c->apply && !c->variation_applied)
    {
      (పునాది+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = నిజం; // set variations only once
    }
  }

  తార్కిక subset (hb_subset_layout_context_t *c, స్థిరరాశి శూన్య *పునాది,
               తార్కిక చొప్పించు_catch_all = అబద్ధం) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->subset_context->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    hb_సమితి_t *retained_conditions = c->feature_రికార్డు_cond_idx_map ?
        c->feature_రికార్డు_cond_idx_map->get (c->cur_feature_var_రికార్డు_idx).get () : శూన్యసూచిక;
    ఒకవేళ (!చొప్పించు_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      బహిర్గతం->conditions = 0;
    లేకపోతే
      బహిర్గతం->conditions.serialize_subset (c->subset_context, conditions, పునాది, c, చొప్పించు_catch_all);
    బహిర్గతం->substitutions.serialize_subset (c->subset_context, substitutions, పునాది, c, చొప్పించు_catch_all);

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది,
		 స్థిరరాశి FeatureList *feature_list) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (conditions.sanitize (c, పునాది) &&
		  substitutions.sanitize (c, పునాది, feature_list));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  { (పునాది+conditions).collect_variation_indices (c); }

  రక్షిత:
  Offset32_గమ్యం<Condition_సమితి>
			conditions;
  Offset32_గమ్యం<Feature_పట్టిక_Substitution>
			substitutions;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం Lookup_సూచీ_List
{
  శూన్య కూడు_lookup_indexes_గమ్యం (hb_సమితి_t *lookup_indexes) స్థిరరాశి
  { lookupIndices.కూడు_indexes_గమ్యం (lookup_indexes); }

  తార్కిక intersects_lookup_indexes (స్థిరరాశి hb_map_t *lookup_indexes) స్థిరరాశి
  { తిరిగి lookupIndices.intersects (lookup_indexes); }

  తార్కిక subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం)))
      return_trace (అబద్ధం);

    స్వయంచాలక it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_సూచీ_map)
    | hb_map (l->lookup_సూచీ_map)
    ;
    బహిర్గతం->lookupIndices.serialize (c->serializer, l, it);
    return_trace (తార్కిక (బహిర్గతం->lookupIndices));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (lookupIndices.sanitize (c));
  }

  రక్షిత:
  సూచీ_Array	lookupIndices;
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (2, lookupIndices);
};

నిర్మాణం Lookup_Condition_రికార్డు
{
  గుర్తులేని get_condition_స్థానభ్రంశం (స్థిరరాశి శూన్య *పునాది, స్థిరరాశి శూన్య *variations) స్థిరరాశి
  { తిరిగి (స్థిరరాశి అక్షరం *) &(పునాది+condition) - (స్థిరరాశి అక్షరం *) variations; }

  మూస <రకం_పేరు Instancer>
  తార్కిక evaluate (స్థిరరాశి శూన్య *పునాది,
		 స్థిరరాశి పూర్ణాంకం *coords,
		 గుర్తులేని పూర్ణాంకం coord_len,
		 Instancer *instancer) స్థిరరాశి
  { తిరిగి (పునాది+condition).evaluate (coords, coord_len, instancer); }

  తార్కిక evaluate (స్థిరరాశి శూన్య *పునాది,
                 స్థిరరాశి శూన్య *variations,
                 hb_sorted_array_t<స్థిరరాశి గుర్తులేని> conditions) స్థిరరాశి
  {
    తిరిగి !condition || hb_bsearch (get_condition_స్థానభ్రంశం (పునాది, variations),
                                     conditions.arrayZ, conditions.నిడివి,
                                     పరిమాణం (గుర్తులేని),
                                     _hb_cmp_operator<గుర్తులేని, గుర్తులేని>);
  }

  తార్కిక collect_condition (స్థిరరాశి శూన్య *పునాది,
                          స్థిరరాశి శూన్య *variations,
                          స్థిరరాశి పూర్ణాంకం *coords,
                          గుర్తులేని పూర్ణాంకం coord_len,
                          అంశం_Var_Store_Instancer *instancer,
                          hb_సమితి_t *సందర్శించిన,
                          hb_సదిశ_t<గుర్తులేని> *conditions) స్థిరరాశి
  {
    /* Null conditions always apply and need no key state. */
    ఒకవేళ (!condition) తిరిగి నిజం;

    గుర్తులేని స్థానభ్రంశం = get_condition_స్థానభ్రంశం (పునాది, variations);
    ఒకవేళ (సందర్శించిన->has (స్థానభ్రంశం)) తిరిగి నిజం;
    సందర్శించిన->కూడు (స్థానభ్రంశం);
    ఒకవేళ (unlikely (సందర్శించిన->లోపల_error ())) తిరిగి అబద్ధం;

    ఒకవేళ (evaluate (పునాది, coords, coord_len, instancer))
      conditions->push (స్థానభ్రంశం);
    తిరిగి !conditions->లోపల_error ();
  }

  శూన్య కూడు_lookup_indexes_గమ్యం (స్థిరరాశి శూన్య *పునాది,
			      hb_సమితి_t *lookup_indexes) స్థిరరాశి
  { (పునాది+lookupIndices).కూడు_lookup_indexes_గమ్యం (lookup_indexes); }

  తార్కిక intersects_lookup_indexes (స్థిరరాశి శూన్య *పునాది,
				   స్థిరరాశి hb_map_t *lookup_indexes) స్థిరరాశి
  { తిరిగి (పునాది+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(పునాది+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_విలువలు)))
      c->layout_variation_indices->err ();
  }

  తార్కిక subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_ఫలితం_t ఫలితం =
        (పునాది + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    నిజం);
    ఒకవేళ (ఫలితం != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->తీయి_discard ();
      return_trace (అబద్ధం);
    }
    c->serializer->కూడు_link (బహిర్గతం->condition, c->serializer->తీయి_pack ());

    return_trace (బహిర్గతం->lookupIndices.serialize_subset (c, lookupIndices, పునాది, l));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (condition.sanitize (c, పునాది) &&
		  lookupIndices.sanitize (c, పునాది));
  }

  రక్షిత:
  Offset32_గమ్యం<Condition>		condition;
  Offset32_గమ్యం<Lookup_సూచీ_List>	lookupIndices;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం Feature_Lookups_పట్టిక
{
  తార్కిక adds_default_lookups () స్థిరరాశి { తిరిగి flags & 0x0001u; }

  తార్కిక collect_conditions (స్థిరరాశి శూన్య *variations,
                           స్థిరరాశి పూర్ణాంకం *coords,
                           గుర్తులేని పూర్ణాంకం coord_len,
                           అంశం_Var_Store_Instancer *instancer,
                           hb_సమితి_t *సందర్శించిన,
                           hb_సదిశ_t<గుర్తులేని> *conditions) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Condition_రికార్డు &రికార్డు : రికార్డులు)
      ఒకవేళ (unlikely (!రికార్డు.collect_condition (ఇది, variations, coords, coord_len,
                                               instancer, సందర్శించిన, conditions)))
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  శూన్య collect_lookups (స్థిరరాశి శూన్య *variations,
                       hb_sorted_array_t<స్థిరరాశి గుర్తులేని> conditions,
                       hb_సమితి_t *lookup_indexes) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Condition_రికార్డు &రికార్డు : రికార్డులు)
      ఒకవేళ (రికార్డు.evaluate (ఇది, variations, conditions))
	రికార్డు.కూడు_lookup_indexes_గమ్యం (ఇది, lookup_indexes);
  }

  మూస <రకం_పేరు Instancer>
  శూన్య collect_lookups (స్థిరరాశి పూర్ణాంకం *coords,
			గుర్తులేని పూర్ణాంకం coord_len,
			Instancer *instancer,
			hb_సమితి_t *lookup_indexes) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Condition_రికార్డు &రికార్డు : రికార్డులు)
      ఒకవేళ (రికార్డు.evaluate (ఇది, coords, coord_len, instancer))
	రికార్డు.కూడు_lookup_indexes_గమ్యం (ఇది, lookup_indexes);
  }

  శూన్య collect_lookups (hb_సమితి_t *lookup_indexes) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Condition_రికార్డు &రికార్డు : రికార్డులు)
      రికార్డు.కూడు_lookup_indexes_గమ్యం (ఇది, lookup_indexes);
  }

  తార్కిక intersects_lookup_indexes (స్థిరరాశి hb_map_t *lookup_indexes) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Condition_రికార్డు &రికార్డు : రికార్డులు)
      ఒకవేళ (రికార్డు.intersects_lookup_indexes (ఇది, lookup_indexes))
	తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Condition_రికార్డు &రికార్డు : రికార్డులు)
      రికార్డు.collect_variation_indices (c, ఇది);
  }

  తార్కిక subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం)))
      return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ.major = సంస్కరణ.major;
    బహిర్గతం->సంస్కరణ.minor = సంస్కరణ.minor;
    బహిర్గతం->flags = flags;

    కొరకు (స్థిరరాశి Lookup_Condition_రికార్డు &రికార్డు : రికార్డులు)
    {
      స్వయంచాలక snap = c->serializer->snapshot ();
      ఒకవేళ (రికార్డు.subset (c, l, ఇది))
	బహిర్గతం->రికార్డులు.len++;
      లేకపోతే
	c->serializer->revert (snap);
    }

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సంస్కరణ.sanitize (c) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1) &&
		  రికార్డులు.sanitize (c, ఇది));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>	సంస్కరణ;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32_యొక్క<Lookup_Condition_రికార్డు>
		రికార్డులు;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (10, రికార్డులు);
};

నిర్మాణం Lookup_Variation_రికార్డు
{
  మిత్ర నిర్మాణం FeatureVariations;

  గుర్తులేని get_feature_సూచీ () స్థిరరాశి { తిరిగి feature_సూచీ_2; }

  శూన్య collect_lookups (స్థిరరాశి శూన్య *పునాది,
                       స్థిరరాశి Feature &feature,
                       hb_sorted_array_t<స్థిరరాశి గుర్తులేని> conditions,
                       hb_సమితి_t *lookup_indexes) స్థిరరాశి
  {
    స్థిరరాశి Feature_Lookups_పట్టిక &feature_lookups = పునాది+featureLookups;
    ఒకవేళ (feature_lookups.adds_default_lookups ())
      feature.కూడు_lookup_indexes_గమ్యం (lookup_indexes);
    feature_lookups.collect_lookups (పునాది, conditions, lookup_indexes);
  }

  శూన్య collect_lookups (స్థిరరాశి శూన్య *పునాది,
			స్థిరరాశి hb_సమితి_t *feature_indexes,
			hb_సమితి_t *lookup_indexes) స్థిరరాశి
  {
    ఒకవేళ (feature_indexes->has (feature_సూచీ_2))
      (పునాది+featureLookups).collect_lookups (lookup_indexes);
  }

  శూన్య సంవృతం_features (స్థిరరాశి శూన్య *పునాది,
			 స్థిరరాశి hb_map_t *lookup_indexes,
			 hb_సమితి_t *feature_indexes) స్థిరరాశి
  {
    ఒకవేళ ((పునాది+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->కూడు (feature_సూచీ_2);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  { (పునాది+featureLookups).collect_variation_indices (c); }

  తార్కిక subset (hb_subset_layout_context_t *l, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (ఇది);
    uint32_t *new_feature_సూచీ;
    ఒకవేళ (!l->feature_map_w_duplicates->has (feature_సూచీ_2, &new_feature_సూచీ))
      return_trace (అబద్ధం);

    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం ||
		  !c->serializer->check_కేటాయించు (బహిర్గతం->feature_సూచీ_2,
						*new_feature_సూచీ,
						HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
      return_trace (అబద్ధం);

    return_trace (బహిర్గతం->featureLookups.serialize_subset (c, featureLookups, పునాది, l));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  featureLookups.sanitize (c, పునాది));
  }

  రక్షిత:
  HBUINT16			feature_సూచీ_2;
  Offset32_గమ్యం<Feature_Lookups_పట్టిక>	featureLookups;
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం FeatureVariations
{
  స్థిర constexpr గుర్తులేని NOT_దొరికింది_సూచీ = 0xFFFFFFFFu;

  స్థిరరాశి Array32_యొక్క<Lookup_Variation_రికార్డు> &get_lookup_variation_రికార్డులు () స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ.గమ్యం_int () < 0x00010001u)
      తిరిగి Null (Array32_యొక్క<Lookup_Variation_రికార్డు>);
    తిరిగి StructAfter<Array32_యొక్క<Lookup_Variation_రికార్డు>> (var_రికార్డులు);
  }

  గుర్తులేని రికార్డు_లెక్క () స్థిరరాశి
  {
    తిరిగి var_రికార్డులు.len;
  }

  తార్కిక కనుగొను_సూచీ (స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_len,
		   గుర్తులేని పూర్ణాంకం *సూచీ_2,
		   అంశం_Var_Store_Instancer *instancer) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = var_రికార్డులు.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      స్థిరరాశి Feature_Variation_రికార్డు &రికార్డు = var_రికార్డులు.arrayZ[i];
      ఒకవేళ ((ఇది+రికార్డు.conditions).evaluate (coords, coord_len, instancer))
      {
	*సూచీ_2 = i;
	తిరిగి నిజం;
      }
    }
    *సూచీ_2 = NOT_దొరికింది_సూచీ;
    తిరిగి అబద్ధం;
  }

  స్థిరరాశి Feature *కనుగొను_substitute (గుర్తులేని పూర్ణాంకం variations_సూచీ,
				  గుర్తులేని పూర్ణాంకం feature_సూచీ) స్థిరరాశి
  {
    స్థిరరాశి Feature_Variation_రికార్డు &రికార్డు = var_రికార్డులు[variations_సూచీ];
    తిరిగి (ఇది+రికార్డు.substitutions).కనుగొను_substitute (feature_సూచీ);
  }

  తార్కిక collect_lookup_conditions (స్థిరరాశి పూర్ణాంకం *coords,
                                  గుర్తులేని పూర్ణాంకం coord_len,
                                  అంశం_Var_Store_Instancer *instancer,
                                  hb_సదిశ_t<గుర్తులేని> *state) స్థిరరాశి
  {
    hb_సమితి_t సందర్శించిన_tables, సందర్శించిన_conditions;
    కొరకు (స్థిరరాశి Lookup_Variation_రికార్డు &రికార్డు : get_lookup_variation_రికార్డులు ())
    {
      గుర్తులేని స్థానభ్రంశం = రికార్డు.featureLookups;
      ఒకవేళ (సందర్శించిన_tables.has (స్థానభ్రంశం)) కొనసాగు;
      సందర్శించిన_tables.కూడు (స్థానభ్రంశం);
      ఒకవేళ (unlikely (సందర్శించిన_tables.లోపల_error ())) తిరిగి అబద్ధం;

      స్థిరరాశి Feature_Lookups_పట్టిక &feature_lookups = ఇది+రికార్డు.featureLookups;
      ఒకవేళ (unlikely (!feature_lookups.collect_conditions (ఇది, coords, coord_len,
                                                         instancer, &సందర్శించిన_conditions,
                                                         state)))
	తిరిగి అబద్ధం;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<గుర్తులేని, గుర్తులేని>);
    తిరిగి నిజం;
  }

  తార్కిక get_lookup_variations (గుర్తులేని feature_సూచీ,
                              స్థిరరాశి Feature &feature,
                              hb_sorted_array_t<స్థిరరాశి గుర్తులేని> conditions,
                              hb_సమితి_t *lookup_indexes) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Variation_రికార్డు &రికార్డు : get_lookup_variation_రికార్డులు ())
    {
      ఒకవేళ (రికార్డు.feature_సూచీ_2 != feature_సూచీ) కొనసాగు;

      రికార్డు.collect_lookups (ఇది, feature, conditions, lookup_indexes);
      తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
  }

  శూన్య collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = var_రికార్డులు.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      c->cur_రికార్డు_idx = i;
      var_రికార్డులు[i].collect_feature_substitutes_with_variations (c, ఇది);
      ఒకవేళ (c->universal)
        ఆపు;
    }
    ఒకవేళ (c->universal || c->రికార్డు_cond_idx_map->is_empty ())
      c->catch_all_రికార్డు_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed (*ఇది));
  }

  శూన్య collect_lookups (స్థిరరాశి hb_సమితి_t *feature_indexes,
			స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map,
			hb_సమితి_t       *lookup_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    గుర్తులేని లెక్క = var_రికార్డులు.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (feature_రికార్డు_cond_idx_map &&
          !feature_రికార్డు_cond_idx_map->has (i))
        కొనసాగు;
      var_రికార్డులు[i].collect_lookups (ఇది, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  మూస <రకం_పేరు ట్యాగ్_Getter>
  శూన్య collect_name_ids (స్థిరరాశి hb_map_t *feature_సూచీ_map,
                         స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map,
                         ట్యాగ్_Getter get_ట్యాగ్,
                         hb_సమితి_t *nameids_గమ్యం_retain) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < var_రికార్డులు.len; i++)
    {
      ఒకవేళ (feature_రికార్డు_cond_idx_map && !feature_రికార్డు_cond_idx_map->has (i))
        కొనసాగు;
      var_రికార్డులు[i].collect_name_ids (ఇది, feature_సూచీ_map, get_ట్యాగ్, nameids_గమ్యం_retain);
    }
  }

  శూన్య collect_lookup_variation_lookups (స్థిరరాశి hb_సమితి_t *feature_indexes,
					 hb_సమితి_t *lookup_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Variation_రికార్డు &రికార్డు : get_lookup_variation_రికార్డులు ())
      రికార్డు.collect_lookups (ఇది, feature_indexes, lookup_indexes);
  }

  శూన్య సంవృతం_features (స్థిరరాశి hb_map_t *lookup_indexes,
			 స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb::shared_ptr<hb_సమితి_t>> *feature_రికార్డు_cond_idx_map,
			 hb_సమితి_t       *feature_indexes /*బహిర్గతం*/) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = var_రికార్డులు.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (feature_రికార్డు_cond_idx_map != శూన్యసూచిక &&
          !feature_రికార్డు_cond_idx_map->has (i))
        కొనసాగు;
      var_రికార్డులు[i].సంవృతం_features (ఇది, lookup_indexes, feature_indexes);
    }

    కొరకు (స్థిరరాశి Lookup_Variation_రికార్డు &రికార్డు : get_lookup_variation_రికార్డులు ())
      రికార్డు.సంవృతం_features (ఇది, lookup_indexes, feature_indexes);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Feature_Variation_రికార్డు &రికార్డు : var_రికార్డులు)
      రికార్డు.collect_variation_indices (c, ఇది);
    కొరకు (స్థిరరాశి Lookup_Variation_రికార్డు &రికార్డు : get_lookup_variation_రికార్డులు ())
      రికార్డు.collect_variation_indices (c, ఇది);
  }

  శూన్య collect_feature_indices (hb_సమితి_t *feature_indices) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Feature_Variation_రికార్డు &రికార్డు : var_రికార్డులు)
      (ఇది+రికార్డు.substitutions).collect_feature_indices (feature_indices);
  }

  తార్కిక has_lookup_variations (గుర్తులేని feature_సూచీ) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Lookup_Variation_రికార్డు &రికార్డు : get_lookup_variation_రికార్డులు ())
    {
      ఒకవేళ (రికార్డు.feature_సూచీ_2 == feature_సూచీ)
	తిరిగి నిజం;
      ఒకవేళ (రికార్డు.feature_సూచీ_2 > feature_సూచీ)
	ఆపు;
    }
    తిరిగి అబద్ధం;
  }

  తార్కిక subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ.major = సంస్కరణ.major;
    బహిర్గతం->సంస్కరణ.minor = సంస్కరణ.minor;

    పూర్ణాంకం keep_పైకి_గమ్యం = -1;
    కొరకు (పూర్ణాంకం i = var_రికార్డులు.len - 1; i >= 0; i--) {
      ఒకవేళ (var_రికార్డులు[i].intersects_features (ఇది, l->feature_map_w_duplicates)) {
        keep_పైకి_గమ్యం = i;
        ఆపు;
      }
    }

    గుర్తులేని లెక్క = (గుర్తులేని) (keep_పైకి_గమ్యం + 1);
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (l->feature_రికార్డు_cond_idx_map != శూన్యసూచిక &&
          !l->feature_రికార్డు_cond_idx_map->has (i))
        కొనసాగు;

      l->cur_feature_var_రికార్డు_idx = i;
      subset_రికార్డు_array (l, &(బహిర్గతం->var_రికార్డులు), ఇది) (var_రికార్డులు[i]);
    }

    ఒకవేళ (బహిర్గతం->var_రికార్డులు.len && !l->catch_all_రికార్డు_feature_idxes->is_empty ())
    {
      తార్కిక చొప్పించు_catch_all_రికార్డు = నిజం;
      subset_రికార్డు_array (l, &(బహిర్గతం->var_రికార్డులు), ఇది, చొప్పించు_catch_all_రికార్డు) (var_రికార్డులు[0]);
    }

    Array32_యొక్క<Lookup_Variation_రికార్డు> *బహిర్గతం_lookup_రికార్డులు = శూన్యసూచిక;
    ఒకవేళ (సంస్కరణ.minor >= 1)
    {
      బహిర్గతం_lookup_రికార్డులు = &StructAfter<Array32_యొక్క<Lookup_Variation_రికార్డు>> (బహిర్గతం->var_రికార్డులు);
      ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం_lookup_రికార్డులు)))
	return_trace (అబద్ధం);

      + get_lookup_variation_రికార్డులు ().iter ()
      | hb_apply (subset_రికార్డు_array (l, బహిర్గతం_lookup_రికార్డులు, ఇది))
      ;
    }

    return_trace (తార్కిక (బహిర్గతం->var_రికార్డులు) ||
		  (బహిర్గతం_lookup_రికార్డులు && తార్కిక (*బహిర్గతం_lookup_రికార్డులు)));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి FeatureList *feature_list) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సంస్కరణ.sanitize (c) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1) &&
		  var_రికార్డులు.sanitize (c, ఇది, feature_list) &&
		  (సంస్కరణ.minor < 1 ||
		   get_lookup_variation_రికార్డులు ().sanitize (c, ఇది)));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>	సంస్కరణ;	/* Version--0x00010000u */
  Array32_యొక్క<Feature_Variation_రికార్డు>
			var_రికార్డులు;
  బహిరంగ:
  DEFINE_కొలత_ARRAY_SIZED (8, var_రికార్డులు);
};


/*
 * Device Tables
 */

నిర్మాణం HintingDevice
{
  మిత్ర నిర్మాణం Device;

  అంతరంగ:

  hb_position_t get_x_delta (hb_font_t *font) స్థిరరాశి
  { తిరిగి get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) స్థిరరాశి
  { తిరిగి get_delta (font->y_ppem, font->y_scale); }

  బహిరంగ:

  size_t get_కొలత () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం f = deltaFormat;
    ఒకవేళ (unlikely (f < 1 || f > 3 || ప్రారంభం_కొలత > చివర_కొలత)) తిరిగి 3 * HBUINT16::static_కొలత;
    తిరిగి HBUINT16::static_కొలత * (4 + ((చివర_కొలత - ప్రారంభం_కొలత) >> (4 - f)));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && c->check_range (ఇది, ఇది->get_కొలత ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed<HintingDevice> (ఇది));
  }

  అంతరంగ:

  పూర్ణాంకం get_delta (గుర్తులేని పూర్ణాంకం ppem, పూర్ణాంకం scale) స్థిరరాశి
  {
    ఒకవేళ (!ppem) తిరిగి 0;

    పూర్ణాంకం pixels = get_delta_pixels (ppem);

    ఒకవేళ (!pixels) తిరిగి 0;

    తిరిగి (పూర్ణాంకం) (pixels * (int64_t) scale / ppem);
  }
  పూర్ణాంకం get_delta_pixels (గుర్తులేని పూర్ణాంకం ppem_కొలత) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం f = deltaFormat;
    ఒకవేళ (unlikely (f < 1 || f > 3))
      తిరిగి 0;

    ఒకవేళ (ppem_కొలత < ప్రారంభం_కొలత || ppem_కొలత > చివర_కొలత)
      తిరిగి 0;

    గుర్తులేని పూర్ణాంకం s = ppem_కొలత - ప్రారంభం_కొలత;

    గుర్తులేని పూర్ణాంకం byte = delta_విలువ_Z[s >> (4 - f)];
    గుర్తులేని పూర్ణాంకం బిట్లు = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    గుర్తులేని పూర్ణాంకం ముసుగు = (0xFFFFu >> (16 - (1 << f)));

    పూర్ణాంకం delta = బిట్లు & ముసుగు;

    ఒకవేళ ((గుర్తులేని పూర్ణాంకం) delta >= ((ముసుగు + 1) >> 1))
      delta -= ముసుగు + 1;

    తిరిగి delta;
  }

  రక్షిత:
  HBUINT16	ప్రారంభం_కొలత;		/* Smallest size to correct--in ppem */
  HBUINT16	చివర_కొలత;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  Unsized_Array_యొక్క<HBUINT16>
		delta_విలువ_Z;		/* Array of compressed data */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, delta_విలువ_Z);
};

నిర్మాణం VariationDevice
{
  మిత్ర నిర్మాణం Device;

  అంతరంగ:

  hb_position_t get_x_delta (hb_font_t *font,
			     స్థిరరాశి అంశం_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  { తిరిగి !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     స్థిరరాశి అంశం_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  { తిరిగి !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  దశాంశం get_x_delta_f (hb_font_t *font,
		       స్థిరరాశి అంశం_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) స్థిరరాశి
  { తిరిగి !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  దశాంశం get_y_delta_f (hb_font_t *font,
		       స్థిరరాశి అంశం_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) స్థిరరాశి
  { తిరిగి !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (!layout_variation_idx_delta_map) return_trace (శూన్యసూచిక);

    hb_pair_t<గుర్తులేని, పూర్ణాంకం> *v;
    ఒకవేళ (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (శూన్యసూచిక);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    ఒకవేళ (hb_మొదటి (*v) == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
      return_trace (శూన్యసూచిక);

    c->ప్రారంభం_zerocopy (ఇది->static_కొలత);
    స్వయంచాలక *బహిర్గతం = c->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (శూన్యసూచిక);

    ఒకవేళ (!c->check_కేటాయించు (బహిర్గతం->varIdx, hb_మొదటి (*v), HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (శూన్యసూచిక);
    return_trace (బహిర్గతం);
  }

  శూన్య collect_variation_సూచీ (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  { c->layout_variation_indices->కూడు (varIdx); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  అంతరంగ:

  దశాంశం get_delta (hb_font_t *font,
		   స్థిరరాశి అంశం_Variation_Store &store,
		   hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
    తిరిగి store.get_delta (varIdx, font->coords, font->అంకె_coords, store_cache);
  }

  రక్షిత:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం Device_శీర్షిక
{
  రక్షిత:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  బహిరంగ:
  HBUINT16		format;		/* Format identifier */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  దశాంశం get_x_delta_f (hb_font_t *font,
		       స్థిరరాశి అంశం_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (u.b.format == 0x8000)
    {
      hb_barrier ();
      తిరిగి u.variation.get_x_delta_f (font, store, store_cache);
    }
#అయితే_ముగింపు
    తిరిగి get_x_delta (font, store, store_cache);
  }

  దశాంశం get_y_delta_f (hb_font_t *font,
		       స్థిరరాశి అంశం_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (u.b.format == 0x8000)
    {
      hb_barrier ();
      తిరిగి u.variation.get_y_delta_f (font, store, store_cache);
    }
#అయితే_ముగింపు
    తిరిగి get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     స్థిరరాశి అంశం_Variation_Store &store=Null (అంశం_Variation_Store),
			     hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఎంపిక (u.b.format)
    {
#నిర్వచితం_కాకపోతే HB_NO_HINTING
    స్థితి 1: స్థితి 2: స్థితి 3:
      hb_barrier ();
      తిరిగి u.hinting.get_x_delta (font);
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR
    స్థితి 0x8000:
      hb_barrier ();
      తిరిగి u.variation.get_x_delta (font, store, store_cache);
#అయితే_ముగింపు
    అప్రమేయ:
      తిరిగి 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     స్థిరరాశి అంశం_Variation_Store &store=Null (అంశం_Variation_Store),
			     hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఎంపిక (u.b.format)
    {
    స్థితి 1: స్థితి 2: స్థితి 3:
#నిర్వచితం_కాకపోతే HB_NO_HINTING
      hb_barrier ();
      తిరిగి u.hinting.get_y_delta (font);
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR
    స్థితి 0x8000:
      hb_barrier ();
      తిరిగి u.variation.get_y_delta (font, store, store_cache);
#అయితే_ముగింపు
    అప్రమేయ:
      తిరిగి 0;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.b.format.sanitize (c)) return_trace (అబద్ధం);
    ఎంపిక (u.b.format) {
#నిర్వచితం_కాకపోతే HB_NO_HINTING
    స్థితి 1: స్థితి 2: స్థితి 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR
    స్థితి 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#అయితే_ముగింపు
    అప్రమేయ:
      return_trace (నిజం);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map=శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    ఎంపిక (u.b.format) {
#నిర్వచితం_కాకపోతే HB_NO_HINTING
    స్థితి 1:
    స్థితి 2:
    స్థితి 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR
    స్థితి 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#అయితే_ముగింపు
    అప్రమేయ:
      return_trace (శూన్యసూచిక);
    }
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    ఎంపిక (u.b.format) {
#నిర్వచితం_కాకపోతే HB_NO_HINTING
    స్థితి 1:
    స్థితి 2:
    స్థితి 3:
      తిరిగి;
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR
    స్థితి 0x8000:
      hb_barrier ();
      u.variation.collect_variation_సూచీ (c);
      తిరిగి;
#అయితే_ముగింపు
    అప్రమేయ:
      తిరిగి;
    }
  }

  గుర్తులేని get_variation_సూచీ () స్థిరరాశి
  {
    ఎంపిక (u.b.format) {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    స్థితి 0x8000:
      hb_barrier ();
      తిరిగి u.variation.varIdx;
#అయితే_ముగింపు
    అప్రమేయ:
      తిరిగి HB_OT_LAYOUT_NO_VARIATIONS_సూచీ;
    }
  }

  తార్కిక is_variation_device () స్థిరరాశి
  {
    ఎంపిక (u.b.format) {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    స్థితి 0x8000:
      తిరిగి నిజం;
#అయితే_ముగింపు
    అప్రమేయ:
      తిరిగి అబద్ధం;
    }
  }

  రక్షిత:
  సంఘం {
  Device_శీర్షిక		b;
  HintingDevice		hinting;
#నిర్వచితం_కాకపోతే HB_NO_VAR
  VariationDevice	variation;
#అయితే_ముగింపు
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (6, b);
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_LAYOUT_COMMON_HH */

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_LAYOUT_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_COMMON_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bimap.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cache.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-dual.hh"

ਵਰਤੋਂ hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (ਆਕਾਰ (hb_ot_layout_mapping_cache_t) == 512, "");

ਵਰਤੋਂ hb_ot_layout_ਦੁਆਧਾਰੀ_cache_t = hb_cache_t<14, 1, 8>;
static_assert (ਆਕਾਰ (hb_ot_layout_ਦੁਆਧਾਰੀ_cache_t) == 256, "");

#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
ਵਰਤੋਂ OT::Layout::Common::Coverage;
ਵਰਤੋਂ OT::Layout::Common::Range_ਰਿਕਾਰਡ;
ਵਰਤੋਂ OT::Layout::SmallTypes;
ਵਰਤੋਂ OT::Layout::MediumTypes;


ਨਾਮਾਕਾਸ਼ OT {

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ਸਥਿਰ ਤਾਰਕਿਕ ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ਅਚਲ hb_ਸੈੱਟ_t &klasses,
    ਤਾਰਕਿਕ use_class_zero,
    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

ਬਣਤਰ ਵਸਤੂ_Var_Store_Instancer;

ਬਣਤਰ hb_collect_feature_substitutes_with_var_context_t
{
  ਅਚਲ hb_map_t *axes_ਸੂਚਕਾਂਕ_ਟੈਗ_map;
  ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *axes_location;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *ਰਿਕਾਰਡ_cond_idx_map;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Feature*> *feature_substitutes_map;
  hb_ਸੈੱਟ_t& catch_all_ਰਿਕਾਰਡ_feature_idxes;

  // not stored in subset_plan
  hb_ਸੈੱਟ_t *feature_indices;
  ਤਾਰਕਿਕ apply;
  ਤਾਰਕਿਕ variation_applied;
  ਤਾਰਕਿਕ universal;
  ਬਿਨਾਨਿਸ਼ਾਨੀ cur_ਰਿਕਾਰਡ_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, ਬਿਨਾਨਿਸ਼ਾਨੀ> *conditionset_map;
  ਵਸਤੂ_Var_Store_Instancer *instancer;
  ਤਾਰਕਿਕ all_axes_pinned;
};

ਬਣਤਰ hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ਅਚਲ ਖਾਲੀ         *ਸਾਰਣੀ_2,
                              hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> *script_langsys_map_,
                              ਅਚਲ hb_map_t     *duplicate_feature_map_,
                              hb_ਸੈੱਟ_t           *new_collected_feature_indexes_)
      :ਸਾਰਣੀ (ਸਾਰਣੀ_2),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_ਗਿਣਤੀ (0),langsys_feature_ਗਿਣਤੀ (0) {}

  ਤਾਰਕਿਕ ਦੇਖੋ_Script ()
  { ਵਾਪਸ script_ਗਿਣਤੀ++ < HB_MAX_SCRIPTS; }

  ਤਾਰਕਿਕ ਦੇਖੋ_Langsys (ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਗਿਣਤੀ_2)
  {
    langsys_feature_ਗਿਣਤੀ += feature_ਗਿਣਤੀ_2;
    ਵਾਪਸ langsys_feature_ਗਿਣਤੀ < HB_MAX_LANGSYS_FEATURE_ਗਿਣਤੀ;
  }

  ਜਨਤਕ:
  ਅਚਲ ਖਾਲੀ *ਸਾਰਣੀ;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> *script_langsys_map;
  ਅਚਲ hb_map_t     *duplicate_feature_map;
  hb_ਸੈੱਟ_t           *new_feature_indexes;

  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ script_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ langsys_feature_ਗਿਣਤੀ;
};

ਬਣਤਰ hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_ਖਾਲੀ_ਥਾਵਾਂ_t, HB_DEBUG_SUBSET>
{
  ਅਚਲ ਅੱਖਰ *get_name () { ਵਾਪਸ "SUBSET_LAYOUT"; }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }

  ਤਾਰਕਿਕ ਦੇਖੋ_Script ()
  {
    ਵਾਪਸ script_ਗਿਣਤੀ++ < HB_MAX_SCRIPTS;
  }

  ਤਾਰਕਿਕ ਦੇਖੋ_Lang_Sys ()
  {
    ਵਾਪਸ langsys_ਗਿਣਤੀ++ < HB_MAX_LANGSYS;
  }

  ਤਾਰਕਿਕ ਦੇਖੋ_Feature_ਸੂਚਕਾਂਕ (ਪੂਰਨ ਗਿਣਤੀ_2)
  {
    feature_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ += ਗਿਣਤੀ_2;
    ਵਾਪਸ feature_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ < HB_MAX_FEATURE_INDICES;
  }

  ਤਾਰਕਿਕ ਦੇਖੋ_Lookup_ਸੂਚਕਾਂਕ()
  {
    lookup_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ++;
    ਵਾਪਸ lookup_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ < HB_MAX_LOOKUP_ਦੇਖੋ_ਗਿਣਤੀ;
  }

  hb_subset_context_t *subset_context;
  ਅਚਲ hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2;
  ਅਚਲ hb_map_t *lookup_ਸੂਚਕਾਂਕ_map;
  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::unique_ptr<hb_ਸੈੱਟ_t>> *script_langsys_map;
  ਅਚਲ hb_map_t *feature_ਸੂਚਕਾਂਕ_map;
  ਅਚਲ hb_map_t *feature_map_w_duplicates;
  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Feature*> *feature_substitutes_map;
  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *lookup_variations;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map;
  ਅਚਲ hb_ਸੈੱਟ_t *catch_all_ਰਿਕਾਰਡ_feature_idxes;
  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਅਚਲ ਖਾਲੀ*, ਅਚਲ ਖਾਲੀ*>> *feature_idx_ਟੈਗ_map;

  ਬਿਨਾਨਿਸ਼ਾਨੀ cur_script_ਸੂਚਕਾਂਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ cur_feature_ਸੂਚਕਾਂਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ cur_feature_var_ਰਿਕਾਰਡ_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_ਟੈਗ_t ਟੈਗ_4) :
				subset_context (c_),
				ਸਾਰਣੀ_ਟੈਗ_2 (ਟੈਗ_4),
				cur_script_ਸੂਚਕਾਂਕ (0xFFFFu),
				cur_feature_ਸੂਚਕਾਂਕ (ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ),
				cur_feature_var_ਰਿਕਾਰਡ_idx (0u),
				script_ਗਿਣਤੀ (0),
				langsys_ਗਿਣਤੀ (0),
				feature_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ (0),
				lookup_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ (0)
  {
    ਜੇ (ਟੈਗ_4 == HB_OT_ਟੈਗ_GSUB)
    {
      lookup_ਸੂਚਕਾਂਕ_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_ਸੂਚਕਾਂਕ_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_ਰਿਕਾਰਡ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? ਖਾਲੀ_ਸੂਚਕ : &c_->plan->gsub_feature_ਰਿਕਾਰਡ_cond_idx_map;
      catch_all_ਰਿਕਾਰਡ_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_ਟੈਗ_map = &c_->plan->gsub_old_feature_idx_ਟੈਗ_map;
    }
    ਨਹੀਂਤਾਂ
    {
      lookup_ਸੂਚਕਾਂਕ_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_ਸੂਚਕਾਂਕ_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_ਰਿਕਾਰਡ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? ਖਾਲੀ_ਸੂਚਕ : &c_->plan->gpos_feature_ਰਿਕਾਰਡ_cond_idx_map;
      catch_all_ਰਿਕਾਰਡ_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_ਟੈਗ_map = &c_->plan->gpos_old_feature_idx_ਟੈਗ_map;
    }
  }

  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ script_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ langsys_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ;
};

ਬਣਤਰ ਵਸਤੂ_Variation_Store;
ਬਣਤਰ hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj) { obj.collect_variation_indices (ਇਹ); ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }

  hb_ਸੈੱਟ_t *layout_variation_indices;
  ਅਚਲ hb_ਸੈੱਟ_t *glyph_ਸੈੱਟ;
  ਅਚਲ hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *condition_ਮੁੱਲਾਂ = ਖਾਲੀ_ਸੂਚਕ;

  hb_collect_variation_indices_context_t (hb_ਸੈੱਟ_t *layout_variation_indices_,
					  ਅਚਲ hb_ਸੈੱਟ_t *glyph_ਸੈੱਟ_2,
					  ਅਚਲ hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_ਸੈੱਟ (glyph_ਸੈੱਟ_2),
					gpos_lookups (gpos_lookups_) {}
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray>
ਬਣਤਰ subset_ਵਿੱਥ_array_t
{
  subset_ਵਿੱਥ_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& ਬਾਹਰਲਾ_4,
			 ਅਚਲ ਖਾਲੀ *ਅਧਾਰ_2) : subset_context (subset_context_),
					      ਬਾਹਰਲਾ (ਬਾਹਰਲਾ_4), ਅਧਾਰ (ਅਧਾਰ_2) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (T&& ਵਿੱਥ)
  {
    ਸਵੈਚਾਲਿਤ snap = subset_context->serializer->snapshot ();
    ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ.serialize_append (subset_context->serializer);
    ਜੇ (unlikely (!o)) ਵਾਪਸ ਝੂਠ;
    ਤਾਰਕਿਕ ret = o->serialize_subset (subset_context, ਵਿੱਥ, ਅਧਾਰ);
    ਜੇ (!ret)
    {
      ਬਾਹਰਲਾ.ਕੱਢੋ ();
      subset_context->serializer->revert (snap);
    }
    ਵਾਪਸ ret;
  }

  ਨਿਜੀ:
  hb_subset_context_t *subset_context;
  OutputArray &ਬਾਹਰਲਾ;
  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
};


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray, ਕਿਸਮ_ਨਾਂ Arg>
ਬਣਤਰ subset_ਵਿੱਥ_array_arg_t
{
  subset_ਵਿੱਥ_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& ਬਾਹਰਲਾ_4,
			     ਅਚਲ ਖਾਲੀ *ਅਧਾਰ_2,
			     Arg &&arg_) : subset_context (subset_context_), ਬਾਹਰਲਾ (ਬਾਹਰਲਾ_4),
					  ਅਧਾਰ (ਅਧਾਰ_2), arg (arg_) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (T&& ਵਿੱਥ)
  {
    ਸਵੈਚਾਲਿਤ snap = subset_context->serializer->snapshot ();
    ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ.serialize_append (subset_context->serializer);
    ਜੇ (unlikely (!o)) ਵਾਪਸ ਝੂਠ;
    ਤਾਰਕਿਕ ret = o->serialize_subset (subset_context, ਵਿੱਥ, ਅਧਾਰ, arg);
    ਜੇ (!ret)
    {
      ਬਾਹਰਲਾ.ਕੱਢੋ ();
      subset_context->serializer->revert (snap);
    }
    ਵਾਪਸ ret;
  }

  ਨਿਜੀ:
  hb_subset_context_t *subset_context;
  OutputArray &ਬਾਹਰਲਾ;
  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
ਬਣਤਰ
{
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray>
  subset_ਵਿੱਥ_array_t<OutputArray>
  ਸੰਚਾਲਕ () (hb_subset_context_t *subset_context, OutputArray& ਬਾਹਰਲਾ,
	       ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  { ਵਾਪਸ subset_ਵਿੱਥ_array_t<OutputArray> (subset_context, ਬਾਹਰਲਾ, ਅਧਾਰ); }

  /* Variant with one extra argument passed to serialize_subset */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray, ਕਿਸਮ_ਨਾਂ Arg>
  subset_ਵਿੱਥ_array_arg_t<OutputArray, Arg>
  ਸੰਚਾਲਕ () (hb_subset_context_t *subset_context, OutputArray& ਬਾਹਰਲਾ,
	       ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, Arg &&arg) ਅਚਲ
  { ਵਾਪਸ subset_ਵਿੱਥ_array_arg_t<OutputArray, Arg> (subset_context, ਬਾਹਰਲਾ, ਅਧਾਰ, arg); }
}
HB_FUNCOBJ (subset_ਵਿੱਥ_array);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray>
ਬਣਤਰ subset_ਰਿਕਾਰਡ_array_t
{
  subset_ਰਿਕਾਰਡ_array_t (hb_subset_layout_context_t *c_, OutputArray* ਬਾਹਰਲਾ_4,
			 ਅਚਲ ਖਾਲੀ *ਅਧਾਰ_2) : subset_layout_context (c_),
					      ਬਾਹਰਲਾ (ਬਾਹਰਲਾ_4), ਅਧਾਰ (ਅਧਾਰ_2) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ
  ਸੰਚਾਲਕ () (T&& ਰਿਕਾਰਡ)
  {
    ਸਵੈਚਾਲਿਤ snap = subset_layout_context->subset_context->serializer->snapshot ();
    ਤਾਰਕਿਕ ret = ਰਿਕਾਰਡ.subset (subset_layout_context, ਅਧਾਰ);
    ਜੇ (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    ਨਹੀਂਤਾਂ ਬਾਹਰਲਾ->len++;
  }

  ਨਿਜੀ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *ਬਾਹਰਲਾ;
  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray, ਕਿਸਮ_ਨਾਂ Arg>
ਬਣਤਰ subset_ਰਿਕਾਰਡ_array_arg_t
{
  subset_ਰਿਕਾਰਡ_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* ਬਾਹਰਲਾ_4,
			     ਅਚਲ ਖਾਲੀ *ਅਧਾਰ_2,
			     Arg &&arg_) : subset_layout_context (c_),
					   ਬਾਹਰਲਾ (ਬਾਹਰਲਾ_4), ਅਧਾਰ (ਅਧਾਰ_2), arg (arg_) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ
  ਸੰਚਾਲਕ () (T&& ਰਿਕਾਰਡ)
  {
    ਸਵੈਚਾਲਿਤ snap = subset_layout_context->subset_context->serializer->snapshot ();
    ਤਾਰਕਿਕ ret = ਰਿਕਾਰਡ.subset (subset_layout_context, ਅਧਾਰ, arg);
    ਜੇ (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    ਨਹੀਂਤਾਂ ਬਾਹਰਲਾ->len++;
  }

  ਨਿਜੀ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *ਬਾਹਰਲਾ;
  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
ਬਣਤਰ
{
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray>
  subset_ਰਿਕਾਰਡ_array_t<OutputArray>
  ਸੰਚਾਲਕ () (hb_subset_layout_context_t *c, OutputArray* ਬਾਹਰਲਾ,
	       ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  { ਵਾਪਸ subset_ਰਿਕਾਰਡ_array_t<OutputArray> (c, ਬਾਹਰਲਾ, ਅਧਾਰ); }

  /* Variant with one extra argument passed to subset */
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray, ਕਿਸਮ_ਨਾਂ Arg>
  subset_ਰਿਕਾਰਡ_array_arg_t<OutputArray, Arg>
  ਸੰਚਾਲਕ () (hb_subset_layout_context_t *c, OutputArray* ਬਾਹਰਲਾ,
               ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, Arg &&arg) ਅਚਲ
  { ਵਾਪਸ subset_ਰਿਕਾਰਡ_array_arg_t<OutputArray, Arg> (c, ਬਾਹਰਲਾ, ਅਧਾਰ, arg); }
}
HB_FUNCOBJ (subset_ਰਿਕਾਰਡ_array);


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray>
ਬਣਤਰ serialize_math_ਰਿਕਾਰਡ_array_t
{
  serialize_math_ਰਿਕਾਰਡ_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& ਬਾਹਰਲਾ_4,
                         ਅਚਲ ਖਾਲੀ *ਅਧਾਰ_2,
                         ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              ਬਾਹਰਲਾ (ਬਾਹਰਲਾ_4), ਅਧਾਰ (ਅਧਾਰ_2), variation_map (variation_map_) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (T&& ਰਿਕਾਰਡ)
  {
    ਜੇ (!serialize_context->copy (ਰਿਕਾਰਡ, ਅਧਾਰ, variation_map)) ਵਾਪਸ ਝੂਠ;
    ਬਾਹਰਲਾ.len++;
    ਵਾਪਸ ਸੱਚ;
  }

  ਨਿਜੀ:
  hb_serialize_context_t *serialize_context;
  OutputArray &ਬਾਹਰਲਾ;
  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
ਬਣਤਰ
{
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ OutputArray>
  serialize_math_ਰਿਕਾਰਡ_array_t<OutputArray>
  ਸੰਚਾਲਕ () (hb_serialize_context_t *serialize_context, OutputArray& ਬਾਹਰਲਾ,
               ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
               ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *variation_map) ਅਚਲ
  { ਵਾਪਸ serialize_math_ਰਿਕਾਰਡ_array_t<OutputArray> (serialize_context, ਬਾਹਰਲਾ, ਅਧਾਰ, variation_map); }

}
HB_FUNCOBJ (serialize_math_ਰਿਕਾਰਡ_array);

/*
 *
 * OpenType Layout Common Table Formats
 *
 */


/*
 * Script, ScriptList, LangSys, Feature, FeatureList, Lookup, LookupList
 */

ਬਣਤਰ ਸੂਚਕਾਂਕ_Array : Array16_ਦਾ<ਸੂਚਕਾਂਕ>
{
  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_map_t *indexes) ਅਚਲ
  { ਵਾਪਸ hb_any (*ਇਹ, indexes); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    ਜੇ (!it) ਵਾਪਸ;
    ਜੇ (unlikely (!c->extend_min ((*ਇਹ)))) ਵਾਪਸ;

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : it)
    {
      ਜੇ (!l->ਦੇਖੋ_Lookup_ਸੂਚਕਾਂਕ()) ਰੋਕੋ;

      ਸੂਚਕਾਂਕ i;
      i = _;
      c->copy (i);
      ਇਹ->len++;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_indexes (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਗਿਣਤੀ_3 /* IN/OUT */,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (ਗਿਣਤੀ_3 && _indexes)
    {
      + ਇਹ->as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਗਿਣਤੀ_3)
      | hb_sink (hb_array (_indexes, *ਗਿਣਤੀ_3))
      ;
    }
    ਵਾਪਸ ਇਹ->len;
  }

  ਖਾਲੀ ਜੋੜੋ_indexes_ਮੰਜ਼ਿਲ (hb_ਸੈੱਟ_t* output /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    output->ਜੋੜੋ_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
ਬਣਤਰ Feature_Params_ਮਾਪ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ))) return_trace (ਝੂਠ);
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

    ਜੇ (!design_ਮਾਪ)
      return_trace (ਝੂਠ);
    ਨਹੀਂਤਾਂ ਜੇ (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     range_ਸ਼ੁਰੂ == 0 &&
	     range_ਸਿਰਾ == 0)
      return_trace (ਸੱਚ);
    ਨਹੀਂਤਾਂ ਜੇ (design_ਮਾਪ < range_ਸ਼ੁਰੂ ||
	     design_ਮਾਪ > range_ਸਿਰਾ ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (ਝੂਠ);
    ਨਹੀਂਤਾਂ
      return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_name_ids (hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { nameids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (subfamilyNameID); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    return_trace ((ਤਾਰਕਿਕ) c->serializer->embed (*ਇਹ));
  }

  HBUINT16	design_ਮਾਪ;	/* Represents the design size in 720/inch
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
  HBUINT16	range_ਸ਼ੁਰੂ;	/* Large end of the recommended usage range
				 * (inclusive), stored in 720/inch units
				 * (decipoints). */
  HBUINT16	range_ਸਿਰਾ;	/* Small end of the recommended usage range
				   (exclusive), stored in 720/inch units
				 * (decipoints). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
ਬਣਤਰ Feature_Params_Stylistic_ਸੈੱਟ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ collect_name_ids (hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { nameids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (uiNameID); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    return_trace ((ਤਾਰਕਿਕ) c->serializer->embed (*ਇਹ));
  }

  HBUINT16	ਸੰਸਕਰਣ;	/* (set to 0): This corresponds to a “minor”
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
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
ਬਣਤਰ FeatureParamsCharacterVariants
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ
  get_characters (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ *char_ਗਿਣਤੀ, hb_codepoint_t *chars) ਅਚਲ
  {
    ਜੇ (char_ਗਿਣਤੀ && chars)
    {
      + characters.as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, char_ਗਿਣਤੀ)
      | hb_sink (hb_array (chars, *char_ਗਿਣਤੀ))
      ;
    }
    ਵਾਪਸ characters.len;
  }

  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (characters.len, HBUINT24::static_ਮਾਪ, min_ਮਾਪ); }

  ਖਾਲੀ collect_name_ids (hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (featUILableNameID) nameids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (featUILableNameID);
    ਜੇ (feat_UITooltip_ਪਾਠ_Name_ID) nameids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (feat_UITooltip_ਪਾਠ_Name_ID);
    ਜੇ (sample_ਪਾਠ_Name_ID) nameids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ (sample_ਪਾਠ_Name_ID);

    ਜੇ (!ਪਹਿਲਾ_Param_UILabel_Name_ID || !ਅੰਕ_Named_Parameters || ਅੰਕ_Named_Parameters >= 0x7FFF)
      ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_name_id = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਪਹਿਲਾ_Param_UILabel_Name_ID + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕ_Named_Parameters - 1;
    nameids_ਮੰਜ਼ਿਲ_retain->ਜੋੜੋ_range (ਪਹਿਲਾ_Param_UILabel_Name_ID, ਆਖਰੀ_name_id);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    return_trace ((ਤਾਰਕਿਕ) c->serializer->embed (*ਇਹ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  characters.sanitize (c));
  }

  HBUINT16	format;			/* Format number is set to 0. */
  NameID	featUILableNameID;	/* The ‘name’ table name ID that
					 * specifies a string (or strings,
					 * for multiple languages) for a
					 * user-interface label for this
					 * feature. (May be NULL.) */
  NameID	feat_UITooltip_ਪਾਠ_Name_ID;/* The ‘name’ table name ID that
					 * specifies a string (or strings,
					 * for multiple languages) that an
					 * application can use for tooltip
					 * text for this feature. (May be
					 * nullptr.) */
  NameID	sample_ਪਾਠ_Name_ID;	/* The ‘name’ table name ID that
					 * specifies sample text that
					 * illustrates the effect of this
					 * feature. (May be NULL.) */
  HBUINT16	ਅੰਕ_Named_Parameters;	/* Number of named parameters. (May
					 * be zero.) */
  NameID	ਪਹਿਲਾ_Param_UILabel_Name_ID;/* The first ‘name’ table name ID
					 * used to specify strings for
					 * user-interface labels for the
					 * feature parameters. (Must be zero
					 * if numParameters is zero.) */
  Array16_ਦਾ<HBUINT24>
		characters;		/* Array of the Unicode Scalar Value
					 * of the characters for which this
					 * feature provides glyph variants.
					 * (May be zero.) */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (14, characters);
};

ਬਣਤਰ FeatureParams
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, hb_ਟੈਗ_t ਟੈਗ_2) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_FEATURE_PARAMS
    ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (ਟੈਗ_2 == HB_ਟੈਗ ('s','i','z','e') && hb_barrier ())
      return_trace (u.ਮਾਪ.sanitize (c));
    ਜੇ ((ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_ਸੈੱਟ.sanitize (c));
    ਜੇ ((ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_name_ids (hb_ਟੈਗ_t ਟੈਗ_2, hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_FEATURE_PARAMS
    ਵਾਪਸ;
#ਜੇ_ਅੰਤ
    ਜੇ (ਟੈਗ_2 == HB_ਟੈਗ ('s','i','z','e') && hb_barrier ())
      ਵਾਪਸ (u.ਮਾਪ.collect_name_ids (nameids_ਮੰਜ਼ਿਲ_retain));
    ਜੇ ((ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('s','s','\0','\0')) /* ssXX */
      ਵਾਪਸ (u.stylistic_ਸੈੱਟ.collect_name_ids (nameids_ਮੰਜ਼ਿਲ_retain));
    ਜੇ ((ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('c','v','\0','\0')) /* cvXX */
      ਵਾਪਸ (u.characterVariants.collect_name_ids (nameids_ਮੰਜ਼ਿਲ_retain));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c, ਅਚਲ ਟੈਗ* ਟੈਗ_2) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_FEATURE_PARAMS
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
    TRACE_SUBSET (ਇਹ);
    ਜੇ (!ਟੈਗ_2) return_trace (ਝੂਠ);
    ਜੇ (*ਟੈਗ_2 == HB_ਟੈਗ ('s','i','z','e') && hb_barrier ())
      return_trace (u.ਮਾਪ.subset (c));
    ਜੇ ((*ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_ਸੈੱਟ.subset (c));
    ਜੇ ((*ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (ਝੂਠ);
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LAYOUT_FEATURE_PARAMS
  ਅਚਲ Feature_Params_ਮਾਪ& get_ਮਾਪ_params (hb_ਟੈਗ_t ਟੈਗ_2) ਅਚਲ
  {
    ਜੇ (ਟੈਗ_2 == HB_ਟੈਗ ('s','i','z','e') && hb_barrier ())
      ਵਾਪਸ u.ਮਾਪ;
    ਵਾਪਸ Null (Feature_Params_ਮਾਪ);
  }
  ਅਚਲ Feature_Params_Stylistic_ਸੈੱਟ& get_stylistic_ਸੈੱਟ_params (hb_ਟੈਗ_t ਟੈਗ_2) ਅਚਲ
  {
    ਜੇ ((ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('s','s','\0','\0')) /* ssXX */
      ਵਾਪਸ u.stylistic_ਸੈੱਟ;
    ਵਾਪਸ Null (Feature_Params_Stylistic_ਸੈੱਟ);
  }
  ਅਚਲ FeatureParamsCharacterVariants& get_character_variants_params (hb_ਟੈਗ_t ਟੈਗ_2) ਅਚਲ
  {
    ਜੇ ((ਟੈਗ_2 & 0xFFFF0000u) == HB_ਟੈਗ ('c','v','\0','\0')) /* cvXX */
      ਵਾਪਸ u.characterVariants;
    ਵਾਪਸ Null (FeatureParamsCharacterVariants);
  }
#ਜੇ_ਅੰਤ

  ਨਿਜੀ:
  ਸੰਘ {
  Feature_Params_ਮਾਪ			ਮਾਪ;
  Feature_Params_Stylistic_ਸੈੱਟ		stylistic_ਸੈੱਟ;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (0);
};

ਬਣਤਰ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t {
  hb_ਟੈਗ_t ਟੈਗ_2;
  ਅਚਲ ਖਾਲੀ *list_ਅਧਾਰ;
};

ਬਣਤਰ Feature
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lookup_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ lookup_ਸੂਚਕਾਂਕ_2.len; }
  hb_ਟੈਗ_t get_lookup_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ lookup_ਸੂਚਕਾਂਕ_2[i]; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lookup_indexes (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_ਗਿਣਤੀ /* IN/OUT */,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lookup_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ lookup_ਸੂਚਕਾਂਕ_2.get_indexes (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ, lookup_ਗਿਣਤੀ, lookup_ਟੈਗ_ਸੂਚੀ); }
  ਖਾਲੀ ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  { lookup_ਸੂਚਕਾਂਕ_2.ਜੋੜੋ_indexes_ਮੰਜ਼ਿਲ (lookup_indexes); }

  ਅਚਲ FeatureParams &get_feature_params () ਅਚਲ
  { ਵਾਪਸ ਇਹ+featureParams; }

  ਤਾਰਕਿਕ intersects_lookup_indexes (ਅਚਲ hb_map_t *lookup_indexes) ਅਚਲ
  { ਵਾਪਸ lookup_ਸੂਚਕਾਂਕ_2.intersects (lookup_indexes); }

  ਖਾਲੀ collect_name_ids (hb_ਟੈਗ_t ਟੈਗ_2, hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (featureParams)
      get_feature_params ().collect_name_ids (ਟੈਗ_2, nameids_ਮੰਜ਼ਿਲ_retain);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ਅਚਲ ਟੈਗ                   *ਟੈਗ_2 = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->featureParams.serialize_subset (c, featureParams, ਇਹ, ਟੈਗ_2);

    hb::shared_ptr<hb_ਸੈੱਟ_t> *lookup_variations;
    ਜੇ (l->cur_feature_ਸੂਚਕਾਂਕ != ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ &&
	l->lookup_variations->has (l->cur_feature_ਸੂਚਕਾਂਕ, &lookup_variations))
    {
      ਸਵੈਚਾਲਿਤ it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_ਸੂਚਕਾਂਕ_map)
      | hb_map (l->lookup_ਸੂਚਕਾਂਕ_map)
      ;
      ਬਾਹਰਲਾ->lookup_ਸੂਚਕਾਂਕ_2.serialize (c->serializer, l, it);
    }
    ਨਹੀਂਤਾਂ
    {
      ਸਵੈਚਾਲਿਤ it =
      + hb_iter (lookup_ਸੂਚਕਾਂਕ_2)
      | hb_filter (l->lookup_ਸੂਚਕਾਂਕ_map)
      | hb_map (l->lookup_ਸੂਚਕਾਂਕ_map)
      ;
      ਬਾਹਰਲਾ->lookup_ਸੂਚਕਾਂਕ_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t *ਘੇਰਾ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  featureParams.sanitize (c, ਇਹ, ਘੇਰਾ ? ਘੇਰਾ->ਟੈਗ_2 : HB_ਟੈਗ_NONE) &&
		  lookup_ਸੂਚਕਾਂਕ_2.sanitize (c));
  }

  Offset16_ਮੰਜ਼ਿਲ<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  ਸੂਚਕਾਂਕ_Array	 lookup_ਸੂਚਕਾਂਕ_2;	/* Array of LookupList indices */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (4, lookup_ਸੂਚਕਾਂਕ_2);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ ਰਿਕਾਰਡ_2
{
  ਪੂਰਨ cmp (hb_ਟੈਗ_t a) ਅਚਲ { ਵਾਪਸ ਟੈਗ_2.cmp (a); }

  ਤਾਰਕਿਕ subset (hb_subset_layout_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, ਅਚਲ ਖਾਲੀ *f_sub = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->subset_context->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (!f_sub)
      return_trace (ਬਾਹਰਲਾ->ਵਿੱਥ.serialize_subset (c->subset_context, ਵਿੱਥ, ਅਧਾਰ, c, &ਟੈਗ_2));

    ਅਚਲ Feature& f = *reinterpret_cast<ਅਚਲ Feature *> (f_sub);
    ਸਵੈਚਾਲਿਤ *s = c->subset_context->serializer;
    s->push ();

    ਬਾਹਰਲਾ->ਵਿੱਥ = 0;
    ਤਾਰਕਿਕ ret = f.subset (c->subset_context, c, &ਟੈਗ_2);
    ਜੇ (ret)
      s->ਜੋੜੋ_link (ਬਾਹਰਲਾ->ਵਿੱਥ, s->ਕੱਢੋ_pack ());
    ਨਹੀਂਤਾਂ
      s->ਕੱਢੋ_discard ();

    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਅਚਲ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t ਘੇਰਾ = {ਟੈਗ_2, ਅਧਾਰ};
    return_trace (c->check_struct (ਇਹ) &&
		  ਵਿੱਥ.sanitize (c, ਅਧਾਰ, &ਘੇਰਾ));
  }

  ਟੈਗ           ਟੈਗ_2;            /* 4-byte Tag identifier */
  Offset16_ਮੰਜ਼ਿਲ<Type>
                ਵਿੱਥ;         /* Offset from beginning of object holding
                                 * the Record */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ ਰਿਕਾਰਡ_Array_ਦਾ : Sorted_Array16_ਦਾ<ਰਿਕਾਰਡ_2<Type>>
{
  ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Type>& get_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ (*ਇਹ)[i].ਵਿੱਥ; }
  Offset16_ਮੰਜ਼ਿਲ<Type>& get_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  { ਵਾਪਸ (*ਇਹ)[i].ਵਿੱਥ; }
  ਅਚਲ ਟੈਗ& get_ਟੈਗ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ (*ਇਹ)[i].ਟੈਗ_2; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਰਿਕਾਰਡ_ਗਿਣਤੀ /* IN/OUT */,
                         hb_ਟੈਗ_t     *ਰਿਕਾਰਡ_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (ਰਿਕਾਰਡ_ਗਿਣਤੀ && ਰਿਕਾਰਡ_ਟੈਗ_ਸੂਚੀ)
    {
      + ਇਹ->as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, ਰਿਕਾਰਡ_ਗਿਣਤੀ)
      | hb_map (&ਰਿਕਾਰਡ_2<Type>::ਟੈਗ_2)
      | hb_sink (hb_array (ਰਿਕਾਰਡ_ਟੈਗ_ਸੂਚੀ, *ਰਿਕਾਰਡ_ਗਿਣਤੀ))
      ;
    }
    ਵਾਪਸ ਇਹ->len;
  }
  ਤਾਰਕਿਕ ਲੱਭੋ_ਸੂਚਕਾਂਕ (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
    ਵਾਪਸ ਇਹ->bfind (ਟੈਗ_2, ਸੂਚਕਾਂਕ_2, HB_NOT_ਮਿਲਿਆ_STORE, ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ);
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ ਰਿਕਾਰਡ_List_ਦਾ : ਰਿਕਾਰਡ_Array_ਦਾ<Type>
{
  ਅਚਲ Type& ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ ਇਹ+ਇਹ->get_ਵਿੱਥ (i); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    + ਇਹ->iter ()
    | hb_apply (subset_ਰਿਕਾਰਡ_array (l, ਬਾਹਰਲਾ, ਇਹ))
    ;
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਰਿਕਾਰਡ_Array_ਦਾ<Type>::sanitize (c, ਇਹ));
  }
};

ਬਣਤਰ ਰਿਕਾਰਡ_List_ਦਾ_Feature : ਰਿਕਾਰਡ_List_ਦਾ<Feature>
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    + hb_enumerate (*ਇਹ)
    | hb_filter (l->feature_ਸੂਚਕਾਂਕ_map, hb_ਪਹਿਲਾ)
    | hb_apply ([l, ਬਾਹਰਲਾ, ਇਹ] (ਅਚਲ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ ਰਿਕਾਰਡ_2<Feature>&>& _)
                {
                  ਅਚਲ Feature *f_sub = ਖਾਲੀ_ਸੂਚਕ;
                  ਅਚਲ Feature **f = ਖਾਲੀ_ਸੂਚਕ;
                  ਜੇ (l->feature_substitutes_map->has (_.ਪਹਿਲਾ, &f))
                    f_sub = *f;

                  l->cur_feature_ਸੂਚਕਾਂਕ = _.ਪਹਿਲਾ;
                  subset_ਰਿਕਾਰਡ_array (l, ਬਾਹਰਲਾ, ਇਹ, f_sub) (_.second);
                  l->cur_feature_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ;
                })
    ;

    return_trace (ਸੱਚ);
  }
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਰਿਕਾਰਡ_List_ਦਾ<Feature> FeatureList;


ਬਣਤਰ LangSys
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_feature_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ feature_ਸੂਚਕਾਂਕ_2.len; }
  hb_ਟੈਗ_t get_feature_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ feature_ਸੂਚਕਾਂਕ_2[i]; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_feature_indexes (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_ਗਿਣਤੀ_2 /* IN/OUT */,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *feature_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ feature_ਸੂਚਕਾਂਕ_2.get_indexes (ਸ਼ੁਰੂ_ਵਿੱਥ, feature_ਗਿਣਤੀ_2, feature_indexes); }
  ਖਾਲੀ ਜੋੜੋ_feature_indexes_ਮੰਜ਼ਿਲ (hb_ਸੈੱਟ_t *feature_indexes) ਅਚਲ
  { feature_ਸੂਚਕਾਂਕ_2.ਜੋੜੋ_indexes_ਮੰਜ਼ਿਲ (feature_indexes); }

  ਤਾਰਕਿਕ has_required_feature () ਅਚਲ { ਵਾਪਸ req_Feature_ਸੂਚਕਾਂਕ != 0xFFFFu; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_required_feature_ਸੂਚਕਾਂਕ () ਅਚਲ
  {
    ਜੇ (req_Feature_ਸੂਚਕਾਂਕ == 0xFFFFu)
      ਵਾਪਸ ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ;
   ਵਾਪਸ req_Feature_ਸੂਚਕਾਂਕ;
  }

  LangSys* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed (*ਇਹ));
  }

  ਤਾਰਕਿਕ compare (ਅਚਲ LangSys& o, ਅਚਲ hb_map_t *feature_ਸੂਚਕਾਂਕ_map) ਅਚਲ
  {
    ਜੇ (req_Feature_ਸੂਚਕਾਂਕ != o.req_Feature_ਸੂਚਕਾਂਕ)
      ਵਾਪਸ ਝੂਠ;

    ਸਵੈਚਾਲਿਤ iter =
    + hb_iter (feature_ਸੂਚਕਾਂਕ_2)
    | hb_filter (feature_ਸੂਚਕਾਂਕ_map)
    | hb_map (feature_ਸੂਚਕਾਂਕ_map)
    ;

    ਸਵੈਚਾਲਿਤ o_iter =
    + hb_iter (o.feature_ਸੂਚਕਾਂਕ_2)
    | hb_filter (feature_ਸੂਚਕਾਂਕ_map)
    | hb_map (feature_ਸੂਚਕਾਂਕ_map)
    ;

    ਲਈ (; iter && o_iter; iter++, o_iter++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ a = *iter;
      ਬਿਨਾਨਿਸ਼ਾਨੀ b = *o_iter;
      ਜੇ (a != b) ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (iter || o_iter) ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ collect_features (hb_prune_langsys_context_t *c) ਅਚਲ
  {
    ਜੇ (!has_required_feature () && !get_feature_ਗਿਣਤੀ ()) ਵਾਪਸ;
    ਜੇ (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_ਸੂਚਕਾਂਕ))
      c->new_feature_indexes->ਜੋੜੋ (get_required_feature_ਸੂਚਕਾਂਕ ());

    + hb_iter (feature_ਸੂਚਕਾਂਕ_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ਅਚਲ ਟੈਗ                  *ਟੈਗ_2 = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਅਚਲ uint32_t *v;
    ਬਾਹਰਲਾ->req_Feature_ਸੂਚਕਾਂਕ = l->feature_map_w_duplicates->has (req_Feature_ਸੂਚਕਾਂਕ, &v) ? *v : 0xFFFFu;

    ਜੇ (!l->ਦੇਖੋ_Feature_ਸੂਚਕਾਂਕ (feature_ਸੂਚਕਾਂਕ_2.len))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ it =
    + hb_iter (feature_ਸੂਚਕਾਂਕ_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    ਤਾਰਕਿਕ ret = ਤਾਰਕਿਕ (it);
    ਬਾਹਰਲਾ->feature_ਸੂਚਕਾਂਕ_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t * = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && feature_ਸੂਚਕਾਂਕ_2.sanitize (c));
  }

  Offset16	lookup_ਕ੍ਰਮ_Z;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	req_Feature_ਸੂਚਕਾਂਕ;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  ਸੂਚਕਾਂਕ_Array	feature_ਸੂਚਕਾਂਕ_2;	/* Array of indices into the FeatureList */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (6, feature_ਸੂਚਕਾਂਕ_2);
};
DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ (OT, LangSys);

ਬਣਤਰ Script
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lang_sys_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ langSys.len; }
  ਅਚਲ ਟੈਗ& get_lang_sys_ਟੈਗ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ langSys.get_ਟੈਗ (i); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lang_sys_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lang_sys_ਗਿਣਤੀ /* IN/OUT */,
				  hb_ਟੈਗ_t     *lang_sys_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ langSys.get_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, lang_sys_ਗਿਣਤੀ, lang_sys_ਟੈਗ_ਸੂਚੀ); }
  ਅਚਲ LangSys& get_lang_sys (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
    ਜੇ (i == ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ) ਵਾਪਸ get_default_lang_sys ();
    ਵਾਪਸ ਇਹ+langSys[i].ਵਿੱਥ;
  }
  ਤਾਰਕਿਕ ਲੱਭੋ_lang_sys_ਸੂਚਕਾਂਕ (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
  { ਵਾਪਸ langSys.ਲੱਭੋ_ਸੂਚਕਾਂਕ (ਟੈਗ_2, ਸੂਚਕਾਂਕ_2); }

  ਤਾਰਕਿਕ has_default_lang_sys () ਅਚਲ           { ਵਾਪਸ defaultLangSys != 0; }
  ਅਚਲ LangSys& get_default_lang_sys () ਅਚਲ { ਵਾਪਸ ਇਹ+defaultLangSys; }

  ਖਾਲੀ prune_langsys (hb_prune_langsys_context_t *c,
                      ਬਿਨਾਨਿਸ਼ਾਨੀ script_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (!has_default_lang_sys () && !get_lang_sys_ਗਿਣਤੀ ()) ਵਾਪਸ;
    ਜੇ (!c->ਦੇਖੋ_Script ()) ਵਾਪਸ;

    ਜੇ (!c->script_langsys_map->has (script_ਸੂਚਕਾਂਕ))
    {
      ਜੇ (unlikely (!c->script_langsys_map->ਸੈੱਟ (script_ਸੂਚਕਾਂਕ, hb::unique_ptr<hb_ਸੈੱਟ_t> {hb_ਸੈੱਟ_create ()})))
	ਵਾਪਸ;
    }

    ਜੇ (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ਅਚਲ LangSys& d = get_default_lang_sys ();
      ਜੇ (c->ਦੇਖੋ_Langsys (d.get_feature_ਗਿਣਤੀ ())) {
        d.collect_features (c);
      }

      ਲਈ (ਸਵੈਚਾਲਿਤ _ : + hb_enumerate (langSys))
      {
        ਅਚਲ LangSys& l = ਇਹ+_.second.ਵਿੱਥ;
        ਜੇ (!c->ਦੇਖੋ_Langsys (l.get_feature_ਗਿਣਤੀ ())) ਜਾਰੀ;
        ਜੇ (l.compare (d, c->duplicate_feature_map)) ਜਾਰੀ;

        l.collect_features (c);
        c->script_langsys_map->get (script_ਸੂਚਕਾਂਕ)->ਜੋੜੋ (_.ਪਹਿਲਾ);
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ _ : + hb_enumerate (langSys))
      {
        ਅਚਲ LangSys& l = ਇਹ+_.second.ਵਿੱਥ;
        ਜੇ (!c->ਦੇਖੋ_Langsys (l.get_feature_ਗਿਣਤੀ ())) ਜਾਰੀ;
        l.collect_features (c);
        c->script_langsys_map->get (script_ਸੂਚਕਾਂਕ)->ਜੋੜੋ (_.ਪਹਿਲਾ);
      }
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ਅਚਲ ਟੈਗ                   *ਟੈਗ_2) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (!l->ਦੇਖੋ_Script ()) return_trace (ਝੂਠ);
    ਜੇ (ਟੈਗ_2 && !c->plan->layout_scripts.has (*ਟੈਗ_2))
      ਵਾਪਸ ਝੂਠ;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ defaultLang = ਝੂਠ;
    ਜੇ (has_default_lang_sys ())
    {
      c->serializer->push ();
      ਅਚਲ LangSys& ls = ਇਹ+defaultLangSys;
      ਤਾਰਕਿਕ ret = ls.subset (c, l);
      ਜੇ (!ret && ਟੈਗ_2 && *ਟੈਗ_2 != HB_ਟੈਗ ('D', 'F', 'L', 'T'))
      {
	c->serializer->ਕੱਢੋ_discard ();
	ਬਾਹਰਲਾ->defaultLangSys = 0;
      }
      ਨਹੀਂਤਾਂ
      {
	c->serializer->ਜੋੜੋ_link (ਬਾਹਰਲਾ->defaultLangSys, c->serializer->ਕੱਢੋ_pack ());
	defaultLang = ਸੱਚ;
      }
    }

    ਅਚਲ hb_ਸੈੱਟ_t *ਸਰਗਰਮ_langsys = l->script_langsys_map->get (l->cur_script_ਸੂਚਕਾਂਕ);
    ਜੇ (ਸਰਗਰਮ_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (ਸਰਗਰਮ_langsys, hb_ਪਹਿਲਾ)
      | hb_map (hb_ਦੂਜਾ)
      | hb_filter ([=] (ਅਚਲ ਰਿਕਾਰਡ_2<LangSys>& ਰਿਕਾਰਡ) {ਵਾਪਸ l->ਦੇਖੋ_Lang_Sys (); })
      | hb_apply (subset_ਰਿਕਾਰਡ_array (l, &(ਬਾਹਰਲਾ->langSys), ਇਹ))
      ;
    }

    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->langSys.len) || defaultLang || l->ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t * = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (defaultLangSys.sanitize (c, ਇਹ) && langSys.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  ਰਿਕਾਰਡ_Array_ਦਾ<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (4, langSys);
};

ਬਣਤਰ ਰਿਕਾਰਡ_List_ਦਾ_Script : ਰਿਕਾਰਡ_List_ਦਾ<Script>
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਲਈ (ਸਵੈਚਾਲਿਤ _ : + hb_enumerate (*ਇਹ))
    {
      ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
      l->cur_script_ਸੂਚਕਾਂਕ = _.ਪਹਿਲਾ;
      ਤਾਰਕਿਕ ret = _.second.subset (l, ਇਹ);
      ਜੇ (!ret) c->serializer->revert (snap);
      ਨਹੀਂਤਾਂ ਬਾਹਰਲਾ->len++;
    }

    return_trace (ਸੱਚ);
  }
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਰਿਕਾਰਡ_List_ਦਾ_Script ScriptList;



ਬਣਤਰ LookupFlag : HBUINT16
{
  ਗਿਣਤੀ Flags {
    ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ		= 0x0001u,
    Ignore_ਅਧਾਰ_Glyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    Use_Mark_Filtering_ਸੈੱਟ	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
ਨਾਮਾਕਾਸ਼ OT {

ਬਣਤਰ Lookup
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_subtable_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ sub_ਸਾਰਣੀ.len; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TSub_ਸਾਰਣੀ>
  ਅਚਲ Array16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<TSub_ਸਾਰਣੀ>& get_subtables () ਅਚਲ
  { ਵਾਪਸ reinterpret_cast<ਅਚਲ Array16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<TSub_ਸਾਰਣੀ> &> (sub_ਸਾਰਣੀ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TSub_ਸਾਰਣੀ>
  Array16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<TSub_ਸਾਰਣੀ>& get_subtables ()
  { ਵਾਪਸ reinterpret_cast<Array16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<TSub_ਸਾਰਣੀ> &> (sub_ਸਾਰਣੀ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TSub_ਸਾਰਣੀ>
  ਅਚਲ TSub_ਸਾਰਣੀ& get_subtable (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ ਇਹ+get_subtables<TSub_ਸਾਰਣੀ> ()[i]; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TSub_ਸਾਰਣੀ>
  TSub_ਸਾਰਣੀ& get_subtable (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  { ਵਾਪਸ ਇਹ+get_subtables<TSub_ਸਾਰਣੀ> ()[i]; }

  size_t get_ਮਾਪ () ਅਚਲ
  {
    ਅਚਲ HBUINT16 &mark_Filtering_ਸੈੱਟ = StructAfter<ਅਚਲ HBUINT16> (sub_ਸਾਰਣੀ);
    ਜੇ (lookupFlag & LookupFlag::Use_Mark_Filtering_ਸੈੱਟ)
      ਵਾਪਸ (ਅਚਲ ਅੱਖਰ *) &StructAfter<ਅਚਲ ਅੱਖਰ> (mark_Filtering_ਸੈੱਟ) - (ਅਚਲ ਅੱਖਰ *) ਇਹ;
    ਵਾਪਸ (ਅਚਲ ਅੱਖਰ *) &mark_Filtering_ਸੈੱਟ - (ਅਚਲ ਅੱਖਰ *) ਇਹ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_type () ਅਚਲ { ਵਾਪਸ lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flag = lookupFlag;
    ਜੇ (unlikely (flag & LookupFlag::Use_Mark_Filtering_ਸੈੱਟ))
    {
      ਅਚਲ HBUINT16 &mark_Filtering_ਸੈੱਟ = StructAfter<HBUINT16> (sub_ਸਾਰਣੀ);
      flag += (mark_Filtering_ਸੈੱਟ << 16);
    }
    ਵਾਪਸ flag;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TSub_ਸਾਰਣੀ, ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_type = get_type ();
    TRACE_DISPATCH (ਇਹ, lookup_type);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = get_subtable_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++) {
      ਕਿਸਮ_ਨਾਂ context_t::return_t r = get_subtable<TSub_ਸਾਰਣੀ> (i).dispatch (c, lookup_type, ds...);
      ਜੇ (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_ਮੁੱਲ ());
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_type,
		  uint32_t lookup_props,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_subtables)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    ਜੇ (unlikely (!sub_ਸਾਰਣੀ.serialize (c, ਅੰਕ_subtables))) return_trace (ਝੂਠ);
    ਜੇ (lookupFlag & LookupFlag::Use_Mark_Filtering_ਸੈੱਟ)
    {
      ਜੇ (unlikely (!c->extend (ਇਹ))) return_trace (ਝੂਠ);
      HBUINT16 &mark_Filtering_ਸੈੱਟ = StructAfter<HBUINT16> (sub_ਸਾਰਣੀ);
      mark_Filtering_ਸੈੱਟ = lookup_props >> 16;
    }
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TSub_ਸਾਰਣੀ>
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->lookupType = lookupType;
    ਬਾਹਰਲਾ->lookupFlag = lookupFlag;

    ਅਚਲ hb_ਸੈੱਟ_t *glyphset = c->plan->glyphset_gsub ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_type = get_type ();
    + hb_iter (get_subtables <TSub_ਸਾਰਣੀ> ())
    | hb_filter ([ਇਹ, glyphset, lookup_type] (ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<TSub_ਸਾਰਣੀ> &_) { ਵਾਪਸ (ਇਹ+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->get_subtables<TSub_ਸਾਰਣੀ> (), ਇਹ, lookup_type))
    ;

    ਜੇ (lookupFlag & LookupFlag::Use_Mark_Filtering_ਸੈੱਟ)
    {
      ਅਚਲ HBUINT16 &mark_Filtering_ਸੈੱਟ = StructAfter<HBUINT16> (sub_ਸਾਰਣੀ);
      hb_codepoint_t *idx;
      ਜੇ (!c->plan->ਵਰਤਿਆ_mark_sets_map.has (mark_Filtering_ਸੈੱਟ, &idx))
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ new_flag = lookupFlag;
        new_flag &= ~LookupFlag::Use_Mark_Filtering_ਸੈੱਟ;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        ਬਾਹਰਲਾ->lookupFlag = new_flag;
      }
      ਨਹੀਂਤਾਂ
      {
        ਜੇ (unlikely (!c->serializer->extend (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
        HBUINT16 &ਬਾਹਰਲਾ_Mark_Filtering_ਸੈੱਟ = StructAfter<HBUINT16> (ਬਾਹਰਲਾ->sub_ਸਾਰਣੀ);
        ਬਾਹਰਲਾ_Mark_Filtering_ਸੈੱਟ = *idx;
      }
    }

    // Always keep the lookup even if it's empty. The rest of layout subsetting depends on lookup
    // indices being consistent with those computed during planning. So if an empty lookup is
    // discarded during the subset phase it will invalidate all subsequent lookup indices.
    // Generally we shouldn't end up with an empty lookup as we pre-prune them during the planning
    // phase, but it can happen in rare cases such as when during closure subtable is considered
    // degenerate (see: https://github.com/harfbuzz/harfbuzz/issues/3853)
    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TSub_ਸਾਰਣੀ>
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(c->check_struct (ਇਹ) && sub_ਸਾਰਣੀ.sanitize (c))) return_trace (ਝੂਠ);
    hb_barrier ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ subtables = get_subtable_ਗਿਣਤੀ ();
    ਜੇ (unlikely (!c->ਦੇਖੋ_subtables (subtables))) return_trace (ਝੂਠ);

    ਜੇ (lookupFlag & LookupFlag::Use_Mark_Filtering_ਸੈੱਟ)
    {
      ਅਚਲ HBUINT16 &mark_Filtering_ਸੈੱਟ = StructAfter<HBUINT16> (sub_ਸਾਰਣੀ);
      ਜੇ (!mark_Filtering_ਸੈੱਟ.sanitize (c)) return_trace (ਝੂਠ);
    }

    ਜੇ (unlikely (!get_subtables<TSub_ਸਾਰਣੀ> ().sanitize (c, ਇਹ, get_type ())))
      return_trace (ਝੂਠ);

    ਜੇ (unlikely (get_type () == TSub_ਸਾਰਣੀ::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ type = get_subtable<TSub_ਸਾਰਣੀ> (0).u.extension.get_type ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < subtables; i++)
	ਜੇ (get_subtable<TSub_ਸਾਰਣੀ> (i).u.extension.get_type () != type)
	  return_trace (ਝੂਠ);
    }
    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16_ਦਾ<Offset16>
		sub_ਸਾਰਣੀ;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, sub_ਸਾਰਣੀ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਵਰਤੋਂ LookupList = List16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ<Lookup, ਕਿਸਮ_ਨਾਂ Types::HBLUINT>;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TLookup, ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type>
ਬਣਤਰ Lookup_ਵਿੱਥ_List : List16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ<TLookup, ਵਿੱਥ_Type>
{
  ਤਾਰਕਿਕ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    + hb_enumerate (*ਇਹ)
    | hb_filter (l->lookup_ਸੂਚਕਾਂਕ_map, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_apply (subset_ਵਿੱਥ_array (c, *ਬਾਹਰਲਾ, ਇਹ))
    ;
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace ((List16_ਦਾ_ਵਿੱਥ_ਮੰਜ਼ਿਲ<TLookup, ਵਿੱਥ_Type>::sanitize (c, ਇਹ)));
  }
};


/*
 * Coverage Table
 */


ਸਥਿਰ ਤਾਰਕਿਕ ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ਅਚਲ hb_ਸੈੱਟ_t &klasses,
                                          ਤਾਰਕਿਕ use_class_zero,
                                          hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  ਜੇ (!klass_map)
    ਵਾਪਸ ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  ਜੇ (!use_class_zero)
    klass_map->ਸੈੱਟ (0, 0);

  ਬਿਨਾਨਿਸ਼ਾਨੀ idx = klass_map->has (0) ? 1 : 0;
  ਲਈ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ k: klasses)
  {
    ਜੇ (klass_map->has (k)) ਜਾਰੀ;
    klass_map->ਸੈੱਟ (k, idx);
    idx++;
  }


  ਜੇ (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    ਵਾਪਸ ਝੂਠ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < glyph_and_klass.ਲੰਮਾਈ; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  ਵਾਪਸ ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ClassDefFormat1_3
{
  ਮਿੱਤਰ ਬਣਤਰ ClassDef;

  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_class (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਵਾਪਸ class_ਮੁੱਲ[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (glyph_id - ਸ਼ੁਰੂ_Glyph)];
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ
  {
    ਵਾਪਸ class_ਮੁੱਲ.len;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    ਜੇ (unlikely (!it))
    {
      classFormat = Types::ਮਾਪ == 2 ? 1 : 3;
      ਸ਼ੁਰੂ_Glyph = 0;
      class_ਮੁੱਲ.len = 0;
      return_trace (ਸੱਚ);
    }

    hb_codepoint_t glyph_min = (*it).ਪਹਿਲਾ;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_ਪਹਿਲਾ)
			       | hb_reduce (hb_max, 0u);
    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ = glyph_max - glyph_min + 1;

    ਸ਼ੁਰੂ_Glyph = glyph_min;
    ਜੇ (unlikely (!class_ਮੁੱਲ.serialize (c, glyph_ਗਿਣਤੀ))) return_trace (ਝੂਠ);
    ਲਈ (ਅਚਲ hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ idx = gid_klass_pair.ਪਹਿਲਾ - glyph_min;
      class_ਮੁੱਲ[idx] = gid_klass_pair.second;
    }
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = ਖਾਲੀ_ਸੂਚਕ /*OUT*/,
               ਤਾਰਕਿਕ keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ = ਸੱਚ,
               ਤਾਰਕਿਕ use_class_zero = ਸੱਚ,
               ਅਚਲ Coverage* glyph_filter = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_ਸੈੱਟ_t orig_klasses;

    hb_codepoint_t ਸ਼ੁਰੂ = ਸ਼ੁਰੂ_Glyph;
    hb_codepoint_t ਸਿਰਾ   = ਸ਼ੁਰੂ + class_ਮੁੱਲ.len;

    ਲਈ (ਅਚਲ hb_codepoint_t gid : + hb_range (ਸ਼ੁਰੂ, ਸਿਰਾ))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      ਜੇ (new_gid == HB_MAP_ਮੁੱਲ_INVALID) ਜਾਰੀ;
      ਜੇ (glyph_filter && !glyph_filter->has(gid)) ਜਾਰੀ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = class_ਮੁੱਲ[gid - ਸ਼ੁਰੂ];
      ਜੇ (!klass) ਜਾਰੀ;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.ਜੋੜੋ (klass);
    }

    ਜੇ (use_class_zero)
    {
      ਅਚਲ hb_ਸੈੱਟ_t& glyphset = *c->plan->glyphset_gsub ();
      ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_ਗਿਣਤੀ <= glyph_and_klass.ਲੰਮਾਈ;
    }
    ਜੇ (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (ਝੂਠ);
    return_trace (keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ || (ਤਾਰਕਿਕ) glyph_and_klass);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && class_ਮੁੱਲ.sanitize (c));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ () ਅਚਲ { ਵਾਪਸ 1; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = class_ਮੁੱਲ.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (class_ਮੁੱਲ[i])
	ਜਾਰੀ;

      ਜੇ (ਸ਼ੁਰੂ != i)
	ਜੇ (unlikely (!glyphs->ਜੋੜੋ_range (ਸ਼ੁਰੂ_Glyph + ਸ਼ੁਰੂ, ਸ਼ੁਰੂ_Glyph + i)))
	  ਵਾਪਸ ਝੂਠ;

      ਸ਼ੁਰੂ = i + 1;
    }
    ਜੇ (ਸ਼ੁਰੂ != ਗਿਣਤੀ_2)
      ਜੇ (unlikely (!glyphs->ਜੋੜੋ_range (ਸ਼ੁਰੂ_Glyph + ਸ਼ੁਰੂ, ਸ਼ੁਰੂ_Glyph + ਗਿਣਤੀ_2)))
	ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_class (ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ klass) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = class_ਮੁੱਲ.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (class_ਮੁੱਲ[i] == klass) glyphs->ਜੋੜੋ (ਸ਼ੁਰੂ_Glyph + i);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    hb_codepoint_t ਸ਼ੁਰੂ = ਸ਼ੁਰੂ_Glyph;
    hb_codepoint_t ਸਿਰਾ = ਸ਼ੁਰੂ_Glyph + class_ਮੁੱਲ.len;
    ਲਈ (hb_codepoint_t iter = ਸ਼ੁਰੂ_Glyph - 1;
	 glyphs->next (&iter) && iter < ਸਿਰਾ;)
      ਜੇ (class_ਮੁੱਲ[iter - ਸ਼ੁਰੂ]) ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ intersects_class (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ klass) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = class_ਮੁੱਲ.len;
    ਜੇ (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      ਜੇ (!glyphs->next (&g)) ਵਾਪਸ ਝੂਠ;
      ਜੇ (g < ਸ਼ੁਰੂ_Glyph) ਵਾਪਸ ਸੱਚ;
      g = ਸ਼ੁਰੂ_Glyph + ਗਿਣਤੀ_2 - 1;
      ਜੇ (glyphs->next (&g)) ਵਾਪਸ ਸੱਚ;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::HBUINT *arr = class_ਮੁੱਲ.arrayZ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (arr[i] == klass && glyphs->has (ਸ਼ੁਰੂ_Glyph + i))
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ intersected_class_glyphs (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ klass, hb_ਸੈੱਟ_t *intersect_glyphs) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = class_ਮੁੱਲ.len;
    ਜੇ (klass == 0)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_glyph = ਸ਼ੁਰੂ_Glyph;
      ਲਈ (uint32_t g = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
	   glyphs->next (&g) && g < ਸ਼ੁਰੂ_glyph;)
	intersect_glyphs->ਜੋੜੋ (g);

      ਲਈ (uint32_t g = ਸ਼ੁਰੂ_Glyph + ਗਿਣਤੀ_2 - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->ਜੋੜੋ (g);

      ਵਾਪਸ;
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (class_ਮੁੱਲ[i] == klass && glyphs->has (ਸ਼ੁਰੂ_Glyph + i))
	intersect_glyphs->ਜੋੜੋ (ਸ਼ੁਰੂ_Glyph + i);

#ਜੇ 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_glyph = ਸ਼ੁਰੂ_Glyph;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_glyph = ਸ਼ੁਰੂ_glyph + ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ g = ਸ਼ੁਰੂ_Glyph - 1;
	 glyphs->next (&g) && g < ਸਿਰਾ_glyph;)
      ਜੇ (class_ਮੁੱਲ.arrayZ[g - ਸ਼ੁਰੂ_glyph] == klass)
        intersect_glyphs->ਜੋੜੋ (g);
#ਜੇ_ਅੰਤ
  }

  ਖਾਲੀ intersected_classes (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, hb_ਸੈੱਟ_t *intersect_classes) ਅਚਲ
  {
    ਜੇ (glyphs->is_empty ()) ਵਾਪਸ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    ਜੇ (!class_ਮੁੱਲ.len)
    {
      intersect_classes->ਜੋੜੋ (0);
      ਵਾਪਸ;
    }

    hb_codepoint_t ਸਿਰਾ_glyph = ਸ਼ੁਰੂ_Glyph + class_ਮੁੱਲ.len - 1;
    ਜੇ (glyphs->get_min () < ਸ਼ੁਰੂ_Glyph ||
        glyphs->get_max () > ਸਿਰਾ_glyph)
      intersect_classes->ਜੋੜੋ (0);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + hb_enumerate (class_ਮੁੱਲ))
    {
      hb_codepoint_t g = ਸ਼ੁਰੂ_Glyph + _.ਪਹਿਲਾ;
      ਜੇ (glyphs->has (g))
        intersect_classes->ਜੋੜੋ (_.second);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::HBGlyphID
		 ਸ਼ੁਰੂ_Glyph;	/* First GlyphID of the classValueArray */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBUINT>
		class_ਮੁੱਲ;	/* Array of Class Values--one per GlyphID */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + 2 * Types::ਮਾਪ, class_ਮੁੱਲ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ClassDefFormat2_4
{
  ਮਿੱਤਰ ਬਣਤਰ ClassDef;

  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_class (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਵਾਪਸ range_ਰਿਕਾਰਡ.bsearch (glyph_id).value;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ
  {
    ਕਿਸਮ_ਨਾਂ Types::large_int ret = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &r : range_ਰਿਕਾਰਡ)
      ret += r.get_population ();
    ਵਾਪਸ ret > UINT_MAX ? UINT_MAX : (ਬਿਨਾਨਿਸ਼ਾਨੀ) ret;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    ਜੇ (unlikely (!it))
    {
      classFormat = Types::ਮਾਪ == 2 ? 2 : 4;
      range_ਰਿਕਾਰਡ.len = 0;
      return_trace (ਸੱਚ);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ unsorted = ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ranges = 1;
    hb_codepoint_t prev_gid = (*it).ਪਹਿਲਾ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ prev_klass = (*it).second;

    Range_ਰਿਕਾਰਡ<Types> range_rec;
    range_rec.ਪਹਿਲਾ = prev_gid;
    range_rec.ਆਖਰੀ = prev_gid;
    range_rec.value = prev_klass;

    ਸਵੈਚਾਲਿਤ *ਰਿਕਾਰਡ = c->copy (range_rec);
    ਜੇ (unlikely (!ਰਿਕਾਰਡ)) return_trace (ਝੂਠ);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.ਪਹਿਲਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ cur_klass = gid_klass_pair.second;

      ਜੇ (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	ਜੇ (unlikely (cur_gid < prev_gid))
	  unsorted = ਸੱਚ;

	ਜੇ (unlikely (!ਰਿਕਾਰਡ)) ਰੋਕੋ;
	ਰਿਕਾਰਡ->ਆਖਰੀ = prev_gid;
	ਅੰਕ_ranges++;

	range_rec.ਪਹਿਲਾ = cur_gid;
	range_rec.ਆਖਰੀ = cur_gid;
	range_rec.value = cur_klass;

	ਰਿਕਾਰਡ = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    ਜੇ (unlikely (c->ਵਿੱਚ_error ())) return_trace (ਝੂਠ);

    ਜੇ (likely (ਰਿਕਾਰਡ)) ਰਿਕਾਰਡ->ਆਖਰੀ = prev_gid;
    range_ਰਿਕਾਰਡ.len = ਅੰਕ_ranges;

    ਜੇ (unlikely (unsorted))
      range_ਰਿਕਾਰਡ.as_array ().qsort (Range_ਰਿਕਾਰਡ<Types>::cmp_range);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = ਖਾਲੀ_ਸੂਚਕ /*OUT*/,
               ਤਾਰਕਿਕ keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ = ਸੱਚ,
               ਤਾਰਕਿਕ use_class_zero = ਸੱਚ,
               ਅਚਲ Coverage* glyph_filter = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyph_ਸੈੱਟ = *c->plan->glyphset_gsub ();

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_ਸੈੱਟ_t orig_klasses;

    ਜੇ (glyph_ਸੈੱਟ.get_population () * hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) range_ਰਿਕਾਰਡ.len)
	< get_population ())
    {
      ਲਈ (hb_codepoint_t g : glyph_ਸੈੱਟ)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ klass = get_class (g);
	ਜੇ (!klass) ਜਾਰੀ;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	ਜੇ (new_gid == HB_MAP_ਮੁੱਲ_INVALID) ਜਾਰੀ;
	ਜੇ (glyph_filter && !glyph_filter->has (g)) ਜਾਰੀ;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.ਜੋੜੋ (klass);
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਸਰੋਤ_glyphs = c->plan->ਸਰੋਤ->get_ਅੰਕ_glyphs ();
      ਲਈ (ਸਵੈਚਾਲਿਤ &range : range_ਰਿਕਾਰਡ)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ klass = range.value;
	ਜੇ (!klass) ਜਾਰੀ;
	hb_codepoint_t ਸ਼ੁਰੂ = range.ਪਹਿਲਾ;
	hb_codepoint_t ਸਿਰਾ   = hb_min (range.ਆਖਰੀ + 1, ਅੰਕ_ਸਰੋਤ_glyphs);
	ਲਈ (hb_codepoint_t g = ਸ਼ੁਰੂ; g < ਸਿਰਾ; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  ਜੇ (new_gid == HB_MAP_ਮੁੱਲ_INVALID) ਜਾਰੀ;
	  ਜੇ (glyph_filter && !glyph_filter->has (g)) ਜਾਰੀ;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.ਜੋੜੋ (klass);
	}
      }
    }

    ਅਚਲ hb_ਸੈੱਟ_t& glyphset = *c->plan->glyphset_gsub ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_ਗਿਣਤੀ <= glyph_and_klass.ਲੰਮਾਈ;
    ਜੇ (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (ਝੂਠ);
    return_trace (keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ || (ਤਾਰਕਿਕ) glyph_and_klass);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (range_ਰਿਕਾਰਡ.sanitize (c));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ () ਅਚਲ { ਵਾਪਸ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) range_ਰਿਕਾਰਡ.len); /* bsearch cost */ }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ &range : range_ਰਿਕਾਰਡ)
      ਜੇ (range.value)
	ਜੇ (unlikely (!range.collect_coverage (glyphs)))
	  ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_class (ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass) ਅਚਲ
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ &range : range_ਰਿਕਾਰਡ)
    {
      ਜੇ (range.value == klass)
	ਜੇ (unlikely (!range.collect_coverage (glyphs)))
	  ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਜੇ (range_ਰਿਕਾਰਡ.len > glyphs->get_population () * hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) range_ਰਿਕਾਰਡ.len))
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ g : *glyphs)
        ਜੇ (get_class (g))
	  ਵਾਪਸ ਸੱਚ;
      ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ hb_any (+ hb_iter (range_ਰਿਕਾਰਡ)
                   | hb_map ([glyphs] (ਅਚਲ Range_ਰਿਕਾਰਡ<Types> &range) { ਵਾਪਸ range.intersects (*glyphs) && range.value; }));
  }
  ਤਾਰਕਿਕ intersects_class (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ klass) ਅਚਲ
  {
    ਜੇ (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      ਜੇ (!range_ਰਿਕਾਰਡ.len)
        ਵਾਪਸ !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      hb_codepoint_t ਆਖਰੀ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      ਸਵੈਚਾਲਿਤ it = hb_iter (range_ਰਿਕਾਰਡ);
      ਲਈ (ਸਵੈਚਾਲਿਤ &range : it)
      {
        ਜੇ (it->ਪਹਿਲਾ == ਆਖਰੀ + 1)
	{
	  it++;
	  ਜਾਰੀ;
	}

	ਜੇ (!glyphs->next (&g))
	  ਰੋਕੋ;
	ਜੇ (g < range.ਪਹਿਲਾ)
	  ਵਾਪਸ ਸੱਚ;
	g = range.ਆਖਰੀ;
	ਆਖਰੀ = g;
      }
      ਜੇ (g != HB_ਸੈੱਟ_ਮੁੱਲ_INVALID && glyphs->next (&g))
	ਵਾਪਸ ਸੱਚ;
      /* Fall through. */
    }
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &range : range_ਰਿਕਾਰਡ)
      ਜੇ (range.value == klass && range.intersects (*glyphs))
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ intersected_class_glyphs (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ klass, hb_ਸੈੱਟ_t *intersect_glyphs) ਅਚਲ
  {
    ਜੇ (klass == 0)
    {
      hb_codepoint_t g = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      ਲਈ (ਸਵੈਚਾਲਿਤ &range : range_ਰਿਕਾਰਡ)
      {
	ਜੇ (!glyphs->next (&g))
	  ਜਾਓ ਪੂਰਾ;
	ਜਦੋਂ (g < range.ਪਹਿਲਾ)
	{
	  intersect_glyphs->ਜੋੜੋ (g);
	  ਜੇ (!glyphs->next (&g))
	    ਜਾਓ ਪੂਰਾ;
        }
        g = range.ਆਖਰੀ;
      }
      ਜਦੋਂ (glyphs->next (&g))
	intersect_glyphs->ਜੋੜੋ (g);
      ਪੂਰਾ:

      ਵਾਪਸ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = range_ਰਿਕਾਰਡ.len;
    ਜੇ (ਗਿਣਤੀ_2 > glyphs->get_population () * hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (ਗਿਣਤੀ_2))
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ g : *glyphs)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ i;
        ਜੇ (range_ਰਿਕਾਰਡ.as_array ().bfind (g, &i) &&
	    range_ਰਿਕਾਰਡ.arrayZ[i].value == klass)
	  intersect_glyphs->ਜੋੜੋ (g);
      }
      ਵਾਪਸ;
    }

    ਲਈ (ਸਵੈਚਾਲਿਤ &range : range_ਰਿਕਾਰਡ)
    {
      ਜੇ (range.value != klass) ਜਾਰੀ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = range.ਆਖਰੀ + 1;
      ਲਈ (hb_codepoint_t g = range.ਪਹਿਲਾ - 1;
	   glyphs->next (&g) && g < ਸਿਰਾ;)
	intersect_glyphs->ਜੋੜੋ (g);
    }
  }

  ਖਾਲੀ intersected_classes (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, hb_ਸੈੱਟ_t *intersect_classes) ਅਚਲ
  {
    ਜੇ (glyphs->is_empty ()) ਵਾਪਸ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    ਜੇ (!range_ਰਿਕਾਰਡ.len)
    {
      intersect_classes->ਜੋੜੋ (0);
      ਵਾਪਸ;
    }

    hb_codepoint_t g = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
    ਲਈ (ਸਵੈਚਾਲਿਤ &range : range_ਰਿਕਾਰਡ)
    {
      ਜੇ (!glyphs->next (&g))
        ਰੋਕੋ;
      ਜੇ (g < range.ਪਹਿਲਾ)
      {
        intersect_classes->ਜੋੜੋ (0);
        ਰੋਕੋ;
      }
      g = range.ਆਖਰੀ;
    }
    ਜੇ (g != HB_ਸੈੱਟ_ਮੁੱਲ_INVALID && glyphs->next (&g))
      intersect_classes->ਜੋੜੋ (0);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& range : range_ਰਿਕਾਰਡ)
      ਜੇ (range.intersects (*glyphs))
        intersect_classes->ਜੋੜੋ (range.value);
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Sorted_Array_ਦਾ<Range_ਰਿਕਾਰਡ<Types>>
		range_ਰਿਕਾਰਡ;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::ਮਾਪ, range_ਰਿਕਾਰਡ);
};

ਬਣਤਰ ClassDef
{
  /* Has interface. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਚਾਲਕ [] (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ get (k); }
  ਤਾਰਕਿਕ has (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ (*ਇਹ)[k]; }
  /* Projection. */
  hb_codepoint_t ਸੰਚਾਲਕ () (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ get (k); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ get_class (k); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_class (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_class (glyph_id);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_class (glyph_id);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_class (glyph_id);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_class (glyph_id);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ klass;
    ਜੇ (cache && cache->get (glyph_id, &klass)) ਵਾਪਸ klass;
    klass = get_class (glyph_id);
    ਜੇ (cache) cache->ਸੈੱਟ (glyph_id, klass);
    ਵਾਪਸ klass;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_population ();
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_population ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_population ();
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_population ();
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ NOT_COVERED;
    }
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ glyphs = + it_with_class_zero | hb_filter (hb_ਦੂਜਾ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ format = 2;
    hb_codepoint_t glyph_max = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ class_max = 0;
    ਜੇ (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).ਪਹਿਲਾ;
      glyph_max = glyph_min;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      ਬਿਨਾਨਿਸ਼ਾਨੀ prev_klass = (*glyphs).second;

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.ਪਹਿਲਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ cur_klass = gid_klass_pair.second;
	ਜੇ (cur_klass > class_max) class_max = cur_klass;
	ਜੇ (cur_gid == glyph_min) ਜਾਰੀ;
        ਜੇ (cur_gid > glyph_max) glyph_max = cur_gid;
	ਜੇ (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  ਅੰਕ_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t ਉੱਤਮ_ਮਾਪ = UINT64_MAX;
      ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਵਿਸਤਾਰ = glyph_max - glyph_min + 1;
      ਸਵੈਚਾਲਿਤ consider = [&] (ਬਿਨਾਨਿਸ਼ਾਨੀ candidate, uint64_t ਮਾਪ)
      {
	ਜੇ (ਮਾਪ >= ਉੱਤਮ_ਮਾਪ) ਵਾਪਸ;
	ਉੱਤਮ_ਮਾਪ = ਮਾਪ;
	format = candidate;
      };

      ਜੇ (glyph_max <= 0xFFFFu && glyph_ਵਿਸਤਾਰ <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_ਮਾਪ * glyph_ਵਿਸਤਾਰ);
      ਜੇ (glyph_max <= 0xFFFFu && ਅੰਕ_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) ਅੰਕ_ranges *
		     (2 * HBGlyphID16::static_ਮਾਪ + HBUINT16::static_ਮਾਪ));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
      ਜੇ (glyph_max <= 0xFFFFFFu && glyph_ਵਿਸਤਾਰ <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_ਮਾਪ * glyph_ਵਿਸਤਾਰ);
      ਜੇ (glyph_max <= 0xFFFFFFu && ਅੰਕ_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) ਅੰਕ_ranges *
		     (2 * HBGlyphID24::static_ਮਾਪ + HBUINT16::static_ਮਾਪ));
#ਜੇ_ਅੰਤ
      ਜੇ (unlikely (ਉੱਤਮ_ਮਾਪ == UINT64_MAX))
      {
	c->check_success (ਝੂਠ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);
	return_trace (ਝੂਠ);
      }
    }

    u.format.v = format;

    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_ਦੂਜਾ)));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_ਦੂਜਾ)));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_ਦੂਜਾ)));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_ਦੂਜਾ)));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = ਖਾਲੀ_ਸੂਚਕ /*OUT*/,
               ਤਾਰਕਿਕ keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ = ਸੱਚ,
               ਤਾਰਕਿਕ use_class_zero = ਸੱਚ,
               ਅਚਲ Coverage* glyph_filter = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ, use_class_zero, glyph_filter));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ, use_class_zero, glyph_filter));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ, use_class_zero, glyph_filter));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_ਖਾਲੀ_ਥਾਵਾਂ_ਸਾਰਣੀ, use_class_zero, glyph_filter));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.ਲਾਗਤ ();
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.ਲਾਗਤ ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.ਲਾਗਤ ();
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.ਲਾਗਤ ();
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.collect_coverage (glyphs);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.collect_coverage (glyphs);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.collect_coverage (glyphs);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.collect_coverage (glyphs);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_class (ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.collect_class (glyphs, klass);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.collect_class (glyphs, klass);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.collect_class (glyphs, klass);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.collect_class (glyphs, klass);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.intersects (glyphs);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.intersects (glyphs);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.intersects (glyphs);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.intersects (glyphs);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }
  ਤਾਰਕਿਕ intersects_class (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.intersects_class (glyphs, klass);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.intersects_class (glyphs, klass);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.intersects_class (glyphs, klass);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.intersects_class (glyphs, klass);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਖਾਲੀ intersected_class_glyphs (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ klass, hb_ਸੈੱਟ_t *intersect_glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ;
    }
  }

  ਖਾਲੀ intersected_classes (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, hb_ਸੈੱਟ_t *intersect_classes) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.intersected_classes (glyphs, intersect_classes);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.intersected_classes (glyphs, intersect_classes);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.intersected_classes (glyphs, intersect_classes);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.intersected_classes (glyphs, intersect_classes);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ;
    }
  }


  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#ਜੇ_ਅੰਤ
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ ਵਾਪਸ (c->ਸ਼ੁਰੂ_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
ਬਣਤਰ delta_ਪੰਕਤੀ_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  ਬਣਤਰ chars_t : hb_ਸਦਿਸ਼_t<uint8_t>
  {
    ਪੂਰਨ cmp (ਅਚਲ chars_t& other) ਅਚਲ
    {
      ਵਾਪਸ as_array ().cmp (other.as_array ());
    }

    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> get_ਚੌੜਾਈ ()
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ columns = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਲੰਮਾਈ; i++)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ v = arrayZ[i];
	ਚੌੜਾਈ += v;
	columns += (v != 0);
      }
      ਵਾਪਸ hb_pair (ਚੌੜਾਈ, columns);
    }

    HB_HOT
    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> combine_ਚੌੜਾਈ (ਅਚਲ chars_t& other) ਅਚਲ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ combined_ਚੌੜਾਈ = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ combined_columns = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਲੰਮਾਈ; i++)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_ਚੌੜਾਈ += v;
	combined_columns += (v != 0);
      }
      ਵਾਪਸ hb_pair (combined_ਚੌੜਾਈ, combined_columns);
    }
  };

  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> combine_ਚੌੜਾਈ (ਅਚਲ delta_ਪੰਕਤੀ_encoding_t& other_encoding) ਅਚਲ { ਵਾਪਸ chars.combine_ਚੌੜਾਈ (other_encoding.chars); }

  // Actual data

  chars_t chars;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ overhead = 0;
  hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*> ਚੀਜ਼ਾਂ;

  delta_ਪੰਕਤੀ_encoding_t () = ਮੂਲ;
  delta_ਪੰਕਤੀ_encoding_t (hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*> &&ਪੰਕਤੀਆਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (ਪੰਕਤੀਆਂ);

    ਚੀਜ਼ਾਂ = std::move (ਪੰਕਤੀਆਂ);

    ਜੇ (unlikely (!chars.resize (ਅੰਕ_ਖਾਨੇ_ਗਿਣਤੀ)))
      ਵਾਪਸ;

    calculate_chars ();
  }

  ਖਾਲੀ ਮਿਲਾਓ (ਅਚਲ delta_ਪੰਕਤੀ_encoding_t& other)
  {
    ਚੀਜ਼ਾਂ.alloc (ਚੀਜ਼ਾਂ.ਲੰਮਾਈ + other.ਚੀਜ਼ਾਂ.ਲੰਮਾਈ);
    ਲਈ (ਸਵੈਚਾਲਿਤ &ਪੰਕਤੀ : other.ਚੀਜ਼ਾਂ)
      ਜੋੜੋ_ਪੰਕਤੀ (ਪੰਕਤੀ);

    // Merge chars
    ਪ੍ਰਤਿਜ੍ਞਾ (chars.ਲੰਮਾਈ == other.chars.ਲੰਮਾਈ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < chars.ਲੰਮਾਈ; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_ਬਦਲਿਆ ();
  }

  ਖਾਲੀ chars_ਬਦਲਿਆ ()
  {
    ਸਵੈਚਾਲਿਤ _ = chars.get_ਚੌੜਾਈ ();
    ਚੌੜਾਈ = _.ਪਹਿਲਾ;
    overhead = get_chars_overhead (_.second);
  }

  ਖਾਲੀ calculate_chars ()
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (ਚੀਜ਼ਾਂ);

    ਤਾਰਕਿਕ long_ਸ਼ਬਦ = ਝੂਠ;

    ਲਈ (ਸਵੈਚਾਲਿਤ &ਪੰਕਤੀ : ਚੀਜ਼ਾਂ)
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (ਪੰਕਤੀ->ਲੰਮਾਈ == chars.ਲੰਮਾਈ);

      /* 0/1/2 byte encoding */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਪੰਕਤੀ->ਲੰਮਾਈ; i++)
      {
	ਪੂਰਨ v =  ਪੰਕਤੀ->arrayZ[i];
	ਜੇ (v == 0)
	  ਜਾਰੀ;
	ਨਹੀਂਤਾਂ ਜੇ (v > 32767 || v < -32768)
	{
	  long_ਸ਼ਬਦ = ਸੱਚ;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	ਨਹੀਂਤਾਂ ਜੇ (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	ਨਹੀਂਤਾਂ
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    ਜੇ (long_ਸ਼ਬਦ)
    {
      // Convert 1s to 2s
      ਲਈ (ਸਵੈਚਾਲਿਤ &v : chars)
	ਜੇ (v == 1)
	  v = 2;
    }

    chars_ਬਦਲਿਆ ();
  }

  ਤਾਰਕਿਕ is_empty () ਅਚਲ
  { ਵਾਪਸ !ਚੀਜ਼ਾਂ; }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
  { ਵਾਪਸ chars.ਵਿੱਚ_error () || ਚੀਜ਼ਾਂ.ਵਿੱਚ_error (); }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ get_chars_overhead (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_columns)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    ਵਾਪਸ c + ਅੰਕ_columns * 2;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_gain (ਬਿਨਾਨਿਸ਼ਾਨੀ additional_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀਆਂ = 1) ਅਚਲ
  {
    ਪੂਰਨ ਗਿਣਤੀ_2 = ਚੀਜ਼ਾਂ.ਲੰਮਾਈ;
    ਵਾਪਸ hb_max (0, (ਪੂਰਨ) overhead - ਗਿਣਤੀ_2 * (ਪੂਰਨ) additional_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀਆਂ);
  }

  ਪੂਰਨ gain_from_merging (ਅਚਲ delta_ਪੰਕਤੀ_encoding_t& other_encoding) ਅਚਲ
  {
    // Back of the envelope calculations to reject early.
    ਨਿਸ਼ਾਨੀਵਾਲਾ additional_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀਆਂ = other_encoding.ਚੌੜਾਈ - ਚੌੜਾਈ;
    ਜੇ (additional_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀਆਂ > 0)
    {
      ਜੇ (get_gain (additional_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀਆਂ) == 0)
        ਵਾਪਸ 0;
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (other_encoding.get_gain (-additional_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਪੰਕਤੀਆਂ) == 0)
	ਵਾਪਸ 0;
    }

    ਸਵੈਚਾਲਿਤ pair = combine_ਚੌੜਾਈ (other_encoding);
    ਬਿਨਾਨਿਸ਼ਾਨੀ combined_ਚੌੜਾਈ = pair.ਪਹਿਲਾ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ combined_columns = pair.second;

    ਪੂਰਨ combined_gain = (ਪੂਰਨ) overhead + (ਪੂਰਨ) other_encoding.overhead;
    combined_gain -= (combined_ਚੌੜਾਈ - (ਪੂਰਨ) ਚੌੜਾਈ) * ਚੀਜ਼ਾਂ.ਲੰਮਾਈ;
    combined_gain -= (combined_ਚੌੜਾਈ - (ਪੂਰਨ) other_encoding.ਚੌੜਾਈ) * other_encoding.ਚੀਜ਼ਾਂ.ਲੰਮਾਈ;
    combined_gain -= get_chars_overhead (combined_columns);

    ਵਾਪਸ combined_gain;
  }

  ਤਾਰਕਿਕ ਜੋੜੋ_ਪੰਕਤੀ (ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>* ਪੰਕਤੀ)
  { ਵਾਪਸ ਚੀਜ਼ਾਂ.push (ਪੰਕਤੀ); }

  ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    ਅਚਲ delta_ਪੰਕਤੀ_encoding_t *a = (ਅਚਲ delta_ਪੰਕਤੀ_encoding_t *)pa;
    ਅਚਲ delta_ਪੰਕਤੀ_encoding_t *b = (ਅਚਲ delta_ਪੰਕਤੀ_encoding_t *)pb;

    ਜੇ (a->ਚੌੜਾਈ != b->ਚੌੜਾਈ)
      ਵਾਪਸ (ਪੂਰਨ) a->ਚੌੜਾਈ - (ਪੂਰਨ) b->ਚੌੜਾਈ;

    ਵਾਪਸ b->chars.cmp (a->chars);
  }
};

ਬਣਤਰ VarRegionAxis
{
  ਦਸ਼ਮਲਵ evaluate (ਪੂਰਨ coord) ਅਚਲ
  {
    ਪੂਰਨ peak = peakCoord.ਮੰਜ਼ਿਲ_int ();
    ਜੇ (peak == 0 || coord == peak)
      ਵਾਪਸ 1.f;
    ਨਹੀਂਤਾਂ ਜੇ (coord == 0) // Faster
      ਵਾਪਸ 0.f;

    ਪੂਰਨ ਸ਼ੁਰੂ = ਸ਼ੁਰੂ_Coord.ਮੰਜ਼ਿਲ_int (), ਸਿਰਾ = ਸਿਰਾ_Coord.ਮੰਜ਼ਿਲ_int ();

    /* TODO Move these to sanitize(). */
    ਜੇ (unlikely (ਸ਼ੁਰੂ > peak || peak > ਸਿਰਾ))
      ਵਾਪਸ 1.f;
    ਜੇ (unlikely (ਸ਼ੁਰੂ < 0 && ਸਿਰਾ > 0))
      ਵਾਪਸ 1.f;

    ਜੇ (coord <= ਸ਼ੁਰੂ || ਸਿਰਾ <= coord)
      ਵਾਪਸ 0.f;

    /* Interpolate */
    ਜੇ (coord < peak)
      ਵਾਪਸ ਦਸ਼ਮਲਵ (coord - ਸ਼ੁਰੂ) / (peak - ਸ਼ੁਰੂ);
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਦਸ਼ਮਲਵ (ਸਿਰਾ - coord) / (ਸਿਰਾ - peak);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed (ਇਹ));
  }

  ਜਨਤਕ:
  F2DOT14	ਸ਼ੁਰੂ_Coord;
  F2DOT14	peakCoord;
  F2DOT14	ਸਿਰਾ_Coord;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};
ਬਣਤਰ SparseVarRegionAxis
{
  ਦਸ਼ਮਲਵ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = axis_ਸੂਚਕਾਂਕ_2;
    ਪੂਰਨ coord = i < coord_len ? coords[i] : 0;
    ਵਾਪਸ axis.evaluate (coord);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ SparseVarRegionAxis *src,
		  ਅਚਲ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (src);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    ਜੇ (axis_map)
    {
      ਜੇ (unlikely (!axis_map->has (ਬਾਹਰਲਾ->axis_ਸੂਚਕਾਂਕ_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (ਝੂਠ);
      }
      ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->axis_ਸੂਚਕਾਂਕ_2, axis_map->get (ਬਾਹਰਲਾ->axis_ਸੂਚਕਾਂਕ_2),
				    HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
	return_trace (ਝੂਠ);
    }
    return_trace (ਸੱਚ);
  }

  ਜਨਤਕ:
  HBUINT16 axis_ਸੂਚਕਾਂਕ_2;
  VarRegionAxis axis;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ hb_scalar_cache_t
{
  ਨਿਜੀ:
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ STATIC_ਲੰਮਾਈ = 128;
  ਸਥਿਰ constexpr ਦਸ਼ਮਲਵ INVALID = NAN;

  ਜਨਤਕ:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ਅਚਲ hb_scalar_cache_t&) = ਮਿਟਾਓ;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = ਮਿਟਾਓ;
  hb_scalar_cache_t& ਸੰਚਾਲਕ= (ਅਚਲ hb_scalar_cache_t&) = ਮਿਟਾਓ;
  hb_scalar_cache_t& ਸੰਚਾਲਕ= (hb_scalar_cache_t&&) = ਮਿਟਾਓ;

  ਸਥਿਰ hb_scalar_cache_t *create (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				    hb_scalar_cache_t *scratch_cache = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (!ਗਿਣਤੀ_2) ਵਾਪਸ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    ਜੇ (scratch_cache && ਗਿਣਤੀ_2 <= STATIC_ਲੰਮਾਈ)
    {
      scratch_cache->ਲੰਮਾਈ = ਗਿਣਤੀ_2;
      scratch_cache->clear ();
      ਵਾਪਸ scratch_cache;
    }

    ਸਵੈਚਾਲਿਤ *cache = (hb_scalar_cache_t *) hb_malloc (ਆਕਾਰ (hb_scalar_cache_t) - ਆਕਾਰ (static_ਮੁੱਲਾਂ) + ਆਕਾਰ (static_ਮੁੱਲਾਂ[0]) * ਗਿਣਤੀ_2);
    ਜੇ (unlikely (!cache)) ਵਾਪਸ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->ਲੰਮਾਈ = ਗਿਣਤੀ_2;
    cache->clear ();

    ਵਾਪਸ cache;
  }

  ਸਥਿਰ ਖਾਲੀ destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  ਖਾਲੀ clear ()
  {
    ਸਵੈਚਾਲਿਤ *values = &static_ਮੁੱਲਾਂ[0];
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
    ਲਈ (; i + 3 < ਲੰਮਾਈ; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; i < ਲੰਮਾਈ; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  ਤਾਰਕਿਕ get (ਬਿਨਾਨਿਸ਼ਾਨੀ i, ਦਸ਼ਮਲਵ *value) ਅਚਲ
  {
    ਜੇ (unlikely (i >= ਲੰਮਾਈ))
    {
      *value = 0.f;
      ਵਾਪਸ ਸੱਚ;
    }
    ਸਵੈਚਾਲਿਤ *values = &static_ਮੁੱਲਾਂ[0];
    ਸਵੈਚਾਲਿਤ *cached_ਮੁੱਲ = &values[i];
    ਦਸ਼ਮਲਵ v = *cached_ਮੁੱਲ;
    ਜੇ (std::isnan (v))
      ਵਾਪਸ ਝੂਠ;
    *value = v;
    ਵਾਪਸ ਸੱਚ;
  }

  HB_ALWAYS_INLINE
  ਖਾਲੀ ਸੈੱਟ (ਬਿਨਾਨਿਸ਼ਾਨੀ i, ਦਸ਼ਮਲਵ value)
  {
    ਜੇ (unlikely (i >= ਲੰਮਾਈ)) ਵਾਪਸ;
    ਸਵੈਚਾਲਿਤ *values = &static_ਮੁੱਲਾਂ[0];
    ਸਵੈਚਾਲਿਤ *cached_ਮੁੱਲ = &values[i];
    *cached_ਮੁੱਲ = value;
  }

  ਜਨਤਕ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ;
  ਨਿਜੀ:
  ਬਦਲਣਯੋਗ hb_atomic_t<ਦਸ਼ਮਲਵ> static_ਮੁੱਲਾਂ[STATIC_ਲੰਮਾਈ];
};

ਬਣਤਰ VarRegionList
{
  ਨਿਜੀ:
  ਦਸ਼ਮਲਵ evaluate_impl (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region_ਸੂਚਕਾਂਕ,
		       ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len) ਅਚਲ
  {
    ਅਚਲ VarRegionAxis *axes = axesZ.arrayZ + (region_ਸੂਚਕਾਂਕ * axis_ਗਿਣਤੀ_2);
    ਦਸ਼ਮਲਵ v = 1.f;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = axis_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਪੂਰਨ coord = i < coord_len ? coords[i] : 0;
      ਦਸ਼ਮਲਵ ਗੁਣਕ = axes[i].evaluate (coord);
      ਜੇ (ਗੁਣਕ == 0.f)
      {
	v = 0.f;
	ਰੋਕੋ;
      }
      v *= ਗੁਣਕ;
    }

    ਵਾਪਸ v;
  }

  ਜਨਤਕ:
  HB_ALWAYS_INLINE
  ਦਸ਼ਮਲਵ evaluate (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region_ਸੂਚਕਾਂਕ,
		  ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		  hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (unlikely (region_ਸੂਚਕਾਂਕ >= region_ਗਿਣਤੀ))
      ਵਾਪਸ 0.;

    ਦਸ਼ਮਲਵ v;
    ਜੇ (cache && cache->get (region_ਸੂਚਕਾਂਕ, &v))
      ਵਾਪਸ v;

    v = evaluate_impl (region_ਸੂਚਕਾਂਕ, coords, coord_len);

    ਜੇ (cache)
      cache->ਸੈੱਟ (region_ਸੂਚਕਾਂਕ, v);
    ਵਾਪਸ v;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_ਗਿਣਤੀ_2 * region_ਗਿਣਤੀ));
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  ਅਚਲ hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t>& axis_ਟੈਗ_ਸੂਚੀ,
                  ਅਚਲ hb_ਸਦਿਸ਼_t<ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ = axis_ਟੈਗ_ਸੂਚੀ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਗਿਣਤੀ_2 = regions.ਲੰਮਾਈ;
    ਜੇ (!axis_ਗਿਣਤੀ || !region_ਗਿਣਤੀ_2) return_trace (ਝੂਠ);
    ਜੇ (unlikely (hb_unsigned_mul_overflows (axis_ਗਿਣਤੀ * region_ਗਿਣਤੀ_2,
                                             VarRegionAxis::static_ਮਾਪ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    axis_ਗਿਣਤੀ_2 = axis_ਗਿਣਤੀ;
    region_ਗਿਣਤੀ = region_ਗਿਣਤੀ_2;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < region_ਗਿਣਤੀ_2; r++)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ& region = regions[r];
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ; i++)
      {
        hb_ਟੈਗ_t ਟੈਗ_2 = axis_ਟੈਗ_ਸੂਚੀ.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        ਜੇ (region->has (ਟੈਗ_2, &coords))
        {
          var_region_rec.ਸ਼ੁਰੂ_Coord.ਸੈੱਟ_float (coords->minimum);
          var_region_rec.peakCoord.ਸੈੱਟ_float (coords->ਵਿਚਕਾਰ);
          var_region_rec.ਸਿਰਾ_Coord.ਸੈੱਟ_float (coords->ਵੱਧ_ਤੋਂ_ਵੱਧ);
        }
        ਨਹੀਂਤਾਂ
        {
          var_region_rec.ਸ਼ੁਰੂ_Coord.ਸੈੱਟ_int (0);
          var_region_rec.peakCoord.ਸੈੱਟ_int (0);
          var_region_rec.ਸਿਰਾ_Coord.ਸੈੱਟ_int (0);
        }
        ਜੇ (!var_region_rec.serialize (c))
          return_trace (ਝੂਠ);
      }
    }
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ VarRegionList *src, ਅਚਲ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    axis_ਗਿਣਤੀ_2 = src->axis_ਗਿਣਤੀ_2;
    region_ਗਿਣਤੀ = region_map.get_population ();
    ਜੇ (unlikely (hb_unsigned_mul_overflows (axis_ਗਿਣਤੀ_2 * region_ਗਿਣਤੀ,
					     VarRegionAxis::static_ਮਾਪ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->extend (ਇਹ))) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region_ਗਿਣਤੀ_2 = src->region_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = 0; r < region_ਗਿਣਤੀ; r++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backward = region_map.backward (r);
      ਜੇ (backward >= region_ਗਿਣਤੀ_2) return_trace (ਝੂਠ);
      hb_memcpy (&axesZ[axis_ਗਿਣਤੀ_2 * r], &src->axesZ[axis_ਗਿਣਤੀ_2 * backward], VarRegionAxis::static_ਮਾਪ * axis_ਗਿਣਤੀ_2);
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ get_var_region (ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਸੂਚਕਾਂਕ,
                       ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                       hb_hashmap_t<hb_ਟੈਗ_t, Triple>& axis_tuples /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (region_ਸੂਚਕਾਂਕ >= region_ਗਿਣਤੀ) ਵਾਪਸ ਝੂਠ;
    ਅਚਲ VarRegionAxis* axis_region = axesZ.arrayZ + (region_ਸੂਚਕਾਂਕ * axis_ਗਿਣਤੀ_2);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      hb_ਟੈਗ_t *axis_ਟੈਗ_2;
      ਜੇ (!axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.has (i, &axis_ਟੈਗ_2))
        ਵਾਪਸ ਝੂਠ;

      ਦਸ਼ਮਲਵ min_val = axis_region->ਸ਼ੁਰੂ_Coord.ਮੰਜ਼ਿਲ_float ();
      ਦਸ਼ਮਲਵ def_val = axis_region->peakCoord.ਮੰਜ਼ਿਲ_float ();
      ਦਸ਼ਮਲਵ max_val = axis_region->ਸਿਰਾ_Coord.ਮੰਜ਼ਿਲ_float ();

      ਜੇ (def_val != 0.f)
        axis_tuples.ਸੈੱਟ (*axis_ਟੈਗ_2, Triple ((ਦੁੱਗਣਾ) min_val, (ਦੁੱਗਣਾ) def_val, (ਦੁੱਗਣਾ) max_val));
      axis_region++;
    }
    ਵਾਪਸ !axis_tuples.ਵਿੱਚ_error ();
  }

  ਤਾਰਕਿਕ get_var_regions (ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                        hb_ਸਦਿਸ਼_t<hb_hashmap_t<hb_ਟੈਗ_t, Triple>>& regions /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (!regions.alloc (region_ਗਿਣਤੀ))
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < region_ਗਿਣਤੀ; i++)
    {
      hb_hashmap_t<hb_ਟੈਗ_t, Triple> axis_tuples;
      ਜੇ (!get_var_region (i, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, axis_tuples))
        ਵਾਪਸ ਝੂਠ;
      regions.push (std::move (axis_tuples));
    }
    ਵਾਪਸ !regions.ਵਿੱਚ_error ();
  }

  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (min_ਮਾਪ,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_ਮਾਪ,
							       axis_ਗਿਣਤੀ_2, region_ਗਿਣਤੀ)); }

  ਜਨਤਕ:
  HBUINT16	axis_ਗਿਣਤੀ_2;
  HBUINT15	region_ਗਿਣਤੀ;
  ਸੁਰੱਖਿਅਤ:
  Unsized_Array_ਦਾ<VarRegionAxis>
		axesZ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_avar2_region_is_dead (ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> &axis_tuples,
			  ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> &reachable_ranges)
{
  ਲਈ (ਸਵੈਚਾਲਿਤ _ : axis_tuples)
  {
    Triple *range;
    ਜੇ (!reachable_ranges.has (_.ਪਹਿਲਾ, &range)) ਜਾਰੀ;
    ਅਚਲ Triple &tent = _.second;
    ਪੂਰਨ ਸ਼ੁਰੂ = (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) tent.minimum * 16384.f);
    ਪੂਰਨ peak  = (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) tent.ਵਿਚਕਾਰ * 16384.f);
    ਪੂਰਨ ਸਿਰਾ   = (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) tent.ਵੱਧ_ਤੋਂ_ਵੱਧ * 16384.f);
    ਜੇ (!peak) ਜਾਰੀ;
    ਜੇ (ਸ਼ੁਰੂ > peak || peak > ਸਿਰਾ) ਜਾਰੀ;
    ਜੇ (ਸ਼ੁਰੂ < 0 && ਸਿਰਾ > 0) ਜਾਰੀ;
    ਪੂਰਨ lo = (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) range->minimum * 16384.f);
    ਪੂਰਨ hi = (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) range->ਵੱਧ_ਤੋਂ_ਵੱਧ * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    ਜੇ (hi < ਸ਼ੁਰੂ || lo > ਸਿਰਾ)
      ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਬਣਤਰ SparseVariationRegion
{
  ਦਸ਼ਮਲਵ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len) ਅਚਲ
  {
    ਦਸ਼ਮਲਵ v = 1.f;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = axes.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਦਸ਼ਮਲਵ ਗੁਣਕ = (ਇਹ+axes[i]).evaluate (coords, coord_len);
      ਜੇ (ਗੁਣਕ == 0.f)
	ਵਾਪਸ 0.;
      v *= ਗੁਣਕ;
    }
    ਵਾਪਸ v;
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ SparseVariationRegion *src,
		  ਅਚਲ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!axes.serialize (c, (ਬਿਨਾਨਿਸ਼ਾਨੀ) src->axes.len))) return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axes.len; i++)
      ਜੇ (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (axes.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ<Offset32_ਮੰਜ਼ਿਲ<SparseVarRegionAxis>> axes;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, axes);
};

ਬਣਤਰ SparseVarRegionList
{
  HB_ALWAYS_INLINE
  ਦਸ਼ਮਲਵ evaluate (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region_ਸੂਚਕਾਂਕ,
		  ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		  hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (unlikely (region_ਸੂਚਕਾਂਕ >= regions.len))
      ਵਾਪਸ 0.;

    ਦਸ਼ਮਲਵ v;
    ਜੇ (cache && cache->get (region_ਸੂਚਕਾਂਕ, &v))
      ਵਾਪਸ v;

    ਅਚਲ SparseVariationRegion &region = ਇਹ+regions[region_ਸੂਚਕਾਂਕ];

    v = region.evaluate (coords, coord_len);
    ਜੇ (cache)
      cache->ਸੈੱਟ (region_ਸੂਚਕਾਂਕ, v);

    ਵਾਪਸ v;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (regions.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ SparseVarRegionList *src,
		  ਅਚਲ hb_inc_bimap_t &region_map,
		  ਅਚਲ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    regions.len = region_map.get_population ();
    ਜੇ (unlikely (!c->extend (regions))) return_trace (ਝੂਠ);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < regions.len; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਸੂਚਕਾਂਕ = region_map.backward (i);
      ਜੇ (unlikely (old_ਸੂਚਕਾਂਕ >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_ਸੂਚਕਾਂਕ]),
					     axis_map)))
	return_trace (ਝੂਠ);
    }
    return_trace (ਸੱਚ);
  }

  ਜਨਤਕ:
  Array32_ਦਾ<Offset32_ਮੰਜ਼ਿਲ<SparseVariationRegion>>
		regions;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, regions);
};


ਬਣਤਰ Var_ਅੰਕੜੇ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਵਸਤੂ_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ ਵਸਤੂ_ਗਿਣਤੀ; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_region_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ regionIndices.len; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_region_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  { ਵਾਪਸ i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_ਪੰਕਤੀ_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ (ਸ਼ਬਦ_ਗਿਣਤੀ () + regionIndices.len) * (long_ਸ਼ਬਦ_2 () ? 2 : 1); }

  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (min_ਮਾਪ - regionIndices.min_ਮਾਪ,
				     regionIndices.get_ਮਾਪ (),
				     hb_unsigned_mul_saturate (ਵਸਤੂ_ਗਿਣਤੀ, get_ਪੰਕਤੀ_ਮਾਪ ())); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
  Float _get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner,
		    ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
		    ਅਚਲ VarRegionList &regions,
		    hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (unlikely (inner >= ਵਸਤੂ_ਗਿਣਤੀ))
      ਵਾਪਸ 0.;
    ਤਾਰਕਿਕ is_long = long_ਸ਼ਬਦ_2 ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = regionIndices.len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ਬਦ_ਗਿਣਤੀ_2 = ਸ਼ਬਦ_ਗਿਣਤੀ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scount = is_long ? ਗਿਣਤੀ_2 : ਸ਼ਬਦ_ਗਿਣਤੀ_2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lcount = is_long ? ਸ਼ਬਦ_ਗਿਣਤੀ_2 : 0;

    ਅਚਲ HBUINT8 *ਬਾਈਟਾਂ = get_delta_ਬਾਈਟਾਂ ();
    ਅਚਲ HBUINT8 *ਪੰਕਤੀ = ਬਾਈਟਾਂ + inner * get_ਪੰਕਤੀ_ਮਾਪ ();

    Float delta = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;

    ਅਚਲ HBINT32 *lcursor = reinterpret_cast<ਅਚਲ HBINT32 *> (ਪੰਕਤੀ);
    ਲਈ (; i < lcount; i++)
    {
      ਦਸ਼ਮਲਵ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ਗਿਣਤੀ, cache);
      ਜੇ (scalar)
        delta += (Float) scalar * (ਪੂਰਨ) *lcursor;
      lcursor++;
    }
    ਅਚਲ HBINT16 *scursor = reinterpret_cast<ਅਚਲ HBINT16 *> (lcursor);
    ਲਈ (; i < scount; i++)
    {
      ਦਸ਼ਮਲਵ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ਗਿਣਤੀ, cache);
      ਜੇ (scalar)
       delta += (Float) scalar * (ਪੂਰਨ) *scursor;
      scursor++;
    }
    ਅਚਲ HBINT8 *bcursor = reinterpret_cast<ਅਚਲ HBINT8 *> (scursor);
    ਲਈ (; i < ਗਿਣਤੀ_2; i++)
    {
      ਦਸ਼ਮਲਵ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ਗਿਣਤੀ, cache);
      ਜੇ (scalar)
        delta += (Float) scalar * (ਪੂਰਨ) *bcursor;
      bcursor++;
    }

    ਵਾਪਸ delta;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
  HB_ALWAYS_INLINE
  Float get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner,
		   ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
		   ਅਚਲ VarRegionList &regions,
		   hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = regionIndices.len;
    ਜੇ (!ਗਿਣਤੀ_2) ਵਾਪਸ Float (0); // This is quite common, so optimize it.
    ਵਾਪਸ _get_delta<Float> (inner, coords, coord_ਗਿਣਤੀ, regions, cache);
  }

  ਖਾਲੀ get_region_scalars (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
			   ਅਚਲ VarRegionList &regions,
			   ਦਸ਼ਮਲਵ *scalars /*OUT */,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_scalars) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_min (ਅੰਕ_scalars, regionIndices.len);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ਗਿਣਤੀ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਗਿਣਤੀ_2; i < ਅੰਕ_scalars; i++)
      scalars[i] = 0.f;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  ਸ਼ਬਦ_ਗਿਣਤੀ () <= regionIndices.len &&
		  c->check_range (get_delta_ਬਾਈਟਾਂ (),
				  ਵਸਤੂ_ਗਿਣਤੀ,
				  get_ਪੰਕਤੀ_ਮਾਪ ()));
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  ਤਾਰਕਿਕ has_long,
                  ਅਚਲ hb_ਸਦਿਸ਼_t<ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>*>& ਪੰਕਤੀਆਂ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_ਗਿਣਤੀ_3 = ਪੰਕਤੀਆਂ.ਲੰਮਾਈ;
    ਜੇ (!ਪੰਕਤੀ_ਗਿਣਤੀ_3) {
      // Nothing to serialize, will be empty.
      ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (ਵਸਤੂ_ਗਿਣਤੀ, ਪੰਕਤੀ_ਗਿਣਤੀ_3,
                                    HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
      return_trace (ਝੂਠ);

    ਪੂਰਨ min_ਹੱਦ = has_long ? -32768 : -128;
    ਪੂਰਨ max_ਹੱਦ = has_long ? +32767 : +127;
    ਗਿਣਤੀ delta_ਮਾਪ_t { kZero=0, k_Non_ਸ਼ਬਦ, k_ਸ਼ਬਦ };
    hb_ਸਦਿਸ਼_t<delta_ਮਾਪ_t> delta_sz;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_regions = ਪੰਕਤੀਆਂ[0]->ਲੰਮਾਈ;
    ਜੇ (!delta_sz.resize (ਅੰਕ_regions))
      return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ਬਦ_ਗਿਣਤੀ_2 = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < ਅੰਕ_regions; r++)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਪੰਕਤੀ_ਗਿਣਤੀ_3; i++)
      {
        ਪੂਰਨ delta = ਪੰਕਤੀਆਂ[i]->arrayZ[r];
        ਜੇ (delta < min_ਹੱਦ || delta > max_ਹੱਦ)
        {
          delta_sz[r] = k_ਸ਼ਬਦ;
          ਸ਼ਬਦ_ਗਿਣਤੀ_2++;
          ਰੋਕੋ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (delta != 0)
        {
          delta_sz[r] = k_Non_ਸ਼ਬਦ;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ਬਦ_ਸੂਚਕਾਂਕ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ non_ਸ਼ਬਦ_ਸੂਚਕਾਂਕ = ਸ਼ਬਦ_ਗਿਣਤੀ_2;
    hb_map_t ri_map;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < ਅੰਕ_regions; r++)
    {
      ਜੇ (!delta_sz[r]) ਜਾਰੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ new_r = (delta_sz[r] == k_ਸ਼ਬਦ)? ਸ਼ਬਦ_ਸੂਚਕਾਂਕ++ : non_ਸ਼ਬਦ_ਸੂਚਕਾਂਕ++;
      ਜੇ (!ri_map.ਸੈੱਟ (new_r, r))
        return_trace (ਝੂਠ);
    }

    ਸ਼ਬਦ_ਮਾਪ_ਗਿਣਤੀ = ਸ਼ਬਦ_ਗਿਣਤੀ_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ri_ਗਿਣਤੀ = ri_map.get_population ();
    regionIndices.len = ri_ਗਿਣਤੀ;
    ਜੇ (unlikely (!c->extend (ਇਹ))) return_trace (ਝੂਠ);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < ri_ਗਿਣਤੀ; r++)
    {
      hb_codepoint_t *idx;
      ਜੇ (!ri_map.has (r, &idx))
        return_trace (ਝੂਠ);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_ਬਾਈਟਾਂ = get_delta_ਬਾਈਟਾਂ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_ਮਾਪ = get_ਪੰਕਤੀ_ਮਾਪ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਪੰਕਤੀ_ਗਿਣਤੀ_3; i++)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = 0; r < ri_ਗਿਣਤੀ; r++)
      {
        ਪੂਰਨ delta = ਪੰਕਤੀਆਂ[i]->arrayZ[ri_map[r]];
        ਸੈੱਟ_ਵਸਤੂ_delta_fast (i, r, delta, delta_ਬਾਈਟਾਂ, ਪੰਕਤੀ_ਮਾਪ);
      }
    }
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ Var_ਅੰਕੜੇ *src,
		  ਅਚਲ hb_inc_bimap_t &inner_map,
		  ਅਚਲ hb_inc_bimap_t &region_map,
		  ਅਚਲ hb_ਸੈੱਟ_t *culled_regions = ਖਾਲੀ_ਸੂਚਕ)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ਵਸਤੂ_ਗਿਣਤੀ = inner_map.get_ਅਗਲਾ_ਮੁੱਲ ();

    /* Optimize word count */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ri_ਗਿਣਤੀ = src->regionIndices.len;
    ਗਿਣਤੀ delta_ਮਾਪ_t { kZero=0, k_Non_ਸ਼ਬਦ, k_ਸ਼ਬਦ };
    hb_ਸਦਿਸ਼_t<delta_ਮਾਪ_t> delta_sz;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_ਗਿਣਤੀ);
    ri_map.resize (ri_ਗਿਣਤੀ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ਸ਼ਬਦ_ਗਿਣਤੀ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r;

    ਅਚਲ HBUINT8 *src_delta_ਬਾਈਟਾਂ = src->get_delta_ਬਾਈਟਾਂ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ src_ਪੰਕਤੀ_ਮਾਪ = src->get_ਪੰਕਤੀ_ਮਾਪ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ src_ਸ਼ਬਦ_ਗਿਣਤੀ = src->ਸ਼ਬਦ_ਗਿਣਤੀ ();
    ਤਾਰਕਿਕ     src_long_ਸ਼ਬਦ = src->long_ਸ਼ਬਦ_2 ();

    ਤਾਰਕਿਕ has_long = ਝੂਠ;
    ਜੇ (src_long_ਸ਼ਬਦ)
    {
      ਲਈ (r = 0; r < src_ਸ਼ਬਦ_ਗਿਣਤੀ; r++)
      {
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ old_gid : inner_map.ਕੁੰਜੀਆਂ())
	{
	  int32_t delta = src->get_ਵਸਤੂ_delta_fast (old_gid, r, src_delta_ਬਾਈਟਾਂ, src_ਪੰਕਤੀ_ਮਾਪ);
	  ਜੇ (delta < -32768 || 32767 < delta)
	  {
	    has_long = ਸੱਚ;
	    ਰੋਕੋ;
	  }
        }
      }
    }

    ਨਿਸ਼ਾਨੀਵਾਲਾ min_ਹੱਦ = has_long ? -32768 : -128;
    ਨਿਸ਼ਾਨੀਵਾਲਾ max_ਹੱਦ = has_long ? +32767 : +127;
    ਲਈ (r = 0; r < ri_ਗਿਣਤੀ; r++)
    {
      ਤਾਰਕਿਕ short_circuit = src_long_ਸ਼ਬਦ == has_long && src_ਸ਼ਬਦ_ਗਿਣਤੀ <= r;

      delta_sz[r] = kZero;
      ਜੇ (culled_regions && culled_regions->has (src->regionIndices[r]))
	ਜਾਰੀ; /* unreachable region: drop its column */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ old_gid : inner_map.ਕੁੰਜੀਆਂ())
      {
	int32_t delta = src->get_ਵਸਤੂ_delta_fast (old_gid, r, src_delta_ਬਾਈਟਾਂ, src_ਪੰਕਤੀ_ਮਾਪ);
	ਜੇ (delta < min_ਹੱਦ || max_ਹੱਦ < delta)
	{
	  delta_sz[r] = k_ਸ਼ਬਦ;
	  new_ਸ਼ਬਦ_ਗਿਣਤੀ++;
	  ਰੋਕੋ;
	}
	ਨਹੀਂਤਾਂ ਜੇ (delta != 0)
	{
	  delta_sz[r] = k_Non_ਸ਼ਬਦ;
	  ਜੇ (short_circuit)
	    ਰੋਕੋ;
	}
      }
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ਬਦ_ਸੂਚਕਾਂਕ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ non_ਸ਼ਬਦ_ਸੂਚਕਾਂਕ = new_ਸ਼ਬਦ_ਗਿਣਤੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ri_ਗਿਣਤੀ = 0;
    ਲਈ (r = 0; r < ri_ਗਿਣਤੀ; r++)
      ਜੇ (delta_sz[r])
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ new_r = (delta_sz[r] == k_ਸ਼ਬਦ)? ਸ਼ਬਦ_ਸੂਚਕਾਂਕ++ : non_ਸ਼ਬਦ_ਸੂਚਕਾਂਕ++;
	ri_map[new_r] = r;
	new_ri_ਗਿਣਤੀ++;
      }

    ਸ਼ਬਦ_ਮਾਪ_ਗਿਣਤੀ = new_ਸ਼ਬਦ_ਗਿਣਤੀ | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_ਗਿਣਤੀ;

    ਜੇ (unlikely (!c->extend (ਇਹ))) return_trace (ਝੂਠ);

    ਲਈ (r = 0; r < new_ri_ਗਿਣਤੀ; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_ਬਾਈਟਾਂ = get_delta_ਬਾਈਟਾਂ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_ਮਾਪ = get_ਪੰਕਤੀ_ਮਾਪ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਵਸਤੂ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old = inner_map.backward (i);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = 0; r < new_ri_ਗਿਣਤੀ; r++)
	ਸੈੱਟ_ਵਸਤੂ_delta_fast (i, r,
			     src->get_ਵਸਤੂ_delta_fast (old, ri_map[r],
						       src_delta_ਬਾਈਟਾਂ, src_ਪੰਕਤੀ_ਮਾਪ),
			     delta_ਬਾਈਟਾਂ, ਪੰਕਤੀ_ਮਾਪ);
    }

    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_region_refs (hb_ਸੈੱਟ_t &region_indices, ਅਚਲ hb_inc_bimap_t &inner_map) ਅਚਲ
  {
    ਅਚਲ HBUINT8 *delta_ਬਾਈਟਾਂ = get_delta_ਬਾਈਟਾਂ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_ਮਾਪ = get_ਪੰਕਤੀ_ਮਾਪ ();

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = 0; r < regionIndices.len; r++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region = regionIndices.arrayZ[r];
      ਜੇ (region_indices.has (region)) ਜਾਰੀ;
      ਲਈ (hb_codepoint_t old_gid : inner_map.ਕੁੰਜੀਆਂ())
	ਜੇ (get_ਵਸਤੂ_delta_fast (old_gid, r, delta_ਬਾਈਟਾਂ, ਪੰਕਤੀ_ਮਾਪ) != 0)
	{
	  region_indices.ਜੋੜੋ (region);
	  ਰੋਕੋ;
	}
    }
  }

  ਜਨਤਕ:
  ਅਚਲ HBUINT8 *get_delta_ਬਾਈਟਾਂ () ਅਚਲ
  { ਵਾਪਸ &StructAfter<HBUINT8> (regionIndices); }

  ਸੁਰੱਖਿਅਤ:
  HBUINT8 *get_delta_ਬਾਈਟਾਂ ()
  { ਵਾਪਸ &StructAfter<HBUINT8> (regionIndices); }

  ਜਨਤਕ:
  int32_t get_ਵਸਤੂ_delta_fast (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਸਤੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region,
			       ਅਚਲ HBUINT8 *delta_ਬਾਈਟਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_ਮਾਪ) ਅਚਲ
  {
    ਜੇ (unlikely (ਵਸਤੂ >= ਵਸਤੂ_ਗਿਣਤੀ || region >= regionIndices.len)) ਵਾਪਸ 0;

    ਅਚਲ HBINT8 *p = (ਅਚਲ HBINT8 *) delta_ਬਾਈਟਾਂ + ਵਸਤੂ * ਪੰਕਤੀ_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ਬਦ_ਗਿਣਤੀ_2 = ਸ਼ਬਦ_ਗਿਣਤੀ ();
    ਤਾਰਕਿਕ is_long = long_ਸ਼ਬਦ_2 ();
    ਜੇ (is_long)
    {
      ਜੇ (region < ਸ਼ਬਦ_ਗਿਣਤੀ_2)
	ਵਾਪਸ ((ਅਚਲ HBINT32 *) p)[region];
      ਨਹੀਂਤਾਂ
	ਵਾਪਸ ((ਅਚਲ HBINT16 *)(p + HBINT32::static_ਮਾਪ * ਸ਼ਬਦ_ਗਿਣਤੀ_2))[region - ਸ਼ਬਦ_ਗਿਣਤੀ_2];
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (region < ਸ਼ਬਦ_ਗਿਣਤੀ_2)
	ਵਾਪਸ ((ਅਚਲ HBINT16 *) p)[region];
      ਨਹੀਂਤਾਂ
	ਵਾਪਸ (p + HBINT16::static_ਮਾਪ * ਸ਼ਬਦ_ਗਿਣਤੀ_2)[region - ਸ਼ਬਦ_ਗਿਣਤੀ_2];
    }
  }
  int32_t get_ਵਸਤੂ_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਸਤੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region) ਅਚਲ
  {
     ਵਾਪਸ get_ਵਸਤੂ_delta_fast (ਵਸਤੂ, region,
				 get_delta_ਬਾਈਟਾਂ (),
				 get_ਪੰਕਤੀ_ਮਾਪ ());
  }

  ਸੁਰੱਖਿਅਤ:
  ਖਾਲੀ ਸੈੱਟ_ਵਸਤੂ_delta_fast (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਸਤੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region, int32_t delta,
			    HBUINT8 *delta_ਬਾਈਟਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_ਮਾਪ)
  {
    HBINT8 *p = (HBINT8 *) delta_ਬਾਈਟਾਂ + ਵਸਤੂ * ਪੰਕਤੀ_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ਬਦ_ਗਿਣਤੀ_2 = ਸ਼ਬਦ_ਗਿਣਤੀ ();
    ਤਾਰਕਿਕ is_long = long_ਸ਼ਬਦ_2 ();
    ਜੇ (is_long)
    {
      ਜੇ (region < ਸ਼ਬਦ_ਗਿਣਤੀ_2)
	((HBINT32 *) p)[region] = delta;
      ਨਹੀਂਤਾਂ
	((HBINT16 *)(p + HBINT32::static_ਮਾਪ * ਸ਼ਬਦ_ਗਿਣਤੀ_2))[region - ਸ਼ਬਦ_ਗਿਣਤੀ_2] = delta;
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (region < ਸ਼ਬਦ_ਗਿਣਤੀ_2)
	((HBINT16 *) p)[region] = delta;
      ਨਹੀਂਤਾਂ
	(p + HBINT16::static_ਮਾਪ * ਸ਼ਬਦ_ਗਿਣਤੀ_2)[region - ਸ਼ਬਦ_ਗਿਣਤੀ_2] = delta;
    }
  }
  ਖਾਲੀ ਸੈੱਟ_ਵਸਤੂ_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਸਤੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region, int32_t delta)
  {
    ਸੈੱਟ_ਵਸਤੂ_delta_fast (ਵਸਤੂ, region, delta,
			 get_delta_ਬਾਈਟਾਂ (),
			 get_ਪੰਕਤੀ_ਮਾਪ ());
  }

  ਤਾਰਕਿਕ long_ਸ਼ਬਦ_2 () ਅਚਲ { ਵਾਪਸ ਸ਼ਬਦ_ਮਾਪ_ਗਿਣਤੀ & 0x8000u /* LONG_WORDS */; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ਬਦ_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ ਸ਼ਬਦ_ਮਾਪ_ਗਿਣਤੀ & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16		ਵਸਤੂ_ਗਿਣਤੀ;
  HBUINT16		ਸ਼ਬਦ_ਮਾਪ_ਗਿਣਤੀ;
  Array16_ਦਾ<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, regionIndices);
};

ਬਣਤਰ Multi_Var_ਅੰਕੜੇ
{
  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_ਜੋੜੋ_saturate (min_ਮਾਪ - regionIndices.min_ਮਾਪ,
				     regionIndices.get_ਮਾਪ ()); }

  ਅਚਲ TupleList &get_delta_sets () ਅਚਲ
  { ਵਾਪਸ ਇਹ+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  ਖਾਲੀ get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner,
		  ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
		  ਅਚਲ SparseVarRegionList &regions,
		  hb_array_t<ਦੁੱਗਣਾ> ਬਾਹਰਲਾ,
		  hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ &deltaSets = get_delta_sets ();

    ਸਵੈਚਾਲਿਤ ਮੁੱਲਾਂ_iter = deltaSets.fetcher (inner);
    ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਗਿਣਤੀ = regionIndices.len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ skip = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਸੂਚਕਾਂਕ_2 = 0; region_ਸੂਚਕਾਂਕ_2 < region_ਗਿਣਤੀ; region_ਸੂਚਕਾਂਕ_2++)
    {
      ਦਸ਼ਮਲਵ scalar = regions.evaluate (regionIndices.arrayZ[region_ਸੂਚਕਾਂਕ_2],
				       coords, coord_ਗਿਣਤੀ,
				       cache);
      // We skip lazily. Helps with the tail end.
      ਜੇ (scalar == 0.0f)
        skip += ਬਾਹਰਲਾ.ਲੰਮਾਈ;
      ਨਹੀਂਤਾਂ
      {
        ਜੇ (skip)
	{
	  ਮੁੱਲਾਂ_iter.skip (skip);
	  skip = 0;
	}
	ਮੁੱਲਾਂ_iter.ਜੋੜੋ_ਮੰਜ਼ਿਲ_2 (ਬਾਹਰਲਾ, scalar);
      }
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, ਇਹ));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਵਸਤੂ_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ get_delta_sets ().ਗਿਣਤੀ_2; }

  ਖਾਲੀ collect_region_refs (hb_ਸੈੱਟ_t &region_indices) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ region : regionIndices)
      region_indices.ਜੋੜੋ (region);
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ Multi_Var_ਅੰਕੜੇ *src,
		  ਅਚਲ hb_inc_bimap_t &inner_map,
		  ਅਚਲ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਗਿਣਤੀ_2 = src->regionIndices.len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਲੇਖ_ਮਾਪ = HBUINT8::static_ਮਾਪ +
			   Array16_ਦਾ<HBUINT16>::min_ਮਾਪ +
			   region_ਗਿਣਤੀ_2 * HBUINT16::static_ਮਾਪ +
			   HBUINT32::static_ਮਾਪ;
    ਜੇ (unlikely (!c->extend_ਮਾਪ (ਇਹ, ਸਿਰਲੇਖ_ਮਾਪ, ਝੂਠ)))
      return_trace (ਝੂਠ);
    format = 1;
    regionIndices.len = region_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < regionIndices.len; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ old_region = src->regionIndices[i];
      ਜੇ (unlikely (!region_map.has (old_region)))
	return_trace (ਝੂਠ);
      regionIndices[i] = region_map.get (old_region);
    }

    ਅਚਲ ਸਵੈਚਾਲਿਤ &src_delta_sets = src->get_delta_sets ();
    hb_ਸਦਿਸ਼_t<hb_ubytes_t> delta_sets;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ = 0;
    ਜੇ (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < inner_map.get_population (); i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ old_inner = inner_map.backward (i);
      ਜੇ (unlikely (old_inner >= src_delta_sets.ਗਿਣਤੀ_2)) return_trace (ਝੂਠ);
      hb_ubytes_t ਬਾਈਟਾਂ = static_cast<ਅਚਲ CFF2_ਸੂਚਕਾਂਕ &> (src_delta_sets)[old_inner];
      ਅੰਕੜੇ_ਮਾਪ = hb_unsigned_ਜੋੜੋ_saturate (ਅੰਕੜੇ_ਮਾਪ, ਬਾਈਟਾਂ.ਲੰਮਾਈ);
      delta_sets.push (ਬਾਈਟਾਂ);
    }
    ਜੇ (unlikely (ਅੰਕੜੇ_ਮਾਪ == UINT_MAX || delta_sets.ਵਿੱਚ_error ()))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ &ਬਾਹਰਲਾ_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (ਬਾਹਰਲਾ_delta_sets.serialize_serialize (c, delta_sets.iter (), &ਅੰਕੜੇ_ਮਾਪ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT8	      format; // 1
  Array16_ਦਾ<HBUINT16> regionIndices;
  Offset32_ਮੰਜ਼ਿਲ<TupleList> deltaSetsX;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (7);
};

ਬਣਤਰ ਵਸਤੂ_Variation_Store
{
  ਮਿੱਤਰ ਬਣਤਰ ਵਸਤੂ_variations_t;

  hb_scalar_cache_t *create_cache () ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ hb_scalar_cache_t::create (0);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ hb_scalar_cache_t::create ((ਇਹ+regions).region_ਗਿਣਤੀ);
  }

  ਸਥਿਰ ਖਾਲੀ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
  Float get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner,
		   ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
		   hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ Float (0);
#ਜੇ_ਅੰਤ

    ਜੇ (unlikely (outer >= ਅੰਕੜੇ_Sets.len))
      ਵਾਪਸ Float (0);

    ਵਾਪਸ (ਇਹ+ਅੰਕੜੇ_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_ਗਿਣਤੀ,
					     ਇਹ+regions,
					     cache);
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ has_delta_ਸੈੱਟ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer = ਸੂਚਕਾਂਕ_2 >> 16;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner = ਸੂਚਕਾਂਕ_2 & 0xFFFF;
    ਵਾਪਸ outer < ਅੰਕੜੇ_Sets.len &&
	   inner < (ਇਹ+ਅੰਕੜੇ_Sets[outer]).get_ਵਸਤੂ_ਗਿਣਤੀ ();
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
  Float get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2,
		   ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
		   hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer = ਸੂਚਕਾਂਕ_2 >> 16;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner = ਸੂਚਕਾਂਕ_2 & 0xFFFF;
    ਵਾਪਸ get_delta<Float> (outer, inner, coords, coord_ਗਿਣਤੀ, cache);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
  Float get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2,
		   hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
		   hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਵਾਪਸ get_delta<Float> (ਸੂਚਕਾਂਕ_2,
		      coords.arrayZ, coords.ਲੰਮਾਈ,
		      cache);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ਇਹ) &&
		  ਅੰਕੜੇ_Sets.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  ਤਾਰਕਿਕ has_long,
                  ਅਚਲ hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t>& axis_ਟੈਗ_ਸੂਚੀ,
                  ਅਚਲ hb_ਸਦਿਸ਼_t<ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>*>& region_list,
                  ਅਚਲ hb_ਸਦਿਸ਼_t<delta_ਪੰਕਤੀ_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (ਇਹ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    format = 1;
    ਜੇ (!regions.serialize_serialize (c, axis_ਟੈਗ_ਸੂਚੀ, region_list))
      return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_var_ਅੰਕੜੇ = vardata_encodings.ਲੰਮਾਈ;
    ਜੇ (!ਅੰਕ_var_ਅੰਕੜੇ) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (ਅੰਕੜੇ_Sets.len, ਅੰਕ_var_ਅੰਕੜੇ,
                                    HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
      return_trace (ਝੂਠ);

    ਜੇ (unlikely (!c->extend (ਅੰਕੜੇ_Sets))) return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_var_ਅੰਕੜੇ; i++)
      ਜੇ (!ਅੰਕੜੇ_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].ਚੀਜ਼ਾਂ))
        return_trace (ਝੂਠ);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ ਵਸਤੂ_Variation_Store *src,
		  ਅਚਲ hb_array_t <ਅਚਲ hb_inc_bimap_t> &inner_maps,
		  ਅਚਲ hb_ਸੈੱਟ_t *culled_regions = ਖਾਲੀ_ਸੂਚਕ)
  {
    TRACE_SERIALIZE (ਇਹ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ

    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੈੱਟ_ਗਿਣਤੀ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < inner_maps.ਲੰਮਾਈ; i++)
      ਜੇ (inner_maps[i].get_population ())
	ਸੈੱਟ_ਗਿਣਤੀ++;

    format = 1;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &src_regions = src+src->regions;

    hb_ਸੈੱਟ_t region_indices;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < inner_maps.ਲੰਮਾਈ; i++)
      (src+src->ਅੰਕੜੇ_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    ਜੇ (region_indices.ਵਿੱਚ_error ())
      return_trace (ਝੂਠ);

    region_indices.del_range ((src_regions).region_ਗਿਣਤੀ, hb_ਸੈੱਟ_t::INVALID);

    ਜੇ (culled_regions)
      region_indices.ਘਟਾਓ (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (ਬਿਨਾਨਿਸ਼ਾਨੀ _) { region_map.ਜੋੜੋ(_); })
    ;
    ਜੇ (region_map.ਵਿੱਚ_error())
      return_trace (ਝੂਠ);

    ਜੇ (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (ਝੂਠ);

    ਅੰਕੜੇ_Sets.len = ਸੈੱਟ_ਗਿਣਤੀ;
    ਜੇ (unlikely (!c->extend (ਅੰਕੜੇ_Sets))) return_trace (ਝੂਠ);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੈੱਟ_ਸੂਚਕਾਂਕ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < inner_maps.ਲੰਮਾਈ; i++)
    {
      ਜੇ (!inner_maps[i].get_population ()) ਜਾਰੀ;
      ਜੇ (unlikely (!ਅੰਕੜੇ_Sets[ਸੈੱਟ_ਸੂਚਕਾਂਕ++]
		     .serialize_serialize (c, &(src+src->ਅੰਕੜੇ_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  ਵਸਤੂ_Variation_Store *copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਖਾਲੀ_ਸੂਚਕ);

    hb_ਸਦਿਸ਼_t <hb_inc_bimap_t> inner_maps;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਅੰਕੜੇ_Sets.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      ਜੇ (unlikely (!c->propagate_error(inner_maps)))
        return_trace(ਖਾਲੀ_ਸੂਚਕ);
      ਸਵੈਚਾਲਿਤ &ਅੰਕੜੇ = ਇਹ+ਅੰਕੜੇ_Sets[i];

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਗਿਣਤੀ = ਅੰਕੜੇ.get_ਵਸਤੂ_ਗਿਣਤੀ ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਵਸਤੂ_ਗਿਣਤੀ; j++)
	map->ਜੋੜੋ (j);
    }

    ਜੇ (unlikely (!ਬਾਹਰਲਾ->serialize (c, ਇਹ, inner_maps))) return_trace (ਖਾਲੀ_ਸੂਚਕ);

    return_trace (ਬਾਹਰਲਾ);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  ਤਾਰਕਿਕ collect_dead_regions (ਅਚਲ hb_map_t &axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
			     ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> &reachable_ranges,
			     hb_ਸੈੱਟ_t &dead_regions /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (!reachable_ranges.get_population ()) ਵਾਪਸ ਸੱਚ;
    ਅਚਲ VarRegionList &region_list = ਇਹ+regions;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = region_list.region_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_hashmap_t<hb_ਟੈਗ_t, Triple> axis_tuples;
      ਜੇ (!region_list.get_var_region (i, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, axis_tuples))
	ਵਾਪਸ ਝੂਠ;
      ਜੇ (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.ਜੋੜੋ (i);
    }
    ਵਾਪਸ !dead_regions.ਵਿੱਚ_error ();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c, ਅਚਲ hb_array_t<ਅਚਲ hb_inc_bimap_t> &inner_maps) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ

    ਵਸਤੂ_Variation_Store *varstore_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed<ਵਸਤੂ_Variation_Store> ();
    ਜੇ (unlikely (!varstore_ਅਭਾਜ)) return_trace (ਝੂਠ);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_ਸੈੱਟ_t dead_regions;
    ਜੇ (c->plan->has_avar2)
      ਜੇ (!collect_dead_regions (c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (ਝੂਠ);

    varstore_ਅਭਾਜ->serialize (c->serializer, ਇਹ, inner_maps,
			       dead_regions.get_population () ? &dead_regions : ਖਾਲੀ_ਸੂਚਕ);

    return_trace (
        !c->serializer->ਵਿੱਚ_error()
        && varstore_ਅਭਾਜ->ਅੰਕੜੇ_Sets);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_region_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ (ਇਹ+ਅੰਕੜੇ_Sets[major]).get_region_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ ();
  }

  ਖਾਲੀ get_region_scalars (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major,
			   ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
			   ਦਸ਼ਮਲਵ *scalars /*OUT*/,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_scalars) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_scalars; i++)
      scalars[i] = 0.f;
    ਵਾਪਸ;
#ਜੇ_ਅੰਤ

    (ਇਹ+ਅੰਕੜੇ_Sets[major]).get_region_scalars (coords, coord_ਗਿਣਤੀ,
					       ਇਹ+regions,
					       &scalars[0], ਅੰਕ_scalars);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ () ਅਚਲ
   {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
     ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
     ਵਾਪਸ ਅੰਕੜੇ_Sets.len;
   }

  ਅਚਲ Var_ਅੰਕੜੇ& get_sub_ਸਾਰਣੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
     ਵਾਪਸ Null (Var_ਅੰਕੜੇ);
#ਜੇ_ਅੰਤ
     ਵਾਪਸ ਇਹ+ਅੰਕੜੇ_Sets[i];
  }

  ਅਚਲ VarRegionList& get_region_list () ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
     ਵਾਪਸ Null (VarRegionList);
#ਜੇ_ਅੰਤ
     ਵਾਪਸ ਇਹ+regions;
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16				format;
  Offset32_ਮੰਜ਼ਿਲ<VarRegionList>		regions;
  Array16_ਦਾ_Offset32_ਮੰਜ਼ਿਲ<Var_ਅੰਕੜੇ>		ਅੰਕੜੇ_Sets;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (8, ਅੰਕੜੇ_Sets);
};

ਬਣਤਰ Multi_ਵਸਤੂ_Variation_Store
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ hb_scalar_cache_t::create (0);
#ਜੇ_ਅੰਤ
    ਸਵੈਚਾਲਿਤ &r = ਇਹ+regions;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = r.regions.len;

    ਵਾਪਸ hb_scalar_cache_t::create (ਗਿਣਤੀ_2, static_cache);
  }

  ਸਥਿਰ ਖਾਲੀ destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = ਖਾਲੀ_ਸੂਚਕ)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  ਨਿਜੀ:
  ਖਾਲੀ get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner,
		  ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
		  hb_array_t<ਦੁੱਗਣਾ> ਬਾਹਰਲਾ,
		  hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ;
#ਜੇ_ਅੰਤ

    ਜੇ (unlikely (outer >= ਅੰਕੜੇ_Sets.len))
      ਵਾਪਸ;

    ਵਾਪਸ (ਇਹ+ਅੰਕੜੇ_Sets[outer]).get_delta (inner,
					     coords, coord_ਗਿਣਤੀ,
					     ਇਹ+regions,
					     ਬਾਹਰਲਾ,
					     cache);
  }

  ਜਨਤਕ:
  ਖਾਲੀ get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2,
		  ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ,
		  hb_array_t<ਦੁੱਗਣਾ> ਬਾਹਰਲਾ,
		  hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer = ਸੂਚਕਾਂਕ_2 >> 16;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner = ਸੂਚਕਾਂਕ_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_ਗਿਣਤੀ, ਬਾਹਰਲਾ, cache);
  }
  ਖਾਲੀ get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2,
		  hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
		  hb_array_t<ਦੁੱਗਣਾ> ਬਾਹਰਲਾ,
		  hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਵਾਪਸ get_delta (ਸੂਚਕਾਂਕ_2,
		      coords.arrayZ, coords.ਲੰਮਾਈ,
		      ਬਾਹਰਲਾ,
		      cache);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ਇਹ) &&
		  ਅੰਕੜੇ_Sets.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ create_subset_plan (ਅਚਲ hb_ਸੈੱਟ_t &var_indices,
			   hb_ਸਦਿਸ਼_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ਅਚਲ
  {
    ਜੇ (unlikely (!inner_maps->resize (ਅੰਕੜੇ_Sets.len))) ਵਾਪਸ ਝੂਠ;
    ਲਈ (hb_codepoint_t var_idx : var_indices)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ outer = var_idx >> 16;
      ਬਿਨਾਨਿਸ਼ਾਨੀ inner = var_idx & 0xFFFFu;
      ਜੇ (unlikely (outer >= ਅੰਕੜੇ_Sets.len ||
		    inner >= (ਇਹ+ਅੰਕੜੇ_Sets[outer]).get_ਵਸਤੂ_ਗਿਣਤੀ ()))
	ਵਾਪਸ ਝੂਠ;
      (*inner_maps)[outer].ਜੋੜੋ (inner);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ new_outer = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ outer = 0; outer < inner_maps->ਲੰਮਾਈ; outer++)
    {
      ਅਚਲ hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      ਜੇ (!inner_map.get_population ()) ਜਾਰੀ;
      ਜੇ (unlikely (inner_map.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ old_inner = inner_map.backward (new_inner);
	varidx_map->ਸੈੱਟ ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    ਵਾਪਸ !inner_maps->ਵਿੱਚ_error () && !varidx_map->ਵਿੱਚ_error ();
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ Multi_ਵਸਤੂ_Variation_Store *src,
		  ਅਚਲ hb_array_t<ਅਚਲ hb_inc_bimap_t> &inner_maps,
		  ਅਚਲ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ) ||
		  inner_maps.ਲੰਮਾਈ < src->ਅੰਕੜੇ_Sets.len))
      return_trace (ਝੂਠ);
    format = 1;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &src_regions = src+src->regions;
    hb_ਸੈੱਟ_t region_indices;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੈੱਟ_ਗਿਣਤੀ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < src->ਅੰਕੜੇ_Sets.len; i++)
    {
      ਜੇ (!inner_maps[i].get_population ()) ਜਾਰੀ;
      ਸੈੱਟ_ਗਿਣਤੀ++;
      (src+src->ਅੰਕੜੇ_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_ਸੈੱਟ_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.ਜੋੜੋ_ਸੈੱਟ (&region_indices);
    ਜੇ (unlikely (region_indices.ਵਿੱਚ_error () || region_map.ਵਿੱਚ_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (ਝੂਠ);

    ਅੰਕੜੇ_Sets.len = ਸੈੱਟ_ਗਿਣਤੀ;
    ਜੇ (unlikely (!c->extend (ਅੰਕੜੇ_Sets))) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੈੱਟ_ਸੂਚਕਾਂਕ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < src->ਅੰਕੜੇ_Sets.len; i++)
    {
      ਜੇ (!inner_maps[i].get_population ()) ਜਾਰੀ;
      ਜੇ (unlikely (!ਅੰਕੜੇ_Sets[ਸੈੱਟ_ਸੂਚਕਾਂਕ++].serialize_serialize (
			    c, &(src+src->ਅੰਕੜੇ_Sets[i]), inner_maps[i], region_map)))
	return_trace (ਝੂਠ);
    }
    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16				format; // 1
  Offset32_ਮੰਜ਼ਿਲ<SparseVarRegionList>	regions;
  Array16_ਦਾ_Offset32_ਮੰਜ਼ਿਲ<Multi_Var_ਅੰਕੜੇ>	ਅੰਕੜੇ_Sets;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (8, ਅੰਕੜੇ_Sets);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Map_ਗਿਣਤੀ_T>
ਬਣਤਰ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map_Format01
{
  ਮਿੱਤਰ ਬਣਤਰ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map;

  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (map_ਗਿਣਤੀ_2, get_ਚੌੜਾਈ (), min_ਮਾਪ); }

  ਨਿਜੀ:
  Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map_Format01* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed (ਇਹ));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ T &plan)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੌੜਾਈ = plan.get_ਚੌੜਾਈ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = plan.get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ ();
    ਅਚਲ hb_array_t<ਅਚਲ uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (output_map.ਲੰਮਾਈ && ((((inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ-1)&~0xF)!=0) || (((ਚੌੜਾਈ-1)&~0x3)!=0))))
      return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    ਇੰਦਰਾਜ_Format = ((ਚੌੜਾਈ-1)<<4)|(inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ-1);
    map_ਗਿਣਤੀ_2 = output_map.ਲੰਮਾਈ;
    HBUINT8 *p = c->allocate_ਮਾਪ<HBUINT8> (ਚੌੜਾਈ * output_map.ਲੰਮਾਈ);
    ਜੇ (unlikely (!p)) return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < output_map.ਲੰਮਾਈ; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v = output_map.arrayZ[i];
      ਜੇ (v)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer = v >> 16;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner = v & 0xFFFF;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ u = (outer << inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ) | inner;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ w = ਚੌੜਾਈ; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += ਚੌੜਾਈ;
    }
    return_trace (ਸੱਚ);
  }

  HB_ALWAYS_INLINE
  uint32_t map (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v) ਅਚਲ /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    ਜੇ (!map_ਗਿਣਤੀ_2)
      ਵਾਪਸ v;
    ਵਾਪਸ _map (v);
  }

  HB_HOT
  uint32_t _map (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v) ਅਚਲ /* Returns 16.16 outer.inner. */
  {
    ਜੇ (v >= map_ਗਿਣਤੀ_2)
      v = map_ਗਿਣਤੀ_2 - 1;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ u = 0;
    { /* Fetch it. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ w = get_ਚੌੜਾਈ ();
      ਅਚਲ HBUINT8 *p = map_ਅੰਕੜੇ_Z.arrayZ + w * v;
      ਲਈ (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n = get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ ();
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ outer = u >> n;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    ਵਾਪਸ u;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_map_ਗਿਣਤੀ () ਅਚਲ       { ਵਾਪਸ map_ਗਿਣਤੀ_2; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਚੌੜਾਈ () ਅਚਲ           { ਵਾਪਸ ((ਇੰਦਰਾਜ_Format >> 4) & 3) + 1; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ (ਇੰਦਰਾਜ_Format & 0xF) + 1; }


  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
                  c->check_range (map_ਅੰਕੜੇ_Z.arrayZ,
                                  map_ਗਿਣਤੀ_2,
                                  get_ਚੌੜਾਈ ()));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       ਇੰਦਰਾਜ_Format;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_ਗਿਣਤੀ_T     map_ਗਿਣਤੀ_2;       /* The number of mapping entries. */
  Unsized_Array_ਦਾ<HBUINT8>
                map_ਅੰਕੜੇ_Z;       /* The delta-set index mapping data. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2+Map_ਗਿਣਤੀ_T::static_ਮਾਪ, map_ਅੰਕੜੇ_Z);
};

ਬਣਤਰ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ T &plan)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = plan.get_output_map ().ਲੰਮਾਈ;
    u.format.v = ਲੰਮਾਈ <= 0xFFFF ? 0 : 1;
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  uint32_t map (ਬਿਨਾਨਿਸ਼ਾਨੀ v) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ (u.format0.map (v));
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ (u.format1.map (v));
    ਮੂਲ:ਵਾਪਸ v;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_map_ਗਿਣਤੀ () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_map_ਗਿਣਤੀ ();
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_map_ਗਿਣਤੀ ();
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਚੌੜਾਈ () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_ਚੌੜਾਈ ();
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_ਚੌੜਾਈ ();
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ () ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ ();
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ ();
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); return_trace (reinterpret_cast<Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *> (u.format0.copy (c)));
    ਹਾਲਤ 1: hb_barrier (); return_trace (reinterpret_cast<Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *> (u.format1.copy (c)));
    ਮੂਲ:return_trace (ਖਾਲੀ_ਸੂਚਕ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT8 v; }             format;         /* Format identifier */
  Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map_Format01<HBUINT16> format0;
  Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map_Format01<HBUINT32> format1;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (1, format.v);
};


ਬਣਤਰ ਵਸਤੂ_Var_Store_Instancer
{
  ਵਸਤੂ_Var_Store_Instancer (ਅਚਲ ਵਸਤੂ_Variation_Store *varStore_,
			 ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *varIdxMap,
			 hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
			 hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    ਜੇ (!varStore)
      varStore = &Null(ਵਸਤੂ_Variation_Store);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    ਜੇ (!hb_any (coords))
      ਇਹ->coords = {};
  }

  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ varStore && ਤਾਰਕਿਕ (coords); }

  ਦਸ਼ਮਲਵ ਸੰਚਾਲਕ[] (uint32_t varIdx) ਅਚਲ
  { ਵਾਪਸ (*ਇਹ) (varIdx); }

  ਦਸ਼ਮਲਵ ਸੰਚਾਲਕ() (uint32_t varIdx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਵਿੱਥ = 0) ਅਚਲ
  { ਵਾਪਸ get_delta (varIdx, ਵਿੱਥ); }

  ਦੁੱਗਣਾ get_condition_delta (uint32_t varIdx) ਅਚਲ
  { ਵਾਪਸ get_delta<ਦੁੱਗਣਾ> (varIdx); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Float = ਦਸ਼ਮਲਵ>
  Float get_delta (uint32_t varIdx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਵਿੱਥ = 0) ਅਚਲ
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   ਜੇ (!coords || varIdx == VarIdx::NO_VARIATION)
     ਵਾਪਸ Float (0);

    varIdx += ਵਿੱਥ;
    ਜੇ (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    ਵਾਪਸ varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ਅਚਲ ਵਸਤੂ_Variation_Store *varStore;
  ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *varIdxMap;
  hb_array_t<ਅਚਲ ਪੂਰਨ> coords;
  hb_scalar_cache_t *cache;
};

ਬਣਤਰ Multi_ਵਸਤੂ_Var_Store_Instancer
{
  Multi_ਵਸਤੂ_Var_Store_Instancer (ਅਚਲ Multi_ਵਸਤੂ_Variation_Store *varStore,
			      ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *varIdxMap,
			      hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
			      hb_scalar_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    ਜੇ (!varStore)
      varStore = &Null(Multi_ਵਸਤੂ_Variation_Store);
  }

  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ varStore; }

  ਦੁੱਗਣਾ get_condition_delta (uint32_t varIdx) ਅਚਲ
  { ਵਾਪਸ (ਦੁੱਗਣਾ) (*ਇਹ)[varIdx]; }

  ਦੁੱਗਣਾ ਸੰਚਾਲਕ[] (uint32_t varIdx) ਅਚਲ
  {
    ਦੁੱਗਣਾ v = 0;
    (*ਇਹ) (hb_array (&v, 1), varIdx);
    ਵਾਪਸ v;
  }

  ਖਾਲੀ ਸੰਚਾਲਕ() (hb_array_t<ਦੁੱਗਣਾ> ਬਾਹਰਲਾ, uint32_t varIdx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਵਿੱਥ = 0) ਅਚਲ
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    ਜੇ (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += ਵਿੱਥ;
      ਜੇ (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, ਬਾਹਰਲਾ, cache);
    }
    ਨਹੀਂਤਾਂ
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਾਹਰਲਾ.ਲੰਮਾਈ; i++)
        ਬਾਹਰਲਾ.arrayZ[i] = 0.;
  }

  ਅਚਲ Multi_ਵਸਤੂ_Variation_Store *varStore;
  ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map *varIdxMap;
  hb_array_t<ਅਚਲ ਪੂਰਨ> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
ਗਿਣਤੀ Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_ਰਿਕਾਰਡ_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_ਰਿਕਾਰਡ_WITH_VAR = 3,
};

ਗਿਣਤੀ lookup_condition_subset_ਨਤੀਜਾ_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

ਬਣਤਰ Condition;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
ਸਥਿਰ ਤਾਰਕਿਕ
_hb_recurse_condition_evaluate (ਅਚਲ ਬਣਤਰ Condition &condition,
				ਅਚਲ ਪੂਰਨ *coords,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
				Instancer *instancer);

ਬਣਤਰ ConditionAxisRange
{
  ਮਿੱਤਰ ਬਣਤਰ Condition;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਅਚਲ hb_map_t *ਸੂਚਕਾਂਕ_map = &c->plan->axes_ਸੂਚਕਾਂਕ_map;
    ਜੇ (ਸੂਚਕਾਂਕ_map->is_empty ()) return_trace (ਸੱਚ);

    ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map = c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map;
    hb_codepoint_t *axis_ਟੈਗ_2;
    ਜੇ (!axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.has (axis_ਸੂਚਕਾਂਕ_2, &axis_ਟੈਗ_2) ||
        !ਸੂਚਕਾਂਕ_map->has (axis_ਸੂਚਕਾਂਕ_2))
      return_trace (ਝੂਠ);

    ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_ਹੱਦਬੰਦੀ{-1.0, 0.0, 1.0};
    Triple *normalized_ਹੱਦਬੰਦੀ;
    ਜੇ (normalized_axes_location.has (*axis_ਟੈਗ_2, &normalized_ਹੱਦਬੰਦੀ))
      axis_ਹੱਦਬੰਦੀ = *normalized_ਹੱਦਬੰਦੀ;

    ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    ਜੇ (axes_triple_distances.has (*axis_ਟੈਗ_2, &triple_dists))
      axis_triple_distances = *triple_dists;

    ਦਸ਼ਮਲਵ normalized_min = renormalize_ਮੁੱਲ ((ਦੁੱਗਣਾ) filter_Range_Min_ਮੁੱਲ.ਮੰਜ਼ਿਲ_float (), axis_ਹੱਦਬੰਦੀ, axis_triple_distances, ਝੂਠ);
    ਦਸ਼ਮਲਵ normalized_max = renormalize_ਮੁੱਲ ((ਦੁੱਗਣਾ) filter_Range_Max_ਮੁੱਲ.ਮੰਜ਼ਿਲ_float (), axis_ਹੱਦਬੰਦੀ, axis_triple_distances, ਝੂਠ);
    ਬਾਹਰਲਾ->filter_Range_Min_ਮੁੱਲ.ਸੈੱਟ_float (normalized_min);
    ਬਾਹਰਲਾ->filter_Range_Max_ਮੁੱਲ.ਸੈੱਟ_float (normalized_max);

    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->axis_ਸੂਚਕਾਂਕ_2, ਸੂਚਕਾਂਕ_map->get (axis_ਸੂਚਕਾਂਕ_2),
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਨਿਜੀ:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    //invalid axis index, drop the entire record
    ਜੇ (!c->axes_ਸੂਚਕਾਂਕ_ਟੈਗ_map->has (axis_ਸੂਚਕਾਂਕ_2))
      ਵਾਪਸ DROP_ਰਿਕਾਰਡ_WITH_VAR;

    hb_ਟੈਗ_t axis_ਟੈਗ_2 = c->axes_ਸੂਚਕਾਂਕ_ਟੈਗ_map->get (axis_ਸੂਚਕਾਂਕ_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_ਹੱਦਬੰਦੀ;
    ਤਾਰਕਿਕ axis_ਸੈੱਟ_by_user = ਝੂਠ;
    ਜੇ (c->axes_location->has (axis_ਟੈਗ_2, &axis_ਹੱਦਬੰਦੀ))
    {
      axis_range = *axis_ਹੱਦਬੰਦੀ;
      axis_ਸੈੱਟ_by_user = ਸੱਚ;
    }

    ਦਸ਼ਮਲਵ axis_min_val = axis_range.minimum;
    ਦਸ਼ਮਲਵ axis_default_val = axis_range.ਵਿਚਕਾਰ;
    ਦਸ਼ਮਲਵ axis_max_val = axis_range.ਵੱਧ_ਤੋਂ_ਵੱਧ;

    ਦਸ਼ਮਲਵ filter_min_val = filter_Range_Min_ਮੁੱਲ.ਮੰਜ਼ਿਲ_float ();
    ਦਸ਼ਮਲਵ filter_max_val = filter_Range_Max_ਮੁੱਲ.ਮੰਜ਼ਿਲ_float ();

    ਜੇ (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = ਝੂਠ;

    //condition not met, drop the entire record
    ਜੇ (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      ਵਾਪਸ DROP_ਰਿਕਾਰਡ_WITH_VAR;

    //condition met and axis pinned, drop the condition
    ਜੇ (axis_ਸੈੱਟ_by_user && axis_range.is_ਬਿੰਦੂ ())
      ਵਾਪਸ DROP_COND_WITH_VAR;

    ਜੇ (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_ਮੁੱਲ.ਮੰਜ਼ਿਲ_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_ਮੁੱਲ.ਮੰਜ਼ਿਲ_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->ਸੈੱਟ (axis_ਸੂਚਕਾਂਕ_2, val);
      ਵਾਪਸ KEEP_COND_WITH_VAR;
    }
    ਵਾਪਸ KEEP_ਰਿਕਾਰਡ_WITH_VAR;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		 Instancer *instancer HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    ਪੂਰਨ coord = axis_ਸੂਚਕਾਂਕ_2 < coord_len ? coords[axis_ਸੂਚਕਾਂਕ_2] : 0;
    ਵਾਪਸ filter_Range_Min_ਮੁੱਲ.ਮੰਜ਼ਿਲ_int () <= coord && coord <= filter_Range_Max_ਮੁੱਲ.ਮੰਜ਼ਿਲ_int ();
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axis_ਸੂਚਕਾਂਕ_2;
  F2DOT14	filter_Range_Min_ਮੁੱਲ;
  F2DOT14	filter_Range_Max_ਮੁੱਲ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ Condition_ਮੁੱਲ
{
  ਮਿੱਤਰ ਬਣਤਰ Condition;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (varIdx == VarIdx::NO_VARIATION)
      return_trace (ਸੱਚ);

    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *condition_ਮੁੱਲ;
    ਜੇ (c->plan->layout_condition_idx_ਮੁੱਲ_map.has ((uintptr_t) ਇਹ, &condition_ਮੁੱਲ))
    {
      ਬਾਹਰਲਾ->default_ਮੁੱਲ_2 = condition_ਮੁੱਲ->second;
      ਬਾਹਰਲਾ->varIdx = condition_ਮੁੱਲ->ਪਹਿਲਾ;
      return_trace (ਸੱਚ);
    }

    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *new_varidx_delta;
    ਜੇ (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (ਝੂਠ);

    ਜੇ (unlikely (!c->serializer->check_ਸੌਂਪੋ (
		ਬਾਹਰਲਾ->default_ਮੁੱਲ_2,
		default_ਮੁੱਲ_2 + hb_ਦੂਜਾ (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
      return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->varIdx = hb_ਪਹਿਲਾ (*new_varidx_delta);
    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		 Instancer *instancer) ਅਚਲ
  {
    ਦੁੱਗਣਾ value = default_ਮੁੱਲ_2;
    value += instancer->get_condition_delta (varIdx);
    ਵਾਪਸ value > 0;
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_ਮੁੱਲ_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ ConditionAnd
{
  ਮਿੱਤਰ ਬਣਤਰ Condition;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ)))
      return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : conditions)
    {
      ਜੇ (ਵਿੱਥ.is_null ())
      {
	ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ_ਵਿੱਥ = ਬਾਹਰਲਾ->conditions.serialize_append (c->serializer);
	ਜੇ (unlikely (!ਬਾਹਰਲਾ_ਵਿੱਥ)) return_trace (ਝੂਠ);
	*ਬਾਹਰਲਾ_ਵਿੱਥ = 0;
      }
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (!subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->conditions, ਇਹ) (ਵਿੱਥ)))
	return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		 Instancer *instancer) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = conditions.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!_hb_recurse_condition_evaluate (ਇਹ+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (conditions.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8_ਦਾ_Offset24_ਮੰਜ਼ਿਲ<ਬਣਤਰ Condition>	conditions;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (3, conditions);
};

ਬਣਤਰ ConditionOr
{
  ਮਿੱਤਰ ਬਣਤਰ Condition;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ)))
      return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : conditions)
    {
      ਜੇ (ਵਿੱਥ.is_null ())
      {
	ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ_ਵਿੱਥ = ਬਾਹਰਲਾ->conditions.serialize_append (c->serializer);
	ਜੇ (unlikely (!ਬਾਹਰਲਾ_ਵਿੱਥ)) return_trace (ਝੂਠ);
	*ਬਾਹਰਲਾ_ਵਿੱਥ = 0;
      }
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (!subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->conditions, ਇਹ) (ਵਿੱਥ)))
	return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		 Instancer *instancer) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = conditions.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (_hb_recurse_condition_evaluate (ਇਹ+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (conditions.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8_ਦਾ_Offset24_ਮੰਜ਼ਿਲ<ਬਣਤਰ Condition>	conditions;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (3, conditions);
};

ਬਣਤਰ ConditionNegate
{
  ਮਿੱਤਰ ਬਣਤਰ Condition;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
    ਜੇ (condition.is_null ())
    {
      ਬਾਹਰਲਾ->condition = 0;
      return_trace (ਸੱਚ);
    }
    return_trace (ਬਾਹਰਲਾ->condition.serialize_subset (c, condition, ਇਹ));
  }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		 Instancer *instancer) ਅਚਲ
  {
    ਵਾਪਸ !_hb_recurse_condition_evaluate (ਇਹ+condition,
					    coords, coord_len,
					    instancer);
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (condition.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24_ਮੰਜ਼ਿਲ<ਬਣਤਰ Condition>	condition;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (5);
};

ਬਣਤਰ Condition
{
  ਮਿੱਤਰ ਬਣਤਰ Condition_ਸੈੱਟ;

  ਸਥਿਰ ਤਾਰਕਿਕ serialize_constant (hb_serialize_context_t *c, ਤਾਰਕਿਕ value)
  {
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed<Condition_ਮੁੱਲ> ();
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->extend_min (ਬਾਹਰਲਾ))) ਵਾਪਸ ਝੂਠ;
    ਬਾਹਰਲਾ->format = 2;
    ਬਾਹਰਲਾ->default_ਮੁੱਲ_2 = value ? 1 : 0;
    ਬਾਹਰਲਾ->varIdx = VarIdx::NO_VARIATION;
    ਵਾਪਸ ਸੱਚ;
  }

  lookup_condition_subset_ਨਤੀਜਾ_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ = HB_MAX_NESTING_LEVEL,
				      ਤਾਰਕਿਕ materialize_true = ਝੂਠ) ਅਚਲ;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		 Instancer *instancer) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    ਹਾਲਤ 0: ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.evaluate (coords, coord_len, instancer);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.evaluate (coords, coord_len, instancer);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.evaluate (coords, coord_len, instancer);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.evaluate (coords, coord_len, instancer);
    ਹਾਲਤ 5: hb_barrier (); ਵਾਪਸ u.format5.evaluate (coords, coord_len, instancer);
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*ਬਾਹਰਲਾ*/,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ = HB_MAX_NESTING_LEVEL) ਅਚਲ
  {
    ਜੇ (unlikely (!ਡੂੰਘਾਈ))
    {
      c->apply = ਝੂਠ;
      ਵਾਪਸ DROP_ਰਿਕਾਰਡ_WITH_VAR;
    }

    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: ਵਾਪਸ KEEP_ਰਿਕਾਰਡ_WITH_VAR;
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.keep_with_variations (c, condition_map);
    ਹਾਲਤ 2: ਹਾਲਤ 3: ਹਾਲਤ 4: ਹਾਲਤ 5: ਰੋਕੋ;
    ਮੂਲ: c->apply = ਝੂਠ; ਵਾਪਸ DROP_ਰਿਕਾਰਡ_WITH_VAR;
    }

    hb_barrier ();
    ਤਾਰਕਿਕ applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.ਲੰਮਾਈ, c->instancer);
    c->apply &= applies;
    ਜੇ (c->all_axes_pinned)
      ਵਾਪਸ applies ? DROP_COND_WITH_VAR : DROP_ਰਿਕਾਰਡ_WITH_VAR;

    ਜੇ (u.format.v == 2)
      ਵਾਪਸ u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_ਸੈੱਟ (u.format2.varIdx) ?
             (applies ? KEEP_ਰਿਕਾਰਡ_WITH_VAR : DROP_ਰਿਕਾਰਡ_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    ਸਵੈਚਾਲਿਤ classify_ਬੱਚਾ = [c, condition_map, ਡੂੰਘਾਈ] (ਅਚਲ Condition &ਬੱਚਾ)
    {
      ਸਵੈਚਾਲਿਤ ਬੱਚਾ_context = *c;
      ਬੱਚਾ_context.apply = ਸੱਚ;
      ਵਾਪਸ ਬੱਚਾ.keep_with_variations (&ਬੱਚਾ_context, condition_map, ਡੂੰਘਾਈ - 1);
    };

    ਜੇ (u.format.v == 5)
    {
      ਸਵੈਚਾਲਿਤ ਨਤੀਜਾ = classify_ਬੱਚਾ (&u.format5 + u.format5.condition);
      ਵਾਪਸ ਨਤੀਜਾ == DROP_ਰਿਕਾਰਡ_WITH_VAR ? KEEP_ਰਿਕਾਰਡ_WITH_VAR :
             ਨਤੀਜਾ == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_ਰਿਕਾਰਡ_WITH_VAR;
    }

    ਤਾਰਕਿਕ is_and = u.format.v == 3;
    ਤਾਰਕਿਕ variable = ਝੂਠ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ਅਚਲ ਖਾਲੀ *ਅਧਾਰ = is_and ? (ਅਚਲ ਖਾਲੀ *) &u.format3 : (ਅਚਲ ਖਾਲੀ *) &u.format4;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : conditions)
    {
      ਸਵੈਚਾਲਿਤ ਨਤੀਜਾ = classify_ਬੱਚਾ (ਅਧਾਰ + ਵਿੱਥ);
      ਜੇ (is_and && ਨਤੀਜਾ == DROP_ਰਿਕਾਰਡ_WITH_VAR)
        ਵਾਪਸ DROP_ਰਿਕਾਰਡ_WITH_VAR;
      ਜੇ (!is_and && (ਨਤੀਜਾ == DROP_COND_WITH_VAR || ਨਤੀਜਾ == KEEP_ਰਿਕਾਰਡ_WITH_VAR))
        ਵਾਪਸ KEEP_ਰਿਕਾਰਡ_WITH_VAR;
      variable |= ਨਤੀਜਾ == KEEP_COND_WITH_VAR;
    }
    ਵਾਪਸ variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_ਰਿਕਾਰਡ_WITH_VAR : DROP_ਰਿਕਾਰਡ_WITH_VAR;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ਹਾਲਤ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ਹਾਲਤ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਜੇ (unlikely (!c->check_ਸ਼ੁਰੂ_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_ਮੁੱਲ ());

    return_trace (c->ਸਿਰਾ_recursion (ਇਹ->dispatch (c)));
  }

  ਤਾਰਕਿਕ collect_var_indices (hb_ਸੈੱਟ_t *var_indices,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *condition_ਮੁੱਲਾਂ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ;

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ Condition *src,
		  ਅਚਲ hb_map_t &varidx_map,
		  ਅਚਲ hb_map_t *axis_map = ਖਾਲੀ_ਸੂਚਕ);

  ਨਿਜੀ:
  lookup_condition_subset_ਨਤੀਜਾ_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ) ਅਚਲ;

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_ਮੁੱਲ	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
ਤਾਰਕਿਕ
_hb_recurse_condition_evaluate (ਅਚਲ ਬਣਤਰ Condition &condition,
				ਅਚਲ ਪੂਰਨ *coords,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
				Instancer *instancer)
{
  ਵਾਪਸ condition.evaluate (coords, coord_len, instancer);
}

ਬਣਤਰ ConditionList
{
  ਅਚਲ Condition& ਸੰਚਾਲਕ[] (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  { ਵਾਪਸ ਇਹ+conditions[i]; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ conditions.len; }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ ConditionList *src,
		  ਅਚਲ hb_inc_bimap_t &condition_map,
		  ਅਚਲ hb_map_t &varidx_map,
		  ਅਚਲ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    conditions.len = condition_map.get_population ();
    ਜੇ (unlikely (!c->extend (conditions))) return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < conditions.len; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਸੂਚਕਾਂਕ = condition_map.backward (i);
      ਜੇ (unlikely (old_ਸੂਚਕਾਂਕ >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_ਸੂਚਕਾਂਕ]), varidx_map, axis_map)))
	return_trace (ਝੂਠ);
    }
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (conditions.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Array32_ਦਾ_Offset32_ਮੰਜ਼ਿਲ<Condition> conditions;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, conditions);
};

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
Condition::collect_var_indices (hb_ਸੈੱਟ_t *var_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ,
                               hb_hashmap_t<uintptr_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *condition_ਮੁੱਲਾਂ) ਅਚਲ
{
  ਜੇ (unlikely (!ਡੂੰਘਾਈ)) ਵਾਪਸ ਝੂਠ;
  ਚੋਣ (u.format.v)
  {
    ਹਾਲਤ 0:
    ਹਾਲਤ 1:
      ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 2:
      ਜੇ (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->ਜੋੜੋ (u.format2.varIdx);
        ਜੇ (condition_ਮੁੱਲਾਂ &&
            !condition_ਮੁੱਲਾਂ->ਸੈੱਟ ((uintptr_t) &u.format2,
                                    hb_pair (ਬਿਨਾਨਿਸ਼ਾਨੀ (u.format2.varIdx), ਪੂਰਨ (u.format2.default_ਮੁੱਲ_2))))
          ਵਾਪਸ ਝੂਠ;
      }
      ਵਾਪਸ !var_indices->ਵਿੱਚ_error ();
    ਹਾਲਤ 3:
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : u.format3.conditions)
	ਜੇ (unlikely (!(&u.format3+ਵਿੱਥ).collect_var_indices (var_indices,
							     ਡੂੰਘਾਈ - 1, condition_ਮੁੱਲਾਂ)))
	  ਵਾਪਸ ਝੂਠ;
      ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 4:
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : u.format4.conditions)
	ਜੇ (unlikely (!(&u.format4+ਵਿੱਥ).collect_var_indices (var_indices,
							     ਡੂੰਘਾਈ - 1, condition_ਮੁੱਲਾਂ)))
	  ਵਾਪਸ ਝੂਠ;
      ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ 5:
      ਵਾਪਸ (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  ਡੂੰਘਾਈ - 1, condition_ਮੁੱਲਾਂ);
    ਮੂਲ:
      ਵਾਪਸ ਝੂਠ;
  }
}

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
Condition::serialize (hb_serialize_context_t *c,
		      ਅਚਲ Condition *src,
		      ਅਚਲ hb_map_t &varidx_map,
		      ਅਚਲ hb_map_t *axis_map)
{
  TRACE_SERIALIZE (ਇਹ);
  ਚੋਣ (src->u.format.v)
  {
    ਹਾਲਤ 0:
    {
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed<Condition_ਮੁੱਲ> ();
      ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
      ਬਾਹਰਲਾ->format = 2;
      ਬਾਹਰਲਾ->default_ਮੁੱਲ_2 = 1;
      ਬਾਹਰਲਾ->varIdx = VarIdx::NO_VARIATION;
      return_trace (ਸੱਚ);
    }
    ਹਾਲਤ 1:
    {
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (&src->u.format1);
      ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);
      ਜੇ (axis_map)
      {
	ਜੇ (unlikely (!axis_map->has (ਬਾਹਰਲਾ->axis_ਸੂਚਕਾਂਕ_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (ਝੂਠ);
	}
	ਜੇ (unlikely (!c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->axis_ਸੂਚਕਾਂਕ_2, axis_map->get (ਬਾਹਰਲਾ->axis_ਸੂਚਕਾਂਕ_2),
				      HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
	  return_trace (ਝੂਠ);
      }
      return_trace (ਸੱਚ);
    }
    ਹਾਲਤ 2:
    {
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (&src->u.format2);
      ਜੇ (unlikely (!ਬਾਹਰਲਾ ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (ਝੂਠ);
      ਬਾਹਰਲਾ->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (ਸੱਚ);
    }
    ਹਾਲਤ 3:
    {
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed<ConditionAnd> ();
      ਜੇ (unlikely (!c->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
      ਬਾਹਰਲਾ->format = 3;
      ਬਾਹਰਲਾ->conditions.len = src->u.format3.conditions.len;
      ਜੇ (unlikely (!c->extend (ਬਾਹਰਲਾ->conditions))) return_trace (ਝੂਠ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਾਹਰਲਾ->conditions.len; i++)
	ਜੇ (unlikely (!ਬਾਹਰਲਾ->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (ਝੂਠ);
      return_trace (ਸੱਚ);
    }
    ਹਾਲਤ 4:
    {
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed<ConditionOr> ();
      ਜੇ (unlikely (!c->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
      ਬਾਹਰਲਾ->format = 4;
      ਬਾਹਰਲਾ->conditions.len = src->u.format4.conditions.len;
      ਜੇ (unlikely (!c->extend (ਬਾਹਰਲਾ->conditions))) return_trace (ਝੂਠ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਾਹਰਲਾ->conditions.len; i++)
	ਜੇ (unlikely (!ਬਾਹਰਲਾ->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (ਝੂਠ);
      return_trace (ਸੱਚ);
    }
    ਹਾਲਤ 5:
    {
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (&src->u.format5);
      ਜੇ (unlikely (!ਬਾਹਰਲਾ ||
		    !ਬਾਹਰਲਾ->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (ਝੂਠ);
      return_trace (ਸੱਚ);
    }
    ਮੂਲ:
      return_trace (ਝੂਠ);
  }
}

ਸਤਰਅੰਦਰ lookup_condition_subset_ਨਤੀਜਾ_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ,
				    ਤਾਰਕਿਕ materialize_true) ਅਚਲ
{
  ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
  lookup_condition_subset_ਨਤੀਜਾ_t ਨਤੀਜਾ =
      subset_lookup_condition_impl (c, ਡੂੰਘਾਈ);
  ਜੇ (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_KEEP)
    ਵਾਪਸ ਨਤੀਜਾ;

  c->serializer->revert (snap);
  ਜੇ (ਨਤੀਜਾ != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    ਵਾਪਸ ਨਤੀਜਾ;

  ਵਾਪਸ serialize_constant (c->serializer, ਸੱਚ) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

ਸਤਰਅੰਦਰ lookup_condition_subset_ਨਤੀਜਾ_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ) ਅਚਲ
{
  ਜੇ (unlikely (!ਡੂੰਘਾਈ))
    ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;

  ਸਵੈਚਾਲਿਤ subset_ਵਿੱਥ = [c, ਡੂੰਘਾਈ] (Array8_ਦਾ_Offset24_ਮੰਜ਼ਿਲ<Condition> &ਬਾਹਰਲਾ,
				   ਅਚਲ Condition &condition)
      -> lookup_condition_subset_ਨਤੀਜਾ_t
  {
    ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
    ਸਵੈਚਾਲਿਤ *ਵਿੱਥ = ਬਾਹਰਲਾ.serialize_append (c->serializer);
    ਜੇ (unlikely (!ਵਿੱਥ))
      ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_ਨਤੀਜਾ_t ਨਤੀਜਾ =
        condition.subset_lookup_condition (c, ਡੂੰਘਾਈ - 1);
    ਜੇ (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->ਜੋੜੋ_link (*ਵਿੱਥ, c->serializer->ਕੱਢੋ_pack ());
    ਨਹੀਂਤਾਂ
    {
      c->serializer->ਕੱਢੋ_discard ();
      ਬਾਹਰਲਾ.ਕੱਢੋ ();
      c->serializer->revert (snap);
    }
    ਵਾਪਸ ਨਤੀਜਾ;
  };

  ਚੋਣ (u.format.v)
  {
    ਹਾਲਤ 0:
      ਵਾਪਸ LOOKUP_CONDITION_SUBSET_TRUE;

    ਹਾਲਤ 1:
    {
      ਅਚਲ ConditionAxisRange &src = u.format1;
      ਜੇ (c->plan->user_axes_location.is_empty ())
        ਵਾਪਸ c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_ਟੈਗ_2;
      ਜੇ (!c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.has (src.axis_ਸੂਚਕਾਂਕ_2, &axis_ਟੈਗ_2))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_ਹੱਦਬੰਦੀ {-1.f, 0.f, 1.f};
      Triple *normalized_ਹੱਦਬੰਦੀ;
      ਜੇ (c->plan->axes_location.has (*axis_ਟੈਗ_2, &normalized_ਹੱਦਬੰਦੀ))
        axis_ਹੱਦਬੰਦੀ = *normalized_ਹੱਦਬੰਦੀ;

      ਦਸ਼ਮਲਵ filter_min = src.filter_Range_Min_ਮੁੱਲ.ਮੰਜ਼ਿਲ_float ();
      ਦਸ਼ਮਲਵ filter_max = src.filter_Range_Max_ਮੁੱਲ.ਮੰਜ਼ਿਲ_float ();
      ਜੇ (filter_min > filter_max)
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_FALSE;
      ਜੇ (axis_ਹੱਦਬੰਦੀ.is_ਬਿੰਦੂ ())
        ਵਾਪਸ filter_min <= axis_ਹੱਦਬੰਦੀ.ਵਿਚਕਾਰ &&
	       axis_ਹੱਦਬੰਦੀ.ਵਿਚਕਾਰ <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      ਜੇ (axis_ਹੱਦਬੰਦੀ.ਵੱਧ_ਤੋਂ_ਵੱਧ < filter_min ||
	  filter_max < axis_ਹੱਦਬੰਦੀ.minimum)
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_FALSE;
      ਜੇ (filter_min <= axis_ਹੱਦਬੰਦੀ.minimum &&
	  axis_ਹੱਦਬੰਦੀ.ਵੱਧ_ਤੋਂ_ਵੱਧ <= filter_max)
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_TRUE;
      ਜੇ (!c->plan->axes_ਸੂਚਕਾਂਕ_map.has (src.axis_ਸੂਚਕਾਂਕ_2))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;

      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (&src);
      ਜੇ (unlikely (!ਬਾਹਰਲਾ))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      ਜੇ (c->plan->axes_triple_distances.has (*axis_ਟੈਗ_2, &distances))
        axis_distances = *distances;

      ਬਾਹਰਲਾ->filter_Range_Min_ਮੁੱਲ.ਸੈੱਟ_float (
	  renormalize_ਮੁੱਲ (filter_min, axis_ਹੱਦਬੰਦੀ, axis_distances, ਝੂਠ));
      ਬਾਹਰਲਾ->filter_Range_Max_ਮੁੱਲ.ਸੈੱਟ_float (
	  renormalize_ਮੁੱਲ (filter_max, axis_ਹੱਦਬੰਦੀ, axis_distances, ਝੂਠ));
      ਜੇ (unlikely (!c->serializer->check_ਸੌਂਪੋ (
		ਬਾਹਰਲਾ->axis_ਸੂਚਕਾਂਕ_2,
		c->plan->axes_ਸੂਚਕਾਂਕ_map.get (src.axis_ਸੂਚਕਾਂਕ_2),
		HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;
      ਵਾਪਸ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    ਹਾਲਤ 2:
    {
      ਅਚਲ Condition_ਮੁੱਲ &src = u.format2;
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (&src);
      ਜੇ (unlikely (!ਬਾਹਰਲਾ))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;

      ਜੇ (src.varIdx == VarIdx::NO_VARIATION)
        ਵਾਪਸ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((ਪੂਰਨ) src.default_ਮੁੱਲ_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *condition_ਮੁੱਲ;
      ਜੇ (c->plan->layout_condition_idx_ਮੁੱਲ_map.has ((uintptr_t) &src, &condition_ਮੁੱਲ))
      {
        ਬਾਹਰਲਾ->default_ਮੁੱਲ_2 = condition_ਮੁੱਲ->second;
        ਬਾਹਰਲਾ->varIdx = condition_ਮੁੱਲ->ਪਹਿਲਾ;
        ਜੇ (ਬਾਹਰਲਾ->varIdx == VarIdx::NO_VARIATION)
          ਵਾਪਸ ਬਾਹਰਲਾ->default_ਮੁੱਲ_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *new_varidx_delta;
      ਜੇ (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        ਵਾਪਸ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((ਪੂਰਨ) src.default_ਮੁੱਲ_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      ਜੇ (!c->plan->user_axes_location.is_empty () &&
          hb_ਪਹਿਲਾ (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        ਦੁੱਗਣਾ value = (ਪੂਰਨ) src.default_ਮੁੱਲ_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        ਵਾਪਸ value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      ਪੂਰਨ default_ਮੁੱਲ = (ਪੂਰਨ) src.default_ਮੁੱਲ_2 +
			  hb_ਦੂਜਾ (*new_varidx_delta);
      ਜੇ (unlikely (!c->serializer->check_ਸੌਂਪੋ (
		ਬਾਹਰਲਾ->default_ਮੁੱਲ_2,
		default_ਮੁੱਲ,
		HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;
      ਬਾਹਰਲਾ->varIdx = hb_ਪਹਿਲਾ (*new_varidx_delta);

      ਵਾਪਸ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    ਹਾਲਤ 3:
    {
      ਅਚਲ ConditionAnd &src = u.format3;
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (&src);
      ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ)))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;
      ਬਾਹਰਲਾ->format = 3;

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : src.conditions)
      {
        lookup_condition_subset_ਨਤੀਜਾ_t ਨਤੀਜਾ =
            subset_ਵਿੱਥ (ਬਾਹਰਲਾ->conditions, &src + ਵਿੱਥ);
        ਜੇ (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_ERROR ||
	    ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_FALSE)
          ਵਾਪਸ ਨਤੀਜਾ;
      }
      ਵਾਪਸ ਬਾਹਰਲਾ->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    ਹਾਲਤ 4:
    {
      ਅਚਲ ConditionOr &src = u.format4;
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (&src);
      ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ)))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;
      ਬਾਹਰਲਾ->format = 4;

      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : src.conditions)
      {
        lookup_condition_subset_ਨਤੀਜਾ_t ਨਤੀਜਾ =
            subset_ਵਿੱਥ (ਬਾਹਰਲਾ->conditions, &src + ਵਿੱਥ);
        ਜੇ (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_ERROR ||
	    ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_TRUE)
          ਵਾਪਸ ਨਤੀਜਾ;
      }
      ਵਾਪਸ ਬਾਹਰਲਾ->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    ਹਾਲਤ 5:
    {
      ਅਚਲ ConditionNegate &src = u.format5;
      ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (&src);
      ਜੇ (unlikely (!ਬਾਹਰਲਾ))
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_ERROR;
      ਬਾਹਰਲਾ->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_ਨਤੀਜਾ_t ਨਤੀਜਾ =
          (&src + src.condition).subset_lookup_condition (c, ਡੂੰਘਾਈ - 1);
      ਜੇ (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->ਜੋੜੋ_link (ਬਾਹਰਲਾ->condition,
				 c->serializer->ਕੱਢੋ_pack ());
        ਵਾਪਸ ਨਤੀਜਾ;
      }

      c->serializer->ਕੱਢੋ_discard ();
      ਜੇ (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_TRUE)
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_FALSE;
      ਜੇ (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_FALSE)
        ਵਾਪਸ LOOKUP_CONDITION_SUBSET_TRUE;
      ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਮੂਲ:
      ਵਾਪਸ LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

ਬਣਤਰ Condition_ਸੈੱਟ
{
  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		 ਵਸਤੂ_Var_Store_Instancer *instancer) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = conditions.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (!(ਇਹ+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ਅਚਲ
  {
    hb_map_t *condition_map = hb_map_create ();
    ਜੇ (unlikely (!condition_map)) ਵਾਪਸ;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_ਸੈੱਟ_t *cond_ਸੈੱਟ = hb_ਸੈੱਟ_create ();
    ਜੇ (unlikely (!cond_ਸੈੱਟ)) ਵਾਪਸ;
    hb::shared_ptr<hb_ਸੈੱਟ_t> s {cond_ਸੈੱਟ};

    c->apply = ਸੱਚ;
    ਤਾਰਕਿਕ should_keep = ਝੂਠ;
    /* The range map cannot identify value or boolean expressions uniquely. */
    ਤਾਰਕਿਕ can_deduplicate = ਸੱਚ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_kept_cond = 0, cond_idx = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& ਵਿੱਥ : conditions)
    {
      ਅਚਲ Condition &condition = ਇਹ+ਵਿੱਥ;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      ਜੇ (ret == DROP_ਰਿਕਾਰਡ_WITH_VAR)
      {
        c->apply = ਝੂਠ;
        ਵਾਪਸ;
      }

      ਜੇ (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = ਸੱਚ;
        cond_ਸੈੱਟ->ਜੋੜੋ (cond_idx);
        ਅੰਕ_kept_cond++;
      }

      ਜੇ (ret == KEEP_ਰਿਕਾਰਡ_WITH_VAR)
        should_keep = ਸੱਚ;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = ਅੰਕ_kept_cond == 0;
    ਜੇ (!should_keep && c->all_axes_pinned) ਵਾਪਸ;

    //check if condition_set is unique with variations
    ਜੇ (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      ਵਾਪਸ;

    ਜੇ (can_deduplicate)
      c->conditionset_map->ਸੈੱਟ (p, 1);
    c->ਰਿਕਾਰਡ_cond_idx_map->ਸੈੱਟ (c->cur_ਰਿਕਾਰਡ_idx, s);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               ਤਾਰਕਿਕ ਪਾਓ_catch_all) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਜੇ (ਪਾਓ_catch_all) return_trace (ਸੱਚ);

    hb_ਸੈੱਟ_t *retained_cond_ਸੈੱਟ = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (l->feature_ਰਿਕਾਰਡ_cond_idx_map != ਖਾਲੀ_ਸੂਚਕ)
      retained_cond_ਸੈੱਟ = l->feature_ਰਿਕਾਰਡ_cond_idx_map->get (l->cur_feature_var_ਰਿਕਾਰਡ_idx);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = conditions.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (retained_cond_ਸੈੱਟ != ਖਾਲੀ_ਸੂਚਕ && !retained_cond_ਸੈੱਟ->has (i))
        ਜਾਰੀ;
      ਅਚਲ Condition &condition = ਇਹ+conditions[i];
      ਜੇ (condition.u.format.v == 1)
      {
        subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->conditions, ਇਹ) (conditions[i]);
        ਜਾਰੀ;
      }

      ਸਵੈਚਾਲਿਤ *ਵਿੱਥ = ਬਾਹਰਲਾ->conditions.serialize_append (c->serializer);
      ਜੇ (unlikely (!ਵਿੱਥ)) return_trace (ਝੂਠ);
      c->serializer->push ();
      ਸਵੈਚਾਲਿਤ ਨਤੀਜਾ = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, ਸੱਚ);
      ਜੇ (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        ਨਤੀਜਾ = Condition::serialize_constant (c->serializer, ਝੂਠ) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      ਜੇ (unlikely (ਨਤੀਜਾ == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->ਕੱਢੋ_discard ();
        c->serializer->check_success (ਝੂਠ);
        return_trace (ਝੂਠ);
      }
      c->serializer->ਜੋੜੋ_link (*ਵਿੱਥ, c->serializer->ਕੱਢੋ_pack ());
    }

    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->conditions));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (conditions.sanitize (c, ਇਹ));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿੱਥ : conditions)
      ਜੇ (unlikely (!(ਇਹ+ਵਿੱਥ).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_ਮੁੱਲਾਂ)))
	c->layout_variation_indices->err ();
  }

  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ_Offset32_ਮੰਜ਼ਿਲ<Condition>	conditions;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, conditions);
};

ਬਣਤਰ Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ
{
  ਮਿੱਤਰ ਬਣਤਰ Feature_ਸਾਰਣੀ_Substitution;

  ਖਾਲੀ collect_lookups (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, hb_ਸੈੱਟ_t *lookup_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਵਾਪਸ (ਅਧਾਰ+feature).ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (lookup_indexes);
  }

  ਖਾਲੀ ਘੇਰਾ_features (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			 ਅਚਲ hb_map_t *lookup_indexes,
			 hb_ਸੈੱਟ_t       *feature_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ ((ਅਧਾਰ+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->ਜੋੜੋ (feature_ਸੂਚਕਾਂਕ_2);
  }

  ਖਾਲੀ collect_feature_substitutes_with_variations (hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਅਚਲ Feature*> *feature_substitutes_map,
                                                    hb_ਸੈੱਟ_t& catch_all_ਰਿਕਾਰਡ_feature_idxes,
                                                    ਅਚਲ hb_ਸੈੱਟ_t *feature_indices,
                                                    ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਜੇ (feature_indices->has (feature_ਸੂਚਕਾਂਕ_2))
    {
      feature_substitutes_map->ਸੈੱਟ (feature_ਸੂਚਕਾਂਕ_2, &(ਅਧਾਰ+feature));
      catch_all_ਰਿਕਾਰਡ_feature_idxes.ਜੋੜੋ (feature_ਸੂਚਕਾਂਕ_2);
    }
  }

  ਤਾਰਕਿਕ serialize (hb_subset_layout_context_t *c,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ,
                  ਅਚਲ Feature *f, ਅਚਲ ਟੈਗ *ਟੈਗ_2)
  {
    TRACE_SERIALIZE (ਇਹ);
    hb_serialize_context_t *s = c->subset_context->serializer;
    ਜੇ (unlikely (!s->extend_min (ਇਹ))) return_trace (ਝੂਠ);

    uint32_t *new_feature_idx;
    ਜੇ (!c->feature_map_w_duplicates->has (feature_ਸੂਚਕਾਂਕ, &new_feature_idx))
      return_trace (ਝੂਠ);

    ਜੇ (!s->check_ਸੌਂਪੋ (feature_ਸੂਚਕਾਂਕ_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    s->push ();
    ਤਾਰਕਿਕ ret = f->subset (c->subset_context, c, ਟੈਗ_2);
    ਜੇ (ret) s->ਜੋੜੋ_link (feature, s->ਕੱਢੋ_pack ());
    ਨਹੀਂਤਾਂ s->ਕੱਢੋ_discard ();

    return_trace (ret);
  }

  ਤਾਰਕਿਕ subset (hb_subset_layout_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    uint32_t *new_feature_ਸੂਚਕਾਂਕ;
    ਜੇ (!c->feature_map_w_duplicates->has (feature_ਸੂਚਕਾਂਕ_2, &new_feature_ਸੂਚਕਾਂਕ))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->subset_context->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->feature_ਸੂਚਕਾਂਕ_2 = *new_feature_ਸੂਚਕਾਂਕ;
    hb_pair_t<ਅਚਲ ਖਾਲੀ*, ਅਚਲ ਖਾਲੀ*> *original;
    ਜੇ (!c->feature_idx_ਟੈਗ_map->has (feature_ਸੂਚਕਾਂਕ_2, &original))
      return_trace (ਝੂਠ);
    ਅਚਲ ਟੈਗ *ਟੈਗ_2 = reinterpret_cast<ਅਚਲ ਟੈਗ*> (original->second);
    return_trace (ਬਾਹਰਲਾ->feature.serialize_subset (c->subset_context, feature, ਅਧਾਰ, c, ਟੈਗ_2));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		 ਅਚਲ FeatureList *feature_list) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ))) return_trace (ਝੂਠ);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ਅਚਲ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t ਘੇਰਾ = {feature_list->get_ਟੈਗ (feature_ਸੂਚਕਾਂਕ_2), ਅਧਾਰ};
    return_trace (feature.sanitize (c, ਅਧਾਰ, &ਘੇਰਾ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16		feature_ਸੂਚਕਾਂਕ_2;
  Offset32_ਮੰਜ਼ਿਲ<Feature>	feature;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ Feature_ਸਾਰਣੀ_Substitution
{
  ਖਾਲੀ collect_feature_indices (hb_ਸੈੱਟ_t *feature_indices) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : substitutions)
      feature_indices->ਜੋੜੋ (ਰਿਕਾਰਡ.feature_ਸੂਚਕਾਂਕ_2);
  }

  ਅਚਲ Feature *ਲੱਭੋ_substitute (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = substitutions.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਅਚਲ Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ = substitutions.arrayZ[i];
      ਜੇ (ਰਿਕਾਰਡ.feature_ਸੂਚਕਾਂਕ_2 == feature_ਸੂਚਕਾਂਕ)
	ਵਾਪਸ &(ਇਹ+ਰਿਕਾਰਡ.feature);
    }
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਖਾਲੀ collect_lookups (ਅਚਲ hb_ਸੈੱਟ_t *feature_indexes,
			hb_ਸੈੱਟ_t       *lookup_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ::feature_ਸੂਚਕਾਂਕ_2)
    | hb_apply ([ਇਹ, lookup_indexes] (ਅਚਲ Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ& r)
		{ r.collect_lookups (ਇਹ, lookup_indexes); })
    ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਟੈਗ_Getter>
  ਖਾਲੀ collect_name_ids (ਅਚਲ hb_map_t *feature_ਸੂਚਕਾਂਕ_map,
                         ਟੈਗ_Getter get_ਟੈਗ,
                         hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : substitutions)
      ਜੇ (feature_ਸੂਚਕਾਂਕ_map->has (ਰਿਕਾਰਡ.feature_ਸੂਚਕਾਂਕ_2))
        (ਇਹ+ਰਿਕਾਰਡ.feature).collect_name_ids (get_ਟੈਗ (ਰਿਕਾਰਡ.feature_ਸੂਚਕਾਂਕ_2), nameids_ਮੰਜ਼ਿਲ_retain);
  }

  ਖਾਲੀ ਘੇਰਾ_features (ਅਚਲ hb_map_t *lookup_indexes,
			 hb_ਸੈੱਟ_t       *feature_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ& ਰਿਕਾਰਡ : substitutions)
      ਰਿਕਾਰਡ.ਘੇਰਾ_features (ਇਹ, lookup_indexes, feature_indexes);
  }

  ਤਾਰਕਿਕ intersects_features (ਅਚਲ hb_map_t *feature_ਸੂਚਕਾਂਕ_map) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ& ਰਿਕਾਰਡ : substitutions)
    {
      ਜੇ (feature_ਸੂਚਕਾਂਕ_map->has (ਰਿਕਾਰਡ.feature_ਸੂਚਕਾਂਕ_2)) ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ& ਰਿਕਾਰਡ : substitutions)
      ਰਿਕਾਰਡ.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_ਰਿਕਾਰਡ_feature_idxes,
                                                          c->feature_indices, ਇਹ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               ਤਾਰਕਿਕ ਪਾਓ_catch_all) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = ਸੰਸਕਰਣ.major;
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = ਸੰਸਕਰਣ.minor;

    ਜੇ (ਪਾਓ_catch_all)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ : *(l->catch_all_ਰਿਕਾਰਡ_feature_idxes))
      {
        hb_pair_t<ਅਚਲ ਖਾਲੀ*, ਅਚਲ ਖਾਲੀ*> *p;
        ਜੇ (!l->feature_idx_ਟੈਗ_map->has (feature_ਸੂਚਕਾਂਕ, &p))
          return_trace (ਝੂਠ);
        ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->substitutions.serialize_append (c->serializer);
        ਜੇ (!o->serialize (l, feature_ਸੂਚਕਾਂਕ,
                           reinterpret_cast<ਅਚਲ Feature*> (p->ਪਹਿਲਾ),
                           reinterpret_cast<ਅਚਲ ਟੈਗ*> (p->second)))
          return_trace (ਝੂਠ);
      }
      return_trace (ਸੱਚ);
    }

    + substitutions.iter ()
    | hb_apply (subset_ਰਿਕਾਰਡ_array (l, &(ਬਾਹਰਲਾ->substitutions), ਇਹ))
    ;

    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->substitutions));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ FeatureList *feature_list) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੰਸਕਰਣ.sanitize (c) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1) &&
		  substitutions.sanitize (c, ਇਹ, feature_list));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>	ਸੰਸਕਰਣ;	/* Version--0x00010000u */
  Array16_ਦਾ<Feature_ਸਾਰਣੀ_Substitution_ਰਿਕਾਰਡ>
			substitutions;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, substitutions);
};

ਬਣਤਰ Feature_Variation_ਰਿਕਾਰਡ
{
  ਮਿੱਤਰ ਬਣਤਰ FeatureVariations;

  ਖਾਲੀ collect_lookups (ਅਚਲ ਖਾਲੀ     *ਅਧਾਰ,
			ਅਚਲ hb_ਸੈੱਟ_t *feature_indexes,
			hb_ਸੈੱਟ_t       *lookup_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਵਾਪਸ (ਅਧਾਰ+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਟੈਗ_Getter>
  ਖਾਲੀ collect_name_ids (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                         ਅਚਲ hb_map_t *feature_ਸੂਚਕਾਂਕ_map,
                         ਟੈਗ_Getter get_ਟੈਗ,
                         hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain) ਅਚਲ
  {
    (ਅਧਾਰ+substitutions).collect_name_ids (feature_ਸੂਚਕਾਂਕ_map, get_ਟੈਗ, nameids_ਮੰਜ਼ਿਲ_retain);
  }

  ਖਾਲੀ ਘੇਰਾ_features (ਅਚਲ ਖਾਲੀ     *ਅਧਾਰ,
			 ਅਚਲ hb_map_t *lookup_indexes,
			 hb_ਸੈੱਟ_t       *feature_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    (ਅਧਾਰ+substitutions).ਘੇਰਾ_features (lookup_indexes, feature_indexes);
  }

  ਤਾਰਕਿਕ intersects_features (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, ਅਚਲ hb_map_t *feature_ਸੂਚਕਾਂਕ_map) ਅਚਲ
  {
    ਵਾਪਸ (ਅਧਾਰ+substitutions).intersects_features (feature_ਸੂਚਕਾਂਕ_map);
  }

  ਖਾਲੀ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    (ਅਧਾਰ+conditions).keep_with_variations (c);
    ਜੇ (c->apply && !c->variation_applied)
    {
      (ਅਧਾਰ+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = ਸੱਚ; // set variations only once
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_layout_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
               ਤਾਰਕਿਕ ਪਾਓ_catch_all = ਝੂਠ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->subset_context->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    hb_ਸੈੱਟ_t *retained_conditions = c->feature_ਰਿਕਾਰਡ_cond_idx_map ?
        c->feature_ਰਿਕਾਰਡ_cond_idx_map->get (c->cur_feature_var_ਰਿਕਾਰਡ_idx).get () : ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (!ਪਾਓ_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      ਬਾਹਰਲਾ->conditions = 0;
    ਨਹੀਂਤਾਂ
      ਬਾਹਰਲਾ->conditions.serialize_subset (c->subset_context, conditions, ਅਧਾਰ, c, ਪਾਓ_catch_all);
    ਬਾਹਰਲਾ->substitutions.serialize_subset (c->subset_context, substitutions, ਅਧਾਰ, c, ਪਾਓ_catch_all);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		 ਅਚਲ FeatureList *feature_list) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (conditions.sanitize (c, ਅਧਾਰ) &&
		  substitutions.sanitize (c, ਅਧਾਰ, feature_list));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  { (ਅਧਾਰ+conditions).collect_variation_indices (c); }

  ਸੁਰੱਖਿਅਤ:
  Offset32_ਮੰਜ਼ਿਲ<Condition_ਸੈੱਟ>
			conditions;
  Offset32_ਮੰਜ਼ਿਲ<Feature_ਸਾਰਣੀ_Substitution>
			substitutions;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ Lookup_ਸੂਚਕਾਂਕ_List
{
  ਖਾਲੀ ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  { lookupIndices.ਜੋੜੋ_indexes_ਮੰਜ਼ਿਲ (lookup_indexes); }

  ਤਾਰਕਿਕ intersects_lookup_indexes (ਅਚਲ hb_map_t *lookup_indexes) ਅਚਲ
  { ਵਾਪਸ lookupIndices.intersects (lookup_indexes); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ)))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_ਸੂਚਕਾਂਕ_map)
    | hb_map (l->lookup_ਸੂਚਕਾਂਕ_map)
    ;
    ਬਾਹਰਲਾ->lookupIndices.serialize (c->serializer, l, it);
    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->lookupIndices));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (lookupIndices.sanitize (c));
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੂਚਕਾਂਕ_Array	lookupIndices;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (2, lookupIndices);
};

ਬਣਤਰ Lookup_Condition_ਰਿਕਾਰਡ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_condition_ਵਿੱਥ (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, ਅਚਲ ਖਾਲੀ *variations) ਅਚਲ
  { ਵਾਪਸ (ਅਚਲ ਅੱਖਰ *) &(ਅਧਾਰ+condition) - (ਅਚਲ ਅੱਖਰ *) variations; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		 ਅਚਲ ਪੂਰਨ *coords,
		 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		 Instancer *instancer) ਅਚਲ
  { ਵਾਪਸ (ਅਧਾਰ+condition).evaluate (coords, coord_len, instancer); }

  ਤਾਰਕਿਕ evaluate (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                 ਅਚਲ ਖਾਲੀ *variations,
                 hb_sorted_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> conditions) ਅਚਲ
  {
    ਵਾਪਸ !condition || hb_bsearch (get_condition_ਵਿੱਥ (ਅਧਾਰ, variations),
                                     conditions.arrayZ, conditions.ਲੰਮਾਈ,
                                     ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ),
                                     _hb_cmp_operator<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>);
  }

  ਤਾਰਕਿਕ collect_condition (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                          ਅਚਲ ਖਾਲੀ *variations,
                          ਅਚਲ ਪੂਰਨ *coords,
                          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
                          ਵਸਤੂ_Var_Store_Instancer *instancer,
                          hb_ਸੈੱਟ_t *ਦੇਖਿਆ,
                          hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *conditions) ਅਚਲ
  {
    /* Null conditions always apply and need no key state. */
    ਜੇ (!condition) ਵਾਪਸ ਸੱਚ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = get_condition_ਵਿੱਥ (ਅਧਾਰ, variations);
    ਜੇ (ਦੇਖਿਆ->has (ਵਿੱਥ)) ਵਾਪਸ ਸੱਚ;
    ਦੇਖਿਆ->ਜੋੜੋ (ਵਿੱਥ);
    ਜੇ (unlikely (ਦੇਖਿਆ->ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

    ਜੇ (evaluate (ਅਧਾਰ, coords, coord_len, instancer))
      conditions->push (ਵਿੱਥ);
    ਵਾਪਸ !conditions->ਵਿੱਚ_error ();
  }

  ਖਾਲੀ ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			      hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  { (ਅਧਾਰ+lookupIndices).ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (lookup_indexes); }

  ਤਾਰਕਿਕ intersects_lookup_indexes (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
				   ਅਚਲ hb_map_t *lookup_indexes) ਅਚਲ
  { ਵਾਪਸ (ਅਧਾਰ+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਅਧਾਰ+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_ਮੁੱਲਾਂ)))
      c->layout_variation_indices->err ();
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_ਨਤੀਜਾ_t ਨਤੀਜਾ =
        (ਅਧਾਰ + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    ਸੱਚ);
    ਜੇ (ਨਤੀਜਾ != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }
    c->serializer->ਜੋੜੋ_link (ਬਾਹਰਲਾ->condition, c->serializer->ਕੱਢੋ_pack ());

    return_trace (ਬਾਹਰਲਾ->lookupIndices.serialize_subset (c, lookupIndices, ਅਧਾਰ, l));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (condition.sanitize (c, ਅਧਾਰ) &&
		  lookupIndices.sanitize (c, ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset32_ਮੰਜ਼ਿਲ<Condition>		condition;
  Offset32_ਮੰਜ਼ਿਲ<Lookup_ਸੂਚਕਾਂਕ_List>	lookupIndices;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ Feature_Lookups_ਸਾਰਣੀ
{
  ਤਾਰਕਿਕ adds_default_lookups () ਅਚਲ { ਵਾਪਸ flags & 0x0001u; }

  ਤਾਰਕਿਕ collect_conditions (ਅਚਲ ਖਾਲੀ *variations,
                           ਅਚਲ ਪੂਰਨ *coords,
                           ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
                           ਵਸਤੂ_Var_Store_Instancer *instancer,
                           hb_ਸੈੱਟ_t *ਦੇਖਿਆ,
                           hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *conditions) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Condition_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : ਰਿਕਾਰਡਾਂ)
      ਜੇ (unlikely (!ਰਿਕਾਰਡ.collect_condition (ਇਹ, variations, coords, coord_len,
                                               instancer, ਦੇਖਿਆ, conditions)))
	ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ collect_lookups (ਅਚਲ ਖਾਲੀ *variations,
                       hb_sorted_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> conditions,
                       hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Condition_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : ਰਿਕਾਰਡਾਂ)
      ਜੇ (ਰਿਕਾਰਡ.evaluate (ਇਹ, variations, conditions))
	ਰਿਕਾਰਡ.ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (ਇਹ, lookup_indexes);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Instancer>
  ਖਾਲੀ collect_lookups (ਅਚਲ ਪੂਰਨ *coords,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
			Instancer *instancer,
			hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Condition_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : ਰਿਕਾਰਡਾਂ)
      ਜੇ (ਰਿਕਾਰਡ.evaluate (ਇਹ, coords, coord_len, instancer))
	ਰਿਕਾਰਡ.ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (ਇਹ, lookup_indexes);
  }

  ਖਾਲੀ collect_lookups (hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Condition_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : ਰਿਕਾਰਡਾਂ)
      ਰਿਕਾਰਡ.ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (ਇਹ, lookup_indexes);
  }

  ਤਾਰਕਿਕ intersects_lookup_indexes (ਅਚਲ hb_map_t *lookup_indexes) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Condition_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : ਰਿਕਾਰਡਾਂ)
      ਜੇ (ਰਿਕਾਰਡ.intersects_lookup_indexes (ਇਹ, lookup_indexes))
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Condition_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : ਰਿਕਾਰਡਾਂ)
      ਰਿਕਾਰਡ.collect_variation_indices (c, ਇਹ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ)))
      return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = ਸੰਸਕਰਣ.major;
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = ਸੰਸਕਰਣ.minor;
    ਬਾਹਰਲਾ->flags = flags;

    ਲਈ (ਅਚਲ Lookup_Condition_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : ਰਿਕਾਰਡਾਂ)
    {
      ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
      ਜੇ (ਰਿਕਾਰਡ.subset (c, l, ਇਹ))
	ਬਾਹਰਲਾ->ਰਿਕਾਰਡਾਂ.len++;
      ਨਹੀਂਤਾਂ
	c->serializer->revert (snap);
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੰਸਕਰਣ.sanitize (c) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1) &&
		  ਰਿਕਾਰਡਾਂ.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>	ਸੰਸਕਰਣ;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32_ਦਾ<Lookup_Condition_ਰਿਕਾਰਡ>
		ਰਿਕਾਰਡਾਂ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (10, ਰਿਕਾਰਡਾਂ);
};

ਬਣਤਰ Lookup_Variation_ਰਿਕਾਰਡ
{
  ਮਿੱਤਰ ਬਣਤਰ FeatureVariations;

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_feature_ਸੂਚਕਾਂਕ () ਅਚਲ { ਵਾਪਸ feature_ਸੂਚਕਾਂਕ_2; }

  ਖਾਲੀ collect_lookups (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                       ਅਚਲ Feature &feature,
                       hb_sorted_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> conditions,
                       hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  {
    ਅਚਲ Feature_Lookups_ਸਾਰਣੀ &feature_lookups = ਅਧਾਰ+featureLookups;
    ਜੇ (feature_lookups.adds_default_lookups ())
      feature.ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (lookup_indexes);
    feature_lookups.collect_lookups (ਅਧਾਰ, conditions, lookup_indexes);
  }

  ਖਾਲੀ collect_lookups (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			ਅਚਲ hb_ਸੈੱਟ_t *feature_indexes,
			hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  {
    ਜੇ (feature_indexes->has (feature_ਸੂਚਕਾਂਕ_2))
      (ਅਧਾਰ+featureLookups).collect_lookups (lookup_indexes);
  }

  ਖਾਲੀ ਘੇਰਾ_features (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			 ਅਚਲ hb_map_t *lookup_indexes,
			 hb_ਸੈੱਟ_t *feature_indexes) ਅਚਲ
  {
    ਜੇ ((ਅਧਾਰ+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->ਜੋੜੋ (feature_ਸੂਚਕਾਂਕ_2);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  { (ਅਧਾਰ+featureLookups).collect_variation_indices (c); }

  ਤਾਰਕਿਕ subset (hb_subset_layout_context_t *l, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (ਇਹ);
    uint32_t *new_feature_ਸੂਚਕਾਂਕ;
    ਜੇ (!l->feature_map_w_duplicates->has (feature_ਸੂਚਕਾਂਕ_2, &new_feature_ਸੂਚਕਾਂਕ))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ ||
		  !c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->feature_ਸੂਚਕਾਂਕ_2,
						*new_feature_ਸੂਚਕਾਂਕ,
						HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
      return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->featureLookups.serialize_subset (c, featureLookups, ਅਧਾਰ, l));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  featureLookups.sanitize (c, ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16			feature_ਸੂਚਕਾਂਕ_2;
  Offset32_ਮੰਜ਼ਿਲ<Feature_Lookups_ਸਾਰਣੀ>	featureLookups;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ FeatureVariations
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ = 0xFFFFFFFFu;

  ਅਚਲ Array32_ਦਾ<Lookup_Variation_ਰਿਕਾਰਡ> &get_lookup_variation_ਰਿਕਾਰਡਾਂ () ਅਚਲ
  {
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010001u)
      ਵਾਪਸ Null (Array32_ਦਾ<Lookup_Variation_ਰਿਕਾਰਡ>);
    ਵਾਪਸ StructAfter<Array32_ਦਾ<Lookup_Variation_ਰਿਕਾਰਡ>> (var_ਰਿਕਾਰਡਾਂ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਗਿਣਤੀ () ਅਚਲ
  {
    ਵਾਪਸ var_ਰਿਕਾਰਡਾਂ.len;
  }

  ਤਾਰਕਿਕ ਲੱਭੋ_ਸੂਚਕਾਂਕ (ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2,
		   ਵਸਤੂ_Var_Store_Instancer *instancer) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = var_ਰਿਕਾਰਡਾਂ.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਅਚਲ Feature_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ = var_ਰਿਕਾਰਡਾਂ.arrayZ[i];
      ਜੇ ((ਇਹ+ਰਿਕਾਰਡ.conditions).evaluate (coords, coord_len, instancer))
      {
	*ਸੂਚਕਾਂਕ_2 = i;
	ਵਾਪਸ ਸੱਚ;
      }
    }
    *ਸੂਚਕਾਂਕ_2 = NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਅਚਲ Feature *ਲੱਭੋ_substitute (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ variations_ਸੂਚਕਾਂਕ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਅਚਲ Feature_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ = var_ਰਿਕਾਰਡਾਂ[variations_ਸੂਚਕਾਂਕ];
    ਵਾਪਸ (ਇਹ+ਰਿਕਾਰਡ.substitutions).ਲੱਭੋ_substitute (feature_ਸੂਚਕਾਂਕ);
  }

  ਤਾਰਕਿਕ collect_lookup_conditions (ਅਚਲ ਪੂਰਨ *coords,
                                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_len,
                                  ਵਸਤੂ_Var_Store_Instancer *instancer,
                                  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *state) ਅਚਲ
  {
    hb_ਸੈੱਟ_t ਦੇਖਿਆ_tables, ਦੇਖਿਆ_conditions;
    ਲਈ (ਅਚਲ Lookup_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : get_lookup_variation_ਰਿਕਾਰਡਾਂ ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = ਰਿਕਾਰਡ.featureLookups;
      ਜੇ (ਦੇਖਿਆ_tables.has (ਵਿੱਥ)) ਜਾਰੀ;
      ਦੇਖਿਆ_tables.ਜੋੜੋ (ਵਿੱਥ);
      ਜੇ (unlikely (ਦੇਖਿਆ_tables.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

      ਅਚਲ Feature_Lookups_ਸਾਰਣੀ &feature_lookups = ਇਹ+ਰਿਕਾਰਡ.featureLookups;
      ਜੇ (unlikely (!feature_lookups.collect_conditions (ਇਹ, coords, coord_len,
                                                         instancer, &ਦੇਖਿਆ_conditions,
                                                         state)))
	ਵਾਪਸ ਝੂਠ;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ>);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ get_lookup_variations (ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ,
                              ਅਚਲ Feature &feature,
                              hb_sorted_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ> conditions,
                              hb_ਸੈੱਟ_t *lookup_indexes) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : get_lookup_variation_ਰਿਕਾਰਡਾਂ ())
    {
      ਜੇ (ਰਿਕਾਰਡ.feature_ਸੂਚਕਾਂਕ_2 != feature_ਸੂਚਕਾਂਕ) ਜਾਰੀ;

      ਰਿਕਾਰਡ.collect_lookups (ਇਹ, feature, conditions, lookup_indexes);
      ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = var_ਰਿਕਾਰਡਾਂ.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      c->cur_ਰਿਕਾਰਡ_idx = i;
      var_ਰਿਕਾਰਡਾਂ[i].collect_feature_substitutes_with_variations (c, ਇਹ);
      ਜੇ (c->universal)
        ਰੋਕੋ;
    }
    ਜੇ (c->universal || c->ਰਿਕਾਰਡ_cond_idx_map->is_empty ())
      c->catch_all_ਰਿਕਾਰਡ_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed (*ਇਹ));
  }

  ਖਾਲੀ collect_lookups (ਅਚਲ hb_ਸੈੱਟ_t *feature_indexes,
			ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map,
			hb_ਸੈੱਟ_t       *lookup_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = var_ਰਿਕਾਰਡਾਂ.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (feature_ਰਿਕਾਰਡ_cond_idx_map &&
          !feature_ਰਿਕਾਰਡ_cond_idx_map->has (i))
        ਜਾਰੀ;
      var_ਰਿਕਾਰਡਾਂ[i].collect_lookups (ਇਹ, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਟੈਗ_Getter>
  ਖਾਲੀ collect_name_ids (ਅਚਲ hb_map_t *feature_ਸੂਚਕਾਂਕ_map,
                         ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map,
                         ਟੈਗ_Getter get_ਟੈਗ,
                         hb_ਸੈੱਟ_t *nameids_ਮੰਜ਼ਿਲ_retain) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < var_ਰਿਕਾਰਡਾਂ.len; i++)
    {
      ਜੇ (feature_ਰਿਕਾਰਡ_cond_idx_map && !feature_ਰਿਕਾਰਡ_cond_idx_map->has (i))
        ਜਾਰੀ;
      var_ਰਿਕਾਰਡਾਂ[i].collect_name_ids (ਇਹ, feature_ਸੂਚਕਾਂਕ_map, get_ਟੈਗ, nameids_ਮੰਜ਼ਿਲ_retain);
    }
  }

  ਖਾਲੀ collect_lookup_variation_lookups (ਅਚਲ hb_ਸੈੱਟ_t *feature_indexes,
					 hb_ਸੈੱਟ_t *lookup_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : get_lookup_variation_ਰਿਕਾਰਡਾਂ ())
      ਰਿਕਾਰਡ.collect_lookups (ਇਹ, feature_indexes, lookup_indexes);
  }

  ਖਾਲੀ ਘੇਰਾ_features (ਅਚਲ hb_map_t *lookup_indexes,
			 ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb::shared_ptr<hb_ਸੈੱਟ_t>> *feature_ਰਿਕਾਰਡ_cond_idx_map,
			 hb_ਸੈੱਟ_t       *feature_indexes /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = var_ਰਿਕਾਰਡਾਂ.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (feature_ਰਿਕਾਰਡ_cond_idx_map != ਖਾਲੀ_ਸੂਚਕ &&
          !feature_ਰਿਕਾਰਡ_cond_idx_map->has (i))
        ਜਾਰੀ;
      var_ਰਿਕਾਰਡਾਂ[i].ਘੇਰਾ_features (ਇਹ, lookup_indexes, feature_indexes);
    }

    ਲਈ (ਅਚਲ Lookup_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : get_lookup_variation_ਰਿਕਾਰਡਾਂ ())
      ਰਿਕਾਰਡ.ਘੇਰਾ_features (ਇਹ, lookup_indexes, feature_indexes);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Feature_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : var_ਰਿਕਾਰਡਾਂ)
      ਰਿਕਾਰਡ.collect_variation_indices (c, ਇਹ);
    ਲਈ (ਅਚਲ Lookup_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : get_lookup_variation_ਰਿਕਾਰਡਾਂ ())
      ਰਿਕਾਰਡ.collect_variation_indices (c, ਇਹ);
  }

  ਖਾਲੀ collect_feature_indices (hb_ਸੈੱਟ_t *feature_indices) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Feature_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : var_ਰਿਕਾਰਡਾਂ)
      (ਇਹ+ਰਿਕਾਰਡ.substitutions).collect_feature_indices (feature_indices);
  }

  ਤਾਰਕਿਕ has_lookup_variations (ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਲਈ (ਅਚਲ Lookup_Variation_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ : get_lookup_variation_ਰਿਕਾਰਡਾਂ ())
    {
      ਜੇ (ਰਿਕਾਰਡ.feature_ਸੂਚਕਾਂਕ_2 == feature_ਸੂਚਕਾਂਕ)
	ਵਾਪਸ ਸੱਚ;
      ਜੇ (ਰਿਕਾਰਡ.feature_ਸੂਚਕਾਂਕ_2 > feature_ਸੂਚਕਾਂਕ)
	ਰੋਕੋ;
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = ਸੰਸਕਰਣ.major;
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = ਸੰਸਕਰਣ.minor;

    ਪੂਰਨ keep_ਉੱਪਰ_ਮੰਜ਼ਿਲ = -1;
    ਲਈ (ਪੂਰਨ i = var_ਰਿਕਾਰਡਾਂ.len - 1; i >= 0; i--) {
      ਜੇ (var_ਰਿਕਾਰਡਾਂ[i].intersects_features (ਇਹ, l->feature_map_w_duplicates)) {
        keep_ਉੱਪਰ_ਮੰਜ਼ਿਲ = i;
        ਰੋਕੋ;
      }
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) (keep_ਉੱਪਰ_ਮੰਜ਼ਿਲ + 1);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (l->feature_ਰਿਕਾਰਡ_cond_idx_map != ਖਾਲੀ_ਸੂਚਕ &&
          !l->feature_ਰਿਕਾਰਡ_cond_idx_map->has (i))
        ਜਾਰੀ;

      l->cur_feature_var_ਰਿਕਾਰਡ_idx = i;
      subset_ਰਿਕਾਰਡ_array (l, &(ਬਾਹਰਲਾ->var_ਰਿਕਾਰਡਾਂ), ਇਹ) (var_ਰਿਕਾਰਡਾਂ[i]);
    }

    ਜੇ (ਬਾਹਰਲਾ->var_ਰਿਕਾਰਡਾਂ.len && !l->catch_all_ਰਿਕਾਰਡ_feature_idxes->is_empty ())
    {
      ਤਾਰਕਿਕ ਪਾਓ_catch_all_ਰਿਕਾਰਡ = ਸੱਚ;
      subset_ਰਿਕਾਰਡ_array (l, &(ਬਾਹਰਲਾ->var_ਰਿਕਾਰਡਾਂ), ਇਹ, ਪਾਓ_catch_all_ਰਿਕਾਰਡ) (var_ਰਿਕਾਰਡਾਂ[0]);
    }

    Array32_ਦਾ<Lookup_Variation_ਰਿਕਾਰਡ> *ਬਾਹਰਲਾ_lookup_ਰਿਕਾਰਡਾਂ = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (ਸੰਸਕਰਣ.minor >= 1)
    {
      ਬਾਹਰਲਾ_lookup_ਰਿਕਾਰਡਾਂ = &StructAfter<Array32_ਦਾ<Lookup_Variation_ਰਿਕਾਰਡ>> (ਬਾਹਰਲਾ->var_ਰਿਕਾਰਡਾਂ);
      ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ_lookup_ਰਿਕਾਰਡਾਂ)))
	return_trace (ਝੂਠ);

      + get_lookup_variation_ਰਿਕਾਰਡਾਂ ().iter ()
      | hb_apply (subset_ਰਿਕਾਰਡ_array (l, ਬਾਹਰਲਾ_lookup_ਰਿਕਾਰਡਾਂ, ਇਹ))
      ;
    }

    return_trace (ਤਾਰਕਿਕ (ਬਾਹਰਲਾ->var_ਰਿਕਾਰਡਾਂ) ||
		  (ਬਾਹਰਲਾ_lookup_ਰਿਕਾਰਡਾਂ && ਤਾਰਕਿਕ (*ਬਾਹਰਲਾ_lookup_ਰਿਕਾਰਡਾਂ)));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ FeatureList *feature_list) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੰਸਕਰਣ.sanitize (c) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1) &&
		  var_ਰਿਕਾਰਡਾਂ.sanitize (c, ਇਹ, feature_list) &&
		  (ਸੰਸਕਰਣ.minor < 1 ||
		   get_lookup_variation_ਰਿਕਾਰਡਾਂ ().sanitize (c, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>	ਸੰਸਕਰਣ;	/* Version--0x00010000u */
  Array32_ਦਾ<Feature_Variation_ਰਿਕਾਰਡ>
			var_ਰਿਕਾਰਡਾਂ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY_SIZED (8, var_ਰਿਕਾਰਡਾਂ);
};


/*
 * Device Tables
 */

ਬਣਤਰ HintingDevice
{
  ਮਿੱਤਰ ਬਣਤਰ Device;

  ਨਿਜੀ:

  hb_position_t get_x_delta (hb_font_t *font) ਅਚਲ
  { ਵਾਪਸ get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ਅਚਲ
  { ਵਾਪਸ get_delta (font->y_ppem, font->y_scale); }

  ਜਨਤਕ:

  size_t get_ਮਾਪ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ f = deltaFormat;
    ਜੇ (unlikely (f < 1 || f > 3 || ਸ਼ੁਰੂ_ਮਾਪ > ਸਿਰਾ_ਮਾਪ)) ਵਾਪਸ 3 * HBUINT16::static_ਮਾਪ;
    ਵਾਪਸ HBUINT16::static_ਮਾਪ * (4 + ((ਸਿਰਾ_ਮਾਪ - ਸ਼ੁਰੂ_ਮਾਪ) >> (4 - f)));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && c->check_range (ਇਹ, ਇਹ->get_ਮਾਪ ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed<HintingDevice> (ਇਹ));
  }

  ਨਿਜੀ:

  ਪੂਰਨ get_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ppem, ਪੂਰਨ scale) ਅਚਲ
  {
    ਜੇ (!ppem) ਵਾਪਸ 0;

    ਪੂਰਨ pixels = get_delta_pixels (ppem);

    ਜੇ (!pixels) ਵਾਪਸ 0;

    ਵਾਪਸ (ਪੂਰਨ) (pixels * (int64_t) scale / ppem);
  }
  ਪੂਰਨ get_delta_pixels (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ppem_ਮਾਪ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ f = deltaFormat;
    ਜੇ (unlikely (f < 1 || f > 3))
      ਵਾਪਸ 0;

    ਜੇ (ppem_ਮਾਪ < ਸ਼ੁਰੂ_ਮਾਪ || ppem_ਮਾਪ > ਸਿਰਾ_ਮਾਪ)
      ਵਾਪਸ 0;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ s = ppem_ਮਾਪ - ਸ਼ੁਰੂ_ਮਾਪ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ byte = delta_ਮੁੱਲ_Z[s >> (4 - f)];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = (0xFFFFu >> (16 - (1 << f)));

    ਪੂਰਨ delta = ਬਿੱਟ & ਨਕਾਬ;

    ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) delta >= ((ਨਕਾਬ + 1) >> 1))
      delta -= ਨਕਾਬ + 1;

    ਵਾਪਸ delta;
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸ਼ੁਰੂ_ਮਾਪ;		/* Smallest size to correct--in ppem */
  HBUINT16	ਸਿਰਾ_ਮਾਪ;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  Unsized_Array_ਦਾ<HBUINT16>
		delta_ਮੁੱਲ_Z;		/* Array of compressed data */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, delta_ਮੁੱਲ_Z);
};

ਬਣਤਰ VariationDevice
{
  ਮਿੱਤਰ ਬਣਤਰ Device;

  ਨਿਜੀ:

  hb_position_t get_x_delta (hb_font_t *font,
			     ਅਚਲ ਵਸਤੂ_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  { ਵਾਪਸ !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ਅਚਲ ਵਸਤੂ_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  { ਵਾਪਸ !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  ਦਸ਼ਮਲਵ get_x_delta_f (hb_font_t *font,
		       ਅਚਲ ਵਸਤੂ_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ਅਚਲ
  { ਵਾਪਸ !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  ਦਸ਼ਮਲਵ get_y_delta_f (hb_font_t *font,
		       ਅਚਲ ਵਸਤੂ_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ਅਚਲ
  { ਵਾਪਸ !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (!layout_variation_idx_delta_map) return_trace (ਖਾਲੀ_ਸੂਚਕ);

    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *v;
    ਜੇ (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (ਖਾਲੀ_ਸੂਚਕ);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    ਜੇ (hb_ਪਹਿਲਾ (*v) == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
      return_trace (ਖਾਲੀ_ਸੂਚਕ);

    c->ਸ਼ੁਰੂ_zerocopy (ਇਹ->static_ਮਾਪ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਖਾਲੀ_ਸੂਚਕ);

    ਜੇ (!c->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->varIdx, hb_ਪਹਿਲਾ (*v), HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਖਾਲੀ_ਸੂਚਕ);
    return_trace (ਬਾਹਰਲਾ);
  }

  ਖਾਲੀ collect_variation_ਸੂਚਕਾਂਕ (hb_collect_variation_indices_context_t *c) ਅਚਲ
  { c->layout_variation_indices->ਜੋੜੋ (varIdx); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਨਿਜੀ:

  ਦਸ਼ਮਲਵ get_delta (hb_font_t *font,
		   ਅਚਲ ਵਸਤੂ_Variation_Store &store,
		   hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਵਾਪਸ store.get_delta (varIdx, font->coords, font->ਅੰਕ_coords, store_cache);
  }

  ਸੁਰੱਖਿਅਤ:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ Device_ਸਿਰਲੇਖ
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  ਜਨਤਕ:
  HBUINT16		format;		/* Format identifier */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  ਦਸ਼ਮਲਵ get_x_delta_f (hb_font_t *font,
		       ਅਚਲ ਵਸਤੂ_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (u.b.format == 0x8000)
    {
      hb_barrier ();
      ਵਾਪਸ u.variation.get_x_delta_f (font, store, store_cache);
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ get_x_delta (font, store, store_cache);
  }

  ਦਸ਼ਮਲਵ get_y_delta_f (hb_font_t *font,
		       ਅਚਲ ਵਸਤੂ_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (u.b.format == 0x8000)
    {
      hb_barrier ();
      ਵਾਪਸ u.variation.get_y_delta_f (font, store, store_cache);
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ਅਚਲ ਵਸਤੂ_Variation_Store &store=Null (ਵਸਤੂ_Variation_Store),
			     hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਚੋਣ (u.b.format)
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_HINTING
    ਹਾਲਤ 1: ਹਾਲਤ 2: ਹਾਲਤ 3:
      hb_barrier ();
      ਵਾਪਸ u.hinting.get_x_delta (font);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਹਾਲਤ 0x8000:
      hb_barrier ();
      ਵਾਪਸ u.variation.get_x_delta (font, store, store_cache);
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      ਵਾਪਸ 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ਅਚਲ ਵਸਤੂ_Variation_Store &store=Null (ਵਸਤੂ_Variation_Store),
			     hb_scalar_cache_t *store_cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਚੋਣ (u.b.format)
    {
    ਹਾਲਤ 1: ਹਾਲਤ 2: ਹਾਲਤ 3:
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_HINTING
      hb_barrier ();
      ਵਾਪਸ u.hinting.get_y_delta (font);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਹਾਲਤ 0x8000:
      hb_barrier ();
      ਵਾਪਸ u.variation.get_y_delta (font, store, store_cache);
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      ਵਾਪਸ 0;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.b.format.sanitize (c)) return_trace (ਝੂਠ);
    ਚੋਣ (u.b.format) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_HINTING
    ਹਾਲਤ 1: ਹਾਲਤ 2: ਹਾਲਤ 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਹਾਲਤ 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      return_trace (ਸੱਚ);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map=ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਚੋਣ (u.b.format) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_HINTING
    ਹਾਲਤ 1:
    ਹਾਲਤ 2:
    ਹਾਲਤ 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਹਾਲਤ 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      return_trace (ਖਾਲੀ_ਸੂਚਕ);
    }
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਚੋਣ (u.b.format) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_HINTING
    ਹਾਲਤ 1:
    ਹਾਲਤ 2:
    ਹਾਲਤ 3:
      ਵਾਪਸ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਹਾਲਤ 0x8000:
      hb_barrier ();
      u.variation.collect_variation_ਸੂਚਕਾਂਕ (c);
      ਵਾਪਸ;
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      ਵਾਪਸ;
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_variation_ਸੂਚਕਾਂਕ () ਅਚਲ
  {
    ਚੋਣ (u.b.format) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਹਾਲਤ 0x8000:
      hb_barrier ();
      ਵਾਪਸ u.variation.varIdx;
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      ਵਾਪਸ HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ;
    }
  }

  ਤਾਰਕਿਕ is_variation_device () ਅਚਲ
  {
    ਚੋਣ (u.b.format) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਹਾਲਤ 0x8000:
      ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
    ਮੂਲ:
      ਵਾਪਸ ਝੂਠ;
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  Device_ਸਿਰਲੇਖ		b;
  HintingDevice		hinting;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  VariationDevice	variation;
#ਜੇ_ਅੰਤ
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (6, b);
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_LAYOUT_COMMON_HH */

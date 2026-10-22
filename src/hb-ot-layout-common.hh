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

#ifndef HB_OT_LAYOUT_COMMON_HH
#define HB_OT_LAYOUT_COMMON_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-set.hh"
#ಅನ್ತರ್ಭಾವ "hb-bimap.hh"
#ಅನ್ತರ್ಭಾವ "hb-cache.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (ಪರಿಮಾಣ (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_ದ್ವಿಮಾನ_cache_t = hb_cache_t<14, 1, 8>;
static_assert (ಪರಿಮಾಣ (hb_ot_layout_ದ್ವಿಮಾನ_cache_t) == 256, "");

#ಅನ್ತರ್ಭಾವ "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


ನಾಮಾಕಾಶ OT {

template<typename Iterator>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ಸ್ಥಿರ ತಾರ್ಕಿಕ ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ಧ್ರುವ hb_set_t &klasses,
    ತಾರ್ಕಿಕ use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

ಸಂರಚನಾ ItemVarStoreInstancer;

ಸಂರಚನಾ hb_collect_feature_substitutes_with_var_context_t
{
  ಧ್ರುವ hb_map_t *axes_index_tag_map;
  ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, ಧ್ರುವ Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  ತಾರ್ಕಿಕ apply;
  ತಾರ್ಕಿಕ variation_applied;
  ತಾರ್ಕಿಕ universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  ತಾರ್ಕಿಕ all_axes_pinned;
};

ಸಂರಚನಾ hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ಧ್ರುವ ಶೂನ್ಯ         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              ಧ್ರುವ hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_ಎಣಿಕೆ (0),langsys_feature_ಎಣಿಕೆ (0) {}

  ತಾರ್ಕಿಕ visitScript ()
  { ಪ್ರತ್ಯಾಗಮ script_ಎಣಿಕೆ++ < HB_MAX_SCRIPTS; }

  ತಾರ್ಕಿಕ visitLangsys (unsigned feature_ಎಣಿಕೆ_2)
  {
    langsys_feature_ಎಣಿಕೆ += feature_ಎಣಿಕೆ_2;
    ಪ್ರತ್ಯಾಗಮ langsys_feature_ಎಣಿಕೆ < HB_MAX_LANGSYS_FEATURE_ಎಣಿಕೆ;
  }

  ಸಾರ್ವಜನಿಕ:
  ಧ್ರುವ ಶೂನ್ಯ *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ಧ್ರುವ hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  ಗುಪ್ತ:
  unsigned script_ಎಣಿಕೆ;
  unsigned langsys_feature_ಎಣಿಕೆ;
};

ಸಂರಚನಾ hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  ಧ್ರುವ ವರ್ಣ *get_name () { ಪ್ರತ್ಯಾಗಮ "SUBSET_LAYOUT"; }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }

  ತಾರ್ಕಿಕ visitScript ()
  {
    ಪ್ರತ್ಯಾಗಮ script_ಎಣಿಕೆ++ < HB_MAX_SCRIPTS;
  }

  ತಾರ್ಕಿಕ visitLangSys ()
  {
    ಪ್ರತ್ಯಾಗಮ langsys_ಎಣಿಕೆ++ < HB_MAX_LANGSYS;
  }

  ತಾರ್ಕಿಕ visitFeatureIndex (ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
  {
    feature_index_ಎಣಿಕೆ += ಎಣಿಕೆ;
    ಪ್ರತ್ಯಾಗಮ feature_index_ಎಣಿಕೆ < HB_MAX_FEATURE_INDICES;
  }

  ತಾರ್ಕಿಕ visitLookupIndex()
  {
    lookup_index_ಎಣಿಕೆ++;
    ಪ್ರತ್ಯಾಗಮ lookup_index_ಎಣಿಕೆ < HB_MAX_LOOKUP_VISIT_ಎಣಿಕೆ;
  }

  hb_subset_context_t *subset_context;
  ಧ್ರುವ hb_tag_t table_tag;
  ಧ್ರುವ hb_map_t *lookup_index_map;
  ಧ್ರುವ hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ಧ್ರುವ hb_map_t *feature_index_map;
  ಧ್ರುವ hb_map_t *feature_map_w_duplicates;
  ಧ್ರುವ hb_hashmap_t<unsigned, ಧ್ರುವ Feature*> *feature_substitutes_map;
  ಧ್ರುವ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  ಧ್ರುವ hb_set_t *catch_all_record_feature_idxes;
  ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*>> *feature_idx_tag_map;

  unsigned cur_script_index;
  unsigned cur_feature_index;
  unsigned cur_feature_var_record_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_tag_t tag_) :
				subset_context (c_),
				table_tag (tag_),
				cur_script_index (0xFFFFu),
				cur_feature_index (Index::NOT_FOUND_INDEX),
				cur_feature_var_record_idx (0u),
				script_ಎಣಿಕೆ (0),
				langsys_ಎಣಿಕೆ (0),
				feature_index_ಎಣಿಕೆ (0),
				lookup_index_ಎಣಿಕೆ (0)
  {
    ಯದಿ (tag_ == HB_OT_TAG_GSUB)
    {
      lookup_index_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_index_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_record_cond_idx_map = c_->plan->user_axes_location.is_empty () ? nullptr : &c_->plan->gsub_feature_record_cond_idx_map;
      catch_all_record_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_tag_map = &c_->plan->gsub_old_feature_idx_tag_map;
    }
    ಅನ್ಯಥಾ
    {
      lookup_index_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_index_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_record_cond_idx_map = c_->plan->user_axes_location.is_empty () ? nullptr : &c_->plan->gpos_feature_record_cond_idx_map;
      catch_all_record_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_tag_map = &c_->plan->gpos_old_feature_idx_tag_map;
    }
  }

  ಗುಪ್ತ:
  unsigned script_ಎಣಿಕೆ;
  unsigned langsys_ಎಣಿಕೆ;
  unsigned feature_index_ಎಣಿಕೆ;
  unsigned lookup_index_ಎಣಿಕೆ;
};

ಸಂರಚನಾ ItemVariationStore;
ಸಂರಚನಾ hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj) { obj.collect_variation_indices (ಏತತ್); ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  ಧ್ರುವ hb_set_t *glyph_set;
  ಧ್ರುವ hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *condition_ಮೌಲ್ಯಗಳು = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  ಧ್ರುವ hb_set_t *glyph_set_,
					  ಧ್ರುವ hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
ಸಂರಚನಾ subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 ಧ್ರುವ ಶೂನ್ಯ *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  ತಾರ್ಕಿಕ operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    ಯದಿ (unlikely (!o)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ ret = o->serialize_subset (subset_context, offset, base);
    ಯದಿ (!ret)
    {
      out.ತೆಗೆ ();
      subset_context->serializer->revert (snap);
    }
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಗುಪ್ತ:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ಧ್ರುವ ಶೂನ್ಯ *base;
};


template<typename OutputArray, typename Arg>
ಸಂರಚನಾ subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     ಧ್ರುವ ಶೂನ್ಯ *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  ತಾರ್ಕಿಕ operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    ಯದಿ (unlikely (!o)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ ret = o->serialize_subset (subset_context, offset, base, arg);
    ಯದಿ (!ret)
    {
      out.ತೆಗೆ ();
      subset_context->serializer->revert (snap);
    }
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಗುಪ್ತ:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ಧ್ರುವ ಶೂನ್ಯ *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
ಸಂರಚನಾ
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ಧ್ರುವ ಶೂನ್ಯ *base, Arg &&arg) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
ಸಂರಚನಾ subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 ಧ್ರುವ ಶೂನ್ಯ *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  ಶೂನ್ಯ
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    ತಾರ್ಕಿಕ ret = record.subset (subset_layout_context, base);
    ಯದಿ (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    ಅನ್ಯಥಾ out->len++;
  }

  ಗುಪ್ತ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ಧ್ರುವ ಶೂನ್ಯ *base;
};

template<typename OutputArray, typename Arg>
ಸಂರಚನಾ subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     ಧ್ರುವ ಶೂನ್ಯ *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  ಶೂನ್ಯ
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    ತಾರ್ಕಿಕ ret = record.subset (subset_layout_context, base, arg);
    ಯದಿ (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    ಅನ್ಯಥಾ out->len++;
  }

  ಗುಪ್ತ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ಧ್ರುವ ಶೂನ್ಯ *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
ಸಂರಚನಾ
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               ಧ್ರುವ ಶೂನ್ಯ *base, Arg &&arg) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
ಸಂರಚನಾ serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         ಧ್ರುವ ಶೂನ್ಯ *base_,
                         ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  ತಾರ್ಕಿಕ operator () (T&& record)
  {
    ಯದಿ (!serialize_context->copy (record, base, variation_map)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    out.len++;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಗುಪ್ತ:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  ಧ್ರುವ ಶೂನ್ಯ *base;
  ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
ಸಂರಚನಾ
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               ಧ್ರುವ ಶೂನ್ಯ *base,
               ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *variation_map) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

}
HB_FUNCOBJ (serialize_math_record_array);

/*
 *
 * OpenType Layout Common Table Formats
 *
 */


/*
 * Script, ScriptList, LangSys, Feature, FeatureList, Lookup, LookupList
 */

ಸಂರಚನಾ IndexArray : Array16Of<Index>
{
  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_map_t *indexes) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_any (*ಏತತ್, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    ಯದಿ (!it) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (!c->extend_min ((*ಏತತ್)))) ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ (ಧ್ರುವ auto _ : it)
    {
      ಯದಿ (!l->visitLookupIndex()) ಭಙ್ಗ;

      Index i;
      i = _;
      c->copy (i);
      ಏತತ್->len++;
    }
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_indexes (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			    unsigned ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ_2 /* IN/OUT */,
			    unsigned ಪೂರ್ಣಾಙ್ಕ *_indexes /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (ಎಣಿಕೆ_2 && _indexes)
    {
      + ಏತತ್->as_array ().sub_array (start_offset, ಎಣಿಕೆ_2)
      | hb_sink (hb_array (_indexes, *ಎಣಿಕೆ_2))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->len;
  }

  ಶೂನ್ಯ add_indexes_to (hb_set_t* output /* OUT */) ಧ್ರುವ
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
ಸಂರಚನಾ FeatureParamsSize
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್))) return_trace (ಅಸತ್ಯ);
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

    ಯದಿ (!designSize)
      return_trace (ಅಸತ್ಯ);
    ಅನ್ಯಥಾ ಯದಿ (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (ಸತ್ಯ);
    ಅನ್ಯಥಾ ಯದಿ (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (ಅಸತ್ಯ);
    ಅನ್ಯಥಾ
      return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ಧ್ರುವ
  { nameids_to_retain->add (subfamilyNameID); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    return_trace ((ತಾರ್ಕಿಕ) c->serializer->embed (*ಏತತ್));
  }

  HBUINT16	designSize;	/* Represents the design size in 720/inch
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
  HBUINT16	rangeStart;	/* Large end of the recommended usage range
				 * (inclusive), stored in 720/inch units
				 * (decipoints). */
  HBUINT16	rangeEnd;	/* Small end of the recommended usage range
				   (exclusive), stored in 720/inch units
				 * (decipoints). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
ಸಂರಚನಾ FeatureParamsStylisticSet
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ಧ್ರುವ
  { nameids_to_retain->add (uiNameID); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    return_trace ((ತಾರ್ಕಿಕ) c->serializer->embed (*ಏತತ್));
  }

  HBUINT16	version;	/* (set to 0): This corresponds to a “minor”
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
ಸಂರಚನಾ FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_ಎಣಿಕೆ, hb_codepoint_t *chars) ಧ್ರುವ
  {
    ಯದಿ (char_ಎಣಿಕೆ && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_ಎಣಿಕೆ)
      | hb_sink (hb_array (chars, *char_ಎಣಿಕೆ))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ characters.len;
  }

  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  ಶೂನ್ಯ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    ಯದಿ (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    ಯದಿ (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    ಯದಿ (!firstParamUILabelNameID || !ಅಂಕೆ_Named_Parameters || ಅಂಕೆ_Named_Parameters >= 0x7FFF)
      ಪ್ರತ್ಯಾಗಮ;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) ಅಂಕೆ_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    return_trace ((ತಾರ್ಕಿಕ) c->serializer->embed (*ಏತತ್));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  characters.sanitize (c));
  }

  HBUINT16	format;			/* Format number is set to 0. */
  NameID	featUILableNameID;	/* The ‘name’ table name ID that
					 * specifies a string (or strings,
					 * for multiple languages) for a
					 * user-interface label for this
					 * feature. (May be NULL.) */
  NameID	featUITooltipTextNameID;/* The ‘name’ table name ID that
					 * specifies a string (or strings,
					 * for multiple languages) that an
					 * application can use for tooltip
					 * text for this feature. (May be
					 * nullptr.) */
  NameID	sampleTextNameID;	/* The ‘name’ table name ID that
					 * specifies sample text that
					 * illustrates the effect of this
					 * feature. (May be NULL.) */
  HBUINT16	ಅಂಕೆ_Named_Parameters;	/* Number of named parameters. (May
					 * be zero.) */
  NameID	firstParamUILabelNameID;/* The first ‘name’ table name ID
					 * used to specify strings for
					 * user-interface labels for the
					 * feature parameters. (Must be zero
					 * if numParameters is zero.) */
  Array16Of<HBUINT24>
		characters;		/* Array of the Unicode Scalar Value
					 * of the characters for which this
					 * feature provides glyph variants.
					 * (May be zero.) */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (14, characters);
};

ಸಂರಚನಾ FeatureParams
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, hb_tag_t tag) ಧ್ರುವ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    ಯದಿ ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    ಯದಿ ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ಧ್ರುವ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ಪ್ರತ್ಯಾಗಮ;
#endif
    ಯದಿ (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ಪ್ರತ್ಯಾಗಮ (u.size.collect_name_ids (nameids_to_retain));
    ಯದಿ ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ಪ್ರತ್ಯಾಗಮ (u.stylisticSet.collect_name_ids (nameids_to_retain));
    ಯದಿ ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ಪ್ರತ್ಯಾಗಮ (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c, ಧ್ರುವ Tag* tag) ಧ್ರುವ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (!tag) return_trace (ಅಸತ್ಯ);
    ಯದಿ (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    ಯದಿ ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    ಯದಿ ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (ಅಸತ್ಯ);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  ಧ್ರುವ FeatureParamsSize& get_size_params (hb_tag_t tag) ಧ್ರುವ
  {
    ಯದಿ (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ಪ್ರತ್ಯಾಗಮ u.size;
    ಪ್ರತ್ಯಾಗಮ Null (FeatureParamsSize);
  }
  ಧ್ರುವ FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) ಧ್ರುವ
  {
    ಯದಿ ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ಪ್ರತ್ಯಾಗಮ u.stylisticSet;
    ಪ್ರತ್ಯಾಗಮ Null (FeatureParamsStylisticSet);
  }
  ಧ್ರುವ FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) ಧ್ರುವ
  {
    ಯದಿ ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ಪ್ರತ್ಯಾಗಮ u.characterVariants;
    ಪ್ರತ್ಯಾಗಮ Null (FeatureParamsCharacterVariants);
  }
#endif

  ಗುಪ್ತ:
  ಸಙ್ಘ {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (0);
};

ಸಂರಚನಾ Record_sanitize_closure_t {
  hb_tag_t tag;
  ಧ್ರುವ ಶೂನ್ಯ *list_base;
};

ಸಂರಚನಾ Feature
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_lookup_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ lookupIndex[i]; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_lookup_indexes (unsigned ಪೂರ್ಣಾಙ್ಕ start_index,
				   unsigned ಪೂರ್ಣಾಙ್ಕ *lookup_ಎಣಿಕೆ /* IN/OUT */,
				   unsigned ಪೂರ್ಣಾಙ್ಕ *lookup_tags /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ lookupIndex.get_indexes (start_index, lookup_ಎಣಿಕೆ, lookup_tags); }
  ಶೂನ್ಯ add_lookup_indexes_to (hb_set_t *lookup_indexes) ಧ್ರುವ
  { lookupIndex.add_indexes_to (lookup_indexes); }

  ಧ್ರುವ FeatureParams &get_feature_params () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+featureParams; }

  ತಾರ್ಕಿಕ intersects_lookup_indexes (ಧ್ರುವ hb_map_t *lookup_indexes) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ lookupIndex.intersects (lookup_indexes); }

  ಶೂನ್ಯ collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ಧ್ರುವ Tag                   *tag = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->featureParams.serialize_subset (c, featureParams, ಏತತ್, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    ಯದಿ (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    ಅನ್ಯಥಾ
    {
      auto it =
      + hb_iter (lookupIndex)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ Record_sanitize_closure_t *closure = nullptr) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  featureParams.sanitize (c, ಏತತ್, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
ಸಂರಚನಾ Record
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tag.cmp (a); }

  ತಾರ್ಕಿಕ subset (hb_subset_layout_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base, ಧ್ರುವ ಶೂನ್ಯ *f_sub = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->subset_context->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    ಧ್ರುವ Feature& f = *reinterpret_cast<ಧ್ರುವ Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    ತಾರ್ಕಿಕ ret = f.subset (c->subset_context, c, &tag);
    ಯದಿ (ret)
      s->add_link (out->offset, s->ತೆಗೆ_pack ());
    ಅನ್ಯಥಾ
      s->ತೆಗೆ_discard ();

    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಧ್ರುವ Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (ಏತತ್) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
ಸಂರಚನಾ RecordArrayOf : SortedArray16Of<Record<Type>>
{
  ಧ್ರುವ Offset16To<Type>& get_offset (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[i].offset; }
  Offset16To<Type>& get_offset (unsigned ಪೂರ್ಣಾಙ್ಕ i)
  { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[i].offset; }
  ಧ್ರುವ Tag& get_tag (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[i].tag; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_tags (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
                         unsigned ಪೂರ್ಣಾಙ್ಕ *record_ಎಣಿಕೆ /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (record_ಎಣಿಕೆ && record_tags)
    {
      + ಏತತ್->as_array ().sub_array (start_offset, record_ಎಣಿಕೆ)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_ಎಣಿಕೆ))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->len;
  }
  ತಾರ್ಕಿಕ find_index (hb_tag_t tag, unsigned ಪೂರ್ಣಾಙ್ಕ *index) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
ಸಂರಚನಾ RecordListOf : RecordArrayOf<Type>
{
  ಧ್ರುವ Type& operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+ಏತತ್->get_offset (i); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    + ಏತತ್->iter ()
    | hb_apply (subset_record_array (l, out, ಏತತ್))
    ;
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (RecordArrayOf<Type>::sanitize (c, ಏತತ್));
  }
};

ಸಂರಚನಾ RecordListOfFeature : RecordListOf<Feature>
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    + hb_enumerate (*ಏತತ್)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, ಏತತ್] (ಧ್ರುವ hb_pair_t<unsigned, ಧ್ರುವ Record<Feature>&>& _)
                {
                  ಧ್ರುವ Feature *f_sub = nullptr;
                  ಧ್ರುವ Feature **f = nullptr;
                  ಯದಿ (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, ಏತತ್, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (ಸತ್ಯ);
  }
};

ಪ್ರಕಾರನಿರ್ದೇಶ RecordListOf<Feature> FeatureList;


ಸಂರಚನಾ LangSys
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_feature_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ featureIndex.len; }
  hb_tag_t get_feature_index (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ featureIndex[i]; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_feature_indexes (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				    unsigned ಪೂರ್ಣಾಙ್ಕ *feature_ಎಣಿಕೆ_2 /* IN/OUT */,
				    unsigned ಪೂರ್ಣಾಙ್ಕ *feature_indexes /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ featureIndex.get_indexes (start_offset, feature_ಎಣಿಕೆ_2, feature_indexes); }
  ಶೂನ್ಯ add_feature_indexes_to (hb_set_t *feature_indexes) ಧ್ರುವ
  { featureIndex.add_indexes_to (feature_indexes); }

  ತಾರ್ಕಿಕ has_required_feature () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ reqFeatureIndex != 0xFFFFu; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_required_feature_index () ಧ್ರುವ
  {
    ಯದಿ (reqFeatureIndex == 0xFFFFu)
      ಪ್ರತ್ಯಾಗಮ Index::NOT_FOUND_INDEX;
   ಪ್ರತ್ಯಾಗಮ reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed (*ಏತತ್));
  }

  ತಾರ್ಕಿಕ compare (ಧ್ರುವ LangSys& o, ಧ್ರುವ hb_map_t *feature_index_map) ಧ್ರುವ
  {
    ಯದಿ (reqFeatureIndex != o.reqFeatureIndex)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    auto iter =
    + hb_iter (featureIndex)
    | hb_filter (feature_index_map)
    | hb_map (feature_index_map)
    ;

    auto o_iter =
    + hb_iter (o.featureIndex)
    | hb_filter (feature_index_map)
    | hb_map (feature_index_map)
    ;

    ಪ್ರತಿ (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      ಯದಿ (a != b) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಯದಿ (iter || o_iter) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ collect_features (hb_prune_langsys_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!has_required_feature () && !get_feature_ಎಣಿಕೆ ()) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ಧ್ರುವ Tag                  *tag = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    ಯದಿ (!l->visitFeatureIndex (featureIndex.len))
      return_trace (ಅಸತ್ಯ);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    ತಾರ್ಕಿಕ ret = ತಾರ್ಕಿಕ (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ Record_sanitize_closure_t * = nullptr) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

ಸಂರಚನಾ Script
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_lang_sys_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ langSys.len; }
  ಧ್ರುವ Tag& get_lang_sys_tag (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ langSys.get_tag (i); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_lang_sys_tags (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				  unsigned ಪೂರ್ಣಾಙ್ಕ *lang_sys_ಎಣಿಕೆ /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ langSys.get_tags (start_offset, lang_sys_ಎಣಿಕೆ, lang_sys_tags); }
  ಧ್ರುವ LangSys& get_lang_sys (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
    ಯದಿ (i == Index::NOT_FOUND_INDEX) ಪ್ರತ್ಯಾಗಮ get_default_lang_sys ();
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+langSys[i].offset;
  }
  ತಾರ್ಕಿಕ find_lang_sys_index (hb_tag_t tag, unsigned ಪೂರ್ಣಾಙ್ಕ *index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ langSys.find_index (tag, index); }

  ತಾರ್ಕಿಕ has_default_lang_sys () ಧ್ರುವ           { ಪ್ರತ್ಯಾಗಮ defaultLangSys != 0; }
  ಧ್ರುವ LangSys& get_default_lang_sys () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+defaultLangSys; }

  ಶೂನ್ಯ prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) ಧ್ರುವ
  {
    ಯದಿ (!has_default_lang_sys () && !get_lang_sys_ಎಣಿಕೆ ()) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!c->visitScript ()) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (!c->script_langsys_map->has (script_index))
    {
      ಯದಿ (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ಧ್ರುವ LangSys& d = get_default_lang_sys ();
      ಯದಿ (c->visitLangsys (d.get_feature_ಎಣಿಕೆ ())) {
        d.collect_features (c);
      }

      ಪ್ರತಿ (auto _ : + hb_enumerate (langSys))
      {
        ಧ್ರುವ LangSys& l = ಏತತ್+_.second.offset;
        ಯದಿ (!c->visitLangsys (l.get_feature_ಎಣಿಕೆ ())) ಅನುವರ್ತ;
        ಯದಿ (l.compare (d, c->duplicate_feature_map)) ಅನುವರ್ತ;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (auto _ : + hb_enumerate (langSys))
      {
        ಧ್ರುವ LangSys& l = ಏತತ್+_.second.offset;
        ಯದಿ (!c->visitLangsys (l.get_feature_ಎಣಿಕೆ ())) ಅನುವರ್ತ;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ಧ್ರುವ Tag                   *tag) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (!l->visitScript ()) return_trace (ಅಸತ್ಯ);
    ಯದಿ (tag && !c->plan->layout_scripts.has (*tag))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ defaultLang = ಅಸತ್ಯ;
    ಯದಿ (has_default_lang_sys ())
    {
      c->serializer->push ();
      ಧ್ರುವ LangSys& ls = ಏತತ್+defaultLangSys;
      ತಾರ್ಕಿಕ ret = ls.subset (c, l);
      ಯದಿ (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->ತೆಗೆ_discard ();
	out->defaultLangSys = 0;
      }
      ಅನ್ಯಥಾ
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->ತೆಗೆ_pack ());
	defaultLang = ಸತ್ಯ;
      }
    }

    ಧ್ರುವ hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    ಯದಿ (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (ಧ್ರುವ Record<LangSys>& record) {ಪ್ರತ್ಯಾಗಮ l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), ಏತತ್))
      ;
    }

    return_trace (ತಾರ್ಕಿಕ (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ Record_sanitize_closure_t * = nullptr) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (defaultLangSys.sanitize (c, ಏತತ್) && langSys.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

ಸಂರಚನಾ RecordListOfScript : RecordListOf<Script>
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (auto _ : + hb_enumerate (*ಏತತ್))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      ತಾರ್ಕಿಕ ret = _.second.subset (l, ಏತತ್);
      ಯದಿ (!ret) c->serializer->revert (snap);
      ಅನ್ಯಥಾ out->len++;
    }

    return_trace (ಸತ್ಯ);
  }
};

ಪ್ರಕಾರನಿರ್ದೇಶ RecordListOfScript ScriptList;



ಸಂರಚನಾ LookupFlag : HBUINT16
{
  ಗಣನಾ Flags {
    ಬಲ_To_ಎಡ		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
ನಾಮಾಕಾಶ OT {

ಸಂರಚನಾ Lookup
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_subtable_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ subTable.len; }

  template <typename TSubTable>
  ಧ್ರುವ Array16OfOffset16To<TSubTable>& get_subtables () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<ಧ್ರುವ Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { ಪ್ರತ್ಯಾಗಮ reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  ಧ್ರುವ TSubTable& get_subtable (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned ಪೂರ್ಣಾಙ್ಕ i)
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+get_subtables<TSubTable> ()[i]; }

  size_t get_size () ಧ್ರುವ
  {
    ಧ್ರುವ HBUINT16 &markFilteringSet = StructAfter<ಧ್ರುವ HBUINT16> (subTable);
    ಯದಿ (lookupFlag & LookupFlag::UseMarkFilteringSet)
      ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ ವರ್ಣ *) &StructAfter<ಧ್ರುವ ವರ್ಣ> (markFilteringSet) - (ಧ್ರುವ ವರ್ಣ *) ಏತತ್;
    ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ ವರ್ಣ *) &markFilteringSet - (ಧ್ರುವ ವರ್ಣ *) ಏತತ್;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ flag = lookupFlag;
    ಯದಿ (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      ಧ್ರುವ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    ಪ್ರತ್ಯಾಗಮ flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ lookup_type = get_type ();
    TRACE_DISPATCH (ಏತತ್, lookup_type);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = get_subtable_ಎಣಿಕೆ ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      ಯದಿ (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_ಮೌಲ್ಯ ());
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_type,
		  uint32_t lookup_props,
		  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_subtables)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    ಯದಿ (unlikely (!subTable.serialize (c, ಅಂಕೆ_subtables))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ಯದಿ (unlikely (!c->extend (ಏತತ್))) return_trace (ಅಸತ್ಯ);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (ಸತ್ಯ);
  }

  template <typename TSubTable>
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    ಧ್ರುವ hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned ಪೂರ್ಣಾಙ್ಕ lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([ಏತತ್, glyphset, lookup_type] (ಧ್ರುವ Offset16To<TSubTable> &_) { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), ಏತತ್, lookup_type))
    ;

    ಯದಿ (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ಧ್ರುವ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      ಯದಿ (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
      {
        unsigned new_flag = lookupFlag;
        new_flag &= ~LookupFlag::UseMarkFilteringSet;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        out->lookupFlag = new_flag;
      }
      ಅನ್ಯಥಾ
      {
        ಯದಿ (unlikely (!c->serializer->extend (out))) return_trace (ಅಸತ್ಯ);
        HBUINT16 &outMarkFilteringSet = StructAfter<HBUINT16> (out->subTable);
        outMarkFilteringSet = *idx;
      }
    }

    // Always keep the lookup even if it's empty. The rest of layout subsetting depends on lookup
    // indices being consistent with those computed during planning. So if an empty lookup is
    // discarded during the subset phase it will invalidate all subsequent lookup indices.
    // Generally we shouldn't end up with an empty lookup as we pre-prune them during the planning
    // phase, but it can happen in rare cases such as when during closure subtable is considered
    // degenerate (see: https://github.com/harfbuzz/harfbuzz/issues/3853)
    return_trace (ಸತ್ಯ);
  }

  template <typename TSubTable>
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(c->check_struct (ಏತತ್) && subTable.sanitize (c))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    unsigned subtables = get_subtable_ಎಣಿಕೆ ();
    ಯದಿ (unlikely (!c->visit_subtables (subtables))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ಧ್ರುವ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      ಯದಿ (!markFilteringSet.sanitize (c)) return_trace (ಅಸತ್ಯ);
    }

    ಯದಿ (unlikely (!get_subtables<TSubTable> ().sanitize (c, ಏತತ್, get_type ())))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned ಪೂರ್ಣಾಙ್ಕ type = get_subtable<TSubTable> (0).u.extension.get_type ();
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < subtables; i++)
	ಯದಿ (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (ಅಸತ್ಯ);
    }
    return_trace (ಸತ್ಯ);
  }

  ರಕ್ಷಿತ:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
ಸಂರಚನಾ LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  ತಾರ್ಕಿಕ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    + hb_enumerate (*ಏತತ್)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, ಏತತ್))
    ;
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, ಏತತ್)));
  }
};


/*
 * Coverage Table
 */


ಸ್ಥಿರ ತಾರ್ಕಿಕ ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ಧ್ರುವ hb_set_t &klasses,
                                          ತಾರ್ಕಿಕ use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  ಯದಿ (!klass_map)
    ಪ್ರತ್ಯಾಗಮ ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  ಯದಿ (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  ಪ್ರತಿ (ಧ್ರುವ unsigned k: klasses)
  {
    ಯದಿ (klass_map->has (k)) ಅನುವರ್ತ;
    klass_map->set (k, idx);
    idx++;
  }


  ಯದಿ (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಪ್ರತಿ (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  ಪ್ರತ್ಯಾಗಮ ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
ಸಂರಚನಾ ClassDefFormat1_3
{
  friend ಸಂರಚನಾ ClassDef;

  ಗುಪ್ತ:
  unsigned ಪೂರ್ಣಾಙ್ಕ get_class (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ class_ಮೌಲ್ಯ[(unsigned ಪೂರ್ಣಾಙ್ಕ) (glyph_id - startGlyph)];
  }

  unsigned get_population () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ class_ಮೌಲ್ಯ.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_ಮೌಲ್ಯ.len = 0;
      return_trace (ಸತ್ಯ);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_ಎಣಿಕೆ = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    ಯದಿ (unlikely (!class_ಮೌಲ್ಯ.serialize (c, glyph_ಎಣಿಕೆ))) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (ಧ್ರುವ hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_ಮೌಲ್ಯ[idx] = gid_klass_pair.second;
    }
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               ತಾರ್ಕಿಕ keep_empty_table = ಸತ್ಯ,
               ತಾರ್ಕಿಕ use_class_zero = ಸತ್ಯ,
               ಧ್ರುವ Coverage* glyph_filter = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_ಮೌಲ್ಯ.len;

    ಪ್ರತಿ (ಧ್ರುವ hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      ಯದಿ (new_gid == HB_MAP_ಮೌಲ್ಯ_INVALID) ಅನುವರ್ತ;
      ಯದಿ (glyph_filter && !glyph_filter->has(gid)) ಅನುವರ್ತ;

      unsigned klass = class_ಮೌಲ್ಯ[gid - start];
      ಯದಿ (!klass) ಅನುವರ್ತ;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    ಯದಿ (use_class_zero)
    {
      ಧ್ರುವ hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_ಎಣಿಕೆ = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_ಎಣಿಕೆ <= glyph_and_klass.length;
    }
    ಯದಿ (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (ಅಸತ್ಯ);
    return_trace (keep_empty_table || (ತಾರ್ಕಿಕ) glyph_and_klass);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && class_ಮೌಲ್ಯ.sanitize (c));
  }

  unsigned cost () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ 1; }

  template <typename set_t>
  ತಾರ್ಕಿಕ collect_coverage (set_t *glyphs) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ start = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = class_ಮೌಲ್ಯ.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (class_ಮೌಲ್ಯ[i])
	ಅನುವರ್ತ;

      ಯದಿ (start != i)
	ಯದಿ (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      start = i + 1;
    }
    ಯದಿ (start != ಎಣಿಕೆ)
      ಯದಿ (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + ಎಣಿಕೆ)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename set_t>
  ತಾರ್ಕಿಕ collect_class (set_t *glyphs, unsigned klass) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = class_ಮೌಲ್ಯ.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (class_ಮೌಲ್ಯ[i] == klass) glyphs->add (startGlyph + i);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_ಮೌಲ್ಯ.len;
    ಪ್ರತಿ (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      ಯದಿ (class_ಮೌಲ್ಯ[iter - start]) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ intersects_class (ಧ್ರುವ hb_set_t *glyphs, unsigned klass) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = class_ಮೌಲ್ಯ.len;
    ಯದಿ (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_ಮೌಲ್ಯ_INVALID;
      ಯದಿ (!glyphs->next (&g)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (g < startGlyph) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      g = startGlyph + ಎಣಿಕೆ - 1;
      ಯದಿ (glyphs->next (&g)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ಧ್ರುವ typename Types::HBUINT *arr = class_ಮೌಲ್ಯ.arrayZ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (arr[i] == klass && glyphs->has (startGlyph + i))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ intersected_class_glyphs (ಧ್ರುವ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = class_ಮೌಲ್ಯ.len;
    ಯದಿ (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      ಪ್ರತಿ (uint32_t g = HB_SET_ಮೌಲ್ಯ_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      ಪ್ರತಿ (uint32_t g = startGlyph + ಎಣಿಕೆ - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (class_ಮೌಲ್ಯ[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#ಯದಿ 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      ಯದಿ (class_ಮೌಲ್ಯ.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  ಶೂನ್ಯ intersected_classes (ಧ್ರುವ hb_set_t *glyphs, hb_set_t *intersect_classes) ಧ್ರುವ
  {
    ಯದಿ (glyphs->is_empty ()) ಪ್ರತ್ಯಾಗಮ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    ಯದಿ (!class_ಮೌಲ್ಯ.len)
    {
      intersect_classes->add (0);
      ಪ್ರತ್ಯಾಗಮ;
    }

    hb_codepoint_t end_glyph = startGlyph + class_ಮೌಲ್ಯ.len - 1;
    ಯದಿ (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : + hb_enumerate (class_ಮೌಲ್ಯ))
    {
      hb_codepoint_t g = startGlyph + _.first;
      ಯದಿ (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  ರಕ್ಷಿತ:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_ಮೌಲ್ಯ;	/* Array of Class Values--one per GlyphID */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_ಮೌಲ್ಯ);
};

template <typename Types>
ಸಂರಚನಾ ClassDefFormat2_4
{
  friend ಸಂರಚನಾ ClassDef;

  ಗುಪ್ತ:
  unsigned ಪೂರ್ಣಾಙ್ಕ get_class (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () ಧ್ರುವ
  {
    typename Types::large_int ret = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto &r : rangeRecord)
      ret += r.get_population ();
    ಪ್ರತ್ಯಾಗಮ ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (ಸತ್ಯ);
    }

    unsigned unsorted = ಅಸತ್ಯ;
    unsigned ಅಂಕೆ_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    ಯದಿ (unlikely (!record)) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (ಧ್ರುವ auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      ಯದಿ (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	ಯದಿ (unlikely (cur_gid < prev_gid))
	  unsorted = ಸತ್ಯ;

	ಯದಿ (unlikely (!record)) ಭಙ್ಗ;
	record->last = prev_gid;
	ಅಂಕೆ_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    ಯದಿ (unlikely (c->in_error ())) return_trace (ಅಸತ್ಯ);

    ಯದಿ (likely (record)) record->last = prev_gid;
    rangeRecord.len = ಅಂಕೆ_ranges;

    ಯದಿ (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               ತಾರ್ಕಿಕ keep_empty_table = ಸತ್ಯ,
               ತಾರ್ಕಿಕ use_class_zero = ಸತ್ಯ,
               ಧ್ರುವ Coverage* glyph_filter = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    ಯದಿ (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      ಪ್ರತಿ (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	ಯದಿ (!klass) ಅನುವರ್ತ;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	ಯದಿ (new_gid == HB_MAP_ಮೌಲ್ಯ_INVALID) ಅನುವರ್ತ;
	ಯದಿ (glyph_filter && !glyph_filter->has (g)) ಅನುವರ್ತ;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    ಅನ್ಯಥಾ
    {
      unsigned ಅಂಕೆ_source_glyphs = c->plan->source->get_ಅಂಕೆ_glyphs ();
      ಪ್ರತಿ (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	ಯದಿ (!klass) ಅನುವರ್ತ;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, ಅಂಕೆ_source_glyphs);
	ಪ್ರತಿ (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  ಯದಿ (new_gid == HB_MAP_ಮೌಲ್ಯ_INVALID) ಅನುವರ್ತ;
	  ಯದಿ (glyph_filter && !glyph_filter->has (g)) ಅನುವರ್ತ;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    ಧ್ರುವ hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_ಎಣಿಕೆ = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_ಎಣಿಕೆ <= glyph_and_klass.length;
    ಯದಿ (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (ಅಸತ್ಯ);
    return_trace (keep_empty_table || (ತಾರ್ಕಿಕ) glyph_and_klass);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  ತಾರ್ಕಿಕ collect_coverage (set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತಿ (auto &range : rangeRecord)
      ಯದಿ (range.value)
	ಯದಿ (unlikely (!range.collect_coverage (glyphs)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename set_t>
  ತಾರ್ಕಿಕ collect_class (set_t *glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ klass) ಧ್ರುವ
  {
    ಪ್ರತಿ (auto &range : rangeRecord)
    {
      ಯದಿ (range.value == klass)
	ಯದಿ (unlikely (!range.collect_coverage (glyphs)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಯದಿ (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      ಪ್ರತಿ (auto g : *glyphs)
        ಯದಿ (get_class (g))
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ಧ್ರುವ RangeRecord<Types> &range) { ಪ್ರತ್ಯಾಗಮ range.intersects (*glyphs) && range.value; }));
  }
  ತಾರ್ಕಿಕ intersects_class (ಧ್ರುವ hb_set_t *glyphs, unsigned klass) ಧ್ರುವ
  {
    ಯದಿ (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      ಯದಿ (!rangeRecord.len)
        ಪ್ರತ್ಯಾಗಮ !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_ಮೌಲ್ಯ_INVALID;
      hb_codepoint_t last = HB_SET_ಮೌಲ್ಯ_INVALID;
      auto it = hb_iter (rangeRecord);
      ಪ್ರತಿ (auto &range : it)
      {
        ಯದಿ (it->first == last + 1)
	{
	  it++;
	  ಅನುವರ್ತ;
	}

	ಯದಿ (!glyphs->next (&g))
	  ಭಙ್ಗ;
	ಯದಿ (g < range.first)
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
	g = range.last;
	last = g;
      }
      ಯದಿ (g != HB_SET_ಮೌಲ್ಯ_INVALID && glyphs->next (&g))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      /* Fall through. */
    }
    ಪ್ರತಿ (ಧ್ರುವ auto &range : rangeRecord)
      ಯದಿ (range.value == klass && range.intersects (*glyphs))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ intersected_class_glyphs (ಧ್ರುವ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ಧ್ರುವ
  {
    ಯದಿ (klass == 0)
    {
      hb_codepoint_t g = HB_SET_ಮೌಲ್ಯ_INVALID;
      ಪ್ರತಿ (auto &range : rangeRecord)
      {
	ಯದಿ (!glyphs->next (&g))
	  ಗಚ್ಛ done;
	ಯಾವತ್ (g < range.first)
	{
	  intersect_glyphs->add (g);
	  ಯದಿ (!glyphs->next (&g))
	    ಗಚ್ಛ done;
        }
        g = range.last;
      }
      ಯಾವತ್ (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      ಪ್ರತ್ಯಾಗಮ;
    }

    unsigned ಎಣಿಕೆ = rangeRecord.len;
    ಯದಿ (ಎಣಿಕೆ > glyphs->get_population () * hb_bit_storage (ಎಣಿಕೆ))
    {
      ಪ್ರತಿ (auto g : *glyphs)
      {
        unsigned i;
        ಯದಿ (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿ (auto &range : rangeRecord)
    {
      ಯದಿ (range.value != klass) ಅನುವರ್ತ;

      unsigned end = range.last + 1;
      ಪ್ರತಿ (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  ಶೂನ್ಯ intersected_classes (ಧ್ರುವ hb_set_t *glyphs, hb_set_t *intersect_classes) ಧ್ರುವ
  {
    ಯದಿ (glyphs->is_empty ()) ಪ್ರತ್ಯಾಗಮ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    ಯದಿ (!rangeRecord.len)
    {
      intersect_classes->add (0);
      ಪ್ರತ್ಯಾಗಮ;
    }

    hb_codepoint_t g = HB_SET_ಮೌಲ್ಯ_INVALID;
    ಪ್ರತಿ (auto &range : rangeRecord)
    {
      ಯದಿ (!glyphs->next (&g))
        ಭಙ್ಗ;
      ಯದಿ (g < range.first)
      {
        intersect_classes->add (0);
        ಭಙ್ಗ;
      }
      g = range.last;
    }
    ಯದಿ (g != HB_SET_ಮೌಲ್ಯ_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    ಪ್ರತಿ (ಧ್ರುವ auto& range : rangeRecord)
      ಯದಿ (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  ರಕ್ಷಿತ:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

ಸಂರಚನಾ ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (k); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (k); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_class (k); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_class (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_class (glyph_id);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_class (glyph_id);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_class (glyph_id);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ಧ್ರುವ
  {
    unsigned klass;
    ಯದಿ (cache && cache->get (glyph_id, &klass)) ಪ್ರತ್ಯಾಗಮ klass;
    klass = get_class (glyph_id);
    ಯದಿ (cache) cache->set (glyph_id, klass);
    ಪ್ರತ್ಯಾಗಮ klass;
  }

  unsigned get_population () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_population ();
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_population ();
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_population ();
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    ಯದಿ (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned ಅಂಕೆ_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      ಪ್ರತಿ (ಧ್ರುವ auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	ಯದಿ (cur_klass > class_max) class_max = cur_klass;
	ಯದಿ (cur_gid == glyph_min) ಅನುವರ್ತ;
        ಯದಿ (cur_gid > glyph_max) glyph_max = cur_gid;
	ಯದಿ (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  ಅಂಕೆ_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	ಯದಿ (size >= best_size) ಪ್ರತ್ಯಾಗಮ;
	best_size = size;
	format = candidate;
      };

      ಯದಿ (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      ಯದಿ (glyph_max <= 0xFFFFu && ಅಂಕೆ_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) ಅಂಕೆ_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      ಯದಿ (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      ಯದಿ (glyph_max <= 0xFFFFFFu && ಅಂಕೆ_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) ಅಂಕೆ_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      ಯದಿ (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (ಅಸತ್ಯ, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (ಅಸತ್ಯ);
      }
    }

    u.format.v = format;

    ವಿಕಲ್ಪ (u.format.v)
    {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               ತಾರ್ಕಿಕ keep_empty_table = ಸತ್ಯ,
               ತಾರ್ಕಿಕ use_class_zero = ಸತ್ಯ,
               ಧ್ರುವ Coverage* glyph_filter = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  unsigned cost () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.cost ();
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.cost ();
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.cost ();
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  ತಾರ್ಕಿಕ collect_coverage (set_t *glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.collect_coverage (glyphs);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.collect_coverage (glyphs);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.collect_coverage (glyphs);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  ತಾರ್ಕಿಕ collect_class (set_t *glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ klass) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.collect_class (glyphs, klass);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.collect_class (glyphs, klass);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.collect_class (glyphs, klass);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.intersects (glyphs);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.intersects (glyphs);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.intersects (glyphs);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }
  ತಾರ್ಕಿಕ intersects_class (ಧ್ರುವ hb_set_t *glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ klass) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.intersects_class (glyphs, klass);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.intersects_class (glyphs, klass);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.intersects_class (glyphs, klass);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ಶೂನ್ಯ intersected_class_glyphs (ಧ್ರುವ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ಶೂನ್ಯ intersected_classes (ಧ್ರುವ hb_set_t *glyphs, hb_set_t *intersect_classes) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.intersected_classes (glyphs, intersect_classes);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.intersected_classes (glyphs, intersect_classes);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }


  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ ಪ್ರತ್ಯಾಗಮ (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
ಸಂರಚನಾ delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  ಸಂರಚನಾ chars_t : hb_vector_t<uint8_t>
  {
    ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ chars_t& other) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      ಪ್ರತಿ (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      ಪ್ರತ್ಯಾಗಮ hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (ಧ್ರುವ chars_t& other) ಧ್ರುವ
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      ಪ್ರತಿ (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      ಪ್ರತ್ಯಾಗಮ hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (ಧ್ರುವ delta_row_encoding_t& other_encoding) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*> ವಸ್ತುಗಳು;

  delta_row_encoding_t () = ಸ್ವಭಾವ;
  delta_row_encoding_t (hb_vector_t<ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*> &&rows, unsigned ಅಂಕೆ_cols)
  {
    ಪ್ರತಿಜ್ಞಾ (rows);

    ವಸ್ತುಗಳು = std::move (rows);

    ಯದಿ (unlikely (!chars.resize (ಅಂಕೆ_cols)))
      ಪ್ರತ್ಯಾಗಮ;

    calculate_chars ();
  }

  ಶೂನ್ಯ merge (ಧ್ರುವ delta_row_encoding_t& other)
  {
    ವಸ್ತುಗಳು.alloc (ವಸ್ತುಗಳು.length + other.ವಸ್ತುಗಳು.length);
    ಪ್ರತಿ (auto &row : other.ವಸ್ತುಗಳು)
      add_row (row);

    // Merge chars
    ಪ್ರತಿಜ್ಞಾ (chars.length == other.chars.length);
    ಪ್ರತಿ (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  ಶೂನ್ಯ chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  ಶೂನ್ಯ calculate_chars ()
  {
    ಪ್ರತಿಜ್ಞಾ (ವಸ್ತುಗಳು);

    ತಾರ್ಕಿಕ long_ಪದಗಳು = ಅಸತ್ಯ;

    ಪ್ರತಿ (auto &row : ವಸ್ತುಗಳು)
    {
      ಪ್ರತಿಜ್ಞಾ (row->length == chars.length);

      /* 0/1/2 byte encoding */
      ಪ್ರತಿ (unsigned i = 0; i < row->length; i++)
      {
	ಪೂರ್ಣಾಙ್ಕ v =  row->arrayZ[i];
	ಯದಿ (v == 0)
	  ಅನುವರ್ತ;
	ಅನ್ಯಥಾ ಯದಿ (v > 32767 || v < -32768)
	{
	  long_ಪದಗಳು = ಸತ್ಯ;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	ಅನ್ಯಥಾ ಯದಿ (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	ಅನ್ಯಥಾ
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    ಯದಿ (long_ಪದಗಳು)
    {
      // Convert 1s to 2s
      ಪ್ರತಿ (auto &v : chars)
	ಯದಿ (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ !ವಸ್ತುಗಳು; }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ chars.in_error () || ವಸ್ತುಗಳು.in_error (); }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned get_chars_overhead (unsigned ಅಂಕೆ_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    ಪ್ರತ್ಯಾಗಮ c + ಅಂಕೆ_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) ಧ್ರುವ
  {
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = ವಸ್ತುಗಳು.length;
    ಪ್ರತ್ಯಾಗಮ hb_max (0, (ಪೂರ್ಣಾಙ್ಕ) overhead - ಎಣಿಕೆ * (ಪೂರ್ಣಾಙ್ಕ) additional_bytes_per_rows);
  }

  ಪೂರ್ಣಾಙ್ಕ gain_from_merging (ಧ್ರುವ delta_row_encoding_t& other_encoding) ಧ್ರುವ
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    ಯದಿ (additional_bytes_per_rows > 0)
    {
      ಯದಿ (get_gain (additional_bytes_per_rows) == 0)
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	ಪ್ರತ್ಯಾಗಮ 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    ಪೂರ್ಣಾಙ್ಕ combined_gain = (ಪೂರ್ಣಾಙ್ಕ) overhead + (ಪೂರ್ಣಾಙ್ಕ) other_encoding.overhead;
    combined_gain -= (combined_width - (ಪೂರ್ಣಾಙ್ಕ) width) * ವಸ್ತುಗಳು.length;
    combined_gain -= (combined_width - (ಪೂರ್ಣಾಙ್ಕ) other_encoding.width) * other_encoding.ವಸ್ತುಗಳು.length;
    combined_gain -= get_chars_overhead (combined_columns);

    ಪ್ರತ್ಯಾಗಮ combined_gain;
  }

  ತಾರ್ಕಿಕ add_row (ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>* row)
  { ಪ್ರತ್ಯಾಗಮ ವಸ್ತುಗಳು.push (row); }

  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    ಧ್ರುವ delta_row_encoding_t *a = (ಧ್ರುವ delta_row_encoding_t *)pa;
    ಧ್ರುವ delta_row_encoding_t *b = (ಧ್ರುವ delta_row_encoding_t *)pb;

    ಯದಿ (a->width != b->width)
      ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) a->width - (ಪೂರ್ಣಾಙ್ಕ) b->width;

    ಪ್ರತ್ಯಾಗಮ b->chars.cmp (a->chars);
  }
};

ಸಂರಚನಾ VarRegionAxis
{
  ಭಿನ್ನ evaluate (ಪೂರ್ಣಾಙ್ಕ coord) ಧ್ರುವ
  {
    ಪೂರ್ಣಾಙ್ಕ peak = peakCoord.to_int ();
    ಯದಿ (peak == 0 || coord == peak)
      ಪ್ರತ್ಯಾಗಮ 1.f;
    ಅನ್ಯಥಾ ಯದಿ (coord == 0) // Faster
      ಪ್ರತ್ಯಾಗಮ 0.f;

    ಪೂರ್ಣಾಙ್ಕ start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    ಯದಿ (unlikely (start > peak || peak > end))
      ಪ್ರತ್ಯಾಗಮ 1.f;
    ಯದಿ (unlikely (start < 0 && end > 0))
      ಪ್ರತ್ಯಾಗಮ 1.f;

    ಯದಿ (coord <= start || end <= coord)
      ಪ್ರತ್ಯಾಗಮ 0.f;

    /* Interpolate */
    ಯದಿ (coord < peak)
      ಪ್ರತ್ಯಾಗಮ ಭಿನ್ನ (coord - start) / (peak - start);
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ ಭಿನ್ನ (end - coord) / (end - peak);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};
ಸಂರಚನಾ SparseVarRegionAxis
{
  ಭಿನ್ನ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len) ಧ್ರುವ
  {
    unsigned i = axisIndex;
    ಪೂರ್ಣಾಙ್ಕ coord = i < coord_len ? coords[i] : 0;
    ಪ್ರತ್ಯಾಗಮ axis.evaluate (coord);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ SparseVarRegionAxis *src,
		  ಧ್ರುವ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->embed (src);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (axis_map)
    {
      ಯದಿ (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (ಅಸತ್ಯ);
      }
      ಯದಿ (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (ಅಸತ್ಯ);
    }
    return_trace (ಸತ್ಯ);
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ hb_scalar_cache_t
{
  ಗುಪ್ತ:
  ಸ್ಥಿರ constexpr unsigned STATIC_LENGTH = 128;
  ಸ್ಥಿರ constexpr ಭಿನ್ನ INVALID = NAN;

  ಸಾರ್ವಜನಿಕ:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ಧ್ರುವ hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (ಧ್ರುವ hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  ಸ್ಥಿರ hb_scalar_cache_t *create (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    ಯದಿ (!ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    ಯದಿ (scratch_cache && ಎಣಿಕೆ <= STATIC_LENGTH)
    {
      scratch_cache->length = ಎಣಿಕೆ;
      scratch_cache->clear ();
      ಪ್ರತ್ಯಾಗಮ scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (ಪರಿಮಾಣ (hb_scalar_cache_t) - ಪರಿಮಾಣ (static_ಮೌಲ್ಯಗಳು) + ಪರಿಮಾಣ (static_ಮೌಲ್ಯಗಳು[0]) * ಎಣಿಕೆ);
    ಯದಿ (unlikely (!cache)) ಪ್ರತ್ಯಾಗಮ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = ಎಣಿಕೆ;
    cache->clear ();

    ಪ್ರತ್ಯಾಗಮ cache;
  }

  ಸ್ಥಿರ ಶೂನ್ಯ destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    ಯದಿ (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  ಶೂನ್ಯ clear ()
  {
    auto *values = &static_ಮೌಲ್ಯಗಳು[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    ಪ್ರತಿ (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    ಪ್ರತಿ (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  ತಾರ್ಕಿಕ get (unsigned i, ಭಿನ್ನ *value) ಧ್ರುವ
  {
    ಯದಿ (unlikely (i >= length))
    {
      *value = 0.f;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    auto *values = &static_ಮೌಲ್ಯಗಳು[0];
    auto *cached_ಮೌಲ್ಯ = &values[i];
    ಭಿನ್ನ v = *cached_ಮೌಲ್ಯ;
    ಯದಿ (std::isnan (v))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *value = v;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  HB_ALWAYS_INLINE
  ಶೂನ್ಯ set (unsigned i, ಭಿನ್ನ value)
  {
    ಯದಿ (unlikely (i >= length)) ಪ್ರತ್ಯಾಗಮ;
    auto *values = &static_ಮೌಲ್ಯಗಳು[0];
    auto *cached_ಮೌಲ್ಯ = &values[i];
    *cached_ಮೌಲ್ಯ = value;
  }

  ಸಾರ್ವಜನಿಕ:
  unsigned length;
  ಗುಪ್ತ:
  mutable hb_atomic_t<ಭಿನ್ನ> static_ಮೌಲ್ಯಗಳು[STATIC_LENGTH];
};

ಸಂರಚನಾ VarRegionList
{
  ಗುಪ್ತ:
  ಭಿನ್ನ evaluate_impl (unsigned ಪೂರ್ಣಾಙ್ಕ region_index,
		       ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len) ಧ್ರುವ
  {
    ಧ್ರುವ VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_ಎಣಿಕೆ_2);
    ಭಿನ್ನ v = 1.f;

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = axis_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ coord = i < coord_len ? coords[i] : 0;
      ಭಿನ್ನ factor = axes[i].evaluate (coord);
      ಯದಿ (factor == 0.f)
      {
	v = 0.f;
	ಭಙ್ಗ;
      }
      v *= factor;
    }

    ಪ್ರತ್ಯಾಗಮ v;
  }

  ಸಾರ್ವಜನಿಕ:
  HB_ALWAYS_INLINE
  ಭಿನ್ನ evaluate (unsigned ಪೂರ್ಣಾಙ್ಕ region_index,
		  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		  hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    ಯದಿ (unlikely (region_index >= region_ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ 0.;

    ಭಿನ್ನ v;
    ಯದಿ (cache && cache->get (region_index, &v))
      ಪ್ರತ್ಯಾಗಮ v;

    v = evaluate_impl (region_index, coords, coord_len);

    ಯದಿ (cache)
      cache->set (region_index, v);
    ಪ್ರತ್ಯಾಗಮ v;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_ಎಣಿಕೆ_2 * region_ಎಣಿಕೆ));
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  ಧ್ರುವ hb_vector_t<hb_tag_t>& axis_tags,
                  ಧ್ರುವ hb_vector_t<ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned axis_ಎಣಿಕೆ = axis_tags.length;
    unsigned region_ಎಣಿಕೆ_2 = regions.length;
    ಯದಿ (!axis_ಎಣಿಕೆ || !region_ಎಣಿಕೆ_2) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (axis_ಎಣಿಕೆ * region_ಎಣಿಕೆ_2,
                                             VarRegionAxis::static_size))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    axis_ಎಣಿಕೆ_2 = axis_ಎಣಿಕೆ;
    region_ಎಣಿಕೆ = region_ಎಣಿಕೆ_2;

    ಪ್ರತಿ (unsigned r = 0; r < region_ಎಣಿಕೆ_2; r++)
    {
      ಧ್ರುವ auto& region = regions[r];
      ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        ಯದಿ (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->ಮಧ್ಯ);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        ಅನ್ಯಥಾ
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        ಯದಿ (!var_region_rec.serialize (c))
          return_trace (ಅಸತ್ಯ);
      }
    }
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ VarRegionList *src, ಧ್ರುವ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    axis_ಎಣಿಕೆ_2 = src->axis_ಎಣಿಕೆ_2;
    region_ಎಣಿಕೆ = region_map.get_population ();
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (axis_ಎಣಿಕೆ_2 * region_ಎಣಿಕೆ,
					     VarRegionAxis::static_size))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->extend (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    unsigned ಪೂರ್ಣಾಙ್ಕ region_ಎಣಿಕೆ_2 = src->region_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ r = 0; r < region_ಎಣಿಕೆ; r++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ backward = region_map.backward (r);
      ಯದಿ (backward >= region_ಎಣಿಕೆ_2) return_trace (ಅಸತ್ಯ);
      hb_memcpy (&axesZ[axis_ಎಣಿಕೆ_2 * r], &src->axesZ[axis_ಎಣಿಕೆ_2 * backward], VarRegionAxis::static_size * axis_ಎಣಿಕೆ_2);
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ get_var_region (unsigned region_index,
                       ಧ್ರುವ hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (region_index >= region_ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_ಎಣಿಕೆ_2);
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      hb_tag_t *axis_tag;
      ಯದಿ (!axes_old_index_tag_map.has (i, &axis_tag))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಭಿನ್ನ min_val = axis_region->startCoord.to_float ();
      ಭಿನ್ನ def_val = axis_region->peakCoord.to_float ();
      ಭಿನ್ನ max_val = axis_region->endCoord.to_float ();

      ಯದಿ (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((ದ್ವಿಗುಣ) min_val, (ದ್ವಿಗುಣ) def_val, (ದ್ವಿಗುಣ) max_val));
      axis_region++;
    }
    ಪ್ರತ್ಯಾಗಮ !axis_tuples.in_error ();
  }

  ತಾರ್ಕಿಕ get_var_regions (ಧ್ರುವ hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (!regions.alloc (region_ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned i = 0; i < region_ಎಣಿಕೆ; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      ಯದಿ (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      regions.push (std::move (axis_tuples));
    }
    ಪ್ರತ್ಯಾಗಮ !regions.in_error ();
  }

  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_ಎಣಿಕೆ_2, region_ಎಣಿಕೆ)); }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16	axis_ಎಣಿಕೆ_2;
  HBUINT15	region_ಎಣಿಕೆ;
  ರಕ್ಷಿತ:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
_hb_avar2_region_is_dead (ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  ಪ್ರತಿ (auto _ : axis_tuples)
  {
    Triple *range;
    ಯದಿ (!reachable_ranges.has (_.first, &range)) ಅನುವರ್ತ;
    ಧ್ರುವ Triple &tent = _.second;
    ಪೂರ್ಣಾಙ್ಕ start = (ಪೂರ್ಣಾಙ್ಕ) roundf ((ಭಿನ್ನ) tent.minimum * 16384.f);
    ಪೂರ್ಣಾಙ್ಕ peak  = (ಪೂರ್ಣಾಙ್ಕ) roundf ((ಭಿನ್ನ) tent.ಮಧ್ಯ * 16384.f);
    ಪೂರ್ಣಾಙ್ಕ end   = (ಪೂರ್ಣಾಙ್ಕ) roundf ((ಭಿನ್ನ) tent.maximum * 16384.f);
    ಯದಿ (!peak) ಅನುವರ್ತ;
    ಯದಿ (start > peak || peak > end) ಅನುವರ್ತ;
    ಯದಿ (start < 0 && end > 0) ಅನುವರ್ತ;
    ಪೂರ್ಣಾಙ್ಕ lo = (ಪೂರ್ಣಾಙ್ಕ) roundf ((ಭಿನ್ನ) range->minimum * 16384.f);
    ಪೂರ್ಣಾಙ್ಕ hi = (ಪೂರ್ಣಾಙ್ಕ) roundf ((ಭಿನ್ನ) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    ಯದಿ (hi < start || lo > end)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸಂರಚನಾ SparseVariationRegion
{
  ಭಿನ್ನ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len) ಧ್ರುವ
  {
    ಭಿನ್ನ v = 1.f;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = axes.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಭಿನ್ನ factor = (ಏತತ್+axes[i]).evaluate (coords, coord_len);
      ಯದಿ (factor == 0.f)
	ಪ್ರತ್ಯಾಗಮ 0.;
      v *= factor;
    }
    ಪ್ರತ್ಯಾಗಮ v;
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ SparseVariationRegion *src,
		  ಧ್ರುವ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned i = 0; i < axes.len; i++)
      ಯದಿ (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (axes.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, axes);
};

ಸಂರಚನಾ SparseVarRegionList
{
  HB_ALWAYS_INLINE
  ಭಿನ್ನ evaluate (unsigned ಪೂರ್ಣಾಙ್ಕ region_index,
		  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		  hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    ಯದಿ (unlikely (region_index >= regions.len))
      ಪ್ರತ್ಯಾಗಮ 0.;

    ಭಿನ್ನ v;
    ಯದಿ (cache && cache->get (region_index, &v))
      ಪ್ರತ್ಯಾಗಮ v;

    ಧ್ರುವ SparseVariationRegion &region = ಏತತ್+regions[region_index];

    v = region.evaluate (coords, coord_len);
    ಯದಿ (cache)
      cache->set (region_index, v);

    ಪ್ರತ್ಯಾಗಮ v;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (regions.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ SparseVarRegionList *src,
		  ಧ್ರುವ hb_inc_bimap_t &region_map,
		  ಧ್ರುವ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    regions.len = region_map.get_population ();
    ಯದಿ (unlikely (!c->extend (regions))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      ಯದಿ (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (ಅಸತ್ಯ);
    }
    return_trace (ಸತ್ಯ);
  }

  ಸಾರ್ವಜನಿಕ:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, regions);
};


ಸಂರಚನಾ VarData
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_item_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ item_ಎಣಿಕೆ; }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_region_index_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ regionIndices.len; }

  unsigned get_region_index (unsigned i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (word_ಎಣಿಕೆ () + regionIndices.len) * (long_ಪದಗಳು_2 () ? 2 : 1); }

  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_ಎಣಿಕೆ, get_row_size ())); }

  template <typename Float = ಭಿನ್ನ>
  Float _get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ inner,
		    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
		    ಧ್ರುವ VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    ಯದಿ (unlikely (inner >= item_ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ 0.;
    ತಾರ್ಕಿಕ is_long = long_ಪದಗಳು_2 ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = regionIndices.len;
    unsigned word_ಎಣಿಕೆ_2 = word_ಎಣಿಕೆ ();
    unsigned ಪೂರ್ಣಾಙ್ಕ scount = is_long ? ಎಣಿಕೆ : word_ಎಣಿಕೆ_2;
    unsigned ಪೂರ್ಣಾಙ್ಕ lcount = is_long ? word_ಎಣಿಕೆ_2 : 0;

    ಧ್ರುವ HBUINT8 *bytes = get_delta_bytes ();
    ಧ್ರುವ HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;

    ಧ್ರುವ HBINT32 *lcursor = reinterpret_cast<ಧ್ರುವ HBINT32 *> (row);
    ಪ್ರತಿ (; i < lcount; i++)
    {
      ಭಿನ್ನ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ಎಣಿಕೆ, cache);
      ಯದಿ (scalar)
        delta += (Float) scalar * (ಪೂರ್ಣಾಙ್ಕ) *lcursor;
      lcursor++;
    }
    ಧ್ರುವ HBINT16 *scursor = reinterpret_cast<ಧ್ರುವ HBINT16 *> (lcursor);
    ಪ್ರತಿ (; i < scount; i++)
    {
      ಭಿನ್ನ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ಎಣಿಕೆ, cache);
      ಯದಿ (scalar)
       delta += (Float) scalar * (ಪೂರ್ಣಾಙ್ಕ) *scursor;
      scursor++;
    }
    ಧ್ರುವ HBINT8 *bcursor = reinterpret_cast<ಧ್ರುವ HBINT8 *> (scursor);
    ಪ್ರತಿ (; i < ಎಣಿಕೆ; i++)
    {
      ಭಿನ್ನ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ಎಣಿಕೆ, cache);
      ಯದಿ (scalar)
        delta += (Float) scalar * (ಪೂರ್ಣಾಙ್ಕ) *bcursor;
      bcursor++;
    }

    ಪ್ರತ್ಯಾಗಮ delta;
  }

  template <typename Float = ಭಿನ್ನ>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ inner,
		   ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
		   ಧ್ರುವ VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = regionIndices.len;
    ಯದಿ (!ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ Float (0); // This is quite common, so optimize it.
    ಪ್ರತ್ಯಾಗಮ _get_delta<Float> (inner, coords, coord_ಎಣಿಕೆ, regions, cache);
  }

  ಶೂನ್ಯ get_region_scalars (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
			   ಧ್ರುವ VarRegionList &regions,
			   ಭಿನ್ನ *scalars /*OUT */,
			   unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_scalars) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = hb_min (ಅಂಕೆ_scalars, regionIndices.len);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ಎಣಿಕೆ);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = ಎಣಿಕೆ; i < ಅಂಕೆ_scalars; i++)
      scalars[i] = 0.f;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_ಎಣಿಕೆ () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_ಎಣಿಕೆ,
				  get_row_size ()));
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  ತಾರ್ಕಿಕ has_long,
                  ಧ್ರುವ hb_vector_t<ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>*>& rows)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned row_ಎಣಿಕೆ_2 = rows.length;
    ಯದಿ (!row_ಎಣಿಕೆ_2) {
      // Nothing to serialize, will be empty.
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->check_assign (item_ಎಣಿಕೆ, row_ಎಣಿಕೆ_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ಅಸತ್ಯ);

    ಪೂರ್ಣಾಙ್ಕ min_threshold = has_long ? -32768 : -128;
    ಪೂರ್ಣಾಙ್ಕ max_threshold = has_long ? +32767 : +127;
    ಗಣನಾ delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned ಅಂಕೆ_regions = rows[0]->length;
    ಯದಿ (!delta_sz.resize (ಅಂಕೆ_regions))
      return_trace (ಅಸತ್ಯ);

    unsigned word_ಎಣಿಕೆ_2 = 0;
    ಪ್ರತಿ (unsigned r = 0; r < ಅಂಕೆ_regions; r++)
    {
      ಪ್ರತಿ (unsigned i = 0; i < row_ಎಣಿಕೆ_2; i++)
      {
        ಪೂರ್ಣಾಙ್ಕ delta = rows[i]->arrayZ[r];
        ಯದಿ (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_ಎಣಿಕೆ_2++;
          ಭಙ್ಗ;
        }
        ಅನ್ಯಥಾ ಯದಿ (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_ಎಣಿಕೆ_2;
    hb_map_t ri_map;
    ಪ್ರತಿ (unsigned r = 0; r < ಅಂಕೆ_regions; r++)
    {
      ಯದಿ (!delta_sz[r]) ಅನುವರ್ತ;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      ಯದಿ (!ri_map.set (new_r, r))
        return_trace (ಅಸತ್ಯ);
    }

    word_Size_ಎಣಿಕೆ = word_ಎಣಿಕೆ_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_ಎಣಿಕೆ = ri_map.get_population ();
    regionIndices.len = ri_ಎಣಿಕೆ;
    ಯದಿ (unlikely (!c->extend (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (unsigned r = 0; r < ri_ಎಣಿಕೆ; r++)
    {
      hb_codepoint_t *idx;
      ಯದಿ (!ri_map.has (r, &idx))
        return_trace (ಅಸತ್ಯ);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < row_ಎಣಿಕೆ_2; i++)
    {
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ r = 0; r < ri_ಎಣಿಕೆ; r++)
      {
        ಪೂರ್ಣಾಙ್ಕ delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ VarData *src,
		  ಧ್ರುವ hb_inc_bimap_t &inner_map,
		  ಧ್ರುವ hb_inc_bimap_t &region_map,
		  ಧ್ರುವ hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    item_ಎಣಿಕೆ = inner_map.get_next_ಮೌಲ್ಯ ();

    /* Optimize word count */
    unsigned ri_ಎಣಿಕೆ = src->regionIndices.len;
    ಗಣನಾ delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_ಎಣಿಕೆ);
    ri_map.resize (ri_ಎಣಿಕೆ);
    unsigned ಪೂರ್ಣಾಙ್ಕ new_word_ಎಣಿಕೆ = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ r;

    ಧ್ರುವ HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_ಎಣಿಕೆ = src->word_ಎಣಿಕೆ ();
    ತಾರ್ಕಿಕ     src_long_ಪದಗಳು = src->long_ಪದಗಳು_2 ();

    ತಾರ್ಕಿಕ has_long = ಅಸತ್ಯ;
    ಯದಿ (src_long_ಪದಗಳು)
    {
      ಪ್ರತಿ (r = 0; r < src_word_ಎಣಿಕೆ; r++)
      {
        ಪ್ರತಿ (unsigned old_gid : inner_map.ಕೀಲಿಗಳು())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  ಯದಿ (delta < -32768 || 32767 < delta)
	  {
	    has_long = ಸತ್ಯ;
	    ಭಙ್ಗ;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    ಪ್ರತಿ (r = 0; r < ri_ಎಣಿಕೆ; r++)
    {
      ತಾರ್ಕಿಕ short_circuit = src_long_ಪದಗಳು == has_long && src_word_ಎಣಿಕೆ <= r;

      delta_sz[r] = kZero;
      ಯದಿ (culled_regions && culled_regions->has (src->regionIndices[r]))
	ಅನುವರ್ತ; /* unreachable region: drop its column */
      ಪ್ರತಿ (unsigned old_gid : inner_map.ಕೀಲಿಗಳು())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	ಯದಿ (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_ಎಣಿಕೆ++;
	  ಭಙ್ಗ;
	}
	ಅನ್ಯಥಾ ಯದಿ (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  ಯದಿ (short_circuit)
	    ಭಙ್ಗ;
	}
      }
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ word_index = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ non_word_index = new_word_ಎಣಿಕೆ;
    unsigned ಪೂರ್ಣಾಙ್ಕ new_ri_ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (r = 0; r < ri_ಎಣಿಕೆ; r++)
      ಯದಿ (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_ಎಣಿಕೆ++;
      }

    word_Size_ಎಣಿಕೆ = new_word_ಎಣಿಕೆ | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_ಎಣಿಕೆ;

    ಯದಿ (unlikely (!c->extend (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (r = 0; r < new_ri_ಎಣಿಕೆ; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned ಎಣಿಕೆ = item_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ old = inner_map.backward (i);
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ r = 0; r < new_ri_ಎಣಿಕೆ; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_region_refs (hb_set_t &region_indices, ಧ್ರುವ hb_inc_bimap_t &inner_map) ಧ್ರುವ
  {
    ಧ್ರುವ HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ r = 0; r < regionIndices.len; r++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ region = regionIndices.arrayZ[r];
      ಯದಿ (region_indices.has (region)) ಅನುವರ್ತ;
      ಪ್ರತಿ (hb_codepoint_t old_gid : inner_map.ಕೀಲಿಗಳು())
	ಯದಿ (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  ಭಙ್ಗ;
	}
    }
  }

  ಸಾರ್ವಜನಿಕ:
  ಧ್ರುವ HBUINT8 *get_delta_bytes () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ &StructAfter<HBUINT8> (regionIndices); }

  ರಕ್ಷಿತ:
  HBUINT8 *get_delta_bytes ()
  { ಪ್ರತ್ಯಾಗಮ &StructAfter<HBUINT8> (regionIndices); }

  ಸಾರ್ವಜನಿಕ:
  int32_t get_item_delta_fast (unsigned ಪೂರ್ಣಾಙ್ಕ item, unsigned ಪೂರ್ಣಾಙ್ಕ region,
			       ಧ್ರುವ HBUINT8 *delta_bytes, unsigned row_size) ಧ್ರುವ
  {
    ಯದಿ (unlikely (item >= item_ಎಣಿಕೆ || region >= regionIndices.len)) ಪ್ರತ್ಯಾಗಮ 0;

    ಧ್ರುವ HBINT8 *p = (ಧ್ರುವ HBINT8 *) delta_bytes + item * row_size;
    unsigned word_ಎಣಿಕೆ_2 = word_ಎಣಿಕೆ ();
    ತಾರ್ಕಿಕ is_long = long_ಪದಗಳು_2 ();
    ಯದಿ (is_long)
    {
      ಯದಿ (region < word_ಎಣಿಕೆ_2)
	ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ HBINT32 *) p)[region];
      ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ HBINT16 *)(p + HBINT32::static_size * word_ಎಣಿಕೆ_2))[region - word_ಎಣಿಕೆ_2];
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (region < word_ಎಣಿಕೆ_2)
	ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ HBINT16 *) p)[region];
      ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ (p + HBINT16::static_size * word_ಎಣಿಕೆ_2)[region - word_ಎಣಿಕೆ_2];
    }
  }
  int32_t get_item_delta (unsigned ಪೂರ್ಣಾಙ್ಕ item, unsigned ಪೂರ್ಣಾಙ್ಕ region) ಧ್ರುವ
  {
     ಪ್ರತ್ಯಾಗಮ get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  ರಕ್ಷಿತ:
  ಶೂನ್ಯ set_item_delta_fast (unsigned ಪೂರ್ಣಾಙ್ಕ item, unsigned ಪೂರ್ಣಾಙ್ಕ region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_ಎಣಿಕೆ_2 = word_ಎಣಿಕೆ ();
    ತಾರ್ಕಿಕ is_long = long_ಪದಗಳು_2 ();
    ಯದಿ (is_long)
    {
      ಯದಿ (region < word_ಎಣಿಕೆ_2)
	((HBINT32 *) p)[region] = delta;
      ಅನ್ಯಥಾ
	((HBINT16 *)(p + HBINT32::static_size * word_ಎಣಿಕೆ_2))[region - word_ಎಣಿಕೆ_2] = delta;
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (region < word_ಎಣಿಕೆ_2)
	((HBINT16 *) p)[region] = delta;
      ಅನ್ಯಥಾ
	(p + HBINT16::static_size * word_ಎಣಿಕೆ_2)[region - word_ಎಣಿಕೆ_2] = delta;
    }
  }
  ಶೂನ್ಯ set_item_delta (unsigned ಪೂರ್ಣಾಙ್ಕ item, unsigned ಪೂರ್ಣಾಙ್ಕ region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  ತಾರ್ಕಿಕ long_ಪದಗಳು_2 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ word_Size_ಎಣಿಕೆ & 0x8000u /* LONG_WORDS */; }
  unsigned word_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ word_Size_ಎಣಿಕೆ & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  ರಕ್ಷಿತ:
  HBUINT16		item_ಎಣಿಕೆ;
  HBUINT16		word_Size_ಎಣಿಕೆ;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

ಸಂರಚನಾ MultiVarData
{
  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  ಧ್ರುವ TupleList &get_delta_sets () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  ಶೂನ್ಯ get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ inner,
		  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
		  ಧ್ರುವ SparseVarRegionList &regions,
		  hb_array_t<ದ್ವಿಗುಣ> out,
		  hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    auto &deltaSets = get_delta_sets ();

    auto ಮೌಲ್ಯಗಳು_iter = deltaSets.fetcher (inner);
    unsigned region_ಎಣಿಕೆ = regionIndices.len;
    unsigned skip = 0;
    ಪ್ರತಿ (unsigned regionIndex = 0; regionIndex < region_ಎಣಿಕೆ; regionIndex++)
    {
      ಭಿನ್ನ scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_ಎಣಿಕೆ,
				       cache);
      // We skip lazily. Helps with the tail end.
      ಯದಿ (scalar == 0.0f)
        skip += out.length;
      ಅನ್ಯಥಾ
      {
        ಯದಿ (skip)
	{
	  ಮೌಲ್ಯಗಳು_iter.skip (skip);
	  skip = 0;
	}
	ಮೌಲ್ಯಗಳು_iter.add_to (out, scalar);
      }
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, ಏತತ್));
  }

  unsigned get_item_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_delta_sets ().ಎಣಿಕೆ; }

  ಶೂನ್ಯ collect_region_refs (hb_set_t &region_indices) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned region : regionIndices)
      region_indices.add (region);
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ MultiVarData *src,
		  ಧ್ರುವ hb_inc_bimap_t &inner_map,
		  ಧ್ರುವ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned region_ಎಣಿಕೆ_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_ಎಣಿಕೆ_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    ಯದಿ (unlikely (!c->extend_size (ಏತತ್, header_size, ಅಸತ್ಯ)))
      return_trace (ಅಸತ್ಯ);
    format = 1;
    regionIndices.len = region_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      ಯದಿ (unlikely (!region_map.has (old_region)))
	return_trace (ಅಸತ್ಯ);
      regionIndices[i] = region_map.get (old_region);
    }

    ಧ್ರುವ auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    ಯದಿ (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      ಯದಿ (unlikely (old_inner >= src_delta_sets.ಎಣಿಕೆ)) return_trace (ಅಸತ್ಯ);
      hb_ubytes_t bytes = static_cast<ಧ್ರುವ CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    ಯದಿ (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (ಅಸತ್ಯ);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  ರಕ್ಷಿತ:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (7);
};

ಸಂರಚನಾ ItemVariationStore
{
  friend ಸಂರಚನಾ item_variations_t;

  hb_scalar_cache_t *create_cache () ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ hb_scalar_cache_t::create (0);
#endif
    ಪ್ರತ್ಯಾಗಮ hb_scalar_cache_t::create ((ಏತತ್+regions).region_ಎಣಿಕೆ);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  ಗುಪ್ತ:
  template <typename Float = ಭಿನ್ನ>
  Float get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ outer, unsigned ಪೂರ್ಣಾಙ್ಕ inner,
		   ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
		   hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ Float (0);
#endif

    ಯದಿ (unlikely (outer >= dataSets.len))
      ಪ್ರತ್ಯಾಗಮ Float (0);

    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_ಎಣಿಕೆ,
					     ಏತತ್+regions,
					     cache);
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ has_delta_set (unsigned ಪೂರ್ಣಾಙ್ಕ index) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif

    unsigned ಪೂರ್ಣಾಙ್ಕ outer = index >> 16;
    unsigned ಪೂರ್ಣಾಙ್ಕ inner = index & 0xFFFF;
    ಪ್ರತ್ಯಾಗಮ outer < dataSets.len &&
	   inner < (ಏತತ್+dataSets[outer]).get_item_ಎಣಿಕೆ ();
  }

  template <typename Float = ಭಿನ್ನ>
  Float get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ index,
		   ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
		   hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ outer = index >> 16;
    unsigned ಪೂರ್ಣಾಙ್ಕ inner = index & 0xFFFF;
    ಪ್ರತ್ಯಾಗಮ get_delta<Float> (outer, inner, coords, coord_ಎಣಿಕೆ, cache);
  }
  template <typename Float = ಭಿನ್ನ>
  Float get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ index,
		   hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
		   hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif

    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ಏತತ್) &&
		  dataSets.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  ತಾರ್ಕಿಕ has_long,
                  ಧ್ರುವ hb_vector_t<hb_tag_t>& axis_tags,
                  ಧ್ರುವ hb_vector_t<ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  ಧ್ರುವ hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (ಏತತ್);
#ifdef HB_NO_VAR
    return_trace (ಅಸತ್ಯ);
#endif
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    format = 1;
    ಯದಿ (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (ಅಸತ್ಯ);

    unsigned ಅಂಕೆ_var_data = vardata_encodings.length;
    ಯದಿ (!ಅಂಕೆ_var_data) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->check_assign (dataSets.len, ಅಂಕೆ_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!c->extend (dataSets))) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_var_data; i++)
      ಯದಿ (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].ವಸ್ತುಗಳು))
        return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ ItemVariationStore *src,
		  ಧ್ರುವ hb_array_t <ಧ್ರುವ hb_inc_bimap_t> &inner_maps,
		  ಧ್ರುವ hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (ಏತತ್);
#ifdef HB_NO_VAR
    return_trace (ಅಸತ್ಯ);
#endif

    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    unsigned ಪೂರ್ಣಾಙ್ಕ set_ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < inner_maps.length; i++)
      ಯದಿ (inner_maps[i].get_population ())
	set_ಎಣಿಕೆ++;

    format = 1;

    ಧ್ರುವ auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    ಯದಿ (region_indices.in_error ())
      return_trace (ಅಸತ್ಯ);

    region_indices.del_range ((src_regions).region_ಎಣಿಕೆ, hb_set_t::INVALID);

    ಯದಿ (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    ಯದಿ (region_map.in_error())
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (ಅಸತ್ಯ);

    dataSets.len = set_ಎಣಿಕೆ;
    ಯದಿ (unlikely (!c->extend (dataSets))) return_trace (ಅಸತ್ಯ);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned ಪೂರ್ಣಾಙ್ಕ set_index = 0;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < inner_maps.length; i++)
    {
      ಯದಿ (!inner_maps[i].get_population ()) ಅನುವರ್ತ;
      ಯದಿ (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned ಎಣಿಕೆ = dataSets.len;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      ಯದಿ (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = ಏತತ್+dataSets[i];

      unsigned item_ಎಣಿಕೆ = data.get_item_ಎಣಿಕೆ ();
      ಪ್ರತಿ (unsigned j = 0; j < item_ಎಣಿಕೆ; j++)
	map->add (j);
    }

    ಯದಿ (unlikely (!out->serialize (c, ಏತತ್, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  ತಾರ್ಕಿಕ collect_dead_regions (ಧ್ರುವ hb_map_t &axes_old_index_tag_map,
			     ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) ಧ್ರುವ
  {
#ifndef HB_NO_VAR
    ಯದಿ (!reachable_ranges.get_population ()) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಧ್ರುವ VarRegionList &region_list = ಏತತ್+regions;
    unsigned ಎಣಿಕೆ = region_list.region_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      ಯದಿ (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    ಪ್ರತ್ಯಾಗಮ !dead_regions.in_error ();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c, ಧ್ರುವ hb_array_t<ಧ್ರುವ hb_inc_bimap_t> &inner_maps) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
#ifdef HB_NO_VAR
    return_trace (ಅಸತ್ಯ);
#endif

    ItemVariationStore *varstore_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed<ItemVariationStore> ();
    ಯದಿ (unlikely (!varstore_ಅವಿಭಾಜ್ಯ)) return_trace (ಅಸತ್ಯ);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    ಯದಿ (c->plan->has_avar2)
      ಯದಿ (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (ಅಸತ್ಯ);

    varstore_ಅವಿಭಾಜ್ಯ->serialize (c->serializer, ಏತತ್, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_ಅವಿಭಾಜ್ಯ->dataSets);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_region_index_ಎಣಿಕೆ (unsigned ಪೂರ್ಣಾಙ್ಕ major) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ 0;
#endif
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+dataSets[major]).get_region_index_ಎಣಿಕೆ ();
  }

  ಶೂನ್ಯ get_region_scalars (unsigned ಪೂರ್ಣಾಙ್ಕ major,
			   ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
			   ಭಿನ್ನ *scalars /*OUT*/,
			   unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_scalars) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_scalars; i++)
      scalars[i] = 0.f;
    ಪ್ರತ್ಯಾಗಮ;
#endif

    (ಏತತ್+dataSets[major]).get_region_scalars (coords, coord_ಎಣಿಕೆ,
					       ಏತತ್+regions,
					       &scalars[0], ಅಂಕೆ_scalars);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_sub_table_ಎಣಿಕೆ () ಧ್ರುವ
   {
#ifdef HB_NO_VAR
     ಪ್ರತ್ಯಾಗಮ 0;
#endif
     ಪ್ರತ್ಯಾಗಮ dataSets.len;
   }

  ಧ್ರುವ VarData& get_sub_table (unsigned i) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
     ಪ್ರತ್ಯಾಗಮ Null (VarData);
#endif
     ಪ್ರತ್ಯಾಗಮ ಏತತ್+dataSets[i];
  }

  ಧ್ರುವ VarRegionList& get_region_list () ಧ್ರುವ
  {
#ifdef HB_NO_VAR
     ಪ್ರತ್ಯಾಗಮ Null (VarRegionList);
#endif
     ಪ್ರತ್ಯಾಗಮ ಏತತ್+regions;
  }

  ರಕ್ಷಿತ:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

ಸಂರಚನಾ MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ hb_scalar_cache_t::create (0);
#endif
    auto &r = ಏತತ್+regions;
    unsigned ಎಣಿಕೆ = r.regions.len;

    ಪ್ರತ್ಯಾಗಮ hb_scalar_cache_t::create (ಎಣಿಕೆ, static_cache);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  ಗುಪ್ತ:
  ಶೂನ್ಯ get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ outer, unsigned ಪೂರ್ಣಾಙ್ಕ inner,
		  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
		  hb_array_t<ದ್ವಿಗುಣ> out,
		  hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ;
#endif

    ಯದಿ (unlikely (outer >= dataSets.len))
      ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+dataSets[outer]).get_delta (inner,
					     coords, coord_ಎಣಿಕೆ,
					     ಏತತ್+regions,
					     out,
					     cache);
  }

  ಸಾರ್ವಜನಿಕ:
  ಶೂನ್ಯ get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ index,
		  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
		  hb_array_t<ದ್ವಿಗುಣ> out,
		  hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ outer = index >> 16;
    unsigned ಪೂರ್ಣಾಙ್ಕ inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_ಎಣಿಕೆ, out, cache);
  }
  ಶೂನ್ಯ get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ index,
		  hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
		  hb_array_t<ದ್ವಿಗುಣ> out,
		  hb_scalar_cache_t *cache = nullptr) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
#ifdef HB_NO_VAR
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif

    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ಏತತ್) &&
		  dataSets.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ create_subset_plan (ಧ್ರುವ hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!inner_maps->resize (dataSets.len))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      ಯದಿ (unlikely (outer >= dataSets.len ||
		    inner >= (ಏತತ್+dataSets[outer]).get_item_ಎಣಿಕೆ ()))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    ಪ್ರತಿ (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      ಧ್ರುವ hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      ಯದಿ (!inner_map.get_population ()) ಅನುವರ್ತ;
      ಯದಿ (unlikely (inner_map.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತಿ (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    ಪ್ರತ್ಯಾಗಮ !inner_maps->in_error () && !varidx_map->in_error ();
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ MultiItemVariationStore *src,
		  ಧ್ರುವ hb_array_t<ಧ್ರುವ hb_inc_bimap_t> &inner_maps,
		  ಧ್ರುವ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (ಅಸತ್ಯ);
    format = 1;

    ಧ್ರುವ auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (unsigned i = 0; i < src->dataSets.len; i++)
    {
      ಯದಿ (!inner_maps[i].get_population ()) ಅನುವರ್ತ;
      set_ಎಣಿಕೆ++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    ಯದಿ (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (ಅಸತ್ಯ);

    dataSets.len = set_ಎಣಿಕೆ;
    ಯದಿ (unlikely (!c->extend (dataSets))) return_trace (ಅಸತ್ಯ);
    unsigned set_index = 0;
    ಪ್ರತಿ (unsigned i = 0; i < src->dataSets.len; i++)
    {
      ಯದಿ (!inner_maps[i].get_population ()) ಅನುವರ್ತ;
      ಯದಿ (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (ಅಸತ್ಯ);
    }
    return_trace (ಸತ್ಯ);
  }

  ರಕ್ಷಿತ:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_ಎಣಿಕೆ_T>
ಸಂರಚನಾ DeltaSetIndexMapFormat01
{
  friend ಸಂರಚನಾ DeltaSetIndexMap;

  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (map_ಎಣಿಕೆ_2, get_width (), min_size); }

  ಗುಪ್ತ:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed (ಏತತ್));
  }

  template <typename T>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ T &plan)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ width = plan.get_width ();
    unsigned ಪೂರ್ಣಾಙ್ಕ inner_bit_ಎಣಿಕೆ = plan.get_inner_bit_ಎಣಿಕೆ ();
    ಧ್ರುವ hb_array_t<ಧ್ರುವ uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (output_map.length && ((((inner_bit_ಎಣಿಕೆ-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    entryFormat = ((width-1)<<4)|(inner_bit_ಎಣಿಕೆ-1);
    map_ಎಣಿಕೆ_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    ಯದಿ (unlikely (!p)) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < output_map.length; i++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ v = output_map.arrayZ[i];
      ಯದಿ (v)
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ outer = v >> 16;
	unsigned ಪೂರ್ಣಾಙ್ಕ inner = v & 0xFFFF;
	unsigned ಪೂರ್ಣಾಙ್ಕ u = (outer << inner_bit_ಎಣಿಕೆ) | inner;
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (ಸತ್ಯ);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned ಪೂರ್ಣಾಙ್ಕ v) ಧ್ರುವ /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    ಯದಿ (!map_ಎಣಿಕೆ_2)
      ಪ್ರತ್ಯಾಗಮ v;
    ಪ್ರತ್ಯಾಗಮ _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned ಪೂರ್ಣಾಙ್ಕ v) ಧ್ರುವ /* Returns 16.16 outer.inner. */
  {
    ಯದಿ (v >= map_ಎಣಿಕೆ_2)
      v = map_ಎಣಿಕೆ_2 - 1;

    unsigned ಪೂರ್ಣಾಙ್ಕ u = 0;
    { /* Fetch it. */
      unsigned ಪೂರ್ಣಾಙ್ಕ w = get_width ();
      ಧ್ರುವ HBUINT8 *p = mapDataZ.arrayZ + w * v;
      ಪ್ರತಿ (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned ಪೂರ್ಣಾಙ್ಕ n = get_inner_bit_ಎಣಿಕೆ ();
      unsigned ಪೂರ್ಣಾಙ್ಕ outer = u >> n;
      unsigned ಪೂರ್ಣಾಙ್ಕ inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    ಪ್ರತ್ಯಾಗಮ u;
  }

  unsigned get_map_ಎಣಿಕೆ () ಧ್ರುವ       { ಪ್ರತ್ಯಾಗಮ map_ಎಣಿಕೆ_2; }
  unsigned get_width () ಧ್ರುವ           { ಪ್ರತ್ಯಾಗಮ ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (entryFormat & 0xF) + 1; }


  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_ಎಣಿಕೆ_2,
                                  get_width ()));
  }

  ರಕ್ಷಿತ:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_ಎಣಿಕೆ_T     map_ಎಣಿಕೆ_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2+Map_ಎಣಿಕೆ_T::static_size, mapDataZ);
};

ಸಂರಚನಾ DeltaSetIndexMap
{
  template <typename T>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ T &plan)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  uint32_t map (unsigned v) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ (u.format0.map (v));
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ (u.format1.map (v));
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ v;
    }
  }

  unsigned get_map_ಎಣಿಕೆ () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_map_ಎಣಿಕೆ ();
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_map_ಎಣಿಕೆ ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  unsigned get_width () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_width ();
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_width ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  unsigned get_inner_bit_ಎಣಿಕೆ () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_inner_bit_ಎಣಿಕೆ ();
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_inner_bit_ಎಣಿಕೆ ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    ಪಕ್ಷ 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    ಸ್ವಭಾವ:return_trace (nullptr);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (1, format.v);
};


ಸಂರಚನಾ ItemVarStoreInstancer
{
  ItemVarStoreInstancer (ಧ್ರುವ ItemVariationStore *varStore_,
			 ಧ್ರುವ DeltaSetIndexMap *varIdxMap,
			 hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    ಯದಿ (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    ಯದಿ (!hb_any (coords))
      ಏತತ್->coords = {};
  }

  operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ varStore && ತಾರ್ಕಿಕ (coords); }

  ಭಿನ್ನ operator[] (uint32_t varIdx) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್) (varIdx); }

  ಭಿನ್ನ operator() (uint32_t varIdx, unsigned short offset = 0) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_delta (varIdx, offset); }

  ದ್ವಿಗುಣ get_condition_delta (uint32_t varIdx) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_delta<ದ್ವಿಗುಣ> (varIdx); }

  template <typename Float = ಭಿನ್ನ>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) ಧ್ರುವ
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   ಯದಿ (!coords || varIdx == VarIdx::NO_VARIATION)
     ಪ್ರತ್ಯಾಗಮ Float (0);

    varIdx += offset;
    ಯದಿ (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    ಪ್ರತ್ಯಾಗಮ varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ಧ್ರುವ ItemVariationStore *varStore;
  ಧ್ರುವ DeltaSetIndexMap *varIdxMap;
  hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords;
  hb_scalar_cache_t *cache;
};

ಸಂರಚನಾ MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (ಧ್ರುವ MultiItemVariationStore *varStore,
			      ಧ್ರುವ DeltaSetIndexMap *varIdxMap,
			      hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    ಯದಿ (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ varStore; }

  ದ್ವಿಗುಣ get_condition_delta (uint32_t varIdx) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ದ್ವಿಗುಣ) (*ಏತತ್)[varIdx]; }

  ದ್ವಿಗುಣ operator[] (uint32_t varIdx) ಧ್ರುವ
  {
    ದ್ವಿಗುಣ v = 0;
    (*ಏತತ್) (hb_array (&v, 1), varIdx);
    ಪ್ರತ್ಯಾಗಮ v;
  }

  ಶೂನ್ಯ operator() (hb_array_t<ದ್ವಿಗುಣ> out, uint32_t varIdx, unsigned short offset = 0) ಧ್ರುವ
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    ಯದಿ (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      ಯದಿ (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    ಅನ್ಯಥಾ
      ಪ್ರತಿ (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  ಧ್ರುವ MultiItemVariationStore *varStore;
  ಧ್ರುವ DeltaSetIndexMap *varIdxMap;
  hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
ಗಣನಾ Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

ಗಣನಾ lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

ಸಂರಚನಾ Condition;

template <typename Instancer>
ಸ್ಥಿರ ತಾರ್ಕಿಕ
_hb_recurse_condition_evaluate (ಧ್ರುವ ಸಂರಚನಾ Condition &condition,
				ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
				unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
				Instancer *instancer);

ಸಂರಚನಾ ConditionAxisRange
{
  friend ಸಂರಚನಾ Condition;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_map_t *index_map = &c->plan->axes_index_map;
    ಯದಿ (index_map->is_empty ()) return_trace (ಸತ್ಯ);

    ಧ್ರುವ hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    ಯದಿ (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    ಯದಿ (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    ಧ್ರುವ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    ಯದಿ (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    ಭಿನ್ನ normalized_min = renormalize_ಮೌಲ್ಯ ((ದ್ವಿಗುಣ) filter_Range_Min_ಮೌಲ್ಯ.to_float (), axis_limit, axis_triple_distances, ಅಸತ್ಯ);
    ಭಿನ್ನ normalized_max = renormalize_ಮೌಲ್ಯ ((ದ್ವಿಗುಣ) filter_Range_Max_ಮೌಲ್ಯ.to_float (), axis_limit, axis_triple_distances, ಅಸತ್ಯ);
    out->filter_Range_Min_ಮೌಲ್ಯ.set_float (normalized_min);
    out->filter_Range_Max_ಮೌಲ್ಯ.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ಗುಪ್ತ:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) ಧ್ರುವ
  {
    //invalid axis index, drop the entire record
    ಯದಿ (!c->axes_index_tag_map->has (axisIndex))
      ಪ್ರತ್ಯಾಗಮ DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    ತಾರ್ಕಿಕ axis_set_by_user = ಅಸತ್ಯ;
    ಯದಿ (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = ಸತ್ಯ;
    }

    ಭಿನ್ನ axis_min_val = axis_range.minimum;
    ಭಿನ್ನ axis_default_val = axis_range.ಮಧ್ಯ;
    ಭಿನ್ನ axis_max_val = axis_range.maximum;

    ಭಿನ್ನ filter_min_val = filter_Range_Min_ಮೌಲ್ಯ.to_float ();
    ಭಿನ್ನ filter_max_val = filter_Range_Max_ಮೌಲ್ಯ.to_float ();

    ಯದಿ (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = ಅಸತ್ಯ;

    //condition not met, drop the entire record
    ಯದಿ (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      ಪ್ರತ್ಯಾಗಮ DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    ಯದಿ (axis_set_by_user && axis_range.is_point ())
      ಪ್ರತ್ಯಾಗಮ DROP_COND_WITH_VAR;

    ಯದಿ (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_ಮೌಲ್ಯ.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_ಮೌಲ್ಯ.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      ಪ್ರತ್ಯಾಗಮ KEEP_COND_WITH_VAR;
    }
    ಪ್ರತ್ಯಾಗಮ KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		 Instancer *instancer HB_UNUSED) ಧ್ರುವ
  {
    ಪೂರ್ಣಾಙ್ಕ coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    ಪ್ರತ್ಯಾಗಮ filter_Range_Min_ಮೌಲ್ಯ.to_int () <= coord && coord <= filter_Range_Max_ಮೌಲ್ಯ.to_int ();
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_ಮೌಲ್ಯ;
  F2DOT14	filter_Range_Max_ಮೌಲ್ಯ;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ Condition_ಮೌಲ್ಯ
{
  friend ಸಂರಚನಾ Condition;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (varIdx == VarIdx::NO_VARIATION)
      return_trace (ಸತ್ಯ);

    hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *condition_ಮೌಲ್ಯ;
    ಯದಿ (c->plan->layout_condition_idx_ಮೌಲ್ಯ_map.has ((uintptr_t) ಏತತ್, &condition_ಮೌಲ್ಯ))
    {
      out->default_ಮೌಲ್ಯ_2 = condition_ಮೌಲ್ಯ->second;
      out->varIdx = condition_ಮೌಲ್ಯ->first;
      return_trace (ಸತ್ಯ);
    }

    hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *new_varidx_delta;
    ಯದಿ (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!c->serializer->check_assign (
		out->default_ಮೌಲ್ಯ_2,
		default_ಮೌಲ್ಯ_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ಅಸತ್ಯ);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  template <typename Instancer>
  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		 Instancer *instancer) ಧ್ರುವ
  {
    ದ್ವಿಗುಣ value = default_ಮೌಲ್ಯ_2;
    value += instancer->get_condition_delta (varIdx);
    ಪ್ರತ್ಯಾಗಮ value > 0;
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_ಮೌಲ್ಯ_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ ConditionAnd
{
  friend ಸಂರಚನಾ Condition;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ಅಸತ್ಯ);
    out->format = format;

    ಪ್ರತಿ (ಧ್ರುವ auto &offset : conditions)
    {
      ಯದಿ (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	ಯದಿ (unlikely (!out_offset)) return_trace (ಅಸತ್ಯ);
	*out_offset = 0;
      }
      ಅನ್ಯಥಾ ಯದಿ (unlikely (!subset_offset_array (c, out->conditions, ಏತತ್) (offset)))
	return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  template <typename Instancer>
  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		 Instancer *instancer) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = conditions.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!_hb_recurse_condition_evaluate (ಏತತ್+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (conditions.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<ಸಂರಚನಾ Condition>	conditions;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (3, conditions);
};

ಸಂರಚನಾ ConditionOr
{
  friend ಸಂರಚನಾ Condition;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ಅಸತ್ಯ);
    out->format = format;

    ಪ್ರತಿ (ಧ್ರುವ auto &offset : conditions)
    {
      ಯದಿ (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	ಯದಿ (unlikely (!out_offset)) return_trace (ಅಸತ್ಯ);
	*out_offset = 0;
      }
      ಅನ್ಯಥಾ ಯದಿ (unlikely (!subset_offset_array (c, out->conditions, ಏತತ್) (offset)))
	return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  template <typename Instancer>
  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		 Instancer *instancer) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = conditions.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (_hb_recurse_condition_evaluate (ಏತತ್+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (conditions.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<ಸಂರಚನಾ Condition>	conditions;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (3, conditions);
};

ಸಂರಚನಾ ConditionNegate
{
  friend ಸಂರಚನಾ Condition;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
    ಯದಿ (condition.is_null ())
    {
      out->condition = 0;
      return_trace (ಸತ್ಯ);
    }
    return_trace (out->condition.serialize_subset (c, condition, ಏತತ್));
  }

  ಗುಪ್ತ:
  template <typename Instancer>
  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		 Instancer *instancer) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !_hb_recurse_condition_evaluate (ಏತತ್+condition,
					    coords, coord_len,
					    instancer);
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (condition.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<ಸಂರಚನಾ Condition>	condition;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (5);
};

ಸಂರಚನಾ Condition
{
  friend ಸಂರಚನಾ ConditionSet;

  ಸ್ಥಿರ ತಾರ್ಕಿಕ serialize_constant (hb_serialize_context_t *c, ತಾರ್ಕಿಕ value)
  {
    auto *out = c->start_embed<Condition_ಮೌಲ್ಯ> ();
    ಯದಿ (unlikely (!out || !c->extend_min (out))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    out->format = 2;
    out->default_ಮೌಲ್ಯ_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      ತಾರ್ಕಿಕ materialize_true = ಅಸತ್ಯ) ಧ್ರುವ;

  template <typename Instancer>
  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		 Instancer *instancer) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    ಪಕ್ಷ 0: ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.evaluate (coords, coord_len, instancer);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.evaluate (coords, coord_len, instancer);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.evaluate (coords, coord_len, instancer);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.evaluate (coords, coord_len, instancer);
    ಪಕ್ಷ 5: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format5.evaluate (coords, coord_len, instancer);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!depth))
    {
      c->apply = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ DROP_RECORD_WITH_VAR;
    }

    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: ಪ್ರತ್ಯಾಗಮ KEEP_RECORD_WITH_VAR;
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.keep_with_variations (c, condition_map);
    ಪಕ್ಷ 2: ಪಕ್ಷ 3: ಪಕ್ಷ 4: ಪಕ್ಷ 5: ಭಙ್ಗ;
    ಸ್ವಭಾವ: c->apply = ಅಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    ತಾರ್ಕಿಕ applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    ಯದಿ (c->all_axes_pinned)
      ಪ್ರತ್ಯಾಗಮ applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    ಯದಿ (u.format.v == 2)
      ಪ್ರತ್ಯಾಗಮ u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (ಧ್ರುವ Condition &child)
    {
      auto child_context = *c;
      child_context.apply = ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    ಯದಿ (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      ಪ್ರತ್ಯಾಗಮ result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    ತಾರ್ಕಿಕ is_and = u.format.v == 3;
    ತಾರ್ಕಿಕ variable = ಅಸತ್ಯ;
    ಧ್ರುವ auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ಧ್ರುವ ಶೂನ್ಯ *base = is_and ? (ಧ್ರುವ ಶೂನ್ಯ *) &u.format3 : (ಧ್ರುವ ಶೂನ್ಯ *) &u.format4;
    ಪ್ರತಿ (ಧ್ರುವ auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      ಯದಿ (is_and && result == DROP_RECORD_WITH_VAR)
        ಪ್ರತ್ಯಾಗಮ DROP_RECORD_WITH_VAR;
      ಯದಿ (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        ಪ್ರತ್ಯಾಗಮ KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    ಪ್ರತ್ಯಾಗಮ variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    ಯದಿ (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_ಮೌಲ್ಯ ());

    return_trace (c->end_recursion (ಏತತ್->dispatch (c)));
  }

  ತಾರ್ಕಿಕ collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *condition_ಮೌಲ್ಯಗಳು = nullptr) ಧ್ರುವ;

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ Condition *src,
		  ಧ್ರುವ hb_map_t &varidx_map,
		  ಧ್ರುವ hb_map_t *axis_map = nullptr);

  ಗುಪ್ತ:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) ಧ್ರುವ;

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_ಮೌಲ್ಯ	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
ತಾರ್ಕಿಕ
_hb_recurse_condition_evaluate (ಧ್ರುವ ಸಂರಚನಾ Condition &condition,
				ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
				unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
				Instancer *instancer)
{
  ಪ್ರತ್ಯಾಗಮ condition.evaluate (coords, coord_len, instancer);
}

ಸಂರಚನಾ ConditionList
{
  ಧ್ರುವ Condition& operator[] (unsigned i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+conditions[i]; }

  unsigned get_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ conditions.len; }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ ConditionList *src,
		  ಧ್ರುವ hb_inc_bimap_t &condition_map,
		  ಧ್ರುವ hb_map_t &varidx_map,
		  ಧ್ರುವ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    conditions.len = condition_map.get_population ();
    ಯದಿ (unlikely (!c->extend (conditions))) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      ಯದಿ (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (ಅಸತ್ಯ);
    }
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (conditions.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  Array32OfOffset32To<Condition> conditions;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, conditions);
};

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *condition_ಮೌಲ್ಯಗಳು) ಧ್ರುವ
{
  ಯದಿ (unlikely (!depth)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ವಿಕಲ್ಪ (u.format.v)
  {
    ಪಕ್ಷ 0:
    ಪಕ್ಷ 1:
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪಕ್ಷ 2:
      ಯದಿ (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        ಯದಿ (condition_ಮೌಲ್ಯಗಳು &&
            !condition_ಮೌಲ್ಯಗಳು->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), ಪೂರ್ಣಾಙ್ಕ (u.format2.default_ಮೌಲ್ಯ_2))))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      ಪ್ರತ್ಯಾಗಮ !var_indices->in_error ();
    ಪಕ್ಷ 3:
      ಪ್ರತಿ (ಧ್ರುವ auto &offset : u.format3.conditions)
	ಯದಿ (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_ಮೌಲ್ಯಗಳು)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪಕ್ಷ 4:
      ಪ್ರತಿ (ಧ್ರುವ auto &offset : u.format4.conditions)
	ಯದಿ (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_ಮೌಲ್ಯಗಳು)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪಕ್ಷ 5:
      ಪ್ರತ್ಯಾಗಮ (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_ಮೌಲ್ಯಗಳು);
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
}

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
Condition::serialize (hb_serialize_context_t *c,
		      ಧ್ರುವ Condition *src,
		      ಧ್ರುವ hb_map_t &varidx_map,
		      ಧ್ರುವ hb_map_t *axis_map)
{
  TRACE_SERIALIZE (ಏತತ್);
  ವಿಕಲ್ಪ (src->u.format.v)
  {
    ಪಕ್ಷ 0:
    {
      auto *out = c->start_embed<Condition_ಮೌಲ್ಯ> ();
      ಯದಿ (unlikely (!out || !c->extend_min (out))) return_trace (ಅಸತ್ಯ);
      out->format = 2;
      out->default_ಮೌಲ್ಯ_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (ಸತ್ಯ);
    }
    ಪಕ್ಷ 1:
    {
      auto *out = c->embed (&src->u.format1);
      ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);
      ಯದಿ (axis_map)
      {
	ಯದಿ (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (ಅಸತ್ಯ);
	}
	ಯದಿ (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (ಅಸತ್ಯ);
      }
      return_trace (ಸತ್ಯ);
    }
    ಪಕ್ಷ 2:
    {
      auto *out = c->embed (&src->u.format2);
      ಯದಿ (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (ಅಸತ್ಯ);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (ಸತ್ಯ);
    }
    ಪಕ್ಷ 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      ಯದಿ (unlikely (!c->extend_min (out))) return_trace (ಅಸತ್ಯ);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      ಯದಿ (unlikely (!c->extend (out->conditions))) return_trace (ಅಸತ್ಯ);
      ಪ್ರತಿ (unsigned i = 0; i < out->conditions.len; i++)
	ಯದಿ (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (ಅಸತ್ಯ);
      return_trace (ಸತ್ಯ);
    }
    ಪಕ್ಷ 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      ಯದಿ (unlikely (!c->extend_min (out))) return_trace (ಅಸತ್ಯ);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      ಯದಿ (unlikely (!c->extend (out->conditions))) return_trace (ಅಸತ್ಯ);
      ಪ್ರತಿ (unsigned i = 0; i < out->conditions.len; i++)
	ಯದಿ (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (ಅಸತ್ಯ);
      return_trace (ಸತ್ಯ);
    }
    ಪಕ್ಷ 5:
    {
      auto *out = c->embed (&src->u.format5);
      ಯದಿ (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (ಅಸತ್ಯ);
      return_trace (ಸತ್ಯ);
    }
    ಸ್ವಭಾವ:
      return_trace (ಅಸತ್ಯ);
  }
}

ಅನ್ತಃಸ್ಥ lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    ತಾರ್ಕಿಕ materialize_true) ಧ್ರುವ
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  ಯದಿ (result == LOOKUP_CONDITION_SUBSET_KEEP)
    ಪ್ರತ್ಯಾಗಮ result;

  c->serializer->revert (snap);
  ಯದಿ (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    ಪ್ರತ್ಯಾಗಮ result;

  ಪ್ರತ್ಯಾಗಮ serialize_constant (c->serializer, ಸತ್ಯ) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

ಅನ್ತಃಸ್ಥ lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) ಧ್ರುವ
{
  ಯದಿ (unlikely (!depth))
    ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   ಧ್ರುವ Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    ಯದಿ (unlikely (!offset))
      ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    ಯದಿ (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->ತೆಗೆ_pack ());
    ಅನ್ಯಥಾ
    {
      c->serializer->ತೆಗೆ_discard ();
      out.ತೆಗೆ ();
      c->serializer->revert (snap);
    }
    ಪ್ರತ್ಯಾಗಮ result;
  };

  ವಿಕಲ್ಪ (u.format.v)
  {
    ಪಕ್ಷ 0:
      ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_TRUE;

    ಪಕ್ಷ 1:
    {
      ಧ್ರುವ ConditionAxisRange &src = u.format1;
      ಯದಿ (c->plan->user_axes_location.is_empty ())
        ಪ್ರತ್ಯಾಗಮ c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      ಯದಿ (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      ಯದಿ (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      ಭಿನ್ನ filter_min = src.filter_Range_Min_ಮೌಲ್ಯ.to_float ();
      ಭಿನ್ನ filter_max = src.filter_Range_Max_ಮೌಲ್ಯ.to_float ();
      ಯದಿ (filter_min > filter_max)
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_FALSE;
      ಯದಿ (axis_limit.is_point ())
        ಪ್ರತ್ಯಾಗಮ filter_min <= axis_limit.ಮಧ್ಯ &&
	       axis_limit.ಮಧ್ಯ <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      ಯದಿ (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_FALSE;
      ಯದಿ (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_TRUE;
      ಯದಿ (!c->plan->axes_index_map.has (src.axisIndex))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      ಯದಿ (unlikely (!out))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      ಯದಿ (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_ಮೌಲ್ಯ.set_float (
	  renormalize_ಮೌಲ್ಯ (filter_min, axis_limit, axis_distances, ಅಸತ್ಯ));
      out->filter_Range_Max_ಮೌಲ್ಯ.set_float (
	  renormalize_ಮೌಲ್ಯ (filter_max, axis_limit, axis_distances, ಅಸತ್ಯ));
      ಯದಿ (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;
      ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    ಪಕ್ಷ 2:
    {
      ಧ್ರುವ Condition_ಮೌಲ್ಯ &src = u.format2;
      auto *out = c->serializer->embed (&src);
      ಯದಿ (unlikely (!out))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;

      ಯದಿ (src.varIdx == VarIdx::NO_VARIATION)
        ಪ್ರತ್ಯಾಗಮ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((ಪೂರ್ಣಾಙ್ಕ) src.default_ಮೌಲ್ಯ_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *condition_ಮೌಲ್ಯ;
      ಯದಿ (c->plan->layout_condition_idx_ಮೌಲ್ಯ_map.has ((uintptr_t) &src, &condition_ಮೌಲ್ಯ))
      {
        out->default_ಮೌಲ್ಯ_2 = condition_ಮೌಲ್ಯ->second;
        out->varIdx = condition_ಮೌಲ್ಯ->first;
        ಯದಿ (out->varIdx == VarIdx::NO_VARIATION)
          ಪ್ರತ್ಯಾಗಮ out->default_ಮೌಲ್ಯ_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *new_varidx_delta;
      ಯದಿ (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        ಪ್ರತ್ಯಾಗಮ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((ಪೂರ್ಣಾಙ್ಕ) src.default_ಮೌಲ್ಯ_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      ಯದಿ (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        ದ್ವಿಗುಣ value = (ಪೂರ್ಣಾಙ್ಕ) src.default_ಮೌಲ್ಯ_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        ಪ್ರತ್ಯಾಗಮ value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      ಪೂರ್ಣಾಙ್ಕ default_ಮೌಲ್ಯ = (ಪೂರ್ಣಾಙ್ಕ) src.default_ಮೌಲ್ಯ_2 +
			  hb_second (*new_varidx_delta);
      ಯದಿ (unlikely (!c->serializer->check_assign (
		out->default_ಮೌಲ್ಯ_2,
		default_ಮೌಲ್ಯ,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    ಪಕ್ಷ 3:
    {
      ಧ್ರುವ ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      ಯದಿ (unlikely (!out || !c->serializer->extend_min (out)))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      ಪ್ರತಿ (ಧ್ರುವ auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        ಯದಿ (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          ಪ್ರತ್ಯಾಗಮ result;
      }
      ಪ್ರತ್ಯಾಗಮ out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    ಪಕ್ಷ 4:
    {
      ಧ್ರುವ ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      ಯದಿ (unlikely (!out || !c->serializer->extend_min (out)))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      ಪ್ರತಿ (ಧ್ರುವ auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        ಯದಿ (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          ಪ್ರತ್ಯಾಗಮ result;
      }
      ಪ್ರತ್ಯಾಗಮ out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    ಪಕ್ಷ 5:
    {
      ಧ್ರುವ ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      ಯದಿ (unlikely (!out))
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      ಯದಿ (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->ತೆಗೆ_pack ());
        ಪ್ರತ್ಯಾಗಮ result;
      }

      c->serializer->ತೆಗೆ_discard ();
      ಯದಿ (result == LOOKUP_CONDITION_SUBSET_TRUE)
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_FALSE;
      ಯದಿ (result == LOOKUP_CONDITION_SUBSET_FALSE)
        ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_TRUE;
      ಪ್ರತ್ಯಾಗಮ result;
    }

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

ಸಂರಚನಾ ConditionSet
{
  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		 ItemVarStoreInstancer *instancer) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = conditions.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!(ಏತತ್+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ಧ್ರುವ
  {
    hb_map_t *condition_map = hb_map_create ();
    ಯದಿ (unlikely (!condition_map)) ಪ್ರತ್ಯಾಗಮ;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    ಯದಿ (unlikely (!cond_set)) ಪ್ರತ್ಯಾಗಮ;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = ಸತ್ಯ;
    ತಾರ್ಕಿಕ should_keep = ಅಸತ್ಯ;
    /* The range map cannot identify value or boolean expressions uniquely. */
    ತಾರ್ಕಿಕ can_deduplicate = ಸತ್ಯ;
    unsigned ಅಂಕೆ_kept_cond = 0, cond_idx = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& offset : conditions)
    {
      ಧ್ರುವ Condition &condition = ಏತತ್+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      ಯದಿ (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = ಅಸತ್ಯ;
        ಪ್ರತ್ಯಾಗಮ;
      }

      ಯದಿ (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = ಸತ್ಯ;
        cond_set->add (cond_idx);
        ಅಂಕೆ_kept_cond++;
      }

      ಯದಿ (ret == KEEP_RECORD_WITH_VAR)
        should_keep = ಸತ್ಯ;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = ಅಂಕೆ_kept_cond == 0;
    ಯದಿ (!should_keep && c->all_axes_pinned) ಪ್ರತ್ಯಾಗಮ;

    //check if condition_set is unique with variations
    ಯದಿ (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               ತಾರ್ಕಿಕ ಸೇರಿಸು_catch_all) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (ಸೇರಿಸು_catch_all) return_trace (ಸತ್ಯ);

    hb_set_t *retained_cond_set = nullptr;
    ಯದಿ (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = conditions.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (retained_cond_set != nullptr && !retained_cond_set->has (i))
        ಅನುವರ್ತ;
      ಧ್ರುವ Condition &condition = ಏತತ್+conditions[i];
      ಯದಿ (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, ಏತತ್) (conditions[i]);
        ಅನುವರ್ತ;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      ಯದಿ (unlikely (!offset)) return_trace (ಅಸತ್ಯ);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, ಸತ್ಯ);
      ಯದಿ (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, ಅಸತ್ಯ) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      ಯದಿ (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->ತೆಗೆ_discard ();
        c->serializer->check_success (ಅಸತ್ಯ);
        return_trace (ಅಸತ್ಯ);
      }
      c->serializer->add_link (*offset, c->serializer->ತೆಗೆ_pack ());
    }

    return_trace (ತಾರ್ಕಿಕ (out->conditions));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (conditions.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &offset : conditions)
      ಯದಿ (unlikely (!(ಏತತ್+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_ಮೌಲ್ಯಗಳು)))
	c->layout_variation_indices->err ();
  }

  ರಕ್ಷಿತ:
  Array16OfOffset32To<Condition>	conditions;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, conditions);
};

ಸಂರಚನಾ FeatureTableSubstitutionRecord
{
  friend ಸಂರಚನಾ FeatureTableSubstitution;

  ಶೂನ್ಯ collect_lookups (ಧ್ರುವ ಶೂನ್ಯ *base, hb_set_t *lookup_indexes /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  ಶೂನ್ಯ closure_features (ಧ್ರುವ ಶೂನ್ಯ *base,
			 ಧ್ರುವ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ಧ್ರುವ
  {
    ಯದಿ ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  ಶೂನ್ಯ collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, ಧ್ರುವ Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    ಧ್ರುವ hb_set_t *feature_indices,
                                                    ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಯದಿ (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  ತಾರ್ಕಿಕ serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  ಧ್ರುವ Feature *f, ಧ್ರುವ Tag *tag)
  {
    TRACE_SERIALIZE (ಏತತ್);
    hb_serialize_context_t *s = c->subset_context->serializer;
    ಯದಿ (unlikely (!s->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);

    uint32_t *new_feature_idx;
    ಯದಿ (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    s->push ();
    ತಾರ್ಕಿಕ ret = f->subset (c->subset_context, c, tag);
    ಯದಿ (ret) s->add_link (feature, s->ತೆಗೆ_pack ());
    ಅನ್ಯಥಾ s->ತೆಗೆ_discard ();

    return_trace (ret);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_layout_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    uint32_t *new_feature_index;
    ಯದಿ (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (ಅಸತ್ಯ);

    auto *out = c->subset_context->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->featureIndex = *new_feature_index;
    hb_pair_t<ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*> *original;
    ಯದಿ (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (ಅಸತ್ಯ);
    ಧ್ರುವ Tag *tag = reinterpret_cast<ಧ್ರುವ Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base,
		 ಧ್ರುವ FeatureList *feature_list) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ಧ್ರುವ Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  ರಕ್ಷಿತ:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ FeatureTableSubstitution
{
  ಶೂನ್ಯ collect_feature_indices (hb_set_t *feature_indices) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  ಧ್ರುವ Feature *find_substitute (unsigned ಪೂರ್ಣಾಙ್ಕ feature_index) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = substitutions.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      ಯದಿ (record.featureIndex == feature_index)
	ಪ್ರತ್ಯಾಗಮ &(ಏತತ್+record.feature);
    }
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಶೂನ್ಯ collect_lookups (ಧ್ರುವ hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ಧ್ರುವ
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([ಏತತ್, lookup_indexes] (ಧ್ರುವ FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (ಏತತ್, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  ಶೂನ್ಯ collect_name_ids (ಧ್ರುವ hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ FeatureTableSubstitutionRecord &record : substitutions)
      ಯದಿ (feature_index_map->has (record.featureIndex))
        (ಏತತ್+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  ಶೂನ್ಯ closure_features (ಧ್ರುವ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (ಏತತ್, lookup_indexes, feature_indexes);
  }

  ತಾರ್ಕಿಕ intersects_features (ಧ್ರುವ hb_map_t *feature_index_map) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ FeatureTableSubstitutionRecord& record : substitutions)
    {
      ಯದಿ (feature_index_map->has (record.featureIndex)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, ಏತತ್);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               ತಾರ್ಕಿಕ ಸೇರಿಸು_catch_all) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->version.major = version.major;
    out->version.minor = version.minor;

    ಯದಿ (ಸೇರಿಸು_catch_all)
    {
      ಪ್ರತಿ (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*> *p;
        ಯದಿ (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (ಅಸತ್ಯ);
        auto *o = out->substitutions.serialize_append (c->serializer);
        ಯದಿ (!o->serialize (l, feature_index,
                           reinterpret_cast<ಧ್ರುವ Feature*> (p->first),
                           reinterpret_cast<ಧ್ರುವ Tag*> (p->second)))
          return_trace (ಅಸತ್ಯ);
      }
      return_trace (ಸತ್ಯ);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), ಏತತ್))
    ;

    return_trace (ತಾರ್ಕಿಕ (out->substitutions));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ FeatureList *feature_list) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, ಏತತ್, feature_list));
  }

  ರಕ್ಷಿತ:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

ಸಂರಚನಾ FeatureVariationRecord
{
  friend ಸಂರಚನಾ FeatureVariations;

  ಶೂನ್ಯ collect_lookups (ಧ್ರುವ ಶೂನ್ಯ     *base,
			ಧ್ರುವ hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  ಶೂನ್ಯ collect_name_ids (ಧ್ರುವ ಶೂನ್ಯ *base,
                         ಧ್ರುವ hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ಧ್ರುವ
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  ಶೂನ್ಯ closure_features (ಧ್ರುವ ಶೂನ್ಯ     *base,
			 ಧ್ರುವ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ಧ್ರುವ
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  ತಾರ್ಕಿಕ intersects_features (ಧ್ರುವ ಶೂನ್ಯ *base, ಧ್ರುವ hb_map_t *feature_index_map) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (base+substitutions).intersects_features (feature_index_map);
  }

  ಶೂನ್ಯ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    (base+conditions).keep_with_variations (c);
    ಯದಿ (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = ಸತ್ಯ; // set variations only once
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_layout_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base,
               ತಾರ್ಕಿಕ ಸೇರಿಸು_catch_all = ಅಸತ್ಯ) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->subset_context->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    ಯದಿ (!ಸೇರಿಸು_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    ಅನ್ಯಥಾ
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, ಸೇರಿಸು_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, ಸೇರಿಸು_catch_all);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base,
		 ಧ್ರುವ FeatureList *feature_list) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  { (base+conditions).collect_variation_indices (c); }

  ರಕ್ಷಿತ:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ LookupIndexList
{
  ಶೂನ್ಯ add_lookup_indexes_to (hb_set_t *lookup_indexes) ಧ್ರುವ
  { lookupIndices.add_indexes_to (lookup_indexes); }

  ತಾರ್ಕಿಕ intersects_lookup_indexes (ಧ್ರುವ hb_map_t *lookup_indexes) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ lookupIndices.intersects (lookup_indexes); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ಅಸತ್ಯ);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (ತಾರ್ಕಿಕ (out->lookupIndices));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (lookupIndices.sanitize (c));
  }

  ರಕ್ಷಿತ:
  IndexArray	lookupIndices;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

ಸಂರಚನಾ LookupConditionRecord
{
  unsigned get_condition_offset (ಧ್ರುವ ಶೂನ್ಯ *base, ಧ್ರುವ ಶೂನ್ಯ *variations) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ ವರ್ಣ *) &(base+condition) - (ಧ್ರುವ ವರ್ಣ *) variations; }

  template <typename Instancer>
  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಶೂನ್ಯ *base,
		 ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
		 unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		 Instancer *instancer) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (base+condition).evaluate (coords, coord_len, instancer); }

  ತಾರ್ಕಿಕ evaluate (ಧ್ರುವ ಶೂನ್ಯ *base,
                 ಧ್ರುವ ಶೂನ್ಯ *variations,
                 hb_sorted_array_t<ಧ್ರುವ unsigned> conditions) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     ಪರಿಮಾಣ (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  ತಾರ್ಕಿಕ collect_condition (ಧ್ರುವ ಶೂನ್ಯ *base,
                          ಧ್ರುವ ಶೂನ್ಯ *variations,
                          ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
                          unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) ಧ್ರುವ
  {
    /* Null conditions always apply and need no key state. */
    ಯದಿ (!condition) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    unsigned offset = get_condition_offset (base, variations);
    ಯದಿ (visited->has (offset)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    visited->add (offset);
    ಯದಿ (unlikely (visited->in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    ಪ್ರತ್ಯಾಗಮ !conditions->in_error ();
  }

  ಶೂನ್ಯ add_lookup_indexes_to (ಧ್ರುವ ಶೂನ್ಯ *base,
			      hb_set_t *lookup_indexes) ಧ್ರುವ
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  ತಾರ್ಕಿಕ intersects_lookup_indexes (ಧ್ರುವ ಶೂನ್ಯ *base,
				   ಧ್ರುವ hb_map_t *lookup_indexes) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_ಮೌಲ್ಯಗಳು)))
      c->layout_variation_indices->err ();
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    ಸತ್ಯ);
    ಯದಿ (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }
    c->serializer->add_link (out->condition, c->serializer->ತೆಗೆ_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  ರಕ್ಷಿತ:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ FeatureLookupsTable
{
  ತಾರ್ಕಿಕ adds_default_lookups () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ flags & 0x0001u; }

  ತಾರ್ಕಿಕ collect_conditions (ಧ್ರುವ ಶೂನ್ಯ *variations,
                           ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
                           unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupConditionRecord &record : records)
      ಯದಿ (unlikely (!record.collect_condition (ಏತತ್, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ collect_lookups (ಧ್ರುವ ಶೂನ್ಯ *variations,
                       hb_sorted_array_t<ಧ್ರುವ unsigned> conditions,
                       hb_set_t *lookup_indexes) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupConditionRecord &record : records)
      ಯದಿ (record.evaluate (ಏತತ್, variations, conditions))
	record.add_lookup_indexes_to (ಏತತ್, lookup_indexes);
  }

  template <typename Instancer>
  ಶೂನ್ಯ collect_lookups (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
			unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupConditionRecord &record : records)
      ಯದಿ (record.evaluate (ಏತತ್, coords, coord_len, instancer))
	record.add_lookup_indexes_to (ಏತತ್, lookup_indexes);
  }

  ಶೂನ್ಯ collect_lookups (hb_set_t *lookup_indexes) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (ಏತತ್, lookup_indexes);
  }

  ತಾರ್ಕಿಕ intersects_lookup_indexes (ಧ್ರುವ hb_map_t *lookup_indexes) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupConditionRecord &record : records)
      ಯದಿ (record.intersects_lookup_indexes (ಏತತ್, lookup_indexes))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupConditionRecord &record : records)
      record.collect_variation_indices (c, ಏತತ್);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ಅಸತ್ಯ);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    ಪ್ರತಿ (ಧ್ರುವ LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      ಯದಿ (record.subset (c, l, ಏತತ್))
	out->records.len++;
      ಅನ್ಯಥಾ
	c->serializer->revert (snap);
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (10, records);
};

ಸಂರಚನಾ LookupVariationRecord
{
  friend ಸಂರಚನಾ FeatureVariations;

  unsigned get_feature_index () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ featureIndex; }

  ಶೂನ್ಯ collect_lookups (ಧ್ರುವ ಶೂನ್ಯ *base,
                       ಧ್ರುವ Feature &feature,
                       hb_sorted_array_t<ಧ್ರುವ unsigned> conditions,
                       hb_set_t *lookup_indexes) ಧ್ರುವ
  {
    ಧ್ರುವ FeatureLookupsTable &feature_lookups = base+featureLookups;
    ಯದಿ (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  ಶೂನ್ಯ collect_lookups (ಧ್ರುವ ಶೂನ್ಯ *base,
			ಧ್ರುವ hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) ಧ್ರುವ
  {
    ಯದಿ (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  ಶೂನ್ಯ closure_features (ಧ್ರುವ ಶೂನ್ಯ *base,
			 ಧ್ರುವ hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) ಧ್ರುವ
  {
    ಯದಿ ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  { (base+featureLookups).collect_variation_indices (c); }

  ತಾರ್ಕಿಕ subset (hb_subset_layout_context_t *l, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (ಏತತ್);
    uint32_t *new_feature_index;
    ಯದಿ (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (ಅಸತ್ಯ);

    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ಅಸತ್ಯ);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  featureLookups.sanitize (c, base));
  }

  ರಕ್ಷಿತ:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ FeatureVariations
{
  ಸ್ಥಿರ constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  ಧ್ರುವ Array32Of<LookupVariationRecord> &get_lookup_variation_records () ಧ್ರುವ
  {
    ಯದಿ (version.to_int () < 0x00010001u)
      ಪ್ರತ್ಯಾಗಮ Null (Array32Of<LookupVariationRecord>);
    ಪ್ರತ್ಯಾಗಮ StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_ಎಣಿಕೆ () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ varRecords.len;
  }

  ತಾರ್ಕಿಕ find_index (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
		   unsigned ಪೂರ್ಣಾಙ್ಕ *index,
		   ItemVarStoreInstancer *instancer) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = varRecords.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ FeatureVariationRecord &record = varRecords.arrayZ[i];
      ಯದಿ ((ಏತತ್+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    *index = NOT_FOUND_INDEX;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಧ್ರುವ Feature *find_substitute (unsigned ಪೂರ್ಣಾಙ್ಕ variations_index,
				  unsigned ಪೂರ್ಣಾಙ್ಕ feature_index) ಧ್ರುವ
  {
    ಧ್ರುವ FeatureVariationRecord &record = varRecords[variations_index];
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+record.substitutions).find_substitute (feature_index);
  }

  ತಾರ್ಕಿಕ collect_lookup_conditions (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
                                  unsigned ಪೂರ್ಣಾಙ್ಕ coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) ಧ್ರುವ
  {
    hb_set_t visited_tables, visited_conditions;
    ಪ್ರತಿ (ಧ್ರುವ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      ಯದಿ (visited_tables.has (offset)) ಅನುವರ್ತ;
      visited_tables.add (offset);
      ಯದಿ (unlikely (visited_tables.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಧ್ರುವ FeatureLookupsTable &feature_lookups = ಏತತ್+record.featureLookups;
      ಯದಿ (unlikely (!feature_lookups.collect_conditions (ಏತತ್, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ get_lookup_variations (unsigned feature_index,
                              ಧ್ರುವ Feature &feature,
                              hb_sorted_array_t<ಧ್ರುವ unsigned> conditions,
                              hb_set_t *lookup_indexes) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      ಯದಿ (record.featureIndex != feature_index) ಅನುವರ್ತ;

      record.collect_lookups (ಏತತ್, feature, conditions, lookup_indexes);
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = varRecords.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, ಏತತ್);
      ಯದಿ (c->universal)
        ಭಙ್ಗ;
    }
    ಯದಿ (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed (*ಏತತ್));
  }

  ಶೂನ್ಯ collect_lookups (ಧ್ರುವ hb_set_t *feature_indexes,
			ಧ್ರುವ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = varRecords.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        ಅನುವರ್ತ;
      varRecords[i].collect_lookups (ಏತತ್, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  ಶೂನ್ಯ collect_name_ids (ಧ್ರುವ hb_map_t *feature_index_map,
                         ಧ್ರುವ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < varRecords.len; i++)
    {
      ಯದಿ (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        ಅನುವರ್ತ;
      varRecords[i].collect_name_ids (ಏತತ್, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  ಶೂನ್ಯ collect_lookup_variation_lookups (ಧ್ರುವ hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (ಏತತ್, feature_indexes, lookup_indexes);
  }

  ಶೂನ್ಯ closure_features (ಧ್ರುವ hb_map_t *lookup_indexes,
			 ಧ್ರುವ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = varRecords.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        ಅನುವರ್ತ;
      varRecords[i].closure_features (ಏತತ್, lookup_indexes, feature_indexes);
    }

    ಪ್ರತಿ (ಧ್ರುವ LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (ಏತತ್, lookup_indexes, feature_indexes);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, ಏತತ್);
    ಪ್ರತಿ (ಧ್ರುವ LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, ಏತತ್);
  }

  ಶೂನ್ಯ collect_feature_indices (hb_set_t *feature_indices) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ FeatureVariationRecord &record : varRecords)
      (ಏತತ್+record.substitutions).collect_feature_indices (feature_indices);
  }

  ತಾರ್ಕಿಕ has_lookup_variations (unsigned feature_index) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      ಯದಿ (record.featureIndex == feature_index)
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಯದಿ (record.featureIndex > feature_index)
	ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->version.major = version.major;
    out->version.minor = version.minor;

    ಪೂರ್ಣಾಙ್ಕ keep_up_to = -1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = varRecords.len - 1; i >= 0; i--) {
      ಯದಿ (varRecords[i].intersects_features (ಏತತ್, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        ಭಙ್ಗ;
      }
    }

    unsigned ಎಣಿಕೆ = (unsigned) (keep_up_to + 1);
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        ಅನುವರ್ತ;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), ಏತತ್) (varRecords[i]);
    }

    ಯದಿ (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      ತಾರ್ಕಿಕ ಸೇರಿಸು_catch_all_record = ಸತ್ಯ;
      subset_record_array (l, &(out->varRecords), ಏತತ್, ಸೇರಿಸು_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    ಯದಿ (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      ಯದಿ (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (ಅಸತ್ಯ);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, ಏತತ್))
      ;
    }

    return_trace (ತಾರ್ಕಿಕ (out->varRecords) ||
		  (out_lookup_records && ತಾರ್ಕಿಕ (*out_lookup_records)));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ FeatureList *feature_list) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, ಏತತ್, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, ಏತತ್)));
  }

  ರಕ್ಷಿತ:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

ಸಂರಚನಾ HintingDevice
{
  friend ಸಂರಚನಾ Device;

  ಗುಪ್ತ:

  hb_position_t get_x_delta (hb_font_t *font) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ get_delta (font->y_ppem, font->y_scale); }

  ಸಾರ್ವಜನಿಕ:

  size_t get_size () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ f = deltaFormat;
    ಯದಿ (unlikely (f < 1 || f > 3 || startSize > endSize)) ಪ್ರತ್ಯಾಗಮ 3 * HBUINT16::static_size;
    ಪ್ರತ್ಯಾಗಮ HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && c->check_range (ಏತತ್, ಏತತ್->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed<HintingDevice> (ಏತತ್));
  }

  ಗುಪ್ತ:

  ಪೂರ್ಣಾಙ್ಕ get_delta (unsigned ಪೂರ್ಣಾಙ್ಕ ppem, ಪೂರ್ಣಾಙ್ಕ scale) ಧ್ರುವ
  {
    ಯದಿ (!ppem) ಪ್ರತ್ಯಾಗಮ 0;

    ಪೂರ್ಣಾಙ್ಕ pixels = get_delta_pixels (ppem);

    ಯದಿ (!pixels) ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) (pixels * (int64_t) scale / ppem);
  }
  ಪೂರ್ಣಾಙ್ಕ get_delta_pixels (unsigned ಪೂರ್ಣಾಙ್ಕ ppem_size) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ f = deltaFormat;
    ಯದಿ (unlikely (f < 1 || f > 3))
      ಪ್ರತ್ಯಾಗಮ 0;

    ಯದಿ (ppem_size < startSize || ppem_size > endSize)
      ಪ್ರತ್ಯಾಗಮ 0;

    unsigned ಪೂರ್ಣಾಙ್ಕ s = ppem_size - startSize;

    unsigned ಪೂರ್ಣಾಙ್ಕ byte = delta_ಮೌಲ್ಯ_Z[s >> (4 - f)];
    unsigned ಪೂರ್ಣಾಙ್ಕ bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned ಪೂರ್ಣಾಙ್ಕ mask = (0xFFFFu >> (16 - (1 << f)));

    ಪೂರ್ಣಾಙ್ಕ delta = bits & mask;

    ಯದಿ ((unsigned ಪೂರ್ಣಾಙ್ಕ) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    ಪ್ರತ್ಯಾಗಮ delta;
  }

  ರಕ್ಷಿತ:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_ಮೌಲ್ಯ_Z;		/* Array of compressed data */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, delta_ಮೌಲ್ಯ_Z);
};

ಸಂರಚನಾ VariationDevice
{
  friend ಸಂರಚನಾ Device;

  ಗುಪ್ತ:

  hb_position_t get_x_delta (hb_font_t *font,
			     ಧ್ರುವ ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ಧ್ರುವ ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  ಭಿನ್ನ get_x_delta_f (hb_font_t *font,
		       ಧ್ರುವ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  ಭಿನ್ನ get_y_delta_f (hb_font_t *font,
		       ಧ್ರುವ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *v;
    ಯದಿ (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    ಯದಿ (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (ಏತತ್->static_size);
    auto *out = c->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (nullptr);

    ಯದಿ (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  ಶೂನ್ಯ collect_variation_index (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  { c->layout_variation_indices->add (varIdx); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಗುಪ್ತ:

  ಭಿನ್ನ get_delta (hb_font_t *font,
		   ಧ್ರುವ ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ store.get_delta (varIdx, font->coords, font->ಅಂಕೆ_coords, store_cache);
  }

  ರಕ್ಷಿತ:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ DeviceHeader
{
  ರಕ್ಷಿತ:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  ಸಾರ್ವಜನಿಕ:
  HBUINT16		format;		/* Format identifier */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  ಭಿನ್ನ get_x_delta_f (hb_font_t *font,
		       ಧ್ರುವ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  {
#ifndef HB_NO_VAR
    ಯದಿ (u.b.format == 0x8000)
    {
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    ಪ್ರತ್ಯಾಗಮ get_x_delta (font, store, store_cache);
  }

  ಭಿನ್ನ get_y_delta_f (hb_font_t *font,
		       ಧ್ರುವ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  {
#ifndef HB_NO_VAR
    ಯದಿ (u.b.format == 0x8000)
    {
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    ಪ್ರತ್ಯಾಗಮ get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ಧ್ರುವ ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.b.format)
    {
#ifndef HB_NO_HINTING
    ಪಕ್ಷ 1: ಪಕ್ಷ 2: ಪಕ್ಷ 3:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    ಪಕ್ಷ 0x8000:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ u.variation.get_x_delta (font, store, store_cache);
#endif
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ಧ್ರುವ ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.b.format)
    {
    ಪಕ್ಷ 1: ಪಕ್ಷ 2: ಪಕ್ಷ 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    ಪಕ್ಷ 0x8000:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ u.variation.get_y_delta (font, store, store_cache);
#endif
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.b.format.sanitize (c)) return_trace (ಅಸತ್ಯ);
    ವಿಕಲ್ಪ (u.b.format) {
#ifndef HB_NO_HINTING
    ಪಕ್ಷ 1: ಪಕ್ಷ 2: ಪಕ್ಷ 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    ಪಕ್ಷ 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    ಸ್ವಭಾವ:
      return_trace (ಸತ್ಯ);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map=nullptr) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    ವಿಕಲ್ಪ (u.b.format) {
#ifndef HB_NO_HINTING
    ಪಕ್ಷ 1:
    ಪಕ್ಷ 2:
    ಪಕ್ಷ 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    ಪಕ್ಷ 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    ಸ್ವಭಾವ:
      return_trace (nullptr);
    }
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.b.format) {
#ifndef HB_NO_HINTING
    ಪಕ್ಷ 1:
    ಪಕ್ಷ 2:
    ಪಕ್ಷ 3:
      ಪ್ರತ್ಯಾಗಮ;
#endif
#ifndef HB_NO_VAR
    ಪಕ್ಷ 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      ಪ್ರತ್ಯಾಗಮ;
#endif
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ;
    }
  }

  unsigned get_variation_index () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.b.format) {
#ifndef HB_NO_VAR
    ಪಕ್ಷ 0x8000:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ u.variation.varIdx;
#endif
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  ತಾರ್ಕಿಕ is_variation_device () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.b.format) {
#ifndef HB_NO_VAR
    ಪಕ್ಷ 0x8000:
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bimap.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cache.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (ଆକାର (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_ଦ୍ୱିମିକ_cache_t = hb_cache_t<14, 1, 8>;
static_assert (ଆକାର (hb_ot_layout_ଦ୍ୱିମିକ_cache_t) == 256, "");

#ଅନ୍ତର୍ଭାଵ "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


ନାମାକାଶ OT {

template<typename Iterator>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ସ୍ଥିର ତାର୍କିକ ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ଅଚଳ hb_set_t &klasses,
    ତାର୍କିକ use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

ସଂରଚନା ItemVarStoreInstancer;

ସଂରଚନା hb_collect_feature_substitutes_with_var_context_t
{
  ଅଚଳ hb_map_t *axes_index_tag_map;
  ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, ଅଚଳ Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  ତାର୍କିକ apply;
  ତାର୍କିକ variation_applied;
  ତାର୍କିକ universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  ତାର୍କିକ all_axes_pinned;
};

ସଂରଚନା hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ଅଚଳ ଶୂନ୍ୟ         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              ଅଚଳ hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_ଗଣନା (0),langsys_feature_ଗଣନା (0) {}

  ତାର୍କିକ visitScript ()
  { ଫେରନ୍ତୁ script_ଗଣନା++ < HB_MAX_SCRIPTS; }

  ତାର୍କିକ visitLangsys (unsigned feature_ଗଣନା_2)
  {
    langsys_feature_ଗଣନା += feature_ଗଣନା_2;
    ଫେରନ୍ତୁ langsys_feature_ଗଣନା < HB_MAX_LANGSYS_FEATURE_ଗଣନା;
  }

  ସର୍ବସାଧାରଣ:
  ଅଚଳ ଶୂନ୍ୟ *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ଅଚଳ hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  ବ୍ୟକ୍ତିଗତ:
  unsigned script_ଗଣନା;
  unsigned langsys_feature_ଗଣନା;
};

ସଂରଚନା hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  ଅଚଳ ଅକ୍ଷର *get_name () { ଫେରନ୍ତୁ "SUBSET_LAYOUT"; }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }

  ତାର୍କିକ visitScript ()
  {
    ଫେରନ୍ତୁ script_ଗଣନା++ < HB_MAX_SCRIPTS;
  }

  ତାର୍କିକ visitLangSys ()
  {
    ଫେରନ୍ତୁ langsys_ଗଣନା++ < HB_MAX_LANGSYS;
  }

  ତାର୍କିକ visitFeatureIndex (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
  {
    feature_index_ଗଣନା += ଗଣନା_2;
    ଫେରନ୍ତୁ feature_index_ଗଣନା < HB_MAX_FEATURE_INDICES;
  }

  ତାର୍କିକ visitLookupIndex()
  {
    lookup_index_ଗଣନା++;
    ଫେରନ୍ତୁ lookup_index_ଗଣନା < HB_MAX_LOOKUP_VISIT_ଗଣନା;
  }

  hb_subset_context_t *subset_context;
  ଅଚଳ hb_tag_t table_tag;
  ଅଚଳ hb_map_t *lookup_index_map;
  ଅଚଳ hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ଅଚଳ hb_map_t *feature_index_map;
  ଅଚଳ hb_map_t *feature_map_w_duplicates;
  ଅଚଳ hb_hashmap_t<unsigned, ଅଚଳ Feature*> *feature_substitutes_map;
  ଅଚଳ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  ଅଚଳ hb_set_t *catch_all_record_feature_idxes;
  ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<ଅଚଳ ଶୂନ୍ୟ*, ଅଚଳ ଶୂନ୍ୟ*>> *feature_idx_tag_map;

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
				script_ଗଣନା (0),
				langsys_ଗଣନା (0),
				feature_index_ଗଣନା (0),
				lookup_index_ଗଣନା (0)
  {
    ଯଦି (tag_ == HB_OT_TAG_GSUB)
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
    ଅନ୍ୟଥା
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

  ବ୍ୟକ୍ତିଗତ:
  unsigned script_ଗଣନା;
  unsigned langsys_ଗଣନା;
  unsigned feature_index_ଗଣନା;
  unsigned lookup_index_ଗଣନା;
};

ସଂରଚନା ItemVariationStore;
ସଂରଚନା hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj) { obj.collect_variation_indices (ଏହି); ଫେରନ୍ତୁ hb_empty_t (); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  ଅଚଳ hb_set_t *glyph_set;
  ଅଚଳ hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *condition_ମୂଲ୍ୟଗୁଡ଼ିକ = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  ଅଚଳ hb_set_t *glyph_set_,
					  ଅଚଳ hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
ସଂରଚନା subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 ଅଚଳ ଶୂନ୍ୟ *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  ତାର୍କିକ operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    ଯଦି (unlikely (!o)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ତାର୍କିକ ret = o->serialize_subset (subset_context, offset, base);
    ଯଦି (!ret)
    {
      out.କାଢ଼ ();
      subset_context->serializer->revert (snap);
    }
    ଫେରନ୍ତୁ ret;
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ଅଚଳ ଶୂନ୍ୟ *base;
};


template<typename OutputArray, typename Arg>
ସଂରଚନା subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     ଅଚଳ ଶୂନ୍ୟ *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  ତାର୍କିକ operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    ଯଦି (unlikely (!o)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ତାର୍କିକ ret = o->serialize_subset (subset_context, offset, base, arg);
    ଯଦି (!ret)
    {
      out.କାଢ଼ ();
      subset_context->serializer->revert (snap);
    }
    ଫେରନ୍ତୁ ret;
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ଅଚଳ ଶୂନ୍ୟ *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
ସଂରଚନା
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  { ଫେରନ୍ତୁ subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ଅଚଳ ଶୂନ୍ୟ *base, Arg &&arg) ଅଚଳ
  { ଫେରନ୍ତୁ subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
ସଂରଚନା subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 ଅଚଳ ଶୂନ୍ୟ *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  ଶୂନ୍ୟ
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    ତାର୍କିକ ret = record.subset (subset_layout_context, base);
    ଯଦି (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    ଅନ୍ୟଥା out->len++;
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ଅଚଳ ଶୂନ୍ୟ *base;
};

template<typename OutputArray, typename Arg>
ସଂରଚନା subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     ଅଚଳ ଶୂନ୍ୟ *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  ଶୂନ୍ୟ
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    ତାର୍କିକ ret = record.subset (subset_layout_context, base, arg);
    ଯଦି (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    ଅନ୍ୟଥା out->len++;
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ଅଚଳ ଶୂନ୍ୟ *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
ସଂରଚନା
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  { ଫେରନ୍ତୁ subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               ଅଚଳ ଶୂନ୍ୟ *base, Arg &&arg) ଅଚଳ
  { ଫେରନ୍ତୁ subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
ସଂରଚନା serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         ଅଚଳ ଶୂନ୍ୟ *base_,
                         ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  ତାର୍କିକ operator () (T&& record)
  {
    ଯଦି (!serialize_context->copy (record, base, variation_map)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    out.len++;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  ଅଚଳ ଶୂନ୍ୟ *base;
  ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
ସଂରଚନା
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               ଅଚଳ ଶୂନ୍ୟ *base,
               ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *variation_map) ଅଚଳ
  { ଫେରନ୍ତୁ serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

ସଂରଚନା IndexArray : Array16Of<Index>
{
  ତାର୍କିକ intersects (ଅଚଳ hb_map_t *indexes) ଅଚଳ
  { ଫେରନ୍ତୁ hb_any (*ଏହି, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    ଯଦି (!it) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (!c->extend_min ((*ଏହି)))) ଫେରନ୍ତୁ;

    ପାଇଁ (ଅଚଳ auto _ : it)
    {
      ଯଦି (!l->visitLookupIndex()) ଭାଙ୍ଗ;

      Index i;
      i = _;
      c->copy (i);
      ଏହି->len++;
    }
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_indexes (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_3 /* IN/OUT */,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *_indexes /* OUT */) ଅଚଳ
  {
    ଯଦି (ଗଣନା_3 && _indexes)
    {
      + ଏହି->as_array ().sub_array (start_offset, ଗଣନା_3)
      | hb_sink (hb_array (_indexes, *ଗଣନା_3))
      ;
    }
    ଫେରନ୍ତୁ ଏହି->len;
  }

  ଶୂନ୍ୟ add_indexes_to (hb_set_t* output /* OUT */) ଅଚଳ
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
ସଂରଚନା FeatureParamsSize
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି))) return_trace (ମିଥ୍ୟା);
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

    ଯଦି (!designSize)
      return_trace (ମିଥ୍ୟା);
    ଅନ୍ୟଥା ଯଦି (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (ସତ୍ୟ);
    ଅନ୍ୟଥା ଯଦି (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (ମିଥ୍ୟା);
    ଅନ୍ୟଥା
      return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ଅଚଳ
  { nameids_to_retain->add (subfamilyNameID); }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    return_trace ((ତାର୍କିକ) c->serializer->embed (*ଏହି));
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
ସଂରଚନା FeatureParamsStylisticSet
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ଅଚଳ
  { nameids_to_retain->add (uiNameID); }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    return_trace ((ତାର୍କିକ) c->serializer->embed (*ଏହି));
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
ସଂରଚନା FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_ଗଣନା, hb_codepoint_t *chars) ଅଚଳ
  {
    ଯଦି (char_ଗଣନା && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_ଗଣନା)
      | hb_sink (hb_array (chars, *char_ଗଣନା))
      ;
    }
    ଫେରନ୍ତୁ characters.len;
  }

  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  ଶୂନ୍ୟ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ଅଚଳ
  {
    ଯଦି (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    ଯଦି (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    ଯଦି (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    ଯଦି (!firstParamUILabelNameID || !ଅଙ୍କ_Named_Parameters || ଅଙ୍କ_Named_Parameters >= 0x7FFF)
      ଫେରନ୍ତୁ;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) ଅଙ୍କ_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    return_trace ((ତାର୍କିକ) c->serializer->embed (*ଏହି));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
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
  HBUINT16	ଅଙ୍କ_Named_Parameters;	/* Number of named parameters. (May
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (14, characters);
};

ସଂରଚନା FeatureParams
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, hb_tag_t tag) ଅଚଳ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
    TRACE_SANITIZE (ଏହି);
    ଯଦି (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    ଯଦି ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    ଯଦି ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ଅଚଳ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ଫେରନ୍ତୁ;
#endif
    ଯଦି (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ଫେରନ୍ତୁ (u.size.collect_name_ids (nameids_to_retain));
    ଯଦି ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ଫେରନ୍ତୁ (u.stylisticSet.collect_name_ids (nameids_to_retain));
    ଯଦି ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ଫେରନ୍ତୁ (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c, ଅଚଳ Tag* tag) ଅଚଳ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
    TRACE_SUBSET (ଏହି);
    ଯଦି (!tag) return_trace (ମିଥ୍ୟା);
    ଯଦି (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    ଯଦି ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    ଯଦି ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (ମିଥ୍ୟା);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  ଅଚଳ FeatureParamsSize& get_size_params (hb_tag_t tag) ଅଚଳ
  {
    ଯଦି (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ଫେରନ୍ତୁ u.size;
    ଫେରନ୍ତୁ Null (FeatureParamsSize);
  }
  ଅଚଳ FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) ଅଚଳ
  {
    ଯଦି ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ଫେରନ୍ତୁ u.stylisticSet;
    ଫେରନ୍ତୁ Null (FeatureParamsStylisticSet);
  }
  ଅଚଳ FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) ଅଚଳ
  {
    ଯଦି ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ଫେରନ୍ତୁ u.characterVariants;
    ଫେରନ୍ତୁ Null (FeatureParamsCharacterVariants);
  }
#endif

  ବ୍ୟକ୍ତିଗତ:
  ସଂଘ {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (0);
};

ସଂରଚନା Record_sanitize_closure_t {
  hb_tag_t tag;
  ଅଚଳ ଶୂନ୍ୟ *list_base;
};

ସଂରଚନା Feature
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lookup_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ lookupIndex[i]; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lookup_indexes (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lookup_ଗଣନା /* IN/OUT */,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lookup_tags /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ lookupIndex.get_indexes (start_index, lookup_ଗଣନା, lookup_tags); }
  ଶୂନ୍ୟ add_lookup_indexes_to (hb_set_t *lookup_indexes) ଅଚଳ
  { lookupIndex.add_indexes_to (lookup_indexes); }

  ଅଚଳ FeatureParams &get_feature_params () ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି+featureParams; }

  ତାର୍କିକ intersects_lookup_indexes (ଅଚଳ hb_map_t *lookup_indexes) ଅଚଳ
  { ଫେରନ୍ତୁ lookupIndex.intersects (lookup_indexes); }

  ଶୂନ୍ୟ collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ଅଚଳ
  {
    ଯଦି (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  ତାର୍କିକ subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ଅଚଳ Tag                   *tag = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->featureParams.serialize_subset (c, featureParams, ଏହି, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    ଯଦି (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    ଅନ୍ୟଥା
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
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ Record_sanitize_closure_t *closure = nullptr) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  featureParams.sanitize (c, ଏହି, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
ସଂରଚନା Record
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t a) ଅଚଳ { ଫେରନ୍ତୁ tag.cmp (a); }

  ତାର୍କିକ subset (hb_subset_layout_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base, ଅଚଳ ଶୂନ୍ୟ *f_sub = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->subset_context->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    ଅଚଳ Feature& f = *reinterpret_cast<ଅଚଳ Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    ତାର୍କିକ ret = f.subset (c->subset_context, c, &tag);
    ଯଦି (ret)
      s->add_link (out->offset, s->କାଢ଼_pack ());
    ଅନ୍ୟଥା
      s->କାଢ଼_discard ();

    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଅଚଳ Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (ଏହି) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
ସଂରଚନା RecordArrayOf : SortedArray16Of<Record<Type>>
{
  ଅଚଳ Offset16To<Type>& get_offset (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ (*ଏହି)[i].offset; }
  Offset16To<Type>& get_offset (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  { ଫେରନ୍ତୁ (*ଏହି)[i].offset; }
  ଅଚଳ Tag& get_tag (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ (*ଏହି)[i].tag; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
                         unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *record_ଗଣନା /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) ଅଚଳ
  {
    ଯଦି (record_ଗଣନା && record_tags)
    {
      + ଏହି->as_array ().sub_array (start_offset, record_ଗଣନା)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_ଗଣନା))
      ;
    }
    ଫେରନ୍ତୁ ଏହି->len;
  }
  ତାର୍କିକ find_index (hb_tag_t tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index) ଅଚଳ
  {
    ଫେରନ୍ତୁ ଏହି->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
ସଂରଚନା RecordListOf : RecordArrayOf<Type>
{
  ଅଚଳ Type& operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି+ଏହି->get_offset (i); }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    + ଏହି->iter ()
    | hb_apply (subset_record_array (l, out, ଏହି))
    ;
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (RecordArrayOf<Type>::sanitize (c, ଏହି));
  }
};

ସଂରଚନା RecordListOfFeature : RecordListOf<Feature>
{
  ତାର୍କିକ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    + hb_enumerate (*ଏହି)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, ଏହି] (ଅଚଳ hb_pair_t<unsigned, ଅଚଳ Record<Feature>&>& _)
                {
                  ଅଚଳ Feature *f_sub = nullptr;
                  ଅଚଳ Feature **f = nullptr;
                  ଯଦି (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, ଏହି, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (ସତ୍ୟ);
  }
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା RecordListOf<Feature> FeatureList;


ସଂରଚନା LangSys
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_feature_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ featureIndex.len; }
  hb_tag_t get_feature_index (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ featureIndex[i]; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_feature_indexes (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *feature_ଗଣନା_2 /* IN/OUT */,
				    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *feature_indexes /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ featureIndex.get_indexes (start_offset, feature_ଗଣନା_2, feature_indexes); }
  ଶୂନ୍ୟ add_feature_indexes_to (hb_set_t *feature_indexes) ଅଚଳ
  { featureIndex.add_indexes_to (feature_indexes); }

  ତାର୍କିକ has_required_feature () ଅଚଳ { ଫେରନ୍ତୁ reqFeatureIndex != 0xFFFFu; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_required_feature_index () ଅଚଳ
  {
    ଯଦି (reqFeatureIndex == 0xFFFFu)
      ଫେରନ୍ତୁ Index::NOT_FOUND_INDEX;
   ଫେରନ୍ତୁ reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed (*ଏହି));
  }

  ତାର୍କିକ compare (ଅଚଳ LangSys& o, ଅଚଳ hb_map_t *feature_index_map) ଅଚଳ
  {
    ଯଦି (reqFeatureIndex != o.reqFeatureIndex)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

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

    ପାଇଁ (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      ଯଦି (a != b) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଯଦି (iter || o_iter) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ collect_features (hb_prune_langsys_context_t *c) ଅଚଳ
  {
    ଯଦି (!has_required_feature () && !get_feature_ଗଣନା ()) ଫେରନ୍ତୁ;
    ଯଦି (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  ତାର୍କିକ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ଅଚଳ Tag                  *tag = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ଅଚଳ uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    ଯଦି (!l->visitFeatureIndex (featureIndex.len))
      return_trace (ମିଥ୍ୟା);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    ତାର୍କିକ ret = ତାର୍କିକ (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ Record_sanitize_closure_t * = nullptr) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

ସଂରଚନା Script
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lang_sys_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ langSys.len; }
  ଅଚଳ Tag& get_lang_sys_tag (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ langSys.get_tag (i); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lang_sys_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lang_sys_ଗଣନା /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ langSys.get_tags (start_offset, lang_sys_ଗଣନା, lang_sys_tags); }
  ଅଚଳ LangSys& get_lang_sys (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
    ଯଦି (i == Index::NOT_FOUND_INDEX) ଫେରନ୍ତୁ get_default_lang_sys ();
    ଫେରନ୍ତୁ ଏହି+langSys[i].offset;
  }
  ତାର୍କିକ find_lang_sys_index (hb_tag_t tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index) ଅଚଳ
  { ଫେରନ୍ତୁ langSys.find_index (tag, index); }

  ତାର୍କିକ has_default_lang_sys () ଅଚଳ           { ଫେରନ୍ତୁ defaultLangSys != 0; }
  ଅଚଳ LangSys& get_default_lang_sys () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+defaultLangSys; }

  ଶୂନ୍ୟ prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) ଅଚଳ
  {
    ଯଦି (!has_default_lang_sys () && !get_lang_sys_ଗଣନା ()) ଫେରନ୍ତୁ;
    ଯଦି (!c->visitScript ()) ଫେରନ୍ତୁ;

    ଯଦି (!c->script_langsys_map->has (script_index))
    {
      ଯଦି (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	ଫେରନ୍ତୁ;
    }

    ଯଦି (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ଅଚଳ LangSys& d = get_default_lang_sys ();
      ଯଦି (c->visitLangsys (d.get_feature_ଗଣନା ())) {
        d.collect_features (c);
      }

      ପାଇଁ (auto _ : + hb_enumerate (langSys))
      {
        ଅଚଳ LangSys& l = ଏହି+_.second.offset;
        ଯଦି (!c->visitLangsys (l.get_feature_ଗଣନା ())) ଜାରି;
        ଯଦି (l.compare (d, c->duplicate_feature_map)) ଜାରି;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    ଅନ୍ୟଥା
    {
      ପାଇଁ (auto _ : + hb_enumerate (langSys))
      {
        ଅଚଳ LangSys& l = ଏହି+_.second.offset;
        ଯଦି (!c->visitLangsys (l.get_feature_ଗଣନା ())) ଜାରି;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ଅଚଳ Tag                   *tag) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (!l->visitScript ()) return_trace (ମିଥ୍ୟା);
    ଯଦି (tag && !c->plan->layout_scripts.has (*tag))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ defaultLang = ମିଥ୍ୟା;
    ଯଦି (has_default_lang_sys ())
    {
      c->serializer->push ();
      ଅଚଳ LangSys& ls = ଏହି+defaultLangSys;
      ତାର୍କିକ ret = ls.subset (c, l);
      ଯଦି (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->କାଢ଼_discard ();
	out->defaultLangSys = 0;
      }
      ଅନ୍ୟଥା
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->କାଢ଼_pack ());
	defaultLang = ସତ୍ୟ;
      }
    }

    ଅଚଳ hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    ଯଦି (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (ଅଚଳ Record<LangSys>& record) {ଫେରନ୍ତୁ l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), ଏହି))
      ;
    }

    return_trace (ତାର୍କିକ (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ Record_sanitize_closure_t * = nullptr) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (defaultLangSys.sanitize (c, ଏହି) && langSys.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

ସଂରଚନା RecordListOfScript : RecordListOf<Script>
{
  ତାର୍କିକ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (auto _ : + hb_enumerate (*ଏହି))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      ତାର୍କିକ ret = _.second.subset (l, ଏହି);
      ଯଦି (!ret) c->serializer->revert (snap);
      ଅନ୍ୟଥା out->len++;
    }

    return_trace (ସତ୍ୟ);
  }
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା RecordListOfScript ScriptList;



ସଂରଚନା LookupFlag : HBUINT16
{
  ଗଣନା Flags {
    ଡାହାଣ_To_ବାମ		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
ନାମାକାଶ OT {

ସଂରଚନା Lookup
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_subtable_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ subTable.len; }

  template <typename TSubTable>
  ଅଚଳ Array16OfOffset16To<TSubTable>& get_subtables () ଅଚଳ
  { ଫେରନ୍ତୁ reinterpret_cast<ଅଚଳ Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { ଫେରନ୍ତୁ reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  ଅଚଳ TSubTable& get_subtable (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  { ଫେରନ୍ତୁ ଏହି+get_subtables<TSubTable> ()[i]; }

  size_t get_size () ଅଚଳ
  {
    ଅଚଳ HBUINT16 &markFilteringSet = StructAfter<ଅଚଳ HBUINT16> (subTable);
    ଯଦି (lookupFlag & LookupFlag::UseMarkFilteringSet)
      ଫେରନ୍ତୁ (ଅଚଳ ଅକ୍ଷର *) &StructAfter<ଅଚଳ ଅକ୍ଷର> (markFilteringSet) - (ଅଚଳ ଅକ୍ଷର *) ଏହି;
    ଫେରନ୍ତୁ (ଅଚଳ ଅକ୍ଷର *) &markFilteringSet - (ଅଚଳ ଅକ୍ଷର *) ଏହି;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_type () ଅଚଳ { ଫେରନ୍ତୁ lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag = lookupFlag;
    ଯଦି (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      ଅଚଳ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    ଫେରନ୍ତୁ flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_type = get_type ();
    TRACE_DISPATCH (ଏହି, lookup_type);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = get_subtable_ଗଣନା ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      ଯଦି (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_ମୂଲ୍ୟ ());
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_type,
		  uint32_t lookup_props,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_subtables)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    ଯଦି (unlikely (!subTable.serialize (c, ଅଙ୍କ_subtables))) return_trace (ମିଥ୍ୟା);
    ଯଦି (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ଯଦି (unlikely (!c->extend (ଏହି))) return_trace (ମିଥ୍ୟା);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (ସତ୍ୟ);
  }

  template <typename TSubTable>
  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    ଅଚଳ hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([ଏହି, glyphset, lookup_type] (ଅଚଳ Offset16To<TSubTable> &_) { ଫେରନ୍ତୁ (ଏହି+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), ଏହି, lookup_type))
    ;

    ଯଦି (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ଅଚଳ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      ଯଦି (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      ଅନ୍ୟଥା
      {
        ଯଦି (unlikely (!c->serializer->extend (out))) return_trace (ମିଥ୍ୟା);
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
    return_trace (ସତ୍ୟ);
  }

  template <typename TSubTable>
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(c->check_struct (ଏହି) && subTable.sanitize (c))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    unsigned subtables = get_subtable_ଗଣନା ();
    ଯଦି (unlikely (!c->visit_subtables (subtables))) return_trace (ମିଥ୍ୟା);

    ଯଦି (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ଅଚଳ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      ଯଦି (!markFilteringSet.sanitize (c)) return_trace (ମିଥ୍ୟା);
    }

    ଯଦି (unlikely (!get_subtables<TSubTable> ().sanitize (c, ଏହି, get_type ())))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = get_subtable<TSubTable> (0).u.extension.get_type ();
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < subtables; i++)
	ଯଦି (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (ମିଥ୍ୟା);
    }
    return_trace (ସତ୍ୟ);
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
ସଂରଚନା LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  ତାର୍କିକ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    + hb_enumerate (*ଏହି)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, ଏହି))
    ;
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, ଏହି)));
  }
};


/*
 * Coverage Table
 */


ସ୍ଥିର ତାର୍କିକ ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ଅଚଳ hb_set_t &klasses,
                                          ତାର୍କିକ use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  ଯଦି (!klass_map)
    ଫେରନ୍ତୁ ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  ଯଦି (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  ପାଇଁ (ଅଚଳ unsigned k: klasses)
  {
    ଯଦି (klass_map->has (k)) ଜାରି;
    klass_map->set (k, idx);
    idx++;
  }


  ଯଦି (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ପାଇଁ (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  ଫେରନ୍ତୁ ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
ସଂରଚନା ClassDefFormat1_3
{
  friend ସଂରଚନା ClassDef;

  ବ୍ୟକ୍ତିଗତ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_class (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ଫେରନ୍ତୁ class_ମୂଲ୍ୟ[(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (glyph_id - startGlyph)];
  }

  unsigned get_population () ଅଚଳ
  {
    ଫେରନ୍ତୁ class_ମୂଲ୍ୟ.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_ମୂଲ୍ୟ.len = 0;
      return_trace (ସତ୍ୟ);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_ଗଣନା = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    ଯଦି (unlikely (!class_ମୂଲ୍ୟ.serialize (c, glyph_ଗଣନା))) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (ଅଚଳ hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_ମୂଲ୍ୟ[idx] = gid_klass_pair.second;
    }
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               ତାର୍କିକ keep_empty_table = ସତ୍ୟ,
               ତାର୍କିକ use_class_zero = ସତ୍ୟ,
               ଅଚଳ Coverage* glyph_filter = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_ମୂଲ୍ୟ.len;

    ପାଇଁ (ଅଚଳ hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      ଯଦି (new_gid == HB_MAP_ମୂଲ୍ୟ_INVALID) ଜାରି;
      ଯଦି (glyph_filter && !glyph_filter->has(gid)) ଜାରି;

      unsigned klass = class_ମୂଲ୍ୟ[gid - start];
      ଯଦି (!klass) ଜାରି;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    ଯଦି (use_class_zero)
    {
      ଅଚଳ hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_ଗଣନା = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_ଗଣନା <= glyph_and_klass.length;
    }
    ଯଦି (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (ମିଥ୍ୟା);
    return_trace (keep_empty_table || (ତାର୍କିକ) glyph_and_klass);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && class_ମୂଲ୍ୟ.sanitize (c));
  }

  unsigned cost () ଅଚଳ { ଫେରନ୍ତୁ 1; }

  template <typename set_t>
  ତାର୍କିକ collect_coverage (set_t *glyphs) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = class_ମୂଲ୍ୟ.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (class_ମୂଲ୍ୟ[i])
	ଜାରି;

      ଯଦି (start != i)
	ଯଦି (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;

      start = i + 1;
    }
    ଯଦି (start != ଗଣନା_2)
      ଯଦି (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + ଗଣନା_2)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename set_t>
  ତାର୍କିକ collect_class (set_t *glyphs, unsigned klass) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = class_ମୂଲ୍ୟ.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (class_ମୂଲ୍ୟ[i] == klass) glyphs->add (startGlyph + i);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_ମୂଲ୍ୟ.len;
    ପାଇଁ (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      ଯଦି (class_ମୂଲ୍ୟ[iter - start]) ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ intersects_class (ଅଚଳ hb_set_t *glyphs, unsigned klass) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = class_ମୂଲ୍ୟ.len;
    ଯଦି (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_ମୂଲ୍ୟ_INVALID;
      ଯଦି (!glyphs->next (&g)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (g < startGlyph) ଫେରନ୍ତୁ ସତ୍ୟ;
      g = startGlyph + ଗଣନା_2 - 1;
      ଯଦି (glyphs->next (&g)) ଫେରନ୍ତୁ ସତ୍ୟ;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ଅଚଳ typename Types::HBUINT *arr = class_ମୂଲ୍ୟ.arrayZ;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (arr[i] == klass && glyphs->has (startGlyph + i))
	ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ intersected_class_glyphs (ଅଚଳ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ଅଚଳ
  {
    unsigned ଗଣନା_2 = class_ମୂଲ୍ୟ.len;
    ଯଦି (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      ପାଇଁ (uint32_t g = HB_SET_ମୂଲ୍ୟ_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      ପାଇଁ (uint32_t g = startGlyph + ଗଣନା_2 - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      ଫେରନ୍ତୁ;
    }

    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      ଯଦି (class_ମୂଲ୍ୟ[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#ଯଦି 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + ଗଣନା_2;
    ପାଇଁ (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      ଯଦି (class_ମୂଲ୍ୟ.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  ଶୂନ୍ୟ intersected_classes (ଅଚଳ hb_set_t *glyphs, hb_set_t *intersect_classes) ଅଚଳ
  {
    ଯଦି (glyphs->is_empty ()) ଫେରନ୍ତୁ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    ଯଦି (!class_ମୂଲ୍ୟ.len)
    {
      intersect_classes->add (0);
      ଫେରନ୍ତୁ;
    }

    hb_codepoint_t end_glyph = startGlyph + class_ମୂଲ୍ୟ.len - 1;
    ଯଦି (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    ପାଇଁ (ଅଚଳ auto& _ : + hb_enumerate (class_ମୂଲ୍ୟ))
    {
      hb_codepoint_t g = startGlyph + _.first;
      ଯଦି (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_ମୂଲ୍ୟ;	/* Array of Class Values--one per GlyphID */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_ମୂଲ୍ୟ);
};

template <typename Types>
ସଂରଚନା ClassDefFormat2_4
{
  friend ସଂରଚନା ClassDef;

  ବ୍ୟକ୍ତିଗତ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_class (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ଫେରନ୍ତୁ rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () ଅଚଳ
  {
    typename Types::large_int ret = 0;
    ପାଇଁ (ଅଚଳ auto &r : rangeRecord)
      ret += r.get_population ();
    ଫେରନ୍ତୁ ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (ସତ୍ୟ);
    }

    unsigned unsorted = ମିଥ୍ୟା;
    unsigned ଅଙ୍କ_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    ଯଦି (unlikely (!record)) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (ଅଚଳ auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      ଯଦି (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	ଯଦି (unlikely (cur_gid < prev_gid))
	  unsorted = ସତ୍ୟ;

	ଯଦି (unlikely (!record)) ଭାଙ୍ଗ;
	record->last = prev_gid;
	ଅଙ୍କ_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    ଯଦି (unlikely (c->in_error ())) return_trace (ମିଥ୍ୟା);

    ଯଦି (likely (record)) record->last = prev_gid;
    rangeRecord.len = ଅଙ୍କ_ranges;

    ଯଦି (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               ତାର୍କିକ keep_empty_table = ସତ୍ୟ,
               ତାର୍କିକ use_class_zero = ସତ୍ୟ,
               ଅଚଳ Coverage* glyph_filter = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    ଯଦି (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      ପାଇଁ (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	ଯଦି (!klass) ଜାରି;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	ଯଦି (new_gid == HB_MAP_ମୂଲ୍ୟ_INVALID) ଜାରି;
	ଯଦି (glyph_filter && !glyph_filter->has (g)) ଜାରି;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    ଅନ୍ୟଥା
    {
      unsigned ଅଙ୍କ_source_glyphs = c->plan->source->get_ଅଙ୍କ_glyphs ();
      ପାଇଁ (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	ଯଦି (!klass) ଜାରି;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, ଅଙ୍କ_source_glyphs);
	ପାଇଁ (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  ଯଦି (new_gid == HB_MAP_ମୂଲ୍ୟ_INVALID) ଜାରି;
	  ଯଦି (glyph_filter && !glyph_filter->has (g)) ଜାରି;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    ଅଚଳ hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_ଗଣନା = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_ଗଣନା <= glyph_and_klass.length;
    ଯଦି (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (ମିଥ୍ୟା);
    return_trace (keep_empty_table || (ତାର୍କିକ) glyph_and_klass);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () ଅଚଳ { ଫେରନ୍ତୁ hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  ତାର୍କିକ collect_coverage (set_t *glyphs) ଅଚଳ
  {
    ପାଇଁ (auto &range : rangeRecord)
      ଯଦି (range.value)
	ଯଦି (unlikely (!range.collect_coverage (glyphs)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename set_t>
  ତାର୍କିକ collect_class (set_t *glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass) ଅଚଳ
  {
    ପାଇଁ (auto &range : rangeRecord)
    {
      ଯଦି (range.value == klass)
	ଯଦି (unlikely (!range.collect_coverage (glyphs)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଯଦି (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      ପାଇଁ (auto g : *glyphs)
        ଯଦି (get_class (g))
	  ଫେରନ୍ତୁ ସତ୍ୟ;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ଅଚଳ RangeRecord<Types> &range) { ଫେରନ୍ତୁ range.intersects (*glyphs) && range.value; }));
  }
  ତାର୍କିକ intersects_class (ଅଚଳ hb_set_t *glyphs, unsigned klass) ଅଚଳ
  {
    ଯଦି (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      ଯଦି (!rangeRecord.len)
        ଫେରନ୍ତୁ !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_ମୂଲ୍ୟ_INVALID;
      hb_codepoint_t last = HB_SET_ମୂଲ୍ୟ_INVALID;
      auto it = hb_iter (rangeRecord);
      ପାଇଁ (auto &range : it)
      {
        ଯଦି (it->first == last + 1)
	{
	  it++;
	  ଜାରି;
	}

	ଯଦି (!glyphs->next (&g))
	  ଭାଙ୍ଗ;
	ଯଦି (g < range.first)
	  ଫେରନ୍ତୁ ସତ୍ୟ;
	g = range.last;
	last = g;
      }
      ଯଦି (g != HB_SET_ମୂଲ୍ୟ_INVALID && glyphs->next (&g))
	ଫେରନ୍ତୁ ସତ୍ୟ;
      /* Fall through. */
    }
    ପାଇଁ (ଅଚଳ auto &range : rangeRecord)
      ଯଦି (range.value == klass && range.intersects (*glyphs))
	ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ intersected_class_glyphs (ଅଚଳ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ଅଚଳ
  {
    ଯଦି (klass == 0)
    {
      hb_codepoint_t g = HB_SET_ମୂଲ୍ୟ_INVALID;
      ପାଇଁ (auto &range : rangeRecord)
      {
	ଯଦି (!glyphs->next (&g))
	  ଯାଅ done;
	ଯେତେବେଳେ (g < range.first)
	{
	  intersect_glyphs->add (g);
	  ଯଦି (!glyphs->next (&g))
	    ଯାଅ done;
        }
        g = range.last;
      }
      ଯେତେବେଳେ (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      ଫେରନ୍ତୁ;
    }

    unsigned ଗଣନା_2 = rangeRecord.len;
    ଯଦି (ଗଣନା_2 > glyphs->get_population () * hb_bit_storage (ଗଣନା_2))
    {
      ପାଇଁ (auto g : *glyphs)
      {
        unsigned i;
        ଯଦି (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      ଫେରନ୍ତୁ;
    }

    ପାଇଁ (auto &range : rangeRecord)
    {
      ଯଦି (range.value != klass) ଜାରି;

      unsigned end = range.last + 1;
      ପାଇଁ (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  ଶୂନ୍ୟ intersected_classes (ଅଚଳ hb_set_t *glyphs, hb_set_t *intersect_classes) ଅଚଳ
  {
    ଯଦି (glyphs->is_empty ()) ଫେରନ୍ତୁ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    ଯଦି (!rangeRecord.len)
    {
      intersect_classes->add (0);
      ଫେରନ୍ତୁ;
    }

    hb_codepoint_t g = HB_SET_ମୂଲ୍ୟ_INVALID;
    ପାଇଁ (auto &range : rangeRecord)
    {
      ଯଦି (!glyphs->next (&g))
        ଭାଙ୍ଗ;
      ଯଦି (g < range.first)
      {
        intersect_classes->add (0);
        ଭାଙ୍ଗ;
      }
      g = range.last;
    }
    ଯଦି (g != HB_SET_ମୂଲ୍ୟ_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    ପାଇଁ (ଅଚଳ auto& range : rangeRecord)
      ଯଦି (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

ସଂରଚନା ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ get (k); }
  ତାର୍କିକ has (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ (*ଏହି)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ get (k); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ get_class (k); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_class (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_class (glyph_id);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_class (glyph_id);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_class (glyph_id);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ଅଚଳ
  {
    unsigned klass;
    ଯଦି (cache && cache->get (glyph_id, &klass)) ଫେରନ୍ତୁ klass;
    klass = get_class (glyph_id);
    ଯଦି (cache) cache->set (glyph_id, klass);
    ଫେରନ୍ତୁ klass;
  }

  unsigned get_population () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_population ();
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_population ();
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_population ();
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    ଯଦି (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned ଅଙ୍କ_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      ପାଇଁ (ଅଚଳ auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	ଯଦି (cur_klass > class_max) class_max = cur_klass;
	ଯଦି (cur_gid == glyph_min) ଜାରି;
        ଯଦି (cur_gid > glyph_max) glyph_max = cur_gid;
	ଯଦି (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  ଅଙ୍କ_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	ଯଦି (size >= best_size) ଫେରନ୍ତୁ;
	best_size = size;
	format = candidate;
      };

      ଯଦି (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      ଯଦି (glyph_max <= 0xFFFFu && ଅଙ୍କ_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) ଅଙ୍କ_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      ଯଦି (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      ଯଦି (glyph_max <= 0xFFFFFFu && ଅଙ୍କ_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) ଅଙ୍କ_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      ଯଦି (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (ମିଥ୍ୟା, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (ମିଥ୍ୟା);
      }
    }

    u.format.v = format;

    ବାଛ (u.format.v)
    {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               ତାର୍କିକ keep_empty_table = ସତ୍ୟ,
               ତାର୍କିକ use_class_zero = ସତ୍ୟ,
               ଅଚଳ Coverage* glyph_filter = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  unsigned cost () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.cost ();
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.cost ();
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.cost ();
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  ତାର୍କିକ collect_coverage (set_t *glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.collect_coverage (glyphs);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.collect_coverage (glyphs);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.collect_coverage (glyphs);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  ତାର୍କିକ collect_class (set_t *glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.collect_class (glyphs, klass);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.collect_class (glyphs, klass);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.collect_class (glyphs, klass);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.intersects (glyphs);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.intersects (glyphs);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.intersects (glyphs);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }
  ତାର୍କିକ intersects_class (ଅଚଳ hb_set_t *glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.intersects_class (glyphs, klass);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.intersects_class (glyphs, klass);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.intersects_class (glyphs, klass);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ଶୂନ୍ୟ intersected_class_glyphs (ଅଚଳ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }

  ଶୂନ୍ୟ intersected_classes (ଅଚଳ hb_set_t *glyphs, hb_set_t *intersect_classes) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.intersected_classes (glyphs, intersect_classes);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.intersected_classes (glyphs, intersect_classes);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }


  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ ଫେରନ୍ତୁ (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
ସଂରଚନା delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  ସଂରଚନା chars_t : hb_vector_t<uint8_t>
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ chars_t& other) ଅଚଳ
    {
      ଫେରନ୍ତୁ as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      ପାଇଁ (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      ଫେରନ୍ତୁ hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (ଅଚଳ chars_t& other) ଅଚଳ
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      ପାଇଁ (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      ଫେରନ୍ତୁ hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (ଅଚଳ delta_row_encoding_t& other_encoding) ଅଚଳ { ଫେରନ୍ତୁ chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*> ବସ୍ତୁଗୁଡ଼ିକ;

  delta_row_encoding_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  delta_row_encoding_t (hb_vector_t<ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*> &&rows, unsigned ଅଙ୍କ_cols)
  {
    ପ୍ରତିଜ୍ଞା (rows);

    ବସ୍ତୁଗୁଡ଼ିକ = std::move (rows);

    ଯଦି (unlikely (!chars.resize (ଅଙ୍କ_cols)))
      ଫେରନ୍ତୁ;

    calculate_chars ();
  }

  ଶୂନ୍ୟ merge (ଅଚଳ delta_row_encoding_t& other)
  {
    ବସ୍ତୁଗୁଡ଼ିକ.alloc (ବସ୍ତୁଗୁଡ଼ିକ.length + other.ବସ୍ତୁଗୁଡ଼ିକ.length);
    ପାଇଁ (auto &row : other.ବସ୍ତୁଗୁଡ଼ିକ)
      add_row (row);

    // Merge chars
    ପ୍ରତିଜ୍ଞା (chars.length == other.chars.length);
    ପାଇଁ (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  ଶୂନ୍ୟ chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  ଶୂନ୍ୟ calculate_chars ()
  {
    ପ୍ରତିଜ୍ଞା (ବସ୍ତୁଗୁଡ଼ିକ);

    ତାର୍କିକ long_ଶବ୍ଦଗୁଡ଼ିକ = ମିଥ୍ୟା;

    ପାଇଁ (auto &row : ବସ୍ତୁଗୁଡ଼ିକ)
    {
      ପ୍ରତିଜ୍ଞା (row->length == chars.length);

      /* 0/1/2 byte encoding */
      ପାଇଁ (unsigned i = 0; i < row->length; i++)
      {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା v =  row->arrayZ[i];
	ଯଦି (v == 0)
	  ଜାରି;
	ଅନ୍ୟଥା ଯଦି (v > 32767 || v < -32768)
	{
	  long_ଶବ୍ଦଗୁଡ଼ିକ = ସତ୍ୟ;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	ଅନ୍ୟଥା ଯଦି (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	ଅନ୍ୟଥା
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    ଯଦି (long_ଶବ୍ଦଗୁଡ଼ିକ)
    {
      // Convert 1s to 2s
      ପାଇଁ (auto &v : chars)
	ଯଦି (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  ତାର୍କିକ is_empty () ଅଚଳ
  { ଫେରନ୍ତୁ !ବସ୍ତୁଗୁଡ଼ିକ; }

  ତାର୍କିକ in_error () ଅଚଳ
  { ଫେରନ୍ତୁ chars.in_error () || ବସ୍ତୁଗୁଡ଼ିକ.in_error (); }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned get_chars_overhead (unsigned ଅଙ୍କ_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    ଫେରନ୍ତୁ c + ଅଙ୍କ_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) ଅଚଳ
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = ବସ୍ତୁଗୁଡ଼ିକ.length;
    ଫେରନ୍ତୁ hb_max (0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) overhead - ଗଣନା_2 * (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) additional_bytes_per_rows);
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା gain_from_merging (ଅଚଳ delta_row_encoding_t& other_encoding) ଅଚଳ
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    ଯଦି (additional_bytes_per_rows > 0)
    {
      ଯଦି (get_gain (additional_bytes_per_rows) == 0)
        ଫେରନ୍ତୁ 0;
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	ଫେରନ୍ତୁ 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା combined_gain = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) overhead + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) other_encoding.overhead;
    combined_gain -= (combined_width - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) width) * ବସ୍ତୁଗୁଡ଼ିକ.length;
    combined_gain -= (combined_width - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) other_encoding.width) * other_encoding.ବସ୍ତୁଗୁଡ଼ିକ.length;
    combined_gain -= get_chars_overhead (combined_columns);

    ଫେରନ୍ତୁ combined_gain;
  }

  ତାର୍କିକ add_row (ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>* row)
  { ଫେରନ୍ତୁ ବସ୍ତୁଗୁଡ଼ିକ.push (row); }

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    ଅଚଳ delta_row_encoding_t *a = (ଅଚଳ delta_row_encoding_t *)pa;
    ଅଚଳ delta_row_encoding_t *b = (ଅଚଳ delta_row_encoding_t *)pb;

    ଯଦି (a->width != b->width)
      ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) a->width - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) b->width;

    ଫେରନ୍ତୁ b->chars.cmp (a->chars);
  }
};

ସଂରଚନା VarRegionAxis
{
  ଦଶମିକ evaluate (ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord) ଅଚଳ
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା peak = peakCoord.to_int ();
    ଯଦି (peak == 0 || coord == peak)
      ଫେରନ୍ତୁ 1.f;
    ଅନ୍ୟଥା ଯଦି (coord == 0) // Faster
      ଫେରନ୍ତୁ 0.f;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    ଯଦି (unlikely (start > peak || peak > end))
      ଫେରନ୍ତୁ 1.f;
    ଯଦି (unlikely (start < 0 && end > 0))
      ଫେରନ୍ତୁ 1.f;

    ଯଦି (coord <= start || end <= coord)
      ଫେରନ୍ତୁ 0.f;

    /* Interpolate */
    ଯଦି (coord < peak)
      ଫେରନ୍ତୁ ଦଶମିକ (coord - start) / (peak - start);
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ ଦଶମିକ (end - coord) / (end - peak);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};
ସଂରଚନା SparseVarRegionAxis
{
  ଦଶମିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len) ଅଚଳ
  {
    unsigned i = axisIndex;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord = i < coord_len ? coords[i] : 0;
    ଫେରନ୍ତୁ axis.evaluate (coord);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ SparseVarRegionAxis *src,
		  ଅଚଳ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->embed (src);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    ଯଦି (axis_map)
    {
      ଯଦି (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (ମିଥ୍ୟା);
      }
      ଯଦି (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (ମିଥ୍ୟା);
    }
    return_trace (ସତ୍ୟ);
  }

  ସର୍ବସାଧାରଣ:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା hb_scalar_cache_t
{
  ବ୍ୟକ୍ତିଗତ:
  ସ୍ଥିର constexpr unsigned STATIC_LENGTH = 128;
  ସ୍ଥିର constexpr ଦଶମିକ INVALID = NAN;

  ସର୍ବସାଧାରଣ:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ଅଚଳ hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (ଅଚଳ hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  ସ୍ଥିର hb_scalar_cache_t *create (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    ଯଦି (!ଗଣନା_2) ଫେରନ୍ତୁ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    ଯଦି (scratch_cache && ଗଣନା_2 <= STATIC_LENGTH)
    {
      scratch_cache->length = ଗଣନା_2;
      scratch_cache->clear ();
      ଫେରନ୍ତୁ scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (ଆକାର (hb_scalar_cache_t) - ଆକାର (static_ମୂଲ୍ୟଗୁଡ଼ିକ) + ଆକାର (static_ମୂଲ୍ୟଗୁଡ଼ିକ[0]) * ଗଣନା_2);
    ଯଦି (unlikely (!cache)) ଫେରନ୍ତୁ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = ଗଣନା_2;
    cache->clear ();

    ଫେରନ୍ତୁ cache;
  }

  ସ୍ଥିର ଶୂନ୍ୟ destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    ଯଦି (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  ଶୂନ୍ୟ clear ()
  {
    auto *values = &static_ମୂଲ୍ୟଗୁଡ଼ିକ[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    ପାଇଁ (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    ପାଇଁ (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  ତାର୍କିକ get (unsigned i, ଦଶମିକ *value) ଅଚଳ
  {
    ଯଦି (unlikely (i >= length))
    {
      *value = 0.f;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    auto *values = &static_ମୂଲ୍ୟଗୁଡ଼ିକ[0];
    auto *cached_ମୂଲ୍ୟ = &values[i];
    ଦଶମିକ v = *cached_ମୂଲ୍ୟ;
    ଯଦି (std::isnan (v))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *value = v;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ set (unsigned i, ଦଶମିକ value)
  {
    ଯଦି (unlikely (i >= length)) ଫେରନ୍ତୁ;
    auto *values = &static_ମୂଲ୍ୟଗୁଡ଼ିକ[0];
    auto *cached_ମୂଲ୍ୟ = &values[i];
    *cached_ମୂଲ୍ୟ = value;
  }

  ସର୍ବସାଧାରଣ:
  unsigned length;
  ବ୍ୟକ୍ତିଗତ:
  mutable hb_atomic_t<ଦଶମିକ> static_ମୂଲ୍ୟଗୁଡ଼ିକ[STATIC_LENGTH];
};

ସଂରଚନା VarRegionList
{
  ବ୍ୟକ୍ତିଗତ:
  ଦଶମିକ evaluate_impl (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region_index,
		       ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len) ଅଚଳ
  {
    ଅଚଳ VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_ଗଣନା_2);
    ଦଶମିକ v = 1.f;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = axis_ଗଣନା_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord = i < coord_len ? coords[i] : 0;
      ଦଶମିକ factor = axes[i].evaluate (coord);
      ଯଦି (factor == 0.f)
      {
	v = 0.f;
	ଭାଙ୍ଗ;
      }
      v *= factor;
    }

    ଫେରନ୍ତୁ v;
  }

  ସର୍ବସାଧାରଣ:
  HB_ALWAYS_INLINE
  ଦଶମିକ evaluate (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region_index,
		  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		  hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    ଯଦି (unlikely (region_index >= region_ଗଣନା))
      ଫେରନ୍ତୁ 0.;

    ଦଶମିକ v;
    ଯଦି (cache && cache->get (region_index, &v))
      ଫେରନ୍ତୁ v;

    v = evaluate_impl (region_index, coords, coord_len);

    ଯଦି (cache)
      cache->set (region_index, v);
    ଫେରନ୍ତୁ v;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_ଗଣନା_2 * region_ଗଣନା));
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  ଅଚଳ hb_vector_t<hb_tag_t>& axis_tags,
                  ଅଚଳ hb_vector_t<ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned axis_ଗଣନା = axis_tags.length;
    unsigned region_ଗଣନା_2 = regions.length;
    ଯଦି (!axis_ଗଣନା || !region_ଗଣନା_2) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (hb_unsigned_mul_overflows (axis_ଗଣନା * region_ଗଣନା_2,
                                             VarRegionAxis::static_size))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    axis_ଗଣନା_2 = axis_ଗଣନା;
    region_ଗଣନା = region_ଗଣନା_2;

    ପାଇଁ (unsigned r = 0; r < region_ଗଣନା_2; r++)
    {
      ଅଚଳ auto& region = regions[r];
      ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        ଯଦି (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->ମଧ୍ୟ);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        ଅନ୍ୟଥା
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        ଯଦି (!var_region_rec.serialize (c))
          return_trace (ମିଥ୍ୟା);
      }
    }
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ VarRegionList *src, ଅଚଳ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    axis_ଗଣନା_2 = src->axis_ଗଣନା_2;
    region_ଗଣନା = region_map.get_population ();
    ଯଦି (unlikely (hb_unsigned_mul_overflows (axis_ଗଣନା_2 * region_ଗଣନା,
					     VarRegionAxis::static_size))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->extend (ଏହି))) return_trace (ମିଥ୍ୟା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region_ଗଣନା_2 = src->region_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 0; r < region_ଗଣନା; r++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା backward = region_map.backward (r);
      ଯଦି (backward >= region_ଗଣନା_2) return_trace (ମିଥ୍ୟା);
      hb_memcpy (&axesZ[axis_ଗଣନା_2 * r], &src->axesZ[axis_ଗଣନା_2 * backward], VarRegionAxis::static_size * axis_ଗଣନା_2);
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ get_var_region (unsigned region_index,
                       ଅଚଳ hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ଅଚଳ
  {
    ଯଦି (region_index >= region_ଗଣନା) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_ଗଣନା_2);
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      hb_tag_t *axis_tag;
      ଯଦି (!axes_old_index_tag_map.has (i, &axis_tag))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଦଶମିକ min_val = axis_region->startCoord.to_float ();
      ଦଶମିକ def_val = axis_region->peakCoord.to_float ();
      ଦଶମିକ max_val = axis_region->endCoord.to_float ();

      ଯଦି (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((ଦ୍ୱିଗୁଣ) min_val, (ଦ୍ୱିଗୁଣ) def_val, (ଦ୍ୱିଗୁଣ) max_val));
      axis_region++;
    }
    ଫେରନ୍ତୁ !axis_tuples.in_error ();
  }

  ତାର୍କିକ get_var_regions (ଅଚଳ hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) ଅଚଳ
  {
    ଯଦି (!regions.alloc (region_ଗଣନା))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned i = 0; i < region_ଗଣନା; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      ଯଦି (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      regions.push (std::move (axis_tuples));
    }
    ଫେରନ୍ତୁ !regions.in_error ();
  }

  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_ଗଣନା_2, region_ଗଣନା)); }

  ସର୍ବସାଧାରଣ:
  HBUINT16	axis_ଗଣନା_2;
  HBUINT15	region_ଗଣନା;
  ସୁରକ୍ଷିତ:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_hb_avar2_region_is_dead (ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  ପାଇଁ (auto _ : axis_tuples)
  {
    Triple *range;
    ଯଦି (!reachable_ranges.has (_.first, &range)) ଜାରି;
    ଅଚଳ Triple &tent = _.second;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) tent.minimum * 16384.f);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା peak  = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) tent.ମଧ୍ୟ * 16384.f);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା end   = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) tent.maximum * 16384.f);
    ଯଦି (!peak) ଜାରି;
    ଯଦି (start > peak || peak > end) ଜାରି;
    ଯଦି (start < 0 && end > 0) ଜାରି;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lo = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) range->minimum * 16384.f);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା hi = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    ଯଦି (hi < start || lo > end)
      ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସଂରଚନା SparseVariationRegion
{
  ଦଶମିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len) ଅଚଳ
  {
    ଦଶମିକ v = 1.f;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = axes.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଦଶମିକ factor = (ଏହି+axes[i]).evaluate (coords, coord_len);
      ଯଦି (factor == 0.f)
	ଫେରନ୍ତୁ 0.;
      v *= factor;
    }
    ଫେରନ୍ତୁ v;
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ SparseVariationRegion *src,
		  ଅଚଳ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (unsigned i = 0; i < axes.len; i++)
      ଯଦି (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (axes.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, axes);
};

ସଂରଚନା SparseVarRegionList
{
  HB_ALWAYS_INLINE
  ଦଶମିକ evaluate (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region_index,
		  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		  hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    ଯଦି (unlikely (region_index >= regions.len))
      ଫେରନ୍ତୁ 0.;

    ଦଶମିକ v;
    ଯଦି (cache && cache->get (region_index, &v))
      ଫେରନ୍ତୁ v;

    ଅଚଳ SparseVariationRegion &region = ଏହି+regions[region_index];

    v = region.evaluate (coords, coord_len);
    ଯଦି (cache)
      cache->set (region_index, v);

    ଫେରନ୍ତୁ v;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (regions.sanitize (c, ଏହି));
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ SparseVarRegionList *src,
		  ଅଚଳ hb_inc_bimap_t &region_map,
		  ଅଚଳ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    regions.len = region_map.get_population ();
    ଯଦି (unlikely (!c->extend (regions))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      ଯଦି (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (ମିଥ୍ୟା);
    }
    return_trace (ସତ୍ୟ);
  }

  ସର୍ବସାଧାରଣ:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, regions);
};


ସଂରଚନା VarData
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_item_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ item_ଗଣନା; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_region_index_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ regionIndices.len; }

  unsigned get_region_index (unsigned i) ଅଚଳ
  { ଫେରନ୍ତୁ i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () ଅଚଳ
  { ଫେରନ୍ତୁ (word_ଗଣନା () + regionIndices.len) * (long_ଶବ୍ଦଗୁଡ଼ିକ_2 () ? 2 : 1); }

  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_ଗଣନା, get_row_size ())); }

  template <typename Float = ଦଶମିକ>
  Float _get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner,
		    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
		    ଅଚଳ VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    ଯଦି (unlikely (inner >= item_ଗଣନା))
      ଫେରନ୍ତୁ 0.;
    ତାର୍କିକ is_long = long_ଶବ୍ଦଗୁଡ଼ିକ_2 ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = regionIndices.len;
    unsigned word_ଗଣନା_2 = word_ଗଣନା ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା scount = is_long ? ଗଣନା_2 : word_ଗଣନା_2;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lcount = is_long ? word_ଗଣନା_2 : 0;

    ଅଚଳ HBUINT8 *bytes = get_delta_bytes ();
    ଅଚଳ HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

    ଅଚଳ HBINT32 *lcursor = reinterpret_cast<ଅଚଳ HBINT32 *> (row);
    ପାଇଁ (; i < lcount; i++)
    {
      ଦଶମିକ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ଗଣନା, cache);
      ଯଦି (scalar)
        delta += (Float) scalar * (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) *lcursor;
      lcursor++;
    }
    ଅଚଳ HBINT16 *scursor = reinterpret_cast<ଅଚଳ HBINT16 *> (lcursor);
    ପାଇଁ (; i < scount; i++)
    {
      ଦଶମିକ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ଗଣନା, cache);
      ଯଦି (scalar)
       delta += (Float) scalar * (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) *scursor;
      scursor++;
    }
    ଅଚଳ HBINT8 *bcursor = reinterpret_cast<ଅଚଳ HBINT8 *> (scursor);
    ପାଇଁ (; i < ଗଣନା_2; i++)
    {
      ଦଶମିକ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ଗଣନା, cache);
      ଯଦି (scalar)
        delta += (Float) scalar * (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) *bcursor;
      bcursor++;
    }

    ଫେରନ୍ତୁ delta;
  }

  template <typename Float = ଦଶମିକ>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner,
		   ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
		   ଅଚଳ VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = regionIndices.len;
    ଯଦି (!ଗଣନା_2) ଫେରନ୍ତୁ Float (0); // This is quite common, so optimize it.
    ଫେରନ୍ତୁ _get_delta<Float> (inner, coords, coord_ଗଣନା, regions, cache);
  }

  ଶୂନ୍ୟ get_region_scalars (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
			   ଅଚଳ VarRegionList &regions,
			   ଦଶମିକ *scalars /*OUT */,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_scalars) ଅଚଳ
  {
    unsigned ଗଣନା_2 = hb_min (ଅଙ୍କ_scalars, regionIndices.len);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ଗଣନା);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଗଣନା_2; i < ଅଙ୍କ_scalars; i++)
      scalars[i] = 0.f;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_ଗଣନା () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_ଗଣନା,
				  get_row_size ()));
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  ତାର୍କିକ has_long,
                  ଅଚଳ hb_vector_t<ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>*>& rows)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned row_ଗଣନା_2 = rows.length;
    ଯଦି (!row_ଗଣନା_2) {
      // Nothing to serialize, will be empty.
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->check_assign (item_ଗଣନା, row_ଗଣନା_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ମିଥ୍ୟା);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_threshold = has_long ? -32768 : -128;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_threshold = has_long ? +32767 : +127;
    ଗଣନା delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned ଅଙ୍କ_regions = rows[0]->length;
    ଯଦି (!delta_sz.resize (ଅଙ୍କ_regions))
      return_trace (ମିଥ୍ୟା);

    unsigned word_ଗଣନା_2 = 0;
    ପାଇଁ (unsigned r = 0; r < ଅଙ୍କ_regions; r++)
    {
      ପାଇଁ (unsigned i = 0; i < row_ଗଣନା_2; i++)
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = rows[i]->arrayZ[r];
        ଯଦି (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_ଗଣନା_2++;
          ଭାଙ୍ଗ;
        }
        ଅନ୍ୟଥା ଯଦି (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_ଗଣନା_2;
    hb_map_t ri_map;
    ପାଇଁ (unsigned r = 0; r < ଅଙ୍କ_regions; r++)
    {
      ଯଦି (!delta_sz[r]) ଜାରି;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      ଯଦି (!ri_map.set (new_r, r))
        return_trace (ମିଥ୍ୟା);
    }

    word_Size_ଗଣନା = word_ଗଣନା_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_ଗଣନା = ri_map.get_population ();
    regionIndices.len = ri_ଗଣନା;
    ଯଦି (unlikely (!c->extend (ଏହି))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (unsigned r = 0; r < ri_ଗଣନା; r++)
    {
      hb_codepoint_t *idx;
      ଯଦି (!ri_map.has (r, &idx))
        return_trace (ମିଥ୍ୟା);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < row_ଗଣନା_2; i++)
    {
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 0; r < ri_ଗଣନା; r++)
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ VarData *src,
		  ଅଚଳ hb_inc_bimap_t &inner_map,
		  ଅଚଳ hb_inc_bimap_t &region_map,
		  ଅଚଳ hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    item_ଗଣନା = inner_map.get_next_ମୂଲ୍ୟ ();

    /* Optimize word count */
    unsigned ri_ଗଣନା = src->regionIndices.len;
    ଗଣନା delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_ଗଣନା);
    ri_map.resize (ri_ଗଣନା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_word_ଗଣନା = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

    ଅଚଳ HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_ଗଣନା = src->word_ଗଣନା ();
    ତାର୍କିକ     src_long_ଶବ୍ଦଗୁଡ଼ିକ = src->long_ଶବ୍ଦଗୁଡ଼ିକ_2 ();

    ତାର୍କିକ has_long = ମିଥ୍ୟା;
    ଯଦି (src_long_ଶବ୍ଦଗୁଡ଼ିକ)
    {
      ପାଇଁ (r = 0; r < src_word_ଗଣନା; r++)
      {
        ପାଇଁ (unsigned old_gid : inner_map.ଚାବିଗୁଡ଼ିକ())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  ଯଦି (delta < -32768 || 32767 < delta)
	  {
	    has_long = ସତ୍ୟ;
	    ଭାଙ୍ଗ;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    ପାଇଁ (r = 0; r < ri_ଗଣନା; r++)
    {
      ତାର୍କିକ short_circuit = src_long_ଶବ୍ଦଗୁଡ଼ିକ == has_long && src_word_ଗଣନା <= r;

      delta_sz[r] = kZero;
      ଯଦି (culled_regions && culled_regions->has (src->regionIndices[r]))
	ଜାରି; /* unreachable region: drop its column */
      ପାଇଁ (unsigned old_gid : inner_map.ଚାବିଗୁଡ଼ିକ())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	ଯଦି (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_ଗଣନା++;
	  ଭାଙ୍ଗ;
	}
	ଅନ୍ୟଥା ଯଦି (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  ଯଦି (short_circuit)
	    ଭାଙ୍ଗ;
	}
      }
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା word_index = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା non_word_index = new_word_ଗଣନା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_ri_ଗଣନା = 0;
    ପାଇଁ (r = 0; r < ri_ଗଣନା; r++)
      ଯଦି (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_ଗଣନା++;
      }

    word_Size_ଗଣନା = new_word_ଗଣନା | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_ଗଣନା;

    ଯଦି (unlikely (!c->extend (ଏହି))) return_trace (ମିଥ୍ୟା);

    ପାଇଁ (r = 0; r < new_ri_ଗଣନା; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned ଗଣନା_2 = item_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା old = inner_map.backward (i);
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 0; r < new_ri_ଗଣନା; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_region_refs (hb_set_t &region_indices, ଅଚଳ hb_inc_bimap_t &inner_map) ଅଚଳ
  {
    ଅଚଳ HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = 0; r < regionIndices.len; r++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region = regionIndices.arrayZ[r];
      ଯଦି (region_indices.has (region)) ଜାରି;
      ପାଇଁ (hb_codepoint_t old_gid : inner_map.ଚାବିଗୁଡ଼ିକ())
	ଯଦି (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  ଭାଙ୍ଗ;
	}
    }
  }

  ସର୍ବସାଧାରଣ:
  ଅଚଳ HBUINT8 *get_delta_bytes () ଅଚଳ
  { ଫେରନ୍ତୁ &StructAfter<HBUINT8> (regionIndices); }

  ସୁରକ୍ଷିତ:
  HBUINT8 *get_delta_bytes ()
  { ଫେରନ୍ତୁ &StructAfter<HBUINT8> (regionIndices); }

  ସର୍ବସାଧାରଣ:
  int32_t get_item_delta_fast (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା item, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region,
			       ଅଚଳ HBUINT8 *delta_bytes, unsigned row_size) ଅଚଳ
  {
    ଯଦି (unlikely (item >= item_ଗଣନା || region >= regionIndices.len)) ଫେରନ୍ତୁ 0;

    ଅଚଳ HBINT8 *p = (ଅଚଳ HBINT8 *) delta_bytes + item * row_size;
    unsigned word_ଗଣନା_2 = word_ଗଣନା ();
    ତାର୍କିକ is_long = long_ଶବ୍ଦଗୁଡ଼ିକ_2 ();
    ଯଦି (is_long)
    {
      ଯଦି (region < word_ଗଣନା_2)
	ଫେରନ୍ତୁ ((ଅଚଳ HBINT32 *) p)[region];
      ଅନ୍ୟଥା
	ଫେରନ୍ତୁ ((ଅଚଳ HBINT16 *)(p + HBINT32::static_size * word_ଗଣନା_2))[region - word_ଗଣନା_2];
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (region < word_ଗଣନା_2)
	ଫେରନ୍ତୁ ((ଅଚଳ HBINT16 *) p)[region];
      ଅନ୍ୟଥା
	ଫେରନ୍ତୁ (p + HBINT16::static_size * word_ଗଣନା_2)[region - word_ଗଣନା_2];
    }
  }
  int32_t get_item_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା item, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region) ଅଚଳ
  {
     ଫେରନ୍ତୁ get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  ସୁରକ୍ଷିତ:
  ଶୂନ୍ୟ set_item_delta_fast (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା item, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_ଗଣନା_2 = word_ଗଣନା ();
    ତାର୍କିକ is_long = long_ଶବ୍ଦଗୁଡ଼ିକ_2 ();
    ଯଦି (is_long)
    {
      ଯଦି (region < word_ଗଣନା_2)
	((HBINT32 *) p)[region] = delta;
      ଅନ୍ୟଥା
	((HBINT16 *)(p + HBINT32::static_size * word_ଗଣନା_2))[region - word_ଗଣନା_2] = delta;
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (region < word_ଗଣନା_2)
	((HBINT16 *) p)[region] = delta;
      ଅନ୍ୟଥା
	(p + HBINT16::static_size * word_ଗଣନା_2)[region - word_ଗଣନା_2] = delta;
    }
  }
  ଶୂନ୍ୟ set_item_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା item, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  ତାର୍କିକ long_ଶବ୍ଦଗୁଡ଼ିକ_2 () ଅଚଳ { ଫେରନ୍ତୁ word_Size_ଗଣନା & 0x8000u /* LONG_WORDS */; }
  unsigned word_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ word_Size_ଗଣନା & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  ସୁରକ୍ଷିତ:
  HBUINT16		item_ଗଣନା;
  HBUINT16		word_Size_ଗଣନା;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

ସଂରଚନା MultiVarData
{
  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  ଅଚଳ TupleList &get_delta_sets () ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  ଶୂନ୍ୟ get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner,
		  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
		  ଅଚଳ SparseVarRegionList &regions,
		  hb_array_t<ଦ୍ୱିଗୁଣ> out,
		  hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    auto &deltaSets = get_delta_sets ();

    auto ମୂଲ୍ୟଗୁଡ଼ିକ_iter = deltaSets.fetcher (inner);
    unsigned region_ଗଣନା = regionIndices.len;
    unsigned skip = 0;
    ପାଇଁ (unsigned regionIndex = 0; regionIndex < region_ଗଣନା; regionIndex++)
    {
      ଦଶମିକ scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_ଗଣନା,
				       cache);
      // We skip lazily. Helps with the tail end.
      ଯଦି (scalar == 0.0f)
        skip += out.length;
      ଅନ୍ୟଥା
      {
        ଯଦି (skip)
	{
	  ମୂଲ୍ୟଗୁଡ଼ିକ_iter.skip (skip);
	  skip = 0;
	}
	ମୂଲ୍ୟଗୁଡ଼ିକ_iter.add_to (out, scalar);
      }
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, ଏହି));
  }

  unsigned get_item_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ get_delta_sets ().ଗଣନା_2; }

  ଶୂନ୍ୟ collect_region_refs (hb_set_t &region_indices) ଅଚଳ
  {
    ପାଇଁ (unsigned region : regionIndices)
      region_indices.add (region);
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ MultiVarData *src,
		  ଅଚଳ hb_inc_bimap_t &inner_map,
		  ଅଚଳ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned region_ଗଣନା_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_ଗଣନା_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    ଯଦି (unlikely (!c->extend_size (ଏହି, header_size, ମିଥ୍ୟା)))
      return_trace (ମିଥ୍ୟା);
    format = 1;
    regionIndices.len = region_ଗଣନା_2;
    ପାଇଁ (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      ଯଦି (unlikely (!region_map.has (old_region)))
	return_trace (ମିଥ୍ୟା);
      regionIndices[i] = region_map.get (old_region);
    }

    ଅଚଳ auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    ଯଦି (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (ମିଥ୍ୟା);
    ପାଇଁ (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      ଯଦି (unlikely (old_inner >= src_delta_sets.ଗଣନା_2)) return_trace (ମିଥ୍ୟା);
      hb_ubytes_t bytes = static_cast<ଅଚଳ CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    ଯଦି (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (ମିଥ୍ୟା);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  ସୁରକ୍ଷିତ:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (7);
};

ସଂରଚନା ItemVariationStore
{
  friend ସଂରଚନା item_variations_t;

  hb_scalar_cache_t *create_cache () ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ hb_scalar_cache_t::create (0);
#endif
    ଫେରନ୍ତୁ hb_scalar_cache_t::create ((ଏହି+regions).region_ଗଣନା);
  }

  ସ୍ଥିର ଶୂନ୍ୟ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  ବ୍ୟକ୍ତିଗତ:
  template <typename Float = ଦଶମିକ>
  Float get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner,
		   ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
		   hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ Float (0);
#endif

    ଯଦି (unlikely (outer >= dataSets.len))
      ଫେରନ୍ତୁ Float (0);

    ଫେରନ୍ତୁ (ଏହି+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_ଗଣନା,
					     ଏହି+regions,
					     cache);
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ has_delta_set (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer = index >> 16;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner = index & 0xFFFF;
    ଫେରନ୍ତୁ outer < dataSets.len &&
	   inner < (ଏହି+dataSets[outer]).get_item_ଗଣନା ();
  }

  template <typename Float = ଦଶମିକ>
  Float get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index,
		   ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
		   hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer = index >> 16;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner = index & 0xFFFF;
    ଫେରନ୍ତୁ get_delta<Float> (outer, inner, coords, coord_ଗଣନା, cache);
  }
  template <typename Float = ଦଶମିକ>
  Float get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index,
		   hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
		   hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    ଫେରନ୍ତୁ get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ ସତ୍ୟ;
#endif

    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ଏହି) &&
		  dataSets.sanitize (c, ଏହି));
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  ତାର୍କିକ has_long,
                  ଅଚଳ hb_vector_t<hb_tag_t>& axis_tags,
                  ଅଚଳ hb_vector_t<ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  ଅଚଳ hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (ଏହି);
#ifdef HB_NO_VAR
    return_trace (ମିଥ୍ୟା);
#endif
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    format = 1;
    ଯଦି (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (ମିଥ୍ୟା);

    unsigned ଅଙ୍କ_var_data = vardata_encodings.length;
    ଯଦି (!ଅଙ୍କ_var_data) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->check_assign (dataSets.len, ଅଙ୍କ_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!c->extend (dataSets))) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_var_data; i++)
      ଯଦି (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].ବସ୍ତୁଗୁଡ଼ିକ))
        return_trace (ମିଥ୍ୟା);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ ItemVariationStore *src,
		  ଅଚଳ hb_array_t <ଅଚଳ hb_inc_bimap_t> &inner_maps,
		  ଅଚଳ hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (ଏହି);
#ifdef HB_NO_VAR
    return_trace (ମିଥ୍ୟା);
#endif

    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_ଗଣନା = 0;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < inner_maps.length; i++)
      ଯଦି (inner_maps[i].get_population ())
	set_ଗଣନା++;

    format = 1;

    ଅଚଳ auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    ଯଦି (region_indices.in_error ())
      return_trace (ମିଥ୍ୟା);

    region_indices.del_range ((src_regions).region_ଗଣନା, hb_set_t::INVALID);

    ଯଦି (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    ଯଦି (region_map.in_error())
      return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (ମିଥ୍ୟା);

    dataSets.len = set_ଗଣନା;
    ଯଦି (unlikely (!c->extend (dataSets))) return_trace (ମିଥ୍ୟା);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_index = 0;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < inner_maps.length; i++)
    {
      ଯଦି (!inner_maps[i].get_population ()) ଜାରି;
      ଯଦି (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->start_embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned ଗଣନା_2 = dataSets.len;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      ଯଦି (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = ଏହି+dataSets[i];

      unsigned item_ଗଣନା = data.get_item_ଗଣନା ();
      ପାଇଁ (unsigned j = 0; j < item_ଗଣନା; j++)
	map->add (j);
    }

    ଯଦି (unlikely (!out->serialize (c, ଏହି, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  ତାର୍କିକ collect_dead_regions (ଅଚଳ hb_map_t &axes_old_index_tag_map,
			     ଅଚଳ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) ଅଚଳ
  {
#ifndef HB_NO_VAR
    ଯଦି (!reachable_ranges.get_population ()) ଫେରନ୍ତୁ ସତ୍ୟ;
    ଅଚଳ VarRegionList &region_list = ଏହି+regions;
    unsigned ଗଣନା_2 = region_list.region_ଗଣନା;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      ଯଦି (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    ଫେରନ୍ତୁ !dead_regions.in_error ();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
  }

  ତାର୍କିକ subset (hb_subset_context_t *c, ଅଚଳ hb_array_t<ଅଚଳ hb_inc_bimap_t> &inner_maps) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
#ifdef HB_NO_VAR
    return_trace (ମିଥ୍ୟା);
#endif

    ItemVariationStore *varstore_ମୌଳିକ = c->serializer->start_embed<ItemVariationStore> ();
    ଯଦି (unlikely (!varstore_ମୌଳିକ)) return_trace (ମିଥ୍ୟା);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    ଯଦି (c->plan->has_avar2)
      ଯଦି (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (ମିଥ୍ୟା);

    varstore_ମୌଳିକ->serialize (c->serializer, ଏହି, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_ମୌଳିକ->dataSets);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_region_index_ଗଣନା (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ 0;
#endif
    ଫେରନ୍ତୁ (ଏହି+dataSets[major]).get_region_index_ଗଣନା ();
  }

  ଶୂନ୍ୟ get_region_scalars (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major,
			   ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
			   ଦଶମିକ *scalars /*OUT*/,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_scalars) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_scalars; i++)
      scalars[i] = 0.f;
    ଫେରନ୍ତୁ;
#endif

    (ଏହି+dataSets[major]).get_region_scalars (coords, coord_ଗଣନା,
					       ଏହି+regions,
					       &scalars[0], ଅଙ୍କ_scalars);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_sub_table_ଗଣନା () ଅଚଳ
   {
#ifdef HB_NO_VAR
     ଫେରନ୍ତୁ 0;
#endif
     ଫେରନ୍ତୁ dataSets.len;
   }

  ଅଚଳ VarData& get_sub_table (unsigned i) ଅଚଳ
  {
#ifdef HB_NO_VAR
     ଫେରନ୍ତୁ Null (VarData);
#endif
     ଫେରନ୍ତୁ ଏହି+dataSets[i];
  }

  ଅଚଳ VarRegionList& get_region_list () ଅଚଳ
  {
#ifdef HB_NO_VAR
     ଫେରନ୍ତୁ Null (VarRegionList);
#endif
     ଫେରନ୍ତୁ ଏହି+regions;
  }

  ସୁରକ୍ଷିତ:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

ସଂରଚନା MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ hb_scalar_cache_t::create (0);
#endif
    auto &r = ଏହି+regions;
    unsigned ଗଣନା_2 = r.regions.len;

    ଫେରନ୍ତୁ hb_scalar_cache_t::create (ଗଣନା_2, static_cache);
  }

  ସ୍ଥିର ଶୂନ୍ୟ destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  ବ୍ୟକ୍ତିଗତ:
  ଶୂନ୍ୟ get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner,
		  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
		  hb_array_t<ଦ୍ୱିଗୁଣ> out,
		  hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ;
#endif

    ଯଦି (unlikely (outer >= dataSets.len))
      ଫେରନ୍ତୁ;

    ଫେରନ୍ତୁ (ଏହି+dataSets[outer]).get_delta (inner,
					     coords, coord_ଗଣନା,
					     ଏହି+regions,
					     out,
					     cache);
  }

  ସର୍ବସାଧାରଣ:
  ଶୂନ୍ୟ get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index,
		  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା,
		  hb_array_t<ଦ୍ୱିଗୁଣ> out,
		  hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer = index >> 16;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_ଗଣନା, out, cache);
  }
  ଶୂନ୍ୟ get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index,
		  hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
		  hb_array_t<ଦ୍ୱିଗୁଣ> out,
		  hb_scalar_cache_t *cache = nullptr) ଅଚଳ
  {
    ଫେରନ୍ତୁ get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
#ifdef HB_NO_VAR
    ଫେରନ୍ତୁ ସତ୍ୟ;
#endif

    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ଏହି) &&
		  dataSets.sanitize (c, ଏହି));
  }

  ତାର୍କିକ create_subset_plan (ଅଚଳ hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ଅଚଳ
  {
    ଯଦି (unlikely (!inner_maps->resize (dataSets.len))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      ଯଦି (unlikely (outer >= dataSets.len ||
		    inner >= (ଏହି+dataSets[outer]).get_item_ଗଣନା ()))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    ପାଇଁ (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      ଅଚଳ hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      ଯଦି (!inner_map.get_population ()) ଜାରି;
      ଯଦି (unlikely (inner_map.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ପାଇଁ (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    ଫେରନ୍ତୁ !inner_maps->in_error () && !varidx_map->in_error ();
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ MultiItemVariationStore *src,
		  ଅଚଳ hb_array_t<ଅଚଳ hb_inc_bimap_t> &inner_maps,
		  ଅଚଳ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (ମିଥ୍ୟା);
    format = 1;

    ଅଚଳ auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_ଗଣନା = 0;
    ପାଇଁ (unsigned i = 0; i < src->dataSets.len; i++)
    {
      ଯଦି (!inner_maps[i].get_population ()) ଜାରି;
      set_ଗଣନା++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    ଯଦି (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (ମିଥ୍ୟା);

    dataSets.len = set_ଗଣନା;
    ଯଦି (unlikely (!c->extend (dataSets))) return_trace (ମିଥ୍ୟା);
    unsigned set_index = 0;
    ପାଇଁ (unsigned i = 0; i < src->dataSets.len; i++)
    {
      ଯଦି (!inner_maps[i].get_population ()) ଜାରି;
      ଯଦି (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (ମିଥ୍ୟା);
    }
    return_trace (ସତ୍ୟ);
  }

  ସୁରକ୍ଷିତ:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_ଗଣନା_T>
ସଂରଚନା DeltaSetIndexMapFormat01
{
  friend ସଂରଚନା DeltaSetIndexMap;

  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (map_ଗଣନା_2, get_width (), min_size); }

  ବ୍ୟକ୍ତିଗତ:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed (ଏହି));
  }

  template <typename T>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ T &plan)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = plan.get_width ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner_bit_ଗଣନା = plan.get_inner_bit_ଗଣନା ();
    ଅଚଳ hb_array_t<ଅଚଳ uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (output_map.length && ((((inner_bit_ଗଣନା-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    entryFormat = ((width-1)<<4)|(inner_bit_ଗଣନା-1);
    map_ଗଣନା_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    ଯଦି (unlikely (!p)) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < output_map.length; i++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = output_map.arrayZ[i];
      ଯଦି (v)
      {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer = v >> 16;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner = v & 0xFFFF;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା u = (outer << inner_bit_ଗଣନା) | inner;
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (ସତ୍ୟ);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v) ଅଚଳ /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    ଯଦି (!map_ଗଣନା_2)
      ଫେରନ୍ତୁ v;
    ଫେରନ୍ତୁ _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v) ଅଚଳ /* Returns 16.16 outer.inner. */
  {
    ଯଦି (v >= map_ଗଣନା_2)
      v = map_ଗଣନା_2 - 1;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା u = 0;
    { /* Fetch it. */
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = get_width ();
      ଅଚଳ HBUINT8 *p = mapDataZ.arrayZ + w * v;
      ପାଇଁ (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = get_inner_bit_ଗଣନା ();
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outer = u >> n;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    ଫେରନ୍ତୁ u;
  }

  unsigned get_map_ଗଣନା () ଅଚଳ       { ଫେରନ୍ତୁ map_ଗଣନା_2; }
  unsigned get_width () ଅଚଳ           { ଫେରନ୍ତୁ ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ (entryFormat & 0xF) + 1; }


  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_ଗଣନା_2,
                                  get_width ()));
  }

  ସୁରକ୍ଷିତ:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_ଗଣନା_T     map_ଗଣନା_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2+Map_ଗଣନା_T::static_size, mapDataZ);
};

ସଂରଚନା DeltaSetIndexMap
{
  template <typename T>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ T &plan)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  uint32_t map (unsigned v) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ (u.format0.map (v));
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ (u.format1.map (v));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ v;
    }
  }

  unsigned get_map_ଗଣନା () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_map_ଗଣନା ();
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_map_ଗଣନା ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  unsigned get_width () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_width ();
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_width ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  unsigned get_inner_bit_ଗଣନା () ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_inner_bit_ଗଣନା ();
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_inner_bit_ଗଣନା ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    ସ୍ଥିତି 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (nullptr);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (1, format.v);
};


ସଂରଚନା ItemVarStoreInstancer
{
  ItemVarStoreInstancer (ଅଚଳ ItemVariationStore *varStore_,
			 ଅଚଳ DeltaSetIndexMap *varIdxMap,
			 hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    ଯଦି (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    ଯଦି (!hb_any (coords))
      ଏହି->coords = {};
  }

  operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ varStore && ତାର୍କିକ (coords); }

  ଦଶମିକ operator[] (uint32_t varIdx) ଅଚଳ
  { ଫେରନ୍ତୁ (*ଏହି) (varIdx); }

  ଦଶମିକ operator() (uint32_t varIdx, unsigned short offset = 0) ଅଚଳ
  { ଫେରନ୍ତୁ get_delta (varIdx, offset); }

  ଦ୍ୱିଗୁଣ get_condition_delta (uint32_t varIdx) ଅଚଳ
  { ଫେରନ୍ତୁ get_delta<ଦ୍ୱିଗୁଣ> (varIdx); }

  template <typename Float = ଦଶମିକ>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) ଅଚଳ
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   ଯଦି (!coords || varIdx == VarIdx::NO_VARIATION)
     ଫେରନ୍ତୁ Float (0);

    varIdx += offset;
    ଯଦି (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    ଫେରନ୍ତୁ varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ଅଚଳ ItemVariationStore *varStore;
  ଅଚଳ DeltaSetIndexMap *varIdxMap;
  hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords;
  hb_scalar_cache_t *cache;
};

ସଂରଚନା MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (ଅଚଳ MultiItemVariationStore *varStore,
			      ଅଚଳ DeltaSetIndexMap *varIdxMap,
			      hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    ଯଦି (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ varStore; }

  ଦ୍ୱିଗୁଣ get_condition_delta (uint32_t varIdx) ଅଚଳ
  { ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ) (*ଏହି)[varIdx]; }

  ଦ୍ୱିଗୁଣ operator[] (uint32_t varIdx) ଅଚଳ
  {
    ଦ୍ୱିଗୁଣ v = 0;
    (*ଏହି) (hb_array (&v, 1), varIdx);
    ଫେରନ୍ତୁ v;
  }

  ଶୂନ୍ୟ operator() (hb_array_t<ଦ୍ୱିଗୁଣ> out, uint32_t varIdx, unsigned short offset = 0) ଅଚଳ
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    ଯଦି (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      ଯଦି (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    ଅନ୍ୟଥା
      ପାଇଁ (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  ଅଚଳ MultiItemVariationStore *varStore;
  ଅଚଳ DeltaSetIndexMap *varIdxMap;
  hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
ଗଣନା Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

ଗଣନା lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

ସଂରଚନା Condition;

template <typename Instancer>
ସ୍ଥିର ତାର୍କିକ
_hb_recurse_condition_evaluate (ଅଚଳ ସଂରଚନା Condition &condition,
				ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
				Instancer *instancer);

ସଂରଚନା ConditionAxisRange
{
  friend ସଂରଚନା Condition;

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_map_t *index_map = &c->plan->axes_index_map;
    ଯଦି (index_map->is_empty ()) return_trace (ସତ୍ୟ);

    ଅଚଳ hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    ଯଦି (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    ଯଦି (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    ଅଚଳ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    ଯଦି (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    ଦଶମିକ normalized_min = renormalize_ମୂଲ୍ୟ ((ଦ୍ୱିଗୁଣ) filter_Range_Min_ମୂଲ୍ୟ.to_float (), axis_limit, axis_triple_distances, ମିଥ୍ୟା);
    ଦଶମିକ normalized_max = renormalize_ମୂଲ୍ୟ ((ଦ୍ୱିଗୁଣ) filter_Range_Max_ମୂଲ୍ୟ.to_float (), axis_limit, axis_triple_distances, ମିଥ୍ୟା);
    out->filter_Range_Min_ମୂଲ୍ୟ.set_float (normalized_min);
    out->filter_Range_Max_ମୂଲ୍ୟ.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ବ୍ୟକ୍ତିଗତ:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) ଅଚଳ
  {
    //invalid axis index, drop the entire record
    ଯଦି (!c->axes_index_tag_map->has (axisIndex))
      ଫେରନ୍ତୁ DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    ତାର୍କିକ axis_set_by_user = ମିଥ୍ୟା;
    ଯଦି (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = ସତ୍ୟ;
    }

    ଦଶମିକ axis_min_val = axis_range.minimum;
    ଦଶମିକ axis_default_val = axis_range.ମଧ୍ୟ;
    ଦଶମିକ axis_max_val = axis_range.maximum;

    ଦଶମିକ filter_min_val = filter_Range_Min_ମୂଲ୍ୟ.to_float ();
    ଦଶମିକ filter_max_val = filter_Range_Max_ମୂଲ୍ୟ.to_float ();

    ଯଦି (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = ମିଥ୍ୟା;

    //condition not met, drop the entire record
    ଯଦି (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      ଫେରନ୍ତୁ DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    ଯଦି (axis_set_by_user && axis_range.is_point ())
      ଫେରନ୍ତୁ DROP_COND_WITH_VAR;

    ଯଦି (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_ମୂଲ୍ୟ.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_ମୂଲ୍ୟ.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      ଫେରନ୍ତୁ KEEP_COND_WITH_VAR;
    }
    ଫେରନ୍ତୁ KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  ତାର୍କିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		 Instancer *instancer HB_UNUSED) ଅଚଳ
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    ଫେରନ୍ତୁ filter_Range_Min_ମୂଲ୍ୟ.to_int () <= coord && coord <= filter_Range_Max_ମୂଲ୍ୟ.to_int ();
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_ମୂଲ୍ୟ;
  F2DOT14	filter_Range_Max_ମୂଲ୍ୟ;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା Condition_ମୂଲ୍ୟ
{
  friend ସଂରଚନା Condition;

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (varIdx == VarIdx::NO_VARIATION)
      return_trace (ସତ୍ୟ);

    hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *condition_ମୂଲ୍ୟ;
    ଯଦି (c->plan->layout_condition_idx_ମୂଲ୍ୟ_map.has ((uintptr_t) ଏହି, &condition_ମୂଲ୍ୟ))
    {
      out->default_ମୂଲ୍ୟ_2 = condition_ମୂଲ୍ୟ->second;
      out->varIdx = condition_ମୂଲ୍ୟ->first;
      return_trace (ସତ୍ୟ);
    }

    hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *new_varidx_delta;
    ଯଦି (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!c->serializer->check_assign (
		out->default_ମୂଲ୍ୟ_2,
		default_ମୂଲ୍ୟ_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ମିଥ୍ୟା);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  template <typename Instancer>
  ତାର୍କିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		 Instancer *instancer) ଅଚଳ
  {
    ଦ୍ୱିଗୁଣ value = default_ମୂଲ୍ୟ_2;
    value += instancer->get_condition_delta (varIdx);
    ଫେରନ୍ତୁ value > 0;
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_ମୂଲ୍ୟ_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା ConditionAnd
{
  friend ସଂରଚନା Condition;

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ମିଥ୍ୟା);
    out->format = format;

    ପାଇଁ (ଅଚଳ auto &offset : conditions)
    {
      ଯଦି (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	ଯଦି (unlikely (!out_offset)) return_trace (ମିଥ୍ୟା);
	*out_offset = 0;
      }
      ଅନ୍ୟଥା ଯଦି (unlikely (!subset_offset_array (c, out->conditions, ଏହି) (offset)))
	return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  template <typename Instancer>
  ତାର୍କିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		 Instancer *instancer) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = conditions.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!_hb_recurse_condition_evaluate (ଏହି+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (conditions.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<ସଂରଚନା Condition>	conditions;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (3, conditions);
};

ସଂରଚନା ConditionOr
{
  friend ସଂରଚନା Condition;

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ମିଥ୍ୟା);
    out->format = format;

    ପାଇଁ (ଅଚଳ auto &offset : conditions)
    {
      ଯଦି (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	ଯଦି (unlikely (!out_offset)) return_trace (ମିଥ୍ୟା);
	*out_offset = 0;
      }
      ଅନ୍ୟଥା ଯଦି (unlikely (!subset_offset_array (c, out->conditions, ଏହି) (offset)))
	return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  template <typename Instancer>
  ତାର୍କିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		 Instancer *instancer) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = conditions.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (_hb_recurse_condition_evaluate (ଏହି+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (conditions.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<ସଂରଚନା Condition>	conditions;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (3, conditions);
};

ସଂରଚନା ConditionNegate
{
  friend ସଂରଚନା Condition;

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
    ଯଦି (condition.is_null ())
    {
      out->condition = 0;
      return_trace (ସତ୍ୟ);
    }
    return_trace (out->condition.serialize_subset (c, condition, ଏହି));
  }

  ବ୍ୟକ୍ତିଗତ:
  template <typename Instancer>
  ତାର୍କିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		 Instancer *instancer) ଅଚଳ
  {
    ଫେରନ୍ତୁ !_hb_recurse_condition_evaluate (ଏହି+condition,
					    coords, coord_len,
					    instancer);
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (condition.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<ସଂରଚନା Condition>	condition;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (5);
};

ସଂରଚନା Condition
{
  friend ସଂରଚନା ConditionSet;

  ସ୍ଥିର ତାର୍କିକ serialize_constant (hb_serialize_context_t *c, ତାର୍କିକ value)
  {
    auto *out = c->start_embed<Condition_ମୂଲ୍ୟ> ();
    ଯଦି (unlikely (!out || !c->extend_min (out))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    out->format = 2;
    out->default_ମୂଲ୍ୟ_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      ତାର୍କିକ materialize_true = ମିଥ୍ୟା) ଅଚଳ;

  template <typename Instancer>
  ତାର୍କିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		 Instancer *instancer) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    ସ୍ଥିତି 0: ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.evaluate (coords, coord_len, instancer);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.evaluate (coords, coord_len, instancer);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.evaluate (coords, coord_len, instancer);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.evaluate (coords, coord_len, instancer);
    ସ୍ଥିତି 5: hb_barrier (); ଫେରନ୍ତୁ u.format5.evaluate (coords, coord_len, instancer);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) ଅଚଳ
  {
    ଯଦି (unlikely (!depth))
    {
      c->apply = ମିଥ୍ୟା;
      ଫେରନ୍ତୁ DROP_RECORD_WITH_VAR;
    }

    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: ଫେରନ୍ତୁ KEEP_RECORD_WITH_VAR;
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.keep_with_variations (c, condition_map);
    ସ୍ଥିତି 2: ସ୍ଥିତି 3: ସ୍ଥିତି 4: ସ୍ଥିତି 5: ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: c->apply = ମିଥ୍ୟା; ଫେରନ୍ତୁ DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    ତାର୍କିକ applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    ଯଦି (c->all_axes_pinned)
      ଫେରନ୍ତୁ applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    ଯଦି (u.format.v == 2)
      ଫେରନ୍ତୁ u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (ଅଚଳ Condition &child)
    {
      auto child_context = *c;
      child_context.apply = ସତ୍ୟ;
      ଫେରନ୍ତୁ child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    ଯଦି (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      ଫେରନ୍ତୁ result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    ତାର୍କିକ is_and = u.format.v == 3;
    ତାର୍କିକ variable = ମିଥ୍ୟା;
    ଅଚଳ auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ଅଚଳ ଶୂନ୍ୟ *base = is_and ? (ଅଚଳ ଶୂନ୍ୟ *) &u.format3 : (ଅଚଳ ଶୂନ୍ୟ *) &u.format4;
    ପାଇଁ (ଅଚଳ auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      ଯଦି (is_and && result == DROP_RECORD_WITH_VAR)
        ଫେରନ୍ତୁ DROP_RECORD_WITH_VAR;
      ଯଦି (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        ଫେରନ୍ତୁ KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    ଫେରନ୍ତୁ variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    ଯଦି (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_ମୂଲ୍ୟ ());

    return_trace (c->end_recursion (ଏହି->dispatch (c)));
  }

  ତାର୍କିକ collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *condition_ମୂଲ୍ୟଗୁଡ଼ିକ = nullptr) ଅଚଳ;

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ Condition *src,
		  ଅଚଳ hb_map_t &varidx_map,
		  ଅଚଳ hb_map_t *axis_map = nullptr);

  ବ୍ୟକ୍ତିଗତ:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) ଅଚଳ;

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_ମୂଲ୍ୟ	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
ତାର୍କିକ
_hb_recurse_condition_evaluate (ଅଚଳ ସଂରଚନା Condition &condition,
				ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
				Instancer *instancer)
{
  ଫେରନ୍ତୁ condition.evaluate (coords, coord_len, instancer);
}

ସଂରଚନା ConditionList
{
  ଅଚଳ Condition& operator[] (unsigned i) ଅଚଳ
  { ଫେରନ୍ତୁ ଏହି+conditions[i]; }

  unsigned get_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ conditions.len; }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ ConditionList *src,
		  ଅଚଳ hb_inc_bimap_t &condition_map,
		  ଅଚଳ hb_map_t &varidx_map,
		  ଅଚଳ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    conditions.len = condition_map.get_population ();
    ଯଦି (unlikely (!c->extend (conditions))) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      ଯଦି (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (ମିଥ୍ୟା);
    }
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (conditions.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  Array32OfOffset32To<Condition> conditions;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, conditions);
};

ଧାଡ଼ିଭିତରେ ତାର୍କିକ
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *condition_ମୂଲ୍ୟଗୁଡ଼ିକ) ଅଚଳ
{
  ଯଦି (unlikely (!depth)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ବାଛ (u.format.v)
  {
    ସ୍ଥିତି 0:
    ସ୍ଥିତି 1:
      ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 2:
      ଯଦି (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        ଯଦି (condition_ମୂଲ୍ୟଗୁଡ଼ିକ &&
            !condition_ମୂଲ୍ୟଗୁଡ଼ିକ->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), ପୂର୍ଣ୍ଣସଂଖ୍ୟା (u.format2.default_ମୂଲ୍ୟ_2))))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      ଫେରନ୍ତୁ !var_indices->in_error ();
    ସ୍ଥିତି 3:
      ପାଇଁ (ଅଚଳ auto &offset : u.format3.conditions)
	ଯଦି (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_ମୂଲ୍ୟଗୁଡ଼ିକ)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 4:
      ପାଇଁ (ଅଚଳ auto &offset : u.format4.conditions)
	ଯଦି (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_ମୂଲ୍ୟଗୁଡ଼ିକ)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି 5:
      ଫେରନ୍ତୁ (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
}

ଧାଡ଼ିଭିତରେ ତାର୍କିକ
Condition::serialize (hb_serialize_context_t *c,
		      ଅଚଳ Condition *src,
		      ଅଚଳ hb_map_t &varidx_map,
		      ଅଚଳ hb_map_t *axis_map)
{
  TRACE_SERIALIZE (ଏହି);
  ବାଛ (src->u.format.v)
  {
    ସ୍ଥିତି 0:
    {
      auto *out = c->start_embed<Condition_ମୂଲ୍ୟ> ();
      ଯଦି (unlikely (!out || !c->extend_min (out))) return_trace (ମିଥ୍ୟା);
      out->format = 2;
      out->default_ମୂଲ୍ୟ_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (ସତ୍ୟ);
    }
    ସ୍ଥିତି 1:
    {
      auto *out = c->embed (&src->u.format1);
      ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);
      ଯଦି (axis_map)
      {
	ଯଦି (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (ମିଥ୍ୟା);
	}
	ଯଦି (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (ମିଥ୍ୟା);
      }
      return_trace (ସତ୍ୟ);
    }
    ସ୍ଥିତି 2:
    {
      auto *out = c->embed (&src->u.format2);
      ଯଦି (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (ମିଥ୍ୟା);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (ସତ୍ୟ);
    }
    ସ୍ଥିତି 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      ଯଦି (unlikely (!c->extend_min (out))) return_trace (ମିଥ୍ୟା);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      ଯଦି (unlikely (!c->extend (out->conditions))) return_trace (ମିଥ୍ୟା);
      ପାଇଁ (unsigned i = 0; i < out->conditions.len; i++)
	ଯଦି (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (ମିଥ୍ୟା);
      return_trace (ସତ୍ୟ);
    }
    ସ୍ଥିତି 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      ଯଦି (unlikely (!c->extend_min (out))) return_trace (ମିଥ୍ୟା);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      ଯଦି (unlikely (!c->extend (out->conditions))) return_trace (ମିଥ୍ୟା);
      ପାଇଁ (unsigned i = 0; i < out->conditions.len; i++)
	ଯଦି (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (ମିଥ୍ୟା);
      return_trace (ସତ୍ୟ);
    }
    ସ୍ଥିତି 5:
    {
      auto *out = c->embed (&src->u.format5);
      ଯଦି (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (ମିଥ୍ୟା);
      return_trace (ସତ୍ୟ);
    }
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      return_trace (ମିଥ୍ୟା);
  }
}

ଧାଡ଼ିଭିତରେ lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    ତାର୍କିକ materialize_true) ଅଚଳ
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  ଯଦି (result == LOOKUP_CONDITION_SUBSET_KEEP)
    ଫେରନ୍ତୁ result;

  c->serializer->revert (snap);
  ଯଦି (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    ଫେରନ୍ତୁ result;

  ଫେରନ୍ତୁ serialize_constant (c->serializer, ସତ୍ୟ) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

ଧାଡ଼ିଭିତରେ lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) ଅଚଳ
{
  ଯଦି (unlikely (!depth))
    ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   ଅଚଳ Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    ଯଦି (unlikely (!offset))
      ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    ଯଦି (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->କାଢ଼_pack ());
    ଅନ୍ୟଥା
    {
      c->serializer->କାଢ଼_discard ();
      out.କାଢ଼ ();
      c->serializer->revert (snap);
    }
    ଫେରନ୍ତୁ result;
  };

  ବାଛ (u.format.v)
  {
    ସ୍ଥିତି 0:
      ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_TRUE;

    ସ୍ଥିତି 1:
    {
      ଅଚଳ ConditionAxisRange &src = u.format1;
      ଯଦି (c->plan->user_axes_location.is_empty ())
        ଫେରନ୍ତୁ c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      ଯଦି (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      ଯଦି (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      ଦଶମିକ filter_min = src.filter_Range_Min_ମୂଲ୍ୟ.to_float ();
      ଦଶମିକ filter_max = src.filter_Range_Max_ମୂଲ୍ୟ.to_float ();
      ଯଦି (filter_min > filter_max)
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_FALSE;
      ଯଦି (axis_limit.is_point ())
        ଫେରନ୍ତୁ filter_min <= axis_limit.ମଧ୍ୟ &&
	       axis_limit.ମଧ୍ୟ <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      ଯଦି (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_FALSE;
      ଯଦି (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_TRUE;
      ଯଦି (!c->plan->axes_index_map.has (src.axisIndex))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      ଯଦି (unlikely (!out))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      ଯଦି (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_ମୂଲ୍ୟ.set_float (
	  renormalize_ମୂଲ୍ୟ (filter_min, axis_limit, axis_distances, ମିଥ୍ୟା));
      out->filter_Range_Max_ମୂଲ୍ୟ.set_float (
	  renormalize_ମୂଲ୍ୟ (filter_max, axis_limit, axis_distances, ମିଥ୍ୟା));
      ଯଦି (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;
      ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    ସ୍ଥିତି 2:
    {
      ଅଚଳ Condition_ମୂଲ୍ୟ &src = u.format2;
      auto *out = c->serializer->embed (&src);
      ଯଦି (unlikely (!out))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;

      ଯଦି (src.varIdx == VarIdx::NO_VARIATION)
        ଫେରନ୍ତୁ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) src.default_ମୂଲ୍ୟ_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *condition_ମୂଲ୍ୟ;
      ଯଦି (c->plan->layout_condition_idx_ମୂଲ୍ୟ_map.has ((uintptr_t) &src, &condition_ମୂଲ୍ୟ))
      {
        out->default_ମୂଲ୍ୟ_2 = condition_ମୂଲ୍ୟ->second;
        out->varIdx = condition_ମୂଲ୍ୟ->first;
        ଯଦି (out->varIdx == VarIdx::NO_VARIATION)
          ଫେରନ୍ତୁ out->default_ମୂଲ୍ୟ_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *new_varidx_delta;
      ଯଦି (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        ଫେରନ୍ତୁ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) src.default_ମୂଲ୍ୟ_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      ଯଦି (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        ଦ୍ୱିଗୁଣ value = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) src.default_ମୂଲ୍ୟ_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        ଫେରନ୍ତୁ value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_ମୂଲ୍ୟ = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) src.default_ମୂଲ୍ୟ_2 +
			  hb_second (*new_varidx_delta);
      ଯଦି (unlikely (!c->serializer->check_assign (
		out->default_ମୂଲ୍ୟ_2,
		default_ମୂଲ୍ୟ,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    ସ୍ଥିତି 3:
    {
      ଅଚଳ ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      ଯଦି (unlikely (!out || !c->serializer->extend_min (out)))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      ପାଇଁ (ଅଚଳ auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        ଯଦି (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          ଫେରନ୍ତୁ result;
      }
      ଫେରନ୍ତୁ out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    ସ୍ଥିତି 4:
    {
      ଅଚଳ ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      ଯଦି (unlikely (!out || !c->serializer->extend_min (out)))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      ପାଇଁ (ଅଚଳ auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        ଯଦି (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          ଫେରନ୍ତୁ result;
      }
      ଫେରନ୍ତୁ out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    ସ୍ଥିତି 5:
    {
      ଅଚଳ ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      ଯଦି (unlikely (!out))
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      ଯଦି (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->କାଢ଼_pack ());
        ଫେରନ୍ତୁ result;
      }

      c->serializer->କାଢ଼_discard ();
      ଯଦି (result == LOOKUP_CONDITION_SUBSET_TRUE)
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_FALSE;
      ଯଦି (result == LOOKUP_CONDITION_SUBSET_FALSE)
        ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_TRUE;
      ଫେରନ୍ତୁ result;
    }

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

ସଂରଚନା ConditionSet
{
  ତାର୍କିକ evaluate (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		 ItemVarStoreInstancer *instancer) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = conditions.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!(ଏହି+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ଅଚଳ
  {
    hb_map_t *condition_map = hb_map_create ();
    ଯଦି (unlikely (!condition_map)) ଫେରନ୍ତୁ;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    ଯଦି (unlikely (!cond_set)) ଫେରନ୍ତୁ;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = ସତ୍ୟ;
    ତାର୍କିକ should_keep = ମିଥ୍ୟା;
    /* The range map cannot identify value or boolean expressions uniquely. */
    ତାର୍କିକ can_deduplicate = ସତ୍ୟ;
    unsigned ଅଙ୍କ_kept_cond = 0, cond_idx = 0;
    ପାଇଁ (ଅଚଳ auto& offset : conditions)
    {
      ଅଚଳ Condition &condition = ଏହି+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      ଯଦି (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = ମିଥ୍ୟା;
        ଫେରନ୍ତୁ;
      }

      ଯଦି (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = ସତ୍ୟ;
        cond_set->add (cond_idx);
        ଅଙ୍କ_kept_cond++;
      }

      ଯଦି (ret == KEEP_RECORD_WITH_VAR)
        should_keep = ସତ୍ୟ;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = ଅଙ୍କ_kept_cond == 0;
    ଯଦି (!should_keep && c->all_axes_pinned) ଫେରନ୍ତୁ;

    //check if condition_set is unique with variations
    ଯଦି (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      ଫେରନ୍ତୁ;

    ଯଦି (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               ତାର୍କିକ ଭର୍ତ୍ତି_catch_all) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ଯଦି (ଭର୍ତ୍ତି_catch_all) return_trace (ସତ୍ୟ);

    hb_set_t *retained_cond_set = nullptr;
    ଯଦି (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = conditions.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (retained_cond_set != nullptr && !retained_cond_set->has (i))
        ଜାରି;
      ଅଚଳ Condition &condition = ଏହି+conditions[i];
      ଯଦି (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, ଏହି) (conditions[i]);
        ଜାରି;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      ଯଦି (unlikely (!offset)) return_trace (ମିଥ୍ୟା);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, ସତ୍ୟ);
      ଯଦି (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, ମିଥ୍ୟା) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      ଯଦି (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->କାଢ଼_discard ();
        c->serializer->check_success (ମିଥ୍ୟା);
        return_trace (ମିଥ୍ୟା);
      }
      c->serializer->add_link (*offset, c->serializer->କାଢ଼_pack ());
    }

    return_trace (ତାର୍କିକ (out->conditions));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (conditions.sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto &offset : conditions)
      ଯଦି (unlikely (!(ଏହି+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_ମୂଲ୍ୟଗୁଡ଼ିକ)))
	c->layout_variation_indices->err ();
  }

  ସୁରକ୍ଷିତ:
  Array16OfOffset32To<Condition>	conditions;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, conditions);
};

ସଂରଚନା FeatureTableSubstitutionRecord
{
  friend ସଂରଚନା FeatureTableSubstitution;

  ଶୂନ୍ୟ collect_lookups (ଅଚଳ ଶୂନ୍ୟ *base, hb_set_t *lookup_indexes /* OUT */) ଅଚଳ
  {
    ଫେରନ୍ତୁ (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  ଶୂନ୍ୟ closure_features (ଅଚଳ ଶୂନ୍ୟ *base,
			 ଅଚଳ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ଅଚଳ
  {
    ଯଦି ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  ଶୂନ୍ୟ collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, ଅଚଳ Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    ଅଚଳ hb_set_t *feature_indices,
                                                    ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଯଦି (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  ତାର୍କିକ serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  ଅଚଳ Feature *f, ଅଚଳ Tag *tag)
  {
    TRACE_SERIALIZE (ଏହି);
    hb_serialize_context_t *s = c->subset_context->serializer;
    ଯଦି (unlikely (!s->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);

    uint32_t *new_feature_idx;
    ଯଦି (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    s->push ();
    ତାର୍କିକ ret = f->subset (c->subset_context, c, tag);
    ଯଦି (ret) s->add_link (feature, s->କାଢ଼_pack ());
    ଅନ୍ୟଥା s->କାଢ଼_discard ();

    return_trace (ret);
  }

  ତାର୍କିକ subset (hb_subset_layout_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    uint32_t *new_feature_index;
    ଯଦି (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (ମିଥ୍ୟା);

    auto *out = c->subset_context->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->featureIndex = *new_feature_index;
    hb_pair_t<ଅଚଳ ଶୂନ୍ୟ*, ଅଚଳ ଶୂନ୍ୟ*> *original;
    ଯଦି (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (ମିଥ୍ୟା);
    ଅଚଳ Tag *tag = reinterpret_cast<ଅଚଳ Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base,
		 ଅଚଳ FeatureList *feature_list) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ଅଚଳ Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା FeatureTableSubstitution
{
  ଶୂନ୍ୟ collect_feature_indices (hb_set_t *feature_indices) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  ଅଚଳ Feature *find_substitute (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature_index) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = substitutions.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଅଚଳ FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      ଯଦି (record.featureIndex == feature_index)
	ଫେରନ୍ତୁ &(ଏହି+record.feature);
    }
    ଫେରନ୍ତୁ nullptr;
  }

  ଶୂନ୍ୟ collect_lookups (ଅଚଳ hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ଅଚଳ
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([ଏହି, lookup_indexes] (ଅଚଳ FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (ଏହି, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  ଶୂନ୍ୟ collect_name_ids (ଅଚଳ hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ FeatureTableSubstitutionRecord &record : substitutions)
      ଯଦି (feature_index_map->has (record.featureIndex))
        (ଏହି+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  ଶୂନ୍ୟ closure_features (ଅଚଳ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (ଏହି, lookup_indexes, feature_indexes);
  }

  ତାର୍କିକ intersects_features (ଅଚଳ hb_map_t *feature_index_map) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ FeatureTableSubstitutionRecord& record : substitutions)
    {
      ଯଦି (feature_index_map->has (record.featureIndex)) ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, ଏହି);
  }

  ତାର୍କିକ subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               ତାର୍କିକ ଭର୍ତ୍ତି_catch_all) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->version.major = version.major;
    out->version.minor = version.minor;

    ଯଦି (ଭର୍ତ୍ତି_catch_all)
    {
      ପାଇଁ (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<ଅଚଳ ଶୂନ୍ୟ*, ଅଚଳ ଶୂନ୍ୟ*> *p;
        ଯଦି (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (ମିଥ୍ୟା);
        auto *o = out->substitutions.serialize_append (c->serializer);
        ଯଦି (!o->serialize (l, feature_index,
                           reinterpret_cast<ଅଚଳ Feature*> (p->first),
                           reinterpret_cast<ଅଚଳ Tag*> (p->second)))
          return_trace (ମିଥ୍ୟା);
      }
      return_trace (ସତ୍ୟ);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), ଏହି))
    ;

    return_trace (ତାର୍କିକ (out->substitutions));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ FeatureList *feature_list) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, ଏହି, feature_list));
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

ସଂରଚନା FeatureVariationRecord
{
  friend ସଂରଚନା FeatureVariations;

  ଶୂନ୍ୟ collect_lookups (ଅଚଳ ଶୂନ୍ୟ     *base,
			ଅଚଳ hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ଅଚଳ
  {
    ଫେରନ୍ତୁ (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  ଶୂନ୍ୟ collect_name_ids (ଅଚଳ ଶୂନ୍ୟ *base,
                         ଅଚଳ hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ଅଚଳ
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  ଶୂନ୍ୟ closure_features (ଅଚଳ ଶୂନ୍ୟ     *base,
			 ଅଚଳ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ଅଚଳ
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  ତାର୍କିକ intersects_features (ଅଚଳ ଶୂନ୍ୟ *base, ଅଚଳ hb_map_t *feature_index_map) ଅଚଳ
  {
    ଫେରନ୍ତୁ (base+substitutions).intersects_features (feature_index_map);
  }

  ଶୂନ୍ୟ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    (base+conditions).keep_with_variations (c);
    ଯଦି (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = ସତ୍ୟ; // set variations only once
    }
  }

  ତାର୍କିକ subset (hb_subset_layout_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base,
               ତାର୍କିକ ଭର୍ତ୍ତି_catch_all = ମିଥ୍ୟା) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->subset_context->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    ଯଦି (!ଭର୍ତ୍ତି_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    ଅନ୍ୟଥା
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, ଭର୍ତ୍ତି_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, ଭର୍ତ୍ତି_catch_all);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base,
		 ଅଚଳ FeatureList *feature_list) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  { (base+conditions).collect_variation_indices (c); }

  ସୁରକ୍ଷିତ:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା LookupIndexList
{
  ଶୂନ୍ୟ add_lookup_indexes_to (hb_set_t *lookup_indexes) ଅଚଳ
  { lookupIndices.add_indexes_to (lookup_indexes); }

  ତାର୍କିକ intersects_lookup_indexes (ଅଚଳ hb_map_t *lookup_indexes) ଅଚଳ
  { ଫେରନ୍ତୁ lookupIndices.intersects (lookup_indexes); }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ମିଥ୍ୟା);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (ତାର୍କିକ (out->lookupIndices));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (lookupIndices.sanitize (c));
  }

  ସୁରକ୍ଷିତ:
  IndexArray	lookupIndices;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

ସଂରଚନା LookupConditionRecord
{
  unsigned get_condition_offset (ଅଚଳ ଶୂନ୍ୟ *base, ଅଚଳ ଶୂନ୍ୟ *variations) ଅଚଳ
  { ଫେରନ୍ତୁ (ଅଚଳ ଅକ୍ଷର *) &(base+condition) - (ଅଚଳ ଅକ୍ଷର *) variations; }

  template <typename Instancer>
  ତାର୍କିକ evaluate (ଅଚଳ ଶୂନ୍ୟ *base,
		 ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
		 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		 Instancer *instancer) ଅଚଳ
  { ଫେରନ୍ତୁ (base+condition).evaluate (coords, coord_len, instancer); }

  ତାର୍କିକ evaluate (ଅଚଳ ଶୂନ୍ୟ *base,
                 ଅଚଳ ଶୂନ୍ୟ *variations,
                 hb_sorted_array_t<ଅଚଳ unsigned> conditions) ଅଚଳ
  {
    ଫେରନ୍ତୁ !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     ଆକାର (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  ତାର୍କିକ collect_condition (ଅଚଳ ଶୂନ୍ୟ *base,
                          ଅଚଳ ଶୂନ୍ୟ *variations,
                          ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
                          unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) ଅଚଳ
  {
    /* Null conditions always apply and need no key state. */
    ଯଦି (!condition) ଫେରନ୍ତୁ ସତ୍ୟ;

    unsigned offset = get_condition_offset (base, variations);
    ଯଦି (visited->has (offset)) ଫେରନ୍ତୁ ସତ୍ୟ;
    visited->add (offset);
    ଯଦି (unlikely (visited->in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    ଫେରନ୍ତୁ !conditions->in_error ();
  }

  ଶୂନ୍ୟ add_lookup_indexes_to (ଅଚଳ ଶୂନ୍ୟ *base,
			      hb_set_t *lookup_indexes) ଅଚଳ
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  ତାର୍କିକ intersects_lookup_indexes (ଅଚଳ ଶୂନ୍ୟ *base,
				   ଅଚଳ hb_map_t *lookup_indexes) ଅଚଳ
  { ଫେରନ୍ତୁ (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଯଦି (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_ମୂଲ୍ୟଗୁଡ଼ିକ)))
      c->layout_variation_indices->err ();
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    ସତ୍ୟ);
    ଯଦି (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }
    c->serializer->add_link (out->condition, c->serializer->କାଢ଼_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  ସୁରକ୍ଷିତ:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା FeatureLookupsTable
{
  ତାର୍କିକ adds_default_lookups () ଅଚଳ { ଫେରନ୍ତୁ flags & 0x0001u; }

  ତାର୍କିକ collect_conditions (ଅଚଳ ଶୂନ୍ୟ *variations,
                           ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
                           unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupConditionRecord &record : records)
      ଯଦି (unlikely (!record.collect_condition (ଏହି, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ collect_lookups (ଅଚଳ ଶୂନ୍ୟ *variations,
                       hb_sorted_array_t<ଅଚଳ unsigned> conditions,
                       hb_set_t *lookup_indexes) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupConditionRecord &record : records)
      ଯଦି (record.evaluate (ଏହି, variations, conditions))
	record.add_lookup_indexes_to (ଏହି, lookup_indexes);
  }

  template <typename Instancer>
  ଶୂନ୍ୟ collect_lookups (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupConditionRecord &record : records)
      ଯଦି (record.evaluate (ଏହି, coords, coord_len, instancer))
	record.add_lookup_indexes_to (ଏହି, lookup_indexes);
  }

  ଶୂନ୍ୟ collect_lookups (hb_set_t *lookup_indexes) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (ଏହି, lookup_indexes);
  }

  ତାର୍କିକ intersects_lookup_indexes (ଅଚଳ hb_map_t *lookup_indexes) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupConditionRecord &record : records)
      ଯଦି (record.intersects_lookup_indexes (ଏହି, lookup_indexes))
	ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupConditionRecord &record : records)
      record.collect_variation_indices (c, ଏହି);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ମିଥ୍ୟା);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    ପାଇଁ (ଅଚଳ LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      ଯଦି (record.subset (c, l, ଏହି))
	out->records.len++;
      ଅନ୍ୟଥା
	c->serializer->revert (snap);
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (10, records);
};

ସଂରଚନା LookupVariationRecord
{
  friend ସଂରଚନା FeatureVariations;

  unsigned get_feature_index () ଅଚଳ { ଫେରନ୍ତୁ featureIndex; }

  ଶୂନ୍ୟ collect_lookups (ଅଚଳ ଶୂନ୍ୟ *base,
                       ଅଚଳ Feature &feature,
                       hb_sorted_array_t<ଅଚଳ unsigned> conditions,
                       hb_set_t *lookup_indexes) ଅଚଳ
  {
    ଅଚଳ FeatureLookupsTable &feature_lookups = base+featureLookups;
    ଯଦି (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  ଶୂନ୍ୟ collect_lookups (ଅଚଳ ଶୂନ୍ୟ *base,
			ଅଚଳ hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) ଅଚଳ
  {
    ଯଦି (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  ଶୂନ୍ୟ closure_features (ଅଚଳ ଶୂନ୍ୟ *base,
			 ଅଚଳ hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) ଅଚଳ
  {
    ଯଦି ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  { (base+featureLookups).collect_variation_indices (c); }

  ତାର୍କିକ subset (hb_subset_layout_context_t *l, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (ଏହି);
    uint32_t *new_feature_index;
    ଯଦି (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (ମିଥ୍ୟା);

    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ମିଥ୍ୟା);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  featureLookups.sanitize (c, base));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା FeatureVariations
{
  ସ୍ଥିର constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  ଅଚଳ Array32Of<LookupVariationRecord> &get_lookup_variation_records () ଅଚଳ
  {
    ଯଦି (version.to_int () < 0x00010001u)
      ଫେରନ୍ତୁ Null (Array32Of<LookupVariationRecord>);
    ଫେରନ୍ତୁ StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_ଗଣନା () ଅଚଳ
  {
    ଫେରନ୍ତୁ varRecords.len;
  }

  ତାର୍କିକ find_index (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index,
		   ItemVarStoreInstancer *instancer) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = varRecords.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଅଚଳ FeatureVariationRecord &record = varRecords.arrayZ[i];
      ଯଦି ((ଏହି+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    *index = NOT_FOUND_INDEX;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଅଚଳ Feature *find_substitute (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା variations_index,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature_index) ଅଚଳ
  {
    ଅଚଳ FeatureVariationRecord &record = varRecords[variations_index];
    ଫେରନ୍ତୁ (ଏହି+record.substitutions).find_substitute (feature_index);
  }

  ତାର୍କିକ collect_lookup_conditions (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
                                  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) ଅଚଳ
  {
    hb_set_t visited_tables, visited_conditions;
    ପାଇଁ (ଅଚଳ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      ଯଦି (visited_tables.has (offset)) ଜାରି;
      visited_tables.add (offset);
      ଯଦି (unlikely (visited_tables.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଅଚଳ FeatureLookupsTable &feature_lookups = ଏହି+record.featureLookups;
      ଯଦି (unlikely (!feature_lookups.collect_conditions (ଏହି, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ get_lookup_variations (unsigned feature_index,
                              ଅଚଳ Feature &feature,
                              hb_sorted_array_t<ଅଚଳ unsigned> conditions,
                              hb_set_t *lookup_indexes) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      ଯଦି (record.featureIndex != feature_index) ଜାରି;

      record.collect_lookups (ଏହି, feature, conditions, lookup_indexes);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = varRecords.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, ଏହି);
      ଯଦି (c->universal)
        ଭାଙ୍ଗ;
    }
    ଯଦି (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed (*ଏହି));
  }

  ଶୂନ୍ୟ collect_lookups (ଅଚଳ hb_set_t *feature_indexes,
			ଅଚଳ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) ଅଚଳ
  {
    unsigned ଗଣନା_2 = varRecords.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        ଜାରି;
      varRecords[i].collect_lookups (ଏହି, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  ଶୂନ୍ୟ collect_name_ids (ଅଚଳ hb_map_t *feature_index_map,
                         ଅଚଳ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < varRecords.len; i++)
    {
      ଯଦି (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        ଜାରି;
      varRecords[i].collect_name_ids (ଏହି, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  ଶୂନ୍ୟ collect_lookup_variation_lookups (ଅଚଳ hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (ଏହି, feature_indexes, lookup_indexes);
  }

  ଶୂନ୍ୟ closure_features (ଅଚଳ hb_map_t *lookup_indexes,
			 ଅଚଳ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = varRecords.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        ଜାରି;
      varRecords[i].closure_features (ଏହି, lookup_indexes, feature_indexes);
    }

    ପାଇଁ (ଅଚଳ LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (ଏହି, lookup_indexes, feature_indexes);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, ଏହି);
    ପାଇଁ (ଅଚଳ LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, ଏହି);
  }

  ଶୂନ୍ୟ collect_feature_indices (hb_set_t *feature_indices) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ FeatureVariationRecord &record : varRecords)
      (ଏହି+record.substitutions).collect_feature_indices (feature_indices);
  }

  ତାର୍କିକ has_lookup_variations (unsigned feature_index) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      ଯଦି (record.featureIndex == feature_index)
	ଫେରନ୍ତୁ ସତ୍ୟ;
      ଯଦି (record.featureIndex > feature_index)
	ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->version.major = version.major;
    out->version.minor = version.minor;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_up_to = -1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = varRecords.len - 1; i >= 0; i--) {
      ଯଦି (varRecords[i].intersects_features (ଏହି, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        ଭାଙ୍ଗ;
      }
    }

    unsigned ଗଣନା_2 = (unsigned) (keep_up_to + 1);
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        ଜାରି;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), ଏହି) (varRecords[i]);
    }

    ଯଦି (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      ତାର୍କିକ ଭର୍ତ୍ତି_catch_all_record = ସତ୍ୟ;
      subset_record_array (l, &(out->varRecords), ଏହି, ଭର୍ତ୍ତି_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    ଯଦି (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      ଯଦି (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (ମିଥ୍ୟା);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, ଏହି))
      ;
    }

    return_trace (ତାର୍କିକ (out->varRecords) ||
		  (out_lookup_records && ତାର୍କିକ (*out_lookup_records)));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ FeatureList *feature_list) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, ଏହି, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

ସଂରଚନା HintingDevice
{
  friend ସଂରଚନା Device;

  ବ୍ୟକ୍ତିଗତ:

  hb_position_t get_x_delta (hb_font_t *font) ଅଚଳ
  { ଫେରନ୍ତୁ get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ଅଚଳ
  { ଫେରନ୍ତୁ get_delta (font->y_ppem, font->y_scale); }

  ସର୍ବସାଧାରଣ:

  size_t get_size () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା f = deltaFormat;
    ଯଦି (unlikely (f < 1 || f > 3 || startSize > endSize)) ଫେରନ୍ତୁ 3 * HBUINT16::static_size;
    ଫେରନ୍ତୁ HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && c->check_range (ଏହି, ଏହି->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed<HintingDevice> (ଏହି));
  }

  ବ୍ୟକ୍ତିଗତ:

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_delta (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ppem, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scale) ଅଚଳ
  {
    ଯଦି (!ppem) ଫେରନ୍ତୁ 0;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା pixels = get_delta_pixels (ppem);

    ଯଦି (!pixels) ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (pixels * (int64_t) scale / ppem);
  }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_delta_pixels (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ppem_size) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା f = deltaFormat;
    ଯଦି (unlikely (f < 1 || f > 3))
      ଫେରନ୍ତୁ 0;

    ଯଦି (ppem_size < startSize || ppem_size > endSize)
      ଫେରନ୍ତୁ 0;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = ppem_size - startSize;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା byte = delta_ମୂଲ୍ୟ_Z[s >> (4 - f)];
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask = (0xFFFFu >> (16 - (1 << f)));

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = bits & mask;

    ଯଦି ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    ଫେରନ୍ତୁ delta;
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_ମୂଲ୍ୟ_Z;		/* Array of compressed data */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, delta_ମୂଲ୍ୟ_Z);
};

ସଂରଚନା VariationDevice
{
  friend ସଂରଚନା Device;

  ବ୍ୟକ୍ତିଗତ:

  hb_position_t get_x_delta (hb_font_t *font,
			     ଅଚଳ ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  { ଫେରନ୍ତୁ !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ଅଚଳ ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  { ଫେରନ୍ତୁ !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  ଦଶମିକ get_x_delta_f (hb_font_t *font,
		       ଅଚଳ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ଅଚଳ
  { ଫେରନ୍ତୁ !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  ଦଶମିକ get_y_delta_f (hb_font_t *font,
		       ଅଚଳ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ଅଚଳ
  { ଫେରନ୍ତୁ !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *v;
    ଯଦି (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    ଯଦି (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (ଏହି->static_size);
    auto *out = c->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (nullptr);

    ଯଦି (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  ଶୂନ୍ୟ collect_variation_index (hb_collect_variation_indices_context_t *c) ଅଚଳ
  { c->layout_variation_indices->add (varIdx); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ବ୍ୟକ୍ତିଗତ:

  ଦଶମିକ get_delta (hb_font_t *font,
		   ଅଚଳ ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
    ଫେରନ୍ତୁ store.get_delta (varIdx, font->coords, font->ଅଙ୍କ_coords, store_cache);
  }

  ସୁରକ୍ଷିତ:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା DeviceHeader
{
  ସୁରକ୍ଷିତ:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  ସର୍ବସାଧାରଣ:
  HBUINT16		format;		/* Format identifier */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  ଦଶମିକ get_x_delta_f (hb_font_t *font,
		       ଅଚଳ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
#ifndef HB_NO_VAR
    ଯଦି (u.b.format == 0x8000)
    {
      hb_barrier ();
      ଫେରନ୍ତୁ u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    ଫେରନ୍ତୁ get_x_delta (font, store, store_cache);
  }

  ଦଶମିକ get_y_delta_f (hb_font_t *font,
		       ଅଚଳ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
#ifndef HB_NO_VAR
    ଯଦି (u.b.format == 0x8000)
    {
      hb_barrier ();
      ଫେରନ୍ତୁ u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    ଫେରନ୍ତୁ get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ଅଚଳ ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
    ବାଛ (u.b.format)
    {
#ifndef HB_NO_HINTING
    ସ୍ଥିତି 1: ସ୍ଥିତି 2: ସ୍ଥିତି 3:
      hb_barrier ();
      ଫେରନ୍ତୁ u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    ସ୍ଥିତି 0x8000:
      hb_barrier ();
      ଫେରନ୍ତୁ u.variation.get_x_delta (font, store, store_cache);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ଅଚଳ ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ଅଚଳ
  {
    ବାଛ (u.b.format)
    {
    ସ୍ଥିତି 1: ସ୍ଥିତି 2: ସ୍ଥିତି 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      ଫେରନ୍ତୁ u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    ସ୍ଥିତି 0x8000:
      hb_barrier ();
      ଫେରନ୍ତୁ u.variation.get_y_delta (font, store, store_cache);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ 0;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.b.format.sanitize (c)) return_trace (ମିଥ୍ୟା);
    ବାଛ (u.b.format) {
#ifndef HB_NO_HINTING
    ସ୍ଥିତି 1: ସ୍ଥିତି 2: ସ୍ଥିତି 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    ସ୍ଥିତି 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      return_trace (ସତ୍ୟ);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map=nullptr) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    ବାଛ (u.b.format) {
#ifndef HB_NO_HINTING
    ସ୍ଥିତି 1:
    ସ୍ଥିତି 2:
    ସ୍ଥିତି 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    ସ୍ଥିତି 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      return_trace (nullptr);
    }
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ବାଛ (u.b.format) {
#ifndef HB_NO_HINTING
    ସ୍ଥିତି 1:
    ସ୍ଥିତି 2:
    ସ୍ଥିତି 3:
      ଫେରନ୍ତୁ;
#endif
#ifndef HB_NO_VAR
    ସ୍ଥିତି 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      ଫେରନ୍ତୁ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ;
    }
  }

  unsigned get_variation_index () ଅଚଳ
  {
    ବାଛ (u.b.format) {
#ifndef HB_NO_VAR
    ସ୍ଥିତି 0x8000:
      hb_barrier ();
      ଫେରନ୍ତୁ u.variation.varIdx;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  ତାର୍କିକ is_variation_device () ଅଚଳ
  {
    ବାଛ (u.b.format) {
#ifndef HB_NO_VAR
    ସ୍ଥିତି 0x8000:
      ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

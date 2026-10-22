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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-ot-layout.hh"
#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-set.hh"
#അന്തര്ഭാവ "hb-bimap.hh"
#അന്തര്ഭാവ "hb-cache.hh"
#അന്തര്ഭാവ "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (വലുപ്പം (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_ദ്വിമാന_cache_t = hb_cache_t<14, 1, 8>;
static_assert (വലുപ്പം (hb_ot_layout_ദ്വിമാന_cache_t) == 256, "");

#അന്തര്ഭാവ "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


നാമാകാശ OT {

template<typename Iterator>
സ്ഥിതം വരിയിൽ തർക്കം ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

സ്ഥിതം തർക്കം ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    സ്ഥിരം hb_set_t &klasses,
    തർക്കം use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

ഘടന ItemVarStoreInstancer;

ഘടന hb_collect_feature_substitutes_with_var_context_t
{
  സ്ഥിരം hb_map_t *axes_index_tag_map;
  സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, സ്ഥിരം Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  തർക്കം apply;
  തർക്കം variation_applied;
  തർക്കം universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  തർക്കം all_axes_pinned;
};

ഘടന hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (സ്ഥിരം ശൂന്യം         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              സ്ഥിരം hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_എണ്ണം (0),langsys_feature_എണ്ണം (0) {}

  തർക്കം visitScript ()
  { തിരികെ script_എണ്ണം++ < HB_MAX_SCRIPTS; }

  തർക്കം visitLangsys (unsigned feature_എണ്ണം_2)
  {
    langsys_feature_എണ്ണം += feature_എണ്ണം_2;
    തിരികെ langsys_feature_എണ്ണം < HB_MAX_LANGSYS_FEATURE_എണ്ണം;
  }

  പൊതു:
  സ്ഥിരം ശൂന്യം *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  സ്ഥിരം hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  സ്വകാര്യ:
  unsigned script_എണ്ണം;
  unsigned langsys_feature_എണ്ണം;
};

ഘടന hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  സ്ഥിരം അക്ഷരം *get_name () { തിരികെ "SUBSET_LAYOUT"; }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ hb_empty_t (); }

  തർക്കം visitScript ()
  {
    തിരികെ script_എണ്ണം++ < HB_MAX_SCRIPTS;
  }

  തർക്കം visitLangSys ()
  {
    തിരികെ langsys_എണ്ണം++ < HB_MAX_LANGSYS;
  }

  തർക്കം visitFeatureIndex (പൂർണ്ണസംഖ്യ എണ്ണം)
  {
    feature_index_എണ്ണം += എണ്ണം;
    തിരികെ feature_index_എണ്ണം < HB_MAX_FEATURE_INDICES;
  }

  തർക്കം visitLookupIndex()
  {
    lookup_index_എണ്ണം++;
    തിരികെ lookup_index_എണ്ണം < HB_MAX_LOOKUP_VISIT_എണ്ണം;
  }

  hb_subset_context_t *subset_context;
  സ്ഥിരം hb_tag_t table_tag;
  സ്ഥിരം hb_map_t *lookup_index_map;
  സ്ഥിരം hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  സ്ഥിരം hb_map_t *feature_index_map;
  സ്ഥിരം hb_map_t *feature_map_w_duplicates;
  സ്ഥിരം hb_hashmap_t<unsigned, സ്ഥിരം Feature*> *feature_substitutes_map;
  സ്ഥിരം hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  സ്ഥിരം hb_set_t *catch_all_record_feature_idxes;
  സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<സ്ഥിരം ശൂന്യം*, സ്ഥിരം ശൂന്യം*>> *feature_idx_tag_map;

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
				script_എണ്ണം (0),
				langsys_എണ്ണം (0),
				feature_index_എണ്ണം (0),
				lookup_index_എണ്ണം (0)
  {
    എങ്കിൽ (tag_ == HB_OT_TAG_GSUB)
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
    അല്ലെങ്കിൽ
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

  സ്വകാര്യ:
  unsigned script_എണ്ണം;
  unsigned langsys_എണ്ണം;
  unsigned feature_index_എണ്ണം;
  unsigned lookup_index_എണ്ണം;
};

ഘടന ItemVariationStore;
ഘടന hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj) { obj.collect_variation_indices (ഇത്); തിരികെ hb_empty_t (); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  സ്ഥിരം hb_set_t *glyph_set;
  സ്ഥിരം hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *condition_മൂല്യങ്ങൾ = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  സ്ഥിരം hb_set_t *glyph_set_,
					  സ്ഥിരം hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
ഘടന subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 സ്ഥിരം ശൂന്യം *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  തർക്കം operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    എങ്കിൽ (unlikely (!o)) തിരികെ അസത്യം;
    തർക്കം ret = o->serialize_subset (subset_context, offset, base);
    എങ്കിൽ (!ret)
    {
      out.എടുക്കുക ();
      subset_context->serializer->revert (snap);
    }
    തിരികെ ret;
  }

  സ്വകാര്യ:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  സ്ഥിരം ശൂന്യം *base;
};


template<typename OutputArray, typename Arg>
ഘടന subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     സ്ഥിരം ശൂന്യം *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  തർക്കം operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    എങ്കിൽ (unlikely (!o)) തിരികെ അസത്യം;
    തർക്കം ret = o->serialize_subset (subset_context, offset, base, arg);
    എങ്കിൽ (!ret)
    {
      out.എടുക്കുക ();
      subset_context->serializer->revert (snap);
    }
    തിരികെ ret;
  }

  സ്വകാര്യ:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  സ്ഥിരം ശൂന്യം *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
ഘടന
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  { തിരികെ subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       സ്ഥിരം ശൂന്യം *base, Arg &&arg) സ്ഥിരം
  { തിരികെ subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
ഘടന subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 സ്ഥിരം ശൂന്യം *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  ശൂന്യം
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    തർക്കം ret = record.subset (subset_layout_context, base);
    എങ്കിൽ (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    അല്ലെങ്കിൽ out->len++;
  }

  സ്വകാര്യ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  സ്ഥിരം ശൂന്യം *base;
};

template<typename OutputArray, typename Arg>
ഘടന subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     സ്ഥിരം ശൂന്യം *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  ശൂന്യം
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    തർക്കം ret = record.subset (subset_layout_context, base, arg);
    എങ്കിൽ (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    അല്ലെങ്കിൽ out->len++;
  }

  സ്വകാര്യ:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  സ്ഥിരം ശൂന്യം *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
ഘടന
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  { തിരികെ subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               സ്ഥിരം ശൂന്യം *base, Arg &&arg) സ്ഥിരം
  { തിരികെ subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
ഘടന serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         സ്ഥിരം ശൂന്യം *base_,
                         സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  തർക്കം operator () (T&& record)
  {
    എങ്കിൽ (!serialize_context->copy (record, base, variation_map)) തിരികെ അസത്യം;
    out.len++;
    തിരികെ സത്യം;
  }

  സ്വകാര്യ:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  സ്ഥിരം ശൂന്യം *base;
  സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
ഘടന
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               സ്ഥിരം ശൂന്യം *base,
               സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *variation_map) സ്ഥിരം
  { തിരികെ serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

ഘടന IndexArray : Array16Of<Index>
{
  തർക്കം intersects (സ്ഥിരം hb_map_t *indexes) സ്ഥിരം
  { തിരികെ hb_any (*ഇത്, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    എങ്കിൽ (!it) തിരികെ;
    എങ്കിൽ (unlikely (!c->extend_min ((*ഇത്)))) തിരികെ;

    വേണ്ടി (സ്ഥിരം auto _ : it)
    {
      എങ്കിൽ (!l->visitLookupIndex()) നിർത്തുക;

      Index i;
      i = _;
      c->copy (i);
      ഇത്->len++;
    }
  }

  unsigned പൂർണ്ണസംഖ്യ get_indexes (unsigned പൂർണ്ണസംഖ്യ start_offset,
			    unsigned പൂർണ്ണസംഖ്യ *എണ്ണം_2 /* IN/OUT */,
			    unsigned പൂർണ്ണസംഖ്യ *_indexes /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (എണ്ണം_2 && _indexes)
    {
      + ഇത്->as_array ().sub_array (start_offset, എണ്ണം_2)
      | hb_sink (hb_array (_indexes, *എണ്ണം_2))
      ;
    }
    തിരികെ ഇത്->len;
  }

  ശൂന്യം add_indexes_to (hb_set_t* output /* OUT */) സ്ഥിരം
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
ഘടന FeatureParamsSize
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്))) return_trace (അസത്യം);
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

    എങ്കിൽ (!designSize)
      return_trace (അസത്യം);
    അല്ലെങ്കിൽ എങ്കിൽ (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (സത്യം);
    അല്ലെങ്കിൽ എങ്കിൽ (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (അസത്യം);
    അല്ലെങ്കിൽ
      return_trace (സത്യം);
  }

  ശൂന്യം collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) സ്ഥിരം
  { nameids_to_retain->add (subfamilyNameID); }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    return_trace ((തർക്കം) c->serializer->embed (*ഇത്));
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
  പൊതു:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
ഘടന FeatureParamsStylisticSet
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (ഇത്));
  }

  ശൂന്യം collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) സ്ഥിരം
  { nameids_to_retain->add (uiNameID); }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    return_trace ((തർക്കം) c->serializer->embed (*ഇത്));
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
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
ഘടന FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_എണ്ണം, hb_codepoint_t *chars) സ്ഥിരം
  {
    എങ്കിൽ (char_എണ്ണം && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_എണ്ണം)
      | hb_sink (hb_array (chars, *char_എണ്ണം))
      ;
    }
    തിരികെ characters.len;
  }

  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  ശൂന്യം collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    എങ്കിൽ (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    എങ്കിൽ (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    എങ്കിൽ (!firstParamUILabelNameID || !അക്കം_Named_Parameters || അക്കം_Named_Parameters >= 0x7FFF)
      തിരികെ;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) അക്കം_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    return_trace ((തർക്കം) c->serializer->embed (*ഇത്));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
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
  HBUINT16	അക്കം_Named_Parameters;	/* Number of named parameters. (May
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
  പൊതു:
  DEFINE_SIZE_ARRAY (14, characters);
};

ഘടന FeatureParams
{
  തർക്കം sanitize (hb_sanitize_context_t *c, hb_tag_t tag) സ്ഥിരം
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    തിരികെ സത്യം;
#endif
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    എങ്കിൽ ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    എങ്കിൽ ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (സത്യം);
  }

  ശൂന്യം collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) സ്ഥിരം
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    തിരികെ;
#endif
    എങ്കിൽ (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      തിരികെ (u.size.collect_name_ids (nameids_to_retain));
    എങ്കിൽ ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      തിരികെ (u.stylisticSet.collect_name_ids (nameids_to_retain));
    എങ്കിൽ ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      തിരികെ (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  തർക്കം subset (hb_subset_context_t *c, സ്ഥിരം Tag* tag) സ്ഥിരം
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    തിരികെ അസത്യം;
#endif
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (!tag) return_trace (അസത്യം);
    എങ്കിൽ (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    എങ്കിൽ ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    എങ്കിൽ ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (അസത്യം);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  സ്ഥിരം FeatureParamsSize& get_size_params (hb_tag_t tag) സ്ഥിരം
  {
    എങ്കിൽ (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      തിരികെ u.size;
    തിരികെ Null (FeatureParamsSize);
  }
  സ്ഥിരം FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) സ്ഥിരം
  {
    എങ്കിൽ ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      തിരികെ u.stylisticSet;
    തിരികെ Null (FeatureParamsStylisticSet);
  }
  സ്ഥിരം FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) സ്ഥിരം
  {
    എങ്കിൽ ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      തിരികെ u.characterVariants;
    തിരികെ Null (FeatureParamsCharacterVariants);
  }
#endif

  സ്വകാര്യ:
  സംഘം {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  പൊതു:
  DEFINE_SIZE_MIN (0);
};

ഘടന Record_sanitize_closure_t {
  hb_tag_t tag;
  സ്ഥിരം ശൂന്യം *list_base;
};

ഘടന Feature
{
  unsigned പൂർണ്ണസംഖ്യ get_lookup_എണ്ണം () സ്ഥിരം
  { തിരികെ lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ lookupIndex[i]; }
  unsigned പൂർണ്ണസംഖ്യ get_lookup_indexes (unsigned പൂർണ്ണസംഖ്യ start_index,
				   unsigned പൂർണ്ണസംഖ്യ *lookup_എണ്ണം /* IN/OUT */,
				   unsigned പൂർണ്ണസംഖ്യ *lookup_tags /* OUT */) സ്ഥിരം
  { തിരികെ lookupIndex.get_indexes (start_index, lookup_എണ്ണം, lookup_tags); }
  ശൂന്യം add_lookup_indexes_to (hb_set_t *lookup_indexes) സ്ഥിരം
  { lookupIndex.add_indexes_to (lookup_indexes); }

  സ്ഥിരം FeatureParams &get_feature_params () സ്ഥിരം
  { തിരികെ ഇത്+featureParams; }

  തർക്കം intersects_lookup_indexes (സ്ഥിരം hb_map_t *lookup_indexes) സ്ഥിരം
  { തിരികെ lookupIndex.intersects (lookup_indexes); }

  ശൂന്യം collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  തർക്കം subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       സ്ഥിരം Tag                   *tag = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    out->featureParams.serialize_subset (c, featureParams, ഇത്, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    എങ്കിൽ (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    അല്ലെങ്കിൽ
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
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം Record_sanitize_closure_t *closure = nullptr) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  featureParams.sanitize (c, ഇത്, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  പൊതു:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
ഘടന Record
{
  പൂർണ്ണസംഖ്യ cmp (hb_tag_t a) സ്ഥിരം { തിരികെ tag.cmp (a); }

  തർക്കം subset (hb_subset_layout_context_t *c, സ്ഥിരം ശൂന്യം *base, സ്ഥിരം ശൂന്യം *f_sub = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->subset_context->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    എങ്കിൽ (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    സ്ഥിരം Feature& f = *reinterpret_cast<സ്ഥിരം Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    തർക്കം ret = f.subset (c->subset_context, c, &tag);
    എങ്കിൽ (ret)
      s->add_link (out->offset, s->എടുക്കുക_pack ());
    അല്ലെങ്കിൽ
      s->എടുക്കുക_discard ();

    return_trace (ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    സ്ഥിരം Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (ഇത്) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
ഘടന RecordArrayOf : SortedArray16Of<Record<Type>>
{
  സ്ഥിരം Offset16To<Type>& get_offset (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ (*ഇത്)[i].offset; }
  Offset16To<Type>& get_offset (unsigned പൂർണ്ണസംഖ്യ i)
  { തിരികെ (*ഇത്)[i].offset; }
  സ്ഥിരം Tag& get_tag (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ (*ഇത്)[i].tag; }
  unsigned പൂർണ്ണസംഖ്യ get_tags (unsigned പൂർണ്ണസംഖ്യ start_offset,
                         unsigned പൂർണ്ണസംഖ്യ *record_എണ്ണം /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (record_എണ്ണം && record_tags)
    {
      + ഇത്->as_array ().sub_array (start_offset, record_എണ്ണം)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_എണ്ണം))
      ;
    }
    തിരികെ ഇത്->len;
  }
  തർക്കം find_index (hb_tag_t tag, unsigned പൂർണ്ണസംഖ്യ *index) സ്ഥിരം
  {
    തിരികെ ഇത്->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
ഘടന RecordListOf : RecordArrayOf<Type>
{
  സ്ഥിരം Type& operator [] (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ ഇത്+ഇത്->get_offset (i); }

  തർക്കം subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    + ഇത്->iter ()
    | hb_apply (subset_record_array (l, out, ഇത്))
    ;
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (RecordArrayOf<Type>::sanitize (c, ഇത്));
  }
};

ഘടന RecordListOfFeature : RecordListOf<Feature>
{
  തർക്കം subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    + hb_enumerate (*ഇത്)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, ഇത്] (സ്ഥിരം hb_pair_t<unsigned, സ്ഥിരം Record<Feature>&>& _)
                {
                  സ്ഥിരം Feature *f_sub = nullptr;
                  സ്ഥിരം Feature **f = nullptr;
                  എങ്കിൽ (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, ഇത്, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (സത്യം);
  }
};

തരനിർവചനം RecordListOf<Feature> FeatureList;


ഘടന LangSys
{
  unsigned പൂർണ്ണസംഖ്യ get_feature_എണ്ണം () സ്ഥിരം
  { തിരികെ featureIndex.len; }
  hb_tag_t get_feature_index (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ featureIndex[i]; }
  unsigned പൂർണ്ണസംഖ്യ get_feature_indexes (unsigned പൂർണ്ണസംഖ്യ start_offset,
				    unsigned പൂർണ്ണസംഖ്യ *feature_എണ്ണം_2 /* IN/OUT */,
				    unsigned പൂർണ്ണസംഖ്യ *feature_indexes /* OUT */) സ്ഥിരം
  { തിരികെ featureIndex.get_indexes (start_offset, feature_എണ്ണം_2, feature_indexes); }
  ശൂന്യം add_feature_indexes_to (hb_set_t *feature_indexes) സ്ഥിരം
  { featureIndex.add_indexes_to (feature_indexes); }

  തർക്കം has_required_feature () സ്ഥിരം { തിരികെ reqFeatureIndex != 0xFFFFu; }
  unsigned പൂർണ്ണസംഖ്യ get_required_feature_index () സ്ഥിരം
  {
    എങ്കിൽ (reqFeatureIndex == 0xFFFFu)
      തിരികെ Index::NOT_FOUND_INDEX;
   തിരികെ reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed (*ഇത്));
  }

  തർക്കം compare (സ്ഥിരം LangSys& o, സ്ഥിരം hb_map_t *feature_index_map) സ്ഥിരം
  {
    എങ്കിൽ (reqFeatureIndex != o.reqFeatureIndex)
      തിരികെ അസത്യം;

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

    വേണ്ടി (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      എങ്കിൽ (a != b) തിരികെ അസത്യം;
    }

    എങ്കിൽ (iter || o_iter) തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }

  ശൂന്യം collect_features (hb_prune_langsys_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!has_required_feature () && !get_feature_എണ്ണം ()) തിരികെ;
    എങ്കിൽ (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  തർക്കം subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       സ്ഥിരം Tag                  *tag = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    സ്ഥിരം uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    എങ്കിൽ (!l->visitFeatureIndex (featureIndex.len))
      return_trace (അസത്യം);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    തർക്കം ret = തർക്കം (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം Record_sanitize_closure_t * = nullptr) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  പൊതു:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

ഘടന Script
{
  unsigned പൂർണ്ണസംഖ്യ get_lang_sys_എണ്ണം () സ്ഥിരം
  { തിരികെ langSys.len; }
  സ്ഥിരം Tag& get_lang_sys_tag (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ langSys.get_tag (i); }
  unsigned പൂർണ്ണസംഖ്യ get_lang_sys_tags (unsigned പൂർണ്ണസംഖ്യ start_offset,
				  unsigned പൂർണ്ണസംഖ്യ *lang_sys_എണ്ണം /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) സ്ഥിരം
  { തിരികെ langSys.get_tags (start_offset, lang_sys_എണ്ണം, lang_sys_tags); }
  സ്ഥിരം LangSys& get_lang_sys (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
    എങ്കിൽ (i == Index::NOT_FOUND_INDEX) തിരികെ get_default_lang_sys ();
    തിരികെ ഇത്+langSys[i].offset;
  }
  തർക്കം find_lang_sys_index (hb_tag_t tag, unsigned പൂർണ്ണസംഖ്യ *index) സ്ഥിരം
  { തിരികെ langSys.find_index (tag, index); }

  തർക്കം has_default_lang_sys () സ്ഥിരം           { തിരികെ defaultLangSys != 0; }
  സ്ഥിരം LangSys& get_default_lang_sys () സ്ഥിരം { തിരികെ ഇത്+defaultLangSys; }

  ശൂന്യം prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) സ്ഥിരം
  {
    എങ്കിൽ (!has_default_lang_sys () && !get_lang_sys_എണ്ണം ()) തിരികെ;
    എങ്കിൽ (!c->visitScript ()) തിരികെ;

    എങ്കിൽ (!c->script_langsys_map->has (script_index))
    {
      എങ്കിൽ (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	തിരികെ;
    }

    എങ്കിൽ (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      സ്ഥിരം LangSys& d = get_default_lang_sys ();
      എങ്കിൽ (c->visitLangsys (d.get_feature_എണ്ണം ())) {
        d.collect_features (c);
      }

      വേണ്ടി (auto _ : + hb_enumerate (langSys))
      {
        സ്ഥിരം LangSys& l = ഇത്+_.second.offset;
        എങ്കിൽ (!c->visitLangsys (l.get_feature_എണ്ണം ())) തുടരുക;
        എങ്കിൽ (l.compare (d, c->duplicate_feature_map)) തുടരുക;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    അല്ലെങ്കിൽ
    {
      വേണ്ടി (auto _ : + hb_enumerate (langSys))
      {
        സ്ഥിരം LangSys& l = ഇത്+_.second.offset;
        എങ്കിൽ (!c->visitLangsys (l.get_feature_എണ്ണം ())) തുടരുക;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  തർക്കം subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       സ്ഥിരം Tag                   *tag) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (!l->visitScript ()) return_trace (അസത്യം);
    എങ്കിൽ (tag && !c->plan->layout_scripts.has (*tag))
      തിരികെ അസത്യം;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    തർക്കം defaultLang = അസത്യം;
    എങ്കിൽ (has_default_lang_sys ())
    {
      c->serializer->push ();
      സ്ഥിരം LangSys& ls = ഇത്+defaultLangSys;
      തർക്കം ret = ls.subset (c, l);
      എങ്കിൽ (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->എടുക്കുക_discard ();
	out->defaultLangSys = 0;
      }
      അല്ലെങ്കിൽ
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->എടുക്കുക_pack ());
	defaultLang = സത്യം;
      }
    }

    സ്ഥിരം hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    എങ്കിൽ (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (സ്ഥിരം Record<LangSys>& record) {തിരികെ l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), ഇത്))
      ;
    }

    return_trace (തർക്കം (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം Record_sanitize_closure_t * = nullptr) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (defaultLangSys.sanitize (c, ഇത്) && langSys.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  പൊതു:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

ഘടന RecordListOfScript : RecordListOf<Script>
{
  തർക്കം subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    വേണ്ടി (auto _ : + hb_enumerate (*ഇത്))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      തർക്കം ret = _.second.subset (l, ഇത്);
      എങ്കിൽ (!ret) c->serializer->revert (snap);
      അല്ലെങ്കിൽ out->len++;
    }

    return_trace (സത്യം);
  }
};

തരനിർവചനം RecordListOfScript ScriptList;



ഘടന LookupFlag : HBUINT16
{
  എണ്ണൽ Flags {
    വലത്_To_ഇടത്		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  പൊതു:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
നാമാകാശ OT {

ഘടന Lookup
{
  unsigned പൂർണ്ണസംഖ്യ get_subtable_എണ്ണം () സ്ഥിരം { തിരികെ subTable.len; }

  template <typename TSubTable>
  സ്ഥിരം Array16OfOffset16To<TSubTable>& get_subtables () സ്ഥിരം
  { തിരികെ reinterpret_cast<സ്ഥിരം Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { തിരികെ reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  സ്ഥിരം TSubTable& get_subtable (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ ഇത്+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned പൂർണ്ണസംഖ്യ i)
  { തിരികെ ഇത്+get_subtables<TSubTable> ()[i]; }

  size_t get_size () സ്ഥിരം
  {
    സ്ഥിരം HBUINT16 &markFilteringSet = StructAfter<സ്ഥിരം HBUINT16> (subTable);
    എങ്കിൽ (lookupFlag & LookupFlag::UseMarkFilteringSet)
      തിരികെ (സ്ഥിരം അക്ഷരം *) &StructAfter<സ്ഥിരം അക്ഷരം> (markFilteringSet) - (സ്ഥിരം അക്ഷരം *) ഇത്;
    തിരികെ (സ്ഥിരം അക്ഷരം *) &markFilteringSet - (സ്ഥിരം അക്ഷരം *) ഇത്;
  }

  unsigned പൂർണ്ണസംഖ്യ get_type () സ്ഥിരം { തിരികെ lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ flag = lookupFlag;
    എങ്കിൽ (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      സ്ഥിരം HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    തിരികെ flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ lookup_type = get_type ();
    TRACE_DISPATCH (ഇത്, lookup_type);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = get_subtable_എണ്ണം ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      എങ്കിൽ (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_മൂല്യം ());
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  unsigned പൂർണ്ണസംഖ്യ lookup_type,
		  uint32_t lookup_props,
		  unsigned പൂർണ്ണസംഖ്യ അക്കം_subtables)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    എങ്കിൽ (unlikely (!subTable.serialize (c, അക്കം_subtables))) return_trace (അസത്യം);
    എങ്കിൽ (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      എങ്കിൽ (unlikely (!c->extend (ഇത്))) return_trace (അസത്യം);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (സത്യം);
  }

  template <typename TSubTable>
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    സ്ഥിരം hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned പൂർണ്ണസംഖ്യ lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([ഇത്, glyphset, lookup_type] (സ്ഥിരം Offset16To<TSubTable> &_) { തിരികെ (ഇത്+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), ഇത്, lookup_type))
    ;

    എങ്കിൽ (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      സ്ഥിരം HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      എങ്കിൽ (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      അല്ലെങ്കിൽ
      {
        എങ്കിൽ (unlikely (!c->serializer->extend (out))) return_trace (അസത്യം);
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
    return_trace (സത്യം);
  }

  template <typename TSubTable>
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(c->check_struct (ഇത്) && subTable.sanitize (c))) return_trace (അസത്യം);
    hb_barrier ();

    unsigned subtables = get_subtable_എണ്ണം ();
    എങ്കിൽ (unlikely (!c->visit_subtables (subtables))) return_trace (അസത്യം);

    എങ്കിൽ (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      സ്ഥിരം HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      എങ്കിൽ (!markFilteringSet.sanitize (c)) return_trace (അസത്യം);
    }

    എങ്കിൽ (unlikely (!get_subtables<TSubTable> ().sanitize (c, ഇത്, get_type ())))
      return_trace (അസത്യം);

    എങ്കിൽ (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned പൂർണ്ണസംഖ്യ type = get_subtable<TSubTable> (0).u.extension.get_type ();
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < subtables; i++)
	എങ്കിൽ (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (അസത്യം);
    }
    return_trace (സത്യം);
  }

  സംരക്ഷിത:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  പൊതു:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
ഘടന LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  തർക്കം subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    + hb_enumerate (*ഇത്)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, ഇത്))
    ;
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, ഇത്)));
  }
};


/*
 * Coverage Table
 */


സ്ഥിതം തർക്കം ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  സ്ഥിരം hb_set_t &klasses,
                                          തർക്കം use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  എങ്കിൽ (!klass_map)
    തിരികെ ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  എങ്കിൽ (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  വേണ്ടി (സ്ഥിരം unsigned k: klasses)
  {
    എങ്കിൽ (klass_map->has (k)) തുടരുക;
    klass_map->set (k, idx);
    idx++;
  }


  എങ്കിൽ (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    തിരികെ അസത്യം;

  വേണ്ടി (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  തിരികെ ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
ഘടന ClassDefFormat1_3
{
  friend ഘടന ClassDef;

  സ്വകാര്യ:
  unsigned പൂർണ്ണസംഖ്യ get_class (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    തിരികെ class_മൂല്യം[(unsigned പൂർണ്ണസംഖ്യ) (glyph_id - startGlyph)];
  }

  unsigned get_population () സ്ഥിരം
  {
    തിരികെ class_മൂല്യം.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_മൂല്യം.len = 0;
      return_trace (സത്യം);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_എണ്ണം = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    എങ്കിൽ (unlikely (!class_മൂല്യം.serialize (c, glyph_എണ്ണം))) return_trace (അസത്യം);
    വേണ്ടി (സ്ഥിരം hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_മൂല്യം[idx] = gid_klass_pair.second;
    }
    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               തർക്കം keep_empty_table = സത്യം,
               തർക്കം use_class_zero = സത്യം,
               സ്ഥിരം Coverage* glyph_filter = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_മൂല്യം.len;

    വേണ്ടി (സ്ഥിരം hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      എങ്കിൽ (new_gid == HB_MAP_മൂല്യം_INVALID) തുടരുക;
      എങ്കിൽ (glyph_filter && !glyph_filter->has(gid)) തുടരുക;

      unsigned klass = class_മൂല്യം[gid - start];
      എങ്കിൽ (!klass) തുടരുക;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    എങ്കിൽ (use_class_zero)
    {
      സ്ഥിരം hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_എണ്ണം = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_എണ്ണം <= glyph_and_klass.length;
    }
    എങ്കിൽ (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (അസത്യം);
    return_trace (keep_empty_table || (തർക്കം) glyph_and_klass);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && class_മൂല്യം.sanitize (c));
  }

  unsigned cost () സ്ഥിരം { തിരികെ 1; }

  template <typename set_t>
  തർക്കം collect_coverage (set_t *glyphs) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ start = 0;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = class_മൂല്യം.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (class_മൂല്യം[i])
	തുടരുക;

      എങ്കിൽ (start != i)
	എങ്കിൽ (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  തിരികെ അസത്യം;

      start = i + 1;
    }
    എങ്കിൽ (start != എണ്ണം)
      എങ്കിൽ (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + എണ്ണം)))
	തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }

  template <typename set_t>
  തർക്കം collect_class (set_t *glyphs, unsigned klass) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = class_മൂല്യം.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (class_മൂല്യം[i] == klass) glyphs->add (startGlyph + i);
    തിരികെ സത്യം;
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_മൂല്യം.len;
    വേണ്ടി (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      എങ്കിൽ (class_മൂല്യം[iter - start]) തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }
  തർക്കം intersects_class (സ്ഥിരം hb_set_t *glyphs, unsigned klass) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = class_മൂല്യം.len;
    എങ്കിൽ (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_മൂല്യം_INVALID;
      എങ്കിൽ (!glyphs->next (&g)) തിരികെ അസത്യം;
      എങ്കിൽ (g < startGlyph) തിരികെ സത്യം;
      g = startGlyph + എണ്ണം - 1;
      എങ്കിൽ (glyphs->next (&g)) തിരികെ സത്യം;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    സ്ഥിരം typename Types::HBUINT *arr = class_മൂല്യം.arrayZ;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (arr[i] == klass && glyphs->has (startGlyph + i))
	തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }

  ശൂന്യം intersected_class_glyphs (സ്ഥിരം hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) സ്ഥിരം
  {
    unsigned എണ്ണം = class_മൂല്യം.len;
    എങ്കിൽ (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      വേണ്ടി (uint32_t g = HB_SET_മൂല്യം_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      വേണ്ടി (uint32_t g = startGlyph + എണ്ണം - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      തിരികെ;
    }

    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (class_മൂല്യം[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#എങ്കിൽ 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + എണ്ണം;
    വേണ്ടി (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      എങ്കിൽ (class_മൂല്യം.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  ശൂന്യം intersected_classes (സ്ഥിരം hb_set_t *glyphs, hb_set_t *intersect_classes) സ്ഥിരം
  {
    എങ്കിൽ (glyphs->is_empty ()) തിരികെ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    എങ്കിൽ (!class_മൂല്യം.len)
    {
      intersect_classes->add (0);
      തിരികെ;
    }

    hb_codepoint_t end_glyph = startGlyph + class_മൂല്യം.len - 1;
    എങ്കിൽ (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    വേണ്ടി (സ്ഥിരം auto& _ : + hb_enumerate (class_മൂല്യം))
    {
      hb_codepoint_t g = startGlyph + _.first;
      എങ്കിൽ (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  സംരക്ഷിത:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_മൂല്യം;	/* Array of Class Values--one per GlyphID */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_മൂല്യം);
};

template <typename Types>
ഘടന ClassDefFormat2_4
{
  friend ഘടന ClassDef;

  സ്വകാര്യ:
  unsigned പൂർണ്ണസംഖ്യ get_class (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    തിരികെ rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () സ്ഥിരം
  {
    typename Types::large_int ret = 0;
    വേണ്ടി (സ്ഥിരം auto &r : rangeRecord)
      ret += r.get_population ();
    തിരികെ ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (സത്യം);
    }

    unsigned unsorted = അസത്യം;
    unsigned അക്കം_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    എങ്കിൽ (unlikely (!record)) return_trace (അസത്യം);

    വേണ്ടി (സ്ഥിരം auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      എങ്കിൽ (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	എങ്കിൽ (unlikely (cur_gid < prev_gid))
	  unsorted = സത്യം;

	എങ്കിൽ (unlikely (!record)) നിർത്തുക;
	record->last = prev_gid;
	അക്കം_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    എങ്കിൽ (unlikely (c->in_error ())) return_trace (അസത്യം);

    എങ്കിൽ (likely (record)) record->last = prev_gid;
    rangeRecord.len = അക്കം_ranges;

    എങ്കിൽ (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               തർക്കം keep_empty_table = സത്യം,
               തർക്കം use_class_zero = സത്യം,
               സ്ഥിരം Coverage* glyph_filter = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    എങ്കിൽ (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      വേണ്ടി (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	എങ്കിൽ (!klass) തുടരുക;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	എങ്കിൽ (new_gid == HB_MAP_മൂല്യം_INVALID) തുടരുക;
	എങ്കിൽ (glyph_filter && !glyph_filter->has (g)) തുടരുക;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    അല്ലെങ്കിൽ
    {
      unsigned അക്കം_source_glyphs = c->plan->source->get_അക്കം_glyphs ();
      വേണ്ടി (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	എങ്കിൽ (!klass) തുടരുക;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, അക്കം_source_glyphs);
	വേണ്ടി (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  എങ്കിൽ (new_gid == HB_MAP_മൂല്യം_INVALID) തുടരുക;
	  എങ്കിൽ (glyph_filter && !glyph_filter->has (g)) തുടരുക;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    സ്ഥിരം hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_എണ്ണം = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_എണ്ണം <= glyph_and_klass.length;
    എങ്കിൽ (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (അസത്യം);
    return_trace (keep_empty_table || (തർക്കം) glyph_and_klass);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () സ്ഥിരം { തിരികെ hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  തർക്കം collect_coverage (set_t *glyphs) സ്ഥിരം
  {
    വേണ്ടി (auto &range : rangeRecord)
      എങ്കിൽ (range.value)
	എങ്കിൽ (unlikely (!range.collect_coverage (glyphs)))
	  തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  template <typename set_t>
  തർക്കം collect_class (set_t *glyphs, unsigned പൂർണ്ണസംഖ്യ klass) സ്ഥിരം
  {
    വേണ്ടി (auto &range : rangeRecord)
    {
      എങ്കിൽ (range.value == klass)
	എങ്കിൽ (unlikely (!range.collect_coverage (glyphs)))
	  തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    എങ്കിൽ (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      വേണ്ടി (auto g : *glyphs)
        എങ്കിൽ (get_class (g))
	  തിരികെ സത്യം;
      തിരികെ അസത്യം;
    }

    തിരികെ hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (സ്ഥിരം RangeRecord<Types> &range) { തിരികെ range.intersects (*glyphs) && range.value; }));
  }
  തർക്കം intersects_class (സ്ഥിരം hb_set_t *glyphs, unsigned klass) സ്ഥിരം
  {
    എങ്കിൽ (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      എങ്കിൽ (!rangeRecord.len)
        തിരികെ !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_മൂല്യം_INVALID;
      hb_codepoint_t last = HB_SET_മൂല്യം_INVALID;
      auto it = hb_iter (rangeRecord);
      വേണ്ടി (auto &range : it)
      {
        എങ്കിൽ (it->first == last + 1)
	{
	  it++;
	  തുടരുക;
	}

	എങ്കിൽ (!glyphs->next (&g))
	  നിർത്തുക;
	എങ്കിൽ (g < range.first)
	  തിരികെ സത്യം;
	g = range.last;
	last = g;
      }
      എങ്കിൽ (g != HB_SET_മൂല്യം_INVALID && glyphs->next (&g))
	തിരികെ സത്യം;
      /* Fall through. */
    }
    വേണ്ടി (സ്ഥിരം auto &range : rangeRecord)
      എങ്കിൽ (range.value == klass && range.intersects (*glyphs))
	തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }

  ശൂന്യം intersected_class_glyphs (സ്ഥിരം hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (klass == 0)
    {
      hb_codepoint_t g = HB_SET_മൂല്യം_INVALID;
      വേണ്ടി (auto &range : rangeRecord)
      {
	എങ്കിൽ (!glyphs->next (&g))
	  പോകുക done;
	ആകുമ്പോൾ (g < range.first)
	{
	  intersect_glyphs->add (g);
	  എങ്കിൽ (!glyphs->next (&g))
	    പോകുക done;
        }
        g = range.last;
      }
      ആകുമ്പോൾ (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      തിരികെ;
    }

    unsigned എണ്ണം = rangeRecord.len;
    എങ്കിൽ (എണ്ണം > glyphs->get_population () * hb_bit_storage (എണ്ണം))
    {
      വേണ്ടി (auto g : *glyphs)
      {
        unsigned i;
        എങ്കിൽ (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      തിരികെ;
    }

    വേണ്ടി (auto &range : rangeRecord)
    {
      എങ്കിൽ (range.value != klass) തുടരുക;

      unsigned end = range.last + 1;
      വേണ്ടി (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  ശൂന്യം intersected_classes (സ്ഥിരം hb_set_t *glyphs, hb_set_t *intersect_classes) സ്ഥിരം
  {
    എങ്കിൽ (glyphs->is_empty ()) തിരികെ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    എങ്കിൽ (!rangeRecord.len)
    {
      intersect_classes->add (0);
      തിരികെ;
    }

    hb_codepoint_t g = HB_SET_മൂല്യം_INVALID;
    വേണ്ടി (auto &range : rangeRecord)
    {
      എങ്കിൽ (!glyphs->next (&g))
        നിർത്തുക;
      എങ്കിൽ (g < range.first)
      {
        intersect_classes->add (0);
        നിർത്തുക;
      }
      g = range.last;
    }
    എങ്കിൽ (g != HB_SET_മൂല്യം_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    വേണ്ടി (സ്ഥിരം auto& range : rangeRecord)
      എങ്കിൽ (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  സംരക്ഷിത:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

ഘടന ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) സ്ഥിരം { തിരികെ get (k); }
  തർക്കം has (hb_codepoint_t k) സ്ഥിരം { തിരികെ (*ഇത്)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) സ്ഥിരം { തിരികെ get (k); }

  unsigned പൂർണ്ണസംഖ്യ get (hb_codepoint_t k) സ്ഥിരം { തിരികെ get_class (k); }
  unsigned പൂർണ്ണസംഖ്യ get_class (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_class (glyph_id);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_class (glyph_id);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_class (glyph_id);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }
  unsigned പൂർണ്ണസംഖ്യ get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) സ്ഥിരം
  {
    unsigned klass;
    എങ്കിൽ (cache && cache->get (glyph_id, &klass)) തിരികെ klass;
    klass = get_class (glyph_id);
    എങ്കിൽ (cache) cache->set (glyph_id, klass);
    തിരികെ klass;
  }

  unsigned get_population () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_population ();
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_population ();
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_population ();
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    എങ്കിൽ (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned അക്കം_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      വേണ്ടി (സ്ഥിരം auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	എങ്കിൽ (cur_klass > class_max) class_max = cur_klass;
	എങ്കിൽ (cur_gid == glyph_min) തുടരുക;
        എങ്കിൽ (cur_gid > glyph_max) glyph_max = cur_gid;
	എങ്കിൽ (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  അക്കം_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	എങ്കിൽ (size >= best_size) തിരികെ;
	best_size = size;
	format = candidate;
      };

      എങ്കിൽ (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      എങ്കിൽ (glyph_max <= 0xFFFFu && അക്കം_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) അക്കം_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      എങ്കിൽ (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      എങ്കിൽ (glyph_max <= 0xFFFFFFu && അക്കം_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) അക്കം_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      എങ്കിൽ (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (അസത്യം, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (അസത്യം);
      }
    }

    u.format.v = format;

    തിരഞ്ഞെടുപ്പ് (u.format.v)
    {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  തർക്കം subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               തർക്കം keep_empty_table = സത്യം,
               തർക്കം use_class_zero = സത്യം,
               സ്ഥിരം Coverage* glyph_filter = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  unsigned cost () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.cost ();
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.cost ();
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.cost ();
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  തർക്കം collect_coverage (set_t *glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.collect_coverage (glyphs);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.collect_coverage (glyphs);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.collect_coverage (glyphs);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  തർക്കം collect_class (set_t *glyphs, unsigned പൂർണ്ണസംഖ്യ klass) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.collect_class (glyphs, klass);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.collect_class (glyphs, klass);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.collect_class (glyphs, klass);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.intersects (glyphs);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.intersects (glyphs);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.intersects (glyphs);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }
  തർക്കം intersects_class (സ്ഥിരം hb_set_t *glyphs, unsigned പൂർണ്ണസംഖ്യ klass) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.intersects_class (glyphs, klass);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.intersects_class (glyphs, klass);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.intersects_class (glyphs, klass);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  ശൂന്യം intersected_class_glyphs (സ്ഥിരം hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }

  ശൂന്യം intersected_classes (സ്ഥിരം hb_set_t *glyphs, hb_set_t *intersect_classes) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.intersected_classes (glyphs, intersect_classes);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.intersected_classes (glyphs, intersect_classes);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }


  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
സ്ഥിതം വരിയിൽ തർക്കം ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ തിരികെ (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
ഘടന delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  ഘടന chars_t : hb_vector_t<uint8_t>
  {
    പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം chars_t& other) സ്ഥിരം
    {
      തിരികെ as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      വേണ്ടി (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      തിരികെ hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (സ്ഥിരം chars_t& other) സ്ഥിരം
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      വേണ്ടി (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      തിരികെ hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (സ്ഥിരം delta_row_encoding_t& other_encoding) സ്ഥിരം { തിരികെ chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*> ഇനങ്ങൾ;

  delta_row_encoding_t () = സ്ഥിരസ്ഥിതി;
  delta_row_encoding_t (hb_vector_t<സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*> &&rows, unsigned അക്കം_cols)
  {
    പ്രതിജ്ഞാ (rows);

    ഇനങ്ങൾ = std::move (rows);

    എങ്കിൽ (unlikely (!chars.resize (അക്കം_cols)))
      തിരികെ;

    calculate_chars ();
  }

  ശൂന്യം merge (സ്ഥിരം delta_row_encoding_t& other)
  {
    ഇനങ്ങൾ.alloc (ഇനങ്ങൾ.length + other.ഇനങ്ങൾ.length);
    വേണ്ടി (auto &row : other.ഇനങ്ങൾ)
      add_row (row);

    // Merge chars
    പ്രതിജ്ഞാ (chars.length == other.chars.length);
    വേണ്ടി (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  ശൂന്യം chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  ശൂന്യം calculate_chars ()
  {
    പ്രതിജ്ഞാ (ഇനങ്ങൾ);

    തർക്കം long_വാക്കുകൾ = അസത്യം;

    വേണ്ടി (auto &row : ഇനങ്ങൾ)
    {
      പ്രതിജ്ഞാ (row->length == chars.length);

      /* 0/1/2 byte encoding */
      വേണ്ടി (unsigned i = 0; i < row->length; i++)
      {
	പൂർണ്ണസംഖ്യ v =  row->arrayZ[i];
	എങ്കിൽ (v == 0)
	  തുടരുക;
	അല്ലെങ്കിൽ എങ്കിൽ (v > 32767 || v < -32768)
	{
	  long_വാക്കുകൾ = സത്യം;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	അല്ലെങ്കിൽ
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    എങ്കിൽ (long_വാക്കുകൾ)
    {
      // Convert 1s to 2s
      വേണ്ടി (auto &v : chars)
	എങ്കിൽ (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  തർക്കം is_empty () സ്ഥിരം
  { തിരികെ !ഇനങ്ങൾ; }

  തർക്കം in_error () സ്ഥിരം
  { തിരികെ chars.in_error () || ഇനങ്ങൾ.in_error (); }

  സ്ഥിതം വരിയിൽ unsigned get_chars_overhead (unsigned അക്കം_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    തിരികെ c + അക്കം_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) സ്ഥിരം
  {
    പൂർണ്ണസംഖ്യ എണ്ണം = ഇനങ്ങൾ.length;
    തിരികെ hb_max (0, (പൂർണ്ണസംഖ്യ) overhead - എണ്ണം * (പൂർണ്ണസംഖ്യ) additional_bytes_per_rows);
  }

  പൂർണ്ണസംഖ്യ gain_from_merging (സ്ഥിരം delta_row_encoding_t& other_encoding) സ്ഥിരം
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    എങ്കിൽ (additional_bytes_per_rows > 0)
    {
      എങ്കിൽ (get_gain (additional_bytes_per_rows) == 0)
        തിരികെ 0;
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	തിരികെ 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    പൂർണ്ണസംഖ്യ combined_gain = (പൂർണ്ണസംഖ്യ) overhead + (പൂർണ്ണസംഖ്യ) other_encoding.overhead;
    combined_gain -= (combined_width - (പൂർണ്ണസംഖ്യ) width) * ഇനങ്ങൾ.length;
    combined_gain -= (combined_width - (പൂർണ്ണസംഖ്യ) other_encoding.width) * other_encoding.ഇനങ്ങൾ.length;
    combined_gain -= get_chars_overhead (combined_columns);

    തിരികെ combined_gain;
  }

  തർക്കം add_row (സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>* row)
  { തിരികെ ഇനങ്ങൾ.push (row); }

  സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    സ്ഥിരം delta_row_encoding_t *a = (സ്ഥിരം delta_row_encoding_t *)pa;
    സ്ഥിരം delta_row_encoding_t *b = (സ്ഥിരം delta_row_encoding_t *)pb;

    എങ്കിൽ (a->width != b->width)
      തിരികെ (പൂർണ്ണസംഖ്യ) a->width - (പൂർണ്ണസംഖ്യ) b->width;

    തിരികെ b->chars.cmp (a->chars);
  }
};

ഘടന VarRegionAxis
{
  ദശാംശം evaluate (പൂർണ്ണസംഖ്യ coord) സ്ഥിരം
  {
    പൂർണ്ണസംഖ്യ peak = peakCoord.to_int ();
    എങ്കിൽ (peak == 0 || coord == peak)
      തിരികെ 1.f;
    അല്ലെങ്കിൽ എങ്കിൽ (coord == 0) // Faster
      തിരികെ 0.f;

    പൂർണ്ണസംഖ്യ start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    എങ്കിൽ (unlikely (start > peak || peak > end))
      തിരികെ 1.f;
    എങ്കിൽ (unlikely (start < 0 && end > 0))
      തിരികെ 1.f;

    എങ്കിൽ (coord <= start || end <= coord)
      തിരികെ 0.f;

    /* Interpolate */
    എങ്കിൽ (coord < peak)
      തിരികെ ദശാംശം (coord - start) / (peak - start);
    അല്ലെങ്കിൽ
      തിരികെ ദശാംശം (end - coord) / (end - peak);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  തർക്കം serialize (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed (ഇത്));
  }

  പൊതു:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};
ഘടന SparseVarRegionAxis
{
  ദശാംശം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len) സ്ഥിരം
  {
    unsigned i = axisIndex;
    പൂർണ്ണസംഖ്യ coord = i < coord_len ? coords[i] : 0;
    തിരികെ axis.evaluate (coord);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം SparseVarRegionAxis *src,
		  സ്ഥിരം hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->embed (src);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);
    എങ്കിൽ (axis_map)
    {
      എങ്കിൽ (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (അസത്യം);
      }
      എങ്കിൽ (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (അസത്യം);
    }
    return_trace (സത്യം);
  }

  പൊതു:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന hb_scalar_cache_t
{
  സ്വകാര്യ:
  സ്ഥിതം constexpr unsigned STATIC_LENGTH = 128;
  സ്ഥിതം constexpr ദശാംശം INVALID = NAN;

  പൊതു:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (സ്ഥിരം hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (സ്ഥിരം hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  സ്ഥിതം hb_scalar_cache_t *create (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    എങ്കിൽ (!എണ്ണം) തിരികെ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    എങ്കിൽ (scratch_cache && എണ്ണം <= STATIC_LENGTH)
    {
      scratch_cache->length = എണ്ണം;
      scratch_cache->clear ();
      തിരികെ scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (വലുപ്പം (hb_scalar_cache_t) - വലുപ്പം (static_മൂല്യങ്ങൾ) + വലുപ്പം (static_മൂല്യങ്ങൾ[0]) * എണ്ണം);
    എങ്കിൽ (unlikely (!cache)) തിരികെ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = എണ്ണം;
    cache->clear ();

    തിരികെ cache;
  }

  സ്ഥിതം ശൂന്യം destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    എങ്കിൽ (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  ശൂന്യം clear ()
  {
    auto *values = &static_മൂല്യങ്ങൾ[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    വേണ്ടി (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    വേണ്ടി (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  തർക്കം get (unsigned i, ദശാംശം *value) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (i >= length))
    {
      *value = 0.f;
      തിരികെ സത്യം;
    }
    auto *values = &static_മൂല്യങ്ങൾ[0];
    auto *cached_മൂല്യം = &values[i];
    ദശാംശം v = *cached_മൂല്യം;
    എങ്കിൽ (std::isnan (v))
      തിരികെ അസത്യം;
    *value = v;
    തിരികെ സത്യം;
  }

  HB_ALWAYS_INLINE
  ശൂന്യം set (unsigned i, ദശാംശം value)
  {
    എങ്കിൽ (unlikely (i >= length)) തിരികെ;
    auto *values = &static_മൂല്യങ്ങൾ[0];
    auto *cached_മൂല്യം = &values[i];
    *cached_മൂല്യം = value;
  }

  പൊതു:
  unsigned length;
  സ്വകാര്യ:
  mutable hb_atomic_t<ദശാംശം> static_മൂല്യങ്ങൾ[STATIC_LENGTH];
};

ഘടന VarRegionList
{
  സ്വകാര്യ:
  ദശാംശം evaluate_impl (unsigned പൂർണ്ണസംഖ്യ region_index,
		       സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len) സ്ഥിരം
  {
    സ്ഥിരം VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_എണ്ണം_2);
    ദശാംശം v = 1.f;

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = axis_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      പൂർണ്ണസംഖ്യ coord = i < coord_len ? coords[i] : 0;
      ദശാംശം factor = axes[i].evaluate (coord);
      എങ്കിൽ (factor == 0.f)
      {
	v = 0.f;
	നിർത്തുക;
      }
      v *= factor;
    }

    തിരികെ v;
  }

  പൊതു:
  HB_ALWAYS_INLINE
  ദശാംശം evaluate (unsigned പൂർണ്ണസംഖ്യ region_index,
		  സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		  hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (region_index >= region_എണ്ണം))
      തിരികെ 0.;

    ദശാംശം v;
    എങ്കിൽ (cache && cache->get (region_index, &v))
      തിരികെ v;

    v = evaluate_impl (region_index, coords, coord_len);

    എങ്കിൽ (cache)
      cache->set (region_index, v);
    തിരികെ v;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_എണ്ണം_2 * region_എണ്ണം));
  }

  തർക്കം serialize (hb_serialize_context_t *c,
                  സ്ഥിരം hb_vector_t<hb_tag_t>& axis_tags,
                  സ്ഥിരം hb_vector_t<സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned axis_എണ്ണം = axis_tags.length;
    unsigned region_എണ്ണം_2 = regions.length;
    എങ്കിൽ (!axis_എണ്ണം || !region_എണ്ണം_2) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (axis_എണ്ണം * region_എണ്ണം_2,
                                             VarRegionAxis::static_size))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    axis_എണ്ണം_2 = axis_എണ്ണം;
    region_എണ്ണം = region_എണ്ണം_2;

    വേണ്ടി (unsigned r = 0; r < region_എണ്ണം_2; r++)
    {
      സ്ഥിരം auto& region = regions[r];
      വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        എങ്കിൽ (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->നടുവ്);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        അല്ലെങ്കിൽ
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        എങ്കിൽ (!var_region_rec.serialize (c))
          return_trace (അസത്യം);
      }
    }
    return_trace (സത്യം);
  }

  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം VarRegionList *src, സ്ഥിരം hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    axis_എണ്ണം_2 = src->axis_എണ്ണം_2;
    region_എണ്ണം = region_map.get_population ();
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (axis_എണ്ണം_2 * region_എണ്ണം,
					     VarRegionAxis::static_size))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->extend (ഇത്))) return_trace (അസത്യം);
    unsigned പൂർണ്ണസംഖ്യ region_എണ്ണം_2 = src->region_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ r = 0; r < region_എണ്ണം; r++)
    {
      unsigned പൂർണ്ണസംഖ്യ backward = region_map.backward (r);
      എങ്കിൽ (backward >= region_എണ്ണം_2) return_trace (അസത്യം);
      hb_memcpy (&axesZ[axis_എണ്ണം_2 * r], &src->axesZ[axis_എണ്ണം_2 * backward], VarRegionAxis::static_size * axis_എണ്ണം_2);
    }

    return_trace (സത്യം);
  }

  തർക്കം get_var_region (unsigned region_index,
                       സ്ഥിരം hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (region_index >= region_എണ്ണം) തിരികെ അസത്യം;
    സ്ഥിരം VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_എണ്ണം_2);
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      hb_tag_t *axis_tag;
      എങ്കിൽ (!axes_old_index_tag_map.has (i, &axis_tag))
        തിരികെ അസത്യം;

      ദശാംശം min_val = axis_region->startCoord.to_float ();
      ദശാംശം def_val = axis_region->peakCoord.to_float ();
      ദശാംശം max_val = axis_region->endCoord.to_float ();

      എങ്കിൽ (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((ഇരട്ട) min_val, (ഇരട്ട) def_val, (ഇരട്ട) max_val));
      axis_region++;
    }
    തിരികെ !axis_tuples.in_error ();
  }

  തർക്കം get_var_regions (സ്ഥിരം hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (!regions.alloc (region_എണ്ണം))
      തിരികെ അസത്യം;

    വേണ്ടി (unsigned i = 0; i < region_എണ്ണം; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      എങ്കിൽ (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        തിരികെ അസത്യം;
      regions.push (std::move (axis_tuples));
    }
    തിരികെ !regions.in_error ();
  }

  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_എണ്ണം_2, region_എണ്ണം)); }

  പൊതു:
  HBUINT16	axis_എണ്ണം_2;
  HBUINT15	region_എണ്ണം;
  സംരക്ഷിത:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  പൊതു:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
സ്ഥിതം വരിയിൽ തർക്കം
_hb_avar2_region_is_dead (സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  വേണ്ടി (auto _ : axis_tuples)
  {
    Triple *range;
    എങ്കിൽ (!reachable_ranges.has (_.first, &range)) തുടരുക;
    സ്ഥിരം Triple &tent = _.second;
    പൂർണ്ണസംഖ്യ start = (പൂർണ്ണസംഖ്യ) roundf ((ദശാംശം) tent.minimum * 16384.f);
    പൂർണ്ണസംഖ്യ peak  = (പൂർണ്ണസംഖ്യ) roundf ((ദശാംശം) tent.നടുവ് * 16384.f);
    പൂർണ്ണസംഖ്യ end   = (പൂർണ്ണസംഖ്യ) roundf ((ദശാംശം) tent.maximum * 16384.f);
    എങ്കിൽ (!peak) തുടരുക;
    എങ്കിൽ (start > peak || peak > end) തുടരുക;
    എങ്കിൽ (start < 0 && end > 0) തുടരുക;
    പൂർണ്ണസംഖ്യ lo = (പൂർണ്ണസംഖ്യ) roundf ((ദശാംശം) range->minimum * 16384.f);
    പൂർണ്ണസംഖ്യ hi = (പൂർണ്ണസംഖ്യ) roundf ((ദശാംശം) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    എങ്കിൽ (hi < start || lo > end)
      തിരികെ സത്യം;
  }
  തിരികെ അസത്യം;
}

ഘടന SparseVariationRegion
{
  ദശാംശം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len) സ്ഥിരം
  {
    ദശാംശം v = 1.f;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = axes.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      ദശാംശം factor = (ഇത്+axes[i]).evaluate (coords, coord_len);
      എങ്കിൽ (factor == 0.f)
	തിരികെ 0.;
      v *= factor;
    }
    തിരികെ v;
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം SparseVariationRegion *src,
		  സ്ഥിരം hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (അസത്യം);
    വേണ്ടി (unsigned i = 0; i < axes.len; i++)
      എങ്കിൽ (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (axes.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  പൊതു:
  DEFINE_SIZE_ARRAY (2, axes);
};

ഘടന SparseVarRegionList
{
  HB_ALWAYS_INLINE
  ദശാംശം evaluate (unsigned പൂർണ്ണസംഖ്യ region_index,
		  സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		  hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (region_index >= regions.len))
      തിരികെ 0.;

    ദശാംശം v;
    എങ്കിൽ (cache && cache->get (region_index, &v))
      തിരികെ v;

    സ്ഥിരം SparseVariationRegion &region = ഇത്+regions[region_index];

    v = region.evaluate (coords, coord_len);
    എങ്കിൽ (cache)
      cache->set (region_index, v);

    തിരികെ v;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (regions.sanitize (c, ഇത്));
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം SparseVarRegionList *src,
		  സ്ഥിരം hb_inc_bimap_t &region_map,
		  സ്ഥിരം hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    regions.len = region_map.get_population ();
    എങ്കിൽ (unlikely (!c->extend (regions))) return_trace (അസത്യം);

    വേണ്ടി (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      എങ്കിൽ (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (അസത്യം);
    }
    return_trace (സത്യം);
  }

  പൊതു:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  പൊതു:
  DEFINE_SIZE_ARRAY (4, regions);
};


ഘടന VarData
{
  unsigned പൂർണ്ണസംഖ്യ get_item_എണ്ണം () സ്ഥിരം
  { തിരികെ item_എണ്ണം; }

  unsigned പൂർണ്ണസംഖ്യ get_region_index_എണ്ണം () സ്ഥിരം
  { തിരികെ regionIndices.len; }

  unsigned get_region_index (unsigned i) സ്ഥിരം
  { തിരികെ i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () സ്ഥിരം
  { തിരികെ (word_എണ്ണം () + regionIndices.len) * (long_വാക്കുകൾ_2 () ? 2 : 1); }

  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_എണ്ണം, get_row_size ())); }

  template <typename Float = ദശാംശം>
  Float _get_delta (unsigned പൂർണ്ണസംഖ്യ inner,
		    സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
		    സ്ഥിരം VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (inner >= item_എണ്ണം))
      തിരികെ 0.;
    തർക്കം is_long = long_വാക്കുകൾ_2 ();
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = regionIndices.len;
    unsigned word_എണ്ണം_2 = word_എണ്ണം ();
    unsigned പൂർണ്ണസംഖ്യ scount = is_long ? എണ്ണം : word_എണ്ണം_2;
    unsigned പൂർണ്ണസംഖ്യ lcount = is_long ? word_എണ്ണം_2 : 0;

    സ്ഥിരം HBUINT8 *bytes = get_delta_bytes ();
    സ്ഥിരം HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned പൂർണ്ണസംഖ്യ i = 0;

    സ്ഥിരം HBINT32 *lcursor = reinterpret_cast<സ്ഥിരം HBINT32 *> (row);
    വേണ്ടി (; i < lcount; i++)
    {
      ദശാംശം scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_എണ്ണം, cache);
      എങ്കിൽ (scalar)
        delta += (Float) scalar * (പൂർണ്ണസംഖ്യ) *lcursor;
      lcursor++;
    }
    സ്ഥിരം HBINT16 *scursor = reinterpret_cast<സ്ഥിരം HBINT16 *> (lcursor);
    വേണ്ടി (; i < scount; i++)
    {
      ദശാംശം scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_എണ്ണം, cache);
      എങ്കിൽ (scalar)
       delta += (Float) scalar * (പൂർണ്ണസംഖ്യ) *scursor;
      scursor++;
    }
    സ്ഥിരം HBINT8 *bcursor = reinterpret_cast<സ്ഥിരം HBINT8 *> (scursor);
    വേണ്ടി (; i < എണ്ണം; i++)
    {
      ദശാംശം scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_എണ്ണം, cache);
      എങ്കിൽ (scalar)
        delta += (Float) scalar * (പൂർണ്ണസംഖ്യ) *bcursor;
      bcursor++;
    }

    തിരികെ delta;
  }

  template <typename Float = ദശാംശം>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned പൂർണ്ണസംഖ്യ inner,
		   സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
		   സ്ഥിരം VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = regionIndices.len;
    എങ്കിൽ (!എണ്ണം) തിരികെ Float (0); // This is quite common, so optimize it.
    തിരികെ _get_delta<Float> (inner, coords, coord_എണ്ണം, regions, cache);
  }

  ശൂന്യം get_region_scalars (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
			   സ്ഥിരം VarRegionList &regions,
			   ദശാംശം *scalars /*OUT */,
			   unsigned പൂർണ്ണസംഖ്യ അക്കം_scalars) സ്ഥിരം
  {
    unsigned എണ്ണം = hb_min (അക്കം_scalars, regionIndices.len);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_എണ്ണം);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = എണ്ണം; i < അക്കം_scalars; i++)
      scalars[i] = 0.f;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_എണ്ണം () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_എണ്ണം,
				  get_row_size ()));
  }

  തർക്കം serialize (hb_serialize_context_t *c,
                  തർക്കം has_long,
                  സ്ഥിരം hb_vector_t<സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>*>& rows)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned row_എണ്ണം_2 = rows.length;
    എങ്കിൽ (!row_എണ്ണം_2) {
      // Nothing to serialize, will be empty.
      തിരികെ അസത്യം;
    }

    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->check_assign (item_എണ്ണം, row_എണ്ണം_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (അസത്യം);

    പൂർണ്ണസംഖ്യ min_threshold = has_long ? -32768 : -128;
    പൂർണ്ണസംഖ്യ max_threshold = has_long ? +32767 : +127;
    എണ്ണൽ delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned അക്കം_regions = rows[0]->length;
    എങ്കിൽ (!delta_sz.resize (അക്കം_regions))
      return_trace (അസത്യം);

    unsigned word_എണ്ണം_2 = 0;
    വേണ്ടി (unsigned r = 0; r < അക്കം_regions; r++)
    {
      വേണ്ടി (unsigned i = 0; i < row_എണ്ണം_2; i++)
      {
        പൂർണ്ണസംഖ്യ delta = rows[i]->arrayZ[r];
        എങ്കിൽ (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_എണ്ണം_2++;
          നിർത്തുക;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_എണ്ണം_2;
    hb_map_t ri_map;
    വേണ്ടി (unsigned r = 0; r < അക്കം_regions; r++)
    {
      എങ്കിൽ (!delta_sz[r]) തുടരുക;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      എങ്കിൽ (!ri_map.set (new_r, r))
        return_trace (അസത്യം);
    }

    word_Size_എണ്ണം = word_എണ്ണം_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_എണ്ണം = ri_map.get_population ();
    regionIndices.len = ri_എണ്ണം;
    എങ്കിൽ (unlikely (!c->extend (ഇത്))) return_trace (അസത്യം);

    വേണ്ടി (unsigned r = 0; r < ri_എണ്ണം; r++)
    {
      hb_codepoint_t *idx;
      എങ്കിൽ (!ri_map.has (r, &idx))
        return_trace (അസത്യം);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < row_എണ്ണം_2; i++)
    {
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ r = 0; r < ri_എണ്ണം; r++)
      {
        പൂർണ്ണസംഖ്യ delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (സത്യം);
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം VarData *src,
		  സ്ഥിരം hb_inc_bimap_t &inner_map,
		  സ്ഥിരം hb_inc_bimap_t &region_map,
		  സ്ഥിരം hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    item_എണ്ണം = inner_map.get_next_മൂല്യം ();

    /* Optimize word count */
    unsigned ri_എണ്ണം = src->regionIndices.len;
    എണ്ണൽ delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_എണ്ണം);
    ri_map.resize (ri_എണ്ണം);
    unsigned പൂർണ്ണസംഖ്യ new_word_എണ്ണം = 0;
    unsigned പൂർണ്ണസംഖ്യ r;

    സ്ഥിരം HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_എണ്ണം = src->word_എണ്ണം ();
    തർക്കം     src_long_വാക്കുകൾ = src->long_വാക്കുകൾ_2 ();

    തർക്കം has_long = അസത്യം;
    എങ്കിൽ (src_long_വാക്കുകൾ)
    {
      വേണ്ടി (r = 0; r < src_word_എണ്ണം; r++)
      {
        വേണ്ടി (unsigned old_gid : inner_map.താക്കോലുകൾ())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  എങ്കിൽ (delta < -32768 || 32767 < delta)
	  {
	    has_long = സത്യം;
	    നിർത്തുക;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    വേണ്ടി (r = 0; r < ri_എണ്ണം; r++)
    {
      തർക്കം short_circuit = src_long_വാക്കുകൾ == has_long && src_word_എണ്ണം <= r;

      delta_sz[r] = kZero;
      എങ്കിൽ (culled_regions && culled_regions->has (src->regionIndices[r]))
	തുടരുക; /* unreachable region: drop its column */
      വേണ്ടി (unsigned old_gid : inner_map.താക്കോലുകൾ())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	എങ്കിൽ (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_എണ്ണം++;
	  നിർത്തുക;
	}
	അല്ലെങ്കിൽ എങ്കിൽ (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  എങ്കിൽ (short_circuit)
	    നിർത്തുക;
	}
      }
    }

    unsigned പൂർണ്ണസംഖ്യ word_index = 0;
    unsigned പൂർണ്ണസംഖ്യ non_word_index = new_word_എണ്ണം;
    unsigned പൂർണ്ണസംഖ്യ new_ri_എണ്ണം = 0;
    വേണ്ടി (r = 0; r < ri_എണ്ണം; r++)
      എങ്കിൽ (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_എണ്ണം++;
      }

    word_Size_എണ്ണം = new_word_എണ്ണം | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_എണ്ണം;

    എങ്കിൽ (unlikely (!c->extend (ഇത്))) return_trace (അസത്യം);

    വേണ്ടി (r = 0; r < new_ri_എണ്ണം; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned എണ്ണം = item_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ old = inner_map.backward (i);
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ r = 0; r < new_ri_എണ്ണം; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (സത്യം);
  }

  ശൂന്യം collect_region_refs (hb_set_t &region_indices, സ്ഥിരം hb_inc_bimap_t &inner_map) സ്ഥിരം
  {
    സ്ഥിരം HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ r = 0; r < regionIndices.len; r++)
    {
      unsigned പൂർണ്ണസംഖ്യ region = regionIndices.arrayZ[r];
      എങ്കിൽ (region_indices.has (region)) തുടരുക;
      വേണ്ടി (hb_codepoint_t old_gid : inner_map.താക്കോലുകൾ())
	എങ്കിൽ (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  നിർത്തുക;
	}
    }
  }

  പൊതു:
  സ്ഥിരം HBUINT8 *get_delta_bytes () സ്ഥിരം
  { തിരികെ &StructAfter<HBUINT8> (regionIndices); }

  സംരക്ഷിത:
  HBUINT8 *get_delta_bytes ()
  { തിരികെ &StructAfter<HBUINT8> (regionIndices); }

  പൊതു:
  int32_t get_item_delta_fast (unsigned പൂർണ്ണസംഖ്യ item, unsigned പൂർണ്ണസംഖ്യ region,
			       സ്ഥിരം HBUINT8 *delta_bytes, unsigned row_size) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (item >= item_എണ്ണം || region >= regionIndices.len)) തിരികെ 0;

    സ്ഥിരം HBINT8 *p = (സ്ഥിരം HBINT8 *) delta_bytes + item * row_size;
    unsigned word_എണ്ണം_2 = word_എണ്ണം ();
    തർക്കം is_long = long_വാക്കുകൾ_2 ();
    എങ്കിൽ (is_long)
    {
      എങ്കിൽ (region < word_എണ്ണം_2)
	തിരികെ ((സ്ഥിരം HBINT32 *) p)[region];
      അല്ലെങ്കിൽ
	തിരികെ ((സ്ഥിരം HBINT16 *)(p + HBINT32::static_size * word_എണ്ണം_2))[region - word_എണ്ണം_2];
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (region < word_എണ്ണം_2)
	തിരികെ ((സ്ഥിരം HBINT16 *) p)[region];
      അല്ലെങ്കിൽ
	തിരികെ (p + HBINT16::static_size * word_എണ്ണം_2)[region - word_എണ്ണം_2];
    }
  }
  int32_t get_item_delta (unsigned പൂർണ്ണസംഖ്യ item, unsigned പൂർണ്ണസംഖ്യ region) സ്ഥിരം
  {
     തിരികെ get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  സംരക്ഷിത:
  ശൂന്യം set_item_delta_fast (unsigned പൂർണ്ണസംഖ്യ item, unsigned പൂർണ്ണസംഖ്യ region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_എണ്ണം_2 = word_എണ്ണം ();
    തർക്കം is_long = long_വാക്കുകൾ_2 ();
    എങ്കിൽ (is_long)
    {
      എങ്കിൽ (region < word_എണ്ണം_2)
	((HBINT32 *) p)[region] = delta;
      അല്ലെങ്കിൽ
	((HBINT16 *)(p + HBINT32::static_size * word_എണ്ണം_2))[region - word_എണ്ണം_2] = delta;
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (region < word_എണ്ണം_2)
	((HBINT16 *) p)[region] = delta;
      അല്ലെങ്കിൽ
	(p + HBINT16::static_size * word_എണ്ണം_2)[region - word_എണ്ണം_2] = delta;
    }
  }
  ശൂന്യം set_item_delta (unsigned പൂർണ്ണസംഖ്യ item, unsigned പൂർണ്ണസംഖ്യ region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  തർക്കം long_വാക്കുകൾ_2 () സ്ഥിരം { തിരികെ word_Size_എണ്ണം & 0x8000u /* LONG_WORDS */; }
  unsigned word_എണ്ണം () സ്ഥിരം { തിരികെ word_Size_എണ്ണം & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  സംരക്ഷിത:
  HBUINT16		item_എണ്ണം;
  HBUINT16		word_Size_എണ്ണം;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  പൊതു:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

ഘടന MultiVarData
{
  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  സ്ഥിരം TupleList &get_delta_sets () സ്ഥിരം
  { തിരികെ ഇത്+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  ശൂന്യം get_delta (unsigned പൂർണ്ണസംഖ്യ inner,
		  സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
		  സ്ഥിരം SparseVarRegionList &regions,
		  hb_array_t<ഇരട്ട> out,
		  hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    auto &deltaSets = get_delta_sets ();

    auto മൂല്യങ്ങൾ_iter = deltaSets.fetcher (inner);
    unsigned region_എണ്ണം = regionIndices.len;
    unsigned skip = 0;
    വേണ്ടി (unsigned regionIndex = 0; regionIndex < region_എണ്ണം; regionIndex++)
    {
      ദശാംശം scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_എണ്ണം,
				       cache);
      // We skip lazily. Helps with the tail end.
      എങ്കിൽ (scalar == 0.0f)
        skip += out.length;
      അല്ലെങ്കിൽ
      {
        എങ്കിൽ (skip)
	{
	  മൂല്യങ്ങൾ_iter.skip (skip);
	  skip = 0;
	}
	മൂല്യങ്ങൾ_iter.add_to (out, scalar);
      }
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, ഇത്));
  }

  unsigned get_item_എണ്ണം () സ്ഥിരം
  { തിരികെ get_delta_sets ().എണ്ണം; }

  ശൂന്യം collect_region_refs (hb_set_t &region_indices) സ്ഥിരം
  {
    വേണ്ടി (unsigned region : regionIndices)
      region_indices.add (region);
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം MultiVarData *src,
		  സ്ഥിരം hb_inc_bimap_t &inner_map,
		  സ്ഥിരം hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned region_എണ്ണം_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_എണ്ണം_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    എങ്കിൽ (unlikely (!c->extend_size (ഇത്, header_size, അസത്യം)))
      return_trace (അസത്യം);
    format = 1;
    regionIndices.len = region_എണ്ണം_2;
    വേണ്ടി (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      എങ്കിൽ (unlikely (!region_map.has (old_region)))
	return_trace (അസത്യം);
      regionIndices[i] = region_map.get (old_region);
    }

    സ്ഥിരം auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    എങ്കിൽ (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (അസത്യം);
    വേണ്ടി (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      എങ്കിൽ (unlikely (old_inner >= src_delta_sets.എണ്ണം)) return_trace (അസത്യം);
      hb_ubytes_t bytes = static_cast<സ്ഥിരം CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    എങ്കിൽ (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (അസത്യം);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  സംരക്ഷിത:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  പൊതു:
  DEFINE_SIZE_MIN (7);
};

ഘടന ItemVariationStore
{
  friend ഘടന item_variations_t;

  hb_scalar_cache_t *create_cache () സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ hb_scalar_cache_t::create (0);
#endif
    തിരികെ hb_scalar_cache_t::create ((ഇത്+regions).region_എണ്ണം);
  }

  സ്ഥിതം ശൂന്യം destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  സ്വകാര്യ:
  template <typename Float = ദശാംശം>
  Float get_delta (unsigned പൂർണ്ണസംഖ്യ outer, unsigned പൂർണ്ണസംഖ്യ inner,
		   സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
		   hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ Float (0);
#endif

    എങ്കിൽ (unlikely (outer >= dataSets.len))
      തിരികെ Float (0);

    തിരികെ (ഇത്+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_എണ്ണം,
					     ഇത്+regions,
					     cache);
  }

  പൊതു:
  തർക്കം has_delta_set (unsigned പൂർണ്ണസംഖ്യ index) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ അസത്യം;
#endif

    unsigned പൂർണ്ണസംഖ്യ outer = index >> 16;
    unsigned പൂർണ്ണസംഖ്യ inner = index & 0xFFFF;
    തിരികെ outer < dataSets.len &&
	   inner < (ഇത്+dataSets[outer]).get_item_എണ്ണം ();
  }

  template <typename Float = ദശാംശം>
  Float get_delta (unsigned പൂർണ്ണസംഖ്യ index,
		   സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
		   hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ outer = index >> 16;
    unsigned പൂർണ്ണസംഖ്യ inner = index & 0xFFFF;
    തിരികെ get_delta<Float> (outer, inner, coords, coord_എണ്ണം, cache);
  }
  template <typename Float = ദശാംശം>
  Float get_delta (unsigned പൂർണ്ണസംഖ്യ index,
		   hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
		   hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    തിരികെ get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ സത്യം;
#endif

    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ഇത്) &&
		  dataSets.sanitize (c, ഇത്));
  }

  തർക്കം serialize (hb_serialize_context_t *c,
                  തർക്കം has_long,
                  സ്ഥിരം hb_vector_t<hb_tag_t>& axis_tags,
                  സ്ഥിരം hb_vector_t<സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  സ്ഥിരം hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (ഇത്);
#ifdef HB_NO_VAR
    return_trace (അസത്യം);
#endif
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    format = 1;
    എങ്കിൽ (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (അസത്യം);

    unsigned അക്കം_var_data = vardata_encodings.length;
    എങ്കിൽ (!അക്കം_var_data) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->check_assign (dataSets.len, അക്കം_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!c->extend (dataSets))) return_trace (അസത്യം);
    വേണ്ടി (unsigned i = 0; i < അക്കം_var_data; i++)
      എങ്കിൽ (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].ഇനങ്ങൾ))
        return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം ItemVariationStore *src,
		  സ്ഥിരം hb_array_t <സ്ഥിരം hb_inc_bimap_t> &inner_maps,
		  സ്ഥിരം hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (ഇത്);
#ifdef HB_NO_VAR
    return_trace (അസത്യം);
#endif

    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    unsigned പൂർണ്ണസംഖ്യ set_എണ്ണം = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < inner_maps.length; i++)
      എങ്കിൽ (inner_maps[i].get_population ())
	set_എണ്ണം++;

    format = 1;

    സ്ഥിരം auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    എങ്കിൽ (region_indices.in_error ())
      return_trace (അസത്യം);

    region_indices.del_range ((src_regions).region_എണ്ണം, hb_set_t::INVALID);

    എങ്കിൽ (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    എങ്കിൽ (region_map.in_error())
      return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (അസത്യം);

    dataSets.len = set_എണ്ണം;
    എങ്കിൽ (unlikely (!c->extend (dataSets))) return_trace (അസത്യം);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned പൂർണ്ണസംഖ്യ set_index = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < inner_maps.length; i++)
    {
      എങ്കിൽ (!inner_maps[i].get_population ()) തുടരുക;
      എങ്കിൽ (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned എണ്ണം = dataSets.len;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      എങ്കിൽ (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = ഇത്+dataSets[i];

      unsigned item_എണ്ണം = data.get_item_എണ്ണം ();
      വേണ്ടി (unsigned j = 0; j < item_എണ്ണം; j++)
	map->add (j);
    }

    എങ്കിൽ (unlikely (!out->serialize (c, ഇത്, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  തർക്കം collect_dead_regions (സ്ഥിരം hb_map_t &axes_old_index_tag_map,
			     സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) സ്ഥിരം
  {
#ifndef HB_NO_VAR
    എങ്കിൽ (!reachable_ranges.get_population ()) തിരികെ സത്യം;
    സ്ഥിരം VarRegionList &region_list = ഇത്+regions;
    unsigned എണ്ണം = region_list.region_എണ്ണം;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      എങ്കിൽ (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	തിരികെ അസത്യം;
      എങ്കിൽ (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    തിരികെ !dead_regions.in_error ();
#അല്ലെങ്കിൽ
    തിരികെ സത്യം;
#endif
  }

  തർക്കം subset (hb_subset_context_t *c, സ്ഥിരം hb_array_t<സ്ഥിരം hb_inc_bimap_t> &inner_maps) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
#ifdef HB_NO_VAR
    return_trace (അസത്യം);
#endif

    ItemVariationStore *varstore_അഭാജ്യം = c->serializer->start_embed<ItemVariationStore> ();
    എങ്കിൽ (unlikely (!varstore_അഭാജ്യം)) return_trace (അസത്യം);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    എങ്കിൽ (c->plan->has_avar2)
      എങ്കിൽ (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (അസത്യം);

    varstore_അഭാജ്യം->serialize (c->serializer, ഇത്, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_അഭാജ്യം->dataSets);
  }

  unsigned പൂർണ്ണസംഖ്യ get_region_index_എണ്ണം (unsigned പൂർണ്ണസംഖ്യ major) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ 0;
#endif
    തിരികെ (ഇത്+dataSets[major]).get_region_index_എണ്ണം ();
  }

  ശൂന്യം get_region_scalars (unsigned പൂർണ്ണസംഖ്യ major,
			   സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
			   ദശാംശം *scalars /*OUT*/,
			   unsigned പൂർണ്ണസംഖ്യ അക്കം_scalars) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    വേണ്ടി (unsigned i = 0; i < അക്കം_scalars; i++)
      scalars[i] = 0.f;
    തിരികെ;
#endif

    (ഇത്+dataSets[major]).get_region_scalars (coords, coord_എണ്ണം,
					       ഇത്+regions,
					       &scalars[0], അക്കം_scalars);
  }

  unsigned പൂർണ്ണസംഖ്യ get_sub_table_എണ്ണം () സ്ഥിരം
   {
#ifdef HB_NO_VAR
     തിരികെ 0;
#endif
     തിരികെ dataSets.len;
   }

  സ്ഥിരം VarData& get_sub_table (unsigned i) സ്ഥിരം
  {
#ifdef HB_NO_VAR
     തിരികെ Null (VarData);
#endif
     തിരികെ ഇത്+dataSets[i];
  }

  സ്ഥിരം VarRegionList& get_region_list () സ്ഥിരം
  {
#ifdef HB_NO_VAR
     തിരികെ Null (VarRegionList);
#endif
     തിരികെ ഇത്+regions;
  }

  സംരക്ഷിത:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  പൊതു:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

ഘടന MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ hb_scalar_cache_t::create (0);
#endif
    auto &r = ഇത്+regions;
    unsigned എണ്ണം = r.regions.len;

    തിരികെ hb_scalar_cache_t::create (എണ്ണം, static_cache);
  }

  സ്ഥിതം ശൂന്യം destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  സ്വകാര്യ:
  ശൂന്യം get_delta (unsigned പൂർണ്ണസംഖ്യ outer, unsigned പൂർണ്ണസംഖ്യ inner,
		  സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
		  hb_array_t<ഇരട്ട> out,
		  hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ;
#endif

    എങ്കിൽ (unlikely (outer >= dataSets.len))
      തിരികെ;

    തിരികെ (ഇത്+dataSets[outer]).get_delta (inner,
					     coords, coord_എണ്ണം,
					     ഇത്+regions,
					     out,
					     cache);
  }

  പൊതു:
  ശൂന്യം get_delta (unsigned പൂർണ്ണസംഖ്യ index,
		  സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
		  hb_array_t<ഇരട്ട> out,
		  hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ outer = index >> 16;
    unsigned പൂർണ്ണസംഖ്യ inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_എണ്ണം, out, cache);
  }
  ശൂന്യം get_delta (unsigned പൂർണ്ണസംഖ്യ index,
		  hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
		  hb_array_t<ഇരട്ട> out,
		  hb_scalar_cache_t *cache = nullptr) സ്ഥിരം
  {
    തിരികെ get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
#ifdef HB_NO_VAR
    തിരികെ സത്യം;
#endif

    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, ഇത്) &&
		  dataSets.sanitize (c, ഇത്));
  }

  തർക്കം create_subset_plan (സ്ഥിരം hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!inner_maps->resize (dataSets.len))) തിരികെ അസത്യം;
    വേണ്ടി (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      എങ്കിൽ (unlikely (outer >= dataSets.len ||
		    inner >= (ഇത്+dataSets[outer]).get_item_എണ്ണം ()))
	തിരികെ അസത്യം;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    വേണ്ടി (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      സ്ഥിരം hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      എങ്കിൽ (!inner_map.get_population ()) തുടരുക;
      എങ്കിൽ (unlikely (inner_map.in_error ())) തിരികെ അസത്യം;
      വേണ്ടി (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    തിരികെ !inner_maps->in_error () && !varidx_map->in_error ();
  }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം MultiItemVariationStore *src,
		  സ്ഥിരം hb_array_t<സ്ഥിരം hb_inc_bimap_t> &inner_maps,
		  സ്ഥിരം hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (അസത്യം);
    format = 1;

    സ്ഥിരം auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_എണ്ണം = 0;
    വേണ്ടി (unsigned i = 0; i < src->dataSets.len; i++)
    {
      എങ്കിൽ (!inner_maps[i].get_population ()) തുടരുക;
      set_എണ്ണം++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    എങ്കിൽ (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (അസത്യം);

    dataSets.len = set_എണ്ണം;
    എങ്കിൽ (unlikely (!c->extend (dataSets))) return_trace (അസത്യം);
    unsigned set_index = 0;
    വേണ്ടി (unsigned i = 0; i < src->dataSets.len; i++)
    {
      എങ്കിൽ (!inner_maps[i].get_population ()) തുടരുക;
      എങ്കിൽ (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (അസത്യം);
    }
    return_trace (സത്യം);
  }

  സംരക്ഷിത:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  പൊതു:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_എണ്ണം_T>
ഘടന DeltaSetIndexMapFormat01
{
  friend ഘടന DeltaSetIndexMap;

  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_mul_add_saturate (map_എണ്ണം_2, get_width (), min_size); }

  സ്വകാര്യ:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed (ഇത്));
  }

  template <typename T>
  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം T &plan)
  {
    unsigned പൂർണ്ണസംഖ്യ width = plan.get_width ();
    unsigned പൂർണ്ണസംഖ്യ inner_bit_എണ്ണം = plan.get_inner_bit_എണ്ണം ();
    സ്ഥിരം hb_array_t<സ്ഥിരം uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (output_map.length && ((((inner_bit_എണ്ണം-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);

    entryFormat = ((width-1)<<4)|(inner_bit_എണ്ണം-1);
    map_എണ്ണം_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    എങ്കിൽ (unlikely (!p)) return_trace (അസത്യം);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < output_map.length; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ v = output_map.arrayZ[i];
      എങ്കിൽ (v)
      {
	unsigned പൂർണ്ണസംഖ്യ outer = v >> 16;
	unsigned പൂർണ്ണസംഖ്യ inner = v & 0xFFFF;
	unsigned പൂർണ്ണസംഖ്യ u = (outer << inner_bit_എണ്ണം) | inner;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (സത്യം);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned പൂർണ്ണസംഖ്യ v) സ്ഥിരം /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    എങ്കിൽ (!map_എണ്ണം_2)
      തിരികെ v;
    തിരികെ _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned പൂർണ്ണസംഖ്യ v) സ്ഥിരം /* Returns 16.16 outer.inner. */
  {
    എങ്കിൽ (v >= map_എണ്ണം_2)
      v = map_എണ്ണം_2 - 1;

    unsigned പൂർണ്ണസംഖ്യ u = 0;
    { /* Fetch it. */
      unsigned പൂർണ്ണസംഖ്യ w = get_width ();
      സ്ഥിരം HBUINT8 *p = mapDataZ.arrayZ + w * v;
      വേണ്ടി (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned പൂർണ്ണസംഖ്യ n = get_inner_bit_എണ്ണം ();
      unsigned പൂർണ്ണസംഖ്യ outer = u >> n;
      unsigned പൂർണ്ണസംഖ്യ inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    തിരികെ u;
  }

  unsigned get_map_എണ്ണം () സ്ഥിരം       { തിരികെ map_എണ്ണം_2; }
  unsigned get_width () സ്ഥിരം           { തിരികെ ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_എണ്ണം () സ്ഥിരം { തിരികെ (entryFormat & 0xF) + 1; }


  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_എണ്ണം_2,
                                  get_width ()));
  }

  സംരക്ഷിത:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_എണ്ണം_T     map_എണ്ണം_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  പൊതു:
  DEFINE_SIZE_ARRAY (2+Map_എണ്ണം_T::static_size, mapDataZ);
};

ഘടന DeltaSetIndexMap
{
  template <typename T>
  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം T &plan)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  uint32_t map (unsigned v) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ (u.format0.map (v));
    അവസ്ഥ 1: hb_barrier (); തിരികെ (u.format1.map (v));
    സ്ഥിരസ്ഥിതി:തിരികെ v;
    }
  }

  unsigned get_map_എണ്ണം () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_map_എണ്ണം ();
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_map_എണ്ണം ();
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  unsigned get_width () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_width ();
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_width ();
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  unsigned get_inner_bit_എണ്ണം () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_inner_bit_എണ്ണം ();
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_inner_bit_എണ്ണം ();
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    അവസ്ഥ 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    സ്ഥിരസ്ഥിതി:return_trace (nullptr);
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (1, format.v);
};


ഘടന ItemVarStoreInstancer
{
  ItemVarStoreInstancer (സ്ഥിരം ItemVariationStore *varStore_,
			 സ്ഥിരം DeltaSetIndexMap *varIdxMap,
			 hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    എങ്കിൽ (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    എങ്കിൽ (!hb_any (coords))
      ഇത്->coords = {};
  }

  operator തർക്കം () സ്ഥിരം { തിരികെ varStore && തർക്കം (coords); }

  ദശാംശം operator[] (uint32_t varIdx) സ്ഥിരം
  { തിരികെ (*ഇത്) (varIdx); }

  ദശാംശം operator() (uint32_t varIdx, unsigned short offset = 0) സ്ഥിരം
  { തിരികെ get_delta (varIdx, offset); }

  ഇരട്ട get_condition_delta (uint32_t varIdx) സ്ഥിരം
  { തിരികെ get_delta<ഇരട്ട> (varIdx); }

  template <typename Float = ദശാംശം>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) സ്ഥിരം
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   എങ്കിൽ (!coords || varIdx == VarIdx::NO_VARIATION)
     തിരികെ Float (0);

    varIdx += offset;
    എങ്കിൽ (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    തിരികെ varStore->get_delta<Float> (varIdx, coords, cache);
  }

  സ്ഥിരം ItemVariationStore *varStore;
  സ്ഥിരം DeltaSetIndexMap *varIdxMap;
  hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords;
  hb_scalar_cache_t *cache;
};

ഘടന MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (സ്ഥിരം MultiItemVariationStore *varStore,
			      സ്ഥിരം DeltaSetIndexMap *varIdxMap,
			      hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    എങ്കിൽ (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator തർക്കം () സ്ഥിരം { തിരികെ varStore; }

  ഇരട്ട get_condition_delta (uint32_t varIdx) സ്ഥിരം
  { തിരികെ (ഇരട്ട) (*ഇത്)[varIdx]; }

  ഇരട്ട operator[] (uint32_t varIdx) സ്ഥിരം
  {
    ഇരട്ട v = 0;
    (*ഇത്) (hb_array (&v, 1), varIdx);
    തിരികെ v;
  }

  ശൂന്യം operator() (hb_array_t<ഇരട്ട> out, uint32_t varIdx, unsigned short offset = 0) സ്ഥിരം
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    എങ്കിൽ (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      എങ്കിൽ (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    അല്ലെങ്കിൽ
      വേണ്ടി (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  സ്ഥിരം MultiItemVariationStore *varStore;
  സ്ഥിരം DeltaSetIndexMap *varIdxMap;
  hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
എണ്ണൽ Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

എണ്ണൽ lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

ഘടന Condition;

template <typename Instancer>
സ്ഥിതം തർക്കം
_hb_recurse_condition_evaluate (സ്ഥിരം ഘടന Condition &condition,
				സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
				unsigned പൂർണ്ണസംഖ്യ coord_len,
				Instancer *instancer);

ഘടന ConditionAxisRange
{
  friend ഘടന Condition;

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    സ്ഥിരം hb_map_t *index_map = &c->plan->axes_index_map;
    എങ്കിൽ (index_map->is_empty ()) return_trace (സത്യം);

    സ്ഥിരം hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    എങ്കിൽ (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (അസത്യം);

    സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    എങ്കിൽ (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    സ്ഥിരം hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    എങ്കിൽ (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    ദശാംശം normalized_min = renormalize_മൂല്യം ((ഇരട്ട) filter_Range_Min_മൂല്യം.to_float (), axis_limit, axis_triple_distances, അസത്യം);
    ദശാംശം normalized_max = renormalize_മൂല്യം ((ഇരട്ട) filter_Range_Max_മൂല്യം.to_float (), axis_limit, axis_triple_distances, അസത്യം);
    out->filter_Range_Min_മൂല്യം.set_float (normalized_min);
    out->filter_Range_Max_മൂല്യം.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  സ്വകാര്യ:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) സ്ഥിരം
  {
    //invalid axis index, drop the entire record
    എങ്കിൽ (!c->axes_index_tag_map->has (axisIndex))
      തിരികെ DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    തർക്കം axis_set_by_user = അസത്യം;
    എങ്കിൽ (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = സത്യം;
    }

    ദശാംശം axis_min_val = axis_range.minimum;
    ദശാംശം axis_default_val = axis_range.നടുവ്;
    ദശാംശം axis_max_val = axis_range.maximum;

    ദശാംശം filter_min_val = filter_Range_Min_മൂല്യം.to_float ();
    ദശാംശം filter_max_val = filter_Range_Max_മൂല്യം.to_float ();

    എങ്കിൽ (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = അസത്യം;

    //condition not met, drop the entire record
    എങ്കിൽ (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      തിരികെ DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    എങ്കിൽ (axis_set_by_user && axis_range.is_point ())
      തിരികെ DROP_COND_WITH_VAR;

    എങ്കിൽ (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_മൂല്യം.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_മൂല്യം.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      തിരികെ KEEP_COND_WITH_VAR;
    }
    തിരികെ KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  തർക്കം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		 Instancer *instancer HB_UNUSED) സ്ഥിരം
  {
    പൂർണ്ണസംഖ്യ coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    തിരികെ filter_Range_Min_മൂല്യം.to_int () <= coord && coord <= filter_Range_Max_മൂല്യം.to_int ();
  }

  പൊതു:
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_മൂല്യം;
  F2DOT14	filter_Range_Max_മൂല്യം;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന Condition_മൂല്യം
{
  friend ഘടന Condition;

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    എങ്കിൽ (varIdx == VarIdx::NO_VARIATION)
      return_trace (സത്യം);

    hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *condition_മൂല്യം;
    എങ്കിൽ (c->plan->layout_condition_idx_മൂല്യം_map.has ((uintptr_t) ഇത്, &condition_മൂല്യം))
    {
      out->default_മൂല്യം_2 = condition_മൂല്യം->second;
      out->varIdx = condition_മൂല്യം->first;
      return_trace (സത്യം);
    }

    hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *new_varidx_delta;
    എങ്കിൽ (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!c->serializer->check_assign (
		out->default_മൂല്യം_2,
		default_മൂല്യം_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (അസത്യം);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (സത്യം);
  }

  സ്വകാര്യ:
  template <typename Instancer>
  തർക്കം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		 Instancer *instancer) സ്ഥിരം
  {
    ഇരട്ട value = default_മൂല്യം_2;
    value += instancer->get_condition_delta (varIdx);
    തിരികെ value > 0;
  }

  പൊതു:
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_മൂല്യം_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന ConditionAnd
{
  friend ഘടന Condition;

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (അസത്യം);
    out->format = format;

    വേണ്ടി (സ്ഥിരം auto &offset : conditions)
    {
      എങ്കിൽ (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	എങ്കിൽ (unlikely (!out_offset)) return_trace (അസത്യം);
	*out_offset = 0;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!subset_offset_array (c, out->conditions, ഇത്) (offset)))
	return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }

  സ്വകാര്യ:
  template <typename Instancer>
  തർക്കം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		 Instancer *instancer) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = conditions.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!_hb_recurse_condition_evaluate (ഇത്+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  പൊതു:
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (conditions.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<ഘടന Condition>	conditions;
  പൊതു:
  DEFINE_SIZE_ARRAY (3, conditions);
};

ഘടന ConditionOr
{
  friend ഘടന Condition;

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (അസത്യം);
    out->format = format;

    വേണ്ടി (സ്ഥിരം auto &offset : conditions)
    {
      എങ്കിൽ (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	എങ്കിൽ (unlikely (!out_offset)) return_trace (അസത്യം);
	*out_offset = 0;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!subset_offset_array (c, out->conditions, ഇത്) (offset)))
	return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }

  സ്വകാര്യ:
  template <typename Instancer>
  തർക്കം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		 Instancer *instancer) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = conditions.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (_hb_recurse_condition_evaluate (ഇത്+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }

  പൊതു:
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (conditions.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<ഘടന Condition>	conditions;
  പൊതു:
  DEFINE_SIZE_ARRAY (3, conditions);
};

ഘടന ConditionNegate
{
  friend ഘടന Condition;

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);
    എങ്കിൽ (condition.is_null ())
    {
      out->condition = 0;
      return_trace (സത്യം);
    }
    return_trace (out->condition.serialize_subset (c, condition, ഇത്));
  }

  സ്വകാര്യ:
  template <typename Instancer>
  തർക്കം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		 Instancer *instancer) സ്ഥിരം
  {
    തിരികെ !_hb_recurse_condition_evaluate (ഇത്+condition,
					    coords, coord_len,
					    instancer);
  }

  പൊതു:
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (condition.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<ഘടന Condition>	condition;
  പൊതു:
  DEFINE_SIZE_STATIC (5);
};

ഘടന Condition
{
  friend ഘടന ConditionSet;

  സ്ഥിതം തർക്കം serialize_constant (hb_serialize_context_t *c, തർക്കം value)
  {
    auto *out = c->start_embed<Condition_മൂല്യം> ();
    എങ്കിൽ (unlikely (!out || !c->extend_min (out))) തിരികെ അസത്യം;
    out->format = 2;
    out->default_മൂല്യം_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    തിരികെ സത്യം;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      തർക്കം materialize_true = അസത്യം) സ്ഥിരം;

  template <typename Instancer>
  തർക്കം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		 Instancer *instancer) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    അവസ്ഥ 0: തിരികെ സത്യം;
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.evaluate (coords, coord_len, instancer);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.evaluate (coords, coord_len, instancer);
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.evaluate (coords, coord_len, instancer);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.evaluate (coords, coord_len, instancer);
    അവസ്ഥ 5: hb_barrier (); തിരികെ u.format5.evaluate (coords, coord_len, instancer);
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!depth))
    {
      c->apply = അസത്യം;
      തിരികെ DROP_RECORD_WITH_VAR;
    }

    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: തിരികെ KEEP_RECORD_WITH_VAR;
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.keep_with_variations (c, condition_map);
    അവസ്ഥ 2: അവസ്ഥ 3: അവസ്ഥ 4: അവസ്ഥ 5: നിർത്തുക;
    സ്ഥിരസ്ഥിതി: c->apply = അസത്യം; തിരികെ DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    തർക്കം applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    എങ്കിൽ (c->all_axes_pinned)
      തിരികെ applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    എങ്കിൽ (u.format.v == 2)
      തിരികെ u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (സ്ഥിരം Condition &child)
    {
      auto child_context = *c;
      child_context.apply = സത്യം;
      തിരികെ child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    എങ്കിൽ (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      തിരികെ result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    തർക്കം is_and = u.format.v == 3;
    തർക്കം variable = അസത്യം;
    സ്ഥിരം auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    സ്ഥിരം ശൂന്യം *base = is_and ? (സ്ഥിരം ശൂന്യം *) &u.format3 : (സ്ഥിരം ശൂന്യം *) &u.format4;
    വേണ്ടി (സ്ഥിരം auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      എങ്കിൽ (is_and && result == DROP_RECORD_WITH_VAR)
        തിരികെ DROP_RECORD_WITH_VAR;
      എങ്കിൽ (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        തിരികെ KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    തിരികെ variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    അവസ്ഥ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    അവസ്ഥ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    എങ്കിൽ (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_മൂല്യം ());

    return_trace (c->end_recursion (ഇത്->dispatch (c)));
  }

  തർക്കം collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *condition_മൂല്യങ്ങൾ = nullptr) സ്ഥിരം;

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം Condition *src,
		  സ്ഥിരം hb_map_t &varidx_map,
		  സ്ഥിരം hb_map_t *axis_map = nullptr);

  സ്വകാര്യ:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) സ്ഥിരം;

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_മൂല്യം	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
തർക്കം
_hb_recurse_condition_evaluate (സ്ഥിരം ഘടന Condition &condition,
				സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
				unsigned പൂർണ്ണസംഖ്യ coord_len,
				Instancer *instancer)
{
  തിരികെ condition.evaluate (coords, coord_len, instancer);
}

ഘടന ConditionList
{
  സ്ഥിരം Condition& operator[] (unsigned i) സ്ഥിരം
  { തിരികെ ഇത്+conditions[i]; }

  unsigned get_എണ്ണം () സ്ഥിരം { തിരികെ conditions.len; }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം ConditionList *src,
		  സ്ഥിരം hb_inc_bimap_t &condition_map,
		  സ്ഥിരം hb_map_t &varidx_map,
		  സ്ഥിരം hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    conditions.len = condition_map.get_population ();
    എങ്കിൽ (unlikely (!c->extend (conditions))) return_trace (അസത്യം);
    വേണ്ടി (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      എങ്കിൽ (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (അസത്യം);
    }
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (conditions.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  Array32OfOffset32To<Condition> conditions;
  പൊതു:
  DEFINE_SIZE_ARRAY (4, conditions);
};

വരിയിൽ തർക്കം
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *condition_മൂല്യങ്ങൾ) സ്ഥിരം
{
  എങ്കിൽ (unlikely (!depth)) തിരികെ അസത്യം;
  തിരഞ്ഞെടുപ്പ് (u.format.v)
  {
    അവസ്ഥ 0:
    അവസ്ഥ 1:
      തിരികെ സത്യം;
    അവസ്ഥ 2:
      എങ്കിൽ (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        എങ്കിൽ (condition_മൂല്യങ്ങൾ &&
            !condition_മൂല്യങ്ങൾ->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), പൂർണ്ണസംഖ്യ (u.format2.default_മൂല്യം_2))))
          തിരികെ അസത്യം;
      }
      തിരികെ !var_indices->in_error ();
    അവസ്ഥ 3:
      വേണ്ടി (സ്ഥിരം auto &offset : u.format3.conditions)
	എങ്കിൽ (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_മൂല്യങ്ങൾ)))
	  തിരികെ അസത്യം;
      തിരികെ സത്യം;
    അവസ്ഥ 4:
      വേണ്ടി (സ്ഥിരം auto &offset : u.format4.conditions)
	എങ്കിൽ (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_മൂല്യങ്ങൾ)))
	  തിരികെ അസത്യം;
      തിരികെ സത്യം;
    അവസ്ഥ 5:
      തിരികെ (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_മൂല്യങ്ങൾ);
    സ്ഥിരസ്ഥിതി:
      തിരികെ അസത്യം;
  }
}

വരിയിൽ തർക്കം
Condition::serialize (hb_serialize_context_t *c,
		      സ്ഥിരം Condition *src,
		      സ്ഥിരം hb_map_t &varidx_map,
		      സ്ഥിരം hb_map_t *axis_map)
{
  TRACE_SERIALIZE (ഇത്);
  തിരഞ്ഞെടുപ്പ് (src->u.format.v)
  {
    അവസ്ഥ 0:
    {
      auto *out = c->start_embed<Condition_മൂല്യം> ();
      എങ്കിൽ (unlikely (!out || !c->extend_min (out))) return_trace (അസത്യം);
      out->format = 2;
      out->default_മൂല്യം_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (സത്യം);
    }
    അവസ്ഥ 1:
    {
      auto *out = c->embed (&src->u.format1);
      എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);
      എങ്കിൽ (axis_map)
      {
	എങ്കിൽ (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (അസത്യം);
	}
	എങ്കിൽ (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (അസത്യം);
      }
      return_trace (സത്യം);
    }
    അവസ്ഥ 2:
    {
      auto *out = c->embed (&src->u.format2);
      എങ്കിൽ (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (അസത്യം);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (സത്യം);
    }
    അവസ്ഥ 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      എങ്കിൽ (unlikely (!c->extend_min (out))) return_trace (അസത്യം);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      എങ്കിൽ (unlikely (!c->extend (out->conditions))) return_trace (അസത്യം);
      വേണ്ടി (unsigned i = 0; i < out->conditions.len; i++)
	എങ്കിൽ (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (അസത്യം);
      return_trace (സത്യം);
    }
    അവസ്ഥ 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      എങ്കിൽ (unlikely (!c->extend_min (out))) return_trace (അസത്യം);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      എങ്കിൽ (unlikely (!c->extend (out->conditions))) return_trace (അസത്യം);
      വേണ്ടി (unsigned i = 0; i < out->conditions.len; i++)
	എങ്കിൽ (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (അസത്യം);
      return_trace (സത്യം);
    }
    അവസ്ഥ 5:
    {
      auto *out = c->embed (&src->u.format5);
      എങ്കിൽ (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (അസത്യം);
      return_trace (സത്യം);
    }
    സ്ഥിരസ്ഥിതി:
      return_trace (അസത്യം);
  }
}

വരിയിൽ lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    തർക്കം materialize_true) സ്ഥിരം
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  എങ്കിൽ (result == LOOKUP_CONDITION_SUBSET_KEEP)
    തിരികെ result;

  c->serializer->revert (snap);
  എങ്കിൽ (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    തിരികെ result;

  തിരികെ serialize_constant (c->serializer, സത്യം) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

വരിയിൽ lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) സ്ഥിരം
{
  എങ്കിൽ (unlikely (!depth))
    തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   സ്ഥിരം Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    എങ്കിൽ (unlikely (!offset))
      തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    എങ്കിൽ (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->എടുക്കുക_pack ());
    അല്ലെങ്കിൽ
    {
      c->serializer->എടുക്കുക_discard ();
      out.എടുക്കുക ();
      c->serializer->revert (snap);
    }
    തിരികെ result;
  };

  തിരഞ്ഞെടുപ്പ് (u.format.v)
  {
    അവസ്ഥ 0:
      തിരികെ LOOKUP_CONDITION_SUBSET_TRUE;

    അവസ്ഥ 1:
    {
      സ്ഥിരം ConditionAxisRange &src = u.format1;
      എങ്കിൽ (c->plan->user_axes_location.is_empty ())
        തിരികെ c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      എങ്കിൽ (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      എങ്കിൽ (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      ദശാംശം filter_min = src.filter_Range_Min_മൂല്യം.to_float ();
      ദശാംശം filter_max = src.filter_Range_Max_മൂല്യം.to_float ();
      എങ്കിൽ (filter_min > filter_max)
        തിരികെ LOOKUP_CONDITION_SUBSET_FALSE;
      എങ്കിൽ (axis_limit.is_point ())
        തിരികെ filter_min <= axis_limit.നടുവ് &&
	       axis_limit.നടുവ് <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      എങ്കിൽ (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        തിരികെ LOOKUP_CONDITION_SUBSET_FALSE;
      എങ്കിൽ (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        തിരികെ LOOKUP_CONDITION_SUBSET_TRUE;
      എങ്കിൽ (!c->plan->axes_index_map.has (src.axisIndex))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      എങ്കിൽ (unlikely (!out))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      എങ്കിൽ (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_മൂല്യം.set_float (
	  renormalize_മൂല്യം (filter_min, axis_limit, axis_distances, അസത്യം));
      out->filter_Range_Max_മൂല്യം.set_float (
	  renormalize_മൂല്യം (filter_max, axis_limit, axis_distances, അസത്യം));
      എങ്കിൽ (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;
      തിരികെ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    അവസ്ഥ 2:
    {
      സ്ഥിരം Condition_മൂല്യം &src = u.format2;
      auto *out = c->serializer->embed (&src);
      എങ്കിൽ (unlikely (!out))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;

      എങ്കിൽ (src.varIdx == VarIdx::NO_VARIATION)
        തിരികെ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((പൂർണ്ണസംഖ്യ) src.default_മൂല്യം_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *condition_മൂല്യം;
      എങ്കിൽ (c->plan->layout_condition_idx_മൂല്യം_map.has ((uintptr_t) &src, &condition_മൂല്യം))
      {
        out->default_മൂല്യം_2 = condition_മൂല്യം->second;
        out->varIdx = condition_മൂല്യം->first;
        എങ്കിൽ (out->varIdx == VarIdx::NO_VARIATION)
          തിരികെ out->default_മൂല്യം_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        തിരികെ LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *new_varidx_delta;
      എങ്കിൽ (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        തിരികെ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((പൂർണ്ണസംഖ്യ) src.default_മൂല്യം_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      എങ്കിൽ (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        ഇരട്ട value = (പൂർണ്ണസംഖ്യ) src.default_മൂല്യം_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        തിരികെ value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      പൂർണ്ണസംഖ്യ default_മൂല്യം = (പൂർണ്ണസംഖ്യ) src.default_മൂല്യം_2 +
			  hb_second (*new_varidx_delta);
      എങ്കിൽ (unlikely (!c->serializer->check_assign (
		out->default_മൂല്യം_2,
		default_മൂല്യം,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      തിരികെ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    അവസ്ഥ 3:
    {
      സ്ഥിരം ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out)))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      വേണ്ടി (സ്ഥിരം auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        എങ്കിൽ (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          തിരികെ result;
      }
      തിരികെ out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    അവസ്ഥ 4:
    {
      സ്ഥിരം ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out)))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      വേണ്ടി (സ്ഥിരം auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        എങ്കിൽ (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          തിരികെ result;
      }
      തിരികെ out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    അവസ്ഥ 5:
    {
      സ്ഥിരം ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      എങ്കിൽ (unlikely (!out))
        തിരികെ LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      എങ്കിൽ (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->എടുക്കുക_pack ());
        തിരികെ result;
      }

      c->serializer->എടുക്കുക_discard ();
      എങ്കിൽ (result == LOOKUP_CONDITION_SUBSET_TRUE)
        തിരികെ LOOKUP_CONDITION_SUBSET_FALSE;
      എങ്കിൽ (result == LOOKUP_CONDITION_SUBSET_FALSE)
        തിരികെ LOOKUP_CONDITION_SUBSET_TRUE;
      തിരികെ result;
    }

    സ്ഥിരസ്ഥിതി:
      തിരികെ LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

ഘടന ConditionSet
{
  തർക്കം evaluate (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		 ItemVarStoreInstancer *instancer) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = conditions.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!(ഇത്+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  ശൂന്യം keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) സ്ഥിരം
  {
    hb_map_t *condition_map = hb_map_create ();
    എങ്കിൽ (unlikely (!condition_map)) തിരികെ;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    എങ്കിൽ (unlikely (!cond_set)) തിരികെ;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = സത്യം;
    തർക്കം should_keep = അസത്യം;
    /* The range map cannot identify value or boolean expressions uniquely. */
    തർക്കം can_deduplicate = സത്യം;
    unsigned അക്കം_kept_cond = 0, cond_idx = 0;
    വേണ്ടി (സ്ഥിരം auto& offset : conditions)
    {
      സ്ഥിരം Condition &condition = ഇത്+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      എങ്കിൽ (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = അസത്യം;
        തിരികെ;
      }

      എങ്കിൽ (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = സത്യം;
        cond_set->add (cond_idx);
        അക്കം_kept_cond++;
      }

      എങ്കിൽ (ret == KEEP_RECORD_WITH_VAR)
        should_keep = സത്യം;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = അക്കം_kept_cond == 0;
    എങ്കിൽ (!should_keep && c->all_axes_pinned) തിരികെ;

    //check if condition_set is unique with variations
    എങ്കിൽ (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      തിരികെ;

    എങ്കിൽ (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  തർക്കം subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               തർക്കം തിരുകുക_catch_all) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (അസത്യം);

    എങ്കിൽ (തിരുകുക_catch_all) return_trace (സത്യം);

    hb_set_t *retained_cond_set = nullptr;
    എങ്കിൽ (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = conditions.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (retained_cond_set != nullptr && !retained_cond_set->has (i))
        തുടരുക;
      സ്ഥിരം Condition &condition = ഇത്+conditions[i];
      എങ്കിൽ (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, ഇത്) (conditions[i]);
        തുടരുക;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!offset)) return_trace (അസത്യം);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, സത്യം);
      എങ്കിൽ (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, അസത്യം) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      എങ്കിൽ (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->എടുക്കുക_discard ();
        c->serializer->check_success (അസത്യം);
        return_trace (അസത്യം);
      }
      c->serializer->add_link (*offset, c->serializer->എടുക്കുക_pack ());
    }

    return_trace (തർക്കം (out->conditions));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (conditions.sanitize (c, ഇത്));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto &offset : conditions)
      എങ്കിൽ (unlikely (!(ഇത്+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_മൂല്യങ്ങൾ)))
	c->layout_variation_indices->err ();
  }

  സംരക്ഷിത:
  Array16OfOffset32To<Condition>	conditions;
  പൊതു:
  DEFINE_SIZE_ARRAY (2, conditions);
};

ഘടന FeatureTableSubstitutionRecord
{
  friend ഘടന FeatureTableSubstitution;

  ശൂന്യം collect_lookups (സ്ഥിരം ശൂന്യം *base, hb_set_t *lookup_indexes /* OUT */) സ്ഥിരം
  {
    തിരികെ (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  ശൂന്യം closure_features (സ്ഥിരം ശൂന്യം *base,
			 സ്ഥിരം hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  ശൂന്യം collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, സ്ഥിരം Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    സ്ഥിരം hb_set_t *feature_indices,
                                                    സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    എങ്കിൽ (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  തർക്കം serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  സ്ഥിരം Feature *f, സ്ഥിരം Tag *tag)
  {
    TRACE_SERIALIZE (ഇത്);
    hb_serialize_context_t *s = c->subset_context->serializer;
    എങ്കിൽ (unlikely (!s->extend_min (ഇത്))) return_trace (അസത്യം);

    uint32_t *new_feature_idx;
    എങ്കിൽ (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (അസത്യം);

    എങ്കിൽ (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);

    s->push ();
    തർക്കം ret = f->subset (c->subset_context, c, tag);
    എങ്കിൽ (ret) s->add_link (feature, s->എടുക്കുക_pack ());
    അല്ലെങ്കിൽ s->എടുക്കുക_discard ();

    return_trace (ret);
  }

  തർക്കം subset (hb_subset_layout_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    uint32_t *new_feature_index;
    എങ്കിൽ (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (അസത്യം);

    auto *out = c->subset_context->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->featureIndex = *new_feature_index;
    hb_pair_t<സ്ഥിരം ശൂന്യം*, സ്ഥിരം ശൂന്യം*> *original;
    എങ്കിൽ (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (അസത്യം);
    സ്ഥിരം Tag *tag = reinterpret_cast<സ്ഥിരം Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base,
		 സ്ഥിരം FeatureList *feature_list) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്))) return_trace (അസത്യം);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    സ്ഥിരം Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  സംരക്ഷിത:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന FeatureTableSubstitution
{
  ശൂന്യം collect_feature_indices (hb_set_t *feature_indices) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  സ്ഥിരം Feature *find_substitute (unsigned പൂർണ്ണസംഖ്യ feature_index) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = substitutions.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      സ്ഥിരം FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      എങ്കിൽ (record.featureIndex == feature_index)
	തിരികെ &(ഇത്+record.feature);
    }
    തിരികെ nullptr;
  }

  ശൂന്യം collect_lookups (സ്ഥിരം hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) സ്ഥിരം
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([ഇത്, lookup_indexes] (സ്ഥിരം FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (ഇത്, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  ശൂന്യം collect_name_ids (സ്ഥിരം hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം FeatureTableSubstitutionRecord &record : substitutions)
      എങ്കിൽ (feature_index_map->has (record.featureIndex))
        (ഇത്+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  ശൂന്യം closure_features (സ്ഥിരം hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (ഇത്, lookup_indexes, feature_indexes);
  }

  തർക്കം intersects_features (സ്ഥിരം hb_map_t *feature_index_map) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം FeatureTableSubstitutionRecord& record : substitutions)
    {
      എങ്കിൽ (feature_index_map->has (record.featureIndex)) തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
  }

  ശൂന്യം collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, ഇത്);
  }

  തർക്കം subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               തർക്കം തിരുകുക_catch_all) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (അസത്യം);

    out->version.major = version.major;
    out->version.minor = version.minor;

    എങ്കിൽ (തിരുകുക_catch_all)
    {
      വേണ്ടി (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<സ്ഥിരം ശൂന്യം*, സ്ഥിരം ശൂന്യം*> *p;
        എങ്കിൽ (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (അസത്യം);
        auto *o = out->substitutions.serialize_append (c->serializer);
        എങ്കിൽ (!o->serialize (l, feature_index,
                           reinterpret_cast<സ്ഥിരം Feature*> (p->first),
                           reinterpret_cast<സ്ഥിരം Tag*> (p->second)))
          return_trace (അസത്യം);
      }
      return_trace (സത്യം);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), ഇത്))
    ;

    return_trace (തർക്കം (out->substitutions));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം FeatureList *feature_list) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, ഇത്, feature_list));
  }

  സംരക്ഷിത:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  പൊതു:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

ഘടന FeatureVariationRecord
{
  friend ഘടന FeatureVariations;

  ശൂന്യം collect_lookups (സ്ഥിരം ശൂന്യം     *base,
			സ്ഥിരം hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) സ്ഥിരം
  {
    തിരികെ (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  ശൂന്യം collect_name_ids (സ്ഥിരം ശൂന്യം *base,
                         സ്ഥിരം hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) സ്ഥിരം
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  ശൂന്യം closure_features (സ്ഥിരം ശൂന്യം     *base,
			 സ്ഥിരം hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) സ്ഥിരം
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  തർക്കം intersects_features (സ്ഥിരം ശൂന്യം *base, സ്ഥിരം hb_map_t *feature_index_map) സ്ഥിരം
  {
    തിരികെ (base+substitutions).intersects_features (feature_index_map);
  }

  ശൂന്യം collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    (base+conditions).keep_with_variations (c);
    എങ്കിൽ (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = സത്യം; // set variations only once
    }
  }

  തർക്കം subset (hb_subset_layout_context_t *c, സ്ഥിരം ശൂന്യം *base,
               തർക്കം തിരുകുക_catch_all = അസത്യം) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->subset_context->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    എങ്കിൽ (!തിരുകുക_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    അല്ലെങ്കിൽ
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, തിരുകുക_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, തിരുകുക_catch_all);

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base,
		 സ്ഥിരം FeatureList *feature_list) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  { (base+conditions).collect_variation_indices (c); }

  സംരക്ഷിത:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന LookupIndexList
{
  ശൂന്യം add_lookup_indexes_to (hb_set_t *lookup_indexes) സ്ഥിരം
  { lookupIndices.add_indexes_to (lookup_indexes); }

  തർക്കം intersects_lookup_indexes (സ്ഥിരം hb_map_t *lookup_indexes) സ്ഥിരം
  { തിരികെ lookupIndices.intersects (lookup_indexes); }

  തർക്കം subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (അസത്യം);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (തർക്കം (out->lookupIndices));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (lookupIndices.sanitize (c));
  }

  സംരക്ഷിത:
  IndexArray	lookupIndices;
  പൊതു:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

ഘടന LookupConditionRecord
{
  unsigned get_condition_offset (സ്ഥിരം ശൂന്യം *base, സ്ഥിരം ശൂന്യം *variations) സ്ഥിരം
  { തിരികെ (സ്ഥിരം അക്ഷരം *) &(base+condition) - (സ്ഥിരം അക്ഷരം *) variations; }

  template <typename Instancer>
  തർക്കം evaluate (സ്ഥിരം ശൂന്യം *base,
		 സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
		 unsigned പൂർണ്ണസംഖ്യ coord_len,
		 Instancer *instancer) സ്ഥിരം
  { തിരികെ (base+condition).evaluate (coords, coord_len, instancer); }

  തർക്കം evaluate (സ്ഥിരം ശൂന്യം *base,
                 സ്ഥിരം ശൂന്യം *variations,
                 hb_sorted_array_t<സ്ഥിരം unsigned> conditions) സ്ഥിരം
  {
    തിരികെ !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     വലുപ്പം (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  തർക്കം collect_condition (സ്ഥിരം ശൂന്യം *base,
                          സ്ഥിരം ശൂന്യം *variations,
                          സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
                          unsigned പൂർണ്ണസംഖ്യ coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) സ്ഥിരം
  {
    /* Null conditions always apply and need no key state. */
    എങ്കിൽ (!condition) തിരികെ സത്യം;

    unsigned offset = get_condition_offset (base, variations);
    എങ്കിൽ (visited->has (offset)) തിരികെ സത്യം;
    visited->add (offset);
    എങ്കിൽ (unlikely (visited->in_error ())) തിരികെ അസത്യം;

    എങ്കിൽ (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    തിരികെ !conditions->in_error ();
  }

  ശൂന്യം add_lookup_indexes_to (സ്ഥിരം ശൂന്യം *base,
			      hb_set_t *lookup_indexes) സ്ഥിരം
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  തർക്കം intersects_lookup_indexes (സ്ഥിരം ശൂന്യം *base,
				   സ്ഥിരം hb_map_t *lookup_indexes) സ്ഥിരം
  { തിരികെ (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_മൂല്യങ്ങൾ)))
      c->layout_variation_indices->err ();
  }

  തർക്കം subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    സത്യം);
    എങ്കിൽ (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }
    c->serializer->add_link (out->condition, c->serializer->എടുക്കുക_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  സംരക്ഷിത:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന FeatureLookupsTable
{
  തർക്കം adds_default_lookups () സ്ഥിരം { തിരികെ flags & 0x0001u; }

  തർക്കം collect_conditions (സ്ഥിരം ശൂന്യം *variations,
                           സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
                           unsigned പൂർണ്ണസംഖ്യ coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupConditionRecord &record : records)
      എങ്കിൽ (unlikely (!record.collect_condition (ഇത്, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  ശൂന്യം collect_lookups (സ്ഥിരം ശൂന്യം *variations,
                       hb_sorted_array_t<സ്ഥിരം unsigned> conditions,
                       hb_set_t *lookup_indexes) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupConditionRecord &record : records)
      എങ്കിൽ (record.evaluate (ഇത്, variations, conditions))
	record.add_lookup_indexes_to (ഇത്, lookup_indexes);
  }

  template <typename Instancer>
  ശൂന്യം collect_lookups (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
			unsigned പൂർണ്ണസംഖ്യ coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupConditionRecord &record : records)
      എങ്കിൽ (record.evaluate (ഇത്, coords, coord_len, instancer))
	record.add_lookup_indexes_to (ഇത്, lookup_indexes);
  }

  ശൂന്യം collect_lookups (hb_set_t *lookup_indexes) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (ഇത്, lookup_indexes);
  }

  തർക്കം intersects_lookup_indexes (സ്ഥിരം hb_map_t *lookup_indexes) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupConditionRecord &record : records)
      എങ്കിൽ (record.intersects_lookup_indexes (ഇത്, lookup_indexes))
	തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupConditionRecord &record : records)
      record.collect_variation_indices (c, ഇത്);
  }

  തർക്കം subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (അസത്യം);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    വേണ്ടി (സ്ഥിരം LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      എങ്കിൽ (record.subset (c, l, ഇത്))
	out->records.len++;
      അല്ലെങ്കിൽ
	c->serializer->revert (snap);
    }

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  പൊതു:
  DEFINE_SIZE_ARRAY (10, records);
};

ഘടന LookupVariationRecord
{
  friend ഘടന FeatureVariations;

  unsigned get_feature_index () സ്ഥിരം { തിരികെ featureIndex; }

  ശൂന്യം collect_lookups (സ്ഥിരം ശൂന്യം *base,
                       സ്ഥിരം Feature &feature,
                       hb_sorted_array_t<സ്ഥിരം unsigned> conditions,
                       hb_set_t *lookup_indexes) സ്ഥിരം
  {
    സ്ഥിരം FeatureLookupsTable &feature_lookups = base+featureLookups;
    എങ്കിൽ (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  ശൂന്യം collect_lookups (സ്ഥിരം ശൂന്യം *base,
			സ്ഥിരം hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) സ്ഥിരം
  {
    എങ്കിൽ (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  ശൂന്യം closure_features (സ്ഥിരം ശൂന്യം *base,
			 സ്ഥിരം hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) സ്ഥിരം
  {
    എങ്കിൽ ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  { (base+featureLookups).collect_variation_indices (c); }

  തർക്കം subset (hb_subset_layout_context_t *l, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (ഇത്);
    uint32_t *new_feature_index;
    എങ്കിൽ (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (അസത്യം);

    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (അസത്യം);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  featureLookups.sanitize (c, base));
  }

  സംരക്ഷിത:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന FeatureVariations
{
  സ്ഥിതം constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  സ്ഥിരം Array32Of<LookupVariationRecord> &get_lookup_variation_records () സ്ഥിരം
  {
    എങ്കിൽ (version.to_int () < 0x00010001u)
      തിരികെ Null (Array32Of<LookupVariationRecord>);
    തിരികെ StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_എണ്ണം () സ്ഥിരം
  {
    തിരികെ varRecords.len;
  }

  തർക്കം find_index (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_len,
		   unsigned പൂർണ്ണസംഖ്യ *index,
		   ItemVarStoreInstancer *instancer) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = varRecords.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      സ്ഥിരം FeatureVariationRecord &record = varRecords.arrayZ[i];
      എങ്കിൽ ((ഇത്+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	തിരികെ സത്യം;
      }
    }
    *index = NOT_FOUND_INDEX;
    തിരികെ അസത്യം;
  }

  സ്ഥിരം Feature *find_substitute (unsigned പൂർണ്ണസംഖ്യ variations_index,
				  unsigned പൂർണ്ണസംഖ്യ feature_index) സ്ഥിരം
  {
    സ്ഥിരം FeatureVariationRecord &record = varRecords[variations_index];
    തിരികെ (ഇത്+record.substitutions).find_substitute (feature_index);
  }

  തർക്കം collect_lookup_conditions (സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
                                  unsigned പൂർണ്ണസംഖ്യ coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) സ്ഥിരം
  {
    hb_set_t visited_tables, visited_conditions;
    വേണ്ടി (സ്ഥിരം LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      എങ്കിൽ (visited_tables.has (offset)) തുടരുക;
      visited_tables.add (offset);
      എങ്കിൽ (unlikely (visited_tables.in_error ())) തിരികെ അസത്യം;

      സ്ഥിരം FeatureLookupsTable &feature_lookups = ഇത്+record.featureLookups;
      എങ്കിൽ (unlikely (!feature_lookups.collect_conditions (ഇത്, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	തിരികെ അസത്യം;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    തിരികെ സത്യം;
  }

  തർക്കം get_lookup_variations (unsigned feature_index,
                              സ്ഥിരം Feature &feature,
                              hb_sorted_array_t<സ്ഥിരം unsigned> conditions,
                              hb_set_t *lookup_indexes) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupVariationRecord &record : get_lookup_variation_records ())
    {
      എങ്കിൽ (record.featureIndex != feature_index) തുടരുക;

      record.collect_lookups (ഇത്, feature, conditions, lookup_indexes);
      തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
  }

  ശൂന്യം collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = varRecords.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, ഇത്);
      എങ്കിൽ (c->universal)
        നിർത്തുക;
    }
    എങ്കിൽ (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed (*ഇത്));
  }

  ശൂന്യം collect_lookups (സ്ഥിരം hb_set_t *feature_indexes,
			സ്ഥിരം hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) സ്ഥിരം
  {
    unsigned എണ്ണം = varRecords.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        തുടരുക;
      varRecords[i].collect_lookups (ഇത്, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  ശൂന്യം collect_name_ids (സ്ഥിരം hb_map_t *feature_index_map,
                         സ്ഥിരം hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < varRecords.len; i++)
    {
      എങ്കിൽ (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        തുടരുക;
      varRecords[i].collect_name_ids (ഇത്, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  ശൂന്യം collect_lookup_variation_lookups (സ്ഥിരം hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (ഇത്, feature_indexes, lookup_indexes);
  }

  ശൂന്യം closure_features (സ്ഥിരം hb_map_t *lookup_indexes,
			 സ്ഥിരം hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = varRecords.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        തുടരുക;
      varRecords[i].closure_features (ഇത്, lookup_indexes, feature_indexes);
    }

    വേണ്ടി (സ്ഥിരം LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (ഇത്, lookup_indexes, feature_indexes);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, ഇത്);
    വേണ്ടി (സ്ഥിരം LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, ഇത്);
  }

  ശൂന്യം collect_feature_indices (hb_set_t *feature_indices) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം FeatureVariationRecord &record : varRecords)
      (ഇത്+record.substitutions).collect_feature_indices (feature_indices);
  }

  തർക്കം has_lookup_variations (unsigned feature_index) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം LookupVariationRecord &record : get_lookup_variation_records ())
    {
      എങ്കിൽ (record.featureIndex == feature_index)
	തിരികെ സത്യം;
      എങ്കിൽ (record.featureIndex > feature_index)
	നിർത്തുക;
    }
    തിരികെ അസത്യം;
  }

  തർക്കം subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (അസത്യം);

    out->version.major = version.major;
    out->version.minor = version.minor;

    പൂർണ്ണസംഖ്യ keep_up_to = -1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = varRecords.len - 1; i >= 0; i--) {
      എങ്കിൽ (varRecords[i].intersects_features (ഇത്, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        നിർത്തുക;
      }
    }

    unsigned എണ്ണം = (unsigned) (keep_up_to + 1);
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        തുടരുക;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), ഇത്) (varRecords[i]);
    }

    എങ്കിൽ (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      തർക്കം തിരുകുക_catch_all_record = സത്യം;
      subset_record_array (l, &(out->varRecords), ഇത്, തിരുകുക_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    എങ്കിൽ (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      എങ്കിൽ (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (അസത്യം);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, ഇത്))
      ;
    }

    return_trace (തർക്കം (out->varRecords) ||
		  (out_lookup_records && തർക്കം (*out_lookup_records)));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം FeatureList *feature_list) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, ഇത്, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, ഇത്)));
  }

  സംരക്ഷിത:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  പൊതു:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

ഘടന HintingDevice
{
  friend ഘടന Device;

  സ്വകാര്യ:

  hb_position_t get_x_delta (hb_font_t *font) സ്ഥിരം
  { തിരികെ get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) സ്ഥിരം
  { തിരികെ get_delta (font->y_ppem, font->y_scale); }

  പൊതു:

  size_t get_size () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ f = deltaFormat;
    എങ്കിൽ (unlikely (f < 1 || f > 3 || startSize > endSize)) തിരികെ 3 * HBUINT16::static_size;
    തിരികെ HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && c->check_range (ഇത്, ഇത്->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed<HintingDevice> (ഇത്));
  }

  സ്വകാര്യ:

  പൂർണ്ണസംഖ്യ get_delta (unsigned പൂർണ്ണസംഖ്യ ppem, പൂർണ്ണസംഖ്യ scale) സ്ഥിരം
  {
    എങ്കിൽ (!ppem) തിരികെ 0;

    പൂർണ്ണസംഖ്യ pixels = get_delta_pixels (ppem);

    എങ്കിൽ (!pixels) തിരികെ 0;

    തിരികെ (പൂർണ്ണസംഖ്യ) (pixels * (int64_t) scale / ppem);
  }
  പൂർണ്ണസംഖ്യ get_delta_pixels (unsigned പൂർണ്ണസംഖ്യ ppem_size) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ f = deltaFormat;
    എങ്കിൽ (unlikely (f < 1 || f > 3))
      തിരികെ 0;

    എങ്കിൽ (ppem_size < startSize || ppem_size > endSize)
      തിരികെ 0;

    unsigned പൂർണ്ണസംഖ്യ s = ppem_size - startSize;

    unsigned പൂർണ്ണസംഖ്യ byte = delta_മൂല്യം_Z[s >> (4 - f)];
    unsigned പൂർണ്ണസംഖ്യ bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned പൂർണ്ണസംഖ്യ mask = (0xFFFFu >> (16 - (1 << f)));

    പൂർണ്ണസംഖ്യ delta = bits & mask;

    എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    തിരികെ delta;
  }

  സംരക്ഷിത:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_മൂല്യം_Z;		/* Array of compressed data */
  പൊതു:
  DEFINE_SIZE_ARRAY (6, delta_മൂല്യം_Z);
};

ഘടന VariationDevice
{
  friend ഘടന Device;

  സ്വകാര്യ:

  hb_position_t get_x_delta (hb_font_t *font,
			     സ്ഥിരം ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  { തിരികെ !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     സ്ഥിരം ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  { തിരികെ !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  ദശാംശം get_x_delta_f (hb_font_t *font,
		       സ്ഥിരം ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) സ്ഥിരം
  { തിരികെ !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  ദശാംശം get_y_delta_f (hb_font_t *font,
		       സ്ഥിരം ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) സ്ഥിരം
  { തിരികെ !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *v;
    എങ്കിൽ (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    എങ്കിൽ (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (ഇത്->static_size);
    auto *out = c->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (nullptr);

    എങ്കിൽ (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  ശൂന്യം collect_variation_index (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  { c->layout_variation_indices->add (varIdx); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സ്വകാര്യ:

  ദശാംശം get_delta (hb_font_t *font,
		   സ്ഥിരം ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
    തിരികെ store.get_delta (varIdx, font->coords, font->അക്കം_coords, store_cache);
  }

  സംരക്ഷിത:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന DeviceHeader
{
  സംരക്ഷിത:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  പൊതു:
  HBUINT16		format;		/* Format identifier */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  ദശാംശം get_x_delta_f (hb_font_t *font,
		       സ്ഥിരം ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
#ifndef HB_NO_VAR
    എങ്കിൽ (u.b.format == 0x8000)
    {
      hb_barrier ();
      തിരികെ u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    തിരികെ get_x_delta (font, store, store_cache);
  }

  ദശാംശം get_y_delta_f (hb_font_t *font,
		       സ്ഥിരം ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
#ifndef HB_NO_VAR
    എങ്കിൽ (u.b.format == 0x8000)
    {
      hb_barrier ();
      തിരികെ u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    തിരികെ get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     സ്ഥിരം ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.b.format)
    {
#ifndef HB_NO_HINTING
    അവസ്ഥ 1: അവസ്ഥ 2: അവസ്ഥ 3:
      hb_barrier ();
      തിരികെ u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    അവസ്ഥ 0x8000:
      hb_barrier ();
      തിരികെ u.variation.get_x_delta (font, store, store_cache);
#endif
    സ്ഥിരസ്ഥിതി:
      തിരികെ 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     സ്ഥിരം ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.b.format)
    {
    അവസ്ഥ 1: അവസ്ഥ 2: അവസ്ഥ 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      തിരികെ u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    അവസ്ഥ 0x8000:
      hb_barrier ();
      തിരികെ u.variation.get_y_delta (font, store, store_cache);
#endif
    സ്ഥിരസ്ഥിതി:
      തിരികെ 0;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.b.format.sanitize (c)) return_trace (അസത്യം);
    തിരഞ്ഞെടുപ്പ് (u.b.format) {
#ifndef HB_NO_HINTING
    അവസ്ഥ 1: അവസ്ഥ 2: അവസ്ഥ 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    അവസ്ഥ 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    സ്ഥിരസ്ഥിതി:
      return_trace (സത്യം);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map=nullptr) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    തിരഞ്ഞെടുപ്പ് (u.b.format) {
#ifndef HB_NO_HINTING
    അവസ്ഥ 1:
    അവസ്ഥ 2:
    അവസ്ഥ 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    അവസ്ഥ 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    സ്ഥിരസ്ഥിതി:
      return_trace (nullptr);
    }
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.b.format) {
#ifndef HB_NO_HINTING
    അവസ്ഥ 1:
    അവസ്ഥ 2:
    അവസ്ഥ 3:
      തിരികെ;
#endif
#ifndef HB_NO_VAR
    അവസ്ഥ 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      തിരികെ;
#endif
    സ്ഥിരസ്ഥിതി:
      തിരികെ;
    }
  }

  unsigned get_variation_index () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.b.format) {
#ifndef HB_NO_VAR
    അവസ്ഥ 0x8000:
      hb_barrier ();
      തിരികെ u.variation.varIdx;
#endif
    സ്ഥിരസ്ഥിതി:
      തിരികെ HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  തർക്കം is_variation_device () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.b.format) {
#ifndef HB_NO_VAR
    അവസ്ഥ 0x8000:
      തിരികെ സത്യം;
#endif
    സ്ഥിരസ്ഥിതി:
      തിരികെ അസത്യം;
    }
  }

  സംരക്ഷിത:
  സംഘം {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

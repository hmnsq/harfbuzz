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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-ot-layout.hh"
#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-set.hh"
#அந்தர்பாவ "hb-bimap.hh"
#அந்தர்பாவ "hb-cache.hh"
#அந்தர்பாவ "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (அளவு (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_இருமம்_cache_t = hb_cache_t<14, 1, 8>;
static_assert (அளவு (hb_ot_layout_இருமம்_cache_t) == 256, "");

#அந்தர்பாவ "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


நாமாகாஶ OT {

template<typename Iterator>
நிலையான உள்வரி தருக்க ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

நிலையான தருக்க ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    மாறிலி hb_set_t &klasses,
    தருக்க use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

கட்டமைப்பு ItemVarStoreInstancer;

கட்டமைப்பு hb_collect_feature_substitutes_with_var_context_t
{
  மாறிலி hb_map_t *axes_index_tag_map;
  மாறிலி hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, மாறிலி Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  தருக்க apply;
  தருக்க variation_applied;
  தருக்க universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  தருக்க all_axes_pinned;
};

கட்டமைப்பு hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (மாறிலி வெறுமை         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              மாறிலி hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_மொத்தம் (0),langsys_feature_மொத்தம் (0) {}

  தருக்க visitScript ()
  { திரும்பு script_மொத்தம்++ < HB_MAX_SCRIPTS; }

  தருக்க visitLangsys (unsigned feature_மொத்தம்_2)
  {
    langsys_feature_மொத்தம் += feature_மொத்தம்_2;
    திரும்பு langsys_feature_மொத்தம் < HB_MAX_LANGSYS_FEATURE_மொத்தம்;
  }

  பொது:
  மாறிலி வெறுமை *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  மாறிலி hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  தனிப்பட்ட:
  unsigned script_மொத்தம்;
  unsigned langsys_feature_மொத்தம்;
};

கட்டமைப்பு hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  மாறிலி எழுத்து *get_name () { திரும்பு "SUBSET_LAYOUT"; }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }

  தருக்க visitScript ()
  {
    திரும்பு script_மொத்தம்++ < HB_MAX_SCRIPTS;
  }

  தருக்க visitLangSys ()
  {
    திரும்பு langsys_மொத்தம்++ < HB_MAX_LANGSYS;
  }

  தருக்க visitFeatureIndex (முழு மொத்தம்)
  {
    feature_index_மொத்தம் += மொத்தம்;
    திரும்பு feature_index_மொத்தம் < HB_MAX_FEATURE_INDICES;
  }

  தருக்க visitLookupIndex()
  {
    lookup_index_மொத்தம்++;
    திரும்பு lookup_index_மொத்தம் < HB_MAX_LOOKUP_VISIT_மொத்தம்;
  }

  hb_subset_context_t *subset_context;
  மாறிலி hb_tag_t table_tag;
  மாறிலி hb_map_t *lookup_index_map;
  மாறிலி hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  மாறிலி hb_map_t *feature_index_map;
  மாறிலி hb_map_t *feature_map_w_duplicates;
  மாறிலி hb_hashmap_t<unsigned, மாறிலி Feature*> *feature_substitutes_map;
  மாறிலி hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  மாறிலி hb_set_t *catch_all_record_feature_idxes;
  மாறிலி hb_hashmap_t<unsigned, hb_pair_t<மாறிலி வெறுமை*, மாறிலி வெறுமை*>> *feature_idx_tag_map;

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
				script_மொத்தம் (0),
				langsys_மொத்தம் (0),
				feature_index_மொத்தம் (0),
				lookup_index_மொத்தம் (0)
  {
    எனில் (tag_ == HB_OT_TAG_GSUB)
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
    இல்லையெனில்
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

  தனிப்பட்ட:
  unsigned script_மொத்தம்;
  unsigned langsys_மொத்தம்;
  unsigned feature_index_மொத்தம்;
  unsigned lookup_index_மொத்தம்;
};

கட்டமைப்பு ItemVariationStore;
கட்டமைப்பு hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (மாறிலி T &obj) { obj.collect_variation_indices (இது); திரும்பு hb_empty_t (); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  மாறிலி hb_set_t *glyph_set;
  மாறிலி hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, முழு>> *condition_மதிப்புகள் = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  மாறிலி hb_set_t *glyph_set_,
					  மாறிலி hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
கட்டமைப்பு subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 மாறிலி வெறுமை *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  தருக்க operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    எனில் (unlikely (!o)) திரும்பு பொய்;
    தருக்க ret = o->serialize_subset (subset_context, offset, base);
    எனில் (!ret)
    {
      out.எடு ();
      subset_context->serializer->revert (snap);
    }
    திரும்பு ret;
  }

  தனிப்பட்ட:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  மாறிலி வெறுமை *base;
};


template<typename OutputArray, typename Arg>
கட்டமைப்பு subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     மாறிலி வெறுமை *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  தருக்க operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    எனில் (unlikely (!o)) திரும்பு பொய்;
    தருக்க ret = o->serialize_subset (subset_context, offset, base, arg);
    எனில் (!ret)
    {
      out.எடு ();
      subset_context->serializer->revert (snap);
    }
    திரும்பு ret;
  }

  தனிப்பட்ட:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  மாறிலி வெறுமை *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
கட்டமைப்பு
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       மாறிலி வெறுமை *base) மாறிலி
  { திரும்பு subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       மாறிலி வெறுமை *base, Arg &&arg) மாறிலி
  { திரும்பு subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
கட்டமைப்பு subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 மாறிலி வெறுமை *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  வெறுமை
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    தருக்க ret = record.subset (subset_layout_context, base);
    எனில் (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    இல்லையெனில் out->len++;
  }

  தனிப்பட்ட:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  மாறிலி வெறுமை *base;
};

template<typename OutputArray, typename Arg>
கட்டமைப்பு subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     மாறிலி வெறுமை *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  வெறுமை
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    தருக்க ret = record.subset (subset_layout_context, base, arg);
    எனில் (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    இல்லையெனில் out->len++;
  }

  தனிப்பட்ட:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  மாறிலி வெறுமை *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
கட்டமைப்பு
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       மாறிலி வெறுமை *base) மாறிலி
  { திரும்பு subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               மாறிலி வெறுமை *base, Arg &&arg) மாறிலி
  { திரும்பு subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
கட்டமைப்பு serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         மாறிலி வெறுமை *base_,
                         மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  தருக்க operator () (T&& record)
  {
    எனில் (!serialize_context->copy (record, base, variation_map)) திரும்பு பொய்;
    out.len++;
    திரும்பு உண்மை;
  }

  தனிப்பட்ட:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  மாறிலி வெறுமை *base;
  மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
கட்டமைப்பு
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               மாறிலி வெறுமை *base,
               மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *variation_map) மாறிலி
  { திரும்பு serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

கட்டமைப்பு IndexArray : Array16Of<Index>
{
  தருக்க intersects (மாறிலி hb_map_t *indexes) மாறிலி
  { திரும்பு hb_any (*இது, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    எனில் (!it) திரும்பு;
    எனில் (unlikely (!c->extend_min ((*இது)))) திரும்பு;

    க்கு (மாறிலி auto _ : it)
    {
      எனில் (!l->visitLookupIndex()) நிறுத்து;

      Index i;
      i = _;
      c->copy (i);
      இது->len++;
    }
  }

  unsigned முழு get_indexes (unsigned முழு start_offset,
			    unsigned முழு *மொத்தம்_2 /* IN/OUT */,
			    unsigned முழு *_indexes /* OUT */) மாறிலி
  {
    எனில் (மொத்தம்_2 && _indexes)
    {
      + இது->as_array ().sub_array (start_offset, மொத்தம்_2)
      | hb_sink (hb_array (_indexes, *மொத்தம்_2))
      ;
    }
    திரும்பு இது->len;
  }

  வெறுமை add_indexes_to (hb_set_t* output /* OUT */) மாறிலி
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
கட்டமைப்பு FeatureParamsSize
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது))) return_trace (பொய்);
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

    எனில் (!designSize)
      return_trace (பொய்);
    இல்லையெனில் எனில் (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (உண்மை);
    இல்லையெனில் எனில் (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (பொய்);
    இல்லையெனில்
      return_trace (உண்மை);
  }

  வெறுமை collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) மாறிலி
  { nameids_to_retain->add (subfamilyNameID); }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    return_trace ((தருக்க) c->serializer->embed (*இது));
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
  பொது:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
கட்டமைப்பு FeatureParamsStylisticSet
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (இது));
  }

  வெறுமை collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) மாறிலி
  { nameids_to_retain->add (uiNameID); }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    return_trace ((தருக்க) c->serializer->embed (*இது));
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
  பொது:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
கட்டமைப்பு FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_மொத்தம், hb_codepoint_t *chars) மாறிலி
  {
    எனில் (char_மொத்தம் && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_மொத்தம்)
      | hb_sink (hb_array (chars, *char_மொத்தம்))
      ;
    }
    திரும்பு characters.len;
  }

  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  வெறுமை collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) மாறிலி
  {
    எனில் (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    எனில் (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    எனில் (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    எனில் (!firstParamUILabelNameID || !இலக்கம்_Named_Parameters || இலக்கம்_Named_Parameters >= 0x7FFF)
      திரும்பு;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) இலக்கம்_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    return_trace ((தருக்க) c->serializer->embed (*இது));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
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
  HBUINT16	இலக்கம்_Named_Parameters;	/* Number of named parameters. (May
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
  பொது:
  DEFINE_SIZE_ARRAY (14, characters);
};

கட்டமைப்பு FeatureParams
{
  தருக்க sanitize (hb_sanitize_context_t *c, hb_tag_t tag) மாறிலி
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    திரும்பு உண்மை;
#endif
    TRACE_SANITIZE (இது);
    எனில் (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    எனில் ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    எனில் ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (உண்மை);
  }

  வெறுமை collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) மாறிலி
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    திரும்பு;
#endif
    எனில் (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      திரும்பு (u.size.collect_name_ids (nameids_to_retain));
    எனில் ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      திரும்பு (u.stylisticSet.collect_name_ids (nameids_to_retain));
    எனில் ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      திரும்பு (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  தருக்க subset (hb_subset_context_t *c, மாறிலி Tag* tag) மாறிலி
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    திரும்பு பொய்;
#endif
    TRACE_SUBSET (இது);
    எனில் (!tag) return_trace (பொய்);
    எனில் (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    எனில் ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    எனில் ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (பொய்);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  மாறிலி FeatureParamsSize& get_size_params (hb_tag_t tag) மாறிலி
  {
    எனில் (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      திரும்பு u.size;
    திரும்பு Null (FeatureParamsSize);
  }
  மாறிலி FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) மாறிலி
  {
    எனில் ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      திரும்பு u.stylisticSet;
    திரும்பு Null (FeatureParamsStylisticSet);
  }
  மாறிலி FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) மாறிலி
  {
    எனில் ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      திரும்பு u.characterVariants;
    திரும்பு Null (FeatureParamsCharacterVariants);
  }
#endif

  தனிப்பட்ட:
  ஒன்றிப்பு {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  பொது:
  DEFINE_SIZE_MIN (0);
};

கட்டமைப்பு Record_sanitize_closure_t {
  hb_tag_t tag;
  மாறிலி வெறுமை *list_base;
};

கட்டமைப்பு Feature
{
  unsigned முழு get_lookup_மொத்தம் () மாறிலி
  { திரும்பு lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned முழு i) மாறிலி
  { திரும்பு lookupIndex[i]; }
  unsigned முழு get_lookup_indexes (unsigned முழு start_index,
				   unsigned முழு *lookup_மொத்தம் /* IN/OUT */,
				   unsigned முழு *lookup_tags /* OUT */) மாறிலி
  { திரும்பு lookupIndex.get_indexes (start_index, lookup_மொத்தம், lookup_tags); }
  வெறுமை add_lookup_indexes_to (hb_set_t *lookup_indexes) மாறிலி
  { lookupIndex.add_indexes_to (lookup_indexes); }

  மாறிலி FeatureParams &get_feature_params () மாறிலி
  { திரும்பு இது+featureParams; }

  தருக்க intersects_lookup_indexes (மாறிலி hb_map_t *lookup_indexes) மாறிலி
  { திரும்பு lookupIndex.intersects (lookup_indexes); }

  வெறுமை collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) மாறிலி
  {
    எனில் (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  தருக்க subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       மாறிலி Tag                   *tag = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    out->featureParams.serialize_subset (c, featureParams, இது, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    எனில் (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    இல்லையெனில்
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
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி Record_sanitize_closure_t *closure = nullptr) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  featureParams.sanitize (c, இது, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
கட்டமைப்பு Record
{
  முழு cmp (hb_tag_t a) மாறிலி { திரும்பு tag.cmp (a); }

  தருக்க subset (hb_subset_layout_context_t *c, மாறிலி வெறுமை *base, மாறிலி வெறுமை *f_sub = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->subset_context->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    மாறிலி Feature& f = *reinterpret_cast<மாறிலி Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    தருக்க ret = f.subset (c->subset_context, c, &tag);
    எனில் (ret)
      s->add_link (out->offset, s->எடு_pack ());
    இல்லையெனில்
      s->எடு_discard ();

    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    மாறிலி Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (இது) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
கட்டமைப்பு RecordArrayOf : SortedArray16Of<Record<Type>>
{
  மாறிலி Offset16To<Type>& get_offset (unsigned முழு i) மாறிலி
  { திரும்பு (*இது)[i].offset; }
  Offset16To<Type>& get_offset (unsigned முழு i)
  { திரும்பு (*இது)[i].offset; }
  மாறிலி Tag& get_tag (unsigned முழு i) மாறிலி
  { திரும்பு (*இது)[i].tag; }
  unsigned முழு get_tags (unsigned முழு start_offset,
                         unsigned முழு *record_மொத்தம் /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) மாறிலி
  {
    எனில் (record_மொத்தம் && record_tags)
    {
      + இது->as_array ().sub_array (start_offset, record_மொத்தம்)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_மொத்தம்))
      ;
    }
    திரும்பு இது->len;
  }
  தருக்க find_index (hb_tag_t tag, unsigned முழு *index) மாறிலி
  {
    திரும்பு இது->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
கட்டமைப்பு RecordListOf : RecordArrayOf<Type>
{
  மாறிலி Type& operator [] (unsigned முழு i) மாறிலி
  { திரும்பு இது+இது->get_offset (i); }

  தருக்க subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    + இது->iter ()
    | hb_apply (subset_record_array (l, out, இது))
    ;
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (RecordArrayOf<Type>::sanitize (c, இது));
  }
};

கட்டமைப்பு RecordListOfFeature : RecordListOf<Feature>
{
  தருக்க subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    + hb_enumerate (*இது)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, இது] (மாறிலி hb_pair_t<unsigned, மாறிலி Record<Feature>&>& _)
                {
                  மாறிலி Feature *f_sub = nullptr;
                  மாறிலி Feature **f = nullptr;
                  எனில் (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, இது, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (உண்மை);
  }
};

வகைவரை RecordListOf<Feature> FeatureList;


கட்டமைப்பு LangSys
{
  unsigned முழு get_feature_மொத்தம் () மாறிலி
  { திரும்பு featureIndex.len; }
  hb_tag_t get_feature_index (unsigned முழு i) மாறிலி
  { திரும்பு featureIndex[i]; }
  unsigned முழு get_feature_indexes (unsigned முழு start_offset,
				    unsigned முழு *feature_மொத்தம்_2 /* IN/OUT */,
				    unsigned முழு *feature_indexes /* OUT */) மாறிலி
  { திரும்பு featureIndex.get_indexes (start_offset, feature_மொத்தம்_2, feature_indexes); }
  வெறுமை add_feature_indexes_to (hb_set_t *feature_indexes) மாறிலி
  { featureIndex.add_indexes_to (feature_indexes); }

  தருக்க has_required_feature () மாறிலி { திரும்பு reqFeatureIndex != 0xFFFFu; }
  unsigned முழு get_required_feature_index () மாறிலி
  {
    எனில் (reqFeatureIndex == 0xFFFFu)
      திரும்பு Index::NOT_FOUND_INDEX;
   திரும்பு reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed (*இது));
  }

  தருக்க compare (மாறிலி LangSys& o, மாறிலி hb_map_t *feature_index_map) மாறிலி
  {
    எனில் (reqFeatureIndex != o.reqFeatureIndex)
      திரும்பு பொய்;

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

    க்கு (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      எனில் (a != b) திரும்பு பொய்;
    }

    எனில் (iter || o_iter) திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  வெறுமை collect_features (hb_prune_langsys_context_t *c) மாறிலி
  {
    எனில் (!has_required_feature () && !get_feature_மொத்தம் ()) திரும்பு;
    எனில் (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  தருக்க subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       மாறிலி Tag                  *tag = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    மாறிலி uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    எனில் (!l->visitFeatureIndex (featureIndex.len))
      return_trace (பொய்);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    தருக்க ret = தருக்க (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி Record_sanitize_closure_t * = nullptr) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

கட்டமைப்பு Script
{
  unsigned முழு get_lang_sys_மொத்தம் () மாறிலி
  { திரும்பு langSys.len; }
  மாறிலி Tag& get_lang_sys_tag (unsigned முழு i) மாறிலி
  { திரும்பு langSys.get_tag (i); }
  unsigned முழு get_lang_sys_tags (unsigned முழு start_offset,
				  unsigned முழு *lang_sys_மொத்தம் /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) மாறிலி
  { திரும்பு langSys.get_tags (start_offset, lang_sys_மொத்தம், lang_sys_tags); }
  மாறிலி LangSys& get_lang_sys (unsigned முழு i) மாறிலி
  {
    எனில் (i == Index::NOT_FOUND_INDEX) திரும்பு get_default_lang_sys ();
    திரும்பு இது+langSys[i].offset;
  }
  தருக்க find_lang_sys_index (hb_tag_t tag, unsigned முழு *index) மாறிலி
  { திரும்பு langSys.find_index (tag, index); }

  தருக்க has_default_lang_sys () மாறிலி           { திரும்பு defaultLangSys != 0; }
  மாறிலி LangSys& get_default_lang_sys () மாறிலி { திரும்பு இது+defaultLangSys; }

  வெறுமை prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) மாறிலி
  {
    எனில் (!has_default_lang_sys () && !get_lang_sys_மொத்தம் ()) திரும்பு;
    எனில் (!c->visitScript ()) திரும்பு;

    எனில் (!c->script_langsys_map->has (script_index))
    {
      எனில் (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	திரும்பு;
    }

    எனில் (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      மாறிலி LangSys& d = get_default_lang_sys ();
      எனில் (c->visitLangsys (d.get_feature_மொத்தம் ())) {
        d.collect_features (c);
      }

      க்கு (auto _ : + hb_enumerate (langSys))
      {
        மாறிலி LangSys& l = இது+_.second.offset;
        எனில் (!c->visitLangsys (l.get_feature_மொத்தம் ())) தொடர்;
        எனில் (l.compare (d, c->duplicate_feature_map)) தொடர்;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    இல்லையெனில்
    {
      க்கு (auto _ : + hb_enumerate (langSys))
      {
        மாறிலி LangSys& l = இது+_.second.offset;
        எனில் (!c->visitLangsys (l.get_feature_மொத்தம் ())) தொடர்;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  தருக்க subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       மாறிலி Tag                   *tag) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (!l->visitScript ()) return_trace (பொய்);
    எனில் (tag && !c->plan->layout_scripts.has (*tag))
      திரும்பு பொய்;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    தருக்க defaultLang = பொய்;
    எனில் (has_default_lang_sys ())
    {
      c->serializer->push ();
      மாறிலி LangSys& ls = இது+defaultLangSys;
      தருக்க ret = ls.subset (c, l);
      எனில் (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->எடு_discard ();
	out->defaultLangSys = 0;
      }
      இல்லையெனில்
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->எடு_pack ());
	defaultLang = உண்மை;
      }
    }

    மாறிலி hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    எனில் (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (மாறிலி Record<LangSys>& record) {திரும்பு l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), இது))
      ;
    }

    return_trace (தருக்க (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி Record_sanitize_closure_t * = nullptr) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (defaultLangSys.sanitize (c, இது) && langSys.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

கட்டமைப்பு RecordListOfScript : RecordListOf<Script>
{
  தருக்க subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    க்கு (auto _ : + hb_enumerate (*இது))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      தருக்க ret = _.second.subset (l, இது);
      எனில் (!ret) c->serializer->revert (snap);
      இல்லையெனில் out->len++;
    }

    return_trace (உண்மை);
  }
};

வகைவரை RecordListOfScript ScriptList;



கட்டமைப்பு LookupFlag : HBUINT16
{
  எண்ணிக்கை Flags {
    வலது_To_இடது		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  பொது:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
நாமாகாஶ OT {

கட்டமைப்பு Lookup
{
  unsigned முழு get_subtable_மொத்தம் () மாறிலி { திரும்பு subTable.len; }

  template <typename TSubTable>
  மாறிலி Array16OfOffset16To<TSubTable>& get_subtables () மாறிலி
  { திரும்பு reinterpret_cast<மாறிலி Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { திரும்பு reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  மாறிலி TSubTable& get_subtable (unsigned முழு i) மாறிலி
  { திரும்பு இது+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned முழு i)
  { திரும்பு இது+get_subtables<TSubTable> ()[i]; }

  size_t get_size () மாறிலி
  {
    மாறிலி HBUINT16 &markFilteringSet = StructAfter<மாறிலி HBUINT16> (subTable);
    எனில் (lookupFlag & LookupFlag::UseMarkFilteringSet)
      திரும்பு (மாறிலி எழுத்து *) &StructAfter<மாறிலி எழுத்து> (markFilteringSet) - (மாறிலி எழுத்து *) இது;
    திரும்பு (மாறிலி எழுத்து *) &markFilteringSet - (மாறிலி எழுத்து *) இது;
  }

  unsigned முழு get_type () மாறிலி { திரும்பு lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () மாறிலி
  {
    unsigned முழு flag = lookupFlag;
    எனில் (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      மாறிலி HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    திரும்பு flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    unsigned முழு lookup_type = get_type ();
    TRACE_DISPATCH (இது, lookup_type);
    unsigned முழு மொத்தம் = get_subtable_மொத்தம் ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      எனில் (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_மதிப்பு ());
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  unsigned முழு lookup_type,
		  uint32_t lookup_props,
		  unsigned முழு இலக்கம்_subtables)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    எனில் (unlikely (!subTable.serialize (c, இலக்கம்_subtables))) return_trace (பொய்);
    எனில் (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      எனில் (unlikely (!c->extend (இது))) return_trace (பொய்);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (உண்மை);
  }

  template <typename TSubTable>
  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    மாறிலி hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned முழு lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([இது, glyphset, lookup_type] (மாறிலி Offset16To<TSubTable> &_) { திரும்பு (இது+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), இது, lookup_type))
    ;

    எனில் (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      மாறிலி HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      எனில் (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      இல்லையெனில்
      {
        எனில் (unlikely (!c->serializer->extend (out))) return_trace (பொய்);
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
    return_trace (உண்மை);
  }

  template <typename TSubTable>
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(c->check_struct (இது) && subTable.sanitize (c))) return_trace (பொய்);
    hb_barrier ();

    unsigned subtables = get_subtable_மொத்தம் ();
    எனில் (unlikely (!c->visit_subtables (subtables))) return_trace (பொய்);

    எனில் (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      மாறிலி HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      எனில் (!markFilteringSet.sanitize (c)) return_trace (பொய்);
    }

    எனில் (unlikely (!get_subtables<TSubTable> ().sanitize (c, இது, get_type ())))
      return_trace (பொய்);

    எனில் (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned முழு type = get_subtable<TSubTable> (0).u.extension.get_type ();
      க்கு (unsigned முழு i = 1; i < subtables; i++)
	எனில் (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (பொய்);
    }
    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  பொது:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
கட்டமைப்பு LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  தருக்க subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    + hb_enumerate (*இது)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, இது))
    ;
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, இது)));
  }
};


/*
 * Coverage Table
 */


நிலையான தருக்க ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  மாறிலி hb_set_t &klasses,
                                          தருக்க use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  எனில் (!klass_map)
    திரும்பு ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  எனில் (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  க்கு (மாறிலி unsigned k: klasses)
  {
    எனில் (klass_map->has (k)) தொடர்;
    klass_map->set (k, idx);
    idx++;
  }


  எனில் (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    திரும்பு பொய்;

  க்கு (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  திரும்பு ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
கட்டமைப்பு ClassDefFormat1_3
{
  friend கட்டமைப்பு ClassDef;

  தனிப்பட்ட:
  unsigned முழு get_class (hb_codepoint_t glyph_id) மாறிலி
  {
    திரும்பு class_மதிப்பு[(unsigned முழு) (glyph_id - startGlyph)];
  }

  unsigned get_population () மாறிலி
  {
    திரும்பு class_மதிப்பு.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    எனில் (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_மதிப்பு.len = 0;
      return_trace (உண்மை);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_மொத்தம் = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    எனில் (unlikely (!class_மதிப்பு.serialize (c, glyph_மொத்தம்))) return_trace (பொய்);
    க்கு (மாறிலி hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_மதிப்பு[idx] = gid_klass_pair.second;
    }
    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               தருக்க keep_empty_table = உண்மை,
               தருக்க use_class_zero = உண்மை,
               மாறிலி Coverage* glyph_filter = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_மதிப்பு.len;

    க்கு (மாறிலி hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      எனில் (new_gid == HB_MAP_மதிப்பு_INVALID) தொடர்;
      எனில் (glyph_filter && !glyph_filter->has(gid)) தொடர்;

      unsigned klass = class_மதிப்பு[gid - start];
      எனில் (!klass) தொடர்;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    எனில் (use_class_zero)
    {
      மாறிலி hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_மொத்தம் = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_மொத்தம் <= glyph_and_klass.length;
    }
    எனில் (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (பொய்);
    return_trace (keep_empty_table || (தருக்க) glyph_and_klass);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && class_மதிப்பு.sanitize (c));
  }

  unsigned cost () மாறிலி { திரும்பு 1; }

  template <typename set_t>
  தருக்க collect_coverage (set_t *glyphs) மாறிலி
  {
    unsigned முழு start = 0;
    unsigned முழு மொத்தம் = class_மதிப்பு.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (class_மதிப்பு[i])
	தொடர்;

      எனில் (start != i)
	எனில் (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  திரும்பு பொய்;

      start = i + 1;
    }
    எனில் (start != மொத்தம்)
      எனில் (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + மொத்தம்)))
	திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  template <typename set_t>
  தருக்க collect_class (set_t *glyphs, unsigned klass) மாறிலி
  {
    unsigned முழு மொத்தம் = class_மதிப்பு.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (class_மதிப்பு[i] == klass) glyphs->add (startGlyph + i);
    திரும்பு உண்மை;
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_மதிப்பு.len;
    க்கு (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      எனில் (class_மதிப்பு[iter - start]) திரும்பு உண்மை;
    திரும்பு பொய்;
  }
  தருக்க intersects_class (மாறிலி hb_set_t *glyphs, unsigned klass) மாறிலி
  {
    unsigned முழு மொத்தம் = class_மதிப்பு.len;
    எனில் (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_மதிப்பு_INVALID;
      எனில் (!glyphs->next (&g)) திரும்பு பொய்;
      எனில் (g < startGlyph) திரும்பு உண்மை;
      g = startGlyph + மொத்தம் - 1;
      எனில் (glyphs->next (&g)) திரும்பு உண்மை;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    மாறிலி typename Types::HBUINT *arr = class_மதிப்பு.arrayZ;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (arr[i] == klass && glyphs->has (startGlyph + i))
	திரும்பு உண்மை;
    திரும்பு பொய்;
  }

  வெறுமை intersected_class_glyphs (மாறிலி hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) மாறிலி
  {
    unsigned மொத்தம் = class_மதிப்பு.len;
    எனில் (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      க்கு (uint32_t g = HB_SET_மதிப்பு_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      க்கு (uint32_t g = startGlyph + மொத்தம் - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      திரும்பு;
    }

    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      எனில் (class_மதிப்பு[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#எனில் 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + மொத்தம்;
    க்கு (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      எனில் (class_மதிப்பு.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  வெறுமை intersected_classes (மாறிலி hb_set_t *glyphs, hb_set_t *intersect_classes) மாறிலி
  {
    எனில் (glyphs->is_empty ()) திரும்பு;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    எனில் (!class_மதிப்பு.len)
    {
      intersect_classes->add (0);
      திரும்பு;
    }

    hb_codepoint_t end_glyph = startGlyph + class_மதிப்பு.len - 1;
    எனில் (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    க்கு (மாறிலி auto& _ : + hb_enumerate (class_மதிப்பு))
    {
      hb_codepoint_t g = startGlyph + _.first;
      எனில் (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_மதிப்பு;	/* Array of Class Values--one per GlyphID */
  பொது:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_மதிப்பு);
};

template <typename Types>
கட்டமைப்பு ClassDefFormat2_4
{
  friend கட்டமைப்பு ClassDef;

  தனிப்பட்ட:
  unsigned முழு get_class (hb_codepoint_t glyph_id) மாறிலி
  {
    திரும்பு rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () மாறிலி
  {
    typename Types::large_int ret = 0;
    க்கு (மாறிலி auto &r : rangeRecord)
      ret += r.get_population ();
    திரும்பு ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    எனில் (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (உண்மை);
    }

    unsigned unsorted = பொய்;
    unsigned இலக்கம்_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    எனில் (unlikely (!record)) return_trace (பொய்);

    க்கு (மாறிலி auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      எனில் (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	எனில் (unlikely (cur_gid < prev_gid))
	  unsorted = உண்மை;

	எனில் (unlikely (!record)) நிறுத்து;
	record->last = prev_gid;
	இலக்கம்_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    எனில் (unlikely (c->in_error ())) return_trace (பொய்);

    எனில் (likely (record)) record->last = prev_gid;
    rangeRecord.len = இலக்கம்_ranges;

    எனில் (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               தருக்க keep_empty_table = உண்மை,
               தருக்க use_class_zero = உண்மை,
               மாறிலி Coverage* glyph_filter = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    எனில் (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      க்கு (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	எனில் (!klass) தொடர்;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	எனில் (new_gid == HB_MAP_மதிப்பு_INVALID) தொடர்;
	எனில் (glyph_filter && !glyph_filter->has (g)) தொடர்;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    இல்லையெனில்
    {
      unsigned இலக்கம்_source_glyphs = c->plan->source->get_இலக்கம்_glyphs ();
      க்கு (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	எனில் (!klass) தொடர்;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, இலக்கம்_source_glyphs);
	க்கு (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  எனில் (new_gid == HB_MAP_மதிப்பு_INVALID) தொடர்;
	  எனில் (glyph_filter && !glyph_filter->has (g)) தொடர்;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    மாறிலி hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_மொத்தம் = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_மொத்தம் <= glyph_and_klass.length;
    எனில் (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (பொய்);
    return_trace (keep_empty_table || (தருக்க) glyph_and_klass);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () மாறிலி { திரும்பு hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  தருக்க collect_coverage (set_t *glyphs) மாறிலி
  {
    க்கு (auto &range : rangeRecord)
      எனில் (range.value)
	எனில் (unlikely (!range.collect_coverage (glyphs)))
	  திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  template <typename set_t>
  தருக்க collect_class (set_t *glyphs, unsigned முழு klass) மாறிலி
  {
    க்கு (auto &range : rangeRecord)
    {
      எனில் (range.value == klass)
	எனில் (unlikely (!range.collect_coverage (glyphs)))
	  திரும்பு பொய்;
    }
    திரும்பு உண்மை;
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    எனில் (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      க்கு (auto g : *glyphs)
        எனில் (get_class (g))
	  திரும்பு உண்மை;
      திரும்பு பொய்;
    }

    திரும்பு hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (மாறிலி RangeRecord<Types> &range) { திரும்பு range.intersects (*glyphs) && range.value; }));
  }
  தருக்க intersects_class (மாறிலி hb_set_t *glyphs, unsigned klass) மாறிலி
  {
    எனில் (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      எனில் (!rangeRecord.len)
        திரும்பு !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_மதிப்பு_INVALID;
      hb_codepoint_t last = HB_SET_மதிப்பு_INVALID;
      auto it = hb_iter (rangeRecord);
      க்கு (auto &range : it)
      {
        எனில் (it->first == last + 1)
	{
	  it++;
	  தொடர்;
	}

	எனில் (!glyphs->next (&g))
	  நிறுத்து;
	எனில் (g < range.first)
	  திரும்பு உண்மை;
	g = range.last;
	last = g;
      }
      எனில் (g != HB_SET_மதிப்பு_INVALID && glyphs->next (&g))
	திரும்பு உண்மை;
      /* Fall through. */
    }
    க்கு (மாறிலி auto &range : rangeRecord)
      எனில் (range.value == klass && range.intersects (*glyphs))
	திரும்பு உண்மை;
    திரும்பு பொய்;
  }

  வெறுமை intersected_class_glyphs (மாறிலி hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) மாறிலி
  {
    எனில் (klass == 0)
    {
      hb_codepoint_t g = HB_SET_மதிப்பு_INVALID;
      க்கு (auto &range : rangeRecord)
      {
	எனில் (!glyphs->next (&g))
	  செல் done;
	போது (g < range.first)
	{
	  intersect_glyphs->add (g);
	  எனில் (!glyphs->next (&g))
	    செல் done;
        }
        g = range.last;
      }
      போது (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      திரும்பு;
    }

    unsigned மொத்தம் = rangeRecord.len;
    எனில் (மொத்தம் > glyphs->get_population () * hb_bit_storage (மொத்தம்))
    {
      க்கு (auto g : *glyphs)
      {
        unsigned i;
        எனில் (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      திரும்பு;
    }

    க்கு (auto &range : rangeRecord)
    {
      எனில் (range.value != klass) தொடர்;

      unsigned end = range.last + 1;
      க்கு (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  வெறுமை intersected_classes (மாறிலி hb_set_t *glyphs, hb_set_t *intersect_classes) மாறிலி
  {
    எனில் (glyphs->is_empty ()) திரும்பு;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    எனில் (!rangeRecord.len)
    {
      intersect_classes->add (0);
      திரும்பு;
    }

    hb_codepoint_t g = HB_SET_மதிப்பு_INVALID;
    க்கு (auto &range : rangeRecord)
    {
      எனில் (!glyphs->next (&g))
        நிறுத்து;
      எனில் (g < range.first)
      {
        intersect_classes->add (0);
        நிறுத்து;
      }
      g = range.last;
    }
    எனில் (g != HB_SET_மதிப்பு_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    க்கு (மாறிலி auto& range : rangeRecord)
      எனில் (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

கட்டமைப்பு ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) மாறிலி { திரும்பு get (k); }
  தருக்க has (hb_codepoint_t k) மாறிலி { திரும்பு (*இது)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) மாறிலி { திரும்பு get (k); }

  unsigned முழு get (hb_codepoint_t k) மாறிலி { திரும்பு get_class (k); }
  unsigned முழு get_class (hb_codepoint_t glyph_id) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_class (glyph_id);
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_class (glyph_id);
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_class (glyph_id);
#endif
    இயல்பு:திரும்பு 0;
    }
  }
  unsigned முழு get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) மாறிலி
  {
    unsigned klass;
    எனில் (cache && cache->get (glyph_id, &klass)) திரும்பு klass;
    klass = get_class (glyph_id);
    எனில் (cache) cache->set (glyph_id, klass);
    திரும்பு klass;
  }

  unsigned get_population () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_population ();
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_population ();
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_population ();
#endif
    இயல்பு:திரும்பு NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    எனில் (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned இலக்கம்_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      க்கு (மாறிலி auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	எனில் (cur_klass > class_max) class_max = cur_klass;
	எனில் (cur_gid == glyph_min) தொடர்;
        எனில் (cur_gid > glyph_max) glyph_max = cur_gid;
	எனில் (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  இலக்கம்_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	எனில் (size >= best_size) திரும்பு;
	best_size = size;
	format = candidate;
      };

      எனில் (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      எனில் (glyph_max <= 0xFFFFu && இலக்கம்_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) இலக்கம்_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      எனில் (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      எனில் (glyph_max <= 0xFFFFFFu && இலக்கம்_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) இலக்கம்_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      எனில் (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (பொய், HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (பொய்);
      }
    }

    u.format.v = format;

    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    நிலை 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    நிலை 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    இயல்பு:return_trace (பொய்);
    }
  }

  தருக்க subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               தருக்க keep_empty_table = உண்மை,
               தருக்க use_class_zero = உண்மை,
               மாறிலி Coverage* glyph_filter = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    நிலை 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    நிலை 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    இயல்பு:return_trace (பொய்);
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    நிலை 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    இயல்பு:return_trace (உண்மை);
    }
  }

  unsigned cost () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.cost ();
    நிலை 2: hb_barrier (); திரும்பு u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.cost ();
    நிலை 4: hb_barrier (); திரும்பு u.format4.cost ();
#endif
    இயல்பு:திரும்பு 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  தருக்க collect_coverage (set_t *glyphs) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.collect_coverage (glyphs);
    நிலை 2: hb_barrier (); திரும்பு u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.collect_coverage (glyphs);
    நிலை 4: hb_barrier (); திரும்பு u.format4.collect_coverage (glyphs);
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  தருக்க collect_class (set_t *glyphs, unsigned முழு klass) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.collect_class (glyphs, klass);
    நிலை 2: hb_barrier (); திரும்பு u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.collect_class (glyphs, klass);
    நிலை 4: hb_barrier (); திரும்பு u.format4.collect_class (glyphs, klass);
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.intersects (glyphs);
    நிலை 2: hb_barrier (); திரும்பு u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.intersects (glyphs);
    நிலை 4: hb_barrier (); திரும்பு u.format4.intersects (glyphs);
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }
  தருக்க intersects_class (மாறிலி hb_set_t *glyphs, unsigned முழு klass) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.intersects_class (glyphs, klass);
    நிலை 2: hb_barrier (); திரும்பு u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.intersects_class (glyphs, klass);
    நிலை 4: hb_barrier (); திரும்பு u.format4.intersects_class (glyphs, klass);
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }

  வெறுமை intersected_class_glyphs (மாறிலி hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    நிலை 2: hb_barrier (); திரும்பு u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    நிலை 4: hb_barrier (); திரும்பு u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    இயல்பு:திரும்பு;
    }
  }

  வெறுமை intersected_classes (மாறிலி hb_set_t *glyphs, hb_set_t *intersect_classes) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.intersected_classes (glyphs, intersect_classes);
    நிலை 2: hb_barrier (); திரும்பு u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); திரும்பு u.format3.intersected_classes (glyphs, intersect_classes);
    நிலை 4: hb_barrier (); திரும்பு u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    இயல்பு:திரும்பு;
    }
  }


  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
நிலையான உள்வரி தருக்க ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ திரும்பு (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
கட்டமைப்பு delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  கட்டமைப்பு chars_t : hb_vector_t<uint8_t>
  {
    முழு cmp (மாறிலி chars_t& other) மாறிலி
    {
      திரும்பு as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      க்கு (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      திரும்பு hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (மாறிலி chars_t& other) மாறிலி
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      க்கு (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      திரும்பு hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (மாறிலி delta_row_encoding_t& other_encoding) மாறிலி { திரும்பு chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<மாறிலி hb_vector_t<முழு>*> பொருட்கள்;

  delta_row_encoding_t () = இயல்பு;
  delta_row_encoding_t (hb_vector_t<மாறிலி hb_vector_t<முழு>*> &&rows, unsigned இலக்கம்_cols)
  {
    ப்ரதிஜ்ஞா (rows);

    பொருட்கள் = std::move (rows);

    எனில் (unlikely (!chars.resize (இலக்கம்_cols)))
      திரும்பு;

    calculate_chars ();
  }

  வெறுமை merge (மாறிலி delta_row_encoding_t& other)
  {
    பொருட்கள்.alloc (பொருட்கள்.length + other.பொருட்கள்.length);
    க்கு (auto &row : other.பொருட்கள்)
      add_row (row);

    // Merge chars
    ப்ரதிஜ்ஞா (chars.length == other.chars.length);
    க்கு (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  வெறுமை chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  வெறுமை calculate_chars ()
  {
    ப்ரதிஜ்ஞா (பொருட்கள்);

    தருக்க long_சொற்கள் = பொய்;

    க்கு (auto &row : பொருட்கள்)
    {
      ப்ரதிஜ்ஞா (row->length == chars.length);

      /* 0/1/2 byte encoding */
      க்கு (unsigned i = 0; i < row->length; i++)
      {
	முழு v =  row->arrayZ[i];
	எனில் (v == 0)
	  தொடர்;
	இல்லையெனில் எனில் (v > 32767 || v < -32768)
	{
	  long_சொற்கள் = உண்மை;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	இல்லையெனில் எனில் (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	இல்லையெனில்
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    எனில் (long_சொற்கள்)
    {
      // Convert 1s to 2s
      க்கு (auto &v : chars)
	எனில் (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  தருக்க is_empty () மாறிலி
  { திரும்பு !பொருட்கள்; }

  தருக்க in_error () மாறிலி
  { திரும்பு chars.in_error () || பொருட்கள்.in_error (); }

  நிலையான உள்வரி unsigned get_chars_overhead (unsigned இலக்கம்_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    திரும்பு c + இலக்கம்_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) மாறிலி
  {
    முழு மொத்தம் = பொருட்கள்.length;
    திரும்பு hb_max (0, (முழு) overhead - மொத்தம் * (முழு) additional_bytes_per_rows);
  }

  முழு gain_from_merging (மாறிலி delta_row_encoding_t& other_encoding) மாறிலி
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    எனில் (additional_bytes_per_rows > 0)
    {
      எனில் (get_gain (additional_bytes_per_rows) == 0)
        திரும்பு 0;
    }
    இல்லையெனில்
    {
      எனில் (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	திரும்பு 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    முழு combined_gain = (முழு) overhead + (முழு) other_encoding.overhead;
    combined_gain -= (combined_width - (முழு) width) * பொருட்கள்.length;
    combined_gain -= (combined_width - (முழு) other_encoding.width) * other_encoding.பொருட்கள்.length;
    combined_gain -= get_chars_overhead (combined_columns);

    திரும்பு combined_gain;
  }

  தருக்க add_row (மாறிலி hb_vector_t<முழு>* row)
  { திரும்பு பொருட்கள்.push (row); }

  நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    மாறிலி delta_row_encoding_t *a = (மாறிலி delta_row_encoding_t *)pa;
    மாறிலி delta_row_encoding_t *b = (மாறிலி delta_row_encoding_t *)pb;

    எனில் (a->width != b->width)
      திரும்பு (முழு) a->width - (முழு) b->width;

    திரும்பு b->chars.cmp (a->chars);
  }
};

கட்டமைப்பு VarRegionAxis
{
  தசம evaluate (முழு coord) மாறிலி
  {
    முழு peak = peakCoord.to_int ();
    எனில் (peak == 0 || coord == peak)
      திரும்பு 1.f;
    இல்லையெனில் எனில் (coord == 0) // Faster
      திரும்பு 0.f;

    முழு start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    எனில் (unlikely (start > peak || peak > end))
      திரும்பு 1.f;
    எனில் (unlikely (start < 0 && end > 0))
      திரும்பு 1.f;

    எனில் (coord <= start || end <= coord)
      திரும்பு 0.f;

    /* Interpolate */
    எனில் (coord < peak)
      திரும்பு தசம (coord - start) / (peak - start);
    இல்லையெனில்
      திரும்பு தசம (end - coord) / (end - peak);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  தருக்க serialize (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed (இது));
  }

  பொது:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  பொது:
  DEFINE_SIZE_STATIC (6);
};
கட்டமைப்பு SparseVarRegionAxis
{
  தசம evaluate (மாறிலி முழு *coords, unsigned முழு coord_len) மாறிலி
  {
    unsigned i = axisIndex;
    முழு coord = i < coord_len ? coords[i] : 0;
    திரும்பு axis.evaluate (coord);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி SparseVarRegionAxis *src,
		  மாறிலி hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->embed (src);
    எனில் (unlikely (!out)) return_trace (பொய்);
    எனில் (axis_map)
    {
      எனில் (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (பொய்);
      }
      எனில் (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (பொய்);
    }
    return_trace (உண்மை);
  }

  பொது:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு hb_scalar_cache_t
{
  தனிப்பட்ட:
  நிலையான constexpr unsigned STATIC_LENGTH = 128;
  நிலையான constexpr தசம INVALID = NAN;

  பொது:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (மாறிலி hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (மாறிலி hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  நிலையான hb_scalar_cache_t *create (unsigned முழு மொத்தம்,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    எனில் (!மொத்தம்) திரும்பு (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    எனில் (scratch_cache && மொத்தம் <= STATIC_LENGTH)
    {
      scratch_cache->length = மொத்தம்;
      scratch_cache->clear ();
      திரும்பு scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (அளவு (hb_scalar_cache_t) - அளவு (static_மதிப்புகள்) + அளவு (static_மதிப்புகள்[0]) * மொத்தம்);
    எனில் (unlikely (!cache)) திரும்பு (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = மொத்தம்;
    cache->clear ();

    திரும்பு cache;
  }

  நிலையான வெறுமை destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    எனில் (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  வெறுமை clear ()
  {
    auto *values = &static_மதிப்புகள்[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    க்கு (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    க்கு (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  தருக்க get (unsigned i, தசம *value) மாறிலி
  {
    எனில் (unlikely (i >= length))
    {
      *value = 0.f;
      திரும்பு உண்மை;
    }
    auto *values = &static_மதிப்புகள்[0];
    auto *cached_மதிப்பு = &values[i];
    தசம v = *cached_மதிப்பு;
    எனில் (std::isnan (v))
      திரும்பு பொய்;
    *value = v;
    திரும்பு உண்மை;
  }

  HB_ALWAYS_INLINE
  வெறுமை set (unsigned i, தசம value)
  {
    எனில் (unlikely (i >= length)) திரும்பு;
    auto *values = &static_மதிப்புகள்[0];
    auto *cached_மதிப்பு = &values[i];
    *cached_மதிப்பு = value;
  }

  பொது:
  unsigned length;
  தனிப்பட்ட:
  mutable hb_atomic_t<தசம> static_மதிப்புகள்[STATIC_LENGTH];
};

கட்டமைப்பு VarRegionList
{
  தனிப்பட்ட:
  தசம evaluate_impl (unsigned முழு region_index,
		       மாறிலி முழு *coords, unsigned முழு coord_len) மாறிலி
  {
    மாறிலி VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_மொத்தம்_2);
    தசம v = 1.f;

    unsigned முழு மொத்தம் = axis_மொத்தம்_2;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      முழு coord = i < coord_len ? coords[i] : 0;
      தசம factor = axes[i].evaluate (coord);
      எனில் (factor == 0.f)
      {
	v = 0.f;
	நிறுத்து;
      }
      v *= factor;
    }

    திரும்பு v;
  }

  பொது:
  HB_ALWAYS_INLINE
  தசம evaluate (unsigned முழு region_index,
		  மாறிலி முழு *coords, unsigned முழு coord_len,
		  hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    எனில் (unlikely (region_index >= region_மொத்தம்))
      திரும்பு 0.;

    தசம v;
    எனில் (cache && cache->get (region_index, &v))
      திரும்பு v;

    v = evaluate_impl (region_index, coords, coord_len);

    எனில் (cache)
      cache->set (region_index, v);
    திரும்பு v;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_மொத்தம்_2 * region_மொத்தம்));
  }

  தருக்க serialize (hb_serialize_context_t *c,
                  மாறிலி hb_vector_t<hb_tag_t>& axis_tags,
                  மாறிலி hb_vector_t<மாறிலி hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (இது);
    unsigned axis_மொத்தம் = axis_tags.length;
    unsigned region_மொத்தம்_2 = regions.length;
    எனில் (!axis_மொத்தம் || !region_மொத்தம்_2) return_trace (பொய்);
    எனில் (unlikely (hb_unsigned_mul_overflows (axis_மொத்தம் * region_மொத்தம்_2,
                                             VarRegionAxis::static_size))) return_trace (பொய்);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    axis_மொத்தம்_2 = axis_மொத்தம்;
    region_மொத்தம் = region_மொத்தம்_2;

    க்கு (unsigned r = 0; r < region_மொத்தம்_2; r++)
    {
      மாறிலி auto& region = regions[r];
      க்கு (unsigned i = 0; i < axis_மொத்தம்; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        எனில் (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->நடு);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        இல்லையெனில்
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        எனில் (!var_region_rec.serialize (c))
          return_trace (பொய்);
      }
    }
    return_trace (உண்மை);
  }

  தருக்க serialize (hb_serialize_context_t *c, மாறிலி VarRegionList *src, மாறிலி hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    axis_மொத்தம்_2 = src->axis_மொத்தம்_2;
    region_மொத்தம் = region_map.get_population ();
    எனில் (unlikely (hb_unsigned_mul_overflows (axis_மொத்தம்_2 * region_மொத்தம்,
					     VarRegionAxis::static_size))) return_trace (பொய்);
    எனில் (unlikely (!c->extend (இது))) return_trace (பொய்);
    unsigned முழு region_மொத்தம்_2 = src->region_மொத்தம்;
    க்கு (unsigned முழு r = 0; r < region_மொத்தம்; r++)
    {
      unsigned முழு backward = region_map.backward (r);
      எனில் (backward >= region_மொத்தம்_2) return_trace (பொய்);
      hb_memcpy (&axesZ[axis_மொத்தம்_2 * r], &src->axesZ[axis_மொத்தம்_2 * backward], VarRegionAxis::static_size * axis_மொத்தம்_2);
    }

    return_trace (உண்மை);
  }

  தருக்க get_var_region (unsigned region_index,
                       மாறிலி hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) மாறிலி
  {
    எனில் (region_index >= region_மொத்தம்) திரும்பு பொய்;
    மாறிலி VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_மொத்தம்_2);
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      hb_tag_t *axis_tag;
      எனில் (!axes_old_index_tag_map.has (i, &axis_tag))
        திரும்பு பொய்;

      தசம min_val = axis_region->startCoord.to_float ();
      தசம def_val = axis_region->peakCoord.to_float ();
      தசம max_val = axis_region->endCoord.to_float ();

      எனில் (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((இரட்டை) min_val, (இரட்டை) def_val, (இரட்டை) max_val));
      axis_region++;
    }
    திரும்பு !axis_tuples.in_error ();
  }

  தருக்க get_var_regions (மாறிலி hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) மாறிலி
  {
    எனில் (!regions.alloc (region_மொத்தம்))
      திரும்பு பொய்;

    க்கு (unsigned i = 0; i < region_மொத்தம்; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      எனில் (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        திரும்பு பொய்;
      regions.push (std::move (axis_tuples));
    }
    திரும்பு !regions.in_error ();
  }

  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_மொத்தம்_2, region_மொத்தம்)); }

  பொது:
  HBUINT16	axis_மொத்தம்_2;
  HBUINT15	region_மொத்தம்;
  பாதுகாக்கப்பட்ட:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  பொது:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
நிலையான உள்வரி தருக்க
_hb_avar2_region_is_dead (மாறிலி hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  மாறிலி hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  க்கு (auto _ : axis_tuples)
  {
    Triple *range;
    எனில் (!reachable_ranges.has (_.first, &range)) தொடர்;
    மாறிலி Triple &tent = _.second;
    முழு start = (முழு) roundf ((தசம) tent.minimum * 16384.f);
    முழு peak  = (முழு) roundf ((தசம) tent.நடு * 16384.f);
    முழு end   = (முழு) roundf ((தசம) tent.maximum * 16384.f);
    எனில் (!peak) தொடர்;
    எனில் (start > peak || peak > end) தொடர்;
    எனில் (start < 0 && end > 0) தொடர்;
    முழு lo = (முழு) roundf ((தசம) range->minimum * 16384.f);
    முழு hi = (முழு) roundf ((தசம) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    எனில் (hi < start || lo > end)
      திரும்பு உண்மை;
  }
  திரும்பு பொய்;
}

கட்டமைப்பு SparseVariationRegion
{
  தசம evaluate (மாறிலி முழு *coords, unsigned முழு coord_len) மாறிலி
  {
    தசம v = 1.f;
    unsigned முழு மொத்தம் = axes.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      தசம factor = (இது+axes[i]).evaluate (coords, coord_len);
      எனில் (factor == 0.f)
	திரும்பு 0.;
      v *= factor;
    }
    திரும்பு v;
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி SparseVariationRegion *src,
		  மாறிலி hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (பொய்);
    க்கு (unsigned i = 0; i < axes.len; i++)
      எனில் (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (axes.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  பொது:
  DEFINE_SIZE_ARRAY (2, axes);
};

கட்டமைப்பு SparseVarRegionList
{
  HB_ALWAYS_INLINE
  தசம evaluate (unsigned முழு region_index,
		  மாறிலி முழு *coords, unsigned முழு coord_len,
		  hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    எனில் (unlikely (region_index >= regions.len))
      திரும்பு 0.;

    தசம v;
    எனில் (cache && cache->get (region_index, &v))
      திரும்பு v;

    மாறிலி SparseVariationRegion &region = இது+regions[region_index];

    v = region.evaluate (coords, coord_len);
    எனில் (cache)
      cache->set (region_index, v);

    திரும்பு v;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (regions.sanitize (c, இது));
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி SparseVarRegionList *src,
		  மாறிலி hb_inc_bimap_t &region_map,
		  மாறிலி hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    regions.len = region_map.get_population ();
    எனில் (unlikely (!c->extend (regions))) return_trace (பொய்);

    க்கு (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      எனில் (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (பொய்);
    }
    return_trace (உண்மை);
  }

  பொது:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  பொது:
  DEFINE_SIZE_ARRAY (4, regions);
};


கட்டமைப்பு VarData
{
  unsigned முழு get_item_மொத்தம் () மாறிலி
  { திரும்பு item_மொத்தம்; }

  unsigned முழு get_region_index_மொத்தம் () மாறிலி
  { திரும்பு regionIndices.len; }

  unsigned get_region_index (unsigned i) மாறிலி
  { திரும்பு i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () மாறிலி
  { திரும்பு (word_மொத்தம் () + regionIndices.len) * (long_சொற்கள்_2 () ? 2 : 1); }

  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_மொத்தம், get_row_size ())); }

  template <typename Float = தசம>
  Float _get_delta (unsigned முழு inner,
		    மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
		    மாறிலி VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    எனில் (unlikely (inner >= item_மொத்தம்))
      திரும்பு 0.;
    தருக்க is_long = long_சொற்கள்_2 ();
    unsigned முழு மொத்தம் = regionIndices.len;
    unsigned word_மொத்தம்_2 = word_மொத்தம் ();
    unsigned முழு scount = is_long ? மொத்தம் : word_மொத்தம்_2;
    unsigned முழு lcount = is_long ? word_மொத்தம்_2 : 0;

    மாறிலி HBUINT8 *bytes = get_delta_bytes ();
    மாறிலி HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned முழு i = 0;

    மாறிலி HBINT32 *lcursor = reinterpret_cast<மாறிலி HBINT32 *> (row);
    க்கு (; i < lcount; i++)
    {
      தசம scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_மொத்தம், cache);
      எனில் (scalar)
        delta += (Float) scalar * (முழு) *lcursor;
      lcursor++;
    }
    மாறிலி HBINT16 *scursor = reinterpret_cast<மாறிலி HBINT16 *> (lcursor);
    க்கு (; i < scount; i++)
    {
      தசம scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_மொத்தம், cache);
      எனில் (scalar)
       delta += (Float) scalar * (முழு) *scursor;
      scursor++;
    }
    மாறிலி HBINT8 *bcursor = reinterpret_cast<மாறிலி HBINT8 *> (scursor);
    க்கு (; i < மொத்தம்; i++)
    {
      தசம scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_மொத்தம், cache);
      எனில் (scalar)
        delta += (Float) scalar * (முழு) *bcursor;
      bcursor++;
    }

    திரும்பு delta;
  }

  template <typename Float = தசம>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned முழு inner,
		   மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
		   மாறிலி VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    unsigned முழு மொத்தம் = regionIndices.len;
    எனில் (!மொத்தம்) திரும்பு Float (0); // This is quite common, so optimize it.
    திரும்பு _get_delta<Float> (inner, coords, coord_மொத்தம், regions, cache);
  }

  வெறுமை get_region_scalars (மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
			   மாறிலி VarRegionList &regions,
			   தசம *scalars /*OUT */,
			   unsigned முழு இலக்கம்_scalars) மாறிலி
  {
    unsigned மொத்தம் = hb_min (இலக்கம்_scalars, regionIndices.len);
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_மொத்தம்);
    க்கு (unsigned முழு i = மொத்தம்; i < இலக்கம்_scalars; i++)
      scalars[i] = 0.f;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_மொத்தம் () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_மொத்தம்,
				  get_row_size ()));
  }

  தருக்க serialize (hb_serialize_context_t *c,
                  தருக்க has_long,
                  மாறிலி hb_vector_t<மாறிலி hb_vector_t<முழு>*>& rows)
  {
    TRACE_SERIALIZE (இது);
    unsigned row_மொத்தம்_2 = rows.length;
    எனில் (!row_மொத்தம்_2) {
      // Nothing to serialize, will be empty.
      திரும்பு பொய்;
    }

    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    எனில் (unlikely (!c->check_assign (item_மொத்தம், row_மொத்தம்_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (பொய்);

    முழு min_threshold = has_long ? -32768 : -128;
    முழு max_threshold = has_long ? +32767 : +127;
    எண்ணிக்கை delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned இலக்கம்_regions = rows[0]->length;
    எனில் (!delta_sz.resize (இலக்கம்_regions))
      return_trace (பொய்);

    unsigned word_மொத்தம்_2 = 0;
    க்கு (unsigned r = 0; r < இலக்கம்_regions; r++)
    {
      க்கு (unsigned i = 0; i < row_மொத்தம்_2; i++)
      {
        முழு delta = rows[i]->arrayZ[r];
        எனில் (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_மொத்தம்_2++;
          நிறுத்து;
        }
        இல்லையெனில் எனில் (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_மொத்தம்_2;
    hb_map_t ri_map;
    க்கு (unsigned r = 0; r < இலக்கம்_regions; r++)
    {
      எனில் (!delta_sz[r]) தொடர்;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      எனில் (!ri_map.set (new_r, r))
        return_trace (பொய்);
    }

    word_Size_மொத்தம் = word_மொத்தம்_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_மொத்தம் = ri_map.get_population ();
    regionIndices.len = ri_மொத்தம்;
    எனில் (unlikely (!c->extend (இது))) return_trace (பொய்);

    க்கு (unsigned r = 0; r < ri_மொத்தம்; r++)
    {
      hb_codepoint_t *idx;
      எனில் (!ri_map.has (r, &idx))
        return_trace (பொய்);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    க்கு (unsigned முழு i = 0; i < row_மொத்தம்_2; i++)
    {
      க்கு (unsigned முழு r = 0; r < ri_மொத்தம்; r++)
      {
        முழு delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (உண்மை);
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி VarData *src,
		  மாறிலி hb_inc_bimap_t &inner_map,
		  மாறிலி hb_inc_bimap_t &region_map,
		  மாறிலி hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    item_மொத்தம் = inner_map.get_next_மதிப்பு ();

    /* Optimize word count */
    unsigned ri_மொத்தம் = src->regionIndices.len;
    எண்ணிக்கை delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned முழு> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_மொத்தம்);
    ri_map.resize (ri_மொத்தம்);
    unsigned முழு new_word_மொத்தம் = 0;
    unsigned முழு r;

    மாறிலி HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_மொத்தம் = src->word_மொத்தம் ();
    தருக்க     src_long_சொற்கள் = src->long_சொற்கள்_2 ();

    தருக்க has_long = பொய்;
    எனில் (src_long_சொற்கள்)
    {
      க்கு (r = 0; r < src_word_மொத்தம்; r++)
      {
        க்கு (unsigned old_gid : inner_map.சாவிகள்())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  எனில் (delta < -32768 || 32767 < delta)
	  {
	    has_long = உண்மை;
	    நிறுத்து;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    க்கு (r = 0; r < ri_மொத்தம்; r++)
    {
      தருக்க short_circuit = src_long_சொற்கள் == has_long && src_word_மொத்தம் <= r;

      delta_sz[r] = kZero;
      எனில் (culled_regions && culled_regions->has (src->regionIndices[r]))
	தொடர்; /* unreachable region: drop its column */
      க்கு (unsigned old_gid : inner_map.சாவிகள்())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	எனில் (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_மொத்தம்++;
	  நிறுத்து;
	}
	இல்லையெனில் எனில் (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  எனில் (short_circuit)
	    நிறுத்து;
	}
      }
    }

    unsigned முழு word_index = 0;
    unsigned முழு non_word_index = new_word_மொத்தம்;
    unsigned முழு new_ri_மொத்தம் = 0;
    க்கு (r = 0; r < ri_மொத்தம்; r++)
      எனில் (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_மொத்தம்++;
      }

    word_Size_மொத்தம் = new_word_மொத்தம் | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_மொத்தம்;

    எனில் (unlikely (!c->extend (இது))) return_trace (பொய்);

    க்கு (r = 0; r < new_ri_மொத்தம்; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned மொத்தம் = item_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      unsigned முழு old = inner_map.backward (i);
      க்கு (unsigned முழு r = 0; r < new_ri_மொத்தம்; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (உண்மை);
  }

  வெறுமை collect_region_refs (hb_set_t &region_indices, மாறிலி hb_inc_bimap_t &inner_map) மாறிலி
  {
    மாறிலி HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    க்கு (unsigned முழு r = 0; r < regionIndices.len; r++)
    {
      unsigned முழு region = regionIndices.arrayZ[r];
      எனில் (region_indices.has (region)) தொடர்;
      க்கு (hb_codepoint_t old_gid : inner_map.சாவிகள்())
	எனில் (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  நிறுத்து;
	}
    }
  }

  பொது:
  மாறிலி HBUINT8 *get_delta_bytes () மாறிலி
  { திரும்பு &StructAfter<HBUINT8> (regionIndices); }

  பாதுகாக்கப்பட்ட:
  HBUINT8 *get_delta_bytes ()
  { திரும்பு &StructAfter<HBUINT8> (regionIndices); }

  பொது:
  int32_t get_item_delta_fast (unsigned முழு item, unsigned முழு region,
			       மாறிலி HBUINT8 *delta_bytes, unsigned row_size) மாறிலி
  {
    எனில் (unlikely (item >= item_மொத்தம் || region >= regionIndices.len)) திரும்பு 0;

    மாறிலி HBINT8 *p = (மாறிலி HBINT8 *) delta_bytes + item * row_size;
    unsigned word_மொத்தம்_2 = word_மொத்தம் ();
    தருக்க is_long = long_சொற்கள்_2 ();
    எனில் (is_long)
    {
      எனில் (region < word_மொத்தம்_2)
	திரும்பு ((மாறிலி HBINT32 *) p)[region];
      இல்லையெனில்
	திரும்பு ((மாறிலி HBINT16 *)(p + HBINT32::static_size * word_மொத்தம்_2))[region - word_மொத்தம்_2];
    }
    இல்லையெனில்
    {
      எனில் (region < word_மொத்தம்_2)
	திரும்பு ((மாறிலி HBINT16 *) p)[region];
      இல்லையெனில்
	திரும்பு (p + HBINT16::static_size * word_மொத்தம்_2)[region - word_மொத்தம்_2];
    }
  }
  int32_t get_item_delta (unsigned முழு item, unsigned முழு region) மாறிலி
  {
     திரும்பு get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  பாதுகாக்கப்பட்ட:
  வெறுமை set_item_delta_fast (unsigned முழு item, unsigned முழு region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_மொத்தம்_2 = word_மொத்தம் ();
    தருக்க is_long = long_சொற்கள்_2 ();
    எனில் (is_long)
    {
      எனில் (region < word_மொத்தம்_2)
	((HBINT32 *) p)[region] = delta;
      இல்லையெனில்
	((HBINT16 *)(p + HBINT32::static_size * word_மொத்தம்_2))[region - word_மொத்தம்_2] = delta;
    }
    இல்லையெனில்
    {
      எனில் (region < word_மொத்தம்_2)
	((HBINT16 *) p)[region] = delta;
      இல்லையெனில்
	(p + HBINT16::static_size * word_மொத்தம்_2)[region - word_மொத்தம்_2] = delta;
    }
  }
  வெறுமை set_item_delta (unsigned முழு item, unsigned முழு region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  தருக்க long_சொற்கள்_2 () மாறிலி { திரும்பு word_Size_மொத்தம் & 0x8000u /* LONG_WORDS */; }
  unsigned word_மொத்தம் () மாறிலி { திரும்பு word_Size_மொத்தம் & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  பாதுகாக்கப்பட்ட:
  HBUINT16		item_மொத்தம்;
  HBUINT16		word_Size_மொத்தம்;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  பொது:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

கட்டமைப்பு MultiVarData
{
  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  மாறிலி TupleList &get_delta_sets () மாறிலி
  { திரும்பு இது+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  வெறுமை get_delta (unsigned முழு inner,
		  மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
		  மாறிலி SparseVarRegionList &regions,
		  hb_array_t<இரட்டை> out,
		  hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    auto &deltaSets = get_delta_sets ();

    auto மதிப்புகள்_iter = deltaSets.fetcher (inner);
    unsigned region_மொத்தம் = regionIndices.len;
    unsigned skip = 0;
    க்கு (unsigned regionIndex = 0; regionIndex < region_மொத்தம்; regionIndex++)
    {
      தசம scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_மொத்தம்,
				       cache);
      // We skip lazily. Helps with the tail end.
      எனில் (scalar == 0.0f)
        skip += out.length;
      இல்லையெனில்
      {
        எனில் (skip)
	{
	  மதிப்புகள்_iter.skip (skip);
	  skip = 0;
	}
	மதிப்புகள்_iter.add_to (out, scalar);
      }
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, இது));
  }

  unsigned get_item_மொத்தம் () மாறிலி
  { திரும்பு get_delta_sets ().மொத்தம்; }

  வெறுமை collect_region_refs (hb_set_t &region_indices) மாறிலி
  {
    க்கு (unsigned region : regionIndices)
      region_indices.add (region);
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி MultiVarData *src,
		  மாறிலி hb_inc_bimap_t &inner_map,
		  மாறிலி hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (இது);
    unsigned region_மொத்தம்_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_மொத்தம்_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    எனில் (unlikely (!c->extend_size (இது, header_size, பொய்)))
      return_trace (பொய்);
    format = 1;
    regionIndices.len = region_மொத்தம்_2;
    க்கு (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      எனில் (unlikely (!region_map.has (old_region)))
	return_trace (பொய்);
      regionIndices[i] = region_map.get (old_region);
    }

    மாறிலி auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    எனில் (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (பொய்);
    க்கு (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      எனில் (unlikely (old_inner >= src_delta_sets.மொத்தம்)) return_trace (பொய்);
      hb_ubytes_t bytes = static_cast<மாறிலி CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    எனில் (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (பொய்);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  பொது:
  DEFINE_SIZE_MIN (7);
};

கட்டமைப்பு ItemVariationStore
{
  friend கட்டமைப்பு item_variations_t;

  hb_scalar_cache_t *create_cache () மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு hb_scalar_cache_t::create (0);
#endif
    திரும்பு hb_scalar_cache_t::create ((இது+regions).region_மொத்தம்);
  }

  நிலையான வெறுமை destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  தனிப்பட்ட:
  template <typename Float = தசம>
  Float get_delta (unsigned முழு outer, unsigned முழு inner,
		   மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
		   hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு Float (0);
#endif

    எனில் (unlikely (outer >= dataSets.len))
      திரும்பு Float (0);

    திரும்பு (இது+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_மொத்தம்,
					     இது+regions,
					     cache);
  }

  பொது:
  தருக்க has_delta_set (unsigned முழு index) மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு பொய்;
#endif

    unsigned முழு outer = index >> 16;
    unsigned முழு inner = index & 0xFFFF;
    திரும்பு outer < dataSets.len &&
	   inner < (இது+dataSets[outer]).get_item_மொத்தம் ();
  }

  template <typename Float = தசம>
  Float get_delta (unsigned முழு index,
		   மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
		   hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    unsigned முழு outer = index >> 16;
    unsigned முழு inner = index & 0xFFFF;
    திரும்பு get_delta<Float> (outer, inner, coords, coord_மொத்தம், cache);
  }
  template <typename Float = தசம>
  Float get_delta (unsigned முழு index,
		   hb_array_t<மாறிலி முழு> coords,
		   hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    திரும்பு get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு உண்மை;
#endif

    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, இது) &&
		  dataSets.sanitize (c, இது));
  }

  தருக்க serialize (hb_serialize_context_t *c,
                  தருக்க has_long,
                  மாறிலி hb_vector_t<hb_tag_t>& axis_tags,
                  மாறிலி hb_vector_t<மாறிலி hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  மாறிலி hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (இது);
#ifdef HB_NO_VAR
    return_trace (பொய்);
#endif
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    format = 1;
    எனில் (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (பொய்);

    unsigned இலக்கம்_var_data = vardata_encodings.length;
    எனில் (!இலக்கம்_var_data) return_trace (பொய்);
    எனில் (unlikely (!c->check_assign (dataSets.len, இலக்கம்_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (பொய்);

    எனில் (unlikely (!c->extend (dataSets))) return_trace (பொய்);
    க்கு (unsigned i = 0; i < இலக்கம்_var_data; i++)
      எனில் (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].பொருட்கள்))
        return_trace (பொய்);

    return_trace (உண்மை);
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி ItemVariationStore *src,
		  மாறிலி hb_array_t <மாறிலி hb_inc_bimap_t> &inner_maps,
		  மாறிலி hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (இது);
#ifdef HB_NO_VAR
    return_trace (பொய்);
#endif

    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    unsigned முழு set_மொத்தம் = 0;
    க்கு (unsigned முழு i = 0; i < inner_maps.length; i++)
      எனில் (inner_maps[i].get_population ())
	set_மொத்தம்++;

    format = 1;

    மாறிலி auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    க்கு (unsigned முழு i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    எனில் (region_indices.in_error ())
      return_trace (பொய்);

    region_indices.del_range ((src_regions).region_மொத்தம், hb_set_t::INVALID);

    எனில் (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    எனில் (region_map.in_error())
      return_trace (பொய்);

    எனில் (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (பொய்);

    dataSets.len = set_மொத்தம்;
    எனில் (unlikely (!c->extend (dataSets))) return_trace (பொய்);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned முழு set_index = 0;
    க்கு (unsigned முழு i = 0; i < inner_maps.length; i++)
    {
      எனில் (!inner_maps[i].get_population ()) தொடர்;
      எனில் (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->start_embed (இது);
    எனில் (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned மொத்தம் = dataSets.len;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      எனில் (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = இது+dataSets[i];

      unsigned item_மொத்தம் = data.get_item_மொத்தம் ();
      க்கு (unsigned j = 0; j < item_மொத்தம்; j++)
	map->add (j);
    }

    எனில் (unlikely (!out->serialize (c, இது, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  தருக்க collect_dead_regions (மாறிலி hb_map_t &axes_old_index_tag_map,
			     மாறிலி hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) மாறிலி
  {
#ifndef HB_NO_VAR
    எனில் (!reachable_ranges.get_population ()) திரும்பு உண்மை;
    மாறிலி VarRegionList &region_list = இது+regions;
    unsigned மொத்தம் = region_list.region_மொத்தம்;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      எனில் (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	திரும்பு பொய்;
      எனில் (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    திரும்பு !dead_regions.in_error ();
#இல்லையெனில்
    திரும்பு உண்மை;
#endif
  }

  தருக்க subset (hb_subset_context_t *c, மாறிலி hb_array_t<மாறிலி hb_inc_bimap_t> &inner_maps) மாறிலி
  {
    TRACE_SUBSET (இது);
#ifdef HB_NO_VAR
    return_trace (பொய்);
#endif

    ItemVariationStore *varstore_பகாஎண் = c->serializer->start_embed<ItemVariationStore> ();
    எனில் (unlikely (!varstore_பகாஎண்)) return_trace (பொய்);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    எனில் (c->plan->has_avar2)
      எனில் (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (பொய்);

    varstore_பகாஎண்->serialize (c->serializer, இது, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_பகாஎண்->dataSets);
  }

  unsigned முழு get_region_index_மொத்தம் (unsigned முழு major) மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு 0;
#endif
    திரும்பு (இது+dataSets[major]).get_region_index_மொத்தம் ();
  }

  வெறுமை get_region_scalars (unsigned முழு major,
			   மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
			   தசம *scalars /*OUT*/,
			   unsigned முழு இலக்கம்_scalars) மாறிலி
  {
#ifdef HB_NO_VAR
    க்கு (unsigned i = 0; i < இலக்கம்_scalars; i++)
      scalars[i] = 0.f;
    திரும்பு;
#endif

    (இது+dataSets[major]).get_region_scalars (coords, coord_மொத்தம்,
					       இது+regions,
					       &scalars[0], இலக்கம்_scalars);
  }

  unsigned முழு get_sub_table_மொத்தம் () மாறிலி
   {
#ifdef HB_NO_VAR
     திரும்பு 0;
#endif
     திரும்பு dataSets.len;
   }

  மாறிலி VarData& get_sub_table (unsigned i) மாறிலி
  {
#ifdef HB_NO_VAR
     திரும்பு Null (VarData);
#endif
     திரும்பு இது+dataSets[i];
  }

  மாறிலி VarRegionList& get_region_list () மாறிலி
  {
#ifdef HB_NO_VAR
     திரும்பு Null (VarRegionList);
#endif
     திரும்பு இது+regions;
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

கட்டமைப்பு MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு hb_scalar_cache_t::create (0);
#endif
    auto &r = இது+regions;
    unsigned மொத்தம் = r.regions.len;

    திரும்பு hb_scalar_cache_t::create (மொத்தம், static_cache);
  }

  நிலையான வெறுமை destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  தனிப்பட்ட:
  வெறுமை get_delta (unsigned முழு outer, unsigned முழு inner,
		  மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
		  hb_array_t<இரட்டை> out,
		  hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு;
#endif

    எனில் (unlikely (outer >= dataSets.len))
      திரும்பு;

    திரும்பு (இது+dataSets[outer]).get_delta (inner,
					     coords, coord_மொத்தம்,
					     இது+regions,
					     out,
					     cache);
  }

  பொது:
  வெறுமை get_delta (unsigned முழு index,
		  மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
		  hb_array_t<இரட்டை> out,
		  hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    unsigned முழு outer = index >> 16;
    unsigned முழு inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_மொத்தம், out, cache);
  }
  வெறுமை get_delta (unsigned முழு index,
		  hb_array_t<மாறிலி முழு> coords,
		  hb_array_t<இரட்டை> out,
		  hb_scalar_cache_t *cache = nullptr) மாறிலி
  {
    திரும்பு get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
#ifdef HB_NO_VAR
    திரும்பு உண்மை;
#endif

    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, இது) &&
		  dataSets.sanitize (c, இது));
  }

  தருக்க create_subset_plan (மாறிலி hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) மாறிலி
  {
    எனில் (unlikely (!inner_maps->resize (dataSets.len))) திரும்பு பொய்;
    க்கு (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      எனில் (unlikely (outer >= dataSets.len ||
		    inner >= (இது+dataSets[outer]).get_item_மொத்தம் ()))
	திரும்பு பொய்;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    க்கு (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      மாறிலி hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      எனில் (!inner_map.get_population ()) தொடர்;
      எனில் (unlikely (inner_map.in_error ())) திரும்பு பொய்;
      க்கு (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    திரும்பு !inner_maps->in_error () && !varidx_map->in_error ();
  }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி MultiItemVariationStore *src,
		  மாறிலி hb_array_t<மாறிலி hb_inc_bimap_t> &inner_maps,
		  மாறிலி hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (பொய்);
    format = 1;

    மாறிலி auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_மொத்தம் = 0;
    க்கு (unsigned i = 0; i < src->dataSets.len; i++)
    {
      எனில் (!inner_maps[i].get_population ()) தொடர்;
      set_மொத்தம்++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    எனில் (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (பொய்);

    dataSets.len = set_மொத்தம்;
    எனில் (unlikely (!c->extend (dataSets))) return_trace (பொய்);
    unsigned set_index = 0;
    க்கு (unsigned i = 0; i < src->dataSets.len; i++)
    {
      எனில் (!inner_maps[i].get_population ()) தொடர்;
      எனில் (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (பொய்);
    }
    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_மொத்தம்_T>
கட்டமைப்பு DeltaSetIndexMapFormat01
{
  friend கட்டமைப்பு DeltaSetIndexMap;

  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_mul_add_saturate (map_மொத்தம்_2, get_width (), min_size); }

  தனிப்பட்ட:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed (இது));
  }

  template <typename T>
  தருக்க serialize (hb_serialize_context_t *c, மாறிலி T &plan)
  {
    unsigned முழு width = plan.get_width ();
    unsigned முழு inner_bit_மொத்தம் = plan.get_inner_bit_மொத்தம் ();
    மாறிலி hb_array_t<மாறிலி uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (இது);
    எனில் (unlikely (output_map.length && ((((inner_bit_மொத்தம்-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (பொய்);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);

    entryFormat = ((width-1)<<4)|(inner_bit_மொத்தம்-1);
    map_மொத்தம்_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    எனில் (unlikely (!p)) return_trace (பொய்);
    க்கு (unsigned முழு i = 0; i < output_map.length; i++)
    {
      unsigned முழு v = output_map.arrayZ[i];
      எனில் (v)
      {
	unsigned முழு outer = v >> 16;
	unsigned முழு inner = v & 0xFFFF;
	unsigned முழு u = (outer << inner_bit_மொத்தம்) | inner;
	க்கு (unsigned முழு w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (உண்மை);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned முழு v) மாறிலி /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    எனில் (!map_மொத்தம்_2)
      திரும்பு v;
    திரும்பு _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned முழு v) மாறிலி /* Returns 16.16 outer.inner. */
  {
    எனில் (v >= map_மொத்தம்_2)
      v = map_மொத்தம்_2 - 1;

    unsigned முழு u = 0;
    { /* Fetch it. */
      unsigned முழு w = get_width ();
      மாறிலி HBUINT8 *p = mapDataZ.arrayZ + w * v;
      க்கு (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned முழு n = get_inner_bit_மொத்தம் ();
      unsigned முழு outer = u >> n;
      unsigned முழு inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    திரும்பு u;
  }

  unsigned get_map_மொத்தம் () மாறிலி       { திரும்பு map_மொத்தம்_2; }
  unsigned get_width () மாறிலி           { திரும்பு ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_மொத்தம் () மாறிலி { திரும்பு (entryFormat & 0xF) + 1; }


  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_மொத்தம்_2,
                                  get_width ()));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_மொத்தம்_T     map_மொத்தம்_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  பொது:
  DEFINE_SIZE_ARRAY (2+Map_மொத்தம்_T::static_size, mapDataZ);
};

கட்டமைப்பு DeltaSetIndexMap
{
  template <typename T>
  தருக்க serialize (hb_serialize_context_t *c, மாறிலி T &plan)
  {
    TRACE_SERIALIZE (இது);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    நிலை 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    இயல்பு:return_trace (பொய்);
    }
  }

  uint32_t map (unsigned v) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); திரும்பு (u.format0.map (v));
    நிலை 1: hb_barrier (); திரும்பு (u.format1.map (v));
    இயல்பு:திரும்பு v;
    }
  }

  unsigned get_map_மொத்தம் () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_map_மொத்தம் ();
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_map_மொத்தம் ();
    இயல்பு:திரும்பு 0;
    }
  }

  unsigned get_width () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_width ();
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_width ();
    இயல்பு:திரும்பு 0;
    }
  }

  unsigned get_inner_bit_மொத்தம் () மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_inner_bit_மொத்தம் ();
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_inner_bit_மொத்தம் ();
    இயல்பு:திரும்பு 0;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    இயல்பு:return_trace (உண்மை);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    நிலை 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    இயல்பு:return_trace (nullptr);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  பொது:
  DEFINE_SIZE_UNION (1, format.v);
};


கட்டமைப்பு ItemVarStoreInstancer
{
  ItemVarStoreInstancer (மாறிலி ItemVariationStore *varStore_,
			 மாறிலி DeltaSetIndexMap *varIdxMap,
			 hb_array_t<மாறிலி முழு> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    எனில் (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    எனில் (!hb_any (coords))
      இது->coords = {};
  }

  operator தருக்க () மாறிலி { திரும்பு varStore && தருக்க (coords); }

  தசம operator[] (uint32_t varIdx) மாறிலி
  { திரும்பு (*இது) (varIdx); }

  தசம operator() (uint32_t varIdx, unsigned short offset = 0) மாறிலி
  { திரும்பு get_delta (varIdx, offset); }

  இரட்டை get_condition_delta (uint32_t varIdx) மாறிலி
  { திரும்பு get_delta<இரட்டை> (varIdx); }

  template <typename Float = தசம>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) மாறிலி
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   எனில் (!coords || varIdx == VarIdx::NO_VARIATION)
     திரும்பு Float (0);

    varIdx += offset;
    எனில் (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    திரும்பு varStore->get_delta<Float> (varIdx, coords, cache);
  }

  மாறிலி ItemVariationStore *varStore;
  மாறிலி DeltaSetIndexMap *varIdxMap;
  hb_array_t<மாறிலி முழு> coords;
  hb_scalar_cache_t *cache;
};

கட்டமைப்பு MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (மாறிலி MultiItemVariationStore *varStore,
			      மாறிலி DeltaSetIndexMap *varIdxMap,
			      hb_array_t<மாறிலி முழு> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    எனில் (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator தருக்க () மாறிலி { திரும்பு varStore; }

  இரட்டை get_condition_delta (uint32_t varIdx) மாறிலி
  { திரும்பு (இரட்டை) (*இது)[varIdx]; }

  இரட்டை operator[] (uint32_t varIdx) மாறிலி
  {
    இரட்டை v = 0;
    (*இது) (hb_array (&v, 1), varIdx);
    திரும்பு v;
  }

  வெறுமை operator() (hb_array_t<இரட்டை> out, uint32_t varIdx, unsigned short offset = 0) மாறிலி
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    எனில் (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      எனில் (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    இல்லையெனில்
      க்கு (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  மாறிலி MultiItemVariationStore *varStore;
  மாறிலி DeltaSetIndexMap *varIdxMap;
  hb_array_t<மாறிலி முழு> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
எண்ணிக்கை Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

எண்ணிக்கை lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

கட்டமைப்பு Condition;

template <typename Instancer>
நிலையான தருக்க
_hb_recurse_condition_evaluate (மாறிலி கட்டமைப்பு Condition &condition,
				மாறிலி முழு *coords,
				unsigned முழு coord_len,
				Instancer *instancer);

கட்டமைப்பு ConditionAxisRange
{
  friend கட்டமைப்பு Condition;

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    மாறிலி hb_map_t *index_map = &c->plan->axes_index_map;
    எனில் (index_map->is_empty ()) return_trace (உண்மை);

    மாறிலி hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    எனில் (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (பொய்);

    மாறிலி hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    எனில் (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    மாறிலி hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    எனில் (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    தசம normalized_min = renormalize_மதிப்பு ((இரட்டை) filter_Range_Min_மதிப்பு.to_float (), axis_limit, axis_triple_distances, பொய்);
    தசம normalized_max = renormalize_மதிப்பு ((இரட்டை) filter_Range_Max_மதிப்பு.to_float (), axis_limit, axis_triple_distances, பொய்);
    out->filter_Range_Min_மதிப்பு.set_float (normalized_min);
    out->filter_Range_Max_மதிப்பு.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தனிப்பட்ட:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) மாறிலி
  {
    //invalid axis index, drop the entire record
    எனில் (!c->axes_index_tag_map->has (axisIndex))
      திரும்பு DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    தருக்க axis_set_by_user = பொய்;
    எனில் (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = உண்மை;
    }

    தசம axis_min_val = axis_range.minimum;
    தசம axis_default_val = axis_range.நடு;
    தசம axis_max_val = axis_range.maximum;

    தசம filter_min_val = filter_Range_Min_மதிப்பு.to_float ();
    தசம filter_max_val = filter_Range_Max_மதிப்பு.to_float ();

    எனில் (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = பொய்;

    //condition not met, drop the entire record
    எனில் (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      திரும்பு DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    எனில் (axis_set_by_user && axis_range.is_point ())
      திரும்பு DROP_COND_WITH_VAR;

    எனில் (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_மதிப்பு.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_மதிப்பு.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      திரும்பு KEEP_COND_WITH_VAR;
    }
    திரும்பு KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  தருக்க evaluate (மாறிலி முழு *coords, unsigned முழு coord_len,
		 Instancer *instancer HB_UNUSED) மாறிலி
  {
    முழு coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    திரும்பு filter_Range_Min_மதிப்பு.to_int () <= coord && coord <= filter_Range_Max_மதிப்பு.to_int ();
  }

  பொது:
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_மதிப்பு;
  F2DOT14	filter_Range_Max_மதிப்பு;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு Condition_மதிப்பு
{
  friend கட்டமைப்பு Condition;

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (varIdx == VarIdx::NO_VARIATION)
      return_trace (உண்மை);

    hb_pair_t<unsigned, முழு> *condition_மதிப்பு;
    எனில் (c->plan->layout_condition_idx_மதிப்பு_map.has ((uintptr_t) இது, &condition_மதிப்பு))
    {
      out->default_மதிப்பு_2 = condition_மதிப்பு->second;
      out->varIdx = condition_மதிப்பு->first;
      return_trace (உண்மை);
    }

    hb_pair_t<unsigned, முழு> *new_varidx_delta;
    எனில் (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (பொய்);

    எனில் (unlikely (!c->serializer->check_assign (
		out->default_மதிப்பு_2,
		default_மதிப்பு_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (பொய்);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (உண்மை);
  }

  தனிப்பட்ட:
  template <typename Instancer>
  தருக்க evaluate (மாறிலி முழு *coords, unsigned முழு coord_len,
		 Instancer *instancer) மாறிலி
  {
    இரட்டை value = default_மதிப்பு_2;
    value += instancer->get_condition_delta (varIdx);
    திரும்பு value > 0;
  }

  பொது:
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_மதிப்பு_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு ConditionAnd
{
  friend கட்டமைப்பு Condition;

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (பொய்);
    out->format = format;

    க்கு (மாறிலி auto &offset : conditions)
    {
      எனில் (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	எனில் (unlikely (!out_offset)) return_trace (பொய்);
	*out_offset = 0;
      }
      இல்லையெனில் எனில் (unlikely (!subset_offset_array (c, out->conditions, இது) (offset)))
	return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  தனிப்பட்ட:
  template <typename Instancer>
  தருக்க evaluate (மாறிலி முழு *coords, unsigned முழு coord_len,
		 Instancer *instancer) மாறிலி
  {
    unsigned முழு மொத்தம் = conditions.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!_hb_recurse_condition_evaluate (இது+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  பொது:
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (conditions.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<கட்டமைப்பு Condition>	conditions;
  பொது:
  DEFINE_SIZE_ARRAY (3, conditions);
};

கட்டமைப்பு ConditionOr
{
  friend கட்டமைப்பு Condition;

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (பொய்);
    out->format = format;

    க்கு (மாறிலி auto &offset : conditions)
    {
      எனில் (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	எனில் (unlikely (!out_offset)) return_trace (பொய்);
	*out_offset = 0;
      }
      இல்லையெனில் எனில் (unlikely (!subset_offset_array (c, out->conditions, இது) (offset)))
	return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  தனிப்பட்ட:
  template <typename Instancer>
  தருக்க evaluate (மாறிலி முழு *coords, unsigned முழு coord_len,
		 Instancer *instancer) மாறிலி
  {
    unsigned முழு மொத்தம் = conditions.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (_hb_recurse_condition_evaluate (இது+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	திரும்பு உண்மை;
    திரும்பு பொய்;
  }

  பொது:
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (conditions.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<கட்டமைப்பு Condition>	conditions;
  பொது:
  DEFINE_SIZE_ARRAY (3, conditions);
};

கட்டமைப்பு ConditionNegate
{
  friend கட்டமைப்பு Condition;

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);
    எனில் (condition.is_null ())
    {
      out->condition = 0;
      return_trace (உண்மை);
    }
    return_trace (out->condition.serialize_subset (c, condition, இது));
  }

  தனிப்பட்ட:
  template <typename Instancer>
  தருக்க evaluate (மாறிலி முழு *coords, unsigned முழு coord_len,
		 Instancer *instancer) மாறிலி
  {
    திரும்பு !_hb_recurse_condition_evaluate (இது+condition,
					    coords, coord_len,
					    instancer);
  }

  பொது:
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (condition.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<கட்டமைப்பு Condition>	condition;
  பொது:
  DEFINE_SIZE_STATIC (5);
};

கட்டமைப்பு Condition
{
  friend கட்டமைப்பு ConditionSet;

  நிலையான தருக்க serialize_constant (hb_serialize_context_t *c, தருக்க value)
  {
    auto *out = c->start_embed<Condition_மதிப்பு> ();
    எனில் (unlikely (!out || !c->extend_min (out))) திரும்பு பொய்;
    out->format = 2;
    out->default_மதிப்பு_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    திரும்பு உண்மை;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      தருக்க materialize_true = பொய்) மாறிலி;

  template <typename Instancer>
  தருக்க evaluate (மாறிலி முழு *coords, unsigned முழு coord_len,
		 Instancer *instancer) மாறிலி
  {
    தேர்வு (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    நிலை 0: திரும்பு உண்மை;
    நிலை 1: hb_barrier (); திரும்பு u.format1.evaluate (coords, coord_len, instancer);
    நிலை 2: hb_barrier (); திரும்பு u.format2.evaluate (coords, coord_len, instancer);
    நிலை 3: hb_barrier (); திரும்பு u.format3.evaluate (coords, coord_len, instancer);
    நிலை 4: hb_barrier (); திரும்பு u.format4.evaluate (coords, coord_len, instancer);
    நிலை 5: hb_barrier (); திரும்பு u.format5.evaluate (coords, coord_len, instancer);
    இயல்பு:திரும்பு பொய்;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) மாறிலி
  {
    எனில் (unlikely (!depth))
    {
      c->apply = பொய்;
      திரும்பு DROP_RECORD_WITH_VAR;
    }

    தேர்வு (u.format.v) {
    நிலை 0: திரும்பு KEEP_RECORD_WITH_VAR;
    நிலை 1: hb_barrier (); திரும்பு u.format1.keep_with_variations (c, condition_map);
    நிலை 2: நிலை 3: நிலை 4: நிலை 5: நிறுத்து;
    இயல்பு: c->apply = பொய்; திரும்பு DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    தருக்க applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    எனில் (c->all_axes_pinned)
      திரும்பு applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    எனில் (u.format.v == 2)
      திரும்பு u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (மாறிலி Condition &child)
    {
      auto child_context = *c;
      child_context.apply = உண்மை;
      திரும்பு child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    எனில் (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      திரும்பு result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    தருக்க is_and = u.format.v == 3;
    தருக்க variable = பொய்;
    மாறிலி auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    மாறிலி வெறுமை *base = is_and ? (மாறிலி வெறுமை *) &u.format3 : (மாறிலி வெறுமை *) &u.format4;
    க்கு (மாறிலி auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      எனில் (is_and && result == DROP_RECORD_WITH_VAR)
        திரும்பு DROP_RECORD_WITH_VAR;
      எனில் (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        திரும்பு KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    திரும்பு variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    நிலை 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    நிலை 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    நிலை 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);

    எனில் (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_மதிப்பு ());

    return_trace (c->end_recursion (இது->dispatch (c)));
  }

  தருக்க collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, முழு>> *condition_மதிப்புகள் = nullptr) மாறிலி;

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி Condition *src,
		  மாறிலி hb_map_t &varidx_map,
		  மாறிலி hb_map_t *axis_map = nullptr);

  தனிப்பட்ட:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) மாறிலி;

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_மதிப்பு	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
தருக்க
_hb_recurse_condition_evaluate (மாறிலி கட்டமைப்பு Condition &condition,
				மாறிலி முழு *coords,
				unsigned முழு coord_len,
				Instancer *instancer)
{
  திரும்பு condition.evaluate (coords, coord_len, instancer);
}

கட்டமைப்பு ConditionList
{
  மாறிலி Condition& operator[] (unsigned i) மாறிலி
  { திரும்பு இது+conditions[i]; }

  unsigned get_மொத்தம் () மாறிலி { திரும்பு conditions.len; }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி ConditionList *src,
		  மாறிலி hb_inc_bimap_t &condition_map,
		  மாறிலி hb_map_t &varidx_map,
		  மாறிலி hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    conditions.len = condition_map.get_population ();
    எனில் (unlikely (!c->extend (conditions))) return_trace (பொய்);
    க்கு (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      எனில் (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (பொய்);
    }
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (conditions.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  Array32OfOffset32To<Condition> conditions;
  பொது:
  DEFINE_SIZE_ARRAY (4, conditions);
};

உள்வரி தருக்க
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, முழு>> *condition_மதிப்புகள்) மாறிலி
{
  எனில் (unlikely (!depth)) திரும்பு பொய்;
  தேர்வு (u.format.v)
  {
    நிலை 0:
    நிலை 1:
      திரும்பு உண்மை;
    நிலை 2:
      எனில் (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        எனில் (condition_மதிப்புகள் &&
            !condition_மதிப்புகள்->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), முழு (u.format2.default_மதிப்பு_2))))
          திரும்பு பொய்;
      }
      திரும்பு !var_indices->in_error ();
    நிலை 3:
      க்கு (மாறிலி auto &offset : u.format3.conditions)
	எனில் (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_மதிப்புகள்)))
	  திரும்பு பொய்;
      திரும்பு உண்மை;
    நிலை 4:
      க்கு (மாறிலி auto &offset : u.format4.conditions)
	எனில் (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_மதிப்புகள்)))
	  திரும்பு பொய்;
      திரும்பு உண்மை;
    நிலை 5:
      திரும்பு (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_மதிப்புகள்);
    இயல்பு:
      திரும்பு பொய்;
  }
}

உள்வரி தருக்க
Condition::serialize (hb_serialize_context_t *c,
		      மாறிலி Condition *src,
		      மாறிலி hb_map_t &varidx_map,
		      மாறிலி hb_map_t *axis_map)
{
  TRACE_SERIALIZE (இது);
  தேர்வு (src->u.format.v)
  {
    நிலை 0:
    {
      auto *out = c->start_embed<Condition_மதிப்பு> ();
      எனில் (unlikely (!out || !c->extend_min (out))) return_trace (பொய்);
      out->format = 2;
      out->default_மதிப்பு_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (உண்மை);
    }
    நிலை 1:
    {
      auto *out = c->embed (&src->u.format1);
      எனில் (unlikely (!out)) return_trace (பொய்);
      எனில் (axis_map)
      {
	எனில் (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (பொய்);
	}
	எனில் (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (பொய்);
      }
      return_trace (உண்மை);
    }
    நிலை 2:
    {
      auto *out = c->embed (&src->u.format2);
      எனில் (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (பொய்);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (உண்மை);
    }
    நிலை 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      எனில் (unlikely (!c->extend_min (out))) return_trace (பொய்);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      எனில் (unlikely (!c->extend (out->conditions))) return_trace (பொய்);
      க்கு (unsigned i = 0; i < out->conditions.len; i++)
	எனில் (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (பொய்);
      return_trace (உண்மை);
    }
    நிலை 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      எனில் (unlikely (!c->extend_min (out))) return_trace (பொய்);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      எனில் (unlikely (!c->extend (out->conditions))) return_trace (பொய்);
      க்கு (unsigned i = 0; i < out->conditions.len; i++)
	எனில் (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (பொய்);
      return_trace (உண்மை);
    }
    நிலை 5:
    {
      auto *out = c->embed (&src->u.format5);
      எனில் (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (பொய்);
      return_trace (உண்மை);
    }
    இயல்பு:
      return_trace (பொய்);
  }
}

உள்வரி lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    தருக்க materialize_true) மாறிலி
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  எனில் (result == LOOKUP_CONDITION_SUBSET_KEEP)
    திரும்பு result;

  c->serializer->revert (snap);
  எனில் (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    திரும்பு result;

  திரும்பு serialize_constant (c->serializer, உண்மை) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

உள்வரி lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) மாறிலி
{
  எனில் (unlikely (!depth))
    திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   மாறிலி Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    எனில் (unlikely (!offset))
      திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    எனில் (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->எடு_pack ());
    இல்லையெனில்
    {
      c->serializer->எடு_discard ();
      out.எடு ();
      c->serializer->revert (snap);
    }
    திரும்பு result;
  };

  தேர்வு (u.format.v)
  {
    நிலை 0:
      திரும்பு LOOKUP_CONDITION_SUBSET_TRUE;

    நிலை 1:
    {
      மாறிலி ConditionAxisRange &src = u.format1;
      எனில் (c->plan->user_axes_location.is_empty ())
        திரும்பு c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      எனில் (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      எனில் (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      தசம filter_min = src.filter_Range_Min_மதிப்பு.to_float ();
      தசம filter_max = src.filter_Range_Max_மதிப்பு.to_float ();
      எனில் (filter_min > filter_max)
        திரும்பு LOOKUP_CONDITION_SUBSET_FALSE;
      எனில் (axis_limit.is_point ())
        திரும்பு filter_min <= axis_limit.நடு &&
	       axis_limit.நடு <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      எனில் (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        திரும்பு LOOKUP_CONDITION_SUBSET_FALSE;
      எனில் (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        திரும்பு LOOKUP_CONDITION_SUBSET_TRUE;
      எனில் (!c->plan->axes_index_map.has (src.axisIndex))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      எனில் (unlikely (!out))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      எனில் (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_மதிப்பு.set_float (
	  renormalize_மதிப்பு (filter_min, axis_limit, axis_distances, பொய்));
      out->filter_Range_Max_மதிப்பு.set_float (
	  renormalize_மதிப்பு (filter_max, axis_limit, axis_distances, பொய்));
      எனில் (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;
      திரும்பு LOOKUP_CONDITION_SUBSET_KEEP;
    }

    நிலை 2:
    {
      மாறிலி Condition_மதிப்பு &src = u.format2;
      auto *out = c->serializer->embed (&src);
      எனில் (unlikely (!out))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;

      எனில் (src.varIdx == VarIdx::NO_VARIATION)
        திரும்பு c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((முழு) src.default_மதிப்பு_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, முழு> *condition_மதிப்பு;
      எனில் (c->plan->layout_condition_idx_மதிப்பு_map.has ((uintptr_t) &src, &condition_மதிப்பு))
      {
        out->default_மதிப்பு_2 = condition_மதிப்பு->second;
        out->varIdx = condition_மதிப்பு->first;
        எனில் (out->varIdx == VarIdx::NO_VARIATION)
          திரும்பு out->default_மதிப்பு_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        திரும்பு LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, முழு> *new_varidx_delta;
      எனில் (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        திரும்பு c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((முழு) src.default_மதிப்பு_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      எனில் (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        இரட்டை value = (முழு) src.default_மதிப்பு_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        திரும்பு value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      முழு default_மதிப்பு = (முழு) src.default_மதிப்பு_2 +
			  hb_second (*new_varidx_delta);
      எனில் (unlikely (!c->serializer->check_assign (
		out->default_மதிப்பு_2,
		default_மதிப்பு,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      திரும்பு LOOKUP_CONDITION_SUBSET_KEEP;
    }

    நிலை 3:
    {
      மாறிலி ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      எனில் (unlikely (!out || !c->serializer->extend_min (out)))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      க்கு (மாறிலி auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        எனில் (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          திரும்பு result;
      }
      திரும்பு out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    நிலை 4:
    {
      மாறிலி ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      எனில் (unlikely (!out || !c->serializer->extend_min (out)))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      க்கு (மாறிலி auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        எனில் (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          திரும்பு result;
      }
      திரும்பு out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    நிலை 5:
    {
      மாறிலி ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      எனில் (unlikely (!out))
        திரும்பு LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      எனில் (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->எடு_pack ());
        திரும்பு result;
      }

      c->serializer->எடு_discard ();
      எனில் (result == LOOKUP_CONDITION_SUBSET_TRUE)
        திரும்பு LOOKUP_CONDITION_SUBSET_FALSE;
      எனில் (result == LOOKUP_CONDITION_SUBSET_FALSE)
        திரும்பு LOOKUP_CONDITION_SUBSET_TRUE;
      திரும்பு result;
    }

    இயல்பு:
      திரும்பு LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

கட்டமைப்பு ConditionSet
{
  தருக்க evaluate (மாறிலி முழு *coords, unsigned முழு coord_len,
		 ItemVarStoreInstancer *instancer) மாறிலி
  {
    unsigned முழு மொத்தம் = conditions.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!(இது+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  வெறுமை keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) மாறிலி
  {
    hb_map_t *condition_map = hb_map_create ();
    எனில் (unlikely (!condition_map)) திரும்பு;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    எனில் (unlikely (!cond_set)) திரும்பு;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = உண்மை;
    தருக்க should_keep = பொய்;
    /* The range map cannot identify value or boolean expressions uniquely. */
    தருக்க can_deduplicate = உண்மை;
    unsigned இலக்கம்_kept_cond = 0, cond_idx = 0;
    க்கு (மாறிலி auto& offset : conditions)
    {
      மாறிலி Condition &condition = இது+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      எனில் (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = பொய்;
        திரும்பு;
      }

      எனில் (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = உண்மை;
        cond_set->add (cond_idx);
        இலக்கம்_kept_cond++;
      }

      எனில் (ret == KEEP_RECORD_WITH_VAR)
        should_keep = உண்மை;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = இலக்கம்_kept_cond == 0;
    எனில் (!should_keep && c->all_axes_pinned) திரும்பு;

    //check if condition_set is unique with variations
    எனில் (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      திரும்பு;

    எனில் (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  தருக்க subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               தருக்க செருகு_catch_all) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out))) return_trace (பொய்);

    எனில் (செருகு_catch_all) return_trace (உண்மை);

    hb_set_t *retained_cond_set = nullptr;
    எனில் (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned முழு மொத்தம் = conditions.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (retained_cond_set != nullptr && !retained_cond_set->has (i))
        தொடர்;
      மாறிலி Condition &condition = இது+conditions[i];
      எனில் (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, இது) (conditions[i]);
        தொடர்;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      எனில் (unlikely (!offset)) return_trace (பொய்);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, உண்மை);
      எனில் (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, பொய்) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      எனில் (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->எடு_discard ();
        c->serializer->check_success (பொய்);
        return_trace (பொய்);
      }
      c->serializer->add_link (*offset, c->serializer->எடு_pack ());
    }

    return_trace (தருக்க (out->conditions));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (conditions.sanitize (c, இது));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி auto &offset : conditions)
      எனில் (unlikely (!(இது+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_மதிப்புகள்)))
	c->layout_variation_indices->err ();
  }

  பாதுகாக்கப்பட்ட:
  Array16OfOffset32To<Condition>	conditions;
  பொது:
  DEFINE_SIZE_ARRAY (2, conditions);
};

கட்டமைப்பு FeatureTableSubstitutionRecord
{
  friend கட்டமைப்பு FeatureTableSubstitution;

  வெறுமை collect_lookups (மாறிலி வெறுமை *base, hb_set_t *lookup_indexes /* OUT */) மாறிலி
  {
    திரும்பு (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  வெறுமை closure_features (மாறிலி வெறுமை *base,
			 மாறிலி hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) மாறிலி
  {
    எனில் ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  வெறுமை collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, மாறிலி Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    மாறிலி hb_set_t *feature_indices,
                                                    மாறிலி வெறுமை *base) மாறிலி
  {
    எனில் (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  தருக்க serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  மாறிலி Feature *f, மாறிலி Tag *tag)
  {
    TRACE_SERIALIZE (இது);
    hb_serialize_context_t *s = c->subset_context->serializer;
    எனில் (unlikely (!s->extend_min (இது))) return_trace (பொய்);

    uint32_t *new_feature_idx;
    எனில் (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (பொய்);

    எனில் (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    s->push ();
    தருக்க ret = f->subset (c->subset_context, c, tag);
    எனில் (ret) s->add_link (feature, s->எடு_pack ());
    இல்லையெனில் s->எடு_discard ();

    return_trace (ret);
  }

  தருக்க subset (hb_subset_layout_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SUBSET (இது);
    uint32_t *new_feature_index;
    எனில் (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (பொய்);

    auto *out = c->subset_context->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->featureIndex = *new_feature_index;
    hb_pair_t<மாறிலி வெறுமை*, மாறிலி வெறுமை*> *original;
    எனில் (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (பொய்);
    மாறிலி Tag *tag = reinterpret_cast<மாறிலி Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base,
		 மாறிலி FeatureList *feature_list) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது))) return_trace (பொய்);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    மாறிலி Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு FeatureTableSubstitution
{
  வெறுமை collect_feature_indices (hb_set_t *feature_indices) மாறிலி
  {
    க்கு (மாறிலி FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  மாறிலி Feature *find_substitute (unsigned முழு feature_index) மாறிலி
  {
    unsigned முழு மொத்தம் = substitutions.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      மாறிலி FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      எனில் (record.featureIndex == feature_index)
	திரும்பு &(இது+record.feature);
    }
    திரும்பு nullptr;
  }

  வெறுமை collect_lookups (மாறிலி hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) மாறிலி
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([இது, lookup_indexes] (மாறிலி FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (இது, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  வெறுமை collect_name_ids (மாறிலி hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) மாறிலி
  {
    க்கு (மாறிலி FeatureTableSubstitutionRecord &record : substitutions)
      எனில் (feature_index_map->has (record.featureIndex))
        (இது+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  வெறுமை closure_features (மாறிலி hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) மாறிலி
  {
    க்கு (மாறிலி FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (இது, lookup_indexes, feature_indexes);
  }

  தருக்க intersects_features (மாறிலி hb_map_t *feature_index_map) மாறிலி
  {
    க்கு (மாறிலி FeatureTableSubstitutionRecord& record : substitutions)
    {
      எனில் (feature_index_map->has (record.featureIndex)) திரும்பு உண்மை;
    }
    திரும்பு பொய்;
  }

  வெறுமை collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) மாறிலி
  {
    க்கு (மாறிலி FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, இது);
  }

  தருக்க subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               தருக்க செருகு_catch_all) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out))) return_trace (பொய்);

    out->version.major = version.major;
    out->version.minor = version.minor;

    எனில் (செருகு_catch_all)
    {
      க்கு (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<மாறிலி வெறுமை*, மாறிலி வெறுமை*> *p;
        எனில் (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (பொய்);
        auto *o = out->substitutions.serialize_append (c->serializer);
        எனில் (!o->serialize (l, feature_index,
                           reinterpret_cast<மாறிலி Feature*> (p->first),
                           reinterpret_cast<மாறிலி Tag*> (p->second)))
          return_trace (பொய்);
      }
      return_trace (உண்மை);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), இது))
    ;

    return_trace (தருக்க (out->substitutions));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி FeatureList *feature_list) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, இது, feature_list));
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  பொது:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

கட்டமைப்பு FeatureVariationRecord
{
  friend கட்டமைப்பு FeatureVariations;

  வெறுமை collect_lookups (மாறிலி வெறுமை     *base,
			மாறிலி hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) மாறிலி
  {
    திரும்பு (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  வெறுமை collect_name_ids (மாறிலி வெறுமை *base,
                         மாறிலி hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) மாறிலி
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  வெறுமை closure_features (மாறிலி வெறுமை     *base,
			 மாறிலி hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) மாறிலி
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  தருக்க intersects_features (மாறிலி வெறுமை *base, மாறிலி hb_map_t *feature_index_map) மாறிலி
  {
    திரும்பு (base+substitutions).intersects_features (feature_index_map);
  }

  வெறுமை collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    மாறிலி வெறுமை *base) மாறிலி
  {
    (base+conditions).keep_with_variations (c);
    எனில் (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = உண்மை; // set variations only once
    }
  }

  தருக்க subset (hb_subset_layout_context_t *c, மாறிலி வெறுமை *base,
               தருக்க செருகு_catch_all = பொய்) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->subset_context->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    எனில் (!செருகு_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    இல்லையெனில்
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, செருகு_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, செருகு_catch_all);

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base,
		 மாறிலி FeatureList *feature_list) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  மாறிலி வெறுமை *base) மாறிலி
  { (base+conditions).collect_variation_indices (c); }

  பாதுகாக்கப்பட்ட:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு LookupIndexList
{
  வெறுமை add_lookup_indexes_to (hb_set_t *lookup_indexes) மாறிலி
  { lookupIndices.add_indexes_to (lookup_indexes); }

  தருக்க intersects_lookup_indexes (மாறிலி hb_map_t *lookup_indexes) மாறிலி
  { திரும்பு lookupIndices.intersects (lookup_indexes); }

  தருக்க subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (பொய்);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (தருக்க (out->lookupIndices));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (lookupIndices.sanitize (c));
  }

  பாதுகாக்கப்பட்ட:
  IndexArray	lookupIndices;
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

கட்டமைப்பு LookupConditionRecord
{
  unsigned get_condition_offset (மாறிலி வெறுமை *base, மாறிலி வெறுமை *variations) மாறிலி
  { திரும்பு (மாறிலி எழுத்து *) &(base+condition) - (மாறிலி எழுத்து *) variations; }

  template <typename Instancer>
  தருக்க evaluate (மாறிலி வெறுமை *base,
		 மாறிலி முழு *coords,
		 unsigned முழு coord_len,
		 Instancer *instancer) மாறிலி
  { திரும்பு (base+condition).evaluate (coords, coord_len, instancer); }

  தருக்க evaluate (மாறிலி வெறுமை *base,
                 மாறிலி வெறுமை *variations,
                 hb_sorted_array_t<மாறிலி unsigned> conditions) மாறிலி
  {
    திரும்பு !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     அளவு (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  தருக்க collect_condition (மாறிலி வெறுமை *base,
                          மாறிலி வெறுமை *variations,
                          மாறிலி முழு *coords,
                          unsigned முழு coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) மாறிலி
  {
    /* Null conditions always apply and need no key state. */
    எனில் (!condition) திரும்பு உண்மை;

    unsigned offset = get_condition_offset (base, variations);
    எனில் (visited->has (offset)) திரும்பு உண்மை;
    visited->add (offset);
    எனில் (unlikely (visited->in_error ())) திரும்பு பொய்;

    எனில் (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    திரும்பு !conditions->in_error ();
  }

  வெறுமை add_lookup_indexes_to (மாறிலி வெறுமை *base,
			      hb_set_t *lookup_indexes) மாறிலி
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  தருக்க intersects_lookup_indexes (மாறிலி வெறுமை *base,
				   மாறிலி hb_map_t *lookup_indexes) மாறிலி
  { திரும்பு (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  மாறிலி வெறுமை *base) மாறிலி
  {
    எனில் (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_மதிப்புகள்)))
      c->layout_variation_indices->err ();
  }

  தருக்க subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    உண்மை);
    எனில் (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->எடு_discard ();
      return_trace (பொய்);
    }
    c->serializer->add_link (out->condition, c->serializer->எடு_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  பாதுகாக்கப்பட்ட:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு FeatureLookupsTable
{
  தருக்க adds_default_lookups () மாறிலி { திரும்பு flags & 0x0001u; }

  தருக்க collect_conditions (மாறிலி வெறுமை *variations,
                           மாறிலி முழு *coords,
                           unsigned முழு coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) மாறிலி
  {
    க்கு (மாறிலி LookupConditionRecord &record : records)
      எனில் (unlikely (!record.collect_condition (இது, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  வெறுமை collect_lookups (மாறிலி வெறுமை *variations,
                       hb_sorted_array_t<மாறிலி unsigned> conditions,
                       hb_set_t *lookup_indexes) மாறிலி
  {
    க்கு (மாறிலி LookupConditionRecord &record : records)
      எனில் (record.evaluate (இது, variations, conditions))
	record.add_lookup_indexes_to (இது, lookup_indexes);
  }

  template <typename Instancer>
  வெறுமை collect_lookups (மாறிலி முழு *coords,
			unsigned முழு coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) மாறிலி
  {
    க்கு (மாறிலி LookupConditionRecord &record : records)
      எனில் (record.evaluate (இது, coords, coord_len, instancer))
	record.add_lookup_indexes_to (இது, lookup_indexes);
  }

  வெறுமை collect_lookups (hb_set_t *lookup_indexes) மாறிலி
  {
    க்கு (மாறிலி LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (இது, lookup_indexes);
  }

  தருக்க intersects_lookup_indexes (மாறிலி hb_map_t *lookup_indexes) மாறிலி
  {
    க்கு (மாறிலி LookupConditionRecord &record : records)
      எனில் (record.intersects_lookup_indexes (இது, lookup_indexes))
	திரும்பு உண்மை;
    திரும்பு பொய்;
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி LookupConditionRecord &record : records)
      record.collect_variation_indices (c, இது);
  }

  தருக்க subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (பொய்);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    க்கு (மாறிலி LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      எனில் (record.subset (c, l, இது))
	out->records.len++;
      இல்லையெனில்
	c->serializer->revert (snap);
    }

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  பொது:
  DEFINE_SIZE_ARRAY (10, records);
};

கட்டமைப்பு LookupVariationRecord
{
  friend கட்டமைப்பு FeatureVariations;

  unsigned get_feature_index () மாறிலி { திரும்பு featureIndex; }

  வெறுமை collect_lookups (மாறிலி வெறுமை *base,
                       மாறிலி Feature &feature,
                       hb_sorted_array_t<மாறிலி unsigned> conditions,
                       hb_set_t *lookup_indexes) மாறிலி
  {
    மாறிலி FeatureLookupsTable &feature_lookups = base+featureLookups;
    எனில் (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  வெறுமை collect_lookups (மாறிலி வெறுமை *base,
			மாறிலி hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) மாறிலி
  {
    எனில் (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  வெறுமை closure_features (மாறிலி வெறுமை *base,
			 மாறிலி hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) மாறிலி
  {
    எனில் ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  மாறிலி வெறுமை *base) மாறிலி
  { (base+featureLookups).collect_variation_indices (c); }

  தருக்க subset (hb_subset_layout_context_t *l, மாறிலி வெறுமை *base) மாறிலி
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (இது);
    uint32_t *new_feature_index;
    எனில் (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (பொய்);

    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (பொய்);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  featureLookups.sanitize (c, base));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு FeatureVariations
{
  நிலையான constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  மாறிலி Array32Of<LookupVariationRecord> &get_lookup_variation_records () மாறிலி
  {
    எனில் (version.to_int () < 0x00010001u)
      திரும்பு Null (Array32Of<LookupVariationRecord>);
    திரும்பு StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_மொத்தம் () மாறிலி
  {
    திரும்பு varRecords.len;
  }

  தருக்க find_index (மாறிலி முழு *coords, unsigned முழு coord_len,
		   unsigned முழு *index,
		   ItemVarStoreInstancer *instancer) மாறிலி
  {
    unsigned முழு மொத்தம் = varRecords.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      மாறிலி FeatureVariationRecord &record = varRecords.arrayZ[i];
      எனில் ((இது+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	திரும்பு உண்மை;
      }
    }
    *index = NOT_FOUND_INDEX;
    திரும்பு பொய்;
  }

  மாறிலி Feature *find_substitute (unsigned முழு variations_index,
				  unsigned முழு feature_index) மாறிலி
  {
    மாறிலி FeatureVariationRecord &record = varRecords[variations_index];
    திரும்பு (இது+record.substitutions).find_substitute (feature_index);
  }

  தருக்க collect_lookup_conditions (மாறிலி முழு *coords,
                                  unsigned முழு coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) மாறிலி
  {
    hb_set_t visited_tables, visited_conditions;
    க்கு (மாறிலி LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      எனில் (visited_tables.has (offset)) தொடர்;
      visited_tables.add (offset);
      எனில் (unlikely (visited_tables.in_error ())) திரும்பு பொய்;

      மாறிலி FeatureLookupsTable &feature_lookups = இது+record.featureLookups;
      எனில் (unlikely (!feature_lookups.collect_conditions (இது, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	திரும்பு பொய்;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    திரும்பு உண்மை;
  }

  தருக்க get_lookup_variations (unsigned feature_index,
                              மாறிலி Feature &feature,
                              hb_sorted_array_t<மாறிலி unsigned> conditions,
                              hb_set_t *lookup_indexes) மாறிலி
  {
    க்கு (மாறிலி LookupVariationRecord &record : get_lookup_variation_records ())
    {
      எனில் (record.featureIndex != feature_index) தொடர்;

      record.collect_lookups (இது, feature, conditions, lookup_indexes);
      திரும்பு உண்மை;
    }
    திரும்பு பொய்;
  }

  வெறுமை collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) மாறிலி
  {
    unsigned முழு மொத்தம் = varRecords.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, இது);
      எனில் (c->universal)
        நிறுத்து;
    }
    எனில் (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed (*இது));
  }

  வெறுமை collect_lookups (மாறிலி hb_set_t *feature_indexes,
			மாறிலி hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) மாறிலி
  {
    unsigned மொத்தம் = varRecords.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        தொடர்;
      varRecords[i].collect_lookups (இது, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  வெறுமை collect_name_ids (மாறிலி hb_map_t *feature_index_map,
                         மாறிலி hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) மாறிலி
  {
    க்கு (unsigned i = 0; i < varRecords.len; i++)
    {
      எனில் (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        தொடர்;
      varRecords[i].collect_name_ids (இது, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  வெறுமை collect_lookup_variation_lookups (மாறிலி hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) மாறிலி
  {
    க்கு (மாறிலி LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (இது, feature_indexes, lookup_indexes);
  }

  வெறுமை closure_features (மாறிலி hb_map_t *lookup_indexes,
			 மாறிலி hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) மாறிலி
  {
    unsigned முழு மொத்தம் = varRecords.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        தொடர்;
      varRecords[i].closure_features (இது, lookup_indexes, feature_indexes);
    }

    க்கு (மாறிலி LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (இது, lookup_indexes, feature_indexes);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (மாறிலி FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, இது);
    க்கு (மாறிலி LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, இது);
  }

  வெறுமை collect_feature_indices (hb_set_t *feature_indices) மாறிலி
  {
    க்கு (மாறிலி FeatureVariationRecord &record : varRecords)
      (இது+record.substitutions).collect_feature_indices (feature_indices);
  }

  தருக்க has_lookup_variations (unsigned feature_index) மாறிலி
  {
    க்கு (மாறிலி LookupVariationRecord &record : get_lookup_variation_records ())
    {
      எனில் (record.featureIndex == feature_index)
	திரும்பு உண்மை;
      எனில் (record.featureIndex > feature_index)
	நிறுத்து;
    }
    திரும்பு பொய்;
  }

  தருக்க subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out))) return_trace (பொய்);

    out->version.major = version.major;
    out->version.minor = version.minor;

    முழு keep_up_to = -1;
    க்கு (முழு i = varRecords.len - 1; i >= 0; i--) {
      எனில் (varRecords[i].intersects_features (இது, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        நிறுத்து;
      }
    }

    unsigned மொத்தம் = (unsigned) (keep_up_to + 1);
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      எனில் (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        தொடர்;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), இது) (varRecords[i]);
    }

    எனில் (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      தருக்க செருகு_catch_all_record = உண்மை;
      subset_record_array (l, &(out->varRecords), இது, செருகு_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    எனில் (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      எனில் (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (பொய்);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, இது))
      ;
    }

    return_trace (தருக்க (out->varRecords) ||
		  (out_lookup_records && தருக்க (*out_lookup_records)));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி FeatureList *feature_list) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, இது, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, இது)));
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  பொது:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

கட்டமைப்பு HintingDevice
{
  friend கட்டமைப்பு Device;

  தனிப்பட்ட:

  hb_position_t get_x_delta (hb_font_t *font) மாறிலி
  { திரும்பு get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) மாறிலி
  { திரும்பு get_delta (font->y_ppem, font->y_scale); }

  பொது:

  size_t get_size () மாறிலி
  {
    unsigned முழு f = deltaFormat;
    எனில் (unlikely (f < 1 || f > 3 || startSize > endSize)) திரும்பு 3 * HBUINT16::static_size;
    திரும்பு HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && c->check_range (இது, இது->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed<HintingDevice> (இது));
  }

  தனிப்பட்ட:

  முழு get_delta (unsigned முழு ppem, முழு scale) மாறிலி
  {
    எனில் (!ppem) திரும்பு 0;

    முழு pixels = get_delta_pixels (ppem);

    எனில் (!pixels) திரும்பு 0;

    திரும்பு (முழு) (pixels * (int64_t) scale / ppem);
  }
  முழு get_delta_pixels (unsigned முழு ppem_size) மாறிலி
  {
    unsigned முழு f = deltaFormat;
    எனில் (unlikely (f < 1 || f > 3))
      திரும்பு 0;

    எனில் (ppem_size < startSize || ppem_size > endSize)
      திரும்பு 0;

    unsigned முழு s = ppem_size - startSize;

    unsigned முழு byte = delta_மதிப்பு_Z[s >> (4 - f)];
    unsigned முழு bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned முழு mask = (0xFFFFu >> (16 - (1 << f)));

    முழு delta = bits & mask;

    எனில் ((unsigned முழு) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    திரும்பு delta;
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_மதிப்பு_Z;		/* Array of compressed data */
  பொது:
  DEFINE_SIZE_ARRAY (6, delta_மதிப்பு_Z);
};

கட்டமைப்பு VariationDevice
{
  friend கட்டமைப்பு Device;

  தனிப்பட்ட:

  hb_position_t get_x_delta (hb_font_t *font,
			     மாறிலி ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  { திரும்பு !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     மாறிலி ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  { திரும்பு !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  தசம get_x_delta_f (hb_font_t *font,
		       மாறிலி ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) மாறிலி
  { திரும்பு !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  தசம get_y_delta_f (hb_font_t *font,
		       மாறிலி ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) மாறிலி
  { திரும்பு !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    எனில் (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, முழு> *v;
    எனில் (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    எனில் (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (இது->static_size);
    auto *out = c->embed (இது);
    எனில் (unlikely (!out)) return_trace (nullptr);

    எனில் (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  வெறுமை collect_variation_index (hb_collect_variation_indices_context_t *c) மாறிலி
  { c->layout_variation_indices->add (varIdx); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  தனிப்பட்ட:

  தசம get_delta (hb_font_t *font,
		   மாறிலி ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
    திரும்பு store.get_delta (varIdx, font->coords, font->இலக்கம்_coords, store_cache);
  }

  பாதுகாக்கப்பட்ட:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு DeviceHeader
{
  பாதுகாக்கப்பட்ட:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  பொது:
  HBUINT16		format;		/* Format identifier */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  தசம get_x_delta_f (hb_font_t *font,
		       மாறிலி ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
#ifndef HB_NO_VAR
    எனில் (u.b.format == 0x8000)
    {
      hb_barrier ();
      திரும்பு u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    திரும்பு get_x_delta (font, store, store_cache);
  }

  தசம get_y_delta_f (hb_font_t *font,
		       மாறிலி ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
#ifndef HB_NO_VAR
    எனில் (u.b.format == 0x8000)
    {
      hb_barrier ();
      திரும்பு u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    திரும்பு get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     மாறிலி ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
    தேர்வு (u.b.format)
    {
#ifndef HB_NO_HINTING
    நிலை 1: நிலை 2: நிலை 3:
      hb_barrier ();
      திரும்பு u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    நிலை 0x8000:
      hb_barrier ();
      திரும்பு u.variation.get_x_delta (font, store, store_cache);
#endif
    இயல்பு:
      திரும்பு 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     மாறிலி ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
    தேர்வு (u.b.format)
    {
    நிலை 1: நிலை 2: நிலை 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      திரும்பு u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    நிலை 0x8000:
      hb_barrier ();
      திரும்பு u.variation.get_y_delta (font, store, store_cache);
#endif
    இயல்பு:
      திரும்பு 0;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.b.format.sanitize (c)) return_trace (பொய்);
    தேர்வு (u.b.format) {
#ifndef HB_NO_HINTING
    நிலை 1: நிலை 2: நிலை 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    நிலை 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    இயல்பு:
      return_trace (உண்மை);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map=nullptr) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    தேர்வு (u.b.format) {
#ifndef HB_NO_HINTING
    நிலை 1:
    நிலை 2:
    நிலை 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    நிலை 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    இயல்பு:
      return_trace (nullptr);
    }
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    தேர்வு (u.b.format) {
#ifndef HB_NO_HINTING
    நிலை 1:
    நிலை 2:
    நிலை 3:
      திரும்பு;
#endif
#ifndef HB_NO_VAR
    நிலை 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      திரும்பு;
#endif
    இயல்பு:
      திரும்பு;
    }
  }

  unsigned get_variation_index () மாறிலி
  {
    தேர்வு (u.b.format) {
#ifndef HB_NO_VAR
    நிலை 0x8000:
      hb_barrier ();
      திரும்பு u.variation.varIdx;
#endif
    இயல்பு:
      திரும்பு HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  தருக்க is_variation_device () மாறிலி
  {
    தேர்வு (u.b.format) {
#ifndef HB_NO_VAR
    நிலை 0x8000:
      திரும்பு உண்மை;
#endif
    இயல்பு:
      திரும்பு பொய்;
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  பொது:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

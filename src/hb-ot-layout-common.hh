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

#شامل "hb.hh"
#شامل "hb-ot-layout.hh"
#شامل "hb-open-type.hh"
#شامل "hb-set.hh"
#شامل "hb-bimap.hh"
#شامل "hb-cache.hh"
#شامل "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (اندازه (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_دودویی_cache_t = hb_cache_t<14, 1, 8>;
static_assert (اندازه (hb_ot_layout_دودویی_cache_t) == 256, "");

#شامل "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


فضاینام OT {

template<typename Iterator>
ایستا درونخطی منطقی ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ایستا منطقی ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ثابت hb_set_t &klasses,
    منطقی use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

ساختار ItemVarStoreInstancer;

ساختار hb_collect_feature_substitutes_with_var_context_t
{
  ثابت hb_map_t *axes_index_tag_map;
  ثابت hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, ثابت Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  منطقی apply;
  منطقی variation_applied;
  منطقی universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  منطقی all_axes_pinned;
};

ساختار hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ثابت تهی         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              ثابت hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_شمار (0),langsys_feature_شمار (0) {}

  منطقی visitScript ()
  { بازگشت script_شمار++ < HB_MAX_SCRIPTS; }

  منطقی visitLangsys (unsigned feature_شمار_2)
  {
    langsys_feature_شمار += feature_شمار_2;
    بازگشت langsys_feature_شمار < HB_MAX_LANGSYS_FEATURE_شمار;
  }

  عمومی:
  ثابت تهی *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ثابت hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  خصوصی:
  unsigned script_شمار;
  unsigned langsys_feature_شمار;
};

ساختار hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  ثابت نویسه *get_name () { بازگشت "SUBSET_LAYOUT"; }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }

  منطقی visitScript ()
  {
    بازگشت script_شمار++ < HB_MAX_SCRIPTS;
  }

  منطقی visitLangSys ()
  {
    بازگشت langsys_شمار++ < HB_MAX_LANGSYS;
  }

  منطقی visitFeatureIndex (صحیح شمار)
  {
    feature_index_شمار += شمار;
    بازگشت feature_index_شمار < HB_MAX_FEATURE_INDICES;
  }

  منطقی visitLookupIndex()
  {
    lookup_index_شمار++;
    بازگشت lookup_index_شمار < HB_MAX_LOOKUP_VISIT_شمار;
  }

  hb_subset_context_t *subset_context;
  ثابت hb_tag_t table_tag;
  ثابت hb_map_t *lookup_index_map;
  ثابت hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ثابت hb_map_t *feature_index_map;
  ثابت hb_map_t *feature_map_w_duplicates;
  ثابت hb_hashmap_t<unsigned, ثابت Feature*> *feature_substitutes_map;
  ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  ثابت hb_set_t *catch_all_record_feature_idxes;
  ثابت hb_hashmap_t<unsigned, hb_pair_t<ثابت تهی*, ثابت تهی*>> *feature_idx_tag_map;

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
				script_شمار (0),
				langsys_شمار (0),
				feature_index_شمار (0),
				lookup_index_شمار (0)
  {
    اگر (tag_ == HB_OT_TAG_GSUB)
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
    وگرنه
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

  خصوصی:
  unsigned script_شمار;
  unsigned langsys_شمار;
  unsigned feature_index_شمار;
  unsigned lookup_index_شمار;
};

ساختار ItemVariationStore;
ساختار hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.collect_variation_indices (این); بازگشت hb_empty_t (); }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  ثابت hb_set_t *glyph_set;
  ثابت hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, صحیح>> *condition_مقادیر = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  ثابت hb_set_t *glyph_set_,
					  ثابت hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
ساختار subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 ثابت تهی *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  منطقی operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    اگر (unlikely (!o)) بازگشت نادرست;
    منطقی ret = o->serialize_subset (subset_context, offset, base);
    اگر (!ret)
    {
      out.برداشت ();
      subset_context->serializer->revert (snap);
    }
    بازگشت ret;
  }

  خصوصی:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ثابت تهی *base;
};


template<typename OutputArray, typename Arg>
ساختار subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     ثابت تهی *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  منطقی operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    اگر (unlikely (!o)) بازگشت نادرست;
    منطقی ret = o->serialize_subset (subset_context, offset, base, arg);
    اگر (!ret)
    {
      out.برداشت ();
      subset_context->serializer->revert (snap);
    }
    بازگشت ret;
  }

  خصوصی:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ثابت تهی *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
ساختار
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ثابت تهی *base) ثابت
  { بازگشت subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ثابت تهی *base, Arg &&arg) ثابت
  { بازگشت subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
ساختار subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 ثابت تهی *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  تهی
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقی ret = record.subset (subset_layout_context, base);
    اگر (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وگرنه out->len++;
  }

  خصوصی:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ثابت تهی *base;
};

template<typename OutputArray, typename Arg>
ساختار subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     ثابت تهی *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  تهی
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقی ret = record.subset (subset_layout_context, base, arg);
    اگر (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وگرنه out->len++;
  }

  خصوصی:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ثابت تهی *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
ساختار
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       ثابت تهی *base) ثابت
  { بازگشت subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               ثابت تهی *base, Arg &&arg) ثابت
  { بازگشت subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
ساختار serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         ثابت تهی *base_,
                         ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  منطقی operator () (T&& record)
  {
    اگر (!serialize_context->copy (record, base, variation_map)) بازگشت نادرست;
    out.len++;
    بازگشت درست;
  }

  خصوصی:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  ثابت تهی *base;
  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
ساختار
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               ثابت تهی *base,
               ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *variation_map) ثابت
  { بازگشت serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

ساختار IndexArray : Array16Of<Index>
{
  منطقی intersects (ثابت hb_map_t *indexes) ثابت
  { بازگشت hb_any (*این, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  تهی serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    اگر (!it) بازگشت;
    اگر (unlikely (!c->extend_min ((*این)))) بازگشت;

    برای (ثابت auto _ : it)
    {
      اگر (!l->visitLookupIndex()) بشکن;

      Index i;
      i = _;
      c->copy (i);
      این->len++;
    }
  }

  unsigned صحیح get_indexes (unsigned صحیح start_offset,
			    unsigned صحیح *شمار_2 /* IN/OUT */,
			    unsigned صحیح *_indexes /* OUT */) ثابت
  {
    اگر (شمار_2 && _indexes)
    {
      + این->as_array ().sub_array (start_offset, شمار_2)
      | hb_sink (hb_array (_indexes, *شمار_2))
      ;
    }
    بازگشت این->len;
  }

  تهی add_indexes_to (hb_set_t* output /* OUT */) ثابت
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
ساختار FeatureParamsSize
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این))) return_trace (نادرست);
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

    اگر (!designSize)
      return_trace (نادرست);
    وگرنه اگر (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (درست);
    وگرنه اگر (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (نادرست);
    وگرنه
      return_trace (درست);
  }

  تهی collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ثابت
  { nameids_to_retain->add (subfamilyNameID); }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    return_trace ((منطقی) c->serializer->embed (*این));
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
  عمومی:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
ساختار FeatureParamsStylisticSet
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (این));
  }

  تهی collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ثابت
  { nameids_to_retain->add (uiNameID); }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    return_trace ((منطقی) c->serializer->embed (*این));
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
  عمومی:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
ساختار FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_شمار, hb_codepoint_t *chars) ثابت
  {
    اگر (char_شمار && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_شمار)
      | hb_sink (hb_array (chars, *char_شمار))
      ;
    }
    بازگشت characters.len;
  }

  size_t get_size () ثابت
  { بازگشت hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  تهی collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    اگر (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    اگر (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    اگر (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    اگر (!firstParamUILabelNameID || !رقم_Named_Parameters || رقم_Named_Parameters >= 0x7FFF)
      بازگشت;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) رقم_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    return_trace ((منطقی) c->serializer->embed (*این));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
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
  HBUINT16	رقم_Named_Parameters;	/* Number of named parameters. (May
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
  عمومی:
  DEFINE_SIZE_ARRAY (14, characters);
};

ساختار FeatureParams
{
  منطقی sanitize (hb_sanitize_context_t *c, hb_tag_t tag) ثابت
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    بازگشت درست;
#endif
    TRACE_SANITIZE (این);
    اگر (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    اگر ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    اگر ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (درست);
  }

  تهی collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    بازگشت;
#endif
    اگر (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      بازگشت (u.size.collect_name_ids (nameids_to_retain));
    اگر ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      بازگشت (u.stylisticSet.collect_name_ids (nameids_to_retain));
    اگر ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      بازگشت (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  منطقی subset (hb_subset_context_t *c, ثابت Tag* tag) ثابت
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    بازگشت نادرست;
#endif
    TRACE_SUBSET (این);
    اگر (!tag) return_trace (نادرست);
    اگر (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    اگر ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    اگر ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (نادرست);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  ثابت FeatureParamsSize& get_size_params (hb_tag_t tag) ثابت
  {
    اگر (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      بازگشت u.size;
    بازگشت Null (FeatureParamsSize);
  }
  ثابت FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) ثابت
  {
    اگر ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      بازگشت u.stylisticSet;
    بازگشت Null (FeatureParamsStylisticSet);
  }
  ثابت FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) ثابت
  {
    اگر ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      بازگشت u.characterVariants;
    بازگشت Null (FeatureParamsCharacterVariants);
  }
#endif

  خصوصی:
  اجتماع {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  عمومی:
  DEFINE_SIZE_MIN (0);
};

ساختار Record_sanitize_closure_t {
  hb_tag_t tag;
  ثابت تهی *list_base;
};

ساختار Feature
{
  unsigned صحیح get_lookup_شمار () ثابت
  { بازگشت lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned صحیح i) ثابت
  { بازگشت lookupIndex[i]; }
  unsigned صحیح get_lookup_indexes (unsigned صحیح start_index,
				   unsigned صحیح *lookup_شمار /* IN/OUT */,
				   unsigned صحیح *lookup_tags /* OUT */) ثابت
  { بازگشت lookupIndex.get_indexes (start_index, lookup_شمار, lookup_tags); }
  تهی add_lookup_indexes_to (hb_set_t *lookup_indexes) ثابت
  { lookupIndex.add_indexes_to (lookup_indexes); }

  ثابت FeatureParams &get_feature_params () ثابت
  { بازگشت این+featureParams; }

  منطقی intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { بازگشت lookupIndex.intersects (lookup_indexes); }

  تهی collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    اگر (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  منطقی subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت Tag                   *tag = nullptr) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    out->featureParams.serialize_subset (c, featureParams, این, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    اگر (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    وگرنه
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
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t *closure = nullptr) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  featureParams.sanitize (c, این, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
ساختار Record
{
  صحیح cmp (hb_tag_t a) ثابت { بازگشت tag.cmp (a); }

  منطقی subset (hb_subset_layout_context_t *c, ثابت تهی *base, ثابت تهی *f_sub = nullptr) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->subset_context->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    ثابت Feature& f = *reinterpret_cast<ثابت Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    منطقی ret = f.subset (c->subset_context, c, &tag);
    اگر (ret)
      s->add_link (out->offset, s->برداشت_pack ());
    وگرنه
      s->برداشت_discard ();

    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    ثابت Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (این) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
ساختار RecordArrayOf : SortedArray16Of<Record<Type>>
{
  ثابت Offset16To<Type>& get_offset (unsigned صحیح i) ثابت
  { بازگشت (*این)[i].offset; }
  Offset16To<Type>& get_offset (unsigned صحیح i)
  { بازگشت (*این)[i].offset; }
  ثابت Tag& get_tag (unsigned صحیح i) ثابت
  { بازگشت (*این)[i].tag; }
  unsigned صحیح get_tags (unsigned صحیح start_offset,
                         unsigned صحیح *record_شمار /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) ثابت
  {
    اگر (record_شمار && record_tags)
    {
      + این->as_array ().sub_array (start_offset, record_شمار)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_شمار))
      ;
    }
    بازگشت این->len;
  }
  منطقی find_index (hb_tag_t tag, unsigned صحیح *index) ثابت
  {
    بازگشت این->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
ساختار RecordListOf : RecordArrayOf<Type>
{
  ثابت Type& operator [] (unsigned صحیح i) ثابت
  { بازگشت این+این->get_offset (i); }

  منطقی subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    + این->iter ()
    | hb_apply (subset_record_array (l, out, این))
    ;
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (RecordArrayOf<Type>::sanitize (c, این));
  }
};

ساختار RecordListOfFeature : RecordListOf<Feature>
{
  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    + hb_enumerate (*این)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, این] (ثابت hb_pair_t<unsigned, ثابت Record<Feature>&>& _)
                {
                  ثابت Feature *f_sub = nullptr;
                  ثابت Feature **f = nullptr;
                  اگر (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, این, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (درست);
  }
};

تعریفنوع RecordListOf<Feature> FeatureList;


ساختار LangSys
{
  unsigned صحیح get_feature_شمار () ثابت
  { بازگشت featureIndex.len; }
  hb_tag_t get_feature_index (unsigned صحیح i) ثابت
  { بازگشت featureIndex[i]; }
  unsigned صحیح get_feature_indexes (unsigned صحیح start_offset,
				    unsigned صحیح *feature_شمار_2 /* IN/OUT */,
				    unsigned صحیح *feature_indexes /* OUT */) ثابت
  { بازگشت featureIndex.get_indexes (start_offset, feature_شمار_2, feature_indexes); }
  تهی add_feature_indexes_to (hb_set_t *feature_indexes) ثابت
  { featureIndex.add_indexes_to (feature_indexes); }

  منطقی has_required_feature () ثابت { بازگشت reqFeatureIndex != 0xFFFFu; }
  unsigned صحیح get_required_feature_index () ثابت
  {
    اگر (reqFeatureIndex == 0xFFFFu)
      بازگشت Index::NOT_FOUND_INDEX;
   بازگشت reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed (*این));
  }

  منطقی compare (ثابت LangSys& o, ثابت hb_map_t *feature_index_map) ثابت
  {
    اگر (reqFeatureIndex != o.reqFeatureIndex)
      بازگشت نادرست;

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

    برای (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      اگر (a != b) بازگشت نادرست;
    }

    اگر (iter || o_iter) بازگشت نادرست;

    بازگشت درست;
  }

  تهی collect_features (hb_prune_langsys_context_t *c) ثابت
  {
    اگر (!has_required_feature () && !get_feature_شمار ()) بازگشت;
    اگر (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  منطقی subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ثابت Tag                  *tag = nullptr) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    ثابت uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    اگر (!l->visitFeatureIndex (featureIndex.len))
      return_trace (نادرست);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    منطقی ret = منطقی (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t * = nullptr) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

ساختار Script
{
  unsigned صحیح get_lang_sys_شمار () ثابت
  { بازگشت langSys.len; }
  ثابت Tag& get_lang_sys_tag (unsigned صحیح i) ثابت
  { بازگشت langSys.get_tag (i); }
  unsigned صحیح get_lang_sys_tags (unsigned صحیح start_offset,
				  unsigned صحیح *lang_sys_شمار /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ثابت
  { بازگشت langSys.get_tags (start_offset, lang_sys_شمار, lang_sys_tags); }
  ثابت LangSys& get_lang_sys (unsigned صحیح i) ثابت
  {
    اگر (i == Index::NOT_FOUND_INDEX) بازگشت get_default_lang_sys ();
    بازگشت این+langSys[i].offset;
  }
  منطقی find_lang_sys_index (hb_tag_t tag, unsigned صحیح *index) ثابت
  { بازگشت langSys.find_index (tag, index); }

  منطقی has_default_lang_sys () ثابت           { بازگشت defaultLangSys != 0; }
  ثابت LangSys& get_default_lang_sys () ثابت { بازگشت این+defaultLangSys; }

  تهی prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) ثابت
  {
    اگر (!has_default_lang_sys () && !get_lang_sys_شمار ()) بازگشت;
    اگر (!c->visitScript ()) بازگشت;

    اگر (!c->script_langsys_map->has (script_index))
    {
      اگر (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	بازگشت;
    }

    اگر (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ثابت LangSys& d = get_default_lang_sys ();
      اگر (c->visitLangsys (d.get_feature_شمار ())) {
        d.collect_features (c);
      }

      برای (auto _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = این+_.second.offset;
        اگر (!c->visitLangsys (l.get_feature_شمار ())) ادامه;
        اگر (l.compare (d, c->duplicate_feature_map)) ادامه;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    وگرنه
    {
      برای (auto _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = این+_.second.offset;
        اگر (!c->visitLangsys (l.get_feature_شمار ())) ادامه;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  منطقی subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت Tag                   *tag) ثابت
  {
    TRACE_SUBSET (این);
    اگر (!l->visitScript ()) return_trace (نادرست);
    اگر (tag && !c->plan->layout_scripts.has (*tag))
      بازگشت نادرست;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    منطقی defaultLang = نادرست;
    اگر (has_default_lang_sys ())
    {
      c->serializer->push ();
      ثابت LangSys& ls = این+defaultLangSys;
      منطقی ret = ls.subset (c, l);
      اگر (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->برداشت_discard ();
	out->defaultLangSys = 0;
      }
      وگرنه
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->برداشت_pack ());
	defaultLang = درست;
      }
    }

    ثابت hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    اگر (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (ثابت Record<LangSys>& record) {بازگشت l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), این))
      ;
    }

    return_trace (منطقی (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t * = nullptr) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (defaultLangSys.sanitize (c, این) && langSys.sanitize (c, این));
  }

  محافظتشده:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

ساختار RecordListOfScript : RecordListOf<Script>
{
  منطقی subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    برای (auto _ : + hb_enumerate (*این))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      منطقی ret = _.second.subset (l, این);
      اگر (!ret) c->serializer->revert (snap);
      وگرنه out->len++;
    }

    return_trace (درست);
  }
};

تعریفنوع RecordListOfScript ScriptList;



ساختار LookupFlag : HBUINT16
{
  شمارش Flags {
    راست_To_چپ		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  عمومی:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
فضاینام OT {

ساختار Lookup
{
  unsigned صحیح get_subtable_شمار () ثابت { بازگشت subTable.len; }

  template <typename TSubTable>
  ثابت Array16OfOffset16To<TSubTable>& get_subtables () ثابت
  { بازگشت reinterpret_cast<ثابت Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { بازگشت reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  ثابت TSubTable& get_subtable (unsigned صحیح i) ثابت
  { بازگشت این+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned صحیح i)
  { بازگشت این+get_subtables<TSubTable> ()[i]; }

  size_t get_size () ثابت
  {
    ثابت HBUINT16 &markFilteringSet = StructAfter<ثابت HBUINT16> (subTable);
    اگر (lookupFlag & LookupFlag::UseMarkFilteringSet)
      بازگشت (ثابت نویسه *) &StructAfter<ثابت نویسه> (markFilteringSet) - (ثابت نویسه *) این;
    بازگشت (ثابت نویسه *) &markFilteringSet - (ثابت نویسه *) این;
  }

  unsigned صحیح get_type () ثابت { بازگشت lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ثابت
  {
    unsigned صحیح flag = lookupFlag;
    اگر (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      ثابت HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    بازگشت flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحیح lookup_type = get_type ();
    TRACE_DISPATCH (این, lookup_type);
    unsigned صحیح شمار = get_subtable_شمار ();
    برای (unsigned صحیح i = 0; i < شمار; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      اگر (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_مقدار ());
  }

  منطقی serialize (hb_serialize_context_t *c,
		  unsigned صحیح lookup_type,
		  uint32_t lookup_props,
		  unsigned صحیح رقم_subtables)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    اگر (unlikely (!subTable.serialize (c, رقم_subtables))) return_trace (نادرست);
    اگر (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      اگر (unlikely (!c->extend (این))) return_trace (نادرست);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (درست);
  }

  template <typename TSubTable>
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    ثابت hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned صحیح lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([این, glyphset, lookup_type] (ثابت Offset16To<TSubTable> &_) { بازگشت (این+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), این, lookup_type))
    ;

    اگر (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ثابت HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      اگر (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      وگرنه
      {
        اگر (unlikely (!c->serializer->extend (out))) return_trace (نادرست);
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
    return_trace (درست);
  }

  template <typename TSubTable>
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(c->check_struct (این) && subTable.sanitize (c))) return_trace (نادرست);
    hb_barrier ();

    unsigned subtables = get_subtable_شمار ();
    اگر (unlikely (!c->visit_subtables (subtables))) return_trace (نادرست);

    اگر (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ثابت HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      اگر (!markFilteringSet.sanitize (c)) return_trace (نادرست);
    }

    اگر (unlikely (!get_subtables<TSubTable> ().sanitize (c, این, get_type ())))
      return_trace (نادرست);

    اگر (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned صحیح type = get_subtable<TSubTable> (0).u.extension.get_type ();
      برای (unsigned صحیح i = 1; i < subtables; i++)
	اگر (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (نادرست);
    }
    return_trace (درست);
  }

  محافظتشده:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  عمومی:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
ساختار LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  منطقی subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    + hb_enumerate (*این)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, این))
    ;
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, این)));
  }
};


/*
 * Coverage Table
 */


ایستا منطقی ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ثابت hb_set_t &klasses,
                                          منطقی use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  اگر (!klass_map)
    بازگشت ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  اگر (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  برای (ثابت unsigned k: klasses)
  {
    اگر (klass_map->has (k)) ادامه;
    klass_map->set (k, idx);
    idx++;
  }


  اگر (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    بازگشت نادرست;

  برای (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  بازگشت ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
ساختار ClassDefFormat1_3
{
  friend ساختار ClassDef;

  خصوصی:
  unsigned صحیح get_class (hb_codepoint_t glyph_id) ثابت
  {
    بازگشت class_مقدار[(unsigned صحیح) (glyph_id - startGlyph)];
  }

  unsigned get_population () ثابت
  {
    بازگشت class_مقدار.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    اگر (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_مقدار.len = 0;
      return_trace (درست);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_شمار = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    اگر (unlikely (!class_مقدار.serialize (c, glyph_شمار))) return_trace (نادرست);
    برای (ثابت hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_مقدار[idx] = gid_klass_pair.second;
    }
    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               منطقی keep_empty_table = درست,
               منطقی use_class_zero = درست,
               ثابت Coverage* glyph_filter = nullptr) ثابت
  {
    TRACE_SUBSET (این);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_مقدار.len;

    برای (ثابت hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      اگر (new_gid == HB_MAP_مقدار_INVALID) ادامه;
      اگر (glyph_filter && !glyph_filter->has(gid)) ادامه;

      unsigned klass = class_مقدار[gid - start];
      اگر (!klass) ادامه;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    اگر (use_class_zero)
    {
      ثابت hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_شمار = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_شمار <= glyph_and_klass.length;
    }
    اگر (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (نادرست);
    return_trace (keep_empty_table || (منطقی) glyph_and_klass);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && class_مقدار.sanitize (c));
  }

  unsigned cost () ثابت { بازگشت 1; }

  template <typename set_t>
  منطقی collect_coverage (set_t *glyphs) ثابت
  {
    unsigned صحیح start = 0;
    unsigned صحیح شمار = class_مقدار.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (class_مقدار[i])
	ادامه;

      اگر (start != i)
	اگر (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  بازگشت نادرست;

      start = i + 1;
    }
    اگر (start != شمار)
      اگر (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + شمار)))
	بازگشت نادرست;

    بازگشت درست;
  }

  template <typename set_t>
  منطقی collect_class (set_t *glyphs, unsigned klass) ثابت
  {
    unsigned صحیح شمار = class_مقدار.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (class_مقدار[i] == klass) glyphs->add (startGlyph + i);
    بازگشت درست;
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_مقدار.len;
    برای (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      اگر (class_مقدار[iter - start]) بازگشت درست;
    بازگشت نادرست;
  }
  منطقی intersects_class (ثابت hb_set_t *glyphs, unsigned klass) ثابت
  {
    unsigned صحیح شمار = class_مقدار.len;
    اگر (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_مقدار_INVALID;
      اگر (!glyphs->next (&g)) بازگشت نادرست;
      اگر (g < startGlyph) بازگشت درست;
      g = startGlyph + شمار - 1;
      اگر (glyphs->next (&g)) بازگشت درست;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ثابت typename Types::HBUINT *arr = class_مقدار.arrayZ;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (arr[i] == klass && glyphs->has (startGlyph + i))
	بازگشت درست;
    بازگشت نادرست;
  }

  تهی intersected_class_glyphs (ثابت hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ثابت
  {
    unsigned شمار = class_مقدار.len;
    اگر (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      برای (uint32_t g = HB_SET_مقدار_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      برای (uint32_t g = startGlyph + شمار - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      بازگشت;
    }

    برای (unsigned i = 0; i < شمار; i++)
      اگر (class_مقدار[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#اگر 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + شمار;
    برای (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      اگر (class_مقدار.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  تهی intersected_classes (ثابت hb_set_t *glyphs, hb_set_t *intersect_classes) ثابت
  {
    اگر (glyphs->is_empty ()) بازگشت;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    اگر (!class_مقدار.len)
    {
      intersect_classes->add (0);
      بازگشت;
    }

    hb_codepoint_t end_glyph = startGlyph + class_مقدار.len - 1;
    اگر (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    برای (ثابت auto& _ : + hb_enumerate (class_مقدار))
    {
      hb_codepoint_t g = startGlyph + _.first;
      اگر (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  محافظتشده:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_مقدار;	/* Array of Class Values--one per GlyphID */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_مقدار);
};

template <typename Types>
ساختار ClassDefFormat2_4
{
  friend ساختار ClassDef;

  خصوصی:
  unsigned صحیح get_class (hb_codepoint_t glyph_id) ثابت
  {
    بازگشت rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () ثابت
  {
    typename Types::large_int ret = 0;
    برای (ثابت auto &r : rangeRecord)
      ret += r.get_population ();
    بازگشت ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    اگر (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (درست);
    }

    unsigned unsorted = نادرست;
    unsigned رقم_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    اگر (unlikely (!record)) return_trace (نادرست);

    برای (ثابت auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      اگر (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	اگر (unlikely (cur_gid < prev_gid))
	  unsorted = درست;

	اگر (unlikely (!record)) بشکن;
	record->last = prev_gid;
	رقم_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    اگر (unlikely (c->in_error ())) return_trace (نادرست);

    اگر (likely (record)) record->last = prev_gid;
    rangeRecord.len = رقم_ranges;

    اگر (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               منطقی keep_empty_table = درست,
               منطقی use_class_zero = درست,
               ثابت Coverage* glyph_filter = nullptr) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    اگر (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      برای (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	اگر (!klass) ادامه;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	اگر (new_gid == HB_MAP_مقدار_INVALID) ادامه;
	اگر (glyph_filter && !glyph_filter->has (g)) ادامه;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    وگرنه
    {
      unsigned رقم_source_glyphs = c->plan->source->get_رقم_glyphs ();
      برای (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	اگر (!klass) ادامه;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, رقم_source_glyphs);
	برای (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  اگر (new_gid == HB_MAP_مقدار_INVALID) ادامه;
	  اگر (glyph_filter && !glyph_filter->has (g)) ادامه;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    ثابت hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_شمار = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_شمار <= glyph_and_klass.length;
    اگر (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (نادرست);
    return_trace (keep_empty_table || (منطقی) glyph_and_klass);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () ثابت { بازگشت hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  منطقی collect_coverage (set_t *glyphs) ثابت
  {
    برای (auto &range : rangeRecord)
      اگر (range.value)
	اگر (unlikely (!range.collect_coverage (glyphs)))
	  بازگشت نادرست;
    بازگشت درست;
  }

  template <typename set_t>
  منطقی collect_class (set_t *glyphs, unsigned صحیح klass) ثابت
  {
    برای (auto &range : rangeRecord)
    {
      اگر (range.value == klass)
	اگر (unlikely (!range.collect_coverage (glyphs)))
	  بازگشت نادرست;
    }
    بازگشت درست;
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    اگر (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      برای (auto g : *glyphs)
        اگر (get_class (g))
	  بازگشت درست;
      بازگشت نادرست;
    }

    بازگشت hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ثابت RangeRecord<Types> &range) { بازگشت range.intersects (*glyphs) && range.value; }));
  }
  منطقی intersects_class (ثابت hb_set_t *glyphs, unsigned klass) ثابت
  {
    اگر (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      اگر (!rangeRecord.len)
        بازگشت !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_مقدار_INVALID;
      hb_codepoint_t last = HB_SET_مقدار_INVALID;
      auto it = hb_iter (rangeRecord);
      برای (auto &range : it)
      {
        اگر (it->first == last + 1)
	{
	  it++;
	  ادامه;
	}

	اگر (!glyphs->next (&g))
	  بشکن;
	اگر (g < range.first)
	  بازگشت درست;
	g = range.last;
	last = g;
      }
      اگر (g != HB_SET_مقدار_INVALID && glyphs->next (&g))
	بازگشت درست;
      /* Fall through. */
    }
    برای (ثابت auto &range : rangeRecord)
      اگر (range.value == klass && range.intersects (*glyphs))
	بازگشت درست;
    بازگشت نادرست;
  }

  تهی intersected_class_glyphs (ثابت hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ثابت
  {
    اگر (klass == 0)
    {
      hb_codepoint_t g = HB_SET_مقدار_INVALID;
      برای (auto &range : rangeRecord)
      {
	اگر (!glyphs->next (&g))
	  برو done;
	وقتی (g < range.first)
	{
	  intersect_glyphs->add (g);
	  اگر (!glyphs->next (&g))
	    برو done;
        }
        g = range.last;
      }
      وقتی (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      بازگشت;
    }

    unsigned شمار = rangeRecord.len;
    اگر (شمار > glyphs->get_population () * hb_bit_storage (شمار))
    {
      برای (auto g : *glyphs)
      {
        unsigned i;
        اگر (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      بازگشت;
    }

    برای (auto &range : rangeRecord)
    {
      اگر (range.value != klass) ادامه;

      unsigned end = range.last + 1;
      برای (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  تهی intersected_classes (ثابت hb_set_t *glyphs, hb_set_t *intersect_classes) ثابت
  {
    اگر (glyphs->is_empty ()) بازگشت;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    اگر (!rangeRecord.len)
    {
      intersect_classes->add (0);
      بازگشت;
    }

    hb_codepoint_t g = HB_SET_مقدار_INVALID;
    برای (auto &range : rangeRecord)
    {
      اگر (!glyphs->next (&g))
        بشکن;
      اگر (g < range.first)
      {
        intersect_classes->add (0);
        بشکن;
      }
      g = range.last;
    }
    اگر (g != HB_SET_مقدار_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    برای (ثابت auto& range : rangeRecord)
      اگر (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  محافظتشده:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

ساختار ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ثابت { بازگشت get (k); }
  منطقی has (hb_codepoint_t k) ثابت { بازگشت (*این)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) ثابت { بازگشت get (k); }

  unsigned صحیح get (hb_codepoint_t k) ثابت { بازگشت get_class (k); }
  unsigned صحیح get_class (hb_codepoint_t glyph_id) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.get_class (glyph_id);
    حالت 2: hb_barrier (); بازگشت u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.get_class (glyph_id);
    حالت 4: hb_barrier (); بازگشت u.format4.get_class (glyph_id);
#endif
    پیشفرض:بازگشت 0;
    }
  }
  unsigned صحیح get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    unsigned klass;
    اگر (cache && cache->get (glyph_id, &klass)) بازگشت klass;
    klass = get_class (glyph_id);
    اگر (cache) cache->set (glyph_id, klass);
    بازگشت klass;
  }

  unsigned get_population () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.get_population ();
    حالت 2: hb_barrier (); بازگشت u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.get_population ();
    حالت 4: hb_barrier (); بازگشت u.format4.get_population ();
#endif
    پیشفرض:بازگشت NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    اگر (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned رقم_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      برای (ثابت auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	اگر (cur_klass > class_max) class_max = cur_klass;
	اگر (cur_gid == glyph_min) ادامه;
        اگر (cur_gid > glyph_max) glyph_max = cur_gid;
	اگر (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  رقم_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	اگر (size >= best_size) بازگشت;
	best_size = size;
	format = candidate;
      };

      اگر (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      اگر (glyph_max <= 0xFFFFu && رقم_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) رقم_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      اگر (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      اگر (glyph_max <= 0xFFFFFFu && رقم_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) رقم_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      اگر (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (نادرست, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (نادرست);
      }
    }

    u.format.v = format;

    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    حالت 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    حالت 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    پیشفرض:return_trace (نادرست);
    }
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               منطقی keep_empty_table = درست,
               منطقی use_class_zero = درست,
               ثابت Coverage* glyph_filter = nullptr) ثابت
  {
    TRACE_SUBSET (این);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    حالت 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    حالت 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    پیشفرض:return_trace (نادرست);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.format.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    پیشفرض:return_trace (درست);
    }
  }

  unsigned cost () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.cost ();
    حالت 2: hb_barrier (); بازگشت u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.cost ();
    حالت 4: hb_barrier (); بازگشت u.format4.cost ();
#endif
    پیشفرض:بازگشت 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  منطقی collect_coverage (set_t *glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.collect_coverage (glyphs);
    حالت 2: hb_barrier (); بازگشت u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.collect_coverage (glyphs);
    حالت 4: hb_barrier (); بازگشت u.format4.collect_coverage (glyphs);
#endif
    پیشفرض:بازگشت نادرست;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  منطقی collect_class (set_t *glyphs, unsigned صحیح klass) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.collect_class (glyphs, klass);
    حالت 2: hb_barrier (); بازگشت u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.collect_class (glyphs, klass);
    حالت 4: hb_barrier (); بازگشت u.format4.collect_class (glyphs, klass);
#endif
    پیشفرض:بازگشت نادرست;
    }
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.intersects (glyphs);
    حالت 2: hb_barrier (); بازگشت u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.intersects (glyphs);
    حالت 4: hb_barrier (); بازگشت u.format4.intersects (glyphs);
#endif
    پیشفرض:بازگشت نادرست;
    }
  }
  منطقی intersects_class (ثابت hb_set_t *glyphs, unsigned صحیح klass) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.intersects_class (glyphs, klass);
    حالت 2: hb_barrier (); بازگشت u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.intersects_class (glyphs, klass);
    حالت 4: hb_barrier (); بازگشت u.format4.intersects_class (glyphs, klass);
#endif
    پیشفرض:بازگشت نادرست;
    }
  }

  تهی intersected_class_glyphs (ثابت hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالت 2: hb_barrier (); بازگشت u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالت 4: hb_barrier (); بازگشت u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    پیشفرض:بازگشت;
    }
  }

  تهی intersected_classes (ثابت hb_set_t *glyphs, hb_set_t *intersect_classes) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.intersected_classes (glyphs, intersect_classes);
    حالت 2: hb_barrier (); بازگشت u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); بازگشت u.format3.intersected_classes (glyphs, intersect_classes);
    حالت 4: hb_barrier (); بازگشت u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    پیشفرض:بازگشت;
    }
  }


  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
ایستا درونخطی منطقی ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ بازگشت (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
ساختار delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  ساختار chars_t : hb_vector_t<uint8_t>
  {
    صحیح cmp (ثابت chars_t& other) ثابت
    {
      بازگشت as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      برای (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      بازگشت hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (ثابت chars_t& other) ثابت
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      برای (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      بازگشت hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (ثابت delta_row_encoding_t& other_encoding) ثابت { بازگشت chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<ثابت hb_vector_t<صحیح>*> اقلام;

  delta_row_encoding_t () = پیشفرض;
  delta_row_encoding_t (hb_vector_t<ثابت hb_vector_t<صحیح>*> &&rows, unsigned رقم_cols)
  {
    أكد (rows);

    اقلام = std::move (rows);

    اگر (unlikely (!chars.resize (رقم_cols)))
      بازگشت;

    calculate_chars ();
  }

  تهی merge (ثابت delta_row_encoding_t& other)
  {
    اقلام.alloc (اقلام.length + other.اقلام.length);
    برای (auto &row : other.اقلام)
      add_row (row);

    // Merge chars
    أكد (chars.length == other.chars.length);
    برای (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  تهی chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  تهی calculate_chars ()
  {
    أكد (اقلام);

    منطقی long_واژگان = نادرست;

    برای (auto &row : اقلام)
    {
      أكد (row->length == chars.length);

      /* 0/1/2 byte encoding */
      برای (unsigned i = 0; i < row->length; i++)
      {
	صحیح v =  row->arrayZ[i];
	اگر (v == 0)
	  ادامه;
	وگرنه اگر (v > 32767 || v < -32768)
	{
	  long_واژگان = درست;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	وگرنه اگر (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	وگرنه
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    اگر (long_واژگان)
    {
      // Convert 1s to 2s
      برای (auto &v : chars)
	اگر (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  منطقی is_empty () ثابت
  { بازگشت !اقلام; }

  منطقی in_error () ثابت
  { بازگشت chars.in_error () || اقلام.in_error (); }

  ایستا درونخطی unsigned get_chars_overhead (unsigned رقم_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    بازگشت c + رقم_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) ثابت
  {
    صحیح شمار = اقلام.length;
    بازگشت hb_max (0, (صحیح) overhead - شمار * (صحیح) additional_bytes_per_rows);
  }

  صحیح gain_from_merging (ثابت delta_row_encoding_t& other_encoding) ثابت
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    اگر (additional_bytes_per_rows > 0)
    {
      اگر (get_gain (additional_bytes_per_rows) == 0)
        بازگشت 0;
    }
    وگرنه
    {
      اگر (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	بازگشت 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    صحیح combined_gain = (صحیح) overhead + (صحیح) other_encoding.overhead;
    combined_gain -= (combined_width - (صحیح) width) * اقلام.length;
    combined_gain -= (combined_width - (صحیح) other_encoding.width) * other_encoding.اقلام.length;
    combined_gain -= get_chars_overhead (combined_columns);

    بازگشت combined_gain;
  }

  منطقی add_row (ثابت hb_vector_t<صحیح>* row)
  { بازگشت اقلام.push (row); }

  ایستا صحیح cmp (ثابت تهی *pa, ثابت تهی *pb)
  {
    ثابت delta_row_encoding_t *a = (ثابت delta_row_encoding_t *)pa;
    ثابت delta_row_encoding_t *b = (ثابت delta_row_encoding_t *)pb;

    اگر (a->width != b->width)
      بازگشت (صحیح) a->width - (صحیح) b->width;

    بازگشت b->chars.cmp (a->chars);
  }
};

ساختار VarRegionAxis
{
  اعشاری evaluate (صحیح coord) ثابت
  {
    صحیح peak = peakCoord.to_int ();
    اگر (peak == 0 || coord == peak)
      بازگشت 1.f;
    وگرنه اگر (coord == 0) // Faster
      بازگشت 0.f;

    صحیح start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    اگر (unlikely (start > peak || peak > end))
      بازگشت 1.f;
    اگر (unlikely (start < 0 && end > 0))
      بازگشت 1.f;

    اگر (coord <= start || end <= coord)
      بازگشت 0.f;

    /* Interpolate */
    اگر (coord < peak)
      بازگشت اعشاری (coord - start) / (peak - start);
    وگرنه
      بازگشت اعشاری (end - coord) / (end - peak);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  منطقی serialize (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed (این));
  }

  عمومی:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  عمومی:
  DEFINE_SIZE_STATIC (6);
};
ساختار SparseVarRegionAxis
{
  اعشاری evaluate (ثابت صحیح *coords, unsigned صحیح coord_len) ثابت
  {
    unsigned i = axisIndex;
    صحیح coord = i < coord_len ? coords[i] : 0;
    بازگشت axis.evaluate (coord);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت SparseVarRegionAxis *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (این);
    auto *out = c->embed (src);
    اگر (unlikely (!out)) return_trace (نادرست);
    اگر (axis_map)
    {
      اگر (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (نادرست);
      }
      اگر (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (نادرست);
    }
    return_trace (درست);
  }

  عمومی:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار hb_scalar_cache_t
{
  خصوصی:
  ایستا constexpr unsigned STATIC_LENGTH = 128;
  ایستا constexpr اعشاری INVALID = NAN;

  عمومی:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ثابت hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (ثابت hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  ایستا hb_scalar_cache_t *create (unsigned صحیح شمار,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    اگر (!شمار) بازگشت (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    اگر (scratch_cache && شمار <= STATIC_LENGTH)
    {
      scratch_cache->length = شمار;
      scratch_cache->clear ();
      بازگشت scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (اندازه (hb_scalar_cache_t) - اندازه (static_مقادیر) + اندازه (static_مقادیر[0]) * شمار);
    اگر (unlikely (!cache)) بازگشت (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = شمار;
    cache->clear ();

    بازگشت cache;
  }

  ایستا تهی destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    اگر (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  تهی clear ()
  {
    auto *values = &static_مقادیر[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    برای (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    برای (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  منطقی get (unsigned i, اعشاری *value) ثابت
  {
    اگر (unlikely (i >= length))
    {
      *value = 0.f;
      بازگشت درست;
    }
    auto *values = &static_مقادیر[0];
    auto *cached_مقدار = &values[i];
    اعشاری v = *cached_مقدار;
    اگر (std::isnan (v))
      بازگشت نادرست;
    *value = v;
    بازگشت درست;
  }

  HB_ALWAYS_INLINE
  تهی set (unsigned i, اعشاری value)
  {
    اگر (unlikely (i >= length)) بازگشت;
    auto *values = &static_مقادیر[0];
    auto *cached_مقدار = &values[i];
    *cached_مقدار = value;
  }

  عمومی:
  unsigned length;
  خصوصی:
  mutable hb_atomic_t<اعشاری> static_مقادیر[STATIC_LENGTH];
};

ساختار VarRegionList
{
  خصوصی:
  اعشاری evaluate_impl (unsigned صحیح region_index,
		       ثابت صحیح *coords, unsigned صحیح coord_len) ثابت
  {
    ثابت VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_شمار_2);
    اعشاری v = 1.f;

    unsigned صحیح شمار = axis_شمار_2;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      صحیح coord = i < coord_len ? coords[i] : 0;
      اعشاری factor = axes[i].evaluate (coord);
      اگر (factor == 0.f)
      {
	v = 0.f;
	بشکن;
      }
      v *= factor;
    }

    بازگشت v;
  }

  عمومی:
  HB_ALWAYS_INLINE
  اعشاری evaluate (unsigned صحیح region_index,
		  ثابت صحیح *coords, unsigned صحیح coord_len,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    اگر (unlikely (region_index >= region_شمار))
      بازگشت 0.;

    اعشاری v;
    اگر (cache && cache->get (region_index, &v))
      بازگشت v;

    v = evaluate_impl (region_index, coords, coord_len);

    اگر (cache)
      cache->set (region_index, v);
    بازگشت v;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_شمار_2 * region_شمار));
  }

  منطقی serialize (hb_serialize_context_t *c,
                  ثابت hb_vector_t<hb_tag_t>& axis_tags,
                  ثابت hb_vector_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (این);
    unsigned axis_شمار = axis_tags.length;
    unsigned region_شمار_2 = regions.length;
    اگر (!axis_شمار || !region_شمار_2) return_trace (نادرست);
    اگر (unlikely (hb_unsigned_mul_overflows (axis_شمار * region_شمار_2,
                                             VarRegionAxis::static_size))) return_trace (نادرست);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    axis_شمار_2 = axis_شمار;
    region_شمار = region_شمار_2;

    برای (unsigned r = 0; r < region_شمار_2; r++)
    {
      ثابت auto& region = regions[r];
      برای (unsigned i = 0; i < axis_شمار; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        اگر (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->میانه);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        وگرنه
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        اگر (!var_region_rec.serialize (c))
          return_trace (نادرست);
      }
    }
    return_trace (درست);
  }

  منطقی serialize (hb_serialize_context_t *c, ثابت VarRegionList *src, ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    axis_شمار_2 = src->axis_شمار_2;
    region_شمار = region_map.get_population ();
    اگر (unlikely (hb_unsigned_mul_overflows (axis_شمار_2 * region_شمار,
					     VarRegionAxis::static_size))) return_trace (نادرست);
    اگر (unlikely (!c->extend (این))) return_trace (نادرست);
    unsigned صحیح region_شمار_2 = src->region_شمار;
    برای (unsigned صحیح r = 0; r < region_شمار; r++)
    {
      unsigned صحیح backward = region_map.backward (r);
      اگر (backward >= region_شمار_2) return_trace (نادرست);
      hb_memcpy (&axesZ[axis_شمار_2 * r], &src->axesZ[axis_شمار_2 * backward], VarRegionAxis::static_size * axis_شمار_2);
    }

    return_trace (درست);
  }

  منطقی get_var_region (unsigned region_index,
                       ثابت hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ثابت
  {
    اگر (region_index >= region_شمار) بازگشت نادرست;
    ثابت VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_شمار_2);
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      hb_tag_t *axis_tag;
      اگر (!axes_old_index_tag_map.has (i, &axis_tag))
        بازگشت نادرست;

      اعشاری min_val = axis_region->startCoord.to_float ();
      اعشاری def_val = axis_region->peakCoord.to_float ();
      اعشاری max_val = axis_region->endCoord.to_float ();

      اگر (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((دوگانه) min_val, (دوگانه) def_val, (دوگانه) max_val));
      axis_region++;
    }
    بازگشت !axis_tuples.in_error ();
  }

  منطقی get_var_regions (ثابت hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) ثابت
  {
    اگر (!regions.alloc (region_شمار))
      بازگشت نادرست;

    برای (unsigned i = 0; i < region_شمار; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      اگر (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        بازگشت نادرست;
      regions.push (std::move (axis_tuples));
    }
    بازگشت !regions.in_error ();
  }

  size_t get_size () ثابت
  { بازگشت hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_شمار_2, region_شمار)); }

  عمومی:
  HBUINT16	axis_شمار_2;
  HBUINT15	region_شمار;
  محافظتشده:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  عمومی:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ایستا درونخطی منطقی
_hb_avar2_region_is_dead (ثابت hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  ثابت hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  برای (auto _ : axis_tuples)
  {
    Triple *range;
    اگر (!reachable_ranges.has (_.first, &range)) ادامه;
    ثابت Triple &tent = _.second;
    صحیح start = (صحیح) roundf ((اعشاری) tent.minimum * 16384.f);
    صحیح peak  = (صحیح) roundf ((اعشاری) tent.میانه * 16384.f);
    صحیح end   = (صحیح) roundf ((اعشاری) tent.maximum * 16384.f);
    اگر (!peak) ادامه;
    اگر (start > peak || peak > end) ادامه;
    اگر (start < 0 && end > 0) ادامه;
    صحیح lo = (صحیح) roundf ((اعشاری) range->minimum * 16384.f);
    صحیح hi = (صحیح) roundf ((اعشاری) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    اگر (hi < start || lo > end)
      بازگشت درست;
  }
  بازگشت نادرست;
}

ساختار SparseVariationRegion
{
  اعشاری evaluate (ثابت صحیح *coords, unsigned صحیح coord_len) ثابت
  {
    اعشاری v = 1.f;
    unsigned صحیح شمار = axes.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اعشاری factor = (این+axes[i]).evaluate (coords, coord_len);
      اگر (factor == 0.f)
	بازگشت 0.;
      v *= factor;
    }
    بازگشت v;
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت SparseVariationRegion *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (نادرست);
    برای (unsigned i = 0; i < axes.len; i++)
      اگر (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (نادرست);
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (axes.sanitize (c, این));
  }

  محافظتشده:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  عمومی:
  DEFINE_SIZE_ARRAY (2, axes);
};

ساختار SparseVarRegionList
{
  HB_ALWAYS_INLINE
  اعشاری evaluate (unsigned صحیح region_index,
		  ثابت صحیح *coords, unsigned صحیح coord_len,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    اگر (unlikely (region_index >= regions.len))
      بازگشت 0.;

    اعشاری v;
    اگر (cache && cache->get (region_index, &v))
      بازگشت v;

    ثابت SparseVariationRegion &region = این+regions[region_index];

    v = region.evaluate (coords, coord_len);
    اگر (cache)
      cache->set (region_index, v);

    بازگشت v;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (regions.sanitize (c, این));
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت SparseVarRegionList *src,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    regions.len = region_map.get_population ();
    اگر (unlikely (!c->extend (regions))) return_trace (نادرست);

    برای (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      اگر (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (نادرست);
    }
    return_trace (درست);
  }

  عمومی:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  عمومی:
  DEFINE_SIZE_ARRAY (4, regions);
};


ساختار VarData
{
  unsigned صحیح get_item_شمار () ثابت
  { بازگشت item_شمار; }

  unsigned صحیح get_region_index_شمار () ثابت
  { بازگشت regionIndices.len; }

  unsigned get_region_index (unsigned i) ثابت
  { بازگشت i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () ثابت
  { بازگشت (word_شمار () + regionIndices.len) * (long_واژگان_2 () ? 2 : 1); }

  size_t get_size () ثابت
  { بازگشت hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_شمار, get_row_size ())); }

  template <typename Float = اعشاری>
  Float _get_delta (unsigned صحیح inner,
		    ثابت صحیح *coords, unsigned صحیح coord_شمار,
		    ثابت VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) ثابت
  {
    اگر (unlikely (inner >= item_شمار))
      بازگشت 0.;
    منطقی is_long = long_واژگان_2 ();
    unsigned صحیح شمار = regionIndices.len;
    unsigned word_شمار_2 = word_شمار ();
    unsigned صحیح scount = is_long ? شمار : word_شمار_2;
    unsigned صحیح lcount = is_long ? word_شمار_2 : 0;

    ثابت HBUINT8 *bytes = get_delta_bytes ();
    ثابت HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned صحیح i = 0;

    ثابت HBINT32 *lcursor = reinterpret_cast<ثابت HBINT32 *> (row);
    برای (; i < lcount; i++)
    {
      اعشاری scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمار, cache);
      اگر (scalar)
        delta += (Float) scalar * (صحیح) *lcursor;
      lcursor++;
    }
    ثابت HBINT16 *scursor = reinterpret_cast<ثابت HBINT16 *> (lcursor);
    برای (; i < scount; i++)
    {
      اعشاری scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمار, cache);
      اگر (scalar)
       delta += (Float) scalar * (صحیح) *scursor;
      scursor++;
    }
    ثابت HBINT8 *bcursor = reinterpret_cast<ثابت HBINT8 *> (scursor);
    برای (; i < شمار; i++)
    {
      اعشاری scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمار, cache);
      اگر (scalar)
        delta += (Float) scalar * (صحیح) *bcursor;
      bcursor++;
    }

    بازگشت delta;
  }

  template <typename Float = اعشاری>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned صحیح inner,
		   ثابت صحیح *coords, unsigned صحیح coord_شمار,
		   ثابت VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) ثابت
  {
    unsigned صحیح شمار = regionIndices.len;
    اگر (!شمار) بازگشت Float (0); // This is quite common, so optimize it.
    بازگشت _get_delta<Float> (inner, coords, coord_شمار, regions, cache);
  }

  تهی get_region_scalars (ثابت صحیح *coords, unsigned صحیح coord_شمار,
			   ثابت VarRegionList &regions,
			   اعشاری *scalars /*OUT */,
			   unsigned صحیح رقم_scalars) ثابت
  {
    unsigned شمار = hb_min (رقم_scalars, regionIndices.len);
    برای (unsigned صحیح i = 0; i < شمار; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمار);
    برای (unsigned صحیح i = شمار; i < رقم_scalars; i++)
      scalars[i] = 0.f;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_شمار () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_شمار,
				  get_row_size ()));
  }

  منطقی serialize (hb_serialize_context_t *c,
                  منطقی has_long,
                  ثابت hb_vector_t<ثابت hb_vector_t<صحیح>*>& rows)
  {
    TRACE_SERIALIZE (این);
    unsigned row_شمار_2 = rows.length;
    اگر (!row_شمار_2) {
      // Nothing to serialize, will be empty.
      بازگشت نادرست;
    }

    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    اگر (unlikely (!c->check_assign (item_شمار, row_شمار_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (نادرست);

    صحیح min_threshold = has_long ? -32768 : -128;
    صحیح max_threshold = has_long ? +32767 : +127;
    شمارش delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned رقم_regions = rows[0]->length;
    اگر (!delta_sz.resize (رقم_regions))
      return_trace (نادرست);

    unsigned word_شمار_2 = 0;
    برای (unsigned r = 0; r < رقم_regions; r++)
    {
      برای (unsigned i = 0; i < row_شمار_2; i++)
      {
        صحیح delta = rows[i]->arrayZ[r];
        اگر (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_شمار_2++;
          بشکن;
        }
        وگرنه اگر (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_شمار_2;
    hb_map_t ri_map;
    برای (unsigned r = 0; r < رقم_regions; r++)
    {
      اگر (!delta_sz[r]) ادامه;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      اگر (!ri_map.set (new_r, r))
        return_trace (نادرست);
    }

    word_Size_شمار = word_شمار_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_شمار = ri_map.get_population ();
    regionIndices.len = ri_شمار;
    اگر (unlikely (!c->extend (این))) return_trace (نادرست);

    برای (unsigned r = 0; r < ri_شمار; r++)
    {
      hb_codepoint_t *idx;
      اگر (!ri_map.has (r, &idx))
        return_trace (نادرست);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    برای (unsigned صحیح i = 0; i < row_شمار_2; i++)
    {
      برای (unsigned صحیح r = 0; r < ri_شمار; r++)
      {
        صحیح delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (درست);
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت VarData *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    item_شمار = inner_map.get_next_مقدار ();

    /* Optimize word count */
    unsigned ri_شمار = src->regionIndices.len;
    شمارش delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned صحیح> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_شمار);
    ri_map.resize (ri_شمار);
    unsigned صحیح new_word_شمار = 0;
    unsigned صحیح r;

    ثابت HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_شمار = src->word_شمار ();
    منطقی     src_long_واژگان = src->long_واژگان_2 ();

    منطقی has_long = نادرست;
    اگر (src_long_واژگان)
    {
      برای (r = 0; r < src_word_شمار; r++)
      {
        برای (unsigned old_gid : inner_map.کلیدها())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  اگر (delta < -32768 || 32767 < delta)
	  {
	    has_long = درست;
	    بشکن;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    برای (r = 0; r < ri_شمار; r++)
    {
      منطقی short_circuit = src_long_واژگان == has_long && src_word_شمار <= r;

      delta_sz[r] = kZero;
      اگر (culled_regions && culled_regions->has (src->regionIndices[r]))
	ادامه; /* unreachable region: drop its column */
      برای (unsigned old_gid : inner_map.کلیدها())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	اگر (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_شمار++;
	  بشکن;
	}
	وگرنه اگر (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  اگر (short_circuit)
	    بشکن;
	}
      }
    }

    unsigned صحیح word_index = 0;
    unsigned صحیح non_word_index = new_word_شمار;
    unsigned صحیح new_ri_شمار = 0;
    برای (r = 0; r < ri_شمار; r++)
      اگر (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_شمار++;
      }

    word_Size_شمار = new_word_شمار | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_شمار;

    اگر (unlikely (!c->extend (این))) return_trace (نادرست);

    برای (r = 0; r < new_ri_شمار; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned شمار = item_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      unsigned صحیح old = inner_map.backward (i);
      برای (unsigned صحیح r = 0; r < new_ri_شمار; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (درست);
  }

  تهی collect_region_refs (hb_set_t &region_indices, ثابت hb_inc_bimap_t &inner_map) ثابت
  {
    ثابت HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    برای (unsigned صحیح r = 0; r < regionIndices.len; r++)
    {
      unsigned صحیح region = regionIndices.arrayZ[r];
      اگر (region_indices.has (region)) ادامه;
      برای (hb_codepoint_t old_gid : inner_map.کلیدها())
	اگر (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  بشکن;
	}
    }
  }

  عمومی:
  ثابت HBUINT8 *get_delta_bytes () ثابت
  { بازگشت &StructAfter<HBUINT8> (regionIndices); }

  محافظتشده:
  HBUINT8 *get_delta_bytes ()
  { بازگشت &StructAfter<HBUINT8> (regionIndices); }

  عمومی:
  int32_t get_item_delta_fast (unsigned صحیح item, unsigned صحیح region,
			       ثابت HBUINT8 *delta_bytes, unsigned row_size) ثابت
  {
    اگر (unlikely (item >= item_شمار || region >= regionIndices.len)) بازگشت 0;

    ثابت HBINT8 *p = (ثابت HBINT8 *) delta_bytes + item * row_size;
    unsigned word_شمار_2 = word_شمار ();
    منطقی is_long = long_واژگان_2 ();
    اگر (is_long)
    {
      اگر (region < word_شمار_2)
	بازگشت ((ثابت HBINT32 *) p)[region];
      وگرنه
	بازگشت ((ثابت HBINT16 *)(p + HBINT32::static_size * word_شمار_2))[region - word_شمار_2];
    }
    وگرنه
    {
      اگر (region < word_شمار_2)
	بازگشت ((ثابت HBINT16 *) p)[region];
      وگرنه
	بازگشت (p + HBINT16::static_size * word_شمار_2)[region - word_شمار_2];
    }
  }
  int32_t get_item_delta (unsigned صحیح item, unsigned صحیح region) ثابت
  {
     بازگشت get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  محافظتشده:
  تهی set_item_delta_fast (unsigned صحیح item, unsigned صحیح region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_شمار_2 = word_شمار ();
    منطقی is_long = long_واژگان_2 ();
    اگر (is_long)
    {
      اگر (region < word_شمار_2)
	((HBINT32 *) p)[region] = delta;
      وگرنه
	((HBINT16 *)(p + HBINT32::static_size * word_شمار_2))[region - word_شمار_2] = delta;
    }
    وگرنه
    {
      اگر (region < word_شمار_2)
	((HBINT16 *) p)[region] = delta;
      وگرنه
	(p + HBINT16::static_size * word_شمار_2)[region - word_شمار_2] = delta;
    }
  }
  تهی set_item_delta (unsigned صحیح item, unsigned صحیح region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  منطقی long_واژگان_2 () ثابت { بازگشت word_Size_شمار & 0x8000u /* LONG_WORDS */; }
  unsigned word_شمار () ثابت { بازگشت word_Size_شمار & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  محافظتشده:
  HBUINT16		item_شمار;
  HBUINT16		word_Size_شمار;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  عمومی:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

ساختار MultiVarData
{
  size_t get_size () ثابت
  { بازگشت hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  ثابت TupleList &get_delta_sets () ثابت
  { بازگشت این+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  تهی get_delta (unsigned صحیح inner,
		  ثابت صحیح *coords, unsigned صحیح coord_شمار,
		  ثابت SparseVarRegionList &regions,
		  hb_array_t<دوگانه> out,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    auto &deltaSets = get_delta_sets ();

    auto مقادیر_iter = deltaSets.fetcher (inner);
    unsigned region_شمار = regionIndices.len;
    unsigned skip = 0;
    برای (unsigned regionIndex = 0; regionIndex < region_شمار; regionIndex++)
    {
      اعشاری scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_شمار,
				       cache);
      // We skip lazily. Helps with the tail end.
      اگر (scalar == 0.0f)
        skip += out.length;
      وگرنه
      {
        اگر (skip)
	{
	  مقادیر_iter.skip (skip);
	  skip = 0;
	}
	مقادیر_iter.add_to (out, scalar);
      }
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, این));
  }

  unsigned get_item_شمار () ثابت
  { بازگشت get_delta_sets ().شمار; }

  تهی collect_region_refs (hb_set_t &region_indices) ثابت
  {
    برای (unsigned region : regionIndices)
      region_indices.add (region);
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت MultiVarData *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (این);
    unsigned region_شمار_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_شمار_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    اگر (unlikely (!c->extend_size (این, header_size, نادرست)))
      return_trace (نادرست);
    format = 1;
    regionIndices.len = region_شمار_2;
    برای (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      اگر (unlikely (!region_map.has (old_region)))
	return_trace (نادرست);
      regionIndices[i] = region_map.get (old_region);
    }

    ثابت auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    اگر (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (نادرست);
    برای (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      اگر (unlikely (old_inner >= src_delta_sets.شمار)) return_trace (نادرست);
      hb_ubytes_t bytes = static_cast<ثابت CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    اگر (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (نادرست);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  محافظتشده:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  عمومی:
  DEFINE_SIZE_MIN (7);
};

ساختار ItemVariationStore
{
  friend ساختار item_variations_t;

  hb_scalar_cache_t *create_cache () ثابت
  {
#ifdef HB_NO_VAR
    بازگشت hb_scalar_cache_t::create (0);
#endif
    بازگشت hb_scalar_cache_t::create ((این+regions).region_شمار);
  }

  ایستا تهی destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  خصوصی:
  template <typename Float = اعشاری>
  Float get_delta (unsigned صحیح outer, unsigned صحیح inner,
		   ثابت صحیح *coords, unsigned صحیح coord_شمار,
		   hb_scalar_cache_t *cache = nullptr) ثابت
  {
#ifdef HB_NO_VAR
    بازگشت Float (0);
#endif

    اگر (unlikely (outer >= dataSets.len))
      بازگشت Float (0);

    بازگشت (این+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_شمار,
					     این+regions,
					     cache);
  }

  عمومی:
  منطقی has_delta_set (unsigned صحیح index) ثابت
  {
#ifdef HB_NO_VAR
    بازگشت نادرست;
#endif

    unsigned صحیح outer = index >> 16;
    unsigned صحیح inner = index & 0xFFFF;
    بازگشت outer < dataSets.len &&
	   inner < (این+dataSets[outer]).get_item_شمار ();
  }

  template <typename Float = اعشاری>
  Float get_delta (unsigned صحیح index,
		   ثابت صحیح *coords, unsigned صحیح coord_شمار,
		   hb_scalar_cache_t *cache = nullptr) ثابت
  {
    unsigned صحیح outer = index >> 16;
    unsigned صحیح inner = index & 0xFFFF;
    بازگشت get_delta<Float> (outer, inner, coords, coord_شمار, cache);
  }
  template <typename Float = اعشاری>
  Float get_delta (unsigned صحیح index,
		   hb_array_t<ثابت صحیح> coords,
		   hb_scalar_cache_t *cache = nullptr) ثابت
  {
    بازگشت get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
#ifdef HB_NO_VAR
    بازگشت درست;
#endif

    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, این) &&
		  dataSets.sanitize (c, این));
  }

  منطقی serialize (hb_serialize_context_t *c,
                  منطقی has_long,
                  ثابت hb_vector_t<hb_tag_t>& axis_tags,
                  ثابت hb_vector_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  ثابت hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (این);
#ifdef HB_NO_VAR
    return_trace (نادرست);
#endif
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    format = 1;
    اگر (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (نادرست);

    unsigned رقم_var_data = vardata_encodings.length;
    اگر (!رقم_var_data) return_trace (نادرست);
    اگر (unlikely (!c->check_assign (dataSets.len, رقم_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (نادرست);

    اگر (unlikely (!c->extend (dataSets))) return_trace (نادرست);
    برای (unsigned i = 0; i < رقم_var_data; i++)
      اگر (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].اقلام))
        return_trace (نادرست);

    return_trace (درست);
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت ItemVariationStore *src,
		  ثابت hb_array_t <ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (این);
#ifdef HB_NO_VAR
    return_trace (نادرست);
#endif

    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    unsigned صحیح set_شمار = 0;
    برای (unsigned صحیح i = 0; i < inner_maps.length; i++)
      اگر (inner_maps[i].get_population ())
	set_شمار++;

    format = 1;

    ثابت auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    برای (unsigned صحیح i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    اگر (region_indices.in_error ())
      return_trace (نادرست);

    region_indices.del_range ((src_regions).region_شمار, hb_set_t::INVALID);

    اگر (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    اگر (region_map.in_error())
      return_trace (نادرست);

    اگر (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (نادرست);

    dataSets.len = set_شمار;
    اگر (unlikely (!c->extend (dataSets))) return_trace (نادرست);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned صحیح set_index = 0;
    برای (unsigned صحیح i = 0; i < inner_maps.length; i++)
    {
      اگر (!inner_maps[i].get_population ()) ادامه;
      اگر (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (نادرست);
    }

    return_trace (درست);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->start_embed (این);
    اگر (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned شمار = dataSets.len;
    برای (unsigned i = 0; i < شمار; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      اگر (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = این+dataSets[i];

      unsigned item_شمار = data.get_item_شمار ();
      برای (unsigned j = 0; j < item_شمار; j++)
	map->add (j);
    }

    اگر (unlikely (!out->serialize (c, این, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  منطقی collect_dead_regions (ثابت hb_map_t &axes_old_index_tag_map,
			     ثابت hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) ثابت
  {
#ifndef HB_NO_VAR
    اگر (!reachable_ranges.get_population ()) بازگشت درست;
    ثابت VarRegionList &region_list = این+regions;
    unsigned شمار = region_list.region_شمار;
    برای (unsigned i = 0; i < شمار; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      اگر (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	بازگشت نادرست;
      اگر (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    بازگشت !dead_regions.in_error ();
#وگرنه
    بازگشت درست;
#endif
  }

  منطقی subset (hb_subset_context_t *c, ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps) ثابت
  {
    TRACE_SUBSET (این);
#ifdef HB_NO_VAR
    return_trace (نادرست);
#endif

    ItemVariationStore *varstore_اول = c->serializer->start_embed<ItemVariationStore> ();
    اگر (unlikely (!varstore_اول)) return_trace (نادرست);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    اگر (c->plan->has_avar2)
      اگر (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (نادرست);

    varstore_اول->serialize (c->serializer, این, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_اول->dataSets);
  }

  unsigned صحیح get_region_index_شمار (unsigned صحیح major) ثابت
  {
#ifdef HB_NO_VAR
    بازگشت 0;
#endif
    بازگشت (این+dataSets[major]).get_region_index_شمار ();
  }

  تهی get_region_scalars (unsigned صحیح major,
			   ثابت صحیح *coords, unsigned صحیح coord_شمار,
			   اعشاری *scalars /*OUT*/,
			   unsigned صحیح رقم_scalars) ثابت
  {
#ifdef HB_NO_VAR
    برای (unsigned i = 0; i < رقم_scalars; i++)
      scalars[i] = 0.f;
    بازگشت;
#endif

    (این+dataSets[major]).get_region_scalars (coords, coord_شمار,
					       این+regions,
					       &scalars[0], رقم_scalars);
  }

  unsigned صحیح get_sub_table_شمار () ثابت
   {
#ifdef HB_NO_VAR
     بازگشت 0;
#endif
     بازگشت dataSets.len;
   }

  ثابت VarData& get_sub_table (unsigned i) ثابت
  {
#ifdef HB_NO_VAR
     بازگشت Null (VarData);
#endif
     بازگشت این+dataSets[i];
  }

  ثابت VarRegionList& get_region_list () ثابت
  {
#ifdef HB_NO_VAR
     بازگشت Null (VarRegionList);
#endif
     بازگشت این+regions;
  }

  محافظتشده:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

ساختار MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) ثابت
  {
#ifdef HB_NO_VAR
    بازگشت hb_scalar_cache_t::create (0);
#endif
    auto &r = این+regions;
    unsigned شمار = r.regions.len;

    بازگشت hb_scalar_cache_t::create (شمار, static_cache);
  }

  ایستا تهی destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  خصوصی:
  تهی get_delta (unsigned صحیح outer, unsigned صحیح inner,
		  ثابت صحیح *coords, unsigned صحیح coord_شمار,
		  hb_array_t<دوگانه> out,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
#ifdef HB_NO_VAR
    بازگشت;
#endif

    اگر (unlikely (outer >= dataSets.len))
      بازگشت;

    بازگشت (این+dataSets[outer]).get_delta (inner,
					     coords, coord_شمار,
					     این+regions,
					     out,
					     cache);
  }

  عمومی:
  تهی get_delta (unsigned صحیح index,
		  ثابت صحیح *coords, unsigned صحیح coord_شمار,
		  hb_array_t<دوگانه> out,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    unsigned صحیح outer = index >> 16;
    unsigned صحیح inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_شمار, out, cache);
  }
  تهی get_delta (unsigned صحیح index,
		  hb_array_t<ثابت صحیح> coords,
		  hb_array_t<دوگانه> out,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    بازگشت get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
#ifdef HB_NO_VAR
    بازگشت درست;
#endif

    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, این) &&
		  dataSets.sanitize (c, این));
  }

  منطقی create_subset_plan (ثابت hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ثابت
  {
    اگر (unlikely (!inner_maps->resize (dataSets.len))) بازگشت نادرست;
    برای (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      اگر (unlikely (outer >= dataSets.len ||
		    inner >= (این+dataSets[outer]).get_item_شمار ()))
	بازگشت نادرست;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    برای (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      ثابت hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      اگر (!inner_map.get_population ()) ادامه;
      اگر (unlikely (inner_map.in_error ())) بازگشت نادرست;
      برای (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    بازگشت !inner_maps->in_error () && !varidx_map->in_error ();
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت MultiItemVariationStore *src,
		  ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (نادرست);
    format = 1;

    ثابت auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_شمار = 0;
    برای (unsigned i = 0; i < src->dataSets.len; i++)
    {
      اگر (!inner_maps[i].get_population ()) ادامه;
      set_شمار++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    اگر (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (نادرست);

    dataSets.len = set_شمار;
    اگر (unlikely (!c->extend (dataSets))) return_trace (نادرست);
    unsigned set_index = 0;
    برای (unsigned i = 0; i < src->dataSets.len; i++)
    {
      اگر (!inner_maps[i].get_population ()) ادامه;
      اگر (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (نادرست);
    }
    return_trace (درست);
  }

  محافظتشده:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_شمار_T>
ساختار DeltaSetIndexMapFormat01
{
  friend ساختار DeltaSetIndexMap;

  size_t get_size () ثابت
  { بازگشت hb_unsigned_mul_add_saturate (map_شمار_2, get_width (), min_size); }

  خصوصی:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed (این));
  }

  template <typename T>
  منطقی serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    unsigned صحیح width = plan.get_width ();
    unsigned صحیح inner_bit_شمار = plan.get_inner_bit_شمار ();
    ثابت hb_array_t<ثابت uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (این);
    اگر (unlikely (output_map.length && ((((inner_bit_شمار-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (نادرست);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);

    entryFormat = ((width-1)<<4)|(inner_bit_شمار-1);
    map_شمار_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    اگر (unlikely (!p)) return_trace (نادرست);
    برای (unsigned صحیح i = 0; i < output_map.length; i++)
    {
      unsigned صحیح v = output_map.arrayZ[i];
      اگر (v)
      {
	unsigned صحیح outer = v >> 16;
	unsigned صحیح inner = v & 0xFFFF;
	unsigned صحیح u = (outer << inner_bit_شمار) | inner;
	برای (unsigned صحیح w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (درست);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned صحیح v) ثابت /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    اگر (!map_شمار_2)
      بازگشت v;
    بازگشت _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned صحیح v) ثابت /* Returns 16.16 outer.inner. */
  {
    اگر (v >= map_شمار_2)
      v = map_شمار_2 - 1;

    unsigned صحیح u = 0;
    { /* Fetch it. */
      unsigned صحیح w = get_width ();
      ثابت HBUINT8 *p = mapDataZ.arrayZ + w * v;
      برای (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned صحیح n = get_inner_bit_شمار ();
      unsigned صحیح outer = u >> n;
      unsigned صحیح inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    بازگشت u;
  }

  unsigned get_map_شمار () ثابت       { بازگشت map_شمار_2; }
  unsigned get_width () ثابت           { بازگشت ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_شمار () ثابت { بازگشت (entryFormat & 0xF) + 1; }


  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_شمار_2,
                                  get_width ()));
  }

  محافظتشده:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_شمار_T     map_شمار_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  عمومی:
  DEFINE_SIZE_ARRAY (2+Map_شمار_T::static_size, mapDataZ);
};

ساختار DeltaSetIndexMap
{
  template <typename T>
  منطقی serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    TRACE_SERIALIZE (این);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    پیشفرض:return_trace (نادرست);
    }
  }

  uint32_t map (unsigned v) ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); بازگشت (u.format0.map (v));
    حالت 1: hb_barrier (); بازگشت (u.format1.map (v));
    پیشفرض:بازگشت v;
    }
  }

  unsigned get_map_شمار () ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); بازگشت u.format0.get_map_شمار ();
    حالت 1: hb_barrier (); بازگشت u.format1.get_map_شمار ();
    پیشفرض:بازگشت 0;
    }
  }

  unsigned get_width () ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); بازگشت u.format0.get_width ();
    حالت 1: hb_barrier (); بازگشت u.format1.get_width ();
    پیشفرض:بازگشت 0;
    }
  }

  unsigned get_inner_bit_شمار () ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); بازگشت u.format0.get_inner_bit_شمار ();
    حالت 1: hb_barrier (); بازگشت u.format1.get_inner_bit_شمار ();
    پیشفرض:بازگشت 0;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.format.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    پیشفرض:return_trace (درست);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    حالت 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    پیشفرض:return_trace (nullptr);
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  عمومی:
  DEFINE_SIZE_UNION (1, format.v);
};


ساختار ItemVarStoreInstancer
{
  ItemVarStoreInstancer (ثابت ItemVariationStore *varStore_,
			 ثابت DeltaSetIndexMap *varIdxMap,
			 hb_array_t<ثابت صحیح> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    اگر (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    اگر (!hb_any (coords))
      این->coords = {};
  }

  operator منطقی () ثابت { بازگشت varStore && منطقی (coords); }

  اعشاری operator[] (uint32_t varIdx) ثابت
  { بازگشت (*این) (varIdx); }

  اعشاری operator() (uint32_t varIdx, unsigned short offset = 0) ثابت
  { بازگشت get_delta (varIdx, offset); }

  دوگانه get_condition_delta (uint32_t varIdx) ثابت
  { بازگشت get_delta<دوگانه> (varIdx); }

  template <typename Float = اعشاری>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) ثابت
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   اگر (!coords || varIdx == VarIdx::NO_VARIATION)
     بازگشت Float (0);

    varIdx += offset;
    اگر (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    بازگشت varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ثابت ItemVariationStore *varStore;
  ثابت DeltaSetIndexMap *varIdxMap;
  hb_array_t<ثابت صحیح> coords;
  hb_scalar_cache_t *cache;
};

ساختار MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (ثابت MultiItemVariationStore *varStore,
			      ثابت DeltaSetIndexMap *varIdxMap,
			      hb_array_t<ثابت صحیح> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    اگر (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator منطقی () ثابت { بازگشت varStore; }

  دوگانه get_condition_delta (uint32_t varIdx) ثابت
  { بازگشت (دوگانه) (*این)[varIdx]; }

  دوگانه operator[] (uint32_t varIdx) ثابت
  {
    دوگانه v = 0;
    (*این) (hb_array (&v, 1), varIdx);
    بازگشت v;
  }

  تهی operator() (hb_array_t<دوگانه> out, uint32_t varIdx, unsigned short offset = 0) ثابت
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    اگر (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      اگر (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    وگرنه
      برای (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  ثابت MultiItemVariationStore *varStore;
  ثابت DeltaSetIndexMap *varIdxMap;
  hb_array_t<ثابت صحیح> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
شمارش Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

شمارش lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

ساختار Condition;

template <typename Instancer>
ایستا منطقی
_hb_recurse_condition_evaluate (ثابت ساختار Condition &condition,
				ثابت صحیح *coords,
				unsigned صحیح coord_len,
				Instancer *instancer);

ساختار ConditionAxisRange
{
  friend ساختار Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    ثابت hb_map_t *index_map = &c->plan->axes_index_map;
    اگر (index_map->is_empty ()) return_trace (درست);

    ثابت hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    اگر (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (نادرست);

    ثابت hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    اگر (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    ثابت hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    اگر (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    اعشاری normalized_min = renormalize_مقدار ((دوگانه) filter_Range_Min_مقدار.to_float (), axis_limit, axis_triple_distances, نادرست);
    اعشاری normalized_max = renormalize_مقدار ((دوگانه) filter_Range_Max_مقدار.to_float (), axis_limit, axis_triple_distances, نادرست);
    out->filter_Range_Min_مقدار.set_float (normalized_min);
    out->filter_Range_Max_مقدار.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  خصوصی:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) ثابت
  {
    //invalid axis index, drop the entire record
    اگر (!c->axes_index_tag_map->has (axisIndex))
      بازگشت DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    منطقی axis_set_by_user = نادرست;
    اگر (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = درست;
    }

    اعشاری axis_min_val = axis_range.minimum;
    اعشاری axis_default_val = axis_range.میانه;
    اعشاری axis_max_val = axis_range.maximum;

    اعشاری filter_min_val = filter_Range_Min_مقدار.to_float ();
    اعشاری filter_max_val = filter_Range_Max_مقدار.to_float ();

    اگر (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = نادرست;

    //condition not met, drop the entire record
    اگر (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      بازگشت DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    اگر (axis_set_by_user && axis_range.is_point ())
      بازگشت DROP_COND_WITH_VAR;

    اگر (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_مقدار.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_مقدار.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      بازگشت KEEP_COND_WITH_VAR;
    }
    بازگشت KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  منطقی evaluate (ثابت صحیح *coords, unsigned صحیح coord_len,
		 Instancer *instancer HB_UNUSED) ثابت
  {
    صحیح coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    بازگشت filter_Range_Min_مقدار.to_int () <= coord && coord <= filter_Range_Max_مقدار.to_int ();
  }

  عمومی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_مقدار;
  F2DOT14	filter_Range_Max_مقدار;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار Condition_مقدار
{
  friend ساختار Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (varIdx == VarIdx::NO_VARIATION)
      return_trace (درست);

    hb_pair_t<unsigned, صحیح> *condition_مقدار;
    اگر (c->plan->layout_condition_idx_مقدار_map.has ((uintptr_t) این, &condition_مقدار))
    {
      out->default_مقدار_2 = condition_مقدار->second;
      out->varIdx = condition_مقدار->first;
      return_trace (درست);
    }

    hb_pair_t<unsigned, صحیح> *new_varidx_delta;
    اگر (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (نادرست);

    اگر (unlikely (!c->serializer->check_assign (
		out->default_مقدار_2,
		default_مقدار_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (نادرست);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (درست);
  }

  خصوصی:
  template <typename Instancer>
  منطقی evaluate (ثابت صحیح *coords, unsigned صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    دوگانه value = default_مقدار_2;
    value += instancer->get_condition_delta (varIdx);
    بازگشت value > 0;
  }

  عمومی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_مقدار_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار ConditionAnd
{
  friend ساختار Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (نادرست);
    out->format = format;

    برای (ثابت auto &offset : conditions)
    {
      اگر (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	اگر (unlikely (!out_offset)) return_trace (نادرست);
	*out_offset = 0;
      }
      وگرنه اگر (unlikely (!subset_offset_array (c, out->conditions, این) (offset)))
	return_trace (نادرست);
    }

    return_trace (درست);
  }

  خصوصی:
  template <typename Instancer>
  منطقی evaluate (ثابت صحیح *coords, unsigned صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    unsigned صحیح شمار = conditions.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!_hb_recurse_condition_evaluate (این+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	بازگشت نادرست;
    بازگشت درست;
  }

  عمومی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (conditions.sanitize (c, این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<ساختار Condition>	conditions;
  عمومی:
  DEFINE_SIZE_ARRAY (3, conditions);
};

ساختار ConditionOr
{
  friend ساختار Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (نادرست);
    out->format = format;

    برای (ثابت auto &offset : conditions)
    {
      اگر (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	اگر (unlikely (!out_offset)) return_trace (نادرست);
	*out_offset = 0;
      }
      وگرنه اگر (unlikely (!subset_offset_array (c, out->conditions, این) (offset)))
	return_trace (نادرست);
    }

    return_trace (درست);
  }

  خصوصی:
  template <typename Instancer>
  منطقی evaluate (ثابت صحیح *coords, unsigned صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    unsigned صحیح شمار = conditions.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (_hb_recurse_condition_evaluate (این+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	بازگشت درست;
    بازگشت نادرست;
  }

  عمومی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (conditions.sanitize (c, این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<ساختار Condition>	conditions;
  عمومی:
  DEFINE_SIZE_ARRAY (3, conditions);
};

ساختار ConditionNegate
{
  friend ساختار Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);
    اگر (condition.is_null ())
    {
      out->condition = 0;
      return_trace (درست);
    }
    return_trace (out->condition.serialize_subset (c, condition, این));
  }

  خصوصی:
  template <typename Instancer>
  منطقی evaluate (ثابت صحیح *coords, unsigned صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    بازگشت !_hb_recurse_condition_evaluate (این+condition,
					    coords, coord_len,
					    instancer);
  }

  عمومی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (condition.sanitize (c, این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<ساختار Condition>	condition;
  عمومی:
  DEFINE_SIZE_STATIC (5);
};

ساختار Condition
{
  friend ساختار ConditionSet;

  ایستا منطقی serialize_constant (hb_serialize_context_t *c, منطقی value)
  {
    auto *out = c->start_embed<Condition_مقدار> ();
    اگر (unlikely (!out || !c->extend_min (out))) بازگشت نادرست;
    out->format = 2;
    out->default_مقدار_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    بازگشت درست;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      منطقی materialize_true = نادرست) ثابت;

  template <typename Instancer>
  منطقی evaluate (ثابت صحیح *coords, unsigned صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    انتخاب (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    حالت 0: بازگشت درست;
    حالت 1: hb_barrier (); بازگشت u.format1.evaluate (coords, coord_len, instancer);
    حالت 2: hb_barrier (); بازگشت u.format2.evaluate (coords, coord_len, instancer);
    حالت 3: hb_barrier (); بازگشت u.format3.evaluate (coords, coord_len, instancer);
    حالت 4: hb_barrier (); بازگشت u.format4.evaluate (coords, coord_len, instancer);
    حالت 5: hb_barrier (); بازگشت u.format5.evaluate (coords, coord_len, instancer);
    پیشفرض:بازگشت نادرست;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) ثابت
  {
    اگر (unlikely (!depth))
    {
      c->apply = نادرست;
      بازگشت DROP_RECORD_WITH_VAR;
    }

    انتخاب (u.format.v) {
    حالت 0: بازگشت KEEP_RECORD_WITH_VAR;
    حالت 1: hb_barrier (); بازگشت u.format1.keep_with_variations (c, condition_map);
    حالت 2: حالت 3: حالت 4: حالت 5: بشکن;
    پیشفرض: c->apply = نادرست; بازگشت DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    منطقی applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    اگر (c->all_axes_pinned)
      بازگشت applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    اگر (u.format.v == 2)
      بازگشت u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (ثابت Condition &child)
    {
      auto child_context = *c;
      child_context.apply = درست;
      بازگشت child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    اگر (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      بازگشت result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    منطقی is_and = u.format.v == 3;
    منطقی variable = نادرست;
    ثابت auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ثابت تهی *base = is_and ? (ثابت تهی *) &u.format3 : (ثابت تهی *) &u.format4;
    برای (ثابت auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      اگر (is_and && result == DROP_RECORD_WITH_VAR)
        بازگشت DROP_RECORD_WITH_VAR;
      اگر (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        بازگشت KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    بازگشت variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالت 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);

    اگر (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_مقدار ());

    return_trace (c->end_recursion (این->dispatch (c)));
  }

  منطقی collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, صحیح>> *condition_مقادیر = nullptr) ثابت;

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت Condition *src,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map = nullptr);

  خصوصی:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) ثابت;

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_مقدار	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
منطقی
_hb_recurse_condition_evaluate (ثابت ساختار Condition &condition,
				ثابت صحیح *coords,
				unsigned صحیح coord_len,
				Instancer *instancer)
{
  بازگشت condition.evaluate (coords, coord_len, instancer);
}

ساختار ConditionList
{
  ثابت Condition& operator[] (unsigned i) ثابت
  { بازگشت این+conditions[i]; }

  unsigned get_شمار () ثابت { بازگشت conditions.len; }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت ConditionList *src,
		  ثابت hb_inc_bimap_t &condition_map,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    conditions.len = condition_map.get_population ();
    اگر (unlikely (!c->extend (conditions))) return_trace (نادرست);
    برای (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      اگر (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (نادرست);
    }
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (conditions.sanitize (c, این));
  }

  محافظتشده:
  Array32OfOffset32To<Condition> conditions;
  عمومی:
  DEFINE_SIZE_ARRAY (4, conditions);
};

درونخطی منطقی
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, صحیح>> *condition_مقادیر) ثابت
{
  اگر (unlikely (!depth)) بازگشت نادرست;
  انتخاب (u.format.v)
  {
    حالت 0:
    حالت 1:
      بازگشت درست;
    حالت 2:
      اگر (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        اگر (condition_مقادیر &&
            !condition_مقادیر->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), صحیح (u.format2.default_مقدار_2))))
          بازگشت نادرست;
      }
      بازگشت !var_indices->in_error ();
    حالت 3:
      برای (ثابت auto &offset : u.format3.conditions)
	اگر (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_مقادیر)))
	  بازگشت نادرست;
      بازگشت درست;
    حالت 4:
      برای (ثابت auto &offset : u.format4.conditions)
	اگر (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_مقادیر)))
	  بازگشت نادرست;
      بازگشت درست;
    حالت 5:
      بازگشت (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_مقادیر);
    پیشفرض:
      بازگشت نادرست;
  }
}

درونخطی منطقی
Condition::serialize (hb_serialize_context_t *c,
		      ثابت Condition *src,
		      ثابت hb_map_t &varidx_map,
		      ثابت hb_map_t *axis_map)
{
  TRACE_SERIALIZE (این);
  انتخاب (src->u.format.v)
  {
    حالت 0:
    {
      auto *out = c->start_embed<Condition_مقدار> ();
      اگر (unlikely (!out || !c->extend_min (out))) return_trace (نادرست);
      out->format = 2;
      out->default_مقدار_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (درست);
    }
    حالت 1:
    {
      auto *out = c->embed (&src->u.format1);
      اگر (unlikely (!out)) return_trace (نادرست);
      اگر (axis_map)
      {
	اگر (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (نادرست);
	}
	اگر (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (نادرست);
      }
      return_trace (درست);
    }
    حالت 2:
    {
      auto *out = c->embed (&src->u.format2);
      اگر (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (نادرست);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (درست);
    }
    حالت 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      اگر (unlikely (!c->extend_min (out))) return_trace (نادرست);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      اگر (unlikely (!c->extend (out->conditions))) return_trace (نادرست);
      برای (unsigned i = 0; i < out->conditions.len; i++)
	اگر (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (نادرست);
      return_trace (درست);
    }
    حالت 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      اگر (unlikely (!c->extend_min (out))) return_trace (نادرست);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      اگر (unlikely (!c->extend (out->conditions))) return_trace (نادرست);
      برای (unsigned i = 0; i < out->conditions.len; i++)
	اگر (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (نادرست);
      return_trace (درست);
    }
    حالت 5:
    {
      auto *out = c->embed (&src->u.format5);
      اگر (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (نادرست);
      return_trace (درست);
    }
    پیشفرض:
      return_trace (نادرست);
  }
}

درونخطی lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    منطقی materialize_true) ثابت
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  اگر (result == LOOKUP_CONDITION_SUBSET_KEEP)
    بازگشت result;

  c->serializer->revert (snap);
  اگر (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    بازگشت result;

  بازگشت serialize_constant (c->serializer, درست) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

درونخطی lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) ثابت
{
  اگر (unlikely (!depth))
    بازگشت LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   ثابت Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    اگر (unlikely (!offset))
      بازگشت LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    اگر (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->برداشت_pack ());
    وگرنه
    {
      c->serializer->برداشت_discard ();
      out.برداشت ();
      c->serializer->revert (snap);
    }
    بازگشت result;
  };

  انتخاب (u.format.v)
  {
    حالت 0:
      بازگشت LOOKUP_CONDITION_SUBSET_TRUE;

    حالت 1:
    {
      ثابت ConditionAxisRange &src = u.format1;
      اگر (c->plan->user_axes_location.is_empty ())
        بازگشت c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      اگر (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      اگر (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      اعشاری filter_min = src.filter_Range_Min_مقدار.to_float ();
      اعشاری filter_max = src.filter_Range_Max_مقدار.to_float ();
      اگر (filter_min > filter_max)
        بازگشت LOOKUP_CONDITION_SUBSET_FALSE;
      اگر (axis_limit.is_point ())
        بازگشت filter_min <= axis_limit.میانه &&
	       axis_limit.میانه <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      اگر (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        بازگشت LOOKUP_CONDITION_SUBSET_FALSE;
      اگر (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        بازگشت LOOKUP_CONDITION_SUBSET_TRUE;
      اگر (!c->plan->axes_index_map.has (src.axisIndex))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      اگر (unlikely (!out))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      اگر (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_مقدار.set_float (
	  renormalize_مقدار (filter_min, axis_limit, axis_distances, نادرست));
      out->filter_Range_Max_مقدار.set_float (
	  renormalize_مقدار (filter_max, axis_limit, axis_distances, نادرست));
      اگر (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;
      بازگشت LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالت 2:
    {
      ثابت Condition_مقدار &src = u.format2;
      auto *out = c->serializer->embed (&src);
      اگر (unlikely (!out))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;

      اگر (src.varIdx == VarIdx::NO_VARIATION)
        بازگشت c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحیح) src.default_مقدار_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, صحیح> *condition_مقدار;
      اگر (c->plan->layout_condition_idx_مقدار_map.has ((uintptr_t) &src, &condition_مقدار))
      {
        out->default_مقدار_2 = condition_مقدار->second;
        out->varIdx = condition_مقدار->first;
        اگر (out->varIdx == VarIdx::NO_VARIATION)
          بازگشت out->default_مقدار_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        بازگشت LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, صحیح> *new_varidx_delta;
      اگر (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        بازگشت c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحیح) src.default_مقدار_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      اگر (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        دوگانه value = (صحیح) src.default_مقدار_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        بازگشت value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      صحیح default_مقدار = (صحیح) src.default_مقدار_2 +
			  hb_second (*new_varidx_delta);
      اگر (unlikely (!c->serializer->check_assign (
		out->default_مقدار_2,
		default_مقدار,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      بازگشت LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالت 3:
    {
      ثابت ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      اگر (unlikely (!out || !c->serializer->extend_min (out)))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      برای (ثابت auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        اگر (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          بازگشت result;
      }
      بازگشت out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    حالت 4:
    {
      ثابت ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      اگر (unlikely (!out || !c->serializer->extend_min (out)))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      برای (ثابت auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        اگر (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          بازگشت result;
      }
      بازگشت out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    حالت 5:
    {
      ثابت ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      اگر (unlikely (!out))
        بازگشت LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      اگر (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->برداشت_pack ());
        بازگشت result;
      }

      c->serializer->برداشت_discard ();
      اگر (result == LOOKUP_CONDITION_SUBSET_TRUE)
        بازگشت LOOKUP_CONDITION_SUBSET_FALSE;
      اگر (result == LOOKUP_CONDITION_SUBSET_FALSE)
        بازگشت LOOKUP_CONDITION_SUBSET_TRUE;
      بازگشت result;
    }

    پیشفرض:
      بازگشت LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

ساختار ConditionSet
{
  منطقی evaluate (ثابت صحیح *coords, unsigned صحیح coord_len,
		 ItemVarStoreInstancer *instancer) ثابت
  {
    unsigned صحیح شمار = conditions.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!(این+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	بازگشت نادرست;
    بازگشت درست;
  }

  تهی keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    hb_map_t *condition_map = hb_map_create ();
    اگر (unlikely (!condition_map)) بازگشت;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    اگر (unlikely (!cond_set)) بازگشت;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = درست;
    منطقی should_keep = نادرست;
    /* The range map cannot identify value or boolean expressions uniquely. */
    منطقی can_deduplicate = درست;
    unsigned رقم_kept_cond = 0, cond_idx = 0;
    برای (ثابت auto& offset : conditions)
    {
      ثابت Condition &condition = این+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      اگر (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = نادرست;
        بازگشت;
      }

      اگر (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = درست;
        cond_set->add (cond_idx);
        رقم_kept_cond++;
      }

      اگر (ret == KEEP_RECORD_WITH_VAR)
        should_keep = درست;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = رقم_kept_cond == 0;
    اگر (!should_keep && c->all_axes_pinned) بازگشت;

    //check if condition_set is unique with variations
    اگر (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      بازگشت;

    اگر (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  منطقی subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               منطقی درج_catch_all) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!out || !c->serializer->extend_min (out))) return_trace (نادرست);

    اگر (درج_catch_all) return_trace (درست);

    hb_set_t *retained_cond_set = nullptr;
    اگر (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned صحیح شمار = conditions.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (retained_cond_set != nullptr && !retained_cond_set->has (i))
        ادامه;
      ثابت Condition &condition = این+conditions[i];
      اگر (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, این) (conditions[i]);
        ادامه;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      اگر (unlikely (!offset)) return_trace (نادرست);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, درست);
      اگر (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, نادرست) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      اگر (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->برداشت_discard ();
        c->serializer->check_success (نادرست);
        return_trace (نادرست);
      }
      c->serializer->add_link (*offset, c->serializer->برداشت_pack ());
    }

    return_trace (منطقی (out->conditions));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (conditions.sanitize (c, این));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت auto &offset : conditions)
      اگر (unlikely (!(این+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_مقادیر)))
	c->layout_variation_indices->err ();
  }

  محافظتشده:
  Array16OfOffset32To<Condition>	conditions;
  عمومی:
  DEFINE_SIZE_ARRAY (2, conditions);
};

ساختار FeatureTableSubstitutionRecord
{
  friend ساختار FeatureTableSubstitution;

  تهی collect_lookups (ثابت تهی *base, hb_set_t *lookup_indexes /* OUT */) ثابت
  {
    بازگشت (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  تهی closure_features (ثابت تهی *base,
			 ثابت hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ثابت
  {
    اگر ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  تهی collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, ثابت Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    ثابت hb_set_t *feature_indices,
                                                    ثابت تهی *base) ثابت
  {
    اگر (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  منطقی serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  ثابت Feature *f, ثابت Tag *tag)
  {
    TRACE_SERIALIZE (این);
    hb_serialize_context_t *s = c->subset_context->serializer;
    اگر (unlikely (!s->extend_min (این))) return_trace (نادرست);

    uint32_t *new_feature_idx;
    اگر (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (نادرست);

    اگر (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    s->push ();
    منطقی ret = f->subset (c->subset_context, c, tag);
    اگر (ret) s->add_link (feature, s->برداشت_pack ());
    وگرنه s->برداشت_discard ();

    return_trace (ret);
  }

  منطقی subset (hb_subset_layout_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SUBSET (این);
    uint32_t *new_feature_index;
    اگر (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (نادرست);

    auto *out = c->subset_context->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    out->featureIndex = *new_feature_index;
    hb_pair_t<ثابت تهی*, ثابت تهی*> *original;
    اگر (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (نادرست);
    ثابت Tag *tag = reinterpret_cast<ثابت Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این))) return_trace (نادرست);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ثابت Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  محافظتشده:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار FeatureTableSubstitution
{
  تهی collect_feature_indices (hb_set_t *feature_indices) ثابت
  {
    برای (ثابت FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  ثابت Feature *find_substitute (unsigned صحیح feature_index) ثابت
  {
    unsigned صحیح شمار = substitutions.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      ثابت FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      اگر (record.featureIndex == feature_index)
	بازگشت &(این+record.feature);
    }
    بازگشت nullptr;
  }

  تهی collect_lookups (ثابت hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ثابت
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([این, lookup_indexes] (ثابت FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (این, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  تهی collect_name_ids (ثابت hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ثابت
  {
    برای (ثابت FeatureTableSubstitutionRecord &record : substitutions)
      اگر (feature_index_map->has (record.featureIndex))
        (این+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  تهی closure_features (ثابت hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ثابت
  {
    برای (ثابت FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (این, lookup_indexes, feature_indexes);
  }

  منطقی intersects_features (ثابت hb_map_t *feature_index_map) ثابت
  {
    برای (ثابت FeatureTableSubstitutionRecord& record : substitutions)
    {
      اگر (feature_index_map->has (record.featureIndex)) بازگشت درست;
    }
    بازگشت نادرست;
  }

  تهی collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    برای (ثابت FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, این);
  }

  منطقی subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               منطقی درج_catch_all) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!out || !c->serializer->extend_min (out))) return_trace (نادرست);

    out->version.major = version.major;
    out->version.minor = version.minor;

    اگر (درج_catch_all)
    {
      برای (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<ثابت تهی*, ثابت تهی*> *p;
        اگر (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (نادرست);
        auto *o = out->substitutions.serialize_append (c->serializer);
        اگر (!o->serialize (l, feature_index,
                           reinterpret_cast<ثابت Feature*> (p->first),
                           reinterpret_cast<ثابت Tag*> (p->second)))
          return_trace (نادرست);
      }
      return_trace (درست);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), این))
    ;

    return_trace (منطقی (out->substitutions));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, این, feature_list));
  }

  محافظتشده:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  عمومی:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

ساختار FeatureVariationRecord
{
  friend ساختار FeatureVariations;

  تهی collect_lookups (ثابت تهی     *base,
			ثابت hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ثابت
  {
    بازگشت (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  تهی collect_name_ids (ثابت تهی *base,
                         ثابت hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ثابت
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  تهی closure_features (ثابت تهی     *base,
			 ثابت hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ثابت
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  منطقی intersects_features (ثابت تهی *base, ثابت hb_map_t *feature_index_map) ثابت
  {
    بازگشت (base+substitutions).intersects_features (feature_index_map);
  }

  تهی collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ثابت تهی *base) ثابت
  {
    (base+conditions).keep_with_variations (c);
    اگر (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = درست; // set variations only once
    }
  }

  منطقی subset (hb_subset_layout_context_t *c, ثابت تهی *base,
               منطقی درج_catch_all = نادرست) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->subset_context->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    اگر (!درج_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    وگرنه
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, درج_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, درج_catch_all);

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت تهی *base) ثابت
  { (base+conditions).collect_variation_indices (c); }

  محافظتشده:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار LookupIndexList
{
  تهی add_lookup_indexes_to (hb_set_t *lookup_indexes) ثابت
  { lookupIndices.add_indexes_to (lookup_indexes); }

  منطقی intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { بازگشت lookupIndices.intersects (lookup_indexes); }

  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (نادرست);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (منطقی (out->lookupIndices));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (lookupIndices.sanitize (c));
  }

  محافظتشده:
  IndexArray	lookupIndices;
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

ساختار LookupConditionRecord
{
  unsigned get_condition_offset (ثابت تهی *base, ثابت تهی *variations) ثابت
  { بازگشت (ثابت نویسه *) &(base+condition) - (ثابت نویسه *) variations; }

  template <typename Instancer>
  منطقی evaluate (ثابت تهی *base,
		 ثابت صحیح *coords,
		 unsigned صحیح coord_len,
		 Instancer *instancer) ثابت
  { بازگشت (base+condition).evaluate (coords, coord_len, instancer); }

  منطقی evaluate (ثابت تهی *base,
                 ثابت تهی *variations,
                 hb_sorted_array_t<ثابت unsigned> conditions) ثابت
  {
    بازگشت !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     اندازه (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  منطقی collect_condition (ثابت تهی *base,
                          ثابت تهی *variations,
                          ثابت صحیح *coords,
                          unsigned صحیح coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) ثابت
  {
    /* Null conditions always apply and need no key state. */
    اگر (!condition) بازگشت درست;

    unsigned offset = get_condition_offset (base, variations);
    اگر (visited->has (offset)) بازگشت درست;
    visited->add (offset);
    اگر (unlikely (visited->in_error ())) بازگشت نادرست;

    اگر (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    بازگشت !conditions->in_error ();
  }

  تهی add_lookup_indexes_to (ثابت تهی *base,
			      hb_set_t *lookup_indexes) ثابت
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  منطقی intersects_lookup_indexes (ثابت تهی *base,
				   ثابت hb_map_t *lookup_indexes) ثابت
  { بازگشت (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت تهی *base) ثابت
  {
    اگر (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_مقادیر)))
      c->layout_variation_indices->err ();
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ثابت تهی *base) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    درست);
    اگر (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->برداشت_discard ();
      return_trace (نادرست);
    }
    c->serializer->add_link (out->condition, c->serializer->برداشت_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  محافظتشده:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار FeatureLookupsTable
{
  منطقی adds_default_lookups () ثابت { بازگشت flags & 0x0001u; }

  منطقی collect_conditions (ثابت تهی *variations,
                           ثابت صحیح *coords,
                           unsigned صحیح coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) ثابت
  {
    برای (ثابت LookupConditionRecord &record : records)
      اگر (unlikely (!record.collect_condition (این, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	بازگشت نادرست;
    بازگشت درست;
  }

  تهی collect_lookups (ثابت تهی *variations,
                       hb_sorted_array_t<ثابت unsigned> conditions,
                       hb_set_t *lookup_indexes) ثابت
  {
    برای (ثابت LookupConditionRecord &record : records)
      اگر (record.evaluate (این, variations, conditions))
	record.add_lookup_indexes_to (این, lookup_indexes);
  }

  template <typename Instancer>
  تهی collect_lookups (ثابت صحیح *coords,
			unsigned صحیح coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) ثابت
  {
    برای (ثابت LookupConditionRecord &record : records)
      اگر (record.evaluate (این, coords, coord_len, instancer))
	record.add_lookup_indexes_to (این, lookup_indexes);
  }

  تهی collect_lookups (hb_set_t *lookup_indexes) ثابت
  {
    برای (ثابت LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (این, lookup_indexes);
  }

  منطقی intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  {
    برای (ثابت LookupConditionRecord &record : records)
      اگر (record.intersects_lookup_indexes (این, lookup_indexes))
	بازگشت درست;
    بازگشت نادرست;
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت LookupConditionRecord &record : records)
      record.collect_variation_indices (c, این);
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (نادرست);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    برای (ثابت LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      اگر (record.subset (c, l, این))
	out->records.len++;
      وگرنه
	c->serializer->revert (snap);
    }

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, این));
  }

  محافظتشده:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  عمومی:
  DEFINE_SIZE_ARRAY (10, records);
};

ساختار LookupVariationRecord
{
  friend ساختار FeatureVariations;

  unsigned get_feature_index () ثابت { بازگشت featureIndex; }

  تهی collect_lookups (ثابت تهی *base,
                       ثابت Feature &feature,
                       hb_sorted_array_t<ثابت unsigned> conditions,
                       hb_set_t *lookup_indexes) ثابت
  {
    ثابت FeatureLookupsTable &feature_lookups = base+featureLookups;
    اگر (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  تهی collect_lookups (ثابت تهی *base,
			ثابت hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) ثابت
  {
    اگر (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  تهی closure_features (ثابت تهی *base,
			 ثابت hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) ثابت
  {
    اگر ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت تهی *base) ثابت
  { (base+featureLookups).collect_variation_indices (c); }

  منطقی subset (hb_subset_layout_context_t *l, ثابت تهی *base) ثابت
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (این);
    uint32_t *new_feature_index;
    اگر (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (نادرست);

    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (نادرست);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  featureLookups.sanitize (c, base));
  }

  محافظتشده:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار FeatureVariations
{
  ایستا constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  ثابت Array32Of<LookupVariationRecord> &get_lookup_variation_records () ثابت
  {
    اگر (version.to_int () < 0x00010001u)
      بازگشت Null (Array32Of<LookupVariationRecord>);
    بازگشت StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_شمار () ثابت
  {
    بازگشت varRecords.len;
  }

  منطقی find_index (ثابت صحیح *coords, unsigned صحیح coord_len,
		   unsigned صحیح *index,
		   ItemVarStoreInstancer *instancer) ثابت
  {
    unsigned صحیح شمار = varRecords.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      ثابت FeatureVariationRecord &record = varRecords.arrayZ[i];
      اگر ((این+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	بازگشت درست;
      }
    }
    *index = NOT_FOUND_INDEX;
    بازگشت نادرست;
  }

  ثابت Feature *find_substitute (unsigned صحیح variations_index,
				  unsigned صحیح feature_index) ثابت
  {
    ثابت FeatureVariationRecord &record = varRecords[variations_index];
    بازگشت (این+record.substitutions).find_substitute (feature_index);
  }

  منطقی collect_lookup_conditions (ثابت صحیح *coords,
                                  unsigned صحیح coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) ثابت
  {
    hb_set_t visited_tables, visited_conditions;
    برای (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      اگر (visited_tables.has (offset)) ادامه;
      visited_tables.add (offset);
      اگر (unlikely (visited_tables.in_error ())) بازگشت نادرست;

      ثابت FeatureLookupsTable &feature_lookups = این+record.featureLookups;
      اگر (unlikely (!feature_lookups.collect_conditions (این, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	بازگشت نادرست;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    بازگشت درست;
  }

  منطقی get_lookup_variations (unsigned feature_index,
                              ثابت Feature &feature,
                              hb_sorted_array_t<ثابت unsigned> conditions,
                              hb_set_t *lookup_indexes) ثابت
  {
    برای (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
    {
      اگر (record.featureIndex != feature_index) ادامه;

      record.collect_lookups (این, feature, conditions, lookup_indexes);
      بازگشت درست;
    }
    بازگشت نادرست;
  }

  تهی collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    unsigned صحیح شمار = varRecords.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, این);
      اگر (c->universal)
        بشکن;
    }
    اگر (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed (*این));
  }

  تهی collect_lookups (ثابت hb_set_t *feature_indexes,
			ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) ثابت
  {
    unsigned شمار = varRecords.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        ادامه;
      varRecords[i].collect_lookups (این, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  تهی collect_name_ids (ثابت hb_map_t *feature_index_map,
                         ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ثابت
  {
    برای (unsigned i = 0; i < varRecords.len; i++)
    {
      اگر (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        ادامه;
      varRecords[i].collect_name_ids (این, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  تهی collect_lookup_variation_lookups (ثابت hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) ثابت
  {
    برای (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (این, feature_indexes, lookup_indexes);
  }

  تهی closure_features (ثابت hb_map_t *lookup_indexes,
			 ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) ثابت
  {
    unsigned صحیح شمار = varRecords.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        ادامه;
      varRecords[i].closure_features (این, lookup_indexes, feature_indexes);
    }

    برای (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (این, lookup_indexes, feature_indexes);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (ثابت FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, این);
    برای (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, این);
  }

  تهی collect_feature_indices (hb_set_t *feature_indices) ثابت
  {
    برای (ثابت FeatureVariationRecord &record : varRecords)
      (این+record.substitutions).collect_feature_indices (feature_indices);
  }

  منطقی has_lookup_variations (unsigned feature_index) ثابت
  {
    برای (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
    {
      اگر (record.featureIndex == feature_index)
	بازگشت درست;
      اگر (record.featureIndex > feature_index)
	بشکن;
    }
    بازگشت نادرست;
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!out || !c->serializer->extend_min (out))) return_trace (نادرست);

    out->version.major = version.major;
    out->version.minor = version.minor;

    صحیح keep_up_to = -1;
    برای (صحیح i = varRecords.len - 1; i >= 0; i--) {
      اگر (varRecords[i].intersects_features (این, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        بشکن;
      }
    }

    unsigned شمار = (unsigned) (keep_up_to + 1);
    برای (unsigned i = 0; i < شمار; i++)
    {
      اگر (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        ادامه;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), این) (varRecords[i]);
    }

    اگر (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      منطقی درج_catch_all_record = درست;
      subset_record_array (l, &(out->varRecords), این, درج_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    اگر (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      اگر (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (نادرست);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, این))
      ;
    }

    return_trace (منطقی (out->varRecords) ||
		  (out_lookup_records && منطقی (*out_lookup_records)));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, این, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, این)));
  }

  محافظتشده:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  عمومی:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

ساختار HintingDevice
{
  friend ساختار Device;

  خصوصی:

  hb_position_t get_x_delta (hb_font_t *font) ثابت
  { بازگشت get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ثابت
  { بازگشت get_delta (font->y_ppem, font->y_scale); }

  عمومی:

  size_t get_size () ثابت
  {
    unsigned صحیح f = deltaFormat;
    اگر (unlikely (f < 1 || f > 3 || startSize > endSize)) بازگشت 3 * HBUINT16::static_size;
    بازگشت HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && c->check_range (این, این->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed<HintingDevice> (این));
  }

  خصوصی:

  صحیح get_delta (unsigned صحیح ppem, صحیح scale) ثابت
  {
    اگر (!ppem) بازگشت 0;

    صحیح pixels = get_delta_pixels (ppem);

    اگر (!pixels) بازگشت 0;

    بازگشت (صحیح) (pixels * (int64_t) scale / ppem);
  }
  صحیح get_delta_pixels (unsigned صحیح ppem_size) ثابت
  {
    unsigned صحیح f = deltaFormat;
    اگر (unlikely (f < 1 || f > 3))
      بازگشت 0;

    اگر (ppem_size < startSize || ppem_size > endSize)
      بازگشت 0;

    unsigned صحیح s = ppem_size - startSize;

    unsigned صحیح byte = delta_مقدار_Z[s >> (4 - f)];
    unsigned صحیح bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned صحیح mask = (0xFFFFu >> (16 - (1 << f)));

    صحیح delta = bits & mask;

    اگر ((unsigned صحیح) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    بازگشت delta;
  }

  محافظتشده:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_مقدار_Z;		/* Array of compressed data */
  عمومی:
  DEFINE_SIZE_ARRAY (6, delta_مقدار_Z);
};

ساختار VariationDevice
{
  friend ساختار Device;

  خصوصی:

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ثابت
  { بازگشت !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ثابت
  { بازگشت !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  اعشاری get_x_delta_f (hb_font_t *font,
		       ثابت ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { بازگشت !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  اعشاری get_y_delta_f (hb_font_t *font,
		       ثابت ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { بازگشت !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map) ثابت
  {
    TRACE_SERIALIZE (این);
    اگر (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, صحیح> *v;
    اگر (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    اگر (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (این->static_size);
    auto *out = c->embed (این);
    اگر (unlikely (!out)) return_trace (nullptr);

    اگر (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  تهی collect_variation_index (hb_collect_variation_indices_context_t *c) ثابت
  { c->layout_variation_indices->add (varIdx); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  خصوصی:

  اعشاری get_delta (hb_font_t *font,
		   ثابت ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    بازگشت store.get_delta (varIdx, font->coords, font->رقم_coords, store_cache);
  }

  محافظتشده:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار DeviceHeader
{
  محافظتشده:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  عمومی:
  HBUINT16		format;		/* Format identifier */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  اعشاری get_x_delta_f (hb_font_t *font,
		       ثابت ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
#ifndef HB_NO_VAR
    اگر (u.b.format == 0x8000)
    {
      hb_barrier ();
      بازگشت u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    بازگشت get_x_delta (font, store, store_cache);
  }

  اعشاری get_y_delta_f (hb_font_t *font,
		       ثابت ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
#ifndef HB_NO_VAR
    اگر (u.b.format == 0x8000)
    {
      hb_barrier ();
      بازگشت u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    بازگشت get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    انتخاب (u.b.format)
    {
#ifndef HB_NO_HINTING
    حالت 1: حالت 2: حالت 3:
      hb_barrier ();
      بازگشت u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      بازگشت u.variation.get_x_delta (font, store, store_cache);
#endif
    پیشفرض:
      بازگشت 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    انتخاب (u.b.format)
    {
    حالت 1: حالت 2: حالت 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      بازگشت u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      بازگشت u.variation.get_y_delta (font, store, store_cache);
#endif
    پیشفرض:
      بازگشت 0;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.b.format.sanitize (c)) return_trace (نادرست);
    انتخاب (u.b.format) {
#ifndef HB_NO_HINTING
    حالت 1: حالت 2: حالت 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    پیشفرض:
      return_trace (درست);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map=nullptr) ثابت
  {
    TRACE_SERIALIZE (این);
    انتخاب (u.b.format) {
#ifndef HB_NO_HINTING
    حالت 1:
    حالت 2:
    حالت 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    پیشفرض:
      return_trace (nullptr);
    }
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    انتخاب (u.b.format) {
#ifndef HB_NO_HINTING
    حالت 1:
    حالت 2:
    حالت 3:
      بازگشت;
#endif
#ifndef HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      بازگشت;
#endif
    پیشفرض:
      بازگشت;
    }
  }

  unsigned get_variation_index () ثابت
  {
    انتخاب (u.b.format) {
#ifndef HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      بازگشت u.variation.varIdx;
#endif
    پیشفرض:
      بازگشت HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  منطقی is_variation_device () ثابت
  {
    انتخاب (u.b.format) {
#ifndef HB_NO_VAR
    حالت 0x8000:
      بازگشت درست;
#endif
    پیشفرض:
      بازگشت نادرست;
    }
  }

  محافظتشده:
  اجتماع {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  عمومی:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

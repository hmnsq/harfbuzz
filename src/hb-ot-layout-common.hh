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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-ot-layout.hh"
#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-set.hh"
#অন্তর্ভাৱ "hb-bimap.hh"
#অন্তর্ভাৱ "hb-cache.hh"
#অন্তর্ভাৱ "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (পরিমাণ (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_দ্বিমিক_cache_t = hb_cache_t<14, 1, 8>;
static_assert (পরিমাণ (hb_ot_layout_দ্বিমিক_cache_t) == 256, "");

#অন্তর্ভাৱ "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


নামাকাশ OT {

template<typename Iterator>
স্থির অন্তঃস্থ তার্কিক ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

স্থির তার্কিক ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ধ্রুৱ hb_set_t &klasses,
    তার্কিক use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

সংরচনা ItemVarStoreInstancer;

সংরচনা hb_collect_feature_substitutes_with_var_context_t
{
  ধ্রুৱ hb_map_t *axes_index_tag_map;
  ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, ধ্রুৱ Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  তার্কিক apply;
  তার্কিক variation_applied;
  তার্কিক universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  তার্কিক all_axes_pinned;
};

সংরচনা hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ধ্রুৱ শূন্য         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              ধ্রুৱ hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_গুনতি (0),langsys_feature_গুনতি (0) {}

  তার্কিক visitScript ()
  { ঘূৰাই script_গুনতি++ < HB_MAX_SCRIPTS; }

  তার্কিক visitLangsys (unsigned feature_গুনতি_2)
  {
    langsys_feature_গুনতি += feature_গুনতি_2;
    ঘূৰাই langsys_feature_গুনতি < HB_MAX_LANGSYS_FEATURE_গুনতি;
  }

  সার্ৱজনিক:
  ধ্রুৱ শূন্য *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ধ্রুৱ hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  গুপ্ত:
  unsigned script_গুনতি;
  unsigned langsys_feature_গুনতি;
};

সংরচনা hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  ধ্রুৱ ৱর্ণ *get_name () { ঘূৰাই "SUBSET_LAYOUT"; }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }

  তার্কিক visitScript ()
  {
    ঘূৰাই script_গুনতি++ < HB_MAX_SCRIPTS;
  }

  তার্কিক visitLangSys ()
  {
    ঘূৰাই langsys_গুনতি++ < HB_MAX_LANGSYS;
  }

  তার্কিক visitFeatureIndex (পূর্ণাঙ্ক গুনতি)
  {
    feature_index_গুনতি += গুনতি;
    ঘূৰাই feature_index_গুনতি < HB_MAX_FEATURE_INDICES;
  }

  তার্কিক visitLookupIndex()
  {
    lookup_index_গুনতি++;
    ঘূৰাই lookup_index_গুনতি < HB_MAX_LOOKUP_VISIT_গুনতি;
  }

  hb_subset_context_t *subset_context;
  ধ্রুৱ hb_tag_t table_tag;
  ধ্রুৱ hb_map_t *lookup_index_map;
  ধ্রুৱ hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ধ্রুৱ hb_map_t *feature_index_map;
  ধ্রুৱ hb_map_t *feature_map_w_duplicates;
  ধ্রুৱ hb_hashmap_t<unsigned, ধ্রুৱ Feature*> *feature_substitutes_map;
  ধ্রুৱ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  ধ্রুৱ hb_set_t *catch_all_record_feature_idxes;
  ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<ধ্রুৱ শূন্য*, ধ্রুৱ শূন্য*>> *feature_idx_tag_map;

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
				script_গুনতি (0),
				langsys_গুনতি (0),
				feature_index_গুনতি (0),
				lookup_index_গুনতি (0)
  {
    যদি (tag_ == HB_OT_TAG_GSUB)
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
    অন্যথা
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

  গুপ্ত:
  unsigned script_গুনতি;
  unsigned langsys_গুনতি;
  unsigned feature_index_গুনতি;
  unsigned lookup_index_গুনতি;
};

সংরচনা ItemVariationStore;
সংরচনা hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { obj.collect_variation_indices (এতত্); ঘূৰাই hb_empty_t (); }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  ধ্রুৱ hb_set_t *glyph_set;
  ধ্রুৱ hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *condition_মানসমূহ = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  ধ্রুৱ hb_set_t *glyph_set_,
					  ধ্রুৱ hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
সংরচনা subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 ধ্রুৱ শূন্য *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  তার্কিক operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    যদি (unlikely (!o)) ঘূৰাই মিছা;
    তার্কিক ret = o->serialize_subset (subset_context, offset, base);
    যদি (!ret)
    {
      out.বেৰ_কৰো ();
      subset_context->serializer->revert (snap);
    }
    ঘূৰাই ret;
  }

  গুপ্ত:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ধ্রুৱ শূন্য *base;
};


template<typename OutputArray, typename Arg>
সংরচনা subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     ধ্রুৱ শূন্য *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  তার্কিক operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    যদি (unlikely (!o)) ঘূৰাই মিছা;
    তার্কিক ret = o->serialize_subset (subset_context, offset, base, arg);
    যদি (!ret)
    {
      out.বেৰ_কৰো ();
      subset_context->serializer->revert (snap);
    }
    ঘূৰাই ret;
  }

  গুপ্ত:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ধ্রুৱ শূন্য *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
সংরচনা
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ধ্রুৱ শূন্য *base) ধ্রুৱ
  { ঘূৰাই subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ধ্রুৱ শূন্য *base, Arg &&arg) ধ্রুৱ
  { ঘূৰাই subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
সংরচনা subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 ধ্রুৱ শূন্য *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  শূন্য
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    তার্কিক ret = record.subset (subset_layout_context, base);
    যদি (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    অন্যথা out->len++;
  }

  গুপ্ত:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ধ্রুৱ শূন্য *base;
};

template<typename OutputArray, typename Arg>
সংরচনা subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     ধ্রুৱ শূন্য *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  শূন্য
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    তার্কিক ret = record.subset (subset_layout_context, base, arg);
    যদি (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    অন্যথা out->len++;
  }

  গুপ্ত:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ধ্রুৱ শূন্য *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
সংরচনা
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       ধ্রুৱ শূন্য *base) ধ্রুৱ
  { ঘূৰাই subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               ধ্রুৱ শূন্য *base, Arg &&arg) ধ্রুৱ
  { ঘূৰাই subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
সংরচনা serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         ধ্রুৱ শূন্য *base_,
                         ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  তার্কিক operator () (T&& record)
  {
    যদি (!serialize_context->copy (record, base, variation_map)) ঘূৰাই মিছা;
    out.len++;
    ঘূৰাই সঁচা;
  }

  গুপ্ত:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  ধ্রুৱ শূন্য *base;
  ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
সংরচনা
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               ধ্রুৱ শূন্য *base,
               ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *variation_map) ধ্রুৱ
  { ঘূৰাই serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

সংরচনা IndexArray : Array16Of<Index>
{
  তার্কিক intersects (ধ্রুৱ hb_map_t *indexes) ধ্রুৱ
  { ঘূৰাই hb_any (*এতত্, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    যদি (!it) ঘূৰাই;
    যদি (unlikely (!c->extend_min ((*এতত্)))) ঘূৰাই;

    বাবে (ধ্রুৱ auto _ : it)
    {
      যদি (!l->visitLookupIndex()) ভঙ্গ;

      Index i;
      i = _;
      c->copy (i);
      এতত্->len++;
    }
  }

  unsigned পূর্ণাঙ্ক get_indexes (unsigned পূর্ণাঙ্ক start_offset,
			    unsigned পূর্ণাঙ্ক *গুনতি_2 /* IN/OUT */,
			    unsigned পূর্ণাঙ্ক *_indexes /* OUT */) ধ্রুৱ
  {
    যদি (গুনতি_2 && _indexes)
    {
      + এতত্->as_array ().sub_array (start_offset, গুনতি_2)
      | hb_sink (hb_array (_indexes, *গুনতি_2))
      ;
    }
    ঘূৰাই এতত্->len;
  }

  শূন্য add_indexes_to (hb_set_t* output /* OUT */) ধ্রুৱ
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
সংরচনা FeatureParamsSize
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্))) return_trace (মিছা);
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

    যদি (!designSize)
      return_trace (মিছা);
    অন্যথা যদি (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (সঁচা);
    অন্যথা যদি (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (মিছা);
    অন্যথা
      return_trace (সঁচা);
  }

  শূন্য collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ধ্রুৱ
  { nameids_to_retain->add (subfamilyNameID); }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    return_trace ((তার্কিক) c->serializer->embed (*এতত্));
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
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
সংরচনা FeatureParamsStylisticSet
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (এতত্));
  }

  শূন্য collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ধ্রুৱ
  { nameids_to_retain->add (uiNameID); }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    return_trace ((তার্কিক) c->serializer->embed (*এতত্));
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
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
সংরচনা FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_গুনতি, hb_codepoint_t *chars) ধ্রুৱ
  {
    যদি (char_গুনতি && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_গুনতি)
      | hb_sink (hb_array (chars, *char_গুনতি))
      ;
    }
    ঘূৰাই characters.len;
  }

  size_t get_size () ধ্রুৱ
  { ঘূৰাই hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  শূন্য collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ধ্রুৱ
  {
    যদি (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    যদি (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    যদি (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    যদি (!firstParamUILabelNameID || !অঙ্ক_Named_Parameters || অঙ্ক_Named_Parameters >= 0x7FFF)
      ঘূৰাই;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) অঙ্ক_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    return_trace ((তার্কিক) c->serializer->embed (*এতত্));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
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
  HBUINT16	অঙ্ক_Named_Parameters;	/* Number of named parameters. (May
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
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (14, characters);
};

সংরচনা FeatureParams
{
  তার্কিক sanitize (hb_sanitize_context_t *c, hb_tag_t tag) ধ্রুৱ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ঘূৰাই সঁচা;
#endif
    TRACE_SANITIZE (এতত্);
    যদি (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (সঁচা);
  }

  শূন্য collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ধ্রুৱ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ঘূৰাই;
#endif
    যদি (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ঘূৰাই (u.size.collect_name_ids (nameids_to_retain));
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ঘূৰাই (u.stylisticSet.collect_name_ids (nameids_to_retain));
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ঘূৰাই (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  তার্কিক subset (hb_subset_context_t *c, ধ্রুৱ Tag* tag) ধ্রুৱ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ঘূৰাই মিছা;
#endif
    TRACE_SUBSET (এতত্);
    যদি (!tag) return_trace (মিছা);
    যদি (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    যদি ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    যদি ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (মিছা);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  ধ্রুৱ FeatureParamsSize& get_size_params (hb_tag_t tag) ধ্রুৱ
  {
    যদি (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ঘূৰাই u.size;
    ঘূৰাই Null (FeatureParamsSize);
  }
  ধ্রুৱ FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) ধ্রুৱ
  {
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ঘূৰাই u.stylisticSet;
    ঘূৰাই Null (FeatureParamsStylisticSet);
  }
  ধ্রুৱ FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) ধ্রুৱ
  {
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ঘূৰাই u.characterVariants;
    ঘূৰাই Null (FeatureParamsCharacterVariants);
  }
#endif

  গুপ্ত:
  সঙ্ঘ {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (0);
};

সংরচনা Record_sanitize_closure_t {
  hb_tag_t tag;
  ধ্রুৱ শূন্য *list_base;
};

সংরচনা Feature
{
  unsigned পূর্ণাঙ্ক get_lookup_গুনতি () ধ্রুৱ
  { ঘূৰাই lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই lookupIndex[i]; }
  unsigned পূর্ণাঙ্ক get_lookup_indexes (unsigned পূর্ণাঙ্ক start_index,
				   unsigned পূর্ণাঙ্ক *lookup_গুনতি /* IN/OUT */,
				   unsigned পূর্ণাঙ্ক *lookup_tags /* OUT */) ধ্রুৱ
  { ঘূৰাই lookupIndex.get_indexes (start_index, lookup_গুনতি, lookup_tags); }
  শূন্য add_lookup_indexes_to (hb_set_t *lookup_indexes) ধ্রুৱ
  { lookupIndex.add_indexes_to (lookup_indexes); }

  ধ্রুৱ FeatureParams &get_feature_params () ধ্রুৱ
  { ঘূৰাই এতত্+featureParams; }

  তার্কিক intersects_lookup_indexes (ধ্রুৱ hb_map_t *lookup_indexes) ধ্রুৱ
  { ঘূৰাই lookupIndex.intersects (lookup_indexes); }

  শূন্য collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ধ্রুৱ
  {
    যদি (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  তার্কিক subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ধ্রুৱ Tag                   *tag = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    out->featureParams.serialize_subset (c, featureParams, এতত্, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    যদি (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    অন্যথা
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
    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুৱ Record_sanitize_closure_t *closure = nullptr) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  featureParams.sanitize (c, এতত্, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
সংরচনা Record
{
  পূর্ণাঙ্ক cmp (hb_tag_t a) ধ্রুৱ { ঘূৰাই tag.cmp (a); }

  তার্কিক subset (hb_subset_layout_context_t *c, ধ্রুৱ শূন্য *base, ধ্রুৱ শূন্য *f_sub = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->subset_context->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    ধ্রুৱ Feature& f = *reinterpret_cast<ধ্রুৱ Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    তার্কিক ret = f.subset (c->subset_context, c, &tag);
    যদি (ret)
      s->add_link (out->offset, s->বেৰ_কৰো_pack ());
    অন্যথা
      s->বেৰ_কৰো_discard ();

    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    ধ্রুৱ Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (এতত্) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
সংরচনা RecordArrayOf : SortedArray16Of<Record<Type>>
{
  ধ্রুৱ Offset16To<Type>& get_offset (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই (*এতত্)[i].offset; }
  Offset16To<Type>& get_offset (unsigned পূর্ণাঙ্ক i)
  { ঘূৰাই (*এতত্)[i].offset; }
  ধ্রুৱ Tag& get_tag (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই (*এতত্)[i].tag; }
  unsigned পূর্ণাঙ্ক get_tags (unsigned পূর্ণাঙ্ক start_offset,
                         unsigned পূর্ণাঙ্ক *record_গুনতি /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) ধ্রুৱ
  {
    যদি (record_গুনতি && record_tags)
    {
      + এতত্->as_array ().sub_array (start_offset, record_গুনতি)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_গুনতি))
      ;
    }
    ঘূৰাই এতত্->len;
  }
  তার্কিক find_index (hb_tag_t tag, unsigned পূর্ণাঙ্ক *index) ধ্রুৱ
  {
    ঘূৰাই এতত্->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
সংরচনা RecordListOf : RecordArrayOf<Type>
{
  ধ্রুৱ Type& operator [] (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই এতত্+এতত্->get_offset (i); }

  তার্কিক subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    + এতত্->iter ()
    | hb_apply (subset_record_array (l, out, এতত্))
    ;
    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (RecordArrayOf<Type>::sanitize (c, এতত্));
  }
};

সংরচনা RecordListOfFeature : RecordListOf<Feature>
{
  তার্কিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    + hb_enumerate (*এতত্)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, এতত্] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ Record<Feature>&>& _)
                {
                  ধ্রুৱ Feature *f_sub = nullptr;
                  ধ্রুৱ Feature **f = nullptr;
                  যদি (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, এতত্, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (সঁচা);
  }
};

প্রকারনির্দেশ RecordListOf<Feature> FeatureList;


সংরচনা LangSys
{
  unsigned পূর্ণাঙ্ক get_feature_গুনতি () ধ্রুৱ
  { ঘূৰাই featureIndex.len; }
  hb_tag_t get_feature_index (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই featureIndex[i]; }
  unsigned পূর্ণাঙ্ক get_feature_indexes (unsigned পূর্ণাঙ্ক start_offset,
				    unsigned পূর্ণাঙ্ক *feature_গুনতি_2 /* IN/OUT */,
				    unsigned পূর্ণাঙ্ক *feature_indexes /* OUT */) ধ্রুৱ
  { ঘূৰাই featureIndex.get_indexes (start_offset, feature_গুনতি_2, feature_indexes); }
  শূন্য add_feature_indexes_to (hb_set_t *feature_indexes) ধ্রুৱ
  { featureIndex.add_indexes_to (feature_indexes); }

  তার্কিক has_required_feature () ধ্রুৱ { ঘূৰাই reqFeatureIndex != 0xFFFFu; }
  unsigned পূর্ণাঙ্ক get_required_feature_index () ধ্রুৱ
  {
    যদি (reqFeatureIndex == 0xFFFFu)
      ঘূৰাই Index::NOT_FOUND_INDEX;
   ঘূৰাই reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed (*এতত্));
  }

  তার্কিক compare (ধ্রুৱ LangSys& o, ধ্রুৱ hb_map_t *feature_index_map) ধ্রুৱ
  {
    যদি (reqFeatureIndex != o.reqFeatureIndex)
      ঘূৰাই মিছা;

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

    বাবে (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      যদি (a != b) ঘূৰাই মিছা;
    }

    যদি (iter || o_iter) ঘূৰাই মিছা;

    ঘূৰাই সঁচা;
  }

  শূন্য collect_features (hb_prune_langsys_context_t *c) ধ্রুৱ
  {
    যদি (!has_required_feature () && !get_feature_গুনতি ()) ঘূৰাই;
    যদি (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  তার্কিক subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ধ্রুৱ Tag                  *tag = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    ধ্রুৱ uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    যদি (!l->visitFeatureIndex (featureIndex.len))
      return_trace (মিছা);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    তার্কিক ret = তার্কিক (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুৱ Record_sanitize_closure_t * = nullptr) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

সংরচনা Script
{
  unsigned পূর্ণাঙ্ক get_lang_sys_গুনতি () ধ্রুৱ
  { ঘূৰাই langSys.len; }
  ধ্রুৱ Tag& get_lang_sys_tag (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই langSys.get_tag (i); }
  unsigned পূর্ণাঙ্ক get_lang_sys_tags (unsigned পূর্ণাঙ্ক start_offset,
				  unsigned পূর্ণাঙ্ক *lang_sys_গুনতি /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ধ্রুৱ
  { ঘূৰাই langSys.get_tags (start_offset, lang_sys_গুনতি, lang_sys_tags); }
  ধ্রুৱ LangSys& get_lang_sys (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  {
    যদি (i == Index::NOT_FOUND_INDEX) ঘূৰাই get_default_lang_sys ();
    ঘূৰাই এতত্+langSys[i].offset;
  }
  তার্কিক find_lang_sys_index (hb_tag_t tag, unsigned পূর্ণাঙ্ক *index) ধ্রুৱ
  { ঘূৰাই langSys.find_index (tag, index); }

  তার্কিক has_default_lang_sys () ধ্রুৱ           { ঘূৰাই defaultLangSys != 0; }
  ধ্রুৱ LangSys& get_default_lang_sys () ধ্রুৱ { ঘূৰাই এতত্+defaultLangSys; }

  শূন্য prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) ধ্রুৱ
  {
    যদি (!has_default_lang_sys () && !get_lang_sys_গুনতি ()) ঘূৰাই;
    যদি (!c->visitScript ()) ঘূৰাই;

    যদি (!c->script_langsys_map->has (script_index))
    {
      যদি (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	ঘূৰাই;
    }

    যদি (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ধ্রুৱ LangSys& d = get_default_lang_sys ();
      যদি (c->visitLangsys (d.get_feature_গুনতি ())) {
        d.collect_features (c);
      }

      বাবে (auto _ : + hb_enumerate (langSys))
      {
        ধ্রুৱ LangSys& l = এতত্+_.second.offset;
        যদি (!c->visitLangsys (l.get_feature_গুনতি ())) অনুৱর্ত;
        যদি (l.compare (d, c->duplicate_feature_map)) অনুৱর্ত;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    অন্যথা
    {
      বাবে (auto _ : + hb_enumerate (langSys))
      {
        ধ্রুৱ LangSys& l = এতত্+_.second.offset;
        যদি (!c->visitLangsys (l.get_feature_গুনতি ())) অনুৱর্ত;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  তার্কিক subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ধ্রুৱ Tag                   *tag) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (!l->visitScript ()) return_trace (মিছা);
    যদি (tag && !c->plan->layout_scripts.has (*tag))
      ঘূৰাই মিছা;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    তার্কিক defaultLang = মিছা;
    যদি (has_default_lang_sys ())
    {
      c->serializer->push ();
      ধ্রুৱ LangSys& ls = এতত্+defaultLangSys;
      তার্কিক ret = ls.subset (c, l);
      যদি (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->বেৰ_কৰো_discard ();
	out->defaultLangSys = 0;
      }
      অন্যথা
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->বেৰ_কৰো_pack ());
	defaultLang = সঁচা;
      }
    }

    ধ্রুৱ hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    যদি (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (ধ্রুৱ Record<LangSys>& record) {ঘূৰাই l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), এতত্))
      ;
    }

    return_trace (তার্কিক (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুৱ Record_sanitize_closure_t * = nullptr) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (defaultLangSys.sanitize (c, এতত্) && langSys.sanitize (c, এতত্));
  }

  রক্ষিত:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

সংরচনা RecordListOfScript : RecordListOf<Script>
{
  তার্কিক subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    বাবে (auto _ : + hb_enumerate (*এতত্))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      তার্কিক ret = _.second.subset (l, এতত্);
      যদি (!ret) c->serializer->revert (snap);
      অন্যথা out->len++;
    }

    return_trace (সঁচা);
  }
};

প্রকারনির্দেশ RecordListOfScript ScriptList;



সংরচনা LookupFlag : HBUINT16
{
  গণনা Flags {
    ডান_To_বাম		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
নামাকাশ OT {

সংরচনা Lookup
{
  unsigned পূর্ণাঙ্ক get_subtable_গুনতি () ধ্রুৱ { ঘূৰাই subTable.len; }

  template <typename TSubTable>
  ধ্রুৱ Array16OfOffset16To<TSubTable>& get_subtables () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { ঘূৰাই reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  ধ্রুৱ TSubTable& get_subtable (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই এতত্+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned পূর্ণাঙ্ক i)
  { ঘূৰাই এতত্+get_subtables<TSubTable> ()[i]; }

  size_t get_size () ধ্রুৱ
  {
    ধ্রুৱ HBUINT16 &markFilteringSet = StructAfter<ধ্রুৱ HBUINT16> (subTable);
    যদি (lookupFlag & LookupFlag::UseMarkFilteringSet)
      ঘূৰাই (ধ্রুৱ ৱর্ণ *) &StructAfter<ধ্রুৱ ৱর্ণ> (markFilteringSet) - (ধ্রুৱ ৱর্ণ *) এতত্;
    ঘূৰাই (ধ্রুৱ ৱর্ণ *) &markFilteringSet - (ধ্রুৱ ৱর্ণ *) এতত্;
  }

  unsigned পূর্ণাঙ্ক get_type () ধ্রুৱ { ঘূৰাই lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক flag = lookupFlag;
    যদি (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      ধ্রুৱ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    ঘূৰাই flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক lookup_type = get_type ();
    TRACE_DISPATCH (এতত্, lookup_type);
    unsigned পূর্ণাঙ্ক গুনতি = get_subtable_গুনতি ();
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      যদি (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_মান ());
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  unsigned পূর্ণাঙ্ক lookup_type,
		  uint32_t lookup_props,
		  unsigned পূর্ণাঙ্ক অঙ্ক_subtables)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    যদি (unlikely (!subTable.serialize (c, অঙ্ক_subtables))) return_trace (মিছা);
    যদি (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      যদি (unlikely (!c->extend (এতত্))) return_trace (মিছা);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (সঁচা);
  }

  template <typename TSubTable>
  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    ধ্রুৱ hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned পূর্ণাঙ্ক lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([এতত্, glyphset, lookup_type] (ধ্রুৱ Offset16To<TSubTable> &_) { ঘূৰাই (এতত্+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), এতত্, lookup_type))
    ;

    যদি (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ধ্রুৱ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      যদি (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      অন্যথা
      {
        যদি (unlikely (!c->serializer->extend (out))) return_trace (মিছা);
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
    return_trace (সঁচা);
  }

  template <typename TSubTable>
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!(c->check_struct (এতত্) && subTable.sanitize (c))) return_trace (মিছা);
    hb_barrier ();

    unsigned subtables = get_subtable_গুনতি ();
    যদি (unlikely (!c->visit_subtables (subtables))) return_trace (মিছা);

    যদি (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ধ্রুৱ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      যদি (!markFilteringSet.sanitize (c)) return_trace (মিছা);
    }

    যদি (unlikely (!get_subtables<TSubTable> ().sanitize (c, এতত্, get_type ())))
      return_trace (মিছা);

    যদি (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned পূর্ণাঙ্ক type = get_subtable<TSubTable> (0).u.extension.get_type ();
      বাবে (unsigned পূর্ণাঙ্ক i = 1; i < subtables; i++)
	যদি (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (মিছা);
    }
    return_trace (সঁচা);
  }

  রক্ষিত:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
সংরচনা LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  তার্কিক subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    + hb_enumerate (*এতত্)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, এতত্))
    ;
    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, এতত্)));
  }
};


/*
 * Coverage Table
 */


স্থির তার্কিক ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ধ্রুৱ hb_set_t &klasses,
                                          তার্কিক use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  যদি (!klass_map)
    ঘূৰাই ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  যদি (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  বাবে (ধ্রুৱ unsigned k: klasses)
  {
    যদি (klass_map->has (k)) অনুৱর্ত;
    klass_map->set (k, idx);
    idx++;
  }


  যদি (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    ঘূৰাই মিছা;

  বাবে (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  ঘূৰাই ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
সংরচনা ClassDefFormat1_3
{
  friend সংরচনা ClassDef;

  গুপ্ত:
  unsigned পূর্ণাঙ্ক get_class (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    ঘূৰাই class_মান[(unsigned পূর্ণাঙ্ক) (glyph_id - startGlyph)];
  }

  unsigned get_population () ধ্রুৱ
  {
    ঘূৰাই class_মান.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);

    যদি (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_মান.len = 0;
      return_trace (সঁচা);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_গুনতি = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    যদি (unlikely (!class_মান.serialize (c, glyph_গুনতি))) return_trace (মিছা);
    বাবে (ধ্রুৱ hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_মান[idx] = gid_klass_pair.second;
    }
    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               তার্কিক keep_empty_table = সঁচা,
               তার্কিক use_class_zero = সঁচা,
               ধ্রুৱ Coverage* glyph_filter = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_মান.len;

    বাবে (ধ্রুৱ hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      যদি (new_gid == HB_MAP_মান_INVALID) অনুৱর্ত;
      যদি (glyph_filter && !glyph_filter->has(gid)) অনুৱর্ত;

      unsigned klass = class_মান[gid - start];
      যদি (!klass) অনুৱর্ত;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    যদি (use_class_zero)
    {
      ধ্রুৱ hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_গুনতি = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_গুনতি <= glyph_and_klass.length;
    }
    যদি (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (মিছা);
    return_trace (keep_empty_table || (তার্কিক) glyph_and_klass);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && class_মান.sanitize (c));
  }

  unsigned cost () ধ্রুৱ { ঘূৰাই 1; }

  template <typename set_t>
  তার্কিক collect_coverage (set_t *glyphs) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক start = 0;
    unsigned পূর্ণাঙ্ক গুনতি = class_মান.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (class_মান[i])
	অনুৱর্ত;

      যদি (start != i)
	যদি (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  ঘূৰাই মিছা;

      start = i + 1;
    }
    যদি (start != গুনতি)
      যদি (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + গুনতি)))
	ঘূৰাই মিছা;

    ঘূৰাই সঁচা;
  }

  template <typename set_t>
  তার্কিক collect_class (set_t *glyphs, unsigned klass) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = class_মান.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (class_মান[i] == klass) glyphs->add (startGlyph + i);
    ঘূৰাই সঁচা;
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_মান.len;
    বাবে (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      যদি (class_মান[iter - start]) ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
  }
  তার্কিক intersects_class (ধ্রুৱ hb_set_t *glyphs, unsigned klass) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = class_মান.len;
    যদি (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_মান_INVALID;
      যদি (!glyphs->next (&g)) ঘূৰাই মিছা;
      যদি (g < startGlyph) ঘূৰাই সঁচা;
      g = startGlyph + গুনতি - 1;
      যদি (glyphs->next (&g)) ঘূৰাই সঁচা;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ধ্রুৱ typename Types::HBUINT *arr = class_মান.arrayZ;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (arr[i] == klass && glyphs->has (startGlyph + i))
	ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
  }

  শূন্য intersected_class_glyphs (ধ্রুৱ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ধ্রুৱ
  {
    unsigned গুনতি = class_মান.len;
    যদি (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      বাবে (uint32_t g = HB_SET_মান_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      বাবে (uint32_t g = startGlyph + গুনতি - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      ঘূৰাই;
    }

    বাবে (unsigned i = 0; i < গুনতি; i++)
      যদি (class_মান[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#যদি 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + গুনতি;
    বাবে (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      যদি (class_মান.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  শূন্য intersected_classes (ধ্রুৱ hb_set_t *glyphs, hb_set_t *intersect_classes) ধ্রুৱ
  {
    যদি (glyphs->is_empty ()) ঘূৰাই;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    যদি (!class_মান.len)
    {
      intersect_classes->add (0);
      ঘূৰাই;
    }

    hb_codepoint_t end_glyph = startGlyph + class_মান.len - 1;
    যদি (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    বাবে (ধ্রুৱ auto& _ : + hb_enumerate (class_মান))
    {
      hb_codepoint_t g = startGlyph + _.first;
      যদি (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  রক্ষিত:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_মান;	/* Array of Class Values--one per GlyphID */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_মান);
};

template <typename Types>
সংরচনা ClassDefFormat2_4
{
  friend সংরচনা ClassDef;

  গুপ্ত:
  unsigned পূর্ণাঙ্ক get_class (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    ঘূৰাই rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () ধ্রুৱ
  {
    typename Types::large_int ret = 0;
    বাবে (ধ্রুৱ auto &r : rangeRecord)
      ret += r.get_population ();
    ঘূৰাই ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);

    যদি (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (সঁচা);
    }

    unsigned unsorted = মিছা;
    unsigned অঙ্ক_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    যদি (unlikely (!record)) return_trace (মিছা);

    বাবে (ধ্রুৱ auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      যদি (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	যদি (unlikely (cur_gid < prev_gid))
	  unsorted = সঁচা;

	যদি (unlikely (!record)) ভঙ্গ;
	record->last = prev_gid;
	অঙ্ক_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    যদি (unlikely (c->in_error ())) return_trace (মিছা);

    যদি (likely (record)) record->last = prev_gid;
    rangeRecord.len = অঙ্ক_ranges;

    যদি (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               তার্কিক keep_empty_table = সঁচা,
               তার্কিক use_class_zero = সঁচা,
               ধ্রুৱ Coverage* glyph_filter = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    যদি (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      বাবে (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	যদি (!klass) অনুৱর্ত;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	যদি (new_gid == HB_MAP_মান_INVALID) অনুৱর্ত;
	যদি (glyph_filter && !glyph_filter->has (g)) অনুৱর্ত;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    অন্যথা
    {
      unsigned অঙ্ক_source_glyphs = c->plan->source->get_অঙ্ক_glyphs ();
      বাবে (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	যদি (!klass) অনুৱর্ত;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, অঙ্ক_source_glyphs);
	বাবে (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  যদি (new_gid == HB_MAP_মান_INVALID) অনুৱর্ত;
	  যদি (glyph_filter && !glyph_filter->has (g)) অনুৱর্ত;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    ধ্রুৱ hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_গুনতি = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_গুনতি <= glyph_and_klass.length;
    যদি (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (মিছা);
    return_trace (keep_empty_table || (তার্কিক) glyph_and_klass);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () ধ্রুৱ { ঘূৰাই hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  তার্কিক collect_coverage (set_t *glyphs) ধ্রুৱ
  {
    বাবে (auto &range : rangeRecord)
      যদি (range.value)
	যদি (unlikely (!range.collect_coverage (glyphs)))
	  ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
  }

  template <typename set_t>
  তার্কিক collect_class (set_t *glyphs, unsigned পূর্ণাঙ্ক klass) ধ্রুৱ
  {
    বাবে (auto &range : rangeRecord)
    {
      যদি (range.value == klass)
	যদি (unlikely (!range.collect_coverage (glyphs)))
	  ঘূৰাই মিছা;
    }
    ঘূৰাই সঁচা;
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    যদি (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      বাবে (auto g : *glyphs)
        যদি (get_class (g))
	  ঘূৰাই সঁচা;
      ঘূৰাই মিছা;
    }

    ঘূৰাই hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ধ্রুৱ RangeRecord<Types> &range) { ঘূৰাই range.intersects (*glyphs) && range.value; }));
  }
  তার্কিক intersects_class (ধ্রুৱ hb_set_t *glyphs, unsigned klass) ধ্রুৱ
  {
    যদি (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      যদি (!rangeRecord.len)
        ঘূৰাই !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_মান_INVALID;
      hb_codepoint_t last = HB_SET_মান_INVALID;
      auto it = hb_iter (rangeRecord);
      বাবে (auto &range : it)
      {
        যদি (it->first == last + 1)
	{
	  it++;
	  অনুৱর্ত;
	}

	যদি (!glyphs->next (&g))
	  ভঙ্গ;
	যদি (g < range.first)
	  ঘূৰাই সঁচা;
	g = range.last;
	last = g;
      }
      যদি (g != HB_SET_মান_INVALID && glyphs->next (&g))
	ঘূৰাই সঁচা;
      /* Fall through. */
    }
    বাবে (ধ্রুৱ auto &range : rangeRecord)
      যদি (range.value == klass && range.intersects (*glyphs))
	ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
  }

  শূন্য intersected_class_glyphs (ধ্রুৱ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ধ্রুৱ
  {
    যদি (klass == 0)
    {
      hb_codepoint_t g = HB_SET_মান_INVALID;
      বাবে (auto &range : rangeRecord)
      {
	যদি (!glyphs->next (&g))
	  গচ্ছ done;
	যেতিয়ালৈ (g < range.first)
	{
	  intersect_glyphs->add (g);
	  যদি (!glyphs->next (&g))
	    গচ্ছ done;
        }
        g = range.last;
      }
      যেতিয়ালৈ (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      ঘূৰাই;
    }

    unsigned গুনতি = rangeRecord.len;
    যদি (গুনতি > glyphs->get_population () * hb_bit_storage (গুনতি))
    {
      বাবে (auto g : *glyphs)
      {
        unsigned i;
        যদি (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      ঘূৰাই;
    }

    বাবে (auto &range : rangeRecord)
    {
      যদি (range.value != klass) অনুৱর্ত;

      unsigned end = range.last + 1;
      বাবে (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  শূন্য intersected_classes (ধ্রুৱ hb_set_t *glyphs, hb_set_t *intersect_classes) ধ্রুৱ
  {
    যদি (glyphs->is_empty ()) ঘূৰাই;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    যদি (!rangeRecord.len)
    {
      intersect_classes->add (0);
      ঘূৰাই;
    }

    hb_codepoint_t g = HB_SET_মান_INVALID;
    বাবে (auto &range : rangeRecord)
    {
      যদি (!glyphs->next (&g))
        ভঙ্গ;
      যদি (g < range.first)
      {
        intersect_classes->add (0);
        ভঙ্গ;
      }
      g = range.last;
    }
    যদি (g != HB_SET_মান_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    বাবে (ধ্রুৱ auto& range : rangeRecord)
      যদি (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  রক্ষিত:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

সংরচনা ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ধ্রুৱ { ঘূৰাই get (k); }
  তার্কিক has (hb_codepoint_t k) ধ্রুৱ { ঘূৰাই (*এতত্)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) ধ্রুৱ { ঘূৰাই get (k); }

  unsigned পূর্ণাঙ্ক get (hb_codepoint_t k) ধ্রুৱ { ঘূৰাই get_class (k); }
  unsigned পূর্ণাঙ্ক get_class (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_class (glyph_id);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.get_class (glyph_id);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.get_class (glyph_id);
#endif
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }
  unsigned পূর্ণাঙ্ক get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ধ্রুৱ
  {
    unsigned klass;
    যদি (cache && cache->get (glyph_id, &klass)) ঘূৰাই klass;
    klass = get_class (glyph_id);
    যদি (cache) cache->set (glyph_id, klass);
    ঘূৰাই klass;
  }

  unsigned get_population () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_population ();
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.get_population ();
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.get_population ();
#endif
    স্ৱভাৱ:ঘূৰাই NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    যদি (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned অঙ্ক_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      বাবে (ধ্রুৱ auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	যদি (cur_klass > class_max) class_max = cur_klass;
	যদি (cur_gid == glyph_min) অনুৱর্ত;
        যদি (cur_gid > glyph_max) glyph_max = cur_gid;
	যদি (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  অঙ্ক_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	যদি (size >= best_size) ঘূৰাই;
	best_size = size;
	format = candidate;
      };

      যদি (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      যদি (glyph_max <= 0xFFFFu && অঙ্ক_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) অঙ্ক_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      যদি (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      যদি (glyph_max <= 0xFFFFFFu && অঙ্ক_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) অঙ্ক_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      যদি (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (মিছা, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (মিছা);
      }
    }

    u.format.v = format;

    ৱিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    স্ৱভাৱ:return_trace (মিছা);
    }
  }

  তার্কিক subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               তার্কিক keep_empty_table = সঁচা,
               তার্কিক use_class_zero = সঁচা,
               ধ্রুৱ Coverage* glyph_filter = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    স্ৱভাৱ:return_trace (মিছা);
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (মিছা);
    hb_barrier ();
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    স্ৱভাৱ:return_trace (সঁচা);
    }
  }

  unsigned cost () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.cost ();
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.cost ();
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.cost ();
#endif
    স্ৱভাৱ:ঘূৰাই 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  তার্কিক collect_coverage (set_t *glyphs) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.collect_coverage (glyphs);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.collect_coverage (glyphs);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.collect_coverage (glyphs);
#endif
    স্ৱভাৱ:ঘূৰাই মিছা;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  তার্কিক collect_class (set_t *glyphs, unsigned পূর্ণাঙ্ক klass) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.collect_class (glyphs, klass);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.collect_class (glyphs, klass);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.collect_class (glyphs, klass);
#endif
    স্ৱভাৱ:ঘূৰাই মিছা;
    }
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.intersects (glyphs);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.intersects (glyphs);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.intersects (glyphs);
#endif
    স্ৱভাৱ:ঘূৰাই মিছা;
    }
  }
  তার্কিক intersects_class (ধ্রুৱ hb_set_t *glyphs, unsigned পূর্ণাঙ্ক klass) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.intersects_class (glyphs, klass);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.intersects_class (glyphs, klass);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.intersects_class (glyphs, klass);
#endif
    স্ৱভাৱ:ঘূৰাই মিছা;
    }
  }

  শূন্য intersected_class_glyphs (ধ্রুৱ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    স্ৱভাৱ:ঘূৰাই;
    }
  }

  শূন্য intersected_classes (ধ্রুৱ hb_set_t *glyphs, hb_set_t *intersect_classes) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.intersected_classes (glyphs, intersect_classes);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.intersected_classes (glyphs, intersect_classes);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    স্ৱভাৱ:ঘূৰাই;
    }
  }


  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
স্থির অন্তঃস্থ তার্কিক ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ ঘূৰাই (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
সংরচনা delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  সংরচনা chars_t : hb_vector_t<uint8_t>
  {
    পূর্ণাঙ্ক cmp (ধ্রুৱ chars_t& other) ধ্রুৱ
    {
      ঘূৰাই as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      বাবে (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      ঘূৰাই hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (ধ্রুৱ chars_t& other) ধ্রুৱ
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      বাবে (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      ঘূৰাই hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (ধ্রুৱ delta_row_encoding_t& other_encoding) ধ্রুৱ { ঘূৰাই chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<ধ্রুৱ hb_vector_t<পূর্ণাঙ্ক>*> বস্তুসমূহ;

  delta_row_encoding_t () = স্ৱভাৱ;
  delta_row_encoding_t (hb_vector_t<ধ্রুৱ hb_vector_t<পূর্ণাঙ্ক>*> &&rows, unsigned অঙ্ক_cols)
  {
    প্রতিজ্ঞা (rows);

    বস্তুসমূহ = std::move (rows);

    যদি (unlikely (!chars.resize (অঙ্ক_cols)))
      ঘূৰাই;

    calculate_chars ();
  }

  শূন্য merge (ধ্রুৱ delta_row_encoding_t& other)
  {
    বস্তুসমূহ.alloc (বস্তুসমূহ.length + other.বস্তুসমূহ.length);
    বাবে (auto &row : other.বস্তুসমূহ)
      add_row (row);

    // Merge chars
    প্রতিজ্ঞা (chars.length == other.chars.length);
    বাবে (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  শূন্য chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  শূন্য calculate_chars ()
  {
    প্রতিজ্ঞা (বস্তুসমূহ);

    তার্কিক long_শব্দগুলি = মিছা;

    বাবে (auto &row : বস্তুসমূহ)
    {
      প্রতিজ্ঞা (row->length == chars.length);

      /* 0/1/2 byte encoding */
      বাবে (unsigned i = 0; i < row->length; i++)
      {
	পূর্ণাঙ্ক v =  row->arrayZ[i];
	যদি (v == 0)
	  অনুৱর্ত;
	অন্যথা যদি (v > 32767 || v < -32768)
	{
	  long_শব্দগুলি = সঁচা;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	অন্যথা যদি (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	অন্যথা
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    যদি (long_শব্দগুলি)
    {
      // Convert 1s to 2s
      বাবে (auto &v : chars)
	যদি (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  তার্কিক is_empty () ধ্রুৱ
  { ঘূৰাই !বস্তুসমূহ; }

  তার্কিক in_error () ধ্রুৱ
  { ঘূৰাই chars.in_error () || বস্তুসমূহ.in_error (); }

  স্থির অন্তঃস্থ unsigned get_chars_overhead (unsigned অঙ্ক_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    ঘূৰাই c + অঙ্ক_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) ধ্রুৱ
  {
    পূর্ণাঙ্ক গুনতি = বস্তুসমূহ.length;
    ঘূৰাই hb_max (0, (পূর্ণাঙ্ক) overhead - গুনতি * (পূর্ণাঙ্ক) additional_bytes_per_rows);
  }

  পূর্ণাঙ্ক gain_from_merging (ধ্রুৱ delta_row_encoding_t& other_encoding) ধ্রুৱ
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    যদি (additional_bytes_per_rows > 0)
    {
      যদি (get_gain (additional_bytes_per_rows) == 0)
        ঘূৰাই 0;
    }
    অন্যথা
    {
      যদি (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	ঘূৰাই 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    পূর্ণাঙ্ক combined_gain = (পূর্ণাঙ্ক) overhead + (পূর্ণাঙ্ক) other_encoding.overhead;
    combined_gain -= (combined_width - (পূর্ণাঙ্ক) width) * বস্তুসমূহ.length;
    combined_gain -= (combined_width - (পূর্ণাঙ্ক) other_encoding.width) * other_encoding.বস্তুসমূহ.length;
    combined_gain -= get_chars_overhead (combined_columns);

    ঘূৰাই combined_gain;
  }

  তার্কিক add_row (ধ্রুৱ hb_vector_t<পূর্ণাঙ্ক>* row)
  { ঘূৰাই বস্তুসমূহ.push (row); }

  স্থির পূর্ণাঙ্ক cmp (ধ্রুৱ শূন্য *pa, ধ্রুৱ শূন্য *pb)
  {
    ধ্রুৱ delta_row_encoding_t *a = (ধ্রুৱ delta_row_encoding_t *)pa;
    ধ্রুৱ delta_row_encoding_t *b = (ধ্রুৱ delta_row_encoding_t *)pb;

    যদি (a->width != b->width)
      ঘূৰাই (পূর্ণাঙ্ক) a->width - (পূর্ণাঙ্ক) b->width;

    ঘূৰাই b->chars.cmp (a->chars);
  }
};

সংরচনা VarRegionAxis
{
  ভিন্ন evaluate (পূর্ণাঙ্ক coord) ধ্রুৱ
  {
    পূর্ণাঙ্ক peak = peakCoord.to_int ();
    যদি (peak == 0 || coord == peak)
      ঘূৰাই 1.f;
    অন্যথা যদি (coord == 0) // Faster
      ঘূৰাই 0.f;

    পূর্ণাঙ্ক start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    যদি (unlikely (start > peak || peak > end))
      ঘূৰাই 1.f;
    যদি (unlikely (start < 0 && end > 0))
      ঘূৰাই 1.f;

    যদি (coord <= start || end <= coord)
      ঘূৰাই 0.f;

    /* Interpolate */
    যদি (coord < peak)
      ঘূৰাই ভিন্ন (coord - start) / (peak - start);
    অন্যথা
      ঘূৰাই ভিন্ন (end - coord) / (end - peak);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  তার্কিক serialize (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed (এতত্));
  }

  সার্ৱজনিক:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};
সংরচনা SparseVarRegionAxis
{
  ভিন্ন evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len) ধ্রুৱ
  {
    unsigned i = axisIndex;
    পূর্ণাঙ্ক coord = i < coord_len ? coords[i] : 0;
    ঘূৰাই axis.evaluate (coord);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ SparseVarRegionAxis *src,
		  ধ্রুৱ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->embed (src);
    যদি (unlikely (!out)) return_trace (মিছা);
    যদি (axis_map)
    {
      যদি (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (মিছা);
      }
      যদি (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (মিছা);
    }
    return_trace (সঁচা);
  }

  সার্ৱজনিক:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা hb_scalar_cache_t
{
  গুপ্ত:
  স্থির constexpr unsigned STATIC_LENGTH = 128;
  স্থির constexpr ভিন্ন INVALID = NAN;

  সার্ৱজনিক:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ধ্রুৱ hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (ধ্রুৱ hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  স্থির hb_scalar_cache_t *create (unsigned পূর্ণাঙ্ক গুনতি,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    যদি (!গুনতি) ঘূৰাই (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    যদি (scratch_cache && গুনতি <= STATIC_LENGTH)
    {
      scratch_cache->length = গুনতি;
      scratch_cache->clear ();
      ঘূৰাই scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (পরিমাণ (hb_scalar_cache_t) - পরিমাণ (static_মানসমূহ) + পরিমাণ (static_মানসমূহ[0]) * গুনতি);
    যদি (unlikely (!cache)) ঘূৰাই (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = গুনতি;
    cache->clear ();

    ঘূৰাই cache;
  }

  স্থির শূন্য destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    যদি (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  শূন্য clear ()
  {
    auto *values = &static_মানসমূহ[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    বাবে (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    বাবে (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  তার্কিক get (unsigned i, ভিন্ন *value) ধ্রুৱ
  {
    যদি (unlikely (i >= length))
    {
      *value = 0.f;
      ঘূৰাই সঁচা;
    }
    auto *values = &static_মানসমূহ[0];
    auto *cached_মান = &values[i];
    ভিন্ন v = *cached_মান;
    যদি (std::isnan (v))
      ঘূৰাই মিছা;
    *value = v;
    ঘূৰাই সঁচা;
  }

  HB_ALWAYS_INLINE
  শূন্য set (unsigned i, ভিন্ন value)
  {
    যদি (unlikely (i >= length)) ঘূৰাই;
    auto *values = &static_মানসমূহ[0];
    auto *cached_মান = &values[i];
    *cached_মান = value;
  }

  সার্ৱজনিক:
  unsigned length;
  গুপ্ত:
  mutable hb_atomic_t<ভিন্ন> static_মানসমূহ[STATIC_LENGTH];
};

সংরচনা VarRegionList
{
  গুপ্ত:
  ভিন্ন evaluate_impl (unsigned পূর্ণাঙ্ক region_index,
		       ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len) ধ্রুৱ
  {
    ধ্রুৱ VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_গুনতি_2);
    ভিন্ন v = 1.f;

    unsigned পূর্ণাঙ্ক গুনতি = axis_গুনতি_2;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      পূর্ণাঙ্ক coord = i < coord_len ? coords[i] : 0;
      ভিন্ন factor = axes[i].evaluate (coord);
      যদি (factor == 0.f)
      {
	v = 0.f;
	ভঙ্গ;
      }
      v *= factor;
    }

    ঘূৰাই v;
  }

  সার্ৱজনিক:
  HB_ALWAYS_INLINE
  ভিন্ন evaluate (unsigned পূর্ণাঙ্ক region_index,
		  ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    যদি (unlikely (region_index >= region_গুনতি))
      ঘূৰাই 0.;

    ভিন্ন v;
    যদি (cache && cache->get (region_index, &v))
      ঘূৰাই v;

    v = evaluate_impl (region_index, coords, coord_len);

    যদি (cache)
      cache->set (region_index, v);
    ঘূৰাই v;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_গুনতি_2 * region_গুনতি));
  }

  তার্কিক serialize (hb_serialize_context_t *c,
                  ধ্রুৱ hb_vector_t<hb_tag_t>& axis_tags,
                  ধ্রুৱ hb_vector_t<ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned axis_গুনতি = axis_tags.length;
    unsigned region_গুনতি_2 = regions.length;
    যদি (!axis_গুনতি || !region_গুনতি_2) return_trace (মিছা);
    যদি (unlikely (hb_unsigned_mul_overflows (axis_গুনতি * region_গুনতি_2,
                                             VarRegionAxis::static_size))) return_trace (মিছা);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    axis_গুনতি_2 = axis_গুনতি;
    region_গুনতি = region_গুনতি_2;

    বাবে (unsigned r = 0; r < region_গুনতি_2; r++)
    {
      ধ্রুৱ auto& region = regions[r];
      বাবে (unsigned i = 0; i < axis_গুনতি; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        যদি (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->মধ্য);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        অন্যথা
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        যদি (!var_region_rec.serialize (c))
          return_trace (মিছা);
      }
    }
    return_trace (সঁচা);
  }

  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুৱ VarRegionList *src, ধ্রুৱ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    axis_গুনতি_2 = src->axis_গুনতি_2;
    region_গুনতি = region_map.get_population ();
    যদি (unlikely (hb_unsigned_mul_overflows (axis_গুনতি_2 * region_গুনতি,
					     VarRegionAxis::static_size))) return_trace (মিছা);
    যদি (unlikely (!c->extend (এতত্))) return_trace (মিছা);
    unsigned পূর্ণাঙ্ক region_গুনতি_2 = src->region_গুনতি;
    বাবে (unsigned পূর্ণাঙ্ক r = 0; r < region_গুনতি; r++)
    {
      unsigned পূর্ণাঙ্ক backward = region_map.backward (r);
      যদি (backward >= region_গুনতি_2) return_trace (মিছা);
      hb_memcpy (&axesZ[axis_গুনতি_2 * r], &src->axesZ[axis_গুনতি_2 * backward], VarRegionAxis::static_size * axis_গুনতি_2);
    }

    return_trace (সঁচা);
  }

  তার্কিক get_var_region (unsigned region_index,
                       ধ্রুৱ hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ধ্রুৱ
  {
    যদি (region_index >= region_গুনতি) ঘূৰাই মিছা;
    ধ্রুৱ VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_গুনতি_2);
    বাবে (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      hb_tag_t *axis_tag;
      যদি (!axes_old_index_tag_map.has (i, &axis_tag))
        ঘূৰাই মিছা;

      ভিন্ন min_val = axis_region->startCoord.to_float ();
      ভিন্ন def_val = axis_region->peakCoord.to_float ();
      ভিন্ন max_val = axis_region->endCoord.to_float ();

      যদি (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((দ্ৱিগুণ) min_val, (দ্ৱিগুণ) def_val, (দ্ৱিগুণ) max_val));
      axis_region++;
    }
    ঘূৰাই !axis_tuples.in_error ();
  }

  তার্কিক get_var_regions (ধ্রুৱ hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) ধ্রুৱ
  {
    যদি (!regions.alloc (region_গুনতি))
      ঘূৰাই মিছা;

    বাবে (unsigned i = 0; i < region_গুনতি; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      যদি (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        ঘূৰাই মিছা;
      regions.push (std::move (axis_tuples));
    }
    ঘূৰাই !regions.in_error ();
  }

  size_t get_size () ধ্রুৱ
  { ঘূৰাই hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_গুনতি_2, region_গুনতি)); }

  সার্ৱজনিক:
  HBUINT16	axis_গুনতি_2;
  HBUINT15	region_গুনতি;
  রক্ষিত:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
স্থির অন্তঃস্থ তার্কিক
_hb_avar2_region_is_dead (ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  বাবে (auto _ : axis_tuples)
  {
    Triple *range;
    যদি (!reachable_ranges.has (_.first, &range)) অনুৱর্ত;
    ধ্রুৱ Triple &tent = _.second;
    পূর্ণাঙ্ক start = (পূর্ণাঙ্ক) roundf ((ভিন্ন) tent.minimum * 16384.f);
    পূর্ণাঙ্ক peak  = (পূর্ণাঙ্ক) roundf ((ভিন্ন) tent.মধ্য * 16384.f);
    পূর্ণাঙ্ক end   = (পূর্ণাঙ্ক) roundf ((ভিন্ন) tent.maximum * 16384.f);
    যদি (!peak) অনুৱর্ত;
    যদি (start > peak || peak > end) অনুৱর্ত;
    যদি (start < 0 && end > 0) অনুৱর্ত;
    পূর্ণাঙ্ক lo = (পূর্ণাঙ্ক) roundf ((ভিন্ন) range->minimum * 16384.f);
    পূর্ণাঙ্ক hi = (পূর্ণাঙ্ক) roundf ((ভিন্ন) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    যদি (hi < start || lo > end)
      ঘূৰাই সঁচা;
  }
  ঘূৰাই মিছা;
}

সংরচনা SparseVariationRegion
{
  ভিন্ন evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len) ধ্রুৱ
  {
    ভিন্ন v = 1.f;
    unsigned পূর্ণাঙ্ক গুনতি = axes.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      ভিন্ন factor = (এতত্+axes[i]).evaluate (coords, coord_len);
      যদি (factor == 0.f)
	ঘূৰাই 0.;
      v *= factor;
    }
    ঘূৰাই v;
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ SparseVariationRegion *src,
		  ধ্রুৱ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (মিছা);
    বাবে (unsigned i = 0; i < axes.len; i++)
      যদি (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (মিছা);
    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (axes.sanitize (c, এতত্));
  }

  রক্ষিত:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2, axes);
};

সংরচনা SparseVarRegionList
{
  HB_ALWAYS_INLINE
  ভিন্ন evaluate (unsigned পূর্ণাঙ্ক region_index,
		  ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    যদি (unlikely (region_index >= regions.len))
      ঘূৰাই 0.;

    ভিন্ন v;
    যদি (cache && cache->get (region_index, &v))
      ঘূৰাই v;

    ধ্রুৱ SparseVariationRegion &region = এতত্+regions[region_index];

    v = region.evaluate (coords, coord_len);
    যদি (cache)
      cache->set (region_index, v);

    ঘূৰাই v;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (regions.sanitize (c, এতত্));
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ SparseVarRegionList *src,
		  ধ্রুৱ hb_inc_bimap_t &region_map,
		  ধ্রুৱ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    regions.len = region_map.get_population ();
    যদি (unlikely (!c->extend (regions))) return_trace (মিছা);

    বাবে (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      যদি (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (মিছা);
    }
    return_trace (সঁচা);
  }

  সার্ৱজনিক:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, regions);
};


সংরচনা VarData
{
  unsigned পূর্ণাঙ্ক get_item_গুনতি () ধ্রুৱ
  { ঘূৰাই item_গুনতি; }

  unsigned পূর্ণাঙ্ক get_region_index_গুনতি () ধ্রুৱ
  { ঘূৰাই regionIndices.len; }

  unsigned get_region_index (unsigned i) ধ্রুৱ
  { ঘূৰাই i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () ধ্রুৱ
  { ঘূৰাই (word_গুনতি () + regionIndices.len) * (long_শব্দগুলি_2 () ? 2 : 1); }

  size_t get_size () ধ্রুৱ
  { ঘূৰাই hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_গুনতি, get_row_size ())); }

  template <typename Float = ভিন্ন>
  Float _get_delta (unsigned পূর্ণাঙ্ক inner,
		    ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
		    ধ্রুৱ VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    যদি (unlikely (inner >= item_গুনতি))
      ঘূৰাই 0.;
    তার্কিক is_long = long_শব্দগুলি_2 ();
    unsigned পূর্ণাঙ্ক গুনতি = regionIndices.len;
    unsigned word_গুনতি_2 = word_গুনতি ();
    unsigned পূর্ণাঙ্ক scount = is_long ? গুনতি : word_গুনতি_2;
    unsigned পূর্ণাঙ্ক lcount = is_long ? word_গুনতি_2 : 0;

    ধ্রুৱ HBUINT8 *bytes = get_delta_bytes ();
    ধ্রুৱ HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned পূর্ণাঙ্ক i = 0;

    ধ্রুৱ HBINT32 *lcursor = reinterpret_cast<ধ্রুৱ HBINT32 *> (row);
    বাবে (; i < lcount; i++)
    {
      ভিন্ন scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_গুনতি, cache);
      যদি (scalar)
        delta += (Float) scalar * (পূর্ণাঙ্ক) *lcursor;
      lcursor++;
    }
    ধ্রুৱ HBINT16 *scursor = reinterpret_cast<ধ্রুৱ HBINT16 *> (lcursor);
    বাবে (; i < scount; i++)
    {
      ভিন্ন scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_গুনতি, cache);
      যদি (scalar)
       delta += (Float) scalar * (পূর্ণাঙ্ক) *scursor;
      scursor++;
    }
    ধ্রুৱ HBINT8 *bcursor = reinterpret_cast<ধ্রুৱ HBINT8 *> (scursor);
    বাবে (; i < গুনতি; i++)
    {
      ভিন্ন scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_গুনতি, cache);
      যদি (scalar)
        delta += (Float) scalar * (পূর্ণাঙ্ক) *bcursor;
      bcursor++;
    }

    ঘূৰাই delta;
  }

  template <typename Float = ভিন্ন>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned পূর্ণাঙ্ক inner,
		   ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
		   ধ্রুৱ VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = regionIndices.len;
    যদি (!গুনতি) ঘূৰাই Float (0); // This is quite common, so optimize it.
    ঘূৰাই _get_delta<Float> (inner, coords, coord_গুনতি, regions, cache);
  }

  শূন্য get_region_scalars (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
			   ধ্রুৱ VarRegionList &regions,
			   ভিন্ন *scalars /*OUT */,
			   unsigned পূর্ণাঙ্ক অঙ্ক_scalars) ধ্রুৱ
  {
    unsigned গুনতি = hb_min (অঙ্ক_scalars, regionIndices.len);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_গুনতি);
    বাবে (unsigned পূর্ণাঙ্ক i = গুনতি; i < অঙ্ক_scalars; i++)
      scalars[i] = 0.f;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_গুনতি () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_গুনতি,
				  get_row_size ()));
  }

  তার্কিক serialize (hb_serialize_context_t *c,
                  তার্কিক has_long,
                  ধ্রুৱ hb_vector_t<ধ্রুৱ hb_vector_t<পূর্ণাঙ্ক>*>& rows)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned row_গুনতি_2 = rows.length;
    যদি (!row_গুনতি_2) {
      // Nothing to serialize, will be empty.
      ঘূৰাই মিছা;
    }

    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    যদি (unlikely (!c->check_assign (item_গুনতি, row_গুনতি_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (মিছা);

    পূর্ণাঙ্ক min_threshold = has_long ? -32768 : -128;
    পূর্ণাঙ্ক max_threshold = has_long ? +32767 : +127;
    গণনা delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned অঙ্ক_regions = rows[0]->length;
    যদি (!delta_sz.resize (অঙ্ক_regions))
      return_trace (মিছা);

    unsigned word_গুনতি_2 = 0;
    বাবে (unsigned r = 0; r < অঙ্ক_regions; r++)
    {
      বাবে (unsigned i = 0; i < row_গুনতি_2; i++)
      {
        পূর্ণাঙ্ক delta = rows[i]->arrayZ[r];
        যদি (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_গুনতি_2++;
          ভঙ্গ;
        }
        অন্যথা যদি (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_গুনতি_2;
    hb_map_t ri_map;
    বাবে (unsigned r = 0; r < অঙ্ক_regions; r++)
    {
      যদি (!delta_sz[r]) অনুৱর্ত;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      যদি (!ri_map.set (new_r, r))
        return_trace (মিছা);
    }

    word_Size_গুনতি = word_গুনতি_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_গুনতি = ri_map.get_population ();
    regionIndices.len = ri_গুনতি;
    যদি (unlikely (!c->extend (এতত্))) return_trace (মিছা);

    বাবে (unsigned r = 0; r < ri_গুনতি; r++)
    {
      hb_codepoint_t *idx;
      যদি (!ri_map.has (r, &idx))
        return_trace (মিছা);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < row_গুনতি_2; i++)
    {
      বাবে (unsigned পূর্ণাঙ্ক r = 0; r < ri_গুনতি; r++)
      {
        পূর্ণাঙ্ক delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (সঁচা);
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ VarData *src,
		  ধ্রুৱ hb_inc_bimap_t &inner_map,
		  ধ্রুৱ hb_inc_bimap_t &region_map,
		  ধ্রুৱ hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    item_গুনতি = inner_map.get_next_মান ();

    /* Optimize word count */
    unsigned ri_গুনতি = src->regionIndices.len;
    গণনা delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned পূর্ণাঙ্ক> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_গুনতি);
    ri_map.resize (ri_গুনতি);
    unsigned পূর্ণাঙ্ক new_word_গুনতি = 0;
    unsigned পূর্ণাঙ্ক r;

    ধ্রুৱ HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_গুনতি = src->word_গুনতি ();
    তার্কিক     src_long_শব্দগুলি = src->long_শব্দগুলি_2 ();

    তার্কিক has_long = মিছা;
    যদি (src_long_শব্দগুলি)
    {
      বাবে (r = 0; r < src_word_গুনতি; r++)
      {
        বাবে (unsigned old_gid : inner_map.চাবিগুলি())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  যদি (delta < -32768 || 32767 < delta)
	  {
	    has_long = সঁচা;
	    ভঙ্গ;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    বাবে (r = 0; r < ri_গুনতি; r++)
    {
      তার্কিক short_circuit = src_long_শব্দগুলি == has_long && src_word_গুনতি <= r;

      delta_sz[r] = kZero;
      যদি (culled_regions && culled_regions->has (src->regionIndices[r]))
	অনুৱর্ত; /* unreachable region: drop its column */
      বাবে (unsigned old_gid : inner_map.চাবিগুলি())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	যদি (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_গুনতি++;
	  ভঙ্গ;
	}
	অন্যথা যদি (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  যদি (short_circuit)
	    ভঙ্গ;
	}
      }
    }

    unsigned পূর্ণাঙ্ক word_index = 0;
    unsigned পূর্ণাঙ্ক non_word_index = new_word_গুনতি;
    unsigned পূর্ণাঙ্ক new_ri_গুনতি = 0;
    বাবে (r = 0; r < ri_গুনতি; r++)
      যদি (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_গুনতি++;
      }

    word_Size_গুনতি = new_word_গুনতি | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_গুনতি;

    যদি (unlikely (!c->extend (এতত্))) return_trace (মিছা);

    বাবে (r = 0; r < new_ri_গুনতি; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned গুনতি = item_গুনতি;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      unsigned পূর্ণাঙ্ক old = inner_map.backward (i);
      বাবে (unsigned পূর্ণাঙ্ক r = 0; r < new_ri_গুনতি; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (সঁচা);
  }

  শূন্য collect_region_refs (hb_set_t &region_indices, ধ্রুৱ hb_inc_bimap_t &inner_map) ধ্রুৱ
  {
    ধ্রুৱ HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    বাবে (unsigned পূর্ণাঙ্ক r = 0; r < regionIndices.len; r++)
    {
      unsigned পূর্ণাঙ্ক region = regionIndices.arrayZ[r];
      যদি (region_indices.has (region)) অনুৱর্ত;
      বাবে (hb_codepoint_t old_gid : inner_map.চাবিগুলি())
	যদি (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  ভঙ্গ;
	}
    }
  }

  সার্ৱজনিক:
  ধ্রুৱ HBUINT8 *get_delta_bytes () ধ্রুৱ
  { ঘূৰাই &StructAfter<HBUINT8> (regionIndices); }

  রক্ষিত:
  HBUINT8 *get_delta_bytes ()
  { ঘূৰাই &StructAfter<HBUINT8> (regionIndices); }

  সার্ৱজনিক:
  int32_t get_item_delta_fast (unsigned পূর্ণাঙ্ক item, unsigned পূর্ণাঙ্ক region,
			       ধ্রুৱ HBUINT8 *delta_bytes, unsigned row_size) ধ্রুৱ
  {
    যদি (unlikely (item >= item_গুনতি || region >= regionIndices.len)) ঘূৰাই 0;

    ধ্রুৱ HBINT8 *p = (ধ্রুৱ HBINT8 *) delta_bytes + item * row_size;
    unsigned word_গুনতি_2 = word_গুনতি ();
    তার্কিক is_long = long_শব্দগুলি_2 ();
    যদি (is_long)
    {
      যদি (region < word_গুনতি_2)
	ঘূৰাই ((ধ্রুৱ HBINT32 *) p)[region];
      অন্যথা
	ঘূৰাই ((ধ্রুৱ HBINT16 *)(p + HBINT32::static_size * word_গুনতি_2))[region - word_গুনতি_2];
    }
    অন্যথা
    {
      যদি (region < word_গুনতি_2)
	ঘূৰাই ((ধ্রুৱ HBINT16 *) p)[region];
      অন্যথা
	ঘূৰাই (p + HBINT16::static_size * word_গুনতি_2)[region - word_গুনতি_2];
    }
  }
  int32_t get_item_delta (unsigned পূর্ণাঙ্ক item, unsigned পূর্ণাঙ্ক region) ধ্রুৱ
  {
     ঘূৰাই get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  রক্ষিত:
  শূন্য set_item_delta_fast (unsigned পূর্ণাঙ্ক item, unsigned পূর্ণাঙ্ক region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_গুনতি_2 = word_গুনতি ();
    তার্কিক is_long = long_শব্দগুলি_2 ();
    যদি (is_long)
    {
      যদি (region < word_গুনতি_2)
	((HBINT32 *) p)[region] = delta;
      অন্যথা
	((HBINT16 *)(p + HBINT32::static_size * word_গুনতি_2))[region - word_গুনতি_2] = delta;
    }
    অন্যথা
    {
      যদি (region < word_গুনতি_2)
	((HBINT16 *) p)[region] = delta;
      অন্যথা
	(p + HBINT16::static_size * word_গুনতি_2)[region - word_গুনতি_2] = delta;
    }
  }
  শূন্য set_item_delta (unsigned পূর্ণাঙ্ক item, unsigned পূর্ণাঙ্ক region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  তার্কিক long_শব্দগুলি_2 () ধ্রুৱ { ঘূৰাই word_Size_গুনতি & 0x8000u /* LONG_WORDS */; }
  unsigned word_গুনতি () ধ্রুৱ { ঘূৰাই word_Size_গুনতি & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  রক্ষিত:
  HBUINT16		item_গুনতি;
  HBUINT16		word_Size_গুনতি;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

সংরচনা MultiVarData
{
  size_t get_size () ধ্রুৱ
  { ঘূৰাই hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  ধ্রুৱ TupleList &get_delta_sets () ধ্রুৱ
  { ঘূৰাই এতত্+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  শূন্য get_delta (unsigned পূর্ণাঙ্ক inner,
		  ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
		  ধ্রুৱ SparseVarRegionList &regions,
		  hb_array_t<দ্ৱিগুণ> out,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    auto &deltaSets = get_delta_sets ();

    auto মানসমূহ_iter = deltaSets.fetcher (inner);
    unsigned region_গুনতি = regionIndices.len;
    unsigned skip = 0;
    বাবে (unsigned regionIndex = 0; regionIndex < region_গুনতি; regionIndex++)
    {
      ভিন্ন scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_গুনতি,
				       cache);
      // We skip lazily. Helps with the tail end.
      যদি (scalar == 0.0f)
        skip += out.length;
      অন্যথা
      {
        যদি (skip)
	{
	  মানসমূহ_iter.skip (skip);
	  skip = 0;
	}
	মানসমূহ_iter.add_to (out, scalar);
      }
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, এতত্));
  }

  unsigned get_item_গুনতি () ধ্রুৱ
  { ঘূৰাই get_delta_sets ().গুনতি; }

  শূন্য collect_region_refs (hb_set_t &region_indices) ধ্রুৱ
  {
    বাবে (unsigned region : regionIndices)
      region_indices.add (region);
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ MultiVarData *src,
		  ধ্রুৱ hb_inc_bimap_t &inner_map,
		  ধ্রুৱ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned region_গুনতি_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_গুনতি_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    যদি (unlikely (!c->extend_size (এতত্, header_size, মিছা)))
      return_trace (মিছা);
    format = 1;
    regionIndices.len = region_গুনতি_2;
    বাবে (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      যদি (unlikely (!region_map.has (old_region)))
	return_trace (মিছা);
      regionIndices[i] = region_map.get (old_region);
    }

    ধ্রুৱ auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    যদি (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (মিছা);
    বাবে (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      যদি (unlikely (old_inner >= src_delta_sets.গুনতি)) return_trace (মিছা);
      hb_ubytes_t bytes = static_cast<ধ্রুৱ CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    যদি (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (মিছা);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  রক্ষিত:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (7);
};

সংরচনা ItemVariationStore
{
  friend সংরচনা item_variations_t;

  hb_scalar_cache_t *create_cache () ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই hb_scalar_cache_t::create (0);
#endif
    ঘূৰাই hb_scalar_cache_t::create ((এতত্+regions).region_গুনতি);
  }

  স্থির শূন্য destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  গুপ্ত:
  template <typename Float = ভিন্ন>
  Float get_delta (unsigned পূর্ণাঙ্ক outer, unsigned পূর্ণাঙ্ক inner,
		   ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
		   hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই Float (0);
#endif

    যদি (unlikely (outer >= dataSets.len))
      ঘূৰাই Float (0);

    ঘূৰাই (এতত্+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_গুনতি,
					     এতত্+regions,
					     cache);
  }

  সার্ৱজনিক:
  তার্কিক has_delta_set (unsigned পূর্ণাঙ্ক index) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই মিছা;
#endif

    unsigned পূর্ণাঙ্ক outer = index >> 16;
    unsigned পূর্ণাঙ্ক inner = index & 0xFFFF;
    ঘূৰাই outer < dataSets.len &&
	   inner < (এতত্+dataSets[outer]).get_item_গুনতি ();
  }

  template <typename Float = ভিন্ন>
  Float get_delta (unsigned পূর্ণাঙ্ক index,
		   ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
		   hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক outer = index >> 16;
    unsigned পূর্ণাঙ্ক inner = index & 0xFFFF;
    ঘূৰাই get_delta<Float> (outer, inner, coords, coord_গুনতি, cache);
  }
  template <typename Float = ভিন্ন>
  Float get_delta (unsigned পূর্ণাঙ্ক index,
		   hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
		   hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    ঘূৰাই get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই সঁচা;
#endif

    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, এতত্) &&
		  dataSets.sanitize (c, এতত্));
  }

  তার্কিক serialize (hb_serialize_context_t *c,
                  তার্কিক has_long,
                  ধ্রুৱ hb_vector_t<hb_tag_t>& axis_tags,
                  ধ্রুৱ hb_vector_t<ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  ধ্রুৱ hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (এতত্);
#ifdef HB_NO_VAR
    return_trace (মিছা);
#endif
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);

    format = 1;
    যদি (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (মিছা);

    unsigned অঙ্ক_var_data = vardata_encodings.length;
    যদি (!অঙ্ক_var_data) return_trace (মিছা);
    যদি (unlikely (!c->check_assign (dataSets.len, অঙ্ক_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (মিছা);

    যদি (unlikely (!c->extend (dataSets))) return_trace (মিছা);
    বাবে (unsigned i = 0; i < অঙ্ক_var_data; i++)
      যদি (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].বস্তুসমূহ))
        return_trace (মিছা);

    return_trace (সঁচা);
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ ItemVariationStore *src,
		  ধ্রুৱ hb_array_t <ধ্রুৱ hb_inc_bimap_t> &inner_maps,
		  ধ্রুৱ hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (এতত্);
#ifdef HB_NO_VAR
    return_trace (মিছা);
#endif

    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);

    unsigned পূর্ণাঙ্ক set_গুনতি = 0;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < inner_maps.length; i++)
      যদি (inner_maps[i].get_population ())
	set_গুনতি++;

    format = 1;

    ধ্রুৱ auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    যদি (region_indices.in_error ())
      return_trace (মিছা);

    region_indices.del_range ((src_regions).region_গুনতি, hb_set_t::INVALID);

    যদি (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    যদি (region_map.in_error())
      return_trace (মিছা);

    যদি (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (মিছা);

    dataSets.len = set_গুনতি;
    যদি (unlikely (!c->extend (dataSets))) return_trace (মিছা);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned পূর্ণাঙ্ক set_index = 0;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < inner_maps.length; i++)
    {
      যদি (!inner_maps[i].get_population ()) অনুৱর্ত;
      যদি (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (মিছা);
    }

    return_trace (সঁচা);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->start_embed (এতত্);
    যদি (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned গুনতি = dataSets.len;
    বাবে (unsigned i = 0; i < গুনতি; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      যদি (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = এতত্+dataSets[i];

      unsigned item_গুনতি = data.get_item_গুনতি ();
      বাবে (unsigned j = 0; j < item_গুনতি; j++)
	map->add (j);
    }

    যদি (unlikely (!out->serialize (c, এতত্, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  তার্কিক collect_dead_regions (ধ্রুৱ hb_map_t &axes_old_index_tag_map,
			     ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) ধ্রুৱ
  {
#ifndef HB_NO_VAR
    যদি (!reachable_ranges.get_population ()) ঘূৰাই সঁচা;
    ধ্রুৱ VarRegionList &region_list = এতত্+regions;
    unsigned গুনতি = region_list.region_গুনতি;
    বাবে (unsigned i = 0; i < গুনতি; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      যদি (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	ঘূৰাই মিছা;
      যদি (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    ঘূৰাই !dead_regions.in_error ();
#অন্যথা
    ঘূৰাই সঁচা;
#endif
  }

  তার্কিক subset (hb_subset_context_t *c, ধ্রুৱ hb_array_t<ধ্রুৱ hb_inc_bimap_t> &inner_maps) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
#ifdef HB_NO_VAR
    return_trace (মিছা);
#endif

    ItemVariationStore *varstore_মৌলিক = c->serializer->start_embed<ItemVariationStore> ();
    যদি (unlikely (!varstore_মৌলিক)) return_trace (মিছা);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    যদি (c->plan->has_avar2)
      যদি (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (মিছা);

    varstore_মৌলিক->serialize (c->serializer, এতত্, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_মৌলিক->dataSets);
  }

  unsigned পূর্ণাঙ্ক get_region_index_গুনতি (unsigned পূর্ণাঙ্ক major) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই 0;
#endif
    ঘূৰাই (এতত্+dataSets[major]).get_region_index_গুনতি ();
  }

  শূন্য get_region_scalars (unsigned পূর্ণাঙ্ক major,
			   ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
			   ভিন্ন *scalars /*OUT*/,
			   unsigned পূর্ণাঙ্ক অঙ্ক_scalars) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    বাবে (unsigned i = 0; i < অঙ্ক_scalars; i++)
      scalars[i] = 0.f;
    ঘূৰাই;
#endif

    (এতত্+dataSets[major]).get_region_scalars (coords, coord_গুনতি,
					       এতত্+regions,
					       &scalars[0], অঙ্ক_scalars);
  }

  unsigned পূর্ণাঙ্ক get_sub_table_গুনতি () ধ্রুৱ
   {
#ifdef HB_NO_VAR
     ঘূৰাই 0;
#endif
     ঘূৰাই dataSets.len;
   }

  ধ্রুৱ VarData& get_sub_table (unsigned i) ধ্রুৱ
  {
#ifdef HB_NO_VAR
     ঘূৰাই Null (VarData);
#endif
     ঘূৰাই এতত্+dataSets[i];
  }

  ধ্রুৱ VarRegionList& get_region_list () ধ্রুৱ
  {
#ifdef HB_NO_VAR
     ঘূৰাই Null (VarRegionList);
#endif
     ঘূৰাই এতত্+regions;
  }

  রক্ষিত:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

সংরচনা MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই hb_scalar_cache_t::create (0);
#endif
    auto &r = এতত্+regions;
    unsigned গুনতি = r.regions.len;

    ঘূৰাই hb_scalar_cache_t::create (গুনতি, static_cache);
  }

  স্থির শূন্য destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  গুপ্ত:
  শূন্য get_delta (unsigned পূর্ণাঙ্ক outer, unsigned পূর্ণাঙ্ক inner,
		  ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
		  hb_array_t<দ্ৱিগুণ> out,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই;
#endif

    যদি (unlikely (outer >= dataSets.len))
      ঘূৰাই;

    ঘূৰাই (এতত্+dataSets[outer]).get_delta (inner,
					     coords, coord_গুনতি,
					     এতত্+regions,
					     out,
					     cache);
  }

  সার্ৱজনিক:
  শূন্য get_delta (unsigned পূর্ণাঙ্ক index,
		  ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
		  hb_array_t<দ্ৱিগুণ> out,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক outer = index >> 16;
    unsigned পূর্ণাঙ্ক inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_গুনতি, out, cache);
  }
  শূন্য get_delta (unsigned পূর্ণাঙ্ক index,
		  hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
		  hb_array_t<দ্ৱিগুণ> out,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুৱ
  {
    ঘূৰাই get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই সঁচা;
#endif

    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, এতত্) &&
		  dataSets.sanitize (c, এতত্));
  }

  তার্কিক create_subset_plan (ধ্রুৱ hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ধ্রুৱ
  {
    যদি (unlikely (!inner_maps->resize (dataSets.len))) ঘূৰাই মিছা;
    বাবে (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      যদি (unlikely (outer >= dataSets.len ||
		    inner >= (এতত্+dataSets[outer]).get_item_গুনতি ()))
	ঘূৰাই মিছা;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    বাবে (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      ধ্রুৱ hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      যদি (!inner_map.get_population ()) অনুৱর্ত;
      যদি (unlikely (inner_map.in_error ())) ঘূৰাই মিছা;
      বাবে (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    ঘূৰাই !inner_maps->in_error () && !varidx_map->in_error ();
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ MultiItemVariationStore *src,
		  ধ্রুৱ hb_array_t<ধ্রুৱ hb_inc_bimap_t> &inner_maps,
		  ধ্রুৱ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (মিছা);
    format = 1;

    ধ্রুৱ auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_গুনতি = 0;
    বাবে (unsigned i = 0; i < src->dataSets.len; i++)
    {
      যদি (!inner_maps[i].get_population ()) অনুৱর্ত;
      set_গুনতি++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    যদি (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (মিছা);

    dataSets.len = set_গুনতি;
    যদি (unlikely (!c->extend (dataSets))) return_trace (মিছা);
    unsigned set_index = 0;
    বাবে (unsigned i = 0; i < src->dataSets.len; i++)
    {
      যদি (!inner_maps[i].get_population ()) অনুৱর্ত;
      যদি (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (মিছা);
    }
    return_trace (সঁচা);
  }

  রক্ষিত:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_গুনতি_T>
সংরচনা DeltaSetIndexMapFormat01
{
  friend সংরচনা DeltaSetIndexMap;

  size_t get_size () ধ্রুৱ
  { ঘূৰাই hb_unsigned_mul_add_saturate (map_গুনতি_2, get_width (), min_size); }

  গুপ্ত:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed (এতত্));
  }

  template <typename T>
  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুৱ T &plan)
  {
    unsigned পূর্ণাঙ্ক width = plan.get_width ();
    unsigned পূর্ণাঙ্ক inner_bit_গুনতি = plan.get_inner_bit_গুনতি ();
    ধ্রুৱ hb_array_t<ধ্রুৱ uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (output_map.length && ((((inner_bit_গুনতি-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (মিছা);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);

    entryFormat = ((width-1)<<4)|(inner_bit_গুনতি-1);
    map_গুনতি_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    যদি (unlikely (!p)) return_trace (মিছা);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < output_map.length; i++)
    {
      unsigned পূর্ণাঙ্ক v = output_map.arrayZ[i];
      যদি (v)
      {
	unsigned পূর্ণাঙ্ক outer = v >> 16;
	unsigned পূর্ণাঙ্ক inner = v & 0xFFFF;
	unsigned পূর্ণাঙ্ক u = (outer << inner_bit_গুনতি) | inner;
	বাবে (unsigned পূর্ণাঙ্ক w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (সঁচা);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned পূর্ণাঙ্ক v) ধ্রুৱ /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    যদি (!map_গুনতি_2)
      ঘূৰাই v;
    ঘূৰাই _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned পূর্ণাঙ্ক v) ধ্রুৱ /* Returns 16.16 outer.inner. */
  {
    যদি (v >= map_গুনতি_2)
      v = map_গুনতি_2 - 1;

    unsigned পূর্ণাঙ্ক u = 0;
    { /* Fetch it. */
      unsigned পূর্ণাঙ্ক w = get_width ();
      ধ্রুৱ HBUINT8 *p = mapDataZ.arrayZ + w * v;
      বাবে (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned পূর্ণাঙ্ক n = get_inner_bit_গুনতি ();
      unsigned পূর্ণাঙ্ক outer = u >> n;
      unsigned পূর্ণাঙ্ক inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    ঘূৰাই u;
  }

  unsigned get_map_গুনতি () ধ্রুৱ       { ঘূৰাই map_গুনতি_2; }
  unsigned get_width () ধ্রুৱ           { ঘূৰাই ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_গুনতি () ধ্রুৱ { ঘূৰাই (entryFormat & 0xF) + 1; }


  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_গুনতি_2,
                                  get_width ()));
  }

  রক্ষিত:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_গুনতি_T     map_গুনতি_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2+Map_গুনতি_T::static_size, mapDataZ);
};

সংরচনা DeltaSetIndexMap
{
  template <typename T>
  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুৱ T &plan)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    পক্ষ 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    স্ৱভাৱ:return_trace (মিছা);
    }
  }

  uint32_t map (unsigned v) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); ঘূৰাই (u.format0.map (v));
    পক্ষ 1: hb_barrier (); ঘূৰাই (u.format1.map (v));
    স্ৱভাৱ:ঘূৰাই v;
    }
  }

  unsigned get_map_গুনতি () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); ঘূৰাই u.format0.get_map_গুনতি ();
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_map_গুনতি ();
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }

  unsigned get_width () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); ঘূৰাই u.format0.get_width ();
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_width ();
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }

  unsigned get_inner_bit_গুনতি () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); ঘূৰাই u.format0.get_inner_bit_গুনতি ();
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_inner_bit_গুনতি ();
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (মিছা);
    hb_barrier ();
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    স্ৱভাৱ:return_trace (সঁচা);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    পক্ষ 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    স্ৱভাৱ:return_trace (nullptr);
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (1, format.v);
};


সংরচনা ItemVarStoreInstancer
{
  ItemVarStoreInstancer (ধ্রুৱ ItemVariationStore *varStore_,
			 ধ্রুৱ DeltaSetIndexMap *varIdxMap,
			 hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    যদি (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    যদি (!hb_any (coords))
      এতত্->coords = {};
  }

  operator তার্কিক () ধ্রুৱ { ঘূৰাই varStore && তার্কিক (coords); }

  ভিন্ন operator[] (uint32_t varIdx) ধ্রুৱ
  { ঘূৰাই (*এতত্) (varIdx); }

  ভিন্ন operator() (uint32_t varIdx, unsigned short offset = 0) ধ্রুৱ
  { ঘূৰাই get_delta (varIdx, offset); }

  দ্ৱিগুণ get_condition_delta (uint32_t varIdx) ধ্রুৱ
  { ঘূৰাই get_delta<দ্ৱিগুণ> (varIdx); }

  template <typename Float = ভিন্ন>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) ধ্রুৱ
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   যদি (!coords || varIdx == VarIdx::NO_VARIATION)
     ঘূৰাই Float (0);

    varIdx += offset;
    যদি (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    ঘূৰাই varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ধ্রুৱ ItemVariationStore *varStore;
  ধ্রুৱ DeltaSetIndexMap *varIdxMap;
  hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords;
  hb_scalar_cache_t *cache;
};

সংরচনা MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (ধ্রুৱ MultiItemVariationStore *varStore,
			      ধ্রুৱ DeltaSetIndexMap *varIdxMap,
			      hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    যদি (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator তার্কিক () ধ্রুৱ { ঘূৰাই varStore; }

  দ্ৱিগুণ get_condition_delta (uint32_t varIdx) ধ্রুৱ
  { ঘূৰাই (দ্ৱিগুণ) (*এতত্)[varIdx]; }

  দ্ৱিগুণ operator[] (uint32_t varIdx) ধ্রুৱ
  {
    দ্ৱিগুণ v = 0;
    (*এতত্) (hb_array (&v, 1), varIdx);
    ঘূৰাই v;
  }

  শূন্য operator() (hb_array_t<দ্ৱিগুণ> out, uint32_t varIdx, unsigned short offset = 0) ধ্রুৱ
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    যদি (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      যদি (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    অন্যথা
      বাবে (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  ধ্রুৱ MultiItemVariationStore *varStore;
  ধ্রুৱ DeltaSetIndexMap *varIdxMap;
  hb_array_t<ধ্রুৱ পূর্ণাঙ্ক> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
গণনা Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

গণনা lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

সংরচনা Condition;

template <typename Instancer>
স্থির তার্কিক
_hb_recurse_condition_evaluate (ধ্রুৱ সংরচনা Condition &condition,
				ধ্রুৱ পূর্ণাঙ্ক *coords,
				unsigned পূর্ণাঙ্ক coord_len,
				Instancer *instancer);

সংরচনা ConditionAxisRange
{
  friend সংরচনা Condition;

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    ধ্রুৱ hb_map_t *index_map = &c->plan->axes_index_map;
    যদি (index_map->is_empty ()) return_trace (সঁচা);

    ধ্রুৱ hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    যদি (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (মিছা);

    ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    যদি (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    ধ্রুৱ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    যদি (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    ভিন্ন normalized_min = renormalize_মান ((দ্ৱিগুণ) filter_Range_Min_মান.to_float (), axis_limit, axis_triple_distances, মিছা);
    ভিন্ন normalized_max = renormalize_মান ((দ্ৱিগুণ) filter_Range_Max_মান.to_float (), axis_limit, axis_triple_distances, মিছা);
    out->filter_Range_Min_মান.set_float (normalized_min);
    out->filter_Range_Max_মান.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  গুপ্ত:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) ধ্রুৱ
  {
    //invalid axis index, drop the entire record
    যদি (!c->axes_index_tag_map->has (axisIndex))
      ঘূৰাই DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    তার্কিক axis_set_by_user = মিছা;
    যদি (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = সঁচা;
    }

    ভিন্ন axis_min_val = axis_range.minimum;
    ভিন্ন axis_default_val = axis_range.মধ্য;
    ভিন্ন axis_max_val = axis_range.maximum;

    ভিন্ন filter_min_val = filter_Range_Min_মান.to_float ();
    ভিন্ন filter_max_val = filter_Range_Max_মান.to_float ();

    যদি (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = মিছা;

    //condition not met, drop the entire record
    যদি (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      ঘূৰাই DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    যদি (axis_set_by_user && axis_range.is_point ())
      ঘূৰাই DROP_COND_WITH_VAR;

    যদি (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_মান.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_মান.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      ঘূৰাই KEEP_COND_WITH_VAR;
    }
    ঘূৰাই KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  তার্কিক evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		 Instancer *instancer HB_UNUSED) ধ্রুৱ
  {
    পূর্ণাঙ্ক coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    ঘূৰাই filter_Range_Min_মান.to_int () <= coord && coord <= filter_Range_Max_মান.to_int ();
  }

  সার্ৱজনিক:
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_মান;
  F2DOT14	filter_Range_Max_মান;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা Condition_মান
{
  friend সংরচনা Condition;

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (varIdx == VarIdx::NO_VARIATION)
      return_trace (সঁচা);

    hb_pair_t<unsigned, পূর্ণাঙ্ক> *condition_মান;
    যদি (c->plan->layout_condition_idx_মান_map.has ((uintptr_t) এতত্, &condition_মান))
    {
      out->default_মান_2 = condition_মান->second;
      out->varIdx = condition_মান->first;
      return_trace (সঁচা);
    }

    hb_pair_t<unsigned, পূর্ণাঙ্ক> *new_varidx_delta;
    যদি (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (মিছা);

    যদি (unlikely (!c->serializer->check_assign (
		out->default_মান_2,
		default_মান_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (মিছা);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (সঁচা);
  }

  গুপ্ত:
  template <typename Instancer>
  তার্কিক evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		 Instancer *instancer) ধ্রুৱ
  {
    দ্ৱিগুণ value = default_মান_2;
    value += instancer->get_condition_delta (varIdx);
    ঘূৰাই value > 0;
  }

  সার্ৱজনিক:
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_মান_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা ConditionAnd
{
  friend সংরচনা Condition;

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (মিছা);
    out->format = format;

    বাবে (ধ্রুৱ auto &offset : conditions)
    {
      যদি (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	যদি (unlikely (!out_offset)) return_trace (মিছা);
	*out_offset = 0;
      }
      অন্যথা যদি (unlikely (!subset_offset_array (c, out->conditions, এতত্) (offset)))
	return_trace (মিছা);
    }

    return_trace (সঁচা);
  }

  গুপ্ত:
  template <typename Instancer>
  তার্কিক evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		 Instancer *instancer) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = conditions.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!_hb_recurse_condition_evaluate (এতত্+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
  }

  সার্ৱজনিক:
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (conditions.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<সংরচনা Condition>	conditions;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (3, conditions);
};

সংরচনা ConditionOr
{
  friend সংরচনা Condition;

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (মিছা);
    out->format = format;

    বাবে (ধ্রুৱ auto &offset : conditions)
    {
      যদি (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	যদি (unlikely (!out_offset)) return_trace (মিছা);
	*out_offset = 0;
      }
      অন্যথা যদি (unlikely (!subset_offset_array (c, out->conditions, এতত্) (offset)))
	return_trace (মিছা);
    }

    return_trace (সঁচা);
  }

  গুপ্ত:
  template <typename Instancer>
  তার্কিক evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		 Instancer *instancer) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = conditions.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (_hb_recurse_condition_evaluate (এতত্+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
  }

  সার্ৱজনিক:
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (conditions.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<সংরচনা Condition>	conditions;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (3, conditions);
};

সংরচনা ConditionNegate
{
  friend সংরচনা Condition;

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);
    যদি (condition.is_null ())
    {
      out->condition = 0;
      return_trace (সঁচা);
    }
    return_trace (out->condition.serialize_subset (c, condition, এতত্));
  }

  গুপ্ত:
  template <typename Instancer>
  তার্কিক evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		 Instancer *instancer) ধ্রুৱ
  {
    ঘূৰাই !_hb_recurse_condition_evaluate (এতত্+condition,
					    coords, coord_len,
					    instancer);
  }

  সার্ৱজনিক:
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (condition.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<সংরচনা Condition>	condition;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (5);
};

সংরচনা Condition
{
  friend সংরচনা ConditionSet;

  স্থির তার্কিক serialize_constant (hb_serialize_context_t *c, তার্কিক value)
  {
    auto *out = c->start_embed<Condition_মান> ();
    যদি (unlikely (!out || !c->extend_min (out))) ঘূৰাই মিছা;
    out->format = 2;
    out->default_মান_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    ঘূৰাই সঁচা;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      তার্কিক materialize_true = মিছা) ধ্রুৱ;

  template <typename Instancer>
  তার্কিক evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		 Instancer *instancer) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    পক্ষ 0: ঘূৰাই সঁচা;
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.evaluate (coords, coord_len, instancer);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.evaluate (coords, coord_len, instancer);
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.evaluate (coords, coord_len, instancer);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.evaluate (coords, coord_len, instancer);
    পক্ষ 5: hb_barrier (); ঘূৰাই u.format5.evaluate (coords, coord_len, instancer);
    স্ৱভাৱ:ঘূৰাই মিছা;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) ধ্রুৱ
  {
    যদি (unlikely (!depth))
    {
      c->apply = মিছা;
      ঘূৰাই DROP_RECORD_WITH_VAR;
    }

    ৱিকল্প (u.format.v) {
    পক্ষ 0: ঘূৰাই KEEP_RECORD_WITH_VAR;
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.keep_with_variations (c, condition_map);
    পক্ষ 2: পক্ষ 3: পক্ষ 4: পক্ষ 5: ভঙ্গ;
    স্ৱভাৱ: c->apply = মিছা; ঘূৰাই DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    তার্কিক applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    যদি (c->all_axes_pinned)
      ঘূৰাই applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    যদি (u.format.v == 2)
      ঘূৰাই u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (ধ্রুৱ Condition &child)
    {
      auto child_context = *c;
      child_context.apply = সঁচা;
      ঘূৰাই child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    যদি (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      ঘূৰাই result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    তার্কিক is_and = u.format.v == 3;
    তার্কিক variable = মিছা;
    ধ্রুৱ auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ধ্রুৱ শূন্য *base = is_and ? (ধ্রুৱ শূন্য *) &u.format3 : (ধ্রুৱ শূন্য *) &u.format4;
    বাবে (ধ্রুৱ auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      যদি (is_and && result == DROP_RECORD_WITH_VAR)
        ঘূৰাই DROP_RECORD_WITH_VAR;
      যদি (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        ঘূৰাই KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    ঘূৰাই variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    পক্ষ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    পক্ষ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    পক্ষ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);

    যদি (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_মান ());

    return_trace (c->end_recursion (এতত্->dispatch (c)));
  }

  তার্কিক collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *condition_মানসমূহ = nullptr) ধ্রুৱ;

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ Condition *src,
		  ধ্রুৱ hb_map_t &varidx_map,
		  ধ্রুৱ hb_map_t *axis_map = nullptr);

  গুপ্ত:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) ধ্রুৱ;

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_মান	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
তার্কিক
_hb_recurse_condition_evaluate (ধ্রুৱ সংরচনা Condition &condition,
				ধ্রুৱ পূর্ণাঙ্ক *coords,
				unsigned পূর্ণাঙ্ক coord_len,
				Instancer *instancer)
{
  ঘূৰাই condition.evaluate (coords, coord_len, instancer);
}

সংরচনা ConditionList
{
  ধ্রুৱ Condition& operator[] (unsigned i) ধ্রুৱ
  { ঘূৰাই এতত্+conditions[i]; }

  unsigned get_গুনতি () ধ্রুৱ { ঘূৰাই conditions.len; }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ ConditionList *src,
		  ধ্রুৱ hb_inc_bimap_t &condition_map,
		  ধ্রুৱ hb_map_t &varidx_map,
		  ধ্রুৱ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    conditions.len = condition_map.get_population ();
    যদি (unlikely (!c->extend (conditions))) return_trace (মিছা);
    বাবে (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      যদি (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (মিছা);
    }
    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (conditions.sanitize (c, এতত্));
  }

  রক্ষিত:
  Array32OfOffset32To<Condition> conditions;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, conditions);
};

অন্তঃস্থ তার্কিক
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *condition_মানসমূহ) ধ্রুৱ
{
  যদি (unlikely (!depth)) ঘূৰাই মিছা;
  ৱিকল্প (u.format.v)
  {
    পক্ষ 0:
    পক্ষ 1:
      ঘূৰাই সঁচা;
    পক্ষ 2:
      যদি (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        যদি (condition_মানসমূহ &&
            !condition_মানসমূহ->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), পূর্ণাঙ্ক (u.format2.default_মান_2))))
          ঘূৰাই মিছা;
      }
      ঘূৰাই !var_indices->in_error ();
    পক্ষ 3:
      বাবে (ধ্রুৱ auto &offset : u.format3.conditions)
	যদি (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_মানসমূহ)))
	  ঘূৰাই মিছা;
      ঘূৰাই সঁচা;
    পক্ষ 4:
      বাবে (ধ্রুৱ auto &offset : u.format4.conditions)
	যদি (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_মানসমূহ)))
	  ঘূৰাই মিছা;
      ঘূৰাই সঁচা;
    পক্ষ 5:
      ঘূৰাই (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_মানসমূহ);
    স্ৱভাৱ:
      ঘূৰাই মিছা;
  }
}

অন্তঃস্থ তার্কিক
Condition::serialize (hb_serialize_context_t *c,
		      ধ্রুৱ Condition *src,
		      ধ্রুৱ hb_map_t &varidx_map,
		      ধ্রুৱ hb_map_t *axis_map)
{
  TRACE_SERIALIZE (এতত্);
  ৱিকল্প (src->u.format.v)
  {
    পক্ষ 0:
    {
      auto *out = c->start_embed<Condition_মান> ();
      যদি (unlikely (!out || !c->extend_min (out))) return_trace (মিছা);
      out->format = 2;
      out->default_মান_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (সঁচা);
    }
    পক্ষ 1:
    {
      auto *out = c->embed (&src->u.format1);
      যদি (unlikely (!out)) return_trace (মিছা);
      যদি (axis_map)
      {
	যদি (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (মিছা);
	}
	যদি (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (মিছা);
      }
      return_trace (সঁচা);
    }
    পক্ষ 2:
    {
      auto *out = c->embed (&src->u.format2);
      যদি (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (মিছা);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (সঁচা);
    }
    পক্ষ 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      যদি (unlikely (!c->extend_min (out))) return_trace (মিছা);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      যদি (unlikely (!c->extend (out->conditions))) return_trace (মিছা);
      বাবে (unsigned i = 0; i < out->conditions.len; i++)
	যদি (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (মিছা);
      return_trace (সঁচা);
    }
    পক্ষ 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      যদি (unlikely (!c->extend_min (out))) return_trace (মিছা);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      যদি (unlikely (!c->extend (out->conditions))) return_trace (মিছা);
      বাবে (unsigned i = 0; i < out->conditions.len; i++)
	যদি (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (মিছা);
      return_trace (সঁচা);
    }
    পক্ষ 5:
    {
      auto *out = c->embed (&src->u.format5);
      যদি (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (মিছা);
      return_trace (সঁচা);
    }
    স্ৱভাৱ:
      return_trace (মিছা);
  }
}

অন্তঃস্থ lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    তার্কিক materialize_true) ধ্রুৱ
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  যদি (result == LOOKUP_CONDITION_SUBSET_KEEP)
    ঘূৰাই result;

  c->serializer->revert (snap);
  যদি (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    ঘূৰাই result;

  ঘূৰাই serialize_constant (c->serializer, সঁচা) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

অন্তঃস্থ lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) ধ্রুৱ
{
  যদি (unlikely (!depth))
    ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   ধ্রুৱ Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    যদি (unlikely (!offset))
      ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    যদি (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->বেৰ_কৰো_pack ());
    অন্যথা
    {
      c->serializer->বেৰ_কৰো_discard ();
      out.বেৰ_কৰো ();
      c->serializer->revert (snap);
    }
    ঘূৰাই result;
  };

  ৱিকল্প (u.format.v)
  {
    পক্ষ 0:
      ঘূৰাই LOOKUP_CONDITION_SUBSET_TRUE;

    পক্ষ 1:
    {
      ধ্রুৱ ConditionAxisRange &src = u.format1;
      যদি (c->plan->user_axes_location.is_empty ())
        ঘূৰাই c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      যদি (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      যদি (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      ভিন্ন filter_min = src.filter_Range_Min_মান.to_float ();
      ভিন্ন filter_max = src.filter_Range_Max_মান.to_float ();
      যদি (filter_min > filter_max)
        ঘূৰাই LOOKUP_CONDITION_SUBSET_FALSE;
      যদি (axis_limit.is_point ())
        ঘূৰাই filter_min <= axis_limit.মধ্য &&
	       axis_limit.মধ্য <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      যদি (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        ঘূৰাই LOOKUP_CONDITION_SUBSET_FALSE;
      যদি (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        ঘূৰাই LOOKUP_CONDITION_SUBSET_TRUE;
      যদি (!c->plan->axes_index_map.has (src.axisIndex))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      যদি (unlikely (!out))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      যদি (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_মান.set_float (
	  renormalize_মান (filter_min, axis_limit, axis_distances, মিছা));
      out->filter_Range_Max_মান.set_float (
	  renormalize_মান (filter_max, axis_limit, axis_distances, মিছা));
      যদি (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;
      ঘূৰাই LOOKUP_CONDITION_SUBSET_KEEP;
    }

    পক্ষ 2:
    {
      ধ্রুৱ Condition_মান &src = u.format2;
      auto *out = c->serializer->embed (&src);
      যদি (unlikely (!out))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;

      যদি (src.varIdx == VarIdx::NO_VARIATION)
        ঘূৰাই c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((পূর্ণাঙ্ক) src.default_মান_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, পূর্ণাঙ্ক> *condition_মান;
      যদি (c->plan->layout_condition_idx_মান_map.has ((uintptr_t) &src, &condition_মান))
      {
        out->default_মান_2 = condition_মান->second;
        out->varIdx = condition_মান->first;
        যদি (out->varIdx == VarIdx::NO_VARIATION)
          ঘূৰাই out->default_মান_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        ঘূৰাই LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, পূর্ণাঙ্ক> *new_varidx_delta;
      যদি (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        ঘূৰাই c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((পূর্ণাঙ্ক) src.default_মান_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      যদি (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        দ্ৱিগুণ value = (পূর্ণাঙ্ক) src.default_মান_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        ঘূৰাই value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      পূর্ণাঙ্ক default_মান = (পূর্ণাঙ্ক) src.default_মান_2 +
			  hb_second (*new_varidx_delta);
      যদি (unlikely (!c->serializer->check_assign (
		out->default_মান_2,
		default_মান,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      ঘূৰাই LOOKUP_CONDITION_SUBSET_KEEP;
    }

    পক্ষ 3:
    {
      ধ্রুৱ ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      যদি (unlikely (!out || !c->serializer->extend_min (out)))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      বাবে (ধ্রুৱ auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        যদি (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          ঘূৰাই result;
      }
      ঘূৰাই out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    পক্ষ 4:
    {
      ধ্রুৱ ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      যদি (unlikely (!out || !c->serializer->extend_min (out)))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      বাবে (ধ্রুৱ auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        যদি (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          ঘূৰাই result;
      }
      ঘূৰাই out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    পক্ষ 5:
    {
      ধ্রুৱ ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      যদি (unlikely (!out))
        ঘূৰাই LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      যদি (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->বেৰ_কৰো_pack ());
        ঘূৰাই result;
      }

      c->serializer->বেৰ_কৰো_discard ();
      যদি (result == LOOKUP_CONDITION_SUBSET_TRUE)
        ঘূৰাই LOOKUP_CONDITION_SUBSET_FALSE;
      যদি (result == LOOKUP_CONDITION_SUBSET_FALSE)
        ঘূৰাই LOOKUP_CONDITION_SUBSET_TRUE;
      ঘূৰাই result;
    }

    স্ৱভাৱ:
      ঘূৰাই LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

সংরচনা ConditionSet
{
  তার্কিক evaluate (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		 ItemVarStoreInstancer *instancer) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = conditions.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!(এতত্+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
  }

  শূন্য keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ধ্রুৱ
  {
    hb_map_t *condition_map = hb_map_create ();
    যদি (unlikely (!condition_map)) ঘূৰাই;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    যদি (unlikely (!cond_set)) ঘূৰাই;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = সঁচা;
    তার্কিক should_keep = মিছা;
    /* The range map cannot identify value or boolean expressions uniquely. */
    তার্কিক can_deduplicate = সঁচা;
    unsigned অঙ্ক_kept_cond = 0, cond_idx = 0;
    বাবে (ধ্রুৱ auto& offset : conditions)
    {
      ধ্রুৱ Condition &condition = এতত্+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      যদি (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = মিছা;
        ঘূৰাই;
      }

      যদি (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = সঁচা;
        cond_set->add (cond_idx);
        অঙ্ক_kept_cond++;
      }

      যদি (ret == KEEP_RECORD_WITH_VAR)
        should_keep = সঁচা;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = অঙ্ক_kept_cond == 0;
    যদি (!should_keep && c->all_axes_pinned) ঘূৰাই;

    //check if condition_set is unique with variations
    যদি (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      ঘূৰাই;

    যদি (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  তার্কিক subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               তার্কিক ঢোকাও_catch_all) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিছা);

    যদি (ঢোকাও_catch_all) return_trace (সঁচা);

    hb_set_t *retained_cond_set = nullptr;
    যদি (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned পূর্ণাঙ্ক গুনতি = conditions.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (retained_cond_set != nullptr && !retained_cond_set->has (i))
        অনুৱর্ত;
      ধ্রুৱ Condition &condition = এতত্+conditions[i];
      যদি (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, এতত্) (conditions[i]);
        অনুৱর্ত;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      যদি (unlikely (!offset)) return_trace (মিছা);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, সঁচা);
      যদি (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, মিছা) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      যদি (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->বেৰ_কৰো_discard ();
        c->serializer->check_success (মিছা);
        return_trace (মিছা);
      }
      c->serializer->add_link (*offset, c->serializer->বেৰ_কৰো_pack ());
    }

    return_trace (তার্কিক (out->conditions));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (conditions.sanitize (c, এতত্));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    বাবে (ধ্রুৱ auto &offset : conditions)
      যদি (unlikely (!(এতত্+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_মানসমূহ)))
	c->layout_variation_indices->err ();
  }

  রক্ষিত:
  Array16OfOffset32To<Condition>	conditions;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2, conditions);
};

সংরচনা FeatureTableSubstitutionRecord
{
  friend সংরচনা FeatureTableSubstitution;

  শূন্য collect_lookups (ধ্রুৱ শূন্য *base, hb_set_t *lookup_indexes /* OUT */) ধ্রুৱ
  {
    ঘূৰাই (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  শূন্য closure_features (ধ্রুৱ শূন্য *base,
			 ধ্রুৱ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ধ্রুৱ
  {
    যদি ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  শূন্য collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, ধ্রুৱ Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    ধ্রুৱ hb_set_t *feature_indices,
                                                    ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    যদি (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  তার্কিক serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  ধ্রুৱ Feature *f, ধ্রুৱ Tag *tag)
  {
    TRACE_SERIALIZE (এতত্);
    hb_serialize_context_t *s = c->subset_context->serializer;
    যদি (unlikely (!s->extend_min (এতত্))) return_trace (মিছা);

    uint32_t *new_feature_idx;
    যদি (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (মিছা);

    যদি (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিছা);

    s->push ();
    তার্কিক ret = f->subset (c->subset_context, c, tag);
    যদি (ret) s->add_link (feature, s->বেৰ_কৰো_pack ());
    অন্যথা s->বেৰ_কৰো_discard ();

    return_trace (ret);
  }

  তার্কিক subset (hb_subset_layout_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    uint32_t *new_feature_index;
    যদি (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (মিছা);

    auto *out = c->subset_context->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    out->featureIndex = *new_feature_index;
    hb_pair_t<ধ্রুৱ শূন্য*, ধ্রুৱ শূন্য*> *original;
    যদি (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (মিছা);
    ধ্রুৱ Tag *tag = reinterpret_cast<ধ্রুৱ Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base,
		 ধ্রুৱ FeatureList *feature_list) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্))) return_trace (মিছা);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ধ্রুৱ Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  রক্ষিত:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা FeatureTableSubstitution
{
  শূন্য collect_feature_indices (hb_set_t *feature_indices) ধ্রুৱ
  {
    বাবে (ধ্রুৱ FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  ধ্রুৱ Feature *find_substitute (unsigned পূর্ণাঙ্ক feature_index) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = substitutions.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      ধ্রুৱ FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      যদি (record.featureIndex == feature_index)
	ঘূৰাই &(এতত্+record.feature);
    }
    ঘূৰাই nullptr;
  }

  শূন্য collect_lookups (ধ্রুৱ hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ধ্রুৱ
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([এতত্, lookup_indexes] (ধ্রুৱ FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (এতত্, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  শূন্য collect_name_ids (ধ্রুৱ hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ধ্রুৱ
  {
    বাবে (ধ্রুৱ FeatureTableSubstitutionRecord &record : substitutions)
      যদি (feature_index_map->has (record.featureIndex))
        (এতত্+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  শূন্য closure_features (ধ্রুৱ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ধ্রুৱ
  {
    বাবে (ধ্রুৱ FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (এতত্, lookup_indexes, feature_indexes);
  }

  তার্কিক intersects_features (ধ্রুৱ hb_map_t *feature_index_map) ধ্রুৱ
  {
    বাবে (ধ্রুৱ FeatureTableSubstitutionRecord& record : substitutions)
    {
      যদি (feature_index_map->has (record.featureIndex)) ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
  }

  শূন্য collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ধ্রুৱ
  {
    বাবে (ধ্রুৱ FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, এতত্);
  }

  তার্কিক subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               তার্কিক ঢোকাও_catch_all) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিছা);

    out->version.major = version.major;
    out->version.minor = version.minor;

    যদি (ঢোকাও_catch_all)
    {
      বাবে (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<ধ্রুৱ শূন্য*, ধ্রুৱ শূন্য*> *p;
        যদি (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (মিছা);
        auto *o = out->substitutions.serialize_append (c->serializer);
        যদি (!o->serialize (l, feature_index,
                           reinterpret_cast<ধ্রুৱ Feature*> (p->first),
                           reinterpret_cast<ধ্রুৱ Tag*> (p->second)))
          return_trace (মিছা);
      }
      return_trace (সঁচা);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), এতত্))
    ;

    return_trace (তার্কিক (out->substitutions));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ FeatureList *feature_list) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, এতত্, feature_list));
  }

  রক্ষিত:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

সংরচনা FeatureVariationRecord
{
  friend সংরচনা FeatureVariations;

  শূন্য collect_lookups (ধ্রুৱ শূন্য     *base,
			ধ্রুৱ hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ধ্রুৱ
  {
    ঘূৰাই (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  শূন্য collect_name_ids (ধ্রুৱ শূন্য *base,
                         ধ্রুৱ hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ধ্রুৱ
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  শূন্য closure_features (ধ্রুৱ শূন্য     *base,
			 ধ্রুৱ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ধ্রুৱ
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  তার্কিক intersects_features (ধ্রুৱ শূন্য *base, ধ্রুৱ hb_map_t *feature_index_map) ধ্রুৱ
  {
    ঘূৰাই (base+substitutions).intersects_features (feature_index_map);
  }

  শূন্য collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    (base+conditions).keep_with_variations (c);
    যদি (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = সঁচা; // set variations only once
    }
  }

  তার্কিক subset (hb_subset_layout_context_t *c, ধ্রুৱ শূন্য *base,
               তার্কিক ঢোকাও_catch_all = মিছা) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->subset_context->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    যদি (!ঢোকাও_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    অন্যথা
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, ঢোকাও_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, ঢোকাও_catch_all);

    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base,
		 ধ্রুৱ FeatureList *feature_list) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ধ্রুৱ শূন্য *base) ধ্রুৱ
  { (base+conditions).collect_variation_indices (c); }

  রক্ষিত:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা LookupIndexList
{
  শূন্য add_lookup_indexes_to (hb_set_t *lookup_indexes) ধ্রুৱ
  { lookupIndices.add_indexes_to (lookup_indexes); }

  তার্কিক intersects_lookup_indexes (ধ্রুৱ hb_map_t *lookup_indexes) ধ্রুৱ
  { ঘূৰাই lookupIndices.intersects (lookup_indexes); }

  তার্কিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (মিছা);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (তার্কিক (out->lookupIndices));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (lookupIndices.sanitize (c));
  }

  রক্ষিত:
  IndexArray	lookupIndices;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

সংরচনা LookupConditionRecord
{
  unsigned get_condition_offset (ধ্রুৱ শূন্য *base, ধ্রুৱ শূন্য *variations) ধ্রুৱ
  { ঘূৰাই (ধ্রুৱ ৱর্ণ *) &(base+condition) - (ধ্রুৱ ৱর্ণ *) variations; }

  template <typename Instancer>
  তার্কিক evaluate (ধ্রুৱ শূন্য *base,
		 ধ্রুৱ পূর্ণাঙ্ক *coords,
		 unsigned পূর্ণাঙ্ক coord_len,
		 Instancer *instancer) ধ্রুৱ
  { ঘূৰাই (base+condition).evaluate (coords, coord_len, instancer); }

  তার্কিক evaluate (ধ্রুৱ শূন্য *base,
                 ধ্রুৱ শূন্য *variations,
                 hb_sorted_array_t<ধ্রুৱ unsigned> conditions) ধ্রুৱ
  {
    ঘূৰাই !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     পরিমাণ (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  তার্কিক collect_condition (ধ্রুৱ শূন্য *base,
                          ধ্রুৱ শূন্য *variations,
                          ধ্রুৱ পূর্ণাঙ্ক *coords,
                          unsigned পূর্ণাঙ্ক coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) ধ্রুৱ
  {
    /* Null conditions always apply and need no key state. */
    যদি (!condition) ঘূৰাই সঁচা;

    unsigned offset = get_condition_offset (base, variations);
    যদি (visited->has (offset)) ঘূৰাই সঁচা;
    visited->add (offset);
    যদি (unlikely (visited->in_error ())) ঘূৰাই মিছা;

    যদি (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    ঘূৰাই !conditions->in_error ();
  }

  শূন্য add_lookup_indexes_to (ধ্রুৱ শূন্য *base,
			      hb_set_t *lookup_indexes) ধ্রুৱ
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  তার্কিক intersects_lookup_indexes (ধ্রুৱ শূন্য *base,
				   ধ্রুৱ hb_map_t *lookup_indexes) ধ্রুৱ
  { ঘূৰাই (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    যদি (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_মানসমূহ)))
      c->layout_variation_indices->err ();
  }

  তার্কিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    সঁচা);
    যদি (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->বেৰ_কৰো_discard ();
      return_trace (মিছা);
    }
    c->serializer->add_link (out->condition, c->serializer->বেৰ_কৰো_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  রক্ষিত:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা FeatureLookupsTable
{
  তার্কিক adds_default_lookups () ধ্রুৱ { ঘূৰাই flags & 0x0001u; }

  তার্কিক collect_conditions (ধ্রুৱ শূন্য *variations,
                           ধ্রুৱ পূর্ণাঙ্ক *coords,
                           unsigned পূর্ণাঙ্ক coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupConditionRecord &record : records)
      যদি (unlikely (!record.collect_condition (এতত্, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
  }

  শূন্য collect_lookups (ধ্রুৱ শূন্য *variations,
                       hb_sorted_array_t<ধ্রুৱ unsigned> conditions,
                       hb_set_t *lookup_indexes) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupConditionRecord &record : records)
      যদি (record.evaluate (এতত্, variations, conditions))
	record.add_lookup_indexes_to (এতত্, lookup_indexes);
  }

  template <typename Instancer>
  শূন্য collect_lookups (ধ্রুৱ পূর্ণাঙ্ক *coords,
			unsigned পূর্ণাঙ্ক coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupConditionRecord &record : records)
      যদি (record.evaluate (এতত্, coords, coord_len, instancer))
	record.add_lookup_indexes_to (এতত্, lookup_indexes);
  }

  শূন্য collect_lookups (hb_set_t *lookup_indexes) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (এতত্, lookup_indexes);
  }

  তার্কিক intersects_lookup_indexes (ধ্রুৱ hb_map_t *lookup_indexes) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupConditionRecord &record : records)
      যদি (record.intersects_lookup_indexes (এতত্, lookup_indexes))
	ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupConditionRecord &record : records)
      record.collect_variation_indices (c, এতত্);
  }

  তার্কিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (মিছা);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    বাবে (ধ্রুৱ LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      যদি (record.subset (c, l, এতত্))
	out->records.len++;
      অন্যথা
	c->serializer->revert (snap);
    }

    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, এতত্));
  }

  রক্ষিত:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (10, records);
};

সংরচনা LookupVariationRecord
{
  friend সংরচনা FeatureVariations;

  unsigned get_feature_index () ধ্রুৱ { ঘূৰাই featureIndex; }

  শূন্য collect_lookups (ধ্রুৱ শূন্য *base,
                       ধ্রুৱ Feature &feature,
                       hb_sorted_array_t<ধ্রুৱ unsigned> conditions,
                       hb_set_t *lookup_indexes) ধ্রুৱ
  {
    ধ্রুৱ FeatureLookupsTable &feature_lookups = base+featureLookups;
    যদি (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  শূন্য collect_lookups (ধ্রুৱ শূন্য *base,
			ধ্রুৱ hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) ধ্রুৱ
  {
    যদি (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  শূন্য closure_features (ধ্রুৱ শূন্য *base,
			 ধ্রুৱ hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) ধ্রুৱ
  {
    যদি ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ধ্রুৱ শূন্য *base) ধ্রুৱ
  { (base+featureLookups).collect_variation_indices (c); }

  তার্কিক subset (hb_subset_layout_context_t *l, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (এতত্);
    uint32_t *new_feature_index;
    যদি (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (মিছা);

    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (মিছা);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  featureLookups.sanitize (c, base));
  }

  রক্ষিত:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা FeatureVariations
{
  স্থির constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  ধ্রুৱ Array32Of<LookupVariationRecord> &get_lookup_variation_records () ধ্রুৱ
  {
    যদি (version.to_int () < 0x00010001u)
      ঘূৰাই Null (Array32Of<LookupVariationRecord>);
    ঘূৰাই StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_গুনতি () ধ্রুৱ
  {
    ঘূৰাই varRecords.len;
  }

  তার্কিক find_index (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_len,
		   unsigned পূর্ণাঙ্ক *index,
		   ItemVarStoreInstancer *instancer) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = varRecords.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      ধ্রুৱ FeatureVariationRecord &record = varRecords.arrayZ[i];
      যদি ((এতত্+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	ঘূৰাই সঁচা;
      }
    }
    *index = NOT_FOUND_INDEX;
    ঘূৰাই মিছা;
  }

  ধ্রুৱ Feature *find_substitute (unsigned পূর্ণাঙ্ক variations_index,
				  unsigned পূর্ণাঙ্ক feature_index) ধ্রুৱ
  {
    ধ্রুৱ FeatureVariationRecord &record = varRecords[variations_index];
    ঘূৰাই (এতত্+record.substitutions).find_substitute (feature_index);
  }

  তার্কিক collect_lookup_conditions (ধ্রুৱ পূর্ণাঙ্ক *coords,
                                  unsigned পূর্ণাঙ্ক coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) ধ্রুৱ
  {
    hb_set_t visited_tables, visited_conditions;
    বাবে (ধ্রুৱ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      যদি (visited_tables.has (offset)) অনুৱর্ত;
      visited_tables.add (offset);
      যদি (unlikely (visited_tables.in_error ())) ঘূৰাই মিছা;

      ধ্রুৱ FeatureLookupsTable &feature_lookups = এতত্+record.featureLookups;
      যদি (unlikely (!feature_lookups.collect_conditions (এতত্, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	ঘূৰাই মিছা;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    ঘূৰাই সঁচা;
  }

  তার্কিক get_lookup_variations (unsigned feature_index,
                              ধ্রুৱ Feature &feature,
                              hb_sorted_array_t<ধ্রুৱ unsigned> conditions,
                              hb_set_t *lookup_indexes) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      যদি (record.featureIndex != feature_index) অনুৱর্ত;

      record.collect_lookups (এতত্, feature, conditions, lookup_indexes);
      ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
  }

  শূন্য collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = varRecords.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, এতত্);
      যদি (c->universal)
        ভঙ্গ;
    }
    যদি (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed (*এতত্));
  }

  শূন্য collect_lookups (ধ্রুৱ hb_set_t *feature_indexes,
			ধ্রুৱ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) ধ্রুৱ
  {
    unsigned গুনতি = varRecords.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        অনুৱর্ত;
      varRecords[i].collect_lookups (এতত্, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  শূন্য collect_name_ids (ধ্রুৱ hb_map_t *feature_index_map,
                         ধ্রুৱ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ধ্রুৱ
  {
    বাবে (unsigned i = 0; i < varRecords.len; i++)
    {
      যদি (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        অনুৱর্ত;
      varRecords[i].collect_name_ids (এতত্, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  শূন্য collect_lookup_variation_lookups (ধ্রুৱ hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (এতত্, feature_indexes, lookup_indexes);
  }

  শূন্য closure_features (ধ্রুৱ hb_map_t *lookup_indexes,
			 ধ্রুৱ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক গুনতি = varRecords.len;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        অনুৱর্ত;
      varRecords[i].closure_features (এতত্, lookup_indexes, feature_indexes);
    }

    বাবে (ধ্রুৱ LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (এতত্, lookup_indexes, feature_indexes);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    বাবে (ধ্রুৱ FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, এতত্);
    বাবে (ধ্রুৱ LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, এতত্);
  }

  শূন্য collect_feature_indices (hb_set_t *feature_indices) ধ্রুৱ
  {
    বাবে (ধ্রুৱ FeatureVariationRecord &record : varRecords)
      (এতত্+record.substitutions).collect_feature_indices (feature_indices);
  }

  তার্কিক has_lookup_variations (unsigned feature_index) ধ্রুৱ
  {
    বাবে (ধ্রুৱ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      যদি (record.featureIndex == feature_index)
	ঘূৰাই সঁচা;
      যদি (record.featureIndex > feature_index)
	ভঙ্গ;
    }
    ঘূৰাই মিছা;
  }

  তার্কিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিছা);

    out->version.major = version.major;
    out->version.minor = version.minor;

    পূর্ণাঙ্ক keep_up_to = -1;
    বাবে (পূর্ণাঙ্ক i = varRecords.len - 1; i >= 0; i--) {
      যদি (varRecords[i].intersects_features (এতত্, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        ভঙ্গ;
      }
    }

    unsigned গুনতি = (unsigned) (keep_up_to + 1);
    বাবে (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        অনুৱর্ত;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), এতত্) (varRecords[i]);
    }

    যদি (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      তার্কিক ঢোকাও_catch_all_record = সঁচা;
      subset_record_array (l, &(out->varRecords), এতত্, ঢোকাও_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    যদি (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      যদি (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (মিছা);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, এতত্))
      ;
    }

    return_trace (তার্কিক (out->varRecords) ||
		  (out_lookup_records && তার্কিক (*out_lookup_records)));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ FeatureList *feature_list) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, এতত্, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, এতত্)));
  }

  রক্ষিত:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

সংরচনা HintingDevice
{
  friend সংরচনা Device;

  গুপ্ত:

  hb_position_t get_x_delta (hb_font_t *font) ধ্রুৱ
  { ঘূৰাই get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ধ্রুৱ
  { ঘূৰাই get_delta (font->y_ppem, font->y_scale); }

  সার্ৱজনিক:

  size_t get_size () ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক f = deltaFormat;
    যদি (unlikely (f < 1 || f > 3 || startSize > endSize)) ঘূৰাই 3 * HBUINT16::static_size;
    ঘূৰাই HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && c->check_range (এতত্, এতত্->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (c->embed<HintingDevice> (এতত্));
  }

  গুপ্ত:

  পূর্ণাঙ্ক get_delta (unsigned পূর্ণাঙ্ক ppem, পূর্ণাঙ্ক scale) ধ্রুৱ
  {
    যদি (!ppem) ঘূৰাই 0;

    পূর্ণাঙ্ক pixels = get_delta_pixels (ppem);

    যদি (!pixels) ঘূৰাই 0;

    ঘূৰাই (পূর্ণাঙ্ক) (pixels * (int64_t) scale / ppem);
  }
  পূর্ণাঙ্ক get_delta_pixels (unsigned পূর্ণাঙ্ক ppem_size) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক f = deltaFormat;
    যদি (unlikely (f < 1 || f > 3))
      ঘূৰাই 0;

    যদি (ppem_size < startSize || ppem_size > endSize)
      ঘূৰাই 0;

    unsigned পূর্ণাঙ্ক s = ppem_size - startSize;

    unsigned পূর্ণাঙ্ক byte = delta_মান_Z[s >> (4 - f)];
    unsigned পূর্ণাঙ্ক bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned পূর্ণাঙ্ক mask = (0xFFFFu >> (16 - (1 << f)));

    পূর্ণাঙ্ক delta = bits & mask;

    যদি ((unsigned পূর্ণাঙ্ক) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    ঘূৰাই delta;
  }

  রক্ষিত:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_মান_Z;		/* Array of compressed data */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, delta_মান_Z);
};

সংরচনা VariationDevice
{
  friend সংরচনা Device;

  গুপ্ত:

  hb_position_t get_x_delta (hb_font_t *font,
			     ধ্রুৱ ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ধ্রুৱ
  { ঘূৰাই !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ধ্রুৱ ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ধ্রুৱ
  { ঘূৰাই !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  ভিন্ন get_x_delta_f (hb_font_t *font,
		       ধ্রুৱ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ধ্রুৱ
  { ঘূৰাই !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  ভিন্ন get_y_delta_f (hb_font_t *font,
		       ধ্রুৱ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ধ্রুৱ
  { ঘূৰাই !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    যদি (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, পূর্ণাঙ্ক> *v;
    যদি (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    যদি (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (এতত্->static_size);
    auto *out = c->embed (এতত্);
    যদি (unlikely (!out)) return_trace (nullptr);

    যদি (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  শূন্য collect_variation_index (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  { c->layout_variation_indices->add (varIdx); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  গুপ্ত:

  ভিন্ন get_delta (hb_font_t *font,
		   ধ্রুৱ ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) ধ্রুৱ
  {
    ঘূৰাই store.get_delta (varIdx, font->coords, font->অঙ্ক_coords, store_cache);
  }

  রক্ষিত:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা DeviceHeader
{
  রক্ষিত:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  সার্ৱজনিক:
  HBUINT16		format;		/* Format identifier */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  ভিন্ন get_x_delta_f (hb_font_t *font,
		       ধ্রুৱ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ধ্রুৱ
  {
#ifndef HB_NO_VAR
    যদি (u.b.format == 0x8000)
    {
      hb_barrier ();
      ঘূৰাই u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    ঘূৰাই get_x_delta (font, store, store_cache);
  }

  ভিন্ন get_y_delta_f (hb_font_t *font,
		       ধ্রুৱ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ধ্রুৱ
  {
#ifndef HB_NO_VAR
    যদি (u.b.format == 0x8000)
    {
      hb_barrier ();
      ঘূৰাই u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    ঘূৰাই get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ধ্রুৱ ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ধ্রুৱ
  {
    ৱিকল্প (u.b.format)
    {
#ifndef HB_NO_HINTING
    পক্ষ 1: পক্ষ 2: পক্ষ 3:
      hb_barrier ();
      ঘূৰাই u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    পক্ষ 0x8000:
      hb_barrier ();
      ঘূৰাই u.variation.get_x_delta (font, store, store_cache);
#endif
    স্ৱভাৱ:
      ঘূৰাই 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ধ্রুৱ ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ধ্রুৱ
  {
    ৱিকল্প (u.b.format)
    {
    পক্ষ 1: পক্ষ 2: পক্ষ 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      ঘূৰাই u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    পক্ষ 0x8000:
      hb_barrier ();
      ঘূৰাই u.variation.get_y_delta (font, store, store_cache);
#endif
    স্ৱভাৱ:
      ঘূৰাই 0;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.b.format.sanitize (c)) return_trace (মিছা);
    ৱিকল্প (u.b.format) {
#ifndef HB_NO_HINTING
    পক্ষ 1: পক্ষ 2: পক্ষ 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    পক্ষ 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    স্ৱভাৱ:
      return_trace (সঁচা);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map=nullptr) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    ৱিকল্প (u.b.format) {
#ifndef HB_NO_HINTING
    পক্ষ 1:
    পক্ষ 2:
    পক্ষ 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    পক্ষ 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    স্ৱভাৱ:
      return_trace (nullptr);
    }
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    ৱিকল্প (u.b.format) {
#ifndef HB_NO_HINTING
    পক্ষ 1:
    পক্ষ 2:
    পক্ষ 3:
      ঘূৰাই;
#endif
#ifndef HB_NO_VAR
    পক্ষ 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      ঘূৰাই;
#endif
    স্ৱভাৱ:
      ঘূৰাই;
    }
  }

  unsigned get_variation_index () ধ্রুৱ
  {
    ৱিকল্প (u.b.format) {
#ifndef HB_NO_VAR
    পক্ষ 0x8000:
      hb_barrier ();
      ঘূৰাই u.variation.varIdx;
#endif
    স্ৱভাৱ:
      ঘূৰাই HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  তার্কিক is_variation_device () ধ্রুৱ
  {
    ৱিকল্প (u.b.format) {
#ifndef HB_NO_VAR
    পক্ষ 0x8000:
      ঘূৰাই সঁচা;
#endif
    স্ৱভাৱ:
      ঘূৰাই মিছা;
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

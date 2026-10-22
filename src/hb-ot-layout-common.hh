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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-ot-layout.hh"
#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-set.hh"
#অন্তর্ভাব "hb-bimap.hh"
#অন্তর্ভাব "hb-cache.hh"
#অন্তর্ভাব "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (আকার (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_দ্বিমিক_cache_t = hb_cache_t<14, 1, 8>;
static_assert (আকার (hb_ot_layout_দ্বিমিক_cache_t) == 256, "");

#অন্তর্ভাব "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


নামাকাশ OT {

template<typename Iterator>
স্থির অন্তঃরেখ যৌক্তিক ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

স্থির যৌক্তিক ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ধ্রুবক hb_set_t &klasses,
    যৌক্তিক use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

কাঠামো ItemVarStoreInstancer;

কাঠামো hb_collect_feature_substitutes_with_var_context_t
{
  ধ্রুবক hb_map_t *axes_সূচক_tag_map;
  ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, ধ্রুবক Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  যৌক্তিক apply;
  যৌক্তিক variation_applied;
  যৌক্তিক universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  যৌক্তিক all_axes_pinned;
};

কাঠামো hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ধ্রুবক শূন্য         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              ধ্রুবক hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_গুনতি (0),langsys_feature_গুনতি (0) {}

  যৌক্তিক visitScript ()
  { ফেরত script_গুনতি++ < HB_MAX_SCRIPTS; }

  যৌক্তিক visitLangsys (unsigned feature_গুনতি_2)
  {
    langsys_feature_গুনতি += feature_গুনতি_2;
    ফেরত langsys_feature_গুনতি < HB_MAX_LANGSYS_FEATURE_গুনতি;
  }

  সর্বজনীন:
  ধ্রুবক শূন্য *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ধ্রুবক hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  ব্যক্তিগত:
  unsigned script_গুনতি;
  unsigned langsys_feature_গুনতি;
};

কাঠামো hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  ধ্রুবক অক্ষর *get_name () { ফেরত "SUBSET_LAYOUT"; }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }

  যৌক্তিক visitScript ()
  {
    ফেরত script_গুনতি++ < HB_MAX_SCRIPTS;
  }

  যৌক্তিক visitLangSys ()
  {
    ফেরত langsys_গুনতি++ < HB_MAX_LANGSYS;
  }

  যৌক্তিক visit_Feature_সূচক (পূর্ণসংখ্যা গুনতি)
  {
    feature_সূচক_গুনতি += গুনতি;
    ফেরত feature_সূচক_গুনতি < HB_MAX_FEATURE_INDICES;
  }

  যৌক্তিক visit_Lookup_সূচক()
  {
    lookup_সূচক_গুনতি++;
    ফেরত lookup_সূচক_গুনতি < HB_MAX_LOOKUP_VISIT_গুনতি;
  }

  hb_subset_context_t *subset_context;
  ধ্রুবক hb_tag_t table_tag;
  ধ্রুবক hb_map_t *lookup_সূচক_map;
  ধ্রুবক hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ধ্রুবক hb_map_t *feature_সূচক_map;
  ধ্রুবক hb_map_t *feature_map_w_duplicates;
  ধ্রুবক hb_hashmap_t<unsigned, ধ্রুবক Feature*> *feature_substitutes_map;
  ধ্রুবক hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  ধ্রুবক hb_set_t *catch_all_record_feature_idxes;
  ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<ধ্রুবক শূন্য*, ধ্রুবক শূন্য*>> *feature_idx_tag_map;

  unsigned cur_script_সূচক;
  unsigned cur_feature_সূচক;
  unsigned cur_feature_var_record_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_tag_t tag_) :
				subset_context (c_),
				table_tag (tag_),
				cur_script_সূচক (0xFFFFu),
				cur_feature_সূচক (সূচক::NOT_FOUND_সূচক),
				cur_feature_var_record_idx (0u),
				script_গুনতি (0),
				langsys_গুনতি (0),
				feature_সূচক_গুনতি (0),
				lookup_সূচক_গুনতি (0)
  {
    যদি (tag_ == HB_OT_TAG_GSUB)
    {
      lookup_সূচক_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_সূচক_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_record_cond_idx_map = c_->plan->user_axes_location.is_empty () ? nullptr : &c_->plan->gsub_feature_record_cond_idx_map;
      catch_all_record_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_tag_map = &c_->plan->gsub_old_feature_idx_tag_map;
    }
    অন্যথা
    {
      lookup_সূচক_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_সূচক_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_record_cond_idx_map = c_->plan->user_axes_location.is_empty () ? nullptr : &c_->plan->gpos_feature_record_cond_idx_map;
      catch_all_record_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_tag_map = &c_->plan->gpos_old_feature_idx_tag_map;
    }
  }

  ব্যক্তিগত:
  unsigned script_গুনতি;
  unsigned langsys_গুনতি;
  unsigned feature_সূচক_গুনতি;
  unsigned lookup_সূচক_গুনতি;
};

কাঠামো ItemVariationStore;
কাঠামো hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { obj.collect_variation_indices (এই); ফেরত hb_empty_t (); }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  ধ্রুবক hb_set_t *glyph_set;
  ধ্রুবক hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *condition_মানসমূহ = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  ধ্রুবক hb_set_t *glyph_set_,
					  ধ্রুবক hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
কাঠামো subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 ধ্রুবক শূন্য *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  যৌক্তিক operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    যদি (unlikely (!o)) ফেরত মিথ্যা;
    যৌক্তিক ret = o->serialize_subset (subset_context, offset, base);
    যদি (!ret)
    {
      out.বের_করো ();
      subset_context->serializer->revert (snap);
    }
    ফেরত ret;
  }

  ব্যক্তিগত:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ধ্রুবক শূন্য *base;
};


template<typename OutputArray, typename Arg>
কাঠামো subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     ধ্রুবক শূন্য *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  যৌক্তিক operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    যদি (unlikely (!o)) ফেরত মিথ্যা;
    যৌক্তিক ret = o->serialize_subset (subset_context, offset, base, arg);
    যদি (!ret)
    {
      out.বের_করো ();
      subset_context->serializer->revert (snap);
    }
    ফেরত ret;
  }

  ব্যক্তিগত:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ধ্রুবক শূন্য *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
কাঠামো
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ধ্রুবক শূন্য *base) ধ্রুবক
  { ফেরত subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ধ্রুবক শূন্য *base, Arg &&arg) ধ্রুবক
  { ফেরত subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
কাঠামো subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 ধ্রুবক শূন্য *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  শূন্য
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    যৌক্তিক ret = record.subset (subset_layout_context, base);
    যদি (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    অন্যথা out->len++;
  }

  ব্যক্তিগত:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ধ্রুবক শূন্য *base;
};

template<typename OutputArray, typename Arg>
কাঠামো subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     ধ্রুবক শূন্য *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  শূন্য
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    যৌক্তিক ret = record.subset (subset_layout_context, base, arg);
    যদি (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    অন্যথা out->len++;
  }

  ব্যক্তিগত:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ধ্রুবক শূন্য *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
কাঠামো
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       ধ্রুবক শূন্য *base) ধ্রুবক
  { ফেরত subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               ধ্রুবক শূন্য *base, Arg &&arg) ধ্রুবক
  { ফেরত subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
কাঠামো serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         ধ্রুবক শূন্য *base_,
                         ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  যৌক্তিক operator () (T&& record)
  {
    যদি (!serialize_context->copy (record, base, variation_map)) ফেরত মিথ্যা;
    out.len++;
    ফেরত সত্য;
  }

  ব্যক্তিগত:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  ধ্রুবক শূন্য *base;
  ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
কাঠামো
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               ধ্রুবক শূন্য *base,
               ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *variation_map) ধ্রুবক
  { ফেরত serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

কাঠামো সূচক_Array : Array16Of<সূচক>
{
  যৌক্তিক intersects (ধ্রুবক hb_map_t *indexes) ধ্রুবক
  { ফেরত hb_any (*এই, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    যদি (!it) ফেরত;
    যদি (unlikely (!c->extend_min ((*এই)))) ফেরত;

    জন্য (ধ্রুবক auto _ : it)
    {
      যদি (!l->visit_Lookup_সূচক()) বিরতি;

      সূচক i;
      i = _;
      c->copy (i);
      এই->len++;
    }
  }

  unsigned পূর্ণসংখ্যা get_indexes (unsigned পূর্ণসংখ্যা start_offset,
			    unsigned পূর্ণসংখ্যা *গুনতি_2 /* IN/OUT */,
			    unsigned পূর্ণসংখ্যা *_indexes /* OUT */) ধ্রুবক
  {
    যদি (গুনতি_2 && _indexes)
    {
      + এই->as_array ().sub_array (start_offset, গুনতি_2)
      | hb_sink (hb_array (_indexes, *গুনতি_2))
      ;
    }
    ফেরত এই->len;
  }

  শূন্য add_indexes_to (hb_set_t* output /* OUT */) ধ্রুবক
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
কাঠামো Feature_Params_আকার
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই))) return_trace (মিথ্যা);
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

    যদি (!design_আকার)
      return_trace (মিথ্যা);
    অন্যথা যদি (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (সত্য);
    অন্যথা যদি (design_আকার < rangeStart ||
	     design_আকার > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (মিথ্যা);
    অন্যথা
      return_trace (সত্য);
  }

  শূন্য collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ধ্রুবক
  { nameids_to_retain->add (subfamilyNameID); }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    return_trace ((যৌক্তিক) c->serializer->embed (*এই));
  }

  HBUINT16	design_আকার;	/* Represents the design size in 720/inch
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
কাঠামো FeatureParamsStylisticSet
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (এই));
  }

  শূন্য collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ধ্রুবক
  { nameids_to_retain->add (uiNameID); }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    return_trace ((যৌক্তিক) c->serializer->embed (*এই));
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
কাঠামো FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_গুনতি, hb_codepoint_t *chars) ধ্রুবক
  {
    যদি (char_গুনতি && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_গুনতি)
      | hb_sink (hb_array (chars, *char_গুনতি))
      ;
    }
    ফেরত characters.len;
  }

  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_আকার, min_আকার); }

  শূন্য collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ধ্রুবক
  {
    যদি (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    যদি (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    যদি (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    যদি (!firstParamUILabelNameID || !অঙ্ক_Named_Parameters || অঙ্ক_Named_Parameters >= 0x7FFF)
      ফেরত;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) অঙ্ক_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    return_trace ((যৌক্তিক) c->serializer->embed (*এই));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
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
  সর্বজনীন:
  DEFINE_আকার_ARRAY (14, characters);
};

কাঠামো FeatureParams
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c, hb_tag_t tag) ধ্রুবক
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ফেরত সত্য;
#endif
    TRACE_SANITIZE (এই);
    যদি (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.আকার_2.sanitize (c));
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (সত্য);
  }

  শূন্য collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ধ্রুবক
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ফেরত;
#endif
    যদি (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ফেরত (u.আকার_2.collect_name_ids (nameids_to_retain));
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ফেরত (u.stylisticSet.collect_name_ids (nameids_to_retain));
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ফেরত (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  যৌক্তিক subset (hb_subset_context_t *c, ধ্রুবক Tag* tag) ধ্রুবক
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ফেরত মিথ্যা;
#endif
    TRACE_SUBSET (এই);
    যদি (!tag) return_trace (মিথ্যা);
    যদি (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.আকার_2.subset (c));
    যদি ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    যদি ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (মিথ্যা);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  ধ্রুবক Feature_Params_আকার& get_আকার_params (hb_tag_t tag) ধ্রুবক
  {
    যদি (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ফেরত u.আকার_2;
    ফেরত Null (Feature_Params_আকার);
  }
  ধ্রুবক FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) ধ্রুবক
  {
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ফেরত u.stylisticSet;
    ফেরত Null (FeatureParamsStylisticSet);
  }
  ধ্রুবক FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) ধ্রুবক
  {
    যদি ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ফেরত u.characterVariants;
    ফেরত Null (FeatureParamsCharacterVariants);
  }
#endif

  ব্যক্তিগত:
  সংঘ {
  Feature_Params_আকার			আকার_2;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  সর্বজনীন:
  DEFINE_আকার_MIN (0);
};

কাঠামো Record_sanitize_closure_t {
  hb_tag_t tag;
  ধ্রুবক শূন্য *list_base;
};

কাঠামো Feature
{
  unsigned পূর্ণসংখ্যা get_lookup_গুনতি () ধ্রুবক
  { ফেরত lookup_সূচক_2.len; }
  hb_tag_t get_lookup_সূচক (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত lookup_সূচক_2[i]; }
  unsigned পূর্ণসংখ্যা get_lookup_indexes (unsigned পূর্ণসংখ্যা start_সূচক,
				   unsigned পূর্ণসংখ্যা *lookup_গুনতি /* IN/OUT */,
				   unsigned পূর্ণসংখ্যা *lookup_tags /* OUT */) ধ্রুবক
  { ফেরত lookup_সূচক_2.get_indexes (start_সূচক, lookup_গুনতি, lookup_tags); }
  শূন্য add_lookup_indexes_to (hb_set_t *lookup_indexes) ধ্রুবক
  { lookup_সূচক_2.add_indexes_to (lookup_indexes); }

  ধ্রুবক FeatureParams &get_feature_params () ধ্রুবক
  { ফেরত এই+featureParams; }

  যৌক্তিক intersects_lookup_indexes (ধ্রুবক hb_map_t *lookup_indexes) ধ্রুবক
  { ফেরত lookup_সূচক_2.intersects (lookup_indexes); }

  শূন্য collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ধ্রুবক
  {
    যদি (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  যৌক্তিক subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ধ্রুবক Tag                   *tag = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    out->featureParams.serialize_subset (c, featureParams, এই, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    যদি (l->cur_feature_সূচক != সূচক::NOT_FOUND_সূচক &&
	l->lookup_variations->has (l->cur_feature_সূচক, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_সূচক_map)
      | hb_map (l->lookup_সূচক_map)
      ;
      out->lookup_সূচক_2.serialize (c->serializer, l, it);
    }
    অন্যথা
    {
      auto it =
      + hb_iter (lookup_সূচক_2)
      | hb_filter (l->lookup_সূচক_map)
      | hb_map (l->lookup_সূচক_map)
      ;
      out->lookup_সূচক_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক Record_sanitize_closure_t *closure = nullptr) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  featureParams.sanitize (c, এই, closure ? closure->tag : HB_TAG_NONE) &&
		  lookup_সূচক_2.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  সূচক_Array	 lookup_সূচক_2;	/* Array of LookupList indices */
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (4, lookup_সূচক_2);
};

template <typename Type>
কাঠামো Record
{
  পূর্ণসংখ্যা cmp (hb_tag_t a) ধ্রুবক { ফেরত tag.cmp (a); }

  যৌক্তিক subset (hb_subset_layout_context_t *c, ধ্রুবক শূন্য *base, ধ্রুবক শূন্য *f_sub = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->subset_context->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    ধ্রুবক Feature& f = *reinterpret_cast<ধ্রুবক Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    যৌক্তিক ret = f.subset (c->subset_context, c, &tag);
    যদি (ret)
      s->add_link (out->offset, s->বের_করো_pack ());
    অন্যথা
      s->বের_করো_discard ();

    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    ধ্রুবক Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (এই) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

template <typename Type>
কাঠামো RecordArrayOf : SortedArray16Of<Record<Type>>
{
  ধ্রুবক Offset16To<Type>& get_offset (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত (*এই)[i].offset; }
  Offset16To<Type>& get_offset (unsigned পূর্ণসংখ্যা i)
  { ফেরত (*এই)[i].offset; }
  ধ্রুবক Tag& get_tag (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত (*এই)[i].tag; }
  unsigned পূর্ণসংখ্যা get_tags (unsigned পূর্ণসংখ্যা start_offset,
                         unsigned পূর্ণসংখ্যা *record_গুনতি /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) ধ্রুবক
  {
    যদি (record_গুনতি && record_tags)
    {
      + এই->as_array ().sub_array (start_offset, record_গুনতি)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_গুনতি))
      ;
    }
    ফেরত এই->len;
  }
  যৌক্তিক find_সূচক (hb_tag_t tag, unsigned পূর্ণসংখ্যা *সূচক_2) ধ্রুবক
  {
    ফেরত এই->bfind (tag, সূচক_2, HB_NOT_FOUND_STORE, সূচক::NOT_FOUND_সূচক);
  }
};

template <typename Type>
কাঠামো RecordListOf : RecordArrayOf<Type>
{
  ধ্রুবক Type& operator [] (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত এই+এই->get_offset (i); }

  যৌক্তিক subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    + এই->iter ()
    | hb_apply (subset_record_array (l, out, এই))
    ;
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (RecordArrayOf<Type>::sanitize (c, এই));
  }
};

কাঠামো RecordListOfFeature : RecordListOf<Feature>
{
  যৌক্তিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    + hb_enumerate (*এই)
    | hb_filter (l->feature_সূচক_map, hb_first)
    | hb_apply ([l, out, এই] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক Record<Feature>&>& _)
                {
                  ধ্রুবক Feature *f_sub = nullptr;
                  ধ্রুবক Feature **f = nullptr;
                  যদি (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_সূচক = _.first;
                  subset_record_array (l, out, এই, f_sub) (_.second);
                  l->cur_feature_সূচক = সূচক::NOT_FOUND_সূচক;
                })
    ;

    return_trace (সত্য);
  }
};

প্রকারলেখ RecordListOf<Feature> FeatureList;


কাঠামো LangSys
{
  unsigned পূর্ণসংখ্যা get_feature_গুনতি () ধ্রুবক
  { ফেরত feature_সূচক_2.len; }
  hb_tag_t get_feature_সূচক (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত feature_সূচক_2[i]; }
  unsigned পূর্ণসংখ্যা get_feature_indexes (unsigned পূর্ণসংখ্যা start_offset,
				    unsigned পূর্ণসংখ্যা *feature_গুনতি_2 /* IN/OUT */,
				    unsigned পূর্ণসংখ্যা *feature_indexes /* OUT */) ধ্রুবক
  { ফেরত feature_সূচক_2.get_indexes (start_offset, feature_গুনতি_2, feature_indexes); }
  শূন্য add_feature_indexes_to (hb_set_t *feature_indexes) ধ্রুবক
  { feature_সূচক_2.add_indexes_to (feature_indexes); }

  যৌক্তিক has_required_feature () ধ্রুবক { ফেরত req_Feature_সূচক != 0xFFFFu; }
  unsigned পূর্ণসংখ্যা get_required_feature_সূচক () ধ্রুবক
  {
    যদি (req_Feature_সূচক == 0xFFFFu)
      ফেরত সূচক::NOT_FOUND_সূচক;
   ফেরত req_Feature_সূচক;
  }

  LangSys* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed (*এই));
  }

  যৌক্তিক compare (ধ্রুবক LangSys& o, ধ্রুবক hb_map_t *feature_সূচক_map) ধ্রুবক
  {
    যদি (req_Feature_সূচক != o.req_Feature_সূচক)
      ফেরত মিথ্যা;

    auto iter =
    + hb_iter (feature_সূচক_2)
    | hb_filter (feature_সূচক_map)
    | hb_map (feature_সূচক_map)
    ;

    auto o_iter =
    + hb_iter (o.feature_সূচক_2)
    | hb_filter (feature_সূচক_map)
    | hb_map (feature_সূচক_map)
    ;

    জন্য (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      যদি (a != b) ফেরত মিথ্যা;
    }

    যদি (iter || o_iter) ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  শূন্য collect_features (hb_prune_langsys_context_t *c) ধ্রুবক
  {
    যদি (!has_required_feature () && !get_feature_গুনতি ()) ফেরত;
    যদি (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_সূচক))
      c->new_feature_indexes->add (get_required_feature_সূচক ());

    + hb_iter (feature_সূচক_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  যৌক্তিক subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ধ্রুবক Tag                  *tag = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    ধ্রুবক uint32_t *v;
    out->req_Feature_সূচক = l->feature_map_w_duplicates->has (req_Feature_সূচক, &v) ? *v : 0xFFFFu;

    যদি (!l->visit_Feature_সূচক (feature_সূচক_2.len))
      return_trace (মিথ্যা);

    auto it =
    + hb_iter (feature_সূচক_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    যৌক্তিক ret = যৌক্তিক (it);
    out->feature_সূচক_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক Record_sanitize_closure_t * = nullptr) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && feature_সূচক_2.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	req_Feature_সূচক;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  সূচক_Array	feature_সূচক_2;	/* Array of indices into the FeatureList */
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (6, feature_সূচক_2);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

কাঠামো Script
{
  unsigned পূর্ণসংখ্যা get_lang_sys_গুনতি () ধ্রুবক
  { ফেরত langSys.len; }
  ধ্রুবক Tag& get_lang_sys_tag (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত langSys.get_tag (i); }
  unsigned পূর্ণসংখ্যা get_lang_sys_tags (unsigned পূর্ণসংখ্যা start_offset,
				  unsigned পূর্ণসংখ্যা *lang_sys_গুনতি /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ধ্রুবক
  { ফেরত langSys.get_tags (start_offset, lang_sys_গুনতি, lang_sys_tags); }
  ধ্রুবক LangSys& get_lang_sys (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
    যদি (i == সূচক::NOT_FOUND_সূচক) ফেরত get_default_lang_sys ();
    ফেরত এই+langSys[i].offset;
  }
  যৌক্তিক find_lang_sys_সূচক (hb_tag_t tag, unsigned পূর্ণসংখ্যা *সূচক_2) ধ্রুবক
  { ফেরত langSys.find_সূচক (tag, সূচক_2); }

  যৌক্তিক has_default_lang_sys () ধ্রুবক           { ফেরত defaultLangSys != 0; }
  ধ্রুবক LangSys& get_default_lang_sys () ধ্রুবক { ফেরত এই+defaultLangSys; }

  শূন্য prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_সূচক) ধ্রুবক
  {
    যদি (!has_default_lang_sys () && !get_lang_sys_গুনতি ()) ফেরত;
    যদি (!c->visitScript ()) ফেরত;

    যদি (!c->script_langsys_map->has (script_সূচক))
    {
      যদি (unlikely (!c->script_langsys_map->set (script_সূচক, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	ফেরত;
    }

    যদি (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ধ্রুবক LangSys& d = get_default_lang_sys ();
      যদি (c->visitLangsys (d.get_feature_গুনতি ())) {
        d.collect_features (c);
      }

      জন্য (auto _ : + hb_enumerate (langSys))
      {
        ধ্রুবক LangSys& l = এই+_.second.offset;
        যদি (!c->visitLangsys (l.get_feature_গুনতি ())) চালু;
        যদি (l.compare (d, c->duplicate_feature_map)) চালু;

        l.collect_features (c);
        c->script_langsys_map->get (script_সূচক)->add (_.first);
      }
    }
    অন্যথা
    {
      জন্য (auto _ : + hb_enumerate (langSys))
      {
        ধ্রুবক LangSys& l = এই+_.second.offset;
        যদি (!c->visitLangsys (l.get_feature_গুনতি ())) চালু;
        l.collect_features (c);
        c->script_langsys_map->get (script_সূচক)->add (_.first);
      }
    }
  }

  যৌক্তিক subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ধ্রুবক Tag                   *tag) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (!l->visitScript ()) return_trace (মিথ্যা);
    যদি (tag && !c->plan->layout_scripts.has (*tag))
      ফেরত মিথ্যা;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    যৌক্তিক defaultLang = মিথ্যা;
    যদি (has_default_lang_sys ())
    {
      c->serializer->push ();
      ধ্রুবক LangSys& ls = এই+defaultLangSys;
      যৌক্তিক ret = ls.subset (c, l);
      যদি (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->বের_করো_discard ();
	out->defaultLangSys = 0;
      }
      অন্যথা
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->বের_করো_pack ());
	defaultLang = সত্য;
      }
    }

    ধ্রুবক hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_সূচক);
    যদি (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (ধ্রুবক Record<LangSys>& record) {ফেরত l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), এই))
      ;
    }

    return_trace (যৌক্তিক (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক Record_sanitize_closure_t * = nullptr) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (defaultLangSys.sanitize (c, এই) && langSys.sanitize (c, এই));
  }

  সুরক্ষিত:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (4, langSys);
};

কাঠামো RecordListOfScript : RecordListOf<Script>
{
  যৌক্তিক subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    জন্য (auto _ : + hb_enumerate (*এই))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_সূচক = _.first;
      যৌক্তিক ret = _.second.subset (l, এই);
      যদি (!ret) c->serializer->revert (snap);
      অন্যথা out->len++;
    }

    return_trace (সত্য);
  }
};

প্রকারলেখ RecordListOfScript ScriptList;



কাঠামো LookupFlag : HBUINT16
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
নামাকাশ OT {

কাঠামো Lookup
{
  unsigned পূর্ণসংখ্যা get_subtable_গুনতি () ধ্রুবক { ফেরত subTable.len; }

  template <typename TSubTable>
  ধ্রুবক Array16OfOffset16To<TSubTable>& get_subtables () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { ফেরত reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  ধ্রুবক TSubTable& get_subtable (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত এই+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned পূর্ণসংখ্যা i)
  { ফেরত এই+get_subtables<TSubTable> ()[i]; }

  size_t get_আকার () ধ্রুবক
  {
    ধ্রুবক HBUINT16 &markFilteringSet = StructAfter<ধ্রুবক HBUINT16> (subTable);
    যদি (lookupFlag & LookupFlag::UseMarkFilteringSet)
      ফেরত (ধ্রুবক অক্ষর *) &StructAfter<ধ্রুবক অক্ষর> (markFilteringSet) - (ধ্রুবক অক্ষর *) এই;
    ফেরত (ধ্রুবক অক্ষর *) &markFilteringSet - (ধ্রুবক অক্ষর *) এই;
  }

  unsigned পূর্ণসংখ্যা get_type () ধ্রুবক { ফেরত lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা flag = lookupFlag;
    যদি (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      ধ্রুবক HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    ফেরত flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা lookup_type = get_type ();
    TRACE_DISPATCH (এই, lookup_type);
    unsigned পূর্ণসংখ্যা গুনতি = get_subtable_গুনতি ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      যদি (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_মান ());
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  unsigned পূর্ণসংখ্যা lookup_type,
		  uint32_t lookup_props,
		  unsigned পূর্ণসংখ্যা অঙ্ক_subtables)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    যদি (unlikely (!subTable.serialize (c, অঙ্ক_subtables))) return_trace (মিথ্যা);
    যদি (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      যদি (unlikely (!c->extend (এই))) return_trace (মিথ্যা);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (সত্য);
  }

  template <typename TSubTable>
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    ধ্রুবক hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned পূর্ণসংখ্যা lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([এই, glyphset, lookup_type] (ধ্রুবক Offset16To<TSubTable> &_) { ফেরত (এই+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), এই, lookup_type))
    ;

    যদি (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ধ্রুবক HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
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
        যদি (unlikely (!c->serializer->extend (out))) return_trace (মিথ্যা);
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
    return_trace (সত্য);
  }

  template <typename TSubTable>
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(c->check_struct (এই) && subTable.sanitize (c))) return_trace (মিথ্যা);
    hb_barrier ();

    unsigned subtables = get_subtable_গুনতি ();
    যদি (unlikely (!c->visit_subtables (subtables))) return_trace (মিথ্যা);

    যদি (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ধ্রুবক HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      যদি (!markFilteringSet.sanitize (c)) return_trace (মিথ্যা);
    }

    যদি (unlikely (!get_subtables<TSubTable> ().sanitize (c, এই, get_type ())))
      return_trace (মিথ্যা);

    যদি (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned পূর্ণসংখ্যা type = get_subtable<TSubTable> (0).u.extension.get_type ();
      জন্য (unsigned পূর্ণসংখ্যা i = 1; i < subtables; i++)
	যদি (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (মিথ্যা);
    }
    return_trace (সত্য);
  }

  সুরক্ষিত:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
কাঠামো LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  যৌক্তিক subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    + hb_enumerate (*এই)
    | hb_filter (l->lookup_সূচক_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, এই))
    ;
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, এই)));
  }
};


/*
 * Coverage Table
 */


স্থির যৌক্তিক ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ধ্রুবক hb_set_t &klasses,
                                          যৌক্তিক use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  যদি (!klass_map)
    ফেরত ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  যদি (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  জন্য (ধ্রুবক unsigned k: klasses)
  {
    যদি (klass_map->has (k)) চালু;
    klass_map->set (k, idx);
    idx++;
  }


  যদি (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    ফেরত মিথ্যা;

  জন্য (unsigned i = 0; i < glyph_and_klass.দৈর্ঘ্য; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  ফেরত ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
কাঠামো ClassDefFormat1_3
{
  friend কাঠামো ClassDef;

  ব্যক্তিগত:
  unsigned পূর্ণসংখ্যা get_class (hb_codepoint_t glyph_id) ধ্রুবক
  {
    ফেরত class_মান[(unsigned পূর্ণসংখ্যা) (glyph_id - startGlyph)];
  }

  unsigned get_population () ধ্রুবক
  {
    ফেরত class_মান.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);

    যদি (unlikely (!it))
    {
      classFormat = Types::আকার_2 == 2 ? 1 : 3;
      startGlyph = 0;
      class_মান.len = 0;
      return_trace (সত্য);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_গুনতি = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    যদি (unlikely (!class_মান.serialize (c, glyph_গুনতি))) return_trace (মিথ্যা);
    জন্য (ধ্রুবক hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_মান[idx] = gid_klass_pair.second;
    }
    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               যৌক্তিক keep_empty_table = সত্য,
               যৌক্তিক use_class_zero = সত্য,
               ধ্রুবক Coverage* glyph_filter = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_মান.len;

    জন্য (ধ্রুবক hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      যদি (new_gid == HB_MAP_মান_INVALID) চালু;
      যদি (glyph_filter && !glyph_filter->has(gid)) চালু;

      unsigned klass = class_মান[gid - start];
      যদি (!klass) চালু;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    যদি (use_class_zero)
    {
      ধ্রুবক hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_গুনতি = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_গুনতি <= glyph_and_klass.দৈর্ঘ্য;
    }
    যদি (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (মিথ্যা);
    return_trace (keep_empty_table || (যৌক্তিক) glyph_and_klass);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && class_মান.sanitize (c));
  }

  unsigned cost () ধ্রুবক { ফেরত 1; }

  template <typename set_t>
  যৌক্তিক collect_coverage (set_t *glyphs) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা start = 0;
    unsigned পূর্ণসংখ্যা গুনতি = class_মান.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (class_মান[i])
	চালু;

      যদি (start != i)
	যদি (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  ফেরত মিথ্যা;

      start = i + 1;
    }
    যদি (start != গুনতি)
      যদি (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + গুনতি)))
	ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  template <typename set_t>
  যৌক্তিক collect_class (set_t *glyphs, unsigned klass) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = class_মান.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (class_মান[i] == klass) glyphs->add (startGlyph + i);
    ফেরত সত্য;
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_মান.len;
    জন্য (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      যদি (class_মান[iter - start]) ফেরত সত্য;
    ফেরত মিথ্যা;
  }
  যৌক্তিক intersects_class (ধ্রুবক hb_set_t *glyphs, unsigned klass) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = class_মান.len;
    যদি (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_মান_INVALID;
      যদি (!glyphs->next (&g)) ফেরত মিথ্যা;
      যদি (g < startGlyph) ফেরত সত্য;
      g = startGlyph + গুনতি - 1;
      যদি (glyphs->next (&g)) ফেরত সত্য;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ধ্রুবক typename Types::HBUINT *arr = class_মান.arrayZ;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (arr[i] == klass && glyphs->has (startGlyph + i))
	ফেরত সত্য;
    ফেরত মিথ্যা;
  }

  শূন্য intersected_class_glyphs (ধ্রুবক hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ধ্রুবক
  {
    unsigned গুনতি = class_মান.len;
    যদি (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      জন্য (uint32_t g = HB_SET_মান_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      জন্য (uint32_t g = startGlyph + গুনতি - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      ফেরত;
    }

    জন্য (unsigned i = 0; i < গুনতি; i++)
      যদি (class_মান[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#যদি 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + গুনতি;
    জন্য (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      যদি (class_মান.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  শূন্য intersected_classes (ধ্রুবক hb_set_t *glyphs, hb_set_t *intersect_classes) ধ্রুবক
  {
    যদি (glyphs->is_empty ()) ফেরত;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    যদি (!class_মান.len)
    {
      intersect_classes->add (0);
      ফেরত;
    }

    hb_codepoint_t end_glyph = startGlyph + class_মান.len - 1;
    যদি (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    জন্য (ধ্রুবক auto& _ : + hb_enumerate (class_মান))
    {
      hb_codepoint_t g = startGlyph + _.first;
      যদি (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  সুরক্ষিত:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_মান;	/* Array of Class Values--one per GlyphID */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + 2 * Types::আকার_2, class_মান);
};

template <typename Types>
কাঠামো ClassDefFormat2_4
{
  friend কাঠামো ClassDef;

  ব্যক্তিগত:
  unsigned পূর্ণসংখ্যা get_class (hb_codepoint_t glyph_id) ধ্রুবক
  {
    ফেরত rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () ধ্রুবক
  {
    typename Types::large_int ret = 0;
    জন্য (ধ্রুবক auto &r : rangeRecord)
      ret += r.get_population ();
    ফেরত ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);

    যদি (unlikely (!it))
    {
      classFormat = Types::আকার_2 == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (সত্য);
    }

    unsigned unsorted = মিথ্যা;
    unsigned অঙ্ক_ranges = 1;
    hb_codepoint_t পূর্ব_gid = (*it).first;
    unsigned পূর্ব_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = পূর্ব_gid;
    range_rec.last = পূর্ব_gid;
    range_rec.value = পূর্ব_klass;

    auto *record = c->copy (range_rec);
    যদি (unlikely (!record)) return_trace (মিথ্যা);

    জন্য (ধ্রুবক auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      যদি (cur_gid != পূর্ব_gid + 1 ||
	  cur_klass != পূর্ব_klass)
      {

	যদি (unlikely (cur_gid < পূর্ব_gid))
	  unsorted = সত্য;

	যদি (unlikely (!record)) বিরতি;
	record->last = পূর্ব_gid;
	অঙ্ক_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      পূর্ব_klass = cur_klass;
      পূর্ব_gid = cur_gid;
    }

    যদি (unlikely (c->in_error ())) return_trace (মিথ্যা);

    যদি (likely (record)) record->last = পূর্ব_gid;
    rangeRecord.len = অঙ্ক_ranges;

    যদি (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               যৌক্তিক keep_empty_table = সত্য,
               যৌক্তিক use_class_zero = সত্য,
               ধ্রুবক Coverage* glyph_filter = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    যদি (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      জন্য (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	যদি (!klass) চালু;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	যদি (new_gid == HB_MAP_মান_INVALID) চালু;
	যদি (glyph_filter && !glyph_filter->has (g)) চালু;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    অন্যথা
    {
      unsigned অঙ্ক_source_glyphs = c->plan->source->get_অঙ্ক_glyphs ();
      জন্য (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	যদি (!klass) চালু;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, অঙ্ক_source_glyphs);
	জন্য (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  যদি (new_gid == HB_MAP_মান_INVALID) চালু;
	  যদি (glyph_filter && !glyph_filter->has (g)) চালু;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    ধ্রুবক hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_গুনতি = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_গুনতি <= glyph_and_klass.দৈর্ঘ্য;
    যদি (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (মিথ্যা);
    return_trace (keep_empty_table || (যৌক্তিক) glyph_and_klass);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () ধ্রুবক { ফেরত hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  যৌক্তিক collect_coverage (set_t *glyphs) ধ্রুবক
  {
    জন্য (auto &range : rangeRecord)
      যদি (range.value)
	যদি (unlikely (!range.collect_coverage (glyphs)))
	  ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  template <typename set_t>
  যৌক্তিক collect_class (set_t *glyphs, unsigned পূর্ণসংখ্যা klass) ধ্রুবক
  {
    জন্য (auto &range : rangeRecord)
    {
      যদি (range.value == klass)
	যদি (unlikely (!range.collect_coverage (glyphs)))
	  ফেরত মিথ্যা;
    }
    ফেরত সত্য;
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    যদি (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      জন্য (auto g : *glyphs)
        যদি (get_class (g))
	  ফেরত সত্য;
      ফেরত মিথ্যা;
    }

    ফেরত hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ধ্রুবক RangeRecord<Types> &range) { ফেরত range.intersects (*glyphs) && range.value; }));
  }
  যৌক্তিক intersects_class (ধ্রুবক hb_set_t *glyphs, unsigned klass) ধ্রুবক
  {
    যদি (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      যদি (!rangeRecord.len)
        ফেরত !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_মান_INVALID;
      hb_codepoint_t last = HB_SET_মান_INVALID;
      auto it = hb_iter (rangeRecord);
      জন্য (auto &range : it)
      {
        যদি (it->first == last + 1)
	{
	  it++;
	  চালু;
	}

	যদি (!glyphs->next (&g))
	  বিরতি;
	যদি (g < range.first)
	  ফেরত সত্য;
	g = range.last;
	last = g;
      }
      যদি (g != HB_SET_মান_INVALID && glyphs->next (&g))
	ফেরত সত্য;
      /* Fall through. */
    }
    জন্য (ধ্রুবক auto &range : rangeRecord)
      যদি (range.value == klass && range.intersects (*glyphs))
	ফেরত সত্য;
    ফেরত মিথ্যা;
  }

  শূন্য intersected_class_glyphs (ধ্রুবক hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ধ্রুবক
  {
    যদি (klass == 0)
    {
      hb_codepoint_t g = HB_SET_মান_INVALID;
      জন্য (auto &range : rangeRecord)
      {
	যদি (!glyphs->next (&g))
	  যাও done;
	যখন (g < range.first)
	{
	  intersect_glyphs->add (g);
	  যদি (!glyphs->next (&g))
	    যাও done;
        }
        g = range.last;
      }
      যখন (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      ফেরত;
    }

    unsigned গুনতি = rangeRecord.len;
    যদি (গুনতি > glyphs->get_population () * hb_bit_storage (গুনতি))
    {
      জন্য (auto g : *glyphs)
      {
        unsigned i;
        যদি (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      ফেরত;
    }

    জন্য (auto &range : rangeRecord)
    {
      যদি (range.value != klass) চালু;

      unsigned end = range.last + 1;
      জন্য (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  শূন্য intersected_classes (ধ্রুবক hb_set_t *glyphs, hb_set_t *intersect_classes) ধ্রুবক
  {
    যদি (glyphs->is_empty ()) ফেরত;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    যদি (!rangeRecord.len)
    {
      intersect_classes->add (0);
      ফেরত;
    }

    hb_codepoint_t g = HB_SET_মান_INVALID;
    জন্য (auto &range : rangeRecord)
    {
      যদি (!glyphs->next (&g))
        বিরতি;
      যদি (g < range.first)
      {
        intersect_classes->add (0);
        বিরতি;
      }
      g = range.last;
    }
    যদি (g != HB_SET_মান_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    জন্য (ধ্রুবক auto& range : rangeRecord)
      যদি (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  সুরক্ষিত:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::আকার_2, rangeRecord);
};

কাঠামো ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ধ্রুবক { ফেরত get (k); }
  যৌক্তিক has (hb_codepoint_t k) ধ্রুবক { ফেরত (*এই)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) ধ্রুবক { ফেরত get (k); }

  unsigned পূর্ণসংখ্যা get (hb_codepoint_t k) ধ্রুবক { ফেরত get_class (k); }
  unsigned পূর্ণসংখ্যা get_class (hb_codepoint_t glyph_id) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_class (glyph_id);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_class (glyph_id);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_class (glyph_id);
#endif
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }
  unsigned পূর্ণসংখ্যা get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ধ্রুবক
  {
    unsigned klass;
    যদি (cache && cache->get (glyph_id, &klass)) ফেরত klass;
    klass = get_class (glyph_id);
    যদি (cache) cache->set (glyph_id, klass);
    ফেরত klass;
  }

  unsigned get_population () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_population ();
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_population ();
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_population ();
#endif
    পূর্বনির্ধারিত:ফেরত NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    যদি (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned অঙ্ক_ranges = 1;
      hb_codepoint_t পূর্ব_gid = glyph_min;
      unsigned পূর্ব_klass = (*glyphs).second;

      জন্য (ধ্রুবক auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	যদি (cur_klass > class_max) class_max = cur_klass;
	যদি (cur_gid == glyph_min) চালু;
        যদি (cur_gid > glyph_max) glyph_max = cur_gid;
	যদি (cur_gid != পূর্ব_gid + 1 ||
	    cur_klass != পূর্ব_klass)
	  অঙ্ক_ranges++;

	পূর্ব_gid = cur_gid;
	পূর্ব_klass = cur_klass;
      }

      uint64_t best_আকার = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t আকার_2)
      {
	যদি (আকার_2 >= best_আকার) ফেরত;
	best_আকার = আকার_2;
	format = candidate;
      };

      যদি (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_আকার * glyph_span);
      যদি (glyph_max <= 0xFFFFu && অঙ্ক_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) অঙ্ক_ranges *
		     (2 * HBGlyphID16::static_আকার + HBUINT16::static_আকার));
#ifndef HB_NO_BEYOND_64K
      যদি (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_আকার * glyph_span);
      যদি (glyph_max <= 0xFFFFFFu && অঙ্ক_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) অঙ্ক_ranges *
		     (2 * HBGlyphID24::static_আকার + HBUINT16::static_আকার));
#endif
      যদি (unlikely (best_আকার == UINT64_MAX))
      {
	c->check_success (মিথ্যা, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (মিথ্যা);
      }
    }

    u.format.v = format;

    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               যৌক্তিক keep_empty_table = সত্য,
               যৌক্তিক use_class_zero = সত্য,
               ধ্রুবক Coverage* glyph_filter = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  unsigned cost () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.cost ();
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.cost ();
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.cost ();
#endif
    পূর্বনির্ধারিত:ফেরত 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  যৌক্তিক collect_coverage (set_t *glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.collect_coverage (glyphs);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.collect_coverage (glyphs);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.collect_coverage (glyphs);
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  যৌক্তিক collect_class (set_t *glyphs, unsigned পূর্ণসংখ্যা klass) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.collect_class (glyphs, klass);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.collect_class (glyphs, klass);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.collect_class (glyphs, klass);
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.intersects (glyphs);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.intersects (glyphs);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.intersects (glyphs);
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }
  যৌক্তিক intersects_class (ধ্রুবক hb_set_t *glyphs, unsigned পূর্ণসংখ্যা klass) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.intersects_class (glyphs, klass);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.intersects_class (glyphs, klass);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.intersects_class (glyphs, klass);
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  শূন্য intersected_class_glyphs (ধ্রুবক hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    পূর্বনির্ধারিত:ফেরত;
    }
  }

  শূন্য intersected_classes (ধ্রুবক hb_set_t *glyphs, hb_set_t *intersect_classes) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.intersected_classes (glyphs, intersect_classes);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.intersected_classes (glyphs, intersect_classes);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    পূর্বনির্ধারিত:ফেরত;
    }
  }


  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);
};

template<typename Iterator>
স্থির অন্তঃরেখ যৌক্তিক ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ ফেরত (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
কাঠামো delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  কাঠামো chars_t : hb_vector_t<uint8_t>
  {
    পূর্ণসংখ্যা cmp (ধ্রুবক chars_t& other) ধ্রুবক
    {
      ফেরত as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      জন্য (unsigned i = 0; i < দৈর্ঘ্য; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      ফেরত hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (ধ্রুবক chars_t& other) ধ্রুবক
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      জন্য (unsigned i = 0; i < দৈর্ঘ্য; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      ফেরত hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (ধ্রুবক delta_row_encoding_t& other_encoding) ধ্রুবক { ফেরত chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*> বস্তুসমূহ;

  delta_row_encoding_t () = পূর্বনির্ধারিত;
  delta_row_encoding_t (hb_vector_t<ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*> &&rows, unsigned অঙ্ক_cols)
  {
    প্রতিজ্ঞা (rows);

    বস্তুসমূহ = std::move (rows);

    যদি (unlikely (!chars.resize (অঙ্ক_cols)))
      ফেরত;

    calculate_chars ();
  }

  শূন্য merge (ধ্রুবক delta_row_encoding_t& other)
  {
    বস্তুসমূহ.alloc (বস্তুসমূহ.দৈর্ঘ্য + other.বস্তুসমূহ.দৈর্ঘ্য);
    জন্য (auto &row : other.বস্তুসমূহ)
      add_row (row);

    // Merge chars
    প্রতিজ্ঞা (chars.দৈর্ঘ্য == other.chars.দৈর্ঘ্য);
    জন্য (unsigned i = 0; i < chars.দৈর্ঘ্য; i++)
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

    যৌক্তিক long_শব্দগুলি = মিথ্যা;

    জন্য (auto &row : বস্তুসমূহ)
    {
      প্রতিজ্ঞা (row->দৈর্ঘ্য == chars.দৈর্ঘ্য);

      /* 0/1/2 byte encoding */
      জন্য (unsigned i = 0; i < row->দৈর্ঘ্য; i++)
      {
	পূর্ণসংখ্যা v =  row->arrayZ[i];
	যদি (v == 0)
	  চালু;
	অন্যথা যদি (v > 32767 || v < -32768)
	{
	  long_শব্দগুলি = সত্য;
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
      জন্য (auto &v : chars)
	যদি (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  যৌক্তিক is_empty () ধ্রুবক
  { ফেরত !বস্তুসমূহ; }

  যৌক্তিক in_error () ধ্রুবক
  { ফেরত chars.in_error () || বস্তুসমূহ.in_error (); }

  স্থির অন্তঃরেখ unsigned get_chars_overhead (unsigned অঙ্ক_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    ফেরত c + অঙ্ক_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) ধ্রুবক
  {
    পূর্ণসংখ্যা গুনতি = বস্তুসমূহ.দৈর্ঘ্য;
    ফেরত hb_max (0, (পূর্ণসংখ্যা) overhead - গুনতি * (পূর্ণসংখ্যা) additional_bytes_per_rows);
  }

  পূর্ণসংখ্যা gain_from_merging (ধ্রুবক delta_row_encoding_t& other_encoding) ধ্রুবক
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    যদি (additional_bytes_per_rows > 0)
    {
      যদি (get_gain (additional_bytes_per_rows) == 0)
        ফেরত 0;
    }
    অন্যথা
    {
      যদি (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	ফেরত 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    পূর্ণসংখ্যা combined_gain = (পূর্ণসংখ্যা) overhead + (পূর্ণসংখ্যা) other_encoding.overhead;
    combined_gain -= (combined_width - (পূর্ণসংখ্যা) width) * বস্তুসমূহ.দৈর্ঘ্য;
    combined_gain -= (combined_width - (পূর্ণসংখ্যা) other_encoding.width) * other_encoding.বস্তুসমূহ.দৈর্ঘ্য;
    combined_gain -= get_chars_overhead (combined_columns);

    ফেরত combined_gain;
  }

  যৌক্তিক add_row (ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>* row)
  { ফেরত বস্তুসমূহ.push (row); }

  স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    ধ্রুবক delta_row_encoding_t *a = (ধ্রুবক delta_row_encoding_t *)pa;
    ধ্রুবক delta_row_encoding_t *b = (ধ্রুবক delta_row_encoding_t *)pb;

    যদি (a->width != b->width)
      ফেরত (পূর্ণসংখ্যা) a->width - (পূর্ণসংখ্যা) b->width;

    ফেরত b->chars.cmp (a->chars);
  }
};

কাঠামো VarRegionAxis
{
  দশমিক evaluate (পূর্ণসংখ্যা coord) ধ্রুবক
  {
    পূর্ণসংখ্যা peak = peakCoord.to_int ();
    যদি (peak == 0 || coord == peak)
      ফেরত 1.f;
    অন্যথা যদি (coord == 0) // Faster
      ফেরত 0.f;

    পূর্ণসংখ্যা start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    যদি (unlikely (start > peak || peak > end))
      ফেরত 1.f;
    যদি (unlikely (start < 0 && end > 0))
      ফেরত 1.f;

    যদি (coord <= start || end <= coord)
      ফেরত 0.f;

    /* Interpolate */
    যদি (coord < peak)
      ফেরত দশমিক (coord - start) / (peak - start);
    অন্যথা
      ফেরত দশমিক (end - coord) / (end - peak);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  যৌক্তিক serialize (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed (এই));
  }

  সর্বজনীন:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};
কাঠামো SparseVarRegionAxis
{
  দশমিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len) ধ্রুবক
  {
    unsigned i = axis_সূচক_2;
    পূর্ণসংখ্যা coord = i < coord_len ? coords[i] : 0;
    ফেরত axis.evaluate (coord);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক SparseVarRegionAxis *src,
		  ধ্রুবক hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->embed (src);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    যদি (axis_map)
    {
      যদি (unlikely (!axis_map->has (out->axis_সূচক_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (মিথ্যা);
      }
      যদি (unlikely (!c->check_assign (out->axis_সূচক_2, axis_map->get (out->axis_সূচক_2),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (মিথ্যা);
    }
    return_trace (সত্য);
  }

  সর্বজনীন:
  HBUINT16 axis_সূচক_2;
  VarRegionAxis axis;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো hb_scalar_cache_t
{
  ব্যক্তিগত:
  স্থির constexpr unsigned STATIC_দৈর্ঘ্য = 128;
  স্থির constexpr দশমিক INVALID = NAN;

  সর্বজনীন:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ধ্রুবক hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (ধ্রুবক hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  স্থির hb_scalar_cache_t *create (unsigned পূর্ণসংখ্যা গুনতি,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    যদি (!গুনতি) ফেরত (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    যদি (scratch_cache && গুনতি <= STATIC_দৈর্ঘ্য)
    {
      scratch_cache->দৈর্ঘ্য = গুনতি;
      scratch_cache->clear ();
      ফেরত scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (আকার (hb_scalar_cache_t) - আকার (static_মানসমূহ) + আকার (static_মানসমূহ[0]) * গুনতি);
    যদি (unlikely (!cache)) ফেরত (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->দৈর্ঘ্য = গুনতি;
    cache->clear ();

    ফেরত cache;
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
#ifndef HB_OPTIMIZE_আকার
    জন্য (; i + 3 < দৈর্ঘ্য; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    জন্য (; i < দৈর্ঘ্য; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  যৌক্তিক get (unsigned i, দশমিক *value) ধ্রুবক
  {
    যদি (unlikely (i >= দৈর্ঘ্য))
    {
      *value = 0.f;
      ফেরত সত্য;
    }
    auto *values = &static_মানসমূহ[0];
    auto *cached_মান = &values[i];
    দশমিক v = *cached_মান;
    যদি (std::isnan (v))
      ফেরত মিথ্যা;
    *value = v;
    ফেরত সত্য;
  }

  HB_ALWAYS_INLINE
  শূন্য set (unsigned i, দশমিক value)
  {
    যদি (unlikely (i >= দৈর্ঘ্য)) ফেরত;
    auto *values = &static_মানসমূহ[0];
    auto *cached_মান = &values[i];
    *cached_মান = value;
  }

  সর্বজনীন:
  unsigned দৈর্ঘ্য;
  ব্যক্তিগত:
  mutable hb_atomic_t<দশমিক> static_মানসমূহ[STATIC_দৈর্ঘ্য];
};

কাঠামো VarRegionList
{
  ব্যক্তিগত:
  দশমিক evaluate_impl (unsigned পূর্ণসংখ্যা region_সূচক,
		       ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len) ধ্রুবক
  {
    ধ্রুবক VarRegionAxis *axes = axesZ.arrayZ + (region_সূচক * axis_গুনতি_2);
    দশমিক v = 1.f;

    unsigned পূর্ণসংখ্যা গুনতি = axis_গুনতি_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      পূর্ণসংখ্যা coord = i < coord_len ? coords[i] : 0;
      দশমিক factor = axes[i].evaluate (coord);
      যদি (factor == 0.f)
      {
	v = 0.f;
	বিরতি;
      }
      v *= factor;
    }

    ফেরত v;
  }

  সর্বজনীন:
  HB_ALWAYS_INLINE
  দশমিক evaluate (unsigned পূর্ণসংখ্যা region_সূচক,
		  ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    যদি (unlikely (region_সূচক >= region_গুনতি))
      ফেরত 0.;

    দশমিক v;
    যদি (cache && cache->get (region_সূচক, &v))
      ফেরত v;

    v = evaluate_impl (region_সূচক, coords, coord_len);

    যদি (cache)
      cache->set (region_সূচক, v);
    ফেরত v;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_গুনতি_2 * region_গুনতি));
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
                  ধ্রুবক hb_vector_t<hb_tag_t>& axis_tags,
                  ধ্রুবক hb_vector_t<ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (এই);
    unsigned axis_গুনতি = axis_tags.দৈর্ঘ্য;
    unsigned region_গুনতি_2 = regions.দৈর্ঘ্য;
    যদি (!axis_গুনতি || !region_গুনতি_2) return_trace (মিথ্যা);
    যদি (unlikely (hb_unsigned_mul_overflows (axis_গুনতি * region_গুনতি_2,
                                             VarRegionAxis::static_আকার))) return_trace (মিথ্যা);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    axis_গুনতি_2 = axis_গুনতি;
    region_গুনতি = region_গুনতি_2;

    জন্য (unsigned r = 0; r < region_গুনতি_2; r++)
    {
      ধ্রুবক auto& region = regions[r];
      জন্য (unsigned i = 0; i < axis_গুনতি; i++)
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
          return_trace (মিথ্যা);
      }
    }
    return_trace (সত্য);
  }

  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক VarRegionList *src, ধ্রুবক hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    axis_গুনতি_2 = src->axis_গুনতি_2;
    region_গুনতি = region_map.get_population ();
    যদি (unlikely (hb_unsigned_mul_overflows (axis_গুনতি_2 * region_গুনতি,
					     VarRegionAxis::static_আকার))) return_trace (মিথ্যা);
    যদি (unlikely (!c->extend (এই))) return_trace (মিথ্যা);
    unsigned পূর্ণসংখ্যা region_গুনতি_2 = src->region_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা r = 0; r < region_গুনতি; r++)
    {
      unsigned পূর্ণসংখ্যা backward = region_map.backward (r);
      যদি (backward >= region_গুনতি_2) return_trace (মিথ্যা);
      hb_memcpy (&axesZ[axis_গুনতি_2 * r], &src->axesZ[axis_গুনতি_2 * backward], VarRegionAxis::static_আকার * axis_গুনতি_2);
    }

    return_trace (সত্য);
  }

  যৌক্তিক get_var_region (unsigned region_সূচক,
                       ধ্রুবক hb_map_t& axes_old_সূচক_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ধ্রুবক
  {
    যদি (region_সূচক >= region_গুনতি) ফেরত মিথ্যা;
    ধ্রুবক VarRegionAxis* axis_region = axesZ.arrayZ + (region_সূচক * axis_গুনতি_2);
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      hb_tag_t *axis_tag;
      যদি (!axes_old_সূচক_tag_map.has (i, &axis_tag))
        ফেরত মিথ্যা;

      দশমিক min_val = axis_region->startCoord.to_float ();
      দশমিক def_val = axis_region->peakCoord.to_float ();
      দশমিক max_val = axis_region->endCoord.to_float ();

      যদি (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((দ্বিগুণ) min_val, (দ্বিগুণ) def_val, (দ্বিগুণ) max_val));
      axis_region++;
    }
    ফেরত !axis_tuples.in_error ();
  }

  যৌক্তিক get_var_regions (ধ্রুবক hb_map_t& axes_old_সূচক_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) ধ্রুবক
  {
    যদি (!regions.alloc (region_গুনতি))
      ফেরত মিথ্যা;

    জন্য (unsigned i = 0; i < region_গুনতি; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      যদি (!get_var_region (i, axes_old_সূচক_tag_map, axis_tuples))
        ফেরত মিথ্যা;
      regions.push (std::move (axis_tuples));
    }
    ফেরত !regions.in_error ();
  }

  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_add_saturate (min_আকার,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_আকার,
							       axis_গুনতি_2, region_গুনতি)); }

  সর্বজনীন:
  HBUINT16	axis_গুনতি_2;
  HBUINT15	region_গুনতি;
  সুরক্ষিত:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
স্থির অন্তঃরেখ যৌক্তিক
_hb_avar2_region_is_dead (ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  জন্য (auto _ : axis_tuples)
  {
    Triple *range;
    যদি (!reachable_ranges.has (_.first, &range)) চালু;
    ধ্রুবক Triple &tent = _.second;
    পূর্ণসংখ্যা start = (পূর্ণসংখ্যা) roundf ((দশমিক) tent.minimum * 16384.f);
    পূর্ণসংখ্যা peak  = (পূর্ণসংখ্যা) roundf ((দশমিক) tent.মধ্য * 16384.f);
    পূর্ণসংখ্যা end   = (পূর্ণসংখ্যা) roundf ((দশমিক) tent.maximum * 16384.f);
    যদি (!peak) চালু;
    যদি (start > peak || peak > end) চালু;
    যদি (start < 0 && end > 0) চালু;
    পূর্ণসংখ্যা lo = (পূর্ণসংখ্যা) roundf ((দশমিক) range->minimum * 16384.f);
    পূর্ণসংখ্যা hi = (পূর্ণসংখ্যা) roundf ((দশমিক) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    যদি (hi < start || lo > end)
      ফেরত সত্য;
  }
  ফেরত মিথ্যা;
}

কাঠামো SparseVariationRegion
{
  দশমিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len) ধ্রুবক
  {
    দশমিক v = 1.f;
    unsigned পূর্ণসংখ্যা গুনতি = axes.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      দশমিক factor = (এই+axes[i]).evaluate (coords, coord_len);
      যদি (factor == 0.f)
	ফেরত 0.;
      v *= factor;
    }
    ফেরত v;
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক SparseVariationRegion *src,
		  ধ্রুবক hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (মিথ্যা);
    জন্য (unsigned i = 0; i < axes.len; i++)
      যদি (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (axes.sanitize (c, এই));
  }

  সুরক্ষিত:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, axes);
};

কাঠামো SparseVarRegionList
{
  HB_ALWAYS_INLINE
  দশমিক evaluate (unsigned পূর্ণসংখ্যা region_সূচক,
		  ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    যদি (unlikely (region_সূচক >= regions.len))
      ফেরত 0.;

    দশমিক v;
    যদি (cache && cache->get (region_সূচক, &v))
      ফেরত v;

    ধ্রুবক SparseVariationRegion &region = এই+regions[region_সূচক];

    v = region.evaluate (coords, coord_len);
    যদি (cache)
      cache->set (region_সূচক, v);

    ফেরত v;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (regions.sanitize (c, এই));
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক SparseVarRegionList *src,
		  ধ্রুবক hb_inc_bimap_t &region_map,
		  ধ্রুবক hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    regions.len = region_map.get_population ();
    যদি (unlikely (!c->extend (regions))) return_trace (মিথ্যা);

    জন্য (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_সূচক = region_map.backward (i);
      যদি (unlikely (old_সূচক >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_সূচক]),
					     axis_map)))
	return_trace (মিথ্যা);
    }
    return_trace (সত্য);
  }

  সর্বজনীন:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, regions);
};


কাঠামো Var_তথ্য
{
  unsigned পূর্ণসংখ্যা get_item_গুনতি () ধ্রুবক
  { ফেরত item_গুনতি; }

  unsigned পূর্ণসংখ্যা get_region_সূচক_গুনতি () ধ্রুবক
  { ফেরত regionIndices.len; }

  unsigned get_region_সূচক (unsigned i) ধ্রুবক
  { ফেরত i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_আকার () ধ্রুবক
  { ফেরত (word_গুনতি () + regionIndices.len) * (long_শব্দগুলি_2 () ? 2 : 1); }

  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_add_saturate (min_আকার - regionIndices.min_আকার,
				     regionIndices.get_আকার (),
				     hb_unsigned_mul_saturate (item_গুনতি, get_row_আকার ())); }

  template <typename Float = দশমিক>
  Float _get_delta (unsigned পূর্ণসংখ্যা inner,
		    ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
		    ধ্রুবক VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    যদি (unlikely (inner >= item_গুনতি))
      ফেরত 0.;
    যৌক্তিক is_long = long_শব্দগুলি_2 ();
    unsigned পূর্ণসংখ্যা গুনতি = regionIndices.len;
    unsigned word_গুনতি_2 = word_গুনতি ();
    unsigned পূর্ণসংখ্যা scount = is_long ? গুনতি : word_গুনতি_2;
    unsigned পূর্ণসংখ্যা lcount = is_long ? word_গুনতি_2 : 0;

    ধ্রুবক HBUINT8 *bytes = get_delta_bytes ();
    ধ্রুবক HBUINT8 *row = bytes + inner * get_row_আকার ();

    Float delta = 0;
    unsigned পূর্ণসংখ্যা i = 0;

    ধ্রুবক HBINT32 *lcursor = reinterpret_cast<ধ্রুবক HBINT32 *> (row);
    জন্য (; i < lcount; i++)
    {
      দশমিক scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_গুনতি, cache);
      যদি (scalar)
        delta += (Float) scalar * (পূর্ণসংখ্যা) *lcursor;
      lcursor++;
    }
    ধ্রুবক HBINT16 *scursor = reinterpret_cast<ধ্রুবক HBINT16 *> (lcursor);
    জন্য (; i < scount; i++)
    {
      দশমিক scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_গুনতি, cache);
      যদি (scalar)
       delta += (Float) scalar * (পূর্ণসংখ্যা) *scursor;
      scursor++;
    }
    ধ্রুবক HBINT8 *bcursor = reinterpret_cast<ধ্রুবক HBINT8 *> (scursor);
    জন্য (; i < গুনতি; i++)
    {
      দশমিক scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_গুনতি, cache);
      যদি (scalar)
        delta += (Float) scalar * (পূর্ণসংখ্যা) *bcursor;
      bcursor++;
    }

    ফেরত delta;
  }

  template <typename Float = দশমিক>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned পূর্ণসংখ্যা inner,
		   ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
		   ধ্রুবক VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = regionIndices.len;
    যদি (!গুনতি) ফেরত Float (0); // This is quite common, so optimize it.
    ফেরত _get_delta<Float> (inner, coords, coord_গুনতি, regions, cache);
  }

  শূন্য get_region_scalars (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
			   ধ্রুবক VarRegionList &regions,
			   দশমিক *scalars /*OUT */,
			   unsigned পূর্ণসংখ্যা অঙ্ক_scalars) ধ্রুবক
  {
    unsigned গুনতি = hb_min (অঙ্ক_scalars, regionIndices.len);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_গুনতি);
    জন্য (unsigned পূর্ণসংখ্যা i = গুনতি; i < অঙ্ক_scalars; i++)
      scalars[i] = 0.f;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_গুনতি () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_গুনতি,
				  get_row_আকার ()));
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
                  যৌক্তিক has_long,
                  ধ্রুবক hb_vector_t<ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>*>& rows)
  {
    TRACE_SERIALIZE (এই);
    unsigned row_গুনতি_2 = rows.দৈর্ঘ্য;
    যদি (!row_গুনতি_2) {
      // Nothing to serialize, will be empty.
      ফেরত মিথ্যা;
    }

    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    যদি (unlikely (!c->check_assign (item_গুনতি, row_গুনতি_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (মিথ্যা);

    পূর্ণসংখ্যা min_threshold = has_long ? -32768 : -128;
    পূর্ণসংখ্যা max_threshold = has_long ? +32767 : +127;
    গণনা delta_আকার_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_আকার_t> delta_sz;
    unsigned অঙ্ক_regions = rows[0]->দৈর্ঘ্য;
    যদি (!delta_sz.resize (অঙ্ক_regions))
      return_trace (মিথ্যা);

    unsigned word_গুনতি_2 = 0;
    জন্য (unsigned r = 0; r < অঙ্ক_regions; r++)
    {
      জন্য (unsigned i = 0; i < row_গুনতি_2; i++)
      {
        পূর্ণসংখ্যা delta = rows[i]->arrayZ[r];
        যদি (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_গুনতি_2++;
          বিরতি;
        }
        অন্যথা যদি (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_সূচক = 0;
    unsigned non_word_সূচক = word_গুনতি_2;
    hb_map_t ri_map;
    জন্য (unsigned r = 0; r < অঙ্ক_regions; r++)
    {
      যদি (!delta_sz[r]) চালু;
      unsigned new_r = (delta_sz[r] == kWord)? word_সূচক++ : non_word_সূচক++;
      যদি (!ri_map.set (new_r, r))
        return_trace (মিথ্যা);
    }

    word_আকার_গুনতি = word_গুনতি_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_গুনতি = ri_map.get_population ();
    regionIndices.len = ri_গুনতি;
    যদি (unlikely (!c->extend (এই))) return_trace (মিথ্যা);

    জন্য (unsigned r = 0; r < ri_গুনতি; r++)
    {
      hb_codepoint_t *idx;
      যদি (!ri_map.has (r, &idx))
        return_trace (মিথ্যা);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_আকার = get_row_আকার ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < row_গুনতি_2; i++)
    {
      জন্য (unsigned পূর্ণসংখ্যা r = 0; r < ri_গুনতি; r++)
      {
        পূর্ণসংখ্যা delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_আকার);
      }
    }
    return_trace (সত্য);
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক Var_তথ্য *src,
		  ধ্রুবক hb_inc_bimap_t &inner_map,
		  ধ্রুবক hb_inc_bimap_t &region_map,
		  ধ্রুবক hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    item_গুনতি = inner_map.get_পরবর্তী_মান ();

    /* Optimize word count */
    unsigned ri_গুনতি = src->regionIndices.len;
    গণনা delta_আকার_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_আকার_t> delta_sz;
    hb_vector_t<unsigned পূর্ণসংখ্যা> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_গুনতি);
    ri_map.resize (ri_গুনতি);
    unsigned পূর্ণসংখ্যা new_word_গুনতি = 0;
    unsigned পূর্ণসংখ্যা r;

    ধ্রুবক HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_আকার = src->get_row_আকার ();
    unsigned src_word_গুনতি = src->word_গুনতি ();
    যৌক্তিক     src_long_শব্দগুলি = src->long_শব্দগুলি_2 ();

    যৌক্তিক has_long = মিথ্যা;
    যদি (src_long_শব্দগুলি)
    {
      জন্য (r = 0; r < src_word_গুনতি; r++)
      {
        জন্য (unsigned old_gid : inner_map.চাবিগুলি())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_আকার);
	  যদি (delta < -32768 || 32767 < delta)
	  {
	    has_long = সত্য;
	    বিরতি;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    জন্য (r = 0; r < ri_গুনতি; r++)
    {
      যৌক্তিক short_circuit = src_long_শব্দগুলি == has_long && src_word_গুনতি <= r;

      delta_sz[r] = kZero;
      যদি (culled_regions && culled_regions->has (src->regionIndices[r]))
	চালু; /* unreachable region: drop its column */
      জন্য (unsigned old_gid : inner_map.চাবিগুলি())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_আকার);
	যদি (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_গুনতি++;
	  বিরতি;
	}
	অন্যথা যদি (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  যদি (short_circuit)
	    বিরতি;
	}
      }
    }

    unsigned পূর্ণসংখ্যা word_সূচক = 0;
    unsigned পূর্ণসংখ্যা non_word_সূচক = new_word_গুনতি;
    unsigned পূর্ণসংখ্যা new_ri_গুনতি = 0;
    জন্য (r = 0; r < ri_গুনতি; r++)
      যদি (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_সূচক++ : non_word_সূচক++;
	ri_map[new_r] = r;
	new_ri_গুনতি++;
      }

    word_আকার_গুনতি = new_word_গুনতি | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_গুনতি;

    যদি (unlikely (!c->extend (এই))) return_trace (মিথ্যা);

    জন্য (r = 0; r < new_ri_গুনতি; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_আকার = get_row_আকার ();
    unsigned গুনতি = item_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      unsigned পূর্ণসংখ্যা old = inner_map.backward (i);
      জন্য (unsigned পূর্ণসংখ্যা r = 0; r < new_ri_গুনতি; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_আকার),
			     delta_bytes, row_আকার);
    }

    return_trace (সত্য);
  }

  শূন্য collect_region_refs (hb_set_t &region_indices, ধ্রুবক hb_inc_bimap_t &inner_map) ধ্রুবক
  {
    ধ্রুবক HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_আকার = get_row_আকার ();

    জন্য (unsigned পূর্ণসংখ্যা r = 0; r < regionIndices.len; r++)
    {
      unsigned পূর্ণসংখ্যা region = regionIndices.arrayZ[r];
      যদি (region_indices.has (region)) চালু;
      জন্য (hb_codepoint_t old_gid : inner_map.চাবিগুলি())
	যদি (get_item_delta_fast (old_gid, r, delta_bytes, row_আকার) != 0)
	{
	  region_indices.add (region);
	  বিরতি;
	}
    }
  }

  সর্বজনীন:
  ধ্রুবক HBUINT8 *get_delta_bytes () ধ্রুবক
  { ফেরত &StructAfter<HBUINT8> (regionIndices); }

  সুরক্ষিত:
  HBUINT8 *get_delta_bytes ()
  { ফেরত &StructAfter<HBUINT8> (regionIndices); }

  সর্বজনীন:
  int32_t get_item_delta_fast (unsigned পূর্ণসংখ্যা item, unsigned পূর্ণসংখ্যা region,
			       ধ্রুবক HBUINT8 *delta_bytes, unsigned row_আকার) ধ্রুবক
  {
    যদি (unlikely (item >= item_গুনতি || region >= regionIndices.len)) ফেরত 0;

    ধ্রুবক HBINT8 *p = (ধ্রুবক HBINT8 *) delta_bytes + item * row_আকার;
    unsigned word_গুনতি_2 = word_গুনতি ();
    যৌক্তিক is_long = long_শব্দগুলি_2 ();
    যদি (is_long)
    {
      যদি (region < word_গুনতি_2)
	ফেরত ((ধ্রুবক HBINT32 *) p)[region];
      অন্যথা
	ফেরত ((ধ্রুবক HBINT16 *)(p + HBINT32::static_আকার * word_গুনতি_2))[region - word_গুনতি_2];
    }
    অন্যথা
    {
      যদি (region < word_গুনতি_2)
	ফেরত ((ধ্রুবক HBINT16 *) p)[region];
      অন্যথা
	ফেরত (p + HBINT16::static_আকার * word_গুনতি_2)[region - word_গুনতি_2];
    }
  }
  int32_t get_item_delta (unsigned পূর্ণসংখ্যা item, unsigned পূর্ণসংখ্যা region) ধ্রুবক
  {
     ফেরত get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_আকার ());
  }

  সুরক্ষিত:
  শূন্য set_item_delta_fast (unsigned পূর্ণসংখ্যা item, unsigned পূর্ণসংখ্যা region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_আকার)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_আকার;
    unsigned word_গুনতি_2 = word_গুনতি ();
    যৌক্তিক is_long = long_শব্দগুলি_2 ();
    যদি (is_long)
    {
      যদি (region < word_গুনতি_2)
	((HBINT32 *) p)[region] = delta;
      অন্যথা
	((HBINT16 *)(p + HBINT32::static_আকার * word_গুনতি_2))[region - word_গুনতি_2] = delta;
    }
    অন্যথা
    {
      যদি (region < word_গুনতি_2)
	((HBINT16 *) p)[region] = delta;
      অন্যথা
	(p + HBINT16::static_আকার * word_গুনতি_2)[region - word_গুনতি_2] = delta;
    }
  }
  শূন্য set_item_delta (unsigned পূর্ণসংখ্যা item, unsigned পূর্ণসংখ্যা region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_আকার ());
  }

  যৌক্তিক long_শব্দগুলি_2 () ধ্রুবক { ফেরত word_আকার_গুনতি & 0x8000u /* LONG_WORDS */; }
  unsigned word_গুনতি () ধ্রুবক { ফেরত word_আকার_গুনতি & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  সুরক্ষিত:
  HBUINT16		item_গুনতি;
  HBUINT16		word_আকার_গুনতি;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, regionIndices);
};

কাঠামো Multi_Var_তথ্য
{
  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_add_saturate (min_আকার - regionIndices.min_আকার,
				     regionIndices.get_আকার ()); }

  ধ্রুবক TupleList &get_delta_sets () ধ্রুবক
  { ফেরত এই+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  শূন্য get_delta (unsigned পূর্ণসংখ্যা inner,
		  ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
		  ধ্রুবক SparseVarRegionList &regions,
		  hb_array_t<দ্বিগুণ> out,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    auto &deltaSets = get_delta_sets ();

    auto মানসমূহ_iter = deltaSets.fetcher (inner);
    unsigned region_গুনতি = regionIndices.len;
    unsigned skip = 0;
    জন্য (unsigned region_সূচক_2 = 0; region_সূচক_2 < region_গুনতি; region_সূচক_2++)
    {
      দশমিক scalar = regions.evaluate (regionIndices.arrayZ[region_সূচক_2],
				       coords, coord_গুনতি,
				       cache);
      // We skip lazily. Helps with the tail end.
      যদি (scalar == 0.0f)
        skip += out.দৈর্ঘ্য;
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

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, এই));
  }

  unsigned get_item_গুনতি () ধ্রুবক
  { ফেরত get_delta_sets ().গুনতি; }

  শূন্য collect_region_refs (hb_set_t &region_indices) ধ্রুবক
  {
    জন্য (unsigned region : regionIndices)
      region_indices.add (region);
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক Multi_Var_তথ্য *src,
		  ধ্রুবক hb_inc_bimap_t &inner_map,
		  ধ্রুবক hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (এই);
    unsigned region_গুনতি_2 = src->regionIndices.len;
    unsigned header_আকার = HBUINT8::static_আকার +
			   Array16Of<HBUINT16>::min_আকার +
			   region_গুনতি_2 * HBUINT16::static_আকার +
			   HBUINT32::static_আকার;
    যদি (unlikely (!c->extend_আকার (এই, header_আকার, মিথ্যা)))
      return_trace (মিথ্যা);
    format = 1;
    regionIndices.len = region_গুনতি_2;
    জন্য (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      যদি (unlikely (!region_map.has (old_region)))
	return_trace (মিথ্যা);
      regionIndices[i] = region_map.get (old_region);
    }

    ধ্রুবক auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned তথ্য_আকার = 0;
    যদি (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (মিথ্যা);
    জন্য (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      যদি (unlikely (old_inner >= src_delta_sets.গুনতি)) return_trace (মিথ্যা);
      hb_ubytes_t bytes = static_cast<ধ্রুবক CFF2_সূচক &> (src_delta_sets)[old_inner];
      তথ্য_আকার = hb_unsigned_add_saturate (তথ্য_আকার, bytes.দৈর্ঘ্য);
      delta_sets.push (bytes);
    }
    যদি (unlikely (তথ্য_আকার == UINT_MAX || delta_sets.in_error ()))
      return_trace (মিথ্যা);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &তথ্য_আকার));
  }

  সুরক্ষিত:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  সর্বজনীন:
  DEFINE_আকার_MIN (7);
};

কাঠামো ItemVariationStore
{
  friend কাঠামো item_variations_t;

  hb_scalar_cache_t *create_cache () ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত hb_scalar_cache_t::create (0);
#endif
    ফেরত hb_scalar_cache_t::create ((এই+regions).region_গুনতি);
  }

  স্থির শূন্য destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  ব্যক্তিগত:
  template <typename Float = দশমিক>
  Float get_delta (unsigned পূর্ণসংখ্যা outer, unsigned পূর্ণসংখ্যা inner,
		   ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
		   hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত Float (0);
#endif

    যদি (unlikely (outer >= তথ্য_Sets.len))
      ফেরত Float (0);

    ফেরত (এই+তথ্য_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_গুনতি,
					     এই+regions,
					     cache);
  }

  সর্বজনীন:
  যৌক্তিক has_delta_set (unsigned পূর্ণসংখ্যা সূচক_2) ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত মিথ্যা;
#endif

    unsigned পূর্ণসংখ্যা outer = সূচক_2 >> 16;
    unsigned পূর্ণসংখ্যা inner = সূচক_2 & 0xFFFF;
    ফেরত outer < তথ্য_Sets.len &&
	   inner < (এই+তথ্য_Sets[outer]).get_item_গুনতি ();
  }

  template <typename Float = দশমিক>
  Float get_delta (unsigned পূর্ণসংখ্যা সূচক_2,
		   ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
		   hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা outer = সূচক_2 >> 16;
    unsigned পূর্ণসংখ্যা inner = সূচক_2 & 0xFFFF;
    ফেরত get_delta<Float> (outer, inner, coords, coord_গুনতি, cache);
  }
  template <typename Float = দশমিক>
  Float get_delta (unsigned পূর্ণসংখ্যা সূচক_2,
		   hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
		   hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    ফেরত get_delta<Float> (সূচক_2,
		      coords.arrayZ, coords.দৈর্ঘ্য,
		      cache);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত সত্য;
#endif

    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, এই) &&
		  তথ্য_Sets.sanitize (c, এই));
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
                  যৌক্তিক has_long,
                  ধ্রুবক hb_vector_t<hb_tag_t>& axis_tags,
                  ধ্রুবক hb_vector_t<ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  ধ্রুবক hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (এই);
#ifdef HB_NO_VAR
    return_trace (মিথ্যা);
#endif
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);

    format = 1;
    যদি (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (মিথ্যা);

    unsigned অঙ্ক_var_তথ্য = vardata_encodings.দৈর্ঘ্য;
    যদি (!অঙ্ক_var_তথ্য) return_trace (মিথ্যা);
    যদি (unlikely (!c->check_assign (তথ্য_Sets.len, অঙ্ক_var_তথ্য,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (মিথ্যা);

    যদি (unlikely (!c->extend (তথ্য_Sets))) return_trace (মিথ্যা);
    জন্য (unsigned i = 0; i < অঙ্ক_var_তথ্য; i++)
      যদি (!তথ্য_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].বস্তুসমূহ))
        return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক ItemVariationStore *src,
		  ধ্রুবক hb_array_t <ধ্রুবক hb_inc_bimap_t> &inner_maps,
		  ধ্রুবক hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (এই);
#ifdef HB_NO_VAR
    return_trace (মিথ্যা);
#endif

    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);

    unsigned পূর্ণসংখ্যা set_গুনতি = 0;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < inner_maps.দৈর্ঘ্য; i++)
      যদি (inner_maps[i].get_population ())
	set_গুনতি++;

    format = 1;

    ধ্রুবক auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < inner_maps.দৈর্ঘ্য; i++)
      (src+src->তথ্য_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    যদি (region_indices.in_error ())
      return_trace (মিথ্যা);

    region_indices.del_range ((src_regions).region_গুনতি, hb_set_t::INVALID);

    যদি (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    যদি (region_map.in_error())
      return_trace (মিথ্যা);

    যদি (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (মিথ্যা);

    তথ্য_Sets.len = set_গুনতি;
    যদি (unlikely (!c->extend (তথ্য_Sets))) return_trace (মিথ্যা);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned পূর্ণসংখ্যা set_সূচক = 0;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < inner_maps.দৈর্ঘ্য; i++)
    {
      যদি (!inner_maps[i].get_population ()) চালু;
      যদি (unlikely (!তথ্য_Sets[set_সূচক++]
		     .serialize_serialize (c, &(src+src->তথ্য_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->start_embed (এই);
    যদি (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned গুনতি = তথ্য_Sets.len;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      যদি (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &তথ্য = এই+তথ্য_Sets[i];

      unsigned item_গুনতি = তথ্য.get_item_গুনতি ();
      জন্য (unsigned j = 0; j < item_গুনতি; j++)
	map->add (j);
    }

    যদি (unlikely (!out->serialize (c, এই, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  যৌক্তিক collect_dead_regions (ধ্রুবক hb_map_t &axes_old_সূচক_tag_map,
			     ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) ধ্রুবক
  {
#ifndef HB_NO_VAR
    যদি (!reachable_ranges.get_population ()) ফেরত সত্য;
    ধ্রুবক VarRegionList &region_list = এই+regions;
    unsigned গুনতি = region_list.region_গুনতি;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      যদি (!region_list.get_var_region (i, axes_old_সূচক_tag_map, axis_tuples))
	ফেরত মিথ্যা;
      যদি (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    ফেরত !dead_regions.in_error ();
#অন্যথা
    ফেরত সত্য;
#endif
  }

  যৌক্তিক subset (hb_subset_context_t *c, ধ্রুবক hb_array_t<ধ্রুবক hb_inc_bimap_t> &inner_maps) ধ্রুবক
  {
    TRACE_SUBSET (এই);
#ifdef HB_NO_VAR
    return_trace (মিথ্যা);
#endif

    ItemVariationStore *varstore_মৌলিক = c->serializer->start_embed<ItemVariationStore> ();
    যদি (unlikely (!varstore_মৌলিক)) return_trace (মিথ্যা);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    যদি (c->plan->has_avar2)
      যদি (!collect_dead_regions (c->plan->axes_old_সূচক_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (মিথ্যা);

    varstore_মৌলিক->serialize (c->serializer, এই, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_মৌলিক->তথ্য_Sets);
  }

  unsigned পূর্ণসংখ্যা get_region_সূচক_গুনতি (unsigned পূর্ণসংখ্যা major) ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত 0;
#endif
    ফেরত (এই+তথ্য_Sets[major]).get_region_সূচক_গুনতি ();
  }

  শূন্য get_region_scalars (unsigned পূর্ণসংখ্যা major,
			   ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
			   দশমিক *scalars /*OUT*/,
			   unsigned পূর্ণসংখ্যা অঙ্ক_scalars) ধ্রুবক
  {
#ifdef HB_NO_VAR
    জন্য (unsigned i = 0; i < অঙ্ক_scalars; i++)
      scalars[i] = 0.f;
    ফেরত;
#endif

    (এই+তথ্য_Sets[major]).get_region_scalars (coords, coord_গুনতি,
					       এই+regions,
					       &scalars[0], অঙ্ক_scalars);
  }

  unsigned পূর্ণসংখ্যা get_sub_table_গুনতি () ধ্রুবক
   {
#ifdef HB_NO_VAR
     ফেরত 0;
#endif
     ফেরত তথ্য_Sets.len;
   }

  ধ্রুবক Var_তথ্য& get_sub_table (unsigned i) ধ্রুবক
  {
#ifdef HB_NO_VAR
     ফেরত Null (Var_তথ্য);
#endif
     ফেরত এই+তথ্য_Sets[i];
  }

  ধ্রুবক VarRegionList& get_region_list () ধ্রুবক
  {
#ifdef HB_NO_VAR
     ফেরত Null (VarRegionList);
#endif
     ফেরত এই+regions;
  }

  সুরক্ষিত:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<Var_তথ্য>		তথ্য_Sets;
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (8, তথ্য_Sets);
};

কাঠামো MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত hb_scalar_cache_t::create (0);
#endif
    auto &r = এই+regions;
    unsigned গুনতি = r.regions.len;

    ফেরত hb_scalar_cache_t::create (গুনতি, static_cache);
  }

  স্থির শূন্য destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  ব্যক্তিগত:
  শূন্য get_delta (unsigned পূর্ণসংখ্যা outer, unsigned পূর্ণসংখ্যা inner,
		  ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
		  hb_array_t<দ্বিগুণ> out,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত;
#endif

    যদি (unlikely (outer >= তথ্য_Sets.len))
      ফেরত;

    ফেরত (এই+তথ্য_Sets[outer]).get_delta (inner,
					     coords, coord_গুনতি,
					     এই+regions,
					     out,
					     cache);
  }

  সর্বজনীন:
  শূন্য get_delta (unsigned পূর্ণসংখ্যা সূচক_2,
		  ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
		  hb_array_t<দ্বিগুণ> out,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা outer = সূচক_2 >> 16;
    unsigned পূর্ণসংখ্যা inner = সূচক_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_গুনতি, out, cache);
  }
  শূন্য get_delta (unsigned পূর্ণসংখ্যা সূচক_2,
		  hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
		  hb_array_t<দ্বিগুণ> out,
		  hb_scalar_cache_t *cache = nullptr) ধ্রুবক
  {
    ফেরত get_delta (সূচক_2,
		      coords.arrayZ, coords.দৈর্ঘ্য,
		      out,
		      cache);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত সত্য;
#endif

    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, এই) &&
		  তথ্য_Sets.sanitize (c, এই));
  }

  যৌক্তিক create_subset_plan (ধ্রুবক hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ধ্রুবক
  {
    যদি (unlikely (!inner_maps->resize (তথ্য_Sets.len))) ফেরত মিথ্যা;
    জন্য (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      যদি (unlikely (outer >= তথ্য_Sets.len ||
		    inner >= (এই+তথ্য_Sets[outer]).get_item_গুনতি ()))
	ফেরত মিথ্যা;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    জন্য (unsigned outer = 0; outer < inner_maps->দৈর্ঘ্য; outer++)
    {
      ধ্রুবক hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      যদি (!inner_map.get_population ()) চালু;
      যদি (unlikely (inner_map.in_error ())) ফেরত মিথ্যা;
      জন্য (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    ফেরত !inner_maps->in_error () && !varidx_map->in_error ();
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক MultiItemVariationStore *src,
		  ধ্রুবক hb_array_t<ধ্রুবক hb_inc_bimap_t> &inner_maps,
		  ধ্রুবক hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই) ||
		  inner_maps.দৈর্ঘ্য < src->তথ্য_Sets.len))
      return_trace (মিথ্যা);
    format = 1;

    ধ্রুবক auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_গুনতি = 0;
    জন্য (unsigned i = 0; i < src->তথ্য_Sets.len; i++)
    {
      যদি (!inner_maps[i].get_population ()) চালু;
      set_গুনতি++;
      (src+src->তথ্য_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    যদি (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (মিথ্যা);

    তথ্য_Sets.len = set_গুনতি;
    যদি (unlikely (!c->extend (তথ্য_Sets))) return_trace (মিথ্যা);
    unsigned set_সূচক = 0;
    জন্য (unsigned i = 0; i < src->তথ্য_Sets.len; i++)
    {
      যদি (!inner_maps[i].get_population ()) চালু;
      যদি (unlikely (!তথ্য_Sets[set_সূচক++].serialize_serialize (
			    c, &(src+src->তথ্য_Sets[i]), inner_maps[i], region_map)))
	return_trace (মিথ্যা);
    }
    return_trace (সত্য);
  }

  সুরক্ষিত:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<Multi_Var_তথ্য>	তথ্য_Sets;
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (8, তথ্য_Sets);
};

template <typename Map_গুনতি_T>
কাঠামো Delta_Set_সূচক_Map_Format01
{
  friend কাঠামো Delta_Set_সূচক_Map;

  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_mul_add_saturate (map_গুনতি_2, get_width (), min_আকার); }

  ব্যক্তিগত:
  Delta_Set_সূচক_Map_Format01* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed (এই));
  }

  template <typename T>
  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক T &plan)
  {
    unsigned পূর্ণসংখ্যা width = plan.get_width ();
    unsigned পূর্ণসংখ্যা inner_bit_গুনতি = plan.get_inner_bit_গুনতি ();
    ধ্রুবক hb_array_t<ধ্রুবক uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (এই);
    যদি (unlikely (output_map.দৈর্ঘ্য && ((((inner_bit_গুনতি-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (মিথ্যা);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);

    entryFormat = ((width-1)<<4)|(inner_bit_গুনতি-1);
    map_গুনতি_2 = output_map.দৈর্ঘ্য;
    HBUINT8 *p = c->allocate_আকার<HBUINT8> (width * output_map.দৈর্ঘ্য);
    যদি (unlikely (!p)) return_trace (মিথ্যা);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < output_map.দৈর্ঘ্য; i++)
    {
      unsigned পূর্ণসংখ্যা v = output_map.arrayZ[i];
      যদি (v)
      {
	unsigned পূর্ণসংখ্যা outer = v >> 16;
	unsigned পূর্ণসংখ্যা inner = v & 0xFFFF;
	unsigned পূর্ণসংখ্যা u = (outer << inner_bit_গুনতি) | inner;
	জন্য (unsigned পূর্ণসংখ্যা w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (সত্য);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned পূর্ণসংখ্যা v) ধ্রুবক /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    যদি (!map_গুনতি_2)
      ফেরত v;
    ফেরত _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned পূর্ণসংখ্যা v) ধ্রুবক /* Returns 16.16 outer.inner. */
  {
    যদি (v >= map_গুনতি_2)
      v = map_গুনতি_2 - 1;

    unsigned পূর্ণসংখ্যা u = 0;
    { /* Fetch it. */
      unsigned পূর্ণসংখ্যা w = get_width ();
      ধ্রুবক HBUINT8 *p = map_তথ্য_Z.arrayZ + w * v;
      জন্য (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned পূর্ণসংখ্যা n = get_inner_bit_গুনতি ();
      unsigned পূর্ণসংখ্যা outer = u >> n;
      unsigned পূর্ণসংখ্যা inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    ফেরত u;
  }

  unsigned get_map_গুনতি () ধ্রুবক       { ফেরত map_গুনতি_2; }
  unsigned get_width () ধ্রুবক           { ফেরত ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_গুনতি () ধ্রুবক { ফেরত (entryFormat & 0xF) + 1; }


  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
                  c->check_range (map_তথ্য_Z.arrayZ,
                                  map_গুনতি_2,
                                  get_width ()));
  }

  সুরক্ষিত:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_গুনতি_T     map_গুনতি_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                map_তথ্য_Z;       /* The delta-set index mapping data. */

  সর্বজনীন:
  DEFINE_আকার_ARRAY (2+Map_গুনতি_T::static_আকার, map_তথ্য_Z);
};

কাঠামো Delta_Set_সূচক_Map
{
  template <typename T>
  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক T &plan)
  {
    TRACE_SERIALIZE (এই);
    unsigned দৈর্ঘ্য = plan.get_output_map ().দৈর্ঘ্য;
    u.format.v = দৈর্ঘ্য <= 0xFFFF ? 0 : 1;
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  uint32_t map (unsigned v) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); ফেরত (u.format0.map (v));
    ক্ষেত্র 1: hb_barrier (); ফেরত (u.format1.map (v));
    পূর্বনির্ধারিত:ফেরত v;
    }
  }

  unsigned get_map_গুনতি () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_map_গুনতি ();
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_map_গুনতি ();
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  unsigned get_width () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_width ();
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_width ();
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  unsigned get_inner_bit_গুনতি () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_inner_bit_গুনতি ();
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_inner_bit_গুনতি ();
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  Delta_Set_সূচক_Map* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); return_trace (reinterpret_cast<Delta_Set_সূচক_Map *> (u.format0.copy (c)));
    ক্ষেত্র 1: hb_barrier (); return_trace (reinterpret_cast<Delta_Set_সূচক_Map *> (u.format1.copy (c)));
    পূর্বনির্ধারিত:return_trace (nullptr);
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT8 v; }             format;         /* Format identifier */
  Delta_Set_সূচক_Map_Format01<HBUINT16> format0;
  Delta_Set_সূচক_Map_Format01<HBUINT32> format1;
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (1, format.v);
};


কাঠামো ItemVarStoreInstancer
{
  ItemVarStoreInstancer (ধ্রুবক ItemVariationStore *varStore_,
			 ধ্রুবক Delta_Set_সূচক_Map *varIdxMap,
			 hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    যদি (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    যদি (!hb_any (coords))
      এই->coords = {};
  }

  operator যৌক্তিক () ধ্রুবক { ফেরত varStore && যৌক্তিক (coords); }

  দশমিক operator[] (uint32_t varIdx) ধ্রুবক
  { ফেরত (*এই) (varIdx); }

  দশমিক operator() (uint32_t varIdx, unsigned short offset = 0) ধ্রুবক
  { ফেরত get_delta (varIdx, offset); }

  দ্বিগুণ get_condition_delta (uint32_t varIdx) ধ্রুবক
  { ফেরত get_delta<দ্বিগুণ> (varIdx); }

  template <typename Float = দশমিক>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) ধ্রুবক
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   যদি (!coords || varIdx == VarIdx::NO_VARIATION)
     ফেরত Float (0);

    varIdx += offset;
    যদি (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    ফেরত varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ধ্রুবক ItemVariationStore *varStore;
  ধ্রুবক Delta_Set_সূচক_Map *varIdxMap;
  hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords;
  hb_scalar_cache_t *cache;
};

কাঠামো MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (ধ্রুবক MultiItemVariationStore *varStore,
			      ধ্রুবক Delta_Set_সূচক_Map *varIdxMap,
			      hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    যদি (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator যৌক্তিক () ধ্রুবক { ফেরত varStore; }

  দ্বিগুণ get_condition_delta (uint32_t varIdx) ধ্রুবক
  { ফেরত (দ্বিগুণ) (*এই)[varIdx]; }

  দ্বিগুণ operator[] (uint32_t varIdx) ধ্রুবক
  {
    দ্বিগুণ v = 0;
    (*এই) (hb_array (&v, 1), varIdx);
    ফেরত v;
  }

  শূন্য operator() (hb_array_t<দ্বিগুণ> out, uint32_t varIdx, unsigned short offset = 0) ধ্রুবক
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
      জন্য (unsigned i = 0; i < out.দৈর্ঘ্য; i++)
        out.arrayZ[i] = 0.;
  }

  ধ্রুবক MultiItemVariationStore *varStore;
  ধ্রুবক Delta_Set_সূচক_Map *varIdxMap;
  hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords;
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

গণনা lookup_condition_subset_ফলাফল_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

কাঠামো Condition;

template <typename Instancer>
স্থির যৌক্তিক
_hb_recurse_condition_evaluate (ধ্রুবক কাঠামো Condition &condition,
				ধ্রুবক পূর্ণসংখ্যা *coords,
				unsigned পূর্ণসংখ্যা coord_len,
				Instancer *instancer);

কাঠামো ConditionAxisRange
{
  friend কাঠামো Condition;

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    ধ্রুবক hb_map_t *সূচক_map = &c->plan->axes_সূচক_map;
    যদি (সূচক_map->is_empty ()) return_trace (সত্য);

    ধ্রুবক hb_map_t& axes_old_সূচক_tag_map = c->plan->axes_old_সূচক_tag_map;
    hb_codepoint_t *axis_tag;
    যদি (!axes_old_সূচক_tag_map.has (axis_সূচক_2, &axis_tag) ||
        !সূচক_map->has (axis_সূচক_2))
      return_trace (মিথ্যা);

    ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    যদি (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    ধ্রুবক hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    যদি (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    দশমিক normalized_min = renormalize_মান ((দ্বিগুণ) filter_Range_Min_মান.to_float (), axis_limit, axis_triple_distances, মিথ্যা);
    দশমিক normalized_max = renormalize_মান ((দ্বিগুণ) filter_Range_Max_মান.to_float (), axis_limit, axis_triple_distances, মিথ্যা);
    out->filter_Range_Min_মান.set_float (normalized_min);
    out->filter_Range_Max_মান.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axis_সূচক_2, সূচক_map->get (axis_সূচক_2),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ব্যক্তিগত:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) ধ্রুবক
  {
    //invalid axis index, drop the entire record
    যদি (!c->axes_সূচক_tag_map->has (axis_সূচক_2))
      ফেরত DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_সূচক_tag_map->get (axis_সূচক_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    যৌক্তিক axis_set_by_user = মিথ্যা;
    যদি (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = সত্য;
    }

    দশমিক axis_min_val = axis_range.minimum;
    দশমিক axis_default_val = axis_range.মধ্য;
    দশমিক axis_max_val = axis_range.maximum;

    দশমিক filter_min_val = filter_Range_Min_মান.to_float ();
    দশমিক filter_max_val = filter_Range_Max_মান.to_float ();

    যদি (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = মিথ্যা;

    //condition not met, drop the entire record
    যদি (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      ফেরত DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    যদি (axis_set_by_user && axis_range.is_point ())
      ফেরত DROP_COND_WITH_VAR;

    যদি (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_মান.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_মান.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axis_সূচক_2, val);
      ফেরত KEEP_COND_WITH_VAR;
    }
    ফেরত KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  যৌক্তিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		 Instancer *instancer HB_UNUSED) ধ্রুবক
  {
    পূর্ণসংখ্যা coord = axis_সূচক_2 < coord_len ? coords[axis_সূচক_2] : 0;
    ফেরত filter_Range_Min_মান.to_int () <= coord && coord <= filter_Range_Max_মান.to_int ();
  }

  সর্বজনীন:
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axis_সূচক_2;
  F2DOT14	filter_Range_Min_মান;
  F2DOT14	filter_Range_Max_মান;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো Condition_মান
{
  friend কাঠামো Condition;

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (varIdx == VarIdx::NO_VARIATION)
      return_trace (সত্য);

    hb_pair_t<unsigned, পূর্ণসংখ্যা> *condition_মান;
    যদি (c->plan->layout_condition_idx_মান_map.has ((uintptr_t) এই, &condition_মান))
    {
      out->default_মান_2 = condition_মান->second;
      out->varIdx = condition_মান->first;
      return_trace (সত্য);
    }

    hb_pair_t<unsigned, পূর্ণসংখ্যা> *new_varidx_delta;
    যদি (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (মিথ্যা);

    যদি (unlikely (!c->serializer->check_assign (
		out->default_মান_2,
		default_মান_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (মিথ্যা);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (সত্য);
  }

  ব্যক্তিগত:
  template <typename Instancer>
  যৌক্তিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		 Instancer *instancer) ধ্রুবক
  {
    দ্বিগুণ value = default_মান_2;
    value += instancer->get_condition_delta (varIdx);
    ফেরত value > 0;
  }

  সর্বজনীন:
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_মান_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো ConditionAnd
{
  friend কাঠামো Condition;

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (মিথ্যা);
    out->format = format;

    জন্য (ধ্রুবক auto &offset : conditions)
    {
      যদি (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	যদি (unlikely (!out_offset)) return_trace (মিথ্যা);
	*out_offset = 0;
      }
      অন্যথা যদি (unlikely (!subset_offset_array (c, out->conditions, এই) (offset)))
	return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  ব্যক্তিগত:
  template <typename Instancer>
  যৌক্তিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		 Instancer *instancer) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = conditions.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!_hb_recurse_condition_evaluate (এই+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  সর্বজনীন:
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (conditions.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<কাঠামো Condition>	conditions;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (3, conditions);
};

কাঠামো ConditionOr
{
  friend কাঠামো Condition;

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (মিথ্যা);
    out->format = format;

    জন্য (ধ্রুবক auto &offset : conditions)
    {
      যদি (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	যদি (unlikely (!out_offset)) return_trace (মিথ্যা);
	*out_offset = 0;
      }
      অন্যথা যদি (unlikely (!subset_offset_array (c, out->conditions, এই) (offset)))
	return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  ব্যক্তিগত:
  template <typename Instancer>
  যৌক্তিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		 Instancer *instancer) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = conditions.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (_hb_recurse_condition_evaluate (এই+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	ফেরত সত্য;
    ফেরত মিথ্যা;
  }

  সর্বজনীন:
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (conditions.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<কাঠামো Condition>	conditions;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (3, conditions);
};

কাঠামো ConditionNegate
{
  friend কাঠামো Condition;

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    যদি (condition.is_null ())
    {
      out->condition = 0;
      return_trace (সত্য);
    }
    return_trace (out->condition.serialize_subset (c, condition, এই));
  }

  ব্যক্তিগত:
  template <typename Instancer>
  যৌক্তিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		 Instancer *instancer) ধ্রুবক
  {
    ফেরত !_hb_recurse_condition_evaluate (এই+condition,
					    coords, coord_len,
					    instancer);
  }

  সর্বজনীন:
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (condition.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<কাঠামো Condition>	condition;
  সর্বজনীন:
  DEFINE_আকার_STATIC (5);
};

কাঠামো Condition
{
  friend কাঠামো ConditionSet;

  স্থির যৌক্তিক serialize_constant (hb_serialize_context_t *c, যৌক্তিক value)
  {
    auto *out = c->start_embed<Condition_মান> ();
    যদি (unlikely (!out || !c->extend_min (out))) ফেরত মিথ্যা;
    out->format = 2;
    out->default_মান_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    ফেরত সত্য;
  }

  lookup_condition_subset_ফলাফল_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      যৌক্তিক materialize_true = মিথ্যা) ধ্রুবক;

  template <typename Instancer>
  যৌক্তিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		 Instancer *instancer) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    ক্ষেত্র 0: ফেরত সত্য;
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.evaluate (coords, coord_len, instancer);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.evaluate (coords, coord_len, instancer);
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.evaluate (coords, coord_len, instancer);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.evaluate (coords, coord_len, instancer);
    ক্ষেত্র 5: hb_barrier (); ফেরত u.format5.evaluate (coords, coord_len, instancer);
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) ধ্রুবক
  {
    যদি (unlikely (!depth))
    {
      c->apply = মিথ্যা;
      ফেরত DROP_RECORD_WITH_VAR;
    }

    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: ফেরত KEEP_RECORD_WITH_VAR;
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.keep_with_variations (c, condition_map);
    ক্ষেত্র 2: ক্ষেত্র 3: ক্ষেত্র 4: ক্ষেত্র 5: বিরতি;
    পূর্বনির্ধারিত: c->apply = মিথ্যা; ফেরত DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    যৌক্তিক applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.দৈর্ঘ্য, c->instancer);
    c->apply &= applies;
    যদি (c->all_axes_pinned)
      ফেরত applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    যদি (u.format.v == 2)
      ফেরত u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (ধ্রুবক Condition &child)
    {
      auto child_context = *c;
      child_context.apply = সত্য;
      ফেরত child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    যদি (u.format.v == 5)
    {
      auto ফলাফল = classify_child (&u.format5 + u.format5.condition);
      ফেরত ফলাফল == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             ফলাফল == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    যৌক্তিক is_and = u.format.v == 3;
    যৌক্তিক variable = মিথ্যা;
    ধ্রুবক auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ধ্রুবক শূন্য *base = is_and ? (ধ্রুবক শূন্য *) &u.format3 : (ধ্রুবক শূন্য *) &u.format4;
    জন্য (ধ্রুবক auto &offset : conditions)
    {
      auto ফলাফল = classify_child (base + offset);
      যদি (is_and && ফলাফল == DROP_RECORD_WITH_VAR)
        ফেরত DROP_RECORD_WITH_VAR;
      যদি (!is_and && (ফলাফল == DROP_COND_WITH_VAR || ফলাফল == KEEP_RECORD_WITH_VAR))
        ফেরত KEEP_RECORD_WITH_VAR;
      variable |= ফলাফল == KEEP_COND_WITH_VAR;
    }
    ফেরত variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ক্ষেত্র 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ক্ষেত্র 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ক্ষেত্র 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    যদি (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_মান ());

    return_trace (c->end_recursion (এই->dispatch (c)));
  }

  যৌক্তিক collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *condition_মানসমূহ = nullptr) ধ্রুবক;

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক Condition *src,
		  ধ্রুবক hb_map_t &varidx_map,
		  ধ্রুবক hb_map_t *axis_map = nullptr);

  ব্যক্তিগত:
  lookup_condition_subset_ফলাফল_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) ধ্রুবক;

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_মান	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);
};

template <typename Instancer>
যৌক্তিক
_hb_recurse_condition_evaluate (ধ্রুবক কাঠামো Condition &condition,
				ধ্রুবক পূর্ণসংখ্যা *coords,
				unsigned পূর্ণসংখ্যা coord_len,
				Instancer *instancer)
{
  ফেরত condition.evaluate (coords, coord_len, instancer);
}

কাঠামো ConditionList
{
  ধ্রুবক Condition& operator[] (unsigned i) ধ্রুবক
  { ফেরত এই+conditions[i]; }

  unsigned get_গুনতি () ধ্রুবক { ফেরত conditions.len; }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক ConditionList *src,
		  ধ্রুবক hb_inc_bimap_t &condition_map,
		  ধ্রুবক hb_map_t &varidx_map,
		  ধ্রুবক hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    conditions.len = condition_map.get_population ();
    যদি (unlikely (!c->extend (conditions))) return_trace (মিথ্যা);
    জন্য (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_সূচক = condition_map.backward (i);
      যদি (unlikely (old_সূচক >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_সূচক]), varidx_map, axis_map)))
	return_trace (মিথ্যা);
    }
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (conditions.sanitize (c, এই));
  }

  সুরক্ষিত:
  Array32OfOffset32To<Condition> conditions;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, conditions);
};

অন্তঃরেখ যৌক্তিক
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *condition_মানসমূহ) ধ্রুবক
{
  যদি (unlikely (!depth)) ফেরত মিথ্যা;
  নির্বাচন (u.format.v)
  {
    ক্ষেত্র 0:
    ক্ষেত্র 1:
      ফেরত সত্য;
    ক্ষেত্র 2:
      যদি (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        যদি (condition_মানসমূহ &&
            !condition_মানসমূহ->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), পূর্ণসংখ্যা (u.format2.default_মান_2))))
          ফেরত মিথ্যা;
      }
      ফেরত !var_indices->in_error ();
    ক্ষেত্র 3:
      জন্য (ধ্রুবক auto &offset : u.format3.conditions)
	যদি (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_মানসমূহ)))
	  ফেরত মিথ্যা;
      ফেরত সত্য;
    ক্ষেত্র 4:
      জন্য (ধ্রুবক auto &offset : u.format4.conditions)
	যদি (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_মানসমূহ)))
	  ফেরত মিথ্যা;
      ফেরত সত্য;
    ক্ষেত্র 5:
      ফেরত (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_মানসমূহ);
    পূর্বনির্ধারিত:
      ফেরত মিথ্যা;
  }
}

অন্তঃরেখ যৌক্তিক
Condition::serialize (hb_serialize_context_t *c,
		      ধ্রুবক Condition *src,
		      ধ্রুবক hb_map_t &varidx_map,
		      ধ্রুবক hb_map_t *axis_map)
{
  TRACE_SERIALIZE (এই);
  নির্বাচন (src->u.format.v)
  {
    ক্ষেত্র 0:
    {
      auto *out = c->start_embed<Condition_মান> ();
      যদি (unlikely (!out || !c->extend_min (out))) return_trace (মিথ্যা);
      out->format = 2;
      out->default_মান_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (সত্য);
    }
    ক্ষেত্র 1:
    {
      auto *out = c->embed (&src->u.format1);
      যদি (unlikely (!out)) return_trace (মিথ্যা);
      যদি (axis_map)
      {
	যদি (unlikely (!axis_map->has (out->axis_সূচক_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (মিথ্যা);
	}
	যদি (unlikely (!c->check_assign (out->axis_সূচক_2, axis_map->get (out->axis_সূচক_2),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (মিথ্যা);
      }
      return_trace (সত্য);
    }
    ক্ষেত্র 2:
    {
      auto *out = c->embed (&src->u.format2);
      যদি (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (মিথ্যা);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (সত্য);
    }
    ক্ষেত্র 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      যদি (unlikely (!c->extend_min (out))) return_trace (মিথ্যা);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      যদি (unlikely (!c->extend (out->conditions))) return_trace (মিথ্যা);
      জন্য (unsigned i = 0; i < out->conditions.len; i++)
	যদি (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (মিথ্যা);
      return_trace (সত্য);
    }
    ক্ষেত্র 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      যদি (unlikely (!c->extend_min (out))) return_trace (মিথ্যা);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      যদি (unlikely (!c->extend (out->conditions))) return_trace (মিথ্যা);
      জন্য (unsigned i = 0; i < out->conditions.len; i++)
	যদি (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (মিথ্যা);
      return_trace (সত্য);
    }
    ক্ষেত্র 5:
    {
      auto *out = c->embed (&src->u.format5);
      যদি (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (মিথ্যা);
      return_trace (সত্য);
    }
    পূর্বনির্ধারিত:
      return_trace (মিথ্যা);
  }
}

অন্তঃরেখ lookup_condition_subset_ফলাফল_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    যৌক্তিক materialize_true) ধ্রুবক
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_ফলাফল_t ফলাফল =
      subset_lookup_condition_impl (c, depth);
  যদি (ফলাফল == LOOKUP_CONDITION_SUBSET_KEEP)
    ফেরত ফলাফল;

  c->serializer->revert (snap);
  যদি (ফলাফল != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    ফেরত ফলাফল;

  ফেরত serialize_constant (c->serializer, সত্য) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

অন্তঃরেখ lookup_condition_subset_ফলাফল_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) ধ্রুবক
{
  যদি (unlikely (!depth))
    ফেরত LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   ধ্রুবক Condition &condition)
      -> lookup_condition_subset_ফলাফল_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    যদি (unlikely (!offset))
      ফেরত LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_ফলাফল_t ফলাফল =
        condition.subset_lookup_condition (c, depth - 1);
    যদি (ফলাফল == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->বের_করো_pack ());
    অন্যথা
    {
      c->serializer->বের_করো_discard ();
      out.বের_করো ();
      c->serializer->revert (snap);
    }
    ফেরত ফলাফল;
  };

  নির্বাচন (u.format.v)
  {
    ক্ষেত্র 0:
      ফেরত LOOKUP_CONDITION_SUBSET_TRUE;

    ক্ষেত্র 1:
    {
      ধ্রুবক ConditionAxisRange &src = u.format1;
      যদি (c->plan->user_axes_location.is_empty ())
        ফেরত c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      যদি (!c->plan->axes_old_সূচক_tag_map.has (src.axis_সূচক_2, &axis_tag))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      যদি (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      দশমিক filter_min = src.filter_Range_Min_মান.to_float ();
      দশমিক filter_max = src.filter_Range_Max_মান.to_float ();
      যদি (filter_min > filter_max)
        ফেরত LOOKUP_CONDITION_SUBSET_FALSE;
      যদি (axis_limit.is_point ())
        ফেরত filter_min <= axis_limit.মধ্য &&
	       axis_limit.মধ্য <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      যদি (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        ফেরত LOOKUP_CONDITION_SUBSET_FALSE;
      যদি (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        ফেরত LOOKUP_CONDITION_SUBSET_TRUE;
      যদি (!c->plan->axes_সূচক_map.has (src.axis_সূচক_2))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      যদি (unlikely (!out))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      যদি (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_মান.set_float (
	  renormalize_মান (filter_min, axis_limit, axis_distances, মিথ্যা));
      out->filter_Range_Max_মান.set_float (
	  renormalize_মান (filter_max, axis_limit, axis_distances, মিথ্যা));
      যদি (unlikely (!c->serializer->check_assign (
		out->axis_সূচক_2,
		c->plan->axes_সূচক_map.get (src.axis_সূচক_2),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;
      ফেরত LOOKUP_CONDITION_SUBSET_KEEP;
    }

    ক্ষেত্র 2:
    {
      ধ্রুবক Condition_মান &src = u.format2;
      auto *out = c->serializer->embed (&src);
      যদি (unlikely (!out))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;

      যদি (src.varIdx == VarIdx::NO_VARIATION)
        ফেরত c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((পূর্ণসংখ্যা) src.default_মান_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, পূর্ণসংখ্যা> *condition_মান;
      যদি (c->plan->layout_condition_idx_মান_map.has ((uintptr_t) &src, &condition_মান))
      {
        out->default_মান_2 = condition_মান->second;
        out->varIdx = condition_মান->first;
        যদি (out->varIdx == VarIdx::NO_VARIATION)
          ফেরত out->default_মান_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        ফেরত LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, পূর্ণসংখ্যা> *new_varidx_delta;
      যদি (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        ফেরত c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((পূর্ণসংখ্যা) src.default_মান_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      যদি (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        দ্বিগুণ value = (পূর্ণসংখ্যা) src.default_মান_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        ফেরত value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      পূর্ণসংখ্যা default_মান = (পূর্ণসংখ্যা) src.default_মান_2 +
			  hb_second (*new_varidx_delta);
      যদি (unlikely (!c->serializer->check_assign (
		out->default_মান_2,
		default_মান,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      ফেরত LOOKUP_CONDITION_SUBSET_KEEP;
    }

    ক্ষেত্র 3:
    {
      ধ্রুবক ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      যদি (unlikely (!out || !c->serializer->extend_min (out)))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      জন্য (ধ্রুবক auto &offset : src.conditions)
      {
        lookup_condition_subset_ফলাফল_t ফলাফল =
            subset_offset (out->conditions, &src + offset);
        যদি (ফলাফল == LOOKUP_CONDITION_SUBSET_ERROR ||
	    ফলাফল == LOOKUP_CONDITION_SUBSET_FALSE)
          ফেরত ফলাফল;
      }
      ফেরত out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    ক্ষেত্র 4:
    {
      ধ্রুবক ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      যদি (unlikely (!out || !c->serializer->extend_min (out)))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      জন্য (ধ্রুবক auto &offset : src.conditions)
      {
        lookup_condition_subset_ফলাফল_t ফলাফল =
            subset_offset (out->conditions, &src + offset);
        যদি (ফলাফল == LOOKUP_CONDITION_SUBSET_ERROR ||
	    ফলাফল == LOOKUP_CONDITION_SUBSET_TRUE)
          ফেরত ফলাফল;
      }
      ফেরত out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    ক্ষেত্র 5:
    {
      ধ্রুবক ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      যদি (unlikely (!out))
        ফেরত LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_ফলাফল_t ফলাফল =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      যদি (ফলাফল == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->বের_করো_pack ());
        ফেরত ফলাফল;
      }

      c->serializer->বের_করো_discard ();
      যদি (ফলাফল == LOOKUP_CONDITION_SUBSET_TRUE)
        ফেরত LOOKUP_CONDITION_SUBSET_FALSE;
      যদি (ফলাফল == LOOKUP_CONDITION_SUBSET_FALSE)
        ফেরত LOOKUP_CONDITION_SUBSET_TRUE;
      ফেরত ফলাফল;
    }

    পূর্বনির্ধারিত:
      ফেরত LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

কাঠামো ConditionSet
{
  যৌক্তিক evaluate (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		 ItemVarStoreInstancer *instancer) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = conditions.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!(এই+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  শূন্য keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ধ্রুবক
  {
    hb_map_t *condition_map = hb_map_create ();
    যদি (unlikely (!condition_map)) ফেরত;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    যদি (unlikely (!cond_set)) ফেরত;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = সত্য;
    যৌক্তিক should_keep = মিথ্যা;
    /* The range map cannot identify value or boolean expressions uniquely. */
    যৌক্তিক can_deduplicate = সত্য;
    unsigned অঙ্ক_kept_cond = 0, cond_idx = 0;
    জন্য (ধ্রুবক auto& offset : conditions)
    {
      ধ্রুবক Condition &condition = এই+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      যদি (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = মিথ্যা;
        ফেরত;
      }

      যদি (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = সত্য;
        cond_set->add (cond_idx);
        অঙ্ক_kept_cond++;
      }

      যদি (ret == KEEP_RECORD_WITH_VAR)
        should_keep = সত্য;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = অঙ্ক_kept_cond == 0;
    যদি (!should_keep && c->all_axes_pinned) ফেরত;

    //check if condition_set is unique with variations
    যদি (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      ফেরত;

    যদি (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               যৌক্তিক ঢোকাও_catch_all) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিথ্যা);

    যদি (ঢোকাও_catch_all) return_trace (সত্য);

    hb_set_t *retained_cond_set = nullptr;
    যদি (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned পূর্ণসংখ্যা গুনতি = conditions.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (retained_cond_set != nullptr && !retained_cond_set->has (i))
        চালু;
      ধ্রুবক Condition &condition = এই+conditions[i];
      যদি (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, এই) (conditions[i]);
        চালু;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      যদি (unlikely (!offset)) return_trace (মিথ্যা);
      c->serializer->push ();
      auto ফলাফল = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, সত্য);
      যদি (ফলাফল == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        ফলাফল = Condition::serialize_constant (c->serializer, মিথ্যা) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      যদি (unlikely (ফলাফল == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->বের_করো_discard ();
        c->serializer->check_success (মিথ্যা);
        return_trace (মিথ্যা);
      }
      c->serializer->add_link (*offset, c->serializer->বের_করো_pack ());
    }

    return_trace (যৌক্তিক (out->conditions));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (conditions.sanitize (c, এই));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক auto &offset : conditions)
      যদি (unlikely (!(এই+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_মানসমূহ)))
	c->layout_variation_indices->err ();
  }

  সুরক্ষিত:
  Array16OfOffset32To<Condition>	conditions;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, conditions);
};

কাঠামো FeatureTableSubstitutionRecord
{
  friend কাঠামো FeatureTableSubstitution;

  শূন্য collect_lookups (ধ্রুবক শূন্য *base, hb_set_t *lookup_indexes /* OUT */) ধ্রুবক
  {
    ফেরত (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  শূন্য closure_features (ধ্রুবক শূন্য *base,
			 ধ্রুবক hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ধ্রুবক
  {
    যদি ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (feature_সূচক_2);
  }

  শূন্য collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, ধ্রুবক Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    ধ্রুবক hb_set_t *feature_indices,
                                                    ধ্রুবক শূন্য *base) ধ্রুবক
  {
    যদি (feature_indices->has (feature_সূচক_2))
    {
      feature_substitutes_map->set (feature_সূচক_2, &(base+feature));
      catch_all_record_feature_idxes.add (feature_সূচক_2);
    }
  }

  যৌক্তিক serialize (hb_subset_layout_context_t *c,
                  unsigned feature_সূচক,
                  ধ্রুবক Feature *f, ধ্রুবক Tag *tag)
  {
    TRACE_SERIALIZE (এই);
    hb_serialize_context_t *s = c->subset_context->serializer;
    যদি (unlikely (!s->extend_min (এই))) return_trace (মিথ্যা);

    uint32_t *new_feature_idx;
    যদি (!c->feature_map_w_duplicates->has (feature_সূচক, &new_feature_idx))
      return_trace (মিথ্যা);

    যদি (!s->check_assign (feature_সূচক_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    s->push ();
    যৌক্তিক ret = f->subset (c->subset_context, c, tag);
    যদি (ret) s->add_link (feature, s->বের_করো_pack ());
    অন্যথা s->বের_করো_discard ();

    return_trace (ret);
  }

  যৌক্তিক subset (hb_subset_layout_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    uint32_t *new_feature_সূচক;
    যদি (!c->feature_map_w_duplicates->has (feature_সূচক_2, &new_feature_সূচক))
      return_trace (মিথ্যা);

    auto *out = c->subset_context->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->feature_সূচক_2 = *new_feature_সূচক;
    hb_pair_t<ধ্রুবক শূন্য*, ধ্রুবক শূন্য*> *original;
    যদি (!c->feature_idx_tag_map->has (feature_সূচক_2, &original))
      return_trace (মিথ্যা);
    ধ্রুবক Tag *tag = reinterpret_cast<ধ্রুবক Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base,
		 ধ্রুবক FeatureList *feature_list) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই))) return_trace (মিথ্যা);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ধ্রুবক Record_sanitize_closure_t closure = {feature_list->get_tag (feature_সূচক_2), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  সুরক্ষিত:
  HBUINT16		feature_সূচক_2;
  Offset32To<Feature>	feature;
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো FeatureTableSubstitution
{
  শূন্য collect_feature_indices (hb_set_t *feature_indices) ধ্রুবক
  {
    জন্য (ধ্রুবক FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.feature_সূচক_2);
  }

  ধ্রুবক Feature *find_substitute (unsigned পূর্ণসংখ্যা feature_সূচক) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = substitutions.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      ধ্রুবক FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      যদি (record.feature_সূচক_2 == feature_সূচক)
	ফেরত &(এই+record.feature);
    }
    ফেরত nullptr;
  }

  শূন্য collect_lookups (ধ্রুবক hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ধ্রুবক
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::feature_সূচক_2)
    | hb_apply ([এই, lookup_indexes] (ধ্রুবক FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (এই, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  শূন্য collect_name_ids (ধ্রুবক hb_map_t *feature_সূচক_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ধ্রুবক
  {
    জন্য (ধ্রুবক FeatureTableSubstitutionRecord &record : substitutions)
      যদি (feature_সূচক_map->has (record.feature_সূচক_2))
        (এই+record.feature).collect_name_ids (get_tag (record.feature_সূচক_2), nameids_to_retain);
  }

  শূন্য closure_features (ধ্রুবক hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ধ্রুবক
  {
    জন্য (ধ্রুবক FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (এই, lookup_indexes, feature_indexes);
  }

  যৌক্তিক intersects_features (ধ্রুবক hb_map_t *feature_সূচক_map) ধ্রুবক
  {
    জন্য (ধ্রুবক FeatureTableSubstitutionRecord& record : substitutions)
    {
      যদি (feature_সূচক_map->has (record.feature_সূচক_2)) ফেরত সত্য;
    }
    ফেরত মিথ্যা;
  }

  শূন্য collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, এই);
  }

  যৌক্তিক subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               যৌক্তিক ঢোকাও_catch_all) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিথ্যা);

    out->version.major = version.major;
    out->version.minor = version.minor;

    যদি (ঢোকাও_catch_all)
    {
      জন্য (unsigned feature_সূচক : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<ধ্রুবক শূন্য*, ধ্রুবক শূন্য*> *p;
        যদি (!l->feature_idx_tag_map->has (feature_সূচক, &p))
          return_trace (মিথ্যা);
        auto *o = out->substitutions.serialize_append (c->serializer);
        যদি (!o->serialize (l, feature_সূচক,
                           reinterpret_cast<ধ্রুবক Feature*> (p->first),
                           reinterpret_cast<ধ্রুবক Tag*> (p->second)))
          return_trace (মিথ্যা);
      }
      return_trace (সত্য);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), এই))
    ;

    return_trace (যৌক্তিক (out->substitutions));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক FeatureList *feature_list) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, এই, feature_list));
  }

  সুরক্ষিত:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, substitutions);
};

কাঠামো FeatureVariationRecord
{
  friend কাঠামো FeatureVariations;

  শূন্য collect_lookups (ধ্রুবক শূন্য     *base,
			ধ্রুবক hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ধ্রুবক
  {
    ফেরত (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  শূন্য collect_name_ids (ধ্রুবক শূন্য *base,
                         ধ্রুবক hb_map_t *feature_সূচক_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ধ্রুবক
  {
    (base+substitutions).collect_name_ids (feature_সূচক_map, get_tag, nameids_to_retain);
  }

  শূন্য closure_features (ধ্রুবক শূন্য     *base,
			 ধ্রুবক hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ধ্রুবক
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  যৌক্তিক intersects_features (ধ্রুবক শূন্য *base, ধ্রুবক hb_map_t *feature_সূচক_map) ধ্রুবক
  {
    ফেরত (base+substitutions).intersects_features (feature_সূচক_map);
  }

  শূন্য collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ধ্রুবক শূন্য *base) ধ্রুবক
  {
    (base+conditions).keep_with_variations (c);
    যদি (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = সত্য; // set variations only once
    }
  }

  যৌক্তিক subset (hb_subset_layout_context_t *c, ধ্রুবক শূন্য *base,
               যৌক্তিক ঢোকাও_catch_all = মিথ্যা) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->subset_context->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    যদি (!ঢোকাও_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    অন্যথা
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, ঢোকাও_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, ঢোকাও_catch_all);

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base,
		 ধ্রুবক FeatureList *feature_list) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ধ্রুবক শূন্য *base) ধ্রুবক
  { (base+conditions).collect_variation_indices (c); }

  সুরক্ষিত:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো Lookup_সূচক_List
{
  শূন্য add_lookup_indexes_to (hb_set_t *lookup_indexes) ধ্রুবক
  { lookupIndices.add_indexes_to (lookup_indexes); }

  যৌক্তিক intersects_lookup_indexes (ধ্রুবক hb_map_t *lookup_indexes) ধ্রুবক
  { ফেরত lookupIndices.intersects (lookup_indexes); }

  যৌক্তিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (মিথ্যা);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_সূচক_map)
    | hb_map (l->lookup_সূচক_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (যৌক্তিক (out->lookupIndices));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (lookupIndices.sanitize (c));
  }

  সুরক্ষিত:
  সূচক_Array	lookupIndices;
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (2, lookupIndices);
};

কাঠামো LookupConditionRecord
{
  unsigned get_condition_offset (ধ্রুবক শূন্য *base, ধ্রুবক শূন্য *variations) ধ্রুবক
  { ফেরত (ধ্রুবক অক্ষর *) &(base+condition) - (ধ্রুবক অক্ষর *) variations; }

  template <typename Instancer>
  যৌক্তিক evaluate (ধ্রুবক শূন্য *base,
		 ধ্রুবক পূর্ণসংখ্যা *coords,
		 unsigned পূর্ণসংখ্যা coord_len,
		 Instancer *instancer) ধ্রুবক
  { ফেরত (base+condition).evaluate (coords, coord_len, instancer); }

  যৌক্তিক evaluate (ধ্রুবক শূন্য *base,
                 ধ্রুবক শূন্য *variations,
                 hb_sorted_array_t<ধ্রুবক unsigned> conditions) ধ্রুবক
  {
    ফেরত !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.দৈর্ঘ্য,
                                     আকার (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  যৌক্তিক collect_condition (ধ্রুবক শূন্য *base,
                          ধ্রুবক শূন্য *variations,
                          ধ্রুবক পূর্ণসংখ্যা *coords,
                          unsigned পূর্ণসংখ্যা coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) ধ্রুবক
  {
    /* Null conditions always apply and need no key state. */
    যদি (!condition) ফেরত সত্য;

    unsigned offset = get_condition_offset (base, variations);
    যদি (visited->has (offset)) ফেরত সত্য;
    visited->add (offset);
    যদি (unlikely (visited->in_error ())) ফেরত মিথ্যা;

    যদি (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    ফেরত !conditions->in_error ();
  }

  শূন্য add_lookup_indexes_to (ধ্রুবক শূন্য *base,
			      hb_set_t *lookup_indexes) ধ্রুবক
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  যৌক্তিক intersects_lookup_indexes (ধ্রুবক শূন্য *base,
				   ধ্রুবক hb_map_t *lookup_indexes) ধ্রুবক
  { ফেরত (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ধ্রুবক শূন্য *base) ধ্রুবক
  {
    যদি (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_মানসমূহ)))
      c->layout_variation_indices->err ();
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_ফলাফল_t ফলাফল =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    সত্য);
    যদি (ফলাফল != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->বের_করো_discard ();
      return_trace (মিথ্যা);
    }
    c->serializer->add_link (out->condition, c->serializer->বের_করো_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  সুরক্ষিত:
  Offset32To<Condition>		condition;
  Offset32To<Lookup_সূচক_List>	lookupIndices;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো FeatureLookupsTable
{
  যৌক্তিক adds_default_lookups () ধ্রুবক { ফেরত flags & 0x0001u; }

  যৌক্তিক collect_conditions (ধ্রুবক শূন্য *variations,
                           ধ্রুবক পূর্ণসংখ্যা *coords,
                           unsigned পূর্ণসংখ্যা coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupConditionRecord &record : records)
      যদি (unlikely (!record.collect_condition (এই, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  শূন্য collect_lookups (ধ্রুবক শূন্য *variations,
                       hb_sorted_array_t<ধ্রুবক unsigned> conditions,
                       hb_set_t *lookup_indexes) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupConditionRecord &record : records)
      যদি (record.evaluate (এই, variations, conditions))
	record.add_lookup_indexes_to (এই, lookup_indexes);
  }

  template <typename Instancer>
  শূন্য collect_lookups (ধ্রুবক পূর্ণসংখ্যা *coords,
			unsigned পূর্ণসংখ্যা coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupConditionRecord &record : records)
      যদি (record.evaluate (এই, coords, coord_len, instancer))
	record.add_lookup_indexes_to (এই, lookup_indexes);
  }

  শূন্য collect_lookups (hb_set_t *lookup_indexes) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (এই, lookup_indexes);
  }

  যৌক্তিক intersects_lookup_indexes (ধ্রুবক hb_map_t *lookup_indexes) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupConditionRecord &record : records)
      যদি (record.intersects_lookup_indexes (এই, lookup_indexes))
	ফেরত সত্য;
    ফেরত মিথ্যা;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupConditionRecord &record : records)
      record.collect_variation_indices (c, এই);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (মিথ্যা);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    জন্য (ধ্রুবক LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      যদি (record.subset (c, l, এই))
	out->records.len++;
      অন্যথা
	c->serializer->revert (snap);
    }

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, এই));
  }

  সুরক্ষিত:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (10, records);
};

কাঠামো LookupVariationRecord
{
  friend কাঠামো FeatureVariations;

  unsigned get_feature_সূচক () ধ্রুবক { ফেরত feature_সূচক_2; }

  শূন্য collect_lookups (ধ্রুবক শূন্য *base,
                       ধ্রুবক Feature &feature,
                       hb_sorted_array_t<ধ্রুবক unsigned> conditions,
                       hb_set_t *lookup_indexes) ধ্রুবক
  {
    ধ্রুবক FeatureLookupsTable &feature_lookups = base+featureLookups;
    যদি (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  শূন্য collect_lookups (ধ্রুবক শূন্য *base,
			ধ্রুবক hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) ধ্রুবক
  {
    যদি (feature_indexes->has (feature_সূচক_2))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  শূন্য closure_features (ধ্রুবক শূন্য *base,
			 ধ্রুবক hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) ধ্রুবক
  {
    যদি ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (feature_সূচক_2);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ধ্রুবক শূন্য *base) ধ্রুবক
  { (base+featureLookups).collect_variation_indices (c); }

  যৌক্তিক subset (hb_subset_layout_context_t *l, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (এই);
    uint32_t *new_feature_সূচক;
    যদি (!l->feature_map_w_duplicates->has (feature_সূচক_2, &new_feature_সূচক))
      return_trace (মিথ্যা);

    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out ||
		  !c->serializer->check_assign (out->feature_সূচক_2,
						*new_feature_সূচক,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (মিথ্যা);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  featureLookups.sanitize (c, base));
  }

  সুরক্ষিত:
  HBUINT16			feature_সূচক_2;
  Offset32To<FeatureLookupsTable>	featureLookups;
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো FeatureVariations
{
  স্থির constexpr unsigned NOT_FOUND_সূচক = 0xFFFFFFFFu;

  ধ্রুবক Array32Of<LookupVariationRecord> &get_lookup_variation_records () ধ্রুবক
  {
    যদি (version.to_int () < 0x00010001u)
      ফেরত Null (Array32Of<LookupVariationRecord>);
    ফেরত StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_গুনতি () ধ্রুবক
  {
    ফেরত varRecords.len;
  }

  যৌক্তিক find_সূচক (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_len,
		   unsigned পূর্ণসংখ্যা *সূচক_2,
		   ItemVarStoreInstancer *instancer) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = varRecords.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      ধ্রুবক FeatureVariationRecord &record = varRecords.arrayZ[i];
      যদি ((এই+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*সূচক_2 = i;
	ফেরত সত্য;
      }
    }
    *সূচক_2 = NOT_FOUND_সূচক;
    ফেরত মিথ্যা;
  }

  ধ্রুবক Feature *find_substitute (unsigned পূর্ণসংখ্যা variations_সূচক,
				  unsigned পূর্ণসংখ্যা feature_সূচক) ধ্রুবক
  {
    ধ্রুবক FeatureVariationRecord &record = varRecords[variations_সূচক];
    ফেরত (এই+record.substitutions).find_substitute (feature_সূচক);
  }

  যৌক্তিক collect_lookup_conditions (ধ্রুবক পূর্ণসংখ্যা *coords,
                                  unsigned পূর্ণসংখ্যা coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) ধ্রুবক
  {
    hb_set_t visited_tables, visited_conditions;
    জন্য (ধ্রুবক LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      যদি (visited_tables.has (offset)) চালু;
      visited_tables.add (offset);
      যদি (unlikely (visited_tables.in_error ())) ফেরত মিথ্যা;

      ধ্রুবক FeatureLookupsTable &feature_lookups = এই+record.featureLookups;
      যদি (unlikely (!feature_lookups.collect_conditions (এই, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	ফেরত মিথ্যা;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    ফেরত সত্য;
  }

  যৌক্তিক get_lookup_variations (unsigned feature_সূচক,
                              ধ্রুবক Feature &feature,
                              hb_sorted_array_t<ধ্রুবক unsigned> conditions,
                              hb_set_t *lookup_indexes) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupVariationRecord &record : get_lookup_variation_records ())
    {
      যদি (record.feature_সূচক_2 != feature_সূচক) চালু;

      record.collect_lookups (এই, feature, conditions, lookup_indexes);
      ফেরত সত্য;
    }
    ফেরত মিথ্যা;
  }

  শূন্য collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = varRecords.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, এই);
      যদি (c->universal)
        বিরতি;
    }
    যদি (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed (*এই));
  }

  শূন্য collect_lookups (ধ্রুবক hb_set_t *feature_indexes,
			ধ্রুবক hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) ধ্রুবক
  {
    unsigned গুনতি = varRecords.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        চালু;
      varRecords[i].collect_lookups (এই, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  শূন্য collect_name_ids (ধ্রুবক hb_map_t *feature_সূচক_map,
                         ধ্রুবক hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < varRecords.len; i++)
    {
      যদি (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        চালু;
      varRecords[i].collect_name_ids (এই, feature_সূচক_map, get_tag, nameids_to_retain);
    }
  }

  শূন্য collect_lookup_variation_lookups (ধ্রুবক hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (এই, feature_indexes, lookup_indexes);
  }

  শূন্য closure_features (ধ্রুবক hb_map_t *lookup_indexes,
			 ধ্রুবক hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = varRecords.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        চালু;
      varRecords[i].closure_features (এই, lookup_indexes, feature_indexes);
    }

    জন্য (ধ্রুবক LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (এই, lookup_indexes, feature_indexes);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    জন্য (ধ্রুবক FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, এই);
    জন্য (ধ্রুবক LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, এই);
  }

  শূন্য collect_feature_indices (hb_set_t *feature_indices) ধ্রুবক
  {
    জন্য (ধ্রুবক FeatureVariationRecord &record : varRecords)
      (এই+record.substitutions).collect_feature_indices (feature_indices);
  }

  যৌক্তিক has_lookup_variations (unsigned feature_সূচক) ধ্রুবক
  {
    জন্য (ধ্রুবক LookupVariationRecord &record : get_lookup_variation_records ())
    {
      যদি (record.feature_সূচক_2 == feature_সূচক)
	ফেরত সত্য;
      যদি (record.feature_সূচক_2 > feature_সূচক)
	বিরতি;
    }
    ফেরত মিথ্যা;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিথ্যা);

    out->version.major = version.major;
    out->version.minor = version.minor;

    পূর্ণসংখ্যা keep_up_to = -1;
    জন্য (পূর্ণসংখ্যা i = varRecords.len - 1; i >= 0; i--) {
      যদি (varRecords[i].intersects_features (এই, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        বিরতি;
      }
    }

    unsigned গুনতি = (unsigned) (keep_up_to + 1);
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        চালু;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), এই) (varRecords[i]);
    }

    যদি (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      যৌক্তিক ঢোকাও_catch_all_record = সত্য;
      subset_record_array (l, &(out->varRecords), এই, ঢোকাও_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    যদি (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      যদি (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (মিথ্যা);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, এই))
      ;
    }

    return_trace (যৌক্তিক (out->varRecords) ||
		  (out_lookup_records && যৌক্তিক (*out_lookup_records)));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক FeatureList *feature_list) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, এই, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, এই)));
  }

  সুরক্ষিত:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  সর্বজনীন:
  DEFINE_আকার_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

কাঠামো HintingDevice
{
  friend কাঠামো Device;

  ব্যক্তিগত:

  hb_position_t get_x_delta (hb_font_t *font) ধ্রুবক
  { ফেরত get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ধ্রুবক
  { ফেরত get_delta (font->y_ppem, font->y_scale); }

  সর্বজনীন:

  size_t get_আকার () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা f = deltaFormat;
    যদি (unlikely (f < 1 || f > 3 || start_আকার > end_আকার)) ফেরত 3 * HBUINT16::static_আকার;
    ফেরত HBUINT16::static_আকার * (4 + ((end_আকার - start_আকার) >> (4 - f)));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && c->check_range (এই, এই->get_আকার ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed<HintingDevice> (এই));
  }

  ব্যক্তিগত:

  পূর্ণসংখ্যা get_delta (unsigned পূর্ণসংখ্যা ppem, পূর্ণসংখ্যা scale) ধ্রুবক
  {
    যদি (!ppem) ফেরত 0;

    পূর্ণসংখ্যা pixels = get_delta_pixels (ppem);

    যদি (!pixels) ফেরত 0;

    ফেরত (পূর্ণসংখ্যা) (pixels * (int64_t) scale / ppem);
  }
  পূর্ণসংখ্যা get_delta_pixels (unsigned পূর্ণসংখ্যা ppem_আকার) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা f = deltaFormat;
    যদি (unlikely (f < 1 || f > 3))
      ফেরত 0;

    যদি (ppem_আকার < start_আকার || ppem_আকার > end_আকার)
      ফেরত 0;

    unsigned পূর্ণসংখ্যা s = ppem_আকার - start_আকার;

    unsigned পূর্ণসংখ্যা byte = delta_মান_Z[s >> (4 - f)];
    unsigned পূর্ণসংখ্যা bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned পূর্ণসংখ্যা mask = (0xFFFFu >> (16 - (1 << f)));

    পূর্ণসংখ্যা delta = bits & mask;

    যদি ((unsigned পূর্ণসংখ্যা) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    ফেরত delta;
  }

  সুরক্ষিত:
  HBUINT16	start_আকার;		/* Smallest size to correct--in ppem */
  HBUINT16	end_আকার;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_মান_Z;		/* Array of compressed data */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, delta_মান_Z);
};

কাঠামো VariationDevice
{
  friend কাঠামো Device;

  ব্যক্তিগত:

  hb_position_t get_x_delta (hb_font_t *font,
			     ধ্রুবক ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  { ফেরত !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ধ্রুবক ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  { ফেরত !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  দশমিক get_x_delta_f (hb_font_t *font,
		       ধ্রুবক ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ধ্রুবক
  { ফেরত !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  দশমিক get_y_delta_f (hb_font_t *font,
		       ধ্রুবক ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ধ্রুবক
  { ফেরত !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *layout_variation_idx_delta_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    যদি (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, পূর্ণসংখ্যা> *v;
    যদি (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    যদি (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
      return_trace (nullptr);

    c->start_zerocopy (এই->static_আকার);
    auto *out = c->embed (এই);
    যদি (unlikely (!out)) return_trace (nullptr);

    যদি (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  শূন্য collect_variation_সূচক (hb_collect_variation_indices_context_t *c) ধ্রুবক
  { c->layout_variation_indices->add (varIdx); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  ব্যক্তিগত:

  দশমিক get_delta (hb_font_t *font,
		   ধ্রুবক ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
    ফেরত store.get_delta (varIdx, font->coords, font->অঙ্ক_coords, store_cache);
  }

  সুরক্ষিত:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো DeviceHeader
{
  সুরক্ষিত:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  সর্বজনীন:
  HBUINT16		format;		/* Format identifier */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  দশমিক get_x_delta_f (hb_font_t *font,
		       ধ্রুবক ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
#ifndef HB_NO_VAR
    যদি (u.b.format == 0x8000)
    {
      hb_barrier ();
      ফেরত u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    ফেরত get_x_delta (font, store, store_cache);
  }

  দশমিক get_y_delta_f (hb_font_t *font,
		       ধ্রুবক ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
#ifndef HB_NO_VAR
    যদি (u.b.format == 0x8000)
    {
      hb_barrier ();
      ফেরত u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    ফেরত get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ধ্রুবক ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
    নির্বাচন (u.b.format)
    {
#ifndef HB_NO_HINTING
    ক্ষেত্র 1: ক্ষেত্র 2: ক্ষেত্র 3:
      hb_barrier ();
      ফেরত u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    ক্ষেত্র 0x8000:
      hb_barrier ();
      ফেরত u.variation.get_x_delta (font, store, store_cache);
#endif
    পূর্বনির্ধারিত:
      ফেরত 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ধ্রুবক ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
    নির্বাচন (u.b.format)
    {
    ক্ষেত্র 1: ক্ষেত্র 2: ক্ষেত্র 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      ফেরত u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    ক্ষেত্র 0x8000:
      hb_barrier ();
      ফেরত u.variation.get_y_delta (font, store, store_cache);
#endif
    পূর্বনির্ধারিত:
      ফেরত 0;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.b.format.sanitize (c)) return_trace (মিথ্যা);
    নির্বাচন (u.b.format) {
#ifndef HB_NO_HINTING
    ক্ষেত্র 1: ক্ষেত্র 2: ক্ষেত্র 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    ক্ষেত্র 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    পূর্বনির্ধারিত:
      return_trace (সত্য);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *layout_variation_idx_delta_map=nullptr) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    নির্বাচন (u.b.format) {
#ifndef HB_NO_HINTING
    ক্ষেত্র 1:
    ক্ষেত্র 2:
    ক্ষেত্র 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    ক্ষেত্র 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    পূর্বনির্ধারিত:
      return_trace (nullptr);
    }
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    নির্বাচন (u.b.format) {
#ifndef HB_NO_HINTING
    ক্ষেত্র 1:
    ক্ষেত্র 2:
    ক্ষেত্র 3:
      ফেরত;
#endif
#ifndef HB_NO_VAR
    ক্ষেত্র 0x8000:
      hb_barrier ();
      u.variation.collect_variation_সূচক (c);
      ফেরত;
#endif
    পূর্বনির্ধারিত:
      ফেরত;
    }
  }

  unsigned get_variation_সূচক () ধ্রুবক
  {
    নির্বাচন (u.b.format) {
#ifndef HB_NO_VAR
    ক্ষেত্র 0x8000:
      hb_barrier ();
      ফেরত u.variation.varIdx;
#endif
    পূর্বনির্ধারিত:
      ফেরত HB_OT_LAYOUT_NO_VARIATIONS_সূচক;
    }
  }

  যৌক্তিক is_variation_device () ধ্রুবক
  {
    নির্বাচন (u.b.format) {
#ifndef HB_NO_VAR
    ক্ষেত্র 0x8000:
      ফেরত সত্য;
#endif
    পূর্বনির্ধারিত:
      ফেরত মিথ্যা;
    }
  }

  সুরক্ষিত:
  সংঘ {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

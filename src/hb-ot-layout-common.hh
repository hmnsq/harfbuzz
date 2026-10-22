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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-ot-layout.hh"
#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-set.hh"
#अन्तर्भाव "hb-bimap.hh"
#अन्तर्भाव "hb-cache.hh"
#अन्तर्भाव "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (परिमाण (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_द्विमान_cache_t = hb_cache_t<14, 1, 8>;
static_assert (परिमाण (hb_ot_layout_द्विमान_cache_t) == 256, "");

#अन्तर्भाव "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


नामाकाश OT {

template<typename Iterator>
स्थिर अन्तःस्थ तार्किक ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

स्थिर तार्किक ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ध्रुव hb_set_t &klasses,
    तार्किक use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

संरचना ItemVarStoreInstancer;

संरचना hb_collect_feature_substitutes_with_var_context_t
{
  ध्रुव hb_map_t *axes_index_tag_map;
  ध्रुव hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, ध्रुव Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  तार्किक apply;
  तार्किक variation_applied;
  तार्किक universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  तार्किक all_axes_pinned;
};

संरचना hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ध्रुव शून्य         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              ध्रुव hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_गणनम् (0),langsys_feature_गणनम् (0) {}

  तार्किक visitScript ()
  { पटिनिवत्त script_गणनम्++ < HB_MAX_SCRIPTS; }

  तार्किक visitLangsys (unsigned feature_गणनम्_2)
  {
    langsys_feature_गणनम् += feature_गणनम्_2;
    पटिनिवत्त langsys_feature_गणनम् < HB_MAX_LANGSYS_FEATURE_गणनम्;
  }

  सार्वजनिक:
  ध्रुव शून्य *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ध्रुव hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  गुप्त:
  unsigned script_गणनम्;
  unsigned langsys_feature_गणनम्;
};

संरचना hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  ध्रुव वर्ण *get_name () { पटिनिवत्त "SUBSET_LAYOUT"; }
  स्थिर return_t default_return_मानम् () { पटिनिवत्त hb_empty_t (); }

  तार्किक visitScript ()
  {
    पटिनिवत्त script_गणनम्++ < HB_MAX_SCRIPTS;
  }

  तार्किक visitLangSys ()
  {
    पटिनिवत्त langsys_गणनम्++ < HB_MAX_LANGSYS;
  }

  तार्किक visitFeatureIndex (पूर्णाङ्क गणनम्)
  {
    feature_index_गणनम् += गणनम्;
    पटिनिवत्त feature_index_गणनम् < HB_MAX_FEATURE_INDICES;
  }

  तार्किक visitLookupIndex()
  {
    lookup_index_गणनम्++;
    पटिनिवत्त lookup_index_गणनम् < HB_MAX_LOOKUP_VISIT_गणनम्;
  }

  hb_subset_context_t *subset_context;
  ध्रुव hb_tag_t table_tag;
  ध्रुव hb_map_t *lookup_index_map;
  ध्रुव hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ध्रुव hb_map_t *feature_index_map;
  ध्रुव hb_map_t *feature_map_w_duplicates;
  ध्रुव hb_hashmap_t<unsigned, ध्रुव Feature*> *feature_substitutes_map;
  ध्रुव hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  ध्रुव hb_set_t *catch_all_record_feature_idxes;
  ध्रुव hb_hashmap_t<unsigned, hb_pair_t<ध्रुव शून्य*, ध्रुव शून्य*>> *feature_idx_tag_map;

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
				script_गणनम् (0),
				langsys_गणनम् (0),
				feature_index_गणनम् (0),
				lookup_index_गणनम् (0)
  {
    सचे (tag_ == HB_OT_TAG_GSUB)
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
    अञ्ञथा
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

  गुप्त:
  unsigned script_गणनम्;
  unsigned langsys_गणनम्;
  unsigned feature_index_गणनम्;
  unsigned lookup_index_गणनम्;
};

संरचना ItemVariationStore;
संरचना hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (ध्रुव T &obj) { obj.collect_variation_indices (एतत्); पटिनिवत्त hb_empty_t (); }
  स्थिर return_t default_return_मानम् () { पटिनिवत्त hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  ध्रुव hb_set_t *glyph_set;
  ध्रुव hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, पूर्णाङ्क>> *condition_मानानि = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  ध्रुव hb_set_t *glyph_set_,
					  ध्रुव hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
संरचना subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 ध्रुव शून्य *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  तार्किक operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    सचे (unlikely (!o)) पटिनिवत्त मिच्छा;
    तार्किक ret = o->serialize_subset (subset_context, offset, base);
    सचे (!ret)
    {
      out.निष्कासय ();
      subset_context->serializer->revert (snap);
    }
    पटिनिवत्त ret;
  }

  गुप्त:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ध्रुव शून्य *base;
};


template<typename OutputArray, typename Arg>
संरचना subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     ध्रुव शून्य *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  तार्किक operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    सचे (unlikely (!o)) पटिनिवत्त मिच्छा;
    तार्किक ret = o->serialize_subset (subset_context, offset, base, arg);
    सचे (!ret)
    {
      out.निष्कासय ();
      subset_context->serializer->revert (snap);
    }
    पटिनिवत्त ret;
  }

  गुप्त:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ध्रुव शून्य *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
संरचना
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ध्रुव शून्य *base) ध्रुव
  { पटिनिवत्त subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ध्रुव शून्य *base, Arg &&arg) ध्रुव
  { पटिनिवत्त subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
संरचना subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 ध्रुव शून्य *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  शून्य
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    तार्किक ret = record.subset (subset_layout_context, base);
    सचे (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    अञ्ञथा out->len++;
  }

  गुप्त:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ध्रुव शून्य *base;
};

template<typename OutputArray, typename Arg>
संरचना subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     ध्रुव शून्य *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  शून्य
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    तार्किक ret = record.subset (subset_layout_context, base, arg);
    सचे (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    अञ्ञथा out->len++;
  }

  गुप्त:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ध्रुव शून्य *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
संरचना
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       ध्रुव शून्य *base) ध्रुव
  { पटिनिवत्त subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               ध्रुव शून्य *base, Arg &&arg) ध्रुव
  { पटिनिवत्त subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
संरचना serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         ध्रुव शून्य *base_,
                         ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  तार्किक operator () (T&& record)
  {
    सचे (!serialize_context->copy (record, base, variation_map)) पटिनिवत्त मिच्छा;
    out.len++;
    पटिनिवत्त सच्च;
  }

  गुप्त:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  ध्रुव शून्य *base;
  ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
संरचना
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               ध्रुव शून्य *base,
               ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *variation_map) ध्रुव
  { पटिनिवत्त serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

संरचना IndexArray : Array16Of<Index>
{
  तार्किक intersects (ध्रुव hb_map_t *indexes) ध्रुव
  { पटिनिवत्त hb_any (*एतत्, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    सचे (!it) पटिनिवत्त;
    सचे (unlikely (!c->extend_min ((*एतत्)))) पटिनिवत्त;

    अत्थाय (ध्रुव auto _ : it)
    {
      सचे (!l->visitLookupIndex()) भङ्ग;

      Index i;
      i = _;
      c->copy (i);
      एतत्->len++;
    }
  }

  unsigned पूर्णाङ्क get_indexes (unsigned पूर्णाङ्क start_offset,
			    unsigned पूर्णाङ्क *गणनम्_2 /* IN/OUT */,
			    unsigned पूर्णाङ्क *_indexes /* OUT */) ध्रुव
  {
    सचे (गणनम्_2 && _indexes)
    {
      + एतत्->as_array ().sub_array (start_offset, गणनम्_2)
      | hb_sink (hb_array (_indexes, *गणनम्_2))
      ;
    }
    पटिनिवत्त एतत्->len;
  }

  शून्य add_indexes_to (hb_set_t* output /* OUT */) ध्रुव
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
संरचना FeatureParamsSize
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!c->check_struct (एतत्))) return_trace (मिच्छा);
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

    सचे (!designSize)
      return_trace (मिच्छा);
    अञ्ञथा सचे (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (सच्च);
    अञ्ञथा सचे (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (मिच्छा);
    अञ्ञथा
      return_trace (सच्च);
  }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ध्रुव
  { nameids_to_retain->add (subfamilyNameID); }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    return_trace ((तार्किक) c->serializer->embed (*एतत्));
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
संरचना FeatureParamsStylisticSet
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (एतत्));
  }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ध्रुव
  { nameids_to_retain->add (uiNameID); }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    return_trace ((तार्किक) c->serializer->embed (*एतत्));
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
संरचना FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_गणनम्, hb_codepoint_t *chars) ध्रुव
  {
    सचे (char_गणनम् && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_गणनम्)
      | hb_sink (hb_array (chars, *char_गणनम्))
      ;
    }
    पटिनिवत्त characters.len;
  }

  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ध्रुव
  {
    सचे (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    सचे (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    सचे (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    सचे (!firstParamUILabelNameID || !अङ्कः_Named_Parameters || अङ्कः_Named_Parameters >= 0x7FFF)
      पटिनिवत्त;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) अङ्कः_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    return_trace ((तार्किक) c->serializer->embed (*एतत्));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
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
  HBUINT16	अङ्कः_Named_Parameters;	/* Number of named parameters. (May
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
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (14, characters);
};

संरचना FeatureParams
{
  तार्किक sanitize (hb_sanitize_context_t *c, hb_tag_t tag) ध्रुव
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    पटिनिवत्त सच्च;
#endif
    TRACE_SANITIZE (एतत्);
    सचे (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    सचे ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    सचे ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (सच्च);
  }

  शून्य collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ध्रुव
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    पटिनिवत्त;
#endif
    सचे (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      पटिनिवत्त (u.size.collect_name_ids (nameids_to_retain));
    सचे ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      पटिनिवत्त (u.stylisticSet.collect_name_ids (nameids_to_retain));
    सचे ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      पटिनिवत्त (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  तार्किक subset (hb_subset_context_t *c, ध्रुव Tag* tag) ध्रुव
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    पटिनिवत्त मिच्छा;
#endif
    TRACE_SUBSET (एतत्);
    सचे (!tag) return_trace (मिच्छा);
    सचे (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    सचे ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    सचे ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (मिच्छा);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  ध्रुव FeatureParamsSize& get_size_params (hb_tag_t tag) ध्रुव
  {
    सचे (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      पटिनिवत्त u.size;
    पटिनिवत्त Null (FeatureParamsSize);
  }
  ध्रुव FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) ध्रुव
  {
    सचे ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      पटिनिवत्त u.stylisticSet;
    पटिनिवत्त Null (FeatureParamsStylisticSet);
  }
  ध्रुव FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) ध्रुव
  {
    सचे ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      पटिनिवत्त u.characterVariants;
    पटिनिवत्त Null (FeatureParamsCharacterVariants);
  }
#endif

  गुप्त:
  सङ्घ {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (0);
};

संरचना Record_sanitize_closure_t {
  hb_tag_t tag;
  ध्रुव शून्य *list_base;
};

संरचना Feature
{
  unsigned पूर्णाङ्क get_lookup_गणनम् () ध्रुव
  { पटिनिवत्त lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त lookupIndex[i]; }
  unsigned पूर्णाङ्क get_lookup_indexes (unsigned पूर्णाङ्क start_index,
				   unsigned पूर्णाङ्क *lookup_गणनम् /* IN/OUT */,
				   unsigned पूर्णाङ्क *lookup_tags /* OUT */) ध्रुव
  { पटिनिवत्त lookupIndex.get_indexes (start_index, lookup_गणनम्, lookup_tags); }
  शून्य add_lookup_indexes_to (hb_set_t *lookup_indexes) ध्रुव
  { lookupIndex.add_indexes_to (lookup_indexes); }

  ध्रुव FeatureParams &get_feature_params () ध्रुव
  { पटिनिवत्त एतत्+featureParams; }

  तार्किक intersects_lookup_indexes (ध्रुव hb_map_t *lookup_indexes) ध्रुव
  { पटिनिवत्त lookupIndex.intersects (lookup_indexes); }

  शून्य collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ध्रुव
  {
    सचे (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  तार्किक subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ध्रुव Tag                   *tag = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    out->featureParams.serialize_subset (c, featureParams, एतत्, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    सचे (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    अञ्ञथा
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
    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव Record_sanitize_closure_t *closure = nullptr) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  featureParams.sanitize (c, एतत्, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
संरचना Record
{
  पूर्णाङ्क cmp (hb_tag_t a) ध्रुव { पटिनिवत्त tag.cmp (a); }

  तार्किक subset (hb_subset_layout_context_t *c, ध्रुव शून्य *base, ध्रुव शून्य *f_sub = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->subset_context->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);

    सचे (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    ध्रुव Feature& f = *reinterpret_cast<ध्रुव Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    तार्किक ret = f.subset (c->subset_context, c, &tag);
    सचे (ret)
      s->add_link (out->offset, s->निष्कासय_pack ());
    अञ्ञथा
      s->निष्कासय_discard ();

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    ध्रुव Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (एतत्) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
संरचना RecordArrayOf : SortedArray16Of<Record<Type>>
{
  ध्रुव Offset16To<Type>& get_offset (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त (*एतत्)[i].offset; }
  Offset16To<Type>& get_offset (unsigned पूर्णाङ्क i)
  { पटिनिवत्त (*एतत्)[i].offset; }
  ध्रुव Tag& get_tag (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त (*एतत्)[i].tag; }
  unsigned पूर्णाङ्क get_tags (unsigned पूर्णाङ्क start_offset,
                         unsigned पूर्णाङ्क *record_गणनम् /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) ध्रुव
  {
    सचे (record_गणनम् && record_tags)
    {
      + एतत्->as_array ().sub_array (start_offset, record_गणनम्)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_गणनम्))
      ;
    }
    पटिनिवत्त एतत्->len;
  }
  तार्किक find_index (hb_tag_t tag, unsigned पूर्णाङ्क *index) ध्रुव
  {
    पटिनिवत्त एतत्->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
संरचना RecordListOf : RecordArrayOf<Type>
{
  ध्रुव Type& operator [] (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त एतत्+एतत्->get_offset (i); }

  तार्किक subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    + एतत्->iter ()
    | hb_apply (subset_record_array (l, out, एतत्))
    ;
    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (RecordArrayOf<Type>::sanitize (c, एतत्));
  }
};

संरचना RecordListOfFeature : RecordListOf<Feature>
{
  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    + hb_enumerate (*एतत्)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, एतत्] (ध्रुव hb_pair_t<unsigned, ध्रुव Record<Feature>&>& _)
                {
                  ध्रुव Feature *f_sub = nullptr;
                  ध्रुव Feature **f = nullptr;
                  सचे (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, एतत्, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (सच्च);
  }
};

प्रकारनिर्देश RecordListOf<Feature> FeatureList;


संरचना LangSys
{
  unsigned पूर्णाङ्क get_feature_गणनम् () ध्रुव
  { पटिनिवत्त featureIndex.len; }
  hb_tag_t get_feature_index (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त featureIndex[i]; }
  unsigned पूर्णाङ्क get_feature_indexes (unsigned पूर्णाङ्क start_offset,
				    unsigned पूर्णाङ्क *feature_गणनम्_2 /* IN/OUT */,
				    unsigned पूर्णाङ्क *feature_indexes /* OUT */) ध्रुव
  { पटिनिवत्त featureIndex.get_indexes (start_offset, feature_गणनम्_2, feature_indexes); }
  शून्य add_feature_indexes_to (hb_set_t *feature_indexes) ध्रुव
  { featureIndex.add_indexes_to (feature_indexes); }

  तार्किक has_required_feature () ध्रुव { पटिनिवत्त reqFeatureIndex != 0xFFFFu; }
  unsigned पूर्णाङ्क get_required_feature_index () ध्रुव
  {
    सचे (reqFeatureIndex == 0xFFFFu)
      पटिनिवत्त Index::NOT_FOUND_INDEX;
   पटिनिवत्त reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (*एतत्));
  }

  तार्किक compare (ध्रुव LangSys& o, ध्रुव hb_map_t *feature_index_map) ध्रुव
  {
    सचे (reqFeatureIndex != o.reqFeatureIndex)
      पटिनिवत्त मिच्छा;

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

    अत्थाय (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      सचे (a != b) पटिनिवत्त मिच्छा;
    }

    सचे (iter || o_iter) पटिनिवत्त मिच्छा;

    पटिनिवत्त सच्च;
  }

  शून्य collect_features (hb_prune_langsys_context_t *c) ध्रुव
  {
    सचे (!has_required_feature () && !get_feature_गणनम् ()) पटिनिवत्त;
    सचे (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  तार्किक subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ध्रुव Tag                  *tag = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    ध्रुव uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    सचे (!l->visitFeatureIndex (featureIndex.len))
      return_trace (मिच्छा);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    तार्किक ret = तार्किक (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव Record_sanitize_closure_t * = nullptr) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

संरचना Script
{
  unsigned पूर्णाङ्क get_lang_sys_गणनम् () ध्रुव
  { पटिनिवत्त langSys.len; }
  ध्रुव Tag& get_lang_sys_tag (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त langSys.get_tag (i); }
  unsigned पूर्णाङ्क get_lang_sys_tags (unsigned पूर्णाङ्क start_offset,
				  unsigned पूर्णाङ्क *lang_sys_गणनम् /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ध्रुव
  { पटिनिवत्त langSys.get_tags (start_offset, lang_sys_गणनम्, lang_sys_tags); }
  ध्रुव LangSys& get_lang_sys (unsigned पूर्णाङ्क i) ध्रुव
  {
    सचे (i == Index::NOT_FOUND_INDEX) पटिनिवत्त get_default_lang_sys ();
    पटिनिवत्त एतत्+langSys[i].offset;
  }
  तार्किक find_lang_sys_index (hb_tag_t tag, unsigned पूर्णाङ्क *index) ध्रुव
  { पटिनिवत्त langSys.find_index (tag, index); }

  तार्किक has_default_lang_sys () ध्रुव           { पटिनिवत्त defaultLangSys != 0; }
  ध्रुव LangSys& get_default_lang_sys () ध्रुव { पटिनिवत्त एतत्+defaultLangSys; }

  शून्य prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) ध्रुव
  {
    सचे (!has_default_lang_sys () && !get_lang_sys_गणनम् ()) पटिनिवत्त;
    सचे (!c->visitScript ()) पटिनिवत्त;

    सचे (!c->script_langsys_map->has (script_index))
    {
      सचे (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	पटिनिवत्त;
    }

    सचे (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ध्रुव LangSys& d = get_default_lang_sys ();
      सचे (c->visitLangsys (d.get_feature_गणनम् ())) {
        d.collect_features (c);
      }

      अत्थाय (auto _ : + hb_enumerate (langSys))
      {
        ध्रुव LangSys& l = एतत्+_.second.offset;
        सचे (!c->visitLangsys (l.get_feature_गणनम् ())) अनुवर्त;
        सचे (l.compare (d, c->duplicate_feature_map)) अनुवर्त;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    अञ्ञथा
    {
      अत्थाय (auto _ : + hb_enumerate (langSys))
      {
        ध्रुव LangSys& l = एतत्+_.second.offset;
        सचे (!c->visitLangsys (l.get_feature_गणनम् ())) अनुवर्त;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  तार्किक subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ध्रुव Tag                   *tag) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    सचे (!l->visitScript ()) return_trace (मिच्छा);
    सचे (tag && !c->plan->layout_scripts.has (*tag))
      पटिनिवत्त मिच्छा;

    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    तार्किक defaultLang = मिच्छा;
    सचे (has_default_lang_sys ())
    {
      c->serializer->push ();
      ध्रुव LangSys& ls = एतत्+defaultLangSys;
      तार्किक ret = ls.subset (c, l);
      सचे (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->निष्कासय_discard ();
	out->defaultLangSys = 0;
      }
      अञ्ञथा
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->निष्कासय_pack ());
	defaultLang = सच्च;
      }
    }

    ध्रुव hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    सचे (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (ध्रुव Record<LangSys>& record) {पटिनिवत्त l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), एतत्))
      ;
    }

    return_trace (तार्किक (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव Record_sanitize_closure_t * = nullptr) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (defaultLangSys.sanitize (c, एतत्) && langSys.sanitize (c, एतत्));
  }

  रक्षित:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

संरचना RecordListOfScript : RecordListOf<Script>
{
  तार्किक subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    अत्थाय (auto _ : + hb_enumerate (*एतत्))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      तार्किक ret = _.second.subset (l, एतत्);
      सचे (!ret) c->serializer->revert (snap);
      अञ्ञथा out->len++;
    }

    return_trace (सच्च);
  }
};

प्रकारनिर्देश RecordListOfScript ScriptList;



संरचना LookupFlag : HBUINT16
{
  गणना Flags {
    दक्षिणः_To_वामः		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
नामाकाश OT {

संरचना Lookup
{
  unsigned पूर्णाङ्क get_subtable_गणनम् () ध्रुव { पटिनिवत्त subTable.len; }

  template <typename TSubTable>
  ध्रुव Array16OfOffset16To<TSubTable>& get_subtables () ध्रुव
  { पटिनिवत्त reinterpret_cast<ध्रुव Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { पटिनिवत्त reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  ध्रुव TSubTable& get_subtable (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त एतत्+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned पूर्णाङ्क i)
  { पटिनिवत्त एतत्+get_subtables<TSubTable> ()[i]; }

  size_t get_size () ध्रुव
  {
    ध्रुव HBUINT16 &markFilteringSet = StructAfter<ध्रुव HBUINT16> (subTable);
    सचे (lookupFlag & LookupFlag::UseMarkFilteringSet)
      पटिनिवत्त (ध्रुव वर्ण *) &StructAfter<ध्रुव वर्ण> (markFilteringSet) - (ध्रुव वर्ण *) एतत्;
    पटिनिवत्त (ध्रुव वर्ण *) &markFilteringSet - (ध्रुव वर्ण *) एतत्;
  }

  unsigned पूर्णाङ्क get_type () ध्रुव { पटिनिवत्त lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ध्रुव
  {
    unsigned पूर्णाङ्क flag = lookupFlag;
    सचे (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      ध्रुव HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    पटिनिवत्त flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    unsigned पूर्णाङ्क lookup_type = get_type ();
    TRACE_DISPATCH (एतत्, lookup_type);
    unsigned पूर्णाङ्क गणनम् = get_subtable_गणनम् ();
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      सचे (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_मानम् ());
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  unsigned पूर्णाङ्क lookup_type,
		  uint32_t lookup_props,
		  unsigned पूर्णाङ्क अङ्कः_subtables)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    सचे (unlikely (!subTable.serialize (c, अङ्कः_subtables))) return_trace (मिच्छा);
    सचे (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      सचे (unlikely (!c->extend (एतत्))) return_trace (मिच्छा);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (सच्च);
  }

  template <typename TSubTable>
  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    ध्रुव hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned पूर्णाङ्क lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([एतत्, glyphset, lookup_type] (ध्रुव Offset16To<TSubTable> &_) { पटिनिवत्त (एतत्+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), एतत्, lookup_type))
    ;

    सचे (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ध्रुव HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      सचे (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      अञ्ञथा
      {
        सचे (unlikely (!c->serializer->extend (out))) return_trace (मिच्छा);
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
    return_trace (सच्च);
  }

  template <typename TSubTable>
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!(c->check_struct (एतत्) && subTable.sanitize (c))) return_trace (मिच्छा);
    hb_barrier ();

    unsigned subtables = get_subtable_गणनम् ();
    सचे (unlikely (!c->visit_subtables (subtables))) return_trace (मिच्छा);

    सचे (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ध्रुव HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      सचे (!markFilteringSet.sanitize (c)) return_trace (मिच्छा);
    }

    सचे (unlikely (!get_subtables<TSubTable> ().sanitize (c, एतत्, get_type ())))
      return_trace (मिच्छा);

    सचे (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned पूर्णाङ्क type = get_subtable<TSubTable> (0).u.extension.get_type ();
      अत्थाय (unsigned पूर्णाङ्क i = 1; i < subtables; i++)
	सचे (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (मिच्छा);
    }
    return_trace (सच्च);
  }

  रक्षित:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
संरचना LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  तार्किक subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    + hb_enumerate (*एतत्)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, एतत्))
    ;
    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, एतत्)));
  }
};


/*
 * Coverage Table
 */


स्थिर तार्किक ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ध्रुव hb_set_t &klasses,
                                          तार्किक use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  सचे (!klass_map)
    पटिनिवत्त ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  सचे (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  अत्थाय (ध्रुव unsigned k: klasses)
  {
    सचे (klass_map->has (k)) अनुवर्त;
    klass_map->set (k, idx);
    idx++;
  }


  सचे (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    पटिनिवत्त मिच्छा;

  अत्थाय (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  पटिनिवत्त ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
संरचना ClassDefFormat1_3
{
  friend संरचना ClassDef;

  गुप्त:
  unsigned पूर्णाङ्क get_class (hb_codepoint_t glyph_id) ध्रुव
  {
    पटिनिवत्त class_मानम्[(unsigned पूर्णाङ्क) (glyph_id - startGlyph)];
  }

  unsigned get_population () ध्रुव
  {
    पटिनिवत्त class_मानम्.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);

    सचे (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_मानम्.len = 0;
      return_trace (सच्च);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_गणनम् = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    सचे (unlikely (!class_मानम्.serialize (c, glyph_गणनम्))) return_trace (मिच्छा);
    अत्थाय (ध्रुव hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_मानम्[idx] = gid_klass_pair.second;
    }
    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सच्च,
               तार्किक use_class_zero = सच्च,
               ध्रुव Coverage* glyph_filter = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_मानम्.len;

    अत्थाय (ध्रुव hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      सचे (new_gid == HB_MAP_मानम्_INVALID) अनुवर्त;
      सचे (glyph_filter && !glyph_filter->has(gid)) अनुवर्त;

      unsigned klass = class_मानम्[gid - start];
      सचे (!klass) अनुवर्त;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    सचे (use_class_zero)
    {
      ध्रुव hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_गणनम् = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_गणनम् <= glyph_and_klass.length;
    }
    सचे (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (मिच्छा);
    return_trace (keep_empty_table || (तार्किक) glyph_and_klass);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && class_मानम्.sanitize (c));
  }

  unsigned cost () ध्रुव { पटिनिवत्त 1; }

  template <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) ध्रुव
  {
    unsigned पूर्णाङ्क start = 0;
    unsigned पूर्णाङ्क गणनम् = class_मानम्.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      सचे (class_मानम्[i])
	अनुवर्त;

      सचे (start != i)
	सचे (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  पटिनिवत्त मिच्छा;

      start = i + 1;
    }
    सचे (start != गणनम्)
      सचे (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + गणनम्)))
	पटिनिवत्त मिच्छा;

    पटिनिवत्त सच्च;
  }

  template <typename set_t>
  तार्किक collect_class (set_t *glyphs, unsigned klass) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = class_मानम्.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (class_मानम्[i] == klass) glyphs->add (startGlyph + i);
    पटिनिवत्त सच्च;
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_मानम्.len;
    अत्थाय (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      सचे (class_मानम्[iter - start]) पटिनिवत्त सच्च;
    पटिनिवत्त मिच्छा;
  }
  तार्किक intersects_class (ध्रुव hb_set_t *glyphs, unsigned klass) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = class_मानम्.len;
    सचे (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_मानम्_INVALID;
      सचे (!glyphs->next (&g)) पटिनिवत्त मिच्छा;
      सचे (g < startGlyph) पटिनिवत्त सच्च;
      g = startGlyph + गणनम् - 1;
      सचे (glyphs->next (&g)) पटिनिवत्त सच्च;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ध्रुव typename Types::HBUINT *arr = class_मानम्.arrayZ;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (arr[i] == klass && glyphs->has (startGlyph + i))
	पटिनिवत्त सच्च;
    पटिनिवत्त मिच्छा;
  }

  शून्य intersected_class_glyphs (ध्रुव hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ध्रुव
  {
    unsigned गणनम् = class_मानम्.len;
    सचे (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      अत्थाय (uint32_t g = HB_SET_मानम्_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      अत्थाय (uint32_t g = startGlyph + गणनम् - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      पटिनिवत्त;
    }

    अत्थाय (unsigned i = 0; i < गणनम्; i++)
      सचे (class_मानम्[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#सचे 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + गणनम्;
    अत्थाय (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      सचे (class_मानम्.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  शून्य intersected_classes (ध्रुव hb_set_t *glyphs, hb_set_t *intersect_classes) ध्रुव
  {
    सचे (glyphs->is_empty ()) पटिनिवत्त;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    सचे (!class_मानम्.len)
    {
      intersect_classes->add (0);
      पटिनिवत्त;
    }

    hb_codepoint_t end_glyph = startGlyph + class_मानम्.len - 1;
    सचे (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    अत्थाय (ध्रुव auto& _ : + hb_enumerate (class_मानम्))
    {
      hb_codepoint_t g = startGlyph + _.first;
      सचे (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  रक्षित:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_मानम्;	/* Array of Class Values--one per GlyphID */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_मानम्);
};

template <typename Types>
संरचना ClassDefFormat2_4
{
  friend संरचना ClassDef;

  गुप्त:
  unsigned पूर्णाङ्क get_class (hb_codepoint_t glyph_id) ध्रुव
  {
    पटिनिवत्त rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () ध्रुव
  {
    typename Types::large_int ret = 0;
    अत्थाय (ध्रुव auto &r : rangeRecord)
      ret += r.get_population ();
    पटिनिवत्त ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);

    सचे (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (सच्च);
    }

    unsigned unsorted = मिच्छा;
    unsigned अङ्कः_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    सचे (unlikely (!record)) return_trace (मिच्छा);

    अत्थाय (ध्रुव auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      सचे (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	सचे (unlikely (cur_gid < prev_gid))
	  unsorted = सच्च;

	सचे (unlikely (!record)) भङ्ग;
	record->last = prev_gid;
	अङ्कः_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    सचे (unlikely (c->in_error ())) return_trace (मिच्छा);

    सचे (likely (record)) record->last = prev_gid;
    rangeRecord.len = अङ्कः_ranges;

    सचे (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सच्च,
               तार्किक use_class_zero = सच्च,
               ध्रुव Coverage* glyph_filter = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    सचे (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      अत्थाय (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	सचे (!klass) अनुवर्त;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	सचे (new_gid == HB_MAP_मानम्_INVALID) अनुवर्त;
	सचे (glyph_filter && !glyph_filter->has (g)) अनुवर्त;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    अञ्ञथा
    {
      unsigned अङ्कः_source_glyphs = c->plan->source->get_अङ्कः_glyphs ();
      अत्थाय (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	सचे (!klass) अनुवर्त;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, अङ्कः_source_glyphs);
	अत्थाय (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  सचे (new_gid == HB_MAP_मानम्_INVALID) अनुवर्त;
	  सचे (glyph_filter && !glyph_filter->has (g)) अनुवर्त;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    ध्रुव hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_गणनम् = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_गणनम् <= glyph_and_klass.length;
    सचे (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (मिच्छा);
    return_trace (keep_empty_table || (तार्किक) glyph_and_klass);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () ध्रुव { पटिनिवत्त hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) ध्रुव
  {
    अत्थाय (auto &range : rangeRecord)
      सचे (range.value)
	सचे (unlikely (!range.collect_coverage (glyphs)))
	  पटिनिवत्त मिच्छा;
    पटिनिवत्त सच्च;
  }

  template <typename set_t>
  तार्किक collect_class (set_t *glyphs, unsigned पूर्णाङ्क klass) ध्रुव
  {
    अत्थाय (auto &range : rangeRecord)
    {
      सचे (range.value == klass)
	सचे (unlikely (!range.collect_coverage (glyphs)))
	  पटिनिवत्त मिच्छा;
    }
    पटिनिवत्त सच्च;
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    सचे (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      अत्थाय (auto g : *glyphs)
        सचे (get_class (g))
	  पटिनिवत्त सच्च;
      पटिनिवत्त मिच्छा;
    }

    पटिनिवत्त hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ध्रुव RangeRecord<Types> &range) { पटिनिवत्त range.intersects (*glyphs) && range.value; }));
  }
  तार्किक intersects_class (ध्रुव hb_set_t *glyphs, unsigned klass) ध्रुव
  {
    सचे (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      सचे (!rangeRecord.len)
        पटिनिवत्त !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_मानम्_INVALID;
      hb_codepoint_t last = HB_SET_मानम्_INVALID;
      auto it = hb_iter (rangeRecord);
      अत्थाय (auto &range : it)
      {
        सचे (it->first == last + 1)
	{
	  it++;
	  अनुवर्त;
	}

	सचे (!glyphs->next (&g))
	  भङ्ग;
	सचे (g < range.first)
	  पटिनिवत्त सच्च;
	g = range.last;
	last = g;
      }
      सचे (g != HB_SET_मानम्_INVALID && glyphs->next (&g))
	पटिनिवत्त सच्च;
      /* Fall through. */
    }
    अत्थाय (ध्रुव auto &range : rangeRecord)
      सचे (range.value == klass && range.intersects (*glyphs))
	पटिनिवत्त सच्च;
    पटिनिवत्त मिच्छा;
  }

  शून्य intersected_class_glyphs (ध्रुव hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ध्रुव
  {
    सचे (klass == 0)
    {
      hb_codepoint_t g = HB_SET_मानम्_INVALID;
      अत्थाय (auto &range : rangeRecord)
      {
	सचे (!glyphs->next (&g))
	  गच्छ done;
	याव (g < range.first)
	{
	  intersect_glyphs->add (g);
	  सचे (!glyphs->next (&g))
	    गच्छ done;
        }
        g = range.last;
      }
      याव (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      पटिनिवत्त;
    }

    unsigned गणनम् = rangeRecord.len;
    सचे (गणनम् > glyphs->get_population () * hb_bit_storage (गणनम्))
    {
      अत्थाय (auto g : *glyphs)
      {
        unsigned i;
        सचे (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      पटिनिवत्त;
    }

    अत्थाय (auto &range : rangeRecord)
    {
      सचे (range.value != klass) अनुवर्त;

      unsigned end = range.last + 1;
      अत्थाय (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  शून्य intersected_classes (ध्रुव hb_set_t *glyphs, hb_set_t *intersect_classes) ध्रुव
  {
    सचे (glyphs->is_empty ()) पटिनिवत्त;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    सचे (!rangeRecord.len)
    {
      intersect_classes->add (0);
      पटिनिवत्त;
    }

    hb_codepoint_t g = HB_SET_मानम्_INVALID;
    अत्थाय (auto &range : rangeRecord)
    {
      सचे (!glyphs->next (&g))
        भङ्ग;
      सचे (g < range.first)
      {
        intersect_classes->add (0);
        भङ्ग;
      }
      g = range.last;
    }
    सचे (g != HB_SET_मानम्_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    अत्थाय (ध्रुव auto& range : rangeRecord)
      सचे (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  रक्षित:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

संरचना ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ध्रुव { पटिनिवत्त get (k); }
  तार्किक has (hb_codepoint_t k) ध्रुव { पटिनिवत्त (*एतत्)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) ध्रुव { पटिनिवत्त get (k); }

  unsigned पूर्णाङ्क get (hb_codepoint_t k) ध्रुव { पटिनिवत्त get_class (k); }
  unsigned पूर्णाङ्क get_class (hb_codepoint_t glyph_id) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_class (glyph_id);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_class (glyph_id);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.get_class (glyph_id);
#endif
    स्वभाव:पटिनिवत्त 0;
    }
  }
  unsigned पूर्णाङ्क get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ध्रुव
  {
    unsigned klass;
    सचे (cache && cache->get (glyph_id, &klass)) पटिनिवत्त klass;
    klass = get_class (glyph_id);
    सचे (cache) cache->set (glyph_id, klass);
    पटिनिवत्त klass;
  }

  unsigned get_population () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_population ();
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_population ();
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.get_population ();
#endif
    स्वभाव:पटिनिवत्त NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    सचे (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned अङ्कः_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      अत्थाय (ध्रुव auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	सचे (cur_klass > class_max) class_max = cur_klass;
	सचे (cur_gid == glyph_min) अनुवर्त;
        सचे (cur_gid > glyph_max) glyph_max = cur_gid;
	सचे (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  अङ्कः_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	सचे (size >= best_size) पटिनिवत्त;
	best_size = size;
	format = candidate;
      };

      सचे (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      सचे (glyph_max <= 0xFFFFu && अङ्कः_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) अङ्कः_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      सचे (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      सचे (glyph_max <= 0xFFFFFFu && अङ्कः_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) अङ्कः_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      सचे (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (मिच्छा, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (मिच्छा);
      }
    }

    u.format.v = format;

    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    पक्ष 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    पक्ष 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    स्वभाव:return_trace (मिच्छा);
    }
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सच्च,
               तार्किक use_class_zero = सच्च,
               ध्रुव Coverage* glyph_filter = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    पक्ष 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    पक्ष 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    स्वभाव:return_trace (मिच्छा);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!u.format.v.sanitize (c)) return_trace (मिच्छा);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    स्वभाव:return_trace (सच्च);
    }
  }

  unsigned cost () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.cost ();
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.cost ();
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.cost ();
#endif
    स्वभाव:पटिनिवत्त 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.collect_coverage (glyphs);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.collect_coverage (glyphs);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.collect_coverage (glyphs);
#endif
    स्वभाव:पटिनिवत्त मिच्छा;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  तार्किक collect_class (set_t *glyphs, unsigned पूर्णाङ्क klass) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.collect_class (glyphs, klass);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.collect_class (glyphs, klass);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.collect_class (glyphs, klass);
#endif
    स्वभाव:पटिनिवत्त मिच्छा;
    }
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.intersects (glyphs);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.intersects (glyphs);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.intersects (glyphs);
#endif
    स्वभाव:पटिनिवत्त मिच्छा;
    }
  }
  तार्किक intersects_class (ध्रुव hb_set_t *glyphs, unsigned पूर्णाङ्क klass) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.intersects_class (glyphs, klass);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.intersects_class (glyphs, klass);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.intersects_class (glyphs, klass);
#endif
    स्वभाव:पटिनिवत्त मिच्छा;
    }
  }

  शून्य intersected_class_glyphs (ध्रुव hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    स्वभाव:पटिनिवत्त;
    }
  }

  शून्य intersected_classes (ध्रुव hb_set_t *glyphs, hb_set_t *intersect_classes) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.intersected_classes (glyphs, intersect_classes);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.intersected_classes (glyphs, intersect_classes);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    स्वभाव:पटिनिवत्त;
    }
  }


  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
स्थिर अन्तःस्थ तार्किक ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ पटिनिवत्त (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
संरचना delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  संरचना chars_t : hb_vector_t<uint8_t>
  {
    पूर्णाङ्क cmp (ध्रुव chars_t& other) ध्रुव
    {
      पटिनिवत्त as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      अत्थाय (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      पटिनिवत्त hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (ध्रुव chars_t& other) ध्रुव
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      अत्थाय (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      पटिनिवत्त hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (ध्रुव delta_row_encoding_t& other_encoding) ध्रुव { पटिनिवत्त chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क>*> वस्तूनि;

  delta_row_encoding_t () = स्वभाव;
  delta_row_encoding_t (hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क>*> &&rows, unsigned अङ्कः_cols)
  {
    प्रतिज्ञा (rows);

    वस्तूनि = std::move (rows);

    सचे (unlikely (!chars.resize (अङ्कः_cols)))
      पटिनिवत्त;

    calculate_chars ();
  }

  शून्य merge (ध्रुव delta_row_encoding_t& other)
  {
    वस्तूनि.alloc (वस्तूनि.length + other.वस्तूनि.length);
    अत्थाय (auto &row : other.वस्तूनि)
      add_row (row);

    // Merge chars
    प्रतिज्ञा (chars.length == other.chars.length);
    अत्थाय (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  शून्य chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  शून्य calculate_chars ()
  {
    प्रतिज्ञा (वस्तूनि);

    तार्किक long_शब्दाः = मिच्छा;

    अत्थाय (auto &row : वस्तूनि)
    {
      प्रतिज्ञा (row->length == chars.length);

      /* 0/1/2 byte encoding */
      अत्थाय (unsigned i = 0; i < row->length; i++)
      {
	पूर्णाङ्क v =  row->arrayZ[i];
	सचे (v == 0)
	  अनुवर्त;
	अञ्ञथा सचे (v > 32767 || v < -32768)
	{
	  long_शब्दाः = सच्च;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	अञ्ञथा सचे (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	अञ्ञथा
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    सचे (long_शब्दाः)
    {
      // Convert 1s to 2s
      अत्थाय (auto &v : chars)
	सचे (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  तार्किक is_empty () ध्रुव
  { पटिनिवत्त !वस्तूनि; }

  तार्किक in_error () ध्रुव
  { पटिनिवत्त chars.in_error () || वस्तूनि.in_error (); }

  स्थिर अन्तःस्थ unsigned get_chars_overhead (unsigned अङ्कः_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    पटिनिवत्त c + अङ्कः_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) ध्रुव
  {
    पूर्णाङ्क गणनम् = वस्तूनि.length;
    पटिनिवत्त hb_max (0, (पूर्णाङ्क) overhead - गणनम् * (पूर्णाङ्क) additional_bytes_per_rows);
  }

  पूर्णाङ्क gain_from_merging (ध्रुव delta_row_encoding_t& other_encoding) ध्रुव
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    सचे (additional_bytes_per_rows > 0)
    {
      सचे (get_gain (additional_bytes_per_rows) == 0)
        पटिनिवत्त 0;
    }
    अञ्ञथा
    {
      सचे (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	पटिनिवत्त 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    पूर्णाङ्क combined_gain = (पूर्णाङ्क) overhead + (पूर्णाङ्क) other_encoding.overhead;
    combined_gain -= (combined_width - (पूर्णाङ्क) width) * वस्तूनि.length;
    combined_gain -= (combined_width - (पूर्णाङ्क) other_encoding.width) * other_encoding.वस्तूनि.length;
    combined_gain -= get_chars_overhead (combined_columns);

    पटिनिवत्त combined_gain;
  }

  तार्किक add_row (ध्रुव hb_vector_t<पूर्णाङ्क>* row)
  { पटिनिवत्त वस्तूनि.push (row); }

  स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    ध्रुव delta_row_encoding_t *a = (ध्रुव delta_row_encoding_t *)pa;
    ध्रुव delta_row_encoding_t *b = (ध्रुव delta_row_encoding_t *)pb;

    सचे (a->width != b->width)
      पटिनिवत्त (पूर्णाङ्क) a->width - (पूर्णाङ्क) b->width;

    पटिनिवत्त b->chars.cmp (a->chars);
  }
};

संरचना VarRegionAxis
{
  भिन्न evaluate (पूर्णाङ्क coord) ध्रुव
  {
    पूर्णाङ्क peak = peakCoord.to_int ();
    सचे (peak == 0 || coord == peak)
      पटिनिवत्त 1.f;
    अञ्ञथा सचे (coord == 0) // Faster
      पटिनिवत्त 0.f;

    पूर्णाङ्क start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    सचे (unlikely (start > peak || peak > end))
      पटिनिवत्त 1.f;
    सचे (unlikely (start < 0 && end > 0))
      पटिनिवत्त 1.f;

    सचे (coord <= start || end <= coord)
      पटिनिवत्त 0.f;

    /* Interpolate */
    सचे (coord < peak)
      पटिनिवत्त भिन्न (coord - start) / (peak - start);
    अञ्ञथा
      पटिनिवत्त भिन्न (end - coord) / (end - peak);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  तार्किक serialize (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (एतत्));
  }

  सार्वजनिक:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};
संरचना SparseVarRegionAxis
{
  भिन्न evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len) ध्रुव
  {
    unsigned i = axisIndex;
    पूर्णाङ्क coord = i < coord_len ? coords[i] : 0;
    पटिनिवत्त axis.evaluate (coord);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव SparseVarRegionAxis *src,
		  ध्रुव hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->embed (src);
    सचे (unlikely (!out)) return_trace (मिच्छा);
    सचे (axis_map)
    {
      सचे (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (मिच्छा);
      }
      सचे (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (मिच्छा);
    }
    return_trace (सच्च);
  }

  सार्वजनिक:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना hb_scalar_cache_t
{
  गुप्त:
  स्थिर constexpr unsigned STATIC_LENGTH = 128;
  स्थिर constexpr भिन्न INVALID = NAN;

  सार्वजनिक:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ध्रुव hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (ध्रुव hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  स्थिर hb_scalar_cache_t *create (unsigned पूर्णाङ्क गणनम्,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    सचे (!गणनम्) पटिनिवत्त (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    सचे (scratch_cache && गणनम् <= STATIC_LENGTH)
    {
      scratch_cache->length = गणनम्;
      scratch_cache->clear ();
      पटिनिवत्त scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (परिमाण (hb_scalar_cache_t) - परिमाण (static_मानानि) + परिमाण (static_मानानि[0]) * गणनम्);
    सचे (unlikely (!cache)) पटिनिवत्त (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = गणनम्;
    cache->clear ();

    पटिनिवत्त cache;
  }

  स्थिर शून्य destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    सचे (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  शून्य clear ()
  {
    auto *values = &static_मानानि[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    अत्थाय (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    अत्थाय (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  तार्किक get (unsigned i, भिन्न *value) ध्रुव
  {
    सचे (unlikely (i >= length))
    {
      *value = 0.f;
      पटिनिवत्त सच्च;
    }
    auto *values = &static_मानानि[0];
    auto *cached_मानम् = &values[i];
    भिन्न v = *cached_मानम्;
    सचे (std::isnan (v))
      पटिनिवत्त मिच्छा;
    *value = v;
    पटिनिवत्त सच्च;
  }

  HB_ALWAYS_INLINE
  शून्य set (unsigned i, भिन्न value)
  {
    सचे (unlikely (i >= length)) पटिनिवत्त;
    auto *values = &static_मानानि[0];
    auto *cached_मानम् = &values[i];
    *cached_मानम् = value;
  }

  सार्वजनिक:
  unsigned length;
  गुप्त:
  mutable hb_atomic_t<भिन्न> static_मानानि[STATIC_LENGTH];
};

संरचना VarRegionList
{
  गुप्त:
  भिन्न evaluate_impl (unsigned पूर्णाङ्क region_index,
		       ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len) ध्रुव
  {
    ध्रुव VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_गणनम्_2);
    भिन्न v = 1.f;

    unsigned पूर्णाङ्क गणनम् = axis_गणनम्_2;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      पूर्णाङ्क coord = i < coord_len ? coords[i] : 0;
      भिन्न factor = axes[i].evaluate (coord);
      सचे (factor == 0.f)
      {
	v = 0.f;
	भङ्ग;
      }
      v *= factor;
    }

    पटिनिवत्त v;
  }

  सार्वजनिक:
  HB_ALWAYS_INLINE
  भिन्न evaluate (unsigned पूर्णाङ्क region_index,
		  ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		  hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    सचे (unlikely (region_index >= region_गणनम्))
      पटिनिवत्त 0.;

    भिन्न v;
    सचे (cache && cache->get (region_index, &v))
      पटिनिवत्त v;

    v = evaluate_impl (region_index, coords, coord_len);

    सचे (cache)
      cache->set (region_index, v);
    पटिनिवत्त v;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_गणनम्_2 * region_गणनम्));
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  ध्रुव hb_vector_t<hb_tag_t>& axis_tags,
                  ध्रुव hb_vector_t<ध्रुव hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned axis_गणनम् = axis_tags.length;
    unsigned region_गणनम्_2 = regions.length;
    सचे (!axis_गणनम् || !region_गणनम्_2) return_trace (मिच्छा);
    सचे (unlikely (hb_unsigned_mul_overflows (axis_गणनम् * region_गणनम्_2,
                                             VarRegionAxis::static_size))) return_trace (मिच्छा);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    axis_गणनम्_2 = axis_गणनम्;
    region_गणनम् = region_गणनम्_2;

    अत्थाय (unsigned r = 0; r < region_गणनम्_2; r++)
    {
      ध्रुव auto& region = regions[r];
      अत्थाय (unsigned i = 0; i < axis_गणनम्; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        सचे (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->मध्यः);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        अञ्ञथा
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        सचे (!var_region_rec.serialize (c))
          return_trace (मिच्छा);
      }
    }
    return_trace (सच्च);
  }

  तार्किक serialize (hb_serialize_context_t *c, ध्रुव VarRegionList *src, ध्रुव hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    axis_गणनम्_2 = src->axis_गणनम्_2;
    region_गणनम् = region_map.get_population ();
    सचे (unlikely (hb_unsigned_mul_overflows (axis_गणनम्_2 * region_गणनम्,
					     VarRegionAxis::static_size))) return_trace (मिच्छा);
    सचे (unlikely (!c->extend (एतत्))) return_trace (मिच्छा);
    unsigned पूर्णाङ्क region_गणनम्_2 = src->region_गणनम्;
    अत्थाय (unsigned पूर्णाङ्क r = 0; r < region_गणनम्; r++)
    {
      unsigned पूर्णाङ्क backward = region_map.backward (r);
      सचे (backward >= region_गणनम्_2) return_trace (मिच्छा);
      hb_memcpy (&axesZ[axis_गणनम्_2 * r], &src->axesZ[axis_गणनम्_2 * backward], VarRegionAxis::static_size * axis_गणनम्_2);
    }

    return_trace (सच्च);
  }

  तार्किक get_var_region (unsigned region_index,
                       ध्रुव hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ध्रुव
  {
    सचे (region_index >= region_गणनम्) पटिनिवत्त मिच्छा;
    ध्रुव VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_गणनम्_2);
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      hb_tag_t *axis_tag;
      सचे (!axes_old_index_tag_map.has (i, &axis_tag))
        पटिनिवत्त मिच्छा;

      भिन्न min_val = axis_region->startCoord.to_float ();
      भिन्न def_val = axis_region->peakCoord.to_float ();
      भिन्न max_val = axis_region->endCoord.to_float ();

      सचे (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((द्विगुण) min_val, (द्विगुण) def_val, (द्विगुण) max_val));
      axis_region++;
    }
    पटिनिवत्त !axis_tuples.in_error ();
  }

  तार्किक get_var_regions (ध्रुव hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) ध्रुव
  {
    सचे (!regions.alloc (region_गणनम्))
      पटिनिवत्त मिच्छा;

    अत्थाय (unsigned i = 0; i < region_गणनम्; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      सचे (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        पटिनिवत्त मिच्छा;
      regions.push (std::move (axis_tuples));
    }
    पटिनिवत्त !regions.in_error ();
  }

  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_गणनम्_2, region_गणनम्)); }

  सार्वजनिक:
  HBUINT16	axis_गणनम्_2;
  HBUINT15	region_गणनम्;
  रक्षित:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
स्थिर अन्तःस्थ तार्किक
_hb_avar2_region_is_dead (ध्रुव hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  ध्रुव hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  अत्थाय (auto _ : axis_tuples)
  {
    Triple *range;
    सचे (!reachable_ranges.has (_.first, &range)) अनुवर्त;
    ध्रुव Triple &tent = _.second;
    पूर्णाङ्क start = (पूर्णाङ्क) roundf ((भिन्न) tent.minimum * 16384.f);
    पूर्णाङ्क peak  = (पूर्णाङ्क) roundf ((भिन्न) tent.मध्यः * 16384.f);
    पूर्णाङ्क end   = (पूर्णाङ्क) roundf ((भिन्न) tent.maximum * 16384.f);
    सचे (!peak) अनुवर्त;
    सचे (start > peak || peak > end) अनुवर्त;
    सचे (start < 0 && end > 0) अनुवर्त;
    पूर्णाङ्क lo = (पूर्णाङ्क) roundf ((भिन्न) range->minimum * 16384.f);
    पूर्णाङ्क hi = (पूर्णाङ्क) roundf ((भिन्न) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    सचे (hi < start || lo > end)
      पटिनिवत्त सच्च;
  }
  पटिनिवत्त मिच्छा;
}

संरचना SparseVariationRegion
{
  भिन्न evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len) ध्रुव
  {
    भिन्न v = 1.f;
    unsigned पूर्णाङ्क गणनम् = axes.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      भिन्न factor = (एतत्+axes[i]).evaluate (coords, coord_len);
      सचे (factor == 0.f)
	पटिनिवत्त 0.;
      v *= factor;
    }
    पटिनिवत्त v;
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव SparseVariationRegion *src,
		  ध्रुव hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (मिच्छा);
    अत्थाय (unsigned i = 0; i < axes.len; i++)
      सचे (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (मिच्छा);
    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (axes.sanitize (c, एतत्));
  }

  रक्षित:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, axes);
};

संरचना SparseVarRegionList
{
  HB_ALWAYS_INLINE
  भिन्न evaluate (unsigned पूर्णाङ्क region_index,
		  ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		  hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    सचे (unlikely (region_index >= regions.len))
      पटिनिवत्त 0.;

    भिन्न v;
    सचे (cache && cache->get (region_index, &v))
      पटिनिवत्त v;

    ध्रुव SparseVariationRegion &region = एतत्+regions[region_index];

    v = region.evaluate (coords, coord_len);
    सचे (cache)
      cache->set (region_index, v);

    पटिनिवत्त v;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (regions.sanitize (c, एतत्));
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव SparseVarRegionList *src,
		  ध्रुव hb_inc_bimap_t &region_map,
		  ध्रुव hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    regions.len = region_map.get_population ();
    सचे (unlikely (!c->extend (regions))) return_trace (मिच्छा);

    अत्थाय (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      सचे (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (मिच्छा);
    }
    return_trace (सच्च);
  }

  सार्वजनिक:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, regions);
};


संरचना VarData
{
  unsigned पूर्णाङ्क get_item_गणनम् () ध्रुव
  { पटिनिवत्त item_गणनम्; }

  unsigned पूर्णाङ्क get_region_index_गणनम् () ध्रुव
  { पटिनिवत्त regionIndices.len; }

  unsigned get_region_index (unsigned i) ध्रुव
  { पटिनिवत्त i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () ध्रुव
  { पटिनिवत्त (word_गणनम् () + regionIndices.len) * (long_शब्दाः_2 () ? 2 : 1); }

  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_गणनम्, get_row_size ())); }

  template <typename Float = भिन्न>
  Float _get_delta (unsigned पूर्णाङ्क inner,
		    ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
		    ध्रुव VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    सचे (unlikely (inner >= item_गणनम्))
      पटिनिवत्त 0.;
    तार्किक is_long = long_शब्दाः_2 ();
    unsigned पूर्णाङ्क गणनम् = regionIndices.len;
    unsigned word_गणनम्_2 = word_गणनम् ();
    unsigned पूर्णाङ्क scount = is_long ? गणनम् : word_गणनम्_2;
    unsigned पूर्णाङ्क lcount = is_long ? word_गणनम्_2 : 0;

    ध्रुव HBUINT8 *bytes = get_delta_bytes ();
    ध्रुव HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned पूर्णाङ्क i = 0;

    ध्रुव HBINT32 *lcursor = reinterpret_cast<ध्रुव HBINT32 *> (row);
    अत्थाय (; i < lcount; i++)
    {
      भिन्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गणनम्, cache);
      सचे (scalar)
        delta += (Float) scalar * (पूर्णाङ्क) *lcursor;
      lcursor++;
    }
    ध्रुव HBINT16 *scursor = reinterpret_cast<ध्रुव HBINT16 *> (lcursor);
    अत्थाय (; i < scount; i++)
    {
      भिन्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गणनम्, cache);
      सचे (scalar)
       delta += (Float) scalar * (पूर्णाङ्क) *scursor;
      scursor++;
    }
    ध्रुव HBINT8 *bcursor = reinterpret_cast<ध्रुव HBINT8 *> (scursor);
    अत्थाय (; i < गणनम्; i++)
    {
      भिन्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गणनम्, cache);
      सचे (scalar)
        delta += (Float) scalar * (पूर्णाङ्क) *bcursor;
      bcursor++;
    }

    पटिनिवत्त delta;
  }

  template <typename Float = भिन्न>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned पूर्णाङ्क inner,
		   ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
		   ध्रुव VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = regionIndices.len;
    सचे (!गणनम्) पटिनिवत्त Float (0); // This is quite common, so optimize it.
    पटिनिवत्त _get_delta<Float> (inner, coords, coord_गणनम्, regions, cache);
  }

  शून्य get_region_scalars (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
			   ध्रुव VarRegionList &regions,
			   भिन्न *scalars /*OUT */,
			   unsigned पूर्णाङ्क अङ्कः_scalars) ध्रुव
  {
    unsigned गणनम् = hb_min (अङ्कः_scalars, regionIndices.len);
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गणनम्);
    अत्थाय (unsigned पूर्णाङ्क i = गणनम्; i < अङ्कः_scalars; i++)
      scalars[i] = 0.f;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_गणनम् () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_गणनम्,
				  get_row_size ()));
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  तार्किक has_long,
                  ध्रुव hb_vector_t<ध्रुव hb_vector_t<पूर्णाङ्क>*>& rows)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned row_गणनम्_2 = rows.length;
    सचे (!row_गणनम्_2) {
      // Nothing to serialize, will be empty.
      पटिनिवत्त मिच्छा;
    }

    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    सचे (unlikely (!c->check_assign (item_गणनम्, row_गणनम्_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (मिच्छा);

    पूर्णाङ्क min_threshold = has_long ? -32768 : -128;
    पूर्णाङ्क max_threshold = has_long ? +32767 : +127;
    गणना delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned अङ्कः_regions = rows[0]->length;
    सचे (!delta_sz.resize (अङ्कः_regions))
      return_trace (मिच्छा);

    unsigned word_गणनम्_2 = 0;
    अत्थाय (unsigned r = 0; r < अङ्कः_regions; r++)
    {
      अत्थाय (unsigned i = 0; i < row_गणनम्_2; i++)
      {
        पूर्णाङ्क delta = rows[i]->arrayZ[r];
        सचे (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_गणनम्_2++;
          भङ्ग;
        }
        अञ्ञथा सचे (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_गणनम्_2;
    hb_map_t ri_map;
    अत्थाय (unsigned r = 0; r < अङ्कः_regions; r++)
    {
      सचे (!delta_sz[r]) अनुवर्त;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      सचे (!ri_map.set (new_r, r))
        return_trace (मिच्छा);
    }

    word_Size_गणनम् = word_गणनम्_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_गणनम् = ri_map.get_population ();
    regionIndices.len = ri_गणनम्;
    सचे (unlikely (!c->extend (एतत्))) return_trace (मिच्छा);

    अत्थाय (unsigned r = 0; r < ri_गणनम्; r++)
    {
      hb_codepoint_t *idx;
      सचे (!ri_map.has (r, &idx))
        return_trace (मिच्छा);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < row_गणनम्_2; i++)
    {
      अत्थाय (unsigned पूर्णाङ्क r = 0; r < ri_गणनम्; r++)
      {
        पूर्णाङ्क delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (सच्च);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव VarData *src,
		  ध्रुव hb_inc_bimap_t &inner_map,
		  ध्रुव hb_inc_bimap_t &region_map,
		  ध्रुव hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    item_गणनम् = inner_map.get_next_मानम् ();

    /* Optimize word count */
    unsigned ri_गणनम् = src->regionIndices.len;
    गणना delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned पूर्णाङ्क> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_गणनम्);
    ri_map.resize (ri_गणनम्);
    unsigned पूर्णाङ्क new_word_गणनम् = 0;
    unsigned पूर्णाङ्क r;

    ध्रुव HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_गणनम् = src->word_गणनम् ();
    तार्किक     src_long_शब्दाः = src->long_शब्दाः_2 ();

    तार्किक has_long = मिच्छा;
    सचे (src_long_शब्दाः)
    {
      अत्थाय (r = 0; r < src_word_गणनम्; r++)
      {
        अत्थाय (unsigned old_gid : inner_map.कुञ्चिकाः())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  सचे (delta < -32768 || 32767 < delta)
	  {
	    has_long = सच्च;
	    भङ्ग;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    अत्थाय (r = 0; r < ri_गणनम्; r++)
    {
      तार्किक short_circuit = src_long_शब्दाः == has_long && src_word_गणनम् <= r;

      delta_sz[r] = kZero;
      सचे (culled_regions && culled_regions->has (src->regionIndices[r]))
	अनुवर्त; /* unreachable region: drop its column */
      अत्थाय (unsigned old_gid : inner_map.कुञ्चिकाः())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	सचे (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_गणनम्++;
	  भङ्ग;
	}
	अञ्ञथा सचे (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  सचे (short_circuit)
	    भङ्ग;
	}
      }
    }

    unsigned पूर्णाङ्क word_index = 0;
    unsigned पूर्णाङ्क non_word_index = new_word_गणनम्;
    unsigned पूर्णाङ्क new_ri_गणनम् = 0;
    अत्थाय (r = 0; r < ri_गणनम्; r++)
      सचे (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_गणनम्++;
      }

    word_Size_गणनम् = new_word_गणनम् | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_गणनम्;

    सचे (unlikely (!c->extend (एतत्))) return_trace (मिच्छा);

    अत्थाय (r = 0; r < new_ri_गणनम्; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned गणनम् = item_गणनम्;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      unsigned पूर्णाङ्क old = inner_map.backward (i);
      अत्थाय (unsigned पूर्णाङ्क r = 0; r < new_ri_गणनम्; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (सच्च);
  }

  शून्य collect_region_refs (hb_set_t &region_indices, ध्रुव hb_inc_bimap_t &inner_map) ध्रुव
  {
    ध्रुव HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    अत्थाय (unsigned पूर्णाङ्क r = 0; r < regionIndices.len; r++)
    {
      unsigned पूर्णाङ्क region = regionIndices.arrayZ[r];
      सचे (region_indices.has (region)) अनुवर्त;
      अत्थाय (hb_codepoint_t old_gid : inner_map.कुञ्चिकाः())
	सचे (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  भङ्ग;
	}
    }
  }

  सार्वजनिक:
  ध्रुव HBUINT8 *get_delta_bytes () ध्रुव
  { पटिनिवत्त &StructAfter<HBUINT8> (regionIndices); }

  रक्षित:
  HBUINT8 *get_delta_bytes ()
  { पटिनिवत्त &StructAfter<HBUINT8> (regionIndices); }

  सार्वजनिक:
  int32_t get_item_delta_fast (unsigned पूर्णाङ्क item, unsigned पूर्णाङ्क region,
			       ध्रुव HBUINT8 *delta_bytes, unsigned row_size) ध्रुव
  {
    सचे (unlikely (item >= item_गणनम् || region >= regionIndices.len)) पटिनिवत्त 0;

    ध्रुव HBINT8 *p = (ध्रुव HBINT8 *) delta_bytes + item * row_size;
    unsigned word_गणनम्_2 = word_गणनम् ();
    तार्किक is_long = long_शब्दाः_2 ();
    सचे (is_long)
    {
      सचे (region < word_गणनम्_2)
	पटिनिवत्त ((ध्रुव HBINT32 *) p)[region];
      अञ्ञथा
	पटिनिवत्त ((ध्रुव HBINT16 *)(p + HBINT32::static_size * word_गणनम्_2))[region - word_गणनम्_2];
    }
    अञ्ञथा
    {
      सचे (region < word_गणनम्_2)
	पटिनिवत्त ((ध्रुव HBINT16 *) p)[region];
      अञ्ञथा
	पटिनिवत्त (p + HBINT16::static_size * word_गणनम्_2)[region - word_गणनम्_2];
    }
  }
  int32_t get_item_delta (unsigned पूर्णाङ्क item, unsigned पूर्णाङ्क region) ध्रुव
  {
     पटिनिवत्त get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  रक्षित:
  शून्य set_item_delta_fast (unsigned पूर्णाङ्क item, unsigned पूर्णाङ्क region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_गणनम्_2 = word_गणनम् ();
    तार्किक is_long = long_शब्दाः_2 ();
    सचे (is_long)
    {
      सचे (region < word_गणनम्_2)
	((HBINT32 *) p)[region] = delta;
      अञ्ञथा
	((HBINT16 *)(p + HBINT32::static_size * word_गणनम्_2))[region - word_गणनम्_2] = delta;
    }
    अञ्ञथा
    {
      सचे (region < word_गणनम्_2)
	((HBINT16 *) p)[region] = delta;
      अञ्ञथा
	(p + HBINT16::static_size * word_गणनम्_2)[region - word_गणनम्_2] = delta;
    }
  }
  शून्य set_item_delta (unsigned पूर्णाङ्क item, unsigned पूर्णाङ्क region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  तार्किक long_शब्दाः_2 () ध्रुव { पटिनिवत्त word_Size_गणनम् & 0x8000u /* LONG_WORDS */; }
  unsigned word_गणनम् () ध्रुव { पटिनिवत्त word_Size_गणनम् & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  रक्षित:
  HBUINT16		item_गणनम्;
  HBUINT16		word_Size_गणनम्;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

संरचना MultiVarData
{
  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  ध्रुव TupleList &get_delta_sets () ध्रुव
  { पटिनिवत्त एतत्+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  शून्य get_delta (unsigned पूर्णाङ्क inner,
		  ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
		  ध्रुव SparseVarRegionList &regions,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    auto &deltaSets = get_delta_sets ();

    auto मानानि_iter = deltaSets.fetcher (inner);
    unsigned region_गणनम् = regionIndices.len;
    unsigned skip = 0;
    अत्थाय (unsigned regionIndex = 0; regionIndex < region_गणनम्; regionIndex++)
    {
      भिन्न scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_गणनम्,
				       cache);
      // We skip lazily. Helps with the tail end.
      सचे (scalar == 0.0f)
        skip += out.length;
      अञ्ञथा
      {
        सचे (skip)
	{
	  मानानि_iter.skip (skip);
	  skip = 0;
	}
	मानानि_iter.add_to (out, scalar);
      }
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, एतत्));
  }

  unsigned get_item_गणनम् () ध्रुव
  { पटिनिवत्त get_delta_sets ().गणनम्; }

  शून्य collect_region_refs (hb_set_t &region_indices) ध्रुव
  {
    अत्थाय (unsigned region : regionIndices)
      region_indices.add (region);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव MultiVarData *src,
		  ध्रुव hb_inc_bimap_t &inner_map,
		  ध्रुव hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned region_गणनम्_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_गणनम्_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    सचे (unlikely (!c->extend_size (एतत्, header_size, मिच्छा)))
      return_trace (मिच्छा);
    format = 1;
    regionIndices.len = region_गणनम्_2;
    अत्थाय (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      सचे (unlikely (!region_map.has (old_region)))
	return_trace (मिच्छा);
      regionIndices[i] = region_map.get (old_region);
    }

    ध्रुव auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    सचे (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (मिच्छा);
    अत्थाय (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      सचे (unlikely (old_inner >= src_delta_sets.गणनम्)) return_trace (मिच्छा);
      hb_ubytes_t bytes = static_cast<ध्रुव CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    सचे (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (मिच्छा);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  रक्षित:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  सार्वजनिक:
  DEFINE_SIZE_MIN (7);
};

संरचना ItemVariationStore
{
  friend संरचना item_variations_t;

  hb_scalar_cache_t *create_cache () ध्रुव
  {
#ifdef HB_NO_VAR
    पटिनिवत्त hb_scalar_cache_t::create (0);
#endif
    पटिनिवत्त hb_scalar_cache_t::create ((एतत्+regions).region_गणनम्);
  }

  स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  गुप्त:
  template <typename Float = भिन्न>
  Float get_delta (unsigned पूर्णाङ्क outer, unsigned पूर्णाङ्क inner,
		   ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
		   hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
#ifdef HB_NO_VAR
    पटिनिवत्त Float (0);
#endif

    सचे (unlikely (outer >= dataSets.len))
      पटिनिवत्त Float (0);

    पटिनिवत्त (एतत्+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_गणनम्,
					     एतत्+regions,
					     cache);
  }

  सार्वजनिक:
  तार्किक has_delta_set (unsigned पूर्णाङ्क index) ध्रुव
  {
#ifdef HB_NO_VAR
    पटिनिवत्त मिच्छा;
#endif

    unsigned पूर्णाङ्क outer = index >> 16;
    unsigned पूर्णाङ्क inner = index & 0xFFFF;
    पटिनिवत्त outer < dataSets.len &&
	   inner < (एतत्+dataSets[outer]).get_item_गणनम् ();
  }

  template <typename Float = भिन्न>
  Float get_delta (unsigned पूर्णाङ्क index,
		   ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
		   hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    unsigned पूर्णाङ्क outer = index >> 16;
    unsigned पूर्णाङ्क inner = index & 0xFFFF;
    पटिनिवत्त get_delta<Float> (outer, inner, coords, coord_गणनम्, cache);
  }
  template <typename Float = भिन्न>
  Float get_delta (unsigned पूर्णाङ्क index,
		   hb_array_t<ध्रुव पूर्णाङ्क> coords,
		   hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    पटिनिवत्त get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
#ifdef HB_NO_VAR
    पटिनिवत्त सच्च;
#endif

    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, एतत्) &&
		  dataSets.sanitize (c, एतत्));
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  तार्किक has_long,
                  ध्रुव hb_vector_t<hb_tag_t>& axis_tags,
                  ध्रुव hb_vector_t<ध्रुव hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  ध्रुव hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (एतत्);
#ifdef HB_NO_VAR
    return_trace (मिच्छा);
#endif
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);

    format = 1;
    सचे (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (मिच्छा);

    unsigned अङ्कः_var_data = vardata_encodings.length;
    सचे (!अङ्कः_var_data) return_trace (मिच्छा);
    सचे (unlikely (!c->check_assign (dataSets.len, अङ्कः_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (मिच्छा);

    सचे (unlikely (!c->extend (dataSets))) return_trace (मिच्छा);
    अत्थाय (unsigned i = 0; i < अङ्कः_var_data; i++)
      सचे (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].वस्तूनि))
        return_trace (मिच्छा);

    return_trace (सच्च);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव ItemVariationStore *src,
		  ध्रुव hb_array_t <ध्रुव hb_inc_bimap_t> &inner_maps,
		  ध्रुव hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (एतत्);
#ifdef HB_NO_VAR
    return_trace (मिच्छा);
#endif

    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);

    unsigned पूर्णाङ्क set_गणनम् = 0;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < inner_maps.length; i++)
      सचे (inner_maps[i].get_population ())
	set_गणनम्++;

    format = 1;

    ध्रुव auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    सचे (region_indices.in_error ())
      return_trace (मिच्छा);

    region_indices.del_range ((src_regions).region_गणनम्, hb_set_t::INVALID);

    सचे (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    सचे (region_map.in_error())
      return_trace (मिच्छा);

    सचे (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (मिच्छा);

    dataSets.len = set_गणनम्;
    सचे (unlikely (!c->extend (dataSets))) return_trace (मिच्छा);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned पूर्णाङ्क set_index = 0;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < inner_maps.length; i++)
    {
      सचे (!inner_maps[i].get_population ()) अनुवर्त;
      सचे (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (मिच्छा);
    }

    return_trace (सच्च);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->start_embed (एतत्);
    सचे (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned गणनम् = dataSets.len;
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      सचे (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = एतत्+dataSets[i];

      unsigned item_गणनम् = data.get_item_गणनम् ();
      अत्थाय (unsigned j = 0; j < item_गणनम्; j++)
	map->add (j);
    }

    सचे (unlikely (!out->serialize (c, एतत्, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  तार्किक collect_dead_regions (ध्रुव hb_map_t &axes_old_index_tag_map,
			     ध्रुव hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) ध्रुव
  {
#ifndef HB_NO_VAR
    सचे (!reachable_ranges.get_population ()) पटिनिवत्त सच्च;
    ध्रुव VarRegionList &region_list = एतत्+regions;
    unsigned गणनम् = region_list.region_गणनम्;
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      सचे (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	पटिनिवत्त मिच्छा;
      सचे (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    पटिनिवत्त !dead_regions.in_error ();
#अञ्ञथा
    पटिनिवत्त सच्च;
#endif
  }

  तार्किक subset (hb_subset_context_t *c, ध्रुव hb_array_t<ध्रुव hb_inc_bimap_t> &inner_maps) ध्रुव
  {
    TRACE_SUBSET (एतत्);
#ifdef HB_NO_VAR
    return_trace (मिच्छा);
#endif

    ItemVariationStore *varstore_अविभाज्यः = c->serializer->start_embed<ItemVariationStore> ();
    सचे (unlikely (!varstore_अविभाज्यः)) return_trace (मिच्छा);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    सचे (c->plan->has_avar2)
      सचे (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (मिच्छा);

    varstore_अविभाज्यः->serialize (c->serializer, एतत्, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_अविभाज्यः->dataSets);
  }

  unsigned पूर्णाङ्क get_region_index_गणनम् (unsigned पूर्णाङ्क major) ध्रुव
  {
#ifdef HB_NO_VAR
    पटिनिवत्त 0;
#endif
    पटिनिवत्त (एतत्+dataSets[major]).get_region_index_गणनम् ();
  }

  शून्य get_region_scalars (unsigned पूर्णाङ्क major,
			   ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
			   भिन्न *scalars /*OUT*/,
			   unsigned पूर्णाङ्क अङ्कः_scalars) ध्रुव
  {
#ifdef HB_NO_VAR
    अत्थाय (unsigned i = 0; i < अङ्कः_scalars; i++)
      scalars[i] = 0.f;
    पटिनिवत्त;
#endif

    (एतत्+dataSets[major]).get_region_scalars (coords, coord_गणनम्,
					       एतत्+regions,
					       &scalars[0], अङ्कः_scalars);
  }

  unsigned पूर्णाङ्क get_sub_table_गणनम् () ध्रुव
   {
#ifdef HB_NO_VAR
     पटिनिवत्त 0;
#endif
     पटिनिवत्त dataSets.len;
   }

  ध्रुव VarData& get_sub_table (unsigned i) ध्रुव
  {
#ifdef HB_NO_VAR
     पटिनिवत्त Null (VarData);
#endif
     पटिनिवत्त एतत्+dataSets[i];
  }

  ध्रुव VarRegionList& get_region_list () ध्रुव
  {
#ifdef HB_NO_VAR
     पटिनिवत्त Null (VarRegionList);
#endif
     पटिनिवत्त एतत्+regions;
  }

  रक्षित:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

संरचना MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) ध्रुव
  {
#ifdef HB_NO_VAR
    पटिनिवत्त hb_scalar_cache_t::create (0);
#endif
    auto &r = एतत्+regions;
    unsigned गणनम् = r.regions.len;

    पटिनिवत्त hb_scalar_cache_t::create (गणनम्, static_cache);
  }

  स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  गुप्त:
  शून्य get_delta (unsigned पूर्णाङ्क outer, unsigned पूर्णाङ्क inner,
		  ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
#ifdef HB_NO_VAR
    पटिनिवत्त;
#endif

    सचे (unlikely (outer >= dataSets.len))
      पटिनिवत्त;

    पटिनिवत्त (एतत्+dataSets[outer]).get_delta (inner,
					     coords, coord_गणनम्,
					     एतत्+regions,
					     out,
					     cache);
  }

  सार्वजनिक:
  शून्य get_delta (unsigned पूर्णाङ्क index,
		  ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गणनम्,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    unsigned पूर्णाङ्क outer = index >> 16;
    unsigned पूर्णाङ्क inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_गणनम्, out, cache);
  }
  शून्य get_delta (unsigned पूर्णाङ्क index,
		  hb_array_t<ध्रुव पूर्णाङ्क> coords,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) ध्रुव
  {
    पटिनिवत्त get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
#ifdef HB_NO_VAR
    पटिनिवत्त सच्च;
#endif

    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, एतत्) &&
		  dataSets.sanitize (c, एतत्));
  }

  तार्किक create_subset_plan (ध्रुव hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ध्रुव
  {
    सचे (unlikely (!inner_maps->resize (dataSets.len))) पटिनिवत्त मिच्छा;
    अत्थाय (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      सचे (unlikely (outer >= dataSets.len ||
		    inner >= (एतत्+dataSets[outer]).get_item_गणनम् ()))
	पटिनिवत्त मिच्छा;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    अत्थाय (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      ध्रुव hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      सचे (!inner_map.get_population ()) अनुवर्त;
      सचे (unlikely (inner_map.in_error ())) पटिनिवत्त मिच्छा;
      अत्थाय (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    पटिनिवत्त !inner_maps->in_error () && !varidx_map->in_error ();
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव MultiItemVariationStore *src,
		  ध्रुव hb_array_t<ध्रुव hb_inc_bimap_t> &inner_maps,
		  ध्रुव hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (मिच्छा);
    format = 1;

    ध्रुव auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_गणनम् = 0;
    अत्थाय (unsigned i = 0; i < src->dataSets.len; i++)
    {
      सचे (!inner_maps[i].get_population ()) अनुवर्त;
      set_गणनम्++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    सचे (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (मिच्छा);

    dataSets.len = set_गणनम्;
    सचे (unlikely (!c->extend (dataSets))) return_trace (मिच्छा);
    unsigned set_index = 0;
    अत्थाय (unsigned i = 0; i < src->dataSets.len; i++)
    {
      सचे (!inner_maps[i].get_population ()) अनुवर्त;
      सचे (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (मिच्छा);
    }
    return_trace (सच्च);
  }

  रक्षित:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_गणनम्_T>
संरचना DeltaSetIndexMapFormat01
{
  friend संरचना DeltaSetIndexMap;

  size_t get_size () ध्रुव
  { पटिनिवत्त hb_unsigned_mul_add_saturate (map_गणनम्_2, get_width (), min_size); }

  गुप्त:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (एतत्));
  }

  template <typename T>
  तार्किक serialize (hb_serialize_context_t *c, ध्रुव T &plan)
  {
    unsigned पूर्णाङ्क width = plan.get_width ();
    unsigned पूर्णाङ्क inner_bit_गणनम् = plan.get_inner_bit_गणनम् ();
    ध्रुव hb_array_t<ध्रुव uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (output_map.length && ((((inner_bit_गणनम्-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (मिच्छा);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);

    entryFormat = ((width-1)<<4)|(inner_bit_गणनम्-1);
    map_गणनम्_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    सचे (unlikely (!p)) return_trace (मिच्छा);
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < output_map.length; i++)
    {
      unsigned पूर्णाङ्क v = output_map.arrayZ[i];
      सचे (v)
      {
	unsigned पूर्णाङ्क outer = v >> 16;
	unsigned पूर्णाङ्क inner = v & 0xFFFF;
	unsigned पूर्णाङ्क u = (outer << inner_bit_गणनम्) | inner;
	अत्थाय (unsigned पूर्णाङ्क w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (सच्च);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned पूर्णाङ्क v) ध्रुव /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    सचे (!map_गणनम्_2)
      पटिनिवत्त v;
    पटिनिवत्त _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned पूर्णाङ्क v) ध्रुव /* Returns 16.16 outer.inner. */
  {
    सचे (v >= map_गणनम्_2)
      v = map_गणनम्_2 - 1;

    unsigned पूर्णाङ्क u = 0;
    { /* Fetch it. */
      unsigned पूर्णाङ्क w = get_width ();
      ध्रुव HBUINT8 *p = mapDataZ.arrayZ + w * v;
      अत्थाय (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned पूर्णाङ्क n = get_inner_bit_गणनम् ();
      unsigned पूर्णाङ्क outer = u >> n;
      unsigned पूर्णाङ्क inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    पटिनिवत्त u;
  }

  unsigned get_map_गणनम् () ध्रुव       { पटिनिवत्त map_गणनम्_2; }
  unsigned get_width () ध्रुव           { पटिनिवत्त ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_गणनम् () ध्रुव { पटिनिवत्त (entryFormat & 0xF) + 1; }


  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_गणनम्_2,
                                  get_width ()));
  }

  रक्षित:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_गणनम्_T     map_गणनम्_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2+Map_गणनम्_T::static_size, mapDataZ);
};

संरचना DeltaSetIndexMap
{
  template <typename T>
  तार्किक serialize (hb_serialize_context_t *c, ध्रुव T &plan)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    पक्ष 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    स्वभाव:return_trace (मिच्छा);
    }
  }

  uint32_t map (unsigned v) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); पटिनिवत्त (u.format0.map (v));
    पक्ष 1: hb_barrier (); पटिनिवत्त (u.format1.map (v));
    स्वभाव:पटिनिवत्त v;
    }
  }

  unsigned get_map_गणनम् () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); पटिनिवत्त u.format0.get_map_गणनम् ();
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_map_गणनम् ();
    स्वभाव:पटिनिवत्त 0;
    }
  }

  unsigned get_width () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); पटिनिवत्त u.format0.get_width ();
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_width ();
    स्वभाव:पटिनिवत्त 0;
    }
  }

  unsigned get_inner_bit_गणनम् () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); पटिनिवत्त u.format0.get_inner_bit_गणनम् ();
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_inner_bit_गणनम् ();
    स्वभाव:पटिनिवत्त 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!u.format.v.sanitize (c)) return_trace (मिच्छा);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्वभाव:return_trace (सच्च);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    पक्ष 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    स्वभाव:return_trace (nullptr);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (1, format.v);
};


संरचना ItemVarStoreInstancer
{
  ItemVarStoreInstancer (ध्रुव ItemVariationStore *varStore_,
			 ध्रुव DeltaSetIndexMap *varIdxMap,
			 hb_array_t<ध्रुव पूर्णाङ्क> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    सचे (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    सचे (!hb_any (coords))
      एतत्->coords = {};
  }

  operator तार्किक () ध्रुव { पटिनिवत्त varStore && तार्किक (coords); }

  भिन्न operator[] (uint32_t varIdx) ध्रुव
  { पटिनिवत्त (*एतत्) (varIdx); }

  भिन्न operator() (uint32_t varIdx, unsigned short offset = 0) ध्रुव
  { पटिनिवत्त get_delta (varIdx, offset); }

  द्विगुण get_condition_delta (uint32_t varIdx) ध्रुव
  { पटिनिवत्त get_delta<द्विगुण> (varIdx); }

  template <typename Float = भिन्न>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) ध्रुव
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   सचे (!coords || varIdx == VarIdx::NO_VARIATION)
     पटिनिवत्त Float (0);

    varIdx += offset;
    सचे (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    पटिनिवत्त varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ध्रुव ItemVariationStore *varStore;
  ध्रुव DeltaSetIndexMap *varIdxMap;
  hb_array_t<ध्रुव पूर्णाङ्क> coords;
  hb_scalar_cache_t *cache;
};

संरचना MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (ध्रुव MultiItemVariationStore *varStore,
			      ध्रुव DeltaSetIndexMap *varIdxMap,
			      hb_array_t<ध्रुव पूर्णाङ्क> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    सचे (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator तार्किक () ध्रुव { पटिनिवत्त varStore; }

  द्विगुण get_condition_delta (uint32_t varIdx) ध्रुव
  { पटिनिवत्त (द्विगुण) (*एतत्)[varIdx]; }

  द्विगुण operator[] (uint32_t varIdx) ध्रुव
  {
    द्विगुण v = 0;
    (*एतत्) (hb_array (&v, 1), varIdx);
    पटिनिवत्त v;
  }

  शून्य operator() (hb_array_t<द्विगुण> out, uint32_t varIdx, unsigned short offset = 0) ध्रुव
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    सचे (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      सचे (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    अञ्ञथा
      अत्थाय (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  ध्रुव MultiItemVariationStore *varStore;
  ध्रुव DeltaSetIndexMap *varIdxMap;
  hb_array_t<ध्रुव पूर्णाङ्क> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
गणना Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

गणना lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

संरचना Condition;

template <typename Instancer>
स्थिर तार्किक
_hb_recurse_condition_evaluate (ध्रुव संरचना Condition &condition,
				ध्रुव पूर्णाङ्क *coords,
				unsigned पूर्णाङ्क coord_len,
				Instancer *instancer);

संरचना ConditionAxisRange
{
  friend संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);

    ध्रुव hb_map_t *index_map = &c->plan->axes_index_map;
    सचे (index_map->is_empty ()) return_trace (सच्च);

    ध्रुव hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    सचे (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (मिच्छा);

    ध्रुव hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    सचे (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    ध्रुव hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    सचे (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    भिन्न normalized_min = renormalize_मानम् ((द्विगुण) filter_Range_Min_मानम्.to_float (), axis_limit, axis_triple_distances, मिच्छा);
    भिन्न normalized_max = renormalize_मानम् ((द्विगुण) filter_Range_Max_मानम्.to_float (), axis_limit, axis_triple_distances, मिच्छा);
    out->filter_Range_Min_मानम्.set_float (normalized_min);
    out->filter_Range_Max_मानम्.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  गुप्त:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) ध्रुव
  {
    //invalid axis index, drop the entire record
    सचे (!c->axes_index_tag_map->has (axisIndex))
      पटिनिवत्त DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    तार्किक axis_set_by_user = मिच्छा;
    सचे (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = सच्च;
    }

    भिन्न axis_min_val = axis_range.minimum;
    भिन्न axis_default_val = axis_range.मध्यः;
    भिन्न axis_max_val = axis_range.maximum;

    भिन्न filter_min_val = filter_Range_Min_मानम्.to_float ();
    भिन्न filter_max_val = filter_Range_Max_मानम्.to_float ();

    सचे (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = मिच्छा;

    //condition not met, drop the entire record
    सचे (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      पटिनिवत्त DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    सचे (axis_set_by_user && axis_range.is_point ())
      पटिनिवत्त DROP_COND_WITH_VAR;

    सचे (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_मानम्.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_मानम्.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      पटिनिवत्त KEEP_COND_WITH_VAR;
    }
    पटिनिवत्त KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  तार्किक evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		 Instancer *instancer HB_UNUSED) ध्रुव
  {
    पूर्णाङ्क coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    पटिनिवत्त filter_Range_Min_मानम्.to_int () <= coord && coord <= filter_Range_Max_मानम्.to_int ();
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_मानम्;
  F2DOT14	filter_Range_Max_मानम्;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना Condition_मानम्
{
  friend संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);

    सचे (varIdx == VarIdx::NO_VARIATION)
      return_trace (सच्च);

    hb_pair_t<unsigned, पूर्णाङ्क> *condition_मानम्;
    सचे (c->plan->layout_condition_idx_मानम्_map.has ((uintptr_t) एतत्, &condition_मानम्))
    {
      out->default_मानम्_2 = condition_मानम्->second;
      out->varIdx = condition_मानम्->first;
      return_trace (सच्च);
    }

    hb_pair_t<unsigned, पूर्णाङ्क> *new_varidx_delta;
    सचे (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (मिच्छा);

    सचे (unlikely (!c->serializer->check_assign (
		out->default_मानम्_2,
		default_मानम्_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (मिच्छा);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (सच्च);
  }

  गुप्त:
  template <typename Instancer>
  तार्किक evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		 Instancer *instancer) ध्रुव
  {
    द्विगुण value = default_मानम्_2;
    value += instancer->get_condition_delta (varIdx);
    पटिनिवत्त value > 0;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_मानम्_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना ConditionAnd
{
  friend संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    सचे (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (मिच्छा);
    out->format = format;

    अत्थाय (ध्रुव auto &offset : conditions)
    {
      सचे (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	सचे (unlikely (!out_offset)) return_trace (मिच्छा);
	*out_offset = 0;
      }
      अञ्ञथा सचे (unlikely (!subset_offset_array (c, out->conditions, एतत्) (offset)))
	return_trace (मिच्छा);
    }

    return_trace (सच्च);
  }

  गुप्त:
  template <typename Instancer>
  तार्किक evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		 Instancer *instancer) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = conditions.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (!_hb_recurse_condition_evaluate (एतत्+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	पटिनिवत्त मिच्छा;
    पटिनिवत्त सच्च;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (conditions.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<संरचना Condition>	conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (3, conditions);
};

संरचना ConditionOr
{
  friend संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    सचे (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (मिच्छा);
    out->format = format;

    अत्थाय (ध्रुव auto &offset : conditions)
    {
      सचे (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	सचे (unlikely (!out_offset)) return_trace (मिच्छा);
	*out_offset = 0;
      }
      अञ्ञथा सचे (unlikely (!subset_offset_array (c, out->conditions, एतत्) (offset)))
	return_trace (मिच्छा);
    }

    return_trace (सच्च);
  }

  गुप्त:
  template <typename Instancer>
  तार्किक evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		 Instancer *instancer) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = conditions.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (_hb_recurse_condition_evaluate (एतत्+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	पटिनिवत्त सच्च;
    पटिनिवत्त मिच्छा;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (conditions.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<संरचना Condition>	conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (3, conditions);
};

संरचना ConditionNegate
{
  friend संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);
    सचे (condition.is_null ())
    {
      out->condition = 0;
      return_trace (सच्च);
    }
    return_trace (out->condition.serialize_subset (c, condition, एतत्));
  }

  गुप्त:
  template <typename Instancer>
  तार्किक evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		 Instancer *instancer) ध्रुव
  {
    पटिनिवत्त !_hb_recurse_condition_evaluate (एतत्+condition,
					    coords, coord_len,
					    instancer);
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (condition.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<संरचना Condition>	condition;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (5);
};

संरचना Condition
{
  friend संरचना ConditionSet;

  स्थिर तार्किक serialize_constant (hb_serialize_context_t *c, तार्किक value)
  {
    auto *out = c->start_embed<Condition_मानम्> ();
    सचे (unlikely (!out || !c->extend_min (out))) पटिनिवत्त मिच्छा;
    out->format = 2;
    out->default_मानम्_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    पटिनिवत्त सच्च;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      तार्किक materialize_true = मिच्छा) ध्रुव;

  template <typename Instancer>
  तार्किक evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		 Instancer *instancer) ध्रुव
  {
    विकल्प (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    पक्ष 0: पटिनिवत्त सच्च;
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.evaluate (coords, coord_len, instancer);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.evaluate (coords, coord_len, instancer);
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.evaluate (coords, coord_len, instancer);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.evaluate (coords, coord_len, instancer);
    पक्ष 5: hb_barrier (); पटिनिवत्त u.format5.evaluate (coords, coord_len, instancer);
    स्वभाव:पटिनिवत्त मिच्छा;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) ध्रुव
  {
    सचे (unlikely (!depth))
    {
      c->apply = मिच्छा;
      पटिनिवत्त DROP_RECORD_WITH_VAR;
    }

    विकल्प (u.format.v) {
    पक्ष 0: पटिनिवत्त KEEP_RECORD_WITH_VAR;
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.keep_with_variations (c, condition_map);
    पक्ष 2: पक्ष 3: पक्ष 4: पक्ष 5: भङ्ग;
    स्वभाव: c->apply = मिच्छा; पटिनिवत्त DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    तार्किक applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    सचे (c->all_axes_pinned)
      पटिनिवत्त applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    सचे (u.format.v == 2)
      पटिनिवत्त u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (ध्रुव Condition &child)
    {
      auto child_context = *c;
      child_context.apply = सच्च;
      पटिनिवत्त child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    सचे (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      पटिनिवत्त result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    तार्किक is_and = u.format.v == 3;
    तार्किक variable = मिच्छा;
    ध्रुव auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ध्रुव शून्य *base = is_and ? (ध्रुव शून्य *) &u.format3 : (ध्रुव शून्य *) &u.format4;
    अत्थाय (ध्रुव auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      सचे (is_and && result == DROP_RECORD_WITH_VAR)
        पटिनिवत्त DROP_RECORD_WITH_VAR;
      सचे (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        पटिनिवत्त KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    पटिनिवत्त variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    सचे (unlikely (!c->may_dispatch (एतत्, &u.format.v))) पटिनिवत्त c->no_dispatch_return_मानम् ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    पक्ष 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    स्वभाव:return_trace (c->default_return_मानम् ());
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    सचे (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_मानम् ());

    return_trace (c->end_recursion (एतत्->dispatch (c)));
  }

  तार्किक collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, पूर्णाङ्क>> *condition_मानानि = nullptr) ध्रुव;

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव Condition *src,
		  ध्रुव hb_map_t &varidx_map,
		  ध्रुव hb_map_t *axis_map = nullptr);

  गुप्त:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) ध्रुव;

  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_मानम्	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
तार्किक
_hb_recurse_condition_evaluate (ध्रुव संरचना Condition &condition,
				ध्रुव पूर्णाङ्क *coords,
				unsigned पूर्णाङ्क coord_len,
				Instancer *instancer)
{
  पटिनिवत्त condition.evaluate (coords, coord_len, instancer);
}

संरचना ConditionList
{
  ध्रुव Condition& operator[] (unsigned i) ध्रुव
  { पटिनिवत्त एतत्+conditions[i]; }

  unsigned get_गणनम् () ध्रुव { पटिनिवत्त conditions.len; }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव ConditionList *src,
		  ध्रुव hb_inc_bimap_t &condition_map,
		  ध्रुव hb_map_t &varidx_map,
		  ध्रुव hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    conditions.len = condition_map.get_population ();
    सचे (unlikely (!c->extend (conditions))) return_trace (मिच्छा);
    अत्थाय (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      सचे (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (मिच्छा);
    }
    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (conditions.sanitize (c, एतत्));
  }

  रक्षित:
  Array32OfOffset32To<Condition> conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, conditions);
};

अन्तःस्थ तार्किक
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, पूर्णाङ्क>> *condition_मानानि) ध्रुव
{
  सचे (unlikely (!depth)) पटिनिवत्त मिच्छा;
  विकल्प (u.format.v)
  {
    पक्ष 0:
    पक्ष 1:
      पटिनिवत्त सच्च;
    पक्ष 2:
      सचे (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        सचे (condition_मानानि &&
            !condition_मानानि->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), पूर्णाङ्क (u.format2.default_मानम्_2))))
          पटिनिवत्त मिच्छा;
      }
      पटिनिवत्त !var_indices->in_error ();
    पक्ष 3:
      अत्थाय (ध्रुव auto &offset : u.format3.conditions)
	सचे (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_मानानि)))
	  पटिनिवत्त मिच्छा;
      पटिनिवत्त सच्च;
    पक्ष 4:
      अत्थाय (ध्रुव auto &offset : u.format4.conditions)
	सचे (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_मानानि)))
	  पटिनिवत्त मिच्छा;
      पटिनिवत्त सच्च;
    पक्ष 5:
      पटिनिवत्त (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_मानानि);
    स्वभाव:
      पटिनिवत्त मिच्छा;
  }
}

अन्तःस्थ तार्किक
Condition::serialize (hb_serialize_context_t *c,
		      ध्रुव Condition *src,
		      ध्रुव hb_map_t &varidx_map,
		      ध्रुव hb_map_t *axis_map)
{
  TRACE_SERIALIZE (एतत्);
  विकल्प (src->u.format.v)
  {
    पक्ष 0:
    {
      auto *out = c->start_embed<Condition_मानम्> ();
      सचे (unlikely (!out || !c->extend_min (out))) return_trace (मिच्छा);
      out->format = 2;
      out->default_मानम्_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (सच्च);
    }
    पक्ष 1:
    {
      auto *out = c->embed (&src->u.format1);
      सचे (unlikely (!out)) return_trace (मिच्छा);
      सचे (axis_map)
      {
	सचे (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (मिच्छा);
	}
	सचे (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (मिच्छा);
      }
      return_trace (सच्च);
    }
    पक्ष 2:
    {
      auto *out = c->embed (&src->u.format2);
      सचे (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (मिच्छा);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (सच्च);
    }
    पक्ष 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      सचे (unlikely (!c->extend_min (out))) return_trace (मिच्छा);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      सचे (unlikely (!c->extend (out->conditions))) return_trace (मिच्छा);
      अत्थाय (unsigned i = 0; i < out->conditions.len; i++)
	सचे (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (मिच्छा);
      return_trace (सच्च);
    }
    पक्ष 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      सचे (unlikely (!c->extend_min (out))) return_trace (मिच्छा);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      सचे (unlikely (!c->extend (out->conditions))) return_trace (मिच्छा);
      अत्थाय (unsigned i = 0; i < out->conditions.len; i++)
	सचे (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (मिच्छा);
      return_trace (सच्च);
    }
    पक्ष 5:
    {
      auto *out = c->embed (&src->u.format5);
      सचे (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (मिच्छा);
      return_trace (सच्च);
    }
    स्वभाव:
      return_trace (मिच्छा);
  }
}

अन्तःस्थ lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    तार्किक materialize_true) ध्रुव
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  सचे (result == LOOKUP_CONDITION_SUBSET_KEEP)
    पटिनिवत्त result;

  c->serializer->revert (snap);
  सचे (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    पटिनिवत्त result;

  पटिनिवत्त serialize_constant (c->serializer, सच्च) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

अन्तःस्थ lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) ध्रुव
{
  सचे (unlikely (!depth))
    पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   ध्रुव Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    सचे (unlikely (!offset))
      पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    सचे (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->निष्कासय_pack ());
    अञ्ञथा
    {
      c->serializer->निष्कासय_discard ();
      out.निष्कासय ();
      c->serializer->revert (snap);
    }
    पटिनिवत्त result;
  };

  विकल्प (u.format.v)
  {
    पक्ष 0:
      पटिनिवत्त LOOKUP_CONDITION_SUBSET_TRUE;

    पक्ष 1:
    {
      ध्रुव ConditionAxisRange &src = u.format1;
      सचे (c->plan->user_axes_location.is_empty ())
        पटिनिवत्त c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      सचे (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      सचे (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      भिन्न filter_min = src.filter_Range_Min_मानम्.to_float ();
      भिन्न filter_max = src.filter_Range_Max_मानम्.to_float ();
      सचे (filter_min > filter_max)
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_FALSE;
      सचे (axis_limit.is_point ())
        पटिनिवत्त filter_min <= axis_limit.मध्यः &&
	       axis_limit.मध्यः <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      सचे (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_FALSE;
      सचे (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_TRUE;
      सचे (!c->plan->axes_index_map.has (src.axisIndex))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      सचे (unlikely (!out))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      सचे (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_मानम्.set_float (
	  renormalize_मानम् (filter_min, axis_limit, axis_distances, मिच्छा));
      out->filter_Range_Max_मानम्.set_float (
	  renormalize_मानम् (filter_max, axis_limit, axis_distances, मिच्छा));
      सचे (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;
      पटिनिवत्त LOOKUP_CONDITION_SUBSET_KEEP;
    }

    पक्ष 2:
    {
      ध्रुव Condition_मानम् &src = u.format2;
      auto *out = c->serializer->embed (&src);
      सचे (unlikely (!out))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;

      सचे (src.varIdx == VarIdx::NO_VARIATION)
        पटिनिवत्त c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((पूर्णाङ्क) src.default_मानम्_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, पूर्णाङ्क> *condition_मानम्;
      सचे (c->plan->layout_condition_idx_मानम्_map.has ((uintptr_t) &src, &condition_मानम्))
      {
        out->default_मानम्_2 = condition_मानम्->second;
        out->varIdx = condition_मानम्->first;
        सचे (out->varIdx == VarIdx::NO_VARIATION)
          पटिनिवत्त out->default_मानम्_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, पूर्णाङ्क> *new_varidx_delta;
      सचे (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        पटिनिवत्त c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((पूर्णाङ्क) src.default_मानम्_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      सचे (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        द्विगुण value = (पूर्णाङ्क) src.default_मानम्_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        पटिनिवत्त value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      पूर्णाङ्क default_मानम् = (पूर्णाङ्क) src.default_मानम्_2 +
			  hb_second (*new_varidx_delta);
      सचे (unlikely (!c->serializer->check_assign (
		out->default_मानम्_2,
		default_मानम्,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      पटिनिवत्त LOOKUP_CONDITION_SUBSET_KEEP;
    }

    पक्ष 3:
    {
      ध्रुव ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      सचे (unlikely (!out || !c->serializer->extend_min (out)))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      अत्थाय (ध्रुव auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        सचे (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          पटिनिवत्त result;
      }
      पटिनिवत्त out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    पक्ष 4:
    {
      ध्रुव ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      सचे (unlikely (!out || !c->serializer->extend_min (out)))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      अत्थाय (ध्रुव auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        सचे (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          पटिनिवत्त result;
      }
      पटिनिवत्त out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    पक्ष 5:
    {
      ध्रुव ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      सचे (unlikely (!out))
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      सचे (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->निष्कासय_pack ());
        पटिनिवत्त result;
      }

      c->serializer->निष्कासय_discard ();
      सचे (result == LOOKUP_CONDITION_SUBSET_TRUE)
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_FALSE;
      सचे (result == LOOKUP_CONDITION_SUBSET_FALSE)
        पटिनिवत्त LOOKUP_CONDITION_SUBSET_TRUE;
      पटिनिवत्त result;
    }

    स्वभाव:
      पटिनिवत्त LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

संरचना ConditionSet
{
  तार्किक evaluate (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		 ItemVarStoreInstancer *instancer) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = conditions.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (!(एतत्+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	पटिनिवत्त मिच्छा;
    पटिनिवत्त सच्च;
  }

  शून्य keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ध्रुव
  {
    hb_map_t *condition_map = hb_map_create ();
    सचे (unlikely (!condition_map)) पटिनिवत्त;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    सचे (unlikely (!cond_set)) पटिनिवत्त;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = सच्च;
    तार्किक should_keep = मिच्छा;
    /* The range map cannot identify value or boolean expressions uniquely. */
    तार्किक can_deduplicate = सच्च;
    unsigned अङ्कः_kept_cond = 0, cond_idx = 0;
    अत्थाय (ध्रुव auto& offset : conditions)
    {
      ध्रुव Condition &condition = एतत्+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      सचे (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = मिच्छा;
        पटिनिवत्त;
      }

      सचे (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = सच्च;
        cond_set->add (cond_idx);
        अङ्कः_kept_cond++;
      }

      सचे (ret == KEEP_RECORD_WITH_VAR)
        should_keep = सच्च;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = अङ्कः_kept_cond == 0;
    सचे (!should_keep && c->all_axes_pinned) पटिनिवत्त;

    //check if condition_set is unique with variations
    सचे (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      पटिनिवत्त;

    सचे (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  तार्किक subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               तार्किक निवेशय_catch_all) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    सचे (unlikely (!out || !c->serializer->extend_min (out))) return_trace (मिच्छा);

    सचे (निवेशय_catch_all) return_trace (सच्च);

    hb_set_t *retained_cond_set = nullptr;
    सचे (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned पूर्णाङ्क गणनम् = conditions.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      सचे (retained_cond_set != nullptr && !retained_cond_set->has (i))
        अनुवर्त;
      ध्रुव Condition &condition = एतत्+conditions[i];
      सचे (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, एतत्) (conditions[i]);
        अनुवर्त;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      सचे (unlikely (!offset)) return_trace (मिच्छा);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, सच्च);
      सचे (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, मिच्छा) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      सचे (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->निष्कासय_discard ();
        c->serializer->check_success (मिच्छा);
        return_trace (मिच्छा);
      }
      c->serializer->add_link (*offset, c->serializer->निष्कासय_pack ());
    }

    return_trace (तार्किक (out->conditions));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (conditions.sanitize (c, एतत्));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    अत्थाय (ध्रुव auto &offset : conditions)
      सचे (unlikely (!(एतत्+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_मानानि)))
	c->layout_variation_indices->err ();
  }

  रक्षित:
  Array16OfOffset32To<Condition>	conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, conditions);
};

संरचना FeatureTableSubstitutionRecord
{
  friend संरचना FeatureTableSubstitution;

  शून्य collect_lookups (ध्रुव शून्य *base, hb_set_t *lookup_indexes /* OUT */) ध्रुव
  {
    पटिनिवत्त (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  शून्य closure_features (ध्रुव शून्य *base,
			 ध्रुव hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ध्रुव
  {
    सचे ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  शून्य collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, ध्रुव Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    ध्रुव hb_set_t *feature_indices,
                                                    ध्रुव शून्य *base) ध्रुव
  {
    सचे (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  तार्किक serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  ध्रुव Feature *f, ध्रुव Tag *tag)
  {
    TRACE_SERIALIZE (एतत्);
    hb_serialize_context_t *s = c->subset_context->serializer;
    सचे (unlikely (!s->extend_min (एतत्))) return_trace (मिच्छा);

    uint32_t *new_feature_idx;
    सचे (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (मिच्छा);

    सचे (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छा);

    s->push ();
    तार्किक ret = f->subset (c->subset_context, c, tag);
    सचे (ret) s->add_link (feature, s->निष्कासय_pack ());
    अञ्ञथा s->निष्कासय_discard ();

    return_trace (ret);
  }

  तार्किक subset (hb_subset_layout_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    uint32_t *new_feature_index;
    सचे (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (मिच्छा);

    auto *out = c->subset_context->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);

    out->featureIndex = *new_feature_index;
    hb_pair_t<ध्रुव शून्य*, ध्रुव शून्य*> *original;
    सचे (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (मिच्छा);
    ध्रुव Tag *tag = reinterpret_cast<ध्रुव Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base,
		 ध्रुव FeatureList *feature_list) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!c->check_struct (एतत्))) return_trace (मिच्छा);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ध्रुव Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  रक्षित:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना FeatureTableSubstitution
{
  शून्य collect_feature_indices (hb_set_t *feature_indices) ध्रुव
  {
    अत्थाय (ध्रुव FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  ध्रुव Feature *find_substitute (unsigned पूर्णाङ्क feature_index) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = substitutions.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      ध्रुव FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      सचे (record.featureIndex == feature_index)
	पटिनिवत्त &(एतत्+record.feature);
    }
    पटिनिवत्त nullptr;
  }

  शून्य collect_lookups (ध्रुव hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ध्रुव
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([एतत्, lookup_indexes] (ध्रुव FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (एतत्, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  शून्य collect_name_ids (ध्रुव hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ध्रुव
  {
    अत्थाय (ध्रुव FeatureTableSubstitutionRecord &record : substitutions)
      सचे (feature_index_map->has (record.featureIndex))
        (एतत्+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  शून्य closure_features (ध्रुव hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ध्रुव
  {
    अत्थाय (ध्रुव FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (एतत्, lookup_indexes, feature_indexes);
  }

  तार्किक intersects_features (ध्रुव hb_map_t *feature_index_map) ध्रुव
  {
    अत्थाय (ध्रुव FeatureTableSubstitutionRecord& record : substitutions)
    {
      सचे (feature_index_map->has (record.featureIndex)) पटिनिवत्त सच्च;
    }
    पटिनिवत्त मिच्छा;
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ध्रुव
  {
    अत्थाय (ध्रुव FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, एतत्);
  }

  तार्किक subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               तार्किक निवेशय_catch_all) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!out || !c->serializer->extend_min (out))) return_trace (मिच्छा);

    out->version.major = version.major;
    out->version.minor = version.minor;

    सचे (निवेशय_catch_all)
    {
      अत्थाय (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<ध्रुव शून्य*, ध्रुव शून्य*> *p;
        सचे (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (मिच्छा);
        auto *o = out->substitutions.serialize_append (c->serializer);
        सचे (!o->serialize (l, feature_index,
                           reinterpret_cast<ध्रुव Feature*> (p->first),
                           reinterpret_cast<ध्रुव Tag*> (p->second)))
          return_trace (मिच्छा);
      }
      return_trace (सच्च);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), एतत्))
    ;

    return_trace (तार्किक (out->substitutions));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव FeatureList *feature_list) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, एतत्, feature_list));
  }

  रक्षित:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

संरचना FeatureVariationRecord
{
  friend संरचना FeatureVariations;

  शून्य collect_lookups (ध्रुव शून्य     *base,
			ध्रुव hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ध्रुव
  {
    पटिनिवत्त (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  शून्य collect_name_ids (ध्रुव शून्य *base,
                         ध्रुव hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ध्रुव
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  शून्य closure_features (ध्रुव शून्य     *base,
			 ध्रुव hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ध्रुव
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  तार्किक intersects_features (ध्रुव शून्य *base, ध्रुव hb_map_t *feature_index_map) ध्रुव
  {
    पटिनिवत्त (base+substitutions).intersects_features (feature_index_map);
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ध्रुव शून्य *base) ध्रुव
  {
    (base+conditions).keep_with_variations (c);
    सचे (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = सच्च; // set variations only once
    }
  }

  तार्किक subset (hb_subset_layout_context_t *c, ध्रुव शून्य *base,
               तार्किक निवेशय_catch_all = मिच्छा) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->subset_context->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    सचे (!निवेशय_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    अञ्ञथा
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, निवेशय_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, निवेशय_catch_all);

    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base,
		 ध्रुव FeatureList *feature_list) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ध्रुव शून्य *base) ध्रुव
  { (base+conditions).collect_variation_indices (c); }

  रक्षित:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना LookupIndexList
{
  शून्य add_lookup_indexes_to (hb_set_t *lookup_indexes) ध्रुव
  { lookupIndices.add_indexes_to (lookup_indexes); }

  तार्किक intersects_lookup_indexes (ध्रुव hb_map_t *lookup_indexes) ध्रुव
  { पटिनिवत्त lookupIndices.intersects (lookup_indexes); }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    सचे (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (मिच्छा);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (तार्किक (out->lookupIndices));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (lookupIndices.sanitize (c));
  }

  रक्षित:
  IndexArray	lookupIndices;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

संरचना LookupConditionRecord
{
  unsigned get_condition_offset (ध्रुव शून्य *base, ध्रुव शून्य *variations) ध्रुव
  { पटिनिवत्त (ध्रुव वर्ण *) &(base+condition) - (ध्रुव वर्ण *) variations; }

  template <typename Instancer>
  तार्किक evaluate (ध्रुव शून्य *base,
		 ध्रुव पूर्णाङ्क *coords,
		 unsigned पूर्णाङ्क coord_len,
		 Instancer *instancer) ध्रुव
  { पटिनिवत्त (base+condition).evaluate (coords, coord_len, instancer); }

  तार्किक evaluate (ध्रुव शून्य *base,
                 ध्रुव शून्य *variations,
                 hb_sorted_array_t<ध्रुव unsigned> conditions) ध्रुव
  {
    पटिनिवत्त !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     परिमाण (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  तार्किक collect_condition (ध्रुव शून्य *base,
                          ध्रुव शून्य *variations,
                          ध्रुव पूर्णाङ्क *coords,
                          unsigned पूर्णाङ्क coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) ध्रुव
  {
    /* Null conditions always apply and need no key state. */
    सचे (!condition) पटिनिवत्त सच्च;

    unsigned offset = get_condition_offset (base, variations);
    सचे (visited->has (offset)) पटिनिवत्त सच्च;
    visited->add (offset);
    सचे (unlikely (visited->in_error ())) पटिनिवत्त मिच्छा;

    सचे (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    पटिनिवत्त !conditions->in_error ();
  }

  शून्य add_lookup_indexes_to (ध्रुव शून्य *base,
			      hb_set_t *lookup_indexes) ध्रुव
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  तार्किक intersects_lookup_indexes (ध्रुव शून्य *base,
				   ध्रुव hb_map_t *lookup_indexes) ध्रुव
  { पटिनिवत्त (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ध्रुव शून्य *base) ध्रुव
  {
    सचे (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_मानानि)))
      c->layout_variation_indices->err ();
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    सच्च);
    सचे (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->निष्कासय_discard ();
      return_trace (मिच्छा);
    }
    c->serializer->add_link (out->condition, c->serializer->निष्कासय_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  रक्षित:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना FeatureLookupsTable
{
  तार्किक adds_default_lookups () ध्रुव { पटिनिवत्त flags & 0x0001u; }

  तार्किक collect_conditions (ध्रुव शून्य *variations,
                           ध्रुव पूर्णाङ्क *coords,
                           unsigned पूर्णाङ्क coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) ध्रुव
  {
    अत्थाय (ध्रुव LookupConditionRecord &record : records)
      सचे (unlikely (!record.collect_condition (एतत्, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	पटिनिवत्त मिच्छा;
    पटिनिवत्त सच्च;
  }

  शून्य collect_lookups (ध्रुव शून्य *variations,
                       hb_sorted_array_t<ध्रुव unsigned> conditions,
                       hb_set_t *lookup_indexes) ध्रुव
  {
    अत्थाय (ध्रुव LookupConditionRecord &record : records)
      सचे (record.evaluate (एतत्, variations, conditions))
	record.add_lookup_indexes_to (एतत्, lookup_indexes);
  }

  template <typename Instancer>
  शून्य collect_lookups (ध्रुव पूर्णाङ्क *coords,
			unsigned पूर्णाङ्क coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) ध्रुव
  {
    अत्थाय (ध्रुव LookupConditionRecord &record : records)
      सचे (record.evaluate (एतत्, coords, coord_len, instancer))
	record.add_lookup_indexes_to (एतत्, lookup_indexes);
  }

  शून्य collect_lookups (hb_set_t *lookup_indexes) ध्रुव
  {
    अत्थाय (ध्रुव LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (एतत्, lookup_indexes);
  }

  तार्किक intersects_lookup_indexes (ध्रुव hb_map_t *lookup_indexes) ध्रुव
  {
    अत्थाय (ध्रुव LookupConditionRecord &record : records)
      सचे (record.intersects_lookup_indexes (एतत्, lookup_indexes))
	पटिनिवत्त सच्च;
    पटिनिवत्त मिच्छा;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    अत्थाय (ध्रुव LookupConditionRecord &record : records)
      record.collect_variation_indices (c, एतत्);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (एतत्);
    सचे (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (मिच्छा);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    अत्थाय (ध्रुव LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      सचे (record.subset (c, l, एतत्))
	out->records.len++;
      अञ्ञथा
	c->serializer->revert (snap);
    }

    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, एतत्));
  }

  रक्षित:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (10, records);
};

संरचना LookupVariationRecord
{
  friend संरचना FeatureVariations;

  unsigned get_feature_index () ध्रुव { पटिनिवत्त featureIndex; }

  शून्य collect_lookups (ध्रुव शून्य *base,
                       ध्रुव Feature &feature,
                       hb_sorted_array_t<ध्रुव unsigned> conditions,
                       hb_set_t *lookup_indexes) ध्रुव
  {
    ध्रुव FeatureLookupsTable &feature_lookups = base+featureLookups;
    सचे (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  शून्य collect_lookups (ध्रुव शून्य *base,
			ध्रुव hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) ध्रुव
  {
    सचे (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  शून्य closure_features (ध्रुव शून्य *base,
			 ध्रुव hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) ध्रुव
  {
    सचे ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ध्रुव शून्य *base) ध्रुव
  { (base+featureLookups).collect_variation_indices (c); }

  तार्किक subset (hb_subset_layout_context_t *l, ध्रुव शून्य *base) ध्रुव
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (एतत्);
    uint32_t *new_feature_index;
    सचे (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (मिच्छा);

    auto *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (मिच्छा);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  featureLookups.sanitize (c, base));
  }

  रक्षित:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना FeatureVariations
{
  स्थिर constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  ध्रुव Array32Of<LookupVariationRecord> &get_lookup_variation_records () ध्रुव
  {
    सचे (version.to_int () < 0x00010001u)
      पटिनिवत्त Null (Array32Of<LookupVariationRecord>);
    पटिनिवत्त StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_गणनम् () ध्रुव
  {
    पटिनिवत्त varRecords.len;
  }

  तार्किक find_index (ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_len,
		   unsigned पूर्णाङ्क *index,
		   ItemVarStoreInstancer *instancer) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = varRecords.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      ध्रुव FeatureVariationRecord &record = varRecords.arrayZ[i];
      सचे ((एतत्+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	पटिनिवत्त सच्च;
      }
    }
    *index = NOT_FOUND_INDEX;
    पटिनिवत्त मिच्छा;
  }

  ध्रुव Feature *find_substitute (unsigned पूर्णाङ्क variations_index,
				  unsigned पूर्णाङ्क feature_index) ध्रुव
  {
    ध्रुव FeatureVariationRecord &record = varRecords[variations_index];
    पटिनिवत्त (एतत्+record.substitutions).find_substitute (feature_index);
  }

  तार्किक collect_lookup_conditions (ध्रुव पूर्णाङ्क *coords,
                                  unsigned पूर्णाङ्क coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) ध्रुव
  {
    hb_set_t visited_tables, visited_conditions;
    अत्थाय (ध्रुव LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      सचे (visited_tables.has (offset)) अनुवर्त;
      visited_tables.add (offset);
      सचे (unlikely (visited_tables.in_error ())) पटिनिवत्त मिच्छा;

      ध्रुव FeatureLookupsTable &feature_lookups = एतत्+record.featureLookups;
      सचे (unlikely (!feature_lookups.collect_conditions (एतत्, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	पटिनिवत्त मिच्छा;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    पटिनिवत्त सच्च;
  }

  तार्किक get_lookup_variations (unsigned feature_index,
                              ध्रुव Feature &feature,
                              hb_sorted_array_t<ध्रुव unsigned> conditions,
                              hb_set_t *lookup_indexes) ध्रुव
  {
    अत्थाय (ध्रुव LookupVariationRecord &record : get_lookup_variation_records ())
    {
      सचे (record.featureIndex != feature_index) अनुवर्त;

      record.collect_lookups (एतत्, feature, conditions, lookup_indexes);
      पटिनिवत्त सच्च;
    }
    पटिनिवत्त मिच्छा;
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = varRecords.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, एतत्);
      सचे (c->universal)
        भङ्ग;
    }
    सचे (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (*एतत्));
  }

  शून्य collect_lookups (ध्रुव hb_set_t *feature_indexes,
			ध्रुव hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) ध्रुव
  {
    unsigned गणनम् = varRecords.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      सचे (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        अनुवर्त;
      varRecords[i].collect_lookups (एतत्, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  शून्य collect_name_ids (ध्रुव hb_map_t *feature_index_map,
                         ध्रुव hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ध्रुव
  {
    अत्थाय (unsigned i = 0; i < varRecords.len; i++)
    {
      सचे (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        अनुवर्त;
      varRecords[i].collect_name_ids (एतत्, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  शून्य collect_lookup_variation_lookups (ध्रुव hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) ध्रुव
  {
    अत्थाय (ध्रुव LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (एतत्, feature_indexes, lookup_indexes);
  }

  शून्य closure_features (ध्रुव hb_map_t *lookup_indexes,
			 ध्रुव hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = varRecords.len;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      सचे (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        अनुवर्त;
      varRecords[i].closure_features (एतत्, lookup_indexes, feature_indexes);
    }

    अत्थाय (ध्रुव LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (एतत्, lookup_indexes, feature_indexes);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    अत्थाय (ध्रुव FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, एतत्);
    अत्थाय (ध्रुव LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, एतत्);
  }

  शून्य collect_feature_indices (hb_set_t *feature_indices) ध्रुव
  {
    अत्थाय (ध्रुव FeatureVariationRecord &record : varRecords)
      (एतत्+record.substitutions).collect_feature_indices (feature_indices);
  }

  तार्किक has_lookup_variations (unsigned feature_index) ध्रुव
  {
    अत्थाय (ध्रुव LookupVariationRecord &record : get_lookup_variation_records ())
    {
      सचे (record.featureIndex == feature_index)
	पटिनिवत्त सच्च;
      सचे (record.featureIndex > feature_index)
	भङ्ग;
    }
    पटिनिवत्त मिच्छा;
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!out || !c->serializer->extend_min (out))) return_trace (मिच्छा);

    out->version.major = version.major;
    out->version.minor = version.minor;

    पूर्णाङ्क keep_up_to = -1;
    अत्थाय (पूर्णाङ्क i = varRecords.len - 1; i >= 0; i--) {
      सचे (varRecords[i].intersects_features (एतत्, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        भङ्ग;
      }
    }

    unsigned गणनम् = (unsigned) (keep_up_to + 1);
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      सचे (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        अनुवर्त;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), एतत्) (varRecords[i]);
    }

    सचे (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      तार्किक निवेशय_catch_all_record = सच्च;
      subset_record_array (l, &(out->varRecords), एतत्, निवेशय_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    सचे (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      सचे (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (मिच्छा);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, एतत्))
      ;
    }

    return_trace (तार्किक (out->varRecords) ||
		  (out_lookup_records && तार्किक (*out_lookup_records)));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव FeatureList *feature_list) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, एतत्, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, एतत्)));
  }

  रक्षित:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

संरचना HintingDevice
{
  friend संरचना Device;

  गुप्त:

  hb_position_t get_x_delta (hb_font_t *font) ध्रुव
  { पटिनिवत्त get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ध्रुव
  { पटिनिवत्त get_delta (font->y_ppem, font->y_scale); }

  सार्वजनिक:

  size_t get_size () ध्रुव
  {
    unsigned पूर्णाङ्क f = deltaFormat;
    सचे (unlikely (f < 1 || f > 3 || startSize > endSize)) पटिनिवत्त 3 * HBUINT16::static_size;
    पटिनिवत्त HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && c->check_range (एतत्, एतत्->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed<HintingDevice> (एतत्));
  }

  गुप्त:

  पूर्णाङ्क get_delta (unsigned पूर्णाङ्क ppem, पूर्णाङ्क scale) ध्रुव
  {
    सचे (!ppem) पटिनिवत्त 0;

    पूर्णाङ्क pixels = get_delta_pixels (ppem);

    सचे (!pixels) पटिनिवत्त 0;

    पटिनिवत्त (पूर्णाङ्क) (pixels * (int64_t) scale / ppem);
  }
  पूर्णाङ्क get_delta_pixels (unsigned पूर्णाङ्क ppem_size) ध्रुव
  {
    unsigned पूर्णाङ्क f = deltaFormat;
    सचे (unlikely (f < 1 || f > 3))
      पटिनिवत्त 0;

    सचे (ppem_size < startSize || ppem_size > endSize)
      पटिनिवत्त 0;

    unsigned पूर्णाङ्क s = ppem_size - startSize;

    unsigned पूर्णाङ्क byte = delta_मानम्_Z[s >> (4 - f)];
    unsigned पूर्णाङ्क bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned पूर्णाङ्क mask = (0xFFFFu >> (16 - (1 << f)));

    पूर्णाङ्क delta = bits & mask;

    सचे ((unsigned पूर्णाङ्क) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    पटिनिवत्त delta;
  }

  रक्षित:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_मानम्_Z;		/* Array of compressed data */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, delta_मानम्_Z);
};

संरचना VariationDevice
{
  friend संरचना Device;

  गुप्त:

  hb_position_t get_x_delta (hb_font_t *font,
			     ध्रुव ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  { पटिनिवत्त !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ध्रुव ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  { पटिनिवत्त !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  भिन्न get_x_delta_f (hb_font_t *font,
		       ध्रुव ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ध्रुव
  { पटिनिवत्त !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  भिन्न get_y_delta_f (hb_font_t *font,
		       ध्रुव ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ध्रुव
  { पटिनिवत्त !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    सचे (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, पूर्णाङ्क> *v;
    सचे (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    सचे (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (एतत्->static_size);
    auto *out = c->embed (एतत्);
    सचे (unlikely (!out)) return_trace (nullptr);

    सचे (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  शून्य collect_variation_index (hb_collect_variation_indices_context_t *c) ध्रुव
  { c->layout_variation_indices->add (varIdx); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  गुप्त:

  भिन्न get_delta (hb_font_t *font,
		   ध्रुव ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
    पटिनिवत्त store.get_delta (varIdx, font->coords, font->अङ्कः_coords, store_cache);
  }

  रक्षित:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना DeviceHeader
{
  रक्षित:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  सार्वजनिक:
  HBUINT16		format;		/* Format identifier */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  भिन्न get_x_delta_f (hb_font_t *font,
		       ध्रुव ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
#ifndef HB_NO_VAR
    सचे (u.b.format == 0x8000)
    {
      hb_barrier ();
      पटिनिवत्त u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    पटिनिवत्त get_x_delta (font, store, store_cache);
  }

  भिन्न get_y_delta_f (hb_font_t *font,
		       ध्रुव ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
#ifndef HB_NO_VAR
    सचे (u.b.format == 0x8000)
    {
      hb_barrier ();
      पटिनिवत्त u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    पटिनिवत्त get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ध्रुव ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
    विकल्प (u.b.format)
    {
#ifndef HB_NO_HINTING
    पक्ष 1: पक्ष 2: पक्ष 3:
      hb_barrier ();
      पटिनिवत्त u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      पटिनिवत्त u.variation.get_x_delta (font, store, store_cache);
#endif
    स्वभाव:
      पटिनिवत्त 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ध्रुव ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ध्रुव
  {
    विकल्प (u.b.format)
    {
    पक्ष 1: पक्ष 2: पक्ष 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      पटिनिवत्त u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      पटिनिवत्त u.variation.get_y_delta (font, store, store_cache);
#endif
    स्वभाव:
      पटिनिवत्त 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!u.b.format.sanitize (c)) return_trace (मिच्छा);
    विकल्प (u.b.format) {
#ifndef HB_NO_HINTING
    पक्ष 1: पक्ष 2: पक्ष 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    स्वभाव:
      return_trace (सच्च);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map=nullptr) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    विकल्प (u.b.format) {
#ifndef HB_NO_HINTING
    पक्ष 1:
    पक्ष 2:
    पक्ष 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    स्वभाव:
      return_trace (nullptr);
    }
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    विकल्प (u.b.format) {
#ifndef HB_NO_HINTING
    पक्ष 1:
    पक्ष 2:
    पक्ष 3:
      पटिनिवत्त;
#endif
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      पटिनिवत्त;
#endif
    स्वभाव:
      पटिनिवत्त;
    }
  }

  unsigned get_variation_index () ध्रुव
  {
    विकल्प (u.b.format) {
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      पटिनिवत्त u.variation.varIdx;
#endif
    स्वभाव:
      पटिनिवत्त HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  तार्किक is_variation_device () ध्रुव
  {
    विकल्प (u.b.format) {
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      पटिनिवत्त सच्च;
#endif
    स्वभाव:
      पटिनिवत्त मिच्छा;
    }
  }

  रक्षित:
  सङ्घ {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */
